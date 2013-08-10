
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
	string port1 = "COM1";
	string port2 = "COM3";
	string recieved;
	string trans = "Hallo Welt";
    Logger log(true, port1);

	char info[100];
	char get_info[100];


	
	Window win;

	//IniFileHandler file;
	////------------------------------------------------------------------------------
	//file.writeINIfile( port1,"COM2", 9600, 0, 1, 0, 2, 0, "");
	//file.writeINIfile("COM2","COM2", 9600, 0, 1, 0, 2, 1, "");
	//file.writeINIfile( port2,"COM2", 9600, 0, 1, 0, 2, 2, "");

	//file.writeINIfile("COM11","COM2", 9600, 1, 1, 0, 2, 0, "");
	//file.writeINIfile("COM22","COM2", 9600, 1, 1, 0, 2, 1, "");
	//file.writeINIfile("COM33","COM2", 9600, 1, 1, 0, 2, 2, "");

	//file.writeINIfile("COM4","COM2", 9600, 2, 1, 0, 2, 0, "");
	//file.writeINIfile("COM5","COM2", 9600, 2, 1, 0, 2, 1, "");
	//file.writeINIfile("COM6","COM2", 9600, 2, 1, 0, 2, 2, "");

	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	//file.readINIFile("C:\\Users\\m.hansertvivar\\AppData\\Local\\Temp\\WN_ComPortTestFile.ini");
	//file.readINIFile("C:\\Users\\Benjamin\\AppData\\Local\\Temp\\WN_ComPortTestFile.ini");
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------

	//Com com(port1);
	//if (com.iExitCode == ERROR_PORT_OPEN)
	//{
	//	clog << "error opening port " << port1 << endl;
	//}
	//else if (com.iExitCode == ERROR_GET_DCB)
	//clog << "error getting dcb" << endl;


	//Com com2(port2);

	//PortCommunications PortComm(com.hCom);
	//PortCommunications PortComm2(com2.hCom);
	

	//if(com.iExitCode == ERROR_SUCCESS )//&& com2.iExitCode == 0)
	//{
	//	clog << "Port init ok" << endl;

	//	SecureZeroMemory(info, sizeof(info));
	//	SecureZeroMemory(get_info, sizeof(get_info));

	//	for (; z < 99; z++)
	//	{
	//		info[z] = 49 + z;
	//		get_info[z] = 0;
	//	}

	//	if (TRUE == PortComm.writeData(info, sizeof(info)) )
	//	{
	//		MessageBoxA(NULL, (LPCSTR)info, "COM 1 SENT", MB_OK);
	//		
	//		
	//		if (TRUE == PortComm.readData(get_info, sizeof(get_info)) )
	//		{
	//			MessageBoxA(NULL, (LPCSTR)get_info, "COM1 ARRIVED", MB_OK);
	//		}
	//	}
	//}

	//if (com.closePort() == ERROR_SUCCESS)
	//{
	//	clog << "port1 closed!"<<endl;
	//}

	//if (com2.closePort() == true)
	//{
	//	clog << "port2 closed!"<<endl;
	//}
	

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