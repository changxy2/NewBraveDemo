// ======================================================================================
// File         : ControlServer.h
// Author       : Zhang yuzhong 
// Last Change  : 11/9/2010
// Description  : mapping message from input service to registered control
//				  delegates and other game state management logic
// ======================================================================================
#pragma once
#ifndef CONTROLSERVICE_H
#define CONTROLSERVICE_H

#include <ecrInput/InputActionMessage.h>
#include <efd/ISystemService.h>
#include <efd/Foundation.h>

class GameControlService :	public efd::ISystemService
{
	EE_DECLARE_CLASS1(GameControlService, game::SERVICE_ID_ControlServer, ISystemService);
	EE_DECLARE_CONCRETE_REFCOUNT;

public:
	GameControlService(void);
	virtual ~GameControlService(void){}

	virtual const char *GetDisplayName() const;

	// 所有注册了的Input Action message 的处理入口
	// @param pMessage : Input Action message
	// @param category : Message categories for action mapped events 
	void HandleInputActionMessage(
		const ecrInput::InputActionMessage* pMessage,
		efd::Category category);

protected:
	// 声明OnInit（）需求的服务 
	virtual efd::SyncResult OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar);

	// 
	virtual efd::AsyncResult OnInit();

	// 
	virtual efd::AsyncResult OnShutdown();

private:
	/// The category on which we listen for actions
	efd::Category m_inputCategory;
};

typedef efd::SmartPointer<GameControlService> GameControlServicePtr;

#endif // CONTROLSERVICE_H
