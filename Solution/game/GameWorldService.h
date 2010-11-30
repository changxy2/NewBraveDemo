// ======================================================================================
// File         : GameWorldService.h
// Author       : Zhang yuzhong 
// Last Change  : 11/30/2010
// Description  : 加载block 和 entity 接口
//				  
// ======================================================================================

#pragma once
#include <efd/ISystemService.h>
#include <egf/Entity.h>

namespace egf
{
	class EntityLoadResult;
	class EntityFactoryResponse;
}

class GameWorldService : public efd::ISystemService
{
	EE_DECLARE_CLASS1(GameWorldService, game::SERVICE_ID_WorldService, ISystemService);
	EE_DECLARE_CONCRETE_REFCOUNT;

public:
	GameWorldService(void);
	virtual ~GameWorldService(void);

	virtual const char *GetDisplayName() const;

protected:
	// 声明OnInit（）需求的服务 
	virtual efd::SyncResult OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar);

	// 
	virtual efd::AsyncResult OnInit();

	// 
	virtual efd::AsyncResult OnShutdown();

public:
	// 加载block场景
	void LoadBlock(const efd::utf8string& blockUrn, bool autoEnter = false);
	// 卸载
	void UnLoadBlock(const efd::utf8string& blockUrn);

	// 生成entity
	egf::EntityID SpawnEntity(const efd::utf8string& modelUrn);
	// 删除entity
	void DeleteEntity(const egf::EntityID& entityId);

	void HandleEntityLoadResult(
		const egf::EntityLoadResult* pWorldMessage,
		efd::Category targetChannel);

	void HandleEntityFactoryResponse(
		const egf::EntityFactoryResponse* pResponse,
		efd::Category targetCategory);

private:
	efd::Category m_blockCat;
	efd::Category m_entityCat;
};

typedef efd::SmartPointer<GameWorldService> GameWorldServicePtr;
