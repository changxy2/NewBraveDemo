// ======================================================================================
// File         : InGameCameraService.h
// Author       : Wu Jie 
// Last Change  : 11/09/2010 | 13:42:18 PM | Tuesday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef INGAMECAMERASERVICE_H_1289273184
#define INGAMECAMERASERVICE_H_1289273184
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include <egf/BuiltinModelHelper.h>
#include <egf/EntityChangeMessage.h>

///////////////////////////////////////////////////////////////////////////////
// class defines
///////////////////////////////////////////////////////////////////////////////

class Camera_base;

///////////////////////////////////////////////////////////////////////////////
// class InGameCameraService
// 
// Purpose: 
// 
///////////////////////////////////////////////////////////////////////////////

class InGameCameraService : public efd::ISystemService
{
    /// @cond EMERGENT_INTERNAL
    EE_DECLARE_CLASS1(InGameCameraService, game::SERVICE_ID_InGameCameraService, efd::ISystemService);
    EE_DECLARE_CONCRETE_REFCOUNT;
    /// @endcond

protected:

    static egf::IBuiltinModel* Camera_base_factory ();
    static egf::IBuiltinModel* Camera_smoothFollow_factory ();

    ///////////////////////////////////////////////////////////////////////////////
    // public
    ///////////////////////////////////////////////////////////////////////////////

public:

    InGameCameraService();

    virtual efd::SyncResult OnPreInit ( efd::IDependencyRegistrar* pDependencyRegistrar );
    virtual efd::AsyncResult OnInit();
    virtual efd::AsyncResult OnTick();
    virtual efd::AsyncResult OnShutdown();
    virtual const char* GetDisplayName() const { return "InGameCameraService"; }

    void addCamera ( const egf::EntityID& _entityID, Camera_base* _pCam );
    void removeCamera ( const egf::EntityID& _entityID, Camera_base* _pCam );

    ///////////////////////////////////////////////////////////////////////////////
    // protected
    ///////////////////////////////////////////////////////////////////////////////

protected:

    typedef efd::map<egf::EntityID, Camera_base*> camera_map_t;
    camera_map_t m_idToCam;

    efd::Float64 m_LastUpdateTime;

}; // end class InGameCameraService

typedef efd::SmartPointer<InGameCameraService> InGameCameraServicePtr;

// #################################################################################
#endif // END INGAMECAMERASERVICE_H_1289273184
// #################################################################################
