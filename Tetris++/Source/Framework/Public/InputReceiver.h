#pragma once

#include "InputHandler.h"

class InputReceiver
{
private:
	InputHandler* m_InputHandler;

public:
	InputReceiver();
	~InputReceiver();
	virtual void SetupInput() = 0;

	template<typename T>
	void AddInput(int key, KeyAction executeOn, T* inObject, void (T::* inMethod)(void))
	{
		m_InputHandler->AddInput(this, key, executeOn, inObject, inMethod);
	}
};