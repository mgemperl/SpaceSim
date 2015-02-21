#include "Sector.h"
#include "EntityManager.h"
#include "CelestialBody.h"
#include "Background.h"
#include "System.h"
#include "Vessel.h"
#include "Damager.h"
#include "VesselController.h"
#include "Fleet.h"

#include <queue>

#define SECTOR_RADIUS 1000

using namespace Space;

namespace SpaceSimNS
{

Sector::Sector(System* pSystem, Graphics* pGraphics)
{
	m_pSystem = pSystem;
	m_pGraphics = pGraphics;

	m_pBackground = new Background();
	m_pBackground->InitializeDrawable(pGraphics);
	LoadCelestialBodies(pSystem->GetBodies());
}

Sector::~Sector()
{
	delete m_pBackground;
}

void Sector::LoadCelestialBodies(std::set<CelestialBody*>& pBodies)
{
	for (CelestialBody* body : pBodies)
	{
		AddEntity(body);
	}
}

const Entity::ConstEntitySet Sector::Occupants() const
{
	Entity::ConstEntitySet occupants;
	occupants.insert(m_occupants.begin(), m_occupants.end());
	return occupants;
}

void Sector::Update(double deltaT)
{
	for (Entity* pEntity : m_occupants)
	{
		if (pEntity == NULL)
		{
			throw GameException(GameExceptionNS::FATAL_ERROR,
				"Sector has a NULL entity");
		}

		pEntity->Update(deltaT);
	}

	DeleteFlaggedEntities();
}

void Sector::ConstUpdate()
{
	for (Entity* pEntity : m_occupants)
	{
		if (pEntity == NULL)
		{
			throw GameException(GameExceptionNS::FATAL_ERROR,
				"Sector has a NULL entity");
		}

		pEntity->PhysUpdate();
	}

	HandleCollisions();
}

void Sector::HandleCollisions()
{
	for (Entity* pCollider: m_occupants)
	{
		if (pCollider->IsCollider())
		{
			for (Entity* pEntity : m_occupants)
			{
				if (pEntity != pCollider &&
					pEntity->IsDamageable() &&
					pCollider->CollidesWith(pEntity))
				{
					RemoveEntity(pCollider);
				}
			}
		}
	}
}

void Sector::AddEntity(Entity* entity)
{
	if (entity != NULL)
	{
		m_occupants.emplace(entity);
		entity->SetSector(this);
		entity->InitializeDrawable(m_pGraphics);
	}
}

void Sector::RemoveEntity(Entity* entity)
{
	if (entity != NULL)
	{
		m_toRemove.emplace(entity);
		entity->ReleaseDrawable();
	}
}

void Sector::DestroyEntity(Entity* entity)
{
	if (entity != NULL)
	{
		m_toDelete.emplace(entity);
	}
}

void Sector::DeleteFlaggedEntities()
{
	for (Entity* entity : m_toDelete)
	{
		m_occupants.erase(entity);
		delete entity;
	}

	m_toDelete.clear();
}

void Sector::RemoveFlaggedEntities()
{
	for (Entity* entity : m_toRemove)
	{
		m_occupants.erase(entity);
	}
}

bool Sector::ContainsEntity(const Entity* entity)
{
	return (entity != NULL && entity->GetSector() == this);
}

void Sector::AddFleets(std::vector<Fleet*>& fleets)
{
	for (int itr = 0; itr < fleets.size(); itr++)
	{
		Point2D<double> fleetCenter = Point2D<double>(100, 100);
			//Space::Vector2D::GetTerminalFromPolar(PI * itr, SECTOR_RADIUS);

		std::priority_queue<VesselController*> battleworthy = 
			fleets[itr]->GetBattleworthy();

		while (!battleworthy.empty())
		{
			Vessel* pVessel = battleworthy.top()->GetEntity();
			battleworthy.pop();
			pVessel->SetPosition(fleetCenter);
			pVessel->SetOrientation(PI - PI * itr);
			pVessel->Accelerate(Space::Vector2D(0.0, 5.0));
			fleetCenter += Point2D<double>(0, 30);
			AddEntity(pVessel);
		}

		m_fleets.emplace(fleets[itr]);
	}
}

void Sector::Render(double interp, Point2D<double>& offset)
{
	m_pBackground->Render(interp, offset);

	for (Entity* pEntity : m_occupants)
	{
		if (pEntity == NULL)
		{
			throw GameException(GameExceptionNS::FATAL_ERROR,
				"Sector has a NULL entity");
		}

		pEntity->Render(interp, offset);
	}
}

}

