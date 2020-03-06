#include <iostream>
#include <Windows.h>

using namespace std;

int main()
{
	HWND hwnd = FindWindow(NULL, L"Git Gui");
	if (hwnd == NULL)
	{
		cout << "Could not find target window." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	DWORD pid = NULL;
	DWORD tid = GetWindowThreadProcessId(hwnd, &pid);
	if (tid == NULL)
	{
		cout << "Could not get thread ID of the target window." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	HMODULE dll = LoadLibraryEx(L"keyflagcleaner.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);

	if (dll == NULL)
	{
		cout << "The DLL could not be found." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	HOOKPROC addr = (HOOKPROC)GetProcAddress(dll, "AttachMe");
	if (addr == NULL) {
		cout << "The function was not found." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	HHOOK handle = SetWindowsHookEx(WH_GETMESSAGE, addr, dll, tid);
	if (handle == NULL) {
		cout << "Couldn't set the hook with SetWindowsHookEx." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	PostThreadMessage(tid, WM_NULL, NULL, NULL);

	cout << "Press any key to unhook (This will unload the DLL)." << endl;
	system("pause > nul");

	BOOL unhook = UnhookWindowsHookEx(handle);
	if (unhook == FALSE) {
		cout << "Could not remove the hook." << endl;
		system("pause");
		return EXIT_FAILURE;
	}

	cout << "Done. Press any key to exit." << endl;
	system("pause > nul");
	return EXIT_SUCCESS;
}