
#include "stdafx.h"

#include "game.h"
#include "object.h"
#include "timer.h"
#include "Sun.h"

#include "Physics\Physics.h"
#include "Physics\PhysicsObject.h"

//--------------------------------------------------------------------------------------------------------------
// CObjectIDManager
//--------------------------------------------------------------------------------------------------------------

CObjectIDManager& CObjectIDManager::GetInstance()
{
	static CObjectIDManager sInstance;
	return sInstance;
}

ulong CObjectIDManager::GetNextID()
{
	return sNextID++;
}

ulong CObjectIDManager::GetObjectTypeID(char *aObjectType)
{
	bool bAddNew = false;
	ulong iRetVal = 0;

	// check if its the first element.
	if(sObjectTypeID.size() == 0)
		bAddNew = true;
	
	// check whether there's a valid entry.
	iRetVal = sObjectTypeID[aObjectType];
	if(	iRetVal == 0 )
		bAddNew = true;

	// todo. check whether the second entry is being filled up correctly.
	if(bAddNew)
	{
		iRetVal = sNextTypeID;
		sObjectTypeID[aObjectType] = iRetVal;
		sNextTypeID ++;
	}
		
	return iRetVal;
}

//--------------------------------------------------------------------------------------------------------------
// CCelestialObject
//--------------------------------------------------------------------------------------------------------------
CCelestialObject::CCelestialObject()
: mPosition(0,0)
, mVelocity(0,0)
, mPhysicsObject(NULL)
{
	mID = CObjectIDManager::GetInstance().GetNextID();
}


int CCelestialObject::GetColor()
{
	// generic Red Color
	return 0x000000ff;
}

CCelestialObject::~CCelestialObject()
{
	if(mPhysicsObject)
	{
		engPhysics::Physics::GetInstance()->RemovePhysicsObject(mPhysicsObject);
		mPhysicsObject = NULL;
	}
}


bool CCelestialObject::CanBeDeleted()
{
	int iX, iY, iWidth, iHeight;
	Game::GetInstance().GetPlayableSpace(iX, iY, iWidth, iHeight);

	CVector vPos(GetPos());
	if(		-1 > vPos.x 
		||	vPos.x > iWidth + 1
		||  -1 > vPos.y 
		||  vPos.y > iHeight + 1)
		return true;
	return false;
}

void CCelestialObject::SetPos(CVector aPos) 
{ 
	mPosition = aPos;
	if(mPhysicsObject)
	{
		mPhysicsObject->SetPosition(aPos);
		mPhysicsObject->SetDirty(true);
	}
}