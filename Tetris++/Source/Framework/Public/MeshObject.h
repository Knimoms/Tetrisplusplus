#pragma once
#include "GameObject.h"
#include "Mesh.h"

#include <memory>

class MeshObject : public GameObject
{
private:
	std::shared_ptr<Mesh> m_Mesh;
	bool b_RegisteredMesh = false;

protected:
	Transform m_Transform;

public:
	~MeshObject();

	void Register();
	void Unregister();

public:
	inline std::shared_ptr<Mesh> GetMesh() const { return m_Mesh; }
	void SetMesh(std::shared_ptr<Mesh> inMesh);

	inline const Transform& GetTransform() const { return m_Transform; }
	void SetTransform(const Transform& inTransform);


};

