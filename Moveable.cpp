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

bool Moveable::CollidesWith(Entity* other)
{
	bool collides = CollisionPolygon::DetectCollision(
		GetCollisionPolygon(),
		other->GetCollisionPolygon(),
		other->GetPos() - GetPos(),
		-m_Velocity,
		-other->GetVelocity(),
		GetOrientationRad(),
		other->GetOrientationRad());

	if (SqrDistance(other) < 2.0 && !collides)
	{
		char* str = "breakpoint";
	}

	return collides;
}

Space::Point2D<double> Moveable::ProjectCollisionPointSimple(
	const Space::Point2D<double>& colliderPos,
	double colliderSpeed) const
{
	double ticks = (GetPos().Distance(colliderPos) / colliderSpeed);
	Space::Vector2D colVec = Space::Vector2D(m_Velocity);
	colVec.scaleBy(ticks);

	return colVec.GetTerminal(GetPos());
}

Space::Point2D<double> Moveable::ProjectCollisionPoint(
	const Space::Point2D<double>& colliderPos, 
	double colliderSpeed) const
{
	Point2D<double> collideePos = GetPos();

	int ticks = 1;

	Vector2D collideeVec = m_Velocity;
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