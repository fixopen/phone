#ifndef __TELEPHONE_LOGICAL_TELEPHONEWARP_H__
#define __TELEPHONE_LOGICAL_TELEPHONEWARP_H__

#include "../../Util/fsm.h"
#include "../../Util/ATCommandWarp.h"

namespace Telephone {
    namespace Logical {
        class TelephoneWarp : public Util::FSM<TelephoneWarp> {
        public:
            enum State {
                sIdle,
                sPickuping,
                sDialing,
                sConnecting,
                sRecording,
                sRemoteBusy,
                sIncalling,
            };
            enum Event {
                ePickup,
                eHangup,
                eDial,
                eStartRecord,
                eStopRecord,
                eLeaveWord,
                eReject,
                eTimeout,
                eError,
                eRemotePickup,
                eRemoteHangup,
                eRemoteCall,
                eRemoteReject,
                eRemoteBusy,
            };
            TelephoneWarp(Util::ATCommand* atChannel);
            void Dial(std::string const& number);
		    void Hangup();
		    void Answer();
            void Reject();
		    void Redial();
		    void SubDial(char const number);
        protected:
            void showDialpad(void* p);
            void waitRemoteAnswer(void* p);
            void hideDialpadAndShowCallController(void* p);
            void startRecord(void* p);
            void stopRecordAndSaveFile(void* p);
            void hideCallControllerAndRecordHistory(void* p);
            void showRemoteCallAndRecordCallId(void* p);
            void showCallController(void* p);
            void recordHistory(void* p);
            void hideCallController(void* p);
            void hideDialpad(void* p);
            void finishWaitRemoteAnswer(void* p);
            void hideRemoteCallAndRecordHistory(void* p);
            void stopRecordAndSaveFileAndHideCallControllerAndRecordHistory(void* p);
        private:
            static void telephoneParser(std::string const& data);
            void showDialpad_(void* p);
            void waitRemoteAnswer_(void* p);
            void hideDialpad_(void* p);
            void showCallController_(void* p);
            void startRecord_(void* p);
            void stopRecord_(void* p);
            void saveFile_(void* p);
            void hideCallController_(void* p);
            void recordHistory_(void* p);
            void showRemoteCall_(void* p);
            void recordCallId_(void* p);
            void finishWaitRemoteAnswer_(void* p);
            void hideRemoteCall_(void* p);
        private:
            Util::ATCommand* at_;
        };
    }
}

#define MAX_PHONE_SEND		128		// 发送队列长度 
#define MAX_PHONE_RECV		128		// 接收队列长度 

namespace Telephone
{
	class TelephoneWarp 
	{
	 public:
		 TelephoneWarp();
		 TelephoneWarp(const TelephoneWarp& tel);

		 ~TelephoneWarp();

		 static TelephoneWarp* GetTelephoneWarp();
		 void Bind(Util::ATCommandWarp* at);
		 typedef struct
		 {
			 char NUM[16];
			 int TYPE;
		 }TEL_NUM;

	private:
		Util::ATCommandWarp* m_pAT;
	private:
		int m_nSendIn;		// 发送队列的输入指针 
		int m_nSendOut;		// 发送队列的输出指针 

		int m_nRecvIn;		// 接收队列的输入指针 
		int m_nRecvOut;		// 接收队列的输出指针 

		TEL_NUM m_PhoneSend[MAX_PHONE_SEND];		// 发送短消息队列 
		TEL_NUM m_PhoneRecv[MAX_PHONE_RECV];		// 接收短消息队列 

		CRITICAL_SECTION m_csSend;		// 与发送相关的临界段 
		CRITICAL_SECTION m_csRecv;		// 与接收相关的临界段 

		HANDLE m_hKillThreadEvent;		// 通知子线程关闭的事件 
		HANDLE m_hThreadKilledEvent;	// 子线程宣告关闭的事件 

		void PutSendMessage(TEL_NUM* pSmParam);	// 将短消息放入发送队列 
		BOOL GetSendMessage(TEL_NUM* pSmParam);	// 从发送队列中取一条短消息 
		void PutRecvMessage(TEL_NUM* pSmParam, int nCount);	// 将短消息放入接收队列 
		BOOL GetRecvMessage(TEL_NUM* pSmParam);	// 从接收队列中取一条短消息 

		static UINT TelephoneThread(LPVOID lpParam);	// 电话收发处理子线程

	private:
		//内部调用
		bool Dial_(TEL_NUM num);
		unsigned char State_(void);
		bool Hangup_(void);
		bool Answer_(void);
		bool Redial_();
		bool Incoming_(TEL_NUM* num);
		//回调
		void Incoming(char* num);
		void Ring(void);
		void Connected(void);
		void Held(void);
		void Dialing(void);
		void Alerting(void);
		void Waiting(void);
		void NoDialTone(void);
		void Busy(void);
		void NoAnswer(void);
		void NoCarrier(void);
		void Connect(void);
		void Congestion(void);
		void OppHangup(void);
		void Odb(void);

	public:
		//外部调用
		bool Dial(char* number);
		bool Hangup(void);
		bool Answer(void);
		bool Redial(void);
		bool SubDial(char number);
	};

}

#endif // __TELEPHONE_LOGICAL_TELEPHONEWARP_H__
