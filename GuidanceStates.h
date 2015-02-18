#pragma once
#include "AIState.h"
#include "GuidedProjectile.h"
#include "Controller.h"

namespace SpaceSimNS
{

class GuidanceController : public Controller < GuidedProjectile >
{

};

class SeekTarget : public AIState <GuidedProjectile>
{
public:
	SeekTarget(GuidedProjectile* pProj);
	~SeekTarget();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const;
};


}