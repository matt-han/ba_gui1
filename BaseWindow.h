/*
 *	Header file with template for GUI. Example taken from:
 *  http://msdn.microsoft.com/en-us/library/windows/desktop/ff381400%28v=vs.85%29.aspx
 */


#ifndef _BASEWINDOW_H
#define _BASEWINDOW_H

#include <Windows.h>


//Window constants
#define WINDOW_X	150
#define WINDOW_Y	150
#define WIN_HEIGHT	600
#define WIN_WIDTH	420
#define POS_Y		10
#define POS_Y2		190
#define POS_X		20


template <class DERIVED_TYPE> 
class BaseWindow
{
public:
	//Windows default STATIC message handler
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE *pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			//extract pointer
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (pThis)
			return pThis->HandleMessage(uMsg, wParam, lParam);
		else
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}//WindowProc

    
	BaseWindow() : m_hwnd(NULL) { }

	BOOL Create(LPCSTR lpWindowName,
				DWORD dwStyle,
				DWORD dwExStyle = 0,
				int x = WINDOW_X,
				int y = WINDOW_Y,
				int nWidth = WIN_WIDTH,
				int nHeight = WIN_HEIGHT,
				HWND hWndParent = 0,
				HMENU hMenu = 0)
    {
		//Window properties
		WNDCLASS wc = {0};
		
		//wc.lpszClassName = L"Serial Port Tester";
		wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
		wc.hCursor       = LoadCursor(0, IDC_ARROW);
		wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
		wc.lpfnWndProc   = DERIVED_TYPE::WindowProc;
		wc.hInstance     = GetModuleHandle(NULL);
		wc.lpszClassName =  ClassName();

		RegisterClass(&wc);

		//main window creation and handle
		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this);

        return (m_hwnd ? TRUE : FALSE);
    }//create

	HWND window() const { return m_hwnd;}

protected:

    virtual LPCSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
};

#endif