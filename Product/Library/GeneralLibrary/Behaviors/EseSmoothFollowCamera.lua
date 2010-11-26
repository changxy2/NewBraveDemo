-- ======================================================================================
-- File         : EseSmoothFollowCamera.lua
-- Author       : Wu Jie 
-- Last Change  : 11/11/2010 | 15:38:22 PM | Thursday,November
-- Description  : 
-- ======================================================================================

require("global")
require("ex.misc")
require("ex.ease")

--/////////////////////////////////////////////////////////////////////////////
-- modules
--/////////////////////////////////////////////////////////////////////////////

module("EseSmoothFollowCamera", package.seeall)

--/////////////////////////////////////////////////////////////////////////////
-- functions
--/////////////////////////////////////////////////////////////////////////////

BehaviorAPI.LogMessage(2, "EseSmoothFollowCamera:Loading")

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function Zoom(self, params)
    local my_curve = function (_t)
        -- return ex.ease.bounce_out (_t, 0.1)
        return ex.ease.bounce_out (_t, 0.5)
        -- return ex.ease.smooth (_t)
    end

    local zoomFactor = params:GetFloat32()
    local duration = params:GetFloat32()

    local wanted_fov = self.FOV / zoomFactor
    local tick = ex.misc.make_curve ( self.FOV, wanted_fov, duration, my_curve )

    while 1 do
        if (self.FOV == wanted_fov) then break end
        local fov = tick()
        self.FOV = fov
        BehaviorAPI.Sleep( global.dt * 1000 )
    end
end
