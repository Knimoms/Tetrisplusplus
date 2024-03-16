#pragma once
#include "GameObject.h"
#include "Command.h"
#include "Event.h"

#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include <random>
#include <functional>

class Tetromino;
class DroppedBlocksContainer;
class Mesh;

struct ShapeColorCombination
{
	bool shape[5][5];
	glm::vec3 color;
};

typedef std::_Binder<std::_Unforced, std::uniform_int_distribution<int>, std::mt19937> CappedRNG;

class GameMode : public GameObject
{
private:
	std::shared_ptr<DroppedBlocksContainer> m_DroppedBlocksContainer;
	std::shared_ptr<Tetromino> m_CurrentTetromino;
	std::vector<ShapeColorCombination> m_AllTetrominoShapes;
	std::vector<std::shared_ptr<Mesh>> m_AllTetrominoMeshes;

	CappedRNG m_RNG;

	std::shared_ptr<Command<void>> m_TetrominoDroppedCommand;

	float LastFallSecondsAgo = 0.f;
	bool b_GameOver = false;

public:
	GameMode();
	virtual void Init() override;
	virtual void Update(float DeltaTimeSeconds) override;
	std::shared_ptr<Tetromino> SpawnRandomTetromino();

	void CurrentTetrominoDropped();

};