#include "GameMode.h"
#include "Tetromino.h"
#include "TetrominoPreview.h"
#include "DroppedBlocksContainer.h"
#include "Event.h"
#include "Command.h"
#include "Game.h"

#include <chrono>

std::vector<ShapeColorCombination> GameMode::m_AllTetrominoShapes =
{
	ShapeColorCombination(
	{
		{0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0,
		 0, 1, 1, 1, 1,
		 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0},
		{0.f, 240.f / 255.f, 240.f / 255.f}
	}),
	ShapeColorCombination(
	{
		{0, 0, 0, 0, 0,
		 0, 1, 0, 0, 0,
		 0, 1, 1, 1, 0,
		 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0},
		{0.f, 0.f, 240.f / 255.f}
	}),
	ShapeColorCombination(
	{
		{0, 0, 0, 0, 0,
		 0, 0, 0, 1, 0,
		 0, 1, 1, 1, 0,
		 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0},
		{240.f / 255.f, 160.f / 255.f, 0.f}
	}),
	ShapeColorCombination(
	{
		{0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0,
		 0, 0, 1, 1, 0,
		 0, 0, 1, 1, 0,
		 0, 0, 0, 0, 0},
		{240.f / 255.f, 240.f / 255.f, 0.f}
	}),
	ShapeColorCombination(
	{
		{0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0,
		 0, 0, 1, 1, 0,
		 0, 1, 1, 0, 0,
		 0, 0, 0, 0, 0},
		{0.f, 240.f / 255.f, 0.f}
	}),
	ShapeColorCombination(
	{
		{0, 0, 0, 0, 0,
		 0, 0, 1, 0, 0,
		 0, 1, 1, 1, 0,
		 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0},
		{160.f / 255.f, 0.f, 240.f / 255.f}
	}),
	ShapeColorCombination(
	{
		{0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0,
		 0, 1, 1, 0, 0,
		 0, 0, 1, 1, 0,
		 0, 0, 0, 0, 0},
		{240.f / 255.f, 0.f, 0.f}
	})
};

GameMode::GameMode()
	:m_RNG(std::bind(std::uniform_int_distribution<int>(0, 6), std::mt19937((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count())))
{
	
	float brightness = 0.4f;
	std::vector<Vertex> vertices = {
		{ { 9.5f, 19.5f}, {0.1f, 0.1f, 0.1f}, brightness },
		{ {12.5f, 19.5f}, {0.1f, 0.1f, 0.1f}, brightness },
	    { {12.5f, -0.5f}, {0.1f, 0.1f, 0.1f}, brightness },
		{ { 9.5f, -0.5f}, {0.1f, 0.1f, 0.1f}, brightness }
	};

	std::vector<unsigned int> indices = {
		0, 1, 2,
		2, 3, 0
	};

	m_SidebarBackground = std::make_shared<Mesh>(vertices, indices);

	Game::GetGameInstance().GetRenderer()->AddRenderEntry(this, m_SidebarBackground.get(), nullptr, nullptr, 0);

	for (int i = 0; i < 7; ++i)
		m_AllTetrominoMeshes.push_back(Tetromino::GenerateMeshFromMat5(m_AllTetrominoShapes[i].shape, m_AllTetrominoShapes[i].color));

	SetupInput();
}

void GameMode::SetupInput()
{
	AddInput(32, PRESSED, this, &GameMode::StartGame);
}

void GameMode::Init()
{
	m_TetrominoDroppedCommand = std::shared_ptr<Command<void>>(new ObjectCommand<GameMode, void>(this, &GameMode::CurrentTetrominoDropped));

	GameObject::Init();
}

void GameMode::Update(float DeltaTimeSeconds)
{
	if(b_GameOver)
		return;
	
	LastFallSecondsAgo += DeltaTimeSeconds;

	if (LastFallSecondsAgo > m_DropDelaySeconds)
	{
		m_CurrentTetromino->Fall();	
		LastFallSecondsAgo = 0.f;
	}
}

void GameMode::StartGame()
{
	b_GameOver = false;

	Game::GetGameInstance().GetInputHandler()->Clear();

	m_DroppedBlocksContainer = SpawnGameObject<DroppedBlocksContainer>();
	m_DroppedBlocksContainer->GetAddingTetrominoFinishedEvent().AddCommand(std::make_shared<ObjectCommand<GameMode, int>>(this, &GameMode::DroppedContainerFinishedAdding));
	
	m_DropDelaySeconds = START_DELAY;
	m_Score = 0.f;
	m_Level = 1;
	m_RowsCompletedThisLevel = 0;

	AddTetrominoPreview();
	AddTetrominoPreview();

	SpawnTetromino();
}

void GameMode::LevelUp()
{
	++m_Level;
	m_DropDelaySeconds *= LEVELUP_SPEED_MULTIPLIER;
	m_RowsCompletedThisLevel = 0;
}

void GameMode::AddTetrominoPreview()
{
	if (m_TetrominoPreviews.size() > 1)
		m_TetrominoPreviews.erase(m_TetrominoPreviews.begin());

	if (m_TetrominoPreviews.size() > 0)
		m_TetrominoPreviews[0]->LiftPreview();

	unsigned int shapeIndex = m_RNG();
	m_TetrominoPreviews.push_back(std::make_shared<TetrominoPreview>(m_AllTetrominoMeshes[shapeIndex].get(), shapeIndex));
}

#include "iostream"

void GameMode::DroppedContainerFinishedAdding(int completedRows)
{
	m_Score += BASESCORE_ADD * completedRows * (float)pow(MULTIROW_SCORE_MULTIPLIER, completedRows);

	m_RowsCompletedThisLevel += completedRows;
	if(m_RowsCompletedThisLevel >= COMPLETED_ROWS_TO_LEVELUP)
		LevelUp();

	system("CLS");
	std::cout << "Score: " <<  m_Score << " Level: " << m_Level << " Completed Rows: " << m_RowsCompletedThisLevel << std::endl;
	SpawnTetromino();
}

void GameMode::SpawnTetromino()
{
	unsigned int i = m_TetrominoPreviews[0]->GetShapeIndex();
	auto newTetromino = SpawnGameObject<Tetromino>(m_AllTetrominoMeshes[i], m_AllTetrominoShapes[i].shape, m_AllTetrominoShapes[i].color, m_DroppedBlocksContainer.get());
	newTetromino->GetDroppedEvent().AddCommand(m_TetrominoDroppedCommand);

	m_CurrentTetromino = newTetromino;

	AddTetrominoPreview();

	if (!newTetromino->ValidateCurrentTransform())
	{
		b_GameOver = true;
		Game::GetGameInstance().GetInputHandler()->Clear();
		SetupInput();
	}
}

void GameMode::CurrentTetrominoDropped()
{
	if (b_GameOver )
		return;

	m_DroppedBlocksContainer->AddTetromino(m_CurrentTetromino.get());
}
