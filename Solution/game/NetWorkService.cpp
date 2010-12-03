#include "game_pch.h"
#include "NetWorkService.h"

EE_IMPLEMENT_CONCRETE_CLASS_INFO(NetWorkService);

NetWorkService::NetWorkService(void)
{
}

NetWorkService::~NetWorkService(void)
{
}

const char * NetWorkService::GetDisplayName() const
{
	return "NetWorkService";
}

	// 声明OnInit（）需求的服务 
efd::SyncResult NetWorkService::OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar)
{
	return efd::SyncResult_Success;
}

	// 
efd::AsyncResult NetWorkService::OnInit()
{
	// 注册各个网络消息的处理
	return efd::AsyncResult_Complete;
}

	// 
efd::AsyncResult NetWorkService::OnShutdown()
{
	return efd::AsyncResult_Complete;
}

efd::AsyncResult NetWorkService::OnTick()
{
	// 处理每个网络消息
	return efd::AsyncResult_Pending;
}
