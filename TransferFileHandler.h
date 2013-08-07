#ifndef _TRANSFERFILEHANDLER_H
#define _TRANSFERFILEHANDLER_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <Winerror.h>
#include "Constants.h"

using namespace std;

class TransferFileHandler
{
	
public:
	TransferFileHandler(void);
	~TransferFileHandler(void);

	
	long openFile(string sFilePath);
	void closeFile();
	long readTransferFile();

	ifstream inputFile;
	vector<string> vTranferFileLines;
	const char * path;
	string templine;
};

#endif