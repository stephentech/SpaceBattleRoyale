
#ifndef __GAME_H__
#define __GAME_H__

#include <list>
#include "timer.h"
#include "Event\EventBase.h"

// Externally defined classes.
class CCelestialObject;
class ObjectManager;
class CShip;

class Game : public EventListener
{
public:
	static Game& GetInstance();
	~Game();

	bool Initialise();
	void Update(bool& outNeedRedraw);
	void Draw(HDC hdc, PAINTSTRUCT* ps);
	void printToConsole(std::string *);

	CVector GetSafeLocationForPlacingNewSun();
	CVector GetSafeLocationSpawnShip();
	ObjectManager *GetObjectManager();
	void GetPlayableSpace(int &aStartX, int &aStartY, int &aWidth, int &aHeight);

	virtual bool OnEvent(EventBase*);
	EventHandler& GetGameEventListener();

	// helper functions
	CShip* CreateNewShip(int aAIType);
	CShip* Game::RespawnShip( ulong aShipObjectID );

	// Debug draw calls
	void ChangeDrawColorTo(int);
	void DrawCircle(CVector aPt, float aRadius);
	void DrawLine( CVector aStart, CVector aEnd);
	void DrawRectangle(CVector aStart, CVector aDim);
	void DrawBar( CVector aStart, CVector aDim);
	

private:
	Game();
	void AddObjectsIntoWorld();

	EventHandler mGameEvents;
	ObjectManager *mObjManager;
};


extern const int NUM_OF_SUNS;

extern const int X_SAFEREGION_MIN;
extern const int X_SAFEREGION_MAX;
extern const int Y_SAFEREGION_MIN;
extern const int Y_SAFEREGION_MAX;
extern const int PLACE_ATTEMPTS_PER_SUN;
extern const float MINIMUM_DISTANCE_BETWEEN_SUNS;
extern const float DRAW_TIME;

#endif // __GAME_H__