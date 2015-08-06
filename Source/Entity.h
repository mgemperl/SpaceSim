#pragma once

#include <unordered_map>
#include <set>
#include <map>

#include "Drawable.h"
#include "Data.h"
#include "Vector2D.h"

namespace Space
{
	class Vector2D;
}

namespace SpaceSimNS
{

class MessageDispatcher;
class Sector;
class EntityManager;
class CollisionPolygon;

class Entity
{
public:

	// Public instance fields

	// Comparator struct for sorting a collection of entities
	struct EntityPtrComp
	{
		// Order entities by ship class, then by address
		// Return true if lhs has a smaller ship class than rhs,
		// False otherwise 
		bool operator()(const Entity* lhs, const Entity* rhs) const
		{
			bool comp = false;

			// Be sure both argued entities exist
			if (lhs != NULL && rhs != NULL)
			{
				ShipClass lhsClass = lhs->GetShipClass();
				ShipClass rhsClass = rhs->GetShipClass();

				// If both entities have the same ship class, compare
				// by address to prevent false conclusions of equality
				if (lhsClass == rhsClass)
				{
					comp = lhs < rhs;
				}
				else
				{
					comp = lhsClass < rhsClass;
				}
			}

			return comp;
		}
	};

	/** Set of entities sorted by the EntityPtrComp comparator struct **/
	typedef std::set<SpaceSimNS::Entity*, Entity::EntityPtrComp> EntitySet;

	/** Set of constant entities sorted by the EntityPtrComp struct **/
	typedef std::set<const SpaceSimNS::Entity*, Entity::EntityPtrComp> ConstEntitySet;

	/** ID number for an entity **/
	typedef unsigned int ID;


	/** Constructors and Initialization **/

	// Constructor for an entity object with no allegience
	Entity(const Space::Point2D<double>& inPos);

	// Constructor for an entity with an allegience
	Entity(const Space::Point2D<double>& inPos, Faction inAllegience);
	~Entity();

	/** Meant to call protected InitializeDrawable methods **/
	virtual bool InitializeDrawable(Graphics* pGraphics) = 0;

	void ReleaseDrawable();

	/** Accessor Methods **/

	// Accessor for this entity's ID number
	UINT GetID() const { return m_ID; }

	// Accessor for this entity's position in its sector
	Space::Point2D<double> GetPos() const { return m_position; }

	// Computes the offset of this entity's position caused by interpolation
	// Meant to be overriden in moveable derivative classes
	virtual Space::Point2D<double> GetInterpPos(double interpolation) const
	{
		return m_position;
	}

	// Accessor for this entity's orientation in radians
	double GetOrientationRad() const { return m_dOrientation; }

	// Accessor for this entity's orientation in radians
	double GetOrientationDeg() const { return m_dOrientation * (180.0 / PI); }

	virtual Space::Vector2D GetVelocity() const
	{
		static Space::Vector2D velocity(0, 0);
		return velocity;
	}

	// Accessor for the name of this class 
	inline virtual ClassType GetClass() const { return ENTITY; }

	// Accessor for this entity's ship class
	virtual ShipClass GetShipClass() const { return NONSHIP; }

	// Accessor for this entity's allegience
	Faction GetAllegience() const { return m_allegience; }

	// Accessor for the sector that contains this entity, 
	// NULL for no sector
	Sector* GetSector() const { return m_pSector; }

	// Checks whether or not this entity collides with damagers.
	bool IsDamageable() const { return m_bDamageable; }

	bool IsCollider() const { return m_bCollider; }

	// Returns a pointer to this entity's collision polygon. Returns
	// NULL if the collision polygon hasn't been initialized.
	CollisionPolygon* GetCollisionPolygon() { return m_pPolygon; }

	// Accessor for whether this entity is hostile toward the argued entity
	bool IsHostile(const Entity* entity) const;

	// Accessor for whether this entity is intact
	// Entities that are not intact will soon be destroyed
	// Meant to be overridden by destructible derivatives
	virtual bool Intact() const { return true; }


	/** Setter Methods **/

	// Sets which sector contains this entity,
	// NULL for no sector
	void SetSector(Sector* neoSector) { m_pSector = neoSector; }

	// Sets the allegience of this entity
	void SetAllegience(Faction newMaster) { m_allegience = newMaster; }

	// Sets the position of this entity
	void SetPosition(const Space::Point2D<double>& inPos) { m_position = inPos; }

	// Sets the orientation of this entity in radians
	void SetOrientation(double theta) 
	{ 
		m_dOrientation = Space::Vector2D::SimplifyAngle(theta); 
	}

	// Turns this entity counter-clockwise by the argued
	// number of radians
	void Turn(double dTheta) { SetOrientation(m_dOrientation + dTheta); }

	// Sets whether his entity collides with damagers. If true, damagers
	// such as projectiles will check if they collide with this entity,
	// and inform this entity in the case of collision through the
	// messaging system (HandleMessage).
	void SetDamageable(bool damageable)
	{
		m_bDamageable = damageable;
	}

	// True if this entity collides with damageables. False if it
	// doesn't affect them.
	void SetCollider(bool collider)
	{
		m_bCollider = collider;
	}

	// Set this entity's collision polygon using the argued vector
	// of vertices. If the collision polygon is already initialized,
	// it will be freed, and this new one will take its place.
	void SetCollisionPolygon(std::vector<Space::Point2D<double>>& vertices);
	

	/** Misc Methods **/

	// Physics update for this entity
	// Overrides should update position and orientation using
	// the SetPosition, SetOrientation, and Turn methods
	virtual void ConstUpdate() = 0; 

	// Check for collisions with other entities
	// Not currently used, as projectiles check for collision
	// using code in the Damager interface
	virtual void CollisionUpdate() {};

	// Non-Constant update for this entity
	// Used for animation updates and for AI updates
	// in intelligent derivative classes
	virtual void Update(double deltaT);

	// Handler for Telegram structs
	// Meant to be overridden by derivative classes that take
	// and/or send messages
	virtual void HandleMessage(const Telegram& message) {}

	// Computes and returns the square distance between this and
	// the argued entity
	double SqrDistance(const Entity* other) const
	{
		return m_position.SqrDistance(other->m_position);
	}

	// Returns true if this entity's collision polygon overlaps
	// with the argued entity's. False otherwise.
	virtual bool CollidesWith(const Entity* other) const;

	// Roughly predicts the point at which it would be possible for an
	// entity at the argued position with the argued speed to collide 
	// with this entity
	// Meant to be overridden by moveable derivatives of Entity
	virtual Space::Point2D<double> ProjectCollisionPoint(
		const Space::Point2D<double>& colliderPos,
		double colliderSpeed) const
	{
		return m_position;
	}

	// More accurate prediction of a possible collision point for an
	// entity with the argued position and speed to collide with this one
	// Meant to be overridden by moveable derivatives of Entity
	virtual Space::Point2D<double> ProjectCollisionPointAdv(
		const Space::Point2D<double>& colliderPos,
		double colliderSpeed) const
	{
		return m_position;
	}

	// Renders this entity using its inherited drawable methods
	virtual void Render(float interpolation, Space::Point2D<double>& offset);

	virtual bool CanCollide(const Entity* other) const;

	// Compares entities by ship class
	bool operator<(const Entity& other) const;




protected:

	/** Object used to send messages to other entities **/
	MessageDispatcher* dispatch;

	bool InitializeDrawable(Graphics* pGraphics, int width, int height, int nCols, 
		const char* textureName);

	bool InitializeDrawable(Graphics* pGraphics, const char* textureName) 
	{ 
		return InitializeDrawable(pGraphics, 0, 0, 0, textureName);
	}

	Drawable* GetDrawable() { return m_pDrawable; }

private:

	/** This entity's unique ID number **/
	ID m_ID;

	const char* m_textureName;
	
	/** Sector containing this entity **/
	Sector* m_pSector;

	/** This entity's orientation in radians **/
	double m_dOrientation; 

	/** This entity's position in its sector **/
	Space::Point2D<double> m_position;

	/** This entity's allegience **/
	Faction m_allegience;

	/** The drawble for handling this entity's rendering **/
	Drawable* m_pDrawable;

	/** Whether this entity collides with damagers **/
	bool m_bDamageable;

	/** Whether this entity collides with damageables **/
	bool m_bCollider;

	/** This entity's collision polygon **/
	CollisionPolygon* m_pPolygon;

	// Method for assigning a unique ID number to an entity
	// on construction
	static unsigned int NextValidID()
	{
		static ID nextValidID = 1;
		return nextValidID++;
	}

	// Updates the drawable instance fields using the
	// entity's instance fields
	// Called by Render method before rendering
	virtual void UpdateDrawable(double interpolation,
		Space::Point2D<double>& offset);

};



}

