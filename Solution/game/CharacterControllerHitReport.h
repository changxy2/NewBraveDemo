/********************************************************************
	创建时间: 12:12:2010   13:13
	作者:	  常齐
	作用:	character 控制器的碰撞回调类
*********************************************************************/
#pragma once
#include <nxcontroller.h>

class CharacterControllerHitReport :
	public NxUserControllerHitReport
{
public:
	CharacterControllerHitReport(void);
	virtual ~CharacterControllerHitReport(void);

	//当碰撞事件在主人 charcter 和一个动力学PhysX actor 之间发生的时候调用这个函数
	//hit 是碰到的Shape ，返回值是返回执行的行为，比如推或者不推
	virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit);

	//这个是在主人charcter 和其他character 碰撞的时候调用这个函数
	// hit 是碰到的character
	//返回值是返回执行的行为，比如推或者不推
	virtual NxControllerAction onControllerHit(const NxControllersHit& hit);

	/// 设置推物体的PhysX character controller
	void SetOwner(NxController* pController) { m_pOwner = pController; }
	/// 设置推力系数
	void SetPushMultiplier(efd::Float32 multiplier) { m_pushMultiplier = multiplier; }

private:
	//这个是主人控制器
	NxController* m_pOwner;
	//这个是推力系数
	float m_pushMultiplier;

};
