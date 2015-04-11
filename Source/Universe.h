#pragma once

#include "StateMachine.h"
#include <set>
#include <functional>
#include <iterator>
#include <string.h>

class Graphics;
class Input;

namespace SpaceSimNS
{
	class System;
	class Sector;
	class Player;
	class ShipAI;
	class Fleet;

class Universe
{
public:

	Universe();
	~Universe();

	typedef unsigned long TICK;
	void Initialize(Graphics* pGraphics, Input* pInput);

	void AddFleet(){};

	void RemoveFleet(){};

	void UpdateTime(double elapsedTime);

	// Handle user input, update player sector
	void Update(double elapsedTime);

	void ConstUpdate();

	void SetGameState(StateType state);

	void Render();

private:

	Graphics* m_pGraphics;
	Input* m_pInput;

	std::set<System*> m_Universe;
	
	std::set<Fleet*> m_Occupants;

	Player* m_pPlayer;

	StateMachine<Universe>* m_pGameStateMachine;


	
};

}