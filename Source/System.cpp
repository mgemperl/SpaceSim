#include "System.h"
#include "Sector.h"
#include "Fleet.h"

namespace SpaceSimNS
{

System::System(double systemRadius) 
{
	m_SystemRadius = systemRadius;
}


System::~System()
{
}

Space::Point2D<double> System::GetCoords() const
{
	return m_Coordinates;
}

std::set<CelestialBody*> System::GetBodies()
{
	return m_CelestialBodies;
}

void System::AddBody(CelestialBody* body)
{
	m_CelestialBodies.emplace(body);
}

void System::RemoveBody(CelestialBody* body)
{
	m_CelestialBodies.erase(body);
}

void System::AddFleet(Fleet* fleet)
{
	if (fleet == NULL)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Attempted to add NULL fleet to system");
	}

	m_FleetsPresent.emplace(fleet);
}

void System::RemoveFleet(Fleet* fleet)
{
	if (fleet == NULL)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Attempted to remove NULL fleet to system");
	}

	m_FleetsPresent.erase(fleet);
}

bool System::ContainsFleet(Fleet* fleet) const
{
	return (fleet->GetSystem() == this);
}

}