#include "stdafx.h"
#include <wingui/Notification.h>

namespace wingui {

Notifier *gNotifier = NULL;
static Notifier *g_ActiveNotifier = NULL;

Notifier::Notifier() :
	_postCount(0)
{
	WINGUI_ASSERT(g_ActiveNotifier == NULL);
	g_ActiveNotifier = this;
}

Notifier::~Notifier()
{
	WINGUI_ASSERT(g_ActiveNotifier == this);
	g_ActiveNotifier = NULL;
}

Notifier *Notifier::GetNotifier()
{
	WINGUI_ASSERT(g_ActiveNotifier != NULL);
	return g_ActiveNotifier;
}

void Notifier::AddObserver(int notificationId, NotificationTarget *pTarget, NotificationHandler handler, void *context)
{
	WINGUI_ASSERT(_postCount == 0);

	std::map<int,std::vector<Observer>>::iterator pos = _observers.find(notificationId);
	if (pos == _observers.end()) {	
		// add new observer list for thie notification
		std::vector<Observer> list;
		_observers[notificationId] = list;
	}

	std::vector<Observer> &observerList = _observers[notificationId];
	for (std::vector<Observer>::iterator it = observerList.begin(); it != observerList.end(); it++) {
		// if the target and handler already exists for this notification, just update the context data
		if (it->target == pTarget && it->handler == handler) {
			it->context = context;
			return;
		}
	}

	// add new target
	Observer observer;
	observer.target = pTarget;
	observer.handler = handler;
	observer.context = context;
	observerList.push_back(observer);
}

void Notifier::RemoveObserver(int notificationId, NotificationTarget *pTarget, NotificationHandler handler)
{
	WINGUI_ASSERT(_postCount == 0);

	std::map<int,std::vector<Observer>>::iterator pos = _observers.find(notificationId);
	if (pos != _observers.end()) {
		std::vector<Observer> &observerList = pos->second;
		for (std::vector<Observer>::iterator it = observerList.begin(); it != observerList.end(); it++) {
			// if the target and handler exists for this notification remove it
			if (it->target == pTarget && it->handler == handler) {
				observerList.erase(it);
				return;
			}
		}
	}
}

void Notifier::RemoveObservers(int notificationId)
{
	WINGUI_ASSERT(_postCount == 0);

	std::map<int,std::vector<Observer>>::iterator pos = _observers.find(notificationId);
	if (pos != _observers.end()) {
		std::vector<Observer> &observerList = pos->second;
		observerList.clear();
	}
}

void Notifier::RemoveObservers(NotificationTarget *pTarget)
{
	WINGUI_ASSERT(_postCount == 0);

	for (ObserverMap::iterator pos = _observers.begin(); pos != _observers.end(); pos++) {
		ObserverList &observerList = pos->second;

		std::vector<ObserverList::iterator> temp;

		for (ObserverList::iterator it = observerList.begin(); it != observerList.end(); it++) {
			if (it->target == pTarget) {
				temp.push_back(it);
			}
		}

		for (std::vector<ObserverList::iterator>::iterator it = temp.begin(); it != temp.end(); it++) {
			observerList.erase(*it);
		}
	}
}

void Notifier::RemoveAllObservers()
{
	WINGUI_ASSERT(_postCount == 0);

	_observers.clear();
}

void Notifier::Post(int notificationId, void *data, void *pSender)
{
	++_postCount;

	// create a new notification
	Notification notif;
	Notification *_curNotif = &notif;

	_curNotif->id = notificationId;
	_curNotif->data = data;
	_curNotif->sender = pSender;
	_curNotif->context = NULL;

	ObserverMap::iterator pos = _observers.find(notificationId);
	if (pos != _observers.end()) {
		ObserverList &observerList = pos->second;
		for (ObserverList::iterator it = observerList.begin(); it != observerList.end(); it++) {
			NotificationTarget *pTarget = it->target;
			NotificationHandler handler = it->handler;
			// set the context for this target and call the handler
			_curNotif->context = it->context;
			(pTarget->*handler)(_curNotif->id, _curNotif->data, _curNotif->sender, _curNotif->context);
		}
	}

	--_postCount;
}

} // wingui namespace
