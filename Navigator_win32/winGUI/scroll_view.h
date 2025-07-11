#ifndef __WINGUI_SCROLLVIEW_H__
#define __WINGUI_SCROLLVIEW_H__

#include "view.h"

namespace wingui {

class ScrollView : public View
{
public:
	ScrollView (int docViewCx, int docViewCy);

	inline int GetXScroll () const { return _xScroll; }
	inline int GetYScroll () const { return _yScroll; }

	void SetDocViewSize (int docViewCx, int docViewCy);
 
protected:
	virtual void OnCreate ();
	virtual void OnResize (int cx, int cy);
	virtual void OnHScroll (int scrollPos);
	virtual void OnVScroll (int scrollPos);

	void UpdateScrollbars ();

protected:
	int			_xScroll;
	int			_yScroll;
	int			_docViewCx;
	int			_docViewCy;
};

}; // wingui namespace

#endif // !__WINGUI_SCROLLVIEW_H__
