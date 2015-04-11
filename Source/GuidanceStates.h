#pragma once
#include "AIState.h"
#include "GuidedProjectileAI.h"

namespace SpaceSimNS
{

class SeekTarget : public AIState <GuidedProjectileAI>
{
public:
	SeekTarget(GuidedProjectileAI* pProj);
	~SeekTarget();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const;
};


}