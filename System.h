#pragma once

#include <set>
#include "Vector2D.h"
#include "CelestialBody.h"

namespace SpaceSimNS
{

class Fleet;

class System 
{
public:

	System(double systemRadius);
	~System();

	Space::Point2D<double> GetCoords() const;

	std::set<CelestialBody*> GetBodies();

	void AddBody(CelestialBody* body);

	void RemoveBody(CelestialBody* body);

	std::set<Fleet*> GetFleets() { return m_FleetsPresent; }

	void AddFleet(Fleet* fleet);

	void RemoveFleet(Fleet* fleet);

	bool ContainsFleet(Fleet* fleet) const;

private:

	std::set<CelestialBody*> m_CelestialBodies;
	std::set<Fleet*> m_FleetsPresent;
	Space::Point2D<double> m_Coordinates;
	double m_SystemRadius;

};

}