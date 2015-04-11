#include "Projectile.h"
#include "Sector.h"
#include "Data.h"
#include "MessageDispatcher.h"
#include "Vector2D.h"
#include <vector>

using namespace Space;

namespace SpaceSimNS
{

	Projectile::Projectile(
		const Space::Point2D<double>& inPos,
		const Space::Vector2D& inVelocity,
		Faction inAllegience,
		const WeapDef& def) :
	Moveable(inPos,
	inVelocity,
	inAllegience),
	Damager(def)
{
	SetOrientation(inVelocity.GetAngle());
	owner = inAllegience;
	SetCollider(true);
	madeImpact = false;

	// Set up collision polygon (replace this later with a constructor parameter)
	std::vector<Point2D<double>> polygon;
	polygon.emplace_back(Point2D<double>(1, 3));
	polygon.emplace_back(Point2D<double>(-1, 3));
	polygon.emplace_back(Point2D<double>(-1, -3));
	polygon.emplace_back(Point2D<double>(1, -3));
	SetCollisionPolygon(polygon);
}

Projectile::~Projectile()
{
}

void Projectile::CollisionUpdate()
{
	CheckForCollisions(this);
}

void Projectile::Update(double deltaT)
{
	UpdateLifetime(deltaT);

	if (RemainingLifetime() < 0)
	{
		GetSector()->DestroyEntity(this);
	}
}

void Projectile::HandleMessage(const Telegram& message)
{

}

inline ClassType Projectile::GetClass() const
{
	return PROJECTILE;
}

bool Projectile::Impact(
	const Projectile* proj,
	const Space::Point2D<double>& impactPoint)
{
	return false;
}

bool Projectile::Intact() const
{
	return !madeImpact;
}

}