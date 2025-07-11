#include "stdafx.h"

#include "ObjectTest.h"
#include "SceneGraph\ObjectManager_DoublyLinkedList.h"
#include "stdio.h"

void PerformObjectManagerTestOne(ObjectManager *aMgr)
{
	fprintf(stderr,"\n Perform Test 1");
	CCelestialObject* aObjArr[25];
	
	int i;
	for(i=0; i<5; i++)
	{
		aObjArr[i] = (CCelestialObject *)new TestObject_One;
		aMgr->AddObject( aObjArr[i] );
	}

	for(i=5; i<10; i++)
	{
		aObjArr[i] = (CCelestialObject *)new TestObject_Two;
		aMgr->AddObject( aObjArr[i] );
	}

	for(i=10; i<15; i++)
	{
		aObjArr[i] = (CCelestialObject *)new TestObject_Three;
		aMgr->AddObject( aObjArr[i] );
	}

	for(i=15; i<20; i++)
	{
		aObjArr[i] = (CCelestialObject *)new TestObject_Four;
		aMgr->AddObject( aObjArr[i] );
	}

	for(i=20; i<25; i++)
	{
		aObjArr[i] = (CCelestialObject *)new TestObject_Five;
		aMgr->AddObject( aObjArr[i] );
	}
	

	aMgr->DumpInfo();

	for(i=0; i<25 / 2; i++)
	{
		aMgr->RemoveObject(aObjArr[i]->GetObjectID() );
		aMgr->RemoveObject(aObjArr[24 -i] );
	}

	aMgr->DumpInfo();

	aMgr->RemoveAll();

}

void PerformObjectManagerTestTwo(ObjectManager *aMgr)
{
	CCelestialObject* aObjArr[1];

	aObjArr[0] = (CCelestialObject *)new TestObject_One;
	aMgr->AddObject( aObjArr[0] );

	aMgr->RemoveAll();

	aMgr->DumpInfo();
}

void PerformObjectManagerTestThree(ObjectManager *aMgr)
{
	CCelestialObject* aObjArr[5];
	aObjArr[0] = (CCelestialObject *)new TestObject_One;
	aMgr->AddObject( aObjArr[0] );

	aObjArr[1] = (CCelestialObject *)new TestObject_Two;
	aMgr->AddObject( aObjArr[1] );
	aObjArr[2] = (CCelestialObject *)new TestObject_Three;
	aMgr->AddObject( aObjArr[2] );
	aObjArr[3] = (CCelestialObject *)new TestObject_Four;
	aMgr->AddObject( aObjArr[3] );


	aMgr->DumpInfo();

	for(int i=0; i<4 / 2; i++)
	{
		aMgr->RemoveObject(aObjArr[i]->GetObjectID() );
		aMgr->RemoveObject(aObjArr[3 -i] );
	}

	aMgr->DumpInfo();
}