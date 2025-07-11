

#ifndef __GAMESPACE_H__
#define __GAMESPACE_H__

#include "winGUI\window.h"

class GameSpace : public wingui::Window
{
public:
	GameSpace() {};

protected:
	// overidables.
	virtual void GetCreateStyle (DWORD &dwStyle, DWORD &dwExStyle);

	// message handlers
	virtual void OnCreate () { }
	virtual void OnPreDestroy () { }
	virtual void OnDestroy () { }
	virtual void OnEraseBackground (HDC hdc, PAINTSTRUCT &ps) { }
	virtual void OnMouseDown (int shift, int button, int x, int y) { }
	virtual void OnMouseUp (int shift, int button, int x, int y) { }
	virtual void OnMouseMove (int shift, int button, int x, int y) { }
	virtual void OnMouseLeave () { }
	virtual void OnKeyPress (int virtkey, int repeatCount) { }
	virtual void OnKeyDown (int virtkey) { }
	virtual void OnKeyUp (int virtkey) { }
	virtual void OnPaint (HDC hdc, PAINTSTRUCT &ps);
	virtual void OnResize (int cx, int cy) { }
	virtual void OnGotFocus () { }
	virtual void OnLostFocus () { }
	virtual void OnContextMenu () { }
	virtual void OnClose (BOOL &cancel) { };
	virtual void OnHScroll (int scrollPos) { }
	virtual void OnVScroll (int scrollPos) { }

public:

	// force the system to redraw the next frame.
	void Invalidate();
	void GetPlayArea(int &aStartX, int &aStartY, int &aWidth, int &aHeight);
	
private:

};

#endif  // __GAMESPACE_H__