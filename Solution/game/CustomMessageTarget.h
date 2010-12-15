#pragma once
#ifndef CUSTOMMESSAGETARGET_H
#define CUSTOMMESSAGETARGET_H

#include <efd\IBase.h>
#include "CustomMessage.h"

class CustomMessageTarget :	public efd::IBase
{
	EE_DECLARE_CLASS1(CustomMessageTarget, game::CLASS_ID_CustomMessageTarget, efd::IBase);
	EE_DECLARE_CONCRETE_REFCOUNT;

public:
	CustomMessageTarget(efd::ServiceManager* pServiceManager);
	virtual ~CustomMessageTarget(void);

	bool init ();

	void HandleCustomMessage( const CustomMessage* pMessage, efd::Category targetChannel);

	void HandleCustomWrapMessage(const efd::IMessage* pMessage,	efd::Category targetChannel);
		

protected:

	/// Local pointer to the Service Manager
	efd::ServiceManager* m_pServiceManager;
};

typedef efd::SmartPointer<CustomMessageTarget> CustomMessageTargetPtr;

#endif // CUSTOMMESSAGETARGET_H