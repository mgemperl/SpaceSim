#include "Vessel.h"
#include "VesselController.h"
#include "Sector.h"
#include "Damager.h"
#include "WeaponSlot.h"
#include "Data.h"
#include "EntityManager.h"
#include "DefinitionManager.h"
#include <vector>

using namespace Space;

namespace SpaceSimNS
{

Vessel::Vessel(const char* defName, Faction allegience, VesselController* pCont) :
	Moveable(Point2D<double>(0, 0), allegience)
{
	const ShipDef* def = DefinitionManager::Instance()->GetShipDef(defName);
	m_pDef = def;
	m_Stats = def->stats;
	m_TurnWidth = def->stats.turnRate;
	actions.fill(false);

	m_pController = pCont;
	pCont->Assign(this);

	SetDamageable(true);

	std::vector<Point2D<double>> polygon;
	polygon.emplace_back(Point2D<double>(3, 14));
	polygon.emplace_back(Point2D<double>(-3, 14));
	polygon.emplace_back(Point2D<double>(-6, -16));
	polygon.emplace_back(Point2D<double>(6, -16));
	SetCollisionPolygon(polygon);

	m_pTarget = NULL;
	justTargeted = false;

	for (int itr = 0; itr < def->weapSlots.size(); itr++)
	{
		m_Weapons.emplace_back(new WeaponSlot(def->weapSlots[itr], this));
	}

	m_NextToFire = 0;
	m_SecondsToFire = 0;
	m_SumCooldown = GetSumCooldown();

	largestFiringArc = 0;
	longestFiringRange = 0;
}

Vessel::~Vessel()
{
	delete m_pController;

	for (WeaponSlot* slot : m_Weapons)
	{
		delete slot;
	}

	m_Weapons.clear();
}

bool Vessel::InitializeDrawable(Graphics* pGraphics)
{
	bool success = Entity::InitializeDrawable(pGraphics, m_pDef->texture);

	for (int index = 0; index < m_Weapons.size() && success; index++)
	{
		success = m_Weapons[index]->InitializeDrawable(pGraphics);
	}

	return success;
}

bool Vessel::Intact() const
{
	return m_Stats.hull > 0;
}

bool Vessel::AtFullSpeed() const
{
	return (GetVelocity().GetSqrMagnitude() >= m_Stats.maxSpeed * m_Stats.maxSpeed);
}

const Vessel* Vessel::GetTarget() const
{
	return m_pTarget;
}

bool Vessel::VerifyTarget() const
{
	return ((m_pTarget != NULL) && (m_pTarget->GetSector() == GetSector()));
}

void Vessel::SetTarget(const Vessel* pTarget)
{
	m_pTarget = pTarget;
}

inline ClassType Vessel::GetClass() const
{
	return VESSEL;
}

ShipClass Vessel::GetShipClass() const
{
	return m_pDef->shipClass;
}

ShipStats Vessel::GetStats() const
{
	return m_Stats;
}

std::vector<const WeaponSlot*> Vessel::GetWeapons() const
{
	std::vector<const WeaponSlot*> weapons;
	weapons.assign(m_Weapons.begin(), m_Weapons.end());
	return weapons;
}

bool Vessel::Facing(const Space::Point2D<double>& point) const
{
	return Facing(Space::Vector2D::ComputeAngle(GetPos(), point));
}

bool Vessel::Facing(double theta) const
{
	return (abs(GetOrientationRad() - theta) <= m_Stats.turnRate);
}

bool Vessel::MovingToward(const Space::Point2D<double>& point) const
{
	return MovingToward(Vector2D::ComputeAngle(GetPos(), point));
}

bool Vessel::MovingToward(double theta) const
{
	return ((abs(GetVelocity().GetAngle() - theta) < EIGHTH_PI) && AtFullSpeed());
}

bool Vessel::FasterThan(const Vessel* other) const
{
	return (m_Stats.maxSpeed >= other->m_Stats.maxSpeed);
}

bool Vessel::IsInFiringRange() const
{
	return m_TargetInFiringRange;
}

bool Vessel::IsInFiringRange(const Entity* entity) const
{
	bool inRange = false;

	for (int itr = 0; (itr < m_Weapons.size()) && !inRange; itr++)
	{
		inRange = (inRange || m_Weapons[itr++]->IsInRange(entity));
	}

	return inRange;
} 

double Vessel::Range() const
{
	return longestFiringRange;
}

double Vessel::Arc() const
{
	return largestFiringArc;
}

UINT Vessel::GetSumCooldown() const
{
	UINT sum = 0;

	for (WeaponSlot* pWeapon : m_Weapons)
	{
		sum += pWeapon->Cooldown();
	}

	return sum;
}

void Vessel::ConstUpdate()
{
	Moveable::ConstUpdate();
	
	// Update m_Weapons
	for (int itr = 0; itr < m_Weapons.size(); itr++)
	{
		m_Weapons[itr]->ConstUpdate();
	}
}

void Vessel::Update(double deltaT)
{
	Moveable::Update(deltaT);
	
	if (!VerifyTarget())
	{
		m_pTarget = NULL;
	}

	if (m_SecondsToFire > 0)
	{
		m_SecondsToFire -= deltaT;
	}

	if (m_SecondsToFire < 0)
	{
		m_SecondsToFire = 0;
	}
	

	if (m_pTarget != NULL)
	{
		
		m_TargetInFiringRange = IsInFiringRange(m_pTarget);
	}
	else
	{
		m_TargetInFiringRange = false;
	}

	UpdateActions();
	TakeActions();
}

void Vessel::HandleMessage(const Telegram& message)
{
	if (message.type == PROJECTILEIMPACT)
	{
		Entity* sender = EntityManager::Instance()->GetEntityFromID(message.senderID);
		//Impact(dynamic_cast<Projectile*>(sender), Dereference<Point2D<double>>(message.info));
	}

}

void Vessel::UpdateActions()
{
	if (m_pController != NULL)
	{
		m_pController->UpdateActions();
	}
}

bool Vessel::InstallWeapon(WeapDef weapon, int slot)
{
	bool installed = false;

	if (slot < m_Weapons.size())
	{
		installed = m_Weapons[slot]->InstallWeapon(weapon);
	}

	UpdateFiringRange();
	m_SumCooldown = GetSumCooldown();

	return installed;
}

bool Vessel::UninstallWeapon(int slot)
{
	bool uninstalled = false;

	if (slot < m_Weapons.size())
	{
		m_Weapons[slot]->UninstallWeapon();
		uninstalled = true;
	}

	UpdateFiringRange();
	m_SumCooldown = GetSumCooldown();

	return uninstalled;
}

void Vessel::UpdateFiringRange()
{	
	for (WeaponSlot* pWeap : m_Weapons)
	{
		longestFiringRange = max(longestFiringRange, pWeap->Range());
		largestFiringArc = max(largestFiringArc, pWeap->Arc());
	}
}

bool Vessel::operator<(const Vessel& other) const
{
	bool comp = false;
	
	if (m_pDef->shipClass < other.m_pDef->shipClass)
	{
		comp = true;
	}
	else if (((m_pDef->shipClass == other.m_pDef->shipClass) && 
		     ((m_Stats.maxHull + m_Stats.maxShield) < 
			  (other.m_Stats.maxHull + other.m_Stats.maxShield))))
	{
		comp = true;
	}

	return comp;
}

void Vessel::DoAction(Action action)
{
	actions[action] = true;
}

void Vessel::StopAction(Action action)
{
	actions[action] = false;

	if ((action == TARGET) || 
		(action == BESTTARGET) || 
		(action == UNTARGET))
	{
		justTargeted = false;
	}
}

void Vessel::StopAll()
{
	actions.fill(false);
	justTargeted = false;
}

void Vessel::TakeActions()
{
	if (actions[FIRE])
	{
		Fire();
	}
	if (actions[LAUNCH])
	{
		Launch();
	}
	if (actions[TURNCW])
	{
		TurnCW();
	}
	if (actions[TURNCCW])
	{
		TurnCCW();
	}
	if (actions[ACCEL])
	{
		Accelerate(Space::Vector2D(GetOrientationRad(), m_Stats.forAccelRate), m_Stats.maxSpeed);
	}
	if (actions[BACK])
	{
		Accelerate(Space::Vector2D((GetOrientationRad() - PI), m_Stats.backAccelRate), m_Stats.maxSpeed);
	}
	if (actions[LEFT])
	{
		Accelerate(Space::Vector2D((GetOrientationRad() - HALF_PI), m_Stats.sideAccelRate), m_Stats.maxSpeed);
	}
	if (actions[RIGHT])
	{
		Accelerate(Space::Vector2D((GetOrientationRad() + HALF_PI), m_Stats.sideAccelRate), m_Stats.maxSpeed);
	}
	if (actions[LAND])
	{
		Land();
	}
	if (actions[BOARD])
	{
		Board();
	}
	if (actions[CONTACT])
	{
		
	}
	if (actions[UNTARGET])
	{
		SetTarget(NULL);
		justTargeted = true;
	}
	else if (actions[TARGET] && !justTargeted)
	{
		NextTarget();
		justTargeted = true;
	}
	else if (actions[BESTTARGET] && !justTargeted)
	{
		SetTarget(BestTarget(this, GetSector()));
		justTargeted = true;
	}
	

}

void Vessel::TurnCW()
{
	Turn(m_TurnWidth);
}

void Vessel::TurnCCW()
{
	Turn(-m_TurnWidth);
}

void Vessel::Fire()
 {
/* Uncomment and implement if I don't want all weapons to fire at once.
	if (m_SecondsToFire <= 0)
	{
		if (++m_NextToFire >= m_Weapons.size())
		{
			m_NextToFire = 0;
		}

		if (m_Weapons[m_NextToFire]->AimReady())
		{
			m_Weapons[m_NextToFire]->Fire();
		}
		
		UINT averageCooldown = m_SumCooldown / m_Weapons.size();
		m_SecondsToFire = averageCooldown / m_Weapons.size() + 1;
	}
*/

	// Fire all weapons that can hit their target
	// Delete this loop when commented code above is implemented
	for (WeaponSlot* weap : m_Weapons)
	{
		if (weap->TargetInArc())
		{
			weap->Fire();
		}
		
	}

}

void Vessel::Launch()
{

}

void Vessel::TurnAround()
{
	double crossProduct = 
		GetVelocity().cross(Space::Vector2D(GetOrientationRad(), 1));

	if (crossProduct > 0)
	{
		Turn(m_TurnWidth);
	}
	else
	{
		Turn(-m_TurnWidth);
	}
	
}

void Vessel::Land()
{

}

void Vessel::Board()
{

}

void Vessel::Contact()
{

}

void Vessel::NextTarget()
{
	bool done = false;
	bool ready = false;
	bool isPlayer = (GetClass() == PLAYER);

	const Entity::ConstEntitySet occupants = GetSector()->Occupants();
	
	std::set<const Entity*>::const_iterator begin = occupants.begin();
	std::set<const Entity*>::const_iterator itr;

	if (m_pTarget != NULL)
	{
		itr = occupants.find(m_pTarget);
	}
	else
	{
		itr = occupants.begin();
	}
	
	std::set<const Entity*>::const_iterator end = occupants.end();
	std::set<const Entity*>::const_iterator primeTarget = itr;

	while (!ready)
	{
		if ((*itr)->GetShipClass() <= MISSILE)
		{
			itr++;
		}
		else
		{
			ready = true;
		}
	}

	bool iterated = false;
	while (!done)
	{
		if (!done && ((itr) == end))
		{
			if (iterated)
			{
				done = true;
				m_pTarget = NULL;
			}

			iterated = true;
			itr = begin;
		}
		
		if (IsHostile(*itr) && (*itr != m_pTarget) && ((*itr)->GetClass() != PROJECTILE))
		{
			SetTarget(dynamic_cast<const Vessel*>(*itr));
			done = true;
		}

		itr++;
	}

	StopAction(Action::TARGET);
}

const Vessel* Vessel::BestTarget(const Entity* pEnt, const Sector* pSector)
{
	const Vessel* bestTarget = NULL;
	
	const Entity::ConstEntitySet occupants = pSector->Occupants();
	std::set<const Entity*>::const_iterator itr = occupants.begin();
	std::set<const Entity*>::const_iterator end = occupants.end();
	std::set<const Entity*>::const_iterator primeTarget = end;
	double lowDist;

	while (itr != end)
	{
		if ((*itr)->GetAllegience() != pEnt->GetAllegience() && 
			(*itr)->GetShipClass() > MISSILE)
		{
			double dist = pEnt->GetPos().SqrDistance((*itr)->GetPos());

			if (primeTarget == end || dist < lowDist)
			{
				lowDist = dist;
				primeTarget = itr;
			}
		}

		itr++;
	}

	if (primeTarget != end)
	{
		bestTarget = dynamic_cast<const Vessel*>(*primeTarget);
	}
	
	return bestTarget;
}

void Vessel::SetTurnWidth(double turnWidth)
{
	if (turnWidth > m_Stats.turnRate)
	{
		m_TurnWidth = m_Stats.turnRate;
	}
	else
	{
		m_TurnWidth = turnWidth;
	}
}

// Helpers

bool Vessel::TurnTo(const Entity* entity)
{
	return TurnTo(entity->GetPos());
}

bool Vessel::TurnTo(const Space::Point2D<double>& dest)
{
	return TurnTo(Space::Vector2D::ComputeAngle(GetPos(), dest));
}

bool Vessel::TurnTo(double theta)
{
	double cross = Space::Vector2D(GetOrientationRad(), 1).cross(Space::Vector2D(theta, 1));
		
	if (cross < 0)
	{
		StopAction(TURNCW);
		DoAction(TURNCCW);
	}
	else
	{
		StopAction(TURNCCW);
		DoAction(TURNCW);
	}

	SetTurnWidth(fabs(Vector2D::AngleDiff(GetOrientationRad(), theta)));

	return Facing(theta);
}

bool Vessel::MoveToward(const Moveable* moveable)
{
	return MoveToward(moveable->ProjectCollisionPoint(GetPos(), GetStats().maxSpeed * 2));
}

bool Vessel::MoveToward(const Entity* entity)
{
	return MoveToward(entity->GetPos());
}

bool Vessel::MoveToward(const Space::Point2D<double>& dest)
{
	return MoveToward(Space::Vector2D::ComputeAngle(GetPos(), dest),
		GetPos().SqrDistance(dest));
}

bool Vessel::MoveToward(double theta)
{
	double dir = theta + QUARTER_PI * sin(Vector2D::SimplifyAngle(
		theta - GetVelocity().GetAngle()));

	TurnTo(dir);

	// If the vessel is facing the m_pTarget's general direction, accelerate.
	if (abs(Vector2D::AngleDiff(GetOrientationRad(), dir))) 
	{
		DoAction(ACCEL);
	}
	else
	{
		StopAction(ACCEL);
	}

	return (MovingToward(theta));
}

bool Vessel::MoveToward(double theta, double sqrDist)
{
	double diff = Vector2D::AngleDiff(GetVelocity().GetAngle(), theta);
	double sign = diff > 0 ? 1.0 : -1.0;
	double dir = theta + QUARTER_PI * (diff / PI);
	double add = (sign * QUARTER_PI / std::fmin(1.0, std::log(sqrDist) * 6));
	dir += add;

	TurnTo(dir);

	// If the vessel is facing the m_pTarget's general direction, accelerate.
	if (abs(Vector2D::AngleDiff(GetOrientationRad(), dir)) < HALF_PI)
	{
		DoAction(ACCEL);
	}
	else
	{
		StopAction(ACCEL);
	}

	return (MovingToward(theta));
}

void Vessel::Render(float interp, Space::Point2D<double>& offset)
{
	Entity::Render(interp, offset);

	for (WeaponSlot* pWeap : m_Weapons)
	{
		pWeap->Render(interp, offset);
	}
}

}