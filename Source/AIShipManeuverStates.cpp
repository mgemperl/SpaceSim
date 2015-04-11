#include "StateMachine.h"
#include "AIShipManeuverStates.h"

#include "Vessel.h"
#include "Vector2D.h"

#include <array>

using namespace Space;

namespace SpaceSimNS
{
	/*********************** Strafe Approach ***************************/

	StrafeApproach::StrafeApproach(
		Vessel* ship, 
		StateMachine<Vessel>* pManeuverSM
		) : AIState<Vessel>(ship)
	{
		m_pManeuverSM = pManeuverSM;
	}

	void StrafeApproach::Enter(){}

	void StrafeApproach::Execute()
	{
		const Vessel* target = m_pEntity->GetTarget();

		if (target != NULL)
		{
			Space::Point2D<double> targetPoint = target->GetPos();
			m_pEntity->MoveToward(target);

			if (m_pEntity->IsInFiringRange())
			{
				m_pEntity->DoAction(FIRE);
			}
			else
			{
				m_pEntity->StopAction(FIRE);
			}

			double dist = m_pEntity->GetPos().Distance(
				m_pEntity->GetTarget()->GetPos());

			if (dist < m_pEntity->Range() / 2.5)
			{
				m_pManeuverSM->ChangeState(new Break(m_pEntity, m_pManeuverSM));
			}
		}
	}

	void StrafeApproach::Exit(){}

	void StrafeApproach::OnMessage(const Telegram& message) {}

	StateType StrafeApproach::GetState() const { return StateType::STRAFEAPPROACH; }

	/*********************** Break ***************************/

	Break::Break(
		Vessel* ship,
		StateMachine<Vessel>* pManeuverSM) : AIState<Vessel>(ship)
	{
		m_pManeuverSM = pManeuverSM;
	}

	void Break::Enter()
	{
		char sign = RANDSIGN;
		
		targetDir = Vector2D::SimplifyAngle(
			m_pEntity->GetVelocity().GetAngle() + 
			sign * QUARTER_PI + sign * QUARTER_PI * RANDOM);

	}

	void Break::Execute()
	{
		if (m_pEntity->MovingToward(targetDir))
		{
			const Vessel* target = m_pEntity->GetTarget();

			if (target != NULL)
			{
				m_pEntity->TurnTo(target);

				if (m_pEntity->IsInFiringRange())
				{
					m_pEntity->DoAction(FIRE);
				}
				else
				{
					m_pEntity->StopAction(FIRE);
				}
			}

			double dist = m_pEntity->GetPos().Distance(
				m_pEntity->GetTarget()->GetPos());

			if (dist > m_pEntity->Range())
			{
				m_pEntity->SetTarget(Vessel::BestTarget(m_pEntity, m_pEntity->GetSector()));
				m_pManeuverSM->ChangeState(new StrafeApproach(m_pEntity, m_pManeuverSM));
			}
		}
		else
		{
			m_pEntity->MoveToward(targetDir);
		}
	}

	void Break::Exit(){}

	void Break::OnMessage(const Telegram& message) {}

	StateType Break::GetState() const { return StateType::BREAK; }



	/**************************************************/

	Idle::Idle(Vessel* ship, 
		StateMachine<Vessel>* pManeuverSM) : AIState<Vessel>(ship)
	{
		m_pManeuverSM = pManeuverSM;
	}

	void Idle::Enter(){}

	void Idle::Execute(){}

	void Idle::Exit(){}

	void Idle::OnMessage(const Telegram& message){}

	StateType Idle::GetState() const { return IDLE; }

}