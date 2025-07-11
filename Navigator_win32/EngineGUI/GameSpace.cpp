
#include <stdafx.h>
#include "GameSpace.h"

void GameSpace::OnPaint (HDC hdc, PAINTSTRUCT &ps)
{
	Game::GetInstance().Draw(hdc, &ps);
}

void GameSpace::GetCreateStyle (DWORD &dwStyle, DWORD &dwExStyle) 
{
	dwStyle = WS_THICKFRAME | WS_CHILD;
	dwExStyle = 0;
}

void GameSpace::Invalidate()
{
	RECT rect;
	GetWindowRect(rect);
	rect.top = rect.left = 0;
	InvalidateRect(GetHandle(), &rect, TRUE);
}

void GameSpace::GetPlayArea(int &aStartX, int &aStartY, int &aWidth, int &aHeight)
{
	RECT rect;
	GetWindowRect(rect);

	const int iFrameThickness = 12;

	aStartX = 0;//rect.left;
	aStartY = 0;//rect.top;
	aWidth = rect.right - rect.left - iFrameThickness;
	aHeight = rect.bottom - rect.top - iFrameThickness;
}