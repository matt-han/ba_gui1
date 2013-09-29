#include "Window.h"
#include "Interpreter.h"
#include "Tools.h"

#include <windows.h>
#include <vector>
#include <conio.h>
#include <stdio.h>
#include <thread>

//Methoden
void start();
int createGUI(HINSTANCE hInstance, int nCmdShow);
int parseCmdParameters();
int parsePort();
void setTestError(int iError);


//Variablen
string sFilePath, sPort;
vector<string> svParameters;
bool bContinue = true;
bool bPrint = true;
char c;
int iTestError;
Tools tools;
Interpreter interpreter;


int main(int argc, char * argv[])
{
	
	bool bStopTest = false;
	iTestError = 1;
	int _iError = 2;

	//if no arguments start the GUI
	if (1 == argc)
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		_iError = createGUI(hInstance, SW_SHOW);
	}
	else
	{
		bPrint = false;
		//get the arguments from the command line
		svParameters = tools.parseCmdLine(argv, argc);
		
		//parse the arguments
		_iError = parseCmdParameters();
		if (_iError == ERROR_SUCCESS)
		{

			cout << "\n\nStarting the test, parsing the configuration file" << endl;
			cout << "Press ESC to stop the test" << endl << endl;

			//start the test in a new thread
			thread t1 (&start);
			t1.detach();

			//continues as long as esc nor the test is finished
			while(bContinue && !bStopTest)
			{
				//if a key was stroked
				if(kbhit())
				{
					c = getch();

					//27 for esc key
					if (c == 27)
					{
						interpreter.stopTest();
						cout << "Quiting test" << endl;
						bStopTest = true;
						cout << "Test stopped" << endl;
						iTestError = ERROR_ESC;
						break;
					}
				}
			}

			return iTestError;
		}
		else
		{
			//error parsing parameters
			tools.showCmdHelp();
			return _iError;
		}
	}
	return _iError;
}




//------------------------------------------------------------------------------
//	Starts in a new thread and the interpreter object loads the given file
//	Return:error code signaling if operation succeded or sytax error
//------------------------------------------------------------------------------
void start()
{

	iTestError = interpreter.loadIniFile(sFilePath, sPort);

	cout << "\n\n++++++++++++++++++++" << endl;

	if(iTestError != ERROR_SUCCESS)
		cout << "Error while testing" << endl;
	else
		cout << "Testing finished" << endl;

	
	bContinue = false;
}


//------------------------------------------------------------------------------
//	Parse the cmd line parameters to start testing
//	Return:error code signaling if operation succeded or sytax error
//------------------------------------------------------------------------------
int parseCmdParameters()
{
	if (svParameters.size() > 2)
	{
		return ERROR_CMD_SYNTAX;
	}
	else
	{
		if(string::npos == svParameters.at(0).find("\\"))
			return ERROR_CMD_SYNTAX;

		sFilePath = svParameters.at(0);
		
		//is 2 parameters, then second is a port
		if( 2 == svParameters.size() )
		{
			string sTemp;

			sTemp = svParameters.at(1);

			if(sTemp.at(0) == '/')
			{
				if (ERROR_SUCCESS == parsePort() )
				{
					sPort = sTemp.substr(1, sTemp.npos);
					return ERROR_SUCCESS;
				}
				else 
					return ERROR_CMD_SYNTAX;
			}
			else
				return ERROR_CMD_SYNTAX;
		}
		else
			sPort = "";
	}

	return ERROR_SUCCESS;
}


//------------------------------------------------------------------------------
//	parses the given port from the cmd line
//  Port -> /COMxxx where 0 < xxx < 257
//	Return: _iError code signaling if operation succeded or _iError
//------------------------------------------------------------------------------
int parsePort()
{
	// sPort = /COMXXX
	string sTemp = svParameters.at(1).substr(1, sTemp.npos);

	//sTemp = COMXXX
	if("COM" == sTemp.substr(0, 3) )
	{
		string sNumber = sTemp.substr(3, sTemp.npos);
		int iNumber = strtol(sNumber.c_str(), NULL, 10);
		
		if (iNumber > 0 && iNumber < 257)
			return ERROR_SUCCESS;
	}

	return ERROR_CMD_SYNTAX;
}


//------------------------------------------------------------------------------
//	starts the GUI
//	Parameters:
//	 IN:
//		- HINSTANCE hInstance -> handle to the instance
//		- int nCmdShow -> format for how to show the window
//	Return: _iError code signaling if operation succeded or _iError
//------------------------------------------------------------------------------
int createGUI(HINSTANCE hInstance, int nCmdShow)
{
	Window win;

	win.sethInstance(hInstance);

	if (!win.Create(WINDOW_TITLE , WS_OVERLAPPEDWINDOW))
    {
        return ERROR_CREATE_GUI;
    }

    ShowWindow(win.window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	return ERROR_SUCCESS;
}