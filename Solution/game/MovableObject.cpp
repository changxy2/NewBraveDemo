#include "game_pch.h"
#include "MovableObject.h"
#include "MovementService.h"
#include "egf/Entity.h"
#include "efd/EEMath.h"



EE_IMPLEMENT_CONCRETE_CLASS_INFO(MovableObject);

EE_IMPLEMENT_BUILTINMODEL_PROPERTIES(MovableObject);

MovableObject::MovableObject(void)
:m_prop4_AngularSpeed(0.0f),
m_TargetVelocity(0.0f,0.0f,0.0f),
m_TargetAngular(0.0f),
m_LeftTimeForTransitionVelocity(0.0f),
m_LeftTimeForTransitionAngular(0.0f)
{

}

MovableObject::~MovableObject(void)
{

}


// 当包含这个model 的实体，初始化构造完成后，第一个调用就是这个
void  MovableObject::OnAdded()
{
	egf::IBuiltinModelImpl::OnAdded();



	// 取的service 管理器
	m_pServiceManager = GetServiceManager();

	// 注册到 MovementService
	MovementService* pMovementService = m_pServiceManager->GetSystemServiceAs<MovementService>();
	pMovementService->RegisterMovableObject(m_pOwningEntity->GetEntityID(), this);
}

//当包含 这个model 的实体，被删除的时候调用这个函数
void MovableObject::OnRemoved()
{
	//删除在MovementService 注册的移动对象
	EE_ASSERT(m_pOwningEntity);
	MovementService* pMovementService = m_pServiceManager->GetSystemServiceAs<MovementService>();
	pMovementService->UnregisterMovableObject(m_pOwningEntity->GetEntityID());
	egf::IBuiltinModelImpl::OnRemoved();

}

//设置某个实体的速度在规定的时间变化到某个值
void MovableObject::ChangeVelocityTo(efd::Float32 fDelayTime,efd::Point3 TargetVelocity)
{

	//计算出在规定时间变化到目标速度，所需要的加速度
	SetAccel((TargetVelocity - m_prop1_MoveVelocity)/fDelayTime);
	m_LeftTimeForTransitionVelocity = fDelayTime;
	m_TargetVelocity = TargetVelocity;
	EE_LOG(efd::kApp, 1, (
		"MovableObject::ChangeVelocityTo: 速度变化为 x= %f,y = %f, z = %f\n",
		TargetVelocity.x,TargetVelocity.y,TargetVelocity.z));
	
		





}

//设置在规定时间内旋转到某个沿着旋转轴的某个角度,这里的旋转，会找最小的那个角来旋转
void MovableObject::ChangeAngleTo(efd::Float32 fDelayTime,efd::Float32 TargetfAngular)
{
	//计算出在规定时间内角度变化到目标角度所需要的角速度

	//取的当前的方向
	efd::Point3 currentEulerAngle = efd::Point3::ZERO;
	m_pOwningEntity->GetPropertyValue(
		game::PROPRTY_ID_Rotation,
		currentEulerAngle);

	efd::Float32 CurAngular = currentEulerAngle.z;
	efd::Float32 ChangeAngular = TargetfAngular - CurAngular;

	if (ChangeAngular > 180.0f)
	{
		//说明正传大于180 就要反向旋转，反向就会小于180
		 ChangeAngular -= 360.0f;
		                
    
	}
	//如果小于-180 就要正传，正传会小于180
	if (ChangeAngular < -180.0f )
	{
		  ChangeAngular += 360;

	}

	m_TargetAngular = TargetfAngular;
	SetAngularSpeed (ChangeAngular / fDelayTime);
	//复位转动角的时间
	m_LeftTimeForTransitionAngular = fDelayTime;

	
}

//移动物体的更新函数
void MovableObject::Update(efd::TimeType timeDelta)
{


	efd::Point3 currentPosition = efd::Point3::ZERO;
	m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Position,currentPosition);
	efd::Point3 OffsetPosition = efd::Point3::ZERO;
	
	if (m_LeftTimeForTransitionVelocity != 0.0f)
	{

		float fLastLeftTime = m_LeftTimeForTransitionVelocity;
		m_LeftTimeForTransitionVelocity -= timeDelta;

		
		//执行过渡速度操作
		if(m_LeftTimeForTransitionVelocity > 0.0f)
		{
			OffsetPosition =   m_prop1_MoveVelocity  * timeDelta +0.5f * m_prop2_Accel * timeDelta * timeDelta;
			SetVelocity(m_prop1_MoveVelocity  +  m_prop2_Accel  * timeDelta);


		}
		else
		{
			efd::Float32 AccelTime = fLastLeftTime;
			//加速期间的位移
			OffsetPosition =   m_prop1_MoveVelocity  * AccelTime +0.5f * m_prop2_Accel * AccelTime * AccelTime;
			//再加上匀速期间的位移
			OffsetPosition += m_TargetVelocity * (timeDelta - AccelTime);

			//如果时间用完了，说明过渡时间已经用完了，直接把目标速度设置上去
			m_prop1_MoveVelocity = m_TargetVelocity;
			m_LeftTimeForTransitionVelocity = 0.0f;
			//加速度复位
			SetAccel(efd::Point3::ZERO);

		}
		
	}
	else
	{
		OffsetPosition = m_prop1_MoveVelocity * timeDelta;



	}

	m_pOwningEntity->SetPropertyValue(game::PROPRTY_ID_Position,currentPosition + OffsetPosition);


	if (m_LeftTimeForTransitionAngular > 0.0f)
	{

		m_LeftTimeForTransitionAngular -= timeDelta;
		//执行过渡角度操作
		

		efd::Point3 currentEulerAngle = efd::Point3::ZERO;
		m_pOwningEntity->GetPropertyValue
				(
				game::PROPRTY_ID_Rotation,
				currentEulerAngle
				);

		if (m_LeftTimeForTransitionAngular <= 0.0f)
		{
			m_LeftTimeForTransitionAngular = 0.0f;
			SetAngularSpeed(0.0f);
			currentEulerAngle.z = m_TargetAngular;


		}
		else
		{
			currentEulerAngle.z += m_prop4_AngularSpeed * timeDelta;


		}
		m_pOwningEntity->SetPropertyValue(game::PROPRTY_ID_Rotation,currentEulerAngle);
	}

	// 	efd::Point3 currentEulerAngle = efd::Point3::ZERO;
	// 	 		m_pOwningEntity->GetPropertyValue(
	// 	 			game::PROPRTY_ID_Rotation,
	// 	 			currentEulerAngle);
	// 
	// 	//先取得当前的角度
	// 	currentEulerAngle.z += m_prop4_AngularSpeed * timeDelta * efd::EE_RADIANS_TO_DEGREES;
	// 	m_pOwningEntity->SetPropertyValue(game::PROPRTY_ID_Rotation,currentEulerAngle);
	// 
	// 	if (m_LeftTimeForTransitionAngular)
	// 	{
	// 		currentEulerAngle.z += m_prop4_AngularSpeed * timeDelta;
	// 
	// 	}

	// 
	// 
	// 	
	// 	//计算,并设置新的速度
	// 	SetVelocity(m_prop1_MoveVelocity + m_prop2_Accel * timeDelta);
	// 
	// 
	// 	efd::Point3 currentPosition = efd::Point3::ZERO;
	// 	m_pOwningEntity->GetPropertyValue(
	// 		game::PROPRTY_ID_Position,
	// 		currentPosition);
	// 
	// 	currentPosition += OffsetPosition;
	// 	m_pOwningEntity->SetPropertyValue(game::PROPRTY_ID_Position,currentPosition);
	// 
	// 
	// 	if(m_prop4_AngularSpeed != 0.0f && m_prop3_RotationAxis.Length() != 0)
	// 	{
	// 		//取的当前的方向
	// 		efd::Point3 currentEulerAngle = efd::Point3::ZERO;
	// 		m_pOwningEntity->GetPropertyValue(
	// 			game::PROPRTY_ID_Rotation,
	// 			currentEulerAngle);
	// 
	// 
	// 		//efd::Matrix3 matRot;
	// 		//matRot.FromEulerAnglesXYZ(currentEulerAngle.x,currentEulerAngle.y,currentEulerAngle.z);
	// 
	// 		//EE_ASSERT(m_prop3_RotationAxis.Length() == 1);
	// 
	// 
	// 		//efd::Matrix3 matRotChange; 
	// 		//计算出，旋转的角度
	// 		//matRotChange.MakeRotation(m_prop4_AngularSpeed * timeDelta,m_prop3_RotationAxis);
	// 
	// 		//matRot = matRotChange * matRot  ;
	// 
	// 
	// 		//efd::Point3 EulerAngle;
	// 		//matRot.ToEulerAnglesXYZ(EulerAngle.x,EulerAngle.y,EulerAngle.z);
	// 
	// 
	// 		// 这里现在按照任意轴旋转有问题，所以暂时不采用，任意轴旋转，将来有时间解决
	// 		
	// 		currentEulerAngle.z += m_prop4_AngularSpeed * timeDelta * efd::EE_RADIANS_TO_DEGREES;
	// 		m_pOwningEntity->SetPropertyValue(game::PROPRTY_ID_Rotation,currentEulerAngle);


}


