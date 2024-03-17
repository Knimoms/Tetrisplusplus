#include "Game.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "GameMode.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <chrono>

Game::Game()
{
}

void Game::Run()
{
	Renderer renderer;
	m_Renderer = &renderer;

	GLFWwindow* window = renderer.CreateWindow(m_GameName);
	renderer.Init();

	InputHandler inputHandler(window);
	m_InputHandler = &inputHandler;

	std::shared_ptr<GameMode> gameMode = GameObject::SpawnGameObject<GameMode>();

	auto lastTimestamp = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window))
	{
		auto currentTimestamp = std::chrono::high_resolution_clock::now();
		float deltaTimeSeconds = (float)std::chrono::duration_cast<std::chrono::duration<double>>(currentTimestamp - lastTimestamp).count();

		m_InputHandler->KeyboardInputTick();
		m_UpdateEvent.Emit(deltaTimeSeconds);

		lastTimestamp = currentTimestamp;

		m_Renderer->Clear();
		m_Renderer->RenderFrame();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}