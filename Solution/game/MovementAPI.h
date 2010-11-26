/********************************************************************
	创建时间:	25:11:2010   9:41
	作者:		常齐
	作用:	    移动的 API	,主要为移动的
*********************************************************************/
#pragma once
#include "egf/EntityID.h"

namespace MovementAPI
{

	//设置某个实体的速度在规定的时间变化到某个值
    efd::Bool ChangeVelocityTo(egf::EntityID entityID,efd::Float32 fDelayTime,efd::Point3 TargetVelocity);
	//设置在规定时间内旋转到某个沿着旋转轴的某个角度
	efd::Bool ChangeAngleTo(egf::EntityID entityID,efd::Float32 fDelayTime,efd::Float32 fAngular);


}
