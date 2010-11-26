// ======================================================================================
// File         : Camera_smoothFollow.cpp
// Author       : Wu Jie 
// Last Change  : 11/09/2010 | 14:53:12 PM | Tuesday,November
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "game_pch.h"

#include "Camera_smoothFollow.h"
#include <egf/EntityLoaderService.h>
#include <egf/ScriptContext.h>
#include <egf/Scheduler.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EE_IMPLEMENT_CONCRETE_CLASS_INFO(Camera_smoothFollow);
EE_IMPLEMENT_BUILTINMODEL_PROPERTIES(Camera_smoothFollow);

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

Camera_smoothFollow::Camera_smoothFollow () 
    : Camera_base()
{
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void Camera_smoothFollow::OnTick ( efd::Float64 _deltaTime )
{
    if ( m_pCameraData == NULL ) {
        init();
        return;
    }

    // get Scheduler service
    egf::Scheduler* pScheduler = egf::g_bapiContext.GetSystemServiceAs<egf::Scheduler>();
    EE_ASSERT(pScheduler);

    // get target entity
    egf::EntityID targetID;
    m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Target, targetID);

    // if we have target entity. focus on it.
    if ( targetID != 0L ) {
        egf::Entity* pTarget = pScheduler->FindEntity(targetID);
        if ( pTarget ) {
            efd::Point3 curPos = efd::Point3::ZERO;
            m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Position, curPos);

            efd::Point3 targetPos = efd::Point3::ZERO;
            pTarget->GetPropertyValue(game::PROPRTY_ID_Position, targetPos);

            float yaw = 0.0f;
            m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Yaw, yaw);

            float pitch = 0.0f;
            m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Pitch, pitch);

            float height = 0.0f;
            m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Height, height);

            efd::Point3 rot = m_pCameraData->GetRotate();
            efd::Matrix3 rotMat; 
            rotMat.FromEulerAnglesXYZ(rot.x, rot.y, rot.z);
            efd::Point3 cam_forward;
            rotMat.GetRow(0,cam_forward);

            float distance = 0.0f;
            m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Distance, distance);
            efd::Point3 horin_offset = cam_forward * -distance;
            m_wantedPos = targetPos + efd::Point3( horin_offset.y, horin_offset.z, height );

            float moveDamping = 0.0f;
            m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_MoveDamping, moveDamping);
            curPos.x = EE_LERP (moveDamping * efd::Float32(_deltaTime), curPos.x, m_wantedPos.x );
            curPos.y = EE_LERP (moveDamping * efd::Float32(_deltaTime), curPos.y, m_wantedPos.y );
            curPos.z = EE_LERP (moveDamping * efd::Float32(_deltaTime), curPos.z, m_wantedPos.z );

            //
            // efd::Matrix3 rot = this->LookAt( targetPos, curPos, efd::Point3::UNIT_Z );

            m_pCameraData->SetTranslate(curPos);
            m_pCameraData->SetRotate( efd::Point3( 90.0f, yaw, pitch ) );
        }
    }
}


