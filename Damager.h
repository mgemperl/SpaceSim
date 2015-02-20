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

	void Hit(Vessel* victim, const Space::Point2D<double>& hitPoint);

	virtual Space::Point2D<double> CollidesWith(
		const CollisionPolygon* pOther) = 0;

	const WeapDef GetWeapDef() const { return m_def; }

private:

	WeapDef m_def;
	

};

}
