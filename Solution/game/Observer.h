#pragma once
#include <NiMemObject.h>

#include "ObserverEvent.h"

namespace Event
{
	class Subject;
	class Observer : public NiMemObject
	{
	public:
		Observer(void);
		virtual ~Observer(void);

		virtual void OnEvent(EventType event, Subject* pSubject,void *arg) = 0;
	};
}
