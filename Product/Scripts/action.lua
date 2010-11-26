-- ======================================================================================
-- File         : action.lua
-- Author       : Wu Jie 
-- Last Change  : 11/22/2010 | 18:04:30 PM | Monday,November
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local fsm = require ("fsm") 
local oo = require ("loop.simple") 

local egmAnimation = egmAnimation

module("action")

--/////////////////////////////////////////////////////////////////////////////
-- actions
--/////////////////////////////////////////////////////////////////////////////

PlayAnim = oo.class( {
    name = "",
    entity = nil,
},Action) -- Action this class inherit  from Action

function PlayAnim:exec()
    egmAnimation.SetTargetAnimationByName(self.entity, self.name)
end

