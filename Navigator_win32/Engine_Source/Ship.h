
#ifndef __SHIP_H__
#define __SHIP_H__

#include "object.h"
#include "Event\EventBase.h"
#include "vector"

// ship events.
class ShipSpawnEvent : public EventBase
{
};

class ShipExplodeEvent : public EventBase
{
public:
	ShipExplodeEvent(int aID, int aControllerType)
		: mID(aID)
		, mControllerType(aControllerType)
	{
	}

	ulong	mID;
	int		mControllerType;
};

//-------------------------------------------------------------------------------------------------------------
// Ship
// Controlled by the Player/AI/Network.
//-------------------------------------------------------------------------------------------------------------

// the ship steps to steering.
#define STEERING_ANGLE 2000

// every frame the ship needs to be given any one/multiple commands 
enum ENextMove
{
	eNM_DoNothing		= 1 << 0,
	eNM_SteerLeft		= 1 << 1, // steer left  by STEERING_ANGLE degrees respective to the facing Vector.
	eNM_SteerRight		= 1 << 2, // steer right by STEERING_ANGLE degrees respective to the facing Vector.
	eNM_Deaccelerate	= 1 << 3,
	eNM_Accelerate		= 1 << 4,
	eNM_FireBullet		= 1 << 5,
	eNM_FireMissile		= 1 << 6
};

enum EShipControllerType
{
	eSCT_None = 0, // Abandon pilot.
	eSCT_Player,
	eSCT_AI,
	eSCT_AI2,
	eSCT_PlayerNetwork
};

/// fwd decl
class CShipControllerBase;

class CShip : public CCelestialObject
{
public:
	CShip(CVector);
	~CShip();

	virtual void Update(float);
	virtual void Draw();
	virtual int GetColor();
	virtual bool CanBeDeleted();
	virtual void CollidedWith(CCelestialObject*);
	
	void DrawHealth();

	ulong GetClosestShipInSight();
	std::vector<ulong> GetShipsInSight();

	// querry whether this ship id is withing my FOV.
	bool IsShipVisible(ulong);

	bool IsDestroyed();
	void Spawn(CVector aPos);
	void Explode();

	// set the controller type. fall back to basic AI.
	void SetControllerType(EShipControllerType);
	EShipControllerType GetControllerType() { return mControllerType; }

	// create an Object of Controller and pass in to the ship.
	void SetControllerType(EShipControllerType, CShipControllerBase*);

	CVector GetFacingDir() {return mFacingDir;}
private:
	void TurnShip(float aSteering, float aDT);


public:
	bool mCollidedWithAnotherShipDisplacementDone;

private:
	EShipControllerType mControllerType;
	CShipControllerBase* mController;
	bool mExplosionEventGenerated;

	CVector mFacingDir;
	float mShieldRadius;
	float mShipPhysicsRadius;
	unsigned int mColor;

	float mCoolDownTimeBullet; // if < 0, then can fire next bullet.
	float mCoolDownTimeMissile; // if < 0, then can fire next missile.
	int mCountMissileLeft; // limited missiles in the ship.

	// ship Attributes : all factors have to be Positive.
	float mAcceleration;
	float mDeacceleration;
	float mMinHealthSpeed;
	float mMaxSpeed;
	float mHealth;
	float mShieldHealth;

	// Field of View
	float mFOVAngle;
	float mFOVDist;

	// limitations to a ship
	float mMaxAngleTurnInOneSec;
	float mAgility; // turning capablity. (0-1)

	//weapons Attributes :
	float mBulletFireRate; // per min
	float mMissileFireRate; // per min
};


#endif // __SHIP_H__