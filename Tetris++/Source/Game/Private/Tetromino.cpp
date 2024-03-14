#include "Tetromino.h"
#include "InputHandler.h"

Tetromino::Tetromino(bool collisionMat[5][5], const glm::vec3& color, InputHandler* inputHandler)
	:m_Mesh(GenerateMeshFromMat5(collisionMat, color)), m_Transform({{5.5f, 0.5f}, 0.f}), InputReceiver(inputHandler)
{
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			m_CollisionMatrix[i][j] = collisionMat[i][j];

	SetupInput();
}

Mesh Tetromino::GenerateMeshFromMat5(bool collisionMat[5][5], const glm::vec3& color)
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

void Tetromino::SetupInput()
{
	AddInput(65, KeyAction::PRESSED, this, &Tetromino::MoveLeft);
	AddInput(68, KeyAction::PRESSED, this, &Tetromino::MoveRight);
	AddInput(83, KeyAction::PRESSED, this, &Tetromino::Fall);
	AddInput(87, KeyAction::PRESSED, this, &Tetromino::Rotate);

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
	if (newRotation < -360.f)
		newRotation += 360.f;

	m_Transform.rotation = newRotation;
}

void Tetromino::Update(float DeltaTimeSeconds)
{

}
