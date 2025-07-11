#include "stdafx.h"

#include "Ray.h"
#include "../object.h"

namespace engPhysics
{

	Ray::Ray(CCelestialObject* aOwner, CVector aStart, CVector aEnd)
		: PhysicsObject(aOwner)
		, mStartPt(aStart)
		, mEndPt(aEnd)
	{
		mShape = eRay;
	}

	Ray::~Ray()
	{
		if(mHitResult.mHitObjects.size())
		{
			mHitResult.mHitObjects.clear();
		}
	}


	bool Ray::BroadphaseCollisionDetect(PhysicsObject* aPhysicsObj)
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
			}
			break;

		default:
			break;
		}

		return false;
	}

} ; // namespace engPhysics
