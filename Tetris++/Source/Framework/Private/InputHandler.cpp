#include "InputHandler.h"
#include "GLFW/glfw3.h"

InputHandler::InputHandler(GLFWwindow* inWindow)
	:m_InputWindow(inWindow)
{
}

std::vector<KeyCommand>::iterator InputHandler::GetInputPosition(void* owner, int key, KeyAction executeOn)
{
	return std::find_if(m_KeyCommands.begin(), m_KeyCommands.end(),
		[owner, key, executeOn](const KeyCommand& keycommand) -> bool { 
		return (keycommand.owner == owner) && (keycommand.key == key) && (keycommand.executeAction == executeOn); 
		});
}

std::vector<KeyCommand>::iterator InputHandler::GetFirstInputPosition(void* owner)
{
	return std::find_if(m_KeyCommands.begin(), m_KeyCommands.end(),
		[owner](const KeyCommand& keycommand) -> bool {
			return (keycommand.owner == owner);
		});
}

void InputHandler::RemoveInputs(void* owner)
{
	std::vector<KeyCommand>::iterator position;
	while((position = GetFirstInputPosition(owner)) != m_KeyCommands.end())
		m_KeyCommands.erase(position);
}

void InputHandler::KeyboardInputTick()
{
	size_t keyCommandsNum = m_KeyCommands.size();

	for (int i = 0; i < keyCommandsNum; ++i)
	{
		int key = m_KeyCommands[i].key;
		bool currentlyPressed = glfwGetKey(m_InputWindow, key);

		if (currentlyPressed && m_KeyCommands[i].executeAction && !m_KeyCommands[i].wasPressedOnce)
		{
			m_KeyCommands[i].wasPressedOnce = true;
			m_KeyCommands[i].command->Execute();
			continue;
		}

		if (currentlyPressed == m_PressedKeys[key])
			continue;

		if (currentlyPressed != (bool)m_KeyCommands[i].executeAction)
			continue;
		
		m_KeyCommands[i].command->Execute();
	}

	for (auto [key, value] : m_PressedKeys)
		m_PressedKeys[key] = glfwGetKey(m_InputWindow, key);
}

void InputHandler::Clear()
{
	m_PressedKeys.clear();
	m_KeyCommands.clear();
}
