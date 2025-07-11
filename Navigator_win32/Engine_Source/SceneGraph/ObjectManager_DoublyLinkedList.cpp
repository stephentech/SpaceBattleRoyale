#include "stdafx.h"

#include "ObjectManager_DoublyLinkedList.h"
#include "..\object.h"

static ObjectManager_DoublyLinedList *sInstance = NULL;
ObjectManager_DoublyLinedList* ObjectManager_DoublyLinedList::GetInstance()
{
	if(sInstance == NULL)
		sInstance = new ObjectManager_DoublyLinedList;
	return sInstance;
}

void ObjectManager_DoublyLinedList::DestroyInstance()
{
	delete sInstance;
	sInstance = NULL;
}

ObjectManager_DoublyLinedList::ObjectManager_DoublyLinedList()
	: mObjectListStart(NULL)
	, mBucketListStart(NULL)
	, mCount(0)
{
}

ObjectManager_DoublyLinedList::~ObjectManager_DoublyLinedList()
{
	RemoveAll();
}


 // get Object based on ID
CCelestialObject* ObjectManager_DoublyLinedList::GetObject(ulong aID)
{
	Node* pNode = FindNodeWithObjectID(aID);
	if( pNode )
		return pNode->mData;
	return NULL;
}

// get Objects based on Type
void ObjectManager_DoublyLinedList::GetObjects(ulong aType, CCelestialObject** aRetArr)
{
	if(aType == -1)
	{
		// return all objects.
		int iIndex = 0;
		Node* pNode = mObjectListStart;
		while(pNode)
		{
			aRetArr[iIndex] = pNode->mData;
			pNode = pNode->mNext;
			iIndex ++;
		}
	}
	else
	{
		// return specific type Objects only.
		int iIndex = 0;
		Bucket* pBucket = FindBucketOfType(aType);
		Node* pNode = pBucket->mData;
		while(pNode && pNode->mObjectType == aType)
		{
			aRetArr[iIndex] = pNode->mData;
			pNode = pNode->mNext;
			iIndex ++;
		}
	}

	return;
}

// Add Object.
void ObjectManager_DoublyLinedList::AddObject(CCelestialObject* aObj)
{
	// make sure this object has not been added before ...
	Bucket* pBucket = FindBucketOfType(aObj->GetTypeID());
	if(pBucket)
	{
		Node* pNode = FindNodeInBucket(aObj->GetObjectID(), pBucket);
		if(pNode)
		{
			// this node exists. assert and return;
			assert(0);
			return;
		}
	}

	// create a new Node for this Object.
	Node *pNewNode = CreateNewNode(aObj);
	if(pBucket)
	{
		AddNodeInBucket( pNewNode, pBucket);
	}
	else
	{
		CreateBucketWithNode(pNewNode);
	}
}

// Remove Object directly.
void ObjectManager_DoublyLinedList::RemoveObject(CCelestialObject* aObj)
{
	Bucket* pBucket = FindBucketOfType(aObj->GetTypeID());
	Node *pNode = FindNodeInBucket(aObj->GetObjectID(), pBucket);
	if(pBucket)
	{
		RemoveNodeInBucket( pNode, pBucket);
	}
	else
	{
		// break
		assert(0);
	}
}

// Remove Object based on ID.
void ObjectManager_DoublyLinedList::RemoveObject(ulong aID)
{
	Node *pNode = FindNodeWithObjectID( aID );
	if( pNode )
	{
		Bucket* pBucket = FindBucketOfType(pNode->mObjectType);
		RemoveNodeInBucket( pNode, pBucket );
	}
}

void ObjectManager_DoublyLinedList::RemoveAll()
{
	while(mBucketListStart)
		RemoveBucket(mBucketListStart);
}

uint32 ObjectManager_DoublyLinedList::GetCount(int aType)
{
	if(aType == -1)
		return mCount;

	Bucket* pBucket = FindBucketOfType(aType);
	if(pBucket)
	{
		int iCount = 0;
		Node* pNode = pBucket->mData;
		while( pNode && pNode->mObjectType == aType)
		{
			iCount ++;
			pNode = pNode->mNext;
		}

		return iCount;
	}

	return 0;
}


// internal implementation
#pragma region Node internal impl
ObjectManager_DoublyLinedList::Node* ObjectManager_DoublyLinedList::CreateNewNode(CCelestialObject* aObj)
{
	Node* pNewNode = new Node;
	pNewNode->mData = aObj;
	pNewNode->mObjectID = aObj->GetObjectID();
	pNewNode->mObjectType = aObj->GetTypeID();

	return pNewNode;
}

void ObjectManager_DoublyLinedList::AddNodeAt(Node* aNodeToAdd, Node* aBeforeNode)
{
	if( mObjectListStart == NULL)
	{
		mObjectListStart = aNodeToAdd;
	}
	else
	{
		aNodeToAdd->mNext = aBeforeNode;
		if(aBeforeNode)
		{
			aNodeToAdd->mPrev = aBeforeNode->mPrev;

			if(aBeforeNode->mPrev)
				aBeforeNode->mPrev->mNext = aNodeToAdd;
			aBeforeNode->mPrev = aNodeToAdd;
		}
		else
		{
			// add right at the end.
			Node* pNode = mObjectListStart;
			while(pNode && pNode->mNext)
				pNode = pNode->mNext;

			pNode->mNext = aNodeToAdd;
			aNodeToAdd->mPrev = pNode;
		}
	}

	mCount ++;
}

ObjectManager_DoublyLinedList::Node*ObjectManager_DoublyLinedList::FindNodeWithObjectID(ulong aID)
{
	if(!mBucketListStart)
		return NULL;

	Node *pNode = mBucketListStart->mData;
	while( pNode && pNode->mObjectID != aID)
		pNode = pNode->mNext;

	return pNode;
}

ObjectManager_DoublyLinedList::Node* ObjectManager_DoublyLinedList::FindNodeInBucket(ulong aID, Bucket* aBucket)
{
	Node *pNode = aBucket->mData;
	while(	pNode 
			&& pNode->mObjectID != aID 
			&& pNode->mObjectType == aBucket->mObjectType )
		pNode = pNode->mNext;

	if(!pNode)
		return NULL;

	// return valid pNode only if the Object ID matches.
	return pNode->mObjectID == aID ? pNode : NULL;
}

void ObjectManager_DoublyLinedList::RemoveNode(Node *aNode)
{
	if(aNode == mObjectListStart)
		mObjectListStart = aNode->mNext;

	if(aNode->mNext)
		aNode->mNext->mPrev = aNode->mPrev;
	if(aNode->mPrev)
		aNode->mPrev->mNext = aNode->mNext;

	delete aNode->mData;
	delete aNode;

	mCount --;
}

#pragma region Bucket internal impl

ObjectManager_DoublyLinedList::Bucket* ObjectManager_DoublyLinedList::FindBucketOfType(ulong aType)
{
	Bucket *pBucket = mBucketListStart;
	while(pBucket)
	{
		if(pBucket->mObjectType == aType)
			return pBucket;

		pBucket = pBucket->mNext;
	}

	return false;
}

ObjectManager_DoublyLinedList::Bucket* ObjectManager_DoublyLinedList::CreateBucketWithNode(Node *aNode)
{
	ulong iNodeType = aNode->mData->GetTypeID();
	Bucket *pBucket = mBucketListStart;
	Bucket *pPrevBucket = mBucketListStart;

	// traverse to the end of the Bucket list.
	while(pBucket && pBucket->mObjectType < iNodeType)
	{
		pPrevBucket = pBucket;
		pBucket = pBucket->mNext;
	}

	Bucket* pNewBucket = new Bucket;
	pNewBucket->mData = aNode;
	pNewBucket->mObjectType = aNode->mObjectType;

	if(pBucket == mBucketListStart)
	{
		pNewBucket->mNext = mBucketListStart;
		if(mBucketListStart)
			mBucketListStart->mPrev = pNewBucket;
		mBucketListStart = pNewBucket;

		// lastly, add the node to the nodeList.
		AddNodeAt(aNode, NULL);
	}
	else 
	{
		// add the new bucket before the pBucket.
		pNewBucket->mPrev = pPrevBucket;
		if(pPrevBucket)
		{
			pNewBucket->mNext = pPrevBucket->mNext;	
			pPrevBucket->mNext = pNewBucket;
		}
		if(pBucket)
			pBucket->mPrev = pNewBucket;

		// lastly, add the node to the nodeList.
		if(pBucket)
			AddNodeAt(aNode, pBucket->mData);
		else
			AddNodeAt(aNode, NULL);
	}

	return pNewBucket;
}

void ObjectManager_DoublyLinedList::AddNodeInBucket(Node* aNodeToAdd, Bucket* aInBucket)
{
	// add in the very end of this bucket.

	// first traverse to the very end.
	Node *pNode = aInBucket->mData;
	while(pNode && (pNode->mObjectType == aInBucket->mObjectType) )
		pNode = pNode->mNext;

	// add to the Node list.
	AddNodeAt( aNodeToAdd, pNode);
}

void ObjectManager_DoublyLinedList::RemoveNodeInBucket(Node *aNode, Bucket *aBucket)
{
	// first check whether this is the only Node in this Bucket ???
	// if so, we wish to remove the Bucket itself.
	if(aBucket->mData == aNode)
	{
		// first node. if there are no other nodes belonging to this bucket, delete it.
		Bucket *aNextBucket = aBucket->mNext;
		Node *aNextNode = aNode->mNext;
		//if(aNextBucket && aNextNode && aNextBucket->mData == aNextNode )
		if(!aNextNode || (aNextNode && aNextNode->mObjectType != aBucket->mObjectType) )
			RemoveBucket(aBucket);
		else
		{
			aBucket->mData = aNode->mNext;
			RemoveNode(aNode);
		}
	}
	else
		RemoveNode(aNode);
}

void ObjectManager_DoublyLinedList::RemoveBucket(Bucket* aBucket)
{
	// first, clear all nodes.
	Node* pNode = aBucket->mData;
	while(pNode && pNode->mObjectType == aBucket->mObjectType)
		RemoveNode(pNode);

	// remove links within the NucketList
	if(mBucketListStart == aBucket)
		mBucketListStart = aBucket->mNext;
	
	if(aBucket->mPrev)
		aBucket->mPrev->mNext = aBucket->mNext;
	if(aBucket->mNext)
		aBucket->mNext->mPrev = aBucket->mPrev;

	// last, delete
	delete aBucket;
}


void ObjectManager_DoublyLinedList::DumpInfo()
{
	FILE *fp;
	fp = fopen("abc.txt", "a+");

	fprintf(fp, "\n--------------------------------------------------------------------------\nBucket List\n");
	Bucket* pBucket = mBucketListStart;
	while(pBucket)
	{
		fprintf(fp, "(Addr:%x : Type: %d) -->", pBucket, pBucket->mObjectType);
		pBucket = pBucket->mNext;
	}
	fprintf(stderr, "NULL \n");

	fprintf(fp, "\nNode List\n");
	Node* pNode = mObjectListStart;
	while(pNode)
	{
		fprintf(fp, "(ID:%d Type:%d ) -->", pNode->mObjectID, pNode->mObjectType);
		pNode = pNode->mNext;
	}
	fprintf(fp, "NULL \n----------------------------------------------------------------------------\n");

	fflush(fp);
	fclose(fp);
}

