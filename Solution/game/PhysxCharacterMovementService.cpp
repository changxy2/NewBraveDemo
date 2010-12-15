#include "game_pch.h"
#include "PhysxCharacterMovementService.h"
#include <egfPhysX/SceneService.h>
#include <ecrPhysX\PropService.h>
#include <ecrPhysX\NiPhysXSceneAdded.h>
#include "PhysxCharacterMoveObject.h"
#include "PhysXSceneHandle.h"


EE_IMPLEMENT_CONCRETE_CLASS_INFO(PhysxCharacterMovementService);

//PhysX 场景添加的消息,这个消息来自于PropService
EE_HANDLER(PhysxCharacterMovementService, HandlePhysXSceneAddedMessage, ecrPhysX::NiPhysXSceneAdded);

//这个消息来自于SceneService
EE_HANDLER_WRAP(
				PhysxCharacterMovementService,
				HandlePhysXSceneRemovedMessage,
				egfPhysX::SceneMessage,
				efd::kMSGID_PhysXSceneRemoved);


PhysxCharacterMovementService::PhysxCharacterMovementService(efdPhysX::PhysXAllocator* pAllocator)
:m_pAllocator(pAllocator),
m_pNxScene(NULL),
m_spPhysXScene(NULL)
{
	
}

PhysxCharacterMovementService::~PhysxCharacterMovementService(void)
{


}

efd::SyncResult  PhysxCharacterMovementService::OnPreInit ( efd::IDependencyRegistrar* pDependencyRegistrar )
{
	pDependencyRegistrar->AddDependency<efd::MessageService>();
	pDependencyRegistrar->AddDependency<egfPhysX::SceneService>();
	pDependencyRegistrar->AddDependency<ecrPhysX::PropService>();

	//通过builtinModelID  在FlatModelManager 中把Factory 和 toolbench 中的modle 关联起来
	const egf::FlatModelID builtinModelID = 201573187;

	egf::FlatModelManager* pFlatModelManager =
		m_pServiceManager->GetSystemServiceAs<egf::FlatModelManager>();
	EE_ASSERT(pFlatModelManager);

	//注册PhysxCharacterMoveObject 的创建函数
	pFlatModelManager->RegisterBuiltinModelFactory(
		"PhysxCharacterMoveObject",
		builtinModelID,
		PhysxCharacterMoveObject::Factory);

	return efd::SyncResult_Success;


}
//初始化这个服务
efd::AsyncResult PhysxCharacterMovementService::OnInit()
{
	efd::MessageService* pMessageService =
		m_pServiceManager->GetSystemServiceAs<efd::MessageService>();

	EE_ASSERT(pMessageService);
//要接收PhysX 消息
	pMessageService->Subscribe(this, egfPhysX::kCAT_egfPhysXMessage);

	EE_ASSERT(efdPhysX::PhysXSDKManager::GetManager());

	EE_ASSERT(m_pAllocator);
	m_pkNxControllerManager = NxCreateControllerManager(m_pAllocator);

	EE_ASSERT(m_pkNxControllerManager);
	return efd::AsyncResult_Complete;

}
//服务的 Update 
efd::AsyncResult PhysxCharacterMovementService::OnTick()
{
	efd::TimeType gameTime = m_pServiceManager->GetTime(efd::kCLASSID_GameTimeClock);
	efd::TimeType timeDelta = gameTime - m_LastUpdate;


	m_LastUpdate = gameTime;
	efd::map<egf::EntityID, PhysCharacterMoveObjectPtr>::iterator iter = m_CharacterMoveObjectMap.begin();
	while (iter != m_CharacterMoveObjectMap.end())
	{
		PhysxCharacterMoveObject * pHandler = iter->second;
		pHandler->Update(timeDelta);
		iter++;
	}

	//更新控制器
	m_pkNxControllerManager->updateControllers();
	//继续  Tick 
	return efd::AsyncResult_Pending ;

}
//服务的关闭
efd::AsyncResult PhysxCharacterMovementService::OnShutdown()
{
	NxReleaseControllerManager(m_pkNxControllerManager);
	return efd::AsyncResult_Complete;

}

//注册可移动的物体
void PhysxCharacterMovementService::RegisterMovableObject(const egf::EntityID& entityID, PhysxCharacterMoveObject * pObject, NxControllerDesc& desc, NxController ** ppResultPhysxController)
{
	if (m_CharacterMoveObjectMap.find(entityID) != m_CharacterMoveObjectMap.end())
	{
		//如果已经包含这个Object的话，就先删除他
		m_CharacterMoveObjectMap.erase(entityID);
	}

	if(m_pNxScene)
	{
		

		desc.position.x *= m_spPhysXScene->GetScaleFactor();
		desc.position.y *= m_spPhysXScene->GetScaleFactor();
		desc.position.z *= m_spPhysXScene->GetScaleFactor();
		desc.stepOffset *=(1.0f / m_spPhysXScene->GetScaleFactor());
		* ppResultPhysxController = m_pkNxControllerManager->createController(m_pNxScene,desc);
		m_NxControllerMap[entityID] = * ppResultPhysxController;

		m_CharacterMoveObjectMap[entityID] = pObject;
	}

}

//删除某个移动物体
void PhysxCharacterMovementService::UnregisterMovableObject(const egf::EntityID& entityID)
{
	   
	m_pkNxControllerManager->releaseController(*m_NxControllerMap[entityID]);
	m_CharacterMoveObjectMap.erase(entityID);
	m_NxControllerMap.erase(entityID);


}

//返回这个服务所要显示的名称
const char* PhysxCharacterMovementService::GetDisplayName() const
{
	 return "PhysxCharacterMovementService";
}


//设置某个实体的速度在规定的时间变化到某个值
void PhysxCharacterMovementService::ChangeHorizontalVelocityTo(const egf::EntityID& entityID,efd::Float32 fDelayTime,efd::Point2 TargetVelocity)
{
	PhysCharacterMoveObjectPtr pkMovableObject = NULL;
	if (m_CharacterMoveObjectMap.find(entityID,pkMovableObject))
	{
		pkMovableObject->ChangeHorizontalVelocityTo(fDelayTime,TargetVelocity);
	}
}

//设置在规定时间内旋转到某个沿着旋转轴的某个角度
void PhysxCharacterMovementService::ChangeAngleTo(const egf::EntityID& entityID,efd::Float32 fDelayTime,efd::Float32 fAngular)
{
	PhysCharacterMoveObjectPtr pkMovableObject = NULL;
	if (m_CharacterMoveObjectMap.find(entityID,pkMovableObject))
	{
		pkMovableObject->ChangeAngleTo(fDelayTime,fAngular);

	}
}

//添加物理场景的时候要处理的事件
void PhysxCharacterMovementService::HandlePhysXSceneAddedMessage
(
 const ecrPhysX::NiPhysXSceneAdded* pMessage,
 efd::Category targetChannel
 )
{
	
	//通过消息，取得要添加的场景的名称
	const efd::utf8string& sceneName = pMessage->GetSceneName();

	ecrPhysX::PropService* pPropService =
		m_pServiceManager->GetSystemServiceAs<ecrPhysX::PropService>();
	EE_ASSERT(pPropService);
    //因为这个消息是从PropService 发出的，所以PropService 已经有了这个场景
	NiPhysXScene* pScene = pPropService->GetScene(sceneName);
	
	if (pScene)
	{
		PhysXSceneHandlePtr spSceneHandle = NULL;
		
		spSceneHandle = EE_NEW PhysXSceneHandle(pScene);
	}

	m_CurrentSceneName = sceneName;

	m_spPhysXScene = pScene;
	m_pNxScene = pScene->GetPhysXScene();
	//SetupNxScene(sceneName);
}

//删除物理场景的时候，要处理的事件
void PhysxCharacterMovementService::HandlePhysXSceneRemovedMessage
(
 const egfPhysX::SceneMessage* pMessage,
 efd::Category targetChannel
 )
{
	//因为场景已经被删除了，所以所有的控制器，要被删除了
	//这里会删除所有控制器
	// The Character controllers exist in the PhysX Scenes, so clean them up.
	//DestroyControllers();


	if (m_CurrentSceneName == pMessage->GetSceneName())
	{
		m_CurrentSceneName = efd::utf8string::NullString();
	}

	if (pMessage->GetScene() == m_pNxScene)
	{
		m_spPhysXScene = NULL;
		m_pNxScene = NULL;
	}

}
