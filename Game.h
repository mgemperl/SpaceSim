#ifndef _GAME_H                 // prevent multiple definitions if this 
#define _GAME_H                 // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Mmsystem.h>
#include "Graphics.h"
#include "Input.h"
#include "Data.h"
#include "GameException.h"

class Game
{
protected:
    // common game properties
    Graphics* m_pGraphics;         // pointer to Graphics
    Input* m_pInput;             // pointer to Input
    HWND m_hwnd;               // window handle
    HRESULT m_hr;                 // standard return type
    LARGE_INTEGER m_timeStart;    // Performance Counter start value
    LARGE_INTEGER m_timeEnd;      // Performance Counter end value
    LARGE_INTEGER m_timerFreq;    // Performance Counter frequency
    float m_frameTime;          // time required for last frame
    float m_fps;                // frames per second
    DWORD m_sleepTime;          // number of milli-seconds to sleep between frames
    bool m_paused;             // true if game is paused
    bool m_initialized;

public:
    // Constructor
    Game();
    // Destructor
    virtual ~Game();

    // Member functions

    // Window message handler
    LRESULT HandleMessage( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    // Initialize the game
    // Pre: hwnd is handle to window
    virtual void Initialize(HWND hwnd);

    // Call run repeatedly by the main message loop in WinMain
    virtual void Run(HWND);

    // Call when the graphics device was lost.
    // Release all reserved video memory so graphics device may be reset.
    virtual void ReleaseAll();

    // Recreate all surfaces and reset all entities.
    virtual void ResetAll();

    // Delete all reserved memory.
    virtual void DeleteAll();

    // Render game items.
    virtual void RenderGame();

    // Handle lost graphics device
    virtual void HandleLostGraphicsDevice();

    // Return pointer to Graphics.
    Graphics* GetGraphics() 
	{
		return m_pGraphics;
	}

    // Return pointer to Input.
    Input* GetInput()       
	{
		return m_pInput;
	}

    // Exit the game
    void ExitGame()         
	{
		PostMessage(m_hwnd, WM_DESTROY, 0, 0);
	}

    // Pure virtual function declarations
    // These functions MUST be written in any class that inherits from Game

    // Update game items.
	virtual void Update(double delta) = 0;

	virtual void Render() = 0;

private:

	virtual void UpdateTime(double elapsedTime) = 0;

};

#endif
