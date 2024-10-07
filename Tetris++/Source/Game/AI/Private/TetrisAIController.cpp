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
    4 // Output ()
};

TetrisAIController::TetrisAIController(bool bTrain)
    : m_PlayingGameMode(Game::GetGameInstance().GetGameMode()), m_NeuralNetwork(s_Topology, "tetrisAI"), b_Training(bTrain)
{
    if(bTrain)
        m_NeuralNetwork = NeuralNetwork(m_NeuralNetwork, 0.03f);
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
    
    m_UsedActions[nextAction] = true;
}

void TetrisAIController::EvaluateFitnessWithScore(double inScore)
{
    m_FitnessScore = 0.f;
    m_FitnessScore += inScore;

    for(int i = 0; i < 4; ++i)
        if(m_UsedActions[i])
            m_FitnessScore += 500.f;

    double averageRowThickness = ((double)m_PlayingGameMode->GetDroppedBlocksContainer()->GetNumDroppedBlocks()) / 20;
    
    m_FitnessScore += averageRowThickness * 100;
    
    m_NeuralNetwork.SetFitness(m_FitnessScore);

    if(b_Training)
    {
        m_NeuralNetwork.AutoSave();
        Game::GetGameInstance().StopGame();
    }
}

void TetrisAIController::EvaluateLastAIGenerations()
{
    NeuralNetwork::FilterGenerationsForBest("tetrisAI");
}

void TetrisAIController::DropGensWorseThanLast()
{
    NeuralNetwork::ResetToBestGeneration("tetrisAI");

}
