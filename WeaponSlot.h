#pragma once

#include "Vector2D.h"
#include "Entity.h"
#include "Projectile.h"
#include "Drawable.h"
#include "Data.h"
#include "StateMachine.h"

namespace SpaceSimNS
{

class Vessel;

class WeaponSlot : public Entity
{
	friend class AttackShipTarget;
	friend class AttackBestTarget;
	friend class ManualTurretControl;
	friend class TurretState;
	friend class RemainStationary;
	friend class TrackTarget;

public:

	WeaponSlot(const SlotDef& def, const Vessel* ship);

	~WeaponSlot();

	bool InitializeDrawable(Graphics* pGraphics) override;
	
	Space::Point2D<double> GetInterpPos(double interpolation) const override;

	bool InstallWeapon(WeapDef& weapon);

	void UninstallWeapon();

	bool WeaponInstalled() const;

	const Vessel* GetShip() const { return m_pShip; }

	const WeapDef& GetWeap() const { return m_WeapDef; }

	double Arc() const;

	double Range() const;

	UINT Cooldown() const;

	bool IsInRange(const Entity* pEntity) const;

	bool IsInArc(const Entity* pEntity) const;

	bool TargetInRange() const;

	bool TargetInArc() const;

	const Vessel* Target() const;

	void SetTarget(const Vessel* pTarget);

	void Fire();

	void PhysUpdate() override;

	virtual void HandleMessage(const Telegram& message) override;

	bool Intact() const override;

	//virtual bool Turreted() const { return false; }

	void SetTargetingOrder(StateType order);

	const Vessel* BestTarget() const;

protected:

	void FireDumb();
	void FireGuided();

	void SetFacing(double theta);

	bool Facing(const Space::Point2D<double>& point) const;
	bool Facing(double theta) const;

	bool TurnTo(const Entity* entity);
	bool TurnTo(const Space::Point2D<double>& dest);
	bool TurnTo(double theta);

private:

	const Vessel* m_pShip;
	Space::Vector2D m_RelativePos;

	bool m_WeaponInstalled;
	WeapDef m_WeapDef;

	

	WeaponSize m_Size;
	double m_DefaultFacing;
	double m_RelativeFacing;
	double m_FiringArc;
	double m_BufferArc;
	double m_FiringRange;
	bool m_TargetInRange;
	bool m_TargetInArc;
	double m_TargetAngle;
	bool m_AimReady;

	double m_SecondsUntilReload;
	const Vessel* m_pTarget;
	void UpdateTargetAngle();

	void TurnFacing(double theta);
	void UpdatePos();
	void UpdateOrientation();
	void UpdateRangeArcAim();

	double m_TurnRate;

	StateMachine<WeaponSlot>* m_pTargetingStateMachine;
	StateMachine<WeaponSlot>* m_pTrackingStateMachine;

	static double GetTurnRateFromSize(WeaponSize size);
	// static Space::Ellipse GetEllipseFromSlotDef(const SlotDef& def);

	

	

};

}