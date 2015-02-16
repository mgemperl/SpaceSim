#pragma once

#include "VesselController.h"
#include "StateMachine.h"
#include "Fleet.h"

namespace SpaceSimNS
{


class ShipAI : public VesselController
{
public:

	/* Constructor for ShipAI controller, give NULL parameter
	 * for the vessel to not be assigned to a fleet
	 * I may consider disallowing this option later
	 */
	ShipAI(Fleet* pFleet);

	~ShipAI();

	void UpdateActions() override;

	virtual void SetOrder(StateType order);

	virtual void HandleMessage(const Telegram& message) override;

	void Update(double deltaT) override;

protected:

	StateMachine<ShipAI>* m_pAIStateMachine;

private:

	Vessel* leader;
	Fleet* m_pFleet;

};




}