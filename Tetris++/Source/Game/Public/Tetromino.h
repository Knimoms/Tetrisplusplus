#pragma once

#include "InputReceiver.h"
#include "MeshObject.h"
#include "Mesh.h"
#include "Event.h"

class DroppedBlocksContainer;

class Tetromino : public MeshObject, InputReceiver
{
private:
	glm::vec2 m_BlockOffsets[3];
	glm::vec3 m_Color;

	bool b_MovingLeft = false;
	bool b_MovingRight = false;
	bool b_MovingDown = false;

	DroppedBlocksContainer* m_DroppedBlockContainer;

	float m_HoldingInputForSeconds = 0.f;
	float m_LastInputSecondsAgo = 0.f;

	Event<void> m_DroppedEvent;
	
public:
	Tetromino(bool shapeMatrix[5][5], const glm::vec3& color, DroppedBlocksContainer* droppedBlocksC);
	Tetromino(std::shared_ptr<Mesh> mesh, bool shapeMatrix[5][5], const glm::vec3& color, DroppedBlocksContainer* droppedBlocksC);

	virtual void SetupInput() override;

	virtual void Update(float DeltaTimeSeconds) override;

	void MoveLeft();
	void MoveRight();
	void Fall();
	void Rotate();

public:
	void RotateBlockOffsetsCCW();
	void RotateBlockOffsetsCW();

	void MoveLeft_Pressed();
	void MoveLeft_Released();

	void MoveRight_Pressed();
	void MoveRight_Released();

	void MoveDown_Pressed();
	void MoveDown_Released();

	void Rotate_Pressed();

public:

	void SetBlockOffsetsWithMat5(bool matrix[5][5]);
	inline glm::vec2(&GetBlockOffsets())[3] { return m_BlockOffsets; }

	inline const glm::vec3& GetColor() { return m_Color; }

	bool SetPosition(const glm::vec2& inPosition);
	bool ValidateCurrentTransform();

	inline Event<void>& GetDroppedEvent() { return m_DroppedEvent; }

	static std::shared_ptr<Mesh> GenerateMeshFromMat5(bool shapeMatrix[5][5], const glm::vec3& color);
};