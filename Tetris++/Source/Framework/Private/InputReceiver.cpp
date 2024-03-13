#include "InputReceiver.h"

InputReceiver::InputReceiver(InputHandler& inputHandler)
	:m_InputHandler(inputHandler)
{
}

InputReceiver::~InputReceiver()
{
	RemoveInput();
}

void InputReceiver::RemoveInput()
{
	size_t occupiedKeysNum = m_OccupiedKeys.size();

	for (int i = 0; i < occupiedKeysNum; ++i)
		m_InputHandler.RemoveInput(m_OccupiedKeys[i]);
}

void InputReceiver::AddOccupiedKey(int key)
{
	m_OccupiedKeys.push_back(key);
}
