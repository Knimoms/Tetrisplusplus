#include "GameMode.h"
#include "Tetromino.h"

#include <chrono>

GameMode::GameMode()
	:m_RNG(std::bind(std::uniform_int_distribution<int>(0, 6), std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count())))
{
	

}

void GameMode::Update(float DeltaTimeSeconds)
{
	
}

std::shared_ptr<Tetromino> GameMode::SpawnRandomTetromino()
{

}
