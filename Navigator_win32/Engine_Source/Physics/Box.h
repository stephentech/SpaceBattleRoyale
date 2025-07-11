

#ifndef __BOX_H__
#define __BOX_H__

#include "PhysicsObject.h"

namespace engPhysics
{
	class Box : public PhysicsObject
	{
	public:
		Box(CCelestialObject* aOwner, CVector aStart, CVector aEnd);
		~Box();

		virtual bool BroadphaseCollisionDetect(PhysicsObject*);
		CVector GetCenter();
		CVector GetDimension();

		CVector mStartPt;
		CVector mEndPt;
	};

}; // namespace engPhysics


#endif // __BOX_H__