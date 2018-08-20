#ifndef __SMS_LOGICAL_SMSWARP_H__
#define __SMS_LOGICAL_SMSWARP_H__

#include "../../Util/ATCommandWarp.h"
#include "../../Util/HTTPTransport.h"
#include "../Data/Message.h"

// 用户信息编码方式 
#define GSM_7BIT		0 
#define GSM_8BIT		4 
#define GSM_UCS2		8 

// 应答状态 
#define GSM_WAIT		0		// 等待，不确定
#define GSM_OK			1		// OK
#define GSM_ERR			-1		// ERROR

#define MAX_SM_SEND		128		// 发送队列长度
#define MAX_SM_RECV		128		// 接收队列长度

namespace SMS
{
	class SMSWarp
	{
	public:
		SMSWarp();
		~SMSWarp();
		SMSWarp(const SMSWarp& smsWarp);
		static SMSWarp* GetSMSWarp();
		void Bind(Util::ATCommandWarp* at);

	private:
		Util::ATCommandWarp* m_pAT;
	public:
		// 短消息参数结构，编码/解码共用
		// 其中，字符串以'\0'结尾
		typedef struct { 
			char SCA[16];			// 短消息服务中心号码(SMSC地址)
			char TPA[16];			// 目标号码或回复号码(TP-DA或TP-RA)
			char TP_PID;			// 用户信息协议标识(TP-PID)
			char TP_DCS;			// 用户信息编码方式(TP-DCS)
			char TP_SCTS[16];		// 服务时间戳字符串(TP_SCTS), 接收时用到
			char TP_UD[160];		// 原始用户信息(编码前或解码后的TP-UD)
			short index;			// 短消息序号，在读取时用到

			char Type;	//消息类型
			unsigned int MMS_Size;   //彩信大小
			unsigned int MMS_Expiry; //彩信有效期
		} SM_PARAM;

		// 读取应答的缓冲区
		typedef struct {
			int len;
			char data[16384];
		} SM_BUFF;

		typedef enum
		{
			mtSMS,
			mtMMS,
			mtSI,
			mtOTAMenu = 0x00022B7,
			mtOTAdMediaFile,
			mtOTAInterface,
			mtOTAWeather = 0x0002328,
		} SMS_TYPE;

		int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength); 
		int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength); 
		int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst); 
		int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst); 

		//BOOL gsmInit(); 

		int gsmSendMessage(SM_PARAM* pSrc); 
		int gsmReadMessageList(); 
		int gsmDeleteMessage(int index); 

		int gsmGetResponse(SM_BUFF* pBuff); 

		int gsmParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff); 
		//////////////////////////////////////////////////////////////////////////
		int m_nSendIn;		// 发送队列的输入指针 
		int m_nSendOut;		// 发送队列的输出指针 

		int m_nRecvIn;		// 接收队列的输入指针 
		int m_nRecvOut;		// 接收队列的输出指针 

		SM_PARAM m_SmSend[MAX_SM_SEND];		// 发送短消息队列 
		SM_PARAM m_SmRecv[MAX_SM_SEND];		// 接收短消息队列 

		CRITICAL_SECTION m_csSend;		// 与发送相关的临界段 
		CRITICAL_SECTION m_csRecv;		// 与接收相关的临界段 

		HANDLE m_hKillThreadEvent;		// 通知子线程关闭的事件 
		HANDLE m_hThreadKilledEvent;	// 子线程宣告关闭的事件 

		void PutSendMessage(SM_PARAM* pSmParam);	// 将短消息放入发送队列 
		BOOL GetSendMessage(SM_PARAM* pSmParam);	// 从发送队列中取一条短消息 
		void PutRecvMessage(SM_PARAM* pSmParam, int nCount);	// 将短消息放入接收队列 
		BOOL GetRecvMessage(SM_PARAM* pSmParam);	// 从接收队列中取一条短消息

		static UINT SmThread(LPVOID lpParam);	// 短消息收发处理子线程 

	};
}


namespace SMS {
    namespace Logical {
        class SMSWarp {
        public:
            enum Status {
                sNotifier,
                //sReadedNotifier,
                //sGetContent,
                sContentCompleted,
                sComplete,
            };
            void processMessage(Data::Message const& message);
        private:
            unsigned int timerId_;
            Util::HTTPTransport* transport_;
            std::vector<Data::Message> unreadedMessages_;
            Status state_;
        };
    }
}

// 用户信息编码方式 
#define GSM_7BIT		0 
#define GSM_8BIT		4 
#define GSM_UCS2		8 

// 应答状态 
#define GSM_WAIT		0		// 等待，不确定
#define GSM_OK			1		// OK
#define GSM_ERR			-1		// ERROR

#define MAX_SM_SEND		128		// 发送队列长度
#define MAX_SM_RECV		128		// 接收队列长度

namespace SMS
{
	class SMSWarp
	{
	public:
		SMSWarp();
		~SMSWarp();
		SMSWarp(const SMSWarp& smsWarp);
		static SMSWarp* GetSMSWarp();
		void Bind(Util::ATCommandWarp* at);

	private:
		Util::ATCommandWarp* m_pAT;
	public:
		// 短消息参数结构，编码/解码共用
		// 其中，字符串以'\0'结尾
		typedef struct { 
			char SCA[16];			// 短消息服务中心号码(SMSC地址)
			char TPA[16];			// 目标号码或回复号码(TP-DA或TP-RA)
			char TP_PID;			// 用户信息协议标识(TP-PID)
			char TP_DCS;			// 用户信息编码方式(TP-DCS)
			char TP_SCTS[16];		// 服务时间戳字符串(TP_SCTS), 接收时用到
			char TP_UD[160];		// 原始用户信息(编码前或解码后的TP-UD)
			short index;			// 短消息序号，在读取时用到

			char Type;	//消息类型
			unsigned int MMS_Size;   //彩信大小
			unsigned int MMS_Expiry; //彩信有效期
		} SM_PARAM;

		// 读取应答的缓冲区
		typedef struct {
			int len;
			char data[16384];
		} SM_BUFF;

		typedef enum
		{
			mtSMS,
			mtMMS,
			mtSI,
			mtOTAMenu = 0x00022B7,
			mtOTAdMediaFile,
			mtOTAInterface,
			mtOTAWeather = 0x0002328,
		} SMS_TYPE;

		int gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength); 
		int gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength); 
		int gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength); 
		int gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength); 
		int gsmEncodePdu(const SM_PARAM* pSrc, char* pDst); 
		int gsmDecodePdu(const char* pSrc, SM_PARAM* pDst); 

		//BOOL gsmInit(); 

		int gsmSendMessage(SM_PARAM* pSrc); 
		int gsmReadMessageList(); 
		int gsmDeleteMessage(int index); 

		int gsmGetResponse(SM_BUFF* pBuff); 

		int gsmParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff); 
		//////////////////////////////////////////////////////////////////////////
		int m_nSendIn;		// 发送队列的输入指针 
		int m_nSendOut;		// 发送队列的输出指针 

		int m_nRecvIn;		// 接收队列的输入指针 
		int m_nRecvOut;		// 接收队列的输出指针 

		SM_PARAM m_SmSend[MAX_SM_SEND];		// 发送短消息队列 
		SM_PARAM m_SmRecv[MAX_SM_SEND];		// 接收短消息队列 

		CRITICAL_SECTION m_csSend;		// 与发送相关的临界段 
		CRITICAL_SECTION m_csRecv;		// 与接收相关的临界段 

		HANDLE m_hKillThreadEvent;		// 通知子线程关闭的事件 
		HANDLE m_hThreadKilledEvent;	// 子线程宣告关闭的事件 

		void PutSendMessage(SM_PARAM* pSmParam);	// 将短消息放入发送队列 
		BOOL GetSendMessage(SM_PARAM* pSmParam);	// 从发送队列中取一条短消息 
		void PutRecvMessage(SM_PARAM* pSmParam, int nCount);	// 将短消息放入接收队列 
		BOOL GetRecvMessage(SM_PARAM* pSmParam);	// 从接收队列中取一条短消息

		static UINT SmThread(LPVOID lpParam);	// 短消息收发处理子线程 

	};
}


#endif // __SMS_LOGICAL_SMSWARP_H__
