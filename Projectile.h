#pragma once

#include "Moveable.h"

namespace SpaceSimNS
{

class Projectile : public Moveable
{
public:

	Projectile(
		const Space::Point2D<double>& inPos,
		const Space::Vector2D& inVelocity,
		Faction inAllegience,
		const WeapDef& def
		);

	~Projectile();

	bool InitializeDrawable(Graphics* pGraphics) override
	{
		return Entity::InitializeDrawable(pGraphics, "laser");
	}

	//virtual void PhysUpdate() override;

	virtual void Update(double deltaT) override;

	virtual void HandleMessage(const Telegram& message) override;

	bool Intact() const override;

	inline virtual ClassType GetClass() const override;

	void Hit(Entity* victim, const Space::Point2D<double>& hitPoint);

	virtual bool Impact(
		const Projectile* proj,
		const Space::Point2D<double>& impactPoint
		);

	typedef std::vector<std::pair<Space::Point2D<double>, double>> IntersectVector;

	static bool CheckForCollisions(Moveable* collider) { return false; }

/*
	static IntersectVector CheckIfCollides(
		const Space::Point2D<double>& pos,
		const Space::Point2D<double>& nextPos, 
		const Space::Ellipse& pEll);
*/

private:

	WeapDef weapon;
	double lifetime;
	bool madeImpact;
	Faction owner;

};

}