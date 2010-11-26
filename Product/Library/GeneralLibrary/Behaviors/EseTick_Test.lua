-- ======================================================================================
-- File         : EseTick_Test.lua
-- Author       : Wu Jie 
-- Last Change  : 10/18/2010 | 14:46:23 PM | Monday,October
-- Description  : 
-- ======================================================================================

require("global")
require("ex.misc")
require("ex.ease")

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

module("EseTick_Test", package.seeall)

--/////////////////////////////////////////////////////////////////////////////
-- functions
--/////////////////////////////////////////////////////////////////////////////

BehaviorAPI.LogMessage(2, "EseTick_Test:Loading")

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function RegisterTickEvent(entity)
    entity:SendEvent("OnTick_SendEvent", nil, "", global.dt)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function ResetTickTime(entity)
    local lastTickTime = entity.TickTime
    entity.TickTime = BehaviorAPI.GetGameTime()
    if ( lastTickTime == 0 ) then
        return 0
    end
    return entity.TickTime - lastTickTime
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnEntitySetFinished(self, params)
    if self.TickMethod == "SendEvent" then
        RegisterTickEvent(self)
    elseif self.TickMethod == "WhileOne" then
        self:SendEvent("OnTick_WhileOne")
    elseif self.TickMethod == "CallBack" then
        BehaviorAPI.LogMessage(2, "TODO: implement it!")
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnTick_SendEvent(self, params)
    RegisterTickEvent(self)
    BehaviorAPI.LogMessage(3, string.format("EseTick_Test::OnTick_SendEvent(%x)", self.ID))
    deltaTime = ResetTickTime(self)

    local position = self.Position
    position[3] = position[3] + 10.0 * deltaTime 
    self.Position = position
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnTick_WhileOne(self, params)

    local my_curve = function (_t)
        -- return ex.ease.bounce_out (_t, 0.1)
        return ex.ease.bounce_out (_t, 2.0)
        -- return ex.ease.smooth (_t)
    end
    --
    local pos_tick = nil
    if self.UseSysClock == true then
        pos_tick = ex.misc.make_curve ( 0.0, 360.0, 2.0, my_curve )
    else
        pos_tick = ex.misc.make_curve_gb ( 0.0, 360.0, 2.0, my_curve )
    end

    --
    while 1 do
        local val = pos_tick()

        -- local pos = self.Position
        -- self.Position = Point3.Point3( -val, pos[2], val )

        local rot = self.Rotation
        self.Rotation = Point3.Point3( rot[1], rot[2], val )

        BehaviorAPI.Sleep( global.dt * 1000 )
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

-- function OnMoveForward(self, params)
--     BehaviorAPI.LogMessage(3, string.format("EseTick_Test::OnMoveForward(%x)", self.ID))
--     local magnitude = params:GetFloat32("Magnitude")
--     local position = self.Position
--     position[1] = position[1] + 100.0 * global.dt
--     self.Position = position
-- end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

-- function OnMoveBackward(self, params)
--     BehaviorAPI.LogMessage(3, string.format("EseTick_Test::OnMoveBackword(%x)", self.ID))
--     local magnitude = params:GetFloat32("Magnitude")
--     local position = self.Position
--     position[1] = position[1] - 100.0 * global.dt
--     self.Position = position
-- end
