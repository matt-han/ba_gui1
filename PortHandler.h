#ifndef _PORTHANDLER_H
#define _PORTHANDLER_H

#define READ_BUF_SIZE 1024
#define READ_TIMEOUT      500      // milliseconds

#include "Com.h"
#include <Windows.h>

class PortHandler
{
public:
	PortHandler(HANDLE hCom);
	PortHandler(void);
	~PortHandler(void);

	int iExitCode;
	bool readData();
	bool writeData(char * lpBuf, DWORD dwToWrite);
	int checkPortStatus();


private:
	Com _com;
	HANDLE hCom;
	
	char * _lpBuf;
};

#endif