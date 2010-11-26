// ======================================================================================
// File         : AIService.h
// Author       : Wu Jie 
// Last Change  : 10/18/2010 | 17:25:27 PM | Monday,October
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef AISERVICE_H_1287393936
#define AISERVICE_H_1287393936
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include <egf/BuiltinModelHelper.h>
#include <egf/EntityChangeMessage.h>

///////////////////////////////////////////////////////////////////////////////
// class defines
///////////////////////////////////////////////////////////////////////////////

class AIModel;

///////////////////////////////////////////////////////////////////////////////
// class  AIService
// 
// Purpose: 
// 
///////////////////////////////////////////////////////////////////////////////

class AIService : public efd::ISystemService
{
    /// @cond EMERGENT_INTERNAL
    EE_DECLARE_CLASS1(AIService, game::SERVICE_ID_AIService, efd::ISystemService);
    EE_DECLARE_CONCRETE_REFCOUNT;
    /// @endcond

protected:

    static egf::IBuiltinModel* AIModelFactory ();

    ///////////////////////////////////////////////////////////////////////////////
    // public
    ///////////////////////////////////////////////////////////////////////////////

public:

    AIService();

    virtual efd::SyncResult OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar);
    virtual efd::AsyncResult OnInit();
    virtual efd::AsyncResult OnTick();
    virtual efd::AsyncResult OnShutdown();
    virtual const char* GetDisplayName() const;

    void addAIModel ( AIModel* _model );

    void OnOwnedEntityAdded(const egf::EntityChangeMessage* _pMsg, efd::Category);
    void OnOwnedEntityRemoved(const egf::EntityChangeMessage* _pMsg, efd::Category);

    ///////////////////////////////////////////////////////////////////////////////
    // protected
    ///////////////////////////////////////////////////////////////////////////////

protected:

    typedef efd::map<egf::EntityID, AIModel*> ai_model_t;
    ai_model_t m_aiModels;
    efd::Float64 m_LastUpdateTime;

}; // end class AIService

typedef efd::SmartPointer<AIService> AIServicePtr;

// #################################################################################
#endif // END AISERVICE_H_1287393936
// #################################################################################


