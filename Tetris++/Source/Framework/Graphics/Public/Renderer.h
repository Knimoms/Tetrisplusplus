#pragma once
#include <vector>
#include "glm/gtc/matrix_transform.hpp"

class Mesh;
struct Transform;
class Shader;

struct RenderEntry
{
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
	void RenderFrame();
	void DrawRenderEntry(RenderEntry& renderEntry);

	void AddRenderEntry(void* inOwner, Mesh* inMesh, Transform* inTransform, Shader* inShader, int renderPriority);
	void RemoveRenderEntries(void* inOwner);

	std::vector<RenderEntry>::iterator GetFirstRenderEntry(void* inOwner);

	void SetBackgroundColor(const glm::vec3& inColor);
	void Clear();

};

