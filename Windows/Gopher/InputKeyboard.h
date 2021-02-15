#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cmath> // for abs()
#include <map>
#include <windows.h> // for Beep()
#include <tchar.h>
#include <ShlObj.h>
#include <xinput.h> // controller
#include <stdio.h> // for printf
#include <mmdeviceapi.h> // vol
#include <endpointvolume.h> // vol

#pragma once

void InputKeyboard(WORD cmd, DWORD flag);
void InputKeyboardDown(WORD cmd);
void InputKeyboardUp(WORD cmd);
void MouseEvent(DWORD dwFlags, DWORD mouseData = 0);