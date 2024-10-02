#include "TetrisAIController.h"

#include <iostream>

#include "DroppedBlocksContainer.h"
#include "Game.h"
#include "GameMode.h"
#include "Tetromino.h"

std::vector<int> TetrisAIController::s_Topology = {
    10 + //Column heights
    1 + //Tetromino type
    1 + //Tetromino position x
    1 + //Tetromino position y
    1, //Tetromino rotation
    10, 10, // Hidden layers
    5 // Output ()
};

TetrisAIController::TetrisAIController()
    : m_PlayingGameMode(Game::GetGameInstance().GetGameMode()), m_NeuralNetwork(s_Topology, "tetrisAI")
{
}

void TetrisAIController::Update(float DeltaTimeSeconds)
{
    if (m_PlayingGameMode->IsGameOver())
    {
        m_PlayingGameMode->StartGame();
        return;
    }

    std::vector<double> newInputs;
    std::vector<double> columnHeights = m_PlayingGameMode->GetDroppedBlocksContainer()->GetColumnHeights();
    newInputs.insert(newInputs.end(), columnHeights.begin(), columnHeights.end());

    auto currentTetromino = m_PlayingGameMode->GetCurrentTetromino();

    if (!currentTetromino)
        return;

    newInputs.push_back(currentTetromino->GetTetrominoType());

    const Transform& transform = currentTetromino->GetTransform();
    newInputs.push_back(transform.position.x);
    newInputs.push_back(transform.position.y);

    newInputs.push_back(transform.rotation);

    m_NeuralNetwork.SetCurrentInput(newInputs);

    m_NeuralNetwork.FeedForward();
    Actions nextAction = (Actions)m_NeuralNetwork.GetHighestOutputValueIndex();

    switch (nextAction)
    {
    case NoMove:
        break;
    case MoveLeft:
        currentTetromino->MoveLeft();
        break;
    case MoveRight:
        currentTetromino->MoveRight();
        break;
    case MoveDown:
        currentTetromino->Fall();
        break;
    case Rotate:
        currentTetromino->Rotate_Pressed();
        break;
    default:
        std::cout << "Invalid Action triggered by AI" << std::endl;
    }
}
