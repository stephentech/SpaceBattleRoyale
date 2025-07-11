

#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "PhysicsObject.h"

namespace engPhysics
{
	class Circle : public engPhysics::PhysicsObject
	{
	public:
		Circle(CCelestialObject* aOwner, CVector aStart, float aRadius);
		~Circle();

		virtual bool BroadphaseCollisionDetect(PhysicsObject*);

		float GetRadius();
		void  SetRadius( float );

		// CVector mStartPt; same as PhysicsObject::mPos
		float mRadius;
	};

}; // namespace engPhysics


#endif // __CIRCLE_H__