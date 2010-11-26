// ======================================================================================
// File         : InGameCameraService.cpp
// Author       : Wu Jie 
// Last Change  : 11/09/2010 | 13:42:11 PM | Tuesday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "game_pch.h"

#include "InGameCameraService.h"
#include "Camera_base.h"
#include "Camera_smoothFollow.h"
#include <egf/FlatModel.h>
#include <egf/FlatModelManager.h>
#include <efd/MessageService.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EE_IMPLEMENT_CONCRETE_CLASS_INFO(InGameCameraService);

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

egf::IBuiltinModel* InGameCameraService::Camera_base_factory () {
    return EE_NEW Camera_base();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

egf::IBuiltinModel* InGameCameraService::Camera_smoothFollow_factory () {
    return EE_NEW Camera_smoothFollow();
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

InGameCameraService::InGameCameraService()
    : m_LastUpdateTime (0.0)
{
    // The m_defaultPriority is inherited from efd::ISystemService and 
    // should be set to the default service priority during the constructor.
    m_defaultPriority = 800;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

efd::SyncResult InGameCameraService::OnPreInit ( efd::IDependencyRegistrar* pDependencyRegistrar )
{
    // Depend on the message service so it will initialize before our service and remain avilable
    // until after we complete OnShutdown.
    pDependencyRegistrar->AddDependency<efd::MessageService>();
    
    // Register log service message factories and subscribe to messages.
    // TODO: Register log service message factories and subscribe to messages.
    
    // return Success to indicate we are done with PreInit.
    return efd::SyncResult_Success;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

efd::AsyncResult InGameCameraService::OnInit()
{
    efd::MessageService* pMsgSvc = m_pServiceManager->GetSystemServiceAs<efd::MessageService>();
    pMsgSvc->Subscribe(this, efd::kCAT_LocalMessage);

    // register AIModel here.
    egf::FlatModelManager* pFlatModelManager = m_pServiceManager->GetSystemServiceAs<egf::FlatModelManager>();
    EE_ASSERT(pFlatModelManager);

    // register Camera_base model
    pFlatModelManager->RegisterBuiltinModelFactory ( "InGameCameraService",
                                                     2879517057ul,
                                                     InGameCameraService::Camera_base_factory );
    // register Camera_smoothFollow model
    pFlatModelManager->RegisterBuiltinModelFactory ( "InGameCameraService",
                                                     3513096190ul,
                                                     InGameCameraService::Camera_smoothFollow_factory );

    // Indicate we are done with initialization. 
    return efd::AsyncResult_Complete;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

efd::AsyncResult InGameCameraService::OnTick()
{
    efd::Float64 currentTime = m_pServiceManager->GetTime(efd::kCLASSID_GameTimeClock);
    efd::Float64 deltaTime = currentTime - m_LastUpdateTime;
    m_LastUpdateTime = currentTime;

    // do the main work of the service.
    for ( camera_map_t::iterator it = m_idToCam.begin(); it != m_idToCam.end(); ++it ) {
        it->second->OnTick(deltaTime);
    }
    
    // Indicate we have more work to do by returning efd::ASyncResult_Pending
    // otherwise return efd::ASyncResult_Complete. Once complete the 
    // service manager will no longer tick this service.The service manager 
    // will eventually call OnShutdown to give the service a chance to perform 
    // any additional cleanup before destruction.
    return efd::AsyncResult_Pending;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

efd::AsyncResult InGameCameraService::OnShutdown()
{
    efd::MessageService* pMsgSvc = m_pServiceManager->GetSystemServiceAs<efd::MessageService>();
    if (pMsgSvc) {
        pMsgSvc->Unsubscribe(this, efd::kCAT_LocalMessage);
    }

    // perform any necessary cleanup before the system service is deleted.
    //

    // Note that OnShutdown is an asynchronous method. The service manager
    // will continue to call OnShutdown until either AsyncResult_Complete
    // or AsyncResult_Failure is returned.
    return efd::AsyncResult_Complete;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void InGameCameraService::addCamera ( const egf::EntityID& _entityID, Camera_base* _pCam )
{
    m_idToCam[_entityID] = _pCam;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void InGameCameraService::removeCamera ( const egf::EntityID& _entityID, Camera_base* _pCam )
{
    m_idToCam.erase(_entityID);
}
