#include <iostream>
#include "Serial.h"

#define FRAME_BUFFER_SIZE 128

int wmain(int argc, wchar_t *argv[], wchar_t *envp[]) {
	Serial _Serial(argv[1], CBR_115200, 8, ONESTOPBIT, NOPARITY);

	if (!_Serial.Connect()) {
		std::wcout << "Failed to connect to port: " << argv[1] << std::endl;
		return 0;
	}
	else {
		std::wcout << "Succesfully connected to port: " << argv[1] << std::endl;
	}

	BOOL Status;
	DWORD Attrib = GetFileAttributes(L"IMAGE00.jpg");
	Status = (Attrib != INVALID_FILE_ATTRIBUTES && !(Attrib & FILE_ATTRIBUTE_DIRECTORY));

	if (Status) {
		DeleteFile(L"IMAGE00.jpg");
	}
	
	HANDLE hFile;
	hFile = CreateFile(L"IMAGE00.jpg",
						GENERIC_WRITE,
						0, 
						NULL,
						CREATE_NEW,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE) return 0;

	char Serial_Buffer[FRAME_BUFFER_SIZE];
	char Input_Buffer[32];
	ZeroMemory(Serial_Buffer, FRAME_BUFFER_SIZE);
	ZeroMemory(Input_Buffer, 32);

	DWORD BytesWritten = 0;
	DWORD PictureSize = 0;
	DWORD i = 0;

	/*Press enter to continue string comming from Tiva*/
	_Serial.Read(Input_Buffer, 25);
	std::cout << Input_Buffer << std::endl;

	/*Sending response to Tiva*/
	if (std::cin.get() == '\n') {
		_Serial.Write("\n", 1);
	}
	
	
	ZeroMemory(Input_Buffer, 32);
	/*Picture size from Tiva*/
	_Serial.Read(Input_Buffer, 7);
	std::cout << Input_Buffer << std::endl;
	PictureSize = atoi(Input_Buffer);
	
	
	while (i < PictureSize) {
		_Serial.Read(Serial_Buffer, FRAME_BUFFER_SIZE);
		Status = WriteFile(hFile, Serial_Buffer, FRAME_BUFFER_SIZE, &BytesWritten, NULL);
		if (!Status) {
			std::cout << "Failed to write to file\n";
			return 0;
		}
		i += BytesWritten;
		std::cout << ".";
	}

	CloseHandle(hFile);
	std::cout << "\nDone reading picture\n";
}