#pragma once

#include <vector>
#include "GL/glew.h"
#include "Macros.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}

		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_VertexBufferElements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {};

	template <typename T>
	void Push(unsigned int count)
	{
		ASSERT(false);
	}

	template <>
	void Push<float>(unsigned int count)
	{
		m_VertexBufferElements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}

	template <>
	void Push<unsigned int>(unsigned int count)
	{
		m_VertexBufferElements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template <>
	void Push<unsigned char>(unsigned int count)
	{
		m_VertexBufferElements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement>& GetVertexBufferElements() const { return m_VertexBufferElements; }
	inline unsigned int GetStride() const { return m_Stride; }
};

