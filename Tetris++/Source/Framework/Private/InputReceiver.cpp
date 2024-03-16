#include "InputReceiver.h"
#include "Game.h"

InputReceiver::InputReceiver()
	:m_InputHandler(Game::GetGameInstance().GetInputHandler())
{
}


InputReceiver::~InputReceiver()
{
	m_InputHandler->RemoveInputs(this);
}
