/********************************************************************
	创建时间:	25:11:2010   9:41
	作者:		常齐
	作用:	    移动的 API	,主要为移动的
*********************************************************************/
#pragma once
#include "egf/EntityID.h"

namespace PhysxCharacterMovementAPI
{

	//设置某个实体的速度在规定的时间变化到某个值
    efd::Bool ChangeHorizontalVelocityTo(egf::EntityID entityID,efd::Float32 fDelayTime,efd::Point2 TargetVelocity);
	//设置在规定时间内旋转到某个沿着旋转轴的某个角度
	efd::Bool ChangeAngleTo(egf::EntityID entityID,efd::Float32 fDelayTime,efd::Float32 fAngular);


}
