// Navigator_win32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "main.h"
#include "EngineGUI\EngWindow.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// main HInstance
HINSTANCE _hAppInstance;

// handle to the main Win32 App
//HWND hMainWnd;

// handle to the game draw area App
//HWND hGameDisplayWnd;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	_hAppInstance = hInstance;
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	
	EngWindow& engWindow = EngWindow::GetInstance();
	engWindow.Create(0, 0, X_SAFEREGION_MAX * 2, Y_SAFEREGION_MAX * 1.2f, szTitle);
	engWindow.Show(nCmdShow);

	engWindow.Init();

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		engWindow.Update();
	}

	// no point calling these.
	engWindow.Hide();
	engWindow.Destroy();

	return (int) msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_LBUTTONDOWN:
		{
			/*if(x == -1 || y == -1)
			{
				x = LOWORD(lParam);
				y = HIWORD(lParam);
			}
			else
			{
				int x_ = LOWORD(lParam);
				int y_ = HIWORD(lParam);
				PathNavigation::GetInstance()->CreteNewPath( CVector(x,y), CVector(x_, y_));

				x = y = -1;
			}*/
			//gGame.Fire(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_RBUTTONDOWN:
		{
			//PathNavigation::GetInstance()->DestroyAllPaths();
		}
		break;

	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);
		//Game::GetInstance().Draw(hdc, &ps);
		//EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
