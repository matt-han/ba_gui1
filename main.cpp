
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
	string port2 = "COM3";

    Logger log(true, port1);

	char info[15];
	char get_info[15];
	char get_info2[15];

	//string trans = "Hallo Welt";
	//unsigned char c;


	Window win;
	Interpreter interpreter(&win);

	Com com(port1);
	Com com2(port2);

	PortHandler portHandler(com.hCom);
	PortHandler portHandler2(com2.hCom);
	


	clog <<"test test test!"<< endl;

	if(com.iExitCode == 0 && com2.iExitCode == 0)
	{
		com.getBaudrates();
		com2.getBaudrates();
		
		clog << "Port init ok" << endl;

		for (; z < 14; z++)
		{
			info[z] = 65 + z;
			get_info[z] = 0;
			get_info2[z] = 0;
		}
		info[14] = '\0';

		if (TRUE == portHandler.writeData(info))
		{
			MessageBoxA(NULL, (LPCSTR)info, "COM 1 SENT", MB_OK);
			
			



			//portHandler.readData(get_info);
			MessageBoxA(NULL, (LPCSTR)get_info, "COM1 ARRIVED", MB_OK);

			//portHandler2.readData(get_info2);
			//MessageBoxA(NULL, (LPCSTR)get_info2, "COM 3 ARRIVED", MB_OK);
		}
	}
	if (com.closePort() == true)
	{
		clog << "port1 closed!"<<endl;
	}
	if (com2.closePort() == true)
	{
		clog << "port2 closed!"<<endl;
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