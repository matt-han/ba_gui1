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

//------------------------------------------------------------------------------
//Methods

	long startManager();
	long startTest();
	void evaluateInput();
	long createTestObject();

	//get the text to be transmitted... either from a file, input string or default
	long getTextToTransmit();



};

#endif