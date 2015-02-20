#include "CollisionPolygon.h"
#include "Vector2D.h"

using namespace Space;

namespace SpaceSimNS
{

CollisionPolygon::CollisionPolygon(const std::vector<Point2D<double>>& vertices)
{
	m_vertices.clear();

	for (const Point2D<double>& point : vertices)
	{
		Vector2D vector = Vector2D(point);
		m_vertices.emplace(vector.GetAngle(), vector);
	}

}


CollisionPolygon::~CollisionPolygon()
{
}

}