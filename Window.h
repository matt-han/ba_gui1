#ifndef _WINDOW_H
#define _WINDOW_H

#include "Constants.h"
#include "BaseWindow.h"
#include "Com.h"
#include "Tools.h"


using namespace std;

class Com;
class Window : public BaseWindow<Window>
{

public:
	
	
	Com comEnumerator;
	Tools tools;
	DWORD error;

	void sethInstance(HINSTANCE hInst);
	int getTestMode();
	int getParity();
	int getStopBits();
	int getTransfer();
	int getProtocol();
	string getSelectedPort();
	string getTransferFile();
	bool getLoggerState();
	
	
	void setTransferFile();

	
    PCWSTR  ClassName() const { return L"WN COM Test Tool"; }
    
	//user definied message hanlder. NOT static
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HINSTANCE hInst;
	int ret;
	
	UINT status;
	
	int _iTestMode;
	int _iParity;
	int _iStopBits;
	int _iTransfer;
	int _iProtocol;
	
	
	bool _bLoggerState;
	string _sPort;
	string _sTransferFile;

	string _sBaudrate;

	//were static on default, if errors maybe here?
	HWND hwndCB_Ports;
	HWND hDebug;
	HWND hwndCB_Baud;
	HWND hwnd_Load;
	HWND hwnd_Start;
	HWND hwnd_Close;

	//void CreateMyTooltip (HWND hwnd);
	//HWND CreateToolTip(int toolID, HWND hDlg, PTSTR pszText);
	
};

#endif