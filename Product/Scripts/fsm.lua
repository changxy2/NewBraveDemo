-- ======================================================================================
-- File         : fsm.lua
-- Author       : Wu Jie 
-- Last Change  : 11/22/2010 | 13:50:39 PM | Monday,November
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
--
--/////////////////////////////////////////////////////////////////////////////

local assert = assert
local oo = require ("loop.simple") 

module("fsm")

--/////////////////////////////////////////////////////////////////////////////
-- class Action
--/////////////////////////////////////////////////////////////////////////////

Action = oo.class()

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function Action:exec()
    assert ( false, "please implement the exec function in your inherited Action class" )
end

--/////////////////////////////////////////////////////////////////////////////
-- class State
--/////////////////////////////////////////////////////////////////////////////

State = oo.class({
    entry_action = nil,
    exit_action = nil,
    step_action = nil,
    transition_list = {},
})

--/////////////////////////////////////////////////////////////////////////////
-- class Transition
--/////////////////////////////////////////////////////////////////////////////

Transition = oo.class({
    src_state = nil,
    dest_state = nil,
    action = nil,
})

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function Transition:check()
    return false
end

--/////////////////////////////////////////////////////////////////////////////
-- class FSM
--/////////////////////////////////////////////////////////////////////////////

FSM = oo.class({
    init_state = nil,
    cur_state = nil,
})

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function FSM:init ( _initState )
    assert( _initState, "init state can't be nil, pls set it before using the state machine" )
    self.init_state = _initState
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function FSM:tick ()
    -- clear the action list
    local cur_actions = {}
    local cur_state = self.cur_state

    -- if we don't have any state, begin with init state.
    if ( cur_state == nil ) then
        if ( self.init_state ) then
            cur_state = self.init_state
            cur_actions[#cur_actions+1] = cur_state.entry_action
        end
    else
        -- check if we have any transition satisfied the condition
        local triggeredTrans = nil
        for i=1,#cur_state.transition_list do 
            local trans = cur_state.transition_list[i]
            if ( trans:check() ) then
                triggeredTrans = trans
                break
            end
        end

        -- if we have transition triggered
        if ( triggeredTrans ) then
            triggeredTrans.src_state = cur_state
            local nextState = triggeredTrans.dest_state
            if ( cur_state.exit_action ) then 
                cur_actions[#cur_actions+1] = cur_state.exit_action
            end
            if ( triggeredTrans.action ) then 
                cur_actions[#cur_actions+1] = triggeredTrans.action
            end
            if ( nextState.entry_action ) then 
                cur_actions[#cur_actions+1] = nextState.entry_action
            end
            cur_state = nextState
        else -- otherwise, just perform current state's action 
            if ( cur_state.step_action ) then 
                cur_actions[#cur_actions+1] = cur_state.step_action
            end
        end
    end

    -- perform the actions
    for i=1,#cur_actions do
        cur_actions[i]:exec()
    end
    self.cur_state = cur_state
end
