//
// 
// 
// This file is part of glScript project.
//
//   glScript is free software: you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   glScript is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Lesser General Public License for more details.
//
//   You should have received a copy of the GNU Lesser General Public License
//   along with glScript. If not, see <http://www.gnu.org/licenses/>.
//
//   Copyright (c) 2012 Bal Bahadur Chettri
//
//   Author: Bal Bahadur Chettri (balu.chettri@gmail.com)
//
#ifndef __wingui_main_window_h
 #define __wingui_main_window_h

#include "window.h"
#include "Container.h"

namespace wingui {

/* An SDI style main window for implementing app's main window */
class SDIWindow : public Window {
public:
	/* ctors */
	
	// default construction
	SDIWindow (int resourceId);

	virtual ~SDIWindow ();

public:
	// operations
	void ShowToolbar (BOOL flag = TRUE);
	void ShowStatusbar (BOOL flag = TRUE);

	BOOL GetOpenFilePath (LPTSTR filePathBuffer, int maxFilePathBuffer, LPCTSTR lpstrFilter, 
							int filterIndex, LPCTSTR lpszDefExt);

	BOOL GetSaveAsFilePath (LPTSTR filePathBuffer, int maxFilePathBuffer, 
								LPCTSTR lpstrFilter, int filterIndex, LPCTSTR lpszDefExt);

	void SetContainer (IContainer *pContainer);

	// overridables
	virtual void GetClassStyle (WNDCLASSEX &wcex);
	virtual void UpdateUI ();

protected:
	BOOL CreateToolbar ();
	BOOL CreateStatusbar ();

protected:
	// message handlers
	virtual void OnCreate ();
	virtual void OnResize (int cx, int cy);
	virtual void OnMouseDown (int shift, int button, int x, int y);
	virtual void OnMouseMove (int shift, int button, int x, int y);
	virtual void OnMouseUp (int shift, int button, int x, int y);

protected:
	// data members
	BOOL			m_showToolbar;
	BOOL			m_showStatusbar;
	int				m_resourceId;
	IContainer		*m_pContainer;
};

}; // namespace wingui

#endif /* __wingui_main_window_h */
