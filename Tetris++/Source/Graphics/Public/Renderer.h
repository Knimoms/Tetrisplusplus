#pragma once
#include "Shader.h"

#include "vector"
#include "memory"


class Renderer
{
private:
	glm::vec3 m_BackgroundColor = { 0.f, 0.f, 0.f };
	glm::vec3 m_BackgroundColor;

public:
	void DrawMesh(const Mesh& inMesh, const Transform& inTransform = { {0.f, 0.f} , 0.f }, Shader& inShader = Shader::GetDefaultShader());

	void DrawMesh(const Mesh& inMesh);
	void DrawMesh(const Mesh& inMesh, const Shader& inShader);
	void SetBackgroundColor(const glm::vec3& inColor);
	void Clear();

};

