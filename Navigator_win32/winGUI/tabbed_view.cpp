#include "stdafx.h"
#include <wingui/tabbed_view.h>
#include "resource.h"

namespace wingui {

#define kPad	0
#define kEmptyViewBgColor		RGB(48,66,97)

void EmptyView::GetClassStyle (WNDCLASSEX &wcex)
{
	View::GetClassStyle (wcex);
	wcex.lpszClassName = _T("_wingui_emptyview");
	wcex.hbrBackground = CreateSolidBrush(kEmptyViewBgColor);
}

void EmptyView::OnCreate ()
{
	View::OnCreate ();
	_font.Create(_T("MS Sans Serif"), 9);
}

void EmptyView::OnDestroy ()
{
	_font.Delete();
}

void EmptyView::OnPaint (HDC hdc, PAINTSTRUCT &ps)
{
	HFONT hOldFont = (HFONT)SelectObject(hdc, _font);
	RECT rcClient;
	GetClientRect (rcClient);
	LPCTSTR lpszText = _T("No editor open");
	SetTextColor (hdc, RGB(200,200,200));
	SetBkMode (hdc, TRANSPARENT);
	DrawText (hdc, lpszText, lstrlen(lpszText), &rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	SelectObject(hdc, hOldFont);
}

TabbedView::TabbedView ()
{
}

TabbedView::~TabbedView ()
{
	DeleteAllViews();	
}

void TabbedView::AddView (View *pView, LPCTSTR lpszTitle, int viewId, BOOL autoDelete)
{	
	int index = FindView(viewId);
	WINGUI_ASSERT(index == -1);

	TabStripItems &tabstripItems = m_tabstrip.GetItems();
	
	// add new tabbed view item
	Tab item(pView, viewId, lpszTitle, autoDelete);
	m_views.push_back(item);
	AddSubView(pView);

	// resize the view
	RECT rcView;
	GetViewRect(rcView);
	pView->Move (rcView.left, rcView.top, rcView.right - rcView.left, rcView.bottom - rcView.top);

	// add new tabstrip item
	TabStripItem tabstripItem (viewId, lpszTitle);
	tabstripItems.push_back(tabstripItem);	
	m_tabstrip.RefreshView();
	
	// select the new tab
	m_tabstrip.SetSelectedTabIndex(tabstripItems.size() - 1);
	UpdateUI();
}

void TabbedView::CloseView (View *pView)
{
	WINGUI_ASSERT(pView != NULL);

	int index = FindView(pView);
	WINGUI_ASSERT(index != -1);

	// get tab for this view
	const Tab &item = m_views[index];

	// remove the tab view
	RemoveSubView(item.pView);

	// delete the view if autoDelete is true
	if (item.autoDelete)
	{
		item.pView->Destroy();
		DeleteView(item.pView);
	}

	// delete tab
	m_views.erase(m_views.begin() + index);
	
	// delete tabstrip item
	m_tabstrip.SetSelectedTabIndex(-1);
	TabStripItems &tabstripItems = m_tabstrip.GetItems();
	tabstripItems.erase(tabstripItems.begin() + index);	

	// update view and tabtstrip control
	m_tabstrip.RefreshView();
	UpdateUI();	
}

void TabbedView::CloseAllViews ()
{
	// delete all views
	DeleteAllViews ();

	// update view and tabstrip control
	if (IsValid())
	{
		UpdateUI ();
	}
	if (m_tabstrip.IsValid())
	{
		m_tabstrip.RefreshView();
	}
}

void TabbedView::SelectView (View *pView)
{
	WINGUI_ASSERT(pView != NULL);

	int index = FindView(pView);
	WINGUI_ASSERT(index != -1);

	if (m_tabstrip.GetSelectedTabIndex () != index)
	{
		m_tabstrip.SetSelectedTabIndex(index);
	}
}

View *TabbedView::GetCurrentView ()
{
	// get id of the selected view
	int selectedIndex = m_tabstrip.GetSelectedTabIndex();
	TabStripItems &tabstripItems = m_tabstrip.GetItems();
	if (selectedIndex == -1 || tabstripItems.size() == 0) {
		return NULL;
	} else {
		int selectedViewId = tabstripItems[selectedIndex].GetId();
		for (ViewMap::iterator it = m_views.begin(); it != m_views.end(); it++) {
			View *pView = it->pView;
			if (it->id == selectedViewId) {
				return pView;
			}
		}
	}
	return NULL;
}

void TabbedView::UpdateUI()
{
	int selectedIndex = m_tabstrip.GetSelectedTabIndex();	

	if (selectedIndex == -1) 
	{
		// show the empty view
		m_emptyView.Show ();
		m_emptyView.SetFocus ();
	} 
	else 
	{
		// hide the empty view
		m_emptyView.Hide ();		

		// get id of the selected view
		int selectedViewId;
		TabStripItems &tabstripItems = m_tabstrip.GetItems();
		if (selectedIndex == -1 || tabstripItems.size() == 0) {
			selectedViewId = -1;
		} else {
			selectedViewId = tabstripItems[selectedIndex].GetId();
		}

		// hide all other views except the selected view
		View *pViewToShow = NULL;
		for (ViewMap::iterator it = m_views.begin(); it != m_views.end(); it++) {
			View *pView = it->pView;
			if (it->id == selectedViewId) {
				pViewToShow = pView;				
			} else {
				pView->Hide();
			}
		}

		// show the selected view and set focus to it
		if (pViewToShow) {
			pViewToShow->Show();
			pViewToShow->SetFocus();
		}
	}
}

void TabbedView::OnCreate()
{
	View::OnCreate();

	// create tapstrip control
	m_tabstrip.Create (0, 0, 1, kTabstripHeight, this);
	m_tabstrip.SetEventHandler (this);
	AddSubView(&m_tabstrip);
	m_tabstrip.Show();

	// create empty view to display when no tabs are present
	m_emptyView.Create (0, 0, 1, 1, this);
	AddSubView(&m_emptyView);
}

void TabbedView::OnResize (int cx, int cy)
{
	View::OnResize(cx, cy);

	RECT rcClient;
	GetClientRect(rcClient);
	
	// resize tabstrip control	
	m_tabstrip.Move (0, 0, rcClient.right - rcClient.left, kTabstripHeight);
		
	// if no view is selected resize the empty view
	int selectedView = m_tabstrip.GetSelectedTabIndex();
	if (selectedView == -1) 
	{
		m_emptyView.Move (kPad, kTabstripHeight + kPad,
							rcClient.right - rcClient.left - (kPad * 2), 
							rcClient.bottom - rcClient.top - kTabstripHeight - (kPad * 2)
							);
	}

	// resize all views
	ResizeViews ();

	// update tab conatiner view
	UpdateUI ();
}

// protected member functions...

void TabbedView::GetViewRect(RECT &rcView)
{
	RECT rcClient;
	GetClientRect(rcClient);

	rcView.left = rcClient.left + kPad;
	rcView.top = rcClient.top + kTabstripHeight + kPad;
	rcView.right = rcClient.right - kPad;
	rcView.bottom = rcClient.bottom - kPad;
}

void TabbedView::DeleteView (View *pView)
{
	delete pView;
}

void TabbedView::DeleteAllViews()
{
	for (ViewMap::iterator it = m_views.begin(); it != m_views.end(); it++) 
	{
		const Tab &item = *it;
		if (item.autoDelete)
		{
			item.pView->Destroy();
			DeleteView(item.pView);
		}
	}

	m_views.clear();
	m_tabstrip.GetItems().clear();
}

int TabbedView::FindView(int id) const
{
	int index = 0;
	for (ViewMap::const_iterator it = m_views.begin(); it != m_views.end(); it++, index++) {
		const Tab &item = *it;
		if (item.id == id) {
			return index;
		}
	}
	return -1;
}

int TabbedView::FindView(View *pView) const
{
	int index = 0;
	for (ViewMap::const_iterator it = m_views.begin(); it != m_views.end(); it++, index++) {
		const Tab &item = *it;
		if (item.pView == pView) {
			return index;
		}
	}
	return -1;
}

void TabbedView::ResizeViews()
{	
	// calc rect for views
	RECT rcView;
	GetViewRect(rcView);

	// resize all views
	for (ViewMap::iterator it = m_views.begin(); it != m_views.end(); it++) {
		View *pView = (*it).pView;
		pView->Move (rcView.left, rcView.top, rcView.right - rcView.left, rcView.bottom - rcView.top);
	}
}

void TabbedView::TabStrip_OnSelectTab (TabStrip *sender, int index)
{
	UpdateUI ();
}

void TabbedView::TabStrip_OnDeselectTab (TabStrip *sender, int index)
{
}

void TabbedView::TabStrip_OnContextMenu (TabStrip *sender, int indx)
{	
}

}; // wingui namespace
