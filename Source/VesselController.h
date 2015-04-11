#pragma once

#include "Controller.h"
#include "Vessel.h"

namespace SpaceSimNS
{

class Fleet;

class VesselController : public Controller<Vessel>
{
public:

	VesselController();
	~VesselController();

	virtual void UpdateActions() = 0;

	virtual void HandleMessage(const Telegram& message) = 0;

protected:


};



}