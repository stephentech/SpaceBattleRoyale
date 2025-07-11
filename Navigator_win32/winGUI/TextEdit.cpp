#include "stdafx.h"
#include <wingui/TextEdit.h>

namespace wingui {

TextEdit::~TextEdit()
{
}

wingui_tstring TextEdit::GetText()
{
	wingui_tstring str;
	int length = ::GetWindowTextLength(GetHandle());
	if (length > 0) {
		TCHAR *pBuffer = new TCHAR[length + 1];
		::GetWindowText(GetHandle(), pBuffer, length + 1);
		str = pBuffer;
		delete [] pBuffer;
	}
	return str;
}

void TextEdit::SetText(LPCTSTR lpszText)
{
	WINGUI_ASSERT(lpszText != NULL);
	::SetWindowText(GetHandle(), lpszText);
}

wingui_tstring TextEdit::GetLineText(int line)
{
	// for a multiline textbox line must be 0
	WINGUI_ASSERT((IsMultiline() && line >= 0) || (line == 0));

	// get length of text in the line
	int length = GetLineTextLength(line);
	TCHAR *pBuffer = new TCHAR[length + 1];
	
	// first WORD in the buffer must be set to the length of the string
	*((WORD *)pBuffer) = length;
	
	// get line text
	int count = SendMessage(GetHandle(), EM_GETLINE, (WPARAM)line, (LPARAM)pBuffer);

	// EM_GETLINE does not null terminate the string so do it for ourselves
	pBuffer[count] = _T('\0');

	// copy the text into a string and delete the buffer
	wingui_tstring retstr(pBuffer);
	delete []pBuffer;
	return retstr;
}

void TextEdit::SetLineText(int line, LPCTSTR lpszText)
{
	// get start character index and length of text in the line
	int start, length;
	GetLineInfo(line, start, length);
	// select the line
	SelectText(start, length);
	// set selection text
	SetSelText(lpszText);
}

void TextEdit::AppendText(LPCTSTR lpszText)
{
	MoveCursorToEnd();
	SetSelText(lpszText);
}

wingui_tstring TextEdit::GetSelText()
{
	WINGUI_ASSERT(NULL); // not implemented
	return wingui_tstring(_T(""));
}

void TextEdit::SetSelText(LPCTSTR lpszText)
{
	::SendMessage(GetHandle(), EM_REPLACESEL, (WPARAM)FALSE, (LPARAM) lpszText);
}

void TextEdit::Clear()
{
	::SetWindowText(GetHandle(), NULL);
}

void TextEdit::SetMaxLength(int maxLength)
{
	// if maxLength is zero, the text length is set to maximum supported by the control
	WINGUI_ASSERT(maxLength >= 0);
	::SendMessage(GetHandle(), EM_LIMITTEXT, (WPARAM)maxLength, 0);
}

int TextEdit::GetMaxLength()
{
	return (int)::SendMessage(GetHandle(), EM_GETLIMITTEXT, 0, 0);
}

int TextEdit::GetLineCount()
{
	return (int)::SendMessage(GetHandle(), EM_GETLINECOUNT, 0, 0);
}

void TextEdit::GetLineInfo(int line, int &start, int &length) const
{
	start = (int)::SendMessage(GetHandle(), EM_LINEINDEX, (WPARAM)line, 0);
	length = (int)::SendMessage(GetHandle(), EM_LINELENGTH, (WPARAM)start, 0);
}

int TextEdit::GetLineTextLength(int line) const
{
	int start, length;
	GetLineInfo(line, start, length);
	return length;
}

BOOL TextEdit::IsReadonly()
{
	return (::GetWindowLong(GetHandle(),GWL_STYLE) & ES_READONLY) ? TRUE : FALSE;
}

void TextEdit::SetReadonly(BOOL flag)
{
	::SendMessage(GetHandle(), EM_SETREADONLY, (WPARAM)flag, 0);
}

BOOL TextEdit::IsMultiline()
{
	return (::GetWindowLong(GetHandle(),GWL_STYLE) & ES_MULTILINE) ? TRUE : FALSE;
}

void TextEdit::SetMultiline(BOOL flag)
{
	LONG dwLong = ::GetWindowLong(GetHandle(), GWL_STYLE);
	if (flag) {
		dwLong|= ES_MULTILINE;
	} else {
		dwLong&= ~ES_MULTILINE;
	}
	::SetWindowLong(GetHandle(), GWL_STYLE, dwLong);
}

void TextEdit::EnableScrollbars(BOOL horzScroll, BOOL vertScroll)
{
	LONG dwLong = ::GetWindowLong(GetHandle(), GWL_STYLE);
	if (horzScroll) {
		dwLong|= WS_HSCROLL;
	} else {
		dwLong&= ~WS_HSCROLL;
	}
	if (vertScroll) {
		dwLong|= WS_VSCROLL;
	} else {
		dwLong&= ~WS_VSCROLL;
	}
	::SetWindowLong(GetHandle(), GWL_STYLE, dwLong);
}

void TextEdit::SetMargins (int left, int right)
{
	WPARAM flags = 0;

	if (left >= 0) {
		flags|= EC_LEFTMARGIN;
	} else {
		flags&= ~EC_LEFTMARGIN;
	}

	if (right >= 0) {
		flags|= EC_RIGHTMARGIN;
	} else {
		flags&= ~EC_RIGHTMARGIN;
	}

	::SendMessage(GetHandle(), EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKEWORD(left,right));
}

void TextEdit::MoveCursorToStart()
{
	::SendMessage(GetHandle(), EM_SETSEL, 0, 0);
}

void TextEdit::MoveCursorToEnd()
{
	int length = ::GetWindowTextLength(GetHandle());
	::SendMessage(GetHandle(), EM_SETSEL, length, length);
}

void TextEdit::SelectAll()
{
	int length = ::GetWindowTextLength(GetHandle());
	::SendMessage(GetHandle(), EM_SETSEL, 0, length);
}

void TextEdit::SelectNone()
{
	// remove selection without moving the cursor
	int start, length;
	GetSelection(start, length);
	::SendMessage(GetHandle(), EM_SETSEL, start, start);
}

void TextEdit::SelectText(int start, int length)
{
	::SendMessage(GetHandle(), EM_SETSEL, start, start + length);
}

void TextEdit::SelectLine(int line)
{
	// get start character index and length of text in the line
	int start, length;
	GetLineInfo(line, start, length);

	// select the line
	SelectText(start, length);
}

void TextEdit::GetSelection(int &start, int &length)
{
	::SendMessage(GetHandle(), EM_GETSEL, (WPARAM)&start, (LPARAM)&length);
	length = length - start;
}

void TextEdit::CopyToClipboard(BOOL copyAll)
{
	if (copyAll)
	{
		// open system clipboard to copy entire text
		if (!::OpenClipboard(NULL) || !EmptyClipboard()) {
			return;
		}

		HGLOBAL hGlobalMemory;
		LPTSTR lpszTextToCopy;

		// allocate memory for text
		int length = GetWindowTextLength(GetHandle());
		size_t memSize = (length + 1) * sizeof(TCHAR);
		hGlobalMemory = GlobalAlloc(GMEM_MOVEABLE, memSize);
		if (!hGlobalMemory) {
			::CloseClipboard();
			return;
		}

		// get text into buffer
		lpszTextToCopy = (LPTSTR)GlobalLock(hGlobalMemory);
		GetWindowText (GetHandle(), lpszTextToCopy, length + 1);
		lpszTextToCopy [length] = _T('\0');
		GlobalUnlock (hGlobalMemory);

		// set text into clipboard
		#if defined(UNICODE) || defined(_UNICODE)
			::SetClipboardData(CF_UNICODETEXT, hGlobalMemory);
		#else
			::SetClipboardData(CF_TEXT, hGlobalMemory);
		#endif

		// close clipboard when done
		::CloseClipboard();
	}
	else
	{
		// use WM_COPY message to copy the selected text
		SendMessage (GetHandle(), WM_COPY, 0, 0);
	}
}

// overrides...
void TextEdit::GetCreateStyle (DWORD &dwStyle, DWORD &dwExStyle)
{
	View::GetCreateStyle (dwStyle, dwExStyle);
}

void TextEdit::GetClassStyle (WNDCLASSEX &wcex)
{
	wcex.lpszClassName = WC_EDIT;
}

}; // wingui namespace
