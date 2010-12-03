#include "game_pch.h"
#include "Subject.h"
#include "Observer.h"

namespace Event
{

	Subject::Subject(void)
	{
	}

	Subject::~Subject(void)
	{
		for (mapListObserver::iterator ite = m_mapListObserver.begin(); ite != m_mapListObserver.end(); ++ite)
		{
			ite->second->clear();
			EE_EXTERNAL_DELETE ite->second;
			ite->second = NULL;
		}
		m_mapListObserver.clear();
	}

	void Subject::registerObserver(Observer* o, EventType event)
	{
		mapListObserver::iterator ite = m_mapListObserver.find(event);

		listObserver* listOb = NULL;
		if(ite == m_mapListObserver.end())
		{
			listOb = EE_EXTERNAL_NEW listObserver();
			m_mapListObserver[event] = listOb;
		}
		else
		{
			listOb = ite->second;
		}

		NIASSERT(listOb);
		if (listOb)
		{
			listOb->push_back(o);
		}
	}
	void Subject::removeObserver(Observer* o)
	{
		for (mapListObserver::iterator iteMap = m_mapListObserver.begin(); iteMap != m_mapListObserver.end(); ++iteMap)
		{
			listObserver *listOb = iteMap->second;

			for (listObserver::iterator iteList = listOb->begin(); iteList != listOb->end(); ++ iteList)
			{
				if (*iteList == o)
				{
					listOb->erase(iteList);
					return;
				}
			}
		}
	}

	void Subject::removeEvent(EventType event)
	{
		mapListObserver::iterator iteMap = m_mapListObserver.find(event);
		listObserver *listOb = iteMap->second;

		listOb->clear();	
		m_mapListObserver.erase(iteMap);

		EE_EXTERNAL_DELETE listOb;
		listOb = NULL;
	}

	void Subject::notifyObserver(EventType event, void *arg)
	{
		mapListObserver::iterator iteMap = m_mapListObserver.find(event);
		listObserver *listOb = iteMap->second;

		for (listObserver::iterator iteList = listOb->begin(); iteList != listOb->end(); ++ iteList)
		{
			(*iteList)->OnEvent(event, this, arg);
		}
	}
}
