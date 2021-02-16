/*-------------------------------------------------------------------------------
    Gopher free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------------------*/
#include <iostream>
#include <windows.h> // for Beep()
#include "CXBOXController.h"
#include "Gopher.h"

#pragma comment(lib, "XInput9_1_0.lib")
#pragma comment(lib, "winmm") // for volume

BOOL IsRunningAsAdministrator(); // Check if administrator, makes on-screen keyboard clickable

int main()
{
  CXBOXController controller(1);
  VolumeManager volumeManager;
  Gopher gopher(&controller, &volumeManager);
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

  SetConsoleTitle( TEXT( "Gopher360 !!!RELEASED!!!" ) );
  system("Color 0A");
  SetConsoleTextAttribute(hConsole, 5);
  printf("Gopher is free (as in freedom) software: you can redistribute it and/or modify\n"
	  "it under the terms of the GNU General Public License as published by\nthe Free Software"
	  "Foundation, either version 3 of the License, or\n(at your option) any later version.\n");
  printf("\nTip: Press left and right bumpers simultaneously to toggle speeds!\n");
  printf("\n-------------------------\n\n");

  SetConsoleTextAttribute(hConsole, 15); 

  if (IsRunningAsAdministrator())
  {
    printf("Tip - Not running as an admin! Windows on-screen keyboard and others won't work without admin rights.\n");
  }

  gopher.LoadConfigFile("config.ini");
  while (true)
  {
    gopher.Run();
  }
}

BOOL IsRunningAsAdministrator()
{
  BOOL   fRet = FALSE;
  HANDLE hToken = NULL;

  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
  {
    TOKEN_ELEVATION Elevation;
    DWORD cbSize = sizeof( TOKEN_ELEVATION );

    if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof( Elevation), &cbSize))
    {
      fRet = Elevation.TokenIsElevated;
    }
  }

  if (hToken)
  {
    CloseHandle(hToken);
  }

  return fRet;
}