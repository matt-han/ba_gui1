#include "Com.h"

//------------------------------------------------------------------------------
//	Constructor for new port
//	Parameters:
//	 IN:
//		- string sPort	-> COM port in the system for the programm to open
//------------------------------------------------------------------------------
Com::Com(string sPort)
{
	
	_dwError = 0;
	iExitCode = 0;

	clog << "Opening port " << sPort <<endl;
	this->sPort=sPort;
	
	//Win32 API does not support nonbinary mode transfers
	fBinary = true;

	//Struct with COM port properties
	DCB dcbOrg = {0};

	//open port
	hCom = openPort(sPort);

	if (INVALID_HANDLE_VALUE == hCom)
	{
		_dwError = GetLastError();
		clog << "Error opening Port " << sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _dwError << endl;

		iExitCode = ERROR_PORT_OPEN;
	}
	else	//get dcb config
	{
		//get original dcb
		if (GetCommState(hCom, &dcbOrg) == 0)
		{
			MessageBoxW(NULL, L"Error getting Port default settings",
							  L"ERROR", MB_OK);

			_dwError = GetLastError();
			clog << "Error getting Port default settings´. Error : "
				 << _dwError << endl;

			iExitCode = ERROR_GET_DCB;
		}
		else
		{
			clog << "Port " << sPort << " settings successfully opened" << endl;
		}

		//Port timeout struct
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
			iExitCode = ERROR_SET_TIMEOUTS;
		}
	}
	
	//string array for possible port baud rates
	saDefaultBaudrates[0]  = "75";
	saDefaultBaudrates[1]  = "110";
	saDefaultBaudrates[2]  = "134.5";
	saDefaultBaudrates[3]  = "150";
	saDefaultBaudrates[4]  = "300";
	saDefaultBaudrates[5]  = "600";
	saDefaultBaudrates[6]  = "1200";
	saDefaultBaudrates[7]  = "1800";
	saDefaultBaudrates[8]  = "2400";
	saDefaultBaudrates[9]  = "4800";
	saDefaultBaudrates[10] = "7200";
	saDefaultBaudrates[11] = "9600";
	saDefaultBaudrates[12] = "14400";
	saDefaultBaudrates[13] = "19200";
	saDefaultBaudrates[14] = "38400";
	saDefaultBaudrates[15] = "56000";
	saDefaultBaudrates[16] = "57600";
	saDefaultBaudrates[17] = "115200";
	saDefaultBaudrates[18] = "128000";
	//saDefaultBaudrates[0] = ;
	//saDefaultBaudrates[0] = ;

   
}



//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
Com::Com(void)
{
	//string array for possible port baud rates
	saDefaultBaudrates[0]  = "75";
	saDefaultBaudrates[1]  = "110";
	saDefaultBaudrates[2]  = "134.5";
	saDefaultBaudrates[3]  = "150";
	saDefaultBaudrates[4]  = "300";
	saDefaultBaudrates[5]  = "600";
	saDefaultBaudrates[6]  = "1200";
	saDefaultBaudrates[7]  = "1800";
	saDefaultBaudrates[8]  = "2400";
	saDefaultBaudrates[9]  = "4800";
	saDefaultBaudrates[10] = "7200";
	saDefaultBaudrates[11] = "9600";
	saDefaultBaudrates[12] = "14400";
	saDefaultBaudrates[13] = "19200";
	saDefaultBaudrates[14] = "38400";
	saDefaultBaudrates[15] = "56000";
	saDefaultBaudrates[16] = "57600";
	saDefaultBaudrates[17] = "115200";
	saDefaultBaudrates[18] = "128000";
}



//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
Com::~Com(void)
{
}



//------------------------------------------------------------------------------
//Open a com port and returns its handle
//Parameters:
//  IN:
//	- string portNumber	-> COM port in the system for the programm to open
//Returns handle to opened port
//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
//	Close the opened com port
//	Return: indicating if close was successful
//------------------------------------------------------------------------------
long Com::closePort()
{
	unsigned int close = CloseHandle(this->hCom);
	if(close == 0)
	{
		DWORD _dwError = GetLastError();
		clog << "Error closing COM port. Error : " << _dwError << endl;
		return ERROR_CLOSE_PORT;
	}
	else
		return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Enumerates available COM ports in the current system
//	Fills the vPortList vector with the available ports
//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
//	Get available baud rate for selected port
//	Parameter
//	 IN:
//		- string sChosenPort -> COM port which available baud rates will be
//								listed
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
long Com::getBaudrates(string sChosenPort)
{
	if (INVALID_HANDLE_VALUE != openPort(sChosenPort) )
	{
		if ( 0 != GetCommProperties(hCom, &commProp))
		{
			clog << "Get baud rate for port " << sChosenPort << endl;

			_dwError = decodeBaudrates(commProp.dwSettableBaud);
			if (ERROR_SUCCESS == _dwError)
			{
				return closePort();
			}
			else
			{
				clog << "Error decoding baud rate" << endl;
				
				if (ERROR_SUCCESS == closePort())
					return ERROR_BAUDRATE;
				else
				{
					clog << "Error closing port " << sChosenPort << endl;
					return ERROR_BAUDRATE;
				}
			}
		}
		else
		{
			iExitCode = GetLastError();
			clog << "Error getting posible baud rates. System error: "
				 << iExitCode << endl;
			
			if (ERROR_SUCCESS == closePort())
					return ERROR_BAUDRATE;
				else
				{
					clog << "Error closing port " << sChosenPort << endl;
					return ERROR_BAUDRATE;
				}
		}
	}
	else
	{
		iExitCode = GetLastError();
		clog << "Error opening port to get baud rates. System error: "
			 << iExitCode << endl;
		return ERROR_PORT_OPEN;
	}
}


//------------------------------------------------------------------------------
//	Decode available baud rates for opened port. Fills vector vBaud with
//	available baud rates
//
//	Parameters:
//	 IN:
//		- DWORD dwBitMask -> bit mask containing the available baud rates
//	Return: error code signaling 
//------------------------------------------------------------------------------
long Com::decodeBaudrates(DWORD dwBitMask)
{
	//bitmask with the possible baud rates
	bitset<32> bitMask ((int)dwBitMask);
	
	//ignore first bit -> 75 bps
	//ignore last 4 bits -> BAUD_USER
	for( int i = 1; i < 28; i++)
	{
		//if bit set
		if (bitMask.test(i) == true)
		{
			//then the baud rate is available
			vBaud.push_back(saDefaultBaudrates[i]);
		}
	}

	if (vBaud.size() > 0)
	{
		clog << vBaud.size() << " baud rates available for port" << endl;
		return ERROR_SUCCESS;
	}
	else
		return ERROR_BAUDRATE;
}


//------------------------------------------------------------------------------
//	Translate the listed baud rates to system defined baud rates
//	Parameters:
//	 IN:
//		- string sBaud -> baud rate selected by the user to be translated
//
//	Return: error code signaling fail or correct baud rate
//------------------------------------------------------------------------------
DWORD Com::translateBaudrate(string sBaud)
{
	if(sBaud == "110")
		return CBR_110;

	else if(sBaud == "300")
		return CBR_300;

	else if(sBaud == "600")
		return CBR_600;

    else if(sBaud == "1200")
		return CBR_1200;

    else if(sBaud == "2400")
		return CBR_2400;

    else if(sBaud == "4800")
		return CBR_4800;

	else if(sBaud == "9600")
		return CBR_9600;

    else if(sBaud == "14400:")
		return CBR_14400;

    else if(sBaud == "19200")
		return CBR_19200;

    else if(sBaud == "38400")
		return  CBR_38400;

    else if(sBaud == "56000")
		return CBR_56000;

	else if(sBaud == "57600")
		return CBR_57600;

    else if(sBaud == "115200")
		return CBR_115200;

    else if(sBaud == "128000")
		return CBR_128000;

	else if(sBaud == "256000")
		return  CBR_256000;
	else
		return ERROR_BAUDRATE;
}
//
//
//
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
