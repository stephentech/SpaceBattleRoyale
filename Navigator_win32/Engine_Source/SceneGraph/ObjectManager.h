
#ifndef __OBJECTMANAGER_H__
#define __OBJECTMANAGER_H__

class CCelestialObject;

class ObjectManager
{
public:
	virtual ~ObjectManager() {};

	virtual CCelestialObject* GetObject(ulong) = 0; // get Object based on ID
	virtual void GetObjects(ulong, CCelestialObject**) = 0; // get Objects based on Type
	virtual void AddObject(CCelestialObject*) = 0; // Add Object.
	
	virtual void RemoveObject(CCelestialObject*) = 0; // Remove Object directly.
	virtual void RemoveObject(ulong) = 0; // Remove Object directly.
	virtual void RemoveAll() = 0;

	virtual uint32 GetCount(int aType = -1) = 0;

	// debug section:
	virtual void DumpInfo() {}
};

#endif // __OBJECTMANAGER_H__