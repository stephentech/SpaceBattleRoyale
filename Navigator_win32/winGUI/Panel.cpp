#include "stdafx.h"
#include <wingui/Panel.h>
#include <assert.h>

namespace wingui {

// Panel

Panel::Panel (LayoutOrder layoutOrder, BOOL resizable, Window *pParentWindow) :
	m_layoutOrder(layoutOrder),
	m_resizable(resizable)
{
}

void Panel::AddBar(PanelBar *pPanelBar)
{
	m_views.push_back(pPanelBar);
	AddSubView(pPanelBar);
}

void Panel::AddBars (PanelBar *pPanelBars[], int count)
{
	for (int i = 0; i < count; i++) {
		AddBar(pPanelBars[i]);
	}
}

void Panel::RemoveBar(PanelBar *pPanelBar)
{
	std::vector<PanelBar *>::iterator it = FindBar(pPanelBar);
	assert(it != m_views.end());
	m_views.erase(it);
	pPanelBar->Hide();
	RemoveSubView(pPanelBar);
	DoLayout();
}

void Panel::OnResize(int cx, int cy)
{
	UpdateLayout();
	wingui::View::OnResize(cx, cy);
}

void Panel::UpdateLayout (PanelBar *pPanelBarHint)
{
	const int kSpacing = 1;
	const int kOffsetY = 0;

	int offsetX = 0;
	int offsetY = 0;

	RECT rcClient;
	GetClientRect(rcClient);

	for (std::vector<PanelBar *>::iterator it = m_views.begin(); it != m_views.end(); it++)
	{
		PanelBar *pPanelBar = *it;
		if (!pPanelBar->IsVisible()) {
			continue;
		}

		RECT rcPanelBar;
		pPanelBar->GetWindowRect(rcPanelBar);
		int panelWidth = rcPanelBar.right - rcPanelBar.left;
		int panelHeight = rcPanelBar.bottom - rcPanelBar.top;

		if (m_layoutOrder == kLayoutOrderHorz) {
			pPanelBar->Move (offsetX, offsetY, panelWidth, rcClient.bottom, true);
			offsetX+= panelWidth + kSpacing;
		} else {
			pPanelBar->Move (offsetX, offsetY, rcClient.right, panelHeight, true);
			offsetY+= panelHeight + kSpacing;
		}

		pPanelBar->RedrawView();
	}
}

std::vector<PanelBar *>::iterator Panel::FindBar (PanelBar *pPanelBar)
{
	for (std::vector<PanelBar *>::iterator it = m_views.begin();
		 it != m_views.end();
		 it++)
	{
		if (*it == pPanelBar) {
			return it;
		}
	}
	return m_views.end();
}

}; // wingui namespace
