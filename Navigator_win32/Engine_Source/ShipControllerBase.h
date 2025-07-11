

#ifndef __SHIPCONTROLLER_H__
#define __SHIPCONTROLLER_H__

#include "Ship.h"

// The Brains.
// this is the AI class giving orders to the Ship.
class CShipControllerBase : public EventListener
{
public:
	CShipControllerBase(CShip *aShip)
		: mShipControllerType(eSCT_AI)
	{
		mShip = aShip;
		Game::GetInstance().GetGameEventListener().AddEventListener(this);
	}

	~CShipControllerBase()
	{
		mShipControllerType = eSCT_None;
		mShip = NULL;
		Game::GetInstance().GetGameEventListener().RemoveEventListener(this);
	}

	// analyse/think and return one or more ENextMove.
	virtual ENextMove ThinkNextMove() = 0;

	// will be called from ship indicating an attack on the Vessel. Will provide the direction wrt to the vessel.
	virtual void DetectedAttack(CVector) {}

	// Can draw debug info
	virtual void DrawControllerInfo() {}

	EShipControllerType mShipControllerType;
	CShip* mShip;
};


#endif // __SHIPCONTROLLER_H__