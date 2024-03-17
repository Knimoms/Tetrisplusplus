#include "GameMode.h"
#include "Tetromino.h"
#include "DroppedBlocksContainer.h"
#include "Event.h"
#include "Command.h"
#include "Game.h"

#include <chrono>

GameMode::GameMode()
	:m_RNG(std::bind(std::uniform_int_distribution<int>(0, 6), std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count())))
{
	m_AllTetrominoShapes =
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

	if (LastFallSecondsAgo > 0.6f)
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
	m_DroppedBlocksContainer->GetAddingTetrominoFinishedEvent().AddCommand(std::make_shared<ObjectCommand<GameMode, void>>(this, &GameMode::SpawnRandomTetromino));
	SpawnRandomTetromino();
}

void GameMode::SpawnRandomTetromino()
{
	int i = m_RNG();
	auto newTetromino = SpawnGameObject<Tetromino>(m_AllTetrominoMeshes[i], m_AllTetrominoShapes[i].shape, m_AllTetrominoShapes[i].color, m_DroppedBlocksContainer.get());
	newTetromino->GetDroppedEvent().AddCommand(m_TetrominoDroppedCommand);

	m_CurrentTetromino = newTetromino;

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
