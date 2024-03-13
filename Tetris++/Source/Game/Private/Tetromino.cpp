#include "Tetromino.h"

Tetromino::Tetromino(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices, bool(&collisionMat)[5][5])
	:m_Mesh(vertices, indices), m_Transform({{5.5f, 0.5f}, 0.f})
{
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			m_CollisionMatrix[i][j] = collisionMat[i][j];
Mesh Tetromino::GenerateMeshFromMat5(bool(&collisionMat)[5][5], const glm::vec3& color)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
		{
			if (!collisionMat[i][j])
				continue;

			float x = (float)(j - 2);
			float y = (float)(i - 2);

			const std::vector<glm::vec2>& cubePositions = Mesh::GetCubeVertPositions();
			
			int indexOffset = (int)vertices.size();
			
			for (int z = 0; z < 4; ++z)
				vertices.push_back({ {cubePositions[z][0] + x, cubePositions[z][1] + y}, color});

			const std::vector<unsigned int>& cubeIndices = Mesh::GetCubeIndices();

			for (int z = 0; z < 6; ++z)
				indices.push_back(cubeIndices[z] + indexOffset);
			
		}

	return Mesh(vertices, indices);
}

}

void Tetromino::MoveLeft()
{
	m_Transform.position[0] -= 1.f;
}

void Tetromino::MoveRight()
{
	m_Transform.position[0] += 1.f;
}

void Tetromino::Fall()
{
	m_Transform.position[1] += 1.f;
}

void Tetromino::Rotate()
{
	float newRotation = m_Transform.rotation;
	newRotation -= 90.f;
	if (newRotation > 360.f)
		newRotation -= 360.f;

	m_Transform.rotation -= newRotation;
}
