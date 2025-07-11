#include "stdafx.h"

#include "Physics.h"
#include "PhysicsObject.h"

#include "../object.h"
#include "../game.h"
#include "../SceneGraph/ObjectManager.h"
#include "QuadTreeDataStructure.h"

namespace engPhysics
{

	Physics::Physics()
	{
		int x, y, width, height;
		Game::GetInstance().GetPlayableSpace(x,y,width, height);

		mQuadTree = new QuadTree(CVector(0, 0), CVector(width, height) );
	}

	Physics::~Physics()
	{
		if(mQuadTree)
		{
			delete mQuadTree;
			mQuadTree = NULL;
		}
	}

	Physics* Physics::GetInstance()
	{
		static Physics sPhysicsInstance;;
		return &sPhysicsInstance;
	}

	void Physics::Release()
	{
		// clear all Physics Objects.

	}

	void Physics::Update(float aDT)
	{
	#if 0
		int iNumObjects = Game::GetInstance().GetObjectManager()->GetCount();
		CCelestialObject** Objects = new CCelestialObject*[iNumObjects];
		Game::GetInstance().GetObjectManager()->GetObjects(-1, Objects );
		for (int iIndex = 0; iIndex < iNumObjects; iIndex ++)
		{
			if( Objects[iIndex]->mPhysicsObject == NULL)
				continue;

			for( int iIndex2 = iIndex; iIndex2 < iNumObjects; iIndex2 ++)
			{
				if(Objects[iIndex2] == Objects[iIndex] || Objects[iIndex2]->mPhysicsObject == NULL)
					continue;

				if( Objects[iIndex]->mPhysicsObject->DoesCircleIntersect( Objects[iIndex2]->GetPos(), Objects[iIndex2]->mPhysicsObject->GetRadius() ) )
				{
					Objects[iIndex]->CollidedWith( Objects[iIndex2] );
					Objects[iIndex2]->CollidedWith( Objects[iIndex] );
				}
			}
		}
		delete[] Objects;
	#else
		mQuadTree->Update(aDT);
	#endif
	}

	void Physics::ResolveBroadphaseCollision(std::list<Physics::BroadphaseCollision *> &aCollisionsInfo)
	{
		// note: warning: mBroadphaseCollisionList can contain duplicates. Fix this at the time pushing into the list.
		while(aCollisionsInfo.size())
		{
			BroadphaseCollision* pCollisionInfo = (*aCollisionsInfo.begin());
			pCollisionInfo->pObj1->GetOwner()->CollidedWith( pCollisionInfo->pObj2->GetOwner() );
			pCollisionInfo->pObj2->GetOwner()->CollidedWith( pCollisionInfo->pObj1->GetOwner() );

			aCollisionsInfo.remove(pCollisionInfo);
			delete pCollisionInfo;
		}
	}

	PhysicsObject* Physics::CreatePhysicsObject(CCelestialObject* aObj)
	{
		PhysicsObject* pNewObj = new PhysicsObject(aObj);

		// add this Object to some datastructure we choose to implement
		if( AddPhysicsObject(pNewObj) )
			return pNewObj;

		delete pNewObj;
		return NULL;
	}

	bool Physics::AddPhysicsObject(PhysicsObject* aObj)
	{
		if( !mQuadTree )
			return false;

		mQuadTree->AddObject(aObj);
		return true;
	}

	void Physics::RemovePhysicsObject(PhysicsObject* aObj)
	{
		// remove from datastructure and delete.
		// todo
		if( mQuadTree )
			mQuadTree->RemoveObject(aObj);
	}

	/// debug draw
	void Physics::DrawDebugInfo()
	{
		if( mQuadTree )
			mQuadTree->DrawDebugTree();
	}

	void Physics::DrawCircle(int aCol, CVector aPt, float aRadius)
	{
		Game::GetInstance().DrawCircle(aPt, aRadius);
	}

	void Physics::DrawLine(int aCol, CVector aStart, CVector aEnd)
	{
		Game::GetInstance().ChangeDrawColorTo(aCol);
		Game::GetInstance().DrawLine(aStart, aEnd);
	}

	void Physics::DrawRectangle(int aCol, CVector aStart, CVector aDim)
	{
		Game::GetInstance().ChangeDrawColorTo(aCol);
		Game::GetInstance().DrawRectangle(aStart, aDim);
	}

}; // namespace engPhysics
