#pragma once

#include "Renderer.h"
#include "Event.h"

#include <string>
#include <memory>

class InputHandler;
class Renderer;
struct GLFWwindow;

class Game
{
public:
	static Game& GetGameInstance()
	{
		static Game game;
		return game;
	}

private:
	std::string m_GameName = "Tetris++";
	int m_WindowWidth = 0;
	int m_WindowHeight = 0;

	InputHandler* m_InputHandler;
	Renderer* m_Renderer;

	Event<float> m_UpdateEvent;

private:
	Game();

public:
	Game(Game const&) = delete;
	void operator=(Game const&) = delete;

	void Run();

	inline InputHandler* GetInputHandler() { return m_InputHandler; }
	inline Renderer* GetRenderer() { return m_Renderer; }

	inline Event<float>& GetUpdateEvent() { return m_UpdateEvent; }

};

