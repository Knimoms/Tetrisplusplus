#pragma once

#include "Mesh.h"

class Tetromino
{
private:
	Mesh m_Mesh;
	Transform m_Transform;
	bool m_CollisionMatrix[5][5];

public:
	Tetromino(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices, bool (&collisionMat)[5][5]);

	void MoveLeft();
	void MoveRight();
	void Fall();
	void Rotate();

	inline const Mesh& GetMesh() const { return m_Mesh; }
	inline const Transform& GetTransform() const { return m_Transform; }
	inline const bool (&GetCollisionMatrix() const)[5][5] { return m_CollisionMatrix; }
};

