#ifndef _PORTHANDLER_H
#define _PORTHANDLER_H

#define READ_BUF_SIZE 1024
#define READ_TIMEOUT      500      // milliseconds

#include "Com.h"
#include "Tools.h"
#include <Windows.h>

class PortHandler
{
public:
	PortHandler(HANDLE hCom);
	PortHandler(void);
	~PortHandler(void);

	int iExitCode;
	bool readData(char * lpBuf);
	bool writeData(char * lpBuf);
	int checkPortStatus();

	BOOL ReadFromCOMPort(unsigned char* c_Data);
	BOOL WriteToCOMPort(unsigned char* c_Data);

private:
	Com _com;
	HANDLE hCom;
	Tools tools;
	//char * _lpBuf;
};

#endif