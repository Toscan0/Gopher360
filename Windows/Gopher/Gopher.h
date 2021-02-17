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
#include "CXBOXController.h"
#include "ParseConfigFile.h"
#include "InputKeyboard.h"
#include "MouseInput.h"
#include <shellapi.h>

#pragma once
class Gopher
{
private:
	int _mouse_movement_dead_zone = 6000;	// Thumbstick dead zone to use for mouse movement. Absolute maximum shall be 65534.
	int _scroll_dead_zone = 5000;			// Thumbstick dead zone to use for scroll wheel movement. Absolute maximum shall be 65534.
	float _scroll_speed = 0.1f;             // Speed at which you scroll.
	int _swap_thumbsticks = 0;				// Swaps the function of the thumbsticks when not equal to 0.
	const int _kTriggers_dead_zone = 0;     // Dead zone for the left and right triggers to detect a trigger press. 0 means that any press to trigger will be read as a button press.
	const int _kFPS = 150;                  // Update rate of the main Gopher loop. Interpreted as cycles-per-second.
	const int _kSleep = 1000 / _kFPS;		// Number of milliseconds to sleep per iteration.

	XINPUT_STATE _current_state;

	float _xRest = 0.0f;
	float _yRest = 0.0f;
	bool _left_trigger_previous = false;	// Previous state of the left trigger.
	bool _right_trigger_previous = false;   // Previous state of the right trigger.

	// Gopher settings
	bool _disable_gopher = false;           // Disables the Gopher controller mapping.
	bool _disable_vibration = false;		// Prevents Gopher from producing controller vibrations. 
	bool _show_console = false;				// Gopher main window visibility.

	// Cursor speed settings
	float _acceleration_factor = 0.0f;
	const float _kSpeed_ultra_low = 0.005f;
	const float _kSpeed_low = 0.015f;
	const float _kSpeed_med = 0.025f;
	const float _kSpeed_high = 0.04f;
	float _speed = _kSpeed_med;
	std::vector<float> speeds;	            // Contains actual speeds to choose
	std::vector<std::string> speed_names;   // Contains display names of speeds to display
	unsigned int speed_idx = 0;

	// Volume
	const float _k_volume_offset = 0.02f;

	// Mouse keys
	DWORD _mouse_left_click_key = NULL;
	DWORD _mouse_right_click_key = NULL;
	DWORD _mouse_middle_click_key = NULL;

	// Gopher settings keys
	DWORD _console_hide_key = NULL;
	DWORD _disable_gopher_key = NULL;
	DWORD _disable_vibration_key = NULL;
	DWORD _mouse_speed_change_key = NULL;
	DWORD _on_screen_keyboard_key = NULL;

	// Gamepad keys -> keyboard
	DWORD _gamepad_left_thumb_key = NULL;
	DWORD _gamepad_right_thumb_key = NULL;
	DWORD _gamepad_dpad_up_key = NULL;
	DWORD _gamepad_dpad_down_key = NULL;
	DWORD _gamepad_dpad_left_key = NULL;
	DWORD _gamepad_dpad_right_key = NULL;
	DWORD _gamepad_A_key = NULL;
	DWORD _gamepad_B_key = NULL;
	DWORD _gamepad_X_key = NULL;
	DWORD _gamepad_Y_key = NULL;
	DWORD _gamepad_left_shoulder_key = NULL;
	DWORD _gamepad_right_shoulder_key = NULL;
	DWORD _gamepad_trigger_left_key = NULL;
	DWORD _gamepad_trigger_right_key = NULL;
	DWORD _gamepad_start_key = NULL;
	DWORD _gamepad_back_key = NULL;

	// Button press state logic variables
	std::map<DWORD, bool> _xbox_click_state_last_iteration;
	std::map<DWORD, bool> _xbox_click_is_down;
	std::map<DWORD, bool> _xbox_click_is_down_long;
	std::map<DWORD, int> _xbox_click_down_length;
	std::map<DWORD, bool> _xbox_click_is_up;

	std::list<WORD> _pressed_keys;

	CXBOXController *_controller;

public:
	Gopher(CXBOXController* controller);

	void LoadConfigFile(std::string fileName);
	void Run();

	void ToggleWindowVisibility();
	void SetWindowVisibility(const bool& hidden) const;

	void HandleDisableButton();
	void HandleVibrationButton();
	void HandleMouseMovement();
	void HandleScrolling();
	void HandleTriggers(WORD lKey, WORD rKey);

	void MapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp);
	void MapKeyboard(DWORD STATE, WORD key);

	void SetXboxClickState(DWORD state);
	bool XboxClickStateExists(DWORD xinput);

	void PulseVibrate(const int duration, const int l, const int r) const;
	float GetDelta(short tx);
	float GetMult(float length, float deadzone, float accel = 0.0f);
	
	void LaunchOSK();
private:
	bool ErasePressedKey(WORD key);
};
