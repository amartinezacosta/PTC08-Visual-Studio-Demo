#pragma once
#include <Windows.h>

class Serial
{
public:
	Serial(WCHAR *SerialPort, int BaudRate, int ByteSize, int StopBits, int Parity);
	~Serial();

	BOOL Connect();
	DWORD Write(CHAR *data, DWORD size);
	DWORD Read(CHAR *data, DWORD size);
private:
	HANDLE hComm;
	WCHAR *_SerialPort = L"COM1";
	int _BaudRate = CBR_9600;
	int _ByteSize = 8;
	int _StopBits = ONESTOPBIT;
	int _Parity = NOPARITY;

	BOOL _PortOpen = FALSE;
};

