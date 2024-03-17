#include "GameObject.h"
#include "Game.h"

GameObject::~GameObject()
{
	Game::GetGameInstance().GetUpdateEvent().RemoveCommand(m_UpdateCommand);
}

void GameObject::Init()
{
	m_UpdateCommand = std::shared_ptr<ObjectCommand<GameObject, float>>(new ObjectCommand<GameObject, float>(this, &GameObject::Update));
	Game::GetGameInstance().GetUpdateEvent().AddCommand(m_UpdateCommand);
}
