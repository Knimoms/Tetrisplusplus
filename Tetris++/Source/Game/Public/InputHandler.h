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

#include <unordered_map>
struct GLFWwindow;

class InputHandler
{
private:
	std::unordered_map<int, bool> m_PressedKeys;
	std::unordered_map<int, InputFunction*> m_KeyFunctions;
	GLFWwindow* m_InputWindow;

public:
	InputHandler(GLFWwindow* inWindow);

	template<typename T>
	void AddInput(int key, T* object, void (T::* MemberFunction)(void))
	{
		InputMemberFunction<T>* newInput = new InputMemberFunction(object, MemberFunction);
		m_KeyFunctions[key] = newInput;
		m_PressedKeys[key] = false;
	}

	void RemoveInput(int key);

	void Tick(float DeltaSeconds);

};