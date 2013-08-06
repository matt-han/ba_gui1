#include "FileManager.h"

//------------------------------------------------------------------------------
//	Constructor
//------------------------------------------------------------------------------
FileManager::FileManager(void)
{
}


//------------------------------------------------------------------------------
//	Deconstructor
//------------------------------------------------------------------------------
FileManager::~FileManager(void)
{
}


//------------------------------------------------------------------------------
//	Opens a file given by the parameter
//	Parameters:
//	 IN:
//		- string sFilePath -> file path to be opened
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
long FileManager::openFile(string sFilePath)
{
	clog << "filepath: " << sFilePath << endl;
	
	path = sFilePath.c_str();
	
	inputFile.open(path,ios::in);
	
	if(inputFile.is_open())
		return ERROR_SUCCESS;
	else
		return ERROR_FILE_NOT_OPEN;
}


//------------------------------------------------------------------------------
//	Close the opened file by the object
//------------------------------------------------------------------------------
void FileManager::closeFile()
{
	if ( inputFile.is_open())
	{
		inputFile.close();
	}
}


//------------------------------------------------------------------------------
//	Reads the information/text that will be transmited by the COM port and saves
//	each line in the vector vTranferFileLines
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
long FileManager::readTransferFile()
{

	if (vTranferFileLines.empty())
		return ERROR_EMPTY_FILE;
	else
	{
		while (!inputFile.eof())
		{
			getline(inputFile, templine);
			vTranferFileLines.push_back(templine);
		}
	}
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Read configuration file for automated testing
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
long FileManager::readConfigFile()
{
	index = -1;

	getline(inputFile, templine);

	if(0 == templine.compare("#CFG Datei#"))
	{
		while(!inputFile.eof())
		{
			getline(inputFile, templine);
			templine = tools.delSpacesAndComents(templine);
		
			if (templine != "")
			{
				if ( 0 == templine.compare("[Port]"))
				{
					vComPorts.push_back(comPort());
					index++;
					if ( ERROR_SYNTAX == readPortConfig())
						return ERROR_SYNTAX;
				}
				else
				{
					return ERROR_SYNTAX;
				}
			}
		}//while
	}
	else
	{
		return ERROR_CFG_HEADER;
	}
		

}


//------------------------------------------------------------------------------
//	Reads port configuration from opened configuration file. Saves the
//	information in the corresponding vectors or variables
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
long FileManager::readPortConfig()
{
	while(!inputFile.eof())
	{
		getline(inputFile, templine);
		templine = tools.delSpacesAndComents(templine);

		if (templine != "")
		{
			substr = templine.substr(0, templine.npos - templine.find_first_of("=") );
			
			if (substr == "portname")
			{
				vComPorts[index].sPortname = templine.substr(templine.find_first_of("="), templine.npos);
			}
			else if (substr == "testmode")
			{

				vComPorts[index].sTestmode = templine.substr(templine.find_first_of("="), templine.npos);
			}
			else if (substr == "parity")
			{
				substr = templine.substr(templine.find_first_of("="), templine.npos);
				vComPorts[index].iParity = atoi(substr.c_str() );
			}
			else if (substr == "protocol")
			{
				vComPorts[index].sProtocol = templine.substr(templine.find_first_of("="), templine.npos);
			}
			else if (substr == "stopbits")
			{
				substr = templine.substr(templine.find_first_of("="), templine.npos);
				vComPorts[index].iStopbits = atoi(substr.c_str() );
			}
			else if (substr == "transfer")
			{
				vComPorts[index].sTransfer = templine.substr(templine.find_first_of("="), templine.npos);
			}
			else if (substr == "logfile")
			{
				substr =  templine.substr(templine.find_first_of("="), templine.npos);

				if ( substr == "true")
					vComPorts[index].bLogger = true;
				else
					vComPorts[index].bLogger = false;
			}
			else if (substr == "[/Port]")
			{
				break;
			}
			else
			{
				return ERROR_SYNTAX;
			}
		}//templine != ""
	}//while
	return ERROR_SUCCESS;
}