
#include "Interpreter.h"
#include "Window.h"
#include "Com.h"
#include "Logger.h"
#include <windows.h>

#include "PortHandler.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	int z = 0;
	string port1 = "COM1";
	string port2 = "COM2";
    Logger log(true, port1);
	unsigned char info[15];// = "Hallo Welt";
	unsigned char get_info[15];

	//string trans = "Hallo Welt";
	//unsigned char c;


	Window win;
	Interpreter interpreter(&win);

	Com com(port1);
	Com com2(port2);

	PortHandler portHandler(com.hCom);
	PortHandler portHandler2(com2.hCom);
	


	clog <<"test test test!"<< endl;

	if(com.iExitCode == 0)
	{
		clog << "Port init ok" << endl;

		for (; z < 14; z++)
			info[z] = 65 + z;
		info[14] = '\0';

		MessageBoxA(NULL, (LPCSTR)info, "SENT", MB_OK);

		portHandler.WriteToCOMPort(info);

		portHandler2.ReadFromCOMPort(get_info);

		MessageBoxA(NULL, (LPCSTR)get_info, "ARRIVED", MB_OK);

	}
	if (com.closePort() == true)
	{
		clog << "port closed!"<<endl;
	}
	

	win.sethInstance(hInstance);

    if (!win.Create(L"WN COM Test Tool", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	//interpreter.handleGui();

	log.closelog(true);

    return 0;
}