#pragma once

#include "ShipAI.h"

class Input;

#include <set>
#include <map>
#include <string>


namespace SpaceSimNS
{

	typedef std::unordered_map<UINT, Action> ControlMap;

class Player : public VesselController
{
public:
	Player(Input* pInput, Fleet* pPlayerFleet);
	~Player();

	void UpdateActions();

	inline ClassType GetClass() const { return PLAYER; }

	void HandleMessage(const Telegram& telegram) override;

	void Update(double deltaT) override;

	Vessel* GetVessel() const { return m_pPlayerVessel; }

	void SetVessel(Vessel* pVessel);

private:

	Vessel* m_pPlayerVessel;
	ControlMap m_pControlMap;
	Input* m_pInput;

};

}