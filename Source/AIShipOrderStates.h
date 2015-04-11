#pragma once

#include "AIState.h"
#include <list>
#include <unordered_map>

namespace SpaceSimNS
{

enum Action;
class ShipAI;
class Vessel;

template <class entity_type>
class StateMachine;

class SearchAndDestroy : public AIState <ShipAI>
{
public:

	SearchAndDestroy(ShipAI* pController);
	~SearchAndDestroy();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const;

private:

	// Note: m_pEntity points to the controller, not the
	// controller's entity

	/** pointer to the controller's entity **/
	Vessel* m_pContEntity;
	StateMachine<Vessel>* m_pManeuverStateMachine;

};

class Standby : public AIState < ShipAI >
{
public:
	Standby(ShipAI* pController);
	~Standby();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const;

private:

	/** pointer to the controller's entity **/
	Vessel* m_pContEntity;
	StateMachine<Vessel>* m_pManeuverStateMachine;

};

/*
class ManualControl : public AIState < ShipAI >
{

public:
	typedef std::unordered_map<UINT, Action> ControlMap;
	
	ManualControl(ShipAI* pController, const ControlMap* pControlMap);
	~ManualControl();

	void Enter();
	void Execute();
	void Exit();

	LRESULT HandleUserInput(UINT message, WPARAM wParam, LPARAM lParam);
	void OnMessage(const Telegram& message);
	StateType GetState() const;

	std::list<WPARAM> m_KeysPressed;
	const ControlMap* m_pControlMap;
};
*/

}