#pragma once
#include "MeshObject.h"
#include "Event.h"

class Shader;
class Tetromino;

class DroppedBlocksContainer : public MeshObject
{
private:
	glm::vec4 m_ColorMatrix[20][10];
	bool b_AddingTetromino = false;

	std::shared_ptr<Mesh> m_AnimMesh;
	std::shared_ptr<Shader> m_AnimShader;
	Transform m_AnimTransforms[4];
	glm::ivec4 m_CompletedRows;
	bool b_RemoveAnimRunning = false;

	const float m_RemoveAnimationLengthSeconds = 1.f;
	float m_RemoveAnimationRunningForSeconds = 0.f;

	Event<int> m_AddingTetrominoFinishedEvent; //Parameter returns number of completed Rows

public:
	DroppedBlocksContainer();

	virtual void Update(float DeltaTimeSeconds) override;

	void AddTetromino(Tetromino* addingTetromino);
	void GenerateMesh();
	
	bool IsBlockAtPosition(int x, int y);

	glm::ivec4 GetCompletedRows();
	void RemoveRow(int rowY);

	void StartRemoveAnimation(int rowY);
	void EndRemoveAnimation();

	void DropRows();

	inline Event<int>& GetAddingTetrominoFinishedEvent() { return m_AddingTetrominoFinishedEvent; }
};

