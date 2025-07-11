#include <stdafx.h>

#include "../object.h"
#include "Circle.h"
#include "Box.h"

namespace engPhysics
{
	Circle::Circle(CCelestialObject* aOwner, CVector aStart, float aRadius)
		: PhysicsObject(aOwner)
		//, mStartPt(aStart)
		, mRadius(aRadius)
	{
		mShape = eCircle;
	}

	Circle::~Circle()
	{
	
	}

	bool Circle::BroadphaseCollisionDetect(PhysicsObject* aPhysicsObj)
	{
		switch( aPhysicsObj->GetShape() )
		{
		case eRay:
			break;

		case eBox:
			{
				// algorithm explained on :http://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
				//circleDistance.x = abs(circle.x - rect.x);
				//circleDistance.y = abs(circle.y - rect.y);
				//if (circleDistance.x > (rect.width/2 + circle.r)) { return false; }
				//if (circleDistance.y > (rect.height/2 + circle.r)) { return false; }
				//if (circleDistance.x <= (rect.width/2)) { return true; } 
				//if (circleDistance.y <= (rect.height/2)) { return true; }
				//cornerDistance_sq = (circleDistance.x - rect.width/2)^2 + (circleDistance.y - rect.height/2)^2;
				//return (cornerDistance_sq <= (circle.r^2));


				// assuming AABB.
				Box* pBox = (Box *)aPhysicsObj;
				CVector vBoxCenter = pBox->GetCenter();
				CVector vBoxDim = pBox->GetDimension();

				CVector vCircleCenter = GetPosition();
				float fRadius = GetRadius();

				CVector vDiff( fabs(vBoxCenter.x - vCircleCenter.x), fabs(vBoxCenter.y - vCircleCenter.y) );

				if(vDiff.x > (vBoxDim.x/2 + fRadius) )
					return false;
				if(vDiff.y > (vBoxDim.y/2 + fRadius) )
					return false;
				
				if(vDiff.x <= vBoxDim.x/2 )
					return true;
				if(vDiff.y <= vBoxDim.y/2 )
					return true;

				float cornerDistance_sq = ( (vDiff.x - vBoxDim.x/2) * (vDiff.x - vBoxDim.x/2) )
										+ ( (vDiff.y - vBoxDim.y/2) * (vDiff.y - vBoxDim.y/2) );

				return (cornerDistance_sq <= (fRadius * fRadius) );
			}
			break;

		case eCircle:
			{
				Circle *pCircle = (Circle *) aPhysicsObj;
				float fDist = (GetPosition() - pCircle->GetPosition()).GetLength();
				if(fDist < pCircle->GetRadius() + GetRadius() )
					return true;
			}
			break;

		default:
			break;
		}

		return false;
	}

	float Circle::GetRadius()
	{
		return mRadius;
	}

	void Circle::SetRadius( float aRadius )
	{
		mRadius = aRadius;
	}

}; // namespace engPhysics
