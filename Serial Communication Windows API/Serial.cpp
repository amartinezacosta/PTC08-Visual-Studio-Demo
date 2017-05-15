#include "Serial.h"



Serial::Serial(WCHAR *SerialPort, int BaudRate, int ByteSize, int StopBits, int Parity)
{
	_SerialPort = SerialPort;
	_BaudRate = BaudRate;
	_ByteSize = ByteSize;
	_StopBits = StopBits;
	_Parity = Parity;
}

Serial::~Serial()
{
}

BOOL Serial::Connect() 
{
	BOOL Status;
	
	if (_SerialPort == NULL) {
		return FALSE;
	}
	
	hComm = CreateFile(	_SerialPort,
						GENERIC_READ | GENERIC_WRITE,
						0,
						0,
						OPEN_EXISTING,
						NULL,
						0);

	if (hComm == INVALID_HANDLE_VALUE) return FALSE;

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);
	if (!Status) return FALSE;

	dcbSerialParams.BaudRate = _BaudRate;
	dcbSerialParams.ByteSize = _ByteSize;
	dcbSerialParams.StopBits = _StopBits;
	dcbSerialParams.Parity = _Parity;

	Status = SetCommState(hComm, &dcbSerialParams);
	if (!Status) return FALSE;

	_PortOpen = TRUE;
	
	return TRUE;
}

DWORD Serial::Write(CHAR * Data, DWORD Size)
{
	DWORD BytesWritten = 0;

	WriteFile(hComm, Data, Size, &BytesWritten, NULL);
	
	return BytesWritten;
}

DWORD Serial::Read(CHAR * Data, DWORD Size)
{
	DWORD BytesRead = 0;
	ReadFile(hComm, Data, Size, &BytesRead, NULL);

	return BytesRead;
}

