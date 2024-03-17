#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <string>

class Mesh;
struct Transform;
class Shader;
struct GLFWwindow;

struct RenderEntry
{
	unsigned long long id;
	void* owner;
	Mesh* mesh;
	Transform* transform;
	Shader* shader;
	int renderPriority = 0;
};

class Renderer
{
private:
	glm::vec3 m_BackgroundColor = { 0.f, 0.f, 0.f };
	glm::mat4 m_ProjectionMatrix = glm::ortho(-0.5f, 12.5f, 19.5f, -0.5f, -1.f, 1.f);
	std::vector<RenderEntry> m_RenderEntries;

public:
	int Init();
	GLFWwindow* CreateWindow(const std::string& title, int width = 0, int height = 0);

	glm::ivec2 GetScreenResolution();

	void RenderFrame();
	void DrawRenderEntry(RenderEntry& renderEntry);

	unsigned long long AddRenderEntry(void* inOwner, Mesh* inMesh, Transform* inTransform, Shader* inShader, int renderPriority);
	void RemoveRenderEntries(void* inOwner);
	void RemoveSpecificRenderEntry(unsigned int inRenderEntryID);

	std::vector<RenderEntry>::iterator GetFirstRenderEntry(void* inOwner);

	void SetBackgroundColor(const glm::vec3& inColor);
	void Clear();

};

