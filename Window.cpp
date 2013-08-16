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

			//set all values to default
			_iTestMode = DEFAULT_VALUE;
			_iParity   = DEFAULT_VALUE;
			_iStopBits = DEFAULT_VALUE;
			_iTransfer = DEFAULT_VALUE;
			_iProtocol = DEFAULT_VALUE;
			_iBaudrate = DEFAULT_VALUE;

			_bLoggerState		= true;
			_sMasPort			="";
			_sSlaPort			="";
			_sTransferFilePath	="";
			_sTempBaud			="";


//==============================================================================
//Test mode
			CreateWindowA("button", "Test Mode",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y, 120, 105,
							m_hwnd, 
							(HMENU) ID_GB_TESTMODE,
							NULL, NULL);

			CreateWindowA("button", "Automatic",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 20, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_AUTO,
							NULL, NULL);

			CreateWindowA("button", "Wobble",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 45, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_WOBB,
							NULL, NULL);

			CreateWindowA("button", "Fixed",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y + 70, 100, 30,
							m_hwnd,
							(HMENU)ID_TM_FIXED,
							NULL, NULL);
//==============================================================================
//Transfer
			CreateWindowA("button", "Transfer",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 250, POS_Y, 120, 130,
							m_hwnd, 
							(HMENU) ID_GB_TRANSFER,
							NULL, NULL);

			CreateWindowA("button", "Shorted",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y + 20, 100, 30,
							m_hwnd,
							(HMENU)ID_MOD_SINGLE,
							NULL, NULL);

			CreateWindowA("button", "Double",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y + 45, 100, 30,
							m_hwnd,
							(HMENU)ID_MOD_DOUBLE,
							NULL, NULL);

			CreateWindowA("button", "Master",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260,POS_Y + 70, 100, 30,
							m_hwnd,
							(HMENU)ID_MOD_MASTER,
							NULL, NULL);

			CreateWindowA("button", "Slave",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y + 95, 100, 30,
							m_hwnd,
							(HMENU)ID_MOD_SLAVE,
							NULL, NULL);

//==============================================================================
//Parity
			CreateWindowA("button", "Parity",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X, POS_Y2, 110, 110,
							m_hwnd, 
							(HMENU) ID_GB_PARITY,
							NULL, NULL);

			CreateWindowA("button", "None",
							WS_GROUP | WS_CHILD
							| WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y2 + 20, 60, 30,
							m_hwnd,
							(HMENU)ID_PAR_NONE,
							NULL, NULL);

			CreateWindowA("button", "Odd",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y2 + 45, 60, 30,
							m_hwnd,
							(HMENU)ID_PAR_ODD,
							NULL, NULL);

			CreateWindowA("button", "Even",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 10, POS_Y2 + 70, 60, 30,
							m_hwnd,
							(HMENU)ID_PAR_EVEN,
							NULL, NULL);

//==============================================================================
//Protocol
			CreateWindowA("button", "Protocol",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y2, 120, 110,
							m_hwnd, 
							(HMENU) ID_GB_TRANSFER,
							NULL, NULL);
			CreateWindowA("button", "XON/XOFF",
							WS_GROUP | WS_CHILD|
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y2 + 20, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_XON_OFF,
							NULL, NULL);
			CreateWindowA("button", "DTR/DSR",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y2 + 45, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_DTR_DSR,
							NULL, NULL);
			CreateWindowA("button", "CTS/RTS",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y2 + 70, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_CTS_RTS,
							NULL, NULL);

//==============================================================================
//Stopbits
			CreateWindowA("button", "StopBits",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 250, POS_Y2, 110, 80,
							m_hwnd, 
							(HMENU) ID_GB_STOPBITS,
							NULL, NULL);
			CreateWindowA("button", " 1",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y2 + 20, 50, 30,
							m_hwnd,
							(HMENU)ID_SB_ONE,
							NULL, NULL);
			//CreateWindowA("button", " 1.5",
			//				WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			//				POS_X + 10, POSYYY + 160, 50, 30,
			//				m_hwnd,
			//				(HMENU)ID_SB_ONE5,
			//				NULL, NULL);
			CreateWindowA("button", " 2",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y2 + 45, 50, 30,
							m_hwnd,
							(HMENU)ID_SB_TWO,
							NULL, NULL);

//==============================================================================
//Logger
			CreateWindowA("button", "Logger",
			  WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
			  POS_X + 260, POS_Y2 + 80, 185, 35, m_hwnd, (HMENU) ID_LOGGER, 
			  NULL, NULL);
			
			CheckDlgButton(m_hwnd, ID_LOGGER, BST_CHECKED);

//==============================================================================
//Labels
//------------------------------------------------------------------------------		
//Master Port
			CreateWindowA("static", "Main Port:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y, 90, 15, m_hwnd, NULL, NULL, NULL);
//------------------------------------------------------------------------------
//Baudrate
			CreateWindowA("static", "Baudrate:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y + 50, 90, 15, m_hwnd, NULL, NULL, NULL);

//------------------------------------------------------------------------------		
//Slave Port
			CreateWindowA("static", "Slave Port\nfor Double Test:", 
                  WS_CHILD,
                  POS_X, POS_Y + 100, 120, 30, m_hwnd, (HMENU)ID_LB_SLV, NULL, NULL);

//------------------------------------------------------------------------------
//Load file
			CreateWindowA("static", "Load file to be transfered:",
				WS_CHILD | WS_VISIBLE,
                  //POS_X, POSYYY + 285, 190, 15, m_hwnd, NULL, NULL, NULL);
					POS_X, POS_Y2 + 120, 190, 15, m_hwnd, NULL, NULL, NULL);

//load edit for file transfer path
			hwnd_lbLoad = CreateWindowA("static", NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				POS_X, POS_Y2 + 140, 240, 20, m_hwnd, (HMENU)ID_LB_LOAD,
				NULL, NULL);
			//SendMessage(hwnd_lbLoad, EM_SETLIMITTEXT, 256, 0);
			//SetWindowText(hwnd_lbLoad, "Please type complete file path...");

//------------------------------------------------------------------------------			
//Send text
			CreateWindowA("static", "Send text:",
				WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y2 + 195, 190, 15, m_hwnd, NULL, NULL, NULL);

//load edit for file transfer path
			hwnd_lbText = CreateWindowA("Edit", NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				POS_X, POS_Y2 + 215, 240, 20, m_hwnd, (HMENU)ID_LB_TEXT,
				NULL, NULL);

			SetWindowTextA(hwnd_lbText, "...abc...");
//------------------------------------------------------------------------------
			
			//debug
			hDebug = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE,
                500, 400, 90, 25, m_hwnd, NULL,
				NULL, NULL);

//==============================================================================
//Combo boxes

			//Master port
			hwndCB_MasPorts = CreateWindowA("combobox", NULL,
				WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y + 20, 90, 120, m_hwnd, (HMENU)ID_CB_COM_PORT,
				NULL, NULL);


			//BAUDRATE
			hwndCB_Baud = CreateWindowA("combobox", NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y + 70, 90, 120, m_hwnd, (HMENU)ID_CB_COM_BAUD,
				NULL, NULL);

			//Slave port
			hwndCB_SlvPorts = CreateWindowA("combobox", NULL, 
                WS_CHILD | CBS_DROPDOWN | WS_VSCROLL,
                POS_X + 120, POS_Y + 110, 90, 120, m_hwnd, (HMENU)ID_CB_SLV_PORT,
				NULL, NULL);
			

			//Fill master and slave port combo boxes
			//baudrate stays empty until user selects a port
			interpreter.comEnumerator.enumeratePorts();

			for (vector<string>::iterator it =
				interpreter.comEnumerator.vPortList.begin() ;
				 it != interpreter.comEnumerator.vPortList.end(); ++it)
			{
				string s = *it;
				SendMessageA(hwndCB_MasPorts, CB_ADDSTRING, 0, (LPARAM) s.c_str());
				SendMessageA(hwndCB_SlvPorts, CB_ADDSTRING, 0, (LPARAM) s.c_str());
			}


//==============================================================================
//Buttons
			hwnd_btnLoad = CreateWindowA("button", "Clic to load and start",
				WS_CHILD | WS_VISIBLE,
				POS_X, POS_Y2 + 165, 150, 20, m_hwnd, (HMENU)ID_BT_LOAD,
				NULL, NULL);

			hwnd_btnText = CreateWindowA("button", "Load text and start",
				WS_CHILD | WS_VISIBLE,
				POS_X, POS_Y2 + 240, 150, 20, m_hwnd, (HMENU)ID_BT_TEXT,
				NULL, NULL);

			hwnd_Start = CreateWindowA("button", "Start",
				WS_CHILD | WS_VISIBLE,
				POS_X+20, POS_Y2 + 290, 70, 30, m_hwnd, (HMENU)ID_BT_START,
				NULL, NULL);

			hwnd_Close = CreateWindowA("button", "Close",
				WS_CHILD | WS_VISIBLE,
				POS_X + 140, POS_Y2 + 290, 70, 30, m_hwnd, (HMENU)ID_BT_CLOSE,
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
						_sMasPort = interpreter.comEnumerator.vPortList.at(portIndex);
						SetWindowTextA(hDebug, _sMasPort.c_str() );
						
						//Get baud rate for chosen COM port
						error = interpreter.comEnumerator.getBaudrates(_sMasPort);
						if (ERROR_SUCCESS == error)
						{
							string s;

							for (vector<string>::iterator it =
								interpreter.comEnumerator.vBaud.begin() ;
								it != interpreter.comEnumerator.vBaud.end();
								++it)
							{
								s = *it;
								error = SendMessageA(hwndCB_Baud, CB_ADDSTRING,
													0, (LPARAM)s.c_str() );

								if (error == CB_ERR)
								{
									clog << "error updating baud rates in"
										 <<	"combobox. System error CB_ERR"
										 << endl;
								}
								else if ( error == CB_ERRSPACE)
								{
									clog << "not enough space in combo box."
										 <<	"System error CB_ERRSPACE"
										 << endl;
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
						_sTempBaud = interpreter.comEnumerator.vBaud.at
									 (portIndex);
						//translate it to the system constant
						_iBaudrate = interpreter.comEnumerator.
									  translateBaudrate(_sTempBaud);

						SetWindowTextA(hDebug, _sTempBaud.c_str() );
						break;

//------------------------------------------------------------------------------
//Slave port
					case ID_CB_SLV_PORT:
						portIndex = SendMessage(hwndCB_SlvPorts,
												CB_GETCURSEL, 0, 0);
						_sSlaPort = interpreter.comEnumerator.vPortList.at
									(portIndex);
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
							_bLoggerState = false;
						}
						else if (BST_UNCHECKED == status)
						{
							//it was unchecked, so check it and set to true
							CheckDlgButton(m_hwnd, ID_LOGGER, BST_CHECKED);
							_bLoggerState = true;

						}
						break;

//------------------------------------------------------------------------------
//Test Mode
					case ID_TM_AUTO:
						_iTestMode = 0;
						break;
				
					case ID_TM_WOBB:
						_iTestMode = 1;
						break;

					case ID_TM_FIXED:
						_iTestMode = 2;
						break;

//------------------------------------------------------------------------------
//Parity
					case ID_PAR_NONE:
						_iParity = NOPARITY;
						break;
				
					case ID_PAR_ODD:
						_iParity = ODDPARITY;
						break;

					case ID_PAR_EVEN:
						_iParity = EVENPARITY;
						break;

//------------------------------------------------------------------------------
//Protocol
					case ID_PRO_XON_OFF:
						_iProtocol = 0;
						break;

					case ID_PRO_DTR_DSR:
						_iProtocol = 1;
						break;

					case ID_PRO_CTS_RTS:
						_iProtocol = 2;
						break;

//------------------------------------------------------------------------------
//Stopbits
					case ID_SB_ONE:
						_iStopBits = ONESTOPBIT;
						break;

					//case ID_SB_ONE5:
					//	_iStopBits = ONE5STOPBITS;
					//	break;

					case ID_SB_TWO:
						_iStopBits = TWOSTOPBITS;
						break;

//------------------------------------------------------------------------------
//Transfer
					case ID_MOD_SINGLE:

						ShowWindow(GetDlgItem(m_hwnd, ID_LB_SLV), SW_HIDE);
						ShowWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT), SW_HIDE);
						
						_iTransfer = 0;
						break;

					case ID_MOD_DOUBLE:
						ShowWindow(GetDlgItem(m_hwnd, ID_LB_SLV), SW_SHOWNORMAL);
						ShowWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT), SW_SHOWNORMAL);

						_iTransfer = 1;
						break;
				
					case ID_MOD_MASTER:
						ShowWindow(GetDlgItem(m_hwnd, ID_LB_SLV), SW_HIDE);
						ShowWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT), SW_HIDE);

						_iTransfer = 2;
						break;

					case ID_MOD_SLAVE:
						ShowWindow(GetDlgItem(m_hwnd, ID_LB_SLV), SW_HIDE);
						ShowWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT), SW_HIDE);

						_iTransfer = 3;
						break;
		
//------------------------------------------------------------------------------
//Buttons
					case ID_BT_START:
						
						sendTestSettings(0);
						
						break;

					case ID_BT_CLOSE:
						ret = MessageBoxA(NULL, "Are you sure to quit?", 
									  "Message", MB_OKCANCEL);
						if ( ret == IDOK) {
							SendMessage(m_hwnd, WM_CLOSE, 0, 0);
						}
						break;

					case ID_BT_LOAD:
						_sTransferFilePath = getFilePath();

						_i = _sTransferFilePath.find_last_of("\\")+1;
						_sTemp = _sTransferFilePath.substr(_i,
							_sTransferFilePath.npos - _i);

						SetWindowTextA(hwnd_lbLoad, _sTemp.c_str());
						sendTestSettings(1);
						break;

					case ID_BT_TEXT:
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
	interpreter.setPortBaudRate(_iBaudrate);
}


//------------------------------------------------------------------------------
//	Sends the user selected transfer file for testing
//------------------------------------------------------------------------------
void Window::sendTransferFile()
{
	//interpreter.setTransferFile(this->getTransferFile());
	//int a = _sTransferFilePath.find_last_of("\\");
	//clog << "\nFile to send : "
	//	 << _sTransferFilePath.substr(a,_sTransferFilePath.npos - a)
	//	 << endl;
	interpreter.setTransferFile(this->_sTransferFilePath);
}


//------------------------------------------------------------------------------
//	Sends the user input string for testing
//------------------------------------------------------------------------------
void Window::sendTextToSend()
{
	GetWindowTextA(hwnd_lbText, _szTextToSend, 30);
	clog << "\nText to send : " << _szTextToSend << endl;
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
//	Sends the text transfer mode setting
//------------------------------------------------------------------------------
void Window::sendTransTextMode(int iTransTextMode)
{
	interpreter.setTransTextMode(iTransTextMode);
}


//------------------------------------------------------------------------------
//	Sends the GUI input settings to the interpreter
//------------------------------------------------------------------------------
void Window::sendTestSettings(int iTransTextMode)
{
	//iTransferText
	//0 for default
	//1 for file
	//2 for string
	switch(iTransTextMode)
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
	//send all input information to the interpreter to be decoded
	//interpreter calls the TestManager and sets the test up
	sendTestMode();
	sendParity();
	sendStopBits();
	sendTransfer();
	sendProtocol();
	sendSelectedMasterPort();
	sendSelectedSlavePort();
	sendPortBaudRate();
	sendLoggerState();
	sendTransTextMode(iTransTextMode);

	interpreter.handleGui();
}


//------------------------------------------------------------------------------
//	Set the path for the test file to be used
//	Return: string with the file path
//------------------------------------------------------------------------------
//string Window::getTransferFile()
//{
//	int iLen = GetWindowTextLengthW(hwnd_lbLoad);
//	char* pstrText;
//	pstrText = (char*) malloc (sizeof(char)*iLen+1);
//
//	if(pstrText == NULL)
//	{
//		error = GetLastError();
//		clog << "Error allocating memory. Error : " << error << endl;
//		return "-1";
//	}
//
//	if (0 == GetWindowTextA(hwnd_lbLoad, pstrText, iLen) )
//	{
//		error = GetLastError();
//		clog << "Error getting path for transfer file. Error: " <<
//				error << endl;
//		return "-1";
//	}
//
//	_sTransferFilePath = interpreter.tools.convertToString(pstrText);
//
//	free(pstrText);
//	pstrText = NULL;
//	
//	return _sTransferFilePath;
//}


//------------------------------------------------------------------------------
//	Open file winapi dialog box
//  http://msdn.microsoft.com/en-us/library/ms646829%28v=vs.85%29.aspx#open_file
//	Return: file path as string
//------------------------------------------------------------------------------
string Window::getFilePath()
{

	OPENFILENAMEA ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	//HWND hwnd;              // owner window
	//HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hwnd;
	ofn.lpstrFile = szFile;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileNameA(&ofn)==TRUE)
	{

	}
	//lpstrFile contains filepath
		
//		hf = CreateFileA(ofn.lpstrFile, 
//						GENERIC_READ,
//						0,
//						(LPSECURITY_ATTRIBUTES) NULL,
//						OPEN_EXISTING,
//						FILE_ATTRIBUTE_NORMAL,
//						(HANDLE) NULL);
//
	return ofn.lpstrFile;
}



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