// ======================================================================================
// File         : Camera_base.cpp
// Author       : Wu Jie 
// Last Change  : 11/09/2010 | 11:44:02 AM | Tuesday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "game_pch.h"

#include "Camera_base.h"
#include "InGameCameraService.h"
#include <egf/EntityLoaderService.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EE_IMPLEMENT_CONCRETE_CLASS_INFO(Camera_base);
EE_IMPLEMENT_BUILTINMODEL_PROPERTIES(Camera_base);

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

Camera_base::Camera_base () 
{
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

bool Camera_base::init ()
{
    ecr::RenderService* pRenderService = m_pOwningEntity->GetServiceManager()->GetSystemServiceAs<ecr::RenderService>();
    EE_ASSERT(pRenderService);

    ecr::RenderSurface* pSurface = pRenderService->GetActiveRenderSurface();
    if (pSurface) {
        ecr::CameraService* pCameraService = m_pOwningEntity->GetServiceManager()->GetSystemServiceAs<ecr::CameraService>();
        EE_ASSERT(pCameraService);

        pCameraService->SetActiveCamera(m_pOwningEntity->GetEntityID(), pSurface->GetWindowRef());
        m_pCameraData = pCameraService->GetActiveCamera(pSurface);
        return (m_pCameraData != 0);
    } else {
        EE_LOG( efd::kApp, 
                1, 
                ("MmCamera::InitializeCamera: RenderService returned no active render surface\n") );
        return false;
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void Camera_base::OnAdded()
{
    egf::IBuiltinModelImpl::OnAdded();
    this->init();

    InGameCameraService* pInGameCamSrv = m_pOwningEntity->GetServiceManager()->GetSystemServiceAs<InGameCameraService>();
    if ( pInGameCamSrv ) {
        pInGameCamSrv->addCamera ( m_pOwningEntity->GetEntityID(), this );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void Camera_base::OnRemoved()
{
    InGameCameraService* pInGameCamSrv = m_pOwningEntity->GetServiceManager()->GetSystemServiceAs<InGameCameraService>();
    if ( pInGameCamSrv ) {
        pInGameCamSrv->removeCamera ( m_pOwningEntity->GetEntityID(), this );
    }
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void Camera_base::OnTick ( efd::Float64 _deltaTime )
{
    // do nothing
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

efd::Matrix3 Camera_base::LookAt( const efd::Point3& _focusPos, 
                                  const efd::Point3& _cameraPos, 
                                  const efd::Point3& _upAxis )
{
    efd::Point3 lookVec = _focusPos - _cameraPos;
    lookVec.Unitize();
    efd::Point3 lookTangent = lookVec.Cross(_upAxis);
    lookTangent.Unitize();
    efd::Point3 lookBiTangent = lookTangent.Cross(lookVec);

    return NiMatrix3(lookVec, lookBiTangent, lookTangent);
}
