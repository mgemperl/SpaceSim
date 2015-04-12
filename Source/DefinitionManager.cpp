#include "DefinitionManager.h"

namespace SpaceSimNS
{

DefinitionManager::DefinitionManager()
{
}


DefinitionManager::~DefinitionManager()
{
}

void DefinitionManager::Initialize()
{
	// Player ship
	ShipDef def = ShipDef("Destroyer of Worlds", "ship", FRIGATE, 
		ShipStats(100, 100, 100, 100, 0.1, 0.06, 0.08, 0.05, 12, 10, 10), PLAYERFACTION);
	def.weapSlots.emplace_back(SlotDef(PI + HALF_PI, 6, SMALL, false, 0, QUARTER_PI, QUARTER_PI));
	def.weapSlots.emplace_back(SlotDef(HALF_PI, 6, SMALL, false, 0, QUARTER_PI, QUARTER_PI));
/*
	double angle = 0;

	playerDef.weapSlots.emplace_back(SlotDef(angle, 15, SMALL, true, angle, PI + QUARTER_PI, EIGHTH_PI));
	angle +=  HALF_PI;
	playerDef.weapSlots.emplace_back(SlotDef(angle, 8, SMALL, false, angle, TWO_PI, EIGHTH_PI));
	angle += HALF_PI;
	playerDef.weapSlots.emplace_back(SlotDef(angle, 15, SMALL, true, angle, PI + QUARTER_PI, EIGHTH_PI));
	angle += HALF_PI;
	playerDef.weapSlots.emplace_back(SlotDef(angle, 8, SMALL, false, angle, TWO_PI, EIGHTH_PI));
*/
	AddShipDef(def);

	// Enemy ship
	def = ShipDef("enemy", "ship", FIGHTER, ShipStats(100, 100, 100, 100, 0.15, 0.1, 0.15, 0.05, 16, 10, 10), ENEMYFACTION);
	def.weapSlots.emplace_back(SlotDef(PI + HALF_PI, 6, SMALL, false, 0, QUARTER_PI, QUARTER_PI));
	AddShipDef(def);

	// Friend ship
	def = ShipDef("friend", "ship", FIGHTER, ShipStats(100, 100, 100, 100, 0.15, 0.1, 0.15, 0.05, 16, 10, 10), PLAYERFACTION);
	def.weapSlots.emplace_back(SlotDef(PI + HALF_PI, 6, SMALL, false, 0, QUARTER_PI, QUARTER_PI));
	AddShipDef(def);

	// Missile
	def = ShipDef("boom", "missile", MISSILE, ShipStats(100, 100, 100, 100, 0.3, 0, 0, 0.15, 35, 0, 0), PLAYERFACTION);
	AddShipDef(def);
}

void DefinitionManager::LoadDefinitions()
{

}

}