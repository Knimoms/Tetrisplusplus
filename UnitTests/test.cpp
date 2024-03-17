#include "pch.h"
#include "vector"

#include "Game.h"
#include "GL/glew.h"
#include "Mesh.h"
#include "Renderer.h"
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