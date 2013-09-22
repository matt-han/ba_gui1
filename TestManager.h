#ifndef _TESTMANAGER_H
#define _TESTMANAGER_H


#include "FixedTest.h"
#include "Tools.h"
#include "Logger.h"
#include "Constants.h"
#include "IniFileHandler.h"

#include <vector>


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
	
	int startManager();

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

//------------------------------------------------------------------------------
//Methods

	int startAutomaticTest();
	int startWobbleTest(int iBaudrate, int iParity);
	int startFixedTest();
	void saveSettingsToFile();
};

#endif