#include "GuidedProjectile.h"
#include "GuidedProjectileAI.h"
#include "Sector.h"


namespace SpaceSimNS
{

	GuidedProjectile::GuidedProjectile(
		const char* defName,
		Faction inAllegience,
		const Space::Point2D<double>& inPos,
		const Space::Vector2D& inVel,
		const WeapDef& projDef,
		GuidedProjectileAI* pCont,
		const Vessel* pTarget):
Vessel(defName, inAllegience, pCont),
Damager(projDef)
{
		SetPosition(inPos);
		SetVelocity(inVel);
		SetOrientation(inVel.GetAngle());
		madeImpact = false;
		SetDamageable(false);
		SetTarget(pTarget);
}


GuidedProjectile::~GuidedProjectile()
{
}


void GuidedProjectile::Update(double deltaT)
{
	UpdateLifetime(deltaT);

	// If lifetime is expired, destroy the projectile
	if (RemainingLifetime() < 0)
	{
		GetSector()->DestroyEntity(this);
	}
	// Otherwise, update the projectile.
	else
	{
		Vessel::Update(deltaT);
	}
}

}