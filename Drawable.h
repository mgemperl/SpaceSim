#pragma once

#include "Graphics.h"

class TextureManager;

class Drawable
{
public:

	/** Initialization and destruction methods **/

	Drawable();
	~Drawable();

	/** Initialize this drawable with the texture manager that was assigned
	    the argued name **/
	bool Initialize(Graphics* pGraphics, int width, int height, int nCols,
		const char* textureName);

	/** Accessor methods **/

	// Accessor for sprite data struct
	const virtual SpriteData& GetSpriteInfo() { return m_spriteData; }

	// Accessor for whether drawable is visible
	virtual bool IsVisible() { return m_bVisible; }

	// Accessor for x coordinate of this drawable's sprite
	virtual float GetX() { return m_spriteData.x; }

	// Accessor for y coordinate of this drawable's sprite
	virtual float GetY() { return m_spriteData.y; }

	// Accessor for the scale of this drawable's sprite
	virtual float GetScale() { return m_spriteData.scale; }

	// Accessor for the width of this drawable's sprite
	virtual int GetWidth() { return m_spriteData.width;	}

	// Accessor for the height of this drawable's sprite
	virtual int GetHeight() { return m_spriteData.height; }

	// Accessor for this drawable's sprite's center's x coordinate
	virtual float GetCenterX() 
	{ 
		return m_spriteData.x + m_spriteData.width / 2.0f * GetScale(); 
	}

	// Accessor for this drawable's sprite's center's y coordinate
	virtual float GetCenterY()
	{
		return m_spriteData.y + m_spriteData.height / 2.0f * GetScale();
	}

	// Accessor for the angle of this drawable's sprite in radians
	virtual float GetAngle() { return m_spriteData.angle; }

	// Accessor for the angle of this drawable's sprite in degrees
	virtual float GetAngleDegrees() { return m_spriteData.angle * (180.0f / PI); }

	// Accessor for the delay in seconds between this sprites flipbook pages
	virtual float GetFlipDelay() { return m_fFlipDelay; }

	// Accessor for the first page in this drawable's animation flipbook
	virtual int GetFirstPage() { return m_iFirstPage; }

	// Accessor for the last page in this drawable's animation flipbook
	virtual int GetLastPage() { return m_iLastPage; }

	// Accessor for the currect page of this drawable's animation flipbook
	virtual int GetCurrentPage() { return m_iCurrentPage; }

	// Accessor for this drawable's sprite's data rect
	virtual RECT GetSpriteDataRect() { return m_spriteData.rect; }

	// Accessor for
	virtual bool IsOnLastPage() { return m_bOnLastPage; }


	/** Updater Methods **/

	// Updates the sprite's data rect to draw the current page of the flipbook
	virtual void UpdateRect();

	// Called once per frame to update the animation if enough time has elapsed
	virtual void UpdateAnimation(float deltaT);

	/** Draw methods **/

	// Draw using the color argument as the filter
	virtual void Draw(COLOR_ARGB color = WHITE);

	// Draw using the argued sprite data	
	// virtual void Draw(SpriteData data, COLOR_ARGB color = WHITE);
	
	/** Setter Methods **/

	// Sets the x coordinate of this drawable's sprite
	virtual void SetX(float neoX) { m_spriteData.x = neoX; }

	// Sets the y coordinate of this drawable's sprite
	virtual void SetY(float neoY) { m_spriteData.y = neoY; }

	// Sets the scale of this drawable's sprite
	virtual void SetScale(float neoScale) { m_spriteData.scale = neoScale; }

	// Sets the angle of this drawable's sprite in radians
	virtual void SetAngle(float theta) { m_spriteData.angle = theta; }

	// Sets whether this drawable is visible
	virtual void SetVisible(bool visible) { m_bVisible = visible; }

	// Sets the delay in seconds between flipbook page transitions
	virtual void SetFlipDelay(float delay) { m_fFlipDelay = delay; }

	// Sets the first and last page of this drawable's animation flipbook
	virtual void SetFlipbookPages(int firstPage, int lastPage)
	{
		m_iFirstPage = firstPage;
		m_iLastPage = lastPage;
	}

	// Sets the drawable to the argued page of its animation flipbook
	virtual void SetCurrentPage(int page);

	// Sets this drawable's sprite's data rect to the argued rect
	virtual void SetSpriteDataRect(RECT rect) { m_spriteData.rect = rect; }

	// Set whether to cycle through an animation flipbook
	virtual void SetFlip(bool toFlipOrNotToFlip) { m_bFlip = toFlipOrNotToFlip; }

	// Set whether the flipbook is on the last page	
	virtual void SetOnLastPage(bool onLastPage) { m_bOnLastPage = onLastPage; }

	// Set the color filter (WHITE for no change)
	virtual void SetColorFilter(COLOR_ARGB color) { m_colorFilter = color; }

	// Set this drawable's texture manager
	virtual void SetTextureManager(TextureManager* pTextureMan)
	{
		m_pTextureManager = pTextureMan;
	}

private:

	Graphics* m_pGraphics;
	TextureManager* m_pTextureManager;
	SpriteData m_spriteData;

	COLOR_ARGB m_colorFilter;
	int m_nCols;

	int m_iFirstPage;	// First page of an animation flipbook
	int m_iLastPage;	// Last page of an animation flipbook
	int m_iCurrentPage;	// Current page in animation flipbook
	float m_fFlipDelay;	// Delay in seconds until next page of 
						// flipbook is displayed
	float m_fFlipTimer;	// Timger used to flip the flipbook

	bool m_bFlip;		// Boolean for whether to flip the flipbook
	bool m_bVisible;	// Boolean for whether the image should be visible
	bool m_bInitialized;	// Boolean for whether the image is initialized
	bool m_bOnLastPage;	// Boolean for whether the flipbook is on its
						// last page

	int m_iLayer;		// The layer in which this drawable's sprite will
						// be drawn. Higher values are further from the
						// camera.

	bool Initialize(Graphics* pGraphics, int width, int height, int nCols,
		TextureManager* pManager);
	
};

