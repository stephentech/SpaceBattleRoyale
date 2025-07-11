
#include "stdafx.h"
#include "timer.h"
#include "game.h"

#include <time.h>
#include <stdio.h>

Timer::Timer()
: mTimeDelta(0.f)
, mTime(0.)
, mTicks(0)
{
	srand(time(0));
	Reset();
}

Timer& Timer::GetInstance()
{
	static Timer instance;
	return instance;
}

void Timer::Reset()
{
	mTime = 0.;
	mTimeDelta = 0.f;
	mTicks = clock();
}

void Timer::Update()
{
	long ticks = clock();

	mTimeDelta = float((double)(ticks - mTicks) / CLOCKS_PER_SEC);
	if (mTimeDelta > 0.2f)
	{
		mTimeDelta = 0.2f;
	}

	mTime = mTime + mTimeDelta;
	mTicks = ticks;

	int sizeTimerEventList = mTimerEventList.size();
	if(sizeTimerEventList)
	{
		for(std::list<TimerEvent *>::iterator it=mTimerEventList.begin(); 
			it != mTimerEventList.end(); 
			it++)
		{
			if( (*it)->GetTimeDelay() )
				(*it)->Update(mTimeDelta);
		}
	}

	if(mRemoveTimerEventList.size())
	{
		//for(int i=0; i<mRemoveTimerEventList.size(); i++)
		//{
		//	delete mRemoveTimerEventList[i];
		//	mTimerEventList.remove(mRemoveTimerEventList[i]);
		//}
		for(std::list<TimerEvent *>::iterator it=mRemoveTimerEventList.begin(); 
			it != mRemoveTimerEventList.end(); 
			it++)
		{
			TimerEvent *p = *it;
			mTimerEventList.remove(p);

			// todo: this might be a leak. double check
			//delete p;
		}

		mRemoveTimerEventList.clear();
	}
}

void Timer::AddTimerEvent(TimerEvent* aTimerEvt)
{
	mTimerEventList.push_back( aTimerEvt );
}

void Timer::RemoveTimerEvent(TimerEvent* aTimerEvt)
{
	mRemoveTimerEventList.push_back(aTimerEvt);
}


///////////////////////////////////////////////////////////////////////
// TimerEvent
TimerEvent::TimerEvent(float aDelay)
	: mTimeDelay(aDelay )
{
	Timer::GetInstance().AddTimerEvent(this);
}

void TimerEvent::Update(float aTimeInc)
{
	if(mTimeDelay > 0.0f)
	{
		mTimeDelay -= aTimeInc;
		if(mTimeDelay <= 0.0f)
		{
			Execute();
			SetEventHandled(true);
			Timer::GetInstance().RemoveTimerEvent(this);
		}
	}
}


///////////////////////////////////////////////////////////////////////
// TimerEvent_Spawn
TimerEvent_CreateShip::TimerEvent_CreateShip(float aDelay,  int aAIType )
	: TimerEvent( aDelay )
	, mAIType( aAIType )
{
}

void TimerEvent_CreateShip::Execute()
{
	Game::GetInstance().CreateNewShip( mAIType );
}


///////////////////////////////////////////////////////////////////////
// TimerEvent_Ship ReSpawn
TimerEvent_RespawnShip::TimerEvent_RespawnShip(float aDelay,  ulong aID )
	: TimerEvent( aDelay )
	, mShipID( aID )
{
}

void TimerEvent_RespawnShip::Execute()
{
	Game::GetInstance().RespawnShip( mShipID );
}