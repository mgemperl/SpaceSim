#include "GuidanceStates.hpp"

namespace SpaceSimNS
{

SeekTarget::SeekTarget(GuidedProjectile* pProj) : AIState<ShipAI>(pProj)
{
}


SeekTarget::~SeekTarget()
{
}

void SeekTarget::Enter()
{

}

void SeekTarget::Execute()
{
	if (m_pEntity->Target() == NULL)
	{
		m_pEntity->SetTarget(Vessel::BestTarget(m_pEntity, m_pEntity->GetSector()));
	}

	if (m_pEntity->Target() != NULL)
	{
		m_pEntity->MoveToward(
			m_pEntity->Target()->ProjectCollisionPoint(
									m_pEntity->GetPos(), m_pEntity->GetStats().maxSpeed));
	}

}

void SeekTarget::Exit()
{

}

void SeekTarget::OnMessage(const Telegram& message)
{

}

StateType SeekTarget::GetState() const
{
	return SEEKTARGET;
}

}