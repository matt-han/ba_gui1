#include "TestManager.h"


TestManager::TestManager(void)
{
}


TestManager::~TestManager(void)
{
}

//
long TestManager::startManager()
{
	
	switch(testStruct.iTestMode)
	{
		//automatic test
		case 0:
				MessageBoxA(NULL, "not programmed yet", "!!!!!!!!!",
									MB_OK);
			dwError = startAutomaticTest();
			break;

		case 1:
				MessageBoxA(NULL, "not programmed yet", "!!!!!!!!!",
									MB_OK);
			dwError = startWobbleTest();
			break;

		case 2:
			dwError = startFixedTest();
			break;
	}
	
	//if(dwError == ERROR_SUCCESS)
	return dwError;
}


long TestManager::startFixedTest()
{
	FixedTest fixedTest(&testStruct);

	//set the text to be transfered
	fixedTest.setTextVector(testStruct.iTransTextMode);
	
	
	switch(testStruct.iTransfer)
	{
		//Single test
		case 0:
			dwError = fixedTest.startSingleTest();
			break;

		//Double Test
		case 1:
			dwError = fixedTest.startDoubleTest();
			break;

		//Master Slave
		case 2:
			dwError = fixedTest.startMasterSlaveTest();
			break;
	}
	
	return dwError;
}


long TestManager::startWobbleTest()
{
	return ERROR_SUCCESS;
}

long TestManager::startAutomaticTest()
{
	return ERROR_SUCCESS;
}