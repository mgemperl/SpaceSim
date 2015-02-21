#include "Damager.h"
#include "Data.h"

using namespace Space;

namespace SpaceSimNS
{

Damager::Damager(const WeapDef& def)
{
	m_def = def;
}


Damager::~Damager()
{
}

void Damager::Damage(Vessel* victim, const Point2D<double>& hitPoint)
{
	victim->Impact(this, hitPoint);
}

}