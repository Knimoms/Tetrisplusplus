#pragma once
#include "Mesh.h"

class TetrominoPreview
{
private:
	Mesh* m_Mesh;
	Transform m_Transform;
	unsigned int m_ShapeIndex;

public:
	TetrominoPreview(Mesh* inMesh, unsigned int inShapeIndex);
	~TetrominoPreview();

	void LiftPreview();

	inline unsigned int GetShapeIndex() const { return m_ShapeIndex; }

};

