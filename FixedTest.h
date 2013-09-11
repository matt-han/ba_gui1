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

	HANDLE masterHCom;
	HANDLE slaveHCom;
	
	bool bTransmitionError;
	char empfang[100];

	vector<int> ivTestErrors;

//------------------------------------------------------------------------------
//Methods
	void setTextVector(int iTextMode);	
	int startSingleTest();
	int startDoubleTest();
	int startMasterTest();
	int startSlaveTest();
	
		string createTestInformation();
	int setTestInformation(string sTestInfo);

private:
//------------------------------------------------------------------------------
//Variables
	int _iError;
	int _iExitCode;
	int _iTestError;
	int _iTimeOut;
	int _iLineTimeOut;

	string _ESC;
	string _ACK;
	string _sTestInformation;
	
	int communicate(string sSendData, bool bMaster);

	//needed???????
	int communicateMaster(string sSendData);
	int communicateSlave(string sSendData);

	bool sendData(bool MasterSlave, string sSendData);
	string getData(bool MasterSlave, string sSendData);
	void printTestSettings();
	void setProtocol(int iProtocol, Com *com);
	int setPortDefaultSettings(Com &com); //timeout and 96,o,8,1
	int setPortSettings(Com &com);
	int syncMaster();
	int syncSlave();

};

#endif