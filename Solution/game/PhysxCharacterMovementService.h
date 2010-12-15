/********************************************************************
	创建时间:	7:12:2010   11:27
	作者:		常齐
	作用:	  是对PhysxCharacter 控制器的管理服务
*********************************************************************/
#pragma once
#include <NxControllerManager.h>
#include "PhysxCharacterMoveObject.h"
#include "efdPhysX\PhysXAllocator.h"
#include <ecrPhysX\NiPhysXSceneAdded.h>
#include <egfPhysX\SceneMessage.h>
#include <NiPhysXScene.h>

class PhysxCharacterMovementService
	:public efd::ISystemService
{

	/// @cond EMERGENT_INTERNAL
	EE_DECLARE_CLASS1(
		PhysxCharacterMovementService,
		game::SERVICE_ID_PhysxCharacterMovementService,
		efd::ISystemService);
	EE_DECLARE_CONCRETE_REFCOUNT
public:
	PhysxCharacterMovementService(efdPhysX::PhysXAllocator* pAllocator);
	virtual ~PhysxCharacterMovementService(void);

	virtual efd::SyncResult  OnPreInit ( efd::IDependencyRegistrar* pDependencyRegistrar );
	//初始化这个服务
	virtual efd::AsyncResult OnInit();
	//服务的 Update 
	virtual efd::AsyncResult OnTick();
	//服务的关闭
	virtual efd::AsyncResult OnShutdown();

	//注册可移动的物体
	void RegisterMovableObject(const egf::EntityID& entityID, PhysxCharacterMoveObject * pObject, NxControllerDesc& desc, NxController ** ppResultPhysxController);

	//删除某个移动物体
	void UnregisterMovableObject(const egf::EntityID& entityID);

	//返回这个服务所要显示的名称
	const char* GetDisplayName() const;


	//设置某个实体的速度在规定的时间变化到某个值
	void ChangeHorizontalVelocityTo(const egf::EntityID& entityID,efd::Float32 fDelayTime,efd::Point2 TargetVelocity);

	//设置在规定时间内旋转到某个沿着旋转轴的某个角度
	void ChangeAngleTo(const egf::EntityID& entityID,efd::Float32 fDelayTime,efd::Float32 fAngular);

	//添加物理场景的时候要处理的事件
	void HandlePhysXSceneAddedMessage
		(
		const ecrPhysX::NiPhysXSceneAdded* pMessage,
		efd::Category targetChannel
		);
	

	//删除物理场景的时候，要处理的事件
	void HandlePhysXSceneRemovedMessage
		(
		const egfPhysX::SceneMessage* pMessage,
		efd::Category targetChannel
		);
	

protected:

	//Physx 控制器管理器
	NxControllerManager* m_pkNxControllerManager;

	// 所有 可以动物体的表
	efd::map<egf::EntityID, PhysCharacterMoveObjectPtr> m_CharacterMoveObjectMap;
	//所有的控制器
	efd::map<egf::EntityID, NxController * > m_NxControllerMap;




	// 上一次调用的时间，用来计算 delta time 用的
	efd::TimeType m_LastUpdate;

	//这里确定物理模拟是同步还是异步执行
	bool m_runSynchronously;

	//内存分配器，用来创建CharacterController 
	efdPhysX::PhysXAllocator* m_pAllocator;

	//当前物理场景的名字
	efd::utf8string m_CurrentSceneName;

	//当前的物理场景
	NiPhysXScene* m_spPhysXScene;

	//这是真正的物理场景
	NxScene* m_pNxScene;



};
