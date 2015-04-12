#include "Moveable.h"
#include "CollisionPolygon.h"

using namespace Space;

namespace SpaceSimNS
{

Moveable::Moveable(
	const Space::Point2D<double>& inPos,
	Faction inAllegience) 
	:Entity(inPos, inAllegience)
{
	m_Velocity = Vector2D();
}

Moveable::Moveable(
	const Space::Point2D<double>& inPos,
	const Vector2D& initVel,
	Faction inAllegience)
	: Entity(inPos, inAllegience)
{
	m_Velocity = initVel;
}

Moveable::~Moveable()
{
	
}


void Moveable::Accelerate(const Space::Vector2D& dV, double maxSpeed)
{
	Vector2D neoVel = Vector2D(m_Velocity + dV);
	double neoSqrMag = neoVel.GetSqrMagnitude();
	double sqrMag = m_Velocity.GetSqrMagnitude();
	double maxSqrSpeed = maxSpeed * maxSpeed;

	// If this acceleration would put the entity over max speed,
	// just set the velocity's angle to what it would be after 
	// the acceleration.
	if (neoSqrMag > maxSqrSpeed && neoSqrMag > sqrMag)
	{
		m_Velocity.rotateTo(neoVel.GetAngle());

		// If previously under max speed, scale up to max speed
		if (sqrMag < maxSqrSpeed)
		{
			m_Velocity.scaleTo(neoVel.GetMagnitude());
		}
	}
	else
	{
		m_Velocity = neoVel;
	}
}

Space::Point2D<double> Moveable::ProjectCollisionPoint(
	const Space::Point2D<double>& colliderPos,
	double colliderSpeed) const
{
	Vector2D offsetVector = Vector2D(colliderPos, GetPos());
	offsetVector.scaleTo(1);

	colliderSpeed -= GetVelocity().Dot(offsetVector);

	double ticks = (GetPos().Distance(colliderPos) / colliderSpeed);
	Space::Vector2D colVec = Space::Vector2D(GetVelocity());
	colVec.scaleBy(ticks);

	return colVec.GetTerminal(GetPos());
}

Space::Point2D<double> Moveable::ProjectCollisionPointAdv(
	const Space::Point2D<double>& colliderPos, 
	double colliderSpeed) const
{
	Point2D<double> collideePos = GetPos();
	int ticks = 1;

	Vector2D collideeVec = GetVelocity();
	collideeVec.scaleBy(ticks);

	Vector2D colliderVec = Vector2D(Vector2D::ComputeAngle(colliderPos, collideeVec.GetTerminal(collideePos)), colliderSpeed);
	colliderVec.scaleBy(ticks);

	double minSqrMiss = collideeVec.GetTerminal(collideePos).SqrDistance(colliderVec.GetTerminal(colliderPos));
	
	ticks++;
	bool done = false;
	while (!done)
	{
		collideeVec = m_Velocity;
		collideeVec.scaleBy(ticks);

		colliderVec = Vector2D(Vector2D::ComputeAngle(colliderPos, collideeVec.GetTerminal(collideePos)), colliderSpeed);
		colliderVec.scaleBy(ticks);

		double neoSqrMiss = collideeVec.GetTerminal(collideePos).SqrDistance(colliderVec.GetTerminal(colliderPos));

		if (neoSqrMiss < minSqrMiss)
		{
			minSqrMiss = neoSqrMiss;
			ticks++;
		}
		else
		{
			done = true;
		}
	}
	
	return collideeVec.GetTerminal(collideePos);
}

}