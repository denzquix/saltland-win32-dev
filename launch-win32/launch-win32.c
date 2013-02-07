
#include <windows.h> 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nShowCmd) {
  STARTUPINFO startupInfo;
  PROCESS_INFORMATION processInfo;
  BOOL result;
  
  memset(&startupInfo, 0, sizeof(startupInfo));
  memset(&processInfo, 0, sizeof(processInfo));
  startupInfo.cb = sizeof(startupInfo);
  
  result = CreateProcessA(
	"win32-bin\\luajit.exe",
	"luajit.exe launch.lua",
	NULL, // process attributes
	NULL, // thread attributes
	FALSE, // inherit handles
	CREATE_DEFAULT_ERROR_MODE | CREATE_NO_WINDOW, // creation flags
	NULL, // environment
	".\\", // current directory
	&startupInfo,
	&processInfo);
	
  if (result) {
    DWORD error = GetLastError();
	//MessageBoxW(NULL, L"An error occurred", L"Error", MB_OK | MB_ICONINFORMATION);
  }
  return 0; 
}
