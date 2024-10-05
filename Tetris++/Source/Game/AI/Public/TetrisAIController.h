#pragma once
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

public:
    TetrisAIController();

private:
    enum Actions
    {
        InvalidAction   = -1,
        NoMove          = 0,
        MoveLeft        = 1,
        MoveRight       = 2,
        MoveDown        = 3,
        Rotate          = 4
    };

public:
    virtual void Update(float DeltaTimeSeconds) override;
    void SetFitnessByScore(double inScore);
};
