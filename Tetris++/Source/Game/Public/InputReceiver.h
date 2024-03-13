#pragma once

#include "InputHandler.h"

class InputReceiver
{
private:
	std::vector<int> m_OccupiedKeys;
	InputHandler& m_InputHandler;

public:
	InputReceiver(InputHandler& inputHandler);
	~InputReceiver();
	virtual void SetupInput() = 0;
	virtual void RemoveInput();

	inline InputHandler& GetInputHandler() const { return m_InputHandler; }
	void AddOccupiedKey(int key);

	template<typename T>
	void AddInput(int key, T* object, void (T::* memberFunction)(void))
	{
		m_InputHandler.AddInput(key, object, memberFunction);
		m_OccupiedKeys.push_back(key);
	}
};