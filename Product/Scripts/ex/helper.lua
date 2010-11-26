-- ======================================================================================
-- File         : helper.lua
-- Author       : Wu Jie 
-- Last Change  : 11/08/2010 | 18:28:15 PM | Monday,November
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- require and module
--/////////////////////////////////////////////////////////////////////////////

local assert, type = assert, type
local Matrix3 = Matrix3

module ("ex.helper")

--/////////////////////////////////////////////////////////////////////////////
-- function defines
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function lookat ( _focus, _curPos, _up )
    -- assert( type(_focus) == "Point3", "focus is not point3" )
    -- assert( type(_curPos) == "Point3", "curPos is not point3" )
    -- assert( type(_up) == "Point3", "up is not point3" )

    local look_dir = _focus - _curPos
    look_dir:Unitize()
    local tangent = look_dir:Cross(_up)
    tangent:Unitize()
    local biTangent = tangent:Cross(look_dir)
    return Matrix3.Matrix3( look_dir, biTangent, tangent )
end

