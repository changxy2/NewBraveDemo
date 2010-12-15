#include "game_pch.h"
#include "PhysxCharacterMoveObject.h"
#include <NxBoxController.h>
#include "PhysxCharacterMovementService.h"
#include "egf/Entity.h"
#include "MathHelpers.h"
#include "CharacterControllerHitReport.h"

EE_IMPLEMENT_CONCRETE_CLASS_INFO(PhysxCharacterMoveObject);

EE_IMPLEMENT_BUILTINMODEL_PROPERTIES(PhysxCharacterMoveObject);
PhysxCharacterMoveObject::PhysxCharacterMoveObject()
:m_prop4_AngularSpeed(0.0f),
m_TargetVelocityOfHorizontal(0.0f,0.0f),
m_TargetAngular(0.0f),
m_LeftTimeForTransitionofHorizontalVelocity(0.0f),
m_LeftTimeForTransitionOfAngular(0.0f),
m_AccelOfGravity(-9.8f),
m_VelOfVertical(0.0f)

{
	
}

PhysxCharacterMoveObject::~PhysxCharacterMoveObject(void)
{
}

//移动角色dispVector ， 是想要移动的偏移量,返回的移动的时候碰撞的类型
NxU32 PhysxCharacterMoveObject::MoveCharacter(const NxVec3& dispVector, efd::UInt32 collisionGroups)
{
	NxU32 collisionFlags = 0;


	EE_ASSERT(m_pPhysxController);

	//m_pPhysxController->setInteraction(NXIF_INTERACTION_USE_FILTER);
	m_pPhysxController->move(dispVector, collisionGroups, 0.001f, collisionFlags);

	return collisionFlags;

}
//设置角色的位置。这个设置的是绝对的位置
efd::UInt32 PhysxCharacterMoveObject::SetCharacterPos(const NxVec3& pos)
{
	return m_pPhysxController->setPosition(NxExtendedVec3(pos.x, pos.y, pos.z));
}
// 当包含这个model 的实体，初始化构造完成后，第一个调用就是这个
void  PhysxCharacterMoveObject::OnAdded()
{
	egf::IBuiltinModelImpl::OnAdded();

	// 取的service 管理器
	m_pServiceManager = GetServiceManager();

	
	//这里设定创建的Controller 的参数
	NxBoxControllerDesc BoxControllerDesc;
	BoxControllerDesc.extents = MathHelpers::ToNxVec3(m_Dimension);
	BoxControllerDesc.stepOffset = 10;
	BoxControllerDesc.upDirection = NX_Z;
	BoxControllerDesc.skinWidth = 0.1f;
	BoxControllerDesc.slopeLimit = cosf(NxMath::degToRad(45.0f));
	efd::Point3 currentPosition = efd::Point3::ZERO;
	m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Position,currentPosition);
	BoxControllerDesc.position.x = currentPosition.x;
	BoxControllerDesc.position.y = currentPosition.y;
	BoxControllerDesc.position.z = currentPosition.z;
	BoxControllerDesc.position.z += BoxControllerDesc.stepOffset;
	
	CharacterControllerHitReport * HitReport =  new CharacterControllerHitReport();

	BoxControllerDesc.callback = HitReport;
	

	// 注册到 MovementService
	PhysxCharacterMovementService * PhysxMovementService = m_pServiceManager->GetSystemServiceAs<PhysxCharacterMovementService>();


	PhysxMovementService->RegisterMovableObject(m_pOwningEntity->GetEntityID(), this,BoxControllerDesc,&m_pPhysxController);
	HitReport->SetOwner(m_pPhysxController);

	
	
}


//当包含 这个model 的实体，被删除的时候调用这个函数
void PhysxCharacterMoveObject::OnRemoved()
{
	//删除在MovementService 注册的移动对象
	EE_ASSERT(m_pOwningEntity);
	PhysxCharacterMovementService* pMovementService = m_pServiceManager->GetSystemServiceAs<PhysxCharacterMovementService>();
	pMovementService->UnregisterMovableObject(m_pOwningEntity->GetEntityID());
	egf::IBuiltinModelImpl::OnRemoved();

}

//设置某个实体的水平速度在规定的时间变化到某个值
void PhysxCharacterMoveObject::ChangeHorizontalVelocityTo(efd::Float32 fDelayTime,efd::Point2 TargetVelocity)
{

	//计算出在规定时间变化到目标速度，所需要的加速度
	m_AccelOfHorizontal = ((TargetVelocity - m_MoveVelocityOfHorizontal)/fDelayTime);
	m_LeftTimeForTransitionofHorizontalVelocity = fDelayTime;
	m_TargetVelocityOfHorizontal = TargetVelocity;
	EE_LOG(efd::kApp, 1, (
		"MovableObject::ChangeVelocityTo: 速度变化为 x= %f,y = %f",
		TargetVelocity.x,TargetVelocity.y));


}

//设置在规定时间内旋转到某个沿着旋转轴的某个角度
void PhysxCharacterMoveObject::ChangeAngleTo(efd::Float32 fDelayTime,efd::Float32 TargetfAngular)
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
	m_LeftTimeForTransitionOfAngular = fDelayTime;
}

//取得PhysxController 的位置
const NxExtendedVec3& PhysxCharacterMoveObject::GetPhysxControllerPos()
{
	return m_pPhysxController->getPosition();



}

//更新函数，主要是对位置的更新，包括NxController
void PhysxCharacterMoveObject::Update(efd::TimeType timeDelta)
{
	float timeDeltaSquare =  timeDelta * timeDelta;
	efd::Point3 currentPosition = efd::Point3::ZERO;
	m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Position,currentPosition);
	efd::Point2 OffsetPositionOfHorizontal  = efd::Point2::ZERO;

	if (m_LeftTimeForTransitionofHorizontalVelocity != 0.0f)
	{

		float fLastLeftTime = m_LeftTimeForTransitionofHorizontalVelocity;
		m_LeftTimeForTransitionofHorizontalVelocity -= timeDelta;


		//执行过渡速度操作
		if(m_LeftTimeForTransitionofHorizontalVelocity > 0.0f)
		{
			OffsetPositionOfHorizontal =   m_MoveVelocityOfHorizontal  * timeDelta +0.5f * m_AccelOfHorizontal * timeDeltaSquare;
			m_MoveVelocityOfHorizontal = m_MoveVelocityOfHorizontal  +  m_AccelOfHorizontal  * timeDelta;


		}
		else
		{
			efd::Float32 AccelTime = fLastLeftTime;
			//加速期间的位移
			OffsetPositionOfHorizontal =   m_MoveVelocityOfHorizontal  * AccelTime +0.5f * m_AccelOfHorizontal * AccelTime * AccelTime;
			//再加上匀速期间的位移
			OffsetPositionOfHorizontal +=  m_TargetVelocityOfHorizontal * (timeDelta - AccelTime);

			//如果时间用完了，说明过渡时间已经用完了，直接把目标速度设置上去
			m_MoveVelocityOfHorizontal = m_TargetVelocityOfHorizontal;
			m_LeftTimeForTransitionofHorizontalVelocity = 0.0f;
			//加速度复位
			m_AccelOfHorizontal = efd::Point2::ZERO;

		}

	}
	else
	{
		OffsetPositionOfHorizontal = m_MoveVelocityOfHorizontal * timeDelta;



	}

	//下面计算垂直方向的位移
	
	float OffsetPositionOnVertical = m_VelOfVertical * timeDelta + 0.5f * m_AccelOfGravity * timeDeltaSquare;
	m_VelOfVertical += m_AccelOfGravity * timeDelta;

	efd::Point3 OffsetPosition(OffsetPositionOfHorizontal.x,OffsetPositionOfHorizontal.y,OffsetPositionOnVertical);
	
	if(OffsetPosition.Length() > 0.0f)
	{
		//根据当前所要移动的偏移量,用m_pPhysxController 来在实体场景中移动
		NxU32 ContactInfo = MoveCharacter( MathHelpers::ToNxVec3(OffsetPosition),0xffff);
		const NxExtendedVec3& ExtendedVec3 = GetPhysxControllerPos();
		if (ContactInfo & NXCC_COLLISION_DOWN)
		{
			//这里说明碰到地面了，地面速度要变为零
			m_VelOfVertical = 0.0f;



		}
		
		
		m_pOwningEntity->SetPropertyValue(game::PROPRTY_ID_Position,MathHelpers::ToPoint3(ExtendedVec3));
	}





	if (m_LeftTimeForTransitionOfAngular > 0.0f)
	{

		m_LeftTimeForTransitionOfAngular -= timeDelta;
		//执行过渡角度操作


		efd::Point3 currentEulerAngle = efd::Point3::ZERO;
		m_pOwningEntity->GetPropertyValue
			(
			game::PROPRTY_ID_Rotation,
			currentEulerAngle
			);

		if (m_LeftTimeForTransitionOfAngular <= 0.0f)
		{
			m_LeftTimeForTransitionOfAngular = 0.0f;
			SetAngularSpeed(0.0f);
			currentEulerAngle.z = m_TargetAngular;


		}
		else
		{
			currentEulerAngle.z += m_prop4_AngularSpeed * timeDelta;


		}
		m_pOwningEntity->SetPropertyValue(game::PROPRTY_ID_Rotation,currentEulerAngle);
	}

}





