#include "GuidanceStates.h"

namespace SpaceSimNS
{

SeekTarget::SeekTarget(GuidedProjectileAI* pProj) : 
	AIState<GuidedProjectileAI>(pProj)
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
	Vessel* pProj = m_pEntity->GetEntity();

	if (pProj->GetTarget() == NULL)
	{
		pProj->SetTarget(
			Vessel::BestTarget(pProj, pProj->GetSector()));
	}

	if (pProj->GetTarget() != NULL)
	{
		pProj->MoveToward(
			pProj->GetTarget()->ProjectCollisionPoint(
				pProj->GetPos(), pProj->GetStats().maxSpeed));
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