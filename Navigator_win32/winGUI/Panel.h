#ifndef __WINGUI_PANEL_LAYOUT_H__
#define __WINGUI_PANEL_LAYOUT_H__

#include "Layout.h"
#include "view.h"
#include <vector>

namespace wingui {

class Panel;

class PanelBar : public View
{
public:
	enum State
	{
		kStateCollapsed,
		kStateExpanded
	};

	PanelBar (int panelResId, Panel *pParentLayout);
	PanelBar (View *pPanelView, BOOL bDeleteView, Panel *pParentLayout);
	~PanelBar ();

	virtual BOOL Create (LPCTSTR lpszTitle, Window *pParent);

	inline State GetState() const { return m_state; }
	inline View *GetPanelView() { return m_pPanelView; }

	void Expand();
	void Collapse();

	virtual void UpdateUI()
	{
		m_pPanelView->UpdateUI();
	}
	virtual void OnCreate();
	virtual void OnResize(int cx, int cy);
	virtual void OnPaint (HDC hdc, PAINTSTRUCT &ps);
	virtual void OnMouseDown (int shift, int button, int x, int y);

protected:
	// override to create custom panel view
	virtual View *CreatePanelView()
	{
		return new View();
	}

private:
	int				m_panelResId;
	Panel			*m_pParentLayout;
	View			*m_pPanelView;
	BOOL			m_deletePanelView;
	State			m_state;
	HWND			m_titleLabel;
	HWND			m_toggleButton;
	SIZE			m_sizeExpanded;
	SIZE			m_sizeCollapsed;
};

class Panel : public View
{
public:
	enum LayoutOrder
	{
		kLayoutOrderHorz,
		kLayoutOrderVert
	};

	Panel (LayoutOrder layoutOrder, BOOL resizable, Window *pParentWindow);

	void AddBar (PanelBar *pPanelBar);
	void AddBars (PanelBar *pPanelBars[], int count);
	void RemoveBar (PanelBar *pPanelBar);
	void RemoveAllBars (PanelBar *pPanelBar);
	void ExpandAllBars ();
	void CollapseAllBars ();
	inline void DoLayout () { UpdateLayout(); }

protected:
	virtual void OnResize(int cx, int cy);

	void UpdateLayout (PanelBar *pPanelBarHint = NULL);	
	std::vector<PanelBar *>::iterator FindBar (PanelBar *pPanelBar);

private:
	LayoutOrder				m_layoutOrder;
	BOOL					m_resizable;
	std::vector<PanelBar *>	m_views;
	Frame					m_frame;

	friend class PanelBar;
};

}; // wingui namespace

#endif // !__WINGUI_PANEL_LAYOUT_H__
