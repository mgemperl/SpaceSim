#pragma once

#include "Controller.h"
#include "Vessel.h"

namespace SpaceSimNS
{

class Fleet;

class VesselController : public Controller<Vessel>
{
public:

	VesselController(Fleet* pFleet);
	~VesselController();

	Fleet* GetFleet() const;

	void SetFleet(Fleet* pNeoFleet);

	virtual void UpdateActions() = 0;

	virtual void HandleMessage(const Telegram& message) = 0;

protected:

	Fleet* m_pFleet;

};



}