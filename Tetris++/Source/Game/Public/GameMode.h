#pragma once
#include "GameObject.h"
#include "InputReceiver.h"
#include "Command.h"
#include "Event.h"

#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include <random>
#include <functional>

class Tetromino;
class TetrominoPreview;
class DroppedBlocksContainer;
class Mesh;

struct ShapeColorCombination
{
	bool shape[5][5];
	glm::vec3 color;
};

#define START_DELAY 0.6f
#define COMPLETED_ROWS_TO_LEVELUP 10
#define LEVELUP_SPEED_MULTIPLIER 0.9f
#define BASESCORE_ADD 1000.0f
#define MULTIROW_SCORE_MULTIPLIER 1.25f

typedef std::_Binder<std::_Unforced, std::uniform_int_distribution<int>, std::mt19937> CappedRNG;

class GameMode : public GameObject, InputReceiver
{
private:
	std::shared_ptr<DroppedBlocksContainer> m_DroppedBlocksContainer;
	std::shared_ptr<Tetromino> m_CurrentTetromino;
	std::vector<ShapeColorCombination> m_AllTetrominoShapes;
	std::vector<std::shared_ptr<Mesh>> m_AllTetrominoMeshes;

	
	std::vector<std::shared_ptr<TetrominoPreview>> m_TetrominoPreviews;
	std::shared_ptr<Mesh> m_SidebarBackground;

	CappedRNG m_RNG;

	std::shared_ptr<Command<void>> m_TetrominoDroppedCommand;

	float LastFallSecondsAgo = 0.f;

	bool b_GameOver = true;


	float m_Score = 0.f;
	unsigned int m_Level = 1;
	unsigned int m_RowsCompletedThisLevel = 0;

	float m_DropDelaySeconds = START_DELAY;

public:
	GameMode();

	virtual void SetupInput() override;
	virtual void Init() override;
	virtual void Update(float DeltaTimeSeconds) override;

	void StartGame();
	void LevelUp();

	void AddTetrominoPreview();

	void DroppedContainerFinishedAdding(int completedRows);
	void SpawnTetromino();

	void CurrentTetrominoDropped();

};