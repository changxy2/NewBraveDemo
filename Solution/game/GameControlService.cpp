#include "game_pch.h"
#include "GameControlService.h"

#include <efd/MessageService.h>
#include <ecrInput/InputService.h>

EE_IMPLEMENT_CONCRETE_CLASS_INFO(GameControlService);

EE_HANDLER(
		   GameControlService,
		   HandleInputActionMessage,
		   ecrInput::InputActionMessage);

GameControlService::GameControlService(void)
{


}

const char* GameControlService::GetDisplayName() const
{
	return "GameControlService";
}


efd::SyncResult GameControlService::OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar)
{// 控制服务器主要处理的就是输入和消息的影射 这里声明需要的服务器

	pDependencyRegistrar->AddDependency<efd::MessageService>();
	pDependencyRegistrar->AddDependency<ecrInput::InputService>();

	return efd::SyncResult_Success;
}


efd::AsyncResult GameControlService::OnInit()
{
	efd::MessageService* pMessageService =
		m_pServiceManager->GetSystemServiceAs<efd::MessageService>();	
	EE_ASSERT(pMessageService);

	ecrInput::InputService* pInputService = 
		m_pServiceManager->GetSystemServiceAs<ecrInput::InputService>();	
	EE_ASSERT(pInputService);

	// 加载输入影射文件
	EE_VERIFY(pInputService->LoadAllActionMaps("urn:demoActionMap"));

	// 下面注册需要监听的事件
	m_inputCategory = pMessageService->GetGloballyUniqueCategory();
	pMessageService->Subscribe(this, m_inputCategory);

	pInputService->ListenForInputActionEvent("Test", m_inputCategory);
	pInputService->ListenForInputActionEvent("TestComBo", m_inputCategory);
	pInputService->ListenForInputActionEvent("MoveForward", m_inputCategory);

	return efd::AsyncResult_Complete;
}


efd::AsyncResult GameControlService::OnShutdown()
{
	// 反注册
	efd::MessageService* pMessageService =
		m_pServiceManager->GetSystemServiceAs<efd::MessageService>();	
	if (!pMessageService)
	{
		EE_ASSERT(pMessageService);
		return efd::AsyncResult_Failure;
	}

	pMessageService->Unsubscribe(this, m_inputCategory);
	return efd::AsyncResult_Complete;
}

void GameControlService::HandleInputActionMessage(
	const ecrInput::InputActionMessage* pMessage, 
	efd::Category category)
{
	// 处理所有事件	
	if ("Test" == pMessage->GetEventName())
	{
		EE_LOG(efd::kApp, 1, (
			"GameControlService::HandleInputActionMessage: Test"));
	}	
	else if ("TestComBo" == pMessage->GetEventName())
	{
		EE_LOG(efd::kApp, 1, (
			"GameControlService::HandleInputActionMessage: TestComBo"));
	}
}


