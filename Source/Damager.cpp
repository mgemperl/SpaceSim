#include "Damager.h"
#include "Sector.h"
#include "Data.h"

using namespace Space;

namespace SpaceSimNS
{

Damager::Damager(const WeapDef& def)
{
	m_def = def;
	m_dLifetime = def.lifetime;
}

Damager::~Damager()
{
}

void Damager::CheckForCollisions(Entity* pThis)
{
	for (const Entity* pEntity : pThis->GetSector()->Occupants())
	{
		if (pEntity != pThis &&
			pEntity->IsDamageable() &&
			pEntity->IsHostile(pThis) &&
			pThis->CollidesWith(pEntity))
		{
			Damage(pEntity, Point2D<double>());
			pThis->GetSector()->DestroyEntity(pThis);
		}
	}
}

void Damager::Damage(const Entity* victim, const Point2D<double>& hitPoint)
{
	// Send impact message through message dispatcher 
}

}