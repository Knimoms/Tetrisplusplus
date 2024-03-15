#include "GameMode.h"
#include "Tetromino.h"
#include "Event.h"

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
			{0.f, 240.f / 255.f, 0.f}
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
			 0, 0, 0, 0, 0,
			 0, 0, 1, 1, 0,
			 0, 1, 1, 0, 0,
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

}

void GameMode::Init()
{
	m_TetrominoDroppedCommand = std::shared_ptr<Command<void>>(new ObjectCommand<GameMode, void>(this, &GameMode::CurrentTetrominoDropped));
	GameObject::Init();
}

void GameMode::Update(float DeltaTimeSeconds)
{
	if (!b_ActiveTetromino)
	{
		m_CurrentTetromino = SpawnRandomTetromino();
		b_ActiveTetromino = true;
		
	}
}

std::shared_ptr<Tetromino> GameMode::SpawnRandomTetromino()
{
	int i = m_RNG();
	auto spawnedTetromino = SpawnGameObject<Tetromino>(m_AllTetrominoShapes[i].shape, m_AllTetrominoShapes[i].color);
	spawnedTetromino->GetDroppedEvent().AddCommand(m_TetrominoDroppedCommand);

	return spawnedTetromino;
}

void GameMode::CurrentTetrominoDropped()
{
	m_CurrentTetromino = SpawnRandomTetromino();
}
