#pragma once
#include "MeshObject.h"

class Tetromino;

class DroppedBlocksContainer : public MeshObject
{
private:
	glm::vec4 m_ColorMatrix[20][10];

public:
	virtual void Update(float DeltaTimeSeconds) override;

	void AddTetromino(Tetromino* addingTetromino);
	void GenerateMesh();

	bool IsBlockAtPosition(int x, int y);
};

