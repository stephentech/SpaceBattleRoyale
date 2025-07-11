#ifndef __LISTVIEW_H__
#define __LISTVIEW_H__

#include <wingui/view.h>
#include <wingui/image_list.h>

namespace wingui {

class ListView : public View
{
public:
	ListView ();
	ListView (ImageList *pImageList);
	virtual ~ListView ();

	inline ImageList *GetImageList () { return _imageList; }
	inline void SetImageList (ImageList *pImageList) { _imageList = pImageList; }

	int GetSelectedItem() const { return _selItem; }

	void InsertColumn (int index, LPSTR lpszText);
	void InsertItem (int index, int subItemIndex, LPSTR lpszText);
	void InsertRow (int index, LPSTR lpszText[], int count);
	void Clear ();

	void SetListStyle (LONG style)
	{
		DWORD listViewStyle = GetWindowLong (GetHandle(), GWL_STYLE);
		listViewStyle&= ~(LVS_ICON | LVS_REPORT | LVS_SMALLICON | LVS_LIST);
		listViewStyle|= style;
		SetWindowLong (GetHandle(), GWL_STYLE, (LONG)listViewStyle); 
	}

protected:
	// overrides...	
	virtual void GetCreateStyle (DWORD &dwStyle, DWORD &dwExStyle);
	virtual void GetClassStyle (WNDCLASSEX &wcex);

	// message handlers...
	virtual void OnCreate ();
	virtual LRESULT OnNotify (LPNMHDR lpNotificHdr, WPARAM wParam, LPARAM lParam);

protected:
	ImageList	*_imageList;
	int			_selItem;

	friend class Window;
};

}; // wingui namespace

#endif // !__LISTVIEW_H__
