// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not
// be copied or disclosed except in accordance with the terms of that
// agreement.
//
//      Copyright (c) 1996-2009 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Calabasas, CA 91302
// http://www.emergent.net

#include "game_pch.h"
#include "PhysXSceneHandle.h"
#include "MathHelpers.h"
#include "CollitionGroupDeclare.h"

//------------------------------------------------------------------------------------------------
PhysXSceneHandle::PhysXSceneHandle(NiPhysXScene* pNiPhysXScene) :
    m_spNiScene(pNiPhysXScene)
{

}

//------------------------------------------------------------------------------------------------
PhysXSceneHandle::~PhysXSceneHandle()
{
    if (m_pendingDestroyList.size() > 0)
        ProcessPendingActors();
}

//------------------------------------------------------------------------------------------------
NxActor* PhysXSceneHandle::CreateBox(
    const NiPoint3& position,
    const NiPoint3& dimensions,
    float density,
    float angularDamping)
{
    if (!m_spNiScene)
        return NULL;

    NxScene* pScene = m_spNiScene->GetPhysXScene();
    EE_ASSERT(pScene);

    // Create body
    NxBodyDesc bodyDesc;
    bodyDesc.angularDamping = angularDamping;

    NxBoxShapeDesc boxDesc;
    boxDesc.dimensions = MathHelpers::ToNxVec3(dimensions)
        * (1.0f / m_spNiScene->GetScaleFactor());
    boxDesc.shapeFlags |= NX_SF_VISUALIZATION;

    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&boxDesc);
    actorDesc.body = &bodyDesc;
    actorDesc.density = density;
    actorDesc.globalPose.t = MathHelpers::ToNxVec3(position)
        * (1.0f / m_spNiScene->GetScaleFactor());

    NxActor* pActor = pScene->createActor(actorDesc);
    EE_ASSERT(pActor);
    return pActor;
}

//------------------------------------------------------------------------------------------------
NxActor* PhysXSceneHandle::CreateSphere(
    const NiPoint3& position,
    float radius,
    float density,
    float angularDamping)
{
    if (!m_spNiScene)
        return NULL;

    NxScene* pScene = m_spNiScene->GetPhysXScene();
    EE_ASSERT(pScene);

    // Create body
    NxBodyDesc bodyDesc;
    bodyDesc.angularDamping = angularDamping;

    NxSphereShapeDesc sphereDesc;
    sphereDesc.radius = radius;
    sphereDesc.shapeFlags |= NX_SF_VISUALIZATION;

    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&sphereDesc);

    actorDesc.body = &bodyDesc;
    actorDesc.density = density;
    actorDesc.globalPose.t = MathHelpers::ToNxVec3(position)
        * (1.0f / m_spNiScene->GetScaleFactor());

    NxActor* pActor = pScene->createActor(actorDesc);

    return pActor;
}

//------------------------------------------------------------------------------------------------
NxActor* PhysXSceneHandle::CreateCapsule(
    const NiPoint3& position,
    float radius,
    float height,
    float density,
    float angularDamping)
{
    if (!m_spNiScene)
        return NULL;

    NxScene* pScene = m_spNiScene->GetPhysXScene();
    EE_ASSERT(pScene);

    NxBodyDesc capsBody;
    capsBody.angularDamping = angularDamping;

    NxCapsuleShapeDesc capsShape;
    capsShape.radius = radius;
    capsShape.height = height;

    // Add a dynamic capsule.
    NxActorDesc capsActor;
    capsActor.shapes.pushBack(&capsShape);
    capsActor.body = &capsBody;
    capsActor.density = density;
    capsActor.globalPose.t = MathHelpers::ToNxVec3(position)
        * (1.0f / m_spNiScene->GetScaleFactor());

    // capsule's default to Y up in PhysX
    if (MathHelpers::IsZUp())
    {
        capsActor.globalPose.M.rotX(NxHalfPiF32);
    }

    NxActor* pActor = pScene->createActor(capsActor);
    return pActor;
}

//------------------------------------------------------------------------------------------------
NxActor* PhysXSceneHandle::CreateStaticBox(const NiPoint3& position, const NiPoint3& dimensions)
{
    if (!m_spNiScene)
        return NULL;

    NxScene* pScene = m_spNiScene->GetPhysXScene();
    EE_ASSERT(pScene);

    NxBoxShapeDesc boxDesc;
    boxDesc.dimensions = MathHelpers::ToNxVec3(dimensions)
        * (1.0f / m_spNiScene->GetScaleFactor());
    boxDesc.shapeFlags |= NX_SF_VISUALIZATION;

    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&boxDesc);
    actorDesc.body = NULL;
    actorDesc.globalPose.t = MathHelpers::ToNxVec3(position)
        * (1.0f / m_spNiScene->GetScaleFactor());

    NxActor* pActor = pScene->createActor(actorDesc);
    EE_ASSERT(pActor);
    if (pActor)
        pActor->setGroup(COLLISION_GROUP_STATIC);

    return pActor;
}

//------------------------------------------------------------------------------------------------
NxActor* PhysXSceneHandle::CreateStaticSphere(const NiPoint3& position, float radius)
{
    if (!m_spNiScene)
        return NULL;

    NxScene* pScene = m_spNiScene->GetPhysXScene();
    EE_ASSERT(pScene);

    NxSphereShapeDesc sphereDesc;
    sphereDesc.radius = radius;
    sphereDesc.shapeFlags |= NX_SF_VISUALIZATION;

    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&sphereDesc);

    actorDesc.body = NULL;
    actorDesc.globalPose.t = MathHelpers::ToNxVec3(position)
        * (1.0f / m_spNiScene->GetScaleFactor());

    NxActor* pActor = pScene->createActor(actorDesc);
    if (pActor)
    {
        pActor->setGroup(COLLISION_GROUP_STATIC);
    }

    return pActor;
}

//------------------------------------------------------------------------------------------------
NxActor* PhysXSceneHandle::CreateStaticCapsule(const NiPoint3& position, float radius, float height)
{
    if (!m_spNiScene)
        return NULL;

    NxScene* pScene = m_spNiScene->GetPhysXScene();
    EE_ASSERT(pScene);

    NxCapsuleShapeDesc capsShape;
    capsShape.radius = radius;
    capsShape.height = height;

    // Add a dynamic capsule.
    NxActorDesc capsActor;
    capsActor.shapes.pushBack(&capsShape);
    capsActor.body = NULL;
    //    capsActor.density = density;
    capsActor.globalPose.t = MathHelpers::ToNxVec3(position)
        * (1.0f / m_spNiScene->GetScaleFactor());

    // capsule's default to Y up in PhysX
    if (MathHelpers::IsZUp())
    {
        capsActor.globalPose.M.rotX(NxHalfPiF32);
    }

    NxActor* pActor = pScene->createActor(capsActor);
    if (pActor)
        pActor->setGroup(COLLISION_GROUP_STATIC);
    return pActor;
}

//------------------------------------------------------------------------------------------------
void PhysXSceneHandle::ProcessPendingActors()
{
    if (m_spNiScene)
    {
        for (efd::list<NxActor*>::iterator iter = m_pendingDestroyList.begin();
            iter != m_pendingDestroyList.end();
            iter++)
        {
            NxActor* pActor = *iter;
            m_spNiScene->GetPhysXScene()->releaseActor(*pActor);
        }
    }
    m_pendingDestroyList.clear();

}

//------------------------------------------------------------------------------------------------
