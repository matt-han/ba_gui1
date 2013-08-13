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
	_iBaudrate = DEFAULT_VALUE;

	_bLoggerState = true;

	_sMasPort = "";
	_sSlaPort = "";
	_sFilePath = "";
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
//	Saves the user baud rate input in _iBaudrate
//	Parameters:
//	 IN:
//		- int iBaudRate -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setPortBaudRate(int iBaudRate)
{
	this->_iBaudrate = iBaudRate;
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
//	Saves the user transfer file path input in _sFilePath
//	Parameters:
//	 IN:
//		- string sTransferFile -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setTransferFile(string sTransferFile)
{
	this->_sFilePath = sTransferFile;
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
//	Set the variables to the default values
//------------------------------------------------------------------------------
void Interpreter::setDefaultValues()
{
	_iTestMode  = DEFAULT_VALUE;
	_iParity    = DEFAULT_VALUE;
	_iStopBits  = DEFAULT_VALUE;
	_iTransfer  = DEFAULT_VALUE;
	_iProtocol  = DEFAULT_VALUE;
	_iBaudrate = DEFAULT_VALUE;

	_bLoggerState = true;

	_sMasPort      = "";
	_sSlaPort      = "";
	_sFilePath = "";
	_sTextToSend   = "";
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
	bool bErr = false;
	//iInformationToTransfer
	//0 for default
	//1 for file
	//2 for string
	//_iInfoToTransfer = iTransferText;

	if( _sMasPort != "")
	{
		testManager.testStruct.sMasterPort = _sMasPort;

		//if no test mode was selected -> error
		if (_iTestMode == DEFAULT_VALUE)
		{
			MessageBoxW(NULL,L"Please select a test mode",
				L"Error", MB_OK);
			setDefaultValues();
		}
		else
		{
			testManager.testStruct.iTestMode = _iTestMode;

			//if no transfer mode was selected -> error
			if (_iTransfer == DEFAULT_VALUE)
			{
				MessageBoxW(NULL,L"Please select a transfer mode",
					L"Error", MB_OK);
				setDefaultValues();
			}
			else
			{
				//save the transfer mode
				testManager.testStruct.iTransfer = _iTransfer;

				switch (_iTestMode)
				{
					//Automatic test
					//needs transfer mode, master/slave port and logger settings
					//----------------------------------------------------------
					case 0:

						//if needed get the slave port
						if(_iTransfer == 1 || _iTransfer == 2)
						{
							//if no slave port was selected -> error
							if (_sSlaPort == "")
							{
								MessageBoxW(NULL,L"Please select a slave port",
									L"Error", MB_OK);
								setDefaultValues();
								break;
							}
							else
								testManager.testStruct.sSlavePort = _sSlaPort;
						}
						else if (_iTransfer == 0)
						{
							break;
						}
						else
						{
							MessageBoxW(NULL,L"Please select a transfer mode",
										L"Error", MB_OK);
							setDefaultValues();
							break;
						}
						break;
					//----------------------------------------------------------


					//Wobbeln
					//needs, master/slave port and logger settings
					//----------------------------------------------------------
					case 1:
						//if needed get the slave port
						if(_iTransfer == 1 || _iTransfer == 2)
						{
							//if no slave port was selected -> error
							if (_sSlaPort == "")
							{
								MessageBoxW(NULL,L"Please select a slave port",
									L"Error", MB_OK);
								setDefaultValues();
								break;
							}
							else
								testManager.testStruct.sSlavePort = _sSlaPort;

							//check parity, protocol and stopbits
							if(ERROR_INPUT == checkInputConfigData())
							{
								setDefaultValues();
								break;
							}
						}
						else if (_iTransfer == 0)
						{
							//check parity, protocol and stopbits
							if(ERROR_INPUT == checkInputConfigData())
							{
								setDefaultValues();
								break;
							}
						}
						else
						{
							MessageBoxW(NULL,L"Please select a transfer mode",
										L"Error", MB_OK);
							setDefaultValues();
							break;
						}

					
						break;
					//----------------------------------------------------------


					//Fixed test
					//get all GUI settings
					//----------------------------------------------------------
					case 2:
						//wenn double or master slave
						//if needed get the slave port
						if(_iTransfer == 1 || _iTransfer == 2)
						{
							//if no slave port was selected -> error
							if (_sSlaPort == "")
							{
								MessageBoxW(NULL,L"Please select a slave port",
									L"Error", MB_OK);
								setDefaultValues();
								bErr = true;
								break;
							}
							else
								testManager.testStruct.sSlavePort = _sSlaPort;
						}
					
						if (_iTransfer >= 0 && _iTransfer <= 2)
						{
							//check parity, protocol and stopbits
							if(ERROR_INPUT == checkInputConfigData())
							{
								setDefaultValues();
								bErr = true;
								break;
							}
				
							//check baudrate
							if (_iBaudrate == DEFAULT_VALUE)
							{
								MessageBoxW(NULL,L"Please select a baud rate",
									L"Error", MB_OK);
								setDefaultValues();
								bErr = true;
								break;
							}
							else
								testManager.testStruct.iBaud = _iBaudrate;
						}
						else
						{
							MessageBoxW(NULL,L"Please select a transfer mode",
										L"Error", MB_OK);
							setDefaultValues();
							bErr = true;
							break;
						}
						break;
					//----------------------------------------------------------
			

					//Port was selected but no test mode
					//----------------------------------------------------------
					default:
						MessageBoxW(NULL,L"Please select a Test Mode",
							L"Error", MB_OK);
						setDefaultValues();
						bErr = true;
						break;
					//----------------------------------------------------------

				}//switch

				//save the logger state, file path and textToSend
				testManager.testStruct.bLoggerState = _bLoggerState;
				testManager.testStruct.sFilePath = _sFilePath;
				testManager.testStruct.sTextToTransfer = _sTextToSend;

				//--------------------------------------------------------------
				//--------------------------------------------------------------
				//start the manager to start testing
				//checkj structure to avoid asking for bErr
				if(bErr == false)
				testManager.startManager();
				//--------------------------------------------------------------
				//--------------------------------------------------------------

			}//if transfer mode
		}
	}
	else
	{
		MessageBoxW(NULL,L"Please select the port to be tested",
					L"Error", MB_OK);
		//Delete other values to avoid errors in new copy process
		setDefaultValues();
	}


}

//------------------------------------------------------------------------------
//	Checks the parity, protocol and stopbits in the GUI to save them in the
//	test manager
//------------------------------------------------------------------------------
long Interpreter::checkInputConfigData()
{
	if(_iParity == DEFAULT_VALUE)
	{
		MessageBoxW(NULL,L"Please select the parity",
			L"Error", MB_OK);
		return ERROR_INPUT;
	}
	else
		testManager.testStruct.iParity = _iParity;


	if(_iProtocol == DEFAULT_VALUE)
	{
		MessageBoxW(NULL,L"Please select the protocol",
			L"Error", MB_OK);
		return ERROR_INPUT;
	}
	else
		testManager.testStruct.iProtocol = _iProtocol;

	if(_iStopBits == DEFAULT_VALUE)
	{
		MessageBoxW(NULL,L"Please select the stopbits",
			L"Error", MB_OK);
		return ERROR_INPUT;
	}
	else
		testManager.testStruct.iStopbits = _iStopBits;

	return ERROR_SUCCESS;
}