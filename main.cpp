
#include "Interpreter.h"
#include "Window.h"
#include "Com.h"
#include "Logger.h"
#include <windows.h>


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	string port = "COM1";
    Logger log(true, port);
	
	Window win;
	Interpreter interpreter(&win);
	Com com(port);
	


	clog <<"test test test!"<< endl;

	if(com.iExitCode == 0)
	{
		clog << "Port init ok" << endl;
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