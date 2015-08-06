#pragma once

#include "Moveable.h"
#include <array>
#include <set>
#include <list>

namespace SpaceSimNS
{

class WeaponSlot;
class VesselController;
class Damager;

class Vessel : public Moveable
{
public:

	Vessel(const char* defName,
		Faction allegience,
		VesselController* pCont);

	~Vessel();

	bool InitializeDrawable(Graphics* pGraphics) override;

	// Updaters

	virtual void ConstUpdate() override;

	virtual void Update(double deltaT) override;

	virtual void HandleMessage(const Telegram& message) override;

	virtual void UpdateActions();

	// Accessors

	double Range() const;
	double Arc() const;

	/** Return pointer to this vessel's target, or NULL if it has no target **/
	const Vessel* GetTarget() const;
	
	virtual ClassType GetClass() const override;

	ShipClass GetShipClass() const;

	ShipStats GetStats() const;

	std::vector<const WeaponSlot*> GetWeapons() const;

	// Modifiers

	bool InstallWeapon(WeapDef weapon, int slot);
	bool UninstallWeapon(int slot);

	void SetTarget(const Vessel* pTarget);
	void NextTarget();
	static const Vessel* BestTarget(const Entity* pEnt, const Sector* pSector);

	void SetTurnWidth(double turnWidth);
	void DoAction(Action action);
	void StopAction(Action action);
	void StopAll();

	void AssignController(VesselController* controller);
	void UnassignController();

	// Information

	bool VerifyTarget() const;
	bool Intact() const override;

	bool AtFullSpeed() const;

	bool Facing(const Entity* other) const;
	bool Facing(const Space::Point2D<double>& point) const;
	bool Facing(double theta) const;

	bool MovingToward(const Space::Point2D<double>& point) const;
	bool MovingToward(double theta) const;

	bool FasterThan(const Vessel* other) const; // or equal in speed

	bool IsInFiringRange() const;
	bool IsInFiringRange(const Entity* entity) const;
	
	bool operator<(const Vessel& other) const;

	// Helpers

	bool TurnTo(const Entity* entity);
	bool TurnTo(const Space::Point2D<double>& dest);
	bool TurnTo(double theta);

	bool MoveToward(const Moveable* moveable);
	bool MoveToward(const Entity* entity);
	bool MoveToward(const Space::Point2D<double>& dest);
	bool MoveToward(double theta);

	void Render(float interp, Space::Point2D<double>& offset) override;

private:

	const ShipDef* m_pDef;
	ShipStats m_Stats;
	double m_TurnWidth;

	VesselController* m_pController;
	
	std::vector<WeaponSlot*> m_Weapons;
	double m_SumCooldown;
	double m_SecondsToFire;
	double m_NextToFire;

	UINT GetSumCooldown() const;

	//ShipClass m_ShipClass;
	//const char* m_textureName;

	const Vessel* m_pTarget;

	void UpdateTargetInFiringRange();

	bool m_TargetInFiringRange;

	double largestFiringArc;
	double longestFiringRange;

	std::array<bool, NUM_ACTIONS> actions;
	bool justTargeted;

	void TakeActions();

	void TurnCW();

	void TurnCCW();

	void UpdateFiringRange();

	void Fire();

	void Launch();

	void TurnAround();

	void Land();

	void Board();

	void Contact();

	//static Space::Ellipse GetEllipseFromClass(ShipClass shipClass);
};

}