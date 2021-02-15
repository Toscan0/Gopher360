#include "Gopher.h"

Gopher::Gopher(CXBOXController * controller)
	: _controller(controller)
{
}

void Gopher::LoadConfigFile(std::string fileName)
{
	ConfigFile cfg(fileName);

	//--------------------------------
	// Mouse bindings
	//--------------------------------
	_mouse_left_click = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_LEFT").c_str(), 0, 0);
	_mouse_right_click = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_RIGHT").c_str(), 0, 0);
	_mouse_middle_click = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_MIDDLE").c_str(), 0, 0);
	
	//--------------------------------
	// Gopher bindings
	//--------------------------------
	_console_hide = strtol(cfg.getValueOfKey<std::string>("CONFIG_HIDE").c_str(), 0, 0);
	_disable_gopher = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE").c_str(), 0, 0);
	_disable_vibration = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE_VIBRATION").c_str(), 0, 0);
	CONFIG_SPEED_CHANGE = strtol(cfg.getValueOfKey<std::string>("CONFIG_SPEED_CHANGE").c_str(), 0, 0);
	_disable_on_screen_keyboard = strtol(cfg.getValueOfKey<std::string>("CONFIG_OSK").c_str(), 0, 0);

	//--------------------------------
	// Controller bindings
	//--------------------------------
	_gamepad_left_thumb = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_THUMB").c_str(), 0, 0);
	_gamepad_right_thumb = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_THUMB").c_str(), 0, 0);
	_gamepad_dpad_up = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_UP").c_str(), 0, 0);
	_gamepad_dpad_down = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_DOWN").c_str(), 0, 0);
	_gamepad_dpad_left = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_LEFT").c_str(), 0, 0);
	_gamepad_dpad_right = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_RIGHT").c_str(), 0, 0);
	_gamepad_A = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_A").c_str(), 0, 0);
	_gamepad_B = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_B").c_str(), 0, 0);
	_gamepad_X = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_X").c_str(), 0, 0);
	_gamepad_Y = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_Y").c_str(), 0, 0);
	_gamepad_left_shoulder = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_SHOULDER").c_str(), 0, 0);
	_gamepad_right_shoulder = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_SHOULDER").c_str(), 0, 0);
	_gamepad_trigger_left = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_LEFT").c_str(), 0, 0);
	_gamepad_trigger_right = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_RIGHT").c_str(), 0, 0);
	_gamepad_start = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_START").c_str(), 0, 0);
	_gamepad_back = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_BACK").c_str(), 0, 0);

	//--------------------------------
	// Advanced settings
	//--------------------------------

	// Acceleration factor
	_acceleration_factor = strtof(cfg.getValueOfKey<std::string>("ACCELERATION_FACTOR").c_str(), 0);

	// Dead zones
	_mouse_movement_dead_zone = strtol(cfg.getValueOfKey<std::string>("DEAD_ZONE").c_str(), 0, 0);
	if (_mouse_movement_dead_zone == 0)
	{
		_mouse_movement_dead_zone = 6000;
	}

	_scroll_dead_zone = strtol(cfg.getValueOfKey<std::string>("SCROLL_DEAD_ZONE").c_str(), 0, 0);
	if (_scroll_dead_zone == 0)
	{
		_scroll_dead_zone = 5000;
	}

	_scroll_speed = strtof(cfg.getValueOfKey<std::string>("SCROLL_SPEED").c_str(), 0);
	if (_scroll_speed < 0.00001f)
	{
		_scroll_speed = 0.1f;
	}

	// Variable cursor speeds
	std::istringstream cursor_speed = std::istringstream(cfg.getValueOfKey<std::string>("CURSOR_SPEED"));
	int cur_speed_idx = 1;
	const float CUR_SPEED_MIN = 0.0001f;
	const float CUR_SPEED_MAX = 1.0f;
	for (std::string cur_speed; std::getline(cursor_speed, cur_speed, ',');)
	{
		std::istringstream cursor_speed_entry = std::istringstream(cur_speed);
		std::string cur_name, cur_speed_s;
		// Check to see if we are at the string that includes the equals sign.
		if (cur_speed.find_first_of('=') != std::string::npos)
		{
			std::getline(cursor_speed_entry, cur_name, '=');
		}
		else
		{
			std::ostringstream tmp_name;
			tmp_name << cur_speed_idx++;
			cur_name = tmp_name.str();
		}
		std::getline(cursor_speed_entry, cur_speed_s);
		float cur_speedf = strtof(cur_speed_s.c_str(), 0);
		// Ignore speeds that are not within the allowed range.
		if (cur_speedf > CUR_SPEED_MIN && cur_speedf <= CUR_SPEED_MAX)
		{
			speeds.push_back(cur_speedf);
			speed_names.push_back(cur_name);
		}
	}

	// If no cursor speeds were defined, add a set of default speeds.
	if (speeds.size() == 0)
	{
		speed_names.push_back("ULTRALOW");
		speeds.push_back(0.005f);
		speed_names.push_back("LOW");
		speeds.push_back(0.015f);
		speed_names.push_back("MED");
		speeds.push_back(0.025f);
		speed_names.push_back("HIGH");
		speeds.push_back(0.004f);
	}

	speed_idx = strtof(cfg.getValueOfKey<std::string>("CURSOR_SPEED_INDEX").c_str(), 0);
	if (speed_idx >= speeds.size() || speed_idx < 0)
	{
		speed_idx = 0;
	}
	_speed = speeds[speed_idx];
	printf("Speed loaded %f (%s)...\n", _speed, speed_names[speed_idx].c_str());

	// Swap stick functions
	_swap_thumbsticks = strtol(cfg.getValueOfKey<std::string>("SWAP_THUMBSTICKS").c_str(), 0, 0);

	// Set the initial window visibility
	SetWindowVisibility(_hidden);
}

// Description:
//   The main program loop. Handles the gamepad inputs and converts them
//     to system inputs based on the mapping provided by the configuration
//     file.
void Gopher::Run()
{
	Sleep(_kSleep);

	_current_state = _controller->GetState();

	// Disable Gopher
	HandleDisableButton();
	if (_disabled)
	{
		return;
	}

	// Vibration
	HandleVibrationButton();

	// Mouse functions
	HandleMouseMovement();
	HandleScrolling();

	if (_mouse_left_click)
	{
		MapMouseClick(_mouse_left_click, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
	}
	if (_mouse_right_click)
	{
		MapMouseClick(_mouse_right_click, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
	}
	if (_mouse_middle_click)
	{
		MapMouseClick(_mouse_middle_click, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);
	}

	// Hides the console
	if (_console_hide)
	{
		SetXboxClickState(_console_hide);
		if (_xbox_click_is_down[_console_hide])
		{
			ToggleWindowVisibility();
		}
	}

	// Toggle the on-screen keyboard
	if (_disable_on_screen_keyboard)
	{
		SetXboxClickState(_disable_on_screen_keyboard);
		if (_xbox_click_is_down[_disable_on_screen_keyboard])
		{
			// Get the otk window
			HWND otk_win = GetOskWindow();
			if (otk_win == NULL)
			{
				printf("Please start the On-screen keyboard first\n");
			}
			else if (IsIconic(otk_win))
			{
				ShowWindow(otk_win, SW_RESTORE);
			}
			else
			{
				ShowWindow(otk_win, SW_MINIMIZE);
			}
		}
	}

	// Will change between the current speed values
	SetXboxClickState(CONFIG_SPEED_CHANGE);
	if (_xbox_click_is_down[CONFIG_SPEED_CHANGE])
	{
		const int CHANGE_SPEED_VIBRATION_INTENSITY = 65000;   // Speed of the vibration motors when changing cursor speed.
		const int CHANGE_SPEED_VIBRATION_DURATION = 450;      // Duration of the cursor speed change vibration in milliseconds.

		speed_idx++;
		if (speed_idx >= speeds.size())
		{
			speed_idx = 0;
		}
		_speed = speeds[speed_idx];
		printf("Setting speed to %f (%s)...\n", _speed, 
			speed_names[speed_idx].c_str());
		PulseVibrate(CHANGE_SPEED_VIBRATION_DURATION, 
			CHANGE_SPEED_VIBRATION_INTENSITY, 
			CHANGE_SPEED_VIBRATION_INTENSITY);
	}

	// Update all controller keys.
	HandleTriggers(_gamepad_trigger_left, _gamepad_trigger_right);
	if (_gamepad_dpad_up)
	{
		MapKeyboard(XINPUT_GAMEPAD_DPAD_UP, _gamepad_dpad_up);
	}
	if (_gamepad_dpad_down)
	{
		MapKeyboard(XINPUT_GAMEPAD_DPAD_DOWN, _gamepad_dpad_down);
	}
	if (_gamepad_dpad_left)
	{
		MapKeyboard(XINPUT_GAMEPAD_DPAD_LEFT, _gamepad_dpad_left);
	}
	if (_gamepad_dpad_right)
	{
		MapKeyboard(XINPUT_GAMEPAD_DPAD_RIGHT, _gamepad_dpad_right);
	}
	if (_gamepad_start)
	{
		MapKeyboard(XINPUT_GAMEPAD_START, _gamepad_start);
	}
	if (_gamepad_back)
	{
		MapKeyboard(XINPUT_GAMEPAD_BACK, _gamepad_back);
	}
	if (_gamepad_left_thumb)
	{
		MapKeyboard(XINPUT_GAMEPAD_LEFT_THUMB, _gamepad_left_thumb);
	}
	if (_gamepad_right_thumb)
	{
		MapKeyboard(XINPUT_GAMEPAD_RIGHT_THUMB, _gamepad_right_thumb);
	}
	if (_gamepad_left_shoulder)
	{
		MapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, _gamepad_left_shoulder);
	}
	if (_gamepad_right_shoulder)
	{
		MapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, _gamepad_right_shoulder);
	}
	if (_gamepad_A)
	{
		MapKeyboard(XINPUT_GAMEPAD_A, _gamepad_A);
	}
	if (_gamepad_B)
	{
		MapKeyboard(XINPUT_GAMEPAD_B, _gamepad_B);
	}
	if (_gamepad_X)
	{
		MapKeyboard(XINPUT_GAMEPAD_X, _gamepad_X);
	}
	if (_gamepad_Y)
	{
		MapKeyboard(XINPUT_GAMEPAD_Y, _gamepad_Y);
	}
}


// Description:
//  Toggles the controller mapping after checking
//	for the disable configuration command.
void Gopher::HandleDisableButton()
{
	SetXboxClickState(_disable_gopher);
	if (_xbox_click_is_down[_disable_gopher])
	{
		int duration = 0;   // milliseconds
		int intensity = 0;  // vibration intensity

		_disabled = !_disabled;

		if (_disabled)
		{
			// Transition to a disabled state.
			duration = 400;
			intensity = 10000;

			// Release all keys currently pressed by the Gopher mapping.
			while (_pressed_keys.size() != 0)
			{
				std::list<WORD>::iterator it = _pressed_keys.begin();

				// Handle mouse buttons
				if (*it == VK_LBUTTON)
				{
					MouseEvent(MOUSEEVENTF_LEFTUP);
				}
				else if (*it == VK_RBUTTON)
				{
					MouseEvent(MOUSEEVENTF_RIGHTUP);
				}
				else if (*it == VK_MBUTTON)
				{
					MouseEvent(MOUSEEVENTF_MIDDLEUP);
				}
				// Handle keys (TODO: support mouse X1 and X2 buttons)
				else
				{
					InputKeyboardUp(*it);
				}

				_pressed_keys.erase(it);
			}
		}
		else
		{
			duration = 400;
			intensity = 65000;
		}

		PulseVibrate(duration, intensity, intensity);
	}
}

// Description:
//   Toggles the vibration support after checking for 
//	the diable vibration command. 
//   This function will BLOCK to prevent rapidly toggling 
//	the vibration.
void Gopher::HandleVibrationButton()
{
	SetXboxClickState(_disable_vibration);
	if (_xbox_click_is_down[_disable_vibration])
	{
		_vibration_disabled = !_vibration_disabled;
		printf("Vibration %s\n", _vibration_disabled ? "Disabled" : "Enabled");
		Sleep(1000);
	}
}

// Description:
//   Controls the mouse cursor movement by reading the left thumbstick.
void Gopher::HandleMouseMovement()
{
	POINT cursor;
	GetCursorPos(&cursor);

	short tx;
	short ty;

	if (_swap_thumbsticks == 0)
	{
		// Use left stick
		tx = _current_state.Gamepad.sThumbLX;
		ty = _current_state.Gamepad.sThumbLY;
	}
	else
	{
		// Use right stick
		tx = _current_state.Gamepad.sThumbRX;
		ty = _current_state.Gamepad.sThumbRY;
	}

	float x = cursor.x + _xRest;
	float y = cursor.y + _yRest;

	float dx = 0;
	float dy = 0;

	// Handle dead zone
	float lengthsq = tx * tx + ty * ty;
	if (lengthsq > 
		_mouse_movement_dead_zone * _mouse_movement_dead_zone)
	{
		float mult = _speed * GetMult(lengthsq, 
			_mouse_movement_dead_zone,
			_acceleration_factor);

		dx = GetDelta(tx) * mult;
		dy = GetDelta(ty) * mult;
	}

	x += dx;
	_xRest = x - (float)((int)x);

	y -= dy;
	_yRest = y - (float)((int)y);

	SetCursorPos((int)x, (int)y); //after all click input processing
}

// Description:
//   Controls the scroll wheel movement by reading 
//	the right thumbstick.
void Gopher::HandleScrolling()
{
	float tx;
	float ty;

	if (_swap_thumbsticks == 0)
	{
		// Use right stick
		tx = GetDelta(_current_state.Gamepad.sThumbRX);
		ty = GetDelta(_current_state.Gamepad.sThumbRY);
	}
	else
	{
		// Use left stick
		tx = GetDelta(_current_state.Gamepad.sThumbLX);
		ty = GetDelta(_current_state.Gamepad.sThumbLY);
	}

	// Handle dead zone
	float magnitude = sqrt(tx * tx + ty * ty);

	if (magnitude > _scroll_dead_zone)
	{
		MouseEvent(MOUSEEVENTF_HWHEEL,
			tx * GetMult(tx * tx, _scroll_dead_zone) * _scroll_speed);
		MouseEvent(MOUSEEVENTF_WHEEL,
			ty * GetMult(ty * ty, _scroll_dead_zone) * _scroll_speed);
	}
}

// Description:
//   Handles the trigger-to-key mapping. The triggers are handled separately since
//     they are analog instead of a simple button press.
//
// Params:
//   lKey   The mapped key for the left trigger
//   rKey   The mapped key for the right trigger
void Gopher::HandleTriggers(WORD lKey, WORD rKey)
{
	bool lTriggerIsDown = _current_state.Gamepad.bLeftTrigger > 
		TRIGGER_DEAD_ZONE;
	bool rTriggerIsDown = _current_state.Gamepad.bRightTrigger >
		TRIGGER_DEAD_ZONE;

	// Handle left trigger
	if (lTriggerIsDown != _left_trigger_previous)
	{
		_left_trigger_previous = lTriggerIsDown;
		if (lTriggerIsDown)
		{
			InputKeyboardDown(lKey);
		}
		else
		{
			InputKeyboardUp(lKey);
		}
	}

	// Handle right trigger
	if (rTriggerIsDown != _right_trigger_previous)
	{
		_right_trigger_previous = rTriggerIsDown;
		if (rTriggerIsDown)
		{
			InputKeyboardDown(rKey);
		}
		else
		{
			InputKeyboardUp(rKey);
		}
	}
}

// Description:
//   Determines if the thumbstick value is valid and converts
//	it to a float.
// Params:
//   t  Analog thumbstick value to check and convert
// Returns:
//   If the value is valid, t will be returned as-is as a float.
// If the value is invalid, 0 will be returned.
float Gopher::GetDelta(short t)
{
	//filter non-32768 and 32767, wireless ones can glitch 
	//sometimes and send it to the edge of the screen, 
	//it'll toss out some HUGE integer even when it's centered
	if (t > 32767) t = 0;
	if (t < -32768) t = 0;

	return t;
}

// Description:
//   Calculates a multiplier for an analog thumbstick based
//	on the update rate.
// Params:
//   tValue     The thumbstick value
//   deadzone   The dead zone to use for this thumbstick
//   accel      An exponent to use to create an input curve (Optional). 0 to use a linear input
// Returns:
//   Multiplier used to properly scale the given thumbstick value.
float Gopher::GetMult(float lengthsq, float deadzone, float accel)
{
	// Normalize the thumbstick value.
	float mult = (sqrt(lengthsq) - deadzone) / (MAXSHORT - deadzone);

	// Apply a curve to the normalized thumbstick value.
	if (accel > 0.0001f)
	{
		mult = pow(mult, accel);
	}
	return mult / _kFPS;
}

// Description:
//   Toggles the visibility of the window.
void Gopher::ToggleWindowVisibility()
{
	_hidden = !_hidden;
	printf("Window %s\n", _hidden ? "hidden" : "unhidden");
	SetWindowVisibility(_hidden);
}

// Description:
//   Either hides or shows the window.
// Params:
//   hidden   Hides the window when true
void Gopher::SetWindowVisibility(const bool &hidden) const
{
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, _hidden ? SW_HIDE : SW_SHOW);
}

// Description:
//   Sends a vibration pulse to the controller for a duration of time.
//     This is a BLOCKING call. 
//		Any inputs during the vibration will be IGNORED.
// Params:
//   duration   The length of time in milliseconds to vibrate for
//   l          The speed (intensity) of the left vibration motor
//   r          The speed (intensity) of the right vibration motor
void Gopher::PulseVibrate(const int duration, const int l, const int r) const
{
	if (!_vibration_disabled)
	{
		_controller->Vibrate(l, r);
		Sleep(duration);
		_controller->Vibrate(0, 0);
	}
}

// Description:
//   Presses or releases a mouse button based on a mapped Gopher state
//
// Params:
//   STATE    The Gopher state, or command, to trigger a mouse event
//   keyDown  The button down event for a mouse event
//   keyUp    The button up event for a mouse event
void Gopher::MapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp)
{
	SetXboxClickState(STATE);
	if (_xbox_click_is_down[STATE])
	{
		MouseEvent(keyDown);

		// Add key to the list of pressed keys.
		if (keyDown == MOUSEEVENTF_LEFTDOWN)
		{
			_pressed_keys.push_back(VK_LBUTTON);
		}
		else if (keyDown == MOUSEEVENTF_RIGHTDOWN)
		{
			_pressed_keys.push_back(VK_RBUTTON);
		}
		else if (keyDown == MOUSEEVENTF_MIDDLEDOWN)
		{
			_pressed_keys.push_back(VK_MBUTTON);
		}
	}

	if (_xbox_click_is_up[STATE])
	{
		MouseEvent(keyUp);

		// Remove key from the list of pressed keys.
		if (keyUp == MOUSEEVENTF_LEFTUP)
		{
			ErasePressedKey(VK_LBUTTON);
		}
		else if (keyUp == MOUSEEVENTF_RIGHTUP)
		{
			ErasePressedKey(VK_RBUTTON);
		}
		else if (keyUp == MOUSEEVENTF_MIDDLEUP)
		{
			ErasePressedKey(VK_MBUTTON);
		}
	}
}

// Description:
//   Removes an entry for a pressed key from the list.
// Params:
//   key  The key value to remove from the pressed key list. 
// Returns:
//   True if the given key was found and removed from the list.
bool Gopher::ErasePressedKey(WORD key)
{
	for (std::list<WORD>::iterator it = _pressed_keys.begin();
		it != _pressed_keys.end();
		++it)
	{
		if (*it == key)
		{
			_pressed_keys.erase(it);
			return true;
		}
	}

	return false;
}

// Description:
//   Presses or releases a key based on a mapped Gopher state.
// Params:
//   STATE  The Gopher state, or command, to trigger a key event
//   key    The key value to input to the system
void Gopher::MapKeyboard(DWORD STATE, WORD key)
{
	SetXboxClickState(STATE);
	if (_xbox_click_is_down[STATE])
	{
		InputKeyboardDown(key);

		// Add key to the list of pressed keys.
		_pressed_keys.push_back(key);
	}

	if (_xbox_click_is_up[STATE])
	{
		InputKeyboardUp(key);

		// Remove key from the list of pressed keys.
		ErasePressedKey(key);
	}
}

// Description:
//   Handles the state of a controller button press.
// Params:
//   STATE  The Gopher state, or command, to update
void Gopher::SetXboxClickState(DWORD STATE)
{
	_xbox_click_is_down[STATE] = false;
	_xbox_click_is_up[STATE] = false;

	if (!this->XboxClickStateExists(STATE))
	{
		_xbox_click_state_last_iteration[STATE] = false;
	}

	bool isDown = (_current_state.Gamepad.wButtons & STATE) == STATE;

	// Detect if the button has been pressed.
	if (isDown && !_xbox_click_state_last_iteration[STATE])
	{
		_xbox_click_state_last_iteration[STATE] = true;
		_xbox_click_is_down[STATE] = true;
		_xbox_click_down_length[STATE] = 0;
		_xbox_click_is_down_long[STATE] = false;
	}

	// Detect if the button has been held as a long press.
	if (isDown && _xbox_click_state_last_iteration[STATE])
	{
		const int LONG_PRESS_TIME = 200;  // milliseconds

		++_xbox_click_down_length[STATE];
		if (_xbox_click_down_length[STATE] * _kSleep > LONG_PRESS_TIME)
		{
			_xbox_click_is_down_long[STATE] = true;
		}
	}

	// Detect if the button has been released.
	if (!isDown && _xbox_click_state_last_iteration[STATE])
	{
		_xbox_click_state_last_iteration[STATE] = false;
		_xbox_click_is_up[STATE] = true;
		_xbox_click_is_down_long[STATE] = false;
	}

	_xbox_click_state_last_iteration[STATE] = isDown;
}

// Description:
//   Check to see if a controller state exists in Gopher's button map.
// Params:
//   xinput   The Gopher state, or command, to search for
// Returns:
//   true if the state is present in the map.
bool Gopher::XboxClickStateExists(DWORD STATE)
{
	auto it = _xbox_click_state_last_iteration.find(STATE);
	if (it == _xbox_click_state_last_iteration.end())
	{
		return false;
	}

	return true;
}

// Description:
//   Callback function used for the EnumWindows call to determine if we
//     have found the On-Screen Keyboard window.
//
// Params:
//   curWnd   The current window to check
//   lParam   A callback parameter used to store the window if it is found
//
// Returns:
//   FALSE when the the desired window is found.
static BOOL CALLBACK EnumWindowsProc(HWND curWnd, LPARAM lParam)
{
	TCHAR title[256];
	// Check to see if the window title matches what we are looking for.
	if (GetWindowText(curWnd, title, 256) && !_tcscmp(title, _T("On-Screen Keyboard")))
	{
		*(HWND*)lParam = curWnd;
		return FALSE;  // Correct window found, stop enumerating through windows.
	}

	return TRUE;
}

// Description:
//   Finds the On-Screen Keyboard if it is open.
// Returns:
//   If found, the handle to the On-Screen Keyboard handle. Otherwise, returns NULL.
HWND Gopher::GetOskWindow()
{
	HWND ret = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)&ret);
	return ret;
}

