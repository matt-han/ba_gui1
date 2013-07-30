#include "FileManager.h"


FileManager::FileManager(void)
{
}


FileManager::~FileManager(void)
{
}


long FileManager::openFile(string sFilePath)
{
	std::clog << "filepath: " << sFilePath << endl;
	
	path = sFilePath.c_str();
	
	inputFile.open(path,ios::in);
	
	if(inputFile.is_open())
		return ERROR_SUCCESS;
	else
		return ERROR_FILE_NOT_OPEN;
}

void FileManager::closeFile()
{
	if ( inputFile.is_open())
	{
		inputFile.close();
	}
}

long FileManager::readTransferFile()
{
	//getline(inputFile, templine);

	while (!inputFile.eof())
	{
		getline(inputFile, templine);
		vTranferFileLines.push_back(templine);
	}
	if (vTranferFileLines.empty())
		return ERROR_EMPTY_FILE;

	return ERROR_SUCCESS;


}

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
					if ( ERROR_SYNTAX == readPort())
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

long FileManager::readPort()
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
}