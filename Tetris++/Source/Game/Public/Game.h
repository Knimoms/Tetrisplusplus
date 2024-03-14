#pragma once

#include "Event.h"

#include <string>

struct GLFWwindow;

class Game
{
private:
	std::string m_GameName;
	int m_WindowWidth;
	int m_WindowHeight;

	Event<float> m_UpdateEvent;

public:
	Game(const std::string& gameName, int windowWidth, int windowHeight);

	void Run();
	GLFWwindow* CreateWindow();
	bool SetupOpenGLSettings(); //returns if Setup was successful

	inline Event<float>& GetUpdateEvent() { return m_UpdateEvent; }

};

