/*
 *	Opens, reads and closes cfg/ini files for automated tests
 *	Opens, reads/writes and closes text files to transfer trough the designated
 *	COM port
 */



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
public:
	FileManager(void);
	~FileManager(void);
//------------------------------------------------------------------------------
//Methods
	long openFile(string sFilePath);
	void closeFile();
	long readTransferFile();
	long readConfigFile();
	long readPortConfig();

private:
//------------------------------------------------------------------------------
//Variables	
	Tools tools;
	int index;
	const char * path;
	ifstream inputFile;
	string templine;
	string substr;
	vector<string> vTranferFileLines;
	vector<comPort> vComPorts;
	
	struct comPort{
		string sPortname;
		string sTestmode;
		int iParity;
		string sProtocol;
		int iStopbits;
		string sTransfer;
		bool bLogger;
	};

	
};

#endif