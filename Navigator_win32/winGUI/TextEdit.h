#ifndef __WINGUI_TEXTBOX_H__
#define __WINGUI_TEXTBOX_H__

#include "view.h"

namespace wingui {

class TextEdit : public wingui::View
{
public:
	virtual ~TextEdit();

	wingui_tstring GetText();
	void SetText(LPCTSTR lpszText);
	wingui_tstring GetLineText(int line);
	void SetLineText(int line, LPCTSTR lpszText);
	void AppendText(LPCTSTR lpszText);
	wingui_tstring GetSelText();
	void SetSelText(LPCTSTR lpszText);
	void Clear();
	
	void SetMaxLength(int maxLength);
	int GetMaxLength();
	int GetLineCount();
	void GetLineInfo(int line, int &start, int &length) const;
	int GetLineTextLength(int line) const;

	BOOL IsReadonly();
	void SetReadonly(BOOL flag = TRUE);
	BOOL IsMultiline();
	void SetMultiline(BOOL flag = TRUE);

	void EnableScrollbars(BOOL horzScroll, BOOL vertScroll);
	void SetMargins (int left, int right);		
	void MoveCursorToStart();
	void MoveCursorToEnd();
	void SelectAll();
	void SelectNone();
	void SelectText(int start, int length);
	void SelectLine(int line);
	void GetSelection(int &start, int &length);
	void CopyToClipboard(BOOL copyAll = TRUE);

protected:
	// overrides...	
	virtual void GetCreateStyle (DWORD &dwStyle, DWORD &dwExStyle);
	virtual void GetClassStyle (WNDCLASSEX &wcex);
};

}; // wingui namespace

#endif // !__WINGUI_TEXTBOX_H__
