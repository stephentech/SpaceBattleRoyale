

#ifndef __OBJECTTEST_H__
#define __OBJECTTEST_H__

#include "Object.h"
class ObjectManager;

class TestObject_One : public CCelestialObject
{
public:
	TestObject_One() {mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("TestObj_One");}
	virtual void Update(float) {};
	virtual void Draw() {};
};


class TestObject_Two : public CCelestialObject
{
public:
	TestObject_Two() {mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("TestObj_Two");}
	virtual void Update(float) {};
	virtual void Draw() {};
};


class TestObject_Three : public CCelestialObject
{
public:
	TestObject_Three() {mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("TestObj_Three");}
	virtual void Update(float) {};
	virtual void Draw() {};
};


class TestObject_Four : public CCelestialObject
{
public:
	TestObject_Four() {mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("TestObj_Four");}
	virtual void Update(float) {};
	virtual void Draw() {};
};


class TestObject_Five : public CCelestialObject
{
public:
	TestObject_Five() {mTypeID = CObjectIDManager::GetInstance().GetObjectTypeID("TestObj_Five");}
	virtual void Update(float) {};
	virtual void Draw() {};
};

void PerformObjectManagerTestOne(ObjectManager *);
void PerformObjectManagerTestTwo(ObjectManager *);
void PerformObjectManagerTestThree(ObjectManager *);

#endif // __OBJECTTEST_H__