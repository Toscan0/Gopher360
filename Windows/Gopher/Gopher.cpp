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
		mapMouseClick(_mouse_left_click, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
	}
	if (_mouse_right_click)
	{
		mapMouseClick(_mouse_right_click, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
	}
	if (_mouse_middle_click)
	{
		mapMouseClick(_mouse_middle_click, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);
	}

	// Hides the console
	if (_console_hide)
	{
		setXboxClickState(_console_hide);
		if (_xbox_click_is_down[_console_hide])
		{
			ToggleWindowVisibility();
		}
	}

	// Toggle the on-screen keyboard
	if (_disable_on_screen_keyboard)
	{
		setXboxClickState(_disable_on_screen_keyboard);
		if (_xbox_click_is_down[_disable_on_screen_keyboard])
		{
			// Get the otk window
			HWND otk_win = getOskWindow();
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
	setXboxClickState(CONFIG_SPEED_CHANGE);
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
		mapKeyboard(XINPUT_GAMEPAD_DPAD_UP, _gamepad_dpad_up);
	}
	if (_gamepad_dpad_down)
	{
		mapKeyboard(XINPUT_GAMEPAD_DPAD_DOWN, _gamepad_dpad_down);
	}
	if (_gamepad_dpad_left)
	{
		mapKeyboard(XINPUT_GAMEPAD_DPAD_LEFT, _gamepad_dpad_left);
	}
	if (_gamepad_dpad_right)
	{
		mapKeyboard(XINPUT_GAMEPAD_DPAD_RIGHT, _gamepad_dpad_right);
	}
	if (_gamepad_start)
	{
		mapKeyboard(XINPUT_GAMEPAD_START, _gamepad_start);
	}
	if (_gamepad_back)
	{
		mapKeyboard(XINPUT_GAMEPAD_BACK, _gamepad_back);
	}
	if (_gamepad_left_thumb)
	{
		mapKeyboard(XINPUT_GAMEPAD_LEFT_THUMB, _gamepad_left_thumb);
	}
	if (_gamepad_right_thumb)
	{
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_THUMB, _gamepad_right_thumb);
	}
	if (_gamepad_left_shoulder)
	{
		mapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, _gamepad_left_shoulder);
	}
	if (_gamepad_right_shoulder)
	{
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, _gamepad_right_shoulder);
	}
	if (_gamepad_A)
	{
		mapKeyboard(XINPUT_GAMEPAD_A, _gamepad_A);
	}
	if (_gamepad_B)
	{
		mapKeyboard(XINPUT_GAMEPAD_B, _gamepad_B);
	}
	if (_gamepad_X)
	{
		mapKeyboard(XINPUT_GAMEPAD_X, _gamepad_X);
	}
	if (_gamepad_Y)
	{
		mapKeyboard(XINPUT_GAMEPAD_Y, _gamepad_Y);
	}
}


// Description:
//  Toggles the controller mapping after checking
//	for the disable configuration command.
void Gopher::HandleDisableButton()
{
	setXboxClickState(_disable_gopher);
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
					mouseEvent(MOUSEEVENTF_LEFTUP);
				}
				else if (*it == VK_RBUTTON)
				{
					mouseEvent(MOUSEEVENTF_RIGHTUP);
				}
				else if (*it == VK_MBUTTON)
				{
					mouseEvent(MOUSEEVENTF_MIDDLEUP);
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
	setXboxClickState(_disable_vibration);
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
		mouseEvent(MOUSEEVENTF_HWHEEL, 
			tx * GetMult(tx * tx, _scroll_dead_zone) * _scroll_speed);
		mouseEvent(MOUSEEVENTF_WHEEL,
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
float Gopher::GetMult(float lengthsq, float deadzone, float accel = 0.0f)
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















////////////////
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

// Description:
//   Send a mouse input based on a mouse event type.
//   See https://msdn.microsoft.com/en-us/library/windows/desktop/ms646310(v=vs.85).aspx
//
// Params:
//   dwFlags    The mouse event to send
//   mouseData  Additional information needed for certain mouse events (Optional)
void mouseEvent(DWORD dwFlags, DWORD mouseData = 0)
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