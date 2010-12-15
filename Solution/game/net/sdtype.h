
#ifndef SGDP_SDTYPE_H_20070601
#define SGDP_SDTYPE_H_20070601

//
// 1字节有符号类型
//
typedef signed char         INT8;

//
// 1字节无符号类型
//
typedef unsigned char       UINT8;

//
// 2字节有符号类型
//
typedef signed short        INT16;

//
// 2字节无符号类型
//
typedef unsigned short      UINT16;

//
// 4字节有符号类型
//
typedef signed int          INT32;

//
// 4字节无符号类型
//
typedef unsigned int        UINT32;

//
// 8字节有符号类型
//
typedef signed long long    INT64;

//
// 8字节无符号类型
//
typedef unsigned long long  UINT64;

//
// printf 中的格式字符串定义，例如
// printf("The int64 number is: "FMT_I64, llNumber);
// printf("The uint64 number is: "FMT_U64, qwNumber);
// printf("The uint64 number is: "FMT_64X, qwNumber);
//
#ifdef WIN32
    #define FMT_I64 "%I64"
    #define FMT_U64 "%I64u"
    #define FMT_64X "%I64x"
#else
    #define FMT_I64 "%ll"
    #define FMT_U64 "%llu"
    #define FMT_64X "%llx"
#endif

#endif
