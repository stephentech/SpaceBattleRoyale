
#ifndef __SHIPCONTROLLER_AI_H__
#define __SHIPCONTROLLER_AI_H__

#include "ShipControllerBase.h"

class CShipControllerAI : public CShipControllerBase
{
public:
	enum ECorner
	{
		TOP_LEFT = 0,
		TOP_RIGHT,
		BOTTOM_RIGHT,
		BOTTOM_LEFT
	};

	CShipControllerAI(CShip*);

	virtual ENextMove ThinkNextMove();
	virtual bool OnEvent(EventBase*);

	ECorner mCurrentCornerToReach;
	//CVector mNextPt;

	virtual void DrawControllerInfo();

	int mPathId;
};

#endif // __SHIPCONTROLLER_AI_H__