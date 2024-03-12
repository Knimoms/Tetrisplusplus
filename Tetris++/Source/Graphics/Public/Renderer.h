#pragma once
#include "Shader.h"
#include "Mesh.h"

#include "vector"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Renderer
{
private:
	glm::vec3 m_BackgroundColor = { 0.f, 0.f, 0.f };
	glm::mat4 m_ProjectionMatrix = glm::ortho(-5.f, 5.f, -10.f, 10.f, -1.f, 1.f);

public:
	void DrawMesh(const Mesh& inMesh, const Transform& inTransform = { {0.f, 0.f} , 0.f }, Shader& inShader = Shader::GetDefaultShader());
	void SetBackgroundColor(const glm::vec3& inColor);
	void Clear();

};

