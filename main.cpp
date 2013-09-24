//test headers
#include "IniFileHandler.h"
#include "Logger.h"
#include "FixedTest.h"
#include "Com.h"


#include "Window.h"
#include "Interpreter.h"
#include "Tools.h"

#include <windows.h>
#include <vector>
#include <conio.h>
#include <stdio.h>
#include <thread>

void start();
int createGUI(HINSTANCE hInstance, int nCmdShow);
int parseCmdParameters();
int parsePort();

string sFilePath, sPort;
vector<string> svParameters;
bool bContinue = true;
bool bPrint = true;
char c;
int iError;
Tools tools;
Interpreter interpreter;


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int nCmdShow)
int main(int argc, char * argv[])
{
	
	bool bStopTest = false;
	iError = ERROR_SUCCESS;
	int _iError = ERROR_SUCCESS;

	if (1 == argc)
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);
		_iError = createGUI(hInstance, SW_SHOW);
	}
	else
	{
		bPrint = false;
		svParameters = tools.parseCmdLine(argv, argc);
		
		_iError = parseCmdParameters();
		if (_iError == ERROR_SUCCESS)
		{

			cout << "\n\nStarting the test, parsing the configuration file" << endl;
			cout << "Press ESC to stop the test" << endl;

			thread t1 (&start);
			t1.detach();

			while(bContinue && !bStopTest)
			{
				if(kbhit())
				{
					c = getch();

					//27 for esc key
					if (c == 27)
					{
						interpreter.stopTest();
						cout << "Quiting test" << endl;
						bStopTest = true;
						cout << "Testing stopped. Press enter to finish" << endl;
						break;
					}
				}
			}

			//getchar();
			return iError;
		}
		else
		{//error parsing parameters
			tools.showCmdHelp();
			//getchar();
			return _iError;
		}
	}
	//getchar();
	return _iError;
}





void start()
{
	iError = interpreter.loadIniFile(sFilePath, sPort);
	if(iError != ERROR_SUCCESS)
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