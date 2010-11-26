-- ======================================================================================
-- File         : EseVandal.lua
-- Author       : Wu Jie 
-- Last Change  : 10/25/2010 | 17:58:11 PM | Monday,October
-- Description  : 
-- ======================================================================================

require("global")
require("ex.math")
require("fsm")
require("action")
local oo = require ("loop.simple") 

--/////////////////////////////////////////////////////////////////////////////
-- modules
--/////////////////////////////////////////////////////////////////////////////

module("EseVandal", package.seeall)

--/////////////////////////////////////////////////////////////////////////////
-- transitions
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

trans_IdleToWalk = oo.class( {
    entity = nil,
},fsm.Transition)

function trans_IdleToWalk:check()
    if ( self.entity.MoveVelocity:Length() > 0.0 ) then
        return true
    end
    return false
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

trans_WalkToIdle = oo.class( {
    entity = nil,
},fsm.Transition)

function trans_WalkToIdle:check()
    if ( self.entity.MoveVelocity:Length() == 0.0 ) then
        return true
    end
    return false
end

--/////////////////////////////////////////////////////////////////////////////
-- functions
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnEntitySetFinished(self, params)
    self:SendEvent("AITick")
    self:SendEvent("OnTick")
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function AITick(self, params)
    -- define states
    state_idle = fsm.State {
        entry_action = action.PlayAnim{name="idle",entity=self}
    }
    state_walk = fsm.State {
        entry_action = action.PlayAnim{name="walk",entity=self}
    }

    -- now we connect each state
    state_idle.transition_list = {
        trans_IdleToWalk { 
            dest_state = state_walk, 
            entity = self,
        },
    }
    state_walk.transition_list = {
        trans_WalkToIdle { 
            dest_state = state_idle, 
            entity = self,
        },
    }

    sm = fsm.FSM()
    sm:init(state_idle)

    -- state machine tick
    while 1 do
        sm:tick()
        -- BehaviorAPI.Sleep(500) -- we tick the AI per 0.5 second  
        BehaviorAPI.Sleep( global.dt * 1000 )
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnTick(self, params)
    while 1 do
        local cam = CameraAPI.CurrentCamera()
        local m = Matrix3.Matrix3()
        m:FromEulerAnglesXYZ(cam.Rotation.x * ex.math.deg_to_rad, cam.Rotation.y * ex.math.deg_to_rad,cam.Rotation.z * ex.math.deg_to_rad)

        local camForward = Point3.Point3()
        m:GetCol(0, camForward)
        camForward.z = 0
        camForward:Unitize()
        local camRight = Point3.Point3()
        m:GetCol(2, camRight)
        camRight = camRight * -1.0
        camRight.z = 0
        camRight:Unitize()

        self.MoveDir = camForward * self.MoveFB + camRight * self.MoveLR 
        self.MoveDir:Unitize()
        self.MoveVelocity = self.MoveDir * 30.0 

        BehaviorAPI.Sleep( global.dt * 1000 )
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnMoveForward(self, params)
    self.MoveFB = 1
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnMoveBackward(self, params)
    self.MoveFB = -1
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnMoveLeft(self, params)
    self.MoveLR = -1
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnMoveRight(self, params)
    self.MoveLR = 1
end
