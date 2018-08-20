// SABTD.h
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
//	�����绰æ�������
//  SoftAcoustic Busy Tone Detector(SABTD)
// 
//  Copyright (c) 2007
//  ���������Ƽ����޹�˾
//  ��Ȩ����
//  ��ַ��www.softacoustic.com
//  ����֧�֣�support@softacoustic.com
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

#include "SABTD_Return_Codes.h"     // �����������淵�ش���
#include "SABTD_Param_Type.h"       // æ�����������

#define SABTD_TONE_DETECTED    1
#define SABTD_SILENCE_DETECTED 2

//////////////////////////////////////////////////////////////////////////
// ��������SABTD_Init
// ���ܣ���ʼ��SABTD
// ����˵����
//       sabtdStatus    void��ָ��ĵ�ַ
//       sampleRate     ������(����Ϊ8000����44100!)
//       license        ָ����license����ΪNULL���Զ�Ѱ�ҵ�ǰĿ¼��"sa_license.dat"
// ����ֵ��
//       SARTC_NORMAL   ��ȷ
//       ��ֵ            ��SAVE���ش���
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_Init(void **ppSabtdStatus, int sampleRate, char *license);


//////////////////////////////////////////////////////////////////////////
// ��������SABTD_UnInit
// ���ܣ���ֹSABTD���ͷ���Դ
// ����˵����
//       sabtdStatus    ����SABTD_Init��ʼ����ָ��
// ����ֵ��
//       SARTC_NORMAL   ��ȷ
//       ��ֵ            ��SAVE���ش���
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_UnInit(void *pSabtdStatus);


//////////////////////////////////////////////////////////////////////////
// ��������SABTD_SetParam
// ���ܣ����ò���
// ����˵����
//       sabtdStatus    ����SABTD_Init��ʼ����ָ��
//       paramType      ��Ҫ�趨�Ĳ�������(���ײ��Ķ���)
//       paramValue     ��Ҫ�趨�Ĳ���ֵ          
// ����ֵ��
//       SARTC_NORMAL   ��ȷ
//       ��ֵ            ��SAVE���ش���
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_SetParam(void *pSabtdStatus, ParamType paramType, float paramValue);


//////////////////////////////////////////////////////////////////////////
// ��������SABTD_SetParam
// ���ܣ����ò���
// ����˵����
//       sabtdStatus    ����SABTD_Init��ʼ����ָ��
//       paramType      ��Ҫ��ȡ�Ĳ�������(���ײ��Ķ���)
//       paramValue     ��ȡ�Ĳ���ֵָ��          
// ����ֵ��
//       SARTC_NORMAL   ��ȷ
//       ��ֵ            ��SAVE���ش���
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_GetParam(void *pSabtdStatus, ParamType paramType, float *pParamValue);


//////////////////////////////////////////////////////////////////////////
// ��������SABTD_Reset
// ���ܣ�����æ�������״̬
// ����˵����
//       sabtdStatus    ����SABTD_Init��ʼ����ָ��
// ����ֵ��
//       SARTC_NORMAL   ��ȷ
//       ��ֵ            ��SAVE���ش���
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_Reset(void *pSabtdStatus);


//////////////////////////////////////////////////////////////////////////
// ��������SABTD_Process
// ���ܣ�����
// ����˵����
//       sabtdStatus    ����SABTD_Init��ʼ����ָ��
//       result         ��������0Ϊ��������1Ϊæ����2Ϊ����
//       data           �������������ָ�룬����Ϊ������(short)
//       length         ������������ĳ���
// ����ֵ��
//       SARTC_NORMAL   ��ȷ
//       ��ֵ            ��SAVE���ش���
//////////////////////////////////////////////////////////////////////////
SABTD_CEDLL_API int
SABTD_Process(void *pSabtdStatus, int *result, short *data, int length);


#ifdef __cplusplus
}
#endif

#endif // SABTD_H