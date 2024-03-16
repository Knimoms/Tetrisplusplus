#include "Tetromino.h"
#include "InputHandler.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "DroppedBlocksContainer.h"

Tetromino::Tetromino(bool shapeMatrix[5][5], const glm::vec3& color, DroppedBlocksContainer* droppedBlocksC)
	:m_Color(color)
{
	SetMesh(GenerateMeshFromMat5(shapeMatrix, color));
	m_Transform = { {5.f, 0.f}, 0.f };

	SetBlockOffsetsWithMat5(shapeMatrix);

	SetupInput();
}

Tetromino::Tetromino(std::shared_ptr<Mesh> mesh, bool shapeMatrix[5][5], const glm::vec3& color, DroppedBlocksContainer* droppedBlocksC)
	:m_Color(color), m_DroppedBlockContainer(droppedBlocksC)
{
	SetMesh(mesh);
	m_Transform = { {5.f, 0.f}, 0.f };

	SetBlockOffsetsWithMat5(shapeMatrix);

	SetupInput();
}

void Tetromino::SetupInput()
{
	AddInput(65, KeyAction::PRESSED, this, &Tetromino::MoveLeft_Pressed);
	AddInput(65, KeyAction::RELEASED, this, &Tetromino::MoveLeft_Released);

	AddInput(68, KeyAction::PRESSED, this, &Tetromino::MoveRight_Pressed);
	AddInput(68, KeyAction::RELEASED, this, &Tetromino::MoveRight_Released);

	AddInput(83, KeyAction::PRESSED, this, &Tetromino::MoveDown_Pressed);
	AddInput(83, KeyAction::RELEASED, this, &Tetromino::MoveDown_Released);

	AddInput(87, KeyAction::PRESSED, this, &Tetromino::Rotate_Pressed);

}

void Tetromino::Update(float DeltaTimeSeconds)
{
	if (!(b_MovingLeft || b_MovingRight || b_MovingDown))
	{
		m_HoldingInputForSeconds = 0.f;
		m_LastInputSecondsAgo = 0.f;
		return;
	}

	m_HoldingInputForSeconds += DeltaTimeSeconds;
	m_LastInputSecondsAgo += DeltaTimeSeconds;

	if (m_HoldingInputForSeconds < 0.3f || m_LastInputSecondsAgo < 0.02f)
		return;

	m_LastInputSecondsAgo = 0.f;

	if (b_MovingLeft)
		MoveLeft();

	if (b_MovingRight)
		MoveRight();

	if (b_MovingDown)
		Fall();
}

void Tetromino::MoveLeft()
{
	glm::vec2 newPosition = m_Transform.position;
	newPosition[0] -= 1.f;
	SetPosition(newPosition);
}

void Tetromino::MoveRight()
{
	glm::vec2 newPosition = m_Transform.position;
	newPosition[0] += 1.f;
	SetPosition(newPosition);
}

void Tetromino::Fall()
{
	glm::vec2 newPosition = m_Transform.position;
	newPosition[1] += 1.f;
	if(!SetPosition(newPosition))
		m_DroppedEvent.Emit();
}

void Tetromino::Rotate()
{
	float oldRotation = m_Transform.rotation;
	float newRotation = oldRotation;

	newRotation += 90.f;
	if (newRotation > 360.f)
		newRotation += 0.f;

	m_Transform.rotation = newRotation;

	glm::vec2 oldBlockOffsets[3];

	for (int i = 0; i < 3; ++i)
		oldBlockOffsets[i] = m_BlockOffsets[i];


	RotateBlockOffsetsCW();

	if (ValidateCurrentTransform())
		return;

	m_Transform.rotation = oldRotation;
	for (int i = 0; i < 3; ++i)
		m_BlockOffsets[i] = oldBlockOffsets[i];
}

void Tetromino::SetBlockOffsetsWithMat5(bool matrix[5][5])
{
	int blockOffsetsIndex = 0;

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
		{
			if (!matrix[i][j] || (i == 2) && (j == 2))
				continue;

			m_BlockOffsets[blockOffsetsIndex] = { j - 2, i - 2 };
			++blockOffsetsIndex;
		}
}

bool Tetromino::SetPosition(const glm::vec2& inPosition)
{
	glm::vec2 oldPosition = m_Transform.position;
	m_Transform.position = inPosition;

	if (ValidateCurrentTransform())
		return true;

	m_Transform.position = oldPosition;
	return false;
}

bool Tetromino::ValidateCurrentTransform()
{
	int x = (int)m_Transform.position.x;
	int y = (int)m_Transform.position.y;

	if (y > 19 || x < 0 || x > 9 || m_DroppedBlockContainer->IsBlockAtPosition(x, y))
		return false;

	for (int i = 0; i < 3; i++)
	{
		int alteredX = x + m_BlockOffsets[i][0];
		int alteredY = y + m_BlockOffsets[i][1];

		if (alteredY > 19 || alteredX < 0 || alteredX > 9 || m_DroppedBlockContainer->IsBlockAtPosition(alteredX,alteredY))
			return false;
	}

	return true;
}

void Tetromino::RotateBlockOffsetsCCW()
{
	for (int i = 0; i < 3; ++i)
	{
		float x = m_BlockOffsets[i][0];
		float y = m_BlockOffsets[i][1];

		m_BlockOffsets[i] = { y , 0 - x };
	}
}

void Tetromino::RotateBlockOffsetsCW()
{
	for (int i = 0; i < 3; ++i)
	{
		float x = m_BlockOffsets[i][0];
		float y = m_BlockOffsets[i][1];

		m_BlockOffsets[i] = { 0 - y, x };
	}
}

void Tetromino::MoveLeft_Pressed()
{
	MoveLeft();
	b_MovingLeft = true;
}

void Tetromino::MoveLeft_Released()
{
	b_MovingLeft = false;
}

void Tetromino::MoveRight_Pressed()
{
	MoveRight();
	b_MovingRight = true;
}

void Tetromino::MoveRight_Released()
{
	b_MovingRight = false;
}

void Tetromino::MoveDown_Pressed()
{
	Fall();
	b_MovingDown = true;
}

void Tetromino::MoveDown_Released()
{
	b_MovingDown = false;
}

void Tetromino::Rotate_Pressed()
{
	Rotate();
}

std::shared_ptr<Mesh> Tetromino::GenerateMeshFromMat5(bool shapeMatrix[5][5], const glm::vec3& color)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
		{
			if (!shapeMatrix[i][j])
				continue;

			float x = (float)(j - 2);
			float y = (float)(i - 2);

			const std::vector<glm::vec2>& cubePositions = Mesh::GetCubeVertPositions();

			int indexOffset = (int)vertices.size();

			for (int z = 0; z < 4; ++z)
				vertices.push_back({ {cubePositions[z][0] + x, cubePositions[z][1] + y}, color });

			const std::vector<unsigned int>& cubeIndices = Mesh::GetCubeIndices();

			for (int z = 0; z < 6; ++z)
				indices.push_back(cubeIndices[z] + indexOffset);

		}

	return std::make_shared<Mesh>(vertices, indices);
}