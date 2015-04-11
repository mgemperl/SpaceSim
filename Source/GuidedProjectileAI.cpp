#include "GuidedProjectileAI.h"
#include "GuidanceStates.h"

namespace SpaceSimNS
{

GuidedProjectileAI::GuidedProjectileAI()
{
	m_pAIStateMachine = new StateMachine<GuidedProjectileAI>(this);
	m_pAIStateMachine->SetState(new SeekTarget(this));
}

GuidedProjectileAI::~GuidedProjectileAI()
{
	delete m_pAIStateMachine;
}

void GuidedProjectileAI::UpdateActions()
{
	m_pEntity->StopAll();
	m_pAIStateMachine->Update();
}

void GuidedProjectileAI::HandleMessage(const Telegram& message)
{

}

void GuidedProjectileAI::Update(double deltaT)
{

}

}