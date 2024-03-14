#pragma once

/*
* The template argument states what parameter type this command uses.
*/
template<typename T>
class Command
{
public:
	virtual void Execute(T) = 0;
};

template<typename T, typename U>
class ObjectCommand : Command<U>
{
private:
	typedef void (T::* MemberFunction)(U);

	T* m_Object;
	MemberFunction m_Method;

public:

	ObjectCommand(T* inObject, MemberFunction inMethod)
		:m_Object(inObject), m_Method(inMethod)
	{
	}

	virtual void Execute(U param1) override
	{
		(m_Object->*m_Method)(param1);
	}
};

template<typename T>
class ObjectCommand<T, void> : Command<void>
{
private:
	typedef void (T::* MemberFunction)(U);

	T* m_Object;
	MemberFunction m_Method;

public:

	ObjectCommand(T* inObject, MemberFunction inMethod)
		:m_Object(inObject), m_Method(inMethod)
	{
	}

	virtual void Execute() override
	{
		(m_Object->*m_Method)();
	}
};