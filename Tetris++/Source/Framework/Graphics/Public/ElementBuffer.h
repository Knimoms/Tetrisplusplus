#pragma once

#include <vector>

class ElementBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
	bool b_DynamicBuffer;

public:
	ElementBuffer(const std::vector<unsigned int>& indices);
	ElementBuffer(int maxIndexCount);
	~ElementBuffer();

	void SetIndices(const std::vector<unsigned int>& indices);

	void Bind() const;
	static void Unbind();

	inline unsigned int GetCount() const { return m_Count; }
};

