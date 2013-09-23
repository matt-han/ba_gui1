#include "TestManager.h"

//------------------------------------------------------------------------------
//Default constructor
//------------------------------------------------------------------------------
TestManager::TestManager(void)
{
	_bTestStarted  = false;
	_bContinueTest = true;
	bStopButton    = false;

	_logger  = NULL;
	_IniFile = NULL;
}


//------------------------------------------------------------------------------
//Default deconstructor
//closes the logger if a logger was opened
//------------------------------------------------------------------------------
TestManager::~TestManager(void)
{
	if(_logger != NULL)
		delete _logger;
}


//------------------------------------------------------------------------------
//	Starts the test manager to start a fixed, wobble or automatic
//Return: test exit code
//------------------------------------------------------------------------------
int TestManager::startManager()
{

	//always create a logger, if not required then it redirects clog to null
	_logger = new Logger(testStruct.bLoggerState, testStruct.sMasterPort, testStruct.iTransfer);
	if(_logger->iError != ERROR_SUCCESS)
	{
		_iError = _logger->iError;
		delete _logger;
		return _iError;
	}

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

			//find baud rates
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

			//user wrote MIN in INI file
			if(testStruct.iBaud == MIN_BAUD)
				iBegin = 0;

			//user wrote MAX in INI file
			if(testStruct.iBaudrateMax == MAX_BAUD)
				iEnd = testStruct.svBaudrates.size() - 1;

			if(iBegin >= iEnd)
			{
				MessageBoxA(NULL, "MAX baud rate has to be higher than MIN baud rate", WINDOW_TITLE,
							MB_OK | MB_ICONERROR);
				_iError = ERROR_BAUD_MINMAX;
				break;
			}


			//min max parity
			if(testStruct.iParity == 3)
			{
				_bWobbleParity = true;
			}
			else
				iPar = testStruct.iParity;

			//if master wait 5 sec
			if(testStruct.iTransfer == 2)
				_tools.wait(300);

			//save test to a ini file if logging is enabled
			if(testStruct.bLoggerState)
				saveSettingsToFile();

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
								<< "\n" << _tools.errorCodeParser(_iError) << endl;
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
						clog << "Error in wobble test. Error: " << _iError 
							 << "\n" << _tools.errorCodeParser(_iError) << endl;
						clog << "Baudrate: " << iTemp << endl;
						clog << "Parity:   " << iPar << endl;
						_bError = true;
					}
				}

			}//for baudrates

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
	_bTestStarted  = true;
	_bContinueTest = true;

	int iRepeat = 1;

	//set the text to be transfered
	fixedTest.setTextVector(testStruct.iTransTextMode);

	//save test to a ini file if logging is enabled
if(testStruct.bLoggerState)
		saveSettingsToFile();

	do
	{
		clog << "\n\nTEST NR. " << iRepeat << endl;
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
				if(iRepeat == 1)
					_tools.wait(300);

				_iError = fixedTest.startMasterTest();
				break;

			//Slave
			case 3:
				_iError = fixedTest.startSlaveTest();
				break;
		}//switch
	
		if(	iRepeat == testStruct.iRepeater)
			_bContinueTest = false;

		//if test should stop on first error and the return code was no success,
		//stop the current test
		if(testStruct.bStopOnError && _iError != ERROR_SUCCESS)
			_bContinueTest = false;

		iRepeat++;

	}while(_bContinueTest && !bStopButton);


	if(testStruct.bStopOnError)
		return _iError;
	else
		return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Starts wooble test
//Return: test exit code in a vector
//------------------------------------------------------------------------------
int TestManager::startWobbleTest(int iBaudrate, int iParity)
{
	_bTestStarted  = true;
	_bContinueTest = true;
	int iRepeat = 1;
	
	//create for each test a new object to avoid errors
	FixedTest wobble(&testStruct);

	wobble.setTextVector(testStruct.iTransTextMode);
	wobble.testStruct->iBaud   = iBaudrate;
	wobble.testStruct->iParity = iParity;

	do
	{
		clog << "\n\nTEST NR. " << iRepeat << endl;
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
				_iError = wobble.startMasterTest();
				break;

			//Slave
			case 3:
				_iError = wobble.startSlaveTest();
				break;
		}//switch

		if(	iRepeat == testStruct.iRepeater)
			_bContinueTest = false;

		//if test should stop on first error and the return code was no success,
		//stop the current test
		if(testStruct.bStopOnError && _iError != ERROR_SUCCESS)
			_bContinueTest = false;

		iRepeat++;

	}while(_bContinueTest && !bStopButton);
	
	if(testStruct.bStopOnError)
		return _iError;
	else
		return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Starts automatic test
//Return: test exit code in a vector
//------------------------------------------------------------------------------
int TestManager::startAutomaticTest()
{
	int iRepeat = 1;
	_bTestStarted  = true;
	_bContinueTest = true;

	testStruct.iParity			= ODDPARITY;
	testStruct.iStopbits		= ONESTOPBIT;
	testStruct.iProtocol		= 1;
	testStruct.iBaud			= B_9600;
	testStruct.iDatabits		= 8;
	testStruct.sTextToTransfer	= "";
	testStruct.iTransTextMode	= DEFAULT_VALUE;


	FixedTest automatic(&testStruct);

	automatic.setTextVector(testStruct.iTransTextMode);

	//save test to a ini file if logging is enabled
	if(testStruct.bLoggerState)
		saveSettingsToFile();

	//ask stop button..........
	do
	{
		clog << "\n\nTEST NR. " << iRepeat << endl;
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
				if(iRepeat == 1)
					_tools.wait(300);
				_iError = automatic.startMasterTest();
				break;

			//Slave
			case 3:
				_iError = automatic.startSlaveTest();
				break;
		}//switch

		iRepeat++;

		//if test should stop on first error and the return code was no success,
		//stop the current test
		if(testStruct.bStopOnError && _iError != ERROR_SUCCESS)
			_bContinueTest = false;

	}while(_bContinueTest && !bStopButton);

	if(testStruct.bStopOnError)
		return _iError;
	else
		return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	Saves the current test settings to a test configuration file
//------------------------------------------------------------------------------
void TestManager::saveSettingsToFile()
{
	_IniFile = new IniFileHandler();

	string sRepeater = _tools.convertToString(testStruct.iRepeater);

	_IniFile->writeINIfile(testStruct.sMasterPort,
							testStruct.sSlavePort,
							testStruct.iBaud,
							testStruct.iBaudrateMax,
							testStruct.iTestMode,
							testStruct.iParity,
							testStruct.iProtocol,
							testStruct.iStopbits,
							testStruct.iDatabits,
							testStruct.iTransfer,
							testStruct.iTestMode,
							testStruct.sTextToTransfer,
							sRepeater,
							testStruct.bLoggerState,
							testStruct.bStopOnError,
							"");

	if(_IniFile != NULL)
		delete _IniFile;

}