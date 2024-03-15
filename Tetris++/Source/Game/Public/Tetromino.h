#pragma once

#include "InputReceiver.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Event.h"

class Tetromino : public GameObject, InputReceiver
{
private:
	Mesh m_Mesh;
	Transform m_Transform;
	bool m_ShapeMatrix[5][5];
	glm::vec2 m_BlockOffsets[3];

	bool b_MovingLeft = false;
	bool b_MovingRight = false;
	bool b_MovingDown = false;
	bool b_Rotating = false;

	float m_HoldingInputForSeconds = 0.f;
	float m_LastInputSecondsAgo = 0.f;

	Event<void> m_DroppedEvent;

public:
	Tetromino(bool shapeMatrix[5][5], const glm::vec3& color);
	~Tetromino();

	static Mesh GenerateMeshFromMat5(bool shapeMatrix[5][5], const glm::vec3& color);

	virtual void SetupInput() override;

	virtual void Update(float DeltaTimeSeconds) override;

	void MoveLeft();
	void MoveRight();
	void Fall();
	void Rotate();

	bool SetPosition(const glm::vec2& inPosition);

	bool ValidateCurrentTransform();

	void RotateBlockOffsetsCCW();
	void RotateBlockOffsetsCW();

	void MoveLeft_Pressed();
	void MoveLeft_Released();

	void MoveRight_Pressed();
	void MoveRight_Released();

	void MoveDown_Pressed();
	void MoveDown_Released();

	void Rotate_Pressed();
	void Rotate_Released();

	inline const Mesh& GetMesh() const { return m_Mesh; }
	inline const Transform& GetTransform() const { return m_Transform; }
	inline const bool (&GetShapeMatrix() const)[5][5] { return m_ShapeMatrix; }
	inline Event<void>& GetDroppedEvent() { return m_DroppedEvent; }

};