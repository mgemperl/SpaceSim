#include "CelestialBody.h"
#include "System.h"
#include "Vector2D.h"
#include <cmath>

#define ORBIT_RATE 0.01

namespace SpaceSimNS
{

CelestialBody::CelestialBody(double bodyRadius, 
	System* pSystem,
	double orbitRadius,
	double initTheta) :
	Entity(Space::Point2D<double>(0, 0))
{
	m_pSystem = pSystem;
	m_pSystem->AddBody(this);
	m_OrbitRadius = orbitRadius;
	m_Theta = initTheta;
}


CelestialBody::~CelestialBody()
{
	m_pSystem->RemoveBody(this);
}

bool CelestialBody::InitializeDrawable(Graphics* pGraphics)
{
	return Entity::InitializeDrawable(pGraphics, "planet");
}

/* I need some sort of function to figure out where in the system a body would be
 * at a given point in the orbit cycle.
 * This update function should have no implementation though, I just didn't bother
 * deleting these lines in case they'd be useful later.
void CelestialBody::Update(double deltaT)
{
	double neoTheta = Space::Vector2D::SimplifyAngle(m_Theta + ORBIT_RATE);
	Space::Point2D<double> relPos = Space::Point2D<double>(cos(neoTheta) * m_OrbitRadius, sin(neoTheta) * m_OrbitRadius);
	SetPosition(m_pSystem->GetCoords() + relPos);
}
*/

System* CelestialBody::GetSystem() const
{
	return m_pSystem;
}

double CelestialBody::GetOrbitRadius() const
{
	return m_OrbitRadius;
}

double CelestialBody::GetOrbitTheta() const
{
	return m_Theta;
}

}