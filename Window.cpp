/*
 * Source
 *http://msdn.microsoft.com/en-us/library/windows/desktop/ff381400%28v=vs.85%29.aspx
 */



#include "Window.h"


//ORIGINAL
//
//LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//    switch (uMsg)
//    {
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//
//    case WM_PAINT:
//        {
//            PAINTSTRUCT ps;
//            HDC hdc = BeginPaint(m_hwnd, &ps);
//            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
//            EndPaint(m_hwnd, &ps);
//        }
//        return 0;
//
//    default:
//        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
//    }
//    return TRUE;
//}

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT portIndex = 0;

	switch(uMsg)  
	{
		case WM_CREATE:


//LOGGER
//================================================================================

			CreateWindowW(L"button", L"Logger",
			  WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
			  POS_X, POS_Y + 310, 185, 35, m_hwnd, (HMENU) ID_LOGGER, 
			  NULL, NULL);

			CheckDlgButton(m_hwnd, ID_LOGGER, BST_CHECKED);


//TEST MODE
//================================================================================

			CreateWindowW(L"button", L"Test Mode",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y - 130, 120, 110,
							m_hwnd, 
							(HMENU) ID_GB_TESTMODE,
							NULL, NULL);

			CreateWindowW(L"button", L"Auto",
							WS_GROUP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y - 105, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_AUTO,
							NULL, NULL);

			CreateWindowW(L"button", L"Wobble",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y - 80, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_WOBB,
							NULL, NULL);

			CreateWindowW(L"button", L"Fixed",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y - 55, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_FIXED,
							NULL, NULL);



//PARITY
//================================================================================

			CreateWindowW(L"button", L"Parity",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X, POS_Y, 110, 110,
							m_hwnd, 
							(HMENU) ID_GB_PARITY,
							NULL, NULL);

			CreateWindowW(L"button", L"None",
							WS_GROUP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y + 20, 60, 30,
							m_hwnd,
							(HMENU)ID_PAR_NONE,
							NULL, NULL);

			CreateWindowW(L"button", L"Odd",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y + 45, 60, 30,
							m_hwnd,
							(HMENU)ID_PAR_ODD,
							NULL, NULL);

			CreateWindowW(L"button", L"Even",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y + 70, 60, 30,
							m_hwnd,
							(HMENU)ID_PAR_EVEN,
							NULL, NULL);


//PROTOCOL
//================================================================================

			CreateWindowW(L"button", L"Protocol",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y, 120, 110,
							m_hwnd, 
							(HMENU) ID_GB_TRANSFER,
							NULL, NULL);
			CreateWindowW(L"button", L"XON/XOFF",
							WS_GROUP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 20, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_XON_OFF,
							NULL, NULL);
			CreateWindowW(L"button", L"DTR/DSR",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 45, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_DTR_DSR,
							NULL, NULL);
			CreateWindowW(L"button", L"CTS/RTS",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 70, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_CTS_RTS,
							NULL, NULL);


//STOPBITS
//================================================================================

			CreateWindowW(L"button", L"StopBits",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X, POS_Y + 115, 110, 110,
							m_hwnd, 
							(HMENU) ID_GB_STOPBITS,
							NULL, NULL);
			CreateWindowW(L"button", L" 1",
							WS_GROUP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y + 135, 50, 30,
							m_hwnd,
							(HMENU)ID_SB_ONE,
							NULL, NULL);
			CreateWindowW(L"button", L" 1.5",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y + 160, 50, 30,
							m_hwnd,
							(HMENU)ID_SB_ONE5,
							NULL, NULL);
			CreateWindowW(L"button", L" 2",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y + 185, 50, 30,
							m_hwnd,
							(HMENU)ID_SB_TWO,
							NULL, NULL);


//TRANSFER
//================================================================================

			CreateWindowW(L"button", L"Transfer",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y + 115, 120, 110,
							m_hwnd, 
							(HMENU) ID_GB_TRANSFER,
							NULL, NULL);

			CreateWindowW(L"button", L"Single",
							WS_GROUP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 135, 100, 30,
							m_hwnd,
							(HMENU)ID_MOD_SINGLE,
							NULL, NULL);

			CreateWindowW(L"button", L"Double",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 160, 100, 30,
							m_hwnd,
							(HMENU)ID_MOD_DOUBLE,
							NULL, NULL);

			CreateWindowW(L"button", L"MasterSlave",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 185, 100, 30,
							m_hwnd,
							(HMENU)ID_MOD_MS,
							NULL, NULL);

//LABELS
//================================================================================

			//COM Port
			CreateWindowW(L"static", L"COM Port:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y - 130, 90, 15, m_hwnd, NULL, NULL, NULL);
			
			//Baudrate
			CreateWindowW(L"static", L"Baudrate:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y - 80, 90, 15, m_hwnd, NULL, NULL, NULL);
			
			//Load file
			CreateWindowW(L"static", L"Load file to be transfered:",
				WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y + 240, 190, 15, m_hwnd, NULL, NULL, NULL);

			//load edit for file transfer path
			hwnd_Load = CreateWindowW(L"Edit", NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				POS_X, POS_Y + 260, 240, 20, m_hwnd, (HMENU)ID_LB_LOAD,
				NULL, NULL);

			SetWindowText(hwnd_Load, L"Please type complete file path...");

			//debug
			hDebug = CreateWindowW(L"static", L"",
				WS_CHILD | WS_VISIBLE,
                500, 400, 90, 25, m_hwnd, NULL,
				NULL, NULL);
//COMBOBOX
//================================================================================

			//COM PORTS
			hwndCB_Ports = CreateWindowW(L"combobox", NULL,
				WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y - 110, 90, 120, m_hwnd, (HMENU)ID_CB_COM_PORT,
				NULL, NULL);

			//BAUDRATE
			hwndCB_Baud = CreateWindowW(L"combobox", NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y - 60, 90, 120, m_hwnd, (HMENU)ID_CB_COM_BAUD,
				NULL, NULL);

			//Fill combo boxes
			//baudrate empty until user selects a port!!!!!!!!!!!!

			comEnumerator.enumeratePorts();

			for (vector<string>::iterator it = comEnumerator.vPortList.begin() ; it != comEnumerator.vPortList.end(); ++it)
			{
				string s = *it;
				SendMessageA(hwndCB_Ports, CB_ADDSTRING, 0, (LPARAM) s.c_str() );
			}


			//for ( i = 0; i < comEnumerator.vPortList.size(); i++ ) {
			//	string s = comEnumerator.vPortList(1);
			//	SendMessage(hwndCB_Ports, CB_ADDSTRING, 0, (LPARAM) s.c_str() );//interpreter.getComPort(i));
			//	SendMessage(hwndCB_Baud, CB_ADDSTRING, 0, (LPARAM) interpreter.getBaudrate(i));
   //         }
			


//BUTTONS
//================================================================================

			//hwnd_Load = CreateWindowW(L"button", L"Load file and start",
			//	WS_CHILD | WS_VISIBLE,
			//	POS_X, POS_Y + 290, 150, 20, m_hwnd, (HMENU)ID_BT_LOAD,
			//	NULL, NULL);

			hwnd_Start = CreateWindowW(L"button", L"Start",
				WS_CHILD | WS_VISIBLE,
				POS_X+20, POS_Y + 350, 70, 30, m_hwnd, (HMENU)ID_BT_START,
				NULL, NULL);

			hwnd_Close = CreateWindowW(L"button", L"Close",
				WS_CHILD | WS_VISIBLE,
				POS_X + 140, POS_Y + 350, 70, 30, m_hwnd, (HMENU)ID_BT_CLOSE,
				NULL, NULL);



			//CreateMyTooltip(m_hwnd);


			break; //WN_CREATE


//================================================================================
// CASE COMMAND
//================================================================================

		case WM_COMMAND:

//Update comboBox to selected item
//================================================================================
			if ( HIWORD(wParam) == CBN_SELCHANGE) {
				switch(LOWORD(wParam))
				{
					case ID_CB_COM_PORT:
						portIndex = SendMessage(hwndCB_Ports, CB_GETCURSEL, 0, 0);
						_sPort = comEnumerator.vPortList.at(portIndex);
						SetWindowTextA(hDebug, _sPort.c_str() );
						break;
						//update baudrate

					case ID_CB_COM_BAUD:
						portIndex = SendMessage(hwndCB_Baud, CB_GETCURSEL, 0, 0);
						//_sBaudrate = get from list in device manager
						break;
				}

           }

//radio buttons status
//================================================================================
			
			if (HIWORD(wParam) == BN_CLICKED)
			{
				switch (LOWORD(wParam))
				{
//LOGGER
//--------------------------------------------------------------------------------
					case ID_LOGGER:
						status = IsDlgButtonChecked(m_hwnd, ID_LOGGER);

						//if checked
						if (BST_INDETERMINATE == status)
							_bLoggerState = false;
						else if (BST_CHECKED == status)
						{
							//then it was checked and set to false
							CheckDlgButton(m_hwnd, ID_LOGGER, BST_UNCHECKED);
							SetWindowTextW(hDebug,L"false");
							_bLoggerState = false;
						}
						else if (BST_UNCHECKED == status)
						{
							//it was unchecked, so check it and set to true
							CheckDlgButton(m_hwnd, ID_LOGGER, BST_CHECKED);
							SetWindowTextW(hDebug,L"true");
							_bLoggerState = true;

						}
						break;

//Test Mode
//--------------------------------------------------------------------------------
					case ID_TM_AUTO:
						SetWindowTextW(hDebug,L"auto");
						_iTestMode = 0;
						break;
				
					case ID_TM_WOBB:
						SetWindowTextW(hDebug, L"wobbeln");
						_iTestMode = 1;
						break;

					case ID_TM_FIXED:
						SetWindowTextW(hDebug, L"fixed");
						_iTestMode = 2;
						break;

//Parity
//--------------------------------------------------------------------------------
					case ID_PAR_NONE:
						SetWindowTextW(hDebug,L"NONE");
						_iParity = NOPARITY;
						break;
				
					case ID_PAR_ODD:
						SetWindowTextW(hDebug, L"ODD");
						_iParity = ODDPARITY;
						break;

					case ID_PAR_EVEN:
						SetWindowTextW(hDebug, L"EVEN");
						_iParity = EVENPARITY;
						break;

				
//Protocol
//--------------------------------------------------------------------------------
					case ID_PRO_XON_OFF:
						SetWindowTextW(hDebug, L"XON_OFF");
						_iProtocol = 0;
						break;

					case ID_PRO_DTR_DSR:
						SetWindowTextW(hDebug, L"DTR_DSR");
						_iProtocol = 1;
						break;

					case ID_PRO_CTS_RTS:
						SetWindowTextW(hDebug, L"CTS_RTS");
						_iProtocol = 2;
						break;


//Stopbits
//--------------------------------------------------------------------------------
					case ID_SB_ONE:
						SetWindowTextW(hDebug, L"ONE");
						_iStopBits = ONESTOPBIT;
						break;

					case ID_SB_ONE5:
						SetWindowTextW(hDebug, L"ONE.5");
						_iStopBits = ONE5STOPBITS;
						break;

					case ID_SB_TWO:
						SetWindowTextW(hDebug, L"TWO");
						_iStopBits = TWOSTOPBITS;
						break;


//Transfer
//--------------------------------------------------------------------------------
					case ID_MOD_SINGLE:
						SetWindowTextW(hDebug, L"Single");
					
						break;

					case ID_MOD_DOUBLE:
						SetWindowTextW(hDebug, L"Double");
					
						break;
				
					case ID_MOD_MS:
						SetWindowTextW(hDebug, L"Master Slave");
					
						break;


//Buttons
//--------------------------------------------------------------------------------

					case ID_BT_START:
						SetWindowTextW(hDebug, L"Start");
						_sTransferFile = getTransferFile();
						//interpreter->handleGui();

						
						break;

					case ID_BT_CLOSE:
						ret = MessageBoxW(NULL, L"Are you sure to quit?", 
									  L"Message", MB_OKCANCEL);
						if ( ret == IDOK) {
							SendMessage(m_hwnd, WM_CLOSE, 0, 0);
						}
						break;

					case ID_BT_LOAD:
						SetWindowTextW(hDebug, L"load");
						MessageBoxW(NULL, L"Loading file and starting to be done", 
									  L"Message", MB_OK);
						break;


//Labels
//--------------------------------------------------------------------------------


				}//switch
			}//if
			break;


    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}

void Window::sethInstance(HINSTANCE hInst)
{
	this->hInst = hInst;
}



int Window::getTestMode()
{
	return this->_iTestMode;
}

int Window::getParity()
{
	return this->_iParity;
}

int Window::getStopBits()
{
	return this->_iStopBits;
}

int Window::getTransfer()
{
	return this->_iTransfer;
}

int Window::getProtocol()
{
	return this->_iProtocol;
}

string Window::getSelectedPort()
{
	return this->_sPort;
}

string Window::getTransferFile()
{
	setTransferFile();
	return _sTransferFile;

}

void Window::setTransferFile()
{
	int iLen = GetWindowTextLengthW(hwnd_Load);
	char* pstrText;
	pstrText = (char*) malloc (sizeof(char)*iLen+1);

	if(pstrText == NULL)
	{
		error = GetLastError();
		clog << "Error allocating memory. Error : " << error << endl;
	}

	if (0 == GetWindowTextA(hwnd_Load, pstrText, iLen) )
	{
		error = GetLastError();
		clog << "Error getting path for transfer file. Error : " << error << endl;
	}

	_sTransferFile = tools.convertToString(pstrText);

	free(pstrText);
	pstrText = NULL;


}
bool Window::getLoggerState()
{
	return this->_bLoggerState;
}


//TOOLTIP TRYOUTS
//
//
//================================================================================
//http://msdn.microsoft.com/en-us/library/windows/desktop/hh298368%28v=vs.85%29.aspx
// Description:
//   Creates a tooltip for an item in a dialog box. 
// Parameters:
//   idTool - identifier of an dialog box item.
//   nDlg - window handle of the dialog box.
//   pszText - string to use as the tooltip text.
// Returns:
//   The handle to the tooltip.
//
//HWND Window::CreateToolTip(int toolID, HWND hDlg, PTSTR pszText)
//{
//    if (!toolID || !hDlg || !pszText)
//    {
//        return FALSE;
//    }
//    // Get the window of the tool.
//    HWND hwndTool = GetDlgItem(hDlg, toolID);
//    
//    // Create the tooltip. g_hInst is the global instance handle.
//    HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
//                              WS_POPUP |TTS_ALWAYSTIP | TTS_BALLOON,
//                              CW_USEDEFAULT, CW_USEDEFAULT,
//                              CW_USEDEFAULT, CW_USEDEFAULT,
//                              hDlg, NULL, 
//                              this->hInst , NULL);
//    
//   if (!hwndTool || !hwndTip)
//   {
//       return (HWND)NULL;
//   }                              
//                              
//    // Associate the tooltip with the tool.
//    TOOLINFO toolInfo = { 0 };
//    toolInfo.cbSize = sizeof(toolInfo);
//    toolInfo.hwnd = hDlg;
//    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
//    toolInfo.uId = (UINT_PTR)hwndTool;
//    toolInfo.lpszText = pszText;
//
//    SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
//	SendMessage(hwndTip, TTM_ACTIVATE, true, NULL);
//
//    return hwndTip;
//}

//void Window::CreateMyTooltip (HWND hwndParent)
//{
//    INITCOMMONCONTROLSEX ic;
//	ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
//	ic.dwICC = ICC_TAB_CLASSES;
// 
//	InitCommonControlsEx(&ic);
//
//    // Create a ToolTip.
//    HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST,
//        TOOLTIPS_CLASS, NULL,
//        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,       
//        CW_USEDEFAULT, CW_USEDEFAULT,
//        CW_USEDEFAULT, CW_USEDEFAULT,
//        hwndParent, NULL, hInst,NULL);
//    SetWindowPos(hwndTT, HWND_TOPMOST,
//        0, 0, 0, 0,
//        SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
//    // Set up "tool" information.
//    // In this case, the "tool" is the entire parent window.
//    TOOLINFO ti = { 0 };
//    ti.cbSize = sizeof(TOOLINFO);
//    ti.uFlags = TTF_SUBCLASS;
//    ti.hwnd = hwndParent;
//    ti.hinst = hInst;
//    ti.lpszText = TEXT("This is your ToolTip string.");;
//    GetClientRect (hwndParent, &ti.rect);
//    // Associate the ToolTip with the "tool" window.
//    SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
//	SendMessage(hwndTT, TTM_ACTIVATE, true, NULL);




 //   INITCOMMONCONTROLSEX iccex; 
 //   HWND hwndTT;                

 //   TOOLINFO ti;
 //   RECT rect;                 
 // 
 //   iccex.dwICC = ICC_WIN95_CLASSES;
 //   iccex.dwSize = sizeof(INITCOMMONCONTROLSEX);
 //   InitCommonControlsEx(&iccex);

 //   hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
 //       WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,		
 //       0, 0, 0, 0, hwnd, NULL, NULL, NULL );

 //   SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
 //       SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
 //  
 //   GetClientRect (hwnd, &rect);

 //   ti.cbSize = sizeof(TOOLINFO);
 //   ti.uFlags = TTF_SUBCLASS;
 //   ti.hwnd = hwnd;
 //   ti.hinst = NULL;
 //   ti.uId = 0;
 //   ti.lpszText = L"A main window";;
 //   ti.rect.left = rect.left;    
 //   ti.rect.top = rect.top;
 //   ti.rect.right = rect.right;
 //   ti.rect.bottom = rect.bottom;

 //   SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	//SendMessage(hwndTT, TTM_ACTIVATE, true, NULL);
//}	