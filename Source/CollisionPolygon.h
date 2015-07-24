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
	
	/** Copy constructor **/
	CollisionPolygon(const CollisionPolygon& other);
	~CollisionPolygon();

	/** 
	 * Checks if the two polygons will collide given their velocities,
	 * orientations, and offset.
	 */
	static bool CollisionPolygon::DetectCollision(
		const CollisionPolygon& firstPoly,
		const CollisionPolygon& secondPoly,
		const Space::Point2D<double>& offset,
		const Space::Vector2D& firstVelocity,
		const Space::Vector2D& secondVelocity,
		double firstOrient,
		double secondOrient);

	/**
	 * Checks if the argued polygons overlap given their offset.
	 */
	static bool CollisionPolygon::DetectOverlap(
		const CollisionPolygon& first,
		const CollisionPolygon& second,
		const Space::Point2D<double>& offset);

	/** Takes a velocity and collision polygon pointer, and rotates by the
		polygon's orientation and then sweeps the collision polygon along 
		the argued vector. (This will destroy the polygon) **/
	static void SweepPolygon(CollisionPolygon& swept, 
		const Space::Vector2D& velocity,
		const Space::Point2D<double>& offset,
		double orientation);

	const Space::Vector2D* GetMinVertex()
	{
		return m_min;
	}

	const Space::Vector2D* GetMaxVertex()
	{
		return m_max;
	}

private:

	/** A set of vectors centered at the origin with 
		terminal points at the polygon's vertices.
		It's sorted according to the angle between the center and
		the vertex in the counter-clockwise direction **/
	std::set<Space::Vector2D*, VectorPtrComp> m_vertices;

	/** Use these for a bounding box collision pre-check (set them
		up during construction) **/
	Space::Vector2D* m_min;
	Space::Vector2D* m_max;

	typedef std::pair<Space::Vector2D, Space::Vector2D> Edge;

	/** Get the two vertices from this polygon that need to be
		checked for overlap with another. These correspond with
		the edge along the line joining the centers of the two
		polygons **/
	Edge GetEdge(Space::Vector2D& offset, double angle) const;

	static void ComputeProjection(const Space::Vector2D& axis, 
		const std::set<Space::Vector2D*, VectorPtrComp>& vertices, 
		double& outMin, 
		double& outMax);

	static bool ProjectionsOverlap(double minA, double maxA,
		double minB, double maxB)
	{
		return	(maxA > minB && minA < minB || 
			maxB > minA && minB < minA ||
			(minA > minB && maxA < maxB) ||
			(minB > minA && maxB < maxA));
	}

	void Rotate(double theta);

	bool IsConvex() const;

	void IdentifyMinMax();

	static bool OverlapPossible(CollisionPolygon* first, 
		CollisionPolygon* second, 
		Space::Vector2D* firstVel,
		Space::Vector2D* secondVel,
		const Space::Point2D<double>& offset);
};

}