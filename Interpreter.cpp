#include "Interpreter.h"


Interpreter::Interpreter(Window *window)
{
	this->window = window;

	_iTestMode = -1;
	_iParity   = -1;
	_iStopBits = -1;
	_iTransfer = -1;
	_iProtocol = -1;

	_sPort = "";
	_sTransferFile = "";


	baudRate[0] = L"1000";
	baudRate[1] = L"2000";
	baudRate[2] = L"3000";
	baudRate[3] = L"35000";
	baudRate[4] = L"100000";
	baudRate[5] = L"400000";

	
}

Interpreter::Interpreter(void)
{
}

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

//Parameter ünerprüfen!!!!!
void Interpreter::handleGui()
{
	//Always get test mode, port and logger
	_iTestMode	  = window->getTestMode();
	_sPort		  = window->getSelectedPort();
	_bLoggerState = window->getLoggerState();
	
	//when windows closes then is handle null
	//action executed with start button, change to
	//here when observer pattern inplemented
	//_sTransferFile = window->getTransferFile();



	if( _sPort != "")
	{
		switch (_iTestMode)
		{
			//Automatic test
			case 0:
				break;
			//Wobbeln
			case 1:
				break;
			//Fixed test
			case 2:
				_iParity   = window->getParity();
				_iStopBits = window->getStopBits();
				_iTransfer = window->getTransfer();
				_iProtocol = window->getProtocol();
				break;
				//TO DO
			//protocol
			default:
				MessageBoxW(NULL,L"Please select a Test Mode", L"Error", MB_OK);
				break;
			
			//TO DO
			//protocol
		}//switch
	}
	else
	{
		MessageBoxW(NULL,L"Please select the port to be tested", L"Error", MB_OK);
	}


}
