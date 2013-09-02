/*
 *	Class to handle input and output operations in a opened COM port
 */

#ifndef _PORTCOMMUNICATIONS_H
#define _PORTCOMMUNICATIONS_H



#include "Tools.h"
#include <Windows.h>

//------------------------------------------------------------------------------
//Constants
#define READ_BUF_SIZE 1024
#define READ_TIMEOUT  500      // milliseconds



class PortCommunications
{

public:
	PortCommunications(HANDLE hCom);
	PortCommunications(void);
	~PortCommunications(void);
//------------------------------------------------------------------------------
//Variables
	

//------------------------------------------------------------------------------
//Methods
	void setComHandle(HANDLE hCom);
	bool readData(char *lpBuf, DWORD dwSize, int iReadTimeOut);
	bool writeData(const char *lpBuf, DWORD dwSize);
	int checkPortStatus();
	//BOOL ReadFromCOMPort(unsigned char* c_Data);
//	BOOL WriteToCOMPort(unsigned char* c_Data);


private:
//------------------------------------------------------------------------------
//Variables

	HANDLE hCom;
	Tools tools;
	//char * _lpBuf;
};

#endif