#pragma once

#include "Command.h"

#include <memory>

enum KeyAction
{
	PRESSED,
	RELEASE
};

struct KeyCommand
{
	std::unique_ptr<Command<void>> Command;
	KeyAction executeAction;
};

#include <unordered_map>

struct GLFWwindow;

class InputHandler
{
private:
	std::unordered_map<int, bool> m_PressedKeys;
	std::unordered_map<int, KeyCommand> m_KeyFunctions;
	GLFWwindow* m_InputWindow;

public:
	InputHandler(GLFWwindow* inWindow);

	template<typename T>
	void AddInput(int key, T* object, void (T::* memberFunction)(void), KeyAction executeOn)
	{
		Command<T>* newInput = new Command<>(object, memberFunction);
		m_KeyFunctions[key] = { std::unique_ptr<Command<void>>, executeOn };
		m_PressedKeys[key] = false;
	}

	void RemoveInput(int key);

	void KeyboardInputTick();

};