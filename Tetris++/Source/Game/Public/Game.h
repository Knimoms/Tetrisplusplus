#pragma once

#include "Renderer.h"

class Game
{
private:
	std::string m_GameName;
	int m_WindowWidth;
	int m_WindowHeight;

public:
	Game(const std::string& gameName, int windowWidth, int windowHeight);
};

