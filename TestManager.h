#ifndef _TESTMANAGER_H
#define _TESTMANAGER_H


#include "FixedTest.h"
#include "Tools.h"
#include "Logger.h"
#include "Constants.h"
#include "IniFileHandler.h"

#include <vector>
//#include <thread>



using namespace std;


class TestManager
{
public:
	TestManager(void);
	~TestManager(void);

//------------------------------------------------------------------------------
//Variables
	
	TestStruct testStruct;
	vector<int> ivTestErrors;
	string s;
	bool bStopButton;
//------------------------------------------------------------------------------
//Methods
	
	void evaluateInput();
	int startManager();
	int startAutomaticTest();
	int startWobbleTest(int iBaudrate, int iParity);
	int startFixedTest();

	//get the text to be transmitted... either from a file, input string or default
	int getTextToTransmit();
	void saveSettingsToFile();


private:
//------------------------------------------------------------------------------
//Variables
	Tools _tools;
	Logger * _logger;
	IniFileHandler * _IniFile;
	int _iError;
	bool _bError;
	bool _bWobbleParity;
	bool _bTestStarted;
	bool _bContinueTest;

	HANDLE _hSubThread;
};

#endif