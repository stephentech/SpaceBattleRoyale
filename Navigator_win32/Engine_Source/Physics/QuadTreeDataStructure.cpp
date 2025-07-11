#include "stdafx.h"
#include "QuadTreeDataStructure.h"
#include "PhysicsObject.h"
#include "Physics.h"


namespace engPhysics
{

	/////////////////////////////////////////////////////////////////////////////////////////
	// QuadTree.
	QuadTree::QuadTree(CVector aStart, CVector aEnd)
	{
		mRoot = new QuadTreeNode(NULL, aStart, aEnd-aStart);
	}

	QuadTree::~QuadTree()
	{
		// clear out the whole tree.
		if(mRoot)
		{
			mRoot->ReleaseChildren();
			delete mRoot;
			mRoot = NULL;
		}
	}

	void QuadTree::Update(float aDT)
	{
		// re position all the dirty physics objects.

		// traverse each TreeNode recursively and if dirty flag is found, check with its parent whether it has changed quadrent.
		mRoot->ReevaluateMyQuadrent();

		// update each object with dt

		// create a list of broadphase collisions. Later use this list to perform narrow phase detection.
		std::list<Physics::BroadphaseCollision *> broadphaseCollisionList;
		mRoot->CheckForBroadphaseCollison(broadphaseCollisionList);
		Physics::GetInstance()->ResolveBroadphaseCollision(broadphaseCollisionList);
	}

	void QuadTree::AddObject(PhysicsObject *aObj)
	{
		mRoot->AddObject(aObj);
	}

	void QuadTree::RemoveObject(PhysicsObject* aObj)
	{
		// todo. magic
		// first find the node the object is supposed to be in.
		// second then delete the Object.

		mRoot->RemoveObj(aObj);
	}

	void QuadTree::DrawDebugTree()
	{
		DrawDebugTree(mRoot);
	}

	void QuadTree::DrawDebugTree(QuadTreeNode *aNode)
	{
		int diff = aNode->mDepthLevel + 1;
		int iCol = 0x000000ff;
		if(aNode->mData.size() == 0)
			iCol = 0x0077ff77;
		Physics::GetInstance()->DrawRectangle(iCol, aNode->mQuadBegin + CVector(diff, diff), aNode->mDimension - CVector(diff, diff) * 2);

		if(aNode->CanSubDivide() == false)
			return;

		for(int iQuad = QuadTreeNode::eQuadTopLeft; iQuad < QuadTreeNode::eQuadMax; iQuad ++)
		{
			QuadTreeNode *pNodeChild = aNode->mChild[iQuad];
			if(pNodeChild)
				DrawDebugTree(pNodeChild);
		}
	}



	/////////////////////////////////////////////////////////////////////////////////////////
	// QuadTreeNode

	QuadTreeNode::QuadTreeNode(QuadTreeNode *aParent, CVector aPtStart, CVector aDimension)
		: mParent(aParent)
		, mQuadBegin(aPtStart)
		, mDimension(aDimension)
	{
		if(aParent)
			mDepthLevel = aParent->mDepthLevel + 1;
		else 
			mDepthLevel = 0;

		CVector vecDimension = mDimension / 2;
		CVector ptMid = mQuadBegin + vecDimension;
	
		if(CanSubDivide())
		{
			mChild[QuadTreeNode::eQuadTopLeft]		= new QuadTreeNode(this, mQuadBegin, vecDimension);
			mChild[QuadTreeNode::eQuadTopRight]		= new QuadTreeNode(this, CVector(ptMid.x, mQuadBegin.y), vecDimension);
			mChild[QuadTreeNode::eQuadBottomLeft]	= new QuadTreeNode(this, CVector(mQuadBegin.x, ptMid.y), vecDimension);
			mChild[QuadTreeNode::eQuadBottomRight]	= new QuadTreeNode(this, ptMid, vecDimension);
		}
		else
		{
			mChild[QuadTreeNode::eQuadTopLeft]		= 
			mChild[QuadTreeNode::eQuadTopRight]		=
			mChild[QuadTreeNode::eQuadBottomLeft]	=
			mChild[QuadTreeNode::eQuadBottomRight]	= NULL;
		}
	}

	bool QuadTreeNode::CanSubDivide()
	{
		if(mDepthLevel < 3) // make this variable. and also, this should be a property of the QuadTree instead of QuadTreeNode.
			return true;
		return false;
	}

	void QuadTreeNode::AddObject(PhysicsObject* aObj)
	{
		if(!CanSubDivide())
		{
			mData.push_back(aObj);
			aObj->SetDirty(false);
			return;
		}

		// first make sure this object intersects more than one quadrant or no.
		// if so, then this object must be added to current node
		int hashMap = 0;
		for(int i=QuadTreeNode::eQuadTopLeft;
				i < QuadTreeNode::eQuadMax;
				i ++)
		{
			QuadTreeNode* pNodeChild = mChild[i];

	#ifdef QUADTREE_RECTANGLES
			if(aObj->DoesBoxIntersect( pNodeChild->mQuadBegin, pNodeChild->mDimension) )
	#elif defined QUADTREE_CIRCLES
			if(aObj->DoesCircleIntersect( pNodeChild->mQuadBegin, pNodeChild->mDimension ) )
	#endif
			hashMap |= 1 << i;
		}	


		if(hashMap == 0)
		{
			printf("\nbolloks: somthing has gone wrong. in the Physics QuadTree::AddObject");
			if(mParent)
			{
				mParent->AddObject(aObj);
				return;
			}
		}

		for(int i=QuadTreeNode::eQuadTopLeft;
				i < QuadTreeNode::eQuadMax;
				i ++)
		{
			if(hashMap == (1 << i) )
			{
				mChild[i]->AddObject(aObj);
				return;
			}
		}
	
		// reachin here means this object belongs to multiple quads.
		aObj->SetDirty(false);
		mData.push_back(aObj);
	}

	void QuadTreeNode::ReleaseChildren()
	{
		while(mData.size())
		{
			PhysicsObject* pPhysicsObj = *mData.begin();
			mData.remove(pPhysicsObj);
			delete pPhysicsObj;
		}

		for(int eQuad = eQuadTopLeft; eQuad < eQuadMax; eQuad ++)
		{
			if(mChild[eQuad])
			{
				mChild[eQuad]->ReleaseChildren();
				delete mChild[eQuad];
			}
		}
	}

	bool QuadTreeNode::RemoveObj(PhysicsObject* aObj)
	{
		// first find in our own data.
		if(mData.size() )
		{
			for(std::list<PhysicsObject*>::iterator it = mData.begin(); it != mData.end(); it ++)
			{
				if(aObj == *it)
				{
					mData.erase(it);
					delete aObj;
					return true;
				}
			}
		}

		// not found!! look in children.
		for(int i= eQuadTopLeft; i< eQuadMax; i ++)
		{
			if( mChild[i] && mChild[i]->RemoveObj(aObj) )
				return true;
		}

		return false;
	}

	void QuadTreeNode::ReevaluateMyQuadrent()
	{
		// first find in our own data.
		if(mData.size())
		{
			std::list<PhysicsObject*> physicsObjectsToReEval;

			for(std::list<PhysicsObject*>::iterator it = mData.begin(); it != mData.end(); it ++)
			{
				PhysicsObject* pObj = (*it);
				if( pObj->IsDirty() )
				{
					physicsObjectsToReEval.push_back(*it);
				}
			}

			while(physicsObjectsToReEval.size() )
			{
				PhysicsObject* pObj = *physicsObjectsToReEval.begin();
				physicsObjectsToReEval.remove( *physicsObjectsToReEval.begin() );
			
				mData.remove(pObj);

				// re-add this physics object from the root.
				Physics::GetInstance()->AddPhysicsObject(pObj);
			}
		}

		// recursively call on children
		for(int i= eQuadTopLeft; i< eQuadMax; i ++)
		{
			if(mChild[i])
				mChild[i]->ReevaluateMyQuadrent();
		}
	}

	void QuadTreeNode::CheckForBroadphaseCollison(std::list<Physics::BroadphaseCollision *> &aRetList)
	{
		if(mData.size())
		{
			for(std::list<PhysicsObject*>::iterator it = mData.begin(); it!= mData.end(); it ++)
			{
				CheckForBroadphaseCollisionWithChildren(*it, aRetList);
			}
		}

		for(int i= eQuadTopLeft; i< eQuadMax; i ++)
		{
			if(mChild[i])
				mChild[i]->CheckForBroadphaseCollison(aRetList);
		}
	}

	void QuadTreeNode::CheckForBroadphaseCollisionWithChildren(PhysicsObject *aObj, std::list<Physics::BroadphaseCollision *> &aRetList)
	{
		if(mData.size() )
		{
			for(std::list<PhysicsObject*>::iterator it = mData.begin(); it!= mData.end(); it ++)
			{
				PhysicsObject* pPhysicsObj = (*it);
				if(aObj == pPhysicsObj)
					continue;

				//if(aObj->DoesCircleIntersect( (*it)->GetPosition(), (*it)->GetRadius()) )
				if(aObj->BroadphaseCollisionDetect( *it ) )
				{
					// before we add this new pair, make sure they are not already added.
					bool bFound = false;
					std::list<Physics::BroadphaseCollision *>::iterator itCollision = aRetList.begin();
					while(itCollision != aRetList.end())
					{
						Physics::BroadphaseCollision *pCollisionPair = *itCollision;
						if(		(pCollisionPair->pObj1 == pPhysicsObj && pCollisionPair->pObj2 == aObj) 
							||	(pCollisionPair->pObj2 == pPhysicsObj && pCollisionPair->pObj1 == aObj) )
						{
							bFound = true;
							break;
						}
						itCollision ++;
					}

					if(!bFound)
						aRetList.push_back( new Physics::BroadphaseCollision(aObj, pPhysicsObj)  );
				}
			}
		}

		// recursively call on children
		for(int i= eQuadTopLeft; i< eQuadMax; i ++)
		{
			if(mChild[i])
				mChild[i]->CheckForBroadphaseCollisionWithChildren(aObj, aRetList);
		}
	}

}//  namespace engPhysics
 