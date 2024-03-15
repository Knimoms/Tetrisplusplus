#include "Tetromino.h"
#include "InputHandler.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"

Tetromino::Tetromino(bool shapeMatrix[5][5], const glm::vec3& color)
	:m_Mesh(GenerateMeshFromMat5(shapeMatrix, color)), m_Transform({{5.f, 0.f}, 0.f}), InputReceiver()
{
	int blockOffsetsIndex = 0;

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
		{
			m_ShapeMatrix[i][j] = shapeMatrix[i][j];

			if(!shapeMatrix[i][j] || (i == 2) && (j == 2))
				continue;

			m_BlockOffsets[blockOffsetsIndex] = {j - 2, i - 2};
			++blockOffsetsIndex;
		}

	SetupInput();
	Game::GetGameInstance().GetRenderer()->AddRenderEntry(&m_Mesh, &m_Transform, &Shader::GetDefaultShader());
}

Tetromino::~Tetromino()
{
	Game::GetGameInstance().GetRenderer()->RemoveRenderEntry(&m_Mesh);
}

Mesh Tetromino::GenerateMeshFromMat5(bool shapeMatrix[5][5], const glm::vec3& color)
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
				vertices.push_back({ {cubePositions[z][0] + x, cubePositions[z][1] + y}, color});

			const std::vector<unsigned int>& cubeIndices = Mesh::GetCubeIndices();

			for (int z = 0; z < 6; ++z)
				indices.push_back(cubeIndices[z] + indexOffset);
			
		}

	return Mesh(vertices, indices);
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
	AddInput(87, KeyAction::RELEASED, this, &Tetromino::Rotate_Released);

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
	SetPosition(newPosition);
}

void Tetromino::Rotate()
{
	float oldRotation = m_Transform.rotation;
	float newRotation = oldRotation;

	newRotation -= 90.f;
	if (newRotation < -360.f)
		newRotation += 360.f;

	m_Transform.rotation = newRotation;

	glm::vec2 oldBlockOffsets[3];

	for (int i = 0; i < 3; ++i)
		oldBlockOffsets[i] = m_BlockOffsets[i];


	RotateBlockOffsetsCCW();

	if (ValidateCurrentTransform())
		return;

	m_Transform.rotation = oldRotation;
	for (int i = 0; i < 3; ++i)
		m_BlockOffsets[i] = oldBlockOffsets[i];
}

void Tetromino::SetPosition(const glm::vec2& inPosition)
{
	glm::vec2 oldPosition = m_Transform.position;
	m_Transform.position = inPosition;

	if (ValidateCurrentTransform())
		return;

	m_Transform.position = oldPosition;
}

bool Tetromino::ValidateCurrentTransform()
{
	if (m_Transform.position.y > 19 || m_Transform.position.x < 0 || m_Transform.position.x > 9)
		return false;

	for (int i = 0; i < 3; i++)
		if (m_Transform.position.y + m_BlockOffsets[i][1] > 19 || m_Transform.position.x + m_BlockOffsets[i][0] < 0 || m_Transform.position.x + m_BlockOffsets[i][0] > 9)
			return false;

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
	b_Rotating = true;
}

void Tetromino::Rotate_Released()
{
	b_Rotating = false;
}

void Tetromino::Update(float DeltaTimeSeconds)
{
	if (!(b_MovingLeft || b_MovingRight || b_MovingDown || b_Rotating))
	{
		m_HoldingInputForSeconds = 0.f;
		m_LastInputSecondsAgo = 0.f;
		return;
	}

	m_HoldingInputForSeconds += DeltaTimeSeconds;
	m_LastInputSecondsAgo += DeltaTimeSeconds;

	if(m_HoldingInputForSeconds < 0.3f || m_LastInputSecondsAgo < 0.02f)
		return;

	m_LastInputSecondsAgo = 0.f;

	if(b_MovingLeft)
		MoveLeft();

	if(b_MovingRight)
		MoveRight();

	if(b_MovingDown)
		Fall();

	if(b_Rotating)
		Rotate();
}