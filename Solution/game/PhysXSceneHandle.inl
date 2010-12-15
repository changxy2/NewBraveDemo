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

//------------------------------------------------------------------------------------------------
inline NiPhysXScene* PhysXSceneHandle::GetNiPhysXScene()
{
    return m_spNiScene;
}

//------------------------------------------------------------------------------------------------
inline void PhysXSceneHandle::SetNiPhysXScene(NiPhysXScene* pPhysXScene)
{
    // Clean up actors we have for our scene if we are changing it
    if (m_spNiScene != NULL && m_spNiScene != pPhysXScene)
        ProcessPendingActors();

    m_spNiScene = pPhysXScene;

}

//------------------------------------------------------------------------------------------------
NxActor* PhysXSceneHandle::CreateCube(
    const NiPoint3& position,
    float size,
    float density,
    float angularDamping)
{
    return CreateBox(position, NiPoint3(size, size, size), density, angularDamping);
}

//------------------------------------------------------------------------------------------------
inline void PhysXSceneHandle::DestroyActor(NxActor* pActor)
{
    m_pendingDestroyList.push_back(pActor);
}

//------------------------------------------------------------------------------------------------
