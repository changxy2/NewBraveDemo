-- ======================================================================================
-- File         : debug.lua
-- Author       : Wu Jie 
-- Last Change  : 09/13/2010 | 10:59:05 AM | Monday,September
-- Description  : 
-- ======================================================================================

--/////////////////////////////////////////////////////////////////////////////
-- require and module
--/////////////////////////////////////////////////////////////////////////////

-- require "lxp"

-- local tinsert, tremove, getn = table.insert, table.remove, table.getn
local assert, type, print, pairs = assert, type, print, pairs
local string, io = string, io
-- local lxp = lxp

module ("ex.debug")

--/////////////////////////////////////////////////////////////////////////////
-- functions defines
--/////////////////////////////////////////////////////////////////////////////

-- ------------------------------------------------------------------ 
-- Desc: 
-- ------------------------------------------------------------------ 

function dump (_o)
    if type(_o) == "number" then
        io.write( _o )
    elseif type(_o) == "string" then
        io.write( string.format("%q", _o) )
    elseif type(_o) == "table" then
        io.write( "{\n" )
        -- str_lv = string.rep("  ",lv)
        for k,v in pairs(_o) do
            io.write( " ", k, " = " )
            dump(v)
            io.write( ",\n" )
        end
        io.write( "}\n" )
    else
        error("cannot dump a " .. type(_o))
    end
end
