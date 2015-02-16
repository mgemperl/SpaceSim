#ifndef _INPUT_H   
#define _INPUT_H 
#define WIN32_LEAN_AND_MEAN

class Input;

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>
#include "Data.h"
#include "GameException.h"


// for high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif
//--------------------------

namespace InputNS
{
    const int KEYS_ARRAY_LEN = 256;     // size of key arrays
    
    // what values for clear(), bit flag
    const UCHAR KEYS_DOWN = 1;
    const UCHAR KEYS_PRESSED = 2;
    const UCHAR MOUSE = 4;
    const UCHAR TEXT_IN = 8;
    const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0X7FFF);    // default to 20% of range as deadzone
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;                      // trigger range 0-255
const DWORD MAX_CONTROLLERS = 4;                                // Maximum number of controllers supported by XInput

// Bit corresponding to gamepad button in state.Gamepad.wButtons
const DWORD GAMEPAD_DPAD_UP        = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN      = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT      = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT     = 0x0008;
const DWORD GAMEPAD_START_BUTTON   = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON    = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB     = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB    = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER  = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A              = 0x1000;
const DWORD GAMEPAD_B              = 0x2000;
const DWORD GAMEPAD_X              = 0x4000;
const DWORD GAMEPAD_Y              = 0x8000;

struct ControllerState
{
    XINPUT_STATE        state;
    XINPUT_VIBRATION    vibration;
    float               vibrateTimeLeft;    // mSec
    float               vibrateTimeRight;   // mSec
    bool                connected;
};

class Input
{
private:
    bool m_aKeysDown[InputNS::KEYS_ARRAY_LEN];     // true if specified key is down
    bool m_aKeysPressed[InputNS::KEYS_ARRAY_LEN];  // true if specified key was pressed
    std::string m_textIn;                         // user entered text
    char m_charIn;                                // last character entered
    bool m_bNewLine;                               // true on start of new line
    int  m_mouseX, m_mouseY;                        // mouse screen coordinates
    int  m_mouseRawX, m_mouseRawY;                  // high-definition mouse data
    RAWINPUTDEVICE Rid[1];                      // for high-definition mouse
    bool m_bMouseCaptured;                         // true if mouse captured
    bool m_bMouseLButton;                          // true if left mouse button down
    bool m_bMouseMButton;                          // true if middle mouse button down
    bool m_bMouseRButton;                          // true if right mouse button down
    bool m_bMouseX1Button;                         // true if X1 mouse button down
    bool m_bMouseX2Button;                         // true if X2 mouse button down
    ControllerState m_aControllers[MAX_CONTROLLERS];    // state of controllers

public:
    // Constructor
    Input();

    // Destructor
    virtual ~Input();

    // Initialize mouse and controller input.
    // Throws GameException
    // Pre: hwnd = window handle
    //      capture = true to capture mouse.
    void Initialize(HWND hwnd, bool capture);

    // Save key down state
    void KeyDown(WPARAM);

    // Save key up state
    void KeyUp(WPARAM);

    // Save the char just entered in textIn string
    void KeyIn(WPARAM);

    // Returns true if the specified VIRTUAL KEY is down, otherwise false.
    bool IsKeyDown(UCHAR vkey) const;

    // Return true if the specified VIRTUAL KEY has been pressed in the most recent frame.
    // Key presses are erased at the end of each frame.
    bool WasKeyPressed(UCHAR vkey) const;

    // Return true if any key was pressed in the most recent frame.
    // Key presses are erased at the end of each frame.
    bool AnyKeyPressed() const;

    // Clear the specified key press
    void ClearKeyPress(UCHAR vkey);

    // Clear specified input buffers where what is any combination of
    // KEYS_DOWN, KEYS_PRESSED, MOUSE, TEXT_IN or KEYS_MOUSE_TEXT.
    // Use OR '|' operator to combine parmeters.
    void Clear(UCHAR what);

    // Clears key, mouse and text input data
    void ClearAll() {Clear(InputNS::KEYS_MOUSE_TEXT);}

    // Clear text input buffer
    void ClearTextIn() {m_textIn.clear();}

    // Return text input as a string
    std::string GetTextIn() {return m_textIn;}

    // Return last character entered
    char GetCharIn()        {return m_charIn;}

    // Reads mouse screen position into mouseX, mouseY
    void MouseIn(LPARAM);

    // Reads raw mouse data into mouseRawX, mouseRawY
    // This routine is compatible with a high-definition mouse
    void MouseRawIn(LPARAM);

    // Save state of mouse button
    void SetMouseLButton(bool b) { m_bMouseLButton = b; }

    // Save state of mouse button
	void SetMouseMButton(bool b) { m_bMouseMButton = b; }

    // Save state of mouse button
	void SetMouseRButton(bool b) { m_bMouseRButton = b; }

    // Save state of mouse button
	void SetMouseXButton(WPARAM wParam) {
		m_bMouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		m_bMouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}
    // Return mouse X position
	int  GetMouseX()        const { return m_mouseX; }

    // Return mouse Y position
	int  GetMouseY()        const { return m_mouseY; }

    // Return raw mouse X movement. Left is <0, Right is >0
    // Compatible with high-definition mouse.
	int  GetMouseRawX()     const { return m_mouseRawX; }

    // Return raw mouse Y movement. Up is <0, Down is >0
    // Compatible with high-definition mouse.
	int  GetMouseRawY()     const { return m_mouseRawY; }

    // Return state of left mouse button.
	bool GetMouseLButton()  const { return m_bMouseLButton; }

    // Return state of middle mouse button.
	bool GetMouseMButton()  const { return m_bMouseMButton; }

    // Return state of right mouse button.
	bool GetMouseRButton()  const { return m_bMouseRButton; }

    // Return state of X1 mouse button.
	bool GetMouseX1Button() const { return m_bMouseX1Button; }

    // Return state of X2 mouse button.
	bool GetMouseX2Button() const { return m_bMouseX2Button; }

    // Update connection status of game controllers.
    void CheckControllers();

    // Save input from connected game controllers.
    void ReadControllers();

    // Return state of specified game controller.
    const ControllerState* GetControllerState(UINT n)
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return &m_aControllers[n];
    }

    // Return state of controller n buttons.
    const WORD GetGamepadButtons(UINT n)
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return m_aControllers[n].state.Gamepad.wButtons;
    }

    // Return state of controller n D-pad Up
    bool GetGamepadDPadUp(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return ((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_UP) != 0);
    }

    // Return state of controller n D-pad Down.
    bool GetGamepadDPadDown(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return ((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_DOWN) != 0);
    }

    // Return state of controller n D-pad Left.
    bool GetGamepadDPadLeft(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return ((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_LEFT) != 0);
    }

    // Return state of controller n D-pad Right.
    bool GetGamepadDPadRight(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_DPAD_RIGHT) != 0);
    }

    // Return state of controller n Start button.
    bool GetGamepadStart(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_START_BUTTON) != 0);
    }

    // Return state of controller n Back button.
    bool GetGamepadBack(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_BACK_BUTTON) != 0);
    }

    // Return state of controller n Left Thumb button.
    bool GetGamepadLeftThumb(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_THUMB) != 0);
    }

    // Return state of controller n Right Thumb button.
    bool GetGamepadRightThumb(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_THUMB) != 0);
    }

    // Return state of controller n Left Shoulder button.
    bool GetGamepadLeftShoulder(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_LEFT_SHOULDER) != 0);
    }

    // Return state of controller n Right Shoulder button.
    bool GetGamepadRightShoulder(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_RIGHT_SHOULDER) != 0);
    }

    // Return state of controller n A button.
    bool GetGamepadA(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_A) != 0);
    }

    // Return state of controller n B button.
    bool GetGamepadB(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_B) != 0);
    }

    // Return state of controller n X button.
    bool GetGamepadX(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_X) != 0);
    }

    // Return state of controller n Y button.
    bool GetGamepadY(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return bool((m_aControllers[n].state.Gamepad.wButtons&GAMEPAD_Y) != 0);
    }

    // Return value of controller n Left Trigger.
    BYTE GetGamepadLeftTrigger(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return (m_aControllers[n].state.Gamepad.bLeftTrigger);
    }

    // Return value of controller n Right Trigger.
    BYTE GetGamepadRightTrigger(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return (m_aControllers[n].state.Gamepad.bRightTrigger);
    }

    // Return value of controller n Left Thumbstick X.
    SHORT GetGamepadThumbLX(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return (m_aControllers[n].state.Gamepad.sThumbLX);
    }

    // Return value of controller n Left Thumbstick Y.
    SHORT GetGamepadThumbLY(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return (m_aControllers[n].state.Gamepad.sThumbLY);
    }

    // Return value of controller n Right Thumbstick X.
    SHORT GetGamepadThumbRX(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return (m_aControllers[n].state.Gamepad.sThumbRX);
    }

    // Return value of controller n Right Thumbstick Y.
    SHORT GetGamepadThumbRY(UINT n) 
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        return (m_aControllers[n].state.Gamepad.sThumbRY);
    }

    // Vibrate controller n left motor.
    // Left is low frequency vibration.
    // speed 0=off, 65536=100 percent
    // sec is time to vibrate in seconds
    void GamePadVibrateLeft(UINT n, WORD speed, float sec)
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        m_aControllers[n].vibration.wLeftMotorSpeed = speed;
        m_aControllers[n].vibrateTimeLeft = sec;
    }

    // Vibrate controller n right motor.
    // Right is high frequency vibration.
    // speed 0=off, 65536=100 percent
    // sec is time to vibrate in seconds
    void GamePadVibrateRight(UINT n, WORD speed, float sec)
    {
        if(n > MAX_CONTROLLERS-1)
            n=MAX_CONTROLLERS-1;
        m_aControllers[n].vibration.wRightMotorSpeed = speed;
        m_aControllers[n].vibrateTimeRight = sec;
    }

    // Vibrates the connected controllers for the desired time.
    void vibrateControllers(float frameTime);
};

#endif

