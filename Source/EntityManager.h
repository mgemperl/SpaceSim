#pragma once

#include "Entity.h"

namespace SpaceSimNS
{

class EntityManager
{
public:

	static EntityManager* Instance();

	void RegisterEntity(Entity* pEntity);

	Entity* GetEntityFromID(Entity::ID id);

	void RemoveEntity(Entity::ID id);

private:

	typedef std::unordered_map<unsigned int, Entity*> EntityMap;
	EntityMap m_EntityMap;

	static Entity::ID nextValidID;

	EntityManager(){}
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);

};

}