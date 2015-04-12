#pragma once

#include "Graphics.h"
#include "Point2D.h"

class DrawableLine
{

public:

	DrawableLine();
	~DrawableLine();

	bool Initialize(Graphics* pGraphics, int width = 1);

	Space::Point2D<float> VertexA() { return m_vertices[0]; }

	Space::Point2D<float> VertexB() { return m_vertices[1]; }

	void SetVertices(const Space::Point2D<float>& neoVertA,
		const Space::Point2D<float>& neoVertB)
	{
		m_vertices[0] = neoVertA;
		m_vertices[1] = neoVertB;
	}

	void SetVertexA(const Space::Point2D<float>& neoVertex) 
	{
		m_vertices[0] = neoVertex;
	}

	void SetVertexB(const Space::Point2D<float>& neoVertex)
	{
		m_vertices[1] = neoVertex;
	}

	void Draw(COLOR_ARGB color = WHITE);

private:

	Graphics* m_pGraphics;
	std::vector<Space::Point2D<float>> m_vertices;
	int m_width;

};

