// ======================================================================================
// File         : CameraAPI.cpp
// Author       : Wu Jie 
// Last Change  : 11/11/2010 | 17:02:48 PM | Thursday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "game_pch.h"
#include "CameraAPI.h"

#include <egf/ScriptContext.h>
#include <ecr/CameraService.h>
#include <egf/EntityLoaderService.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ######################### 
namespace CameraAPI { 
// ######################### 

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

egf::EntityID CurrentCamera () {
    // TODO: use InGameCameraService instead { 
    ecr::RenderService* pRenderService = egf::g_bapiContext.GetSystemServiceAs<ecr::RenderService>();
    EE_ASSERT(pRenderService);

    ecr::RenderSurface* pSurface = pRenderService->GetActiveRenderSurface();
    if (pSurface) {
        ecr::CameraService* pCameraService = egf::g_bapiContext.GetSystemServiceAs<ecr::CameraService>();
        EE_ASSERT(pCameraService);

        ecr::CameraData* pCameraData = pCameraService->GetActiveCamera(pSurface);
        return pCameraData->GetId();
    }
    // } TODO end 
    return egf::EntityID::kENTITY_INVALID;
}

// ######################### 
} // end namespace CameraAPI 
// ######################### 


