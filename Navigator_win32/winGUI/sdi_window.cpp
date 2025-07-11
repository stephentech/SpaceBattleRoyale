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
#include "stdafx.h"
#include <assert.h>
#include <commdlg.h>
#include <wingui/sdi_window.h>

extern HINSTANCE _hAppInstance;

namespace wingui {

SDIWindow::SDIWindow (int resourceId) 
{
	m_showToolbar = TRUE;
	m_showStatusbar = TRUE;

	m_resourceId = resourceId;

	m_isMainWnd = TRUE;
	m_pContainer = NULL;
}

SDIWindow::~SDIWindow () 
{
}

/* operations */
void SDIWindow::ShowToolbar (BOOL flag) 
{
	if (m_showToolbar != flag) {
		if (m_showToolbar = flag) {

		} else {

		}
	}
}

void SDIWindow::ShowStatusbar (BOOL flag) 
{
	if (m_showStatusbar != flag) {
		if (m_showStatusbar = flag) {

		} else {

		}
	}
}

BOOL SDIWindow::GetOpenFilePath (LPTSTR filePathBuffer, int maxFilePathBuffer, LPCTSTR lpstrFilter, 
									int filterIndex, LPCTSTR lpszDefExt)
{
	filePathBuffer[0] = _T('\0');

	OPENFILENAME openFileStruct;
	memset (&openFileStruct, 0, sizeof(openFileStruct));
	openFileStruct.lStructSize = sizeof(openFileStruct);
	openFileStruct.hwndOwner = GetHandle ();
	openFileStruct.hInstance = _hAppInstance;
	openFileStruct.lpstrFilter = lpstrFilter;
	openFileStruct.nFilterIndex = filterIndex;
	openFileStruct.lpstrFile = filePathBuffer;
	openFileStruct.nMaxFile = maxFilePathBuffer;
	openFileStruct.lpstrInitialDir = NULL;
	openFileStruct.lpstrTitle = NULL;
	openFileStruct.Flags = OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	openFileStruct.lpstrDefExt = lpszDefExt;

	BOOL ret = GetOpenFileName (&openFileStruct);
	return ret;
}

BOOL SDIWindow::GetSaveAsFilePath (LPTSTR filePathBuffer, int maxFilePathBuffer, 
									LPCTSTR lpstrFilter, int filterIndex, 
									LPCTSTR lpszDefExt)
{
	OPENFILENAME openFileStruct;

	filePathBuffer[0] = _T('\0');

	memset (&openFileStruct, 0, sizeof(openFileStruct));
	openFileStruct.lStructSize = sizeof(openFileStruct);
	openFileStruct.hwndOwner = GetHandle ();
	openFileStruct.hInstance = _hAppInstance;
	openFileStruct.lpstrFilter = lpstrFilter;
	openFileStruct.nFilterIndex = filterIndex;
	openFileStruct.lpstrFile = filePathBuffer;
	openFileStruct.nMaxFile = maxFilePathBuffer;
	openFileStruct.lpstrInitialDir = NULL;
	openFileStruct.lpstrTitle = NULL;
	openFileStruct.Flags = OFN_ENABLESIZING | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
	openFileStruct.lpstrDefExt = lpszDefExt;

	return ::GetSaveFileName (&openFileStruct);
}

void SDIWindow::SetContainer (IContainer *pContainer)
{
	m_pContainer = pContainer;
}

void SDIWindow::UpdateUI () 
{
}

// overridables
void SDIWindow::GetClassStyle (WNDCLASSEX &wcex) 
{
	// fill in defaults
	Window::GetClassStyle (wcex);

	wcex.lpszClassName = _T("_gls_main_window");
	wcex.lpszMenuName = MAKEINTRESOURCE(m_resourceId);
	wcex.hIcon = LoadIcon (_hAppInstance, MAKEINTRESOURCE(m_resourceId));
	wcex.hIconSm = LoadIcon (_hAppInstance, MAKEINTRESOURCE(m_resourceId));
}

BOOL SDIWindow::CreateToolbar () 
{
	return TRUE;
}

// protected
BOOL SDIWindow::CreateStatusbar () 
{
	return TRUE;
}

void SDIWindow::OnCreate () 
{
	Window::OnCreate ();

	UpdateUI ();
}

void SDIWindow::OnResize (int cx, int cy) 
{
	Window::OnResize (cx, cy);

	if (m_pContainer != NULL)
	{
		ILayout::Frame frame(0, 0, cx, cy);
		m_pContainer->SetFrame (frame);
		m_pContainer->DoLayout();
	}
}

void SDIWindow::OnMouseDown (int shift, int button, int x, int y)
{
	if (m_pContainer) {
		m_pContainer->HandleMouseDownEvent (shift, button, x, y);
		SetCapture (m_hWnd);
	}
}

void SDIWindow::OnMouseMove (int shift, int button, int x, int y)
{
	if (m_pContainer) {
		m_pContainer->HandleMouseMoveEvent (shift, button, x, y);
	}
}

void SDIWindow::OnMouseUp (int shift, int button, int x, int y)
{
	if (m_pContainer) {
		m_pContainer->HandleMouseUpEvent (shift, button, x, y);
		ReleaseCapture ();
	}
}

}; // wingui namesapce
