
#ifndef __SHIPCONTROLLER_AI2_H__
#define __SHIPCONTROLLER_AI2_H__

#include "ShipControllerBase.h"

class CShipControllerAI2 : public CShipControllerBase
{
public:

	CShipControllerAI2(CShip*);
	~CShipControllerAI2();

	virtual ENextMove ThinkNextMove();

	virtual void DetectedAttack(CVector);

	virtual bool OnEvent(EventBase*);

	virtual void DrawControllerInfo();
private:
	enum EAI2State
	{
		eAI2State_Find = 0,
		eAI2State_FindAttacker,
		eAI2State_Attack,
		eAI2State_Defend,
		eAI2State_DangerZone,

		eAI2State_MAX
	};

	// path related stuff
	void		CreateNewPath( CVector aToPos );
	ENextMove	MoveOnPath();
	void		InvalidatePath();

	int mPathId;
	EAI2State mAIState;
	ulong mShipIDToAttack;

	CVector mLastKnownAttackPt;
};

#endif // __SHIPCONTROLLER_AI_H__