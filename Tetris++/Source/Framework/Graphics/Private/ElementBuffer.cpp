#include "ElementBuffer.h"
#include <GL/glew.h>

ElementBuffer::ElementBuffer(const std::vector<unsigned int>& indices)
	:m_Count((unsigned int)indices.size()), b_DynamicBuffer(false)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

ElementBuffer::ElementBuffer(int maxIndexCount)
	:b_DynamicBuffer(true)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndexCount * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
}

ElementBuffer::~ElementBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void ElementBuffer::SetIndices(const std::vector<unsigned int>& indices)
{
	Bind();
	m_Count = indices.size();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
}

void ElementBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void ElementBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
