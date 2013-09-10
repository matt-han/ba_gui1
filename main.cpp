
#include "IniFileHandler.h"
#include "Logger.h"


#include "Window.h"
#include "Interpreter.h"
#include "Tools.h"

#include <windows.h>
#include <vector>
#include <conio.h>
#include <thread>

void start();
int createGUI(HINSTANCE hInstance, int nCmdShow);
int parseCmdParameters();
int parsePort();
string sFilePath, sPort;
vector<string> svParameters;
bool bContinue = true;
int iError;
Tools tools;
Interpreter interpreter;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR pCmdLine, int nCmdShow)
{
	char c;
	bool bStopTest = false;
	iError = ERROR_SUCCESS;
	int _iError = ERROR_SUCCESS;
	int iCount = 0;
	//WobbleTest wobble;
	//Tools t;
	//string s = "abc\\0d-\\0asd";
	//string ss = t.replaceASCII(s);

	//return 0;
	
	if (0 == strcmp(pCmdLine, "") )
		_iError = createGUI(hInstance, nCmdShow);
	else
	{
		svParameters = tools.parseCmdLine(pCmdLine);
		
		_iError = parseCmdParameters();
		if (_iError == ERROR_SUCCESS)
		{

			if (!AllocConsole())
				MessageBoxA(NULL,"console alloc error","",MB_OK);
		
			cout << "starting the thread" << endl;

			thread t1 (&start);

			t1.detach();

			cout << "Test in progress" << endl;
			cout << "Press ESC to stop the test" << endl;
			

			while(bContinue && !bStopTest)
			{
				c = getch();

				//27 for esc key
				if (c == 27)
				{
					interpreter.stopTest();
					cout << "Quiting test" << endl;
					bStopTest = true;
					cout << "Testing stopped. Press enter to finish" << endl;
					//getchar();
				}
				iCount++;
			}

			cout << "Testing finished" << endl;
			
			//getchar();

			FreeConsole();

			return iError;
		}
		else
		{//error parsing parameters
			return _iError;
		}

		return ERROR_SUCCESS;
	}

	return _iError;
}





void start()
{
	iError = interpreter.loadIniFile(sFilePath, sPort);
	bContinue = false;
	cout << "Testing finished. Press enter to finish" << endl;
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

    if (!win.Create(L"WN COM Test Tool", WS_OVERLAPPEDWINDOW))
    {
        return ERROR_CREATE_GUI;
    }

//------------------------------------------------------------------------------
//	Message Loop
//------------------------------------------------------------------------------
    ShowWindow(win.window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

//	win.joinThread();

	return ERROR_SUCCESS;
}