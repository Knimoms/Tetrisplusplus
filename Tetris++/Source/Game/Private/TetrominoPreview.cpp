#include "TetrominoPreview.h"
#include "Game.h"

TetrominoPreview::TetrominoPreview(Mesh* inMesh, unsigned int inShapeIndex)
	:m_Mesh(inMesh), m_ShapeIndex(inShapeIndex)
{
	m_Transform.position = {11.f, 3.f};
	m_Transform.scale = { 0.5f, 0.5f };

	Game::GetGameInstance().GetRenderer()->AddRenderEntry(this, m_Mesh, &m_Transform, nullptr, 1);
}

TetrominoPreview::~TetrominoPreview()
{
	Game::GetGameInstance().GetRenderer()->RemoveRenderEntries(this);
}

void TetrominoPreview::LiftPreview()
{
	m_Transform.position.y -= 2.f;

}
