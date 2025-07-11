#include "stdafx.h"
#include <wingui/Container.h>
#include <wingui/globals.h>

namespace wingui {

#define kSplitSize			5

Container::Container (Window *pWindow, LayoutOrder order, BOOL resiable) :
	_pWindow(pWindow),
	_resizable(_resizable),	
	_autoResiableLayout(-1),
	_order(order),
	_flagSplit(false),
	_splitter(NULL)
{
	_layouts[0] = NULL;
	_layouts[1] = NULL;
	memset (&_frame, 0, sizeof(_frame));
}

void Container::SetLayouts (ILayout *pLayout1, ILayout *pLayout2)
{
	_layouts[0] = pLayout1;
	_layouts[1] = pLayout2;
}

void Container::SetAutoResizableLayout(int index)
{
	WINGUI_ASSERT(index >= -1 && index <= 1);
	_autoResiableLayout = index;
}

void Container::DoLayout ()
{
	if (_order == kLayoutOrderHorz) 
	{
		// get total width available and total width required for horizontal layout
		int maxWidth = _frame.Width();
		int totalWidth = _layouts[0]->GetFrame().Width() + \
						_layouts[1]->GetFrame().Width() + \
						kSplitSize;

		// resize layouts if required
		if (totalWidth < maxWidth) {
			int diffWidth = maxWidth - totalWidth;
			// If we have an auto resizable layout, resize it to fit the remaining area.
			if (_autoResiableLayout != -1 && _layouts[_autoResiableLayout]->IsResizable())
			{
				Frame frame = _layouts[_autoResiableLayout]->GetFrame();
				frame.x2+= diffWidth;
				_layouts[_autoResiableLayout]->SetFrame(frame);
			}
			else
			{
				// enlarge the last resizable layout to fit the remaining area
				for (int i = 1; i >= 0; i--) {			
					if (_layouts[i]->IsResizable()) {
						Frame frame = _layouts[i]->GetFrame();
						frame.x2+= diffWidth;
						_layouts[i]->SetFrame(frame);
						break;
					}
				}
			}
		} 
		else 
		{
			int diffWidth = totalWidth - maxWidth;
			// If we have an auto resizable layout, resize it to fit the remaining area.
			if (_autoResiableLayout != -1 && _layouts[_autoResiableLayout]->IsResizable())
			{
				Frame frame = _layouts[_autoResiableLayout]->GetFrame();
				// check for minimum layout size
				if ( ((frame.x2 - diffWidth) - frame.x1 + 1) >= kMinLayoutSize ) {
					frame.x2-= diffWidth;
					_layouts[_autoResiableLayout]->SetFrame(frame);
				}
			}
			else
			{
				// resize the last resizable layout
				for (int i = 1; i >= 0; i--) {
					if (_layouts[i]->IsResizable()) {
						Frame frame = _layouts[i]->GetFrame();
						// check for minimum layout size
						if ( ((frame.x2 - diffWidth) - frame.x1 + 1) >= kMinLayoutSize ) {
							frame.x2-= diffWidth;
							_layouts[i]->SetFrame(frame);
						}
						break;
					}
				}
			}
		}

		// set layout frames
		int x = _frame.x1;
		for (int i = 0; i < 2; i++) {
			Frame layoutFrame = _layouts[i]->GetFrame();
			Frame frame (x, _frame.y1, x + layoutFrame.Width() - 1, _frame.y2);
			_layouts[i]->SetFrame (frame);
			x = frame.x2 + kSplitSize;
		}
	}
	else
	{
		// get total height available and total height required for vertical layout
		int maxHeight = _frame.Height();
		int totalHeight = _layouts[0]->GetFrame().Height() + \
						_layouts[1]->GetFrame().Height() + \
						kSplitSize;

		// resize layouts if required
		if (totalHeight < maxHeight) {
			int diffHeight = maxHeight - totalHeight;
			// enlarge the last resizable layout
			for (int i = 1; i >= 0; i--) {
				if (_layouts[i]->IsResizable()) {
					Frame frame = _layouts[i]->GetFrame();
					frame.y2+= diffHeight;
					_layouts[i]->SetFrame(frame);
					break;
				}
			}
		} else {
			int diffHeight = totalHeight - maxHeight;
			// resize the first resizable layout
			for (int i = 1; i > 0; i--) {
				if (_layouts[i]->IsResizable()) {
					Frame frame = _layouts[i]->GetFrame();
					// check for minimum layout size
					if ( ((frame.y2 - diffHeight) - frame.y1 + 1) >= kMinLayoutSize ) {
						frame.y2-= diffHeight;
						_layouts[i]->SetFrame(frame);
					}
					break;
				}
			}
		}

		// set layout frames
		int y = _frame.y1;
		for (int i = 0; i < 2; i++) {
			Frame layoutFrame = _layouts[i]->GetFrame();
			Frame frame (_frame.x1, y, _frame.x2, y + layoutFrame.Height() - 1);
			_layouts[i]->SetFrame (frame);
			y = frame.y2 + kSplitSize;
		}
	}

	// if layout is a container call DoLayout on it
	for (int i = 0; i < 2; i++) {
		if (_layouts[i]->IsContainer()) {
			reinterpret_cast<IContainer *>(_layouts[i])->DoLayout ();
		}
	}
}

void Container::HandleMouseDownEvent (int shift, int button, int x, int y)
{
	// check if mouse is over splitter rect
	int x1, y1, x2, y2;
	GetSplitterRect (x1, y1, x2, y2);	

	if (x >= x1 && x <= x2 && y >= y1 && y < y2) 
	{
		if (_layouts[0]->IsResizable() && _layouts[1]->IsResizable()) 
		{
			ILayout *pLayout = _layouts[0];			
			_startFrames[0] = _layouts[0]->GetFrame();
			_startFrames[1] = _layouts[1]->GetFrame();
			_endFrames[0] = _startFrames[0];
			_endFrames[1] = _startFrames[1];
			_startX = x;
			_startY = y;

			// create a spliter and draw it			
			RECT splitterRect;
			SetRect (&splitterRect, x1, y1, x2, y2);
			HDC hdc = GetDC(_pWindow->GetHandle());
			_splitter = new Splitter (splitterRect, _order == kLayoutOrderHorz, hdc);
			_splitter->Draw();

			SetCursor(_order == kLayoutOrderHorz ? gCurHSplit : gCurVSplit);
			SetCapture(_pWindow->GetHandle());
			_flagSplit = TRUE;
			return;
		}
	}

	for (int i = 0; i < 2; i++) {
		if (_layouts[i]->IsContainer()) {
			reinterpret_cast<IContainer *>(_layouts[i])->HandleMouseDownEvent (shift, button, x, y);
		}
	}
}

void Container::HandleMouseUpEvent (int shift, int button, int x, int y)
{
	if (_flagSplit == TRUE) 
	{		
		// erase the splitter by redrawing it and release it
		_splitter->Draw();
		ReleaseDC(_pWindow->GetHandle(), _splitter->GetDC());
		delete _splitter;
		_splitter = NULL;
		
		_layouts[0]->SetFrame (_endFrames[0]);
		if (_layouts[0]->IsContainer()) {
			reinterpret_cast<IContainer *>(_layouts[0])->DoLayout();
		}							
		_layouts[1]->SetFrame (_endFrames[1]);
		if (_layouts[1]->IsContainer()) {
			reinterpret_cast<IContainer *>(_layouts[1])->DoLayout();
		}

		ReleaseCapture();
		_flagSplit = FALSE;
	} 
	else 
	{
		for (int i = 0; i < 2; i++) {
			if (_layouts[i]->IsContainer()) {
				reinterpret_cast<IContainer *>(_layouts[i])->HandleMouseUpEvent (shift, button, x, y);
			}
		}
	}
}

void Container::HandleMouseMoveEvent (int shift, int button, int x, int y)
{
	if (_flagSplit)
	{		
		Frame f1 = _startFrames[0];
		Frame f2 = _startFrames[1];

		if (_order == kLayoutOrderHorz) 
		{
			int diffX = x - _startX;

			// resize 1st layout
			if ( 
				(((f1.x2 + diffX) - f1.x1 + 1) >= kMinLayoutSize) &&
				((f2.x2 - (f2.x1 + diffX) + 1) >= kMinLayoutSize)
				)
			{
				// erase the splitter by redrawing it
				_splitter->Draw();

				f1.x2+= diffX;
				f2.x1+= diffX;	
				_endFrames[0] = f1;
				_endFrames[1] = f2;

				// move the splitter and redraw it
				_splitter->Offset(diffX);
				_splitter->Draw();
			}
		}
		else
		{
			int diffY = y - _startY;

			// resize 1st layout
			if ( 
				(((f1.y2 + diffY) - f1.y1 + 1) >= kMinLayoutSize) &&
				((f2.y2 - (f2.y1 + diffY) + 1) >= kMinLayoutSize)
				)
			{
				// erase the splitter by redrawing it
				_splitter->Draw();

				f1.y2+= diffY;
				f2.y1+= diffY;				
				_endFrames[0] = f1;
				_endFrames[1] = f2;

				// move the splitter and redraw it
				_splitter->Offset(diffY);
				_splitter->Draw();
			}
		}
	}
	else
	{
		// check if mouse is over splitter rect
		int x1, y1, x2, y2;
		GetSplitterRect (x1, y1, x2, y2);	

		if (x >= x1 && x <= x2 && y >= y1 && y < y2) {
			if (_layouts[0]->IsResizable() && _layouts[1]->IsResizable()) {
				SetCursor(_order == kLayoutOrderHorz ? gCurHSplit : gCurVSplit);
				return;
			}
		}

		for (int i = 0; i < 2; i++) {
			if (_layouts[i]->IsContainer()) {
				reinterpret_cast<IContainer *>(_layouts[i])->HandleMouseMoveEvent (shift, button, x, y);
			}
		}
	}
}

void Container::GetSplitterRect (int &x, int &y, int &x2, int &y2)
{	
	if (_order == kLayoutOrderHorz) {
		y = _frame.y1;
		y2 = _frame.y1 + _frame.y2;
		Frame f = _layouts[0]->GetFrame();
		x = f.x2;
		x2 = x + kSplitSize;
	} else {
		x = _frame.x1;
		x2 = _frame.x1 + _frame.x2;
		Frame f = _layouts[0]->GetFrame();
		y = f.y2;
		y2 = y + kSplitSize;
	}
}

}; // wingui namespace
