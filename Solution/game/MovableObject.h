/********************************************************************
创建时间:	12:11:2010   16:29
作者:		常齐
作用:		可移动物体
*********************************************************************/
#pragma once


#include "ID.h"
#include <egf/IBuiltinModelImpl.h>
#include <egf/BuiltinModelHelper.h>

// 通过 检查 输出的 flat model 值 确定 PropertyID 值 
static const egf::PropertyID kPropertyID_prop1_MoveVelocity = 3475307868;
static const egf::PropertyID kPropertyID_prop2_MoveAccel    = 3075443446;
static const egf::PropertyID kPropertyID_prop3_RotationAxis = 3391897551;
static const egf::PropertyID kPropertyID_prop4_AngularSpeed = 1367195968;


class MovableObject :
	public egf::IBuiltinModelImpl
{
	EE_DECLARE_CLASS1(MovableObject, game::CLASS_ID_MovableObject, egf::IBuiltinModelImpl);
	EE_DECLARE_CONCRETE_REFCOUNT;


	     //可以用 EE_BUILTINMODELPROPERTY 宏实现的属性
	     //这个宏绑定成员函数变量到model 的属性。
	    EE_DECLARE_BUILTINMODEL_PROPERTIES

		//这里我们直接绑定属性描述到 m_prop1_MoveDirection 的地址:
		//用Get/Set 存储器方法

		EE_BUILTINMODELPROPERTY_ACCESSOR_REF(
		MovableObject, 
		kPropertyID_prop1_MoveVelocity, 
		efd::Point3, 
		MovableObject, 
		GetVelocity,
		SetVelocity
		)

		EE_BUILTINMODELPROPERTY_ACCESSOR_REF(
		MovableObject, 
		kPropertyID_prop2_MoveAccel, 
		efd::Point3, 
		MovableObject, 
		GetAccel,
		SetAccel
		)


		EE_BUILTINMODELPROPERTY_ACCESSOR_REF(
		MovableObject, 
		kPropertyID_prop3_RotationAxis, 
		efd::Point3,
		MovableObject,
		GetRotationAxis, 
		SetRotationAxis)


		EE_BUILTINMODELPROPERTY_ACCESSOR_REF(
		MovableObject, 
		kPropertyID_prop4_AngularSpeed, 
		efd::Float32, 
		MovableObject, 
		GetAngularSpeed,
		SetAngularSpeed
		)
		EE_END_BUILTINMODEL_PROPERTIES


		
public:

	//设置某个实体的速度在规定的时间变化到某个值
	void ChangeVelocityTo(efd::Float32 fDelayTime,efd::Point3 TargetVelocity);

	//设置在规定时间内旋转到某个沿着旋转轴的某个角度
	void ChangeAngleTo(efd::Float32 fDelayTime,efd::Float32 fAngular);

	//移动物体的更新函数
	void Update(efd::TimeType timeDelta);

	// 当包含这个model 的实体，初始化构造完成后，第一个调用就是这个
	void  OnAdded();

	//当包含 这个model 的实体，被删除的时候调用这个函数
	void OnRemoved();

	MovableObject(void);
	~MovableObject(void);

	//这里是MovableObject 的创建工厂，引擎用他来创建built in Model 实例
	static egf::IBuiltinModel* MovableObject::Factory()
	{
		return EE_NEW MovableObject();
	}


	// 应用程序 getter
	const efd::Point3& GetVelocity() const
	{

		return m_prop1_MoveVelocity;
	}
	void SetVelocity(const efd::Point3& MoveVelocity)
	{
		m_prop1_MoveVelocity = MoveVelocity;
	}



	
	// 应用程序 getter
	const efd::Point3& GetAccel() const
	{

		return m_prop2_Accel;
	}

	void SetAccel(const efd::Point3& Accel)
	{
		m_prop2_Accel = Accel;
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

	//系通服务管理器
	const efd::ServiceManager* m_pServiceManager;
	//当前速度
	efd::Point3 m_prop1_MoveVelocity;
	//加速度
	efd::Point3 m_prop2_Accel;
	//转动轴
	efd::Point3 m_prop3_RotationAxis;
	//角速度,这个是一弧度/秒 为单位的
	efd::Float32 m_prop4_AngularSpeed;
	//目标速度
	efd::Point3 m_TargetVelocity;
	//目标角度
	efd::Float32 m_TargetAngular;
	//向目标速度过渡的剩余时间
	efd::Float32 m_LeftTimeForTransitionVelocity;
	//向目标角度过渡的剩余时间
	efd::Float32 m_LeftTimeForTransitionAngular;

};
typedef efd::SmartPointer<MovableObject> MovableObjectPtr;
