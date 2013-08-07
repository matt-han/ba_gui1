/*
 *	Class to handle input and output operations in a opened COM port
 */

#ifndef _PORTHANDLER_H
#define _PORTHANDLER_H

#include "Com.h"
#include "Tools.h"
#include <Windows.h>

//------------------------------------------------------------------------------
//Constants
#define READ_BUF_SIZE 1024
#define READ_TIMEOUT  500      // milliseconds



class PortHandler
{

public:
	PortHandler(HANDLE hCom);
	PortHandler(void);
	~PortHandler(void);
//------------------------------------------------------------------------------
//Variables
	int iExitCode;
	

//------------------------------------------------------------------------------
//Methods

	bool readData(char *lpBuf, DWORD dwSize);
	bool writeData(const char *lpBuf, DWORD dwSize);
	int checkPortStatus();
	BOOL ReadFromCOMPort(unsigned char* c_Data);
	BOOL WriteToCOMPort(unsigned char* c_Data);


private:
//------------------------------------------------------------------------------
//Variables
	Com _com;
	HANDLE hCom;
	Tools tools;
	//char * _lpBuf;
};

#endif