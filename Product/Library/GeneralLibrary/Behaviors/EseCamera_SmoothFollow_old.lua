-- ======================================================================================
-- File         : EseCamera_SmoothFollow_old.lua
-- Author       : Wu Jie 
-- Last Change  : 11/09/2010 | 13:59:15 PM | Tuesday,November
-- Description  : 
-- ======================================================================================

require("global")
require("ex.helper")

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

module("EseCamera_SmoothFollow_old", package.seeall)

--/////////////////////////////////////////////////////////////////////////////
-- functions
--/////////////////////////////////////////////////////////////////////////////

BehaviorAPI.LogMessage(2, "EseCamera_SmoothFollow_old:Loading")

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnEntitySetFinished(self, params)
    self:SendEvent("OnTick")
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnTick(self, params)
    while 1 do
        -- local rotMat = ex.helper.lookat( self.Target.Position, self.Position, Point3.Point3(0, 0, 1) )
        -- local x,y,z = 0,0,0
        -- local result = false
        -- result, x, y, z = rotMat:ToEulerAnglesXYZ(x,y,z)
        -- self.Rotation = Point3.Point3(x,y,z)

        BehaviorAPI.Sleep(global.dt)
    end
end
