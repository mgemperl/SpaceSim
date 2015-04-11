#include "Background.h"
#include "Drawable.h"
#include "TextureManager.h"

#define TEXTURE "nebula"

namespace SpaceSimNS
{

	Background::Background() : Entity(Space::Point2D<double>(0,0))
{
}


Background::~Background()
{
}

bool Background::InitializeDrawable(Graphics* pGraphics)
{
	bool success = Entity::InitializeDrawable(pGraphics, TEXTURE);

	if (success)
	{
		TextureManager* pTexture = pGraphics->RetrieveTexture(TEXTURE);
		Drawable* pDrawable = GetDrawable();

		pDrawable->SetScale(((float)min(GAME_WIDTH, GAME_HEIGHT)) /
			(float)min(pTexture->GetWidth(), pTexture->GetHeight()));

		pDrawable->SetX(0);
		pDrawable->SetY(0);
	}

	return success;
}

}