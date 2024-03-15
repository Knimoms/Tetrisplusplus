#include "Mesh.h"
#include "VertexBufferLayout.h"

Transform Transform::ZeroTransform = { {0, 0}, 0 };

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	:m_Vertices(vertices), m_Indices(indices), m_VertexBuffer(vertices), m_ElementBuffer(indices)
{
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(3);

	m_VertexArray.AddBuffer(m_VertexBuffer, layout);
}

Mesh Mesh::AddMesh(const Mesh& otherMesh, const glm::vec2& positionOffset)
{
	auto newVertices = m_Vertices;
	const auto& addingVertices = otherMesh.GetVertices();
	size_t verticesSize = addingVertices.size();

	for (int i = 0; i < verticesSize; ++i)
	{
		Vertex newVertex = addingVertices[i];
		newVertex.position[0] += positionOffset[0];
		newVertex.position[1] += positionOffset[1];
		newVertices.push_back(newVertex);
	}

	int indexOffset = (int)m_Vertices.size();

	auto newIndices = m_Indices;
	const auto& addingIndices = otherMesh.GetIndices();
	size_t indicesSize = addingIndices.size();

	for (int i = 0; i < indicesSize; ++i)
		newIndices.push_back(addingIndices[i] + indexOffset);

	return Mesh(newVertices, newIndices);
}

void Mesh::Bind() const
{
	m_VertexArray.Bind();
	m_ElementBuffer.Bind();
}

void Mesh::Unbind()
{
	VertexArray::Unbind();
	ElementBuffer::Unbind();
}