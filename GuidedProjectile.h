#pragma once

#include "ShipAI.h"
#include "Data.h"

namespace SpaceSimNS
{


class GuidedProjectile : public Projectile
{
public:

	GuidedProjectile(
		const ShipDef& shipDef,
		Faction inAllegience,
		const Space::Point2D<double>& inPos,
		const Space::Vector2D& inVelocity,
		const WeapDef& projDef,
		const Vessel* pTarget);

	~GuidedProjectile();

	void Update();

	void Hit(Entity* victim, const Space::Point2D<double>& hitPoint);

	inline ClassType GetClass() const { return GUIDEDPROJECTILE; }

	void SetOrder(StateType order);

	void SetFleet(Fleet* pNeoFleet) {};
	void SetLeader(Vessel* leader) {};

private:

	
	WeapDef weapon;
	int lifetime;
	bool madeImpact;

};


}