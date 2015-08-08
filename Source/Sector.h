#pragma once

#include <unordered_set>
#include <unordered_map>
#include <set>

#include "Data.h"
#include "Entity.h"
#include "Projectile.h"
#include "Point2D.h"

namespace SpaceSimNS
{

class System;
class Fleet;
class CelestialBody;
class Background;

/**
 * This class represents a sector of space, or a single game area.
 *
 * @author Markus Gemperle
 * @version 23/02/2014
 */
class Sector
{

public:

	/**
	 * Constructor for the class sector.
	 */
	Sector(System* pSystem, Graphics* pGraphics);

	~Sector();


	System* GetSystem() { return m_pSystem; }

	/** Returns the radius of the sector **/
	double GetRadius() { return m_dRadius; }

	void Update(double deltaT);

	void ConstUpdate();

	void HandleCollisions();

	void AddFleets(std::vector<Fleet*>& fleets);

	/**
	 * This method adds the argued entity to the sector.
	 *
	 * @param entity  the entity to be added to the sector
	 */
	void AddEntity(Entity* entity);

	/**
	* This method removes the argued entity from the sector if
	* it is present.
	*
	* @param entity  pointer to the entity to be removed
	*/
	void RemoveEntity(Entity* entity);

	bool ContainsEntity(const Entity* entity);

	void DestroyEntity(Entity* entity);

	const Entity::ConstEntitySet Occupants() const;

	void Render(double interpolation, Space::Point2D<double>& offset);

private:

	// Private instance fields

	Graphics* m_pGraphics;
	System* m_pSystem;
	std::set<Fleet*> m_fleets;
	Entity::EntitySet m_occupants;
	Background* m_pBackground;
	double m_dRadius;

	std::set<Entity*> m_toDelete;
	std::set<Entity*> m_toRemove;

	// Private methods

	void RemoveFleets();
	void DeleteFlaggedEntities();
	void RemoveFlaggedEntities();

	void LoadCelestialBodies(std::set<CelestialBody*>& pBodies);
	void LoadBackground();
};



}

