#pragma once
#include "AIState.h"
#include "WeaponSlot.h"

namespace SpaceSimNS
{

class TurretState : public AIState < WeaponSlot >
{
public:
	TurretState(WeaponSlot* pTurret);
	~TurretState();

	virtual void Fire();
};

class AttackShipTarget : public TurretState
{
public:
	AttackShipTarget(WeaponSlot* pTurret);
	~AttackShipTarget();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const { return ATTACKSHIPTARGET; }
};

class AttackBestTarget : public TurretState
{
public:
	AttackBestTarget(WeaponSlot* pTurret);
	~AttackBestTarget();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const { return ATTACKBESTTARGET; }

private:

	UINT m_TicksToRetarget;

};

class ManualTurretControl : public TurretState
{
public:
	ManualTurretControl(WeaponSlot* pTurret);
	~ManualTurretControl();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const { return ATTACKBESTTARGET; }
};

class RemainStationary : public AIState < WeaponSlot >
{
public:
	RemainStationary(WeaponSlot* pTurret);
	~RemainStationary();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const { return REMAINSTATIONARY; }
};

class TrackTarget : public AIState < WeaponSlot >
{
public:
	TrackTarget(WeaponSlot* pTurret);
	~TrackTarget();

	void Enter();
	void Execute();
	void Exit();
	void OnMessage(const Telegram& message);
	StateType GetState() const { return TRACKTARGET; }
};


}