#include "Com.h"


//Constructor for new object
Com::Com(string sPort)
{
	
	_dwError = 0;
	iExitCode = 0;

	clog << "Opening port " << sPort <<endl;
	this->sPort=sPort;
	
	//Win32 API does not support nonbinary mode transfers
	fBinary = true;
	
	DCB dcbOrg = {0};
	//open port
	hCom = openPort(sPort);
	if (INVALID_HANDLE_VALUE == hCom)
	{
		_dwError = GetLastError();
		clog << "Error opening Port " << sPort << ". Invalid Handle was returned. System Error : " << _dwError << endl;
		iExitCode = ERR_PORT_OPEN;
	}
	else	//get dcb config
	{
		//get original dcb
		if (GetCommState(hCom, &dcbOrg) == 0)
		{
			MessageBoxW(NULL, L"Error getting Port default settings", L"ERROR", MB_OK);
			_dwError = GetLastError();
			clog << "Error getting Port default settings´. Error : " << _dwError << endl;
			iExitCode = ERR_GET_DCB;
		}
		else
		{
			clog << "Port " << sPort << " settings successfully opened" << endl;
		}

		COMMTIMEOUTS timeouts;

		// Specify time-out between charactor for receiving.
		timeouts.ReadIntervalTimeout = 20;
		// Specify value that is multiplied 
		// by the requested number of bytes to be read. 
		timeouts.ReadTotalTimeoutMultiplier = 10;
		// Specify value is added to the product of the 
		// ReadTotalTimeoutMultiplier member
		timeouts.ReadTotalTimeoutConstant = 100;
		// Specify value that is multiplied 
		// by the requested number of bytes to be sent.
		timeouts.WriteTotalTimeoutMultiplier = 10;
		// Specify value is added to the product of the 
		// WriteTotalTimeoutMultiplier member
		timeouts.WriteTotalTimeoutConstant = 100;

		//set the parameter to the open port
		if (!SetCommTimeouts(hCom, &timeouts))
		{
			clog << "Error setting the timeouts" << endl;
		}
	}
	

   
}
Com::Com(void)
{

}

Com::~Com(void)
{
}
//
//
//Open a com port and returns its handle
HANDLE Com::openPort(string portNumber)
{
	

	hCom = CreateFileA(portNumber.c_str(),  
            GENERIC_READ|GENERIC_WRITE, // desired access should be read&write  
            0,                          // COM port must be opened in non-sharing mode  
            NULL,                       // don't care about the security  
            OPEN_EXISTING,              // IMPORTANT: must use OPEN_EXISTING for a COM port  
            FILE_FLAG_OVERLAPPED,        
            NULL);                      // always NULL for a general purpose COM port  

	return hCom;
}
//
//
//
//Close the opened com port
bool Com::closePort()
{
	unsigned int close = CloseHandle(this->hCom);
	if(close == 0)
	{
		DWORD _dwError = GetLastError();
		clog << "Error closing COM port. Error : " << _dwError << endl;
		return false;
	}
	else
		return true;
}
//
//
//
//Get available baud rate for opened port
long Com::getBaudrates()
{
	if ( 0 != GetCommProperties(this->hCom, &commProp))
	{
		clog << "baud rate for " << sPort << endl;
		dwMaxBaud = commProp.dwMaxBaud;
		clog << "dwSettableBaud " << commProp.dwSettableBaud << endl;
		//clog << "maxBaud " << dwMaxBaud << endl;

		if (dwMaxBaud == BAUD_USER)
		{
			clog << "maxBaud BAUD_USER" << endl;
			return ERROR_BAUDRATE;
		}
		else
		{
			clog << "maxBaud " << dwMaxBaud << endl;
			return ERROR_SUCCESS;
		}
	}
	else
	{
		iExitCode = GetLastError();
		clog << "Error getting posible baud rates. System error: " << iExitCode << endl;
		return ERROR_BAUDRATE;
	}
}

//
////
////
////
////set port communication protocol
//void Com::setProtocol(int i)
//{
//	switch(i)
//		{
//		//Xon/Xoff
//		case 0:
//			this->dcb.fOutX = true;
//			this->dcb.fInX = true;
//			//this->dcb.XonChar = ;
//			//this->dcb.XoffChar = ;
//			break;
//
//		//DTR/DSR
//		case 1:
//			
//			break;
//
//		//CTS/RTS
//		case 2:
//			
//			break;
//		}
//}
////
////
////
////BYTE stopBit -> ONESTOPBIT; ONE5STOPBITS; TWOSTOPBITS
//void Com::setStopBits(int i)
//{
//		switch(i)
//		{
//		case 0:
//			this->dcb.StopBits = ONESTOPBIT;
//			break;
//
//		case 1:
//			this->dcb.StopBits = ONE5STOPBITS;
//			break;
//
//		case 2:
//			this->dcb.StopBits = TWOSTOPBITS;
//			break;
//		}
//
//}
////
////
////
////
//void Com::setTransfer(int i)
//{
//		switch(i)
//		{
//		case 0:
//			this->dcb.StopBits = ONESTOPBIT;
//			break;
//
//		case 1:
//			this->dcb.StopBits = ONE5STOPBITS;
//			break;
//
//		case 2:
//			this->dcb.StopBits = TWOSTOPBITS;
//			break;
//		}
//
//}
////
////
////
////
void Com::enumeratePorts()
{
	//CUIntArray arrComPortNo; 
	char szComPort[11];
	HANDLE hPort;
 
    for (int i = 1; i <= 255; ++i)  
    {  
        if (i < 10)
			sprintf(szComPort, "COM%d", i);
        else
			sprintf(szComPort, "\\\\.\\COM%d", i);

        hPort = openPort(szComPort);
 
        if (INVALID_HANDLE_VALUE == hPort)  
        {  
            if (ERROR_ACCESS_DENIED == GetLastError() )  
            {   // then it exists and currently opened
				vPortList.push_back(szComPort);
            }  
        }  
        else  
        {   // COM port exist  
			vPortList.push_back(szComPort);
            CloseHandle(hCom);  
        } 
    }
}


/*
switch(atoi(m_baudRate))
  {
  case 110:
    configSerial_.BaudRate = CBR_110;
    break;
  case 300:
    configSerial_.BaudRate = CBR_300;
    break;
  case 600:
    configSerial_.BaudRate = CBR_600;
    break;
  case 1200:
    configSerial_.BaudRate = CBR_1200;
    break;
  case 2400:
    configSerial_.BaudRate = CBR_2400;
    break;
  case 4800:
    configSerial_.BaudRate = CBR_4800;
    break;
  case 9600:
    configSerial_.BaudRate = CBR_9600;
    break;
  case 14400:
    configSerial_.BaudRate = CBR_14400;
    break;
  case 19200:
    configSerial_.BaudRate = CBR_19200;
    break;
  case 38400:
    configSerial_.BaudRate = CBR_38400;
    break;
  case 56000:
    configSerial_.BaudRate = CBR_56000;
    break;
  case 57600:
    configSerial_.BaudRate = CBR_57600;
    break;
  case 115200 :
    configSerial_.BaudRate = CBR_115200;
    break;
  case 128000:
    configSerial_.BaudRate = CBR_128000;
    break;
  case 256000:
    configSerial_.BaudRate = CBR_256000;
    break;
  default:
    break;
  }*/