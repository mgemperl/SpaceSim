#pragma once

#include <set>
#include "Entity.h"

namespace SpaceSimNS
{

class Moveable : public Entity
{
public:

	Moveable::Moveable(
		const Space::Point2D<double>& inPos,
		Faction inAllegience
		);

	Moveable::Moveable(
		const Space::Point2D<double>& inPos,
		const Space::Vector2D& initVel,
		Faction inAllegience
		);

	~Moveable();

	virtual bool Intact() const override = 0;

	inline virtual ClassType GetClass() const override { return MOVEABLE; }

	virtual void PhysUpdate() override;

	Space::Point2D<double> GetInterpPos(double interpolation) const override;

	const Space::Vector2D& GetVelocity() const { return m_Velocity; }

	Space::Point2D<double> ProjectCollisionPointSimple(
		const Space::Point2D<double>& collideePos, 
		double colliderSpeed) const override;

	Space::Point2D<double> ProjectCollisionPoint(
		const Space::Point2D<double>& collideePos,
		double colliderSpeed) const override;

	void SetVelocity(const Space::Vector2D& velocity);
	void Accelerate(const Space::Vector2D& dV);
	void Accelerate(const Space::Vector2D& dV, double maxSpeed);

protected:

	

private:

	Space::Vector2D m_Velocity;
	
};

}