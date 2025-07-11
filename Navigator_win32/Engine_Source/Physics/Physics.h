
#ifndef __PHYSICS_H__
#define __PHYSICS_H__

// fwd decl.
class CCelestialObject;

namespace engPhysics
{

	// fwd decl.
	class PhysicsObject;
	class QuadTree;


	class Physics
	{
	public:
		static Physics* GetInstance();

		~Physics();

		void Release();
		void Update(float aDT);

		PhysicsObject* CreatePhysicsObject(CCelestialObject* aObj);
		bool AddPhysicsObject(PhysicsObject*);
		void RemovePhysicsObject(PhysicsObject*);

		// Debug draw calls
		void DrawDebugInfo();
		void DrawCircle(int aCol, CVector aPt, float aRadius);
		void DrawLine(int aCol, CVector aStart, CVector aEnd);
		void DrawRectangle(int aCol, CVector aStart, CVector aDim);

		// BroadphaseCollisions
		class BroadphaseCollision
		{
		public:
			BroadphaseCollision(PhysicsObject* aObj1, PhysicsObject* aObj2)
				: pObj1(aObj1)
				, pObj2(aObj2)
			{
			}

			PhysicsObject* pObj1;
			PhysicsObject* pObj2;
		};	
		void ResolveBroadphaseCollision(std::list<BroadphaseCollision *> &aCollisionsInfo);

	private:
		Physics();
		QuadTree *mQuadTree;
	};

} ; // namespace engPhysics

#endif // __PHYSICS_H__