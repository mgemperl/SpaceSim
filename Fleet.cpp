#include "Fleet.h"
#include "System.h"
#include "VesselController.h"

namespace SpaceSimNS
{

Fleet::Fleet(Faction inAlleg, System* pSystem)
{
	m_allegience = inAlleg;

	if (pSystem == NULL)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR, "Set fleet to be in NULL system");
	}

	pSystem->AddFleet(this);
	m_pSystem = pSystem;
}

Fleet::~Fleet()
{
}

bool Fleet::Intact() const
{
	return !m_Members.empty();
}

void Fleet::HandleMessage(const Telegram& message)
{

}

void Fleet::AddMember(VesselController* neoMember)
{
	m_Members.emplace(neoMember);
}

void Fleet::RemoveMember(VesselController* exMember)
{
	m_Members.erase(exMember);
}

bool Fleet::Contains(VesselController* member) const
{
	std::set<VesselController*>::const_iterator itr = m_Members.find(member);
	return (itr != m_Members.end());
}

std::set<VesselController*> Fleet::GetMembers() const
{
	return m_Members;
}

std::priority_queue<VesselController*> Fleet::GetBattleworthy()
{
	std::priority_queue<VesselController*> battleworthy;

	for (VesselController* ship : m_Members)
	{
		battleworthy.push(ship);
	}

	return battleworthy;
}

System* Fleet::GetSystem() const
{
	return m_pSystem;
}

void Fleet::SetSystem(System* pSystem)
{
	if (pSystem == NULL)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR, "Set fleet to be in NULL system");
	}

	m_pSystem->RemoveFleet(this);
	pSystem->AddFleet(this);
	m_pSystem = pSystem;
	
}

}