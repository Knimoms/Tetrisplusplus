#include "MeshObject.h"
#include "Game.h"
#include "Shader.h"

MeshObject::~MeshObject()
{
	Unregister();
}

void MeshObject::Register()
{
	Game::GetGameInstance().GetRenderer()->AddRenderEntry(this, m_Mesh.get(), &m_Transform, nullptr, 0);
	b_RegisteredMesh = true;
}

void MeshObject::Unregister()
{
	if (b_RegisteredMesh)
		Game::GetGameInstance().GetRenderer()->RemoveRenderEntries(this);
}

void MeshObject::SetMesh(std::shared_ptr<Mesh> inMesh)
{
	Unregister();
	m_Mesh = inMesh;
	Register();
}

void MeshObject::SetTransform(const Transform& inTransform)
{
	m_Transform = inTransform;
}