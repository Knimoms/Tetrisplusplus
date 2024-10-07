#pragma once
#include "GameObject.h"
#include "InputReceiver.h"
#include "Command.h"

#include "glm/glm.hpp"
#include <memory>
#include <vector>
#include <random>
#include <functional>

#include "TetrominoPreview.h"

class TetrisAIController;
class Tetromino;
class TetrominoPreview;
class DroppedBlocksContainer;
class Mesh;

struct ShapeColorCombination
{
    bool shape[5][5];
    glm::vec3 color;
};

constexpr float START_DELAY = 0.6f;
constexpr int COMPLETED_ROWS_TO_LEVELUP = 10;
constexpr float LEVELUP_SPEED_MULTIPLIER = 0.9f;
constexpr float BASESCORE_ADD = 1000.0f;
constexpr float MULTIROW_SCORE_MULTIPLIER = 1.25f;

typedef std::_Binder<std::_Unforced, std::uniform_int_distribution<int>, std::mt19937> CappedRNG;

class GameMode : public GameObject, InputReceiver
{
private:
    std::shared_ptr<DroppedBlocksContainer> m_DroppedBlocksContainer;
    std::shared_ptr<Tetromino> m_CurrentTetromino;
    static std::vector<ShapeColorCombination> m_AllTetrominoShapes;
    std::vector<std::shared_ptr<Mesh>> m_AllTetrominoMeshes;

    std::vector<std::shared_ptr<TetrominoPreview>> m_TetrominoPreviews;
    std::shared_ptr<Mesh> m_SidebarBackground;

    CappedRNG m_RNG;

    std::shared_ptr<Command<void>> m_TetrominoDroppedCommand;

    float m_LastFallSecondsAgo = 0.f;

    bool b_GameOver = true;

    float m_Score = 0.f;
    unsigned int m_Level = 1;
    unsigned int m_RowsCompletedThisLevel = 0;

    float m_DropDelaySeconds = START_DELAY;

    unsigned int m_StartLevel = 1;

public:
    inline std::shared_ptr<DroppedBlocksContainer> GetDroppedBlocksContainer() { return m_DroppedBlocksContainer; }
    inline std::shared_ptr<Tetromino> GetCurrentTetromino() { return m_CurrentTetromino; }

    inline unsigned int GetNextTetrominoShapeIndex() const { return m_TetrominoPreviews[0]->GetShapeIndex(); }

    inline bool IsGameOver() const { return b_GameOver; }

public:
    GameMode(bool bSpawnAI, bool bTrainAI = false);

    virtual void SetupInput() override;
    virtual void Init() override;
    virtual void Update(float DeltaTimeSeconds) override;

private:
    std::shared_ptr<TetrisAIController> m_PlayingAI;

public:
    void StartGame();
    void LevelUp();

    void AddTetrominoPreview();

    void DroppedContainerFinishedAdding(int completedRows);
    void SpawnTetromino();

    void CurrentTetrominoDropped() const;
};
