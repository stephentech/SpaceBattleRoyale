
#pragma once

#include <vector>
#include "Event\EventBase.h"

class TimerEvent;

class Timer
{
public:
	static Timer& GetInstance();

	void Reset();
	void Update();

	float GetTimeDelta() { return mTimeDelta; }
	double GetTime()     { return mTime; }

	void AddTimerEvent(TimerEvent*);
	void RemoveTimerEvent(TimerEvent*);

private:
	Timer();

	float	mTimeDelta;
	double	mTime;
	ulong   mTicks;

	std::list<TimerEvent *> mTimerEventList;
	std::list<TimerEvent *> mRemoveTimerEventList;
};


// timer based events. Base TimeEvent
class TimerEvent : public EventBase
{
public:
	TimerEvent(float);
	void Update(float);
	float GetTimeDelay() { return mTimeDelay; }

	virtual void Execute() = 0;

private:
	float mTimeDelay;
};

//////////////////////////////////////////////////////////////////////////////////
// TimerEvent_CreateShip
class TimerEvent_CreateShip : public TimerEvent
{
public:
	TimerEvent_CreateShip(float aDelay, int aAIType );
	virtual void Execute();

private:
	int		mAIType;
};

//////////////////////////////////////////////////////////////////////////////////
// TimerEvent_RespawnShip
class TimerEvent_RespawnShip : public TimerEvent
{
public:
	TimerEvent_RespawnShip(float aDelay, ulong aID );
	virtual void Execute();

private:
	int		mShipID;
};