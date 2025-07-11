#include "stdafx.h"

#include "game.h"
#include "object.h"
#include "objectTest.h"
#include "timer.h"
#include "CommonConversion.h"
#include "vector.h"
#include "Sun.h"
#include "Ship.h"
#include "Physics\Physics.h"

#include "SceneGraph\ObjectManager.h"
#include "SceneGraph\ObjectManager_DoublyLinkedList.h"

#include "PathNavigation\PathNavigator.h"

#include "EngineGUI\EngWindow.h"
#include "EngineGUI\GameSpace.h"

//--------------------------------------------------------------------------------------------------------------
// Initialise
// Set up the playing field and spawn the player's ship. Returns true if initialisation was successful.
//--------------------------------------------------------------------------------------------------------------

Game& Game::GetInstance()
{
	static Game game;
	return game;
}

Game::Game()
{
	mGameEvents.AddEventListener(this);
}

Game::~Game()
{
	mObjManager->DumpInfo();

	mObjManager->RemoveAll();
	mGameEvents.RemoveEventListener(this);
}

bool Game::Initialise()
{
	// initialize the Object manager.
	mObjManager = ObjectManager_DoublyLinedList::GetInstance();

	// initilize the path navigation system.
	PathNavigation::GetInstance()->Initialize();

	// Generate suns
	int x,y,width, height;
	GetPlayableSpace(x,y,width,height);

	int iXDisplacement = width / (NUM_OF_SUNS + 1);
	int iYDisplacement = height / (NUM_OF_SUNS + 1);
	for (int sunIndex = 0; sunIndex < NUM_OF_SUNS; sunIndex++)
	{
		CVector pos = GetSafeLocationSpawnShip();
		GetObjectManager()->AddObject(new Sun(pos) );
	}

	// Launch the AI Ship
	//for(int i=0; i<5; i++)
	//{
	//	CVector posAI = GetSafeLocationSpawnShip();
	//	CShip* pShipAI = new CShip(posAI);
	//	pShipAI->SetControllerType(eSCT_AI); 

	//	GetObjectManager()->AddObject(pShipAI);
	//}

	for(int i=0; i<3; i++)
		CreateNewShip(eSCT_AI2);

	//for(int i=0; i<5; i++)
	//	CreateNewShip(eSCT_AI);

	// Launch the player ship
	CShip* pShip = CreateNewShip(eSCT_Player);
	// pShip->SetColor(0x0044ff00);

	return true;
}

CShip* Game::CreateNewShip( int aAIType)
{
	CVector pos = GetSafeLocationSpawnShip();
	CShip* pShip = new CShip(pos);
	pShip->SetControllerType( (EShipControllerType)aAIType);

	mObjManager->AddObject(pShip);

	return pShip;
}

CShip* Game::RespawnShip(ulong aShipObjectID)
{
	CVector pos = GetSafeLocationSpawnShip();
	CShip* pShip = (CShip *)mObjManager->GetObject(aShipObjectID);
	
	if(pShip)
		pShip->Spawn(GetSafeLocationSpawnShip());

	return pShip;
}

HDC gHDC = NULL;
HPEN penColor;// = CreatePen(PS_SOLID, 1, aCol);
void Game::Draw(HDC hdc, PAINTSTRUCT* ps)
{
	gHDC = hdc;
	penColor = CreatePen(PS_SOLID, 1, RGB(200,10,200)); // green-gray brush
#if 0
	// draw PathNavigation
	penColor = CreatePen(PS_SOLID, 1, RGB(170,170,170)); // green-gray brush
	SelectObject(hdc, penColor);
	PathNavigation::GetInstance()->DrawNavigationMeshInfo(hdc);
#endif

#if 0
	// draw the Physics Debug Info
	Physics::GetInstance()->DrawDebugInfo();

#endif

	// draw all objects.
	if(mObjManager)
	{
		int iNumObjects = mObjManager->GetCount();
		CCelestialObject** Objects = new CCelestialObject*[iNumObjects];
		mObjManager->GetObjects(-1, Objects );
		for (int iIndex = 0; iIndex < iNumObjects; iIndex ++)
		{
			penColor = CreatePen(PS_SOLID, 1, Objects[iIndex]->GetColor());
		
			SelectObject(hdc, penColor);
			Objects[iIndex]->Draw();
			DeleteObject(penColor);
		}
		delete[] Objects;
	}

	penColor  = CreatePen(PS_SOLID, 1, RGB(0,0,0)); // Black brush
	SelectObject(hdc, penColor);

	DeleteObject(penColor);
	gHDC = NULL;
}

void Game::ChangeDrawColorTo(int aCol )
{
	DeleteObject(penColor);
	penColor = CreatePen(PS_SOLID, 1, aCol);
	SelectObject(gHDC, penColor);
}

void Game::DrawCircle(CVector aPt, float aRadius)
{
	Ellipse(gHDC, int(aPt.x - aRadius), int(aPt.y - aRadius), int(aPt.x + aRadius), int(aPt.y + aRadius));
}

void Game::DrawLine(CVector aStart, CVector aEnd)
{
	MoveToEx(gHDC, int(aStart.x), int(aStart.y), 0);
	LineTo(gHDC, int(aEnd.x), int(aEnd.y) );
}

void Game::DrawRectangle( CVector aStart, CVector aDim)
{
	CVector vEnd = aStart + aDim;
	MoveToEx(gHDC, (int)aStart.x, (int)aStart.y, 0);
	LineTo(gHDC, (int)vEnd.x, (int)aStart.y);
	LineTo(gHDC, (int)vEnd.x, (int)vEnd.y);
	LineTo(gHDC, (int)aStart.x, (int)vEnd.y);
	LineTo(gHDC, (int)aStart.x, (int)aStart.y);
}

void Game::DrawBar( CVector aStart, CVector aDim)
{
	CVector vEnd = aStart + aDim;

	int iDir = 1;
	if(aDim.y < 0.0f)
		iDir = -1;

	MoveToEx(gHDC, (int)aStart.x, (int)aStart.y, 0);
	while((int)aStart.y != (int)vEnd.y)
	{
		LineTo(gHDC, (int)aStart.x, (int)aStart.y);
		LineTo(gHDC, (int)vEnd.x, (int)aStart.y);
		
		aStart.y += iDir;;
	}
}

void Game::printToConsole(std::string *aStr)
{
	// todo
}

ObjectManager *Game::GetObjectManager()
{
	return mObjManager;
}

void Game::GetPlayableSpace(int &aStartX, int &aStartY, int &aWidth, int &aHeight)
{
	GameSpace *pGameWindow = (GameSpace *)EngWindow::GetInstance().GetGameWindow();
	if(pGameWindow)
		pGameWindow->GetPlayArea(aStartX, aStartY, aWidth, aHeight);
}

void Game::Update(bool& outNeedRedraw)
{
	// Update the timer before doing anything else.
	Timer::GetInstance().Update();
	float fDeltaTime = Timer::GetInstance().GetTimeDelta();

	// dispatch all last Frame events.
	mGameEvents.DispatchEvents();

	int iNumObjects = mObjManager->GetCount();
	CCelestialObject** Objects = new CCelestialObject*[iNumObjects];
	mObjManager->GetObjects(-1, Objects );
	for (int iIndex = 0; iIndex< iNumObjects; iIndex ++)
	{
		Objects[iIndex]->Update( fDeltaTime );
		if( Objects[iIndex]->CanBeDeleted() )
		{
			mObjManager->RemoveObject( Objects[iIndex] );
			Objects[iIndex] = NULL;
		}
	}
	delete[] Objects;

	// process physics.
	engPhysics::Physics::GetInstance()->Update( fDeltaTime );

	// Check if we need a redraw
	static float fNextDraw = 0.f;
	fNextDraw -= fDeltaTime;
	if (fNextDraw < 0.f)
	{
		fNextDraw = DRAW_TIME;
		outNeedRedraw = true;
	}
	else
	{
		outNeedRedraw = false;
	}
}

CVector Game::GetSafeLocationSpawnShip()
{
	int x,y,width, height;
	GetPlayableSpace(x,y,width,height);
	
	int iAttempts = 0;
	do
	{
		CVector safePos = CVector((float)RandomRange(0 + 50, width - 50), (float)RandomRange(0 + 50, height - 50));
		if( PathNavigation::GetInstance()->IsPositionSafe(safePos) )
			return safePos;

		iAttempts ++;

	}while(iAttempts < 50);	

	return CVector(-1,-1);
}

EventHandler& Game::GetGameEventListener()
{
	return mGameEvents;
}

bool Game::OnEvent(EventBase* aEvent)
{
	if(ShipExplodeEvent *shipExplodeEvt = dynamic_cast<ShipExplodeEvent *>(aEvent) )
	{
		//new TimerEvent_CreateShip( 5, shipExplodeEvt->mControllerType );
		new TimerEvent_RespawnShip( 5, shipExplodeEvt->mID );
	}
	return false;
}