// ======================================================================================
// File         : AIService.cpp
// Author       : Wu Jie 
// Last Change  : 10/19/2010 | 13:26:25 PM | Tuesday,October
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "game_pch.h"

#include "AIService.h"
#include "AIModel.h"
#include <egf/FlatModel.h>
#include <egf/FlatModelManager.h>
#include <efd/MessageService.h>




///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EE_IMPLEMENT_CONCRETE_CLASS_INFO(AIService);

//------------------------------------------------------------------------------------------------
EE_HANDLER_WRAP(
    AIService,
    OnOwnedEntityAdded,
    egf::EntityChangeMessage,
    efd::kMSGID_OwnedEntityAdded);

//------------------------------------------------------------------------------------------------
EE_HANDLER_WRAP(
    AIService,
    OnOwnedEntityRemoved,
    egf::EntityChangeMessage,
    efd::kMSGID_OwnedEntityRemoved);

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

egf::IBuiltinModel* AIService::AIModelFactory () {
    AIModel* aiModel = EE_NEW AIModel();
    return aiModel;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

AIService::AIService()
    : m_LastUpdateTime (0.0)
{
    // The m_defaultPriority is inherited from efd::ISystemService and should be set to the default
    // service priority during the constructor.
    m_defaultPriority = 800;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

efd::SyncResult AIService::OnPreInit ( efd::IDependencyRegistrar* pDependencyRegistrar )
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

efd::AsyncResult AIService::OnInit()
{
    efd::MessageService* pMsgSvc = m_pServiceManager->GetSystemServiceAs<efd::MessageService>();
    pMsgSvc->Subscribe(this, efd::kCAT_LocalMessage);

    // register AIModel here.
    egf::FlatModelManager* pFlatModelManager = m_pServiceManager->GetSystemServiceAs<egf::FlatModelManager>();
    EE_ASSERT(pFlatModelManager);
    pFlatModelManager->RegisterBuiltinModelFactory
        ( "AIService",
          3280155636ul, // you should create AIModel first, and get the id from AIModel.flat
          AIService::AIModelFactory );

    // Indicate we are done with initialization. 
    return efd::AsyncResult_Complete;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

efd::AsyncResult AIService::OnTick()
{
    efd::Float64 currentTime = m_pServiceManager->GetTime(efd::kCLASSID_GameTimeClock);
    efd::Float64 deltaTime = currentTime - m_LastUpdateTime;
    m_LastUpdateTime = currentTime;

    // do the main work of the service.
    for ( ai_model_t::iterator it = m_aiModels.begin(); it != m_aiModels.end(); ++it ) {
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

efd::AsyncResult AIService::OnShutdown()
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

const char* AIService::GetDisplayName() const 
{ 
    return "AIService";
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void AIService::OnOwnedEntityAdded(const egf::EntityChangeMessage* _pMsg, efd::Category)
{
    egf::Entity* pEntity = _pMsg->GetEntity();
    if (!pEntity) return;

    AIModel* pAIModel = EE_DYNAMIC_CAST( AIModel,
                                         pEntity->FindBuiltinModelByClass(AIModel::CLASS_ID) );
    if ( pAIModel == NULL ) {
        return;
    }
    m_aiModels[pEntity->GetEntityID()] = pAIModel;
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void AIService::OnOwnedEntityRemoved(const egf::EntityChangeMessage* _pMsg, efd::Category)
{
    egf::Entity* pEntity = _pMsg->GetEntity();
    if (!pEntity) return;

    AIModel* pAIModel = EE_DYNAMIC_CAST( AIModel,
                                         pEntity->FindBuiltinModelByClass(AIModel::CLASS_ID) );
    if ( pAIModel == NULL ) {
        return;
    }
    m_aiModels.erase(pEntity->GetEntityID());
}
