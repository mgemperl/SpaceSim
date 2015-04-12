#include "WeaponSlot.h"
#include "Vessel.h"
#include "Sector.h"
#include "TurretStates.h"
#include "GuidedProjectile.h"

#define POINT_TURN_RATE 0.2
#define SMALL_TURN_RATE 0.1
#define MEDIUM_TURN_RATE 0.05
#define LARGE_TURN_RATE 0.25
#define MASSIVE_TURN_RATE 0.1

#define SPREAD 0.015

using namespace Space;

namespace SpaceSimNS
{

WeaponSlot::WeaponSlot(const SlotDef& def, const Vessel* pShip) :
	Entity(Space::Point2D<double>(), pShip->GetAllegience())
{

	m_pShip = pShip;
	m_RelativePos = Vector2D(def.angleOff, def.magOff);

	m_Size = def.size;
	m_DefaultFacing = def.facing;
	m_RelativeFacing = def.facing;
	m_FiringArc = def.firingArc;
	m_BufferArc = def.bufferArc;
	m_SecondsUntilReload = 0;
	m_pTarget = NULL;
	m_TargetInRange = false;
	m_TargetInArc = false;
	m_AimReady = false;
	m_WeaponInstalled = false;

	m_pTargetingStateMachine = new StateMachine<WeaponSlot>(this);

	if (def.isTurret)
	{
		m_pTargetingStateMachine->SetState(new ManualTurretControl(this));
	}
	else
	{
		m_pTargetingStateMachine->SetState(new ManualTurretControl(this));
	}
	

	m_pTrackingStateMachine = new StateMachine<WeaponSlot>(this);

	if (def.isTurret)
	{
		m_TurnRate = GetTurnRateFromSize(def.size);
		m_pTrackingStateMachine->SetState(new TrackTarget(this));
	}
	else
	{
		m_TurnRate = 0;
		m_pTrackingStateMachine->SetState(new RemainStationary(this));
	}
	
	UpdatePos();
	UpdateOrientation();
}

WeaponSlot::~WeaponSlot()
{
}

bool WeaponSlot::InitializeDrawable(Graphics* pGraphics)
{
	return Entity::InitializeDrawable(pGraphics, "gun");
}

Space::Point2D<double> WeaponSlot::GetInterpPos(double interp) const
{
	Vector2D posVector = m_RelativePos;
	posVector.rotate(m_pShip->GetOrientationRad());
	return posVector.GetTerminal(m_pShip->GetInterpPos(interp));
}

bool WeaponSlot::InstallWeapon(WeapDef& weapon)
{
	bool installable = weapon.size <= m_Size;
	
	if (installable)
	{
		m_WeapDef = weapon;
		m_WeaponInstalled = true;
		m_FiringRange = m_WeapDef.lifetime * m_WeapDef.speed;
	}
	
	return installable;
}

void WeaponSlot::UninstallWeapon()
{
	m_WeaponInstalled = false;
}

bool WeaponSlot::WeaponInstalled() const
{
	return m_WeaponInstalled;
}

double WeaponSlot::Arc() const
{
	return m_FiringArc;
}

double WeaponSlot::Range() const
{
	return m_FiringRange;
}

UINT WeaponSlot::Cooldown() const
{
	UINT cooldown = 0;
	
	if (m_WeaponInstalled)
	{
		cooldown = m_WeapDef.reloadTime;
	}

	return cooldown;
}

bool WeaponSlot::IsInRange(const Entity* pEntity) const
{
	bool isInRange = false;

	if (pEntity != NULL)
	{
		isInRange = GetPos().SqrDistance(pEntity->GetPos()) < (m_FiringRange * m_FiringRange);
	}

	return isInRange;
}

bool WeaponSlot::IsInArc(const Entity* pEntity) const
{
	bool isInArc = false;

	if (pEntity != NULL)
	{
		if (!m_WeapDef.isGuided)
		{
			double facing = m_DefaultFacing + m_pShip->GetOrientationRad();
			double targetAngle = Vector2D::ComputeAngle(GetPos(),
				pEntity->ProjectCollisionPoint(GetPos(), m_WeapDef.speed));

			isInArc = fabs(Vector2D::AngleDiff(facing, targetAngle)) <= 
				m_FiringArc / 2.0;
		}
		else
		{
			isInArc = true;
		}
	}

	return isInArc;
}

bool WeaponSlot::TargetInRange() const
{
	return m_TargetInRange;
}

bool WeaponSlot::TargetInArc() const
{
	return m_TargetInArc;
}

void WeaponSlot::UpdateRangeArcAim()
{
	m_TargetInRange = false;
	m_TargetInArc = false;
	m_AimReady = false;
	
	if (m_pTarget != NULL)
	{
		m_TargetInRange = IsInRange(m_pTarget);
		m_TargetInArc = IsInArc(m_pTarget);

		if (m_TargetInRange && m_TargetInArc)
		{
			m_AimReady = ((m_WeapDef.isGuided) ||
				(fabs(Vector2D::AngleDiff(GetOrientationRad(), m_TargetAngle)) <= 
				(m_BufferArc / 2.0 + THETA_E)));
		}
	}
}

void WeaponSlot::SetTargetingOrder(StateType order)
{
	switch (order)
	{
	case ATTACKSHIPTARGET:
		m_pTargetingStateMachine->ChangeState(new AttackShipTarget(this));
		break;
	case ATTACKBESTTARGET:
		m_pTargetingStateMachine->ChangeState(new AttackBestTarget(this));
		break;
	}
}

const Vessel* WeaponSlot::Target() const
{
	return m_pTarget;
}

void WeaponSlot::SetTarget(const Vessel* pTarget)
{
	m_pTarget = pTarget;
}

void WeaponSlot::Fire()
{	
	static_cast<TurretState*>(m_pTargetingStateMachine->CurrentState())->Fire();
}

void WeaponSlot::FireDumb()
{
	if (m_WeaponInstalled && (m_SecondsUntilReload <= 0))
	{
		double firingAngle = GetOrientationRad();

		if (m_AimReady)
		{
			firingAngle = m_TargetAngle;
		}

		Projectile* proj = new Projectile(
			GetPos(),
			(Space::Vector2D(firingAngle + RANDSIGN * RANDOM * SPREAD, m_WeapDef.speed)),
			m_pShip->GetAllegience(), m_WeapDef);


		m_pShip->GetSector()->AddEntity(proj);

		m_SecondsUntilReload = m_WeapDef.reloadTime;
	}
}

void WeaponSlot::FireGuided()
{
	if (m_WeaponInstalled && (m_SecondsUntilReload <= 0))
	{

		GuidedProjectile* proj = new GuidedProjectile(
			"boom",
			m_pShip->GetAllegience(),
			GetPos(),
			Vector2D(GetOrientationRad() + RANDSIGN * RANDOM * SPREAD, 
				m_WeapDef.speed) + m_pShip->GetVelocity(),
			m_WeapDef,
			new GuidedProjectileAI(),
			m_pTarget);

		m_pShip->GetSector()->AddEntity(proj);

		m_SecondsUntilReload = m_WeapDef.reloadTime;

	}
}

void WeaponSlot::SetFacing(double theta)
{
	if (fabs(Vector2D::AngleDiff(theta, m_DefaultFacing)) < 
		(m_FiringArc / 2.0 + THETA_E))
	{
		m_RelativeFacing = Vector2D::SimplifyAngle(theta);
	}
}

void WeaponSlot::TurnFacing(double dTheta)
{
	double neoFacing = m_RelativeFacing + dTheta;

	if (fabs(Vector2D::AngleDiff(neoFacing, m_DefaultFacing)) < 
		(m_FiringArc / 2.0 + THETA_E))
	{
		m_RelativeFacing = Vector2D::SimplifyAngle(m_RelativeFacing + dTheta);
	}
}

void WeaponSlot::ConstUpdate()
{
	UpdatePos();

	UpdateTargetAngle();
	m_pTrackingStateMachine->Update();

	UpdateOrientation();

	UpdateRangeArcAim();
	m_pTargetingStateMachine->Update();

	if (m_SecondsUntilReload > 0)
	{
		m_SecondsUntilReload--;
	}
	else
	{
		m_SecondsUntilReload = 0;
	}
}

void WeaponSlot::UpdatePos()
{
	Vector2D posVector = m_RelativePos;
	posVector.rotate(m_pShip->GetOrientationRad());
	SetPosition(posVector.GetTerminal(m_pShip->GetPos()));
}

void WeaponSlot::UpdateOrientation()
{
	SetOrientation(m_RelativeFacing + m_pShip->GetOrientationRad());
}

void WeaponSlot::UpdateTargetAngle()
{
	if (m_pTarget != nullptr)
	{
		if (m_TargetInRange)
		{
			m_TargetAngle = Vector2D::ComputeAngle(GetPos(), m_pTarget->ProjectCollisionPoint(GetPos(), m_WeapDef.speed));
		}
		else
		{
			m_TargetAngle = Vector2D::ComputeAngle(GetPos(), m_pTarget->GetPos());
		}
	}
	else
	{
		m_TargetAngle = 0;
	}
}

void WeaponSlot::HandleMessage(const Telegram& message)
{

}

bool WeaponSlot::Facing(const Space::Point2D<double>& point) const
{
	return Facing(Space::Vector2D::ComputeAngle(GetPos(), point));
}

bool WeaponSlot::Facing(double theta) const
{
	return (fabs(Vector2D::AngleDiff(GetOrientationRad(), theta)) <= m_TurnRate);
}

bool WeaponSlot::TurnTo(const Entity* entity)
{
	return TurnTo(entity->GetPos());
}

bool WeaponSlot::TurnTo(const Space::Point2D<double>& point)
{
	return TurnTo(Space::Vector2D::ComputeAngle(GetPos(), point));
}

bool WeaponSlot::TurnTo(double theta)
{
	double diff;

	// If the target exists and is in the weapon slot's firing arc, turn toward the
	// target. Otherwise, turn toward the weapon slot's default facing.
	if (m_TargetInArc || (m_pTarget == nullptr))
	{
		diff = Vector2D::AngleDiff(GetOrientationRad(), theta);
	}
	else
	{
		diff = Vector2D::AngleDiff(GetOrientationRad(),
			m_pShip->GetOrientationRad() + m_DefaultFacing);
	}
	
	// Do the actual turning
	if (fabs(diff) < m_TurnRate)
	{
		TurnFacing(diff);
	}
	else
	{
		if (diff < 0)
		{
			TurnFacing(-m_TurnRate);
		}
		else
		{
			TurnFacing(m_TurnRate);
		}
	}
	

	return (diff < THETA_E);
}

double WeaponSlot::GetTurnRateFromSize(WeaponSize size)
{
	double turnRate = 0;

	switch (size)
	{
	case POINT:
		turnRate = POINT_TURN_RATE;
		break;
	case SMALL:
		turnRate = SMALL_TURN_RATE;
		break;
	case MEDIUM:
		turnRate = MEDIUM_TURN_RATE;
		break;
	case LARGE:
		turnRate = LARGE_TURN_RATE;
		break;
	case MASSIVE:
		turnRate = MASSIVE_TURN_RATE;
		break;
	}

	return turnRate;
}

bool WeaponSlot::Intact() const
{
	return m_pShip->Intact();
}

const Vessel* WeaponSlot::BestTarget() const
{
	const Vessel* bestTarget = nullptr;

	//std::vector<Vessel*> potentialTargets;

	for (const Entity* potential : m_pShip->GetSector()->Occupants())
	{
		// If the entity is hostile and a ship
		if ((potential->GetAllegience() != GetAllegience() &&
			potential->GetShipClass() != MISSILE && 
			potential->GetShipClass() != NONSHIP))
		{
			if (IsInArc(potential) && IsInRange(potential))
			{
				bestTarget = static_cast<const Vessel*>(potential);
			}
		}
	}

	return bestTarget;
}

//HRESULT WeaponSlot::Draw(
//	ID2D1HwndRenderTarget* renderTarget,
//	const D2DRes& res,
//	Space::Point2D<double>& offset,
//	const Space::Point2D<double>& shipInterpOffset)
//{
//	HRESULT hr = S_OK;
//
//	Point2D<double> interpPos = GetPos() + shipInterpOffset;
//
//	const D2D1::Matrix3x2F rot =
//		D2D1::Matrix3x2F::Rotation(
//		GetOrientationDeg(),
//		interpPos.D2DPoint2F());
//
//	const D2D1::Matrix3x2F trans =
//		D2D1::Matrix3x2F::Translation(offset.D2DSizeF());
//
//	Space::Ellipse interpEllipse = GetEll() + shipInterpOffset;
//
//	renderTarget->SetTransform(rot * trans);
//	renderTarget->FillEllipse(interpEllipse.D2DEllipse(), res.gray);
//	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
//
//	return hr;
//}

/*
Space::Ellipse WeaponSlot::GetEllipseFromSlotDef(const SlotDef& def)
{
	Space::Ellipse ellipse;
	
	switch (def.size)
	{
	case SMALL:
		ellipse = Space::Ellipse(Point2D<double>(), 4, 2);
		break;
		
	}

	return ellipse;
}
*/

}

