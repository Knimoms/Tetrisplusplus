#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"

#include "GL/glew.h"
#include <algorithm>

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

void Renderer::AddRenderEntry(void* inOwner, Mesh* inMesh, Transform* inTransform, Shader* inShader, int renderPriority)
{
	m_RenderEntries.push_back({ inOwner, inMesh, inTransform? inTransform : &Transform::ZeroTransform, inShader? inShader : &Shader::GetDefaultShader(), renderPriority });
	std::sort(m_RenderEntries.begin(), m_RenderEntries.end(), [](RenderEntry const& left, RenderEntry const& right) -> bool
		{
			return left.renderPriority < right.renderPriority;
		});
}

#include "iostream"

void Renderer::RemoveRenderEntries(void* inOwner)
{
	std::vector<RenderEntry>::iterator position; 

	while((position = GetFirstRenderEntry(inOwner)) != m_RenderEntries.end())
		m_RenderEntries.erase(position);

	std::cout << m_RenderEntries.size() << std::endl;
}

std::vector<RenderEntry>::iterator Renderer::GetFirstRenderEntry(void* inOwner)
{
	return std::find_if(m_RenderEntries.begin(), m_RenderEntries.end(),
		[inOwner](const RenderEntry& renderEntry) -> bool {
			return (renderEntry.owner == inOwner);
		});
}

void Renderer::SetBackgroundColor(const glm::vec3& inColor)
{
	m_BackgroundColor = inColor;
}

void Renderer::Clear()
{
	glClearColor(m_BackgroundColor.x, m_BackgroundColor.y, m_BackgroundColor.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
