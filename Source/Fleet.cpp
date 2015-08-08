#include "Fleet.h"
#include "System.h"
#include "FleetController.h"
#include "Sector.h"
#include "Vessel.h"
#include "VesselController.h"
#include "Vector2D.h"
#include "Data.h"

using namespace Space;

namespace SpaceSimNS
{

Fleet::Fleet(Faction inAlleg, System* pSystem)
{
	m_allegience = inAlleg;

	if (pSystem == NULL)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR, "Set fleet to be in NULL system");
	}

	m_iNumInSector = 0;
	m_dWarpinAngle = 0;
	m_dWarpinOffset = 0;
	m_dMaxWarpinOffset = SIXTEENTH_PI;
	pSystem->AddFleet(this);
	m_pSystem = pSystem;
	m_combatants = std::priority_queue<VesselController*>();
	m_pController = NULL;
}

Fleet::~Fleet()
{
	delete m_pController;
}

bool Fleet::Intact() const
{
	return !m_members.empty();
}

void Fleet::SetController(FleetController* pCont)
{
	if (m_pController != NULL)
	{
		m_pController->Unassign();
	}

	m_pController = pCont;

	if (pCont != NULL)
	{
		pCont->Assign(this);
	}
}

void Fleet::HandleMessage(const Telegram& message)
{
	
}

void Fleet::Update(double deltaT)
{
	if (m_pController != NULL)
	{
		m_pController->Update(deltaT);
	}
}

void Fleet::AddMember(VesselController* neoMember)
{
	m_members.emplace(neoMember);
}

void Fleet::RemoveMember(VesselController* exMember)
{
	m_members.erase(exMember);
}

void Fleet::DestroyMember(VesselController* exMember)
{
	RemoveMember(exMember);
}

bool Fleet::Contains(VesselController* member) const
{
	std::set<VesselController*>::const_iterator itr = m_members.find(member);
	return (itr != m_members.end());
}

std::set<VesselController*> Fleet::GetMembers() const
{
	return m_members;
}

std::priority_queue<VesselController*> Fleet::GetBattleworthy()
{
	std::priority_queue<VesselController*> battleworthy;

	for (VesselController* ship : m_members)
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
	DestroyMember(NULL);
	
}

void Fleet::EnterSector(Sector* pSector, double warpinAngle)
{
	m_iNumInSector = 0;
	m_dWarpinOffset = 0;

	if (pSector == NULL)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR, "Set fleet to be in NULL sector");
	}

	m_pSector = pSector;
	m_dWarpinAngle = warpinAngle;

	// Add fleet's combat-ready vessels to a queue
	m_combatants = std::priority_queue<VesselController*>();
	for (VesselController* pCont : m_members)
	{
		m_combatants.push(pCont);
	}
}

void Fleet::WarpInNextShip()
{
	if (m_pSector != NULL && !m_combatants.empty())
	{
		VesselController* shipCont = m_combatants.top();
		m_combatants.pop();
		Vessel* ship = shipCont->GetEntity();

		// Establish vector at which the ship will enter the system
		Vector2D entryVector = 
			Vector2D(m_dWarpinAngle + m_dWarpinOffset, m_pSector->GetRadius());

		m_dWarpinOffset = m_dWarpinOffset < m_dMaxWarpinOffset ? 
			m_dWarpinOffset + 0.1 : -m_dMaxWarpinOffset;

		ship->SetPosition(entryVector.GetTerminal());
		ship->SetOrientation(entryVector.GetAngle() + PI);
		ship->SetVelocity(
			Vector2D(entryVector.GetAngle() + PI, ship->GetStats().maxSpeed));

		ship->GetController()->SetOrder(SEARCHANDDESTROY);
		m_pSector->AddEntity(ship);
		m_iNumInSector++;
	}
}

void Fleet::ExitSector()
{
	m_pSector = NULL;
	m_iNumInSector = 0;
	m_dWarpinOffset = 0;
	m_dWarpinAngle = 0;
}

}