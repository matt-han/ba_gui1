#ifndef _FIXEDTEST_H
#define _FIXEDTEST_H

#include "Com.h"
#include "Constants.h"
#include "PortCommunications.h"
#include "TransferFileHandler.h"
#include "Tools.h"

#include <vector>

#define TEXT_LENGTH 8
using namespace std;


class FixedTest
{
public:
	FixedTest(void);
	FixedTest(TestStruct *testStruct);
	~FixedTest(void);

//------------------------------------------------------------------------------
//Variables
	Com masterCom;
	Com slaveCom;
	PortCommunications masterPortComm;
	PortCommunications slavePortComm;

	TransferFileHandler transFile;

	TestStruct *testStruct;

	Tools tools;

	string defaultText[TEXT_LENGTH];
	vector<string>vTextToSend;
	DWORD dwExitCode;
	HANDLE masterHCom;
	HANDLE slaveHCom;
	
	bool bTransmitionError;
	char empfang[100];

//------------------------------------------------------------------------------
//Methods
	void setTextVector(int iTextMode);	
	long startSingleTest();
	long startDoubleTest();
	long startMasterSlaveTest(bool bMaster);
	long startSlaveTest();
	long communicate(string sSendData, bool bMaster);
	
	
	long communicateMaster(string sSendData);
	long communicateSlave(string sSendData);

	bool sendData(bool MasterSlave, string sSendData);
	string getData(bool MasterSlave, string sSendData);
	void printTestSettings();
	
private:
	DWORD _dwError;
	DWORD _dwExitCode;
};

#endif