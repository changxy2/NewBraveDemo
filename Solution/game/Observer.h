#pragma once
#include <NIRefobject.h>

#include "ObserverEvent.h"
namespace Event
{
	class Subject;
	class Observer : public NiRefObject
	{
	public:
		Observer(void);
		virtual ~Observer(void);

		virtual void OnEvent(EventType event, Subject* pSubject,void *arg) = 0;
	};
}
