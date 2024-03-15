#include "GameMode.h"
#include "Tetromino.h"

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

void GameMode::Update(float DeltaTimeSeconds)
{
	
}

std::shared_ptr<Tetromino> GameMode::SpawnRandomTetromino()
{
	int i = m_RNG();
	return SpawnGameObject<Tetromino>(m_AllTetrominoShapes[i].shape, m_AllTetrominoShapes[i].color);
}
