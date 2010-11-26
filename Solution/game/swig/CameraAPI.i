%feature("compactdefaultargs");
%include <windows.i>
%include <typemaps.i>

// module code goes directly into the generated cpp
%module CameraAPI
%{
#include "../ID.h"
#include "egfLua/bapiLua.h"
#include "../Swig_helpers/CameraAPI.h"
%}

%include "egfLua/bapiLuaTypeMaps.i"
%include "Swig_helpers/CameraAPI.h"
