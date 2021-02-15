#include "MouseInput.h"

// Description:
//   Send a mouse input based on a mouse event type.
//   See https://msdn.microsoft.com/en-us/library/windows/desktop/ms646310(v=vs.85).aspx
// Params:
//   dwFlags    The mouse event to send
//   mouseData  Additional information needed for certain mouse events (Optional)
void MouseEvent(DWORD dwFlags, DWORD mouseData)
{
	INPUT input;
	input.type = INPUT_MOUSE;

	// Only set mouseData when using a supported dwFlags type
	if (dwFlags == MOUSEEVENTF_WHEEL ||
		dwFlags == MOUSEEVENTF_XUP ||
		dwFlags == MOUSEEVENTF_XDOWN ||
		dwFlags == MOUSEEVENTF_HWHEEL)
	{
		input.mi.mouseData = mouseData;
	}
	else
	{
		input.mi.mouseData = 0;
	}

	input.mi.dwFlags = dwFlags;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}