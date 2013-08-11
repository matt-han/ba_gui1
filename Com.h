/*
 *	Class to handle COM port operations
 *	Open
 *	Close
 *  Enumerate ports
 *  Get port baud rates
 */

#ifndef _COM_H
#define _COM_H

#include "PortCommunications.h"
#include "Constants.h"
#include "Tools.h"

#include <Windows.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <bitset>

using namespace std;



class Com
{
public:
	Com(string sPort);
	Com(void);
	~Com(void);

//------------------------------------------------------------------------------
//Variables

	vector<string> vPortList;
	vector<string> vBaud;
	vector<DWORD> dwvBaudRates;
	string saDefaultBaudrates[19];
	int iExitCode;
	HANDLE hCom;

//------------------------------------------------------------------------------
//Methods

	HANDLE openPort(string portNumber);
	long closePort();
	long getBaudrates(string sChosenPort);
	long decodeBaudrates(DWORD dwBitMask);
	int translateBaudrate(string sBaud);
	void enumeratePorts();
	//void setParity(int i);
	//void setProtocol(int i);
	//void setStopBits(int i);
	//void setTransfer(int i);

protected:
//------------------------------------------------------------------------------
//Variables
	string sPort;
	DCB dcb;			//Device-Control Block
	COMMPROP commProp;
	bool protocol;
	bool transfer;

private:
//------------------------------------------------------------------------------
//Variables
	Tools tool;
	DWORD fBinary;
	DWORD _dwError;
	DWORD BaudRate;
	

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



// Change the DCB structure settings.
//PortDCB.BaudRate = 9600;              // Current baud 
//PortDCB.fBinary = TRUE;               // Binary mode; no EOF check 
//PortDCB.fParity = TRUE;               // Enable parity checking 
//PortDCB.fOutxCtsFlow = FALSE;         // No CTS output flow control 
//PortDCB.fOutxDsrFlow = FALSE;         // No DSR output flow control 
//PortDCB.fDtrControl = DTR_CONTROL_ENABLE; 
//                                      // DTR flow control type 
//PortDCB.fDsrSensitivity = FALSE;      // DSR sensitivity 
//PortDCB.fTXContinueOnXoff = TRUE;     // XOFF continues Tx 
//PortDCB.fOutX = FALSE;                // No XON/XOFF out flow control 
//PortDCB.fInX = FALSE;                 // No XON/XOFF in flow control 
//PortDCB.fErrorChar = FALSE;           // Disable error replacement 
//PortDCB.fNull = FALSE;                // Disable null stripping 
//PortDCB.fRtsControl = RTS_CONTROL_ENABLE; 
//                                      // RTS flow control 
//PortDCB.fAbortOnError = FALSE;        // Do not abort reads/writes on 
//                                      // error
//PortDCB.ByteSize = 8;                 // Number of bits/byte, 4-8 
//PortDCB.Parity = NOPARITY;            // 0-4=no,odd,even,mark,space 
//PortDCB.StopBits = ONESTOPBIT;        // 0,1,2 = 1, 1.5, 2