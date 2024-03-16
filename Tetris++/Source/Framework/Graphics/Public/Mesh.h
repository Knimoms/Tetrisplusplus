#pragma once

#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

struct Transform
{
	glm::vec2 position = { 0, 0 };
	float rotation = 0.f;

	static Transform ZeroTransform;
};

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

	inline const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
	inline const std::vector<unsigned int>& GetIndices() const { return m_Indices; }

	inline const ElementBuffer& GetElementBuffer() const { return m_ElementBuffer; }

	void Bind() const;
	static void Unbind();

	static const std::vector<Vertex>& GetCubeVertices()
	{
		float edgeThickness = 0.1f;
		float edgeBrightness = 0.6f;

		static std::vector<Vertex> vertices
		{
			{{ -(0.5f - edgeThickness), -(0.5f - edgeThickness)}, { 0.f, 0.f, 0.f },  1.0f},			//0
			{{  (0.5f - edgeThickness), -(0.5f - edgeThickness)}, { 0.f, 0.f, 0.f },  1.0f},			//1
			{{  (0.5f - edgeThickness),  (0.5f - edgeThickness)}, { 0.f, 0.f, 0.f },  1.0f},			//2
			{{ -(0.5f - edgeThickness),  (0.5f - edgeThickness)}, { 0.f, 0.f, 0.f },  1.0f},			//3

			{{ -(0.5f - edgeThickness), -(0.5f - edgeThickness)}, { 0.f, 0.f, 0.f },  edgeBrightness},	//4
			{{  (0.5f - edgeThickness), -(0.5f - edgeThickness)}, { 0.f, 0.f, 0.f },  edgeBrightness},	//5
			{{  (0.5f - edgeThickness),  (0.5f - edgeThickness)}, { 0.f, 0.f, 0.f },  edgeBrightness},	//6
			{{ -(0.5f - edgeThickness),  (0.5f - edgeThickness)}, { 0.f, 0.f, 0.f },  edgeBrightness},	//7
														 
			{{ -0.5f,					-0.5f},					  { 0.f, 0.f, 0.f },  edgeBrightness},	//8
			{{  0.5f,					-0.5f},					  { 0.f, 0.f, 0.f },  edgeBrightness},	//9
			{{  0.5f,					 0.5f},					  { 0.f, 0.f, 0.f },  edgeBrightness},	//10
			{{ -0.5f,					 0.5f},					  { 0.f, 0.f, 0.f },  edgeBrightness}	//11

		};

		return vertices;
	}

	static const std::vector<unsigned int>& GetCubeIndices()
	{
		static std::vector<unsigned int> indices
		{
			0, 1, 2,
			2, 3, 0,

			8, 9, 5,
			5, 4, 8,

			5, 9, 10,
			10, 6, 5,

			7, 6, 10,
			10, 11, 7,

			8, 4, 7,
			7, 11, 8
		};

		return indices;
	}
};