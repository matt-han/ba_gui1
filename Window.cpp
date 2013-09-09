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



//------------------------------------------------------------------------------
//	own window procedure, to avoid the static problem
//	Parameters:
//	 IN:
//		- UINT uMsg -> window message
//		- WPARAM wParam -> windows default parameter
//		- LPARAM lParam -> windows default parameter
//	Return: converted string
//------------------------------------------------------------------------------
LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT portIndex = 0;
	LRESULT baudIndex = -1;
	switch(uMsg)  
	{
		case WM_CREATE:

			//set all values to default
			_iTestMode = DEFAULT_VALUE;
			_iTransfer = DEFAULT_VALUE;
			_iParity   = ODDPARITY;
			_iStopBits = ONESTOPBIT;
			_iDataBits = 8;
			_iProtocol = 2;
			_iBaudrate = DEFAULT_VALUE;
			_iBaudrateMax = DEFAULT_VALUE;
			

			_iTextToTransfer = DEFAULT_VALUE;

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
							POS_X + 250, POS_Y, 110, 130,
							m_hwnd, 
							(HMENU) ID_GB_TRANSFER,
							NULL, NULL);

			CreateWindowA("button", "Shorted",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y + 20, 90, 30,
							m_hwnd,
							(HMENU)ID_MOD_SINGLE,
							NULL, NULL);

			CreateWindowA("button", "Double",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y + 45, 90, 30,
							m_hwnd,
							(HMENU)ID_MOD_DOUBLE,
							NULL, NULL);

			CreateWindowA("button", "Master",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260,POS_Y + 70, 90, 30,
							m_hwnd,
							(HMENU)ID_MOD_MASTER,
							NULL, NULL);

			CreateWindowA("button", "Slave",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y + 95, 90, 30,
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

			CheckDlgButton(m_hwnd, ID_PAR_ODD, BST_CHECKED);

//==============================================================================
//Protocol
			CreateWindowA("button", "Protocol",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 120, POS_Y2, 120, 110,
							m_hwnd, 
							(HMENU) ID_GB_PROTOCOL,
							NULL, NULL);
			CreateWindowA("button", "XON/XOFF",
							WS_GROUP | WS_CHILD|
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y2 + 20, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_XON_OFF,
							NULL, NULL);
			CreateWindowA("button", "Hardware",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y2 + 45, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_HARDWARE,
							NULL, NULL);
			CreateWindowA("button", "None",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 130, POS_Y2 + 70, 100, 30,
							m_hwnd,
							(HMENU)ID_PRO_NONE,
							NULL, NULL);

			CheckDlgButton(m_hwnd, ID_PRO_NONE, BST_CHECKED);

//==============================================================================
//Stopbits
			CreateWindowA("button", "StopBits",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 250, POS_Y2, 110, 80,
							m_hwnd, 
							(HMENU) ID_GB_STOPBITS,
							NULL, NULL);
			CreateWindowA("button", "1",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y2 + 20, 50, 30,
							m_hwnd,
							(HMENU)ID_SB_ONE,
							NULL, NULL);

			CreateWindowA("button", "2",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y2 + 45, 50, 30,
							m_hwnd,
							(HMENU)ID_SB_TWO,
							NULL, NULL);

			CheckDlgButton(m_hwnd, ID_SB_ONE, BST_CHECKED);

//==============================================================================
//Databits
			CreateWindowA("button", "Databits",
							WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
							POS_X + 250, POS_Y2 + 90, 110, 80,
							m_hwnd, 
							(HMENU) ID_GB_DATABITS,
							NULL, NULL);
			CreateWindowA("button", "7",
							WS_GROUP | WS_CHILD |
							WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y2 + 110, 50, 30,
							m_hwnd,
							(HMENU)ID_DB_7,
							NULL, NULL);

			CreateWindowA("button", "8",
							WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
							POS_X + 260, POS_Y2 + 135, 50, 30,
							m_hwnd,
							(HMENU)ID_DB_8,
							NULL, NULL);

			CheckDlgButton(m_hwnd, ID_DB_8, BST_CHECKED);

//==============================================================================
//Logger
			CreateWindowA("button", "Logger",
					WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
					POS_X + 260, POS_Y2 + 180, 100, 35, m_hwnd,
					(HMENU) ID_LOGGER, NULL, NULL);
			
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
			CreateWindowA("static", "Baud rate:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y + 50, 90, 15, m_hwnd,
				  (HMENU)ID_LB_BAUD, NULL, NULL);

//------------------------------------------------------------------------------		
//Min Baud
			CreateWindowA("static", "MIN baud rate:", 
                  WS_CHILD,
                  POS_X, POS_Y + 50, 120, 30, m_hwnd,
				  (HMENU)ID_LB_MIN, NULL, NULL);

//------------------------------------------------------------------------------		
//Slave Port
			CreateWindowA("static", "Slave Port\nfor Double Test:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y + 100, 120, 30, m_hwnd,
				  (HMENU)ID_LB_SLV, NULL, NULL);

//------------------------------------------------------------------------------		
//Max Baud
			CreateWindowA("static", "MAX baud rate:", 
                  WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y + 140, 120, 30, m_hwnd,
				  (HMENU)ID_LB_MAX, NULL, NULL);

//------------------------------------------------------------------------------
//Repeater
			_hwnd_Repeater = CreateWindowA("edit", "1", 
							  WS_VISIBLE | WS_CHILD | WS_BORDER,
							  POS_X + 260, POS_Y2 + 215, 50, 20, m_hwnd,
							  (HMENU)ID_LB_REPEATER, NULL, NULL);

			CreateWindowA("static", "Repeater", 
                  WS_VISIBLE | WS_CHILD,
                 POS_X + 315, POS_Y2 + 215, 60, 15, m_hwnd,
				  NULL, NULL, NULL);

//------------------------------------------------------------------------------
//Load file
			CreateWindowA("static", "Load file to be transfered:",
				WS_CHILD | WS_VISIBLE,
                  //POS_X, POSYYY + 285, 190, 15, m_hwnd, NULL, NULL, NULL);
					POS_X, POS_Y2 + 120, 190, 15, m_hwnd, NULL, NULL, NULL);

//load edit for file transfer path
			_hwnd_lbLoad = CreateWindowA("static", NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				POS_X, POS_Y2 + 140, 240, 20, m_hwnd, (HMENU)ID_LB_LOAD,
				NULL, NULL);
			//SendMessage(_hwnd_lbLoad, EM_SETLIMITTEXT, 256, 0);
			//SetWindowText(_hwnd_lbLoad, "Please type complete file path...");

//------------------------------------------------------------------------------			
//Send text
			CreateWindowA("static", "Send text:",
				WS_CHILD | WS_VISIBLE,
                  POS_X, POS_Y2 + 195, 190, 15, m_hwnd, NULL, NULL, NULL);

//load edit for file transfer path
			_hwnd_lbText = CreateWindowA("Edit", NULL,
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				POS_X, POS_Y2 + 215, 240, 20, m_hwnd, (HMENU)ID_LB_TEXT,
				NULL, NULL);

			SetWindowTextA(_hwnd_lbText, "...abc...");

//==============================================================================
//Combo boxes

			//Master port
			_hwndCB_MasPorts = CreateWindowA("combobox", NULL,
				WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y + 20, 90, 120, m_hwnd, (HMENU)ID_CB_COM_PORT,
				NULL, NULL);


			//BAUDRATE
			_hwndCB_Baud = CreateWindowA("combobox", NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X, POS_Y + 70, 90, 120, m_hwnd, (HMENU)ID_CB_COM_BAUD,
				NULL, NULL);

			//Slave port
			_hwndCB_SlvPorts = CreateWindowA("combobox", NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X + 120, POS_Y + 110, 90, 120, m_hwnd, (HMENU)ID_CB_SLV_PORT,
				NULL, NULL);
			
			//BAUDRATE
			_hwndCB_Baud_MAX = CreateWindowA("combobox", NULL, 
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | WS_VSCROLL,
                POS_X + 120, POS_Y + 140, 90, 120, m_hwnd, (HMENU)ID_CB_BAUD_MAX,
				NULL, NULL);


			//Fill master and slave port combo boxes
			//baudrate stays empty until user selects a port
			infoInterpreter.comEnumerator.enumeratePorts();

			for (vector<string>::iterator it =
				infoInterpreter.comEnumerator.vPortList.begin() ;
				 it != infoInterpreter.comEnumerator.vPortList.end(); ++it)
			{
				string s = *it;
				SendMessageA(_hwndCB_MasPorts, CB_ADDSTRING, 0, (LPARAM) s.c_str());
				SendMessageA(_hwndCB_SlvPorts, CB_ADDSTRING, 0, (LPARAM) s.c_str());
			}


//==============================================================================
//Buttons
			_hwnd_btnLoad = CreateWindowA("button", "Load file to send",
				WS_CHILD | WS_VISIBLE,
				POS_X, POS_Y2 + 165, 150, 20, m_hwnd, (HMENU)ID_BT_LOAD,
				NULL, NULL);

			_hwnd_btnText = CreateWindowA("button", "Load text to send",
				WS_CHILD | WS_VISIBLE,
				POS_X, POS_Y2 + 240, 150, 20, m_hwnd, (HMENU)ID_BT_TEXT,
				NULL, NULL);

			_hwnd_Start = CreateWindowA("button", "Start",
				WS_CHILD | WS_VISIBLE,
				POS_X+20, POS_Y2 + 290, 70, 30, m_hwnd, (HMENU)ID_BT_START,
				NULL, NULL);

			_hwnd_Close = CreateWindowA("button", "Close",
				WS_CHILD | WS_VISIBLE,
				POS_X + 100, POS_Y2 + 290, 70, 30, m_hwnd, (HMENU)ID_BT_CLOSE,
				NULL, NULL);

			_hwnd_Save = CreateWindowA("button", "Save",
				WS_CHILD | WS_VISIBLE,
				POS_X + 180, POS_Y2 + 290, 70, 30, m_hwnd, (HMENU)ID_BT_SAVE,
				NULL, NULL);

			_hwnd_LoadINI = CreateWindowA("button", "Load test file",
				WS_CHILD | WS_VISIBLE,
				POS_X + 260, POS_Y2 + 290, 100, 30, m_hwnd, (HMENU)ID_BT_LOADINI,
				NULL, NULL);

			_hwnd_Stop = CreateWindowA("button", "Stop",
				WS_CHILD | WS_VISIBLE,
				POS_X + 180, POS_Y2 + 325, 70, 30, m_hwnd, (HMENU)ID_BT_STOP,
				NULL, NULL);

			_hwnd_Help = CreateWindowA("button", "Help",
				WS_CHILD | WS_VISIBLE,
				POS_X + 100, POS_Y2 + 325, 70, 30, m_hwnd, (HMENU)ID_BT_HELP,
				NULL, NULL);

			//CreateMyTooltip(m_hwnd);

			//Disable views
			EnableWindow(GetDlgItem(m_hwnd, ID_LB_SLV), FALSE);
			EnableWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT), FALSE);

			EnableWindow(GetDlgItem(m_hwnd, ID_LB_MAX), FALSE);
			EnableWindow(GetDlgItem(m_hwnd, ID_CB_BAUD_MAX), FALSE);

			EnableWindow(_hwnd_Stop, FALSE);


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

						//Delete the baud rates when new port is chosen
						SendMessage(_hwndCB_Baud, CB_RESETCONTENT,0,0);
						SendMessage(_hwndCB_Baud_MAX, CB_RESETCONTENT,0,0);


						portIndex = SendMessage(_hwndCB_MasPorts,
												CB_GETCURSEL, 0, 0);
						_sMasPort = infoInterpreter.comEnumerator.vPortList.at(portIndex);
						
						//Get baud rate for chosen COM port
						_iError = infoInterpreter.comEnumerator.getBaudrates(_sMasPort);
						if (ERROR_SUCCESS == _iError)
						{
							string s;
							int i = 0;
							for (vector<string>::iterator it =
								infoInterpreter.comEnumerator.vBaud.begin() ;
								it != infoInterpreter.comEnumerator.vBaud.end();
								++it)
							{
								s = *it;

								//update the baud rate combo boxes
								_iError = SendMessageA(_hwndCB_Baud, CB_ADDSTRING,
													0, (LPARAM)s.c_str() );

								_iError2 = SendMessageA(_hwndCB_Baud_MAX, CB_ADDSTRING,
													0, (LPARAM)s.c_str() );

								if (_iError == CB_ERR || _iError2 == CB_ERR)
								{
									MessageBoxA(NULL, "Error updating baud rates in\n"
													  "combobox. System Error CB_ERR",
													  "ERROR", MB_OK | MB_ICONWARNING);
									/*clog << "Error updating baud rates in"
										 <<	"combo box. System Error CB_ERR"
										 << endl;*/
								}
								else if (_iError == CB_ERRSPACE || _iError2 == CB_ERRSPACE)
								{
									MessageBoxA(NULL, "Error, not enough space in combo box.\n"
													  "System Error CB_ERRSPACE",
													  "ERROR", MB_OK | MB_ICONWARNING);
									//clog << "not enough space in combo box."
									//	 <<	"System Error CB_ERRSPACE"
									//	 << endl;
								}
								else if (s == "9600")	//set default baud rate
								{
									SendMessage(_hwndCB_Baud, CB_SETCURSEL, i,0);
									//get the baud rate string
									_sTempBaud = "9600";
									//translate it to the system constant
									_iBaudrate =  infoInterpreter.comEnumerator.
												translateBaudrate(_sTempBaud);
								}
								i++;
							}

							//Check if baud rates were written in the combo boxes
							_iError = SendMessageA(_hwndCB_Baud,
												CB_GETCOUNT, 0, 0);
							if (_iError == 0)
								MessageBoxA(NULL, "Error updating the baud rate combo box",
													  "ERROR", MB_OK | MB_ICONWARNING);

							_iError2 = SendMessageA(_hwndCB_Baud_MAX,
												CB_GETCOUNT, 0, 0);
							if (_iError2 == 0)
							{
								MessageBoxA(NULL, "Error updating the max baud rate combo box",
													  "ERROR", MB_OK | MB_ICONWARNING);
							}


						}
						else
						{
							return _iError;
						}
						break;

//------------------------------------------------------------------------------
//Baudrate
					case ID_CB_COM_BAUD:

						//get the index
						baudIndex = SendMessage(_hwndCB_Baud,
												CB_GETCURSEL, 0, 0);
						//get the baud rate string
						_sTempBaud = infoInterpreter.comEnumerator.vBaud.at
									 (baudIndex);
						//translate it to the system constant
						_iBaudrate = infoInterpreter.comEnumerator.
									  translateBaudrate(_sTempBaud);

						break;
//------------------------------------------------------------------------------
//Max Baudrate
					case ID_CB_BAUD_MAX:
						//get the index
						portIndex = SendMessage(_hwndCB_Baud_MAX,
												CB_GETCURSEL, 0, 0);
						//get the baud rate string
						_sTempBaud = infoInterpreter.comEnumerator.vBaud.at
									 (portIndex);
						//translate it to the system constant
						_iBaudrateMax = infoInterpreter.comEnumerator.
									  translateBaudrate(_sTempBaud);
						break;
//------------------------------------------------------------------------------
//Slave port
					case ID_CB_SLV_PORT:
						portIndex = SendMessage(_hwndCB_SlvPorts,
												CB_GETCURSEL, 0, 0);
						_sSlaPort = infoInterpreter.comEnumerator.vPortList.at
									(portIndex);
				}

           }

//==============================================================================
// buttons _status		

			if (HIWORD(wParam) == BN_CLICKED)
			{
				switch (LOWORD(wParam))
				{

//------------------------------------------------------------------------------
//Logger				
					case ID_LOGGER:
						_status = IsDlgButtonChecked(m_hwnd, ID_LOGGER);

						//if checked
						if (BST_INDETERMINATE == _status)
							_bLoggerState = false;
						else if (BST_CHECKED == _status)
						{
							//then it was checked and set to false
							CheckDlgButton(m_hwnd, ID_LOGGER, BST_UNCHECKED);
							_bLoggerState = false;
						}
						else if (BST_UNCHECKED == _status)
						{
							//it was unchecked, so check it and set to true
							CheckDlgButton(m_hwnd, ID_LOGGER, BST_CHECKED);
							_bLoggerState = true;

						}
						break;

//------------------------------------------------------------------------------
//Test Mode
					case ID_TM_AUTO:

						//Hide min max labels and combobox
						EnableWindow(_hwndCB_Baud_MAX,              FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_MAX), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_CB_COM_BAUD), FALSE);
						ShowWindow(GetDlgItem(m_hwnd, ID_LB_MIN), SW_HIDE);
						
						//show baudrate label
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_BAUD), FALSE);

						//disable second row of test parameter
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_NONE), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_ODD),  FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_EVEN), FALSE);

						EnableWindow(GetDlgItem(m_hwnd, ID_SB_ONE), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_SB_TWO),	FALSE);

						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_XON_OFF),  FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_HARDWARE), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_NONE),	  FALSE);

						EnableWindow(GetDlgItem(m_hwnd, ID_DB_7), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_DB_8), FALSE);

						EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), FALSE);


						EnableWindow(_hwnd_Repeater, FALSE);
						EnableWindow(_hwnd_btnLoad,  FALSE);
						EnableWindow(_hwnd_btnText,  FALSE);

						_iTestMode = 0;
						break;
				
					case ID_TM_WOBB:
						//show min max labels and combobox
						EnableWindow(_hwndCB_Baud_MAX,              TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_MAX), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_CB_COM_BAUD), TRUE);
						ShowWindow(GetDlgItem(m_hwnd, ID_LB_MIN), SW_SHOWNORMAL);
						//hide baudrate label
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_BAUD), TRUE);

						//disable second row of test parameter
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_NONE), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_ODD),  TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_EVEN), TRUE);

						EnableWindow(GetDlgItem(m_hwnd, ID_SB_ONE), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_SB_TWO),	TRUE);

						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_XON_OFF),  TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_HARDWARE), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_NONE),	  TRUE);

						EnableWindow(GetDlgItem(m_hwnd, ID_DB_7), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_DB_8), TRUE);

						EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), TRUE);

						EnableWindow(_hwnd_Repeater, TRUE);
						EnableWindow(_hwnd_btnLoad,  TRUE);
						EnableWindow(_hwnd_btnText,  TRUE);

						ID_LB_MIN;

						_iTestMode = 1;
						break;

					case ID_TM_FIXED:
						//Hide min max labels and combobox
						EnableWindow(_hwndCB_Baud_MAX, FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_MAX), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_CB_COM_BAUD), TRUE);
						ShowWindow(GetDlgItem(m_hwnd, ID_LB_MIN), SW_HIDE);
						
						//show baudrate label
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_BAUD), TRUE);

						//disable second row of test parameter
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_NONE), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_ODD),  TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PAR_EVEN), TRUE);

						EnableWindow(GetDlgItem(m_hwnd, ID_SB_ONE), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_SB_TWO),	TRUE);

						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_XON_OFF),  TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_HARDWARE), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_PRO_NONE),	  TRUE);

						EnableWindow(GetDlgItem(m_hwnd, ID_DB_7), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_DB_8), TRUE);
						
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), TRUE);

						EnableWindow(_hwnd_Repeater, TRUE);
						EnableWindow(_hwnd_btnLoad,  TRUE);
						EnableWindow(_hwnd_btnText,  TRUE);

						
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

					case ID_PRO_HARDWARE:
						_iProtocol = 1;
						break;

					case ID_PRO_NONE:
						_iProtocol = 2;
						break;

//------------------------------------------------------------------------------
//Stopbits
					case ID_SB_ONE:
						_iStopBits = ONESTOPBIT;
						break;

					case ID_SB_TWO:
						_iStopBits = TWOSTOPBITS;
						break;


//------------------------------------------------------------------------------
//Databits
					case ID_DB_7:
						_iDataBits = 7;
						break;

					case ID_DB_8:
						_iDataBits = 8;
						break;
//------------------------------------------------------------------------------
//Transfer
					case ID_MOD_SINGLE:

						EnableWindow(GetDlgItem(m_hwnd, ID_LB_SLV), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT), FALSE);
						
						_iTransfer = 0;
						break;

					case ID_MOD_DOUBLE:
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_SLV), TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT),TRUE);

						_iTransfer = 1;
						break;
				
					case ID_MOD_MASTER:
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_SLV), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT), FALSE);

						_iTransfer = 2;
						break;

					case ID_MOD_SLAVE:
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_SLV), FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_CB_SLV_PORT), FALSE);

						_iTransfer = 3;
						break;
		
//------------------------------------------------------------------------------
//Buttons
					case ID_BT_START:

						EnableWindow(_hwnd_btnLoad, FALSE);
						EnableWindow(_hwnd_btnText, FALSE);
						EnableWindow(_hwnd_Start,   FALSE);
						EnableWindow(_hwnd_Close,   FALSE);
						EnableWindow(_hwnd_Save,    FALSE);
						EnableWindow(_hwnd_LoadINI, FALSE);
						EnableWindow(_hwnd_Help,    FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), FALSE);
						
						//enable stop button
						EnableWindow(_hwnd_Stop, TRUE);

						_t1 = thread(&Window::sendTestSettings, this);
						//detach the thread so it can test and main thread waits for it to finish
						//or wait for the user to press stop
						_t1.detach();

						//sendTestSettings();

						break;

					case ID_BT_CLOSE:
						_ret = MessageBoxA(NULL, "Are you sure to quit?", 
									  "Message", MB_OKCANCEL | MB_ICONSTOP);
						if ( _ret == IDOK)
							SendMessage(m_hwnd, WM_CLOSE, 0, 0);
						break;

					case ID_BT_LOAD:
						_sTransferFilePath = getFilePath();

						_i = _sTransferFilePath.find_last_of("\\")+1;
							_sTemp = _sTransferFilePath.substr(_i,
							_sTransferFilePath.npos - _i);

						SetWindowTextA(_hwnd_lbLoad, _sTemp.c_str());
						_iTextToTransfer = ID_BT_LOAD;

						break;

					case ID_BT_TEXT:
						ZeroMemory(_szTextToSend, sizeof(_szTextToSend));
						GetWindowTextA(_hwnd_lbText, _szTextToSend, 30);
						_iTextToTransfer = ID_BT_TEXT;
						MessageBoxA(NULL, "Text loaded", "", MB_OK);
						break;

					case ID_BT_SAVE:
						saveTestSettings();
						MessageBoxA(NULL, "....saved?....\ncheck return codes!!!", "", MB_OK);
						break;

					case ID_BT_LOADINI:
						//Disable Buttons
						EnableWindow(_hwnd_btnLoad, FALSE);
						EnableWindow(_hwnd_btnText, FALSE);
						EnableWindow(_hwnd_Start,   FALSE);
						EnableWindow(_hwnd_Close,   FALSE);
						EnableWindow(_hwnd_Save,    FALSE);
						EnableWindow(_hwnd_LoadINI, FALSE);
						EnableWindow(_hwnd_Help,    FALSE);
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), FALSE);

						_t1 = thread(&Window::loadTestSettings, this, "","");
						
						_t1.detach();

						//_iError = loadTestSettings("","");
						/*if(_iError != ERROR_SUCCESS)
						{
							_sTemp = "Error loading and testing from file: \n";
							_sTemp.append(_sPath.c_str());
							_sTemp.append("\nError : ");
							_sTemp.append(tools.convertToString(_iError));
							MessageBoxA(NULL, _sTemp.c_str()  , "ERROR", MB_OK);
							_sTemp = "";
						}
						else
						{
							MessageBoxA(NULL, "testing finished successfuly", "", MB_OK);
						}

						EnableWindow(_hwnd_btnLoad, TRUE);
						EnableWindow(_hwnd_btnText, TRUE);
						EnableWindow(_hwnd_Start,   TRUE);
						EnableWindow(_hwnd_Close,   TRUE);
						EnableWindow(_hwnd_Save,    TRUE);
						EnableWindow(_hwnd_LoadINI, TRUE);
						EnableWindow(_hwnd_Help,    TRUE);
						EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), TRUE);
*/
						break;

					case ID_BT_STOP:
						interpreter->stopTest();

						//pick the text to send in the new test
						//_iTextToTransfer = DEFAULT_VALUE;
						//SetWindowTextA(_hwnd_lbLoad,"");
						//SetWindowTextA(_hwnd_lbText,"");

						//EnableWindow(_hwnd_btnLoad, TRUE);
						//EnableWindow(_hwnd_btnText, TRUE);
						//EnableWindow(_hwnd_Start,   TRUE);
						//EnableWindow(_hwnd_Close,   TRUE);
						//EnableWindow(_hwnd_Save,    TRUE);
						//EnableWindow(_hwnd_LoadINI, TRUE);
						//EnableWindow(_hwnd_Help,    TRUE);
						//EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), TRUE);

						////disable stop button
						////maybe disable when sendtestsettings over(in function)?
						//EnableWindow(_hwnd_Stop, FALSE);


						break;




					case ID_BT_HELP:
						break;

						

				}//switch
			}//if
			break;

//==============================================================================
// CASE WN_DESTROY
//==============================================================================
    case WM_DESTROY:
        PostQuitMessage(0);
        return ERROR_SUCCESS;

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
	this->_hInst = hInst;
}


//------------------------------------------------------------------------------
//	Sends the user selected test mode
//------------------------------------------------------------------------------
void Window::sendTestMode()
{
	interpreter->setTestMode(_iTestMode);
}


//------------------------------------------------------------------------------
//	Sends the user selected parity setting
//------------------------------------------------------------------------------
void Window::sendParity()
{
	interpreter->setParity(_iParity);
}

//------------------------------------------------------------------------------
//	Sends the user selected stopbit setting
//------------------------------------------------------------------------------
void Window::sendStopBits()
{
	interpreter->setStopBits(_iStopBits);
}


//------------------------------------------------------------------------------
//	Sends the user selected stopbit setting
//------------------------------------------------------------------------------
void Window::sendDataBits()
{
	interpreter->setDataBits(_iDataBits);
}

//------------------------------------------------------------------------------
//	Sends the user selected transfer setting
//------------------------------------------------------------------------------
void Window::sendTransfer()
{
	interpreter->setTransfer(_iTransfer);
}


//------------------------------------------------------------------------------
//	Sends the user selected protocol setting
//------------------------------------------------------------------------------
void Window::sendProtocol()
{
	interpreter->setProtocol(_iProtocol);
}


//------------------------------------------------------------------------------
//	Sends the user selected master port
//------------------------------------------------------------------------------
void Window::sendSelectedMasterPort()
{
	interpreter->setSelectedMasterPort(_sMasPort);
}


//------------------------------------------------------------------------------
//	Sends the user selected slave port
//------------------------------------------------------------------------------
void Window::sendSelectedSlavePort()
{
	interpreter->setSelectedSlavePort(_sSlaPort);
}

//------------------------------------------------------------------------------
//	Sends the user selected baud rate
//------------------------------------------------------------------------------
void Window::sendPortBaudRate()
{
	interpreter->setPortBaudRate(_iBaudrate);
	interpreter->setPortBaudRateMax(_iBaudrateMax);
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
	interpreter->setTransferFile(this->_sTransferFilePath);
}


//------------------------------------------------------------------------------
//	Sends the user input string for testing
//------------------------------------------------------------------------------
void Window::sendTextToSend()
{
	//GetWindowTextA(_hwnd_lbText, _szTextToSend, 30);
	//clog << "\nText to send : " << _szTextToSend << endl;

	interpreter->setTextToSend(_szTextToSend);
}


//------------------------------------------------------------------------------
//	Sends the state of the log setting
//------------------------------------------------------------------------------
void Window::sendLoggerState()
{
	interpreter->setLoggerState(_bLoggerState);	
}


//------------------------------------------------------------------------------
//	Sends the text transfer mode setting
//------------------------------------------------------------------------------
void Window::sendTransTextMode(int iTransTextMode)
{
	interpreter->setTransTextMode(iTransTextMode);
}


//------------------------------------------------------------------------------
//	Sends the GUI input settings to the interpreter
//------------------------------------------------------------------------------
void Window::sendTestSettings()
{
	//lock_guard<mutex> locker(_mutex);
	
	interpreter = new Interpreter();

	switch(_iTextToTransfer)
	{
		//1 for file
		case ID_BT_LOAD:
			sendTransferFile();
			break;
			
		//2 for string
		case ID_BT_TEXT:
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
	sendDataBits();
	sendTransfer();
	sendProtocol();
	sendSelectedMasterPort();
	sendSelectedSlavePort();
	sendPortBaudRate();
	sendLoggerState();
	sendTransTextMode(_iTextToTransfer);
	sendRepeater();
	

	//interpreter->startT1();

	interpreter->handleGui();

	//pick the text to send in the new test
	_iTextToTransfer = DEFAULT_VALUE;
	SetWindowTextA(_hwnd_lbLoad,"");
	SetWindowTextA(_hwnd_lbText,"");

	EnableWindow(_hwnd_btnLoad, TRUE);
	EnableWindow(_hwnd_btnText, TRUE);
	EnableWindow(_hwnd_Start,   TRUE);
	EnableWindow(_hwnd_Close,   TRUE);
	EnableWindow(_hwnd_Save,    TRUE);
	EnableWindow(_hwnd_LoadINI, TRUE);
	EnableWindow(_hwnd_Help,    TRUE);
	EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), TRUE);

	//disable stop button
	//maybe disable when sendtestsettings over(in function)?
	EnableWindow(_hwnd_Stop, FALSE);


	delete interpreter;
	interpreter = NULL;

}

//------------------------------------------------------------------------------
//	Sends the GUI input repeater settings to the interpreter
//------------------------------------------------------------------------------
void Window::sendRepeater()
{
	GetWindowTextA(_hwnd_Repeater, _szRepeater, 5);
	interpreter->setRepeater(_szRepeater);
}

//------------------------------------------------------------------------------
//	saves the test settings to a INI file
//------------------------------------------------------------------------------
void Window::saveTestSettings()
{
	Interpreter interSaveToFile;

	interSaveToFile.setTestMode(_iTestMode);
	interSaveToFile.setParity(_iParity);
	interSaveToFile.setStopBits(_iStopBits);
	interSaveToFile.setDataBits(_iDataBits);
	interSaveToFile.setTransfer(_iTransfer);
	interSaveToFile.setProtocol(_iProtocol);
	interSaveToFile.setSelectedMasterPort(_sMasPort);
	interSaveToFile.setSelectedSlavePort(_sSlaPort);
	interSaveToFile.setPortBaudRate(_iBaudrate);
	interSaveToFile.setPortBaudRateMax(_iBaudrateMax);
	interSaveToFile.setLoggerState(_bLoggerState);
	interSaveToFile.setTransTextMode(_iTextToTransfer);
	
	GetWindowTextA(_hwnd_Repeater, _szRepeater, 5);
	interSaveToFile.setRepeater(_szRepeater);
	
	switch(_iTextToTransfer)
	{
		//1 for file
		case ID_BT_LOAD:
			interSaveToFile.setTransferFile(_sTransferFilePath);
			break;
			
		//2 for string
		case ID_BT_TEXT:
			interSaveToFile.setTextToSend(_szTextToSend);
			break;

		default:
			break;

	}

	_iError = interSaveToFile.saveToFile();
	if(_iError != ERROR_SUCCESS)
	{
		MessageBoxA(NULL, "Error saving to ini file", "ERROR", MB_OK);
	}
	

}


//------------------------------------------------------------------------------
//	Calls loadinifile and loads the file test settings and starts testing
//	Parameters:
//	 IN:
//		- string sFilePath -> file path
//		- string sPort -> port to test
//Return: if file is empty error
//------------------------------------------------------------------------------


void Window::loadTestSettings(string sFilePath, string sPort)
{
	
	interpreter = new Interpreter();

	if (sFilePath == "")
	{
		_sPath = getFilePath();
	}
	else
		_sPath = sFilePath;


	if (_sPath != "")
	{
		_iError = interpreter->loadIniFile(_sPath, sPort);
		//if(_iError != ERROR_SUCCES)
		//delete interpreter;
		//return _iError;
	}
	else
	{
		//delete interpreter;
		_iError = ERROR_EMPTY_FILE;
	}

	delete interpreter;

	if(_iError != ERROR_SUCCESS)
	{
		_sTemp = "Error loading and testing from file: \n";
		_sTemp.append(_sPath.c_str());
		_sTemp.append("\nError : ");
		_sTemp.append(tools.convertToString(_iError));
		MessageBoxA(NULL, _sTemp.c_str()  , "ERROR", MB_OK);
		_sTemp = "";
	}
	else
	{
		MessageBoxA(NULL, "testing finished successfuly", "", MB_OK);
	}

	EnableWindow(_hwnd_btnLoad, TRUE);
	EnableWindow(_hwnd_btnText, TRUE);
	EnableWindow(_hwnd_Start,   TRUE);
	EnableWindow(_hwnd_Close,   TRUE);
	EnableWindow(_hwnd_Save,    TRUE);
	EnableWindow(_hwnd_LoadINI, TRUE);
	EnableWindow(_hwnd_Help,    TRUE);
	EnableWindow(GetDlgItem(m_hwnd, ID_LB_TEXT), TRUE);

}


//------------------------------------------------------------------------------
//	Opens a dialog window for the user to select a INI file
//Return: file path selected by the user
//------------------------------------------------------------------------------
string Window::getFilePath()
{
	//GetSaveFileName()

	OPENFILENAMEA ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name

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
	
		
//		hf = CreateFileA(ofn.lpstrFile, 
//						GENERIC_READ,
//						0,
//						(LPSECURITY_ATTRIBUTES) NULL,
//						OPEN_EXISTING,
//						FILE_ATTRIBUTE_NORMAL,
//						(HANDLE) NULL);
//
	//lpstrFile contains filepath
	return ofn.lpstrFile;
}



//------------------------------------------------------------------------------
//	Set the path for the test file to be used
//	Return: string with the file path
//------------------------------------------------------------------------------
//string Window::getTransferFile()
//{
//	int iLen = GetWindowTextLengthW(_hwnd_lbLoad);
//	char* pstrText;
//	pstrText = (char*) malloc (sizeof(char)*iLen+1);
//
//	if(pstrText == NULL)
//	{
//		_iError = GetLastError();
//		clog << "Error allocating memory. Error : " << _iError << endl;
//		return "-1";
//	}
//
//	if (0 == GetWindowTextA(_hwnd_lbLoad, pstrText, iLen) )
//	{
//		_iError = GetLastError();
//		clog << "Error getting path for transfer file. Error: " <<
//				_iError << endl;
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