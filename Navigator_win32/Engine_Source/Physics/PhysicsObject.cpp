#include "stdafx.h"

#include "PhysicsObject.h"
#include "Physics.h"
#include "../object.h"

namespace engPhysics
{

	PhysicsObject::PhysicsObject(CCelestialObject* aCelestialObject)
		: mOwner( aCelestialObject )
		, mIsStatic( false )
		, mPos(0,0)
	{
		if(aCelestialObject)
			mPos = aCelestialObject->GetPos();
	}

	PhysicsObject::PhysicsObject()
		: mOwner( NULL )
		, mIsStatic( false )
		, mPos(0,0)
	{
	}

	PhysicsObject::~PhysicsObject()
	{
		mOwner = NULL;
	}

	CVector PhysicsObject::GetPosition() 
	{ 
		return mPos; 
	}

	void PhysicsObject::SetPosition(CVector aPos)
	{
		mPos = aPos;
	}

	bool PhysicsObject::DoesBoxIntersect(CVector aStartPt, CVector aDim)
	{
		return false;
		/*
		// for now use broad phase detection.
		switch(mShape)
		{
		case eBox:
		default:
		case eCircle:
			{
				CVector posObj( mPos ); 
				float fRad = mRadius;
		
				// todo. the following algo is flawed and does not hold good if the Circle engulfs the rectangle/box.
				float fXMin = posObj.x - fRad;
				float fXMax = posObj.x + fRad;
				if(		(aStartPt.x <= fXMin && fXMin <= aStartPt.x + aDim.x) 
					||	(aStartPt.x <= fXMax && fXMax <= aStartPt.x + aDim.x) 
					||	(fXMin <= aStartPt.x && aStartPt.x <= fXMax) )
				{
					float fYMin = posObj.y - fRad;
					float fYMax = posObj.y + fRad;
					if(		(aStartPt.y <= fYMin && fYMin <= aStartPt.y + aDim.y) 
						||	(aStartPt.y <= fYMax && fYMax <= aStartPt.y + aDim.y) 
						||	(fYMin <= aStartPt.y && aStartPt.y <= fYMax) )
						return true;
				}
			}
			break;

		case eRay:
			{
				// algorithm.
				// Let the segment endpoints be p1=(x1 y1) and p2=(x2 y2). 
				// Let the rectangle's corners be (xBL yBL) and (xTR yTR).
			
				// First. Check if all four corners of the rectangle are on the same side of the line. The implicit equation for a line through p1 and p2 is:
				// F(x y) = (y2-y1)x + (x1-x2)y + (x2*y1-x1*y2)
				// If F(x y) = 0, (x y) is ON the line. 
				// If F(x y) > 0, (x y) is "above" the line. 
				// If F(x y) < 0, (x y) is "below" the line.

				//bool bIntersects = false;
				//for(int i=0; i<4; i++)
				//{
				//	CVector vCornerPos;
				//	float fVal = 
				//}
			}
			break;
		}*/
		return false;
	}

	bool PhysicsObject::DoesCircleIntersect(CVector aPt, float aRadius)
	{/*
		// for now use broadphase detection.
		switch(mShape)
		{
		case eBox:
		default:
		case eCircle:
			{
			float fDist = (mPos - aPt).GetLength();
			if(fDist < aRadius + mRadius)
				return true;
			}
			break;

		case eRay:
			{
			}
			break;
		}
		*/
		return false;
	}

}