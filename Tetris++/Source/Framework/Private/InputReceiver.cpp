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
	size_t occupiedKeysNum = m_OccupiedInputs.size();

	for (int i = 0; i < occupiedKeysNum; ++i)
		m_InputHandler.RemoveInput(std::get<0>(m_OccupiedInputs[i]),std::get<1>(m_OccupiedInputs[i]));
}
