#include "TetrisAIController.h"

#include <array>
#include <iostream>

#include "DroppedBlocksContainer.h"
#include "Game.h"
#include "GameMode.h"
#include "Tetromino.h"
#include "NeuralNetwork/Matrix.h"

std::vector<int> TetrisAIController::s_Topology = {
    10 + //Column heights
    1 + //Tetromino type
    1 + //Tetromino position x
    1 + //Tetromino position y
    1 + //Tetromino rotation
    1,  // Next Tetromino
    9, 8, 7, // Hidden layers
    5 // Output ()
};

TetrisAIController::TetrisAIController()
    : m_PlayingGameMode(Game::GetGameInstance().GetGameMode()), m_NeuralNetwork(s_Topology, "tetrisAI")
{
}

void TetrisAIController::Update(float DeltaTimeSeconds)
{
    if(!m_PlayingGameMode)
        m_PlayingGameMode = Game::GetGameInstance().GetGameMode();
    
    if (m_PlayingGameMode->IsGameOver())
    {
        if(!b_StartedPlaying)
            m_PlayingGameMode->StartGame();
        
        return;
    }

    b_StartedPlaying = true;

    std::vector<double> newInputs;
    auto columnHeights = m_PlayingGameMode->GetDroppedBlocksContainer()->GetColumnHeights();
    newInputs.insert(newInputs.end(), columnHeights.begin(), columnHeights.end());

    auto currentTetromino = m_PlayingGameMode->GetCurrentTetromino();

    if (!currentTetromino)
        return;

    newInputs.push_back(currentTetromino->GetTetrominoType());

    const Transform& transform = currentTetromino->GetTransform();
    newInputs.push_back(transform.position.x);
    newInputs.push_back(transform.position.y);

    newInputs.push_back(transform.rotation);
    newInputs.push_back(m_PlayingGameMode->GetNextTetrominoShapeIndex());

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
        
    case InvalidAction:
        std::cout << "Invalid Action triggered by AI" << '\n';
    }
}

void TetrisAIController::SetFitnessByScore(double inScore)
{
    m_NeuralNetwork.SetFitness(inScore);
    m_NeuralNetwork.AutoSave();
}
