#include "Projectile.h"
#include "Sector.h"
#include "Data.h"
#include "MessageDispatcher.h"
#include "Vector2D.h"

using namespace Space;

namespace SpaceSimNS
{

	Projectile::Projectile(
		const Space::Point2D<double>& inPos,
		const Space::Vector2D& inVelocity,
		Faction inAllegience,
		const WeapDef& def) :
	Moveable(inPos,
	inVelocity,
	inAllegience)
{
		SetOrientation(inVelocity.GetAngle());
		weapon = def;
		lifetime = def.lifetime;
		owner = inAllegience;
		madeImpact = false;
}

Projectile::~Projectile()
{
}

void Projectile::Update(double deltaT)
{
	if ((lifetime -= deltaT) <= 0 || CheckForCollisions(this))
	{
		GetSector()->DestroyEntity(this);
	}
}

/*
bool Projectile::CheckForCollisions(Moveable* pCollider)
{
	bool madeImpact = false;
	
	Entity::ConstEntitySet occupants = pCollider->GetSector()->Occupants();
	std::set<const Entity*>::const_iterator occItr = occupants.begin();
	std::set<const Entity*>::const_iterator occEnd = occupants.end();

	// Iterate through all occupants of this projectile's sector checking for collisions,
	// or until all entities have been checked.
	while ((occItr != occEnd) && !madeImpact)
	{
		// If the entity is not a projectile, not friendly, and not neutral, check for collision.
		if (((*occItr)->GetClass() != PROJECTILE) &&
			((*occItr)->GetClass() != MISSILE) &&
			((*occItr)->GetAllegience() != pCollider->GetAllegience()) &&
			((*occItr)->GetAllegience() != SWISS))
		{
			Projectile::IntersectVector intersects = Projectile::CheckIfCollides(
				pCollider->GetPos(),
				pCollider->GetVelocity()->GetTerminal(pCollider->GetPos()),
				(*occItr)->GetEll());

			// If there is at least one intersection, flag the projectile for destruction
			// and set the loop's exit condition to true.
			if (intersects.size() == 1)
			{
				//expirationInterpolation = intersects->at(0).second;
				madeImpact = true;

				//Hit(*occItr, intersects->at(0).first);
			}
			else if (intersects.size() == 2)
			{
				//expirationInterpolation = min(intersects->at(0).second, intersects->at(1).second);
				madeImpact = true;

				//Point2D<double> hitPoint =
				//(intersects->at(0).second == expirationInterpolation) ?
				//intersects->at(0).first : intersects->at(1).first;

				//Hit(*occItr, hitPoint);
			}
		}

		occItr++;
	}

	return madeImpact;
}
*/

/*
Projectile::IntersectVector Projectile::CheckIfCollides(
	const Point2D<double>& pos,
	const Point2D<double>& nextPos,
	const Space::Ellipse& pEll)
{
	//Space::Point2D<double> pt1 = pProj.GetPos();
	//Space::Point2D<double> pt2 = pProj.GetVelocity()->GetTerminal(pt1);

	Space::Point2D<double> entityCenter = pEll.Center();
	double radius = max(pEll.RadX(), pEll.RadY());

	IntersectVector intersects;

	// If projectile is near the entity, check if it's movement vector
	// intersects the entity's collision ellipsoid.
	if (nextPos.SqrDistance(entityCenter) < 2.0 * (radius * radius));
	{
		 intersects = pEll.Intersection(pos, nextPos);
	}

	return intersects;
}
*/

void Projectile::HandleMessage(const Telegram& message)
{

}

inline ClassType Projectile::GetClass() const
{
	return PROJECTILE;
}

void Projectile::Hit(Entity* victim, const Point2D<double>& hitPoint)
{
	Telegram message = Telegram(GetID(), victim->GetID(), PROJECTILEIMPACT, 0, new Point2D<double>(hitPoint));
	MessageDispatcher::Instance()->DispatchTelegram(message);
}

bool Projectile::Impact(
	const Projectile* proj,
	const Space::Point2D<double>& impactPoint)
{
	return false;
}

bool Projectile::Intact() const
{
	return !madeImpact;
}

/*
HRESULT Projectile::DrawInSector(
	ID2D1HwndRenderTarget* renderTarget,
	const D2DRes& res,
	double interpolation,
	Space::Point2D<double>& offset)
{
	HRESULT hr = S_OK;

	Space::Point2D<double> interpOffset = GetInterpOffset(interpolation);
	Space::Point2D<double> interpPos = interpOffset + GetPos();

	Space::Point2D<double> point =
		Space::Vector2D::GetTerminalFromPolar(
		GetVelocity()->GetAngle(),
		-10
	);

	D2D1_POINT_2F D2Dpoint2 = interpPos.D2DPoint2F();
	D2D1_POINT_2F D2Dpoint1 = (point + interpPos).D2DPoint2F();
	
	renderTarget->SetTransform(D2D1::Matrix3x2F::Translation(offset.D2DSizeF()));

	if (GetAllegience() == PIRATEFACTION)
	{
		renderTarget->DrawLine(D2Dpoint1, D2Dpoint2, res.green, 1.0F);
	}
	else
	{
		renderTarget->DrawLine(D2Dpoint1, D2Dpoint2, res.red, 1.0F);
	}
	
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	return hr;
}
*/

}