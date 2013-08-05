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

		//// Specify time-out between charactor for receiving.
		//timeouts.ReadIntervalTimeout = 20;
		//// Specify value that is multiplied 
		//// by the requested number of bytes to be read. 
		//timeouts.ReadTotalTimeoutMultiplier = 10;
		//// Specify value is added to the product of the 
		//// ReadTotalTimeoutMultiplier member
		//timeouts.ReadTotalTimeoutConstant = 100;
		//// Specify value that is multiplied 
		//// by the requested number of bytes to be sent.
		//timeouts.WriteTotalTimeoutMultiplier = 10;
		//// Specify value is added to the product of the 
		//// WriteTotalTimeoutMultiplier member
		//timeouts.WriteTotalTimeoutConstant = 100;

		// Specify time-out between charactor for receiving.
		timeouts.ReadIntervalTimeout = MAXDWORD;
		// Specify value that is multiplied 
		// by the requested number of bytes to be read. 
		timeouts.ReadTotalTimeoutMultiplier = 0;
		// Specify value is added to the product of the 
		// ReadTotalTimeoutMultiplier member
		timeouts.ReadTotalTimeoutConstant = 0;
		// Specify value that is multiplied 
		// by the requested number of bytes to be sent.
		timeouts.WriteTotalTimeoutMultiplier = 0;
		// Specify value is added to the product of the 
		// WriteTotalTimeoutMultiplier member
		timeouts.WriteTotalTimeoutConstant = 0;

		//set the parameter to the open port
		if (!SetCommTimeouts(hCom, &timeouts))
		{
			clog << "Error setting the timeouts" << endl;
		}
	}
	
	StandardBaudrates[0]  = "BAUD_075";
	StandardBaudrates[1]  = "BAUD_110";
	StandardBaudrates[2]  = "BAUD_134_5";
	StandardBaudrates[3]  = "BAUD_150";
	StandardBaudrates[4]  = "BAUD_300";
	StandardBaudrates[5]  = "BAUD_600";
	StandardBaudrates[6]  = "BAUD_1200";
	StandardBaudrates[7]  = "BAUD_1800";
	StandardBaudrates[8]  = "BAUD_2400";
	StandardBaudrates[9]  = "BAUD_4800";
	StandardBaudrates[10] = "BAUD_7200";
	StandardBaudrates[11] = "BAUD_9600";
	StandardBaudrates[12] = "BAUD_14400";
	StandardBaudrates[13] = "BAUD_19200";
	StandardBaudrates[14] = "BAUD_38400";
	StandardBaudrates[15] = "BAUD_56K";
	StandardBaudrates[16] = "BAUD_57600";
	StandardBaudrates[17] = "BAUD_115200";
	StandardBaudrates[18] = "BAUD_128K";
	//dwStandardBaudrates[0] = ;
	//dwStandardBaudrates[0] = ;

   
}
//
//
//
Com::Com(void)
{
	//StandardBaudrates[0]  = BAUD_075;
	//StandardBaudrates[1]  = BAUD_110;
	//StandardBaudrates[2]  = BAUD_134_5;
	//StandardBaudrates[3]  = BAUD_150;
	//StandardBaudrates[4]  = BAUD_300;
	//StandardBaudrates[5]  = BAUD_600;
	//StandardBaudrates[6]  = BAUD_1200;
	//StandardBaudrates[7]  = BAUD_1800;
	//StandardBaudrates[8]  = BAUD_2400;
	//StandardBaudrates[9]  = BAUD_4800;
	//StandardBaudrates[10] = BAUD_7200;
	//StandardBaudrates[11] = BAUD_9600;
	//StandardBaudrates[12] = BAUD_14400;
	//StandardBaudrates[13] = BAUD_19200;
	//StandardBaudrates[14] = BAUD_38400;
	//StandardBaudrates[15] = BAUD_56K;
	//StandardBaudrates[16] = BAUD_57600;
	//StandardBaudrates[17] = BAUD_115200;
	//StandardBaudrates[18] = BAUD_128K;
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
//
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
//
//
//
//Get available baud rate for selected port
long Com::getBaudrates(string sPort)
{
	if (INVALID_HANDLE_VALUE != openPort(sPort) )
	{
		if ( 0 != GetCommProperties(hCom, &commProp))
		{
			clog << "baud rate for " << sPort << endl;

			if (ERROR_SUCCESS == parseBaudrates(commProp.dwSettableBaud) )
			{
				closePort();
				return ERROR_SUCCESS;
			}
			else
			{
				closePort();
				return ERROR_BAUDRATE;
			}
		}
		else
		{
			iExitCode = GetLastError();
			clog << "Error getting posible baud rates. System error: " << iExitCode << endl;
			closePort();
			return ERROR_BAUDRATE;
		}
	}
	else
	{
		iExitCode = GetLastError();
		clog << "Error opening port to get baud rates. System error: " << iExitCode << endl;
		return ERR_PORT_OPEN;
	}
}
//
//
//
//
long Com::parseBaudrates(DWORD dwBitMask)
{
	bitset<32> bitMask ((int)dwBitMask);
	
	for( int i = 0; i < 28; i++)
	{
		if (bitMask.test(i) == true)
		{
			//dwvBaudRates.push_back(dwStandardBaudrates[i]);
			clog <<"rates " << StandardBaudrates[i] << endl;
			//char buffer[32];
			//sprintf(buffer, "%d", dwStandardBaudrates[i]);
			vBaud.push_back(StandardBaudrates[i]);
		}

	}

	if (dwvBaudRates.size() > 0)
	{
		clog << dwvBaudRates.size() << " baud rates available for port" << endl;
		return ERROR_SUCCESS;
	}
	else
		return ERROR_BAUDRATE;
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