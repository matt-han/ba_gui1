#include "IniFileHandler.h"

//------------------------------------------------------------------------------
//	Constructor
//------------------------------------------------------------------------------
IniFileHandler::IniFileHandler(void)
{
}


//------------------------------------------------------------------------------
//	Deconstructor
//------------------------------------------------------------------------------
IniFileHandler::~IniFileHandler(void)
{
}




//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//	Read configuration file for automated testing



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
			readPortConfig(sPort, sFilePath, iPort);
			iPort++;
		}

	}
	//if any read the 
	return ERROR_SUCCESS;
	//else Errooooor
}


//------------------------------------------------------------------------------
//	Reads port configuration from opened configuration file. Saves the
//	information in the corresponding vectors or variables




//	Return: error code signaling if operation succeded or error
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
			case 0:	
				//if transfer mode equals double or masterSlave then read
				//the slave port
				if (vComPorts.at(index).iTransfer != 0)
				{
					error = readSlave(sPort, sFilePath.c_str(), index);
					if (error != ERROR_SUCCESS)
						return error;
				}
				break;

			//wobble
			case 1:
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
				break;

			//fixed
			case 2:

				break;

		}
	}
	else
		return error;


	vComPorts.at(index).iBaud = parseBaud("9600");

	return ERROR_SUCCESS;
	
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------



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
	string sTemp = sSlavePort.substr(0, sSlavePort.find_first_of("M"));

	if(sTemp == "COM")
	{
		sTemp = sSlavePort.substr(sSlavePort.find_first_of("M"),
								  sSlavePort.npos - 2);
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
}

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
			vComPorts.at(index).sSlavePort = sTemp;
	}
	else
	{
		clog << "Error in INI file. No slave port given for"
				<< " port " << sPort << endl;
		return ERROR_INI;
	}
	return ERROR_SUCCESS;
}


long IniFileHandler::readSettings(string sPort, string sFilePath, int index)
{
	error = readParity(sPort, sFilePath, index);
	if (error == ERROR_SUCCESS)
	{
		error = readProtocol(sPort, sFilePath, index);
		if (error == ERROR_SUCCESS)
		{
			error = readProtocol(sPort, sFilePath, index);
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


long IniFileHandler::readParity(string sPort, string sFilePath, int index)
{
	int iTemp = -1;
	//Get port parity
	//--------------------------------------------------------------------------
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
}
long IniFileHandler::readProtocol(string sPort, string sFilePath, int index)
{
	int iTemp = -1;
	
	//get port protocol
	//------------------------------------------------------------------
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
	
}

long IniFileHandler::readStopbits(string sPort, string sFilePath, int index)
{
	int iTemp = -1;
	
	//Get port stopbits
	//----------------------------------------------------------
	dwExists =GetPrivateProfileStringA(sPort.c_str(),"Stopbits",
												NULL, szValue,
												sizeof(szValue),
												sFilePath.c_str());
	if (dwExists != 0)
	{
		iTemp = parseParity(szValue);
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

}