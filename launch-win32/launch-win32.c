	
#include <windows.h> 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nShowCmd) {
  STARTUPINFOW startupInfo;
  PROCESS_INFORMATION processInfo;
  BOOL result;
  
  wchar_t launch_path[MAX_PATH];
  DWORD launch_path_len = GetFullPathNameW(L"launch.lua", MAX_PATH, launch_path, NULL);
  
  wchar_t luajit_path[MAX_PATH];
  DWORD luajit_path_len = GetFullPathNameW(L"win32-bin\\luajit.exe", MAX_PATH, luajit_path, NULL);
  
  if (GetFileAttributesW(launch_path) == INVALID_FILE_ATTRIBUTES) {
	MessageBoxW(NULL, L"launch.lua script not found", L"Error", MB_OK | MB_ICONINFORMATION);
	return 0;
  }
  
  if (GetFileAttributesW(luajit_path) == INVALID_FILE_ATTRIBUTES) {
	MessageBoxW(NULL, L"win32-bin\\luajit.exe not found", L"Error", MB_OK | MB_ICONINFORMATION);
	return 0;
  }
  
  wchar_t exe_path[MAX_PATH];
  GetCurrentDirectoryW(MAX_PATH, exe_path);
  
  wchar_t project_path[MAX_PATH] = L"";
  
  OPENFILENAMEW ofn;
  memset(&ofn, 0, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hInstance = hInstance;
  ofn.lpstrFilter = L"*.main.lua\0*.main.lua\0\0";
  ofn.lpstrFile = project_path;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrInitialDir = L"projects";
  ofn.lpstrTitle = L"Open project file...";
  ofn.Flags = OFN_FILEMUSTEXIST;
  if (!GetOpenFileNameW(&ofn)) {
    return 0;
  }
  
  wchar_t project_folder_path[MAX_PATH];
  memset(project_folder_path, 0, sizeof(project_folder_path));
  wcsncpy(project_folder_path, project_path, ofn.nFileOffset);
  
  wchar_t setup[MAX_PATH*6 + 1024] = L"-e \"local m={X=[[";
  wcscat(setup, exe_path);
  wcscat(setup, L"]],P=[[");
  wcsncat(setup, project_path, ofn.nFileOffset-1);
  wcscat(setup, L"]]} package.path=('?.lua;?/init.lua;$P/?.lua;$P/?/init.lua;$X/lua/?.lua;$X/lua/?/init.lua'):gsub('$(.)',m) package.cpath=('?.dll;$P/win32-bin/?.dll;$X/win32-bin/?.dll'):gsub('$(.)',m)\"");
  
  wchar_t cmd_line[1 + luajit_path_len + 2 + wcslen(setup) + 2 + launch_path_len + 3 + wcslen(project_path) + 1 + 1];
  wsprintfW(cmd_line, L"\"%s\" %s \"%s\" \"%s\"", luajit_path, setup, launch_path, project_path);
  
  memset(&startupInfo, 0, sizeof(startupInfo));
  memset(&processInfo, 0, sizeof(processInfo));
  startupInfo.cb = sizeof(startupInfo);
  
  result = CreateProcessW(
	luajit_path,
	cmd_line, // "luajit.exe launch.lua",
	NULL, // process attributes
	NULL, // thread attributes
	FALSE, // inherit handles
	CREATE_DEFAULT_ERROR_MODE, // | CREATE_NO_WINDOW, // creation flags
	NULL, // environment
	project_folder_path, // current directory
	&startupInfo,
	&processInfo);
	
  if (!result) {
    DWORD error = GetLastError();
	wchar_t* lpMsgBuf;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &lpMsgBuf,
        0, NULL );
	MessageBoxW(NULL, lpMsgBuf, L"Error", MB_OK | MB_ICONINFORMATION);
  }
  return 0; 
}
