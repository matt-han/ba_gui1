#ifndef _FIXEDTEST_H
#define _FIXEDTEST_H

#include "Com.h"
#include "Constants.h"
#include "PortCommunications.h"


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

	DWORD dwExitCode;
	HANDLE masterHCom;
	HANDLE slaveHCom;
	TestStruct *testStruct;

	//char test[15];
	string test;
	char empfang[100];

//------------------------------------------------------------------------------
//Methods	
	long startSingleTest();
	long startDoubleTest();
	long startMasterSlaveTest();
	long communicate(bool MasterSlave);
	bool sendData(bool MasterSlave);
	bool getData(bool MasterSlave);
	void printTestSettings();

private:
	DWORD _dwError;
};

#endif