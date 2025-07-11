#include <stdafx.h>

#include "Box.h"
#include "../object.h"
#include "Circle.h"

namespace engPhysics
{

	Box::Box(CCelestialObject* aOwner, CVector aStart, CVector aEnd)
		: PhysicsObject(aOwner)
		, mStartPt(aStart)
		, mEndPt(aEnd)
	{
		mShape = eBox;
	}

	Box::~Box()
	{
	
	}

	CVector Box::GetCenter()
	{
		return (mStartPt + mEndPt) / 2.0f;
	}

	CVector Box::GetDimension()
	{
		return (mEndPt - mStartPt);
	}


	bool Box::BroadphaseCollisionDetect(PhysicsObject* aPhysicsObj)
	{
		switch( aPhysicsObj->GetShape() )
		{
		case eRay:
			break;

		case eBox:
			{
				
			}
			break;

		case eCircle:
			{
				Circle* pCircle = (Circle *)aPhysicsObj;

				// the algorithm is already written in Circle.cpp.
				return pCircle->BroadphaseCollisionDetect( this );
			}
			break;

		default:
			break;
		}

		return false;
	}

}; // namespace engPhysics

