#include "stdafx.h"
#include <wingui/Panel.h>
#include <wingui/globals.h>
#include <assert.h>

namespace wingui {

const int kTitleBarHeight = 16;
const int kMargin = 4;

PanelBar::PanelBar (int panelResId, Panel *pParentLayout) :
	m_panelResId(panelResId),
	m_pParentLayout(pParentLayout),
	m_pPanelView(NULL),
	m_deletePanelView(FALSE),
	m_state(kStateExpanded),
	m_titleLabel(NULL),
	m_toggleButton(NULL)
{
}

PanelBar::PanelBar (View *pPanelView, BOOL bDeleteView, Panel *pParentLayout) :
	m_panelResId(-1),
	m_pParentLayout(pParentLayout),
	m_pPanelView(pPanelView),
	m_deletePanelView(bDeleteView),
	m_state(kStateExpanded),
	m_titleLabel(NULL),
	m_toggleButton(NULL)
{
}

PanelBar::~PanelBar()
{
	if (m_deletePanelView) {
		delete m_pPanelView;
	}
}

BOOL PanelBar::Create (LPCTSTR lpszTitle, Window *pParent)
{
	if (View::Create (0, 0, 1, 1, pParent))
	{
		SetTitleText(lpszTitle);
		return TRUE;
	}
	return FALSE;
}

void PanelBar::Expand()
{
	if (m_state != kStateExpanded) {
		m_state = kStateExpanded;

		Move(0, 0, m_sizeExpanded.cx, m_sizeExpanded.cy, TRUE);
		m_pParentLayout->UpdateLayout (this);
	}
}

void PanelBar::Collapse()
{
	if (m_state != kStateCollapsed) {
		m_state = kStateCollapsed;

		Move(0, 0, m_sizeCollapsed.cx, m_sizeCollapsed.cy, TRUE);
		m_pParentLayout->UpdateLayout (this);
	}
}

void PanelBar::OnCreate()
{
	View::OnCreate();
	
	if (m_pPanelView == NULL && m_panelResId != -1) {
		m_pPanelView = CreatePanelView();
		m_pPanelView->Create(m_panelResId, this);
		m_deletePanelView = TRUE;
	}
	AddSubView (m_pPanelView);
	m_pPanelView->Show();

	RECT rect;
	m_pPanelView->GetWindowRect(rect);

	m_sizeExpanded.cx = (rect.right - rect.left) + (kMargin * 2);
	m_sizeExpanded.cy = (rect.bottom - rect.top) + (kMargin * 2) + kTitleBarHeight;

	m_sizeCollapsed = m_sizeExpanded;
	m_sizeCollapsed.cy = kTitleBarHeight;

	Move(0, 0, m_sizeExpanded.cx, m_sizeExpanded.cy, FALSE); 
}

void PanelBar::OnResize(int cx, int cy)
{	
	RECT rect;	
	m_pPanelView->GetWindowRect (rect);
	m_pPanelView->Move (kMargin, kTitleBarHeight + kMargin, cx - (kMargin * 2), rect.bottom - rect.top);
}

void PanelBar::OnPaint (HDC hdc, PAINTSTRUCT &ps)
{
	RECT rect;
	GetClientRect (rect);
	
	HBRUSH hTitleBarBrush = CreateSolidBrush(RGB(70,90,125));
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(70,90,125));

	// draw title bar
	HGDIOBJ hOldBrush = SelectObject (hdc, hTitleBarBrush);
	HGDIOBJ hOldPen = SelectObject (hdc, GetStockObject(NULL_PEN));
	Rectangle (hdc, rect.left + 1, rect.top, rect.right, rect.top + kTitleBarHeight);

	// draw border
	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	SelectObject (hdc, hPen);
	Rectangle (hdc, rect.left, rect.top, rect.right, rect.bottom);

	SelectObject (hdc, hOldBrush);
	SelectObject (hdc, hPen);

	DeleteObject (hTitleBarBrush);
	DeleteObject (hPen);

	// draw title
	RECT titleRect;
	GetClientRect (titleRect);
	titleRect.left+= 2;
	titleRect.bottom = titleRect.top + kTitleBarHeight;
	
	TCHAR title[255];
	GetWindowText (GetHandle(), title, 255);
	SetBkMode (hdc, TRANSPARENT);
	SetTextColor (hdc, RGB(255,255,255));

	HGDIOBJ hOldFont = SelectObject(hdc, gPanelBarTitleFont);
	DrawText (hdc, title, _tcslen(title), &titleRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	SelectObject(hdc, hOldFont);
}

void PanelBar::OnMouseDown (int shift, int button, int x, int y)
{
	if (button == kMouseButtonLeft)
	{
		RECT titleRect;
		GetClientRect (titleRect);
		titleRect.left+= 2;
		titleRect.bottom = titleRect.top + kTitleBarHeight;

		POINT pt = { x, y };
		if (::PtInRect(&titleRect, pt))
		{
			if (m_state == kStateCollapsed) {
				Expand();
			} else {
				Collapse();
			}
		}
	}
}

}; // wingui namespace
