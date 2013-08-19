/*
 *	GUI class
 */

#ifndef _WINDOW_H
#define _WINDOW_H

#include "Constants.h"
#include "BaseWindow.h"
#include "Interpreter.h"


using namespace std;


class Interpreter;
//use template class BaseWindow
class Window : public BaseWindow<Window>
{

public:
//------------------------------------------------------------------------------
//Variables
	Interpreter interpreter;

//------------------------------------------------------------------------------
//Methods
	
	//get
	string getTransferFile();


	//send variables to interpreter
	void sendTestMode();
	void sendParity();
	void sendStopBits();
	void sendTransfer();
	void sendProtocol();
	void sendSelectedMasterPort();
	void sendSelectedSlavePort();
	void sendPortBaudRate();
	void sendTransferFile();
	void sendTextToSend();
	void sendLoggerState();
	void sendTransTextMode(int iTransTextMode);

	void sendTestSettings();

	void sethInstance(HINSTANCE hInst);

	string getFilePath();
	
    PCWSTR  ClassName() const { return L"WN COM Test Tool"; }
    
	//user definied message handler. NOT static
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
//------------------------------------------------------------------------------
//Variables

	int _iError;
	int _iError2;
	int _iTextToTransfer;

	HINSTANCE _hInst;
	UINT _status;
	int _ret;
	int _iTestMode;
	int _iParity;
	int _iStopBits;
	int _iTransfer;
	int _iProtocol;
	int _iBaudrate;
	int _iBaudrateMax;
	bool _bLoggerState;
	string _sMasPort;
	string _sSlaPort;
	string _sTransferFilePath;
	string _sTempBaud;
	char _szTextToSend[31];
	string _sTemp;
	int _i;

	//Window handles
	HWND _hwndCB_MasPorts;
	HWND _hwndCB_SlvPorts;

	HWND _hwndCB_Baud;
	HWND _hwndCB_Baud_MAX;
	HWND _hwnd_lbLoad;
	HWND _hwnd_btnLoad;
	HWND _hwnd_lbText;
	HWND _hwnd_btnText;
	HWND _hwnd_Start;
	HWND _hwnd_Close;
	HWND _hwnd_Save;

	//void CreateMyTooltip (HWND hwnd);
	//HWND CreateToolTip(int toolID, HWND hDlg, PTSTR pszText);
	
};

#endif