// ======================================================================================
// File         : GameWorldService.h
// Author       : Zhang yuzhong 
// Last Change  : 12/2/2010
// Description  : 网络消息处理
//				  
// ======================================================================================

#pragma once
#include <efd\ISystemService.h>

class NetWorkService : 	public efd::ISystemService
{
	EE_DECLARE_CLASS1(NetWorkService, game::SERVICE_ID_NetWorkService, ISystemService);
	EE_DECLARE_CONCRETE_REFCOUNT;

public:
	NetWorkService(void);
	virtual ~NetWorkService(void);

	virtual const char *GetDisplayName() const;

protected:
	// 声明OnInit（）需求的服务 
	virtual efd::SyncResult OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar);

	// 
	virtual efd::AsyncResult OnInit();

	// 
	virtual efd::AsyncResult OnShutdown();

	virtual efd::AsyncResult OnTick();

public:

	typedef void (*MsgNotifyPtr)(const char*, int);
	typedef	efd::hash_map<int, MsgNotifyPtr>	MsgHandlerMap;

	MsgHandlerMap  m_omapMsgHandler; 
};

typedef efd::SmartPointer<NetWorkService> NetWorkServicePtr;