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
	SetBlockOffsetsWithMat5(shapeMatrix);

}

Tetromino::Tetromino(std::shared_ptr<Mesh> mesh, bool shapeMatrix[5][5], const glm::vec3& color, DroppedBlocksContainer* droppedBlocksC)
	:m_Color(color), m_DroppedBlockContainer(droppedBlocksC)
{
	SetMesh(mesh);
	SetBlockOffsetsWithMat5(shapeMatrix);

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

void Tetromino::Init()
{
	m_Transform = { {5.f, 0.f}, 0.f };

	SetupInput();
	MeshObject::Init();
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

	if (!SetPosition(newPosition))
		m_DroppedEvent.Emit();

}

void Tetromino::Rotate()
{
	float newRotation = m_Transform.rotation;

	newRotation += 90.f;
	if (newRotation > 360.f)
		newRotation += 0.f;

	glm::ivec2 newBlockOffsets[3];

	for (int i = 0; i < 3; ++i)
		newBlockOffsets[i] = m_BlockOffsets[i];

	RotateBlockOffsetsCW(newBlockOffsets);

	if (!ValidateBlockOffsets(newBlockOffsets))
		return;

	m_Transform.rotation = newRotation;
	for (int i = 0; i < 3; ++i)
		m_BlockOffsets[i] = newBlockOffsets[i];
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

	if (!ValidatePosition(inPosition))
		return false;

	m_Transform.position = inPosition;
	return true;
}

bool Tetromino::ValidateCurrentTransform()
{
	int x = (int)m_Transform.position.x;
	int y = (int)m_Transform.position.y;

	if (y > 19 || x < 0 || x > 9 || m_DroppedBlockContainer->IsBlockAtPosition(x, y))
		return false;

	for (int i = 0; i < 3; i++)
	{
		int alteredX = x + (int)m_BlockOffsets[i][0];
		int alteredY = y + (int)m_BlockOffsets[i][1];

		if (alteredY > 19 || alteredX < 0 || alteredX > 9 || m_DroppedBlockContainer->IsBlockAtPosition(alteredX, alteredY))
			return false;
	}

	return true;
}

bool Tetromino::ValidatePosition(const glm::vec2& inPosition)
{
	int x = (int)inPosition.x;
	int y = (int)inPosition.y;

	if (y > 19 || x < 0 || x > 9 || m_DroppedBlockContainer->IsBlockAtPosition(x, y))
		return false;

	for (int i = 0; i < 3; i++)
	{
		int alteredX = x + (int)m_BlockOffsets[i][0];
		int alteredY = y + (int)m_BlockOffsets[i][1];

		if (alteredY > 19 || alteredX < 0 || alteredX > 9 || m_DroppedBlockContainer->IsBlockAtPosition(alteredX,alteredY))
			return false;
	}

	return true;
}

bool Tetromino::ValidateBlockOffsets(glm::ivec2 inBlockOffsets[3])
{
	int x = (int)m_Transform.position.x;
	int y = (int)m_Transform.position.y;

	if (y > 19 || x < 0 || x > 9 || m_DroppedBlockContainer->IsBlockAtPosition(x, y))
		return false;

	for (int i = 0; i < 3; i++)
	{
		int alteredX = x + inBlockOffsets[i][0];
		int alteredY = y + inBlockOffsets[i][1];

		if (alteredY > 19 || alteredX < 0 || alteredX > 9 || m_DroppedBlockContainer->IsBlockAtPosition(alteredX, alteredY))
			return false;
	}

	return true;
}

void Tetromino::RotateBlockOffsetsCCW(glm::ivec2 inBlockOffsets[3])
{
	for (int i = 0; i < 3; ++i)
	{
		int x = inBlockOffsets[i][0];
		int y = inBlockOffsets[i][1];

		inBlockOffsets[i] = { y , 0 - x };
	}
}

void Tetromino::RotateBlockOffsetsCW(glm::ivec2 inBlockOffsets[3])
{
	for (int i = 0; i < 3; ++i)
	{
		int x = inBlockOffsets[i][0];
		int y = inBlockOffsets[i][1];

		inBlockOffsets[i] = { 0 - y, x };
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

			const std::vector<Vertex>& cubeVertices = Mesh::GetCubeVertices();

			size_t cubeVerticesNum = cubeVertices.size();
			int indexOffset = (int)vertices.size();

			for (int z = 0; z < cubeVerticesNum; ++z)
			{
				Vertex newVertex = cubeVertices[z];
				newVertex.position[0] += x;
				newVertex.position[1] += y;
				newVertex.color = color;
				vertices.push_back(newVertex);
			}

			const std::vector<unsigned int>& cubeIndices = Mesh::GetCubeIndices();
			size_t cubeIndicesNum = cubeIndices.size();

			for (int z = 0; z < cubeIndicesNum; ++z)
				indices.push_back(cubeIndices[z] + indexOffset);

		}

	return std::make_shared<Mesh>(vertices, indices);
}