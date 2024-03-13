#pragma once

class InputFunction
{
public:
	virtual void CallFunction() = 0;
};

template<typename T>
struct InputMemberFunction : InputFunction
{
	typedef void (T::* MemberFunction)(void);

	T* object;
	MemberFunction method;

	InputMemberFunction(T* inObject, MemberFunction inMethod)
		:object(inObject), method(inMethod)
	{
	}

	virtual void CallFunction() override
	{
		(object->*method)();
	}
};