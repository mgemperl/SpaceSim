#include "DrawableLine.h"

using namespace Space;

DrawableLine::DrawableLine()
{
	m_pGraphics = NULL;
	m_width = 1;
	m_vertices.push_back(Point2D<float>(0, 0));
	m_vertices.push_back(Point2D<float>(0, 0));
}

DrawableLine::~DrawableLine()
{
}

bool DrawableLine::Initialize(Graphics* pGraphics, int width)
{
	m_pGraphics = pGraphics;
	m_width = width;
	return pGraphics != NULL;
}

void DrawableLine::Draw(COLOR_ARGB color)
{
	m_pGraphics->DrawPolygon(m_vertices, color);
}