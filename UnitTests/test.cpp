#include "pch.h"
#include "vector"

#include "GL/glew.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"

TEST(GraphicsTest, OpenGLSetupTest) {
	Renderer renderer;
	EXPECT_TRUE(!!renderer.CreateWindow("test"));
	EXPECT_TRUE(renderer.Init());

	std::vector<Vertex> triangleVertices = {
		{{ -0.5f, -0.5f}, {1.f, 0.f, 0.f}}, 
		{{  0.5f, -0.5f}, {0.f, 1.f, 0.f}}, 
		{{  0.0f,  0.5f}, {0.f, 0.f, 1.f}}
	};

	std::vector<unsigned int> triangleIndices = {
		0, 1, 2
	};

	Mesh triangle(triangleVertices, triangleIndices);

	Transform transform = { {0, 0}, 0 };

	renderer.AddRenderEntry(this, &triangle, &transform, &Shader::GetDefaultShader(), 0);

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
	auto mesh = Tetromino::GenerateMeshFromMat5(collMat, { 0.f, 0.f, 0.f });
	size_t verticesNum = mesh->GetVertices().size();
	size_t cubeVertNum = Mesh::GetCubeVertices().size();

	bool sameSize = cubeVertNum == verticesNum;
	EXPECT_TRUE(sameSize);

	if (!sameSize)
		return;

	for (int i = 0; i < verticesNum; ++i)
		EXPECT_EQ(Mesh::GetCubeVertices()[i].position, mesh->GetVertices()[i].position);

	EXPECT_EQ(Mesh::GetCubeIndices(), mesh->GetIndices());
}

#include "DroppedBlocksContainer.h"

TEST(DroppedBlockContainerTest, AddingTetromioTest) {
	DroppedBlocksContainer dpc;

	bool shapeMat[5][5]
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 1,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};

	Tetromino* t1 = new Tetromino(shapeMat, {1.0f, 1.0f, 1.0f}, &dpc);
	t1->SetTransform({{1.f, 19.f}});
	
	Tetromino* t2 = new Tetromino(shapeMat, { 1.0f, 1.0f, 1.0f }, &dpc);
	t2->SetTransform({ {5.f, 19.f} });

	bool shapeMat2[5][5]
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 1, 1, 0,
		0, 0, 1, 1, 0,
		0, 0, 0, 0, 0
	};
	
	Tetromino* t3 = new Tetromino(shapeMat2, { 1.0f, 1.0f, 1.0f }, &dpc);
	t3->SetTransform({ {8.f, 18.f}});
	
	dpc.AddTetromino(t1);
	for (int i = 0; i < 4; ++i)
		EXPECT_TRUE(dpc.IsBlockAtPosition(i, 19));
	dpc.AddTetromino(t2);
	for (int i = 0; i < 4; ++i)
		EXPECT_TRUE(dpc.IsBlockAtPosition(i + 4, 19));
	dpc.AddTetromino(t3);
	for (int i = 0; i < 2; ++i)
		EXPECT_TRUE(dpc.IsBlockAtPosition(i + 8, 19));

	dpc.RemoveRow(19);

	for (int i = 0; i < 10; ++i)
		EXPECT_TRUE(!dpc.IsBlockAtPosition(i, 19));

	delete t1, t2, t3;

}