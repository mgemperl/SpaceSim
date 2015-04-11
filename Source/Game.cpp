// Mostly implemented by Charles Kelly

#include "Game.h"
#include "Clock.h"

// The primary class should inherit from Game class

//=============================================================================
// Constructor
//=============================================================================
Game::Game()
{
    m_pInput = new Input();        // initialize keyboard input immediately
    // additional initialization is handled in later call to input->initialize()
    m_paused = false;             // game is not paused
    m_pGraphics = NULL;
    m_initialized = false;
}

//=============================================================================
// Destructor
//=============================================================================
Game::~Game()
{
    DeleteAll();                // free all reserved memory
    ShowCursor(true);           // show cursor
}

//=============================================================================
// Window message handler
//=============================================================================
LRESULT Game::HandleMessage( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    if(m_initialized)     // do not process messages if not initialized
    {
        switch( msg )
        {
            case WM_DESTROY:
                PostQuitMessage(0);        //tell Windows to kill this program
                return 0;
            case WM_KEYDOWN: case WM_SYSKEYDOWN:    // key down
                m_pInput->KeyDown(wParam);
                return 0;
            case WM_KEYUP: case WM_SYSKEYUP:        // key up
				m_pInput->KeyUp(wParam);
                return 0;
            case WM_CHAR:                           // character entered
				m_pInput->KeyIn(wParam);
                return 0;
            case WM_MOUSEMOVE:                      // mouse moved
				m_pInput->MouseIn(lParam);
                return 0;
            case WM_INPUT:                          // raw mouse data in
				m_pInput->MouseRawIn(lParam);
                return 0;
            case WM_LBUTTONDOWN:                    // left mouse button down
				m_pInput->SetMouseLButton(true);
				m_pInput->MouseIn(lParam);             // mouse position
                return 0;
            case WM_LBUTTONUP:                      // left mouse button up
				m_pInput->SetMouseLButton(false);
				m_pInput->MouseIn(lParam);             // mouse position
                return 0;
            case WM_MBUTTONDOWN:                    // middle mouse button down
				m_pInput->SetMouseMButton(true);
				m_pInput->MouseIn(lParam);             // mouse position
                return 0;
            case WM_MBUTTONUP:                      // middle mouse button up
				m_pInput->SetMouseMButton(false);
				m_pInput->MouseIn(lParam);             // mouse position
                return 0;
            case WM_RBUTTONDOWN:                    // right mouse button down
				m_pInput->SetMouseRButton(true);
				m_pInput->MouseIn(lParam);             // mouse position
                return 0;
            case WM_RBUTTONUP:                      // right mouse button up
				m_pInput->SetMouseRButton(false);
				m_pInput->MouseIn(lParam);             // mouse position
                return 0;
            case WM_XBUTTONDOWN: case WM_XBUTTONUP: // mouse X button down/up
				m_pInput->SetMouseXButton(wParam);
				m_pInput->MouseIn(lParam);             // mouse position
                return 0;
            case WM_DEVICECHANGE:                   // check for controller insert
				m_pInput->CheckControllers();
                return 0;
        }
    }
    return DefWindowProc( hwnd, msg, wParam, lParam );    // let Windows handle it
}

//=============================================================================
// Initializes the game
// throws GameException on error
//=============================================================================
void Game::Initialize(HWND hw)
{
    m_hwnd = hw;                                  // save window handle

    // initialize graphics
    m_pGraphics = new Graphics();
    // throws GameException
    m_pGraphics->Initialize(m_hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

    // initialize input, do not capture mouse
    m_pInput->Initialize(m_hwnd, false);             // throws GameException

	// Initialize the static game clock class
	Clock::Initialize();
	m_timeStart = Clock::GameTimeMillis();

/*
    // attempt to set up high resolution timer
    if(QueryPerformanceFrequency(&m_timerFreq) == false)
        throw(GameException(GameExceptionNS::FATAL_ERROR, "Error initializing high resolution timer"));

    QueryPerformanceCounter(&m_timeStart);        // get starting time
*/

    m_initialized = true;
}

//=============================================================================
// Render game items
//=============================================================================
void Game::RenderGame()
{
    //start rendering
    if (SUCCEEDED(m_pGraphics->BeginScene()))
    {
        // render is a pure virtual function that must be provided in the
        // inheriting class.
		Render();

        //stop rendering
		m_pGraphics->EndScene();
    }
    
	HandleLostGraphicsDevice();

    //display the back buffer on the screen
	m_pGraphics->ShowBackbuffer();
}

//=============================================================================
// Handle lost graphics device
//=============================================================================
void Game::HandleLostGraphicsDevice()
{
    // test for and handle lost device
	m_hr = m_pGraphics->GetDeviceState();
    if(FAILED(m_hr))                  // if graphics device is not in a valid state
    {
        // if the device is lost and not available for reset
        if(m_hr == D3DERR_DEVICELOST)
        {
            Sleep(100);             // yield cpu time (100 mili-seconds)
            return;
        } 
        // the device was lost but is now available for reset
        else if(m_hr == D3DERR_DEVICENOTRESET)
        {
            ReleaseAll();
			m_hr = m_pGraphics->Reset(); // attempt to reset graphics device
            if(FAILED(m_hr))          // if reset failed
                return;
            ResetAll();
        }
        else
            return;                 // other device error
    }
}

//=============================================================================
// Call repeatedly by the main message loop in WinMain
//=============================================================================
void Game::Run(HWND hwnd)
{
	if (m_pGraphics == NULL)            // if graphics not initialized
        return;

    // calculate elapsed time of last frame, save in frameTime
	m_timeEnd = Clock::GameTimeMillis();
	m_frameTime = (m_timeEnd - m_timeStart) / 1000.0;

    // Power saving code, requires winmm.lib
    // if not enough time has elapsed for desired frame rate
    if (m_frameTime < MIN_FRAME_TIME) 
    {
        m_sleepTime = (DWORD)((MIN_FRAME_TIME - m_frameTime)*1000);
        timeBeginPeriod(1);         // Request 1mS resolution for windows timer
        Sleep(m_sleepTime);           // release cpu for sleepTime
        timeEndPeriod(1);           // End 1mS timer resolution
		m_frameTime = MIN_FRAME_TIME;
    }
	else
	{
		if (m_frameTime > 0.0)
			m_fps = (m_fps*0.99f) + (0.01f / m_frameTime);  // average fps

		if (m_frameTime > MAX_FRAME_TIME) // if frame rate is very slow
			m_frameTime = MAX_FRAME_TIME; // limit maximum frameTime

		m_timeStart = m_timeEnd;

		UpdateTime(m_frameTime);
		RenderGame();                   // draw all game items

		// Update game if not paused
		if (!m_paused)
		{
			Update(m_frameTime);
			m_pInput->vibrateControllers(m_frameTime); // handle controller vibration
		}

		m_pInput->ReadControllers();       // read state of controllers


		// Clear input
		// Call this after all key checks are done
		m_pInput->Clear(InputNS::KEYS_PRESSED);
	}
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Game::ReleaseAll()
{}

//=============================================================================
// Recreate all surfaces and reset all entities.
//=============================================================================
void Game::ResetAll()
{}

//=============================================================================
// Delete all reserved memory
//=============================================================================
void Game::DeleteAll()
{
    ReleaseAll();               // call onLostDevice() for every graphics item
	SAFE_DELETE(m_pGraphics);
    SAFE_DELETE(m_pInput);
    m_initialized = false;
}