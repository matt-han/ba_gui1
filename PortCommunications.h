/*
 *	Class to handle input and output operations in a opened COM port
 */

#ifndef _PORTCOMMUNICATIONS_H
#define _PORTCOMMUNICATIONS_H

#include "Tools.h"
#include <Windows.h>


class PortCommunications
{

public:
	PortCommunications(HANDLE hCom);
	PortCommunications(void);
	~PortCommunications(void);

//------------------------------------------------------------------------------
//Methods
	void setComHandle(HANDLE hCom);
	bool readData(char *lpBuf, DWORD dwSize);
	bool writeData(const char *lpBuf, DWORD dwSize);

private:
//------------------------------------------------------------------------------
//Variables

	HANDLE hCom;
	Tools tools;
};

#endif