/*
 *	Class to handle GUI inputs. Transmits information from the GUI to the
 *	TestProperties class to start testing
 */


#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "Com.h"
#include "TestManager.h"
#include "Constants.h"
#include "Tools.h"
#include "IniFileHandler.h"

#include <Windows.h>
#include <string>
#include <vector>


using namespace std;


class Interpreter
{
public:
	//Interpreter(Window *window);
	Interpreter(void);
	~Interpreter(void);
//------------------------------------------------------------------------------
//Variables
	Com comEnumerator;
	IniFileHandler iniFile;
	Tools tools;

//------------------------------------------------------------------------------
//Methods	
	void setTestMode(int);
	void setParity(int);
	void setStopBits(int);
	void setTransfer(int);
	void setProtocol(int);
	void setSelectedMasterPort(string);
	void setSelectedSlavePort(string);
	void setPortBaudRate(int);
	void setPortBaudRateMax(int);
	void setLoggerState(bool);
	void setTransferFile(string);
	void setTextToSend(string);
	void setTransTextMode(int);
	void setRepeater(string);
	void setDefaultValues();
	//is void 'cos gui does not handle values!
	void handleGui();
	


	int checkInputConfigData();
	int checkBaudrate(int);

	int saveToFile();
	int loadIniFile(string sPath, string sPort);

private:
//------------------------------------------------------------------------------
//Variables

	TestManager * _testManager;

	int _iTestMode;
	int _iParity;
	int _iStopBits;
	int _iTransfer;
	int _iProtocol;
	int _iInfoToTransfer;
	int _iBaudrate;
	int _iBaudrateMax;
	int _iTransTextMode;
	int _iRepeater;
	string _sMasPort;
	string _sSlaPort;
	string _sFilePath;
	string _sTextToSend;
	bool _bLoggerState;
	bool _bErr;
	vector<string> _svBaudrates;
	vector<TestStruct> _vIniFilePorts;

	int _iTemp;
	int _iError;

	string _sTemp;

};

#endif