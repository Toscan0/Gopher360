#include "Gopher.h"

Gopher::Gopher(CXBOXController * controller)
	: _controller(controller)
{
}

void Gopher::LoadConfigFile(std::string fileName)
{
	ConfigFile cfg(fileName);

	//--------------------------------
	// Configuration bindings
	//--------------------------------
	_mouse_left_click = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_LEFT").c_str(), 0, 0);
	_mouse_right_click = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_RIGHT").c_str(), 0, 0);
	_mouse_middle_click = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_MIDDLE").c_str(), 0, 0);
	_console_hide = strtol(cfg.getValueOfKey<std::string>("CONFIG_HIDE").c_str(), 0, 0);
	_disable_gopher = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE").c_str(), 0, 0);
	_disable_vibration = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE_VIBRATION").c_str(), 0, 0);
	CONFIG_SPEED_CHANGE = strtol(cfg.getValueOfKey<std::string>("CONFIG_SPEED_CHANGE").c_str(), 0, 0);
	_disable_on_screen_keyboard = strtol(cfg.getValueOfKey<std::string>("CONFIG_OSK").c_str(), 0, 0);

	//--------------------------------
	// Controller bindings
	//--------------------------------
	_gamepad_dpad_up = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_UP").c_str(), 0, 0);
	_gamepad_dpad_down = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_DOWN").c_str(), 0, 0);
	_gamepad_dpad_left = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_LEFT").c_str(), 0, 0);
	_gamepad_dpad_right = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_RIGHT").c_str(), 0, 0);
	_gamepad_start = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_START").c_str(), 0, 0);
	_gamepad_back = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_BACK").c_str(), 0, 0);
	_gamepad_left_thumb = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_THUMB").c_str(), 0, 0);
	_gamepad_right_thumb = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_THUMB").c_str(), 0, 0);
	_gamepad_A = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_A").c_str(), 0, 0);
	_gamepad_B = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_B").c_str(), 0, 0);
	_gamepad_X = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_X").c_str(), 0, 0);
	_gamepad_Y = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_Y").c_str(), 0, 0);
	_gamepad_left_shoulder = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_SHOULDER").c_str(), 0, 0);
	_gamepad_right_shoulder = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_SHOULDER").c_str(), 0, 0);
	_gamepad_trigger_left = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_LEFT").c_str(), 0, 0);
	_gamepad_trigger_right = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_RIGHT").c_str(), 0, 0);

	//--------------------------------
	// Advanced settings
	//--------------------------------

	// Acceleration factor
	acceleration_factor = strtof(cfg.getValueOfKey<std::string>("ACCELERATION_FACTOR").c_str(), 0);

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
		speeds.push_back(0.005f);
		speeds.push_back(0.015f);
		speeds.push_back(0.025f);
		speeds.push_back(0.004f);
		speed_names.push_back("ULTRALOW");
		speed_names.push_back("LOW");
		speed_names.push_back("MED");
		speed_names.push_back("HIGH");
	}

	speed_idx = strtof(cfg.getValueOfKey<std::string>("CURSOR_SPEED_INDEX").c_str(), 0);
	if (speed_idx >= speeds.size() || speed_idx < 0)
	{
		speed_idx = 0;
	}
	speed = speeds[speed_idx];
	printf("Speed loaded %f (%s)...\n", speed, speed_names[speed_idx].c_str());
	//TODO: Set the speed to a saved speed that was last used when the application was closed last.

	// Swap stick functions
	_swap_thumbsticks = strtol(cfg.getValueOfKey<std::string>("SWAP_THUMBSTICKS").c_str(), 0, 0);

	// Set the initial window visibility
	setWindowVisibility(_hidden);
}