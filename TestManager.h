#ifndef _TESTMANAGER_H
#define _TESTMANAGER_H

#include "Com.h"
#include "FixedTest.h"
#include "Constants.h"


using namespace std;


class TestManager
{
public:
	TestManager(void);
	~TestManager(void);

//------------------------------------------------------------------------------
//Variables
	//FixedTest fixedTest;
	TestStruct testStruct;
	DWORD dwError;
//------------------------------------------------------------------------------
//Methods
void evaluateInput();

	long startManager();
	long startAutomaticTest();
	long startWobbleTest();
	long startFixedTest();

	//get the text to be transmitted... either from a file, input string or default
	long getTextToTransmit();



};

#endif