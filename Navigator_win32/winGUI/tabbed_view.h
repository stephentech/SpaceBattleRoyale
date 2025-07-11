#ifndef __WINGUI_TABBED_VIEW_CONTAINER_H__
#define __WINGUI_TABBED_VIEW_CONTAINER_H__

#include "layout.h"
#include "Tabstrip.h"
#include "gdi.h"
#include <map>

namespace wingui {

class EmptyView : public View
{
public:
	virtual void GetClassStyle (WNDCLASSEX &wcex);
	virtual void OnCreate ();
	virtual void OnDestroy ();
	virtual void OnPaint (HDC hdc, PAINTSTRUCT &ps);
private:
	Font	_font;
};

#define kTabstripHeight						24

class TabbedView : public View, public TabStripEvents
{
public:
	struct Tab
	{
		Tab (View *pView_, int id_, LPCTSTR lpszTitle_, BOOL autoDelete_ = TRUE) :
			id(id_),
			pView(pView_),
			title(lpszTitle_),
			autoDelete(autoDelete_)
		{
		}
		View			*pView;
		int				id;
		wingui_tstring	title;
		BOOL			autoDelete;
	};
	typedef std::vector<Tab> ViewMap;

	TabbedView ();
	virtual ~TabbedView ();

	inline const ViewMap &GetViews() { return m_views; }

	void AddView (View *pView, LPCTSTR lpszTitle, int viewId, BOOL autoDelete = TRUE);
	void CloseView (View *pView);
	void CloseAllViews ();
	void SelectView (View *pView);
	View *GetCurrentView ();

	virtual void UpdateUI();

protected:
	virtual void OnCreate ();
	virtual void OnResize (int cx, int cy);

	void DeleteAllViews();	
	int FindView(int id) const;
	int FindView(View *pView) const;
	void ResizeViews ();	

	virtual void GetViewRect(RECT &rect);
	virtual void DeleteView (View *pView);

	// TabStrip events...
	virtual void TabStrip_OnSelectTab (TabStrip *sender, int index);
	virtual void TabStrip_OnDeselectTab (TabStrip *sender, int index);
	virtual void TabStrip_OnContextMenu (TabStrip *sender, int indx);

protected:
	ViewMap			m_views;
	TabStrip		m_tabstrip;
	EmptyView		m_emptyView;	
};

};// wingui namespace

#endif // !__WINGUI_TABBED_VIEW_CONTAINER_H__
