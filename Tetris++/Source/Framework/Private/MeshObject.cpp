#include "MeshObject.h"
#include "Game.h"
#include "Shader.h"

MeshObject::~MeshObject()
{
	Unregister();
}

void MeshObject::Register()
{
	Game::GetGameInstance().GetRenderer()->AddRenderEntry(m_Mesh.get(), &m_Transform, nullptr);
	b_RegisteredMesh = true;
}

void MeshObject::Unregister()
{
	if (b_RegisteredMesh)
		Game::GetGameInstance().GetRenderer()->RemoveRenderEntry(m_Mesh.get());
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