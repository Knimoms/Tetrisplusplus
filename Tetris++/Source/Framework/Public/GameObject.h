#pragma once

#include "Command.h"
#include "memory"

class Game;

class GameObject
{
private:
	std::shared_ptr<ObjectCommand<GameObject, float>> m_UpdateCommand;

public:
	~GameObject();

	virtual void Init();
	virtual void Update(float DeltaTimeSeconds) = 0;

	template <class T, typename... Args>
	static std::shared_ptr<T> SpawnGameObject(Args... args)
	{
		auto newGameObject = std::shared_ptr<T>(new T(args...));
		newGameObject->Init();

		return newGameObject;
	};
};

