#pragma once

#include <unordered_map>
#include "Data.h"

namespace SpaceSimNS
{

class DefinitionManager
{

public:

	static DefinitionManager* Instance()
	{
		static DefinitionManager defMan;
		return &defMan;
	}

	const ShipDef* GetShipDef(const char* name) const
	{
		const ShipDef* pDef = NULL;

		if (m_shipDefs.count(name) > 0)
		{
			pDef = &m_shipDefs.at(name);
		}

		return pDef;
	}

	const CompDef* GetCompDef(const char* name) const
	{
		const CompDef* pDef = NULL;

		if (m_compDefs.count(name) > 0)
		{
			pDef = &m_compDefs.at(name);
		}

		return pDef;
	}

	const WeapDef* GetWeapDef(const char* name) const
	{
		const WeapDef* pDef = NULL;

		if (m_weapDefs.count(name) > 0)
		{
			pDef = &m_weapDefs.at(name);
		}

		return pDef;
	}

	bool AddShipDef(ShipDef& def)
	{
		return m_shipDefs.emplace(def.name, def).second;
	}

	bool AddCompDef(CompDef& def)
	{
		return m_compDefs.emplace("", def).second;
	}

	bool AddWeapDef(WeapDef& def)
	{
		return m_weapDefs.emplace(def.name, def).second;
	}

	void Initialize();

private:

	DefinitionManager();
	~DefinitionManager();

	void LoadDefinitions();

	std::unordered_map<const char*, ShipDef> m_shipDefs;
	std::unordered_map<const char*, CompDef> m_compDefs;
	std::unordered_map<const char*, WeapDef> m_weapDefs;

};

}