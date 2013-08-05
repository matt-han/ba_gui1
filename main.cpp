
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

	char info[111];
	char get_info[150];
	//char get_info2[15];

	//string trans = "Hallo Welt";
	//unsigned char c;


	Window win;
	Interpreter interpreter(&win);

	//Com com(port1);
	//Com com2(port2);

	//PortHandler portHandler(com.hCom);
	//PortHandler portHandler2(com2.hCom);
	

	//if(com.iExitCode == 0 )//&& com2.iExitCode == 0)
	//{
		//com.getBaudrates();

		//com2.getBaudrates();
		
		/*clog << "Port init ok" << endl;

		for (; z < 110; z++)
		{
			info[z] = 49 + z;
			get_info[z] = 0;
		}
		info[110] = '\0';

		if (TRUE == portHandler.writeData(info, sizeof(info)) )
		{
			MessageBoxA(NULL, (LPCSTR)info, "COM 1 SENT", MB_OK);
			
			
			if (TRUE == portHandler.readData(get_info, sizeof(get_info)) )
			{
				MessageBoxA(NULL, (LPCSTR)get_info, "COM1 ARRIVED", MB_OK);
			}
		}*/
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

	//if (com.closePort() == true)
	//{
	//	clog << "port1 closed!"<<endl;
	//}

    return 0;
}