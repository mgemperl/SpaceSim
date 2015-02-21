#include <iterator>
#include "GameException.h"
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
		m_vertices.emplace(new Vector2D(point));
	}

}


CollisionPolygon::~CollisionPolygon()
{
	for (Vector2D* vector : m_vertices)
	{
		delete vector;
	}
}

bool CollisionPolygon::DetectCollision(CollisionPolygon* other,
	const Point2D<double>& offset, 
	double thisAngle, 
	double otherAngle)
{
	if (other == NULL)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Attempted collision detection on NULL polygon pointer");
	}

	Vector2D distance = Vector2D(offset);

	// Since the polygon's perspectives are mirrored,
	// the first vertex of one edge goes with the second 
	// of the other.
	Edge thisEdge = GetEdge(distance, thisAngle);
	Edge otherEdge = other->GetEdge(-distance, otherAngle);

	return offset == Point2D<double>(0,0) ||
		SAT(distance, thisEdge.first, otherEdge.second) ||
		SAT(distance, thisEdge.second, otherEdge.first);

}

CollisionPolygon::Edge CollisionPolygon::GetEdge(
	Vector2D& offset, double angle)
{
	Vector2D::SimplifyAngle(angle);

	// Rotate the axis to find the edge along the line from this polygon's
	// center to the other polygon's center.
	Vector2D axis = Vector2D(offset.GetAngle() - angle, offset.GetMagnitude());

	Vector2D first;
	Vector2D second;

	// Get the two vertices closest to the line joining the polygon's centers
	std::set<Vector2D*>::iterator itr = m_vertices.lower_bound(&axis);
	if (itr == m_vertices.end())
	{
		first = **m_vertices.rbegin();
		second = **m_vertices.begin();
	}
	else
	{
		second = **itr;

		if (itr == m_vertices.begin())
		{
			first = **m_vertices.rbegin();
		}
		else
		{
			first = **(--itr);
		}
	}
	
	// Rotate the edge according the the argued angle 
	first.rotate(angle);
	second.rotate(angle);

	return CollisionPolygon::Edge(first, second);
}

}