/*
 * TabStrip.h
 */

#ifndef __WINGUI_TABSTRIP_H__
#define __WINGUI_TABSTRIP_H__

#include "view.h"
#include <string>
#include <vector>

namespace wingui {

#ifdef UNCIODE
typedef std::wstring String;
#else
typedef std::string String;
#endif

class TabStrip;

/*
 * class Sim8085IdeTabStripItem
 */
class TabStripItem
{
public:
	TabStripItem (int id) : 
		_id(id),
		_caption(_T("")),
		_width (0),
		_hIcon(NULL)
	{ }

	TabStripItem (int id, LPCTSTR lpszCaption) :
		_id(id),
		_caption(lpszCaption),
		_width (0),
		_hIcon(NULL)
	{
	}
	
	inline int GetId() const { return _id; }

	inline String GetCaption () const { return _caption; }
	inline void SetCaption (const String &caption) { _caption = caption; }
	inline HICON GetIcon () const { return _hIcon; }
	inline void SetIcon (HICON hIcon) { _hIcon = hIcon; }

private:
	int				_id;
	String			_caption;
	int				_width;
	HICON			_hIcon;

	friend class TabStrip;
};

typedef std::vector<TabStripItem> TabStripItems;

/*
 * class TabStripEvents
 */
class TabStripEvents
{
public:
	virtual void TabStrip_OnSelectTab (TabStrip *sender, int index) = 0;
	virtual void TabStrip_OnDeselectTab (TabStrip *sender, int index) = 0;
	virtual void TabStrip_OnContextMenu (TabStrip *sender, int indx) = 0;
};

/*
 * class TabStrip
 */
class TabStrip : public View
{
public:
	enum Orientation
	{
		kOrientationHorizontal,
		kOrientationVertical
	};

private:
	int					_dcContext;

protected:
	Orientation			_orientation;
	TabStripItems		_items;
	int					_selectedIndex;
	COLORREF			_borderColor;
	TabStripEvents		*_eventHandler;

public:
	TabStrip ();
	virtual ~TabStrip ();

	// overrides
	virtual void GetClassStyle (WNDCLASSEX &wcex);

	// properties...
	inline TabStripEvents *GetEventHandler() { return  _eventHandler; }
	inline void SetEventHandler (TabStripEvents *eventHandler) { _eventHandler = eventHandler; }

	inline Orientation GetOrientation () const { return _orientation; }
	inline void SetOrientation (Orientation orientation) { _orientation = orientation; }
	
	inline TabStripItems &GetItems () { return _items; }
	inline const TabStripItems &GetItems () const { return _items; }

	inline void SetBorderColor (COLORREF clr) { _borderColor = clr; }
	inline COLORREF GetBorderColor () { return _borderColor; }

	// operations...
	int HitTest (int x, int y);	
	inline int GetSelectedTabIndex () const { return _selectedIndex; }
	void SetSelectedTabIndex (int index);	

protected:
	// overrides
	virtual void CalcLayout ();	
	virtual void OnPaint (HDC hdc, PAINTSTRUCT &ps);
	virtual void OnMouseDown (int shift, int button, int x, int y);
	virtual void OnResize (int cx, int cy)
	{
		View::OnResize (cx, cy);
	}
	
	// protected helper functions...
	void GetTabStripItemRect (int index, RECT &rt);	
	void DrawTabStrip (HDC hdc);
	void DrawTabStripItem (int index, HDC hdc);
	void Fire_OnSelectTab (int index);
	void Fire_OnDeselectTab (int index);
	void BeforeDraw (HDC hdc);
	void AfterDraw (HDC hdc);
};

}; // wingui namespace

#endif /* !__WINGUI_TABSTRIP_H__ */
