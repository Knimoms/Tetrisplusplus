#pragma once
#include "GameObject.h"

#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include <random>
#include <functional>

class Tetromino;

struct ShapeColorCombination
{
	bool shape[5][5];
	glm::vec3 color;
};

typedef std::_Binder<std::_Unforced, std::uniform_int_distribution<int>, std::mt19937> CappedRNG;

class GameMode : public GameObject
{
private:
	std::shared_ptr<Tetromino> m_CurrentTetromino;
	std::vector<ShapeColorCombination> m_AllTetrominoShapes;
	CappedRNG m_RNG;
	bool b_ActiveTetromino = false;

public:
	GameMode();

	virtual void Update(float DeltaTimeSeconds) override;

	std::shared_ptr<Tetromino> SpawnRandomTetromino();

};

