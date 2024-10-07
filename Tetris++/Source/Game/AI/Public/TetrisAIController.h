#pragma once
#include <array>

#include "GameObject.h"
#include <memory>

#include "NeuralNetwork/NeuralNetwork.h"

class GameMode;
class Game;

class TetrisAIController : public GameObject
{
private:
    std::shared_ptr<GameMode> m_PlayingGameMode;
    NeuralNetwork m_NeuralNetwork;
    
    static std::vector<int> s_Topology;

    bool b_StartedPlaying = false;

    bool b_Training;

    double m_FitnessScore = 0.f;

public:
    TetrisAIController(bool bTrain);

private:
    enum Actions
    {
        InvalidAction   = -1,
        MoveLeft     = 0,
        MoveRight    = 1,
        MoveDown     = 2,
        Rotate       = 3
    };

    std::array<bool, 4> m_UsedActions;
    
public:
    virtual void Update(float DeltaTimeSeconds) override;
    void EvaluateFitnessWithScore(double inScore);

public:
    static void EvaluateLastAIGenerations();
    static void DropGensWorseThanLast();
};
