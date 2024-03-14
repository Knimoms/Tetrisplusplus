#pragma once

#include <vector>
#include "Command.h"

/*
* The template argument states what parameter type this Event supports.
*/
template<typename T>
class Event {
private:
	std::vector<Command<T>*> m_CommandsOnEmit;

public:
	void AddCommand(Command<T>* addingCommand)
	{
		m_CommandsOnEmit.push_back(addingCommand);
	}

	void RemoveCommand(Command<T>* removingCommand)
	{
		typename std::vector<Command<T>*>::iterator position = std::find(m_CommandsOnEmit.begin(), m_CommandsOnEmit.end(), removingCommand);
		m_CommandsOnEmit.erase(position);
	}

	void Emit(T param1)
	{
		size_t commandNum = m_CommandsOnEmit.size();

		for (int i = 0; i < commandNum; ++i)
			m_CommandsOnEmit[i]->Execute(param1);
	}
};