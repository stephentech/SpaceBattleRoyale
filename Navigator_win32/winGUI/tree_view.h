#ifndef __TREEVIEW_H__
#define __TREEVIEW_H__

#include <wingui/view.h>
#include <wingui/image_list.h>

namespace wingui {

class TreeView : public View
{
public:
	TreeView ();
	TreeView (HIMAGELIST hImageList);
	virtual ~TreeView ();	
	
	inline HIMAGELIST GetImageList () { return _hImageList; }
	inline void SetImageList (HIMAGELIST list) { _hImageList = list; }

	HTREEITEM AddItem (HTREEITEM hParentItem, LPCTSTR lpszText, int icon, LPARAM lParam = 0);
		
	inline void Clear () { 
		TreeView_DeleteAllItems(GetHandle()); 
	}

	// overrides...	
	virtual void CalcLayout () { }

protected:
	// overrides...	
	virtual void GetCreateStyle (DWORD &dwStyle, DWORD &dwExStyle);
	virtual void GetClassStyle (WNDCLASSEX &wcex);	

	// message handlers...
	virtual void OnCreate ();
	virtual void OnDestroy ();
	virtual LRESULT OnNotify (LPNMHDR lpNotificHdr, WPARAM wParam, LPARAM lParam);
	// Treeview control messages...
	virtual void OnSelectItem (LPTVITEM itemNew) { }
	virtual void OnItemDragStart (LPTVITEM itemNew) { }
	virtual void OnDblClick () { }
	virtual void OnRightClick () { }

	// virtual void OnMouseMove(int shift, int btn, int x, int y);
	// virtual void OnMouseUp(int shift, int btn, int x, int y);

protected:
	HIMAGELIST _hImageList;
	BOOL g_fDragging;
	friend class Window;
	friend class View;
};

}; // wingui namespace

#endif // !__TREEVIEW_H__
