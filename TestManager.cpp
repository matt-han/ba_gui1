#include "TestManager.h"


TestManager::TestManager(void)
{
}


TestManager::~TestManager(void)
{
}

//
int TestManager::startManager()
{
	_bError = false;
	string sBegin, sEnd;
	int iBegin = 0;
	int iEnd = 0;
	int iTemp;
	switch(testStruct.iTestMode)
	{
		//automatic test
		case 0:
			MessageBoxA(NULL, "not programmed yet", "!!!!!!!!!",
									MB_OK);
			_iError = startAutomaticTest();
			break;

		//wobble
		case 1:

			sBegin = _tools.convertToString(testStruct.iBaud);
			sEnd = _tools.convertToString(testStruct.iBaudrateMax);

			//determine the being and end index of the array
			for( int i = 0;  i < testStruct.svBaudrates.size(); i++)
			{
				if(sBegin == testStruct.svBaudrates.at(i))
					iBegin = i;

				if(sEnd == testStruct.svBaudrates.at(i))
				{
					iEnd = i;
					break;
				}
			}

			if(iBegin >= iEnd)
			{
				MessageBoxA(NULL, "MAX baud rate has to be higher than MIN baud rate", "ERROR",
							MB_OK);
				_iError = ERROR_BAUD_MINMAX;
				break;
			}

			for(int index = iBegin; index <= iEnd; index++)
			{
				iTemp = atoi(testStruct.svBaudrates.at(index).c_str());
				_iError = startWobbleTest(iTemp);
				if (_iError != ERROR_SUCCESS)
				{
					clog << "Error in wobble test. Error: " << _iError << endl;
					_bError = true;
				}
			}
			
			if(_bError)
			{
				clog << "error in double test!"<<endl;
				_iError = ERROR_WOBBLE;
			}

			break;


		//fixed
		case 2:
			_iError = startFixedTest();
			break;
	}
	
	//if(_iError == ERROR_SUCCESS)
	return _iError;
}


int TestManager::startFixedTest()
{
	FixedTest fixedTest(&testStruct);

	//set the text to be transfered
	fixedTest.setTextVector(testStruct.iTransTextMode);
	
	
	switch(testStruct.iTransfer)
	{
		//Single test
		case 0:
			_iError = fixedTest.startSingleTest();
			break;

		//Double Test
		case 1:
			_iError = fixedTest.startDoubleTest();
			break;

		//Master Slave
		case 2:
			_iError = fixedTest.startMasterSlaveTest(true);
			break;

		case 3:
			_iError = fixedTest.startMasterSlaveTest(false);
			break;
	}
	
	return _iError;
}


int TestManager::startWobbleTest(int iBaudrate)
{
	//create for each test a new object to avoid errors
	//WobbleTest wobble;
	return ERROR_SUCCESS;
}

int TestManager::startAutomaticTest()
{
	return ERROR_SUCCESS;
}