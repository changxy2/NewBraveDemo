#pragma once
#include <NiMemObject.h>
#include "ObserverEvent.h"

namespace Event
{
	class Observer;
	class Subject :	public NiMemObject
	{
	public:
		Subject(void);
		virtual ~Subject(void);


		void registerObserver(Observer* o, EventType event);
		void removeObserver(Observer* o);
		void removeEvent(EventType event);
		void notifyObserver(EventType event,void *arg);

	private:
		typedef efd::list<Observer*> listObserver;
		typedef efd::map<EventType, listObserver*> mapListObserver;

		mapListObserver m_mapListObserver;

		void ClearUp();

	};

}
