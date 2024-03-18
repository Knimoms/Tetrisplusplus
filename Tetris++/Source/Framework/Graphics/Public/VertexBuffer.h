#pragma once

#include "glm/glm.hpp"
#include <vector>

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;
	float brightness = 1.0f;

};

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	bool b_DynamicBuffer;

public:
	VertexBuffer(const std::vector<Vertex>& vertices);
	VertexBuffer(int maxVertexCount);
	~VertexBuffer();

	void SetVertices(const std::vector<Vertex>& vertices);

	void Bind() const;
	static void Unbind();
};