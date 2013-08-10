#include "IniFileHandler.h"

//------------------------------------------------------------------------------
//	Constructor
//------------------------------------------------------------------------------
IniFileHandler::IniFileHandler(void)
{
	error = 0;
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
//		- int iTransfer -> single/double/master-slave transmition
//		- string sPath -> path where to save the ini test file
//------------------------------------------------------------------------------
void IniFileHandler::writeINIfile(string sMasterPort, string sSlavePort, int iBaud,
								int iTestMode, int iParity, int iProtocol,
								int iStopbits, int iTransfer, string sPath)
{
	string sBaud, sParity, sProtocol, sStopbits;
	
	//if path is empty then create default path
	if (sPath == "")
	{
		sPath = getenv("TEMP");
		sPath.append("\\WN_ComPortTestFile.ini");
	}

	sFilePath = sPath;

	//copy ints into strings
	if (iTestMode != 0)
	{
		if (iTestMode == 2)
			sBaud = tools.convertToString(iBaud);
		
		sParity	  = parseParity(iParity);
		sProtocol = parseProtocol(iProtocol);
		sStopbits = parseStopbits(iStopbits);
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
			
			//Ignore baudrate,
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
}


//------------------------------------------------------------------------------
//	Writes the transfer settings in the ini test file
//	Parameters:
//	 IN:
//		- string sMasterPort -> Master COM port for test
//		- string sSlavePort  -> Slave COM port for test
//		- int iTransfer -> single/double/master-slave transmition
//		- string sPath -> path where to save the ini test file
//	Return: error code signaling if operation succeded or error
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

	
		//master slave
		case 2:
			WritePrivateProfileStringA(sMasterPort.c_str(),"TransferMode",
										"masterSlave", sPath.c_str());
			WritePrivateProfileStringA(sMasterPort.c_str(),"SlavePort",
										sSlavePort.c_str(), sPath.c_str());
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
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
long IniFileHandler::readINIFile(string sFilePath)
{
	string sPort;
	string sTemp;
	int iPort = 0;

	//try to find test settings for all possible system ports
	for(int iCom = 1; iCom < 10; iCom++)//256
	{
		sPort = "COM";
		sTemp = tools.convertToString(iCom);
		sPort.append(sTemp);

		dwExists = GetPrivateProfileStringA(sPort.c_str(), "TestMode", NULL,
											szValue, sizeof(szValue),
											sFilePath.c_str());

		if (0 != dwExists)
		{
			vComPorts.push_back(comPort());
			vComPorts.at(iPort).sMasterPort = sPort;

			error = readPortConfig(sPort, sFilePath, iPort);
			if (error != ERROR_SUCCESS)
			{
				clog << "Error reading port " << sPort << " from INI file."
					 << "Error " << error << endl;
			}
			iPort++;
		}
		error = 0;
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
//	Return: if success or error in INI file
//------------------------------------------------------------------------------
long IniFileHandler::readPortConfig(string sPort, string sFilePath, int index)
{
	string sTemp = "";
	int iTemp = parseTestMode(szValue);
	vComPorts.at(index).iTestMode = iTemp;

	//always read transfer mode
	//all ports have a transfer mode
	error = readTransferMode(sPort, sFilePath, index);
	if (error == ERROR_SUCCESS)
	{
		switch(iTemp)
		{
			//automatic
			//------------------------------------------------------------------
			case 0:	
				//if transfer mode equals double or masterSlave then read
				//the slave port
				if (vComPorts.at(index).iTransfer != 0)
				{
					error = readSlave(sPort, sFilePath.c_str(), index);
					if (error != ERROR_SUCCESS)
						return error;
				}
				//else do nothing
				break;
			//------------------------------------------------------------------
			

			//wobble
			//------------------------------------------------------------------
			case 1:
				//if transfer mode equals double or masterSlave then read
				//the slave port
				if (vComPorts.at(index).iTransfer != 0)
				{
					error = readSlave(sPort, sFilePath.c_str(), index);
					if (error != ERROR_SUCCESS)
						return error;
					else
					{
						error = readSettings(sPort, sFilePath.c_str(), index);
						if (error != ERROR_SUCCESS)
							return error;
					}
				}
				else	//if transfer mode equals single, then no slave port
				{
					error = readSettings(sPort, sFilePath.c_str(), index);
					if (error != ERROR_SUCCESS)
						return error;
				}
				break;
			//------------------------------------------------------------------


			//fixed
			//------------------------------------------------------------------
			case 2:
				//if transfer mode equals double or masterSlave then read
				//the slave port
				if (vComPorts.at(index).iTransfer != 0)
				{
					error = readSlave(sPort, sFilePath.c_str(), index);
					
					if (error != ERROR_SUCCESS)
						return error;
					else
					{	//read the baud rate
						error = readBaudRate(sPort, sFilePath.c_str(), index);
						
						if (error != ERROR_SUCCESS)
							return error;
						else
						{	//read the other settings
							error = readSettings(sPort, sFilePath.c_str(), index);
							if (error != ERROR_SUCCESS)
								return error;
						}
					}
				}
				else	//if transfer mode equals single, then no slave port
				{		//read the baud rate
					error = readBaudRate(sPort, sFilePath.c_str(), index);
					
					if (error != ERROR_SUCCESS)
						return error;
					else	//read the other settings
					{
						error = readSettings(sPort, sFilePath.c_str(), index);
						
						if (error != ERROR_SUCCESS)
							return error;
					}
				}
				break;
			//------------------------------------------------------------------

		}//switch
	}//if
	else
		return error;

	return ERROR_SUCCESS;
	
}

//------------------------------------------------------------------------------
//	Reads the transfer mode setting from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or error in INI file
//------------------------------------------------------------------------------
long IniFileHandler::readTransferMode(string sPort, string sFilePath, int index)
{
	int iTemp = -1;
	dwExists = GetPrivateProfileStringA(sPort.c_str(), "TransferMode",
											 NULL, szValue, sizeof(szValue),
											 sFilePath.c_str());
	if (dwExists != 0)
	{
		iTemp = parseTransfer(szValue);
		if (iTemp != ERROR_PARSE)
		{
			vComPorts.at(index).iTransfer = iTemp;
		}//error_parse
		else
		{
			clog << "Error in INI file. Incorrect transfer mode for "
				 << "port " << sPort << endl;
			return ERROR_INI;
		}
	}
	else	//error in ini file
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
//	Return: if success or error in INI file
//------------------------------------------------------------------------------
long IniFileHandler::readSlave(string sPort, string sFilePath, int index)
{
	string sTemp = "";
	dwExists = GetPrivateProfileStringA(sPort.c_str(),
												"SlavePort",
												 NULL, szValue, sizeof(szValue),
												 sFilePath.c_str());
	if (dwExists != 0)
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
//	Return: if success or error in INI file
//------------------------------------------------------------------------------
long IniFileHandler::readSettings(string sPort, string sFilePath, int index)
{
	error = readParity(sPort, sFilePath, index);
	if (error == ERROR_SUCCESS)
	{
		error = readProtocol(sPort, sFilePath, index);
		if (error == ERROR_SUCCESS)
		{
			error = readStopbits(sPort, sFilePath, index);
			if (error != ERROR_SUCCESS)
			{
				return error;
			}
		}
		else
			return error;
	}
	else
		return error;

	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Reads the parity setting from the INI file
//	Parameters:
//	 IN:
//		- string sPort -> COM port in the system to use for transfer
//		- string sFilePath -> INI file path
//		- int index -> index of the COM port for the vector
//	Return: if success or error in INI file
//------------------------------------------------------------------------------
long IniFileHandler::readParity(string sPort, string sFilePath, int index)
{
	int iTemp = -1;

	dwExists = GetPrivateProfileStringA(sPort.c_str(), "Parity",
											 NULL, szValue, sizeof(szValue),
											 sFilePath.c_str());
	if (dwExists != 0)
	{
		iTemp = parseParity(szValue);
		if (iTemp != ERROR_PARSE)
		{
			vComPorts.at(index).iParity = iTemp;
		}	
		else
		{
			clog << "Error in INI file. Incorrect parity for "
				<< "port " << sPort << endl;
			return ERROR_INI;
		}
	}
	else	//error in ini file
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
//	Return: if success or error in INI file
//------------------------------------------------------------------------------
long IniFileHandler::readProtocol(string sPort, string sFilePath, int index)
{
	int iTemp = -1;

	dwExists = GetPrivateProfileStringA(sPort.c_str(), "Protocol",
										NULL, szValue, sizeof(szValue),
										sFilePath.c_str());
	if (dwExists != 0)
	{
		iTemp = parseProtocol(szValue);
		if (iTemp != ERROR_PARSE)
		{
			vComPorts.at(index).iProtocol = iTemp;
		}//error_parse
		else
		{
			clog << "Error in INI file. Incorrect protocol for "
				 << "port " << sPort << endl;
			return ERROR_INI;
		}
	}
	else	//error in ini file
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
//	Return: if success or error in INI file
//------------------------------------------------------------------------------
long IniFileHandler::readStopbits(string sPort, string sFilePath, int index)
{
	int iTemp = -1;

	dwExists =GetPrivateProfileStringA(sPort.c_str(),"Stopbits",
												NULL, szValue,
												sizeof(szValue),
												sFilePath.c_str());
	if (dwExists != 0)
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
	else	//error in ini file
	{
		clog << "Error in INI file. No Stopbits definied for "
				<< sPort << " port" << endl;
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
//	Return: if success or error in INI file
//------------------------------------------------------------------------------
long IniFileHandler::readBaudRate(string sPort, string sFilePath, int index)
{
	int iTemp = 0;
	
	//Get port stopbits
	//----------------------------------------------------------
	dwExists =GetPrivateProfileStringA(sPort.c_str(),"BaudRate",
												NULL, szValue,
												sizeof(szValue),
												sFilePath.c_str());
	if (dwExists != 0)
	{
		iTemp = parseBaud(szValue);
		if (iTemp != ERROR_PARSE)
		{
			vComPorts.at(index).iBaud = iTemp;
		}
		else
		{
			clog << "Error in INI file. Incorrect baud rate for "
				 << "port " << sPort << endl;
			return ERROR_INI;
		}
	}
	else	//error in ini file
	{
		clog << "Error in INI file. No baud rate definied for "
				<< sPort << " port" << endl;
		return ERROR_INI;
	}
	return ERROR_SUCCESS;
}


//##############################################################################
//	PARSE METHODS
//##############################################################################
//------------------------------------------------------------------------------
//	Parse GUI parameters to a string
//	Parameters:
//	 IN:
//		- int iParity -> integer describing the parity for the transmition
//	Return: parsed GUI parameters
//------------------------------------------------------------------------------
string IniFileHandler::parseParity(int iParity)
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
string IniFileHandler::parseProtocol(int iProtocol)
{
	string sTemp = "";
	switch(iProtocol)
	{
		case 0:
			sTemp = "XON/XOFF";
			break;

		case 1:
			sTemp = "DTR/DSR";
			break;

		case 2:
			sTemp = "CTS/RTS";
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
string IniFileHandler::parseStopbits(int iStopbits)
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
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sBaud -> COM port baud rate
//	Return: baud rate as an integer, if fails returns parsing error
//------------------------------------------------------------------------------
int IniFileHandler::parseBaud(string sBaud)
{
	int i = 0;
	i = atoi (sBaud.c_str());
	return i;
}

//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sTestMode -> COM port baud rate
//	Return: test mode as an integer, if fails returns parsing error
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
//	Return: parity as an integer, if fails returns parsing error
//------------------------------------------------------------------------------
int IniFileHandler::parseParity(string sParity)
{
	int i = ERROR_PARSE;

	if (sParity == "none")
		i = 0;
	else if(sParity == "odd")
		i = 1;
	else if(sParity == "even")
		i = 2;
	else
	{
		clog << "Error parsing the parity. Wrong parameter." << sParity
			 << "\nUse none, odd or even" << endl;
	}
	
	return i;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sProtocol -> transmition protocol
//	Return: protocol as an integer, if fails returns parsing error
//------------------------------------------------------------------------------
int IniFileHandler::parseProtocol(string sProtocol)
{
	int i = ERROR_PARSE;

	if (sProtocol == "XON/XOFF")
		i = 0;
	else if(sProtocol == "DTR/DSR")
		i = 1;
	else if(sProtocol == "CTS/RTS")
		i = 2;
	else
	{
		clog << "Error parsing the protocol. Wrong parameter." << sProtocol
			<< "\nUse XON/XOFF, DTR/DSR or CTS/RTS" << endl;
	}
	
	return i;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sStopbits -> transmition stopbits
//	Return: stopbits as an integer, if fails returns parsing error
//------------------------------------------------------------------------------
int IniFileHandler::parseStopbits(string sStopbits)
{
	int i = ERROR_PARSE;

	if (sStopbits == "1")
		i = 0;
	else if(sStopbits == "1.5")
		i = 1;
	else if(sStopbits == "2")
		i = 2;
	else
	{
		clog << "Error parsing the stopbits. Wrong parameter." << sStopbits
			<< "\nUse 1, 1.5 or 2" << endl;
	}
	
	return i;
}


//------------------------------------------------------------------------------
//	parse INI file parameter to test setting
//	Parameters:
//	 IN:
//		- string sTransfer -> transfer mode
//	Return: transfer mode as an integer, if fails returns parsing error
//------------------------------------------------------------------------------
int IniFileHandler::parseTransfer(string sTransfer)
{
	int i = ERROR_PARSE;

	if (sTransfer == "single")
		i = 0;
	else if(sTransfer == "double")
		i = 1;
	else if(sTransfer == "masterSlave")
		i = 2;
	else
	{
		clog << "Error parsing the stopbits. Wrong parameter." << sTransfer
			<< "\nUse single, double or masterSlave" << endl;
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