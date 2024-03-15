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
	GLFWwindow* window = CreateWindow();
	SetupOpenGLSettings();

	Renderer renderer;
	m_Renderer = &renderer;

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

bool Game::SetupOpenGLSettings()
{
	if (glewInit() != GLEW_OK)
		return 0;

	return 1;
}

GLFWwindow* Game::CreateWindow()
{
	GLFWwindow* window;

	if (!glfwInit())
		return nullptr;

	window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, m_GameName.c_str(), nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);	

	return window;
}