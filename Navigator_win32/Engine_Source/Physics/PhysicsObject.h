
#ifndef __PHYSICSOBJECT_H__
#define __PHYSICSOBJECT_H__


#include "../Vector.h"
#include "../object.h"

// fwd decl.
//class CCelestialObject;


namespace engPhysics
{


	//struct SPhysicsObjectInfo
	//{
	//	EPhysicsShapes eShape;
	//	float fRadius;
	//	float fSquareDiagonal;
	//	float fBoxHeight;
	//	float fBoxWidth;
	//	bool bStatic;
	//};

	class PhysicsObject
	{
	public:
		PhysicsObject(CCelestialObject*);
		PhysicsObject();
		~PhysicsObject();

		enum EPhysicsShapes
		{
			eCircle = 0,
			eBox,
			eRay,

			eCustom
		};

		CCelestialObject* GetOwner() {return mOwner;};
		CVector GetPosition();
		void SetPosition(CVector);

		virtual bool BroadphaseCollisionDetect(PhysicsObject*) {return false;};

		bool DoesBoxIntersect(CVector aStartPt, CVector aDim);
		bool DoesCircleIntersect(CVector aPt, float aRadius);

		bool IsStaticObject() {return mIsStatic;}
		void SetStatic(bool aStatic) {mIsStatic = aStatic;}
	
		EPhysicsShapes GetShape() {return mShape; }
		//void SetShape(EPhysicsShapes aShape) {mShape = aShape; SetDirty(true); }

		bool IsDirty() {return mDirty; }
		void SetDirty(bool bVal) {mDirty = bVal;}


	protected:
		// shape of this physics object.
		EPhysicsShapes mShape;

		// is static/immovable
		bool mIsStatic;

		CCelestialObject* mOwner;

		bool mDirty;

		CVector mPos;
	};

}; // namespace engPhysics

#endif // __PHYSICSOBJECT_H__