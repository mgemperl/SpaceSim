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

	virtual void ConstUpdate() override
	{
		SetPosition(m_Velocity.GetTerminal(GetPos()));
	}

	Space::Point2D<double> GetInterpPos(double interpolation) const override
	{
		Space::Vector2D interpVec = Space::Vector2D(m_Velocity);
		interpVec.scaleBy(interpolation);

		return interpVec.GetTerminal() + GetPos();
	}

	Space::Vector2D GetVelocity() const override 
	{ 
		return m_Velocity; 
	}

	Space::Point2D<double> ProjectCollisionPointSimple(
		const Space::Point2D<double>& collideePos, 
		double colliderSpeed) const override;

	Space::Point2D<double> ProjectCollisionPoint(
		const Space::Point2D<double>& collideePos,
		double colliderSpeed) const override;

	void SetVelocity(const Space::Vector2D& velocity)
	{
		m_Velocity = velocity;
	}

	void Accelerate(const Space::Vector2D& dV)
	{
		m_Velocity += dV;
	}

	void Accelerate(const Space::Vector2D& dV, double maxSpeed)
	{
		m_Velocity += dV;

		if (m_Velocity.GetSqrMagnitude() > maxSpeed * maxSpeed)
		{
			m_Velocity.scaleTo(maxSpeed);
		}
	}

protected:

	

private:

	Space::Vector2D m_Velocity;
	
};

}