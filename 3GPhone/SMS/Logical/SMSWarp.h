#ifndef __SMS_LOGICAL_SMSWARP_H__
#define __SMS_LOGICAL_SMSWARP_H__

#include "../../Util/ATCommandWarp.h"
#include "../../Util/HTTPTransport.h"
#include "../Data/Message.h"

// �û���Ϣ���뷽ʽ 
#define GSM_7BIT		0 
#define GSM_8BIT		4 
#define GSM_UCS2		8 

// Ӧ��״̬ 
#define GSM_WAIT		0		// �ȴ�����ȷ��
#define GSM_OK			1		// OK
#define GSM_ERR			-1		// ERROR

#define MAX_SM_SEND		128		// ���Ͷ��г���
#define MAX_SM_RECV		128		// ���ն��г���

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
		// ����Ϣ�����ṹ������/���빲��
		// ���У��ַ�����'\0'��β
		typedef struct { 
			char SCA[16];			// ����Ϣ�������ĺ���(SMSC��ַ)
			char TPA[16];			// Ŀ������ظ�����(TP-DA��TP-RA)
			char TP_PID;			// �û���ϢЭ���ʶ(TP-PID)
			char TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)
			char TP_SCTS[16];		// ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
			char TP_UD[160];		// ԭʼ�û���Ϣ(����ǰ�������TP-UD)
			short index;			// ����Ϣ��ţ��ڶ�ȡʱ�õ�

			char Type;	//��Ϣ����
			unsigned int MMS_Size;   //���Ŵ�С
			unsigned int MMS_Expiry; //������Ч��
		} SM_PARAM;

		// ��ȡӦ��Ļ�����
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
		int m_nSendIn;		// ���Ͷ��е�����ָ�� 
		int m_nSendOut;		// ���Ͷ��е����ָ�� 

		int m_nRecvIn;		// ���ն��е�����ָ�� 
		int m_nRecvOut;		// ���ն��е����ָ�� 

		SM_PARAM m_SmSend[MAX_SM_SEND];		// ���Ͷ���Ϣ���� 
		SM_PARAM m_SmRecv[MAX_SM_SEND];		// ���ն���Ϣ���� 

		CRITICAL_SECTION m_csSend;		// �뷢����ص��ٽ�� 
		CRITICAL_SECTION m_csRecv;		// �������ص��ٽ�� 

		HANDLE m_hKillThreadEvent;		// ֪ͨ���̹߳رյ��¼� 
		HANDLE m_hThreadKilledEvent;	// ���߳�����رյ��¼� 

		void PutSendMessage(SM_PARAM* pSmParam);	// ������Ϣ���뷢�Ͷ��� 
		BOOL GetSendMessage(SM_PARAM* pSmParam);	// �ӷ��Ͷ�����ȡһ������Ϣ 
		void PutRecvMessage(SM_PARAM* pSmParam, int nCount);	// ������Ϣ������ն��� 
		BOOL GetRecvMessage(SM_PARAM* pSmParam);	// �ӽ��ն�����ȡһ������Ϣ

		static UINT SmThread(LPVOID lpParam);	// ����Ϣ�շ��������߳� 

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

// �û���Ϣ���뷽ʽ 
#define GSM_7BIT		0 
#define GSM_8BIT		4 
#define GSM_UCS2		8 

// Ӧ��״̬ 
#define GSM_WAIT		0		// �ȴ�����ȷ��
#define GSM_OK			1		// OK
#define GSM_ERR			-1		// ERROR

#define MAX_SM_SEND		128		// ���Ͷ��г���
#define MAX_SM_RECV		128		// ���ն��г���

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
		// ����Ϣ�����ṹ������/���빲��
		// ���У��ַ�����'\0'��β
		typedef struct { 
			char SCA[16];			// ����Ϣ�������ĺ���(SMSC��ַ)
			char TPA[16];			// Ŀ������ظ�����(TP-DA��TP-RA)
			char TP_PID;			// �û���ϢЭ���ʶ(TP-PID)
			char TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)
			char TP_SCTS[16];		// ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
			char TP_UD[160];		// ԭʼ�û���Ϣ(����ǰ�������TP-UD)
			short index;			// ����Ϣ��ţ��ڶ�ȡʱ�õ�

			char Type;	//��Ϣ����
			unsigned int MMS_Size;   //���Ŵ�С
			unsigned int MMS_Expiry; //������Ч��
		} SM_PARAM;

		// ��ȡӦ��Ļ�����
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
		int m_nSendIn;		// ���Ͷ��е�����ָ�� 
		int m_nSendOut;		// ���Ͷ��е����ָ�� 

		int m_nRecvIn;		// ���ն��е�����ָ�� 
		int m_nRecvOut;		// ���ն��е����ָ�� 

		SM_PARAM m_SmSend[MAX_SM_SEND];		// ���Ͷ���Ϣ���� 
		SM_PARAM m_SmRecv[MAX_SM_SEND];		// ���ն���Ϣ���� 

		CRITICAL_SECTION m_csSend;		// �뷢����ص��ٽ�� 
		CRITICAL_SECTION m_csRecv;		// �������ص��ٽ�� 

		HANDLE m_hKillThreadEvent;		// ֪ͨ���̹߳رյ��¼� 
		HANDLE m_hThreadKilledEvent;	// ���߳�����رյ��¼� 

		void PutSendMessage(SM_PARAM* pSmParam);	// ������Ϣ���뷢�Ͷ��� 
		BOOL GetSendMessage(SM_PARAM* pSmParam);	// �ӷ��Ͷ�����ȡһ������Ϣ 
		void PutRecvMessage(SM_PARAM* pSmParam, int nCount);	// ������Ϣ������ն��� 
		BOOL GetRecvMessage(SM_PARAM* pSmParam);	// �ӽ��ն�����ȡһ������Ϣ

		static UINT SmThread(LPVOID lpParam);	// ����Ϣ�շ��������߳� 

	};
}


#endif // __SMS_LOGICAL_SMSWARP_H__
