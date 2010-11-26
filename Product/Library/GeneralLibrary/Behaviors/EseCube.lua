-- ======================================================================================
-- File         : EseCube.lua
-- Author       : Wu Jie 
-- Last Change  : 11/15/2010 | 17:14:52 PM | Monday,November
-- Description  : 
-- ======================================================================================

require("global")
require("ex.misc")
require("ex.ease")

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

module("EseCube", package.seeall)

--/////////////////////////////////////////////////////////////////////////////
-- functions
--/////////////////////////////////////////////////////////////////////////////

BehaviorAPI.LogMessage(2, "EseCube:Loading")

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
    local my_curve = function (_t)
        -- return ex.ease.bounce_out (_t, 1)
        -- return ex.ease.back_inout (_t, 5)
        return ex.ease.elastic_outin (_t, 0.1, 0.05)
        -- return ex.ease.smooth (_t)
    end

    local duration = 1.0
    local tick = ex.misc.make_curve ( 0.0, 40.0, duration, my_curve )
    -- local tick = ex.misc.make_curve ( 0.0, 360.0, duration, my_curve )

    while 1 do
        local val, finished = tick()
        self.Position = Point3.Point3( 0.0, val, 0.0 )
        -- self.Rotation = Point3.Point3( 0.0, 0.0, val )

        if finished then break end
        BehaviorAPI.Sleep(global.dt)
    end
end
