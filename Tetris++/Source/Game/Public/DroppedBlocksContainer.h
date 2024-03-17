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
	std::vector<Transform> m_AnimTransforms;
	glm::ivec4 m_CompletedRows;
	bool b_RemoveAnimRunning = false;

	const float m_RemoveAnimationLengthSeconds = 1.f;
	float m_RemoveAnimationRunningForSeconds = 0.f;

	Event<void> m_AddingTetrominoFinishedEvent;

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

	inline Event<void>& GetAddingTetrominoFinishedEvent() { return m_AddingTetrominoFinishedEvent; }
};

