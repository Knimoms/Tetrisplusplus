#pragma once

#include <vector>

class ElementBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	ElementBuffer(const std::vector<unsigned int>& indices);
	~ElementBuffer();

	void Bind() const;
	static void Unbind();

	inline unsigned int GetCount() const { return m_Count; }
};

