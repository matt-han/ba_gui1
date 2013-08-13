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
	startTest();
	return ERROR_SUCCESS;
}

long TestManager::startTest()
{
	evaluateInput();
	return ERROR_SUCCESS;
}

void TestManager::evaluateInput()
{
	//if fixed test
	if (testStruct.iTestMode == 2)
	{
		//if single transfer mode
		if (testStruct.iTransfer == 0)
		{
			FixedTest fixedTest(&testStruct);
		}
	}

}

long TestManager::createTestObject()
{

	return ERROR_SUCCESS;
}
