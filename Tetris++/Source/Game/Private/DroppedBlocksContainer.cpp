#include "DroppedBlocksContainer.h"
#include "Tetromino.h"

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

			const std::vector<glm::vec2>& cubePositions = Mesh::GetCubeVertPositions();

			size_t cubePositionsNum = cubePositions.size();
			int indexOffset = (int)vertices.size();

			glm::vec3 color = { m_ColorMatrix[j][i][1], m_ColorMatrix[j][i][2], m_ColorMatrix[j][i][3] };

			for (int z = 0; z < cubePositionsNum; ++z)
				vertices.push_back({ {cubePositions[z][0] + (float)i, cubePositions[z][1] + (float)j}, color});

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
