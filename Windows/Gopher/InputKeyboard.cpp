#include "inputKeyboard.h"

// Description:
//   Send a keyboard input to the system based on the key value
//     and its event type.
// Params:
//   cmd    The value of the key to send(see http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx)
//   flag   The KEYEVENT for the key
void InputKeyboard(WORD cmd, DWORD flag)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = cmd;
	input.ki.dwFlags = flag;
	SendInput(1, &input, sizeof(INPUT));
}

// Description:
//   Send a keyboard input based on the key value with the "pressed down" event.
// Params:
//   cmd    The value of the key to send
void InputKeyboardDown(WORD cmd)
{
	InputKeyboard(cmd, 0);
}

// Description:
//   Send a keyboard input based on the key value with the "released" event
//
// Params:
//   cmd    The value of the key to send
void InputKeyboardUp(WORD cmd)
{
	InputKeyboard(cmd, KEYEVENTF_KEYUP);
}