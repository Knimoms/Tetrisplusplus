#include "DroppedBlocksContainer.h"
#include "Shader.h"
#include "Tetromino.h"
#include "Game.h"

DroppedBlocksContainer::DroppedBlocksContainer()
{
	std::vector<Vertex> vertices =
	{
		{{-5.f, -0.5f}, {0.f, 0.f, 0.f}, 1.0f},
		{{ 5.f, -0.5f}, {0.f, 0.f, 0.f}, 1.0f},
		{{ 5.f,  0.5f}, {0.f, 0.f, 0.f}, 1.0f},
		{{-5.f,  0.5f}, {0.f, 0.f, 0.f}, 1.0f}
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	m_AnimMesh = std::make_shared<Mesh>(vertices, indices);
	m_AnimShader = std::make_shared<Shader>("Resources/Shader/RemoveAnimShader.vert", "Resources/Shader/Default.frag");

	m_AnimShader->Bind();
	m_AnimShader->SetUniform1f("u_AnimLengthSeconds", m_RemoveAnimationLengthSeconds);

}

void DroppedBlocksContainer::Update(float DeltaTimeSeconds)
{
	if(!b_AddingTetromino && !b_RemoveAnimRunning)
		return;

	if (b_AddingTetromino)
	{
		b_AddingTetromino = false;
		m_CompletedRows = GetCompletedRows();

		if (m_CompletedRows[0] < 0)
		{
			m_AddingTetrominoFinishedEvent.Emit(0);
			return;
		}

		for (int i = 0; i < 4; ++i)
			if(!(m_CompletedRows[i] < 0))
				StartRemoveAnimation(m_CompletedRows[i]);
		
		return;
	}

	m_RemoveAnimationRunningForSeconds += DeltaTimeSeconds;
	m_AnimShader->Bind();
	m_AnimShader->SetUniform1f("u_AnimLengthProgressSeconds", m_RemoveAnimationRunningForSeconds);

	if (m_RemoveAnimationRunningForSeconds < m_RemoveAnimationLengthSeconds)
		return;
	
	EndRemoveAnimation();

	int removedRows = 0;

	for (int i = 0; i < 4; ++i)
		if (!(m_CompletedRows[i] < 0))
		{
			++removedRows;
			RemoveRow(m_CompletedRows[i]);
		}

	DropRows();
	b_RemoveAnimRunning = false;
	m_RemoveAnimationRunningForSeconds = 0.f;
	m_AddingTetrominoFinishedEvent.Emit(removedRows);

}

void DroppedBlocksContainer::AddTetromino(Tetromino* addingTetromino)
{
	const glm::vec2& position = addingTetromino->GetTransform().position;
	m_ColorMatrix[(unsigned int)position[1]][(unsigned int)position[0]] = { 1.f, addingTetromino->GetColor() };
	
	glm::vec2(&blockOffsets)[3] = addingTetromino->GetBlockOffsets();
	
	for (int i = 0; i < 3; ++i)
	{
		if(position[1] + blockOffsets[i][1] < 0)
			continue;

		m_ColorMatrix[(unsigned int)(position[1] + blockOffsets[i][1])][(unsigned int)(position[0] + blockOffsets[i][0])] = { 1.f, addingTetromino->GetColor() };
	}

	GenerateMesh();

	b_AddingTetromino = true;
}

void DroppedBlocksContainer::GenerateMesh()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 20; ++j)
		{
			if(!m_ColorMatrix[j][i][0])
				continue;

			const std::vector<Vertex>& cubeVertices = Mesh::GetCubeVertices();

			size_t cubeVerticesNum = cubeVertices.size();
			int indexOffset = (int)vertices.size();

			glm::vec3 color = { m_ColorMatrix[j][i][1], m_ColorMatrix[j][i][2], m_ColorMatrix[j][i][3] };

			for (int z = 0; z < cubeVerticesNum; ++z)
			{
				Vertex newVertex = cubeVertices[z];
				newVertex.position[0] += (float)i;
				newVertex.position[1] += (float)j;
				newVertex.color = color;
				vertices.push_back(newVertex);
			}

			const std::vector<unsigned int>& cubeIndices = Mesh::GetCubeIndices();
			size_t cubeIndicesNum = cubeIndices.size();

			for (int z = 0; z < cubeIndicesNum; ++z)
				indices.push_back(cubeIndices[z] + indexOffset);

		}		

	SetMesh(std::make_shared<Mesh>(vertices, indices));
}

bool DroppedBlocksContainer::IsBlockAtPosition(int x, int y)
{
	return (x < 10 && x > -1 && y < 20 && y > -1)? m_ColorMatrix[y][x][0] : false;
}

glm::ivec4 DroppedBlocksContainer::GetCompletedRows()
{
	glm::ivec4 completedRows = { -1, -1, -1, -1};

	int completedRowIndex = 0;

	for (int i = 0; i < 20; ++i)
	{
		bool completed = true;

		for(int j = 0; j < 10; ++j)
			if (!m_ColorMatrix[i][j][0])
			{
				completed = false;
				break;
			}

		if (!completed)
			continue;
		
		completedRows[completedRowIndex] = i;
		++completedRowIndex;
	}
			
	return completedRows;
}

void DroppedBlocksContainer::RemoveRow(int rowY)
{
	for(int i = 0; i < 10; ++i)
		m_ColorMatrix[rowY][i] = {0.f, 0.f, 0.f, 0.f};

}

void DroppedBlocksContainer::StartRemoveAnimation(int rowY)
{
	b_RemoveAnimRunning = true;
	int i = 0;
	for(; (i < 3) && (m_AnimTransforms[i].position[0] != 0.f); ++i);
	m_AnimTransforms[i].position = { 4.5f, (float)rowY };
	Game::GetGameInstance().GetRenderer()->AddRenderEntry(m_AnimMesh.get(), m_AnimMesh.get(), &m_AnimTransforms[i], m_AnimShader.get(), 2);
}

void DroppedBlocksContainer::EndRemoveAnimation()
{
	Game::GetGameInstance().GetRenderer()->RemoveRenderEntries(m_AnimMesh.get());
	for(int i = 0; i < 4; ++i)
		m_AnimTransforms[i] = Transform::ZeroTransform;
}


void DroppedBlocksContainer::DropRows()
{
	int droppingRows = 0;
	for (int i = 19; i > -1; --i)
	{
		bool emptyRow = true;
		for(int j = 0; j < 10; j++)
			if (m_ColorMatrix[i][j][0])
			{
				emptyRow = false;
				break;
			}

		if (emptyRow)
		{
			++droppingRows;
			continue;
		}

		if(!droppingRows)
			continue;

		for (int j = 0; j < 10; j++)
		{
			m_ColorMatrix[i+droppingRows][j] = m_ColorMatrix[i][j];
			m_ColorMatrix[i][j][0] = 0.f;
		}
			
	}

	GenerateMesh();
}
