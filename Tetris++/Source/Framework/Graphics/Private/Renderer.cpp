#include "Renderer.h"

#include "GL/glew.h"

void Renderer::DrawMesh(const Mesh& inMesh, const Transform& inTransform, Shader& inShader)
{
	inMesh.Bind();
	inShader.Bind();

	glm::mat4 model = glm::translate(glm::mat4(1), { inTransform.position, 0.f });
	model = glm::rotate(model, glm::radians(inTransform.rotation), { 0, 0, 1 });
	glm::mat4 mvp = m_ProjectionMatrix * model;
	inShader.SetUniformMat4f("u_MVP", mvp);
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
