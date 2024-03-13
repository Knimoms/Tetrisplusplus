#include "InputHandler.h"
#include "GLFW/glfw3.h"

InputHandler::InputHandler(GLFWwindow* inWindow)
	:m_InputWindow(inWindow)
{
}

void InputHandler::RemoveInput(int key)
{
	free(m_KeyFunctions[key]);

	m_KeyFunctions.erase(key);
}

void InputHandler::Tick(float DeltaSeconds)
{
	for (auto [key, value] : m_KeyFunctions)
	{
		if (!value)
			continue;
		
		if (!glfwGetKey(m_InputWindow, key))
		{
			m_PressedKeys[key] = false;
			continue;
		}

		if (m_PressedKeys[key])
			continue;

		m_PressedKeys[key] = true;
		value->CallFunction();
	}
}
