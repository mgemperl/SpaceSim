#include "Universe.h"
#include "Graphics.h"
#include "Input.h"
#include "System.h"
#include "Sector.h"
#include "Player.h"
#include "AIShipOrderStates.h"
#include "MessageDispatcher.h"
#include "StateMachine.h"
#include "GameState.h"
#include "Fleet.h"
#include "CelestialBody.h"
#include "DefinitionManager.h"
#include "Data.h"

using namespace Space;

namespace SpaceSimNS
{

Universe::Universe()
{
	m_pGameStateMachine = new StateMachine<Universe>(this);
}

Universe::~Universe()
{
}

void Universe::Initialize(Graphics* pGraphics, Input* pInput)
{
	m_pGraphics = pGraphics;
	m_pInput = pInput;
	MessageDispatcher::Instance()->Initialize();
	DefinitionManager::Instance()->Initialize();
	DefinitionManager* pDefMan = DefinitionManager::Instance();

	System* testSystem = new System(50);
	
	m_pPlayer = new Player(m_pInput, testSystem);
	Vessel* playerVessel = new Vessel("Destroyer of Worlds", PLAYERFACTION, m_pPlayer);
	Fleet* playerFleet = m_pPlayer->GetFleet();

	testSystem->AddFleet(playerFleet);

	CelestialBody* planet = new CelestialBody(5, testSystem, 0, 0);

	WeapDef weapon = WeapDef("weapon", 10, SMALL, false, 50, 10, 10, 30);
	WeapDef turret = WeapDef("weapon", 10, SMALL, false, 50, 10, 10, 30);
	WeapDef guided = WeapDef("guided", 10, SMALL, true, 25, 10, 10, 360);

	playerVessel->InstallWeapon(guided, 0);
	//playerVessel->InstallWeapon(guided, 1);
	playerVessel->InstallWeapon(guided, 1);
	//playerVessel->InstallWeapon(guided, 3);


	Fleet* pirateFleet = new Fleet(ENEMYFACTION, testSystem);
	testSystem->AddFleet(pirateFleet);

	ShipAI* enemy = new ShipAI(pirateFleet);
	Vessel* enemyVessel = new Vessel("enemy", ENEMYFACTION, enemy);

//	ShipAI* friend1 = new ShipAI(friendDef, playerFleet);

	enemy->SetOrder(SEARCHANDDESTROY);
	enemyVessel->InstallWeapon(guided, 0);

//	friend1->SetOrder(SEARCHANDDESTROY);
//	friend1->InstallWeapon(weapon, 0);

	testSystem->AddFleet(playerFleet);

	m_Occupants.emplace(playerFleet);
	m_Occupants.emplace(pirateFleet);
	m_Universe.emplace(testSystem);

	SetGameState(ENCOUNTER);
}

/*
LRESULT Universe::HandleUserInput(m)
{
	return m_pGameStateMachine->CurrentState()->HandleUserInput(message, wParam, lParam);
}
*/

void Universe::UpdateTime(double elapsedTime)
{
	static_cast<GameState*>(m_pGameStateMachine->CurrentState())->UpdateTime(elapsedTime);
}

void Universe::Update(double elapsedTime)
{
	static_cast<GameState*>(m_pGameStateMachine->CurrentState())->Update(elapsedTime);
}

void Universe::Render()
{
	static_cast<GameState*>(m_pGameStateMachine->CurrentState())->Render();
}

void Universe::SetGameState(StateType state)
{
	if (state == ENCOUNTER)
	{
		System* pSystem = m_pPlayer->GetFleet()->GetSystem();
		Sector* pSector = new Sector(pSystem, m_pGraphics);

		std::vector<Fleet*> fleets;
		
		for (Fleet* fleet : pSystem->GetFleets())
		{
			fleets.push_back(fleet);
		}

		GameState* gameState = new EncounterGameState(this, m_pPlayer, pSector, fleets);
		m_pGameStateMachine->ChangeState(gameState);
	}
}

}