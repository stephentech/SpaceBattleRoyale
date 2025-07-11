

#ifndef __ENG_WINDOW_H__
#define __ENG_WINDOW_H__

#include "winGUI\window.h"

class EngWindow : public wingui::Window
{
public:
	static EngWindow& GetInstance();

protected:
	//virtual LRESULT HandleMessage (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// message handlers
	virtual void OnCreate () { }
	virtual void OnPreDestroy ();
	virtual void OnDestroy ();
	virtual void OnEraseBackground (HDC hdc, PAINTSTRUCT &ps) { }
	virtual void OnMouseDown (int shift, int button, int x, int y) { }
	virtual void OnMouseUp (int shift, int button, int x, int y) { }
	virtual void OnMouseMove (int shift, int button, int x, int y) { }
	virtual void OnMouseLeave () { }
	virtual void OnKeyPress (int virtkey, int repeatCount) { }
	virtual void OnKeyDown (int virtkey) { }
	virtual void OnKeyUp (int virtkey) { }
	virtual void OnPaint (HDC hdc, PAINTSTRUCT &ps) {}
	virtual void OnResize (int cx, int cy) { }
	virtual void OnGotFocus () { }
	virtual void OnLostFocus () { }
	virtual void OnContextMenu () { }
	//virtual void OnCommand (int cmdId, int notifMsg);
	//virtual void OnDropFiles (HDROP hdrop);
	virtual void OnClose (BOOL &cancel) { };
	//virtual LRESULT OnNotify (LPNMHDR lpNotificHdr, WPARAM wParam, LPARAM lParam);
	virtual void OnHScroll (int scrollPos) { }
	virtual void OnVScroll (int scrollPos) { }

public:
	void Init();
	void Update();

	wingui::Window *GetGameWindow();

private:
	EngWindow();


	wingui::Window *mGameWindow;
	wingui::Window *mStatusListView;
};

#endif  // __ENG_WINDOW_H__