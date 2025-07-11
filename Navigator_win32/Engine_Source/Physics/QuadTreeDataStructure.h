

#ifndef __QUADTREE_DATASTRUCTURE_H__
#define __QUADTREE_DATASTRUCTURE_H__

#include "../Vector.h"
#include <vector>
#include "Physics.h"

namespace engPhysics
{

	// fwd decl.
	//class PhysicsObject;
	class QuadTreeNode;


	//#define QUADTREE_CIRCLES
	#define QUADTREE_RECTANGLES

	class QuadTree
	{
	public:
		QuadTree(CVector, CVector);
		~QuadTree();

		void Update(float aDT);
		void AddObject(PhysicsObject *);
		void RemoveObject(PhysicsObject *);

		bool IsPhysicsObjectWithinBox(PhysicsObject aObj, CVector aStart, CVector aEnd);
	
		void DrawDebugTree();
		void DrawDebugTree(QuadTreeNode *);

	private:
		QuadTreeNode* mRoot;
	
	};




	class QuadTreeNode
	{
	public:
		QuadTreeNode(QuadTreeNode *aParent, CVector aPtStart, CVector aDimension);

		bool CanSubDivide();
		void AddObject(PhysicsObject*);
		void ReleaseChildren();
		bool RemoveObj(PhysicsObject*);

		void CheckForBroadphaseCollison(std::list<Physics::BroadphaseCollision *> &aRetList);
		void CheckForBroadphaseCollisionWithChildren(PhysicsObject *, std::list<Physics::BroadphaseCollision *> &aRetList);

		void ReevaluateMyQuadrent();

		enum ESubQuadLocation
		{
			eQuadTopLeft = 0,
			eQuadTopRight,
			eQuadBottomLeft,
			eQuadBottomRight,

			eQuadMax
		};
		QuadTreeNode* mChild[eQuadMax];

		std::list<PhysicsObject*> mData; // objects that this node holds.

		int mDepthLevel;
		QuadTreeNode* mParent;

		CVector mQuadBegin;
		CVector mDimension;
	};

}; // namespace engPhysics

#endif // __QUADTREE_DATASTRUCTURE_H__