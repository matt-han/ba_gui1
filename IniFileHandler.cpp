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
//	Read configuration file for automated testing



//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
long IniFileHandler::readINIFile(string sFilePath)
{
	string sPort;
	string sTemp;
	int iPort = 0;

	//try to find test settings for all possible system ports
	for(int iCom = 1; iCom < 10; iCom++)
	{
		sPort = "COM";
		sTemp = tools.convertToString(iCom);
		sPort.append(sTemp);

		clog << "port " << sPort << endl;
		dwExists = GetPrivateProfileStringA(sPort.c_str(), "TestMode", NULL,
											szValue, sizeof(szValue),
											sFilePath.c_str());

		clog << "read error " << dwExists << "\n" << endl;

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
long IniFileHandler::readPortConfig(string sPort, string sFilePath, int iPort)
{
	int iTemp = parseTestMode(szValue);
	vComPorts.at(iPort).iTestMode = iTemp;

	switch(iTemp)
	{
		//automatic
		case 0:
			dwExists = GetPrivateProfileStringA(sPort.c_str(), "TransferMode",
											 NULL, szValue, sizeof(szValue),
											 sFilePath.c_str());
			if (dwExists != 0)
			{
				iTemp = parseTransfer(szValue);
				if (iTemp == 0)
					vComPorts.at(iPort).iTransfer = iTemp;
				else
					readPortSettings(sPort, sFilePath, iPort);
			}
			else
			{
				;//error in ini file
			}

			break;

		//wobble
		case 1:
			break;

		//fixed
		case 2:
			break;

	}

	vComPorts.at(iPort).iBaud = parseBaud("9600");
	return ERROR_SUCCESS;
	
}


long IniFileHandler::readPortSettings(string sPort,  string sFilePath,
									  int iPort)
{
	return ERROR_SUCCESS;
}


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
			<< "\nUse XON/XOFF, DTR/DSR or CTS/RTS" << endl;
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