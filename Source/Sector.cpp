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
	m_dRadius = 500;
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
	// Update fleets
	for (Fleet* pFleet : m_fleets)
	{
		if (pFleet == NULL)
		{
			throw GameException(GameExceptionNS::FATAL_ERROR,
				"Sector has a NULL fleet");
		}

		pFleet->Update(deltaT);
	}

	// Update entities
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

		pEntity->ConstUpdate();
	}

	HandleCollisions();
}

void Sector::HandleCollisions()
{
	// For each entity, if it's a collider (just projectiles at the moment),
	// see if it collides with any hostile, damageable entities in the same sector.
	// If it collides, remove the collider.
	for (Entity* pEntity : m_occupants)
	{
		pEntity->CollisionUpdate();
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
	double rotation = TWO_PI / fleets.size();
	double angle = 0;

	for (int itr = 0; itr < fleets.size(); itr++)
	{
		fleets[itr]->EnterSector(this, angle);
		angle += rotation;

		/*
		Point2D<double> fleetCenter = Point2D<double>(100, 100);

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
		*/

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

