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

/*
* The first template argument states what class the object is, the second argument states what parameter type this command uses.
*/
template<class T, typename U>
class ObjectCommand : public Command<U>
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

template<class T>
class ObjectCommand<T, void> : public Command<void>
{
private:
	typedef void (T::* MemberFunction)(void);

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