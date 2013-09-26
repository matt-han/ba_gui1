#ifndef _FIXEDTEST_H
#define _FIXEDTEST_H

#include "Com.h"
#include "Constants.h"
#include "PortCommunications.h"
#include "TransferFileHandler.h"
#include "Tools.h"

#include <vector>
#include <iostream>
#include <iomanip>


#define TEXT_LENGTH		8
#define HEADER_LENGTH	9	//	-> max header length 9 characters
#define BUFFER_LENGTH	100
#define SYNC_REPEATER	100

using namespace std;


class FixedTest
{
public:
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
	
	bool bTransmissionError;
	char empfang[BUFFER_LENGTH];

	vector<int> ivTestErrors;

//------------------------------------------------------------------------------
//Methods
	int setTextVector(int iTextMode);	
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
	//for slave test
	int _iTextLength;
	int _iLineNumber;

	string _ESC;
	string _ACK;
	string _sReceivedFromMaster;

	//Communication methods
	int communicate(string sSendData, bool bMaster);
	bool sendData(bool MasterSlave, string sSendData);
	string getData(bool MasterSlave, int iDataSize);

	//set settings methods
	void setProtocol(int iProtocol, Com *com);
	int setPortDefaultSettings(Com &com); //timeout and 96,o,8,1
	int setPortSettings(Com &com);

	void printTestSettings();

	//only master / slave methods
	int syncMaster();
	int syncSlave();

	string createHeader(int iLineNumber, int iTextLength);
	int parseHeader(string sHeader);

	int sendAndSync(string sHeader);
	int getAndSync(int iLength);

	int communicateMaster(string sSendData);
	int communicateSlave(int iLength);

};

#endif