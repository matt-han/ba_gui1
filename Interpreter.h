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

	Window *window;

	TCHAR * baudRate[6];

	/*int getTestMode();
	int getParity();
	int getStopBits();
	int getTransfer();
	string getSelectedPort();
	bool getLoggerState();*/

	void handleGui();
	void setBaudrateList();

private:
	int _iTestMode;
	int _iParity;
	int _iStopBits;
	int _iTransfer;
	int _iProtocol;
	DWORD _dwBaudrate;


	string _sPort;
	string _sTransferFile;
	bool _bLoggerState;
};

#endif