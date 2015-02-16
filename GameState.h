#pragma once

#include "State.h"
#include "Entity.h"
#include "Data.h"

class Input;

namespace SpaceSimNS
{

class Universe;
class Player;
class Sector;
class Fleet;

class GameState : public State < Universe >
{

public:
	GameState(Universe* universe) :
		State<Universe>(universe){}

	virtual ~GameState() {};

	virtual void UpdateTime(double elapsedTime) = 0;
	virtual void Update(double elapsedTime) = 0;
	virtual void Render() = 0;

protected:

	typedef unsigned long TICK;
	TICK m_TickCount;

};

/**************************************************/

class EncounterGameState : public GameState
{
public:

	EncounterGameState(Universe* universe, 
		Player* pPlayer, 
		Sector* pEncounterSector,
		const std::vector<Fleet*>& participants);

	~EncounterGameState();

	void Enter();

	void Execute();

	StateType GetState() const;

	void Exit();

	void UpdateTime(double elapsedTime) override 
	{ 
		m_dTimeSinceConstUpdate += elapsedTime; 
	}

	void Update(double elapsedTime) override;
	
	void Render() override;

private:

	Player* m_pPlayer;
	Sector* m_pEncounterSector;
	std::vector<Fleet*> m_Participants;
	double m_dTimeSinceConstUpdate;

};

/**************************************************/
/*
class CampaignGameState : public GameState
{
public:
	
	CampaignGameState(Universe* universe);
	~CampaignGameState();

	void Enter();

	void Execute();

	StateType GetState() const;

	void Exit();

	LRESULT HandleUserInput(
		UINT message, WPARAM wParam, LPARAM lParam);

	virtual HRESULT Render(double interpolation) = 0;

};
*/
}