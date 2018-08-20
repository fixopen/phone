// SABTD.h
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//	赛声电话忙音检测器
//  SoftAcoustic Busy Tone Detector(SABTD)
// 
//  Copyright (c) 2007
//  北京赛声科技有限公司
//  版权所有
//  网址：www.softacoustic.com
//  技术支持：support@softacoustic.com
//  
//////////////////////////////////////////////////////////////////////////
#if defined(SABTD_DLL_EXPORTS)
#define SABTD_CEDLL_API __declspec(dllexport)
#elif defined(SABTD_DLL_IMPORTS)
#define SABTD_CEDLL_API __declspec(dllimport)
#else
#define SABTD_CEDLL_API
#endif


#ifndef SABTD_H
#define SABTD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "SABTD_Return_Codes.h"     // 赛声语音引擎返回代码
#include "SABTD_Param_Type.h"       // 忙音检测器参数

#define SABTD_TONE_DETECTED    1
#define SABTD_SILENCE_DETECTED 2

//////////////////////////////////////////////////////////////////////////
// 函数名：SABTD_Init
// 功能：初始化SABTD
// 参数说明：
//       sabtdStatus    void型指针的地址
//       sampleRate     采样率(必须为8000或者44100!)
//       license        指定的license，若为NULL则自动寻找当前目录的"sa_license.dat"
// 返回值：
//       SARTC_NORMAL   正确
//       负值            见SAVE返回代码
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_Init(void **ppSabtdStatus, int sampleRate, char *license);


//////////////////////////////////////////////////////////////////////////
// 函数名：SABTD_UnInit
// 功能：终止SABTD，释放资源
// 参数说明：
//       sabtdStatus    经过SABTD_Init初始化的指针
// 返回值：
//       SARTC_NORMAL   正确
//       负值            见SAVE返回代码
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_UnInit(void *pSabtdStatus);


//////////////////////////////////////////////////////////////////////////
// 函数名：SABTD_SetParam
// 功能：设置参数
// 参数说明：
//       sabtdStatus    经过SABTD_Init初始化的指针
//       paramType      需要设定的参数类型(见底部的定义)
//       paramValue     需要设定的参数值          
// 返回值：
//       SARTC_NORMAL   正确
//       负值            见SAVE返回代码
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_SetParam(void *pSabtdStatus, ParamType paramType, float paramValue);


//////////////////////////////////////////////////////////////////////////
// 函数名：SABTD_SetParam
// 功能：设置参数
// 参数说明：
//       sabtdStatus    经过SABTD_Init初始化的指针
//       paramType      需要获取的参数类型(见底部的定义)
//       paramValue     获取的参数值指针          
// 返回值：
//       SARTC_NORMAL   正确
//       负值            见SAVE返回代码
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_GetParam(void *pSabtdStatus, ParamType paramType, float *pParamValue);


//////////////////////////////////////////////////////////////////////////
// 函数名：SABTD_Reset
// 功能：重置忙音检测器状态
// 参数说明：
//       sabtdStatus    经过SABTD_Init初始化的指针
// 返回值：
//       SARTC_NORMAL   正确
//       负值            见SAVE返回代码
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_Reset(void *pSabtdStatus);


//////////////////////////////////////////////////////////////////////////
// 函数名：SABTD_Process
// 功能：处理
// 参数说明：
//       sabtdStatus    经过SABTD_Init初始化的指针
//       result         输出结果，0为正常音，1为忙音，2为静音
//       data           输入数据数组的指针，数组为短整型(short)
//       length         输入数据数组的长度
// 返回值：
//       SARTC_NORMAL   正确
//       负值            见SAVE返回代码
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_Process(void *pSabtdStatus, int *result, short *data, int length);


#ifdef __cplusplus
}
#endif

#endif // SABTD_H