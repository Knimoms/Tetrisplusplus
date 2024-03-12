#include "Game.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

Game::Game(const std::string& gameName, int windowWidth, int windowHeight)
	:m_GameName(gameName), m_WindowWidth(windowWidth), m_WindowHeight(windowHeight)
{
}

void Game::Run()
{

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