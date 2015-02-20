#pragma once

#include <map>

namespace Space
{
	class Vector2D;
}

namespace SpaceSimNS
{

class CollisionPolygon
{

public:

	CollisionPolygon(const std::vector<Space::Point2D<double>>& vertices);
	~CollisionPolygon();

private:

	/** A map of this polygon's vertices where each key is the
		angle between the center and its vertex. The map will
		be sorted according to the angle between the center and
		the vertex in the counter-clockwise direction **/
	std::map<double, Vector2D> m_vertices;

};

}