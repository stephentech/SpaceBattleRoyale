#ifndef __WINGUI_NOTIFICATION_H__
#define __WINGUI_NOTIFICATION_H__

#include "Window.h"
#include <vector>
#include <map>

namespace wingui {

typedef Object NotificationTarget;

/** Notification struct. Defines the notification structure used to deliver message to observers. */
struct Notification
{
	int			id;
	void		*data;
	void		*sender;
	void		*context;
};

/** NotificationHandler function pointer type defines the prototype for all notification handlers. */
typedef void (NotificationTarget::*NotificationHandler)(int notId, void *data, void *sender, void *ctx);

class Notifier
{
public:	
	/** Observer struct. Defines the type for storing info about the notification listener/observer. */
	struct Observer
	{
		NotificationTarget	*target;
		NotificationHandler	handler;
		void				*context;
	};

public:
	Notifier();
	virtual ~Notifier();

	/** Returns the singleton notifier object. Alwyas use this function to get the notifier object. */
	static Notifier *GetNotifier();

	/** Adds a new notification observer */
	void AddObserver(int notificationId, NotificationTarget *pTarget, NotificationHandler handler, void *context = NULL);

	/** Removes an observer for specified notification id and target  */
	void RemoveObserver(int notificationId, NotificationTarget *pTarget, NotificationHandler handler);

	/** Removes all observers handling specified notification */
	void RemoveObservers(int notificationId);

	/** Removes all observers with specified target */
	void RemoveObservers(NotificationTarget *pTarget);	

	/** Removes all observers */
	void RemoveAllObservers();

	/** posts a notification */
	void Post(int notificationId, void *data, void *pSender);

private:
	typedef std::vector<Observer> ObserverList;
	typedef std::map<int,ObserverList> ObserverMap;
	ObserverMap		_observers;
	int				_postCount;
};

extern Notifier *gNotifier;

#define WINGUI_REGISTER_OBSERVER(_NOTIFID, _HANDLER) \
wingui::Notifier::GetNotifier()->AddObserver(_NOTIFID, reinterpret_cast<wingui::NotificationTarget *>(this), (wingui::NotificationHandler)&_HANDLER)

} // wingui namespace

#endif // !__WINGUI_NOTIFICATION_H__
