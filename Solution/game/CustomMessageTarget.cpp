#include "game_pch.h"
#include "CustomMessageTarget.h"

#include <efd/ServiceManager.h>

using namespace efd;
using namespace egf;

EE_IMPLEMENT_CONCRETE_CLASS_INFO(CustomMessageTarget);

EE_HANDLER(
		   CustomMessageTarget,   // The class that will handle the message
		   HandleCustomMessage,   // The member method that will handle the message
		   CustomMessage);        // The message class id to pass to MessageWrapper

EE_HANDLER_WRAP(
				CustomMessageTarget,
				HandleCustomWrapMessage,
				IMessage,
				game::kMSGID_CustomMessageWrap)


CustomMessageTarget::CustomMessageTarget(ServiceManager* pServiceManager)
	:m_pServiceManager(pServiceManager)
{
}

CustomMessageTarget::~CustomMessageTarget(void)
{
}


bool CustomMessageTarget::init ()
{
	MessageService* pMessageService = m_pServiceManager->GetSystemServiceAs<MessageService>();
	EE_ASSERT(pMessageService);

	pMessageService->Subscribe(this, efd::kCAT_LocalMessage);
	return true;
}

void CustomMessageTarget::HandleCustomMessage(
	const CustomMessage* pMessage,
	efd::Category targetChannel)
{ 
	int a = 0;
}

void CustomMessageTarget::HandleCustomWrapMessage(
	const efd::IMessage* pMessage,	
	efd::Category targetChannel)
{
	int a = 0;
}