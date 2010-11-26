// ======================================================================================
// File         : Camera_base.h
// Author       : Wu Jie 
// Last Change  : 11/09/2010 | 11:41:27 AM | Tuesday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef CAMERA_BASE_H_1289274090
#define CAMERA_BASE_H_1289274090
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "ID.h"
#include <egf/BuiltinModelHelper.h>
#include <egf/BehaviorDescriptor.h>
#include <efd/ParameterList.h>
#include <ecr/CameraService.h>

///////////////////////////////////////////////////////////////////////////////
// classes defines
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// class 
// 
// Purpose: 
// 
///////////////////////////////////////////////////////////////////////////////

class Camera_base : public egf::IBuiltinModelImpl
{
    EE_DECLARE_CLASS1(Camera_base, game::CLASS_ID_Camera_base, egf::IBuiltinModelImpl);
    EE_DECLARE_CONCRETE_REFCOUNT;

    EE_DECLARE_BUILTINMODEL_PROPERTIES
    EE_END_BUILTINMODEL_PROPERTIES

    ///////////////////////////////////////////////////////////////////////////////
    // public
    ///////////////////////////////////////////////////////////////////////////////

public:

    Camera_base (); 
    bool init ();
    virtual void OnTick ( efd::Float64 _deltaTime );

    efd::Matrix3 LookAt( const efd::Point3& _focusPos, 
                         const efd::Point3& _cameraPos, 
                         const efd::Point3& _upAxis );

    ///////////////////////////////////////////////////////////////////////////////
    // protected
    ///////////////////////////////////////////////////////////////////////////////

protected:

    virtual void OnAdded();
    virtual void OnRemoved();
    virtual bool Dispatch ( const egf::BehaviorDescriptor* pBehaviorToInvoke, efd::ParameterList* pParams ) { return false; }

    ///////////////////////////////////////////////////////////////////////////////
    // protected member
    ///////////////////////////////////////////////////////////////////////////////

protected:

    ecr::CameraData* m_pCameraData;

}; // end class Camera_base 


// #################################################################################
#endif // END CAMERA_BASE_H_1289274090
// #################################################################################


