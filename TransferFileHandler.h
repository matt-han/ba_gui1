/*
 *	Opens, reads and closes text files to transfer trough the designated
 */

#ifndef _TRANSFERFILEHANDLER_H
#define _TRANSFERFILEHANDLER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <Winerror.h>

#include "Constants.h"
#include "Tools.h"

using namespace std;

class TransferFileHandler
{
	
public:
	TransferFileHandler(void);
	~TransferFileHandler(void);

//------------------------------------------------------------------------------
//Variablen
	Tools tools;

	ifstream inputFile;
	vector<string> vTranferFileLines;
	const char * path;
	string templine;

//------------------------------------------------------------------------------
//Methods
	
	int openFile(string sFilePath);
	void closeFile();
	int readTransferFile();
};

#endif