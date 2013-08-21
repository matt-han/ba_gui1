#include "Interpreter.h"


//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
Interpreter::Interpreter(void)
{
	_iTestMode		= DEFAULT_VALUE;
	_iParity		= DEFAULT_VALUE;
	//_iParityMax		= DEFAULT_VALUE;
	_iStopBits		= DEFAULT_VALUE;
	_iTransfer		= DEFAULT_VALUE;
	_iProtocol		= DEFAULT_VALUE;
	_iBaudrate		= DEFAULT_VALUE;
	_iBaudrateMax	= DEFAULT_VALUE;

	_iTransTextMode = DEFAULT_VALUE;

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
//	Saves the user max baud rate input in _iBaudrate
//	Parameters:
//	 IN:
//		- int iBaudRate -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setPortBaudRateMax(int iBaudrateMax)
{
	this->_iBaudrateMax = iBaudrateMax;
	//this->_svBaudrates = this->comEnumerator.vBaud;
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



void Interpreter::setTransTextMode(int iTransTextMode)
{
	this->_iTransTextMode = iTransTextMode;
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
	_iBaudrate  = DEFAULT_VALUE;
	_iTransTextMode = DEFAULT_VALUE;
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
						if(_iTransfer == 1)
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
					//needs, master/slave port, max baud and logger settings
					//----------------------------------------------------------
					case 1:
						//if needed get the slave port
						if (_iTransfer >= 0 && _iTransfer <= 3)
						{
							//if transfer mode = double then get the slave
							if(_iTransfer == 1)
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

							//check parity, protocol and stopbits
							if(ERROR_INPUT == checkInputConfigData())
							{
								setDefaultValues();
								bErr = true;
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

						//get the baud rates
						//MIN
						_iTemp = checkBaudrate(_iBaudrate);
						if (_iTemp != DEFAULT_VALUE || _iTemp != ERROR_BAUDRATE)
							testManager.testStruct.iBaud = _iBaudrate;
						else
						{
							bErr = true;
							break;
						}
						//MAX
						_iTemp = checkBaudrate(_iBaudrateMax);
						if (_iTemp != DEFAULT_VALUE || _iTemp != ERROR_BAUDRATE)
							testManager.testStruct.iBaudrateMax = _iBaudrateMax;
						else
						{
							bErr = true;
							break;
						}

						testManager.testStruct.svBaudrates = this->comEnumerator.vBaud;
						
						break;
					//----------------------------------------------------------


					//Fixed test
					//get all GUI settings
					//----------------------------------------------------------
					case 2:
						//wenn double or master slave
						//if needed get the slave port
						if(_iTransfer == 1)
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
					
						if (_iTransfer >= 0 && _iTransfer <= 3)
						{
							//check parity, protocol and stopbits
							if(ERROR_INPUT == checkInputConfigData())
							{
								setDefaultValues();
								bErr = true;
								break;
							}
							
							//check baudrate
							_iTemp = checkBaudrate(_iBaudrate);
							if (_iTemp != DEFAULT_VALUE || _iTemp != ERROR_BAUDRATE)
								testManager.testStruct.iBaud = _iBaudrate;
							else
							{
								bErr = true;
								break;
							}
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

				//--------------------------------------------------------------
				//--------------------------------------------------------------
				//start the manager to start testing
				//check structure to avoid asking for bErr
				if(bErr == false)
				{
					//save the logger state, file path, textToSend and transtextmode
					testManager.testStruct.bLoggerState = _bLoggerState;
					testManager.testStruct.sFilePath = _sFilePath;
					testManager.testStruct.sTextToTransfer = _sTextToSend;
					testManager.testStruct.iTransTextMode = _iTransTextMode;


					_iError = testManager.startManager();
					if(_iError == ERROR_SUCCESS)
					{
						MessageBoxA(NULL, "Send and Recieve OK", "SUCCESS!!",
									MB_OK);
					}
					else
					{
						MessageBoxA(NULL, "Send and Recieve failure", "FAIL!!",
									MB_OK);
					}
				}
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
int Interpreter::checkInputConfigData()
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


int Interpreter::checkBaudrate(int iBaudrate)
{
	if (iBaudrate == DEFAULT_VALUE)
	{
		MessageBoxW(NULL,L"Please select a baud rate",
			L"Error", MB_OK);
		setDefaultValues();
		return DEFAULT_VALUE;

	}
	else if(iBaudrate == ERROR_BAUDRATE)
	{
		MessageBoxW(NULL,L"Baud rate is not suported by"
			L" this port (WinBase define error)\n"
			L"Please choose another baud rate",
			L"Error", MB_OK);
		setDefaultValues();
		return ERROR_BAUDRATE;

	}
	else
		return iBaudrate;
}


int Interpreter::saveToFile()
{

	string sTemp;

	switch(_iTransTextMode)
	{
		//1 for file
		case ID_BT_LOAD:
			sTemp = _sFilePath;
			break;
			
		//2 for string
		case ID_BT_TEXT:
			sTemp = _sTextToSend;
			break;

		default:
			sTemp = "";
			break;
	}



	iniFile.writeINIfile(_sMasPort,
						 _sSlaPort,
						 _iBaudrate,
						 _iBaudrateMax,
						 _iTestMode,
						 _iParity,
						 _iProtocol,
						 _iStopBits,
						 _iTransfer,
						 _iTransTextMode,
						 sTemp,
						 "");

	return ERROR_SUCCESS;
}


int Interpreter::loadIniFile(string sPath)
{
	_bErr = false;
	_iError = iniFile.readINIFile(sPath);

	if(_iError == ERROR_SUCCESS)
	{
		_vIniFilePorts = iniFile.getTestStructure();

		for( int index = 0; index < _vIniFilePorts.size(); index++)
		{
			testManager.testStruct = _vIniFilePorts.at(index);

			testManager.testStruct.svBaudrates =
				comEnumerator.returnBaudrates(_vIniFilePorts.at(index).sMasterPort);
			
			if(testManager.testStruct.svBaudrates.empty())
			{
				return ERROR_BAUDRATE;
			}
			
			
			_iError = testManager.startManager();

			if(_iError != ERROR_SUCCESS)
			{
				_bErr = true;//handle errors
				clog << "Error testing port "
					 << _vIniFilePorts.at(index).sMasterPort
					 << "Please read "
					 << endl;
			}
		}
		if(_bErr)
		{
			_vIniFilePorts.clear();
			return ERROR_TEST;
		}


	}
	else
	{
		clog << "error reading test file: "
			 << sPath
			 << endl;
		return _iError;
	}

	_vIniFilePorts.clear();
	return ERROR_SUCCESS;
}