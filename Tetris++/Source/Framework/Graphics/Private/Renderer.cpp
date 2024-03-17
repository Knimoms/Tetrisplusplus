#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <algorithm>

int Renderer::Init()
{
	if (glewInit() != GLEW_OK)
		return 0;

	glfwSwapInterval(1);

	return 1;
}

GLFWwindow* Renderer::CreateWindow(const std::string& title, int width, int height)
{
	GLFWwindow* window;

	if (!glfwInit())
		return nullptr;

	glm::ivec2 monitorResolution = GetScreenResolution();

	if (!width)
	{
		height = (int)((float)monitorResolution[1] * 0.8f);
		width = (int)((float)height * 0.65f);
	}

	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	return window;
}

glm::ivec2 Renderer::GetScreenResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	return { mode->width, mode->height };
}

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
	model = glm::scale(model, { renderEntry.transform->scale, 0.f});

	glm::mat4 mvp = m_ProjectionMatrix * model;
	
	renderEntry.shader->SetUniformMat4f("u_MVP", mvp);
	glDrawElements(GL_TRIANGLES, renderEntry.mesh->GetElementBuffer().GetCount(), GL_UNSIGNED_INT, nullptr);

	Mesh::Unbind();
	Shader::Unbind();

}

unsigned long long Renderer::AddRenderEntry(void* inOwner, Mesh* inMesh, Transform* inTransform, Shader* inShader, int renderPriority)
{
	unsigned long long id = (unsigned long long)inOwner + (unsigned long long)inMesh + (unsigned long long)inTransform + (unsigned long long)inShader;

	m_RenderEntries.push_back({ id, inOwner, inMesh, inTransform? inTransform : &Transform::ZeroTransform, inShader? inShader : &Shader::GetDefaultShader(), renderPriority });
	std::sort(m_RenderEntries.begin(), m_RenderEntries.end(), [](RenderEntry const& left, RenderEntry const& right) -> bool
		{
			return left.renderPriority < right.renderPriority;
		});

	return id;
}

void Renderer::RemoveRenderEntries(void* inOwner)
{
	std::vector<RenderEntry>::iterator position; 

	while((position = GetFirstRenderEntry(inOwner)) != m_RenderEntries.end())
		m_RenderEntries.erase(position);
}

void Renderer::RemoveSpecificRenderEntry(unsigned int inID)
{
	auto position = std::find_if(m_RenderEntries.begin(), m_RenderEntries.end(),
		[inID](const RenderEntry& renderEntry) -> bool {
			return renderEntry.id == inID;
		});

	if (position == m_RenderEntries.end())
		return;

	m_RenderEntries.erase(position);
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
	glClear(GL_COLOR_BUFFER_BIT);
}
