/*
 *	GUI class
 */

#ifndef _WINDOW_H
#define _WINDOW_H

#include "Constants.h"
#include "BaseWindow.h"
#include "Com.h"
#include "Tools.h"

using namespace std;

class Com;
//use templace class BaseWindow
class Window : public BaseWindow<Window>
{

public:
//------------------------------------------------------------------------------
//Variables
	Com comEnumerator;
	Tools tools;

	DWORD error;

//------------------------------------------------------------------------------
//Methods
	
	//get
	int getTestMode();
	int getParity();
	int getStopBits();
	int getTransfer();
	int getProtocol();
	string getSelectedPort();
	DWORD getPortBaudRate();
	string getTransferFile();
	string getTextToSend();
	bool getLoggerState();
	
	//set
	void setTransferFile();
	void sethInstance(HINSTANCE hInst);
	
    PCWSTR  ClassName() const { return L"WN COM Test Tool"; }
    
	//user definied message handler. NOT static
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
//------------------------------------------------------------------------------
//Variables

	HINSTANCE hInst;
	UINT status;
	int ret;
	int _iTestMode;
	int _iParity;
	int _iStopBits;
	int _iTransfer;
	int _iProtocol;
	
	bool _bLoggerState;
	string _sPort;
	string _sTransferFilePath;
	string _sTempBaud;
	char _szTextToSend[31];
	DWORD _dwBaudrate;

	//Window handles
	HWND hwndCB_Ports;
	HWND hDebug;
	HWND hwndCB_Baud;
	HWND hwnd_lbLoad;
	HWND hwnd_btnLoad;
	HWND hwnd_lbText;
	HWND hwnd_btnText;
	HWND hwnd_Start;
	HWND hwnd_Close;

	//void CreateMyTooltip (HWND hwnd);
	//HWND CreateToolTip(int toolID, HWND hDlg, PTSTR pszText);
	
};

#endif