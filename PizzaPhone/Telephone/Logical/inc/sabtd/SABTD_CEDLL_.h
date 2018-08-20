// SABTD_CEDLL.h
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
//  �����绰æ�������
//  SoftAcoustic Busy Tone Detector(SABTD)
// 
//  Copyright (c) 2007
//  ���������Ƽ����޹�˾
//  ��Ȩ����
//  ��ַ��www.softacoustic.com
//  ����֧�֣�support@softacoustic.com
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
// ��������SABTD_Init
// ���ܣ���ʼ��SABTD
// ����˵����
//       sabtdStatus    void��ָ��ĵ�ַ
//       sampleRate     �����ʣ�!!����Ϊ8000����44100!!
// ����ֵ��
//       0              ��ȷ
//       -1000          ��Ȩ�ļ�����
//       ������ֵ        ��ʼ������
//////////////////////////////////////////////////////////////
int SABTD_Init(void** sabtdStatus, int sampleRate) {
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������SABTD_UnInit
// ���ܣ���ֹSABTD���ͷ���Դ
// ����˵����
//       sabtdStatus    ����SABTD_Init��ʼ����ָ��
// ����ֵ��
//       0              ��ȷ
//       -1000          ��Ȩ�ļ�����
//       ������ֵ        ��ֹ����
//////////////////////////////////////////////////////////////
int SABTD_UnInit(void* sabtdStatus) {
    return 0;
}

//////////////////////////////////////////////////////////////
// ��������SABTD_Process
// ���ܣ�����
// ����˵����
//       sabtdStatus    ����SABTD_Init��ʼ����ָ��
//       result         ��������0Ϊ��������1Ϊæ��
//       data           �������������ָ�룬����Ϊ������(short)
//       length         ������������ĳ���
// ����ֵ��
//       0              ��ȷ
//       -1000          ��Ȩ�ļ�����
//       ������ֵ        �������
//////////////////////////////////////////////////////////////
int SABTD_Process(void* sabtdStatus, int* result, short* data, int length) {
    return 0;
}

#endif // SABTD_CEDLL_H