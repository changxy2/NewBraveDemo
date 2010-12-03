#pragma once
#include <efd/IMessage.h>
#include "ID.h"


class CustomMessage : public efd::IMessage
{
	 EE_DECLARE_CLASS1(CustomMessage, game::kMSGID_CustomMessage, IMessage);

public:
	CustomMessage(void);
	virtual ~CustomMessage(void);
};

typedef efd::SmartPointer<CustomMessage> CustomMessagePtr;
