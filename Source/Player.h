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
	Player(Input* pInput, System* pSystem);
	~Player();

	void UpdateActions();

	Fleet* GetFleet() { return m_pPlayerFleet; }

	inline ClassType GetClass() const { return PLAYER; }

	void HandleMessage(const Telegram& telegram) override;

	void Update(double deltaT) override;

	Vessel* GetVessel() const { return m_pPlayerVessel; }

	void SetVessel(Vessel* pVessel);

	void SetOrder(int command) override {}

private:

	Vessel* m_pPlayerVessel;
	Fleet* m_pPlayerFleet;
	ControlMap m_pControlMap;
	Input* m_pInput;

};

}