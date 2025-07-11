#ifndef __WINGUI_H__
#define __WINGUI_H__

// wingui global functions...
namespace wingui
{

// Library initialization and termination functions...
BOOL InitLibrary();
void TerminateLibrary();

BOOL SetClipboardText(LPCTSTR lpszText);

}; // wingui namespace

#endif // !__WINGUI_H__
