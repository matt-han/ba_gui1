
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

	//Com com(sPort1);
	//clog << "PAR:none; STOP:1, none" << endl;
	//com.closePort();

	//Com com2("COM2");
	//clog << "PAR:even; STOP:2, XON/XOFF"<<endl;
	//com2.closePort();

	//Com com3(sPort2);
	//clog << "PAR:uneven; STOP:1, HARDWARE"<<endl;
	//com3.closePort();

	//IniFileHandler file;
	////------------------------------------------------------------------------------
	//file.writeINIfile( sPort1,"COM2", 9600, 0, 1, 0, 2, 0,"file","somefile", "");
	//file.writeINIfile("COM2","COM2", 9600, 0, 1, 0, 2, 1, "text", "texttosend","");
	//file.writeINIfile( sPort2,"COM2", 9600, 0, 1, 0, 2, 2, "default", "", "");

	//file.writeINIfile("COM11","COM2", 9600, 1, 1, 0, 2, 0, "text", "texttosend","");
	//file.writeINIfile("COM22","COM2", 9600, 1, 1, 0, 2, 1, "default", "", "");
	//file.writeINIfile("COM33","COM2", 9600, 1, 1, 0, 2, 2,"file","somefile", "");

	//file.writeINIfile("COM4","COM2", 9600, 2, 1, 0, 2, 0, "default", "", "");
	//file.writeINIfile("COM5","COM2", 9600, 2, 1, 0, 2, 1,"file","somefile", "");
	//file.writeINIfile("COM6","COM2", 9600, 2, 1, 0, 2, 2, "text", "texttosend","");

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	//file.readINIFile("C:\\Users\\m.hansertvivar\\AppData\\Local\\Temp\\WN_ComPortTestFile.ini");
	//file.readINIFile("C:\\Users\\Matthias Hansert\\AppData\\Local\\Temp\\WN_ComPortTestFile_COM1.ini");
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------

	win.sethInstance(hInstance);

    if (!win.Create(L"WN COM Test Tool", WS_OVERLAPPEDWINDOW))
    {
        return -100;
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