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

#include <Windows.h>
#include <string>
//#include <array>
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
	void setDefaultValues();
	void handleGui();
	void setBaudrateList();
	
	int checkInputConfigData();
	int checkBaudrate(int);

private:
//------------------------------------------------------------------------------
//Variables

	TestManager testManager;

	int _iTestMode;
	int _iParity;
	int _iStopBits;
	int _iTransfer;
	int _iProtocol;
	int _iInfoToTransfer;
	int _iBaudrate;
	int _iBaudrateMax;
	int _iTransTextMode;
	string _sMasPort;
	string _sSlaPort;
	string _sFilePath;
	string _sTextToSend;
	bool _bLoggerState;

	vector<string> _svBaudrates;

	int _iTemp;
	int iError;

};

#endif