

#ifndef __EVENT_H__
#define __EVENT_H__

#include <list>

class EventBase
{
public:
	EventBase();
	virtual ~EventBase();

	void SetEventHandled(bool);

private:
	bool mHandled;
};


class EventListener
{
public:
	virtual bool OnEvent(EventBase*) = 0;
};


// Will queue up the events and flush them once every frame.
class EventHandler : public EventListener
{
public:
	EventHandler();
	virtual ~EventHandler();
	virtual bool OnEvent(EventBase*);

	// add and remove Event Listeners.
	void AddEventListener(EventListener *);
	void RemoveEventListener(EventListener *);

	void DispatchEvents();

private:
	// this list does not include the EventHandler itself.
	std::list<EventListener *> mRegisteredEventListeners;

	// List of the queued events. will be flushed once per frame.
	std::list<EventBase *> mEventQueue;
};



#endif // __EVENT_H__