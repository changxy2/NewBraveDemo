-- ======================================================================================
-- File         : ease.lua
-- Author       : Wu Jie 
-- Last Change  : 11/02/2010 | 17:08:24 PM | Tuesday,November
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- modules
--/////////////////////////////////////////////////////////////////////////////

require("ex.math") 

local assert,type = assert,type
local pi,two_pi,half_pi = math.pi,ex.math.two_pi,ex.math.half_pi
local sin,cos,asin,acos,sqrt,min,max,abs = math.sin,math.cos,math.asin,math.acos,math.sqrt,math.min,math.max,math.abs

module("ex.ease")

--/////////////////////////////////////////////////////////////////////////////
-- functions
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- linear
-- ------------------------------------------------------------------ 

function linear (_t) return _t end

-- ------------------------------------------------------------------ 
-- quad
--  Easing equation function for a quadratic (_t^2)
--  @param _t: Current time (in frames or seconds).
--  @return: The correct value.
-- ------------------------------------------------------------------ 

function quad_in (_t) return _t^2 end
function quad_out (_t) return -_t * (_t - 2) end
function quad_inout (_t) 
    _t = _t * 2
    if ( _t < 1 ) then 
        return _t^2 / 2
    else
        _t = _t - 1
        return -0.5 * (_t*(_t-2) - 1)
    end
end
function quad_outin (_t) 
    if (_t < 0.5) then return quad_out(_t*2)/2 end
    return quad_in(2*_t-1)/2 + 0.5
end

-- ------------------------------------------------------------------ 
-- cubic 
--  Easing equation function for a cubic (_t^3)
--  @param _t: Current time (in frames or seconds).
--  @return: The correct value.
-- ------------------------------------------------------------------ 

function cubic_in (_t) return _t^3 end
function cubic_out (_t) _t = _t - 1; return _t^3 + 1 end 
function cubic_inout (_t) 
    _t = _t * 2
    if (_t < 1) then
        return _t^3 / 2
    else
        _t = _t - 2
        return (_t^3 + 2)/2
    end
end 
function cubic_outin (_t) 
    if ( _t < 0.5 ) then return cubic_out(2*_t)/2 end
    return cubic_in(2*_t-1)/2 + 0.5
end

-- ------------------------------------------------------------------ 
-- quart 
--  Easing equation function for a quartic (_t^4)
--  @param _t: Current time (in frames or seconds).
--  @return: The correct value.
-- ------------------------------------------------------------------ 

function quart_in (_t) return _t^4 end
function quart_out (_t) _t = _t - 1; return -(_t^4 - 1) end
function quart_inout (_t) 
    _t = _t * 2
    if (_t < 1) then 
        return _t^4 * 0.5
    else
        _t = _t - 2
        return (_t^4 - 2)/-2
    end
end
function quart_outin (_t) 
    if (_t < 0.5) then return quart_out(2*_t)/2 end
    return quart_in(2*_t-1)/2 + 0.5
end

-- ------------------------------------------------------------------ 
-- quint
--  Easing equation function for a quintic (_t^5)
--  @param _t: Current time (in frames or seconds).
--  @return: The correct value.
-- ------------------------------------------------------------------ 

function quint_in (_t) return _t^5 end
function quint_out (_t) _t = _t - 1; return _t^5 + 1 end
function quint_inout (_t)
    _t = _t * 2
    if (_t < 1) then 
        return _t^5 / 2
    else
        _t = _t - 2
        return (_t^5 + 2)/2
    end
end
function quint_outin (_t)
    if (_t < 0.5) then return quint_out (2*_t)/2 end
    return quint_in(2*_t - 1)/2 + 0.5
end

-- ------------------------------------------------------------------ 
-- sine
--  Easing equation function for a sinusoidal (sin(_t))
--  @param _t: Current time (in frames or seconds).
--  @return: The correct value.
-- ------------------------------------------------------------------ 

function sine_in (_t) return (_t == 1) and 1 or -cos(_t * half_pi) + 1 end
function sine_out (_t) return sin(_t * half_pi) end
function sine_inout (_t) return (cos(pi*_t)-1)/-2 end
function sine_outin (_t) 
    if (_t < 0.5) then return sine_out (2*_t)/2 end
    return sine_in(2*_t - 1)/2 + 0.5
end

-- ------------------------------------------------------------------ 
-- class expo 
--  Easing equation function for an exponential (2^_t)
--  param _t: Current time (in frames or seconds).
--  return: The correct value.
-- ------------------------------------------------------------------ 

function expo_in (_t) return (_t == 0 or _t == 1) and _t or 2^(10*(_t-1)) - 0.001 end
function expo_out (_t) return (_t == 1) and 1 or 1.001 * ( 1 - (2^(-10*_t)) ) end
function expo_inout (_t)
    if (_t==0) then return 0 end
    if (_t==1) then return 1 end
    _t = _t * 2
    if (_t < 1) then return 2^(10*(_t-1))/2 - 0.0005 end
    return 1.0005 * (2-2^(-10*(_t-1))) / 2
end
function expo_outin (_t)
    if (_t < 0.5) then return expo_out(2*_t)/2 end
    return expo_in(2*_t-1)/2 + 0.5
end

-- ------------------------------------------------------------------ 
-- circ 
--  Easing equation function for a circular (sqrt(1-_t^2))
--  @param _t: Current time (in frames or seconds).
--  @return:	The correct value.
-- ------------------------------------------------------------------ 

function circ_in (_t) return -(sqrt(1-_t^2) - 1) end
function circ_out (_t) _t = _t - 1; return sqrt(1-_t^2) end
function circ_inout (_t) 
    _t = _t * 2
    if ( _t < 1 ) then
        return -( sqrt(1-_t^2) - 1 )/2
    end
    _t = _t - 2
    return ( sqrt(1-_t^2) + 1 )/2
end
function circ_outin (_t)
    if (_t < 0.5) then 
        return circ_out(2*_t)/2 
    end
    return circ_in(2*_t - 1)/2 + 0.5
end

-- ------------------------------------------------------------------ 
-- elastic 
--  Easing equation function for an elastic (exponentially decaying sine wave)
--  @param _t: Current time (in frames or seconds).
--  @param a: Amplitude.
--  @param p: Period.
--  @return: The correct value.
--  recommand value: elastic (t, 0.1, 0.05)
-- ------------------------------------------------------------------ 

local function elastic_in_helper ( _t, _b, _c, _d, _a, _p )
    if (_t==0) then return _b end
    local t_adj = _t / _d
    if (t_adj==1) then return _b+_c end
    local s
    if ( _a < abs(_c) ) then 
        _a = _c
        s = _p / 4
    else
        s = _p / two_pi * asin(_c/_a)
    end
    t_adj = t_adj - 1
    return -(_a * 2^(10 * t_adj) * sin( two_pi * (t_adj*_d-s) / _p )) + _b
end
local function elastic_out_helper ( _t, _b --[[ dummy --]], _c, _d --[[ dummy --]], _a, _p )
    if (_t==0) then return 0 end
    if (_t==1) then return _c end
    local s
    if (_a < _c) then _a = _c; s = _p / 4
    else s = _p / two_pi * asin(_c/_a) end
    return _a * 2^(-10*_t) * sin( (_t-s) * two_pi / _p ) + _c
end

function elastic_in ( _t, _a, _p ) return elastic_in_helper ( _t, 0, 1, 1, _a, _p ) end
function elastic_out ( _t, _a, _p ) return elastic_out_helper ( _t, 0, 1, 1, _a, _p ) end
function elastic_inout ( _t, _a, _p ) 
    if (_t==0) then return 0 end
    _t = _t * 2
    if (_t==2) then return 1 end

    local s
    if (_a<1) then _a = 1; s = _p/4
    else s = _p / two_pi * asin(1/_a) end

    if (_t < 1) then
        return (_a * 2^(10*(_t-1)) * sin( (_t-1-s) * two_pi / _p ))/-2
    end
    return _a * 2^(-10*(_t-1)) * sin( (_t-1-s) * two_pi / _p ) / 2 + 1
end
function elastic_outin ( _t, _a, _p ) 
    if ( _t < 0.5 ) then return elastic_out_helper ( 2*_t, 0, 0.5, 1, _a, _p ) end
    return elastic_in_helper ( 2*_t - 1, 0.5, 0.5, 1, _a, _p )
end


-- ------------------------------------------------------------------ 
-- back 
--  Easing equation function for a back (overshooting cubic easing: (s+1)*_t^3 - s*_t^2)
--  @param _t: Current time (in frames or seconds).
--  @param s: Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
--  @return: The correct value.
-- ------------------------------------------------------------------ 

function back_in ( _t, _s ) return _t * _t * ( (_s+1) * _t - _s ) end
function back_out ( _t, _s ) _t = _t - 1; return _t * _t * ( (_s+1) * _t + _s ) + 1 end
function back_inout ( _t, _s ) 
    _t = _t * 2
    if ( _t < 1 ) then
        _s = _s * 1.525
        return 0.5 * ( _t * _t * ( (_s+1) * _t - _s ) )
    else
        _t = _t - 2
        _s = _s * 1.525
        return 0.5 * ( _t * _t * ( (_s+1) * _t + _s ) + 2 )
    end
end
function back_outin ( _t, _s ) 
    if ( _t < 0.5 ) then return back_out(2*_t,_s) / 2 end
    return back_in(2*_t - 1, _s)/2 + 0.5
end

-- ------------------------------------------------------------------ 
-- bounce 
--  Easing equation function for a bounce (exponentially decaying parabolic bounce)
--  @param _t: Current time (in frames or seconds).
--  @param a: Amplitude.
--  @return: The correct value.
-- ------------------------------------------------------------------ 

local function bounce_out_helper ( _t, _c, _a )
    if (_t == 1) then return _c end
    if ( _t < (4/11) ) then
        return _c * ( 7.5625 * _t * _t )
    elseif ( _t < 8/11 ) then
        _t = _t - (6/11)
        return -_a * (1 - (7.5625*_t*_t + 0.75)) + _c
    elseif (_t < 10/11) then
        _t = _t - 9/11
        return -_a * (1 - (7.5625*_t*_t + 0.9375)) + _c
    else
        _t = _t - 21/22
        return -_a * (1 - (7.5625*_t*_t + 0.984375)) + _c
    end
end

function bounce_out ( _t, _a ) return bounce_out_helper(_t, 1, _a) end
function bounce_in ( _t, _a ) return 1 - bounce_out_helper(1-_t, 1, _a) end
function bounce_inout ( _t, _a ) 
    if (_t < 0.5) then return bounce_in(2*_t,_a)/2; end
    return (_t == 1) and 1 or bounce_out(2*_t-1, _a)/2 + 0.5
end
function bounce_outin ( _t, _a ) 
    if (_t < 0.5) then return bounce_out_helper(_t*2, 0.5, _a) end
    return 1 - bounce_out_helper(2-2*_t, 0.5, _a)
end

-- ------------------------------------------------------------------ 
-- smooth 
-- _t<=0: 0 | 0<_t<1: 3*_t^2 - 2*t^3 | _t>=1: 1
-- ------------------------------------------------------------------ 

function smooth (_t)
    if ( _t <= 0 ) then return 0 end
    if ( _t >= 1 ) then return 1 end
    return _t*_t*(3 - 2*_t)
end

-- ------------------------------------------------------------------ 
-- fade 
-- _t<=0: 0 | 0<_t<1: 6*_t^5 - 15*_t^4 + 10*_t^3 | _t>=1: 1
-- ------------------------------------------------------------------ 

function fade (_t)
    if ( _t <= 0 ) then return 0 end
    if ( _t >= 1 ) then return 1 end
    return _t*_t*_t*(_t*(_t*6-15)+10)
end

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

local function sin_progress (_v) return sin((_v * pi) - half_pi) / 2 + 0.5 end
local function smooth_begin_end_mix_factor (_v) return min(max(1 - _v * 2 + 0.3, 0), 1) end

-- SmoothBegin blends Smooth and Linear Interpolation.
-- Progress 0 - 0.3      : Smooth only
-- Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
-- Progress ~ 0.5  - 1   : Linear only

-- Easing function that starts growing slowly, then increases in speed. At the end of the curve the speed will be constant.
function curve_in (_t)
    local sinProgress = sin_progress(_t)
    local mix = smooth_begin_end_mix_factor(_t)
    return sinProgress * mix + _t * (1 - mix)
end

-- Easing function that starts growing steadily, then ends slowly. The speed will be constant at the beginning of the curve.
function curve_out (_t)
    local sinProgress = sin_progress(_t)
    local mix = smooth_begin_end_mix_factor(1 - _t)
    return sinProgress * mix + _t * (1 - mix)
end

-- Easing function where the value grows sinusoidally. Note that the calculated  end value will be 0 rather than 1.
function curve_sine (_t) 
    return (sin(((_t * two_pi)) - half_pi) + 1) / 2 
end

-- Easing function where the value grows cosinusoidally. Note that the calculated start value will be 0.5 and the end value will be 0.5
-- contrary to the usual 0 to 1 easing curve.
function curve_cosine (_t) 
    return (cos(((_t * two_pi)) - half_pi) + 1) / 2 
end
