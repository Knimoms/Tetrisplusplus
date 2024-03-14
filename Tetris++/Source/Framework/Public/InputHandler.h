#pragma once

#include "Command.h"

#include <memory>

enum KeyAction
{
	RELEASED,
	PRESSED
};

struct KeyCommand
{
	int key;
	KeyAction executeAction;
	std::shared_ptr<Command<void>> Command;
};

#include <unordered_map>
#include <string>

struct GLFWwindow;

class InputHandler
{
private:
	std::unordered_map<int, bool> m_PressedKeys;
	std::vector<KeyCommand> m_KeyCommands;
	GLFWwindow* m_InputWindow;

public:
	InputHandler(GLFWwindow* inWindow);

	template<typename T>
	void AddInput(int key, KeyAction executeOn, T* inObject, void (T::* inMethod)(void))
	{
		m_KeyCommands.push_back({ key, executeOn, std::shared_ptr<ObjectCommand<T, void>>(new ObjectCommand<T, void>(inObject, inMethod))});
		m_PressedKeys[key] = false;
	}

	void RemoveInput(int key, KeyAction executeOn);

	void KeyboardInputTick();

};