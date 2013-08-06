#include "Interpreter.h"

//------------------------------------------------------------------------------
//	Constructor for GUI interpreter
//	Parameters:
//	 IN:
//		- Window *window	-> pointer to created GUI
//----------------------------------------------------------------------------
Interpreter::Interpreter(Window *window)
{
	this->window = window;

	_iTestMode = -1;
	_iParity   = -1;
	_iStopBits = -1;
	_iTransfer = -1;
	_iProtocol = -1;
	_dwBaudrate = 0;

	_sPort = "";
	_sTransferFile = "";
	
}


//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
Interpreter::Interpreter(void)
{
}


//------------------------------------------------------------------------------
//Default deconstructor
//------------------------------------------------------------------------------
Interpreter::~Interpreter(void)
{
}

//int Interpreter::getTestMode()
//{
//	return window->getTestMode();
//}
//
//int Interpreter::getParity()
//{
//	return window->getParity();
//}
//
//
//bool Window::getLoggerState()
//{
//	return window->_bLoggerState;
//}

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
	
	//Always get test mode, port, logger and which information to transfer
	_iTestMode	  = window->getTestMode();
	_sPort		  = window->getSelectedPort();
	_bLoggerState = window->getLoggerState();

	_iInfoToTransfer = iInformationToTransfer;
	
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
				_iParity   = window->getParity();
				_iStopBits = window->getStopBits();
				_iTransfer = window->getTransfer();
				_iProtocol = window->getProtocol();
				_dwBaudrate = window->getPortBaudRate();
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
