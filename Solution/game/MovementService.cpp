#include "game_pch.h"
#include "MovementService.h"
#include "egf/FlatModelManager.h"

EE_IMPLEMENT_CONCRETE_CLASS_INFO(MovementService);

MovementService::MovementService(void)
{

}

MovementService::~MovementService(void)
{

}


efd::SyncResult MovementService:: OnPreInit ( efd::IDependencyRegistrar* pDependencyRegistrar )
{

	//通过builtinModelID  在FlatModelManager 中把Factory 和 toolbench 中的modle 关联起来
	const egf::FlatModelID builtinModelID = 264070643;

	egf::FlatModelManager* pFlatModelManager =
		m_pServiceManager->GetSystemServiceAs<egf::FlatModelManager>();
	EE_ASSERT(pFlatModelManager);

	//注册MovableObject 的创建函数
	pFlatModelManager->RegisterBuiltinModelFactory(
		"MovableObject",
		builtinModelID,
		MovableObject::Factory);

	return efd::SyncResult_Success;
	



}


//注册可移动的物体
void MovementService::RegisterMovableObject
(
 const egf::EntityID& entityID, MovableObject * pObject
 )
{
	
	if (m_MovableObjectMap.find(entityID) != m_MovableObjectMap.end())
	{
		//如果已经包含这个Object的话，就先删除他
		m_MovableObjectMap.erase(entityID);
	}
	m_MovableObjectMap[entityID] = pObject;


}
//删除某个移动物体
void MovementService::UnregisterMovableObject(const egf::EntityID& entityID)
{
	m_MovableObjectMap.erase(entityID);
}

//初始化这个服务
 efd::AsyncResult MovementService::OnInit()
{

	
	return efd::AsyncResult_Complete;


}
//服务的 Update 
 efd::AsyncResult MovementService::OnTick()
{
	efd::TimeType gameTime = m_pServiceManager->GetTime(efd::kCLASSID_GameTimeClock);
	efd::TimeType timeDelta = gameTime - m_lastUpdate;
	m_lastUpdate = gameTime;
	efd::map<egf::EntityID, MovableObjectPtr>::iterator iter = m_MovableObjectMap.begin();
	while (iter != m_MovableObjectMap.end())
	{
		MovableObject* pHandler = iter->second;
		pHandler->Update(timeDelta);
		iter++;
	}

	//继续  Tick 
	return efd::AsyncResult_Pending ;

}
//服务的关闭
 efd::AsyncResult MovementService::OnShutdown()
{
	return efd::AsyncResult_Complete;

}

 //返回这个服务所要显示的名称
 const char* MovementService::GetDisplayName() const
 {
	 return "MovementService";
 }

 //设置某个实体的速度在规定的时间变化到某个值
 void MovementService::ChangeVelocityTo(const egf::EntityID& entityID,efd::Float32 fDelayTime,efd::Point3 TargetVelocity)
 {

	 MovableObjectPtr pkMovableObject = NULL;
	 if (m_MovableObjectMap.find(entityID,pkMovableObject))
	 {
		 pkMovableObject->ChangeVelocityTo(fDelayTime,TargetVelocity);
	   
	 }

 }

 //设置在规定时间内旋转到某个沿着旋转轴的某个角度
 void MovementService::ChangeAngleTo(const egf::EntityID& entityID,efd::Float32 fDelayTime,efd::Float32 fAngular)
 {
	 MovableObjectPtr pkMovableObject = NULL;
	 if (m_MovableObjectMap.find(entityID,pkMovableObject))
	 {
		 pkMovableObject->ChangeAngleTo(fDelayTime,fAngular);

	 }





 }