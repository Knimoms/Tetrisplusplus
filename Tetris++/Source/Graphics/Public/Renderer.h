#pragma once
#include "Shader.h"

#include "vector"
#include "memory"

class Mesh;

class Renderer
{
private:
	Shader m_DefaultShader;
	glm::vec3 m_BackgroundColor;

public:
	Renderer();

	void DrawMesh(const Mesh& inMesh);
	void DrawMesh(const Mesh& inMesh, const Shader& inShader);
	void SetBackgroundColor(const glm::vec3& inColor);
	void Clear();

};

