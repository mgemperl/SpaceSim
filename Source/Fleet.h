#pragma once

#include "Data.h"
#include <queue>
#include <set>

namespace SpaceSimNS
{

class VesselController;
class System;
class Sector;
class FleetController;

class Fleet
{
public:

	Fleet(Faction inAllegience, System* pSystem);

	~Fleet();

	Faction GetAllegience() { return m_allegience; }

	bool Intact() const;

	void SetController(FleetController* pController);

	FleetController* GetController() { return m_pController; }

	void HandleMessage(const Telegram& message);

	void Update(double deltaT);

	void AddMember(VesselController* neoMember);

	void RemoveMember(VesselController* exMember);

	void DestroyMember(VesselController* exMember);

	bool Contains(VesselController* member) const;

	std::set<VesselController*> GetMembers() const;

	std::priority_queue<VesselController*> GetBattleworthy();

	System* GetSystem() const;

	void SetSystem(System* pSystem);

	void EnterSector(Sector* pSector, double warpinAngle);

	void ExitSector();

	int NumShipsInSector() { return m_iNumInSector; }
	void WarpInNextShip();

private:

	std::set<VesselController*> m_members;
	/** System in which this fleet is present **/
	System* m_pSystem;
	Faction m_allegience;
	FleetController* m_pController;

	// Sector Encounter Stuff

	/** Sector in which the fleet is present during an encounter 
		Should be NULL outside an encounter **/
	Sector* m_pSector;

	/** Combat-capable ships in the fleet queued up to join the encounter **/
	std::priority_queue<VesselController*> m_combatants;

	/** Angle at which the fleet is warping into the sector **/
	double m_dWarpinAngle;

	/** Number of ships currently warped in to the sector **/
	int m_iNumInSector;
	

	/** Angle from warpin angle at which ships are currently being warped in **/
	double m_dWarpinOffset;
	double m_dMaxWarpinOffset;
};

}