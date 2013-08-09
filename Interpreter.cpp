#include "Interpreter.h"

//------------------------------------------------------------------------------
//	Constructor for GUI interpreter
//	Parameters:
//	 IN:
//		- Window *window	-> pointer to created GUI
//----------------------------------------------------------------------------
//Interpreter::Interpreter(Window *window)
//{
//	//Window * window = &window;
//	this->window = window;
//
//	_iTestMode = -1;
//	_iParity   = -1;
//	_iStopBits = -1;
//	_iTransfer = -1;
//	_iProtocol = -1;
//	_dwBaudrate = 0;
//
//	_sPort = "";
//	_sTransferFile = "";
//	
//}


//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
Interpreter::Interpreter(void)
{
	_iTestMode = -1;
	_iParity   = -1;
	_iStopBits = -1;
	_iTransfer = -1;
	_iProtocol = -1;
	_dwBaudrate = 0;

	_sPort = "";
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
void Interpreter::setSelectedPort(string sPort)
{
	this->_sPort = sPort;
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
void Interpreter::handleGui(int iInformationToTransfer)
{
	//iInformationToTransfer
	//0 for default
	//1 for file
	//2 for string
	_iInfoToTransfer = iInformationToTransfer;

	//Always get test mode, port, logger and which information to transfer
	//_iTestMode	  = window->getTestMode();
	//_sPort		  = window->getSelectedPort();
	//_bLoggerState = window->getLoggerState();

	
	
	//when windows closes then is handle null
	//action executed with start button, change to
	//here when observer pattern inplemented
	//_sTransferFile = window->getTransferFile();



	if( _sPort != "")
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
