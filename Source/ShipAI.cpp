#include "ShipAI.h"

#include "StateMachine.h"
#include "AIShipOrderStates.h"
#include "Fleet.h"

#include <array>
#include <iterator>

using namespace Space;

namespace SpaceSimNS
{

ShipAI::ShipAI(Fleet* pFleet) : VesselController()
{
	m_pAIStateMachine = new StateMachine<ShipAI>(this);
	m_pAIStateMachine->SetState(new Standby(this));

	m_pFleet = pFleet;

	if (pFleet != NULL)
	{
		pFleet->AddMember(this);
	}
}

ShipAI::~ShipAI()
{
	delete m_pAIStateMachine;
}

void ShipAI::SetFleet(Fleet* pFleet)
{
	if (m_pFleet != NULL)
	{
		m_pFleet->RemoveMember(this);
	}

	if (pFleet != NULL)
	{
		pFleet->AddMember(this);
	}

	m_pFleet = pFleet;
}

void ShipAI::SetOrder(int inOrder)
{
	switch (inOrder)
	{
	case SEARCHANDDESTROY: 
		m_pAIStateMachine->ChangeState(new SearchAndDestroy(this));
		break;
	case STANDBY:
		m_pAIStateMachine->ChangeState(new Standby(this));
		break;
	}
}

void ShipAI::HandleMessage(const Telegram& message)
{

}

void ShipAI::Update(double deltaT)
{

}

void ShipAI::UpdateActions()
{
	m_pEntity->StopAll();
	m_pAIStateMachine->Update();
}

}