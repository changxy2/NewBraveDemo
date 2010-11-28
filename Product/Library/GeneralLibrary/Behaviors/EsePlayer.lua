-- state machine for movement 
-- author: chang qi
-- created time : 2010 -11- 18 
require("ex.math")
require("global")
require("action")
require("fsm")
module("EsePlayer", package.seeall)

local oo = require ("loop.simple") 


g_MoveKeysIsChange =  false
g_MoveKeys = {Forward = false,Backward = false,Left = false,Right = false}
g_TransitionTime = 1
-- walk speed 
g_walkSpeed = 30

g_RotTimePercent = 0.8


-- ------------------------------------------------------------------ 
-- Desc: transition From idle to walk class
-- ------------------------------------------------------------------ 

trans_IdleToWalk = oo.class(
  {
     entity = nil
  },fsm.Transition
)

function trans_IdleToWalk:check()
   if g_MoveKeysIsChange then 
      g_MoveKeysIsChange = false
      return true
   else
      return false
   end 
end

-- ------------------------------------------------------------------ 
-- Desc: transition from walk to idle class
-- ------------------------------------------------------------------ 

trans_WalkToIdle = oo.class( 
  {
    entity = nil
  },fsm.Transition
)


function trans_WalkToIdle:check()
    if g_MoveKeysIsChange and  not g_MoveKeys.Forward and  not g_MoveKeys.Backward and not g_MoveKeys.Left and not g_MoveKeys.Right then 
      g_MoveKeysIsChange = false
      return true;
    else
      return false;
    end 
end

-- ------------------------------------------------------------------ 
-- Desc: transition from walk to walk class
-- ------------------------------------------------------------------ 
trans_WalkToWalk = oo.class(
  {
   entity = nil
  },
  fsm.Transition
)
function trans_WalkToWalk:check()
     if g_MoveKeysIsChange then 
      g_MoveKeysIsChange = false
      return true
     else
      return false
     end 
end



SetMovementTarget = oo.class( 
  {
    Animationname = "",
    entity = nil
  },Action
) -- Action this class inherit  from Action


function GetAngleFromXYComponent(xComponent,yComponent)

   if xComponent == 0 then 
     if yComponent == 1 then 
      return 90.0
     end  
     if yComponent == -1 then
      return 270.0
     end  
   end
   
   if xComponent == 1 then
     if yComponent == 0 then 
       return 0.0
     end
     if yComponent == 1 then 
       return 45.0
     end 
     if yComponent == -1 then
       return 315.0
     end
    end   
    
   if xComponent == -1 then 
     if yComponent == 0 then 
       return 180.0
     end
       
     if yComponent == 1 then 
        return 135.0
     end  
       
     if yComponent == -1 then 
        return 225.0 
     end 
    end 

end


function SetMovementTarget:exec()
    
     local XDirection = 0
     local YDirection = 0
   
     if  g_MoveKeys.Forward then 
        YDirection = 1
     end  
    
     if g_MoveKeys.Left then 
       XDirection = -1
     end
     
     if g_MoveKeys.Backward then 
        YDirection = -1
     end
     
     if g_MoveKeys.Right then
        XDirection = 1 
     end
     
     
     local cam = CameraAPI.CurrentCamera()
     local m1 = Matrix3.Matrix3()
     m1:FromEulerAnglesXYZ(cam.Rotation.x * ex.math.deg_to_rad, cam.Rotation.y * ex.math.deg_to_rad,cam.Rotation.z * ex.math.deg_to_rad)
  
     local camForward = Point3.Point3()
     m1:GetCol(0, camForward)
     camForward.z = 0
     camForward:Unitize()
     local camRight = Point3.Point3()
     m1:GetCol(2, camRight)
     camRight = camRight * -1.0
     camRight.z = 0
     camRight:Unitize()
       
     local MoveDirection = camForward * YDirection + camRight * XDirection 
       
     MoveDirection:Unitize()
     
     -- calculate  euler angle  of MoveDirection along horizontal
     DirectionDegree = GetAngleFromXYComponent(XDirection,YDirection)
     
     MovementAPI.ChangeVelocityTo(self.entity,g_TransitionTime,MoveDirection * g_walkSpeed);
     MovementAPI.ChangeAngleTo(self.entity,g_TransitionTime * g_RotTimePercent,DirectionDegree); 
     
     egmAnimation.SetTargetAnimationByName(self.entity, self.Animationname)
    
     
end


SetToStop = oo.class( 
  {
    Animationname = "",
    entity = nil
  },Action
) -- Action this class inherit  from Action

function SetToStop:exec()
    
   MovementAPI.ChangeVelocityTo(self.entity,g_TransitionTime,Point3.Point3_ZERO);
   egmAnimation.SetTargetAnimationByName(self.entity, self.Animationname)
     
end







--/////////////////////////////////////////////////////////////////////////////
-- functions
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function OnEntitySetFinished(self, params)
    AIInit(self,params);
    RunningAIStep(self, params);
end


function RunningAIStep(self, params)
    sm:tick();
    self:SendEvent("RunningAIStep")
end


-- ------------------------------------------------------------------ 
-- Desc: Initialize AI 
-- ------------------------------------------------------------------ 
function AIInit(self, params)
  -- define states
    state_idle = fsm.State
    {
        name = "idle",
        entry_action = SetToStop{Animationname="idle",entity=self}
    }
    
    state_walk = fsm.State 
    {
        name = "walk",
        entry_action = SetMovementTarget{Animationname="walk",entity=self}
    }

    -- now we connect each state
    state_idle.transition_list = 
    {
        trans_IdleToWalk 
        { 
            dest_state = state_walk, 
            entity = self,
        },
    }
    
    state_walk.transition_list = 
    {
        trans_WalkToIdle 
        { 
            dest_state = state_idle, 
            entity = self,
        },
        
        trans_WalkToWalk 
        { 
            dest_state = state_walk, 
            entity = self,
        },
        
    }
    -- begen with idle state
    sm = fsm.FSM()
    sm:init(state_idle)

end



-- callback just Forward key press or release  
function Move_Forward(self, params)

 g_MoveKeys.Forward =  not g_MoveKeys.Forward
 g_MoveKeysIsChange = true
 
 
 
 
 end

-- callback just Forward key press or release  
function Move_Backward(self, params)
 g_MoveKeys.Backward =  not g_MoveKeys.Backward
 g_MoveKeysIsChange = true
 

 
end

-- callback just Forward key press or release  
function Move_Left(self, params)
  g_MoveKeys.Left =  not g_MoveKeys.Left
  g_MoveKeysIsChange = true
  
end

-- callback just Forward key press or release  
function Move_Right(self, params)
  g_MoveKeys.Right =  not g_MoveKeys.Right
  g_MoveKeysIsChange = true
 
end
