
//-------------------------------------------------------------------------------------------------------------
// Sun
// A star, a simple unmoving object.
//-------------------------------------------------------------------------------------------------------------


#ifndef __SUN_H__
#define __SUN_H__

#include "Object.h"

class SunSpawnEvent : public EventBase
{
public:
	SunSpawnEvent(CVector aPos, float aRadius)
		: EventBase()
		, vPos(aPos)
		, fRadius(aRadius)
	{
	}

	CVector vPos;
	float fRadius;
};

class Sun : public CCelestialObject
{
public:
	Sun(CVector);
	virtual void Update(float) {}
	virtual void Draw();
	virtual int GetColor();
	virtual bool CanBeDeleted();

	float	GetRadius() {return mRadius; };
	void	SetRadius(float aRadius);

private:
	float mRadius;
};

#endif // __SUN_H__