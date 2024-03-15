#pragma once

#include "InputReceiver.h"
#include "GameObject.h"
#include "Mesh.h"

class Tetromino : public GameObject, InputReceiver
{
private:
	Mesh m_Mesh;
	Transform m_Transform;
	bool m_CollisionMatrix[5][5];

public:
	Tetromino(bool collisionMat[5][5], const glm::vec3& color);
	~Tetromino();

	static Mesh GenerateMeshFromMat5(bool collisionMat[5][5], const glm::vec3& color);

	virtual void SetupInput() override;

	virtual void Update(float DeltaTimeSeconds) override;

	void MoveLeft();
	void MoveRight();
	void Fall();
	void Rotate();

	inline const Mesh& GetMesh() const { return m_Mesh; }
	inline const Transform& GetTransform() const { return m_Transform; }
	inline const bool (&GetCollisionMatrix() const)[5][5] { return m_CollisionMatrix; }

};

