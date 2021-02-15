#include "CXBOXController.h"

CXBOXController::CXBOXController(int playerNumber)
{
	_controller_num = playerNumber - 1; //set number
}

XINPUT_STATE CXBOXController::GetState()
{
  ZeroMemory(&this->_controller_state, sizeof(XINPUT_STATE));
  XInputGetState(_controller_num, &this->_controller_state);

  return _controller_state;
}

bool CXBOXController::IsConnected()
{
  ZeroMemory(&this->_controller_state, sizeof(XINPUT_STATE));
  DWORD Result = XInputGetState(_controller_num, &this->_controller_state);

  return (Result == ERROR_SUCCESS);
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
  // Create a Vibraton State
  XINPUT_VIBRATION Vibration;

  // Zeroise the Vibration
  ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

  // Set the Vibration Values
  Vibration.wLeftMotorSpeed = leftVal;
  Vibration.wRightMotorSpeed = rightVal;

  // Vibrate the controller
  XInputSetState(_controller_num, &Vibration);
}