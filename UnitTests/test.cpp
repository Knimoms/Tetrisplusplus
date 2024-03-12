#include "pch.h"
#include "vector"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

struct GLFWwindow;
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