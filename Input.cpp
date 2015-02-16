// Mostly implemented by Charles Kelly

#include "Input.h"

//=============================================================================
// default constructor
//=============================================================================
Input::Input()
{
    // clear key down array
    for (size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
        m_aKeysDown[i] = false;
    // clear key pressed array
    for (size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
        m_aKeysPressed[i] = false;
	m_bNewLine = true;                     // start new line
    m_textIn = "";                        // clear textIn
    m_charIn = 0;                         // clear charIn

    // mouse data
	m_mouseX = 0;                         // screen X
	m_mouseY = 0;                         // screen Y
    m_mouseRawX = 0;                      // high-definition X
    m_mouseRawY = 0;                      // high-definition Y
    m_bMouseLButton = false;               // true if left mouse button is down
    m_bMouseMButton = false;               // true if middle mouse button is down
    m_bMouseRButton = false;               // true if right mouse button is down
    m_bMouseX1Button = false;              // true if X1 mouse button is down
    m_bMouseX2Button = false;              // true if X2 mouse button is down

    for(int i=0; i<MAX_CONTROLLERS; i++)
    {
		m_aControllers[i].vibrateTimeLeft = 0;
		m_aControllers[i].vibrateTimeRight = 0;
    }
}

//=============================================================================
// destructor
//=============================================================================
Input::~Input()
{
    if(m_bMouseCaptured)
        ReleaseCapture();               // release mouse
}

//=============================================================================
// Initialize mouse and controller input
// Set capture=true to capture mouse
// Throws GameException
//=============================================================================
void Input::Initialize(HWND hwnd, bool capture)
{
    try{
        m_bMouseCaptured = capture;

        // register high-definition mouse
        Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC; 
        Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE; 
        Rid[0].dwFlags = RIDEV_INPUTSINK;   
        Rid[0].hwndTarget = hwnd;
        RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

        if(m_bMouseCaptured)
            SetCapture(hwnd);           // capture mouse

        // Clear controllers state
		ZeroMemory(m_aControllers, sizeof(ControllerState) * MAX_CONTROLLERS);

        CheckControllers();             // check for connected controllers
    }
    catch(...)
    {
        throw(GameException(GameExceptionNS::FATAL_ERROR, "Error initializing input system"));
    }
}

//=============================================================================
// Set true in the keysDown and keysPessed array for this key
// Pre: wParam contains the virtual key code (0--255)
//=============================================================================
void Input::KeyDown(WPARAM wParam)
{
    // make sure key code is within buffer range
    if (wParam < InputNS::KEYS_ARRAY_LEN)
    {
        m_aKeysDown[wParam] = true;    // update keysDown array
        // key has been "pressed, erased by clear()
        m_aKeysPressed[wParam] = true; // update keysPressed arrayK
    }
}

//=============================================================================
// Set false in the keysDown array for this key
// Pre: wParam contains the virtual key code (0--255)
//=============================================================================
void Input::KeyUp(WPARAM wParam)
{
    // make sure key code is within buffer range
    if (wParam < InputNS::KEYS_ARRAY_LEN)
        // update state table
        m_aKeysDown[wParam] = false;
}

//=============================================================================
// Save the char just entered in textIn string
// Pre: wParam contains the char
//=============================================================================
void Input::KeyIn(WPARAM wParam)
{
    if (m_bNewLine)                            // if start of new line
    {
        m_textIn.clear();
        m_bNewLine = false;
    }

    if (wParam == '\b')                     // if backspace
    {
        if(m_textIn.length() > 0)             // if characters exist
            m_textIn.erase(m_textIn.size()-1);  // erase last character entered
    }
    else
    {
        m_textIn += wParam;                   // add character to textIn
        m_charIn = wParam;                    // save last char entered
    }

    if ((char)wParam == '\r')               // if return    
        m_bNewLine = true;                     // start new line
}

//=============================================================================
// Returns true if the specified VIRTUAL KEY is down, otherwise false.
//=============================================================================
bool Input::IsKeyDown(UCHAR vkey) const
{
    if (vkey < InputNS::KEYS_ARRAY_LEN)
        return m_aKeysDown[vkey];
    else
        return false;
}

//=============================================================================
// Return true if the specified VIRTUAL KEY has been pressed in the most recent
// frame. Key presses are erased at the end of each frame.
//=============================================================================
bool Input::WasKeyPressed(UCHAR vkey) const
{
    if (vkey < InputNS::KEYS_ARRAY_LEN)
        return m_aKeysPressed[vkey];
    else
        return false;
}

//=============================================================================
// Return true if any key was pressed in the most recent frame.
// Key presses are erased at the end of each frame.
//=============================================================================
bool Input::AnyKeyPressed() const
{
    for (size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
        if(m_aKeysPressed[i] == true)
            return true;
    return false;
}

//=============================================================================
// Clear the specified key press
//=============================================================================
void Input::ClearKeyPress(UCHAR vkey)
{
    if (vkey < InputNS::KEYS_ARRAY_LEN)
        m_aKeysPressed[vkey] = false;
}

//=============================================================================
// Clear specified input buffers
// See input.h for what values
//=============================================================================
void Input::Clear(UCHAR what)
{
    if(what & InputNS::KEYS_DOWN)       // if clear keys down
    {
        for (size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
            m_aKeysDown[i] = false;
    }
    if(what & InputNS::KEYS_PRESSED)    // if clear keys pressed
    {
        for (size_t i = 0; i < InputNS::KEYS_ARRAY_LEN; i++)
            m_aKeysPressed[i] = false;
    }
    if(what & InputNS::MOUSE)           // if clear mouse
    {
        m_mouseX = 0;
        m_mouseY = 0;
        m_mouseRawX = 0;
        m_mouseRawY = 0;
    }
    if(what & InputNS::TEXT_IN)
        ClearTextIn();
}

//=============================================================================
// Reads mouse screen position into mouseX, mouseY
//=============================================================================
void Input::MouseIn(LPARAM lParam)
{
    m_mouseX = GET_X_LPARAM(lParam); 
    m_mouseY = GET_Y_LPARAM(lParam);
}

//=============================================================================
// Reads raw mouse data into m_mouseRawX, m_mouseRawY
// This routine is compatible with a high-definition mouse
//=============================================================================
void Input::MouseRawIn(LPARAM lParam)
{
    UINT dwSize = 40;
    static BYTE lpb[40];
    
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, 
                    lpb, &dwSize, sizeof(RAWINPUTHEADER));
    
    RAWINPUT* raw = (RAWINPUT*)lpb;
    
    if (raw->header.dwType == RIM_TYPEMOUSE) 
    {
        m_mouseRawX = raw->data.mouse.lLastX;
        m_mouseRawY = raw->data.mouse.lLastY;
    } 
}

//=============================================================================
// Check for connected controllers
//=============================================================================
void Input::CheckControllers()
{
    DWORD result;
    for( DWORD i = 0; i <MAX_CONTROLLERS; i++)
    {
        result = XInputGetState(i, &m_aControllers[i].state);
        if(result == ERROR_SUCCESS)
            m_aControllers[i].connected = true;
        else
            m_aControllers[i].connected = false;
    }
}

//=============================================================================
// Read state of connected controllers
//=============================================================================
void Input::ReadControllers()
{
    DWORD result;
    for( DWORD i = 0; i <MAX_CONTROLLERS; i++)
    {
        if(m_aControllers[i].connected)
        {
            result = XInputGetState(i, &m_aControllers[i].state);
            if(result == ERROR_DEVICE_NOT_CONNECTED)    // if controller disconnected
                m_aControllers[i].connected = false;
        }
    }
}

//=============================================================================
// Vibrate connected controllers
//=============================================================================
void Input::vibrateControllers(float frameTime)
{
    for(int i=0; i < MAX_CONTROLLERS; i++)
    {
        if(m_aControllers[i].connected)
        {
			m_aControllers[i].vibrateTimeLeft -= frameTime;
			if (m_aControllers[i].vibrateTimeLeft < 0)
            {
				m_aControllers[i].vibrateTimeLeft = 0;
				m_aControllers[i].vibration.wLeftMotorSpeed = 0;
            }
			m_aControllers[i].vibrateTimeRight -= frameTime;
			if (m_aControllers[i].vibrateTimeRight < 0)
            {
				m_aControllers[i].vibrateTimeRight = 0;
				m_aControllers[i].vibration.wRightMotorSpeed = 0;
            }
			XInputSetState(i, &m_aControllers[i].vibration);
        }
    }
}
