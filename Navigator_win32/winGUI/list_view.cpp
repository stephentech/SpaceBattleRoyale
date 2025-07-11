#include "stdafx.h"
#include <wingui/list_view.h>

namespace wingui {

ListView::ListView () :
	_imageList(NULL),
	_selItem(-1)
{
}

ListView::ListView (ImageList *pImageList) :
	_imageList(pImageList),
	_selItem(-1)
{
}

ListView::~ListView ()
{
}

void ListView::InsertColumn (int index, LPSTR lpszText)
{
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT  | LVCF_SUBITEM;
	lvc.iSubItem = 0;
	lvc.pszText = lpszText;
	lvc.cx = 80;
	lvc.fmt = LVCFMT_LEFT;
	ListView_InsertColumn (GetHandle(), index, &lvc);
}

void ListView::InsertItem (int index, int subItemIndex, LPSTR lpszText)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_STATE; 
	lvi.state = 0; 
	lvi.stateMask = 0;

	lvi.iItem = index;
	lvi.iImage = 0;
	lvi.iSubItem = subItemIndex;
	lvi.lParam = 0;
	lvi.pszText = lpszText;

	if (lvi.iSubItem == 0) {
		ListView_InsertItem (GetHandle(), &lvi);
	} else {
		ListView_SetItem (GetHandle(), &lvi);
	}
}

void ListView::InsertRow (int index, LPSTR lpszText[], int count)
{
	for (int i = 0; i < count; i++) {
		InsertItem (index, i, lpszText[i]);
	}
}

void ListView::Clear ()
{
	ListView_DeleteAllItems (GetHandle());
}

void ListView::GetCreateStyle (DWORD &dwStyle, DWORD &dwExStyle)
{
	View::GetCreateStyle (dwStyle, dwExStyle);
	dwStyle|= (LVS_REPORT | LVS_SHOWSELALWAYS | LVS_EDITLABELS);
}

void ListView::GetClassStyle (WNDCLASSEX &wcex)
{
	wcex.lpszClassName = WC_LISTVIEW;
}

void ListView::OnCreate ()
{
	View::OnCreate();

	// Map the tree view window handle to this object. This enables the parent 
	// window / view to route tree view messages to this class.
	MapHandle ();

	// set image lists
	if (_imageList) {
		ListView_SetImageList (GetHandle(), _imageList->GetHandle(), LVSIL_NORMAL); 
		ListView_SetImageList (GetHandle(), _imageList->GetHandle(), LVSIL_SMALL); 
	}
	
	ListView_SetExtendedListViewStyle (GetHandle(), LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
}

LRESULT ListView::OnNotify (LPNMHDR lpNotificHdr, WPARAM wParam, LPARAM lParam)
{
	switch (lpNotificHdr->code) 
	{
	case NM_CLICK:
		{
			LPNMITEMACTIVATE pInfo = (LPNMITEMACTIVATE)lParam;
			_selItem = pInfo->iItem;
			return TRUE;
		}
	}

	return View::OnNotify (lpNotificHdr, wParam, lParam);
}

}; // namespace wingui
