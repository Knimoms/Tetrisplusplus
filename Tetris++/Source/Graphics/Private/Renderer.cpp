#include "Renderer.h"
#include "Mesh.h"

#include "GL/glew.h"

void Renderer::DrawMesh(const Mesh& inMesh, const Transform& inTransform, Shader& inShader)

void Renderer::DrawMesh(const Mesh& inMesh)
{
	inMesh.Bind();
	m_DefaultShader.Bind();
	glDrawElements(GL_TRIANGLES, inMesh.GetElementBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
	Mesh::Unbind();
	Shader::Unbind();
}

void Renderer::DrawMesh(const Mesh& inMesh, const Shader& inShader)
{
	inMesh.Bind();
	inShader.Bind();
	glDrawElements(GL_TRIANGLES, inMesh.GetElementBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);
	Mesh::Unbind();
	Shader::Unbind();
}

void Renderer::SetBackgroundColor(const glm::vec3& inColor)
{
	m_BackgroundColor = inColor;
}

void Renderer::Clear()
{
	glClearColor(m_BackgroundColor.x, m_BackgroundColor.y, m_BackgroundColor.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}
