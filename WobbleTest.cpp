#include "WobbleTest.h"


WobbleTest::WobbleTest(void)
{
}


WobbleTest::~WobbleTest(void)
{
}

WobbleTest::WobbleTest(TestStruct *testStruct)
{
	this->testStruct = testStruct;
	masterCom.sPort = this->testStruct->sMasterPort;
	slaveCom.sPort = this->testStruct->sSlavePort;

	_iExitCode = ERROR_SUCCESS;

	bTransmitionError = false;

	//MessageBoxA(NULL, defaultText[1].c_str(), "error", MB_OK);
}