#pragma once

#include "Data.h"
#include <queue>
#include <set>

namespace SpaceSimNS
{

class VesselController;
class System;

class Fleet
{
public:

	Fleet(Faction inAllegience, System* pSystem);

	~Fleet();

	Faction GetAllegience() { return m_allegience; }

	bool Intact() const;

	void HandleMessage(const Telegram& message);

	void Update();

	void AddMember(VesselController* neoMember);

	void RemoveMember(VesselController* exMember);

	void DestroyMember(VesselController* exMember);

	bool Contains(VesselController* member) const;

	std::set<VesselController*> GetMembers() const;

	std::priority_queue<VesselController*> GetBattleworthy();

	System* GetSystem() const;

	void SetSystem(System* pSystem);

private:

	std::set<VesselController*> m_Members;
	System* m_pSystem;
	Faction m_allegience;

};

}