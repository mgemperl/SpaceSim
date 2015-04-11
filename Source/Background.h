#pragma once

#include "Entity.h"

namespace SpaceSimNS
{

class Background : public Entity
{
public:

	Background();
	~Background();
	
	bool InitializeDrawable(Graphics* pGraphics) override;

	void ConstUpdate() override {}

	void UpdateDrawable(double interpolation, 
		Space::Point2D<double>& offset) override {}
};

}