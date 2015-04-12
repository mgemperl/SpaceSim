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
		// Since input vertices expect zero radians to face north and the engine
		// expects zero radians to face east, rotate input vertices to compensate
		Vector2D vector = Vector2D(point);
		vector.rotate(-HALF_PI);

		m_vertices.emplace(new Vector2D(vector));
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

bool CollisionPolygon::DetectCollision(
	const CollisionPolygon& firstPoly,
	const CollisionPolygon& secondPoly,
	const Point2D<double>& offset,
	const Vector2D& firstVelocity,
	const Vector2D& secondVelocity,
	double firstOrient,
	double secondOrient)
{
	bool collision = false;

	CollisionPolygon firstSwept = CollisionPolygon(firstPoly);
	CollisionPolygon secondSwept = CollisionPolygon(secondPoly);

	SweepPolygon(firstSwept, firstVelocity, Point2D<double>(0, 0), firstOrient);
	SweepPolygon(secondSwept, secondVelocity, offset, secondOrient);

	return DetectOverlap(firstSwept, secondSwept, offset) &&
		DetectOverlap(secondSwept, firstSwept, -offset);
}

bool CollisionPolygon::DetectOverlap(
	const CollisionPolygon& first,
	const CollisionPolygon& second,
	const Point2D<double>& offset) 
{
	// Projection min and max
	double firstMin;
	double firstMax;
	double secondMin;
	double secondMax;

	// Set up iterators and first axis
	std::set<Space::Vector2D*>::iterator itr = first.m_vertices.begin();
	std::set<Space::Vector2D*>::iterator prev = first.m_vertices.end();
	prev--;

	// Get separating axes and check for overlap on the first polygon
	while (itr != first.m_vertices.end())
	{
		Vector2D axis = Vector2D((*itr)->GetTerminal(), 
			(*prev)->GetTerminal());
		axis.rotate(HALF_PI);

		ComputeProjection(axis, first.m_vertices, firstMin, firstMax);

		ComputeProjection(axis, second.m_vertices, secondMin, secondMax);

		// If projections don't overlap, return false 
		if (!ProjectionsOverlap(firstMin, firstMax, secondMin, secondMax))
		{
			return false;
		}

		prev = itr++;
	}

	return true;
}

void CollisionPolygon::ComputeProjection(const Vector2D& axis, 
	const std::set<Vector2D*, VectorPtrComp>& vertices, 
	double& outMin, 
	double& outMax)
{
	std::set<Vector2D*, VectorPtrComp>::iterator itr = vertices.begin();

	// Initialize projection minimum and maximum	
	outMin = outMax = axis.Dot(**itr);

	// Scan the rest to find the min and max projection
	while (++itr != vertices.end())
	{
		double length = axis.Dot(**itr);

		// If projection is shorter than the min projection or longer
		// than the max projection, set update the min/max
		if (length < outMin)
		{
			outMin = length;
		}
		else if (length > outMax)
		{
			outMax = length;
		}
	}
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

/*
std::vector<Vector2D> GetNormal(Vector2D first, Vector2D second)
{
	Point2D<double> init = vertices[index].GetTerminal();
	Point2D<double> term = vertices[index + 1].GetTerminal();
	term.Set(term.GetX(), -term.GetY());
	return Vector2D(init, term);
}
*/

void CollisionPolygon::SweepPolygon(CollisionPolygon& swept,
	const Vector2D& velocity,
	const Point2D<double>& offset,
	double orient)
{
	// Translate all vertices in the same general direction (within HALF_PI inclusive) 
	// as the velocity vector by half the velocity vector. Translate all vertices 
	// in the opposite direction by half the negative velocity. Return half the
	// velocity vector as the offset of the center.

	// If orientation is non-zero, rotate all vertices
	if (orient + THETA_E < 0 || orient - THETA_E > 0)
	{
		swept.Rotate(orient);
	}

	bool nonZero = velocity.GetTerminal().GetX() - THETA_E > 0 ||
		velocity.GetTerminal().GetX() + THETA_E < 0 ||
		velocity.GetTerminal().GetY() - THETA_E > 0 ||
		velocity.GetTerminal().GetY() + THETA_E < 0;

	if (nonZero)
	{
		Point2D<double> displacement = velocity.GetTerminal() * 0.5;

		for (Vector2D* pVector : swept.m_vertices)
		{
			double angleDiff =
				fabs(Vector2D::AngleDiff(pVector->GetAngle(), velocity.GetAngle()));

			if (angleDiff + THETA_E < HALF_PI)
			{
				*pVector = Vector2D(pVector->GetTerminal() + displacement + offset);
			}
			else if (angleDiff - THETA_E > HALF_PI)
			{
				*pVector = Vector2D(pVector->GetTerminal() - displacement + offset);
			}
			else
			{
				*pVector = Vector2D(pVector->GetTerminal() + offset);
			}
		}
	}
}

void CollisionPolygon::Rotate(double theta)
{
	for (Vector2D* pVector : m_vertices)
	{
		pVector->rotate(theta);
	}
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

		angle = nextAngle;
	}

	return true;
}

}