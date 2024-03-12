#include "Game.h"
#include "Renderer.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

Game::Game(const std::string& gameName, int windowWidth, int windowHeight)
	:m_GameName(gameName), m_WindowWidth(windowWidth), m_WindowHeight(windowHeight)
{
}

void Game::Run()
{
	GLFWwindow* window = CreateWindow();
	SetupOpenGLSettings();

	Renderer renderer;

	while (!glfwWindowShouldClose(window))
	{
		renderer.Clear();

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