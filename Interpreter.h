/*
 *	Class to handle GUI inputs. Transmits information from the GUI to the
 *	TestProperties class to start testing
 */


#ifndef _INTERPRETER_H
#define _INTERPRETER_H

#include "Constants.h"
#include "Window.h"

#include <Windows.h>
#include <string>
#include <array>


class Interpreter
{
public:
	Interpreter(Window *window);
	Interpreter(void);
	~Interpreter(void);
//------------------------------------------------------------------------------
//Variables

	Window *window;
	/*int getTestMode();
	int getParity();
	int getStopBits();
	int getTransfer();
	string getSelectedPort();
	bool getLoggerState();*/
//------------------------------------------------------------------------------
//Methods
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
	bool _bLoggerState;
};

#endif