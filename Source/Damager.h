#pragma once

#include "Vessel.h"

namespace SpaceSimNS
{

class CollisionPolygon;

class Damager
{

public:

	Damager(const WeapDef& def);
	~Damager();

	void UpdateLifetime(double elapsedTime)
	{
		m_dLifetime -= elapsedTime;
	}

	// Assumes the damager is extending an Entity 
	void CheckForCollisions(Entity* pThis);

	void Damage(const Entity* victim, const Space::Point2D<double>& hitPoint);

	double RemainingLifetime() const { return m_dLifetime; }

	inline const WeapDef GetWeapDef() const { return m_def; }

private:

	double m_dLifetime;
	WeapDef m_def;
	

};

}
