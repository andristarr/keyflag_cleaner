#include "pch.h"

int clearKthBit(DWORD number, int k)
{
	number &= ~(1UL << k);
	return number;
}

LRESULT CALLBACK ClearInjectedKeyboardFlags(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
			if (p->flags & 0x00000002 || p->flags & 0x00000010)
			{
				p->flags = clearKthBit(p->flags, 4);
				p->flags = clearKthBit(p->flags, 1);

				bool llkhf_injected = p->flags & 0x00000010;
				bool llkhf_lower_il_injected = p->flags & 0x00000002;
			}

			break;
		}
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void InstallLLKeyboardHook()
{
	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, ClearInjectedKeyboardFlags, 0, 0);

	// arent unregistering the hook for now
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{

	case DLL_PROCESS_ATTACH:
		// will only work if the process didnt already hook CreateThread or SetWindowsHookEx... 
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(InstallLLKeyboardHook), NULL, 0, 0);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;

	}

	return TRUE;
}

extern "C" int CALLBACK AttachMe(int code, WPARAM wParam, LPARAM lParam)
{
	return(CallNextHookEx(NULL, code, wParam, lParam));
}
