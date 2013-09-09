#include "TestManager.h"

//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
TestManager::TestManager(void)
{
	_bTestStarted = false;
	_bContinueTest = true;
	bStopButton = false;

}


//------------------------------------------------------------------------------
//Default deconstructor
//closes the logger if a logger was opened
//------------------------------------------------------------------------------
TestManager::~TestManager(void)
{
	if(testStruct.bLoggerState && _bTestStarted)
	{
		//_logger->closelog(true);
		delete _logger;
	}
}


//------------------------------------------------------------------------------
//	Starts the test manager to start a fixed, wobble or automatic
//Return: test exit code
//------------------------------------------------------------------------------
int TestManager::startManager()
{

	if(testStruct.bLoggerState)
		_logger = new Logger(true, testStruct.sMasterPort);

	_bError = false;
	string sBegin, sEnd;
	int iBegin = 0;
	int iEnd = 0;
	int iTemp;

	switch(testStruct.iTestMode)
	{
		//automatic test
		case 0:
			_iError = startAutomaticTest();
			break;

	//wobble
	//----------------------------------------------------------------------
		case 1:
			_bWobbleParity = false;
			int iPar;
			//user wrote MIN - MAX Baudrate in INI file
			if(testStruct.iBaud == 0 && testStruct.iBaudrateMax == 1)
			{
				iBegin = 0;
				iEnd = testStruct.svBaudrates.size() - 1;
			}
			else
			{

				sBegin = _tools.convertToString(testStruct.iBaud);
				sEnd   = _tools.convertToString(testStruct.iBaudrateMax);

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
			}//MIN MAX Baudrate


			//min max parity
			if(testStruct.iParity == 3)
			{
				_bWobbleParity = true;
			}
			else
				iPar = testStruct.iParity;



			//start the tests
			for(int index = iBegin; index <= iEnd; index++)
			{
				iTemp = atoi(testStruct.svBaudrates.at(index).c_str());
				
				//for each new baud rate, reset the error vector
				ivTestErrors.clear();

				//for min max parity
				if(_bWobbleParity)
				{
					for(iPar = 0; iPar < 3; iPar++)
					{
						_iError = startWobbleTest(iTemp, iPar);
						if (_iError != ERROR_SUCCESS)
						{
							clog << "Error in wobble test. Error: " << _iError
								 << endl;
							clog << "Baudrate: " << iTemp << endl;
							clog << "Parity:   " << iPar << endl;
							_bError = true;
						}
					}
				}
				else
				{
					_iError = startWobbleTest(iTemp, iPar);

					if (_iError != ERROR_SUCCESS)
					{
						clog << "Error in wobble test. Error: " << _iError << endl;
						clog << "Baudrate: " << iTemp << endl;
						clog << "Parity:   " << iPar << endl;
						_bError = true;
					}
				}

				
			}//for baudrates
			
			if(_bError)
			{
				clog << "error in double test!"<<endl;
				_iError = ERROR_WOBBLE;
			}

			break;


	//fixed
	//----------------------------------------------------------------------
		case 2:
			_iError = startFixedTest();
			break;
	}
	
	return _iError;
}


//------------------------------------------------------------------------------
//	Starts fixed test
//Return: test exit code in a vector
//------------------------------------------------------------------------------
int TestManager::startFixedTest()
{
	FixedTest fixedTest(&testStruct);
	_bTestStarted = true;
	_bContinueTest = true;

	//ivTestErrors.clear();

	int iRepeat = 1;

	//set the text to be transfered
	fixedTest.setTextVector(testStruct.iTransTextMode);

	do
	{
		clog << "TEST NR. " << iRepeat << endl;
		clog << "*********************************************" << endl;

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

			//Master 
			case 2:
				_iError = fixedTest.startMasterSlaveTest(true);
				break;

			//Slave
			case 3:
				_iError = fixedTest.startMasterSlaveTest(false);
				break;
		}//switch

		//ivTestErrors.push_back(_iError);
	
		if(testStruct.iRepeater > 0)
		{
			if(	iRepeat < testStruct.iRepeater)
				iRepeat++;
			else
				_bContinueTest = false;
		}
		//clog << "..............................."<<endl;
		//clog << "_bContinueTest " << _bContinueTest << endl;
		//clog << "bStopButton   " << bStopButton << endl;

	}while(_bContinueTest && !bStopButton);

	//_tools.printErrorVector(testStruct.bLoggerState, ivTestErrors);

	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Starts wooble test
//Return: test exit code in a vector
//------------------------------------------------------------------------------
int TestManager::startWobbleTest(int iBaudrate, int iParity)
{
	_bTestStarted = true;
	_bContinueTest = true;
	int iRepeat = 1;
	
	//create for each test a new object to avoid errors
	FixedTest wobble(&testStruct);

	wobble.setTextVector(testStruct.iTransTextMode);
	wobble.testStruct->iBaud   = iBaudrate;
	wobble.testStruct->iParity = iParity;

	do
	{
		clog << "TEST NR. " << iRepeat << endl;
		clog << "*********************************************" << endl;
		switch(testStruct.iTransfer)
		{
			//Single test
			case 0:
				_iError = wobble.startSingleTest();
				break;

			//Double Test
			case 1:
				_iError = wobble.startDoubleTest();
				break;

			//Master 
			case 2:
				_iError = wobble.startMasterSlaveTest(true);
				break;

			//Slave
			case 3:
				_iError = wobble.startMasterSlaveTest(false);
				break;
		}//switch

		//ivTestErrors.push_back(_iError);

		if(testStruct.iRepeater > 0)
		{
			if(	iRepeat < testStruct.iRepeater)
				iRepeat++;
			else
				_bContinueTest = false;
		}

	}while(_bContinueTest && !bStopButton);

	//_tools.printErrorVector(testStruct.bLoggerState, ivTestErrors);
	
	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Starts automatic test
//Return: test exit code in a vector
//------------------------------------------------------------------------------
int TestManager::startAutomaticTest()
{
	int iRepeat = 1;
	_bTestStarted = true;

	testStruct.iParity			= ODDPARITY;
	testStruct.iStopbits		= ONESTOPBIT;
	testStruct.iProtocol		= 1;
	testStruct.iBaud			= B_9600;
	testStruct.iDatabits		= 8;
	testStruct.iRepeater		= -1;
	testStruct.sTextToTransfer	= "";
	testStruct.iTransTextMode	= DEFAULT_VALUE;


	FixedTest automatic(&testStruct);

	//ask stop button..........
	//do
	//{
		clog << "TEST NR. " << iRepeat << endl;
		clog << "*********************************************" << endl;
		switch(testStruct.iTransfer)
		{
			//Single test
			case 0:
				_iError = automatic.startSingleTest();
				break;

			//Double Test
			case 1:
				_iError = automatic.startDoubleTest();
				break;

			//Master 
			case 2:
				_iError = automatic.startMasterSlaveTest(true);
				break;

			//Slave
			case 3:
				_iError = automatic.startMasterSlaveTest(false);
				break;
		}//switch
		iRepeat++;
	//}while(!bStopButton);


	return ERROR_SUCCESS;
}