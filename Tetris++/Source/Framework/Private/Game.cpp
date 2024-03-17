#include "Game.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "GameMode.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <chrono>

Game::Game()
	:m_Renderer(std::make_shared<Renderer>())
{
	m_Window = m_Renderer->CreateWindow(m_GameName);
	m_InputHandler = std::make_shared<InputHandler>(m_Window);
}

void Game::Run()
{
	m_Renderer->Init();

	std::shared_ptr<GameMode> gameMode = GameObject::SpawnGameObject<GameMode>();

	auto lastTimestamp = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(m_Window))
	{
		auto currentTimestamp = std::chrono::high_resolution_clock::now();
		float deltaTimeSeconds = (float)std::chrono::duration_cast<std::chrono::duration<double>>(currentTimestamp - lastTimestamp).count();

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