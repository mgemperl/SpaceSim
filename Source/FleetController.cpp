#include "FleetController.h"
#include "Fleet.h"

namespace SpaceSimNS
{

FleetController::FleetController()
{
	m_iTargetNumInSector = 3;
}


FleetController::~FleetController()
{
}

void FleetController::Update(double deltaT)
{
	if (m_pEntity->NumShipsInSector() < m_iTargetNumInSector)
	{
		m_pEntity->WarpInNextShip();
	}
}

void FleetController::SetOrder(int command)
{

}

}