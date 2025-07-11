#include "stdafx.h"
#include "WeaponExplosives.h"
#include "Physics\Physics.h"
#include "Physics\Circle.h"

//--------------------------------------------------------------------------------------------------------------
// CMissile
//--------------------------------------------------------------------------------------------------------------
CMissile::CMissile(ulong aID, CVector& aPos, CVector aDir)
	: CCelestialObject()
	, mDestroyed(false)
{
	SetPos( aPos );
	mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("Missile");
	mFiredFromShipID = aID;

	mVelocity = aDir;
	mVelocity.Normalise();
	mAcceleration = 100.0f;
	mMaxSpeed = 700;

	mShieldDamage = 100.0f;
	mShipHullDamage = 35.0f;

	mPhysicsObject = (engPhysics::PhysicsObject *) new engPhysics::Circle(this, aPos, 2.0f); 
	if(mPhysicsObject)
	{
		if( engPhysics::Physics::GetInstance()->AddPhysicsObject(mPhysicsObject) == false)
			delete mPhysicsObject;
	}

	//mPhysicsObject = Physics::GetInstance()->CreatePhysicsObject(this);
	//if(mPhysicsObject)
	//{
	//	mPhysicsObject->SetShape(PhysicsObject::eCircle);
	//	mPhysicsObject->SetRadius(2.0f);
	//}
}

void CMissile::Update(float aDT)
{

	SetPos(GetPos() + mVelocity * aDT);

	if(mVelocity.GetLength() < mMaxSpeed)
	{
		CVector vVec = mVelocity;
		vVec.Normalise();
		mVelocity = mVelocity + vVec * mAcceleration * aDT;
	}
}

void CMissile::Draw()
{
	Game::GetInstance().ChangeDrawColorTo(GetColor());
	Game::GetInstance().DrawCircle(GetPos(),3);

	//CVector vPos(GetPos());
	//Ellipse(hdc, (int)vPos.x - 2, (int)vPos.y - 2, (int)vPos.x + 2, (int)vPos.y + 2);
}

bool CMissile::CanBeDeleted()
{
	return CCelestialObject::CanBeDeleted() || mDestroyed;
}

void CMissile::CollidedWith(CCelestialObject* aObj)
{
	mDestroyed = true;
	if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("Bullet") )
	{
		CBullet *pBullet = (CBullet *)aObj;
		if(pBullet->mFiredFromShipID == mFiredFromShipID)
			 mDestroyed = false; // different Source of origin.
	}
	else if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("Missile") )
	{
		CMissile *pMissile = (CMissile *)aObj;
		if(pMissile->mFiredFromShipID == mFiredFromShipID)
			 mDestroyed = false; // different Source of origin.
	}
	else if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("ship") )
	{
		if(aObj->GetObjectID() == mFiredFromShipID)
			mDestroyed = false;
	}
}

float CMissile::GetShieldDamage() 
{
	return mShieldDamage;
}

float CMissile::GetShipHullDamage()
{
	return mShipHullDamage;
}



//----------------------------------------------------------------------------------
// CBullet
//----------------------------------------------------------------------------------
const unsigned int CBullet::MAX_VELOCITY = 900;
CBullet::CBullet(ulong aID, CVector& aPos, CVector& aDir)
	: CCelestialObject()
	, mConstantDampeningFactor(0.9f)
	, mDestroyed(false)
{
	SetPos(aPos);
	mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("Bullet");
	mFiredFromShipID = aID;
	mVelocity = aDir * MAX_VELOCITY;

	mShieldDamage = 5.0f;
	mShipHullDamage = 3.5f;

	mPhysicsObject = (engPhysics::PhysicsObject *) new engPhysics::Circle(this, aPos, 2.0f); 
	if(mPhysicsObject)
	{
		if( engPhysics::Physics::GetInstance()->AddPhysicsObject(mPhysicsObject) == false)
			delete mPhysicsObject;
	}
}

void CBullet::Update(float aDT)
{
	SetPos( GetPos() + mVelocity * aDT );
	mVelocity = mVelocity * (1.0f - mConstantDampeningFactor * aDT);
}

void CBullet::Draw()
{
	Game::GetInstance().ChangeDrawColorTo(GetColor());
	Game::GetInstance().DrawCircle(GetPos(), 2);

	//CVector vPos(GetPos());
	//Ellipse(hdc, (int)vPos.x - 2, (int)vPos.y - 2, (int)vPos.x + 2, (int)vPos.y + 2);
}

bool CBullet::CanBeDeleted()
{
	return CCelestialObject::CanBeDeleted() || IsUseless();
}

bool CBullet::IsUseless()
{
	if(mDestroyed || mVelocity.GetLength() < 50.0f)
		return true;
	return false;
}

void CBullet::CollidedWith(CCelestialObject* aObj)
{
	mDestroyed = true;
	if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("Bullet") )
	{
		CBullet *pBullet = (CBullet *)aObj;
		if(pBullet->mFiredFromShipID == mFiredFromShipID)
			 mDestroyed = false; // different Source of origin.
	}
	else if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("Missile") )
	{
		CMissile *pMissile = (CMissile *)aObj;
		if(pMissile->mFiredFromShipID == mFiredFromShipID)
			 mDestroyed = false; // different Source of origin.
	}
	else if(aObj->GetTypeID() == CObjectIDManager::GetInstance().GetObjectTypeID("ship") )
	{
		if(aObj->GetObjectID() == mFiredFromShipID)
			mDestroyed = false;
	}
}

float CBullet::GetShieldDamage() 
{
	return mShieldDamage;
}

float CBullet::GetShipHullDamage()
{
	return mShipHullDamage;
}