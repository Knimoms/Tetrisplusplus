#pragma once
#include "MeshObject.h"
#include "Event.h"

class Tetromino;

class DroppedBlocksContainer : public MeshObject
{
private:
	glm::vec4 m_ColorMatrix[20][10];
	bool b_AddingTetromino = false;

	glm::ivec4 m_CompletedRows;
	Event<void> m_AddingTetrominoFinishedEvent;
public:
	virtual void Update(float DeltaTimeSeconds) override;

	void AddTetromino(Tetromino* addingTetromino);
	void GenerateMesh();
	
	bool IsBlockAtPosition(int x, int y);
	glm::ivec4 GetCompletedRows();
	void RemoveRow(int rowY);
	void DropRows();

	inline Event<void>& GetAddingTetrominoFinishedEvent() { return m_AddingTetrominoFinishedEvent; }
};

