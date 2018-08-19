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


//状态
enum {   
	stBeginRest,                // 开始休息/延时    
		stContinueRest,             // 继续休息/延时    
		stSendMessageRequest,       // 发送短消息    
		stSendMessageResponse,      // 读取发送应答到缓冲区    
		stReadMessageRequest,       // 发送读取短消息列表的命令    
		stReadMessageResponse,      // 读取短消息列表到缓冲区    
		stDeleteMessageRequest,     // 删除短消息    
		stExitThread                // 退出    
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
		if(*ptr >= 0xA0)  //汉字
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
	SetEvent(m_hKillThreadEvent);           // 发出关闭子线程的信号    
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);    // 等待子线程关闭    

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
	// 启动子线程  
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
	int nPduLength;     // PDU串长度    
	unsigned char nSmscLength;  // SMSC串长度    

	nPduLength = m_smsPdu[m_nHaveSend].len;
	strcat(m_smsPdu[m_nHaveSend].buff, "\x01a"); // 以Ctrl-Z结束    
	gsmString2Bytes(m_smsPdu[m_nHaveSend].buff, &nSmscLength, 2);  // 取PDU串中的SMSC信息长度    
	nSmscLength++;//加上长度字节本身    
	
	Dprintf("Gsm SendMessageMiddle\r\n");
	
	main->m_pATCommandWarp1->SmsSend(nPduLength / 2 - nSmscLength);

//	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_SEND_LENGTH,nPduLength / 2 - nSmscLength,0);

	Dprintf("Gsm SmsSendPdu\r\n");
	
//  Test 
//  	for (int i = 0 ; i < 28;i++)
//  	{
//  		main->m_pATCommandWarp1->SmsWrite2Sim(nPduLength/2 - nSmscLength,3);//长度	
//  		Sleep(50);
//  		main->m_pATCommandWarp1->SmsWrite2Sim(m_smsPdu[m_nHaveSend].buff, strlen(m_smsPdu[m_nHaveSend].buff));//数据
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
	int nPduLength;     // PDU串长度    
	unsigned char nSmscLength;  // SMSC串长度    
	
	nPduLength = m_smsPdu[m_nHaveWrite].len;
//	strcat(m_smsPdu[m_nHaveWrite].buff, "\x01a"); // 以Ctrl-Z结束    
	gsmString2Bytes(m_smsPdu[m_nHaveWrite].buff, &nSmscLength, 2);  // 取PDU串中的SMSC信息长度    
	nSmscLength++;//加上长度字节本身    
	
	Dprintf("Gsm WriteMessageMessageMiddle\r\n");
	
	main->m_pATCommandWarp1->SmsWrite2Sim(nPduLength/2 - nSmscLength,3);//长度	

	main->m_pATCommandWarp1->SmsWrite2Sim(m_smsPdu[m_nHaveWrite].buff, strlen(m_smsPdu[m_nHaveWrite].buff));//数据
	m_nHaveWrite++;
}

void SMSWarp::SendSmsPdu(std::string const& data)
{
	// 发送短信数据
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
	if (data.find("+CMS ERROR: 332") != std::string::npos)//网络超时
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
		if (main->m_pMessageSet->SmsSendSaveSim())//把发送的信息保存到SIM卡
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
				
				if (index >= 0)//发送成功
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
			else //长短信
			{
				//拆分成几条存储
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
		sta = atoi(s.c_str());//状态，Messages 
		
		t = s.find("\r\n");
		
		if ( t != std::string::npos)
		{	
			s = s.substr(t+2);
			
			t = s.find("\r\n");
			pdu = s.substr(0,t);//截取一个完整的pdu串
						
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
		
		if ( 0 == smstype)//接受
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
		
		msg->reference = 1;//存在SIM卡中
		msg->uplevelProtocol = index ;// 先用 uplevelProtocol 表示 sms 在短信中的index
		
		msg->total = sm->Total;
		if (sm->Total > 1)
		{
			msg->transactionId = sm->Serial;
			msg->total = sm->Total;
			msg->no = sm->Number;
			//msg->group = Data::Message::gExternSms; //家庭百事通的超长短信
		}
		msg->Insert();
		// CombineData(msg); 先不合并
		delete msg;
	}
	else if(sm->Type == mtMMS)//处理SIM里的短息通知
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
	
	if (main->m_pMessageSet->SmsSendSaveSim())//发送信息保存到SIM
	{
		int nPduLength;     // PDU串长度    
		unsigned char nSmscLength;  // SMSC串长度    	
		Dprintf("m_nHaveSend :%d\r\n",m_nHaveSend);

		nPduLength = m_smsPdu[m_nHaveSend-1].len;
//		strcat(m_smsPdu[m_nHaveSend-1].buff, "\x01a"); // 以Ctrl-Z结束    
		gsmString2Bytes(m_smsPdu[m_nHaveSend-1].buff, &nSmscLength, 2);  // 取PDU串中的SMSC信息长度    
		nSmscLength++;//加上长度字节本身 
		
		//change by qi 20100629
		//PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_WRITE_LENGTH,nPduLength/2 - nSmscLength,0);
	
		//static std::string d = "";
		//d = m_smsPdu[m_nHaveSend-1].buff;
		//PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_WRITE_TO_SIM,(WPARAM)&d,0);
		
		main->m_pATCommandWarp1->SmsWrite2Sim(nPduLength/2 - nSmscLength,3);//长度				
		main->m_pATCommandWarp1->SmsWrite2Sim(m_smsPdu[m_nHaveSend-1].buff, strlen(m_smsPdu[m_nHaveSend-1].buff));//数据
		
	}

}

void SMSWarp::WritePduData(std::string const& data)
{	
	//往SIM卡里写的真正数据
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
// 可打印字符串转换为字节数据    
// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标数据指针    
// 返回: 目标数据长度    
int SMSWarp::gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
	for (int i = 0; i < nSrcLength; i += 2)   
	{   
		// 输出高4位    
		if ((*pSrc >= '0') && (*pSrc <= '9'))   
		{   
			*pDst = (*pSrc - '0') << 4;   
		}   
		else   
		{   
			*pDst = (*pSrc - 'A' + 10) << 4;   
		}   

		pSrc++;   

		// 输出低4位    
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

	// 返回目标数据长度    
	return (nSrcLength / 2);   
}   

// 字节数据转换为可打印字符串    
// 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01"     
// 输入: pSrc - 源数据指针    
//       nSrcLength - 源数据长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)   
{   
	const char tab[]="0123456789ABCDEF";    // 0x0-0xf的字符查找表    

	for (int i = 0; i < nSrcLength; i++)   
	{   
		*pDst++ = tab[*pSrc >> 4];        // 输出高4位    
		*pDst++ = tab[*pSrc & 0x0f];    // 输出低4位    
		pSrc++;   
	}   

	// 输出字符串加个结束符    
	*pDst = '\0';   

	// 返回目标字符串长度    
	return (nSrcLength * 2);   
}   

// 7bit编码    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标编码串指针    
// 返回: 目标编码串长度    
int SMSWarp::gsmEncode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
	int nSrc;       // 源字符串的计数值    
	int nDst;       // 目标编码串的计数值    
	int nChar;      // 当前正在处理的组内字符字节的序号，范围是0-7    
	unsigned char nLeft;    // 上一字节残余的数据    

	// 计数值初始化    
	nSrc = 0;
	nDst = 0;  

	// 将源串每8个字节分为一组，压缩成7个字节    
	// 循环该处理过程，直至源串被处理完    
	// 如果分组不到8字节，也能正确处理    
	while (nSrc < nSrcLength)   
	{   
		// 取源字符串的计数值的最低3位    
		nChar = nSrc & 7;   

		// 处理源串的每个字节    
		if(nChar == 0)   
		{   
			// 组内第一个字节，只是保存起来，待处理下一个字节时使用    
			nLeft = *pSrc;   
		}   
		else   
		{   
			// 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节    
			*pDst = (*pSrc << (8-nChar)) | nLeft;   

			// 将该字节剩下的左边部分，作为残余数据保存起来    
			nLeft = *pSrc >> nChar;   

			// 修改目标串的指针和计数值    
			pDst++;   
			nDst++;   
		}   

		// 修改源串的指针和计数值    
		pSrc++;   
		nSrc++;   
	}   

	// 返回目标串长度    
	return nDst;   
}

int SMSWarp::gsmEncodeLong7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
	int nSrc;       // 源字符串的计数值    
	int nDst;       // 目标编码串的计数值    
	int nChar;      // 当前正在处理的组内字符字节的序号，范围是0-7    
	unsigned char nLeft;    // 上一字节残余的数据    

	// 计数值初始化    
	nSrc = 0;   
	nDst = 0;   

	*pDst++ = *pSrc++ << 1;
	nDst++;

	// 将源串每8个字节分为一组，压缩成7个字节    
	// 循环该处理过程，直至源串被处理完    
	// 如果分组不到8字节，也能正确处理    
	while (nSrc < nSrcLength - 1)   
	{   
		// 取源字符串的计数值的最低3位    
		nChar = nSrc & 7;   

		// 处理源串的每个字节    
		if(nChar == 0)   
		{   
			// 组内第一个字节，只是保存起来，待处理下一个字节时使用    
			nLeft = *pSrc;   
		}   
		else   
		{
			// 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节    
			*pDst = (*pSrc << (8-nChar)) | nLeft;   

			// 将该字节剩下的左边部分，作为残余数据保存起来    
			nLeft = *pSrc >> nChar;   

			// 修改目标串的指针和计数值    
			pDst++;   
			nDst++;   
		}   

		// 修改源串的指针和计数值    
		pSrc++;   
		nSrc++;   
	}   

	// 返回目标串长度  
	return nDst;   
} 

// 7bit解码    
// 输入: pSrc - 源编码串指针    
//       nSrcLength - 源编码串长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmDecode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength) {
	int nSrc; // 源字符串的计数值
	int nDst; // 目标解码串的计数值
	int nByte; // 当前正在处理的组内字节的序号，范围是0-6
	unsigned char nLeft; // 上一字节残余的数据
	// 计数值初始化
	nSrc = 0;
	nDst = 0;
	// 组内字节序号和残余数据初始化
	nByte = 0;
	nLeft = 0;
	// 将源数据每7个字节分为一组，解压缩成8个字节
	// 循环该处理过程，直至源数据被处理完
	// 如果分组不到7字节，也能正确处理
	while(nSrc<nSrcLength) {
		// 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
		// 将该字节剩下的左边部分，作为残余数据保存起来
		nLeft = *pSrc >> (7-nByte);
		// 修改目标串的指针和计数值
		pDst++;
		nDst++;
		// 修改字节计数值
		nByte++;
		// 到了一组的最后一个字节
		if(nByte == 7) {
			// 额外得到一个目标解码字节
			*pDst = nLeft;
			// 修改目标串的指针和计数值
			pDst++;
			nDst++;
			// 组内字节序号和残余数据初始化
			nByte = 0;
			nLeft = 0;
		}
		// 修改源串的指针和计数值
		pSrc++;
		nSrc++;
	}
	*pDst = 0;
	// 返回目标串长度
	return nDst;
}

//7bit 长短信
int SMSWarp::gsmDecodeLong7bit(const unsigned char* pSrc, char* pDst, int nSrcLength) {
	int nSrc; // 源字符串的计数值
	int nDst; // 目标解码串的计数值
	int nByte; // 当前正在处理的组内字节的序号，范围是0-6
	unsigned char nLeft; // 上一字节残余的数据
	// 计数值初始化
	nSrc = 0;
	nDst = 0;
	// 组内字节序号和残余数据初始化
	nByte = 0;
	nLeft = 0;
	
	*pDst++ = *pSrc++ >> 1;
	nDst++;

	// 将源数据每7个字节分为一组，解压缩成8个字节
	// 循环该处理过程，直至源数据被处理完
	// 如果分组不到7字节，也能正确处理
	while(nSrc < nSrcLength-1) {
		// 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;
		// 将该字节剩下的左边部分，作为残余数据保存起来
		nLeft = *pSrc >> (7-nByte);
		// 修改目标串的指针和计数值
		pDst++;
		nDst++;
		// 修改字节计数值
		nByte++;
		// 到了一组的最后一个字节
		if(nByte == 7) {
			// 额外得到一个目标解码字节
			*pDst = nLeft;
			// 修改目标串的指针和计数值
			pDst++;
			nDst++;
			// 组内字节序号和残余数据初始化
			nByte = 0;
			nLeft = 0;
		}
		// 修改源串的指针和计数值
		pSrc++;
		nSrc++;
	}
	*pDst = 0;
	// 返回目标串长度
	return nDst;
}

// 8bit编码    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标编码串指针    
// 返回: 目标编码串长度    
int SMSWarp::gsmEncode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)   
{   
	// 简单复制    
	memcpy(pDst, pSrc, nSrcLength);   

	return nSrcLength;   
}   

// 8bit解码    
// 输入: pSrc - 源编码串指针    
//       nSrcLength -  源编码串长度    
// 输出: pDst -  目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmDecode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)   
{   
	// 简单复制    
	memcpy(pDst, pSrc, nSrcLength);   

	// 输出字符串加个结束符    
	pDst[nSrcLength] = '\0';   

	return nSrcLength;   
}   

// UCS2编码    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标编码串指针    
// 返回: 目标编码串长度    
int SMSWarp::gsmEncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength) {
	int nDstLength; // UNICODE宽字符数目
	WCHAR wchar[256] = {0}; // UNICODE串缓冲区
	// 字符串-->UNICODE串
	nDstLength = ::MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 256);
	// 高低字节对调，输出
	for(int i=0; i<nDstLength; i++) {
		// 先输出高位字节
		*pDst++ = wchar[i] >> 8;
		// 后输出低位字节
		*pDst++ = wchar[i] & 0xff;
	}
	// 返回目标编码串长度
	return nDstLength * 2;
}

// UCS2解码    
// 输入: pSrc - 源编码串指针    
//       nSrcLength -  源编码串长度    
// 输出: pDst -  目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmDecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength) {
	int nDstLength;
	// UNICODE宽字符数目
	WCHAR wchar[128]; // UNICODE串缓冲区
	// 高低字节对调，拼成UNICODE
	for(int i=0; i<nSrcLength/2; i++) {
		// 先高位字节
		wchar[i] = *pSrc++ << 8;
		// 后低位字节
		wchar[i] |= *pSrc++;
	}
	// UNICODE串-->字符串
	nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160,
		NULL, NULL);
	// 输出字符串加个结束符
	pDst[nDstLength] = '\0';
	// 返回目标字符串长度
	return nDstLength;
}

// 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数    
// 如："8613851872468" --> "683158812764F8"    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度        	

int SMSWarp::gsmInvertNumbers(const char* pSrc, char* pDst, int nSrcLength) 
{
	int nDstLength; // 目标字符串长度
	char ch; // 用于保存一个字符
	// 复制串长度
	nDstLength = nSrcLength;
	// 两两颠倒
	for(int i=0; i<nSrcLength;i+=2) {
		ch = *pSrc++; // 保存先出现的字符
		*pDst++ = *pSrc++; // 复制后出现的字符
		*pDst++ = ch; // 复制先出现的字符
	}
	// 源串长度是奇数吗？
	if(nSrcLength & 1) {
		*(pDst-2) = 'F'; // 补'F'
		nDstLength++; // 目标串长度加1
	}
	*pDst = '\0'; // 输出字符串加个结束符
	return nDstLength; // 返回目标字符串长度
}

// 两两颠倒的字符串转换为正常顺序的字符串
// 如："683158812764F8" --> "8613851872468"   	
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标字符串指针    
// 返回: 目标字符串长度    
int SMSWarp::gsmSerializeNumbers(const char* pSrc, char* pDst, int nSrcLength) {
	int nDstLength; // 目标字符串长度
	char ch; // 用于保存一个字符
	// 复制串长度
	nDstLength = nSrcLength;
	// 两两颠倒
	for(int i=0; i<nSrcLength;i+=2) {
		ch = *pSrc++; // 保存先出现的字符
		*pDst++ = *pSrc++; // 复制后出现的字符
		*pDst++ = ch; // 复制先出现的字符
	}
	// 最后的字符是'F'吗？
	if(*(pDst-1) == 'F') {
		pDst--;
		nDstLength--; // 目标字符串长度减1
	}
	*pDst = '\0'; // 输出字符串加个结束符
	return nDstLength; // 返回目标字符串长度
}

// PDU编码，用于编制、发送短消息
// pSrc: 源PDU参数指针
// pDst: 目标PDU串指针
// 返回: 目标PDU串长度
int SMSWarp::gsmEncodePdu_(const SM_PARAM* pSrc, SMS_BUFF* pDst_) {

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nLength; // 内部用的串长度
	int nDstLength; // 目标PDU串长度
	unsigned char buf[256]; // 内部用的缓冲区

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

		// SMSC地址信息段
		nLength = strlen(pSrc->SCA); // SMSC地址字符串的长度
		buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;// SMSC地址信息长度
		buf[1] = 0x91; // 固定: 用国际格式号码
		nDstLength = gsmBytes2String(buf, pDst, 2); // 转换2个字节到目标PDU串
		// 转换SMSC到目标PDU串
		nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);
		// TPDU段基本参数、目标地址等
		nLength = strlen(pSrc->TPA); // TP-DA地址字符串的长度
		unsigned char report = 0;
		if(1 == main->m_pMessageSet->SmsReceipt())
		{
			report = 0x20;
		}

		if(nTotal == 1)
		{
			strcpy(s_Mulit, ptr1);
		
			buf[0] = 0x11+report; // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
		}
		else
		{
			memcpy(s_Mulit, ptr1+lineCount[i], lineCount[i+1] - lineCount[i]);
		
			buf[0] = 0x51+report; // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
			
			head[4] = nTotal;
			head[5] = i+1;
		
		}
		buf[1] = 0; // TP-MR=0
		buf[2] = (char)nLength; // 目标地址数字个数(TP-DA地址字符串真实长度)
		if ((pSrc->TPA[0] == '8') && (pSrc->TPA[1] == '6'))
		{
			buf[3] = 0x91; // 固定: 用国际格式号码
		}
		else
		{
			buf[3] = 0xA1; // 用国内格式号码
		}
		nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//转换4个字节到目标PDU串
		// 转换TP-DA到目标PDU串
		nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
		// TPDU段协议标识、编码方式、用户信息等
		
		nLength = strlen(s_Mulit); // 用户信息字符串的长度
		buf[0] = pSrc->TP_PID; // 协议标识(TP-PID)
		buf[1] = pSrc->TP_DCS; // 用户信息编码方式(TP-DCS)
		
		if(1 == main->m_pMessageSet->SmsValidity())
		{
			buf[2] = 0x0B; // 有效期(TP-VP) 0为5分钟 90为12小时 A7为24小时
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
			// 7-bit编码方式
			buf[3] = nLength; // 编码前长度
			// 转换TP-DA到目标PDU串
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
			// UCS2编码方式
			// 转换TP-DA到目标PDU串
			buf[3] = gsmEncodeUcs2(s_Mulit, &buf[4], nLength);
			nLength = buf[3] + 4; // nLength等于该段数据长度
		}
		else {
			// 8-bit编码方式
			// 转换TP-DA到目标PDU串
			buf[3] = gsmEncode8bit(s_Mulit, &buf[4], nLength);
			nLength = buf[3] + 4; // nLength等于该段数据长度
		}
		// 转换该段数据到目标PDU串
		// 转换该段数据到目标PDU串
		//nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
		//return nDstLength; // 返回目标字符串长度

		if(nTotal > 1)
		{
			char head_[25];
			if(pSrc->TP_DCS == GSM_7BIT)
			{
				buf[3] = buf[3] + 7;   //7bit时头长度加7
			}
			else
			{
				buf[3] = buf[3]+6;	//长短信长度加上6
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

	return nCount; // 返回目标字符串长度
}

// PDU编码，用于编制、发送短消息
// pSrc: 源PDU参数指针
// pDst: 目标PDU串指针
// 返回: 目标PDU串长度
int SMSWarp::gsmEncodePdu(const SM_PARAM* pSrc, char* pDst) {
	int nLength; // 内部用的串长度
	int nDstLength; // 目标PDU串长度
	unsigned char buf[256]; // 内部用的缓冲区
	// SMSC地址信息段
	nLength = strlen(pSrc->SCA); // SMSC地址字符串的长度
	buf[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;// SMSC地址信息长度
	buf[1] = 0x91; // 固定: 用国际格式号码
	nDstLength = gsmBytes2String(buf, pDst, 2); // 转换2个字节到目标PDU串
	// 转换SMSC到目标PDU串
	nDstLength += gsmInvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);
	// TPDU段基本参数、目标地址等
	nLength = strlen(pSrc->TPA); // TP-DA地址字符串的长度
	buf[0] = 0x11; // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
	buf[1] = 0; // TP-MR=0
	buf[2] = (char)nLength; // 目标地址数字个数(TP-DA地址字符串真实长度)
	if ((pSrc->TPA[0] == '8') && (pSrc->TPA[1] == '6'))
	{
		buf[3] = 0x91; // 固定: 用国际格式号码
	}
	else
	{
		buf[3] = 0xA1; // 用国内格式号码
	}
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], 4);//转换4个字节到目标PDU串
	// 转换TP-DA到目标PDU串
	nDstLength += gsmInvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength);
	// TPDU段协议标识、编码方式、用户信息等
	nLength = strlen(pSrc->TP_UD); // 用户信息字符串的长度
	buf[0] = pSrc->TP_PID; // 协议标识(TP-PID)
	buf[1] = pSrc->TP_DCS; // 用户信息编码方式(TP-DCS)
	buf[2] = 0xA7; // 有效期(TP-VP) 0为5分钟 90为12小时 A7为24小时
	if(pSrc->TP_DCS == GSM_7BIT) {
		// 7-bit编码方式
		buf[3] = nLength; // 编码前长度
		// 转换TP-DA到目标PDU串
		nLength = gsmEncode7bit(pSrc->TP_UD, &buf[4], nLength+1) + 4;
	}
	else if (pSrc->TP_DCS == GSM_UCS2) {
		// UCS2编码方式
		// 转换TP-DA到目标PDU串
		buf[3] = gsmEncodeUcs2(pSrc->TP_UD, &buf[4], nLength);
		nLength = buf[3] + 4; // nLength等于该段数据长度
	}
	else {
		// 8-bit编码方式
		// 转换TP-DA到目标PDU串
		buf[3] = gsmEncode8bit(pSrc->TP_UD, &buf[4], nLength);
		nLength = buf[3] + 4; // nLength等于该段数据长度
	}
	// 转换该段数据到目标PDU串
	nDstLength += gsmBytes2String(buf, &pDst[nDstLength], nLength);
	return nDstLength; // 返回目标字符串长度
}
// PDU解码，用于接收、阅读短消息
// pSrc: 源PDU串指针
// pDst: 目标PDU参数指针
// 返回: 用户信息串长度
int SMSWarp::gsmDecodePdu(const char* pSrc, SM_PARAM* pDst,int type) {
	int nDstLength; // 目标PDU串长度
	unsigned char tmp; // 内部用的临时字节变量
	unsigned char buf[256]; // 内部用的缓冲区
	unsigned char udhi = 0; //是否有数据头
	unsigned char udh[32]; //数据头内容

	// SMSC地址信息段
	gsmString2Bytes(pSrc, &tmp, 2); // 取长度
	tmp = (tmp - 1) * 2; // SMSC号码串长度
	pSrc += 4; // 指针后移
	gsmSerializeNumbers(pSrc, pDst->SCA, tmp); // 转换SMSC号码到目标PDU串
	pSrc += tmp; // 指针后移
	// TPDU段基本参数、回复地址等
	gsmString2Bytes(pSrc, &tmp, 2); // 取基本参数
	
	// change by qi 20100407
	pSrc += 2;// 指针后移
	if ( 1 == type)
	{
		pSrc += 2;//跳过<mr>
	}

	udhi = tmp & 0x40;//用户数据头标识
	//	 if(tmp & 0x80) 
	{
		// 包含回复地址，取回复地址信息
		gsmString2Bytes(pSrc, &tmp, 2); // 取长度
		if(tmp & 1) tmp += 1; // 调整奇偶性
		pSrc += 4; // 指针后移
		gsmSerializeNumbers(pSrc, pDst->TPA, tmp); // 取TP-RA号码
		pSrc += tmp; // 指针后移
	}
	// TPDU段协议标识、编码方式、用户信息等
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2); // 取协议标识(TP-PID)
	pSrc += 2; // 指针后移
	gsmString2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);
	// 取编码方式(TP-DCS)
	pSrc += 2; // 指针后移
	//change by qi 20100407
	if ( 1 != type )// 发送没有时间戳
	{
		gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14); // 服务时间戳字符串(TP_SCTS)
		pSrc += 14; // 指针后移
	}
	else
	{   
		//add by qi 20100401
		gsmString2Bytes(pSrc, &tmp, 2);
		if ( tmp != (strlen(pSrc+2)/2))
		{
			pSrc += 2; // 指针后移	
		}
	}
//	gsmSerializeNumbers(pSrc, pDst->TP_SCTS, 14); // 服务时间戳字符串(TP_SCTS)
//	pSrc += 14; // 指针后移
	
	gsmString2Bytes(pSrc, &tmp, 2); // 用户信息长度(TP-UDL)
	pSrc += 2; // 指针后移

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
	pDst->MMS_Type = 0x85;//不是彩信通知全是85

	if(pDst->TP_DCS == GSM_7BIT) 
	{
		// 7-bit解码
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
		// 格式转换
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
			gsmDecode7bit(buf, pDst->TP_UD, nDstLength); // 转换到TP-DU
		}
		nDstLength = tmp;
	}
	else if(pDst->TP_DCS == GSM_UCS2) 
	{
		// UCS2解码
		nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);
		// 格式转换
		nDstLength = gsmDecodeUcs2(buf, pDst->TP_UD, nDstLength); // 转换到TP-DU
	}
	else 
	{
		// 8-bit解码
		nDstLength = gsmString2Bytes(pSrc, buf, tmp * 2);
		unsigned char* pBuf = &buf[0];
		//		 char udh[] = {0x05, 0x04, 0x0B, 0x84, 0x23, 0xF0};// 通知
		//		 if (memcmp(pBuf + 1, udh, 6) == 0)
		if ((pBuf[1] == 0x06) && (memcmp(pBuf + 3, "application/vnd.wap", strlen("application/vnd.wap")) == 0))
		{
			//pBuf += 1; //0x06
			//tmp = *pBuf; //head 1 长度
			//pBuf += 1;
			//pBuf += tmp;
			pBuf += 2; // Transaction ID ,PDU Type: Push 0x06
			tmp = *pBuf; //头2长度
			pBuf += 1;

			char* mms = "application/vnd.wap.mms-message";
			if (memcmp(pBuf, mms, strlen(mms)) == 0)
			{
				pBuf += tmp;

				nDstLength -= (3 + tmp);//Transaction ID ,PDU Type,头2长度,头内容

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
				// 格式转换
				nDstLength = gsmDecode8bit(buf, pDst->TP_UD, nDstLength); // 转换到TP-DU
				pDst->Type = mtSMS;
			}
		}
	}

	// 返回目标字符串长度
	return nDstLength;
}

int SMSWarp::gsmDecodeReceiptPdu(const char *pSrc, SM_RECEIPT *pDst)
{
	int nDstLength; // 目标PDU串长度
	unsigned char tmp; // 临时字节变量
	
	gsmString2Bytes(pSrc, &tmp, 2); // 取长度
	if(tmp != 0)
	{
		tmp = (tmp - 1) * 2; // SMSC号码串长度
		pSrc += 4; // 指针后移
		nDstLength += 4;
		nDstLength += tmp;
		gsmSerializeNumbers(pSrc, pDst->smsc, tmp); // 转换SMSC号码到目标PDU串
		pSrc += tmp;
	}
	
	gsmString2Bytes(pSrc, &tmp, 2);
	if(tmp == 0x06)
	{
		pSrc += 4;  //跳过头字节和发送序号
		nDstLength += 4;
	}
	
	gsmString2Bytes(pSrc, &tmp, 2);
	if(tmp & 1)
	{
		tmp += 1; // 调整奇偶性
	}
	pSrc += 4; // 指针后移
	gsmSerializeNumbers(pSrc, pDst->reciveAddress, tmp); // 取接收号码
	pSrc += tmp; // 指针后移
	nDstLength += 4;
	nDstLength += tmp;
	
	gsmSerializeNumbers(pSrc, pDst->toSmsc, 14); // 消息到达短信息中心的时间
	pSrc += 14;
	nDstLength += 14;
	
	gsmSerializeNumbers(pSrc, pDst->toDest, 14); // 消息到达对方手机的时间
	pSrc += 14;
	nDstLength += 14;
	return nDstLength;

} 

// 发送短消息，仅发送命令，不读取应答    
// 输入: pSrc - 源PDU参数指针    
int SMSWarp::gsmSendMessage(SM_PARAM* pSrc, SM_BUFF* pBuff)   
{   
	Dprintf("Gsm SendMessage\r\n");
	memcpy(&m_smPa,pSrc,sizeof(SM_PARAM));//add by qi 20100407
	m_nHaveSend = 0 ;
	m_nSendNum  = gsmEncodePdu_(pSrc, m_smsPdu);
	m_nWriteNum = m_nSendNum ;

	// add by qi 20100407
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_nSendNum > 1 && main->m_pMessageSet->SmsSendSaveSim())//删除将要存到SIM的长短信。
	{
		std::string filter;
		filter = "[reference] = 1 AND [uplevelProtocol] = 200";
		Data::Message::Remove(filter);
	}

//	main->m_pATCommandWarp1->CMMS();
	
	main->m_pATCommandWarp1->CMGF(0);
	main->m_pATCommandWarp1->CSCS("HEX");
	SendMessage();

/*	int nPduLength;     // PDU串长度    
	unsigned char nSmscLength;  // SMSC串长度    
	SMS_BUFF pdu[MAX_SM_SEND];  // PDU串   	

	int count = gsmEncodePdu_(pSrc, pdu);   // 根据PDU参数，编码PDU串    

	for(int i = 0; i < count; i++)
	{
		nPduLength = pdu[i].len;
		strcat(pdu[i].buff, "\x01a"); // 以Ctrl-Z结束    
		gsmString2Bytes(pdu[i].buff, &nSmscLength, 2);  // 取PDU串中的SMSC信息长度    
		nSmscLength++;      // 加上长度字节本身    
		
		//// 命令中的长度，不包括SMSC信息长度，以数据字节计    
		//sprintf(cmd, "AT+CMGS=%d\r", nPduLength / 2 - nSmscLength); // 生成命令    
		
		//  TRACE("%s", cmd);    
		//  TRACE("%s\n", pdu);    
		
		//WriteComm(cmd, strlen(cmd));    // 先输出命令串    
		
		//nLength = ReadComm(ans, 128);   // 读应答数据    
		
		//// 根据能否找到"\r\n> "决定成功与否    
		//if(nLength == 4 && strncmp(ans, "\r\n> ", 4) == 0)   
		//{   
		//	return WriteComm(pdu, strlen(pdu));     // 得到肯定回答，继续输出PDU串    
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

// 读取短消息，仅发送命令，不读取应答    
// 用+CMGL代替+CMGR，可一次性读出全部短消息    
int SMSWarp::gsmReadMessageList(SM_BUFF* pBuff)   
{   
	m_pAT->SmsReadList(pBuff->data, 16384);
	return 1;
}   

// 删除短消息，仅发送命令，不读取应答    
// 输入: index - 短消息序号，1-255    
int SMSWarp::gsmDeleteMessage(int index, SM_BUFF* pBuff)   
{   
	//char cmd[16];       // 命令串    
	//sprintf(cmd, "AT+CMGD=%d\r", index);    // 生成命令    
	//// 输出命令串    
	//return WriteComm(cmd, strlen(cmd)); 
//	return m_pAT->SmsDelete(index);
	m_pAT->SmsDelete(index, "", 16384);
	return 1;
}   

// 读取GSM MODEM的应答，可能是一部分    
// 输出: pBuff - 接收应答缓冲区    
// 返回: GSM MODEM的应答状态, GSM_WAIT/GSM_OK/GSM_ERR    
// 备注: 可能需要多次调用才能完成读取一次应答，首次调用时应将pBuff初始化    
int SMSWarp::gsmGetResponse(SM_BUFF* pBuff)  
{   
	int nLength;        // 串口收到的数据长度    
	int nState;   

	// 从串口读数据，追加到缓冲区尾部    
//	nLength = ReadComm(&pBuff->data[pBuff->len], 128);
	
//	0526	
// 	nLength = m_pAT->SmsReadResponse(&pBuff->data[pBuff->len], 16384 - pBuff->len);
// 	pBuff->len += nLength;   

	// 确定GSM MODEM的应答状态    
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
		else if (strstr(pBuff->data,"+CME ERROR") != NULL)//发送错误
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

// 从列表中解析出全部短消息    
// 输入: pBuff - 短消息列表缓冲区    
// 输出: pMsg - 短消息缓冲区    
// 返回: 短消息条数    
int SMSWarp::gsmParseMessageList(SM_PARAM* pMsg, SM_BUFF* pBuff)   
{   
	int nMsg;           // 短消息计数值    
	char* ptr;          // 内部用的数据指针    

	nMsg = 0;   
	ptr = pBuff->data;   

	// 循环读取每一条短消息, 以"+CMGL:"开头    
	while((ptr = strstr(ptr, "+CMGL:")) != NULL)   
	{   
		ptr += 6;       // 跳过"+CMGL:", 定位到序号    
		sscanf(ptr, "%d", &pMsg->index); // 读取序号    
		//      TRACE("  index=%d\n",pMsg->index);    

		ptr = strstr(ptr, "\r\n");  //找下一行    
		if (ptr != NULL)   
		{   
			ptr += 2;       // 跳过"\r\n", 定位到PDU    

			gsmDecodePdu(ptr, pMsg);    // PDU串解码    

			if(pMsg->MMS_Type < 0x86) //如果是发送报告则不显示给用户
			{
				pMsg++;     // 准备读下一条短消息    
				nMsg++;     // 短消息计数加1 
			}
			else
			{
				pBuff->len = 0;
			
				extern VOID WriteMyLog_(char *ptr, int size);
				char log[100];
				sprintf(log,"MMS_Type:%d",pMsg->MMS_Type);
				WriteMyLog_(log,strlen(log));

				if(pMsg->MMS_Type == 0x86)  //发送报告
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

// 将一条短消息放入发送队列    
void SMSWarp::PutSendMessage(SM_PARAM* pparam)   
{   
	EnterCriticalSection(&m_csSend);   

	memcpy(&m_SmSend[m_nSendIn], pparam, sizeof(SM_PARAM));   

	m_nSendIn++;   
	if (m_nSendIn > MAX_SM_SEND)  m_nSendIn = 0;   

	LeaveCriticalSection(&m_csSend);   
}   

// 从发送队列中取一条短消息    
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

// 将短消息放入接收队列    
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

// 从接收队列中取一条短消息    
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
	int nMsg;               // 收到短消息条数    
	int nDelete;            // 目前正在删除的短消息编号    
//	SM_BUFF buff;           // 接收短消息列表的缓冲区   
//	p->m_pbuff.data = (char*)malloc(16384);
	SM_PARAM param[MAX_SM_SEND];    // 发送/接收短消息缓冲区    
/*	enum {   
		stBeginRest,                // 开始休息/延时    
		stContinueRest,             // 继续休息/延时    
		stSendMessageRequest,       // 发送短消息    
		stSendMessageResponse,      // 读取发送应答到缓冲区    
		stReadMessageRequest,       // 发送读取短消息列表的命令    
		stReadMessageResponse,      // 读取短消息列表到缓冲区    
		stDeleteMessageRequest,     // 删除短消息    
		stExitThread                // 退出    
	} nState;						// 处理过程的状态    
*/
	// 初始状态    
	nState = stBeginRest;
	
	// 发送和接收处理的状态循环    
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
					//电话状态
					Sleep(500);
					p->State_();
										
					if (main->m_bInsertSim) //没插入SIM卡，先不检查信号
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
				nState = stSendMessageRequest;  //有待发短消息，就不休息了    
			}   
			else  
			{   
				nState = stReadMessageRequest;  //转到读取短消息状态    
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
				//超时判断 10秒
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
						TRACE(L"Send  GSM_OK \n"); //需要通知APP，方式待定
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
					if (nTime > 300)//超时30秒出来
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
				if (main->m_bInsertSim)//插入SIM卡
				{
					p->gsmReadMessageList(&(p->m_pbuff)); 
				}
			}
			nState = stBeginRest; 
			LeaveCriticalSection(&csCom); 
			break;   
			
		}   

		// 检测是否有关闭本线程的信号    
		DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);
		if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;   
	}   

	free(p->m_pbuff.data);
	// 置该线程结束标志    
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

	// 去掉号码前的"+"	// 在号码前加"86"    
	if(sca[0] == '+')  sca = sca.substr(1);   
	if(sca.substr(0, 2) != "86")  sca = "86" + sca;   
 
	// 填充短消息结构    
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
		//存到SIM卡中
		if (main->m_pMessageSet->SmsReciveSaveSim())//接受的信息保存到SIM卡
		{
			msg->uplevelProtocol = sms->index;//在SIM卡中的索引
			msg->reference = 1;
		}
		else 
		{	
			if ((sms->TP_DCS & 0x02))//没有设置，只保存class-2 sms
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
			result[0]->state = Data::Message::sReaded;//发送成功	
			m_sendOkNumber += number;
			m_sendOkNumber += ";";
		}
		else if( error > 1)
		{
			result[0]->state = Data::Message::sNoRead;//发送失败
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
	
	// 去掉号码前的"+"	// 在号码前加"86"    
	if(sca[0] == '+')  sca = sca.substr(1);   
	if(sca.substr(0, 2) != "86")  sca = "86" + sca;   

	nLength = sca.length();
	buff[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;
	buff[1] = 0x91;   //国际号码固定格式
	nDstLength = gsmBytes2String(buff, pDst, 2);
	nDstLength += gsmInvertNumbers(sca.c_str(), &pDst[nDstLength], nLength);
	nLength = strlen("1065840409");			//10654040
	buff[0] = 0x11;
	buff[1] = 0x00;
	buff[2] = (char)nLength;
	buff[3] = 0xA1;  //短信特服号，国内格式号码
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
//	strcat(tempData, "/DESAY/TD-169(J)/V521R14C01S34");    // 验证
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
	SM_PARAM param[MAX_SM_SEND];    // 接收短消息缓冲区
			
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
	
	while (nDelete < nMsg)//是否删除SIM卡里的信息
	{
		if (param[nDelete].Type == mtOTA || param[nDelete].Type == mtMMS)
		{
			int index = param[nDelete].index;
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_DELETE_MESSAGE,index,0);
		//	gsmDeleteMessage(param[nDelete].index, pbuff);   
		}
		else 
		{
			if (!(main->m_pMessageSet->SmsReciveSaveSim()))//保存到本地
			{
				if (!(param[nDelete].TP_DCS & 0x02))//不是 class-2 删除
				{	
					int index = param[nDelete].index;
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_DELETE_MESSAGE,index,0);
				//	gsmDeleteMessage(param[nDelete].index, pbuff);
				}
				else
				{	
					//不删的话查询SIM卡的存储容量
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SIM_STORAGE,0,0);
				}
			}
			else //不删的话查询SIM卡的存储容量
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
		|| nState == stSendMessageResponse )//有要发的短信
	{
		return true;
	}
	return false;
}