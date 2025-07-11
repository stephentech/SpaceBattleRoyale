#include "stdafx.h"
#include <assert.h>
#include <wingui/tree_view.h>

namespace wingui {

#ifdef DEBUG
#define ASSERT(C) assert((C))
#else
#define ASSERT(C)
#endif

TreeView::TreeView() :
	_hImageList(NULL)
{
	g_fDragging = FALSE;
}

TreeView::TreeView(HIMAGELIST hImageList) :
	_hImageList(hImageList)
{ 
	g_fDragging = FALSE;
}

TreeView::~TreeView ()
{
}

HTREEITEM TreeView::AddItem (HTREEITEM hParentItem, LPCTSTR lpszText, int icon, LPARAM lParam)
{
    TVITEM tvi;
	// set style
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 
    // Set the text of the item.     
	tvi.pszText = (LPSTR)lpszText; 
    tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);
	// set icon
    tvi.iImage = icon;
    tvi.iSelectedImage = icon;
	// set data
    tvi.lParam = lParam;

	TVINSERTSTRUCT tvins; 
    tvins.item = tvi;
    tvins.hInsertAfter = (HTREEITEM)TVI_LAST; 
	tvins.hParent = hParentItem; 

    // Add the item
    return TreeView_InsertItem (GetHandle(), &tvins);
}

void TreeView::GetCreateStyle (DWORD &dwStyle, DWORD &dwExStyle)
{
	View::GetCreateStyle (dwStyle, dwExStyle);
	dwStyle|= (WS_BORDER  | TVS_HASLINES | TVS_HASBUTTONS | TVS_SHOWSELALWAYS);
}

void TreeView::GetClassStyle (WNDCLASSEX &wcex)
{
	wcex.lpszClassName = WC_TREEVIEW;
}

void TreeView::OnCreate ()
{
	View::OnCreate();

	// Map the tree view window handle to this object. This enables the parent 
	// window / view to route tree view messages to this class.
	MapHandle ();

	// Map window procedure
	MapProc ();

	// Associate the image list with the tree-view control. 
	if (_hImageList != NULL) {
		TreeView_SetImageList (GetHandle(), _hImageList, TVSIL_NORMAL); 
	}
	// set background color to white
	TreeView_SetBkColor (GetHandle(), RGB(255,255,255));
}

void TreeView::OnDestroy ()
{
	UnmapProc();
	UnmapHandle();
	View::OnDestroy ();
}

LRESULT TreeView::OnNotify (LPNMHDR lpNotificHdr, WPARAM wParam, LPARAM lParam)
{
	switch (lpNotificHdr->code) {
	case TVN_SELCHANGED:
		{
			LPNMTREEVIEW pSelInfo = (LPNMTREEVIEW)lParam;
			OnSelectItem(&pSelInfo->itemNew);
			break;
		}
	case TVN_BEGINDRAG:
		{
			LPNMTREEVIEW pSelInfo = (LPNMTREEVIEW)lParam;
			OnItemDragStart(&pSelInfo->itemNew);
			break;
		}
	case NM_DBLCLK:
		{
			OnDblClick();
			break;
		}
	case NM_RCLICK:
		{
			OnRightClick();
			break;
		}
	}

	return FALSE;
}

#if 0
void TreeView::OnItemDragStart (LPTVITEM itemNew)
{
	HIMAGELIST himl;    // handle to image list 
	RECT rcItem;        // bounding rectangle of item 
	DWORD dwLevel;      // heading level of item 
	DWORD dwIndent;     // amount that child items are indented 

	// Tell the tree-view control to create an image to use 
	// for dragging. 
	himl = TreeView_CreateDragImage(GetHandle(), itemNew->hItem); 

	// Get the bounding rectangle of the item being dragged. 
	TreeView_GetItemRect(GetHandle(), itemNew->hItem, &rcItem, TRUE); 

	// Get the heading level and the amount that the child items are 
	// indented. 
	dwLevel = itemNew->lParam; 
	dwIndent = (DWORD)SendMessage(GetHandle(), TVM_GETINDENT, 0, 0); 

	// Start the drag operation. 
	ImageList_BeginDrag(himl, 0, 0, 0);
	ImageList_DragEnter(GetParent(), 50, 50);  

	// Hide the mouse pointer, and direct mouse input to the 
	// parent window. 
	ShowCursor(FALSE); 
	SetCapture(GetHandle()); 
	g_fDragging = TRUE; 
}

void TreeView::OnMouseMove(int shift, int btn, int x, int y)
{
	HTREEITEM htiTarget;  // handle to target item 
    TVHITTESTINFO tvht;  // hit test information 

    if (g_fDragging) 
    { 
        // Drag the item to the current position of the mouse pointer. 
        ImageList_DragMove(x, y);        
    } 
}

void TreeView::OnMouseUp(int shift, int btn, int x, int y)
{
	 if (g_fDragging) 
    { 
        ImageList_EndDrag(); 
        ReleaseCapture(); 
        ShowCursor(TRUE); 
        g_fDragging = FALSE; 
    } 
}
#endif

}; // wingui namespace
