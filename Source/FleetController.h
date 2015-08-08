#pragma once
#include "Controller.h"

namespace SpaceSimNS
{

class Fleet;

class FleetController : public Controller<Fleet>
{
public:
	FleetController();
	~FleetController();

	void Update(double deltaT) override;

	void SetOrder(int command) override;

private:

	/** Used to establish update frequency **/
	int m_iUpdateCounter;
	int m_iUpdateCounterMax;
	
	/** Target number of ships warped in to sector **/
	int m_iTargetNumInSector;
};

}