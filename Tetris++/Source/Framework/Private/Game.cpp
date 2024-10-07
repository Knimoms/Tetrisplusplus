#include "Game.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "GameMode.h"
#include "TetrisAIController.h"

#include "GLFW/glfw3.h"
#include <chrono>
#include <complex>


Game::Game()
    : m_Renderer(std::make_shared<Renderer>())
{
    m_Window = m_Renderer->CreateWindow(m_GameName);
    m_InputHandler = std::make_shared<InputHandler>(m_Window);
}

bool IsArgumentInVector(const std::vector<std::string>& arguments, const std::string& checkingArgument)
{
    return std::find(arguments.begin(), arguments.begin(), "evaluate") != arguments.end();
}

void Game::Run(const std::string& arguments)
{
    if (arguments.find("evaluate") != std::string::npos)
        TetrisAIController::EvaluateLastAIGenerations();

    if (arguments.find("dropworsegens") != std::string::npos)
        TetrisAIController::DropGensWorseThanLast();
    
    if (arguments.find("skipgame") != std::string::npos)
        return;

    m_Renderer->Init();

    bool bTrainAI = arguments.find("aitrain") != std::string::npos;

    m_GameMode = GameObject::SpawnGameObject<GameMode>(bTrainAI || arguments.find("aiplay") != std::string::npos,
                                                       bTrainAI);
    auto lastTimestamp = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(m_Window) && b_RunGame)
    {
        auto currentTimestamp = std::chrono::high_resolution_clock::now();
        float deltaTimeSeconds = (float)std::chrono::duration_cast<std::chrono::duration<double>>(
            currentTimestamp - lastTimestamp).count();

        m_InputHandler->KeyboardInputTick();
        m_UpdateEvent.Emit(deltaTimeSeconds);

        lastTimestamp = currentTimestamp;

        m_Renderer->Clear();
        m_Renderer->RenderFrame();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Game::StopGame()
{
    b_RunGame = false;
}
