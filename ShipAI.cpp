#include "ShipAI.h"

#include "StateMachine.h"
#include "AIShipOrderStates.h"
#include "Fleet.h"

#include <array>
#include <iterator>

using namespace Space;

namespace SpaceSimNS
{

ShipAI::ShipAI(Fleet* pFleet) : VesselController(pFleet)
{
	m_pAIStateMachine = new StateMachine<ShipAI>(this);
	m_pAIStateMachine->SetState(new Standby(this));
}

ShipAI::~ShipAI()
{
	delete m_pAIStateMachine;
}

void ShipAI::SetOrder(StateType inOrder)
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

// Update AI Actions

void ShipAI::UpdateActions()
{
	m_pEntity->StopAll();
	m_pAIStateMachine->Update();
}

}