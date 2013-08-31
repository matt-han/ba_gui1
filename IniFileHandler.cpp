#include "IniFileHandler.h"

//------------------------------------------------------------------------------
//	Constructor
//------------------------------------------------------------------------------
IniFileHandler::IniFileHandler(void)
{
	_iError = 0;
}


//------------------------------------------------------------------------------
//	Deconstructor
//------------------------------------------------------------------------------
IniFileHandler::~IniFileHandler(void)
{
}

//##############################################################################
//	WRITE METHODS
//##############################################################################
//------------------------------------------------------------------------------
//	Writes the configuration the user picked in the GUI into a ini file to be
//	used again for testing
//	Parameters:
//	 IN:
//		- string sMasterPort -> Master COM port for test
//		- string sSlavePort  -> Slave COM port for test
//		- int iBaud -> Baud rate for test
//		- int iTestMode -> testing mode
//		- int iParity -> COM port(s) baud rate
//		- int iProtocol -> transmition protocol for test
//		- int iStopbits -> stopbits to be set
//		- int iTransfer -> single/double/master/slave transmition
//		- int iTextMode	-> text mode to transfer
//		- string sTextToTransfer -> default text, input text or input file
//		- bool bLoggerStatur -> true for logfile, false for no logging
//		- string sRepeater	-> hoy many times each test is repeated
//		- string sPath -> path where to save the ini test file
//------------------------------------------------------------------------------
void IniFileHandler::writeINIfile(string sMasterPort, string sSlavePort, int iBaud,
								int iBaudMax, int iTestMode, int iParity, int iProtocol,
								int iStopbits, int iTransfer,int iTextMode,
								string sTextToTransfer, string sRepeater, bool bLogger, string sPath)
{
	string sBaud, sBaudMax, sParity, sProtocol, sStopbits, sLogger;
	
	//if path is empty then create default path
	if (sPath == "")
	{
		sPath = getenv("TEMP");
		sPath.append("\\WN_ComPortTestFile_");
		sPath.append(sMasterPort);
		sPath.append(".ini");
	}

	sFilePath = sPath;

	//copy ints into strings
	if (iTestMode != 0)
	{
		if (iTestMode != 0)
		{	
			sBaud = tools.convertToString(iBaud);
			
			if(iTestMode == 1)
				sBaudMax = tools.convertToString(iBaudMax);
		}
		
		sParity	  = parseParityToIni(iParity);
		sProtocol = parseProtocolToIni(iProtocol);
		sStopbits = parseStopbitsToIni(iStopbits);
		
		if(bLogger)
			sLogger = "true";
		else
			sLogger = "false";
	}


	switch(iTestMode)
	{
		//automatic
		case 0:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TestMode",
										"automatic", sPath.c_str());
			//Transfer mode + slave port if needed
			writeINItransferSettings(sMasterPort, sSlavePort, iTransfer, sPath);
			//Ignore baudrate, parity, protocol and stopbits
			break;


		//wobble
		case 1:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TestMode",
										"wobble", sPath.c_str());
			//Transfer mode + slave port if needed
			writeINItransferSettings(sMasterPort, sSlavePort, iTransfer, sPath);
			


			//baudrate
			sBaud.append("-");
			sBaud.append(sBaudMax);
			WritePrivateProfileStringA(sMasterPort.c_str(),"BaudRate",
										sBaud.c_str(), sPath.c_str());

			//Parity
			WritePrivateProfileStringA(sMasterPort.c_str(),"Parity",
										sParity.c_str(), sPath.c_str());
			//Protocol
			WritePrivateProfileStringA(sMasterPort.c_str(),"Protocol",
										sProtocol.c_str(), sPath.c_str());
			//Stopbits
			WritePrivateProfileStringA(sMasterPort.c_str(),"Stopbits",
										sStopbits.c_str(), sPath.c_str());
			break;


		//fixed
		case 2:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TestMode",
										"fixed", sPath.c_str());
			//Transfer mode + slave port if needed
			writeINItransferSettings(sMasterPort, sSlavePort, iTransfer, sPath);
			
			//Baud rate
			WritePrivateProfileStringA(sMasterPort.c_str(),"BaudRate",
										sBaud.c_str(), sPath.c_str());
			//Parity
			WritePrivateProfileStringA(sMasterPort.c_str(),"Parity",
										sParity.c_str(), sPath.c_str());
			//Protocol
			WritePrivateProfileStringA(sMasterPort.c_str(),"Protocol",
										sProtocol.c_str(), sPath.c_str());
			//Stopbits
			WritePrivateProfileStringA(sMasterPort.c_str(),"Stopbits",
										sStopbits.c_str(), sPath.c_str());
			break;
	}//switch


	switch(iTextMode)
	{
		//1 for file
		case ID_BT_LOAD:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TransferTextMode",
								"file", sPath.c_str());
			break;
			
		//2 for string
		case ID_BT_TEXT:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TransferTextMode",
								"text", sPath.c_str());
			break;

		default:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TransferTextMode",
								"default", sPath.c_str());
			break;
	}

	//Transfer text
	WritePrivateProfileStringA(sMasterPort.c_str(),"TransferText",
								sTextToTransfer.c_str(), sPath.c_str());

	//Logger
	WritePrivateProfileStringA(sMasterPort.c_str(),"Logger",
										sLogger.c_str(), sPath.c_str());

	//Repeater
	WritePrivateProfileStringA(sMasterPort.c_str(),"Repeater",
										sRepeater.c_str(), sPath.c_str());
}


//------------------------------------------------------------------------------
//	Writes the transfer settings in the ini test file
//	Parameters:
//	 IN:
//		- string sMasterPort -> Master COM port for test
//		- string sSlavePort  -> Slave COM port for test
//		- int iTransfer -> single/double/master-slave transmition
//		- string sPath -> path where to save the ini test file
//	Return: _iError code signaling if operation succeded or _iError
//------------------------------------------------------------------------------
void IniFileHandler::writeINItransferSettings(string sMasterPort,
										   string sSlavePort,
										   int iTransfer,
										   string sPath)
{
	switch(iTransfer)
	{
		//single
		case 0:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TransferMode",
										"single", sPath.c_str());
			break;
	
		//double
		case 1:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TransferMode",
										"double", sPath.c_str());
			WritePrivateProfileStringA(sMasterPort.c_str(),"SlavePort",
										sSlavePort.c_str(), sPath.c_str());
			break;

	
		//master
		case 2:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TransferMode",
										"master", sPath.c_str());
			break;

		//slave
		case 3:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TransferMode",
										"slave", sPath.c_str());
			break;

	}
}

//##############################################################################
//	READ METHODS
//##############################################################################
//	Read configuration file for automated testing
//	Parameters:
//	 IN:
//		- string sFilePath -> INI file path
//	Return: _iError code signaling if operation succeded or _iError
//------------------------------------------------------------------------------
int IniFileHandler::readINIFile(string sFilePath, string sMainPort)
{
	string sPort;
	string sTemp;
	int iPort = 0;


	//if a port is given, then this is the port to test
	if( sMainPort != "")
	{
		vComPorts.push_back(TestStruct());
		vComPorts.at(iPort).sMasterPort = sMainPort;

		_iError = readPortConfig("COM", sFilePath, 0);
		if (_iError != ERROR_SUCCESS)
		{
			clog << "Error reading port from INI file."
				 << "Error " << _iError << endl;
		}

		return _iError;
	}


	//try to find test settings for all possible system ports
	for(int iCom = 1; iCom < 256; iCom++)//256 ports
	{
		sPort = "COM";
		sTemp = tools.convertToString(iCom);
		sPort.append(sTemp);

		_dwExists = GetPrivateProfileStringA(sPort.c_str(), "TestMode", NULL,
											szValue, sizeof(szValue),
											sFilePath.c_str());

		if (0 != _dwExists)
		{
			vComPorts.push_back(TestStruct());
			vComPorts.at(iPort).sMasterPort = sPort;

			_iError = readPortConfig(sPort, sFilePath, iPort);
			if (_iError != ERROR_SUCCESS)
			{
				clog << "Error reading port " << sPort << " from INI file."
					 << "Error " << _iError << endl;
			}
			iPort++;
		}
		_iError = 0;
	}
	//if any read the 
	return ERROR_SUCCESS;
	//else Errooooor
}


//------------------------------------------------------------------------------
//	Reads port configuration from opened configuration file. Saves the
//	information in the corresponding vectors
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readPortConfig(string sPort, string sFilePath, int index)
{
	string sTemp = "";
	int iTemp = parseTestMode(szValue);
	vComPorts.at(index).iTestMode = iTemp;

	//always read transfer mode
	//all ports have a transfer mode
	_iError = readTransferMode(sPort, sFilePath, index);
	if (_iError == ERROR_SUCCESS)
	{
		switch(vComPorts.at(index).iTestMode)
		{
			//automatic
			//------------------------------------------------------------------
			case 0:	
				//if transfer mode equals double then read
				//the slave port
				if (vComPorts.at(index).iTransfer == 1)
				{
					_iError = readSlave(sPort, sFilePath.c_str(), index);
					if (_iError != ERROR_SUCCESS)
						return _iError;
				}
				//else do nothing
				break;
			//------------------------------------------------------------------
			

			//wobble
			//------------------------------------------------------------------
			case 1:
				//if transfer mode equals double then read
				//the slave port
				if (vComPorts.at(index).iTransfer == 1)
				{
					_iError = readSlave(sPort, sFilePath.c_str(), index);
					if (_iError != ERROR_SUCCESS)
						return _iError;
					
				}

				_iError = readSettings(sPort, sFilePath.c_str(), index);
				if (_iError != ERROR_SUCCESS)
					return _iError;
				else
				{
					//read the baud rate
					_iError = readBaudRate(sPort, sFilePath.c_str(), index);
						
					if (_iError != ERROR_SUCCESS)
						return _iError;
				}
	
				break;
			//------------------------------------------------------------------


			//fixed
			//------------------------------------------------------------------
			case 2:
				//if transfer mode equals double then read
				//the slave port
				if (vComPorts.at(index).iTransfer == 1)
				{
					_iError = readSlave(sPort, sFilePath.c_str(), index);
					
					if (_iError != ERROR_SUCCESS)
						return _iError;
				}

				//read the baud rate
				_iError = readBaudRate(sPort, sFilePath.c_str(), index);
					
				if (_iError != ERROR_SUCCESS)
					return _iError;
				else	//read the other settings
				{
					_iError = readSettings(sPort, sFilePath.c_str(), index);
						
					if (_iError != ERROR_SUCCESS)
						return _iError;
				}

				break;
			//------------------------------------------------------------------

		}//switch

		_iError = readTextToTransfer(sPort, sFilePath.c_str(), index);
		if (_iError != ERROR_SUCCESS)
				return _iError;

		//logger
		_iError = readLogger(sPort, sFilePath.c_str());
		if(_iError != ERROR_INI)
		{
			if(_iError == 0)
				vComPorts.at(index).bLoggerState = true;
			else
				vComPorts.at(index).bLoggerState = false;
		}
		else
			return _iError;

		//repeater
		_iError = readRepeater(sPort, sFilePath.c_str());
		if(_iError != ERROR_INI)
		{
			vComPorts.at(index).iRepeater = _iError;
		}
		else
			return _iError;

	}//if
	else
		return _iError;

	return ERROR_SUCCESS;
	
}

//------------------------------------------------------------------------------
//	Reads the transfer mode setting from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readTransferMode(string sPort, string sFilePath, int index)
{
	int iTemp = -1;
	_dwExists = GetPrivateProfileStringA(sPort.c_str(), "TransferMode",
											 NULL, szValue, sizeof(szValue),
											 sFilePath.c_str());
	if (_dwExists != 0)
	{
		iTemp = parseTransfer(szValue);
		if (iTemp != ERROR_PARSE)
		{
			vComPorts.at(index).iTransfer = iTemp;
		}//_iError_parse
		else
		{
			clog << "Error in INI file. Incorrect transfer mode for "
				 << "port " << sPort << endl;
			return ERROR_INI;
		}
	}
	else	//_iError in ini file
	{
		clog << "Error in INI file. No transfer mode definied for "
				<< sPort << " port" << endl;
		return ERROR_INI;
	}
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the slave port from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readSlave(string sPort, string sFilePath, int index)
{
	string sTemp = "";
	_dwExists = GetPrivateProfileStringA(sPort.c_str(),
												"SlavePort",
												 NULL, szValue, sizeof(szValue),
												 sFilePath.c_str());
	if (_dwExists != 0)
	{
		sTemp = parsePort(szValue);

		if(sTemp == "")
		{
			clog << "Error in INI file. Incorrect slave "
					<< "port for " << sPort << endl;
			return ERROR_INI;
		}
		else
			vComPorts.at(index).sSlavePort = szValue;
	}
	else
	{
		clog << "Error in INI file. No slave port given for"
				<< " port " << sPort << endl;
		return ERROR_INI;
	}
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the parity, protocol and stopbits settings from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readSettings(string sPort, string sFilePath, int index)
{
	_iError = readParity(sPort, sFilePath, index);
	if (_iError == ERROR_SUCCESS)
	{
		_iError = readProtocol(sPort, sFilePath, index);
		if (_iError == ERROR_SUCCESS)
		{
			_iError = readStopbits(sPort, sFilePath, index);
			if (_iError != ERROR_SUCCESS)
			{
				return _iError;
			}
		}
		else
			return _iError;
	}
	else
		return _iError;

	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the parity setting from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readParity(string sPort, string sFilePath, int index)
{
	int iTemp = -1;

	_dwExists = GetPrivateProfileStringA(sPort.c_str(), "Parity",
											 NULL, szValue, sizeof(szValue),
											 sFilePath.c_str());
	if (_dwExists != 0)
	{
		iTemp = parseParity(szValue, index);
		if (iTemp == ERROR_PARSE)
		{
			clog << "Error in INI file. Incorrect parity for "
				 << "port " << sPort << endl;
			return ERROR_INI;
		}
	}
	else	//_iError in ini file
	{
		clog << "Error in INI file. No parity definied for "
				<< sPort << " port" << endl;
		return ERROR_INI;
	}
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the protocol setting from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readProtocol(string sPort, string sFilePath, int index)
{
	int iTemp = -1;

	_dwExists = GetPrivateProfileStringA(sPort.c_str(), "Protocol",
										NULL, szValue, sizeof(szValue),
										sFilePath.c_str());
	if (_dwExists != 0)
	{
		iTemp = parseProtocol(szValue);
		if (iTemp != ERROR_PARSE)
		{
			vComPorts.at(index).iProtocol = iTemp;
		}//_iError_parse
		else
		{
			clog << "Error in INI file. Incorrect protocol for "
				 << "port " << sPort << endl;
			return ERROR_INI;
		}
	}
	else	//_iError in ini file
	{
		clog << "Error in INI file. No protocol definied for "
				<< sPort << " port" << endl;
		return ERROR_INI;
	}
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the stopbits setting from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readStopbits(string sPort, string sFilePath, int index)
{
	int iTemp = -1;

	_dwExists =GetPrivateProfileStringA(sPort.c_str(),"Stopbits",
												NULL, szValue,
												sizeof(szValue),
												sFilePath.c_str());
	if (_dwExists != 0)
	{
		iTemp = parseStopbits(szValue);
		if (iTemp != ERROR_PARSE)
		{
			vComPorts.at(index).iStopbits = iTemp;
		}
		else
		{
			clog << "Error in INI file. Incorrect stopbits for "
				 << "port " << sPort << endl;
			return ERROR_INI;
		}
	}
	else	//_iError in ini file
	{
		clog << "Error in INI file. No Stopbits definied for "
				<< sPort << endl;
		return ERROR_INI;
	}
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the baud rate setting from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readBaudRate(string sPort, string sFilePath, int index)
{	
	int iTemp = 0;
	//Get port baudrate
	//----------------------------------------------------------
	_dwExists =GetPrivateProfileStringA(sPort.c_str(),"BaudRate",
												NULL, szValue,
												sizeof(szValue),
												sFilePath.c_str());
	if (_dwExists != 0)
	{
		iTemp = parseBaud(szValue, index);
		if(iTemp != ERROR_SUCCESS)
			return iTemp;
	}
	else	//_iError in ini file
	{
		clog << "Error in INI file. No baud rate definied for "
				<< sPort << endl;
		return ERROR_INI;
	}

	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the text to send from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readTextToTransfer(string sPort, string sFilePath, int index)
{
	_dwExists = GetPrivateProfileStringA(sPort.c_str(),"TransferTextMode",
												NULL, szValue,
												sizeof(szValue),
												sFilePath.c_str());
	if (_dwExists != 0)
	{
		parseTextToTransfer(sPort, sFilePath, szValue, index);
	}
	else
	{
		clog << "Error in INI file. No text transfer mode definied for "
			 << sPort << endl;
			return ERROR_INI;
	}

	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the log  setting from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readLogger(string sPort, string sFilePath)
{
	_dwExists = GetPrivateProfileStringA(sPort.c_str(),"Logger",
												NULL, szValue,
												sizeof(szValue),
												sFilePath.c_str());
	if (_dwExists != 0)
	{
		if(0 == strcmp(szValue,"true"))
			return 0;				//for true
		else if(0 == strcmp(szValue,"false"))
			return 1;				// for false
		else
		{
			clog << "Error in INI file. Logging mode has to be either true or false in "
			 << sPort << endl;
			return ERROR_INI;
		}
	}
	else
	{
		clog << "Error in INI file. No logging mode definied for "
			 << sPort << endl;
			return ERROR_INI;
	}
}


//------------------------------------------------------------------------------
//	Reads how many times a test should be repeated from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or _iError in INI file
//------------------------------------------------------------------------------
int IniFileHandler::readRepeater(string sPort, string sFilePath)
{
	int iRepeater = 0;
	_dwExists = GetPrivateProfileStringA(sPort.c_str(),"Repeater",
												NULL, szValue,
												sizeof(szValue),
												sFilePath.c_str());
	if (_dwExists != 0)
	{
		iRepeater = strtol(szValue, NULL, 10);
		
		if (iRepeater > 0)
			return iRepeater;
		else
		{
			clog << "Error in INI file. Repeater has to be a integer number greater than 0, in "
			 << sPort << endl;
			return ERROR_INI;
		}
	}
	else
	{
		clog << "Error in INI file. No repeater definied for "
			 << sPort << endl;
			return ERROR_INI;
	}
}



//##############################################################################
//	PARSE METHODS
//##############################################################################

//------------------------------------------------------------------------------
//	Parse the text to transfer
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- string sTranferTextMode -> text mode for transmition
//		- int index -> index of the COM port for the vector
//	Return: parsed GUI parameters
//------------------------------------------------------------------------------
int IniFileHandler::parseTextToTransfer(string sPort, string sFilePath, string sTransferTextMode, int index)
{
	_dwExists =GetPrivateProfileStringA(sPort.c_str(),"TransferText",
													NULL, szValue,
													sizeof(szValue),
													sFilePath.c_str());
	if (_dwExists != 0)
	{

		if(sTransferTextMode == "file")
		{
			vComPorts.at(index).iTransTextMode = ID_BT_LOAD;
			vComPorts.at(index).sFilePath = szValue;
		}
		else if(sTransferTextMode == "text")
		{
			vComPorts.at(index).iTransTextMode = ID_BT_TEXT;
			vComPorts.at(index).sTextToTransfer = szValue;
		}
		else if(sTransferTextMode == "default")
		{
			return ERROR_SUCCESS;
		}
		else
		{
			clog << "Error in INI file. No correct text for transfer parameter for "
					<< sPort << endl;
			return ERROR_INI;
		}
	}	
	else
	{
		clog << "Error in INI file. No text for transfer definied for "
				<< sPort << endl;
		return ERROR_INI;
	}
	

	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Parse GUI parameters to a string
//	Parameters:
//	 IN:
//		- int iParity -> integer describing the parity for the transmition
//	Return: parsed GUI parameters
//------------------------------------------------------------------------------
string IniFileHandler::parseParityToIni(int iParity)
{
	string sTemp = "";
	switch(iParity)
	{
		case 0:
			sTemp = "none";
			break;

		case 1:
			sTemp = "odd";
			break;

		case 2:
			sTemp = "even";
			break;
	}

	return sTemp;

}




//------------------------------------------------------------------------------
//	Parse GUI parameters to a string
//	Parameters:
//	 IN:
//		- int iProtocol -> integer describing the transfer protocol
//	Return: parsed GUI parameters
//------------------------------------------------------------------------------
string IniFileHandler::parseProtocolToIni(int iProtocol)
{
	string sTemp = "";
	switch(iProtocol)
	{
		case 0:
			sTemp = "XON/XOFF";
			break;

		case 1:
			sTemp = "hardware";
			break;

		case 2:
			sTemp = "none";
			break;
	}

	return sTemp;

}


//------------------------------------------------------------------------------
//	Parse GUI parameters to a string
//	Parameters:
//	 IN:
//		- int iStopbits -> integer describing the stropbits for the transmition
//	Return: parsed GUI parameters
//------------------------------------------------------------------------------
string IniFileHandler::parseStopbitsToIni(int iStopbits)
{
	string sTemp = "";
	switch(iStopbits)
	{
		case 0:
			sTemp = "1";
			break;

		case 1:
			sTemp = "1.5";
			break;

		case 2:
			sTemp = "2";
			break;
	}

	return sTemp;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting and saves it in the right struct
//	Parameters:
//	 IN:
//		- string sBaud -> COM port baud rate
//		- int index -> index of the port struct to be tested
//------------------------------------------------------------------------------
int IniFileHandler::parseBaud(string sBaud, int index)
{
	int iTemp = 0;

	string sRate, sTemp, sTemp2;

	sRate = tools.delSpacesAndComents(sBaud);

	if (sRate == "MIN-MAX")
	{
		vComPorts.at(index).iBaud = 0;
		vComPorts.at(index).iBaudrateMax = 1;
	}
	else if( 0 != sRate.find_first_of("-")) //2 Baud rate values were given
	{
		iTemp = sRate.find_first_of("-");
		sTemp = sRate.substr(0, iTemp);
		sTemp2 = sRate.substr(sRate.find_first_of("-") + 1, sRate.length() - iTemp);

		if(sTemp == "MIN")
			vComPorts.at(index).iBaud = 0;
		else
		{
			iTemp = strtol(sTemp.c_str(),NULL, 10);
			if (iTemp != 0)
				vComPorts.at(index).iBaud = iTemp;
			else
				return ERROR_PARSE;
		}

		if (sTemp2 == "MAX")
			vComPorts.at(index).iBaudrateMax = 1;
		else
		{
			iTemp = strtol(sTemp2.c_str(),NULL, 10);
			if (iTemp != 0)
				vComPorts.at(index).iBaudrateMax = iTemp;
			else
				return ERROR_PARSE;
		}
	}
	else //only one baud rate value was given
	{
		iTemp = strtol(sBaud.c_str(),NULL, 10);
		if (iTemp != 0)
			vComPorts.at(index).iBaud = iTemp;
		else
			return ERROR_PARSE;
	}
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sTestMode -> COM port baud rate
//	Return: test mode as an integer, if fails returns parsing _iError
//------------------------------------------------------------------------------
int IniFileHandler::parseTestMode(string sTestMode)
{
	int i = ERROR_PARSE;

	if (sTestMode == "automatic")
		i = 0;
	else if(sTestMode == "wobble")
		i = 1;
	else if(sTestMode == "fixed")
		i = 2;
	else
	{
		clog << "Error parsing the test mode. Wrong parameter." << sTestMode
			 << "\nUse automatic, wobble or fixed" << endl;
	}
	
	return i;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sParity -> COM port parity
//	Return: parity as an integer, if fails returns parsing _iError
//------------------------------------------------------------------------------
int IniFileHandler::parseParity(string sParity, int index)
{
	string sPar;//, sTemp, sTemp2;
	int iErr = ERROR_SUCCESS;

	sPar = tools.delSpacesAndComents(sParity);

	if (sPar == "MIN-MAX")
	{
		vComPorts.at(index).iParity = 3;
	}
	else if (sParity == "none")
		vComPorts.at(index).iParity = 0;
	else if(sParity == "odd")
		vComPorts.at(index).iParity = 1;
	else if(sParity == "even")
		vComPorts.at(index).iParity = 2;
	else
	{
		clog << "Error parsing the parity. Wrong parameter." << sParity
			 << "\nUse none, odd, even, or MIN-MAX" << endl;
		iErr = ERROR_PARSE;
	}

	return iErr;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sProtocol -> transmition protocol
//	Return: protocol as an integer, if fails returns parsing _iError
//------------------------------------------------------------------------------
int IniFileHandler::parseProtocol(string sProtocol)
{
	int i = ERROR_PARSE;

	if (sProtocol == "XON/XOFF")
		i = 0;
	else if(sProtocol == "hardware")
		i = 1;
	else if(sProtocol == "none")
		i = 2;
	else
	{
		clog << "Error parsing the protocol. Wrong parameter." << sProtocol
			<< "\nUse XON/XOFF, hardware or none" << endl;
	}
	
	return i;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sStopbits -> transmition stopbits
//	Return: stopbits as an integer, if fails returns parsing _iError
//------------------------------------------------------------------------------
int IniFileHandler::parseStopbits(string sStopbits)
{
	int i = ERROR_PARSE;

	if (sStopbits == "1")
		i = 0;
	else if(sStopbits == "2")
		i = 2;
	else
	{
		clog << "Error parsing the stopbits. Wrong parameter." << sStopbits
			<< "\nUse 1 or 2" << endl;
	}
	
	return i;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sTransfer -> transfer mode
//	Return: transfer mode as an integer, if fails returns parsing _iError
//------------------------------------------------------------------------------
int IniFileHandler::parseTransfer(string sTransfer)
{
	int i = ERROR_PARSE;

	if (sTransfer == "single")
		i = 0;
	else if(sTransfer == "double")
		i = 1;
	else if(sTransfer == "master")
		i = 2;
	else if(sTransfer == "slave")
		i = 3;
	else
	{
		clog << "Error parsing the stopbits. Wrong parameter." << sTransfer
			<< "\nUse single, double, master or slave" << endl;
	}
	
	return i;
}


//------------------------------------------------------------------------------
//	Proves that the given port is correct
//	Parameters:
//	 IN:
//		- string sSlavePort -> COM port in the system to use for transfer
//	Return: if success string with the port name, else empty string 
//------------------------------------------------------------------------------
string IniFileHandler::parsePort(string sSlavePort)
{
	//first 3 characters are COM
	string sTemp = sSlavePort.substr(0, 3);

	if(sTemp == "COM")
	{
		sTemp = sSlavePort.substr(sSlavePort.find_first_of("M") + 1,
								  sSlavePort.npos);
		int iTemp = atoi(sTemp.c_str());

		if ( iTemp > 0 && iTemp < 256)
		{
			return sTemp;
		}
		else
		{
			clog << "Error in SlavePort. Please use a port between 1 and 255"
				 << endl;
			return "";
		}
	}
	else
	{
		clog << "Error in SlavePort. Please name the port \"COM\" and a number."
			 << "For example \"COM99\"" << endl;
		return "";
	}
}


//------------------------------------------------------------------------------
//	gets the vector with the ports to test and their settings
//	Return: vector containing the com ports
//------------------------------------------------------------------------------
vector<TestStruct> IniFileHandler::getTestStructure()
{
	return vComPorts;
}