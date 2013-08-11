#include "Interpreter.h"

//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
Interpreter::Interpreter(void)
{
	_iTestMode = DEFAULT_VALUE;
	_iParity   = DEFAULT_VALUE;
	_iStopBits = DEFAULT_VALUE;
	_iTransfer = DEFAULT_VALUE;
	_iProtocol = DEFAULT_VALUE;
	_dwBaudrate = DEFAULT_VALUE;

	_sMasPort = "";
	_sSlaPort = "";
	_sTransferFile = "";
	_sTextToSend = "";
}


//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
Interpreter::~Interpreter(void)
{
}


//------------------------------------------------------------------------------
//	Saves the user test mode input in _iTestMode
//	Parameters:
//	 IN:
//		- int iTestMode -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setTestMode(int iTestMode)
{
	this->_iTestMode = iTestMode;
}


//------------------------------------------------------------------------------
//	Saves the user parity input in _iParity
//	Parameters:
//	 IN:
//		- int iParity -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setParity(int iParity)
{
	this->_iParity = iParity;
}


//------------------------------------------------------------------------------
//	Saves the user stopbits input in _iStopBits
//	Parameters:
//	 IN:
//		- int iStopBits -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setStopBits(int iStopBits)
{
	this->_iStopBits = iStopBits;
}


//------------------------------------------------------------------------------
//	Saves the user transfer mode input in _iTransfer
//	Parameters:
//	 IN:
//		- int iTransfer -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setTransfer(int iTransfer)
{
	this->_iTransfer = iTransfer;
}


//------------------------------------------------------------------------------
//	Saves the user protocol input in _iProtocol
//	Parameters:
//	 IN:
//		- int iProtocol -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setProtocol(int iProtocol)
{
	this->_iProtocol = iProtocol;
}


//------------------------------------------------------------------------------
//	Saves the user port input in _sPort
//	Parameters:
//	 IN:
//		-string sPort -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setSelectedMasterPort(string sPort)
{
	this->_sMasPort = sPort;
}

//------------------------------------------------------------------------------
//	Saves the user port input in _sPort
//	Parameters:
//	 IN:
//		-string sPort -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setSelectedSlavePort(string sPort)
{
	this->_sSlaPort = sPort;
}


//------------------------------------------------------------------------------
//	Saves the user baud rate input in _dwBaudrate
//	Parameters:
//	 IN:
//		- DWORD dwBaudRate -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setPortBaudRate(DWORD dwBaudRate)
{
	this->_dwBaudrate = dwBaudRate;
}


//------------------------------------------------------------------------------
//	Saves if the user wishes for a log file
//	Parameters:
//	 IN:
//		- bool bLoggerState -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setLoggerState(bool bLoggerState)
{
	this->_bLoggerState = bLoggerState;
}


//------------------------------------------------------------------------------
//	Saves the user transfer file path input in _sTransferFile
//	Parameters:
//	 IN:
//		- string sTransferFile -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setTransferFile(string sTransferFile)
{
	this->_sTransferFile = sTransferFile;
}


//------------------------------------------------------------------------------
//	Saves the user transfer text input in _iTransfer
//	Parameters:
//	 IN:
//		- string sTextToSend -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setTextToSend(string sTextToSend)
{
	this->_sTextToSend = sTextToSend;
}


//------------------------------------------------------------------------------
//	Gets the GUI settings and saves them in local variables. Starts the tester
//	object to begin the test(s)
//	Parameters:
//	 IN:
//		- int iInformationToTransfer -> description for using default transfer
//					text, user text or user text file
//------------------------------------------------------------------------------
void Interpreter::handleGui()
{
	//iInformationToTransfer
	//0 for default
	//1 for file
	//2 for string
	//_iInfoToTransfer = iTransferText;

	//Always get test mode, port, logger and which information to transfer
	//_iTestMode	  = window->getTestMode();
	//_sPort		  = window->getSelectedPort();
	//_bLoggerState = window->getLoggerState();

	
	
	//when windows closes then is handle null
	//action executed with start button, change to
	//here when observer pattern inplemented
	//_sTransferFile = window->getTransferFile();



	if( _sMasPort != "")
	{
		switch (_iTestMode)
		{
			//Automatic test
			//needs only port and logger settings
			case 0:
				break;

			//Wobbeln
			//needs only port and logger settings
			case 1:
				break;

			//Fixed test
			//get all GUI settings
			case 2:
				//CALL TESTPORPERTIES?????????????????????
				//_iParity   = window->getParity();
				//_iStopBits = window->getStopBits();
				//_iTransfer = window->getTransfer();
				//_iProtocol = window->getProtocol();
				//_dwBaudrate = window->getPortBaudRate();
				break;
			
			//Port was selected but no test mode
			default:
				MessageBoxW(NULL,L"Please select a Test Mode", L"Error", MB_OK);
				break;

		}//switch
	}
	else
	{
		MessageBoxW(NULL,L"Please select the port to be tested", L"Error", MB_OK);
	}
}
