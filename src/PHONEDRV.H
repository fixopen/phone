/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/
  
/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      phonedrv.cpp                                    PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年11月19日   */
/*                                                                       */
/*      电话驱动程序头文件                                               */
/*                                                                       */
/* 作者                                                                  */
/*                                                                       */
/*      白亚伟                                                           */
/*                                                                       */
/*                                                                       */
/* 数据结构                                                              */
/*                                                                       */
/*                                                                       */
/* 函数                                                                  */
/*                                                                       */
/*                                                                       */
/* 关联                                                                  */
/*                                                                       */
/*                                                                       */
/* 历史记录                                                              */
/*                                                                       */
/*************************************************************************/

#ifndef _E201_TELDRV_H__
#define _E201_TELDRV_H__
#include "hwfcl.h"


/*
*	物理层命令定义
*/

#define		TEL_NONE			0xFF	// 无效字符
#define		TEL_VERSION			0x80	// 读取版本
//#define	TEL_??				0x81	// 保留
#define		TEL_HANDLEON		0x82	// 手柄摘机
#define		TEL_HANDLEOFF		0x83	// 手柄挂机
#define		TEL_SPEAKERON		0x82	// 免提摘机
#define		TEL_SPEAKEROFF		0x83	// 免提挂机
#define		TEL_RING			0x84	// 振铃
#define		TEL_KEYUP			0x85	// 按键抬起, 保留
#define		SMS_OFF				0x86	// 短消息挂机
#define		SMS_ON				0x87	// 短消息摘机
#define		SMS_CASTONE			0x88	// 短消息CAS检测到
#define		TEL_MICMUTE			0x8A	// 免提摘机MIC静音 //ygr11-28
#define		SMS_FSKEND			0x8D	// 短消息 FSK 包结束
#define		TEL_DTMFEND			0x8E	// DTMF 来电结束
#define		TEL_FSKEND			0x8F	// FSK 来电结束
#define		SMS_FSKENABLE		0x90	// 短消息 FSK 接收使能
#define		TEL_SPEAKINGPICKUP	0x94	// 免提状态下手柄摘机(停止留言)
#define		SMS_QUITSMS			0x95	// 短消息模式下按免提(终止短消息)


/*// 老协议
#define		TEL_REDIAL		0x89	// 重拨, 包含于[TEL_KEYSTART, TEL_KEYEND]
#define		TEL_MUTE		0x8B	// 静音
#define		TEL_FLASH		0x8C	// 闪断
*/

// 新协议，底板程序已经更新
#define		TEL_REDIAL		0xBD	// 重拨, 包含于[TEL_KEYSTART, TEL_KEYEND]
#define		TEL_MUTE		0xBE	// 静音, 包含于[TEL_KEYSTART, TEL_KEYEND]
#define		TEL_FLASH		0xBF	// 闪断, 包含于[TEL_KEYSTART, TEL_KEYEND]

//#define		TEL_CALLIDSTART	0x30	// Call ID 编码空间开始
//#define		TEL_CALLIDEND	0x3F	// Call ID 编码空间结束
#define		TEL_KEYSTART	0xB0	// 按键编码空间开始
#define		TEL_KEYEND		0xBF	// 按键编码空间结束
#define		TEL_START		0x30	// 来电显示编码空间开始
#define		TEL_END			0xBF	// 来电显示编码空间结束

#define		DTMF_CHANNEL_STRAT	0x30	// DTMF 来电通道开始
#define		DTMF_CHANNEL_END	0x3F	// DTMF 来电通道结束
#define		FSK_CHANNEL_STRAT	0x40	// FSK 来电通道开始
#define		FSK_CHANNEL_END		0x4F	// FSK 来电通道结束

#define		VER_CHANNEL_START	0x50	// 版本信息通道开始
#define		VER_CHANNEL_END		0x5F	// 版本信息通道结束

#define		SMS_CHANNEL_START	0x10	// SMS 通道开始
#define		SMS_CHANNEL_END		0x1F	// SMS 通道结束

/*
*	数据 buffer 大小定义
*/
#define		TEL_BUFFER_SIZE	4096	// 命令接收缓存大小
#define		TEL_NUM_LEN		30		// 来电号码长度


/*
*	 FSK 包类型
*/
#define		CALLID_TYPE_NONE		0
#define		CALLID_TYPE_DTMF		1
#define		CALLID_TYPE_FSK_SIMPLE	0x04	// YDN069-1997 标准规定
#define		CALLID_TYPE_FSK_COMPLEX	0x80	// YDN069-1997 标准规定

/*
*	复合格式参数类型（用于解码）
*/
#define		CALLID_PARAM_TIME		0x01	// YDN069-1997 标准规定
#define		CALLID_PARAM_NUMBER		0x02	// YDN069-1997 标准规定
#define		CALLID_PARAM_NONUMBER	0x04	// YDN069-1997 标准规定
#define		CALLID_PARAM_NAME		0x07	// YDN069-1997 标准规定
#define		CALLID_PARAM_NONAME		0x08	// YDN069-1997 标准规定

/*
*	不能显示来电原因（用于应用）
*/
#define		CALLID_OK				0
#define		CALLID_NONE				1
#define		CALLID_FORBID			2
#define		CALLID_CANNOT_GET		3

/*
*	来电显示数据结构
*/
#define		PHONE_CALLIDBUF_SEZE	1024
#define		CALLID_NUM_LEN			64
#define		CALLID_NAME_LEN			64

typedef struct
{
	BYTE		type;			// CALLID_TYPE_SIMPLE / CALLID_TYPE_COMPLEX
	BYTE		timestatus;		// CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	BYTE		month;			// 1 - 12
	BYTE		day;			// 0 - 31
	BYTE		hour;			// 0 - 23
	BYTE		minute;			// 0 - 59
	BYTE		numberstatus;	// CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	BYTE		number[CALLID_NUM_LEN];		// string ending with '\0'
	BYTE		namestatus;		// CALLID_OK / CALLID_NONE / CALLID_FORBID / CALLID_CANNOT_GET
	BYTE		name[CALLID_NAME_LEN];		// string ending with '\0'
} CALLID;


/*
*	应用接口
*/

void InitPhoneDrv();			// 初始化
void OpenPhoneMic();			// 免提开
void ClosePhoneMic();			// 免提关
void ForceToFlash();			// 闪断
void DialOut(char *strDial);	// 拨号
BOOL GetPhoneStatus();			// 获取状态，true = 摘机，flase = 挂机
BOOL SetCallIdMode(BOOL bMode = 0);		// 设置来电制式，bMode: 0 = FSK, 1 = DTMF 
BOOL GetSoftMicFlag();
void SetSoftMicFlag(BOOL bShow);
void MicMute();
char* GetPhoneVersion();

//////////////////////////////////////
///向国威增加代码

////////////////////////////////////////////////////////////////////////////////
//描述
//       SMS拨号
//参数
//       strDial   指向以'\0'结束的电话号码字符串
//返回
//       无
bool SmsDialOut(char *strDial);

////////////////////////////////////////////////////////////////////////////////
//描述
//       拨号
//参数
//       strDTMFdata   指的是DTMF码数组
//返回
//       无
//void OrderSendDTMFChainVZ(UINT8 *strDTMFdata,int ByteNumber);

////////////////////////////////////////////////////////////////////////////////
//描述
//       FSKDATA ACK
//参数
//       strDTMFdata   指的是FSK ACK数组
//返回
//       无
void SendDTMFFskAck(UINT8 *strDTMFdata);

////////////////////////////////////////////////////////////////////////////////
//描述
//       DTMF数据包上传函数
//参数
//       strDTMFdata   指的是以0x7F结束的DTMF码数组
//返回
//       无
void OrderSendDTMFData(UINT8 *strDTMFdata,int ByteNumber,UINT8 ServerType);

///////////////////////////////////////////////////////////
//描述   
//       短消息挂机命令
//返回
//       无 
void DisconnectServer();
///////////////////////////////////////////////////////////
//描述   
//       短消息摘机命令
//返回
//       无 
void  SmsPickUpPhone();
//////////////////////////////////////////////////////////
//void OrderSendOrderToEM(UINT8 Order);

#endif
