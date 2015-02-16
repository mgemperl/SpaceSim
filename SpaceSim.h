// Mostly implemented by Charles Kelly

#ifndef _SPACEWAR_H             // prevent multiple definitions if this 
#define _SPACEWAR_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "Game.h"
#include "TextureManager.h"
#include "Drawable.h"

namespace SpaceSimNS
{
	class Universe;
}

//=============================================================================
// Create game class
//=============================================================================
class SpaceSim : public Game
{
private:

	/** Universe object containing all game entities and locations **/
	SpaceSimNS::Universe* m_pUniverse;

public:
    // Constructor
	SpaceSim();

    // Destructor
	virtual ~SpaceSim();

    // Initialize the game
    void Initialize(HWND hwnd) override;

	void UpdateTime(double delta) override;

	// Game update
    void Update(double delta) override;  

	// Render the game with the argued interpolation
    void Render() override;

	// Release all game objects
    void ReleaseAll() override;

	// Reset
    void ResetAll() override;
};

#endif
