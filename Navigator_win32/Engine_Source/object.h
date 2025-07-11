

#ifndef __OBJECT_H__
#define __OBJECT_H__


// Includes
#include <map>
#include <list>
#include "Vector.h"

// fwd decl
namespace engPhysics
{
	class PhysicsObject;

}; // namespace engPhysics

// CCelestialObject
// The base class for all game objects.

class CObjectIDManager
{
public:
	static CObjectIDManager& GetInstance();
	ulong GetObjectTypeID(char *);
	ulong GetNextID();

private:
	CObjectIDManager() {sNextID = sNextTypeID = 1;};

	ulong sNextID;
	ulong sNextTypeID;
	std::map<char*, ulong> sObjectTypeID;
};


class CCelestialObject
{
public:
	CCelestialObject();
	CCelestialObject(const CVector& position);

	~CCelestialObject();

	virtual void Update(float) = 0;
	virtual void Draw() = 0;
	virtual int GetColor();
	virtual bool CanBeDeleted(); // indicates whether or not the gameObject has any usefullness anymore.
	virtual void CollidedWith(CCelestialObject*) {};

	ulong GetObjectID() {return mID;}
	ulong GetTypeID() { return mTypeID; }

	
	CVector GetPos() { return mPosition; }
	void SetPos(CVector aPos);

	CVector mVelocity;
	
	// physics property if any.
	engPhysics::PhysicsObject* mPhysicsObject;

protected:
	// keep a track of Object by an ID
	ulong mID;

	// a unique type id to track all same type of objects.
	ulong mTypeID;

private:
	CVector mPosition;
};

#endif // __OBJECT_H__