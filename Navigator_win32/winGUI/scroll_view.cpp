#include "stdafx.h"
#include <wingui/scroll_view.h>

namespace wingui {

ScrollView::ScrollView (int docViewCx, int docViewCy) :
	_xScroll(0),
	_yScroll(0),
	_docViewCx(docViewCx),
	_docViewCy(docViewCy)
{
}

void ScrollView::SetDocViewSize (int docViewCx, int docViewCy)
{
	_docViewCx = docViewCx;
	_docViewCy = docViewCy;
	UpdateScrollbars();
	RedrawView();
}

void ScrollView::OnCreate ()
{
	View::OnCreate ();
	UpdateScrollbars ();
}

void ScrollView::OnResize (int cx, int cy)
{
	View::OnResize (cx, cy);
	UpdateScrollbars ();
}

void ScrollView::OnHScroll (int scrollPos)
{	
	RECT rcRedraw;
	GetClientRect (rcRedraw);
	
	/*
	// find scrolled region
	int regionCx = scrollPos - _xScroll;
	if (regionCx > 0) {
		// update right region
		rcRedraw.left = rcRedraw.right - regionCx;
	} else {
		// update left region
		rcRedraw.right = rcRedraw.left - regionCx;
	}
	*/

	// set new scroll pos
	_xScroll = scrollPos;
	SetScrollPos (GetHandle(), SB_HORZ, _xScroll, TRUE);

	// redraw scrolled region
	InvalidateRect (GetHandle(), &rcRedraw, TRUE);
}

void ScrollView::OnVScroll (int scrollPos)
{
	RECT rcRedraw;
	GetClientRect (rcRedraw);
	
	/*
	// find scrolled region
	int regionCy = scrollPos - _yScroll;
	if (regionCy > 0) {
		// update bottom region
		rcRedraw.top = rcRedraw.bottom - regionCy;
	} else {
		// update top region
		rcRedraw.bottom = rcRedraw.top - regionCy;
	}
	*/

	// set new scroll pos
	_yScroll = scrollPos;
	SetScrollPos (GetHandle(), SB_VERT, _yScroll, TRUE);

	// redraw scrolled region
	InvalidateRect (GetHandle(), &rcRedraw, TRUE);
}

void ScrollView::UpdateScrollbars ()
{
	RECT rcClient;
	GetClientRect (rcClient);

	BOOL bShowHorzScrollbar, bShowVertScrollbar;
	bShowHorzScrollbar = bShowVertScrollbar = FALSE;

	UINT sbFlags = 0;
	int scrollMax;

	if (_docViewCx > rcClient.right) {
		// rcClient.bottom-= GetSystemMetrics(SM_CYHSCROLL);
	}
	if (_docViewCy > rcClient.bottom) {
		// rcClient.right-= GetSystemMetrics(SM_CXVSCROLL);
	}

	if (_docViewCx > rcClient.right) {
		sbFlags = SB_HORZ;
		bShowHorzScrollbar = TRUE;
		scrollMax = _docViewCx - rcClient.right;
		if (_xScroll > scrollMax) {
			_xScroll = scrollMax;
		}
		SetScrollRange (GetHandle(), SB_HORZ, 0, scrollMax, FALSE);
	}
	if (_docViewCy > rcClient.bottom) {
		sbFlags|= SB_VERT;
		bShowVertScrollbar = TRUE;
		scrollMax = _docViewCy - rcClient.bottom;
		if (_yScroll > scrollMax) {
			_yScroll = scrollMax;
		}
		SetScrollRange (GetHandle(), SB_VERT, 0, scrollMax, FALSE);
	}

	EnableScrollBar (GetHandle(), sbFlags, ESB_ENABLE_BOTH);
	ShowScrollBar (GetHandle(), SB_HORZ, bShowHorzScrollbar);
	ShowScrollBar (GetHandle(), SB_VERT, bShowVertScrollbar);
}

}; // wingui namespace
