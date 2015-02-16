#include "VesselController.h"
#include "GameException.h"
#include "Fleet.h"

namespace SpaceSimNS
{

VesselController::VesselController(Fleet* pFleet) : Controller<Vessel>()
{
	if (pFleet == NULL)
	{
		throw(GameException(GameExceptionNS::FATAL_ERROR,
			"Created vessel controller without a fleet"));
	}

	pFleet->AddMember(this);
	m_pFleet = pFleet;
}


VesselController::~VesselController()
{
	m_pFleet->RemoveMember(this);
}

Fleet* VesselController::GetFleet() const
{
	return m_pFleet;
}

void VesselController::SetFleet(Fleet* pNeoFleet)
{
	m_pFleet->RemoveMember(this);
	pNeoFleet->AddMember(this);
	m_pFleet = pNeoFleet;
}

}