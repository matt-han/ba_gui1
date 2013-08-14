#ifndef _FIXEDTEST_H
#define _FIXEDTEST_H

#include "Com.h"
#include "Constants.h"
#include "PortCommunications.h"
#include "TransferFileHandler.h"

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

	string defaultText[TEXT_LENGTH];
	vector<string>vTextToSend;
	DWORD dwExitCode;
	HANDLE masterHCom;
	HANDLE slaveHCom;
	

	//char test[15];
	string test;
	char empfang[100];

//------------------------------------------------------------------------------
//Methods
	void setTextVector(int iTextMode);	
	long startSingleTest();
	long startDoubleTest();
	long startMasterSlaveTest();
	long communicate(string sSendData);
	bool sendData(bool MasterSlave, string sSendData);
	string getData(bool MasterSlave, string sSendData);
	void printTestSettings();
	
private:
	DWORD _dwError;
};

#endif