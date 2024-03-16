#pragma once

#include "glm/glm.hpp"
#include <vector>

struct Vertex
{
	glm::vec2 position;
	glm::vec3 color;
	float brightness;

};

class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const std::vector<Vertex>& vertices);
	~VertexBuffer();

	void Bind() const;
	static void Unbind();
};