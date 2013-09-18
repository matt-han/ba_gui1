#include "Com.h"

//------------------------------------------------------------------------------
//	Constructor for new port
//	Parameters:
//	 IN:
//		- string sPort	-> COM port in the system for the programm to open
//------------------------------------------------------------------------------
Com::Com(string sPort)
{
	
	_iError = 0;
	iExitCode = 0;

	clog << "Opening port " << sPort <<endl;
	this->sPort=sPort;


	//open port
	hCom = openPort(sPort);

	if (INVALID_HANDLE_VALUE == hCom)
	{
		_iError = GetLastError();
		clog << "Error opening Port " << sPort
			 << ". Invalid Handle was returned. System Error : "
			 << _iError << endl;

		iExitCode = ERROR_PORT_OPEN;
	}
	else	//get dcb config
	{
		if ( ERROR_SUCCESS != getDCB())
		{
			clog << "Port " << sPort << " settings could NOT be opened" << endl;
			iExitCode = ERROR_GET_DCB;
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
	saDefaultBaudrates[19] = "256000";
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
	saDefaultBaudrates[19] = "256000";
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
	clog << "\n**********************" << endl;
	clog << "Trying to open " << this->sPort << endl << endl;

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
int Com::closePort()
{
	unsigned int close = CloseHandle(this->hCom);
	if(close == 0)
	{
		DWORD _iError = GetLastError();
		clog << "Error closing COM port. Error : " << _iError << endl;
		return ERROR_CLOSE_PORT;
	}
	else
	{
		clog << "Closing " << this->sPort << endl << endl;
		return ERROR_SUCCESS;
	}
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
	string sPort;
 
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
			sPort = szComPort;
			//sPort = sPort.substr(sPort.find_first_of("C"), sPort.npos);
			vPortList.push_back(sPort);
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
int Com::getBaudrates(string sChosenPort)
{
	if (INVALID_HANDLE_VALUE != openPort(sChosenPort) )
	{
		if ( 0 != GetCommProperties(hCom, &commProp))
		{
			//clog << "Get baud rate for port " << sChosenPort << endl;

			_iError = decodeBaudrates(commProp.dwSettableBaud);
			if (ERROR_SUCCESS == _iError)
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
//	List available baud rate for selected port
//	Parameter
//	 IN:
//		- string sChosenPort -> COM port which available baud rates will be
//								listed
//	Return: vector with posible baud rates
//------------------------------------------------------------------------------
vector<string> Com::returnBaudrates(string sChosenPort)
{
	vBaud.clear();

	if (INVALID_HANDLE_VALUE != openPort(sChosenPort) )
	{
		if ( 0 != GetCommProperties(hCom, &commProp))
		{
			clog << "Get baud rate for port " << sChosenPort << endl;

			_iError = decodeBaudrates(commProp.dwSettableBaud);
			if (ERROR_SUCCESS == _iError)
			{
				closePort();
				return vBaud;
			}
			else
			{
				clog << "Error decoding baud rates" << endl;
				
				if (ERROR_SUCCESS != closePort())
					clog << "Error closing port " << sChosenPort << endl;
				
				return vBaud;
			}
		}
		else
		{
			iExitCode = GetLastError();
			clog << "Error getting posible baud rates. System error: "
				 << iExitCode << endl;
			
			if (ERROR_SUCCESS == closePort())
					return vBaud;
				else
				{
					clog << "Error closing port " << sChosenPort << endl;
					return vBaud;
				}
		}
	}
	else
	{
		iExitCode = GetLastError();
		clog << "Error opening port to get baud rates. System error: "
			 << iExitCode << endl;
		return vBaud;
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
int Com::decodeBaudrates(DWORD dwBitMask)
{
	//bitmask with the possible baud rates
	bitset<32> bitMask ((int)dwBitMask);
	
	//ignore last 4 bits -> BAUD_USER
	for( int i = 0; i < 28; i++)
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
		//clog << vBaud.size() << " baud rates available for port" << endl;
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
int Com::translateBaudrate(string sBaud)
{
	if(sBaud == "75")
		return B_075;

	else if(sBaud == "110")
		return B_110;

	else if(sBaud == "134.5")
		return B_134_5;

	else if(sBaud == "150")
		return B_150;

	else if(sBaud == "300")
		return B_300;

	else if(sBaud == "600")
		return B_600;

    else if(sBaud == "1200")
		return B_1200;

	else if(sBaud == "1800")
		return B_1800;

    else if(sBaud == "2400")
		return B_2400;

    else if(sBaud == "4800")
		return B_4800;

	else if(sBaud == "7200")
		return B_7200;

	else if(sBaud == "9600")
		return B_9600;

    else if(sBaud == "14400")
		return B_14400;

    else if(sBaud == "19200")
		return B_19200;

    else if(sBaud == "38400")
		return B_38400;

    else if(sBaud == "56000")
		return B_56K;

	else if(sBaud == "57600")
		return B_57600;

    else if(sBaud == "115200")
		return B_115200;

    else if(sBaud == "128000")
		return B_128K;

	else if(sBaud == "256000")
		return B_256K;
	
	//SUNIX BAUD RATES
	//
	//
	//
	else
		return ERROR_BAUDRATE;
}


//------------------------------------------------------------------------------
//	Set the timeouts for the selected baudrate
//	Parameter
//	 IN:
//		- int iTimeOut -> time out calculated according to the baudrate
//
//	Return: error code signaling if setting the timeouts succeded
//------------------------------------------------------------------------------
int Com::setTimeOuts(int iTimeOut)
{
	// Specify time-out between charactor for receiving.
	//value from msdn
	timeouts.ReadIntervalTimeout = 20;
	// Specify value that is multiplied 
	// by the requested number of bytes to be read. 
	timeouts.ReadTotalTimeoutMultiplier = iTimeOut;
	// Specify value is added to the product of the 
	// ReadTotalTimeoutMultiplier member
	//value from msdn
	timeouts.ReadTotalTimeoutConstant = 100;
	// Specify value that is multiplied 
	// by the requested number of bytes to be sent.
	timeouts.WriteTotalTimeoutMultiplier = iTimeOut;
	// Specify value is added to the product of the 
	// WriteTotalTimeoutMultiplier member
	//value from msdn
	timeouts.WriteTotalTimeoutConstant = 100;


	//EVENT BASED READ

	//// Specify time-out between character for receiving.
	//timeouts.ReadIntervalTimeout = MAXDWORD;
	//// Specify value that is multiplied 
	//// by the requested number of bytes to be read. 
	//timeouts.ReadTotalTimeoutMultiplier = 0;
	//// Specify value is added to the product of the 
	//// ReadTotalTimeoutMultiplier member
	//timeouts.ReadTotalTimeoutConstant = 0;
	//// Specify value that is multiplied 
	//// by the requested number of bytes to be sent.
	//timeouts.WriteTotalTimeoutMultiplier = 0;
	//// Specify value is added to the product of the 
	//// WriteTotalTimeoutMultiplier member
	//timeouts.WriteTotalTimeoutConstant = 0;



		//set the parameter to the open port
		if (!SetCommTimeouts(hCom, &timeouts))
		{
			_iError = GetLastError();
			clog << "Error setting the timeouts. System error : " << _iError << endl;
			return ERROR_SET_TIMEOUTS;
		}
		return ERROR_SUCCESS;
}

//------------------------------------------------------------------------------
//	Calculate the timeouts acording to the information length and baud rate
//	Parameter
//	 IN:
//		- int iParity -> set parity for the port
//		- int iStopbits -> set stopbits for the port
//		- int iBaud -> set baud rate for the port
//	Return: returns the calculated timeout in milisecond
//------------------------------------------------------------------------------
int Com::calculateTimeOut(int iParity, int iStopbits, int iDatabits, int iBaud)
{
	double dPar;
	double dStop;
	double dData;
	int iTimeOutms = 0;
	double dTimeOut = 0;
	 
	//determine how many bits are sent per character
	if(iParity == 0)
		dPar = 0.0;
	else
		dPar = 1.0;
	
	if(iStopbits == 0)
		dStop = 1.0;
	else
		dStop = 2.0;

	if(iDatabits == 8)
		dData = 8.0;
	else
		dData = 7.0;

	double dAnzBits = dPar + dData + dStop + 1; //+1 for startbit
	
	// Time out per character is equal to
	// the number of bits per character divided by the baud rate
	// multiplied with 1.1 for a 10% delay increment
	dTimeOut = dAnzBits / (double)iBaud * 1.1;
	iTimeOutms = 1000 * dTimeOut;

	//if less than 1 milisecond, make it 1 ms
	if (iTimeOutms < 1)
		iTimeOutms = 1;

	return iTimeOutms;
}

//------------------------------------------------------------------------------
//	Save the port DCB structure for the opened port
//
//	Return: error code signaling if loading the DCB succeded
//------------------------------------------------------------------------------
int Com::getDCB()
{
	//get original dcb
		if (0 == GetCommState(hCom, &dcb))
		{
			MessageBoxA(NULL, "Error getting Port default settings",
				WINDOW_TITLE, MB_OK | MB_ICONERROR);

			_iError = GetLastError();
			clog << "Error getting Port default settings. Error : "
				 << _iError << endl;

			return ERROR_GET_DCB;
		}
		else
		{
			return ERROR_SUCCESS;
		}
}

//------------------------------------------------------------------------------
//	Set the port DCB structure after being edited
//	Return: error code signaling if setting the DCB succeded
//------------------------------------------------------------------------------
int Com::setDCB()
{
	//set original dcb
	dcb.fParity = TRUE;

		if (0 == SetCommState(hCom, &dcb))
		{
			_iError = GetLastError();
			
			MessageBoxA(NULL, "Error saving Port settings. For more details check "
				"the log file if it was enabled", WINDOW_TITLE, MB_OK | MB_ICONERROR);

			clog << "Error setting " << sPort << " dcb. System Error : "
				 << _iError << endl;
			clog << "Probably baud rate is not truly supported. Check with \"mode com\'x\'\" in the command line" << endl;

			clog << "BaudRate :" << dcb.BaudRate << endl;
			clog << "Parity   :" << (int)dcb.Parity   << endl;
			clog << "Stopbits :" << (int)dcb.StopBits << endl;
			clog << "Databits :" << (int)dcb.ByteSize << endl;
			return ERROR_SET_DCB;
		}
		else

		return ERROR_SUCCESS;
}


//Debbug methode, prints DCB structure
//void Com::printDCB()
//{
//	clog << "\n\n DCB SETTINGS for " << this->sPort<<endl;
//	clog << "---------------------------------"<<endl;
//	clog << "fBinary			" << dcb.fBinary << endl;
//	clog << "fParity			" << dcb.fParity << endl;
//	clog << "fOutxCtsFlow		" << dcb.fOutxCtsFlow << endl;
//	clog << "fOutxDsrFlow		" << dcb.fOutxDsrFlow << endl;
//	clog << "fDtrControl		" << dcb.fDtrControl  << endl;
//	clog << "fDsrSensitivity	" << dcb.fDsrSensitivity  << endl;
//	clog << "fTXContinueOnXoff	" << dcb.fTXContinueOnXoff  << endl;
//	clog << "fOutX				" << dcb.fOutX  << endl;
//	clog << "fInX				" << dcb.fInX  << endl;
//	clog << "fErrorChar			" << dcb.fErrorChar  << endl;
//	clog << "fNull				" << dcb.fNull  << endl;
//	clog << "fRtsControl		" << dcb.fRtsControl  << endl;
//	clog << "fAbortOnError		" << dcb.fAbortOnError  << endl;
//	clog << "XonLim				" << dcb.XonLim << endl;
//	clog << "XoffLim			" << dcb.XoffLim << endl;
//	clog << "ByteSize			" << dcb.ByteSize  << endl;
//	clog << "Parity				" << dcb.Parity << endl;
//	clog << "StopBits			" << dcb.StopBits << endl;
//	clog << "XonChar			" << dcb.XonChar << endl;
//	clog << "XoffChar			" << dcb.XoffChar << endl;
//	clog << "ErrorChar			" << dcb.ErrorChar<< endl;
//	clog << "EofChar			" << dcb.EofChar << endl;
//	clog << "EvtChar			" << dcb.EvtChar  << endl;
//
//
//}
