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

#pragma once
#ifndef EE_PHYSXSCENEHANDLE_H
#define EE_PHYSXSCENEHANDLE_H

#include <NiPhysXScene.h>
#include <NxControllerManager.h>



#define DEFAULT_PHYSX_DENSITY 10.0f
#define DEFAULT_PHYSX_ANGULAR_DAMPING 0.5f

class PhysXSceneHandle : public efd::MemObject
{
    EE_DECLARE_CONCRETE_REFCOUNT;
public:

    /// Constructor
    /// @param pNiPhysXScene The NiPhysXScene that you wish this class to control.
    PhysXSceneHandle(NiPhysXScene* pNiPhysXScene = NULL);
    virtual ~PhysXSceneHandle();

    /**
        Get the NiPhysXScene that is used by this class

        @return The NiPhysXScene used by this class
    */
    inline NiPhysXScene* GetNiPhysXScene();

    /**
        Set the NiPhysXScene that is used by this class

        @param pPhysXScene The NiPhysXScene used by this class
    */
    inline void SetNiPhysXScene(NiPhysXScene* pPhysXScene);

    /**
        Creates a NxShape of a cube, that is a rigidbody and a corresponding NxActor to manage it.

        @param position The position in the world to create this at.
        @param size The width, height, and depth of the cube.
        @param density This property is passed directly to PhysX. It is the density of the object.
        @param angularDamping This property is passed directly to PhysX. It is the angulardamping
            of the cube.
        @return This function returns an NxActor with the shape added to it. It is a rigidbody
            object.
    */
    inline NxActor* CreateCube(
        const NiPoint3& position,
        float size,
        float density = DEFAULT_PHYSX_DENSITY,
        float angularDamping = DEFAULT_PHYSX_ANGULAR_DAMPING);

    /**
        Creates a NxShape of a box, that is a rigidbody and a corresponding NxActor to manage it.


        @param position The position in the world of the Actor this shape is part of.
        @param dimensions The width, height, and depth of the box.
        @param density This property is passed directly to PhysX. It is the density of the object.
        @param angularDamping This property is passed directly to PhysX. It is the angulardamping
            of the cube.
        @return This function returns an NxActor with the shape added to it. It is a rigidbody
            object.
    */
    NxActor* CreateBox(
        const NiPoint3& position,
        const NiPoint3& dimensions,
        float density = DEFAULT_PHYSX_DENSITY,
        float angularDamping = DEFAULT_PHYSX_ANGULAR_DAMPING);

    /**
        Creates a NxShape of a sphere, that is a rigidbody and a corresponding NxActor to manage it.

        @param position The position in the world of the Actor this shape is part of.
        @param radius The radius of the sphere.
        @param density This property is passed directly to PhysX. It is the density of the object.
        @param angularDamping This property is passed directly to PhysX. It is the angulardamping of
            the cube.
        @return This function returns an NxActor with the shape added to it. It is a rigidbody
            object.
    */
    NxActor* CreateSphere(
        const NiPoint3& position,
        float radius,
        float density = DEFAULT_PHYSX_DENSITY,
        float angularDamping = DEFAULT_PHYSX_ANGULAR_DAMPING);

    /**
        Creates a NxShape of a capsule, that is a rigidbody and a corresponding NxActor to manage
        it.

        @param position The position in the world of the Actor this shape is part of.t.
        @param radius The radius of the capsule.
        @param height The height of the capsule.
        @param density This property is passed directly to PhysX. It is the density of the object.
        @param angularDamping This property is passed directly to PhysX. It is the angulardamping of
            the cube.
        @return This function returns an NxActor with the shape added to it. It is a rigidbody
            object.
    */
    NxActor* CreateCapsule(
        const NiPoint3& position,
        float radius,
        float height,
        float density = DEFAULT_PHYSX_DENSITY,
        float angularDamping = DEFAULT_PHYSX_ANGULAR_DAMPING);

    /**
        Creates a NxShape of a box, that is static and a corresponding NxActor to manage it.

        @param position The position in the world of the Actor this shape is part of.
        @param dimensions The width, height, and depth of the box.
        @return This function returns an NxActor with the shape added to it. It is a static object.
    */
    NxActor* CreateStaticBox(const NiPoint3& position, const NiPoint3& dimensions);

    /**
        Creates a NxShape of a sphere, that is static and a corresponding NxActor to manage it.

        @param position The position in the world of the Actor this shape is part of.
        @param radius The radius of the sphere.
        @return This function returns an NxActor with the shape added to it. It is a static object.
    */
    NxActor* CreateStaticSphere(const NiPoint3& position, float radius);

    /**
        Creates a NxShape of a capsule, that is static and a corresponding NxActor to manage it.

        @param position The position in the world of the Actor this shape is part of.
        @param radius The radius of the capsule.
        @param height The height of the capsule.
        @return This function returns an NxActor with the shape added to it. It is a static object.
    */
    NxActor* CreateStaticCapsule(const NiPoint3& position, float radius, float height);

    /**
        Removes an Actor from the PhysX Scene

        @param pActor The actor to remove from the Scene.
    */
    inline void DestroyActor(NxActor* pActor);

    /// Forces the release of any NxActor's that are pending destruction.
    void ProcessPendingActors();

protected:
    NiPhysXScenePtr m_spNiScene;
    efd::list<NxActor*> m_pendingDestroyList;
};

/// A SmartPointer for the PhysXSceneHandle class
typedef efd::SmartPointer<PhysXSceneHandle> PhysXSceneHandlePtr;


#include "PhysXSceneHandle.inl"

#endif  // EE_PHYSXSCENEHANDLE_H
