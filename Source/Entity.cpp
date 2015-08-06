#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "CollisionPolygon.h"
#include "Vector2D.h"
#include <string>

using namespace Space;

namespace SpaceSimNS
{

Entity::Entity(const Space::Point2D<double>& inPos)
{
	m_ID = Entity::NextValidID();
	m_position = inPos;
	m_allegience = SWISS;
	m_pDrawable = NULL;
	m_dOrientation = 0;
	m_bCollider = false;
	m_bDamageable = false;
	m_pPolygon = NULL;

	EntityManager::Instance()->RegisterEntity(this);
}

Entity::Entity(const Space::Point2D<double>& inPos, Faction inAllegience)
{
	m_ID = Entity::NextValidID();
	m_position = inPos;
	m_allegience = inAllegience;
	m_pDrawable = NULL;
	m_dOrientation = 0;
	m_bCollider = false;
	m_bDamageable = false;
	m_pPolygon = NULL;

	EntityManager::Instance()->RegisterEntity(this);
}

bool Entity::InitializeDrawable(Graphics* pGraphics, int width, int height,
	int nCols, const char* texture)
{
	ReleaseDrawable();
	m_pDrawable = new Drawable();
	return m_pDrawable->Initialize(pGraphics, width, height, nCols, texture);
}

void Entity::ReleaseDrawable()
{
	if (m_pDrawable != NULL)
	{
		delete m_pDrawable;
		m_pDrawable = NULL;
	}
}

Entity::~Entity()
{
	if (m_pPolygon != NULL)
	{
		delete m_pPolygon;
	}

	ReleaseDrawable();
	EntityManager::Instance()->RemoveEntity(m_ID);
}

void Entity::SetCollisionPolygon(std::vector<Point2D<double>>& vertices)
{
	if (m_pPolygon != NULL)
	{
		delete m_pPolygon;
	}

	m_pPolygon = new CollisionPolygon(vertices);
}

void Entity::Update(double deltaT)
{
	if (m_pDrawable != NULL)
	{
		m_pDrawable->UpdateAnimation(deltaT);
	}
}

bool Entity::CanCollide(const Entity* other) const
{
	// Maybe override in children to check if it's hostile or whatever

	if (GetPos().Distance(other->GetPos()) <= sqrt(
		m_pPolygon->GetMaxVertex()->GetSqrMagnitude() +
		other->m_pPolygon->GetMaxVertex()->GetSqrMagnitude()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Entity::IsHostile(const Entity* entity) const
{
	bool isHostile = false;

	if ((GetAllegience() == ENEMYFACTION) && (entity->GetAllegience() == PLAYERFACTION))
	{
		isHostile = true;
	}
	else if ((GetAllegience() == PLAYERFACTION) && (entity->GetAllegience() == ENEMYFACTION))
	{
		isHostile = true;
	}

	return isHostile;
}

bool Entity::CollidesWith(const Entity* other) const
{
	return CanCollide(other) ?
		CollisionPolygon::DetectCollision(
		*m_pPolygon,
		*other->m_pPolygon,
		other->m_position - m_position,
		GetVelocity(),
		other->GetVelocity(),
		m_dOrientation,
		other->m_dOrientation) :
		false;
}

void Entity::Render(float interpolation, Point2D<double>& offset)
{
	if (m_pDrawable != NULL)
	{
		UpdateDrawable(interpolation, offset);
		m_pDrawable->Draw(WHITE);
	}
}

bool Entity::operator<(const Entity& other) const
{
	return (GetShipClass() < other.GetShipClass());
}

void Entity::UpdateDrawable(double interp, Space::Point2D<double>& offset)
{
	if (m_pDrawable != NULL)
	{
		Point2D<double> interpPos = GetInterpPos(interp) + offset;
		m_pDrawable->SetX(interpPos.GetX() - m_pDrawable->GetWidth() / 2.0);
		m_pDrawable->SetY(interpPos.GetY() - m_pDrawable->GetHeight() / 2.0);
		m_pDrawable->SetAngle(Vector2D::SimplifyAngle(m_dOrientation + HALF_PI));

		m_pDrawable->Draw();
	}
}

}

