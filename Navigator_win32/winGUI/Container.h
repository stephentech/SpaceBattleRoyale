#ifndef __WINGUI_LAYOUTCONTAINER_H__
#define __WINGUI_LAYOUTCONTAINER_H__

#include "Layout.h"
#include "window.h"

namespace wingui {

/** IContainer interface. Contains a set of layouts */
class IContainer : public ILayout
{
public:
	/** LayoutOrder enum. */
	enum LayoutOrder {
		kLayoutOrderInvalid = -1,
		kLayoutOrderHorz,
		kLayoutOrderVert,
		kLayoutOrderCustom
	};
	
	/** Returns layout order for this container */
	virtual LayoutOrder GetLayoutOrder () = 0;

	/** Sets layout order for this container */
	virtual void SetLayoutOrder (LayoutOrder order) = 0;
	
	/** Returns the layout at specified index */
	virtual ILayout *GetLayout (int index) = 0;

	/** Sets layouts in this container */
	virtual void SetLayouts (ILayout *pLayout1, ILayout *pLayout2) = 0;

	/** Set the index of auto resizable layou (0 or 1). */
	virtual void SetAutoResizableLayout(int index) = 0;
	
	/** Perform layout operation */
	virtual void DoLayout () = 0;

	// Mouse event handlers for layout resizing...
	virtual void HandleMouseDownEvent (int shift, int button, int x, int y) = 0;
	virtual void HandleMouseUpEvent (int shift, int button, int x, int y) = 0;
	virtual void HandleMouseMoveEvent (int shift, int button, int x, int y) = 0;
};

/** Splitter class. */ 

class Splitter
{
public:
	Splitter (const RECT &rectOrg, BOOL isHorizontal, HDC hdc) :
	  _rectOrg(rectOrg),
	  _rect(rectOrg),
	  _isHorizontal(isHorizontal),
	  _hdc(hdc)
	{
		SetROP2 (_hdc, SRCINVERT);
	}
	
	 inline HDC GetDC() const { return _hdc; }

	void Offset (int offset)
	{
		if (_isHorizontal) {
			_rect.left = _rectOrg.left + offset;
			_rect.right = _rectOrg.right + offset;
		} else {
			_rect.top = _rectOrg.top + offset;
			_rect.bottom = _rectOrg.bottom + offset;
		}
	}

	void Draw()
	{
		HGDIOBJ hOldBrush = SelectObject(_hdc, GetStockObject(BLACK_BRUSH));
		HGDIOBJ hOldPen = SelectObject(_hdc, GetStockObject(BLACK_PEN));		
		Rectangle (_hdc, _rect.left, _rect.top, _rect.right, _rect.bottom);
		SelectObject(_hdc, hOldBrush);
		SelectObject(_hdc, hOldPen);
	}

private:
	BOOL	_isHorizontal;
	RECT	_rectOrg;
	RECT	_rect;
	HDC		_hdc;
};

/** Container class. Base layout container implementation. */

#define kMinLayoutSize			100

class Container : public IContainer
{
public:
	Container (Window *pWindow, LayoutOrder order, BOOL resizable = TRUE);

private:
	Container (const Container &) { }
	Container & operator =(const Container &) { return *this; }

public:
	// ILayout methods...
	virtual Frame GetFrame () { return _frame; }
	virtual void SetFrame (const Frame &f) { _frame = f; }
	virtual BOOL IsResizable () { return _resizable; }
	virtual void SetResizable (BOOL flag) { _resizable = flag; }
	virtual BOOL IsContainer () { return TRUE; }

	// IContainer methods...
	virtual LayoutOrder GetLayoutOrder () { return _order; }
	virtual void SetLayoutOrder (LayoutOrder order) { _order = order; }	
	virtual ILayout *GetLayout (int i) { return _layouts[i]; }
	virtual void SetLayouts (ILayout *pLayout1, ILayout *pLayout2);	
	virtual void SetAutoResizableLayout(int index);
	virtual void DoLayout ();
	virtual void HandleMouseDownEvent (int shift, int button, int x, int y);
	virtual void HandleMouseUpEvent (int shift, int button, int x, int y);
	virtual void HandleMouseMoveEvent (int shift, int button, int x, int y);

protected:
	void GetSplitterRect (int &x, int &y, int &x2, int &y2);

protected:
	Window					*_pWindow;
	BOOL					_resizable;
	Frame					_frame;
	LayoutOrder				_order;	
	ILayout					*_layouts[2];
	int						_autoResiableLayout;
	BOOL					_flagSplit;
	Splitter				*_splitter;
	int						_startX, _startY;
	Frame					_startFrames[2];
	Frame					_endFrames[2];
};

}; // wingui namespace

#endif // !__WINGUI_LAYOUTCONTAINER_H__
