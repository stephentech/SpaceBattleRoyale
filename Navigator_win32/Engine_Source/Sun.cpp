#include "stdafx.h"
#include "Sun.h"
#include "CommonConversion.h"
#include "Physics\Physics.h"
#include "Physics\Circle.h"
#include "Engine_Source\Physics\Box.h"

Sun::Sun(CVector aPos)
: CCelestialObject()
{
	SetPos(aPos);
	mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("Sun");
	mRadius = RandomRange(150, 500) * 0.1f;

	mPhysicsObject = (engPhysics::PhysicsObject *) new engPhysics::Circle(this, aPos, mRadius); 
	if(mPhysicsObject)
	{
		if( engPhysics::Physics::GetInstance()->AddPhysicsObject(mPhysicsObject) == false)
			delete mPhysicsObject;
	}

	//mPhysicsObject = Physics::GetInstance()->CreatePhysicsObject(this);
	//if(mPhysicsObject)
	//{
	//	mPhysicsObject->SetShape(PhysicsObject::eCircle);
	//	mPhysicsObject->SetRadius(mRadius);
	//	mPhysicsObject->SetStatic(true);
	//}

	Game::GetInstance().GetGameEventListener().OnEvent( new SunSpawnEvent(GetPos(), mRadius) );
}


void Sun::Draw()
{
	CVector vPos(GetPos());
	Game::GetInstance().ChangeDrawColorTo(GetColor());
	Game::GetInstance().DrawCircle(vPos, mRadius);
}

int Sun::GetColor()
{
	// generic Red Color
	return 0x000033ff;
}

bool Sun::CanBeDeleted()
{
	return false;
}

void Sun::SetRadius(float aRadius) 
{
	mRadius = aRadius;
	((engPhysics::Circle *)mPhysicsObject)->SetRadius(mRadius);
}