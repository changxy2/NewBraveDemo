/********************************************************************
	创建时间:	6:12:2010   11:05
	作者:		常齐
	作用:	  物理控制的角色对象
*********************************************************************/
#pragma once
#include "ID.h"
#include <egf/IBuiltinModelImpl.h>
#include <egf/BuiltinModelHelper.h>
#include <NxController.h> 


// 通过 检查 输出的 flat model 值 确定 PropertyID 值 
static const egf::PropertyID kPropertyID_MoveVelocity = 3475307868;
static const egf::PropertyID kPropertyID_MoveAccel    = 3075443446;
static const egf::PropertyID kPropertyID_RotationAxis = 3391897551;
static const egf::PropertyID kPropertyID_AngularSpeed = 1367195968;

class PhysxCharacterMoveObject :
	public  egf::IBuiltinModelImpl
{
	EE_DECLARE_CLASS1(PhysxCharacterMoveObject, game::CLASS_ID_PhysCharacterMoveObject, egf::IBuiltinModelImpl);
	EE_DECLARE_CONCRETE_REFCOUNT;


	//可以用 EE_BUILTINMODELPROPERTY 宏实现的属性
	//这个宏绑定成员函数变量到model 的属性。
	EE_DECLARE_BUILTINMODEL_PROPERTIES

		EE_BUILTINMODELPROPERTY_ACCESSOR_REF(
		PhysxCharacterMoveObject, 
		game::PROPRTY_ID_Dimension, 
		efd::Point3, 
		PhysxCharacterMoveObject, 
		GetDimension,
		SetDimension
		)

		EE_BUILTINMODELPROPERTY_ACCESSOR_REF(
		PhysxCharacterMoveObject, 
		game::PROPRTY_ID_SceneName, 
		efd::utf8string, 
		PhysxCharacterMoveObject, 
		GetSceneName,
		SetSceneName
		)


		EE_BUILTINMODELPROPERTY_ACCESSOR
		(
		PhysxCharacterMoveObject, 
		game::PROPRTY_ID_CollisionGroup, 
		efd::UInt32,
		PhysxCharacterMoveObject,
		GetCollisionGroup, 
		SetCollisionGroup
		)

		


		EE_BUILTINMODELPROPERTY_ACCESSOR_REF(
		PhysxCharacterMoveObject, 
		kPropertyID_RotationAxis, 
		efd::Point3,
		PhysxCharacterMoveObject,
		GetRotationAxis, 
		SetRotationAxis)


		EE_BUILTINMODELPROPERTY_ACCESSOR_REF(
		PhysxCharacterMoveObject, 
		kPropertyID_AngularSpeed, 
		efd::Float32, 
		PhysxCharacterMoveObject, 
		GetAngularSpeed,
		SetAngularSpeed
		)

		EE_END_BUILTINMODEL_PROPERTIES

public:

	PhysxCharacterMoveObject(void);
	virtual ~PhysxCharacterMoveObject(void);

	//这里是PhysxCharacterMoveObject 的创建工厂，引擎用他来创建built in Model 实例
	static egf::IBuiltinModel* Factory()
	{
		return EE_NEW PhysxCharacterMoveObject();
	}


	

	//设置某个实体的速度在规定的时间变化到某个值
	void ChangeHorizontalVelocityTo(efd::Float32 fDelayTime,efd::Point2 TargetVelocity);

	//设置在规定时间内旋转到某个沿着旋转轴的某个角度
	void ChangeAngleTo(efd::Float32 fDelayTime,efd::Float32 TargetfAngular);

	
	// 当包含这个model 的实体，初始化构造完成后，第一个调用就是这个
	void  OnAdded();

	//当包含 这个model 的实体，被删除的时候调用这个函数
	void OnRemoved();


	//更新函数，主要是对位置的更新，包括NxController
	void Update(efd::TimeType timeDelta);


	

	//取的包围盒尺寸
	const efd::Point3& GetDimension() const
	{
		return m_Dimension;
	}
	//设置包围盒尺寸
	void SetDimension(const efd::Point3& Dim)
	{
		m_Dimension = Dim;

	}

	//取得场景名称
	const efd::utf8string& GetSceneName() const 
	{
		return m_sceneName;

	}
	void SetSceneName(const efd::utf8string& sceneName)
	{
		m_sceneName = sceneName;

	}
	
	//取的碰撞组
	efd::UInt32 GetCollisionGroup() const
	{
		return m_CollisionGroupID;

	}
	//设置碰撞组
	void SetCollisionGroup(const efd::UInt32& CollisionGroupID)
	{
		m_CollisionGroupID = CollisionGroupID;

	}

	


	//设置和取得转动轴
	const efd::Point3& GetRotationAxis() const
	{

		return m_prop3_RotationAxis;
	}
	void SetRotationAxis(const efd::Point3& kRotationAxis)
	{

		m_prop3_RotationAxis = kRotationAxis;
		//当通过Entity::SetPropertyValue 设置属性，dirty 位自动设置。
		//如果应用程序直接调用SetRotationAxis  修改成员，他会想调用Dirty自己
	}

	//取得角速度
	const efd::Float32& GetAngularSpeed() const
	{

		return m_prop4_AngularSpeed;
	}

	//设置角速度
	void SetAngularSpeed(const efd::Float32& AngularSpeed)
	{
		m_prop4_AngularSpeed = AngularSpeed;

	}

	
	

protected:
	//取得PhysxController 的位置
	const NxExtendedVec3& GetPhysxControllerPos();

	//移动角色dispVector ， 是想要移动的偏移量
	NxU32  MoveCharacter(const NxVec3& dispVector, efd::UInt32 collisionGroups);

	//设置角色的位置。这个设置的是绝对的位置
	efd::UInt32 SetCharacterPos(const NxVec3& pos);

	//尺寸
	efd::Point3 m_Dimension;
	//场景名称
	efd::utf8string m_sceneName;
	//碰撞组编号
	efd::UInt32 m_CollisionGroupID;

	//这个是控制Physx 的 NxController
	NxController* m_pPhysxController;

	//系通服务管理器
	const efd::ServiceManager* m_pServiceManager;
	
	
	//垂直加速度
	efd::Float32 m_AccelOfGravity;
	//垂直速度
	efd::Float32 m_VelOfVertical;


	//当前水平速度
	efd::Point2 m_MoveVelocityOfHorizontal;
	//水平加速度
	efd::Point2 m_AccelOfHorizontal;
	//水平目标速度
	efd::Point2 m_TargetVelocityOfHorizontal;
	//向水平目标速度过渡的剩余时间
	efd::Float32 m_LeftTimeForTransitionofHorizontalVelocity;
	
	
	//向目标角度过渡的剩余时间
	efd::Float32 m_LeftTimeForTransitionOfAngular;
	//转动轴
	efd::Point3 m_prop3_RotationAxis;
	//目标角度
	efd::Float32 m_TargetAngular;
	//角速度,这个是一弧度/秒 为单位的
	efd::Float32 m_prop4_AngularSpeed;


	

};
typedef efd::SmartPointer<PhysxCharacterMoveObject> PhysCharacterMoveObjectPtr;
