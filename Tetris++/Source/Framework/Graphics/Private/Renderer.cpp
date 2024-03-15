#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"

#include "GL/glew.h"

void Renderer::RenderFrame()
{
	size_t renderEntriesNum = m_RenderEntries.size();

	for (int i = 0; i < renderEntriesNum; ++i)
		DrawRenderEntry(m_RenderEntries[i]);
}

void Renderer::DrawRenderEntry(RenderEntry& renderEntry)
{
	renderEntry.mesh->Bind();
	renderEntry.shader->Bind();

	glm::mat4 model = glm::translate(glm::mat4(1), { renderEntry.transform->position, 0.f });
	model = glm::rotate(model, glm::radians(renderEntry.transform->rotation), { 0, 0, 1 });
	glm::mat4 mvp = m_ProjectionMatrix * model;
	
	renderEntry.shader->SetUniformMat4f("u_MVP", mvp);
	glDrawElements(GL_TRIANGLES, renderEntry.mesh->GetElementBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);

	Mesh::Unbind();
	Shader::Unbind();
}

void Renderer::AddRenderEntry(Mesh* inMesh, Transform* inTransform, Shader* inShader)
{
	m_RenderEntries.push_back({ inMesh, inTransform? inTransform : &Transform::ZeroTransform, inShader? inShader : &Shader::GetDefaultShader() });
}

void Renderer::RemoveRenderEntry(Mesh* inMesh)
{
	auto position = std::find_if(m_RenderEntries.begin(), m_RenderEntries.end(),
		[inMesh](const RenderEntry& renderEntry) -> bool { return renderEntry.mesh == inMesh; });

	m_RenderEntries.erase(position);
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
