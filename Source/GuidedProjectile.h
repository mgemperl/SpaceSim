#pragma once

#include "Vessel.h"
#include "Damager.h"
#include "Data.h"
#include "GuidedProjectileAI.h"

namespace SpaceSimNS
{

class GuidedProjectile : public Vessel, Damager
{
public:

	GuidedProjectile(
		const char* defName,
		Faction inAllegience,
		const Space::Point2D<double>& inPos,
		const Space::Vector2D& inVelocity,
		const WeapDef& projDef,
		GuidedProjectileAI* pCont,
		const Vessel* pTarget);

	~GuidedProjectile();

	void Update(double elapsedTime) override;

	void CollisionUpdate() override { CheckForCollisions(this); }

	inline ClassType GetClass() const override { return GUIDEDPROJECTILE; }

private:
	
	bool madeImpact;

};


}