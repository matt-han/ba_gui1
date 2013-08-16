
#include "Interpreter.h"
#include "IniFileHandler.h"
#include "Window.h"
#include "Com.h"
#include "Logger.h"
#include <windows.h>

#include "PortCommunications.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	int z = 0;
	string sPort1 = "COM1";
	string sPort2 = "COM3";
    Logger log(true, sPort1);

	char info[100];
	char get_info[100];
	
	Window win;

	IniFileHandler file;
	////------------------------------------------------------------------------------
	file.writeINIfile( sPort1,"COM2", 9600, 0, 1, 0, 2, 0, "");
	file.writeINIfile("COM2","COM2", 9600, 0, 1, 0, 2, 1, "");
	file.writeINIfile( sPort2,"COM2", 9600, 0, 1, 0, 2, 2, "");

	file.writeINIfile("COM11","COM2", 9600, 1, 1, 0, 2, 0, "");
	file.writeINIfile("COM22","COM2", 9600, 1, 1, 0, 2, 1, "");
	file.writeINIfile("COM33","COM2", 9600, 1, 1, 0, 2, 2, "");

	file.writeINIfile("COM4","COM2", 9600, 2, 1, 0, 2, 0, "");
	file.writeINIfile("COM5","COM2", 9600, 2, 1, 0, 2, 1, "");
	file.writeINIfile("COM6","COM2", 9600, 2, 1, 0, 2, 2, "");

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	//file.readINIFile("C:\\Users\\m.hansertvivar\\AppData\\Local\\Temp\\WN_ComPortTestFile.ini");
	//file.readINIFile("C:\\Users\\Benjamin\\AppData\\Local\\Temp\\WN_ComPortTestFile.ini");
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------

	win.sethInstance(hInstance);

    if (!win.Create(L"WN COM Test Tool", WS_OVERLAPPEDWINDOW))
    {
        return 0;
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
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
	//interpreter.handleGui();

	log.closelog(true);


    return ERROR_SUCCESS;
}