/*
 *  Main window class
 *  Source
 *  http://msdn.microsoft.com/en-us/library/windows/desktop/
 *  ff381400%28v=vs.85%29.aspx
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

//==============================================================================
//Logger
			CreateWindowW(L"button", L"Logger",
			  WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
			  POS_X, POS_Y + 230, 185, 35, m_hwnd, (HMENU) ID_LOGGER, 
			  NULL, NULL);
			
			CheckDlgButton(m_hwnd, ID_LOGGER, BST_CHECKED);

//==============================================================================
//Test mode
			CreateWindowW(L"button", L"Test Mode",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y - 170, 120, 110,
							m_hwnd, 
							(HMENU) ID_GB_TESTMODE,
							NULL, NULL);

			CreateWindowW(L"button", L"Automatic",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y - 155, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_AUTO,
							NULL, NULL);

			CreateWindowW(L"button", L"Wobble",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y - 125, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_WOBB,
							NULL, NULL);

			CreateWindowW(L"button", L"Fixed",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y - 95, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_FIXED,
							NULL, NULL);

//==============================================================================
//Parity
			CreateWindowW(L"button", L"Parity",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X, POS_Y, 110, 110,
							m_hwnd, 
							(HMENU) ID_GB_PARITY,
							NULL, NULL);

			CreateWindowW(L"button", L"None",
							WS_GROUP | WS_CHILD
							| WS_VISIBLE | BS_AUTORADIOBUTTON,
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

//==============================================================================
//Protocol
			CreateWindowW(L"button", L"Protocol",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y, 120, 110,
							m_hwnd, 
							(HMENU) ID_GB_TRANSFER,
							NULL, NULL);
			CreateWindowW(L"button", L"XON/XOFF",
							WS_GROUP | WS_CHILD|
							WS_VISIBLE | BS_AUTORADIOBUTTON,
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

//==============================================================================
//Stopbits
			CreateWindowW(L"button", L"StopBits",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X, POS_Y + 115, 110, 110,
							m_hwnd, 
							(HMENU) ID_GB_STOPBITS,
							NULL, NULL);
			CreateWindowW(L"button", L" 1",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
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

//==============================================================================
//Transfer
			CreateWindowW(L"button", L"Transfer",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y + 115, 120, 110,
							m_hwnd, 
							(HMENU) ID_GB_TRANSFER,
							NULL, NULL);

			CreateWindowW(L"button", L"Single",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
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

//==============================================================================
//Labels
//------------------------------------------------------------------------------		
//Master Port
			CreateWindowW(L"static", L"Master Port:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y - 170, 90, 15, m_hwnd, NULL, NULL, NULL);
//------------------------------------------------------------------------------
//Baudrate
			CreateWindowW(L"static", L"Baudrate:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y - 120, 90, 15, m_hwnd, NULL, NULL, NULL);

//------------------------------------------------------------------------------		
//Slave Port
			CreateWindowW(L"static", L"Slave Port:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y - 70, 90, 15, m_hwnd, NULL, NULL, NULL);

//------------------------------------------------------------------------------
//Load file
			CreateWindowW(L"static", L"Load file to be transfered:",
				WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y + 265, 190, 15, m_hwnd, NULL, NULL, NULL);

//load edit for file transfer path
			hwnd_lbLoad = CreateWindowW(L"Edit", NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				POS_X, POS_Y + 285, 240, 20, m_hwnd, (HMENU)ID_LB_LOAD,
				NULL, NULL);
			SendMessage(hwnd_lbLoad, EM_SETLIMITTEXT, 256, 0);
			SetWindowText(hwnd_lbLoad, L"Please type complete file path...");

//------------------------------------------------------------------------------			
//Send text
			CreateWindowW(L"static", L"Send text:",
				WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y + 340, 190, 15, m_hwnd, NULL, NULL, NULL);

//load edit for file transfer path
			hwnd_lbText = CreateWindowW(L"Edit", NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				POS_X, POS_Y + 360, 240, 20, m_hwnd, (HMENU)ID_LB_TEXT,
				NULL, NULL);

			SetWindowText(hwnd_lbText, L"...abc...");
//------------------------------------------------------------------------------
			
			//debug
			hDebug = CreateWindowW(L"static", L"",
				WS_CHILD | WS_VISIBLE,
                500, 400, 90, 25, m_hwnd, NULL,
				NULL, NULL);

//==============================================================================
//Combo boxes

			//Master port
			hwndCB_MasPorts = CreateWindowW(L"combobox", NULL,
				WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y - 150, 90, 120, m_hwnd, (HMENU)ID_CB_COM_PORT,
				NULL, NULL);


			//BAUDRATE
			hwndCB_Baud = CreateWindowW(L"combobox", NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y - 100, 90, 120, m_hwnd, (HMENU)ID_CB_COM_BAUD,
				NULL, NULL);

			//Slave port
			hwndCB_SlvPorts = CreateWindowW(L"combobox", NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y - 50, 90, 120, m_hwnd, (HMENU)ID_CB_SLV_PORT,
				NULL, NULL);
			

			//Fill master and slave port combo boxes
			//baudrate stays empty until user selects a port
			comEnumerator.enumeratePorts();

			for (vector<string>::iterator it = comEnumerator.vPortList.begin() ;
				 it != comEnumerator.vPortList.end(); ++it)
			{
				string s = *it;
				SendMessageA(hwndCB_MasPorts, CB_ADDSTRING, 0, (LPARAM) s.c_str());
				SendMessageA(hwndCB_SlvPorts, CB_ADDSTRING, 0, (LPARAM) s.c_str());
			}
//==============================================================================
//Buttons
			hwnd_btnLoad = CreateWindowW(L"button", L"Load file and start",
				WS_CHILD | WS_VISIBLE,
				POS_X, POS_Y + 308, 150, 20, m_hwnd, (HMENU)ID_BT_LOAD,
				NULL, NULL);

			hwnd_btnText = CreateWindowW(L"button", L"Load text and start",
				WS_CHILD | WS_VISIBLE,
				POS_X, POS_Y + 383, 150, 20, m_hwnd, (HMENU)ID_BT_TEXT,
				NULL, NULL);

			hwnd_Start = CreateWindowW(L"button", L"Start",
				WS_CHILD | WS_VISIBLE,
				POS_X+20, POS_Y + 420, 70, 30, m_hwnd, (HMENU)ID_BT_START,
				NULL, NULL);

			hwnd_Close = CreateWindowW(L"button", L"Close",
				WS_CHILD | WS_VISIBLE,
				POS_X + 140, POS_Y + 420, 70, 30, m_hwnd, (HMENU)ID_BT_CLOSE,
				NULL, NULL);


			//CreateMyTooltip(m_hwnd);


			break; //WN_CREATE


//==============================================================================
// CASE COMMAND
//==============================================================================

		case WM_COMMAND:


//==============================================================================
//Update comboBox to selected item

			if ( HIWORD(wParam) == CBN_SELCHANGE) {
				switch(LOWORD(wParam))
				{
					
//------------------------------------------------------------------------------
//Master port
					case ID_CB_COM_PORT:
						portIndex = SendMessage(hwndCB_MasPorts,
												CB_GETCURSEL, 0, 0);
						_sMasPort = comEnumerator.vPortList.at(portIndex);
						SetWindowTextA(hDebug, _sMasPort.c_str() );
						
						//Get baud rate for chosen COM port
						error = comEnumerator.getBaudrates(_sMasPort);
						if (ERROR_SUCCESS == error)
						{
							string s;

							for (vector<string>::iterator it =
								comEnumerator.vBaud.begin() ;
								it != comEnumerator.vBaud.end(); ++it)
							{
								s = *it;
								error = SendMessageA(hwndCB_Baud, CB_ADDSTRING,
													0, (LPARAM)s.c_str() );

								if (error == CB_ERR)
								{
									clog << "error updating baud rates in" <<
										"combobox. System error CB_ERR" << endl;
								}
								else if ( error == CB_ERRSPACE)
								{
									clog << "not enough space in combo box."<<
										"System error CB_ERRSPACE" << endl;
								}

							}


							error = SendMessageA(hwndCB_Baud,
												CB_GETCOUNT, 0, 0);
							if (error == 0)
								clog << "error updating the combo box" << endl;
						}
						else
						{
							return error;
						}
						break;

//------------------------------------------------------------------------------
//Baudrate
					case ID_CB_COM_BAUD:
						//get the index
						portIndex = SendMessage(hwndCB_Baud,
												CB_GETCURSEL, 0, 0);
						//get the baud rate string
						_sTempBaud = comEnumerator.vBaud.at(portIndex);
						//translate it to the system constant
						_dwBaudrate = comEnumerator.translateBaudrate(
																	_sTempBaud);

						SetWindowTextA(hDebug, _sTempBaud.c_str() );
						break;


					case ID_CB_SLV_PORT:
						portIndex = SendMessage(hwndCB_SlvPorts,
												CB_GETCURSEL, 0, 0);
						_sSlaPort = comEnumerator.vPortList.at(portIndex);
						SetWindowTextA(hDebug, _sSlaPort.c_str() );
				}

           }

//==============================================================================
// buttons status		

			if (HIWORD(wParam) == BN_CLICKED)
			{
				switch (LOWORD(wParam))
				{

//------------------------------------------------------------------------------
//Logger				
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

//------------------------------------------------------------------------------
//Test Mode
					case ID_TM_AUTO:
						SetWindowTextW(hDebug,L"automatic");
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

//------------------------------------------------------------------------------
//Parity
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

//------------------------------------------------------------------------------
//Protocol
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

//------------------------------------------------------------------------------
//Stopbits
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

//------------------------------------------------------------------------------
//Transfer
					case ID_MOD_SINGLE:
						SetWindowTextW(hDebug, L"Single");
						_iTransfer = 0;
						break;

					case ID_MOD_DOUBLE:
						SetWindowTextW(hDebug, L"Double");
						_iTransfer = 1;
						break;
				
					case ID_MOD_MS:
						SetWindowTextW(hDebug, L"Master Slave");
						_iTransfer = 2;
						break;

//------------------------------------------------------------------------------
//Buttons
					case ID_BT_START:
						SetWindowTextW(hDebug, L"Start");
						
						sendTestSettings(0);
						
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
						sendTestSettings(1);
						break;

					case ID_BT_TEXT:
						SetWindowTextW(hDebug, L"load");
						sendTestSettings(2);

				}//switch
			}//if
			break;

//==============================================================================
// CASE WN_DESTROY
//==============================================================================
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    }
    return TRUE;
}


//------------------------------------------------------------------------------
//	Set window handle
//	Parameters:
//	 IN:
//		- HINSTANCE hInst -> handle to the main window
//------------------------------------------------------------------------------
void Window::sethInstance(HINSTANCE hInst)
{
	this->hInst = hInst;
}


//------------------------------------------------------------------------------
//	Sends the user selected test mode
//------------------------------------------------------------------------------
void Window::sendTestMode()
{
	interpreter.setTestMode(_iTestMode);
}


//------------------------------------------------------------------------------
//	Sends the user selected parity setting
//------------------------------------------------------------------------------
void Window::sendParity()
{
	interpreter.setParity(_iParity);
}

//------------------------------------------------------------------------------
//	Sends the user selected stopbit setting
//------------------------------------------------------------------------------
void Window::sendStopBits()
{
	interpreter.setStopBits(_iStopBits);
}


//------------------------------------------------------------------------------
//	Sends the user selected transfer setting
//------------------------------------------------------------------------------
void Window::sendTransfer()
{
	interpreter.setTransfer(_iTransfer);
}


//------------------------------------------------------------------------------
//	Sends the user selected protocol setting
//------------------------------------------------------------------------------
void Window::sendProtocol()
{
	interpreter.setProtocol(_iProtocol);
}


//------------------------------------------------------------------------------
//	Sends the user selected master port
//------------------------------------------------------------------------------
void Window::sendSelectedMasterPort()
{
	interpreter.setSelectedMasterPort(_sMasPort);
}


//------------------------------------------------------------------------------
//	Sends the user selected slave port
//------------------------------------------------------------------------------
void Window::sendSelectedSlavePort()
{
	interpreter.setSelectedSlavePort(_sSlaPort);
}

//------------------------------------------------------------------------------
//	Sends the user selected baud rate
//------------------------------------------------------------------------------
void Window::sendPortBaudRate()
{
	interpreter.setPortBaudRate(_dwBaudrate);
}


//------------------------------------------------------------------------------
//	Sends the user selected transfer file for testing
//------------------------------------------------------------------------------
void Window::sendTransferFile()
{
	interpreter.setTransferFile(this->getTransferFile());
}


//------------------------------------------------------------------------------
//	Sends the user input string for testing
//------------------------------------------------------------------------------
void Window::sendTextToSend()
{
	GetWindowTextA(hwnd_lbText, _szTextToSend, 30);
	clog << "Text to send : " << _szTextToSend << endl;
	interpreter.setTextToSend(_szTextToSend);
}


//------------------------------------------------------------------------------
//	Sends the state of the log setting
//------------------------------------------------------------------------------
void Window::sendLoggerState()
{
	interpreter.setLoggerState(_bLoggerState);	
}

//------------------------------------------------------------------------------
//	Sends the GUI input settings to the interpreter
//------------------------------------------------------------------------------
void Window::sendTestSettings(int iTransferText)
{
	switch(iTransferText)
	{
		//1 for file
		case 1:
			sendTransferFile();
			break;
			
		//2 for string
		case 2:
			sendTextToSend();
			break;

		default:
			break;

	}
	sendTestMode();
	sendParity();
	sendStopBits();
	sendTransfer();
	sendProtocol();
	sendSelectedMasterPort();
	sendSelectedSlavePort();
	sendPortBaudRate();
	sendLoggerState();

	interpreter.handleGui();
}


//------------------------------------------------------------------------------
//	Set the path for the test file to be used
//	Return: string with the file path
//------------------------------------------------------------------------------
string Window::getTransferFile()
{
	int iLen = GetWindowTextLengthW(hwnd_lbLoad);
	char* pstrText;
	pstrText = (char*) malloc (sizeof(char)*iLen+1);

	if(pstrText == NULL)
	{
		error = GetLastError();
		clog << "Error allocating memory. Error : " << error << endl;
		return "-1";
	}

	if (0 == GetWindowTextA(hwnd_lbLoad, pstrText, iLen) )
	{
		error = GetLastError();
		clog << "Error getting path for transfer file. Error: " <<
				error << endl;
		return "-1";
	}

	_sTransferFilePath = tools.convertToString(pstrText);

	free(pstrText);
	pstrText = NULL;
	
	return _sTransferFilePath;
}
//
//

//TOOLTIP TRYOUTS
//
//
//==============================================================================
//http://msdn.microsoft.com/en-us/library/windows/desktop/
//		hh298368%28v=vs.85%29.aspx
//
//
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