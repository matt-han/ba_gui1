#include "TransferFileHandler.h"


TransferFileHandler::TransferFileHandler(void)
{
}


TransferFileHandler::~TransferFileHandler(void)
{
}


//------------------------------------------------------------------------------
//	Opens a file given by the parameter
//	Parameters:
//	 IN:
//		- string sFilePath -> file path to be opened
//	Return: error code signaling if operation succeded or error
//------------------------------------------------------------------------------
int TransferFileHandler::openFile(string sFilePath)
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
void TransferFileHandler::closeFile()
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
int TransferFileHandler::readTransferFile()
{
	while (!inputFile.eof())
	{
		getline(inputFile, templine);

		vTranferFileLines.push_back(templine);
	}

	if (vTranferFileLines.empty())
		return ERROR_EMPTY_FILE;
	else
		return ERROR_SUCCESS;
}