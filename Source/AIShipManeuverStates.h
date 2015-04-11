#pragma once

#include "AIState.h"

namespace SpaceSimNS
{

class Vessel;

template <class entity_type>
class StateMachine;

class StrafeApproach : public AIState <Vessel>
{
public:
	StrafeApproach(Vessel* ship, StateMachine<Vessel>* pManeuverSM);
	
	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const;
	
private:
	StateMachine<Vessel>* m_pManeuverSM;

};

class Break : public AIState <Vessel>
{
public:
	Break(Vessel* ship, StateMachine<Vessel>* pManeuverSM);
	
	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const;

private:
	StateMachine<Vessel>* m_pManeuverSM;
	double targetDir;

};

class Idle : public AIState <Vessel>
{
public:
	Idle(Vessel* ship, StateMachine<Vessel>* pManeuverSM);

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const;

private:
	StateMachine<Vessel>* m_pManeuverSM;
};


}