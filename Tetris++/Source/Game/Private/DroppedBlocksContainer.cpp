#include "DroppedBlocksContainer.h"
#include "Tetromino.h"
#include "Game.h"

void DroppedBlocksContainer::Update(float DeltaTimeSeconds)
{
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
