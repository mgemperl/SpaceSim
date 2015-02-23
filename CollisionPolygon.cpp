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

	IsConvex();
}

CollisionPolygon::CollisionPolygon(const CollisionPolygon& other)
{
	for (Vector2D* vector : other.m_vertices)
	{
		m_vertices.emplace(new Vector2D(*vector));
	}

	IsConvex();
}

CollisionPolygon::~CollisionPolygon()
{
	for (Vector2D* vector : m_vertices)
	{
		delete vector;
	}
}

bool CollisionPolygon::DetectCollision(const CollisionPolygon* firstPoly,
	const CollisionPolygon* secondPoly,
	const Point2D<double>& offset,
	const Vector2D& firstVelocity,
	const Vector2D& secondVelocity,
	double firstOrient,
	double secondOrient)
{
	bool collision = false;



	CollisionPolygon firstSwept = CollisionPolygon(*firstPoly);
	CollisionPolygon secondSwept = CollisionPolygon(*secondPoly);

	Point2D<double> firstDisp = SweepPolygon(
		firstSwept, firstVelocity, firstOrient);

	Point2D<double> secondDisp = SweepPolygon(
		secondSwept, secondVelocity, secondOrient);

	collision = firstSwept.DetectCollision(&secondSwept,
		offset - firstDisp + secondDisp,
		0.0, 0.0);


	return collision;
}


bool CollisionPolygon::DetectCollision(const CollisionPolygon* dynPoly,
	const CollisionPolygon* statPoly,
	const Point2D<double>& offset,
	const Vector2D& velocity,
	double dynOrient,
	double statOrient)
{
	CollisionPolygon swept = CollisionPolygon(*dynPoly);
	Point2D<double> displacement = SweepPolygon(swept, velocity, dynOrient);

	return swept.DetectCollision(statPoly, 
		offset - displacement, 0.0, statOrient);
}

bool CollisionPolygon::DetectCollision(const CollisionPolygon* other,
	const Point2D<double>& offset, 
	double thisAngle, 
	double otherAngle)
{
	if (other == NULL)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Attempted collision detection on NULL polygon");
	}

	Vector2D distance = Vector2D(offset);

// Since the polygon's perspectives are mirrored,
// the first vertex of one edge goes with the second 
// of the other.
Edge thisEdge = GetEdge(distance, thisAngle);
Edge otherEdge = other->GetEdge(-distance, otherAngle);

return offset == Point2D<double>(0, 0) ||
SAT(distance, thisEdge.first, otherEdge.second) ||
SAT(distance, thisEdge.second, otherEdge.first);

}

CollisionPolygon::Edge CollisionPolygon::GetEdge(
	Vector2D& offset, double angle) const
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

Point2D<double> CollisionPolygon::SweepPolygon(CollisionPolygon& swept,
	const Vector2D& velocity,
	double orient)
{
	// Translate all vertices in the same general direction (within HALF_PI inclusive) 
	// as the velocity vector by half the velocity vector. Translate all vertices 
	// in the opposite direction by half the negative velocity. Return half the
	// velocity vector as the offset of the center.

	Point2D<double> displacement;

	for (Vector2D* pVector : swept.m_vertices)
	{
		pVector->rotate(orient);

		double angleDiff =
			fabs(Vector2D::AngleDiff(pVector->GetAngle(), velocity.GetAngle()));

		displacement = velocity.GetTerminal() * 0.5;

		if (angleDiff <= HALF_PI)
		{
			*pVector = Vector2D(pVector->GetTerminal() + displacement);
		}
		else
		{
			*pVector = Vector2D(pVector->GetTerminal() - displacement);
		}
	}

	return displacement;
}

bool CollisionPolygon::IsConvex() const
{
	std::set<Vector2D*, VectorPtrComp>::iterator itr = m_vertices.begin();
	std::set<Vector2D*, VectorPtrComp>::iterator next = m_vertices.begin();
	next++;
	bool convex = true;
	double angle = Vector2D::ComputeAngle((*itr++)->GetTerminal(),
		(*next++)->GetTerminal());

	while (next != m_vertices.end())
	{
		double nextAngle = Vector2D::ComputeAngle((*itr++)->GetTerminal(),
			(*next++)->GetTerminal());

		if (Vector2D::AngleDiff(angle, nextAngle) < 0)
		{
			throw GameException(GameExceptionNS::FATAL_ERROR,
				"Non-convex collision polygon");
		}
	}

	return true;
}

}