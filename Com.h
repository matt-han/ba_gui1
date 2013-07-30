#ifndef _COM_H
#define _COM_H

#include "Constants.h"
#include "Tools.h"

#include <Windows.h>
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;



class Com
{
public:
	Com(string sPort);
	Com(void);
	~Com(void);

	vector<string> vPortList;
	//determine from registry or device manager
	string sBaudrates[11];
	int iExitCode;

	HANDLE openPort(string portNumber);
	bool closePort();

	//void setParity(int i);
	//void setProtocol(int i);
	//void setStopBits(int i);
	//void setTransfer(int i);
	//

	void enumeratePorts();

protected:

	

	//Device-Control Block
	DCB dcb;
	HANDLE hCom;

	bool protocol;

	bool transfer;


private:
	DWORD fBinary;
	DWORD _dwError;
	Tools tool;



};
#endif

//	DWORD	unsigned long
//	BYTE	unsigned 8-bit

//typedef struct _DCB {
//  DWORD DCBlength;
//  DWORD BaudRate;
//  DWORD fBinary  :1;
//  DWORD fParity  :1;
//  DWORD fOutxCtsFlow  :1;
//  DWORD fOutxDsrFlow  :1;
//  DWORD fDtrControl  :2;
//  DWORD fDsrSensitivity  :1;
//  DWORD fTXContinueOnXoff  :1;
//  DWORD fOutX  :1;
//  DWORD fInX  :1;
//  DWORD fErrorChar  :1;
//  DWORD fNull  :1;
//  DWORD fRtsControl  :2;
//  DWORD fAbortOnError  :1;
//  DWORD fDummy2  :17;
//  WORD  wReserved;
//  WORD  XonLim;
//  WORD  XoffLim;
//  BYTE  ByteSize;
//  BYTE  Parity;
//  BYTE  StopBits;
//  char  XonChar;
//  char  XoffChar;
//  char  ErrorChar;
//  char  EofChar;
//  char  EvtChar;
//  WORD  wReserved1;
//} DCB, *LPDCB;
