
#ifndef __WEAPON_EXPLOSIVE_H__
#define __WEAPON_EXPLOSIVE_H__

#include "object.h"

//-------------------------------------------------------------------------------------------------------------
// CMissile
// Fired by a player ship.
//-------------------------------------------------------------------------------------------------------------
class CMissile : public CCelestialObject
{
public:
	CMissile(ulong aID, CVector& aPos, CVector aDir);

	virtual void Update(float);
	virtual void Draw();
	virtual bool CanBeDeleted();
	virtual void CollidedWith(CCelestialObject*);

	virtual float GetShieldDamage();
	virtual float GetShipHullDamage();


//private:
	ulong mFiredFromShipID;
	float mAcceleration;
	float mMaxSpeed;

private:
	float mShieldDamage;
	float mShipHullDamage;
	bool mDestroyed;
};

//-------------------------------------------------------------------------------------------------------------
// CBullet
// Fired by a player ship.
//-------------------------------------------------------------------------------------------------------------
class CBullet : public CCelestialObject
{
public:
	CBullet(ulong aID, CVector& aPos, CVector& aDir);

	virtual void Update(float);
	virtual void Draw();
	virtual bool CanBeDeleted();
	virtual void CollidedWith(CCelestialObject*);

	virtual float GetShieldDamage();
	virtual float GetShipHullDamage();

	// if the damage caused by bullet is very less or negligable then we can delete the CBullet.
	bool IsUseless(); 

	static const unsigned int MAX_VELOCITY;
//private:
	const float mConstantDampeningFactor;
	ulong mFiredFromShipID;

private:
	float mShieldDamage;
	float mShipHullDamage;
	bool mDestroyed;
};

#endif // __WEAPON_EXPLOSIVE_H__