#include "EntityManager.h"

namespace SpaceSimNS
{

	void EntityManager::RegisterEntity(Entity* pEntity)
	{
		if (!m_EntityMap.count(pEntity->GetID()))
		{
			m_EntityMap.emplace(pEntity->GetID(), pEntity);
		}
	}

	Entity* EntityManager::GetEntityFromID(Entity::ID id)
	{
		return m_EntityMap[id];
	}
	
	void EntityManager::RemoveEntity(Entity::ID id)
	{
		m_EntityMap.erase(id);
	}

	EntityManager* EntityManager::Instance()
	{
		static EntityManager manager;
		return &manager;
	}

}