#include "AIShipOrderStates.h"
#include "AIShipManeuverStates.h"
#include "StateMachine.h"
#include "ShipAI.h"
#include "Vessel.h"
#include "Player.h"
#include <unordered_map>

namespace SpaceSimNS
{
	/**************************************************/

	SearchAndDestroy::SearchAndDestroy(ShipAI* ship) : AIState<ShipAI>(ship)
	{
		m_pContEntity = m_pEntity->GetEntity();
		m_pManeuverStateMachine = new StateMachine<Vessel>(m_pContEntity);
		m_pManeuverStateMachine->SetState(new Idle(m_pContEntity, 
			m_pManeuverStateMachine));
	}

	SearchAndDestroy::~SearchAndDestroy()
	{
		delete m_pManeuverStateMachine;
	}

	void SearchAndDestroy::Enter() {}

	void SearchAndDestroy::Execute()
	{
		if (!m_pContEntity->VerifyTarget())
		{
			m_pContEntity->SetTarget(Vessel::BestTarget(m_pContEntity, 
				m_pContEntity->GetSector()));
		}

		if (m_pEntity->GetEntity()->VerifyTarget())
		{
			StateType maneuver = m_pManeuverStateMachine->CurrentState()->GetState();

			if (maneuver != STRAFEAPPROACH && maneuver != BREAK)
			{
				m_pManeuverStateMachine->ChangeState(
					new StrafeApproach(m_pContEntity, m_pManeuverStateMachine));
			}

			m_pManeuverStateMachine->Update();
		}
		else
		{
			m_pManeuverStateMachine->ChangeState(
				new Idle(m_pContEntity, m_pManeuverStateMachine));
		}
	}

	void SearchAndDestroy::Exit() {}

	void SearchAndDestroy::OnMessage(const Telegram& message) {}

	StateType SearchAndDestroy::GetState() const { return StateType::SEARCHANDDESTROY; }

	/**************************************************/

	Standby::Standby(ShipAI* ship) : AIState<ShipAI>(ship)
	{
		m_pContEntity = m_pEntity->GetEntity();
		m_pManeuverStateMachine = new StateMachine<Vessel>(m_pContEntity);
	}

	Standby::~Standby()
	{
		delete m_pManeuverStateMachine;
	}

	void Standby::Enter()
	{
		m_pManeuverStateMachine->ChangeState(
			new Idle(m_pContEntity, m_pManeuverStateMachine));
	}

	void Standby::Execute()
	{
		m_pManeuverStateMachine->Update();
	}

	void Standby::Exit(){}

	void Standby::OnMessage(const Telegram& message){}

	StateType Standby::GetState() const { return STANDBY; }

	/**************************************************/

/*
	ManualControl::ManualControl(ShipAI* ship, const ControlMap* pControlMap) : AIState<ShipAI>(ship) 
	{
		m_pControlMap = pControlMap;
	}

	ManualControl::~ManualControl() {}

	void ManualControl::Enter()
	{
		m_pEntity->SetTurnWidth(m_pEntity->GetStats().turnRate);
	}

	void ManualControl::Execute()
	{
		std::list<WPARAM>::iterator itr = m_KeysPressed.begin();
		std::list<WPARAM>::iterator end = m_KeysPressed.end();
		while (itr != end)
		{

			if (!(GetKeyState(*itr) & 0x8000))
			{
				m_pEntity->StopAction(m_pControlMap->at(*itr));
				m_KeysPressed.erase(itr++);
			}
			else
			{
				m_pEntity->DoAction(m_pControlMap->at(*itr));
				itr++;
			}
		}
	}

	void ManualControl::Exit(){}

	LRESULT ManualControl::HandleUserInput(UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = S_OK;

		if (m_pControlMap->count(wParam))
		{
			m_KeysPressed.push_back(wParam);
			m_KeysPressed.unique();
		}

		return result;
	}

	void ManualControl::OnMessage(const Telegram& message){}

	StateType ManualControl::GetState() const { return StateType::MANUALCONTROL; }
*/

	/**************************************************/

}