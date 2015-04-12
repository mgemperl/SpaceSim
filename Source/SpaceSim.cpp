#include "SpaceSim.h"
#include "GameException.h"
#include "Universe.h"
#include <string>

#include "DrawableLine.h"
#include "Point2D.h"
using namespace Space;

//=============================================================================
// Constructor
//=============================================================================
SpaceSim::SpaceSim()
{
	m_pUniverse = new SpaceSimNS::Universe();
}

//=============================================================================
// Destructor
//=============================================================================
SpaceSim::~SpaceSim()
{
	delete m_pUniverse;
    ReleaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameException on error
//=============================================================================
void SpaceSim::Initialize(HWND hwnd)
{
    Game::Initialize(hwnd); // throws GameException

	// Set up nebula texture
	if (!TextureManager::Initialize(m_pGraphics, "Textures/nebula_01.jpg", "nebula"))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing nebula texture");
	}

	// Set up ship texture
	if (!TextureManager::Initialize(m_pGraphics, "Textures/Hammerhead.png", "ship"))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing ship texture");
	}

	// set up planet texture
	if (!TextureManager::Initialize(m_pGraphics, "Textures/planet_02_alpha.png", "planet"))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing planet texture");
	}

	if (!TextureManager::Initialize(m_pGraphics, "Textures/laser.png", "laser"))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing laser texture");
	}

	if (!TextureManager::Initialize(m_pGraphics, "Textures/gun.png", "gun"))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing gun texture");
	}

	if (!TextureManager::Initialize(m_pGraphics, "Textures/missile.png", "missile"))
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing missile texture");
	}

	m_pUniverse->Initialize(m_pGraphics, m_pInput);

	/*
	// Set up nebula drawable
	if (!nebula.Initialize(m_pGraphics, 0, 0, 0, "nebula"))
	{
		throw(GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing nebula"));
	}

	nebula.SetScale((float)min(GAME_WIDTH, GAME_HEIGHT) / (float)min(nebula.GetWidth(), nebula.GetHeight()));  

	// Set up ship drawable
	if (!ship.Initialize(m_pGraphics, 0, 0, 0, "planet"))
	{
		throw(GameException(GameExceptionNS::FATAL_ERROR,
			"Error initializing ship"));
	}

	ship.SetScale(0.25);
	ship.SetX(GAME_WIDTH * 0.5f - ship.GetWidth() * 0.5f * ship.GetScale());
	ship.SetY(GAME_HEIGHT * 0.5f - ship.GetHeight() * 0.5f * ship.GetScale());
	ship.SetAngle(QUARTER_PI);
	*/
}

void SpaceSim::UpdateTime(double delta)
{
	m_pUniverse->UpdateTime(delta);
}

//=============================================================================
// Update all game items
//=============================================================================
void SpaceSim::Update(double delta)
{
	m_pUniverse->Update(delta);
}

//=============================================================================
// Render game items
//=============================================================================
void SpaceSim::Render()
{
	m_pGraphics->SpriteBegin();
	m_pUniverse->Render();
	m_pGraphics->SpriteEnd();

	m_pGraphics->PolygonBegin();
	DrawableLine line = DrawableLine();
	line.Initialize(m_pGraphics);
	line.SetVertices(Point2D<float>(0, 0), Point2D<float>(400, 400));
	line.Draw();
	m_pGraphics->PolygonEnd();
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void SpaceSim::ReleaseAll()
{
    Game::ReleaseAll();
	m_pGraphics->OnLostDevice();
	Game::ReleaseAll();
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void SpaceSim::ResetAll()
{
    Game::ResetAll();
	m_pGraphics->OnResetDevice();
	Game::ResetAll();
}
