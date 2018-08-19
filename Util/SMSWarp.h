#ifndef SMSWarp_h
#define SMSWarp_h

#include "ATCommandWarp.h"
//#include "../MMS/MMSWarp.h"

// �û���Ϣ���뷽ʽ 
#define GSM_7BIT        0 
#define GSM_8BIT        4 
#define GSM_UCS2        8 

// Ӧ��״̬ 
#define GSM_WAIT        0       // �ȴ�����ȷ�� 
#define GSM_OK          1       // OK 
#define GSM_ERR         -1      // ERROR 

#define MAX_SM_SEND     16      // ���Ͷ��г���      //lxz   128
#define MAX_SM_RECV     16      // ���ն��г��� 

namespace SMS
{
    class SMSWarp {
    public:
        SMSWarp();
        ~SMSWarp();
        SMSWarp(const SMSWarp& smsWarp);
        static SMSWarp* GetSMSWarp();
        void Bind(Util::ATCommandWarp* at);
        void Bind_(Util::ATCommandWarp* at);
        void SetOTANumber(std::string number);

    private:
        Util::ATCommandWarp* m_pAT;
        std::string m_strOTANumber;
        std::string m_strCenterAddress;
    public:

        typedef enum {
            mtSMS,
            mtMMS,
            mtOTA,
            mtSI,
        } SMS_TYPE;
        typedef enum {
            tOTAMenu = 0x000022B7,
            tOTAdMediaFile = 0x000022B8,
            tOTAInterface = 0x000022B9,
            tOTAWeather = 0x00002328,
        }OTA_TYPE;
        //typedef struct
        //{
        //  OTA_TYPE Type;
        //}OTA_NotificationRequest;
        // ����Ϣ�����ṹ������/���빲�� 
        // ���У��ַ�����'\0'��β 
        typedef struct {
            char SCA[32];           // ����Ϣ�������ĺ���(SMSC��ַ) 
            char TPA[32];           // Ŀ������ظ�����(TP-DA��TP-RA) 
            char TP_PID;            // �û���ϢЭ���ʶ(TP-PID) 
            char TP_DCS;            // �û���Ϣ���뷽ʽ(TP-DCS) 
            char TP_SCTS[16];       // ����ʱ����ַ���(TP_SCTS), ����ʱ�õ� 
            char TP_UD[1024];       // ԭʼ�û���Ϣ(����ǰ�������TP-UD)        //160    1024    
            short index;            // ����Ϣ��ţ��ڶ�ȡʱ�õ� 

            char Serial[4];
            unsigned char Total;
            unsigned char Number;

            SMS_TYPE Type;  //��Ϣ����

            char MMS_Type;
            std::string MMS_TransactionID;
            char MMS_Version;
            char MMS_Class;
            unsigned int MMS_Size;   //���Ŵ�С
            unsigned int MMS_TimeOfExpiry; //������Ч��
            std::string MMS_SenderAddress;

            OTA_TYPE OTAType;

            //MMS::MMSWarp::MMS_NotificationRequest* pMMS_Notify;
            //OTA_NotificationRequest* pOTA_Notify;
        } SM_PARAM; 

        // ��ȡӦ��Ļ����� 
        typedef struct {
            int len; 
            char* data;
        } SM_BUFF; 

        typedef struct {
            int len;
            char buff[1024];
        }SMS_BUFF;

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
        int gsmEncodePdu_(const SM_PARAM* pSrc, SMS_BUFF* pDst);

        //BOOL gsmInit(); 

        int gsmSendMessage(SM_PARAM* pSrc, SM_BUFF* pBuff); 
        int gsmReadMessageList(SM_BUFF* pBuff); 
        int gsmDeleteMessage(int index, SM_BUFF* pBuff); 

        int gsmGetResponse(SM_BUFF* pBuff); 

        int gsmParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff); 
        //////////////////////////////////////////////////////////////////////////
        int m_nSendIn;      // ���Ͷ��е�����ָ�� 
        int m_nSendOut;     // ���Ͷ��е����ָ�� 

        int m_nRecvIn;      // ���ն��е�����ָ�� 
        int m_nRecvOut;     // ���ն��е����ָ�� 

        SM_PARAM m_SmSend[MAX_SM_SEND];     // ���Ͷ���Ϣ���� 
        SM_PARAM m_SmRecv[MAX_SM_SEND];     // ���ն���Ϣ���� 

        CRITICAL_SECTION m_csSend;      // �뷢����ص��ٽ�� 
        CRITICAL_SECTION m_csRecv;      // �������ص��ٽ�� 

        HANDLE m_hKillThreadEvent;      // ֪ͨ���̹߳رյ��¼� 
        HANDLE m_hThreadKilledEvent;    // ���߳�����رյ��¼� 

        void PutSendMessage(SM_PARAM* pSmParam);    // ������Ϣ���뷢�Ͷ��� 
        BOOL GetSendMessage(SM_PARAM* pSmParam);    // �ӷ��Ͷ�����ȡһ������Ϣ 
        void PutRecvMessage(SM_PARAM* pSmParam, int nCount);    // ������Ϣ������ն��� 
        BOOL GetRecvMessage(SM_PARAM* pSmParam);    // �ӽ��ն�����ȡһ������Ϣ 

        static UINT SmThread(LPVOID lpParam);   // ����Ϣ�շ��������߳� 

        void Send(std::string number, std::string content);
        void SetCenterAddress();
        std::wstring ToUnicode(std::string content);
        void MapMessage(SM_PARAM* sms);//��ʽת��
        void ReceiveCallback(SMS_TYPE type, void* msg);
        void SendCallback(bool send, SM_PARAM* sm);//���ͽ��֪ͨAPP
    };
}


#endif // SMSWarp_h
