#pragma once

#include <windows.h>
#include <xinput.h>

class CXBOXController
{
	private:
	  XINPUT_STATE _controller_state;
	  int _controller_num;

	public:
	  CXBOXController(int playerNumber);
	  XINPUT_STATE GetState();
	  bool IsConnected();
	  void Vibrate(int leftVal, int rightVal);
};
