

#ifndef __RAY_H__
#define __RAY_H__

#include "PhysicsObject.h"

namespace engPhysics
{

	class Ray : public PhysicsObject
	{
	public:
		Ray(CCelestialObject* aOwner, CVector aStart, CVector aEnd);
		~Ray();

		virtual bool BroadphaseCollisionDetect(PhysicsObject*);

		struct RayCastResult
		{
			std::list<PhysicsObject *> mHitObjects;
		} mHitResult;

		CVector mStartPt;
		CVector mEndPt;
	};

}; // namespace engPhysics

#endif // __RAY_H__