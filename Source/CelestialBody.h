#pragma once

#include "Entity.h"

namespace SpaceSimNS
{

class System;

class CelestialBody : public Entity 
{
public:

	CelestialBody(double bodyRadius,
		System* pSystem, 
		double orbitRadius, 
		double initTheta);

	~CelestialBody();

	bool InitializeDrawable(Graphics* pGraphics) override;

	virtual bool Intact() const {return true;}

	virtual void HandleMessage(const Telegram& message) {}

	void ConstUpdate() override {}
	void Update(double deltaT) override 
	{
		//SetOrientation(GetOrientationRad() + 0.02 * deltaT);
	}

	System* GetSystem() const;

	double GetOrbitRadius() const;

	double GetOrbitTheta() const;

private:

	System* m_pSystem;
	double m_OrbitRadius;
	double m_Theta; // angle along its orbit

};

}