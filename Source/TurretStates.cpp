#include "TurretStates.h"
#include "Vessel.h"

#define TICKS_BETWEEN_TARGET_ATTEMPTS 10;

namespace SpaceSimNS
{

	/**************************************************/

	TurretState::TurretState(WeaponSlot* pTurret) : AIState<WeaponSlot>(pTurret)
	{

	}

	TurretState::~TurretState()
	{

	}

	void TurretState::Fire()
	{
		if (m_pEntity->m_AimReady)
		{
			if (m_pEntity->GetWeap().isGuided)
			{
				m_pEntity->FireGuided();
			}
			else
			{
				m_pEntity->FireDumb();
			}
		}
		
	}


	/**************************************************/


AttackShipTarget::AttackShipTarget(WeaponSlot* pTurret) : TurretState(pTurret)
{

}

AttackShipTarget::~AttackShipTarget()
{

}

void AttackShipTarget::Enter()
{

}

void AttackShipTarget::Execute()
{
	m_pEntity->SetTarget(m_pEntity->GetShip()->GetTarget());
	Fire();
}

void AttackShipTarget::Exit()
{

}

void AttackShipTarget::OnMessage(const Telegram& message)
{

}


/**************************************************/

AttackBestTarget::AttackBestTarget(WeaponSlot* pTurret) : TurretState(pTurret)
{
	m_TicksToRetarget = 0;
}

AttackBestTarget::~AttackBestTarget()
{

}

void AttackBestTarget::Enter()
{
	m_TicksToRetarget = 0;
}

void AttackBestTarget::Execute()
{
	if (m_pEntity->m_pTarget != NULL)
	{
		if (!m_pEntity->m_TargetInRange || !m_pEntity->m_TargetInArc)
		{
			m_pEntity->SetTarget(NULL);
		}
	}
	
	if (m_TicksToRetarget <= 0)
	{
		if (m_pEntity->m_pTarget == NULL)
		{
			m_TicksToRetarget = TICKS_BETWEEN_TARGET_ATTEMPTS;
			m_pEntity->SetTarget(m_pEntity->BestTarget());
			m_pEntity->UpdateRangeArcAim();
		}
	}
	else
	{
		m_TicksToRetarget--;
	}
	
	Fire();
}

void AttackBestTarget::Exit()
{

}

void AttackBestTarget::OnMessage(const Telegram& message)
{

}

/**************************************************/

ManualTurretControl::ManualTurretControl(WeaponSlot* pTurret) : TurretState(pTurret)
{

}

ManualTurretControl::~ManualTurretControl()
{

}

void ManualTurretControl::Enter()
{

}

void ManualTurretControl::Execute()
{
	m_pEntity->SetTarget(m_pEntity->GetShip()->GetTarget());
}

void ManualTurretControl::Exit()
{

}

void ManualTurretControl::OnMessage(const Telegram& message)
{

}

/**************************************************/

RemainStationary::RemainStationary(WeaponSlot* pTurret) : AIState<WeaponSlot>(pTurret)
{

}

RemainStationary::~RemainStationary()
{

}

void RemainStationary::Enter()
{

}

void RemainStationary::Execute()
{
	m_pEntity->SetFacing(m_pEntity->m_DefaultFacing);
}

void RemainStationary::Exit()
{

}

void RemainStationary::OnMessage(const Telegram& message)
{

}

/**************************************************/

TrackTarget::TrackTarget(WeaponSlot* pTurret) : AIState<WeaponSlot>(pTurret)
{

}

TrackTarget::~TrackTarget()
{

}

void TrackTarget::Enter()
{

}

void TrackTarget::Execute()
{
	if (m_pEntity->m_pTarget != NULL)
	{
		m_pEntity->TurnTo(m_pEntity->m_TargetAngle);
	}
	else
	{
		m_pEntity->TurnTo(m_pEntity->GetShip()->GetOrientationRad() + m_pEntity->m_DefaultFacing);
	}
}

void TrackTarget::Exit()
{

}

void TrackTarget::OnMessage(const Telegram& message)
{

}

}