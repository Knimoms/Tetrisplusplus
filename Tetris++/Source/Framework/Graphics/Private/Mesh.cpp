#include "Mesh.h"
#include "VertexBufferLayout.h"

Transform Transform::ZeroTransform = { {0, 0}, 0 };

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
	:m_Vertices(vertices), m_Indices(indices), m_VertexBuffer(vertices), m_ElementBuffer(indices), b_DynamicMesh(false)
{
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(3);
	layout.Push<float>(1);

	m_VertexArray.AddBuffer(m_VertexBuffer, layout);
}

Mesh::Mesh(int maxVertexCount, int maxIndexCount)
	:m_VertexBuffer(maxVertexCount), m_ElementBuffer(maxIndexCount), b_DynamicMesh(true)
{
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(3);
	layout.Push<float>(1);

	m_VertexArray.AddBuffer(m_VertexBuffer, layout);
}

void Mesh::SetVertices(const std::vector<Vertex>& vertices)
{
	if(!b_DynamicMesh)
		return;

	m_Vertices = vertices;
	m_VertexBuffer.SetVertices(vertices);
}

void Mesh::SetIndices(const std::vector<unsigned int>& indices)
{
	if(!b_DynamicMesh)
		return;

	m_Indices = indices;
	m_ElementBuffer.SetIndices(indices);
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