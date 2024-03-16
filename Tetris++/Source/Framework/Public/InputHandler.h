#pragma once

#include "Command.h"

#include <memory>
#include <unordered_map>

enum KeyAction
{
	RELEASED,
	PRESSED
};

struct KeyCommand
{
	void* owner;
	int key;
	KeyAction executeAction;
	std::shared_ptr<Command<void>> command;
	bool wasPressedOnce = false;
};

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
	void AddInput(void* owner, int key, KeyAction executeOn, T* inObject, void (T::*inMethod)(void))
	{
		if (m_KeyCommands.end() != GetInputPosition(owner, key, executeOn))
		{
			ChangeInput(owner, key, executeOn, inObject, inMethod);
			return;
		}

		m_KeyCommands.push_back({ owner, key, executeOn, std::make_shared<ObjectCommand<T, void>>(inObject, inMethod)});
			
		if(!m_PressedKeys.contains(key))
			m_PressedKeys[key] = false;
	}

	template<typename T>
	void ChangeInput(void* owner, int key, KeyAction executeOn, T* inObject, void(T::* inMethod)(void))
	{
		GetInputPosition(owner, key, executeOn)->command = std::make_shared<ObjectCommand<T, void>>(inObject, inMethod);
	}

	std::vector<KeyCommand>::iterator GetInputPosition(void* owner, int key, KeyAction executeOn);
	std::vector<KeyCommand>::iterator GetFirstInputPosition(void* owner);

	void RemoveInputs(void* owner);

	void KeyboardInputTick();

	void Clear();

};