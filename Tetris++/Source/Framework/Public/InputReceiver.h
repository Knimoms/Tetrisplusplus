#pragma once

#include "InputHandler.h"

class InputReceiver
{
private:
	std::vector<std::tuple<int, KeyAction>> m_OccupiedInputs;
	InputHandler& m_InputHandler;

public:
	InputReceiver(InputHandler& inputHandler);
	~InputReceiver();
	virtual void SetupInput() = 0;
	virtual void RemoveInput();

	inline InputHandler& GetInputHandler() const { return m_InputHandler; }

	template<typename T>
	void AddInput(int key, KeyAction executeOn, T* inObject, void (T::* inMethod)(void))
	{
		m_InputHandler.AddInput(key, executeOn, inObject, inMethod);
		m_OccupiedInputs.push_back({ key, executeOn });
	}
};