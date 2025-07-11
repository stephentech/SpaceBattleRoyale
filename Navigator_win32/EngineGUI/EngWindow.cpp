#include <stdafx.h>

#include "EngWindow.h"
#include "GameSpace.h"
#include "winGUI\list_view.h"

EngWindow::EngWindow()
	: mGameWindow(NULL)
	, mStatusListView(NULL)
{
}

EngWindow& EngWindow::GetInstance()
{
	static EngWindow engWindow;
	return engWindow;
}

void EngWindow::Init()
{
	// hope this is not the second call here.
	assert(!mGameWindow);
	const int iMarginConstantX = 50;

	mGameWindow = new GameSpace();

	// assert if the game draw space is a fail.
	assert(mGameWindow);

	mGameWindow->Create(X_SAFEREGION_MIN, Y_SAFEREGION_MIN, X_SAFEREGION_MAX, Y_SAFEREGION_MAX, NULL, this);
	mGameWindow->Show();

	mStatusListView = new wingui::ListView();
	mStatusListView->Create(X_SAFEREGION_MIN + X_SAFEREGION_MAX + iMarginConstantX, Y_SAFEREGION_MIN, 200, Y_SAFEREGION_MAX, NULL, this);
	mStatusListView->Show();

	bool bGameInit = Game::GetInstance().Initialise();
	assert(bGameInit);
}


void EngWindow::Update ()
{
	// Update the game
	if(mGameWindow)
	{
		bool needRedraw;
		Game::GetInstance().Update(needRedraw);
		if (needRedraw)
		{
			((GameSpace *)mGameWindow)->Invalidate();
		}
	}
}

wingui::Window *EngWindow::GetGameWindow()
{
	return mGameWindow;
}

////////////////////////////////////////////
// on Window event.
void EngWindow::OnPreDestroy () 
{ 
}

void EngWindow::OnDestroy() 
{
	if(mGameWindow)
	{
		mGameWindow->Destroy();
		delete mGameWindow;

		mGameWindow = NULL;
	}

	PostQuitMessage(0);
}

