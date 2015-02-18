#include "GuidedProjectile.h"
#include "GuidanceStates.h"
#include "Sector.h"
#include "ShipAI.h"


namespace SpaceSimNS
{

	GuidedProjectile::GuidedProjectile(
		const ShipDef& shipDef,
		Faction inAllegience,
		const Space::Point2D<double>& inPos,
		const Space::Vector2D& inVel,
		const WeapDef& projDef,
		const Vessel* pTarget):
Projectile(inPos, inVel, inAllegience, projDef)
{
		SetPosition(inPos);
		SetVelocity(inVel);
		SetOrientation(inVel.GetAngle());
		weapon = projDef;
		lifetime = weapon.lifetime;
		madeImpact = false;
		SetTarget(pTarget);
		m_pAIStateMachine->SetState(new SeekTarget(this));
}


GuidedProjectile::~GuidedProjectile()
{
}


void GuidedProjectile::Update()
{
	
	// If lifetime has expired or a collision occurs, destroy this projectile.
	if (lifetime-- <= 0 || Projectile::CheckForCollisions(this))
	{
		GetSector()->DestroyEntity(this);
	}
	// Otherwise, update the projectile.
	else
	{
		Vessel::Update();
	}

	if (lifetime == 120)
	{
		m_pAIStateMachine->SetState(NULL);
	}
}

void GuidedProjectile::Hit(Entity* victim, const Space::Point2D<double>& hitPoint)
{

}

void GuidedProjectile::SetOrder(StateType order)
{
	switch (order)
	{
	case ATTACKSHIPTARGET : 
		m_pAIStateMachine->ChangeState(new SeekTarget(this));
		break;
	case STANDBY :
		m_pAIStateMachine->SetState(NULL);
	}
}

}