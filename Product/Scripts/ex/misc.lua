-- ======================================================================================
-- File         : misc.lua
-- Author       : Wu Jie 
-- Last Change  : 11/04/2010 | 16:48:25 PM | Thursday,November
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- require and module
--/////////////////////////////////////////////////////////////////////////////

require("ex.math")
require("ex.ease")

local ex_math,ex_ease = ex.math,ex.ease
local os,math = os,math
local assert,type = assert,type
local BehaviorAPI = BehaviorAPI

module("ex.misc")

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function make_curve ( _from, _to, _duration, _curve )
    assert ( type(_from) == "number", "_from is not a number" )
    assert ( type(_to) == "number", "_to is not a number" )

    -- init startTime and curve
    local startTime = os.clock()
    local curve = _curve or ex_ease.linear  

    -- create tick closure
    return function ()
        local curTime = os.clock()
        local t = math.min( 1.0, (curTime - startTime) / _duration )
        local ratio = curve(t)
        return ex_math.lerp( _from, _to, ratio ), (t == 1.0)
    end
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function make_curve_gb ( _from, _to, _duration, _curve )
    assert ( type(_from) == "number", "_from is not a number" )
    assert ( type(_to) == "number", "_to is not a number" )

    -- init startTime and curve
    local startTime = BehaviorAPI.GetGameTime()
    local curve = _curve or ex_ease.linear  

    -- create tick closure
    return function ()
        local curTime = BehaviorAPI.GetGameTime()
        local t = math.min( 1.0, (curTime - startTime) / _duration )
        local ratio = curve(t)
        return ex_math.lerp( _from, _to, ratio )
    end
end
