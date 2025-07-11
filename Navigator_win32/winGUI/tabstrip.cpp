/*
 * tabstrip.cpp
 *
 * tabstrip control implementation file.
 */

#include "stdafx.h"
#include <wingui/Tabstrip.h>
#include <wingui/gdi.h>

namespace wingui {

#define TABSTRIP_BGCOLOR						RGB(48,66,97)
#define TABSTRIP_BDRCOLOR						RGB(200,200,200)

#define TABSTRIP_ITEM_BGCOLOR					RGB(70,90,125)
#define TABSTRIP_ITEM_BDRCOLOR					RGB(70,90,125)
#define TABSTRIP_ITEM_FGCOLOR					RGB(255,255,255)

#define TABSTRIP_ITEM_SEL_BGCOLOR				RGB(255,232,166)
#define TABSTRIP_ITEM_SEL_BDRCOLOR				RGB(255,232,166)
#define TABSTRIP_ITEM_SEL_FGCOLOR				RGB(99,0,38)


#define TABSTRIP_PADDING						6
#define TABSTRIP_INTERNAL_SPACING				1
#define TABSTRIP_ICON_SIZE_X					16
#define TABSTRIP_ICON_SIZE_Y					16
#define TABSTRIP_MARGIN_TOP						3
#define ROUND_CORNER_SIZE						4

// shared resources...
static int _hTabstripCount = 0;
static Font _hTabstripFont;
static Font _hTabstripBoldFont;

/*
 * class TabStrip
 */
TabStrip::TabStrip () : 
	_orientation (kOrientationHorizontal), 
	_selectedIndex(-1),
	_eventHandler(NULL)
{
	if (++_hTabstripCount == 1)
	{
		HDC hdc = ::CreateCompatibleDC (NULL);
		_hTabstripFont.Create (_T("MS Sans Serif"), 9, hdc);
		_hTabstripBoldFont.Create (_T("MS Sans Serif"), 9, hdc, FW_SEMIBOLD);
		DeleteDC (hdc);
	}
}

TabStrip::~TabStrip ()
{
	if (--_hTabstripCount == 0)
	{
		_hTabstripFont.Delete();
		_hTabstripBoldFont.Delete ();
	}
}

void TabStrip::GetClassStyle (WNDCLASSEX &wcex)
{
	wcex.lpszClassName = _T("_gls_tabstrip");
	wcex.cbSize = sizeof(wcex);
	wcex.style = 0;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hIcon = NULL;
	wcex.hIconSm = NULL;
	wcex.hCursor = LoadCursor (NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(TABSTRIP_BGCOLOR);  // (HBRUSH)COLOR_WINDOW;
	wcex.lpszMenuName = NULL;
	wcex.hInstance = (HINSTANCE)GetModuleHandle(NULL);
}

int TabStrip::HitTest (int x, int y)
{
	RECT rt;
	GetClientRect (rt);;

	POINT pt = { x, y };
	if ( !::PtInRect (&rt, pt) ) {
		return -1;
	}

	for (int i=0; i < _items.size(); i++) {
		GetTabStripItemRect (i, rt);
		if ( ::PtInRect (&rt, pt) ) {
			return i;
		}
	}

	return -1;
}

void TabStrip::SetSelectedTabIndex (int index)
{
	if (_selectedIndex == index) {
		return;	
	}

	if (_selectedIndex != -1) {
		Fire_OnDeselectTab (_selectedIndex);
	}

	_selectedIndex = index;

	CalcLayout();
	RedrawView ();

	if (_selectedIndex != -1) {
		Fire_OnSelectTab (_selectedIndex);
	}
}

void TabStrip::CalcLayout ()
{
	HDC hdc = GetDC(GetHandle());
	BeforeDraw (hdc);

	RECT rtText;
	::SetRect (&rtText, 0, 0, 0, 0);

	for (int i=0; i < _items.size(); i++)
	{
		TabStripItem &item = _items.at (i);

		String &strText = item.GetCaption();
		LPCTSTR text = strText.c_str();

		if (i == _selectedIndex) {
			SelectObject (hdc, _hTabstripBoldFont);
		}

		const int dtFlags = DT_LEFT | DT_TOP | DT_CALCRECT;
		::DrawText (hdc, text, _tcslen (text), &rtText, dtFlags);

		if (i == _selectedIndex) {
			SelectObject (hdc, _hTabstripFont);
		}

		item._width = (rtText.right - rtText.left) + (TABSTRIP_PADDING * 2);

		if (item._hIcon != NULL) {
			item._width+= TABSTRIP_ICON_SIZE_X + TABSTRIP_PADDING;
		}
	}

	AfterDraw (hdc);
	ReleaseDC(GetHandle(), hdc);
}

void TabStrip::OnPaint (HDC hdc, PAINTSTRUCT &ps)
{
	View::OnPaint (hdc, ps);
	BeforeDraw (hdc);
	DrawTabStrip (hdc);
	AfterDraw (hdc);
}

void TabStrip::OnMouseDown (int shift, int button, int x, int y)
{
	View::OnMouseDown (shift, button, x, y);
	
	int index = HitTest (x, y);
	
	if (button == kMouseButtonLeft)
	{	
		if (index != -1 & index != _selectedIndex) {		
			SetSelectedTabIndex (index);
		}
	}
	else if (button == kMouseButtonRight)
	{
		if (index != -1 & index != _selectedIndex) {		
			SetSelectedTabIndex (index);
		}
		if (index != -1 && _eventHandler) {
			_eventHandler->TabStrip_OnContextMenu(this, index);
		}
	}
}

void TabStrip::BeforeDraw (HDC hdc)
{
	_dcContext = ::SaveDC (hdc);
	SelectObject (hdc, _hTabstripFont);
	::SetBkMode (hdc, TRANSPARENT);
}

void TabStrip::AfterDraw (HDC hdc)
{
	::RestoreDC (hdc, _dcContext);
}

void TabStrip::GetTabStripItemRect (int index, RECT &rtItem)
{
	GetClientRect (rtItem);
	rtItem.left++;
	rtItem.top++;

	// hide bottom corners
	rtItem.bottom+= 2;

	for (int i=0; i < index; i++) {
		TabStripItem &item = _items.at (i);
		rtItem.left+= item._width;
	}

	TabStripItem &item = _items.at (index);
	rtItem.right = rtItem.left + item._width;
	rtItem.right-= TABSTRIP_INTERNAL_SPACING;

	if (index != _selectedIndex) {
		rtItem.top+= TABSTRIP_MARGIN_TOP;
	}
}

void TabStrip::DrawTabStrip (HDC hdc)
{
	for (int i=0; i < _items.size(); i++) {
		DrawTabStripItem (i, hdc);
	}	
}

void TabStrip::DrawTabStripItem (int index, HDC hdc)
{
	TabStripItem &item = _items.at (index);	

	COLORREF bgColor, fgColor, bdrColor;
	if (index == _selectedIndex)
	{
		bgColor = TABSTRIP_ITEM_SEL_BGCOLOR;
		fgColor = TABSTRIP_ITEM_SEL_FGCOLOR;
		bdrColor = TABSTRIP_ITEM_SEL_BDRCOLOR;
		SelectObject (hdc, _hTabstripBoldFont);
	}
	else
	{
		bgColor = TABSTRIP_ITEM_BGCOLOR;
		fgColor = TABSTRIP_ITEM_FGCOLOR;
		bdrColor = TABSTRIP_ITEM_BDRCOLOR;
		SelectObject (hdc, _hTabstripFont);
	}
	
	RECT rt;
	GetTabStripItemRect (index, rt);

	// draw bg
	HGDIOBJ hBgBrush = ::CreateSolidBrush (bgColor);
	HGDIOBJ hBdrPen = ::CreatePen (PS_SOLID, 1, bdrColor);
	::SelectObject (hdc, hBgBrush);
	::SelectObject (hdc, hBdrPen);
	::RoundRect (hdc, rt.left, rt.top, rt.right, rt.bottom, ROUND_CORNER_SIZE, ROUND_CORNER_SIZE);
	::DeleteObject (hBgBrush);
	::DeleteObject (hBdrPen);

	if (index == _selectedIndex) {
		rt.top+= TABSTRIP_MARGIN_TOP;
	}

	// draw icon if required
	if (item._hIcon != NULL)
	{
		int x = rt.left + TABSTRIP_PADDING;
		// int y = rt.top + (rt.bottom - rt.top) / 2 - (TABSTRIP_ICON_SIZE_Y / 2);
		int y = rt.top + (rt.bottom - rt.top) / 2;
		::DrawIconEx (hdc, x, y, 
						item._hIcon, 
						TABSTRIP_ICON_SIZE_X, 
						TABSTRIP_ICON_SIZE_Y, 
						0, NULL, DI_NORMAL);

		// rt.left+= TABSTRIP_PADDING + TABSTRIP_ICON_SIZE_X;
		rt.left+= TABSTRIP_PADDING;
	}

	// draw text
	::SetTextColor (hdc, fgColor);

	String &strText = item.GetCaption();
	LPCTSTR text = strText.c_str();
	const int dtFlags = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	::DrawText (hdc, text, _tcslen (text), &rt, dtFlags);
}

void TabStrip::Fire_OnSelectTab (int index)
{
	if (_eventHandler) _eventHandler->TabStrip_OnSelectTab (this, index);
}

void TabStrip::Fire_OnDeselectTab (int index)
{
	if (_eventHandler) _eventHandler->TabStrip_OnDeselectTab (this, index);
}

}; // wingui namespace
