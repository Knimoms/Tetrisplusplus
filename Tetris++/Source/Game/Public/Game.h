#pragma once

#include <string>
struct GLFWwindow;

class Game
{
private:
	std::string m_GameName;
	int m_WindowWidth;
	int m_WindowHeight;

public:
	Game(const std::string& gameName, int windowWidth, int windowHeight);

	void Run();
	GLFWwindow* CreateWindow();
	bool SetupOpenGLSettings(); //returns if Setup was successful
};

