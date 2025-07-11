#include "stdafx.h"
#include "EventBase.h"


//////////////////////////////////////////////////////////////////////////////////////
// EventBase
EventBase::EventBase()
	: mHandled(false)
{
}

EventBase::~EventBase()
{
}

void EventBase::SetEventHandled(bool)
{
}



//////////////////////////////////////////////////////////////////////////////////////
// EventHandler
EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

bool EventHandler::OnEvent(EventBase* aEvent) 
{
	if(aEvent)
		mEventQueue.push_back(aEvent);

	return true;
}
	
// add and remove Event Listeners.
void EventHandler::AddEventListener(EventListener *aEvtListener)
{
	// check, hope someone doesnt pass myself in.
	if(this == aEvtListener)
		return;

	mRegisteredEventListeners.push_back(aEvtListener);
}

void EventHandler::RemoveEventListener(EventListener *aEvtListener)
{
	mRegisteredEventListeners.remove(aEvtListener);
}

void EventHandler::DispatchEvents()
{
	int iNumEvent = mEventQueue.size();
	for(; iNumEvent; iNumEvent --)
	{
		EventBase *pEventToDispatch = *mEventQueue.begin();
		
		// dispatch to all listeners.
		for(std::list<EventListener *>::iterator itEventListeners = mRegisteredEventListeners.begin();
			itEventListeners != mRegisteredEventListeners.end();
			itEventListeners ++)
		{
			(*itEventListeners)->OnEvent( pEventToDispatch );
		}

		delete pEventToDispatch; // potential crash. todo check
		mEventQueue.remove(pEventToDispatch);
	}
}