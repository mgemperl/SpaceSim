#pragma once

#include "VesselController.h"
#include "StateMachine.h"

namespace SpaceSimNS
{

class GuidedProjectileAI : public VesselController
{
public:
	GuidedProjectileAI();
	~GuidedProjectileAI();
	
	void UpdateActions() override;

	virtual void HandleMessage(const Telegram& message) override;

	void Update(double deltaT) override;

private:

	StateMachine<GuidedProjectileAI>* m_pAIStateMachine;

};

}
