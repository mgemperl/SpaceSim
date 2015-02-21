#pragma once

#include <vector>
#include <set>
#include "Vector2D.h"

namespace SpaceSimNS
{

class CollisionPolygon
{

public:

	// Comparator struct for sorting a collection of vector pointers 
	struct VectorPtrComp
	{
		// Order vectors by angle in the counter-clockwise direction
		bool operator()(const Space::Vector2D* lhs, 
			const Space::Vector2D* rhs) const
		{
			bool comp = false;

			// Be sure both argued vectors exist
			if (lhs != NULL && rhs != NULL)
			{
				comp = lhs->GetAngle() < rhs->GetAngle();
			}

			return comp;
		}
	};

	/** Constructs polygon using a vector of points centered at the origin **/
	CollisionPolygon(const std::vector<Space::Point2D<double>>& vertices);
	~CollisionPolygon();

	/** 
	 * Check if this polygon overlaps with the argued polygon offset
	 * by the argued offset point. Returns true if there is overlap,
	 * false if there is no overlap.
	 */
	bool DetectCollision(CollisionPolygon* other, 
		const Space::Point2D<double>& offset,
		double thisAngle,
		double otherAngle);

private:

	/** A set of vectors centered at the origin with 
		terminal points at the polygon's vertices.
		It's sorted according to the angle between the center and
		the vertex in the counter-clockwise direction **/
	std::set<Space::Vector2D*, VectorPtrComp> m_vertices;

	typedef std::pair<Space::Vector2D, Space::Vector2D> Edge;

	/** Get the two vertices from this polygon that need to be
		checked for overlap with another. These correspond with
		the edge along the line joining the centers of the two
		polygons **/
	Edge GetEdge(Space::Vector2D& offset, double angle);

	/** Implementation of a simple separating axis algorithm **/
	inline bool SAT(Space::Vector2D& axis, Space::Vector2D& a, 
		Space::Vector2D& b)
	{
		return ((axis.GetMagnitude() - axis.ProjLength(a) +
			axis.ProjLength(b)) < 0);
	}
};

}