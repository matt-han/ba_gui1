#include "Com.h"


//Constructor for new object
Com::Com(string sPort)
{
	_dwError = 0;
	iExitCode = 0;

	clog << "Opening port " << sPort <<endl;

	
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
	}// if open
	

   
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