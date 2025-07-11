
#ifndef __OBJECTMANAGER_DOUBLYLINKEDLIST_H__
#define __OBJECTMANAGER_DOUBLYLINKEDLIST_H__

#include "ObjectManager.h"

// idea is to have a doubly-linked-list of Nodes.
// ideally, the Objects of common type will be placed together,
// each TypeSet of Nodes will have a HeaderPointer, the Header pointers will also be strored as a linked list.

// Bucket-->Bucket1----------->Bucket2----------->Bucket3---> ...---> NULL
//	Start      |                  |                  |
//			   V                  V                  V
// Object-->Element1     ----> Element4    ---->  Element7
//	Start	   |        |         |        |         |
//			   V        |         V        |         V
//			Element2    |      Element5    |      Element7
//			   |        |         |        |         |
//			   V        |          --------          V
//			Element3----                          Element7--->...--> NULL


class ObjectManager_DoublyLinedList : public ObjectManager
{
public:
	static	ObjectManager_DoublyLinedList* GetInstance();
	void	DestroyInstance();
	
	virtual CCelestialObject* GetObject(ulong); // get Object based on ID
	virtual void GetObjects(ulong, CCelestialObject**); // get Objects based on Type
	virtual void AddObject(CCelestialObject*); // Add Object.
	
	virtual void RemoveObject(CCelestialObject*); // Remove Object directly.
	virtual void RemoveObject(ulong); // Remove Object based on ID.
	virtual void RemoveAll();

	virtual uint32 GetCount(int aType = -1);


	virtual void DumpInfo();
private:
	ObjectManager_DoublyLinedList();
	virtual ~ObjectManager_DoublyLinedList();

	template<class DataType>
	class LinkedListElement
	{
	public:
		// todo, simplify all functions to make use of these member vars.
		ulong mObjectID;
		ulong mObjectType;

		DataType *mData;
		LinkedListElement<DataType>* mNext;
		LinkedListElement<DataType>* mPrev;

		LinkedListElement()
			: mData(NULL)
			, mNext(NULL)
			, mPrev(NULL)
		{
		}
	};

	// maintain two linked lists.
	// 1. with the entire Scene Object, similar object type placed next to each other.
	// 2. with only the Object type. This way if we know the type, we can traverse the list much faster.
	typedef LinkedListElement<CCelestialObject> Node;
	typedef LinkedListElement<Node> Bucket;

	// Node related operations
	Node*	CreateNewNode(CCelestialObject* aObj);
	void	AddNodeAt(Node* aNodeToAdd, Node* aAfterNode);
	Node*	FindNodeWithObjectID(ulong);
	Node*	FindNodeInBucket(ulong, Bucket*);
	void	RemoveNode(Node *);

	// Bucket related operations
	Bucket* FindBucketOfType(ulong);
	Bucket* CreateBucketWithNode(Node *);
	void	AddNodeInBucket(Node* aNodeToAdd, Bucket* aInBucket);
	void	RemoveNodeInBucket(Node *, Bucket *);
	void	RemoveBucket(Bucket*);

	
	Node*	mObjectListStart; // the first list, based on Object.
	Bucket* mBucketListStart; // the second list, based on Object's Type.

	uint32	mCount;
};

#endif // __OBJECTMANAGER_DOUBLYLINKEDLIST_H__