#include "Tetromino.h"

Tetromino::Tetromino(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices, bool(&collisionMat)[5][5])
	:m_Mesh(vertices, indices), m_Transform({{5.5f, 0.5f}, 0.f})
{
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			m_CollisionMatrix[i][j] = collisionMat[i][j];
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
