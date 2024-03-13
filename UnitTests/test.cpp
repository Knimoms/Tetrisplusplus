#include "pch.h"
#include "vector"

#include "Game.h"
#include "GL/glew.h"
#include "Mesh.h"
#include "Renderer.h"

TEST(GraphicsTest, OpenGLSetupTest) {
	Game game("servus", 10, 10);
	EXPECT_TRUE(!!game.CreateWindow());
	EXPECT_TRUE(game.SetupOpenGLSettings());

	std::vector<Vertex> triangleVertices = {
		{{ -0.5f, -0.5f}, {1.f, 0.f, 0.f}}, 
		{{  0.5f, -0.5f}, {0.f, 1.f, 0.f}}, 
		{{  0.0f,  0.5f}, {0.f, 0.f, 1.f}}
	};

	std::vector<unsigned int> triangleIndices = {
		0, 1, 2
	};

	Mesh triangle(triangleVertices, triangleIndices);

	Renderer renderer;
	renderer.DrawMesh(triangle);

	EXPECT_EQ(GL_NO_ERROR, glGetError());
}

#include "Tetromino.h"

TEST(TetrominoTest, MeshGenerationTest){
	bool collMat[5][5]
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	Mesh mesh = Tetromino::GenerateMeshFromMat5(collMat, { 0.f, 0.f, 0.f });
	size_t verticesNum = mesh.GetVertices().size();
	size_t cubePosNum = Mesh::GetCubeVertPositions().size();

	bool sameSize = cubePosNum == verticesNum;
	EXPECT_TRUE(sameSize);

	if (!sameSize)
		return;

	for (int i = 0; i < verticesNum; ++i)
		EXPECT_EQ(Mesh::GetCubeVertPositions()[i], mesh.GetVertices()[i].position);

	EXPECT_EQ(Mesh::GetCubeIndices(), mesh.GetIndices());
}