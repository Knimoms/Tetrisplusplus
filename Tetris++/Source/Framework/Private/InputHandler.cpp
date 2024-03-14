#include "InputHandler.h"
#include "GLFW/glfw3.h"

InputHandler::InputHandler(GLFWwindow* inWindow)
	:m_InputWindow(inWindow)
{
}

void InputHandler::RemoveInput(int key, KeyAction executeOn)
{
	std::vector<KeyCommand>::iterator position = std::find_if(m_KeyCommands.begin(), m_KeyCommands.end(), 
		[key, executeOn](const KeyCommand& keycommand) -> bool { return (keycommand.key == key) && (keycommand.executeAction == executeOn) ; });

	m_KeyCommands.erase(position);

}

#include <iostream>
void InputHandler::KeyboardInputTick()
{
	size_t keyCommandsNum = m_KeyCommands.size();

	for (int i = 0; i < keyCommandsNum; ++i)
	{
		int key = m_KeyCommands[i].key;
		bool currentlyPressed = glfwGetKey(m_InputWindow, key);
		std::cout << currentlyPressed << std::endl;

		if (currentlyPressed == m_PressedKeys[key])
			continue;

		m_PressedKeys[key] = currentlyPressed;

		if (currentlyPressed == (bool)m_KeyCommands[i].executeAction)
			m_KeyCommands[i].Command->Execute();
	}
}