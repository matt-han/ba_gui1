/*
 *	Class to handle GUI inputs. Transmits information from the GUI to the
 *	TestProperties class to start testing
 */


#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "Constants.h"

#include <Windows.h>
#include <string>
#include <array>


using namespace std;


class Interpreter
{
public:
	//Interpreter(Window *window);
	Interpreter(void);
	~Interpreter(void);
//------------------------------------------------------------------------------
//Variables


//------------------------------------------------------------------------------
//Methods	
	void setTestMode(int);
	void setParity(int);
	void setStopBits(int);
	void setTransfer(int);
	void setProtocol(int);
	void setSelectedPort(string);
	void setPortBaudRate(DWORD);
	void setLoggerState(bool);
	void setTransferFile(string);
	void setTextToSend(string);

	void handleGui(int iInformationToTransfer);
	void setBaudrateList();

private:
//------------------------------------------------------------------------------
//Variables

	int _iTestMode;
	int _iParity;
	int _iStopBits;
	int _iTransfer;
	int _iProtocol;
	int _iInfoToTransfer;
	DWORD _dwBaudrate;
	string _sPort;
	string _sTransferFile;
	string _sTextToSend;
	bool _bLoggerState;
};

#endif