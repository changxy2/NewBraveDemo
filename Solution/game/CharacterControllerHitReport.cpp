#include "game_pch.h"
#include "CharacterControllerHitReport.h"
#include <NxActor.h>
#include "MathHelpers.h"


CharacterControllerHitReport::CharacterControllerHitReport(void)
:m_pOwner(NULL),
m_pushMultiplier(1.0f)
{
}

CharacterControllerHitReport::~CharacterControllerHitReport(void)
{
}

//当碰撞事件在拥有的charcter 和一个动力学PhysX actor 之间发生的时候调用这个函数
//hit 是碰到的Shape ，返回值是返回执行的行为，比如推或者不推
NxControllerAction CharacterControllerHitReport::onShapeHit(const NxControllerShapeHit& hit)
{
	if (!m_pOwner)
		return NX_ACTION_NONE;

	
	if (hit.shape)
	{
		

		bool canPush = true;

		

		NxActor* ownerActor = m_pOwner->getActor();

		if (!ownerActor)
		{
			return NX_ACTION_NONE;
		}

		NxVec3 velocity = ownerActor->getLinearVelocity();

		if (velocity.magnitudeSquared() == 0)
		{
			canPush = false;
		}

		NxActor& actor = hit.shape->getActor();
		NxCollisionGroup actorGroup = actor.getGroup();

		

		if (!actor.isDynamic())
			canPush = false;
		else if (actor.readBodyFlag(NX_BF_KINEMATIC))
			canPush = false; // 不能推  kinematic actors

		if (canPush)
		{
			float coeffMult = m_pushMultiplier;

			// We only allow horizontal pushes. Vertical pushes when we stand on
			// dynamic objects creates useless stress on the solver. It would be
			// possible to enable/disable vertical pushes on particular objects,
			// if the gameplay requires it.

			NxVec3 worldPos;
			worldPos.x = (NxReal)hit.worldPos.x;
			worldPos.y = (NxReal)hit.worldPos.y;
			worldPos.z = (NxReal)hit.worldPos.z;

			NxVec3 dir = actor.getGlobalPosition() - worldPos;

			MathHelpers::UP_COMPONENT(dir) = 0;
			dir.normalize();

			NxF32 mass = actor.getMass();

			NxF32 coeff = mass * hit.length * coeffMult;
			actor.addForceAtLocalPos(dir*coeff, NxVec3(0, 0, 0), NX_IMPULSE);
		}
	}
		return NX_ACTION_NONE;


}

//这个是在拥有的charcter 和其他character 碰撞的时候调用这个函数
// hit 是碰到的character
//返回值是返回执行的行为，比如推或者不推
NxControllerAction CharacterControllerHitReport::onControllerHit(const NxControllersHit& hit)
{
	return NX_ACTION_NONE;



}