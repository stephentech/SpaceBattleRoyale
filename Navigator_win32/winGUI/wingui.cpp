#include "stdafx.h"
#include "resource.h"
#include <wingui/wingui.h>
#include <wingui/globals.h>
#include <wingui/Notification.h>

namespace wingui
{

// externs
HCURSOR gCurHSplit;
HCURSOR gCurVSplit;
Font gPanelBarTitleFont;

BOOL InitLibrary()
{
	gCurHSplit = gCurVSplit = NULL;

	HINSTANCE hAppInst = (HINSTANCE)GetModuleHandle(NULL);

	// Load cursors.
	gCurHSplit = LoadCursor(hAppInst, MAKEINTRESOURCE(IDC_WINGUI_CURSOR_HSPLIT));
	if (!gCurHSplit) return FALSE;
	gCurVSplit = LoadCursor(hAppInst, MAKEINTRESOURCE(IDC_WINGUI_CURSOR_VSPLIT));
	if (!gCurVSplit) return FALSE;

	// Create shared fonts.
	HDC hdc = CreateCompatibleDC (NULL);
	gPanelBarTitleFont.Create(_T("Tahoma"), 8, hdc);
	DeleteDC(hdc);

	// Create global singleton objects.
	gNotifier = new Notifier();

	return TRUE;
}

void TerminateLibrary()
{
	if (gCurHSplit) {
		DestroyCursor(gCurHSplit);
	}
	if (gCurVSplit) {
		DestroyCursor(gCurVSplit);
	}
	gPanelBarTitleFont.Delete();
}

BOOL SetClipboardText(LPCTSTR lpszText)
{
	size_t size = _tcslen(lpszText) + 1;
	size*= sizeof(TCHAR);
	
	HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, size);
	if (hMem == NULL) {
		return FALSE;
	}
	
	LPSTR lpszBuffer = (LPSTR)GlobalLock(hMem);
	_tcscpy(lpszBuffer, lpszText);
	GlobalUnlock(hMem);

	if (!OpenClipboard(NULL))
	{
		GlobalFree(hMem);
		return FALSE;
	}

	if (!::EmptyClipboard())
	{
		::CloseClipboard();
		GlobalFree(hMem);
		return FALSE;
	}

#if defined(UNICODE) || defined(_UNICODE)
	::SetClipboardData(CF_UNICODETEXT, hMem);
#else
	::SetClipboardData(CF_TEXT, hMem);
#endif

	::CloseClipboard();
	return TRUE;	
}

}; // wingui namespace
