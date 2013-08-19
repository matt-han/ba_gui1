/*
 *	Opens, reads and closes cfg/ini files for automated testing
 */



#ifndef _INIFILEHANDLER_H
#define _INIFILEHANDLER_H


#include "Constants.h"
#include "Tools.h"
#include <string>
#include <vector>
#include <Windows.h>


using namespace std;

class IniFileHandler
{
public:
	IniFileHandler(void);
	~IniFileHandler(void);
//------------------------------------------------------------------------------
//Methods

	//read methods
	int readINIFile(string sFilePath);
	int readPortConfig(string sPort,  string sFilePath, int index);
	int readTransferMode(string sPort, string sFilePath, int index);
	int readSlave(string sPort, string sFilePath, int index);
	int readSettings(string sPort, string sFilePath, int index);
	int readParity(string sPort, string sFilePath, int index);
	int readProtocol(string sPort, string sFilePath, int index);
	int readStopbits(string sPort, string sFilePath, int index);
	int readBaudRate(string sPort, string sFilePath, int index);

	//write methods
	void writeINIfile(string sMasterPort, string sSlavePort, int iBaud,
					  int iTestMode, int iParity, int iProtocol,
					  int iStopbits, int iTransfer, string sTextMode, 
					  string sTextToTransfer, string sPath);

	void writeINItransferSettings(string sMasterPort, string sSlavePort,
								  int iTransfer, string sPath);
	


private:
//------------------------------------------------------------------------------
//Variables
	
	int _iError;
	DWORD _dwExists;
	TestStruct comPortStruct;
	Tools tools;
	int index;
	char szValue[15];
	string sFilePath;
	string substr;
	
	vector<TestStruct> vComPorts;
	
//------------------------------------------------------------------------------
//Methods	
	//Parse from GUI to INI file
	string parseParityToIni(int iParity);
	string parseProtocolToIni(int iProtocol);
	string parseStopbitsToIni(int iStopbits);

	//Parse from INI file to GUI
	void parseBaud(string, int);
	int parseTestMode(string);
	int parseParity(string, int);
	int parseProtocol(string);
	int parseStopbits(string);
	int parseTransfer(string);
	string parsePort(string);

};

#endif