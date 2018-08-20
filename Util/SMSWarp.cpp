#include "stdafx.h"
#include "SMSWarp.h"
#include "../Data/Message.h"
#include "../Data/MmsMessage.h"
#include "../Data/OtaMessage.h"
#include "../Multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "../UTIL_/ATCommandWarp_.h"
#undef atoi
using namespace SMS;

extern VOID WriteMyLog_(char *ptr, int size);
extern int g_SignalSize;
BOOL g_isUCS2 = FALSE;
extern std::string g_sRecipientAddress;


//״̬
enum {   
	stBeginRest,                // ��ʼ��Ϣ/��ʱ    
		stContinueRest,             // ������Ϣ/��ʱ    
		stSendMessageRequest,       // ���Ͷ���Ϣ    
		stSendMessageResponse,      // ��ȡ����Ӧ�𵽻�����    
		stReadMessageRequest,       // ���Ͷ�ȡ����Ϣ�б������    
		stReadMessageResponse,      // ��ȡ����Ϣ�б�������    
		stDeleteMessageRequest,     // ɾ������Ϣ    
		stExitThread                // �˳�    
	} nState;

int FindUCS2Count(char *buf, int maxLineCount, int *pLineCount)
{
	int i = 0;
	unsigned char *ptr = (unsigned char *)buf;
	int jcount = 0;
	int ncount = 0;
	int maxCount = maxLineCount;
	pLineCount[i++] = jcount;
	while(*ptr++ != '\0')
	{
		jcount++;
	}
	if(jcount < 0x46)
	{
		maxCount = 0x46;
	}
	jcount = 0;
	ptr = (unsigned char *)buf;
	while(*ptr != '\0')
	{
		if(*ptr >= 0xA0)  //����
		{
			ptr++;
			ncount++;
		}
		ptr++;
		jcount++;
		ncount++;
		if(jcount >= maxCount)
		{
			pLineCount[i] = ncount;
			i++;
			jcount = 0;
		}
	}
	if(jcount != 0)
	{
		pLineCount[i] = ncount;
		i++;
	}
	
	return (i-1);
}

int Find7BITCount(char *buf, int maxLineCount, int *pLineCount)
{
	int i = 0;
	unsigned char *ptr = (unsigned char *)buf;
	int jcount = 0;
	int ncount = 0;
	int maxCount = maxLineCount;
	pLineCount[i++] = jcount;
	while(*ptr++ != '\0')
	{
		jcount++;
	}
	if(jcount <= 0xA0)
	{
		maxCount = 0xA0;
	}
	jcount = 0;
	ptr = (unsigned char *)buf;
	while(*ptr != '\0')
	{
		ptr++;
		jcount++;
		ncount++;
		if(jcount >= maxCount)
		{
			pLineCount[i] = ncount;
			i++;
			jcount = 0;
		}
	}
	if(jcount != 0)
	{
		pLineCount[i] = ncount;
		i++;
	}
	
	return (i-1);
}

void FindTP_DCS(const  char *pSrc)
{
	g_isUCS2 = FALSE;
	unsigned char *buf = (unsigned char*)pSrc;
	while(*buf != '\0')
	{
		if(*buf >= 0xA0)
		{
			g_isUCS2 = TRUE;
			break;
		}
		buf++;
	}
}

SMSWarp::SMSWarp():Util::Observer()
{
	m_nSendIn = 0;   
	m_nSendOut = 0;   
	m_nRecvIn = 0;   
	m_nRecvOut = 0;
	m_sendCount = 0;

	m_hKillThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);   
	m_hThreadKilledEvent = CreateEvent(NULL, TRUE, FALSE, NULL);   
	
	InitializeCriticalSection(&m_csSend);   
	InitializeCriticalSection(&m_csRecv); 
	InitializeCriticalSection(&m_csCMGL);

	m_pbuff.data = (char*)malloc(16384);
	
}
SMSWarp::~SMSWarp()
{
	SetEvent(m_hKillThreadEvent);           // �����ر����̵߳��ź�    
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);    // �ȴ����̹߳ر�    

	DeleteCriticalSection(&m_csSend);   
	DeleteCriticalSection(&m_csRecv);
	DeleteCriticalSection(&m_csCMGL);

	CloseHandle(m_hKillThreadEvent);   
	CloseHandle(m_hThreadKilledEvent);  
	
	m_bReadyWrite2sim = 0;
}
SMSWarp::SMSWarp(const SMSWarp& smsWarp)
{

}
SMSWarp* SMSWarp::GetSMSWarp()
{
	static SMSWarp* sms = 0;
	if (sms == 0)
	{
		sms = new SMSWarp();

	//	AfxBeginThread(SmThread, /*this*/sms, THREAD_PRIORITY_NORMAL);   
	}
	return sms;
}
void SMSWarp::Bind_(Util::ATCommandWarp* at)
{
	m_pAT = at;
}
void SMSWarp::Bind(Util::ATCommandWarp* at)
{
	m_pAT = at;
	// �������߳�  
	AfxBeginThread(SmThread, this, THREAD_PRIORITY_NORMAL);
	
}

void SMSWarp::Update(std::string const& data)
{	
	ParseCMGS(data);
	SendSmsPdu(data);
	ParseCMGL(data);
	ParseCMGW(data);
	ParseCMS_ERROR(data);
}

void SMSWarp::SendMessage()
{	
	Dprintf("Gsm SendMessageBegin\r\n");
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nPduLength;     // PDU������    
	unsigned char nSmscLength;  // SMSC������    

	nPduLength = m_smsPdu[m_nHaveSend].len;
	strcat(m_smsPdu[m_nHaveSend].buff, "\x01a"); // ��Ctrl-Z����    
	gsmString2Bytes(m_smsPdu[m_nHaveSend].buff, &nSmscLength, 2);  // ȡPDU���е�SMSC��Ϣ����    
	nSmscLength++;//���ϳ����ֽڱ���    
	
	Dprintf("Gsm SendMessageMiddle\r\n");
	
	main->m_pATCommandWarp1->SmsSend(nPduLength / 2 - nSmscLength);

//	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_SEND_LENGTH,nPduLength / 2 - nSmscLength,0);

	Dprintf("Gsm SmsSendPdu\r\n");
	
//  Test 
//  	for (int i = 0 ; i < 28;i++)
//  	{
//  		main->m_pATCommandWarp1->SmsWrite2Sim(nPduLength/2 - nSmscLength,3);//����	
//  		Sleep(50);
//  		main->m_pATCommandWarp1->SmsWrite2Sim(m_smsPdu[m_nHaveSend].buff, strlen(m_smsPdu[m_nHaveSend].buff));//����
//  		Sleep(100);
//  	}
	
// 	main->m_pATCommandWarp1->SmsSend(m_smsPdu[m_nHaveSend].buff, strlen(m_smsPdu[m_nHaveSend].buff), NULL, 16384);	
// 	m_nHaveSend++;
// 	Dprintf("Gsm SmsSend\r\n");

	//Sleep(40);
	//m_pAT->SmsSend(m_smsPdu[m_nHaveSend].buff, strlen(m_smsPdu[m_nHaveSend].buff), NULL, 16384);
	//m_nHaveSend++;

}

void SMSWarp::WriteMessage2Sim()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nPduLength;     // PDU������    
	unsigned char nSmscLength;  // SMSC������    
	
	nPduLength = m_smsPdu[m_nHaveWrite].len;
//	strcat(m_smsPdu[m_nHaveWrite].buff, "\x01a"); // ��Ctrl-Z����    
	gsmString2Bytes(m_smsPdu[m_nHaveWrite].buff, &nSmscLength, 2);  // ȡPDU���е�SMSC��Ϣ����    
	nSmscLength++;//���ϳ����ֽڱ���    
	
	Dprintf("Gsm WriteMessageMessageMiddle\r\n");
	
	main->m_pATCommandWarp1->SmsWrite2Sim(nPduLength/2 - nSmscLength,3);//����	

	main->m_pATCommandWarp1->SmsWrite2Sim(m_smsPdu[m_nHaveWrite].buff, strlen(m_smsPdu[m_nHaveWrite].buff));//����
	m_nHaveWrite++;
}

void SMSWarp::SendSmsPdu(std::string const& data)
{
	// ���Ͷ�������
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
//	Dprintf("okStatus:%d",main->m_pATCommandWarp1->GetOkStatus());

	if (data.find(">") != std::string::npos &&
		main->m_pATCommandWarp1->GetOkStatus() == okSmsSend)
	{	
		
		static std::string d = "";
		d = m_smsPdu[m_nHaveSend].buff;
	//	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_SEND,(WPARAM)&d,0);
		m_pAT->SmsSend(m_smsPdu[m_nHaveSend].buff, strlen(m_smsPdu[m_nHaveSend].buff), NULL, 16384);		
		m_nHaveSend++;
		Dprintf("Gsm SmsSend\r\n");	

		extern VOID WriteMyLog_(char *ptr, int size);
		WriteMyLog_("Gsm SmsSend",strlen("Gsm SmsSend"));
		
		/*if(m_bReadyWrite2sim == 2)
		{
			m_bReadyWrite2sim = 3;
			static std::string d = "";
			d = m_smsPdu[m_nHaveSend-1].buff;
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_WRITE_TO_SIM,(WPARAM)&d,0);
		}*/
	
	}

}

void SMSWarp::ParseCMGS(std::string const& data)
{
	if (data.find("+CMGS:") != std::string::npos)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		Dprintf("+CMGS: Begin\r\n");
		WritePduLength();
		
		Dprintf("+CMGS: Middle\r\n");

		if (m_nHaveSend != m_nSendNum)
		{	
			
			if (main->m_pMessageSet->SmsSendSaveSim())//
			{
			//	Sleep(50);
			}
			SendMessage();
		}
		else 
		{	
			Dprintf("DataCopy\r\n");
			Dprintf("Data:\r\n");
			Dprintf("%s\r\n",data.c_str());

			main->m_pATCommandWarp1->SetOkStatus(okIdle);
			
			memcpy(m_pbuff.data,(char *)data.c_str(),data.size());
			m_bParseCMS = true;
			m_nHaveSend = 0 ;
			m_nSendNum = 0 ;
			
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CSCS,0,0);

		}	
	}
}

void SMSWarp::ParseCMS_ERROR(std::string const& data)
{	
	if (data.find("+CMS ERROR: 332") != std::string::npos)//���糬ʱ
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		Dprintf("send error");
		Sim2Local();

		main->m_pATCommandWarp1->SetOkStatus(okIdle);
		memcpy(m_pbuff.data,(char *)data.c_str(),data.size());
		m_bParseCMS = true;
		m_nHaveSend = 0 ;
		m_nSendNum = 0 ;
		m_bReadyWrite2sim = 0;
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CSCS,0,0);

	}
	else if (data.find("+CMS ERROR: 322") != std::string::npos)
	{	
		Sim2Local();
		::PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_FULL,0,0);
	}
	else if (data.find("+CMS ERROR") != std::string::npos)
	{	
		
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		Dprintf("send error");
		if (main->m_pATCommandWarp1->GetOkStatus() == okSmsSend)
		{
			Sim2Local();
			main->m_pATCommandWarp1->SetOkStatus(okIdle);
			memcpy(m_pbuff.data,(char *)data.c_str(),data.size());
			m_bParseCMS = true;
			m_nHaveSend = 0 ;
			m_nSendNum = 0 ;
			
			m_bReadyWrite2sim = 0;
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CSCS,0,0);
		}
		
	}
	else if (data.find("+CME ERROR:") != std::string::npos)
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		Dprintf("send error");
		if (main->m_pATCommandWarp1->GetOkStatus() == okSmsSend)
		{
			main->m_pATCommandWarp1->SetOkStatus(okIdle);
			memcpy(m_pbuff.data,(char *)data.c_str(),data.size());
			m_bParseCMS = true;
			m_nHaveSend = 0 ;
			m_nSendNum = 0 ;
			m_bReadyWrite2sim = 0;
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CSCS,0,0);
			
		}
	}
	
}

void SMSWarp::Sim2Local()
{
	std::string filter;
	filter = "[reference] = 1 AND [uplevelProtocol] = 200";
	std::vector<boost::shared_ptr<Data::Message> >pMessage;
	pMessage = Data::Message::GetFromDatabase(filter);
	
	if (!pMessage.empty())
	{
		pMessage[0]->uplevelProtocol = 200;
		pMessage[0]->reference = 0;
		pMessage[0]->Update();
	}
}

void SMSWarp::HandleError()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	Dprintf("send error");
	if (main->m_pATCommandWarp1->GetOkStatus() == okSmsSend)
	{
		main->m_pATCommandWarp1->SetOkStatus(okIdle);
		memcpy(m_pbuff.data,0,16384);
		m_bParseCMS = true;
		m_nHaveSend = 0 ;
		m_nSendNum = 0 ;
		m_bReadyWrite2sim = 0;
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CSCS,0,0);		
	}

}

void SMSWarp::ParseOK(std::string const& data)
{

}

void SMSWarp::ParseCMGW(std::string const& data)
{
	if (data.find("+CMGW:") != std::string::npos )
	{	
		// add by qi 20100407
		extern g_isUCS2;

		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		if (main->m_pMessageSet->SmsSendSaveSim())//�ѷ��͵���Ϣ���浽SIM��
		{	
			static int haveSend = 0;
			int lineCount[24];
			int total;
			if (!g_isUCS2)
			{
				total = Find7BITCount((char *)m_smPa.TP_UD, 152, &lineCount[0]);
			}
			else 
			total = FindUCS2Count(m_smPa.TP_UD,67,&lineCount[0]);

			if ( 0 == strlen(m_smPa.TP_UD))
			{
				total = 1;
			}
			
			std::string s = data ;
			size_t t = s.find("+CMGW:");
			s = s.substr(t+7);
			
			int index = atoi(s.c_str());
			
			if ( 1 == total)
			{
				std::string filter;
				filter = "[reference] = 1 AND [uplevelProtocol] = 200";
				std::vector<boost::shared_ptr<Data::Message> >pMessage;
				pMessage = Data::Message::GetFromDatabase(filter);
				
				if (index >= 0)//���ͳɹ�
				{	
					if (!pMessage.empty())
					{
						pMessage[0]->uplevelProtocol = index;
						pMessage[0]->Update();
					}
				}
				else 
				{
					if (!pMessage.empty())
					{
						pMessage[0]->uplevelProtocol = 200;
						pMessage[0]->reference = 0;
						pMessage[0]->Update();

					}
				}
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SIM_STORAGE,0,0);

			}
			else //������
			{
				//��ֳɼ����洢
				Data::Message* msg = new Data::Message; 
				char data[256];
				memset(data,0,256);
				memcpy(data, m_smPa.TP_UD+lineCount[haveSend], lineCount[haveSend+1] - lineCount[haveSend]);
				
				msg->unicodeData = data;
				msg->remote.address = m_smPa.TPA;
				
				SYSTEMTIME tm;
				GetLocalTime(&tm);
				msg->timestamp.year = tm.wYear%100;
				msg->timestamp.month = tm.wMonth;
				msg->timestamp.day = tm.wDay;
				msg->timestamp.hour = tm.wHour;
				msg->timestamp.minite = tm.wMinute;
				msg->timestamp.second = tm.wSecond;
				msg->group = Data::Message::gSend;
				
				msg->reference = 1;
				msg->uplevelProtocol = index ;
				msg->Insert();
				
				haveSend++;
				if (haveSend == total)
				{
					haveSend = 0 ;
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SIM_STORAGE,0,0);

				}
				delete msg ;
				
			}


		}
	}
}

void SMSWarp::ParseCMGL(std::string const& data)
{	
	/*
		+CMGL: 2,1,,163
	*/
	
// 	int  static size = 0; 
// 	if (data.find("+CMGL: ") != std::string::npos)
// 	{	
// 		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
// 		
// 		extern VOID WriteLog(CHAR* str);
// 		WriteLog((char *)data.c_str());
// 		
// 		memcpy(m_pbuff.data,data.c_str(),data.size());
// 		m_bParseCMGL = true ; 
// 		
// 	}

	int  static size = 0; 
	if (data.find("+CMGL: ") != std::string::npos)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		
		extern VOID WriteLog(CHAR* str);
		WriteLog((char *)data.c_str());
		
//		EnterCriticalSection(&m_csCMGL);
		SM_BUFF buff ;
		buff.data = new char[data.size() +1];
		memset(buff.data,0,data.size() + 1);
		if (buff.data != NULL)
		{
			memcpy(buff.data,data.c_str(),data.size());
			ReadMessage(&buff);
			delete [](char *)(buff.data);
			buff.data = NULL;
		}
//		LeaveCriticalSection(&m_csCMGL);
	}
}

void SMSWarp::ParseCMGL_(char const * data)
{
	std::string s = data;
	int smstype = -1;
	while (s.find("CMGL:") != std::string::npos )
	{  
		SM_PARAM *sm_p = new SM_PARAM();
		std::string pdu ;
		if (sm_p == NULL)
		{
			return ;
		}
		
		int index ;
		int sta;
		size_t t = s.find("CMGL:");
		s = s.substr(t+6);
		index = atoi(s.c_str());
		
		t = s.find(",");
		s = s.substr(t+1);
		sta = atoi(s.c_str());//״̬��Messages 
		
		t = s.find("\r\n");
		
		if ( t != std::string::npos)
		{	
			s = s.substr(t+2);
			
			t = s.find("\r\n");
			pdu = s.substr(0,t);//��ȡһ��������pdu��
						
			if ( 2 == sta || 3 == sta)
			{
				gsmDecodePdu(pdu.c_str(),sm_p,1);
				smstype = 1;
			}
			else
			{
				smstype = 0 ;
				gsmDecodePdu(pdu.c_str(),sm_p);
			}
			
		}
		Dprintf("Insert sms Begin\r\n");
		InsertSms2DB(sm_p,smstype,index);//1 ,send 0 ,recive

		delete sm_p ;
		sm_p = NULL;
	}
}

std::string SMSWarp::ReplaceSingleQuotation(std::string& field)
{
	for (int i = 0; i < field.length(); ++i)
	{
		if (field[i] == '\'')
		{
			field[i] = '\"';
		}
	}
	return field;
}

void SMSWarp::InsertSms2DB(SM_PARAM *sm,int smstype,int index)
{
	if (sm->Type == mtSMS)
	{   
		Dprintf("Insert sms mtSMS\r\n");
		
		Data::Message* msg = new Data::Message;
		
		std::string sca(sm->SCA);
		msg->smsc.address = sca;
		msg->smsc.addressLength = sca.size();
		msg->smsc.addressType.typeOfNumber = Data::AddressType::tInternationNumber;
		
		msg->flag.type = Data::Flag::tDeliver;
		
		std::string num(sm->TPA);
		msg->remote.address = num;
		msg->remote.addressLength = num.size();
		msg->remote.addressType.typeOfNumber = Data::AddressType::tNationNumber;
		
		msg->encoding.encoding = Data::Encoding::UnicodeCodeSet;
		
		if ( 0 == smstype)//����
		{
			msg->timestamp.year = atoi(std::string(&sm->TP_SCTS[0],2).c_str());
			msg->timestamp.month = atoi(std::string(&sm->TP_SCTS[2],2).c_str());
			msg->timestamp.day = atoi(std::string(&sm->TP_SCTS[4],2).c_str());
			msg->timestamp.hour = atoi(std::string(&sm->TP_SCTS[6],2).c_str());
			msg->timestamp.minite = atoi(std::string(&sm->TP_SCTS[8],2).c_str());
			msg->timestamp.second = atoi(std::string(&sm->TP_SCTS[10],2).c_str());
		}
		else
		{
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			msg->timestamp.year = tm.wYear%100;
			msg->timestamp.month = tm.wMonth;
			msg->timestamp.day = tm.wDay;
			msg->timestamp.hour = tm.wHour;
			msg->timestamp.minite = tm.wMinute;
			msg->timestamp.second = tm.wSecond;
			
		}
		
		//msg->unicodeData = sm->TP_UD;
		std::string st_ = sm->TP_UD;		
		msg->unicodeData = ReplaceSingleQuotation(st_);
		
		msg->state = Data::Message::sReaded;
		
		if ( 0 == smstype )//recive
		{
			msg->group = Data::Message::gReceive;
		}
		else //send
		{
			msg->group = Data::Message::gSend;
		}
		
		msg->reference = 1;//����SIM����
		msg->uplevelProtocol = index ;// ���� uplevelProtocol ��ʾ sms �ڶ����е�index
		
		msg->total = sm->Total;
		if (sm->Total > 1)
		{
			msg->transactionId = sm->Serial;
			msg->total = sm->Total;
			msg->no = sm->Number;
			//msg->group = Data::Message::gExternSms; //��ͥ����ͨ�ĳ�������
		}
		msg->Insert();
		// CombineData(msg); �Ȳ��ϲ�
		delete msg;
	}
	else if(sm->Type == mtMMS)//����SIM��Ķ�Ϣ֪ͨ
	{		
		Dprintf("Insert sms mtMMS\r\n");
		
		Data::Message* msg = new Data::Message;
		std::string num(sm->TPA);
		msg->remote.address = num;
		msg->remote.addressLength = num.size();
		
		msg->state = Data::Message::sReaded;
		msg->group = Data::Message::gReceive;
		msg->unicodeData = "application/vnd.wap.mms-message";
		
		msg->reference = 1;
		msg->uplevelProtocol = index ;
		msg->Insert();
		delete msg;
		msg = NULL;
	}
	
}

void SMSWarp::WritePduLength()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if (main->m_pMessageSet->SmsSendSaveSim())//������Ϣ���浽SIM
	{
		int nPduLength;     // PDU������    
		unsigned char nSmscLength;  // SMSC������    	
		Dprintf("m_nHaveSend :%d\r\n",m_nHaveSend);

		nPduLength = m_smsPdu[m_nHaveSend-1].len;
//		strcat(m_smsPdu[m_nHaveSend-1].buff, "\x01a"); // ��Ctrl-Z����    
		gsmString2Bytes(m_smsPdu[m_nHaveSend-1].buff, &nSmscLength, 2);  // ȡPDU���е�SMSC��Ϣ����    
		nSmscLength++;//���ϳ����ֽڱ��� 
		
		//change by qi 20100629
		//PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_WRITE_LENGTH,nPduLength/2 - nSmscLength,0);
	
		//static std::string d = "";
		//d = m_smsPdu[m_nHaveSend-1].buff;
		//PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_WRITE_TO_SIM,(WPARAM)&d,0);
		
		main->m_pATCommandWarp1->SmsWrite2Sim(nPduLength/2 - nSmscLength,3);//����				
		main->m_pATCommandWarp1->SmsWrite2Sim(m_smsPdu[m_nHaveSend-1].buff, strlen(m_smsPdu[m_nHaveSend-1].buff));//����
		
	}

}

void SMSWarp::WritePduData(std::string const& data)
{	
	//��SIM����д����������
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if (data.find(">") != std::string::npos &&
		main->m_pATCommandWarp1->GetOkStatus() == okSmsWrite)
	{	
		Dprintf("Gsm WritePduData\r\n");
		main->m_pATCommandWarp1->SetOkStatus(okIdle);
		m_pAT->SmsWrite2Sim(m_smsPdu[m_nHaveSend].buff, strlen(m_smsPdu[m_nHaveSend].buff));
		
		Dprintf("Gsm WritePduData End\r\n");
	}	
}

void SMSWarp::Register()
{
	Util::ATCommand::Instance()->RegisterObserver(this);
}

void SMSWarp::SetOTANumber(std::string number)
{
	m_strOTANumber = number;
}
// �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����    
// �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ������ָ��    
// ����: Ŀ�����ݳ���    
int SMSWarp::gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
	for (int i = 0; i < nSrcLength; i += 2)   
	{   
		// �����4λ    
		if ((*pSrc >= '0') && (*pSrc <= '9'))   
		{   
			*pDst = (*pSrc - '0') << 4;   
		}   
		else   
		{   
			*pDst = (*pSrc - 'A' + 10) << 4;   
		}   

		pSrc++;   

		// �����4λ    
		if ((*pSrc>='0') && (*pSrc<='9'))   
		{   
			*pDst |= *pSrc - '0';   
		}   
		else   
		{   
			*pDst |= *pSrc - 'A' + 10;   
		}   

		pSrc++;   
		pDst++;   
	}   

	// ����Ŀ�����ݳ���    
	return (nSrcLength / 2);   
}   

// �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���    
// �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"     
// ����: pSrc - Դ����ָ��    
//       nSrcLength - Դ���ݳ���    
// ���: pDst - Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)   
{   
	const char tab[]="0123456789ABCDEF";    // 0x0-0xf���ַ����ұ�    

	for (int i = 0; i < nSrcLength; i++)   
	{   
		*pDst++ = tab[*pSrc >> 4];        // �����4λ    
		*pDst++ = tab[*pSrc & 0x0f];    // �����4λ    
		pSrc++;   
	}   

	// ����ַ����Ӹ�������    
	*pDst = '\0';   

	// ����Ŀ���ַ�������    
	return (nSrcLength * 2);   
}   

// 7bit����    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ����봮ָ��    
// ����: Ŀ����봮����    
int SMSWarp::gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
	int nSrc;       // Դ�ַ����ļ���ֵ    
	int nDst;       // Ŀ����봮�ļ���ֵ    
	int nChar;      // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7    
	unsigned char nLeft;    // ��һ�ֽڲ��������    

	// ����ֵ��ʼ��    
	nSrc = 0;
	nDst = 0;  

	// ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�    
	// ѭ���ô�����̣�ֱ��Դ����������    
	// ������鲻��8�ֽڣ�Ҳ����ȷ����    
	while (nSrc < nSrcLength)   
	{   
		// ȡԴ�ַ����ļ���ֵ�����3λ    
		nChar = nSrc & 7;   

		// ����Դ����ÿ���ֽ�    
		if(nChar == 0)   
		{   
			// ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��    
			nLeft = *pSrc;   
		}   
		else   
		{   
			// ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�    
			*pDst = (*pSrc << (8-nChar)) | nLeft;   

			// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������    
			nLeft = *pSrc >> nChar;   

			// �޸�Ŀ�괮��ָ��ͼ���ֵ    
			pDst++;   
			nDst++;   
		}   

		// �޸�Դ����ָ��ͼ���ֵ    
		pSrc++;   
		nSrc++;   
	}   

	// ����Ŀ�괮����    
	return nDst;   
}

int SMSWarp::gsmEncodeLong7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
	int nSrc;       // Դ�ַ����ļ���ֵ    
	int nDst;       // Ŀ����봮�ļ���ֵ    
	int nChar;      // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7    
	unsigned char nLeft;    // ��һ�ֽڲ��������    

	// ����ֵ��ʼ��    
	nSrc = 0;   
	nDst = 0;   

	*pDst++ = *pSrc++ << 1;
	nDst++;

	// ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�    
	// ѭ���ô�����̣�ֱ��Դ����������    
	// ������鲻��8�ֽڣ�Ҳ����ȷ����    
	while (nSrc < nSrcLength - 1)   
	{   
		// ȡԴ�ַ����ļ���ֵ�����3λ    
		nChar = nSrc & 7;   

		// ����Դ����ÿ���ֽ�    
		if(nChar == 0)   
		{   
			// ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��    
			nLeft = *pSrc;   
		}   
		else   
		{
			// ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�    
			*pDst = (*pSrc << (8-nChar)) | nLeft;   

			// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������    
			nLeft = *pSrc >> nChar;   

			// �޸�Ŀ�괮��ָ��ͼ���ֵ    
			pDst++;   
			nDst++;   
		}   

		// �޸�Դ����ָ��ͼ���ֵ    
		pSrc++;   
		nSrc++;   
	}   

	// ����Ŀ�괮����  
	return nDst;   
} 

// 7bit����    
// ����: pSrc - Դ���봮ָ��    
//       nSrcLength - Դ���봮����    
// ���: pDst - Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength) {
	int nSrc; // Դ�ַ����ļ���ֵ
	int nDst; // Ŀ����봮�ļ���ֵ
	int nByte; // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
	unsigned char nLeft; // ��һ�ֽڲ��������
	// ����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;
	// �����ֽ���źͲ������ݳ�ʼ��
	nByte = 0;
	nLeft = 0;
	// ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
	// ѭ���ô�����̣�ֱ��Դ���ݱ�������
	// ������鲻��7�ֽڣ�Ҳ����ȷ����
	while(nSrc<nSrcLength) {
		// ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
		// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
		nLeft = *pSrc >> (7-nByte);
		// �޸�Ŀ�괮��ָ��ͼ���ֵ
		pDst++;
		nDst++;
		// �޸��ֽڼ���ֵ
		nByte++;
		// ����һ������һ���ֽ�
		if(nByte == 7) {
			// ����õ�һ��Ŀ������ֽ�
			*pDst = nLeft;
			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;
			// �����ֽ���źͲ������ݳ�ʼ��
			nByte = 0;
			nLeft = 0;
		}
		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}
	*pDst = 0;
	// ����Ŀ�괮����
	return nDst;
}

//7bit ������
int SMSWarp::gsmDecodeLong7bit(const unsigned char* pSrc, char* pDst, int nSrcLength) {
	int nSrc; // Դ�ַ����ļ���ֵ
	int nDst; // Ŀ����봮�ļ���ֵ
	int nByte; // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
	unsigned char nLeft; // ��һ�ֽڲ��������
	// ����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;
	// �����ֽ���źͲ������ݳ�ʼ��
	nByte = 0;
	nLeft = 0;
	
	*pDst++ = *pSrc++ >> 1;
	nDst++;

	// ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
	// ѭ���ô�����̣�ֱ��Դ���ݱ�������
	// ������鲻��7�ֽڣ�Ҳ����ȷ����
	while(nSrc < nSrcLength-1) {
		// ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
		// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
		nLeft = *pSrc >> (7-nByte);
		// �޸�Ŀ�괮��ָ��ͼ���ֵ
		pDst++;
		nDst++;
		// �޸��ֽڼ���ֵ
		nByte++;
		// ����һ������һ���ֽ�
		if(nByte == 7) {
			// ����õ�һ��Ŀ������ֽ�
			*pDst = nLeft;
			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;
			// �����ֽ���źͲ������ݳ�ʼ��
			nByte = 0;
			nLeft = 0;
		}
		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}
	*pDst = 0;
	// ����Ŀ�괮����
	return nDst;
}

// 8bit����    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ����봮ָ��    
// ����: Ŀ����봮����    
int SMSWarp::gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
	// �򵥸���    
	memcpy(pDst, pSrc, nSrcLength);   

	return nSrcLength;   
}   

// 8bit����    
// ����: pSrc - Դ���봮ָ��    
//       nSrcLength -  Դ���봮����    
// ���: pDst -  Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)   
{   
	// �򵥸���    
	memcpy(pDst, pSrc, nSrcLength);   

	// ����ַ����Ӹ�������    
	pDst[nSrcLength] = '\0';   

	return nSrcLength;   
}   

// UCS2����    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ����봮ָ��    
// ����: Ŀ����봮����    
int SMSWarp::gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength) {
	int nDstLength; // UNICODE���ַ���Ŀ
	WCHAR wchar[256] = {0}; // UNICODE��������
	// �ַ���-->UNICODE��
	nDstLength = ::MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 256);
	// �ߵ��ֽڶԵ������
	for(int i=0; i<nDstLength; i++) {
		// �������λ�ֽ�
		*pDst++ = wchar[i] >> 8;
		// �������λ�ֽ�
		*pDst++ = wchar[i] & 0xff;
	}
	// ����Ŀ����봮����
	return nDstLength * 2;
}

// UCS2����    
// ����: pSrc - Դ���봮ָ��    
//       nSrcLength -  Դ���봮����    
// ���: pDst -  Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength) {
	int nDstLength;
	// UNICODE���ַ���Ŀ
	WCHAR wchar[128]; // UNICODE��������
	// �ߵ��ֽڶԵ���ƴ��UNICODE
	for(int i=0; i<nSrcLength/2; i++) {
		// �ȸ�λ�ֽ�
		wchar[i] = *pSrc++ << 8;
		// ���λ�ֽ�
		wchar[i] |= *pSrc++;
	}
	// UNICODE��-->�ַ���
	nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160,
		NULL, NULL);
	// ����ַ����Ӹ�������
	pDst[nDstLength] = '\0';
	// ����Ŀ���ַ�������
	return nDstLength;
}

// ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��    
// �磺"8613851872468" --> "683158812764F8"    
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������        	

int SMSWarp::gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength) 
{
	int nDstLength; // Ŀ���ַ�������
	char ch; // ���ڱ���һ���ַ�
	// ���ƴ�����
	nDstLength = nSrcLength;
	// �����ߵ�
	for(int i=0; i<nSrcLength;i+=2) {
		ch = *pSrc++; // �����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++; // ���ƺ���ֵ��ַ�
		*pDst++ = ch; // �����ȳ��ֵ��ַ�
	}
	// Դ��������������
	if(nSrcLength & 1) {
		*(pDst-2) = 'F'; // ��'F'
		nDstLength++; // Ŀ�괮���ȼ�1
	}
	*pDst = '\0'; // ����ַ����Ӹ�������
	return nDstLength; // ����Ŀ���ַ�������
}

// �����ߵ����ַ���ת��Ϊ����˳����ַ���
// �磺"683158812764F8" --> "8613851872468"   	
// ����: pSrc - Դ�ַ���ָ��    
//       nSrcLength - Դ�ַ�������    
// ���: pDst - Ŀ���ַ���ָ��    
// ����: Ŀ���ַ�������    
int SMSWarp::gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength) {
	int nDstLength; // Ŀ���ַ�������
	char ch; // ���ڱ���һ���ַ�
	// ���ƴ�����
	nDstLength = nSrcLength;
	// �����ߵ�
	for(int i=0; i<nSrcLength;i+=2) {
		ch = *pSrc++; // �����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++; // ���ƺ���ֵ��ַ�
		*pDst++ = ch; // �����ȳ��ֵ��ַ�
	}
	// �����ַ���'F'��
	if(*(pDst-1) == 'F') {
		pDst--;
		nDstLength--; // Ŀ���ַ������ȼ�1
	}
	*pDst = '\0'; // ����ַ����Ӹ�������
	return nDstLength; // ����Ŀ���ַ�������
}

// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// pSrc: ԴPDU����ָ��
// pDst: Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int SMSWarp::gsmEncodePdu_(const SM_PARAM* pSrc, SMS_BUFF* pDst_) {

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nLength; // �ڲ��õĴ�����
	int nDstLength; // Ŀ��PDU������
	unsigned char buf[256]; // �ڲ��õĻ�����

	char s_Mulit[512];
    char *ptr1 = (char *)pSrc->TP_UD;
	int nCount = 0;
	char pDst[512];

	int lineCount[24];
	int nTotal = 0;
	if(pSrc->TP_DCS == GSM_7BIT)
	{
		nTotal = Find7BITCount((char *)pSrc->TP_UD, 152, &lineCount[0]);
	}
	else
	{
		nTotal = FindUCS2Count((char *)pSrc->TP_UD, 67, &lineCount[0]);
	}
	
	if ( 0 == strlen(pSrc->TP_UD))
	{
		nTotal = 1;
	}

	if(nTotal > MAX_SM_SEND)
		nTotal = MAX_SM_SEND;

	unsigned char head[6];
// 	head[0] = 0x05;
// 	head[1] = 0x00;
// 	head[2] = 0x03;
// 	head[3] = rand()%255;

//  change by qi 20100514
	if (nTotal > 1)
	{
		static n = 1; 
		head[0] = 0x05;
		head[1] = 0x00;
		head[2] = 0x03;
		head[3] = n++%255;
		
		if ( n == 254)
		{
			n = 1;
		}
	}
// change over

	for(int i = 0; i < nTotal; i++)
	{
		memset(s_Mulit, 0, 512);

		// SMSC��ַ��Ϣ��
		nLength = strlen(pSrc->SCA); // SMSC��ַ�ַ����ĳ���
		buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;// SMSC��ַ��Ϣ����
		buf[1] = 0x91; // �̶�: �ù��ʸ�ʽ����
		nDstLength = gsmBytes2String(buf, pDst, 2); // ת��2���ֽڵ�Ŀ��PDU��
		// ת��SMSC��Ŀ��PDU��
		nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);
		// TPDU�λ���������Ŀ���ַ��
		nLength = strlen(pSrc->TPA); // TP-DA��ַ�ַ����ĳ���
		unsigned char report = 0;
		if(1 == main->m_pMessageSet->SmsReceipt())
		{
			report = 0x20;
		}

		if(nTotal == 1)
		{
			strcpy(s_Mulit, ptr1);
		
			buf[0] = 0x11+report; // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
		}
		else
		{
			memcpy(s_Mulit, ptr1+lineCount[i], lineCount[i+1] - lineCount[i]);
		
			buf[0] = 0x51+report; // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
			
			head[4] = nTotal;
			head[5] = i+1;
		
		}
		buf[1] = 0; // TP-MR=0
		buf[2] = (char)nLength; // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
		if ((pSrc->TPA[0] == '8') && (pSrc->TPA[1] == '6'))
		{
			buf[3] = 0x91; // �̶�: �ù��ʸ�ʽ����
		}
		else
		{
			buf[3] = 0xA1; // �ù��ڸ�ʽ����
		}
		nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//ת��4���ֽڵ�Ŀ��PDU��
		// ת��TP-DA��Ŀ��PDU��
		nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
		// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
		
		nLength = strlen(s_Mulit); // �û���Ϣ�ַ����ĳ���
		buf[0] = pSrc->TP_PID; // Э���ʶ(TP-PID)
		buf[1] = pSrc->TP_DCS; // �û���Ϣ���뷽ʽ(TP-DCS)
		
		if(1 == main->m_pMessageSet->SmsValidity())
		{
			buf[2] = 0x0B; // ��Ч��(TP-VP) 0Ϊ5���� 90Ϊ12Сʱ A7Ϊ24Сʱ
		}
		else if(2 == main->m_pMessageSet->SmsValidity())
		{
			buf[2] = 0xA7;
		}
		else if(3 == main->m_pMessageSet->SmsValidity())
		{
			buf[2] = 0xAD;
		}
		else if(4 == main->m_pMessageSet->SmsValidity())
		{
			buf[2] = 0xFF;
		}

		if(pSrc->TP_DCS == GSM_7BIT) {
			// 7-bit���뷽ʽ
			buf[3] = nLength; // ����ǰ����
			// ת��TP-DA��Ŀ��PDU��
			if(nTotal > 1)
			{
				nLength = gsmEncodeLong7bit(s_Mulit, &buf[4], nLength+1) + 4;
			}
			else
			{
				nLength = gsmEncode7bit(s_Mulit, &buf[4], nLength+1) + 4;
			}
		}
		else if (pSrc->TP_DCS == GSM_UCS2) {
			// UCS2���뷽ʽ
			// ת��TP-DA��Ŀ��PDU��
			buf[3] = gsmEncodeUcs2(s_Mulit, &buf[4], nLength);
			nLength = buf[3] + 4; // nLength���ڸö����ݳ���
		}
		else {
			// 8-bit���뷽ʽ
			// ת��TP-DA��Ŀ��PDU��
			buf[3] = gsmEncode8bit(s_Mulit, &buf[4], nLength);
			nLength = buf[3] + 4; // nLength���ڸö����ݳ���
		}
		// ת���ö����ݵ�Ŀ��PDU��
		// ת���ö����ݵ�Ŀ��PDU��
		//nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
		//return nDstLength; // ����Ŀ���ַ�������

		if(nTotal > 1)
		{
			char head_[25];
			if(pSrc->TP_DCS == GSM_7BIT)
			{
				buf[3] = buf[3] + 7;   //7bitʱͷ���ȼ�7
			}
			else
			{
				buf[3] = buf[3]+6;	//�����ų��ȼ���6
			}
			nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);
			sprintf(head_, "%02x%02x%02x%02x%02x%02x", head[0]&0xFF, head[1]&0xFF,head[2]&0xFF,head[3]&0xFF,head[4]&0xFF,head[5]&0xFF);
			strcpy(&pDst[nDstLength], head_);
			nDstLength += 12;
			nDstLength += gsmBytes2String(buf+4, &pDst[nDstLength], nLength-4);
			strcpy(pDst_[nCount].buff, pDst);
			pDst_[nCount].len = nDstLength;
		}
		else
		{
			nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
			pDst_[nCount].len = nDstLength;
			strcpy(pDst_[nCount].buff, pDst);
		}
		nCount++;
	}

	return nCount; // ����Ŀ���ַ�������
}

// PDU���룬���ڱ��ơ����Ͷ���Ϣ
// pSrc: ԴPDU����ָ��
// pDst: Ŀ��PDU��ָ��
// ����: Ŀ��PDU������
int SMSWarp::gsmEncodePdu(const SM_PARAM* pSrc, char* pDst) {
	int nLength; // �ڲ��õĴ�����
	int nDstLength; // Ŀ��PDU������
	unsigned char buf[256]; // �ڲ��õĻ�����
	// SMSC��ַ��Ϣ��
	nLength = strlen(pSrc->SCA); // SMSC��ַ�ַ����ĳ���
	buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;// SMSC��ַ��Ϣ����
	buf[1] = 0x91; // �̶�: �ù��ʸ�ʽ����
	nDstLength = gsmBytes2String(buf, pDst, 2); // ת��2���ֽڵ�Ŀ��PDU��
	// ת��SMSC��Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);
	// TPDU�λ���������Ŀ���ַ��
	nLength = strlen(pSrc->TPA); // TP-DA��ַ�ַ����ĳ���
	buf[0] = 0x11; // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
	buf[1] = 0; // TP-MR=0
	buf[2] = (char)nLength; // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
	if ((pSrc->TPA[0] == '8') && (pSrc->TPA[1] == '6'))
	{
		buf[3] = 0x91; // �̶�: �ù��ʸ�ʽ����
	}
	else
	{
		buf[3] = 0xA1; // �ù��ڸ�ʽ����
	}
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//ת��4���ֽڵ�Ŀ��PDU��
	// ת��TP-DA��Ŀ��PDU��
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	nLength = strlen(pSrc->TP_UD); // �û���Ϣ�ַ����ĳ���
	buf[0] = pSrc->TP_PID; // Э���ʶ(TP-PID)
	buf[1] = pSrc->TP_DCS; // �û���Ϣ���뷽ʽ(TP-DCS)
	buf[2] = 0xA7; // ��Ч��(TP-VP) 0Ϊ5���� 90Ϊ12Сʱ A7Ϊ24Сʱ
	if(pSrc->TP_DCS == GSM_7BIT) {
		// 7-bit���뷽ʽ
		buf[3] = nLength; // ����ǰ����
		// ת��TP-DA��Ŀ��PDU��
		nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;
	}
	else if (pSrc->TP_DCS == GSM_UCS2) {
		// UCS2���뷽ʽ
		// ת��TP-DA��Ŀ��PDU��
		buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);
		nLength = buf[3] + 4; // nLength���ڸö����ݳ���
	}
	else {
		// 8-bit���뷽ʽ
		// ת��TP-DA��Ŀ��PDU��
		buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);
		nLength = buf[3] + 4; // nLength���ڸö����ݳ���
	}
	// ת���ö����ݵ�Ŀ��PDU��
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
	return nDstLength; // ����Ŀ���ַ�������
}
// PDU���룬���ڽ��ա��Ķ�����Ϣ
// pSrc: ԴPDU��ָ��
// pDst: Ŀ��PDU����ָ��
// ����: �û���Ϣ������
int SMSWarp::gsmDecodePdu(const char* pSrc, SM_PARAM* pDst,int type) {
	int nDstLength; // Ŀ��PDU������
	unsigned char tmp; // �ڲ��õ���ʱ�ֽڱ���
	unsigned char buf[256]; // �ڲ��õĻ�����
	unsigned char udhi = 0; //�Ƿ�������ͷ
	unsigned char udh[32]; //����ͷ����

	// SMSC��ַ��Ϣ��
	gsmString2Bytes(pSrc, &tmp, 2); // ȡ����
	tmp = (tmp - 1) * 2; // SMSC���봮����
	pSrc += 4; // ָ�����
	gsmSerializeNumbers(pSrc, pDst->SCA, tmp); // ת��SMSC���뵽Ŀ��PDU��
	pSrc += tmp; // ָ�����
	// TPDU�λ����������ظ���ַ��
	gsmString2Bytes(pSrc, &tmp, 2); // ȡ��������
	
	// change by qi 20100407
	pSrc += 2;// ָ�����
	if ( 1 == type)
	{
		pSrc += 2;//����<mr>
	}

	udhi = tmp & 0x40;//�û�����ͷ��ʶ
	//	 if(tmp & 0x80) 
	{
		// �����ظ���ַ��ȡ�ظ���ַ��Ϣ
		gsmString2Bytes(pSrc, &tmp, 2); // ȡ����
		if(tmp & 1) tmp += 1; // ������ż��
		pSrc += 4; // ָ�����
		gsmSerializeNumbers(pSrc, pDst->TPA, tmp); // ȡTP-RA����
		pSrc += tmp; // ָ�����
	}
	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2); // ȡЭ���ʶ(TP-PID)
	pSrc += 2; // ָ�����
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);
	// ȡ���뷽ʽ(TP-DCS)
	pSrc += 2; // ָ�����
	//change by qi 20100407
	if ( 1 != type )// ����û��ʱ���
	{
		gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14); // ����ʱ����ַ���(TP_SCTS)
		pSrc += 14; // ָ�����
	}
	else
	{   
		//add by qi 20100401
		gsmString2Bytes(pSrc, &tmp, 2);
		if ( tmp != (strlen(pSrc+2)/2))
		{
			pSrc += 2; // ָ�����	
		}
	}
//	gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14); // ����ʱ����ַ���(TP_SCTS)
//	pSrc += 14; // ָ�����
	
	gsmString2Bytes(pSrc, &tmp, 2); // �û���Ϣ����(TP-UDL)
	pSrc += 2; // ָ�����

	pDst->Total = 1;
	pDst->Number = 1;
	if (udhi == 0x40)
	{
		unsigned char hl;
		gsmString2Bytes(pSrc, &hl, 2);
		if (hl > 0)
		{
			gsmString2Bytes(pSrc, udh, (hl+1) * 2);
			pSrc += 2;
			pSrc += hl * 2;

			//change by qi 20100628
			if (pDst->TP_DCS != GSM_7BIT)
			{
				tmp -= (hl + 1);
			}
			//old
			//tmp -= (hl + 1);

			if (udh[2] == 0x03)
			{
				gsmBytes2String(&udh[3], pDst->Serial , 1);
				pDst->Total = udh[4];
				pDst->Number = udh[5];
			}
			else if (udh[2] == 0x04)
			{
				gsmBytes2String(&udh[3], pDst->Serial, 2);
				pDst->Total = udh[5];
				pDst->Number = udh[6];
			}
		}
	}
	pDst->Type = mtSMS;
	
	//add by qi 20100513
	pDst->MMS_Type = 0x85;//���ǲ���֪ͨȫ��85

	if(pDst->TP_DCS == GSM_7BIT) 
	{
		// 7-bit����
		memset(buf , 0, 256);
		
		//change by qi 20100628
		int ntmp;
		tmp = tmp&7 ? tmp*7/8+1:tmp*7/8;
		if (udhi == 0x40)
		{
			ntmp = 2*(tmp - 0x06);
			nDstLength = tmp;
		}
		else
		{
			ntmp = 2*tmp;
			nDstLength = tmp*2;
		}
		gsmString2Bytes(pSrc, buf, ntmp);
		//change over

// old
//		tmp = tmp&7 ? tmp*7/8+1:tmp*7/8;
//		int ntmp = 2*tmp;
//		nDstLength = gsmString2Bytes(pSrc, buf, ntmp);

// 		tmp = 2*tmp;
// 		nDstLength = gsmString2Bytes(pSrc, buf, tmp);
		// ��ʽת��
		if(udhi == 0x40)
		{	
			if(udh[2] == 0x03)
			{
				gsmDecodeLong7bit(buf, pDst->TP_UD, nDstLength);
			}
			else
			{
				gsmDecode7bit(buf, pDst->TP_UD, nDstLength);
			}
		}
		else
		{	
			gsmDecode7bit(buf, pDst->TP_UD, nDstLength); // ת����TP-DU
		}
		nDstLength = tmp;
	}
	else if(pDst->TP_DCS == GSM_UCS2) 
	{
		// UCS2����
		nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);
		// ��ʽת��
		nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength); // ת����TP-DU
	}
	else 
	{
		// 8-bit����
		nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);
		unsigned char* pBuf = &buf[0];
		//		 char udh[] = {0x05, 0x04, 0x0B, 0x84, 0x23, 0xF0};// ֪ͨ
		//		 if (memcmp(pBuf + 1, udh, 6) == 0)
		if ((pBuf[1] == 0x06) && (memcmp(pBuf + 3, "application/vnd.wap", strlen("application/vnd.wap")) == 0))
		{
			//pBuf += 1; //0x06
			//tmp = *pBuf; //head 1 ����
			//pBuf += 1;
			//pBuf += tmp;
			pBuf += 2; // Transaction ID ,PDU Type: Push 0x06
			tmp = *pBuf; //ͷ2����
			pBuf += 1;

			char* mms = "application/vnd.wap.mms-message";
			if (memcmp(pBuf, mms, strlen(mms)) == 0)
			{
				pBuf += tmp;

				nDstLength -= (3 + tmp);//Transaction ID ,PDU Type,ͷ2����,ͷ����

				//pDst->pMMS_Notify = new MMS::MMSWarp::MMS_NotificationRequest;
				//MMS::MMSWarp::GetMMSWarp()->DecodeNotificationRequest((const char*)pBuf, nDstLength, pDst->pMMS_Notify);

				int length = 0;

				while (length < nDstLength)
				{
					unsigned char flag = pBuf[length++];

					switch(flag)
					{
					case 0x8C:
						pDst->MMS_Type = pBuf[length];
						length++;
						break;
					case 0x98:
						pDst->MMS_TransactionID = std::string((char*)pBuf + length);
						length += pDst->MMS_TransactionID.size() + 1;
						break;
					case 0x8D:
						pDst->MMS_Version = pBuf[length];
						length++;
						break;
					case 0x83:
						{
							std::string url = std::string((char*)pBuf + length);
							gsmDecode8bit((const unsigned char *)url.c_str(), pDst->TP_UD, url.size());
							length += url.size() + 1;
						}
						break;
					case 0x85:
						{
							unsigned char len = pBuf[length++];
							unsigned int n = 0;
							for (int i = 0; i < len; ++i)
							{
								unsigned char t = pBuf[length];
								n = ((n << 8) | t);
								length += 1;
							}
							pDst->DateAndTime = (n+8*3600);
						}
						break;
					case 0x88:
						{
							unsigned char len = pBuf[length++];//0x05 0x81  //Expiry + Length + Relative-token + Length + Delta-secs
							unsigned int n = 0;
							unsigned char token = pBuf[length];
							if (token == 0x81)
							{
								length += 1;
								unsigned char len = pBuf[length];
								length += 1;
								for (int i = 0; i < len; ++i)
								{
									unsigned char t = pBuf[length];
									n = ((n << 8) | t);
									length += 1;
								}
							}
							else
							{
								length += 1;
								unsigned char len = pBuf[length];
								length += 1;
								char buf[128] = {0};
								unsigned int l = gsmSerializeNumbers((const char*)pBuf + length, buf, len);
								std::string t(buf, l);
								
								//add by qi 20100820
								if (atoi(t.substr(0, 2).c_str()) == 0)
								{
									break;	
								}
								//

								CTime expiry(atoi(t.substr(0, 2).c_str()) + 2000, atoi(t.substr(2, 2).c_str()), atoi(t.substr(4, 2).c_str()), atoi(t.substr(6, 2).c_str()), atoi(t.substr(8, 2).c_str()), atoi(t.substr(10, 2).c_str()));
								CTime start(1970, 1, 1, 8, 0, 0);
								CTimeSpan ts = expiry - start;
								n = ts.GetTotalSeconds();
								length += len;
							}
							pDst->MMS_TimeOfExpiry = n;
						}
						break;
					case 0x89:
						{
							extern void ExtractNumber(CString content , std::vector<CString> &vnum);
							std::vector<CString> number;

							unsigned char len = pBuf[length++];
							length += 1;//0x80
							pDst->MMS_SenderAddress = std::string((char*)pBuf + length);
							length += pDst->MMS_SenderAddress.size() + 1;

							ExtractNumber(Util::StringOp::ToCString(pDst->MMS_SenderAddress), number);
							if(number.size() > 0)
							{
								pDst->MMS_SenderAddress = Util::StringOp::FromCString(number[0]);
							}
						}
						break;
					case 0x8A:
						pDst->MMS_Class = pBuf[length];
						length++;
						break;
					case 0x8B:
						pDst->MessageID = string((char*)pBuf + length);
						length += pDst->MessageID.size() + 1;
						break;
					case 0x8E:
						{
							unsigned char len = pBuf[length++];
							unsigned int n = 0;
							for (int i = 0; i < len; ++i)
							{
								unsigned char s = pBuf[length];
								n = ((n << 8) | s);
								length += 1;
							}
							pDst->MMS_Size = n;
						}
						break;
					case 0x95:
						length++;
						break;
					case 0x97:
						pDst->RecipientAddress = string((char*)pBuf + length);
						length += pDst->RecipientAddress.size() + 1;
						if (pDst->RecipientAddress.find("/") != string::npos)
						{
							pDst->RecipientAddress = pDst->RecipientAddress.substr(0, pDst->RecipientAddress.find("/"));
						}
						break;
					}
				}
				pDst->Type = mtMMS;
			}
			else if ((tmp == 1) && (*pSrc == 0xAE)) //si
			{
				pDst->Type = mtSI;
			}
		}
		else //sms
		{
			unsigned int otaType = 0;
			for (int i = 0; i < 4; ++i)
			{
				otaType = ((otaType << 8) | buf[i]);
			}
			//1065805044
			if ((std::string(pDst->TPA) == m_strOTANumber))// || (otaType == tOTAMenu) || (otaType == tOTAdMediaFile) || (otaType == tOTAInterface) || (otaType == tOTAWeather))
			{
				gsmDecode8bit(buf + 4, pDst->TP_UD, nDstLength - 4);
				//pDst->pOTA_Notify = new OTA_NotificationRequest();
				//pDst->pOTA_Notify->Type = (OTA_TYPE)otaType;
				pDst->OTAType = (OTA_TYPE)otaType;
				pDst->Type = mtOTA;
			}
			else
			{
				// ��ʽת��
				nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength); // ת����TP-DU
				pDst->Type = mtSMS;
			}
		}
	}

	// ����Ŀ���ַ�������
	return nDstLength;
}

int SMSWarp::gsmDecodeReceiptPdu(const char *pSrc, SM_RECEIPT *pDst)
{
	int nDstLength; // Ŀ��PDU������
	unsigned char tmp; // ��ʱ�ֽڱ���
	
	gsmString2Bytes(pSrc, &tmp, 2); // ȡ����
	if(tmp != 0)
	{
		tmp = (tmp - 1) * 2; // SMSC���봮����
		pSrc += 4; // ָ�����
		nDstLength += 4;
		nDstLength += tmp;
		gsmSerializeNumbers(pSrc, pDst->smsc, tmp); // ת��SMSC���뵽Ŀ��PDU��
		pSrc += tmp;
	}
	
	gsmString2Bytes(pSrc, &tmp, 2);
	if(tmp == 0x06)
	{
		pSrc += 4;  //����ͷ�ֽںͷ������
		nDstLength += 4;
	}
	
	gsmString2Bytes(pSrc, &tmp, 2);
	if(tmp & 1)
	{
		tmp += 1; // ������ż��
	}
	pSrc += 4; // ָ�����
	gsmSerializeNumbers(pSrc, pDst->reciveAddress, tmp); // ȡ���պ���
	pSrc += tmp; // ָ�����
	nDstLength += 4;
	nDstLength += tmp;
	
	gsmSerializeNumbers(pSrc, pDst->toSmsc, 14); // ��Ϣ�������Ϣ���ĵ�ʱ��
	pSrc += 14;
	nDstLength += 14;
	
	gsmSerializeNumbers(pSrc, pDst->toDest, 14); // ��Ϣ����Է��ֻ���ʱ��
	pSrc += 14;
	nDstLength += 14;
	return nDstLength;

} 

// ���Ͷ���Ϣ���������������ȡӦ��    
// ����: pSrc - ԴPDU����ָ��    
int SMSWarp::gsmSendMessage(SM_PARAM* pSrc, SM_BUFF* pBuff)   
{   
	Dprintf("Gsm SendMessage\r\n");
	memcpy(&m_smPa,pSrc,sizeof(SM_PARAM));//add by qi 20100407
	m_nHaveSend = 0 ;
	m_nSendNum  = gsmEncodePdu_(pSrc, m_smsPdu);
	m_nWriteNum = m_nSendNum ;

	// add by qi 20100407
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_nSendNum > 1 && main->m_pMessageSet->SmsSendSaveSim())//ɾ����Ҫ�浽SIM�ĳ����š�
	{
		std::string filter;
		filter = "[reference] = 1 AND [uplevelProtocol] = 200";
		Data::Message::Remove(filter);
	}

//	main->m_pATCommandWarp1->CMMS();
	
	main->m_pATCommandWarp1->CMGF(0);
	main->m_pATCommandWarp1->CSCS("HEX");
	SendMessage();

/*	int nPduLength;     // PDU������    
	unsigned char nSmscLength;  // SMSC������    
	SMS_BUFF pdu[MAX_SM_SEND];  // PDU��   	

	int count = gsmEncodePdu_(pSrc, pdu);   // ����PDU����������PDU��    

	for(int i = 0; i < count; i++)
	{
		nPduLength = pdu[i].len;
		strcat(pdu[i].buff, "\x01a"); // ��Ctrl-Z����    
		gsmString2Bytes(pdu[i].buff, &nSmscLength, 2);  // ȡPDU���е�SMSC��Ϣ����    
		nSmscLength++;      // ���ϳ����ֽڱ���    
		
		//// �����еĳ��ȣ�������SMSC��Ϣ���ȣ��������ֽڼ�    
		//sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength); // ��������    
		
		//  TRACE("%s", cmd);    
		//  TRACE("%s\n", pdu);    
		
		//WriteComm(cmd, strlen(cmd));    // ��������    
		
		//nLength = ReadComm(ans, 128);   // ��Ӧ������    
		
		//// �����ܷ��ҵ�"\r\n> "�����ɹ����    
		//if(nLength == 4 && strncmp(ans, "\r\n> ", 4) == 0)   
		//{   
		//	return WriteComm(pdu, strlen(pdu));     // �õ��϶��ش𣬼������PDU��    
		//}   
//		if (m_pAT->SmsSend(nPduLength / 2 - nSmscLength))
		{	
			m_pAT->SmsSend(nPduLength / 2 - nSmscLength);
			pBuff->len = m_pAT->SmsSend(pdu[i].buff, strlen(pdu[i].buff), pBuff->data, 16384);
			Sleep(1000);
		}
	}
	*/
	return 0;   
}   

// ��ȡ����Ϣ���������������ȡӦ��    
// ��+CMGL����+CMGR����һ���Զ���ȫ������Ϣ    
int SMSWarp::gsmReadMessageList(SM_BUFF* pBuff)   
{   
	m_pAT->SmsReadList(pBuff->data, 16384);
	return 1;
}   

// ɾ������Ϣ���������������ȡӦ��    
// ����: index - ����Ϣ��ţ�1-255    
int SMSWarp::gsmDeleteMessage(int index, SM_BUFF* pBuff)   
{   
	//char cmd[16];       // ���    
	//sprintf(cmd, "AT+CMGD=%d\r", index);    // ��������    
	//// ������    
	//return WriteComm(cmd, strlen(cmd)); 
//	return m_pAT->SmsDelete(index);
	m_pAT->SmsDelete(index, "", 16384);
	return 1;
}   

// ��ȡGSM MODEM��Ӧ�𣬿�����һ����    
// ���: pBuff - ����Ӧ�𻺳���    
// ����: GSM MODEM��Ӧ��״̬, GSM_WAIT/GSM_OK/GSM_ERR    
// ��ע: ������Ҫ��ε��ò�����ɶ�ȡһ��Ӧ���״ε���ʱӦ��pBuff��ʼ��    
int SMSWarp::gsmGetResponse(SM_BUFF* pBuff)  
{   
	int nLength;        // �����յ������ݳ���    
	int nState;   

	// �Ӵ��ڶ����ݣ�׷�ӵ�������β��    
//	nLength = ReadComm(&pBuff->data[pBuff->len], 128);
	
//	0526	
// 	nLength = m_pAT->SmsReadResponse(&pBuff->data[pBuff->len], 16384 - pBuff->len);
// 	pBuff->len += nLength;   

	// ȷ��GSM MODEM��Ӧ��״̬    
	nState = GSM_WAIT;   
//	if (/*(nLength > 0) && */(pBuff->len >= 4))   //0526
	if ( (strlen(pBuff->data) >= 4) )   //0526
	{   

		if (strstr(pBuff->data, "\r\nOK\r\n") != NULL)
		{
			nState = GSM_OK;   
		}
		else if (strstr(pBuff->data, "+CMS ERROR") != NULL)
		{
			if (strstr(pBuff->data, "332") != NULL)
			{
				nState = GSM_NET_ERROR ;
			}
			else if (strstr(pBuff->data, "500") != NULL)
			{
				nState = GSM_UNKOMN;
			}
			else 
			nState = GSM_ERR;   
		}
		else if (strstr(pBuff->data,"+CME ERROR") != NULL)//���ʹ���
		{
			nState = GSM_ERR;
		}
		else
		{
			nState = GSM_OK; 
		}
	}   

	return nState;   
}   

// ���б��н�����ȫ������Ϣ    
// ����: pBuff - ����Ϣ�б�����    
// ���: pMsg - ����Ϣ������    
// ����: ����Ϣ����    
int SMSWarp::gsmParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff)   
{   
	int nMsg;           // ����Ϣ����ֵ    
	char* ptr;          // �ڲ��õ�����ָ��    

	nMsg = 0;   
	ptr = pBuff->data;   

	// ѭ����ȡÿһ������Ϣ, ��"+CMGL:"��ͷ    
	while((ptr = strstr(ptr, "+CMGL:")) != NULL)   
	{   
		ptr += 6;       // ����"+CMGL:", ��λ�����    
		sscanf(ptr, "%d", &pMsg->index); // ��ȡ���    
		//      TRACE("  index=%d\n",pMsg->index);    

		ptr = strstr(ptr, "\r\n");  //����һ��    
		if (ptr != NULL)   
		{   
			ptr += 2;       // ����"\r\n", ��λ��PDU    

			gsmDecodePdu(ptr, pMsg);    // PDU������    

			if(pMsg->MMS_Type < 0x86) //����Ƿ��ͱ�������ʾ���û�
			{
				pMsg++;     // ׼������һ������Ϣ    
				nMsg++;     // ����Ϣ������1 
			}
			else
			{
				pBuff->len = 0;
			
				extern VOID WriteMyLog_(char *ptr, int size);
				char log[100];
				sprintf(log,"MMS_Type:%d",pMsg->MMS_Type);
				WriteMyLog_(log,strlen(log));

				if(pMsg->MMS_Type == 0x86)  //���ͱ���
				{
					g_sRecipientAddress = pMsg->RecipientAddress;
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_DELETE_MESSAGE,pMsg->index,1);
				}
				else
				{
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_DELETE_MESSAGE,pMsg->index,0);
				}
			}
			
		}

	}   

	return nMsg;   
}   

// ��һ������Ϣ���뷢�Ͷ���    
void SMSWarp::PutSendMessage(SM_PARAM* pparam)   
{   
	EnterCriticalSection(&m_csSend);   

	memcpy(&m_SmSend[m_nSendIn], pparam, sizeof(SM_PARAM));   

	m_nSendIn++;   
	if (m_nSendIn > MAX_SM_SEND)  m_nSendIn = 0;   

	LeaveCriticalSection(&m_csSend);   
}   

// �ӷ��Ͷ�����ȡһ������Ϣ    
BOOL SMSWarp::GetSendMessage(SM_PARAM* pparam)   
{   
	BOOL fSuccess = FALSE;   

	EnterCriticalSection(&m_csSend);   

	if (m_nSendOut != m_nSendIn)   
	{   
		memcpy(pparam, &m_SmSend[m_nSendOut], sizeof(SM_PARAM));   

		m_nSendOut++;   
		if (m_nSendOut > MAX_SM_SEND)  m_nSendOut = 0;   

		fSuccess = TRUE;   
	}   

	LeaveCriticalSection(&m_csSend);

	return fSuccess;
}   

// ������Ϣ������ն���    
void SMSWarp::PutRecvMessage(SM_PARAM* pparam, int nCount)   
{   
	EnterCriticalSection(&m_csRecv);   

	for (int i = 0; i < nCount; i++)   
	{   
		memcpy(&m_SmRecv[m_nRecvIn], pparam, sizeof(SM_PARAM));   

		m_nRecvIn++;   
		if (m_nRecvIn >= MAX_SM_RECV)  m_nRecvIn = 0;   

		pparam++;   
	}   

	LeaveCriticalSection(&m_csRecv);   
}   

// �ӽ��ն�����ȡһ������Ϣ    
BOOL SMSWarp::GetRecvMessage(SM_PARAM* pparam)   
{   
	BOOL fSuccess = FALSE;   

	EnterCriticalSection(&m_csRecv);   

	if (m_nRecvOut != m_nRecvIn)   
	{   
		memcpy(pparam, &m_SmRecv[m_nRecvOut], sizeof(SM_PARAM));   

		m_nRecvOut++;   
		if (m_nRecvOut >= MAX_SM_RECV)  m_nRecvOut = 0;   

		fSuccess = TRUE;   
	}   

	LeaveCriticalSection(&m_csRecv);   

	return fSuccess;   
}   

void SMSWarp::State_(void)
{
	m_pAT->PhoneState();
}

void SMSWarp::PhoneNettype_(void)
{
	m_pAT->PhoneNettype();
}

void SMSWarp::SignalQuality_(void)
{
	m_pAT->PhoneSignalQuality();
}

void SMSWarp::Cmer(void)
{
	m_pAT->CMER();
}

UINT SMSWarp::SmThread(LPVOID lParam)   
{   
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	SMSWarp* p=(SMSWarp *)lParam;   // this    
	int nMsg;               // �յ�����Ϣ����    
	int nDelete;            // Ŀǰ����ɾ���Ķ���Ϣ���    
//	SM_BUFF buff;           // ���ն���Ϣ�б�Ļ�����   
//	p->m_pbuff.data = (char*)malloc(16384);
	SM_PARAM param[MAX_SM_SEND];    // ����/���ն���Ϣ������    
/*	enum {   
		stBeginRest,                // ��ʼ��Ϣ/��ʱ    
		stContinueRest,             // ������Ϣ/��ʱ    
		stSendMessageRequest,       // ���Ͷ���Ϣ    
		stSendMessageResponse,      // ��ȡ����Ӧ�𵽻�����    
		stReadMessageRequest,       // ���Ͷ�ȡ����Ϣ�б������    
		stReadMessageResponse,      // ��ȡ����Ϣ�б�������    
		stDeleteMessageRequest,     // ɾ������Ϣ    
		stExitThread                // �˳�    
	} nState;						// ������̵�״̬    
*/
	// ��ʼ״̬    
	nState = stBeginRest;
	
	// ���ͺͽ��մ����״̬ѭ��    
	while (nState != stExitThread)   
	{   
		switch(nState)
		{   
		case stBeginRest: 
			{
		//		TRACE(L"SMS State=stBeginRest\n"); 
				
				extern IsTestCTA();
				if(!IsTestCTA())
				{
					//�绰״̬
					Sleep(500);
					p->State_();
										
					if (main->m_bInsertSim) //û����SIM�����Ȳ�����ź�
					{
						Sleep(500);
						p->PhoneNettype_();
						Sleep(500);
						p->SignalQuality_();
					}
				
				}
		
				nState = stContinueRest; 
				EnterCriticalSection(&csCom);
			//	TRACE(L"SMS Enter\n");
			}
			break;

		case stContinueRest:   
		//	TRACE(L"SMS State=stContinueRest\n");    
			if (p->GetSendMessage(param))   
			{   
				nState = stSendMessageRequest;  //�д�������Ϣ���Ͳ���Ϣ��    
			}   
			else  
			{   
				nState = stReadMessageRequest;  //ת����ȡ����Ϣ״̬    
			}   
			break;   

		case stSendMessageRequest:   
		//	TRACE(L"SMS State=stSendMessageRequest\n");    
			p->m_pbuff.len = 0;
			memset(p->m_pbuff.data, 0, 16384);
			::SendMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_SENDPRO, (WPARAM)&param, (LPARAM)0);
			p->m_bParseCMS = false;
			p->gsmSendMessage(param, &(p->m_pbuff));
			nState = stSendMessageResponse;   
			break;   

		case stSendMessageResponse:   
	//		TRACE(L"SMS State=stSendMessageResponse\n");
			{
				//��ʱ�ж� 10��
				static int nTime = 0;//add by qi 20100828

				Sleep(100);
				if (p->m_bParseCMS)
				{	
					//add by qi 20100828
					nTime = 0;

					int result = p->gsmGetResponse(&(p->m_pbuff));
					switch (result)   
					{   
					case GSM_OK:    
						TRACE(L"Send  GSM_OK \n"); //��Ҫ֪ͨAPP����ʽ����
						p->SendCallback(1,param);
						break;
					
					case GSM_NET_ERROR:
					case GSM_UNKOMN:
					case GSM_ERR:
						TRACE(L"Send  GSM_ERR \n");    
						p->SendCallback(result,param);
						break; 
						
					default:   
						if (strstr(p->m_pbuff.data, "+CMGS:") != NULL)//+CMGS:
						{
			//				TRACE(L"Send  GSM_WAIT \n"); 
							p->SendCallback(1,param);
						}
						else  
						{   
			//				TRACE(L"Send  Timeout!\n");    
							p->SendCallback(4,param);
						}
					 
					}

					LeaveCriticalSection(&csCom);
			//		TRACE(L"SMS Leave\n");
					nState = stBeginRest; 
				} 
				else
				{	
					//add by qi 20100828
					nTime++;
					if (nTime > 300)//��ʱ30�����
					{
						nTime = 0;

						main->m_pATCommandWarp1->SetOkStatus(okIdle);
						memset(p->m_pbuff.data,0,16384);
						p->m_bParseCMS = true;
						p->m_nHaveSend = 0 ;
						p->m_nSendNum = 0 ;
						p->m_bReadyWrite2sim = 0;
						PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CSCS,0,0);

						p->SendCallback(GSM_UNKOMN,param);

						LeaveCriticalSection(&csCom);
						nState = stBeginRest; 

						extern VOID WriteMyLog_(char *ptr, int size);
						WriteMyLog_("SmsSendTimeOver",strlen("SmsSendTimeOver"));

					}
				}
			}
			break;   

		case stReadMessageRequest:   
			
		//	TRACE(L"SMS State=stReadMessageRequest\n");    
		//	p->m_pbuff.len = 0;
		//	memset(p->m_pbuff.data, 0, 16384);   
		//	p->m_bParseCMGL = false;
			
			extern IsTestCTA();
			if(!IsTestCTA())
			{
				if (main->m_bInsertSim)//����SIM��
				{
					p->gsmReadMessageList(&(p->m_pbuff)); 
				}
			}
			nState = stBeginRest; 
			LeaveCriticalSection(&csCom); 
			break;   
			
		}   

		// ����Ƿ��йرձ��̵߳��ź�    
		DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);
		if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;   
	}   

	free(p->m_pbuff.data);
	// �ø��߳̽�����־    
	SetEvent(p->m_hThreadKilledEvent);   

	return 0;  
}   

//////////////////////////////////////////////////////////////////////////

void SMSWarp::Send(std::string number, std::string content)
{
	SMS::SMSWarp::SM_PARAM SmParam;   
//	memset(&SmParam, 0, sizeof(SMS::SMSWarp::SM_PARAM));   
	std::string sca = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMessageSet->SmsCenterAddress();
	if (sca.empty())
	{
		SetCenterAddress();
		sca = m_strCenterAddress;
	}

	// ȥ������ǰ��"+"	// �ں���ǰ��"86"    
	if(sca[0] == '+')  sca = sca.substr(1);   
	if(sca.substr(0, 2) != "86")  sca = "86" + sca;   
 
	// ������Ϣ�ṹ    
	strcpy(SmParam.SCA, sca.c_str());   
	strcpy(SmParam.TP_UD, content.c_str());
	FindTP_DCS(content.c_str());
	SmParam.TP_PID = 0;   
	if(g_isUCS2)
	{
		SmParam.TP_DCS = GSM_UCS2;
	}
	else
	{
		SmParam.TP_DCS = GSM_7BIT;
	}

	std::string number_;
	m_sendCount = 0;

	do{
		BOOL flag = FALSE; 
		int nIndex = number.find(",");
		if(nIndex < 0)
			nIndex = number.find(";");
		if(nIndex < 0)
		{
			flag = TRUE;
			number_ = number;
		}
		else
		{
			number_ = number.substr(0, nIndex);
			number = number.substr(nIndex+1, number.length() - (nIndex+1));
		}

		if(number_ != "")
		{
			if (number_.substr(0, 2) != "10")
			{
				if(number_[0] == '+')  number_ = number_.substr(1); 
				if(number_.substr(0, 2) != "86")  number_ = "86" + number_;   
			}
			strcpy(SmParam.TPA, number_.c_str());   
			PutSendMessage(&SmParam);
			m_sendCount++;
		}
		
		if(flag)
			break;
	}while(true);

	m_sendErrorNumber = "";
	m_sendOkNumber = "";
}

void SMSWarp::SetCenterAddress()
{
	m_strCenterAddress = ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->speCode12_;
//	m_pAT->SmsCenterAddress(address);
}

std::wstring SMSWarp::ToUnicode(std::string content)
{
	size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), 0, 0);
	wchar_t* wideContent = new wchar_t[wideContentLength + 2];
	memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));
	size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent, wideContentLength);
	std::wstring result(wideContent);
	delete[] wideContent;
	return result;
}

void SMSWarp::MapMessage(SM_PARAM* sms)
{	
	// add by qi 20100407
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	void* pMsg = NULL;
	if (sms->Type == mtSMS)
	{
		Data::Message* msg = new Data::Message;
		pMsg = msg;
		std::string sca(sms->SCA);
		msg->smsc.address = sca;
		msg->smsc.addressLength = sca.size();
		msg->smsc.addressType.typeOfNumber = Data::AddressType::tInternationNumber;

		msg->flag.type = Data::Flag::tDeliver;

		std::string num(sms->TPA);
		msg->remote.address = num;
		msg->remote.addressLength = num.size();
		msg->remote.addressType.typeOfNumber = Data::AddressType::tNationNumber;

		msg->encoding.encoding = Data::Encoding::UnicodeCodeSet;
		msg->timestamp.year = atoi(std::string(&sms->TP_SCTS[0],2).c_str());
		msg->timestamp.month = atoi(std::string(&sms->TP_SCTS[2],2).c_str());
		msg->timestamp.day = atoi(std::string(&sms->TP_SCTS[4],2).c_str());
		msg->timestamp.hour = atoi(std::string(&sms->TP_SCTS[6],2).c_str());
		msg->timestamp.minite = atoi(std::string(&sms->TP_SCTS[8],2).c_str());
		msg->timestamp.second = atoi(std::string(&sms->TP_SCTS[10],2).c_str());

		// add by qi 20100406
		//�浽SIM����
		if (main->m_pMessageSet->SmsReciveSaveSim())//���ܵ���Ϣ���浽SIM��
		{
			msg->uplevelProtocol = sms->index;//��SIM���е�����
			msg->reference = 1;
		}
		else 
		{	
			if ((sms->TP_DCS & 0x02))//û�����ã�ֻ����class-2 sms
			{
				msg->uplevelProtocol = sms->index;
				msg->reference = 1;
				
			}
			else
			{
				msg->uplevelProtocol =  200;
				msg->reference = 0;
			}
		}

		msg->unicodeData = sms->TP_UD;
		msg->state = Data::Message::sNoRead;
		msg->group = Data::Message::gReceive;
		msg->total = sms->Total;
		if (sms->Total > 1)
		{
 			msg->transactionId = sms->Serial;
			msg->total = sms->Total;
 			msg->no = sms->Number;
		}

	}
	else if (sms->Type == mtMMS)
	{
		Data::MmsMessage* msg = new Data::MmsMessage;
		pMsg = msg;
		std::string sca(sms->SCA);
		msg->smsc.address = sca;
		msg->smsc.addressLength = sca.size();
		msg->smsc.addressType.typeOfNumber = Data::AddressType::tInternationNumber;
		
		msg->flag.type = Data::Flag::tDeliver;
		
		std::string num(sms->MMS_SenderAddress);
		msg->remote.address = num;
		msg->remote.addressLength = num.size();
		msg->remote.addressType.typeOfNumber = Data::AddressType::tNationNumber;
		
		msg->encoding.encoding = Data::Encoding::UnicodeCodeSet;
		
		msg->timestamp.year = atoi(std::string(&sms->TP_SCTS[0],2).c_str());
		msg->timestamp.month = atoi(std::string(&sms->TP_SCTS[2],2).c_str());
		msg->timestamp.day = atoi(std::string(&sms->TP_SCTS[4],2).c_str());
		msg->timestamp.hour = atoi(std::string(&sms->TP_SCTS[6],2).c_str());
		msg->timestamp.minite = atoi(std::string(&sms->TP_SCTS[8],2).c_str());
		msg->timestamp.second = atoi(std::string(&sms->TP_SCTS[10],2).c_str());
		
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		msg->localReciveTime.year = tm.wYear - 2000;
		msg->localReciveTime.month = tm.wMonth;
		msg->localReciveTime.day = tm.wDay;
		msg->localReciveTime.hour = tm.wHour;
		msg->localReciveTime.minite = tm.wMinute;
		msg->localReciveTime.second = tm.wSecond;

		msg->state = Data::MmsMessage::stUnDownload;
		msg->transactionId = sms->MMS_TransactionID;
		msg->messageClass = sms->MMS_Class;
		msg->messageSize = sms->MMS_Size;
		msg->timeOfExpiry = sms->MMS_TimeOfExpiry;
		//msg->timeOfExpiry = 30;//test
		msg->downloadCount = 0;
		msg->repeatCount = 0;
		msg->url = sms->TP_UD;
	}
	else if (sms->Type == mtOTA)
	{
		Data::OtaMessage* msg = new Data::OtaMessage;
		pMsg = msg;
		std::string sca(sms->SCA);
		msg->smsc.address = sca;
		msg->smsc.addressLength = sca.size();
		msg->smsc.addressType.typeOfNumber = Data::AddressType::tInternationNumber;
		
		msg->flag.type = Data::Flag::tDeliver;
		
		std::string num(sms->TPA);
		msg->remote.address = num;
		msg->remote.addressLength = num.size();
		msg->remote.addressType.typeOfNumber = Data::AddressType::tNationNumber;
		
		msg->encoding.encoding = Data::Encoding::UnicodeCodeSet;
		
		msg->timestamp.year = atoi(std::string(&sms->TP_SCTS[0],2).c_str());
		msg->timestamp.month = atoi(std::string(&sms->TP_SCTS[2],2).c_str());
		msg->timestamp.day = atoi(std::string(&sms->TP_SCTS[4],2).c_str());
		msg->timestamp.hour = atoi(std::string(&sms->TP_SCTS[6],2).c_str());
		msg->timestamp.minite = atoi(std::string(&sms->TP_SCTS[8],2).c_str());
		msg->timestamp.second = atoi(std::string(&sms->TP_SCTS[10],2).c_str());

// 		std::string content(sms->TP_UD);
// 		size_t wideContentLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), 0, 0);
// 		wchar_t* wideContent = new wchar_t[wideContentLength + 2];
// 		memset(wideContent, 0, sizeof(wchar_t) * (wideContentLength + 2));
// 		size_t convLength = MultiByteToWideChar(CP_ACP, 0, content.c_str(), content.length(), wideContent, wideContentLength);
// 		std::wstring result(wideContent);
// 		delete[] wideContent;

		msg->state = Data::OtaMessage::stUnDownload;
		msg->instruction = Data::Type((int)sms->OTAType);
		msg->url = sms->TP_UD;
	}
	ReceiveCallback(sms->Type, pMsg);
}
void SMSWarp::ReceiveCallback(SMS_TYPE type, void* msg)
{
	switch (type)
	{
	case mtSMS:
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_STATUS, WM_SMS_SMS, (LPARAM)msg);
		break;
	case mtMMS:
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_STATUS, WM_SMS_MMS, (LPARAM)msg);
		break;
	case mtOTA:
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_STATUS, WM_SMS_OTA, (LPARAM)msg);
		break;
	}
}
void SMSWarp::SendCallback(int error,SM_PARAM *sm)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	std::string content = sm->TP_UD;
	std::string number	= sm->TPA ;
	std::string filter	= "data = '" ; 
				filter += content;
				filter += "'";
	
	std::vector<boost::shared_ptr<Data::Message> >result ;
	result = Data::Message::GetFromDatabase(filter);
	if (!result.empty())
	{
		m_sendCount--;
		if(number.substr(0, 2) == "86")
		{
			number = number.substr(2);
		}

		if (error == 1)
		{
			result[0]->state = Data::Message::sReaded;//���ͳɹ�	
			m_sendOkNumber += number;
			m_sendOkNumber += ";";
		}
		else if( error > 1)
		{
			result[0]->state = Data::Message::sNoRead;//����ʧ��
			m_sendErrorNumber += number;
			m_sendErrorNumber += ";";
		}

		if(0 == m_sendCount)
		{
			if(m_sendOkNumber != "" && m_sendErrorNumber != "")
			{
				boost::shared_ptr<Data::Message> result_ = boost::shared_ptr<Data::Message>(new Data::Message);
				if(result_)
				{
					*result_ = *result[0];
					
					result_->remote.address = m_sendErrorNumber;
					result_->state = Data::Message::sNoRead;
					result[0]->remote.address = m_sendOkNumber;
					result[0]->state = Data::Message::sReaded;
					result_->Insert();
				}
			}
			result[0]->Update();
			m_sendErrorNumber = "";
			m_sendOkNumber = "";
		}
	}
	
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SMS_STATUS, WM_SMS_SENDRESULT, error);
}

void SMSWarp::EncodeRegisterSMS(char *pDst, int &dstLen)
{
	unsigned char buff[256];
	int nLength = 0;
	int nDstLength = 0;

	std::string sca = m_pAT->SmsCenterAddress();
	if (sca.empty())
	{
	//	SetCenterAddress();
		m_strCenterAddress = "+8613800100500";
		sca = m_strCenterAddress;
	}
	
	// ȥ������ǰ��"+"	// �ں���ǰ��"86"    
	if(sca[0] == '+')  sca = sca.substr(1);   
	if(sca.substr(0, 2) != "86")  sca = "86" + sca;   

	nLength = sca.length();
	buff[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;
	buff[1] = 0x91;   //���ʺ���̶���ʽ
	nDstLength = gsmBytes2String(buff, pDst, 2);
	nDstLength += gsmInvertNumbers(sca.c_str(), &pDst[nDstLength], nLength);
	nLength = strlen("1065840409");			//10654040
	buff[0] = 0x11;
	buff[1] = 0x00;
	buff[2] = (char)nLength;
	buff[3] = 0xA1;  //�����ط��ţ����ڸ�ʽ����
	nDstLength += gsmBytes2String(buff, &pDst[nDstLength], 4);
	nDstLength += gsmInvertNumbers("1065840409", &pDst[nDstLength], nLength);			//10654040
				
	char tempData[140];
	char data[140];
	char head[] = "06050442664266";
	memset(tempData, 0, 140);
	memset(data, 0, 140);
	strcpy(tempData, "IMEI:");
	
	extern char gIMEI[32];
	char simei[32] = {0};
	char *ptr = strstr(gIMEI, "GSN:");
	if(ptr)
	{
		char *ptr1 = (strstr(ptr, "\r\n"));
		if(ptr1)
			memcpy(simei, ptr+5, ptr1-ptr-5);
	}
	strcat(tempData, simei);

	extern CString s_VerSionTitle;

	strcat(tempData, "/DTmobile/DTmobile_HM56/");
	strcat(tempData, Util::StringOp::FromCString(s_VerSionTitle).c_str());
//	strcat(tempData, "/DESAY/TD-169(J)/V521R14C01S34");    // ��֤
//	strcpy(tempData, "IMEI:358978002001001/DESAY/T560/DESAY_T560_S002_CMCC");

	nLength = strlen(tempData);
	buff[0] = 0x00;
	buff[1] = 0x04; //8bit
	buff[2] = 0xA7;
	buff[3] = (unsigned char)(nLength+strlen(head)/2);   //*2
	nDstLength += gsmBytes2String(buff, &pDst[nDstLength], 4);
	strcat(pDst, head);
	gsmEncode8bit(tempData, (unsigned char *)data, nLength);
	memset(tempData, 0, 140);

	gsmBytes2String((const unsigned char *)data, tempData, nLength);
	strcat(pDst, tempData);

	strcat(pDst, "\x01a");
	char *buf = new char[1024];
	memset(buf, 0, 1024);
	nLength = sca.length();
	nLength = ((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;
	dstLen = strlen(pDst)/2 - (nLength+1);
}

BOOL SMSWarp::SendRegistDM()
{
	char Buffer[512];
	int length = 0;
	memset(Buffer, 0, 512);
	EncodeRegisterSMS(Buffer, length);

	char ans[1024] = {0};
	m_pAT->SmsSend(length, Buffer, strlen(Buffer), ans, 1024);
	if(strstr(ans, "ERROR") != NULL)
		return FALSE;
	return TRUE;

}

void SMSWarp::ReadMessage(SM_BUFF *buff)
{		
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nMsg;
	int nDelete = 0;
	SM_BUFF *pbuff = buff;
	SM_PARAM param[MAX_SM_SEND];    // ���ն���Ϣ������
			
	switch (gsmGetResponse(pbuff))
	{   
	case GSM_OK:    
		nMsg = gsmParseMessageList(param, pbuff);   
		if (nMsg > 0)   
		{   
			for (int i = 0; i < nMsg; ++i)
			{
				MapMessage(&param[i]);
			}
			nDelete = 0;   
		}   
		break;			
	default:   
		break;   
	}
	
	while (nDelete < nMsg)//�Ƿ�ɾ��SIM�������Ϣ
	{
		if (param[nDelete].Type == mtOTA || param[nDelete].Type == mtMMS)
		{
			int index = param[nDelete].index;
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_DELETE_MESSAGE,index,0);
		//	gsmDeleteMessage(param[nDelete].index, pbuff);   
		}
		else 
		{
			if (!(main->m_pMessageSet->SmsReciveSaveSim()))//���浽����
			{
				if (!(param[nDelete].TP_DCS & 0x02))//���� class-2 ɾ��
				{	
					int index = param[nDelete].index;
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_DELETE_MESSAGE,index,0);
				//	gsmDeleteMessage(param[nDelete].index, pbuff);
				}
				else
				{	
					//��ɾ�Ļ���ѯSIM���Ĵ洢����
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SIM_STORAGE,0,0);
				}
			}
			else //��ɾ�Ļ���ѯSIM���Ĵ洢����
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SIM_STORAGE,0,0);
			}
			
		}
		nDelete++;   
	}
	 
}

bool SMSWarp::IsSendingSms()
{
	if (m_nSendIn != m_nSendOut || nState == stSendMessageRequest 
		|| nState == stSendMessageResponse )//��Ҫ���Ķ���
	{
		return true;
	}
	return false;
}