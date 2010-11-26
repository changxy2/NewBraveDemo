// ======================================================================================
// File         : AIModel.cpp
// Author       : Wu Jie 
// Last Change  : 10/19/2010 | 15:28:55 PM | Tuesday,October
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "game_pch.h"

#include "AIModel.h"
#include <egf/EntityLoaderService.h>

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

EE_IMPLEMENT_CONCRETE_CLASS_INFO(AIModel);
EE_IMPLEMENT_BUILTINMODEL_PROPERTIES(AIModel);

///////////////////////////////////////////////////////////////////////////////
// function defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

AIModel::AIModel () 
{
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

void AIModel::OnTick ( efd::Float64 _deltaTime )
{
    efd::Point3 curRotation = efd::Point3::ZERO;
    m_pOwningEntity->GetPropertyValue(game::PROPRTY_ID_Rotation, curRotation);
    curRotation.z += float(_deltaTime * 10.0);

    m_pOwningEntity->SetPropertyValue(game::PROPRTY_ID_Rotation, curRotation);
}
