#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

#include "Com.h"
#include "Constants.h"
#include "Tools.h"

using namespace std;

class FileManager
{
private:
	Tools tools;

	struct comPort{
		string sPortname;
		string sTestmode;
		int iParity;
		string sProtocol;
		int iStopbits;
		string sTransfer;
		bool bLogger;
	};

	int index;
	const char * path;
	ifstream inputFile;
	string templine;
	string substr;
	vector<string> vTranferFileLines;
	vector<comPort> vComPorts;


public:
	FileManager(void);
	~FileManager(void);

	long openFile(string sFilePath);
	void closeFile();
	long readTransferFile();
	long readConfigFile();
	long readPort();

	
	//open
	//read & save
	//close
};

#endif