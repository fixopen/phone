// SABTD_CEDLL.h
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
//  赛声电话忙音检测器
//  SoftAcoustic Busy Tone Detector(SABTD)
// 
//  Copyright (c) 2007
//  北京赛声科技有限公司
//  版权所有
//  网址：www.softacoustic.com
//  技术支持：support@softacoustic.com
//  
//////////////////////////////////////////////////////////////////////
#ifdef SABTD_CEDLL_EXPORTS
#define SABTD_CEDLL_API __declspec(dllexport)
#else
#define SABTD_CEDLL_API __declspec(dllimport)
#endif

#ifndef SABTD_CEDLL_H
#define SABTD_CEDLL_H

//////////////////////////////////////////////////////////////
// 函数名：SABTD_Init
// 功能：初始化SABTD
// 参数说明：
//       sabtdStatus    void型指针的地址
//       sampleRate     采样率，!!必须为8000或者44100!!
// 返回值：
//       0              正确
//       -1000          授权文件出错
//       其它负值        初始化出错
//////////////////////////////////////////////////////////////
int SABTD_Init(void** sabtdStatus, int sampleRate) {
    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名：SABTD_UnInit
// 功能：终止SABTD，释放资源
// 参数说明：
//       sabtdStatus    经过SABTD_Init初始化的指针
// 返回值：
//       0              正确
//       -1000          授权文件出错
//       其它负值        终止出错
//////////////////////////////////////////////////////////////
int SABTD_UnInit(void* sabtdStatus) {
    return 0;
}

//////////////////////////////////////////////////////////////
// 函数名：SABTD_Process
// 功能：处理
// 参数说明：
//       sabtdStatus    经过SABTD_Init初始化的指针
//       result         输出结果，0为正常音，1为忙音
//       data           输入数据数组的指针，数组为短整型(short)
//       length         输入数据数组的长度
// 返回值：
//       0              正确
//       -1000          授权文件出错
//       其它负值        处理出错
//////////////////////////////////////////////////////////////
int SABTD_Process(void* sabtdStatus, int* result, short* data, int length) {
    return 0;
}

#endif // SABTD_CEDLL_H