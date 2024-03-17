#pragma once

#include "Command.h"

#include <vector>
#include <memory>
/*
* The template argument states what parameter type this Event supports.
*/
template<typename T>
class Event {
private:
	std::vector<std::shared_ptr<Command<T>>> m_CommandsOnEmit;

public:
	void AddCommand(std::shared_ptr<Command<T>> addingCommand)
	{
		m_CommandsOnEmit.push_back(addingCommand);
	}

	void RemoveCommand(std::shared_ptr<Command<T>> removingCommand)
	{
		typename std::vector<std::shared_ptr<Command<T>>>::iterator position = std::find(m_CommandsOnEmit.begin(), m_CommandsOnEmit.end(), removingCommand);
		m_CommandsOnEmit.erase(position);
	}

	void Emit(T param1)
	{
		size_t commandNum = m_CommandsOnEmit.size();

		for (int i = 0; i < commandNum; ++i)
			m_CommandsOnEmit[i]->Execute(param1);
	}
};

template<>
class Event<void> {
private:
	std::vector<std::shared_ptr<Command<void>>> m_CommandsOnEmit;

public:
	void AddCommand(std::shared_ptr<Command<void>> addingCommand)
	{
		m_CommandsOnEmit.push_back(addingCommand);
	}

	void RemoveCommand(std::shared_ptr<Command<void>> removingCommand)
	{
		typename std::vector<std::shared_ptr<Command<void>>>::iterator position = std::find(m_CommandsOnEmit.begin(), m_CommandsOnEmit.end(), removingCommand);
		m_CommandsOnEmit.erase(position);
	}

	void Emit()
	{
		size_t commandNum = m_CommandsOnEmit.size();

		for (int i = 0; i < commandNum; ++i)
			m_CommandsOnEmit[i]->Execute();
	}
};