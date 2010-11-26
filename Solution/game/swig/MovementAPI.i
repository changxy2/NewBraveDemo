%feature("compactdefaultargs");
%include <windows.i>
%include <typemaps.i>

//下面的名字是曝露给脚本语言的名字空间。
%module MovementAPI

//包含在"%{....%}" 块内的代码是被直接写到包裹文件中的，一般的你会包含定义你内建API 函数的
//所有头文件
%{
#include "../ID.h"
#include "egfLua/bapiLua.h"
#include "../MovementAPI.h" 
%}



// egf/BehaviorAPI.i  为写你自己的内建APIs 包含了很多有辅助特性。推荐把他倒入到所有其他内建API 模块中
// 通过使用'%import' 而不是'%include' 我们在没有复制任何一个他的函数就把BehaviorAPI的功能引进进来 

//%import "egf/BehaviorAPI.i" 

// 类型表例子 我们提供支持非常多的功能和游戏框架数据类型。他也为作为处理事务的输入，输出，或者输出参数
//的  maps and lists and 规则定义类型。
%include "egfLua/bapiLuaTypeMaps.i"

// 定义要曝露的函数最简的方法是 include 一个 包含所有你想 绑定 内建 API 函数 的 c++ 头文件
%include "MovementAPI.h" 

