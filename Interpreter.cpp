#include "Interpreter.h"


//void Interpreter::startT1()
//{
//	_t1 = thread (&Interpreter::handleGui, this);
//}

//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
Interpreter::Interpreter(void)
{
	setDefaultValues();
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
//	Saves the user stopbits input in _iStopBits
//	Parameters:
//	 IN:
//		- int iStopBits -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setDataBits(int iDataBits)
{
	this->_iDataBits = iDataBits;
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
//	Saves if the user wishes to stop on the first Transmission error
//	Parameters:
//	 IN:
//		- bool _bStopOnError -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setStopOnError(bool bStopOnError)
{
	this->_bStopOnError = bStopOnError;
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
	//this->_sTextToSend = tools.replaceASCII(sTextToSend);

}


//------------------------------------------------------------------------------
//	Saves the user transfer text mode input in _iTransTextMode
//	Parameters:
//	 IN:
//		- int iTransTextMode -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setTransTextMode(int iTransTextMode)
{
	this->_iTransTextMode = iTransTextMode;
}


//------------------------------------------------------------------------------
//	Saves the user input for how many times a test wil be repeated in _iRepeater
//	Parameters:
//	 IN:
//		- string sRepeater -> user GUI input
//------------------------------------------------------------------------------
void Interpreter::setRepeater(string sRepeater)
{
	if (sRepeater != "0")
	{
		int iTemp = strtol(sRepeater.c_str(), NULL, 10);
		if(iTemp != 0)
			this->_iRepeater = iTemp;
		else
		{
			MessageBoxA(NULL, "Error in repeater value,\n"
							  "please type an number > 0",
							  WINDOW_TITLE, MB_OK | MB_ICONWARNING);
			this->_iRepeater = DEFAULT_VALUE;
		}
	}
	else
	{
		this->_iRepeater = 0;
	}
}


//------------------------------------------------------------------------------
//	Saves the vector containing the available baud rates
//	Parameters:
//	 IN:
//		- vector<string> svBaud -> baud rates as string in vector
//------------------------------------------------------------------------------
void Interpreter::setBaudVector(vector<string> svBaud)
{
	this->_svBaudrates = svBaud;
}


//------------------------------------------------------------------------------
//	Checks the parity, protocol and stopbits in the GUI to save them in the
//	test manager
//------------------------------------------------------------------------------
int Interpreter::checkInputConfigData()
{

	//parity
	if(_iParity == DEFAULT_VALUE)
	{
		MessageBoxA(NULL,"Please select the parity",
			WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return ERROR_INPUT;
	}
	else
		_testManager->testStruct.iParity = _iParity;

	//protocol
	if(_iProtocol == DEFAULT_VALUE)
	{
		MessageBoxA(NULL,"Please select the protocol",
			WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return ERROR_INPUT;
	}
	else
		_testManager->testStruct.iProtocol = _iProtocol;

	//stopbits
	if(_iStopBits == DEFAULT_VALUE)
	{
		MessageBoxA(NULL,"Please select the stopbits",
			WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return ERROR_INPUT;
	}
	else
		_testManager->testStruct.iStopbits = _iStopBits;

	//stopbits
	if(_iDataBits == DEFAULT_VALUE)
	{
		MessageBoxA(NULL,"Please select the databits",
			WINDOW_TITLE, MB_OK | MB_ICONERROR);
		return ERROR_INPUT;
	}
	else
		_testManager->testStruct.iDatabits = _iDataBits;


	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Checks the given baud rate
//	Parameters:
//	 IN:
//		- int iBaudrate -> the baud rate
//Return: the baud rate or error
//------------------------------------------------------------------------------
int Interpreter::checkBaudrate(int iBaudrate)
{
	if (iBaudrate == DEFAULT_VALUE)
	{
		MessageBoxA(NULL,"Please select a baud rate!!!!!!",
			WINDOW_TITLE, MB_OK | MB_ICONERROR);
		setDefaultValues();
		return DEFAULT_VALUE;

	}
	else if(iBaudrate == ERROR_BAUDRATE)
	{
		MessageBoxA(NULL,"Baud rate is not suported by"
			" this port\n"
			"Please choose another baud rate",
			WINDOW_TITLE, MB_OK | MB_ICONERROR);
		setDefaultValues();
		return ERROR_BAUDRATE;

	}
	else
		return iBaudrate;
}


//------------------------------------------------------------------------------
//	Set the variables to the default values
//------------------------------------------------------------------------------
void Interpreter::setDefaultValues()
{
	_iTestMode		= DEFAULT_VALUE;
	_iParity		= DEFAULT_VALUE;
	_iStopBits		= DEFAULT_VALUE;
	_iDataBits		= DEFAULT_VALUE;
	_iTransfer		= DEFAULT_VALUE;
	_iProtocol		= DEFAULT_VALUE;
	_iBaudrate		= DEFAULT_VALUE;
	_iBaudrateMax	= DEFAULT_VALUE;
	_iRepeater		= DEFAULT_VALUE;
	_iTransTextMode = DEFAULT_VALUE;

	
	
	_bLoggerState	= true;
	_bStopOnError   = false;

	_sMasPort		= "";
	_sSlaPort		= "";
	_sFilePath		= "";
	_sTextToSend	= "";

	_svBaudrates.clear();

	_testManager = NULL;
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

	_testManager = new TestManager();


	if( _sMasPort != "")
	{
		_testManager->testStruct.sMasterPort = _sMasPort;

		//if no test mode was selected -> error
		if (_iTestMode == DEFAULT_VALUE)
		{
			MessageBoxA(NULL,"Please select a test mode",
				WINDOW_TITLE, MB_OK | MB_ICONERROR);
			setDefaultValues();
		}
		else
		{
			_testManager->testStruct.iTestMode = _iTestMode;

			//if no transfer mode was selected -> error
			if (_iTransfer == DEFAULT_VALUE)
			{
				MessageBoxA(NULL,"Please select a transfer mode",
					WINDOW_TITLE, MB_OK | MB_ICONERROR);
				setDefaultValues();
			}
			else
			{
				//save the transfer mode
				_testManager->testStruct.iTransfer = _iTransfer;

				switch (_iTestMode)
				{
					//Automatic test
					//needs transfer mode, master/slave port and logger settings
					//----------------------------------------------------------
					case 0:
						if (_iTransfer >= 0 && _iTransfer <= 3)
						{
							//if needed get the slave port
							if(_iTransfer == 1)
							{
								//if no slave port was selected -> error
								if (_sSlaPort == "")
								{
									MessageBoxA(NULL,"Please select a slave port",
										WINDOW_TITLE, MB_OK | MB_ICONERROR);
									setDefaultValues();
									break;
								}
								else
									_testManager->testStruct.sSlavePort = _sSlaPort;
							}
						}
						else
						{
							MessageBoxA(NULL, "Please select a transfer mode",
										WINDOW_TITLE, MB_OK | MB_ICONERROR);
							setDefaultValues();
							break;
						}
						break;
					// end case 0
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
									MessageBoxA(NULL,"Please select a slave port",
										WINDOW_TITLE, MB_OK | MB_ICONERROR);
									setDefaultValues();
									bErr = true;
									break;
								}
								else
									_testManager->testStruct.sSlavePort = _sSlaPort;
							}
							
							if(_iTransfer != 3) //if not slave mode
							{
								//check parity, protocol and stopbits
								if(ERROR_INPUT == checkInputConfigData())
								{
									setDefaultValues();
									bErr = true;
									break;
								}
							}
						}
						else
						{
							MessageBoxA(NULL,"Please select a transfer mode",
										WINDOW_TITLE, MB_OK | MB_ICONERROR);
							setDefaultValues();
							break;
						}

						//get the baud rates
						//MIN
						_iTemp = checkBaudrate(_iBaudrate);
						if (_iTemp != DEFAULT_VALUE && _iTemp != ERROR_BAUDRATE)
							_testManager->testStruct.iBaud = _iBaudrate;
						else
						{
							bErr = true;
							break;
						}
						//MAX
						_iTemp = checkBaudrate(_iBaudrateMax);
						if (_iTemp != DEFAULT_VALUE && _iTemp != ERROR_BAUDRATE)
							_testManager->testStruct.iBaudrateMax = _iBaudrateMax;
						else
						{
							bErr = true;
							break;
						}

						//_testManager->testStruct.svBaudrates = this->comEnumerator.vBaud;
						_testManager->testStruct.svBaudrates = this->_svBaudrates;
						
						break;
					//end case 1
					//----------------------------------------------------------


					//Fixed test
					//get all GUI settings
					//----------------------------------------------------------
					case 2:
						
						if (_iTransfer >= 0 && _iTransfer <= 3)
						{
						
							//if needed get the slave port
							if(_iTransfer == 1)
							{
								//if no slave port was selected -> error
								if (_sSlaPort == "")
								{
									MessageBoxA(NULL,"Please select a slave port",
										WINDOW_TITLE, MB_OK | MB_ICONERROR);
									setDefaultValues();
									bErr = true;
									break;
								}
								else
									_testManager->testStruct.sSlavePort = _sSlaPort;
							}
							
							if(_iTransfer != 3) //if not slave mode
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
									_testManager->testStruct.iBaud = _iBaudrate;
								else
								{
									bErr = true;
									break;
								}
							}
						}
						else
						{
							MessageBoxA(NULL,"Please select a transfer mode",
										WINDOW_TITLE, MB_OK | MB_ICONERROR);
							setDefaultValues();
							bErr = true;
							break;
						}
						break;
					//end case 2
					//----------------------------------------------------------
			

					//Port was selected but no test mode
					//----------------------------------------------------------
					default:
						MessageBoxA(NULL,"Please select a Test Mode",
							WINDOW_TITLE, MB_OK | MB_ICONERROR);
						setDefaultValues();
						bErr = true;
						break;
					//----------------------------------------------------------

				}//switch

				
				//start the testManager for testing if no input errors were reported
				if(bErr == false)
				{
					//save the repeater, logger state and stop on first error
					_testManager->testStruct.bLoggerState	 = _bLoggerState;
					_testManager->testStruct.bStopOnError	 = _bStopOnError;
					_testManager->testStruct.iRepeater		 = _iRepeater;

					if(_iTransfer != 3) //if not slave mode
					{	
						//textToSend, file path, baud rates and transtextmode				
						_testManager->testStruct.sFilePath		 = _sFilePath;
						_testManager->testStruct.sTextToTransfer = _sTextToSend;
						_testManager->testStruct.iTransTextMode	 = _iTransTextMode;
						_testManager->testStruct.svBaudrates	 = _svBaudrates;
					}


					//**********************************************************
					//*	START THE TEST MANAGER
					//**********************************************************
					_iError = _testManager->startManager();
					if(_iError == ERROR_SUCCESS)
					{
						MessageBoxA(NULL, "Transmission finished", WINDOW_TITLE,
									MB_OK);
					}
					else
					{
						_sTemp = "Send and/or receive failure\n";
						_sTemp.append(tools.errorCodeParser(_iError));

						MessageBoxA(NULL, _sTemp.c_str(),
							WINDOW_TITLE,
							MB_OK | MB_ICONWARNING);
					}
				}
				else //bErr
				{
					delete _testManager;
					_testManager = NULL;
				}

			}//if transfer mode
		}
	}
	else
	{
		MessageBoxA(NULL,"Please select the port to be tested",
					WINDOW_TITLE, MB_OK | MB_ICONERROR);
		//Delete other values to avoid errors in new copy process
		setDefaultValues();
	}

	delete _testManager;
	_testManager = NULL;
}


//------------------------------------------------------------------------------
//	saves the settings to a ini file
//	Parameters:
//Return: if error or fail
//------------------------------------------------------------------------------
int Interpreter::saveToFile(string sSavePath)
{
	int iError = ERROR_SUCCESS;
	string sTempTransferTextMode, sRepeater;

	switch(_iTransTextMode)
	{
		//1 for file
		case ID_BT_LOAD:
			sTempTransferTextMode = _sFilePath;
			break;
			
		//2 for string
		case ID_BT_TEXT:
			sTempTransferTextMode = _sTextToSend;
			break;

		default:
			sTempTransferTextMode = "";
			break;
	}

	sRepeater = tools.convertToString(_iRepeater);

	if(_sMasPort == "")
	{
		MessageBoxA(NULL, "Please choose a port to test", WINDOW_TITLE, MB_OK | MB_ICONERROR);
		iError = ERROR_MISSING_PAR;
	}
	else
	{
		if (_iTransfer != DEFAULT_VALUE)
		{
			if(_iTransfer == 1 && _sSlaPort == "")//double transmiton
			{
				MessageBoxA(NULL, "Please choose a slave port", WINDOW_TITLE, MB_OK | MB_ICONERROR);
				iError = ERROR_MISSING_PAR;
			}
			
			if(_iTestMode != DEFAULT_VALUE)
			{
				if (_iTestMode != 0)
				{
					if(_iBaudrate == DEFAULT_VALUE)
					{
						MessageBoxA(NULL, "Please choose a baud rate", WINDOW_TITLE, MB_OK | MB_ICONERROR);
						iError = ERROR_MISSING_PAR;
					}
				
					if(_iTestMode == 1)//wobble
					{
						if(_iBaudrateMax == DEFAULT_VALUE)
						{
							MessageBoxA(NULL, "Please choose a MAX baud rate", WINDOW_TITLE, MB_OK | MB_ICONERROR);
							iError = ERROR_MISSING_PAR;
						}
					}
				}//testemode != 0
			}
			else
			{
				MessageBoxA(NULL, "Please choose a test mode", WINDOW_TITLE, MB_OK | MB_ICONERROR);
				iError = ERROR_MISSING_PAR;
			}
		}//if transfer
		else
		{
			MessageBoxA(NULL, "Please choose a transfer mode", WINDOW_TITLE, MB_OK | MB_ICONERROR);
			iError = ERROR_MISSING_PAR;
		}
	}//else master port


	if (iError == ERROR_SUCCESS)
	{
		iniFile.writeINIfile(_sMasPort,
							 _sSlaPort,
							 _iBaudrate,
							 _iBaudrateMax,
							 _iTestMode,
							 _iParity,
							 _iProtocol,
							 _iStopBits,
							 _iDataBits,
							 _iTransfer,
							 _iTransTextMode,
							 sTempTransferTextMode,
							 sRepeater,
							 _bLoggerState,
							 _bStopOnError,
							 sSavePath);
	}

	return iError;
}


//------------------------------------------------------------------------------
//	Loads the given ini file settings for the port given port
//	Parameters:
//	 IN:
//		- string sPath -> file path
//		- string sPort -> com port
//Return success or test error
//------------------------------------------------------------------------------
int Interpreter::loadIniFile(string sPath, string sPort)
{
	_testManager = new TestManager();
	
	_bErr = false;
	_iError = iniFile.readINIFile(sPath, sPort);

	if(_iError == ERROR_SUCCESS)
	{
		_vIniFilePorts = iniFile.getTestStructure();

		for( int index = 0; index < _vIniFilePorts.size(); index++)
		{
			_testManager->testStruct = _vIniFilePorts.at(index);

			_testManager->testStruct.svBaudrates =
				comEnumerator.returnBaudrates(_vIniFilePorts.at(index).sMasterPort);
			
			if(_testManager->testStruct.svBaudrates.empty())
			{

				delete _testManager;
				_testManager = NULL;
				return ERROR_BAUDRATE;
			}

			_iError = _testManager->startManager();
			//_testManager->startManager();
			if(_iError != ERROR_SUCCESS)
			{
				_bErr = true;//handle errors
				if(_bLoggerState)
					clog << "Error testing port "
						 << _vIniFilePorts.at(index).sMasterPort
						 << ". Error " << _iError
						 << endl;
				else
				{
					string sError = "Error testing port ";
					sError.append(_vIniFilePorts.at(index).sMasterPort);
					sError.append(".\n");
					sError.append(tools.errorCodeParser(_iError));
					MessageBoxA(NULL, sError.c_str(), WINDOW_TITLE, MB_OK | MB_ICONERROR);
				}
			}

		}//for

		//if(_bErr)
		//{
		//	_vIniFilePorts.clear();
		//	delete _testManager;
		//	_testManager = NULL;
		//	return ERROR_TEST;
		//}
	}
	else
	{
		string sError = "Error reading test file:\n";
		sError.append(sPath);
		MessageBoxA(NULL, sError.c_str(),
					WINDOW_TITLE, MB_OK | MB_ICONERROR);
		delete _testManager;
		_testManager = NULL;
		return _iError;
	}

	_vIniFilePorts.clear();
	delete _testManager;
	_testManager = NULL;
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Sets the stop test flag from the main thread
//------------------------------------------------------------------------------
void Interpreter::stopTest()
{
	if(_testManager != NULL)
	{
		clog << "STOP BUTTON PRESSED" << endl;
		_testManager->bStopButton = true;
	}
	else
		clog << "NO TESTMANAGER AVAILABLE" << endl;
}
