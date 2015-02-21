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

	void Damage(Vessel* victim, const Space::Point2D<double>& hitPoint);

	inline const WeapDef GetWeapDef() const { return m_def; }

private:

	WeapDef m_def;
	

};

}
