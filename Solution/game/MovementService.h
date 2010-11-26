/********************************************************************
	创建时间:	12:11:2010   16:27
	作者:		常齐
	作用:	   所有移动的服务
*********************************************************************/
#pragma once
#include "ID.h"
#include <efd/ISystemService.h>
#include <efd/Foundation.h>
#include "MovableObject.h"

class MovementService :
	public efd::ISystemService
{
	/// @cond EMERGENT_INTERNAL
	EE_DECLARE_CLASS1(
		MovementService,
		game::CLASS_ID_MovementService,
		efd::ISystemService);
	EE_DECLARE_CONCRETE_REFCOUNT;
public:
	MovementService(void);
	~MovementService(void);

	
	virtual efd::SyncResult  OnPreInit ( efd::IDependencyRegistrar* pDependencyRegistrar );
	//初始化这个服务
	virtual efd::AsyncResult OnInit();
	//服务的 Update 
	virtual efd::AsyncResult OnTick();
	//服务的关闭
	virtual efd::AsyncResult OnShutdown();

	//注册可移动的物体
	void RegisterMovableObject(const egf::EntityID& entityID, MovableObject * pObject);

	//删除某个移动物体
	void UnregisterMovableObject(const egf::EntityID& entityID);

	//返回这个服务所要显示的名称
	const char* GetDisplayName() const;


	//设置某个实体的速度在规定的时间变化到某个值
	void ChangeVelocityTo(const egf::EntityID& entityID,efd::Float32 fDelayTime,efd::Point3 TargetVelocity);

	//设置在规定时间内旋转到某个沿着旋转轴的某个角度
	void ChangeAngleTo(const egf::EntityID& entityID,efd::Float32 fDelayTime,efd::Float32 fAngular);
	
		

protected:

	// 所有 可以动物体的表
	efd::map<egf::EntityID, MovableObjectPtr> m_MovableObjectMap;

	/// 上一次调用的时间，用来计算 delta time 用的
	efd::TimeType m_lastUpdate;




};


