

#ifndef __SHIPCONTROLLER_PLAYER_H__
#define __SHIPCONTROLLER_PLAYER_H__

#include "ShipControllerBase.h"

class CShipControlleerPlayer : public CShipControllerBase
{
public:
	CShipControlleerPlayer(CShip *);
	virtual ENextMove ThinkNextMove();
	virtual bool OnEvent(EventBase*);
};

#endif // __SHIPCONTROLLER_PLAYER_H__