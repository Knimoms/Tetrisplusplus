#pragma once

#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

class Mesh
{
private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;

	ElementBuffer m_ElementBuffer;
	VertexBuffer m_VertexBuffer;

	VertexArray m_VertexArray;

public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	Mesh AddMesh(const Mesh& otherMesh, const glm::vec2& positionOffset);

	inline const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
	inline const std::vector<unsigned int>& GetIndices() const { return m_Indices; }

	inline const ElementBuffer& GetElementBuffer() const { return m_ElementBuffer; }

	void Bind() const;
	static void Unbind();
};