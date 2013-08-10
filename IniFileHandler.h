/*
 *	Opens, reads and closes cfg/ini files for automated tests
 *	Opens, reads/writes and closes text files to transfer trough the designated
 *	COM port
 */



#ifndef _INIFILEHANDLER_H
#define _INIFILEHANDLER_H

#include <string>
#include <vector>
#include <Windows.h>

#include "Com.h"
#include "Constants.h"
#include "Tools.h"

using namespace std;

class IniFileHandler
{
public:
	IniFileHandler(void);
	~IniFileHandler(void);
//------------------------------------------------------------------------------
//Methods

	//read methods
	long readINIFile(string sFilePath);
	long readPortConfig(string sPort,  string sFilePath, int index);
	long readTransferMode(string sPort, string sFilePath, int index);
	long readSlave(string sPort, string sFilePath, int index);
	long readSettings(string sPort, string sFilePath, int index);
	long readParity(string sPort, string sFilePath, int index);
	long readProtocol(string sPort, string sFilePath, int index);
	long readStopbits(string sPort, string sFilePath, int index);
	long readBaudRate(string sPort, string sFilePath, int index);

	//write methods
	void writeINIfile(string sMasterPort, string sSlavePort, int iBaud,
					  int iTestMode, int iParity, int iProtocol,
					  int iStopbits, int iTransfer, string sPath);

	void writeINItransferSettings(string sMasterPort, string sSlavePort,
								  int iTransfer, string sPath);
	


private:
//------------------------------------------------------------------------------
//Variables
	struct comPort
	{
		string sMasterPort;
		string sSlavePort;
		int iTransfer;
		int iBaud;
		int iTestMode;
		int iParity;
		int iProtocol;
		int iStopbits;
		
		//constructor
		comPort(void)
		{
			sMasterPort	= "";
			sSlavePort	= "";
			iBaud		= DEFAULT_VALUE;
			iTestMode	= DEFAULT_VALUE;
			iParity		= DEFAULT_VALUE;
			iProtocol	= DEFAULT_VALUE;
			iStopbits	= DEFAULT_VALUE;
			iTransfer	= DEFAULT_VALUE;
		}
	};//struct

	DWORD error;
	DWORD dwExists;
	comPort comPortStruct;
	Tools tools;
	int index;
	char szValue[15];
	string sFilePath;
	string substr;
	
	vector<comPort> vComPorts;
	
//------------------------------------------------------------------------------
//Methods	
	//Parse from GUI to INI file
	string parseParity(int iParity);
	string parseProtocol(int iProtocol);
	string parseStopbits(int iStopbits);

	//Parse from INI file to GUI
	int parseBaud(string);
	int parseTestMode(string);
	int parseParity(string);
	int parseProtocol(string);
	int parseStopbits(string);
	int parseTransfer(string);
	string parsePort(string);

};

#endif