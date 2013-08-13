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
	Com com;
	PortCommunications portComm;
	HANDLE hCom;
	TestStruct *testStruct;
	long startTest();
	long communicate();
	void printTestSettings();

private:
	DWORD _dwError;
};

#endif