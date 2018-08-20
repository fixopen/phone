#include "stdafx.h"
#include "ATCommandWarp.h"

#include "resource.h"
#include "../Multimediaphone.h"
#include "../MultimediaPhoneDlg.h"

using Util::ATCommandWarp;

bool ATCommandWarp::bInit = false;
char *IMEI = NULL;
char *IMSI = NULL;
extern  BOOL g_isSimEnable;
BOOL g_IsFDNFindDrap = TRUE;
int g_p1;
int g_p2;
int g_pu1;
int g_pu2;
extern BOOL FindVideoDlg(BOOL flag);

/************************************************************************/
/*        add by qi                                                      */
/************************************************************************/
struct Converter {
	Converter() {
		memset(convertBuffer, 0, 7);
	}
	unsigned char convertBuffer[7];
	unsigned char sevenBit0() {
		return convertBuffer[0] >> 1;
	}
	unsigned char sevenBit1() {
		return ((convertBuffer[0] & 0x1) << 6) | (convertBuffer[1] >> 2);
	}
	unsigned char sevenBit2() {
		return ((convertBuffer[1] & 0x3) << 5) | (convertBuffer[2] >> 3);
	}
	unsigned char sevenBit3() {
		return ((convertBuffer[2] & 0x7) << 4) | (convertBuffer[3] >> 4);
	}
	unsigned char sevenBit4() {
		return ((convertBuffer[3] & 0xF) << 3) | (convertBuffer[4] >> 5);
	}
	unsigned char sevenBit5() {
		return ((convertBuffer[4] & 0x1F) << 2) | (convertBuffer[5] >> 6);
	}
	unsigned char sevenBit6() {
		return ((convertBuffer[5] & 0x3F) << 1) | (convertBuffer[6] >> 7);
	}
	unsigned char sevenBit7() {
		return ((convertBuffer[6] & 0x7F) << 0);
	}
	void sevenBit0(unsigned char ch) {
		ch &= 0x7F;
		convertBuffer[0] = ch << 1;
	}
	void sevenBit1(unsigned char ch) {
		ch &= 0x7F;
		convertBuffer[0] |= (ch & 0x40) >> 6;
		convertBuffer[1] = (ch & 0x3F) << 2;
	}
	void sevenBit2(unsigned char ch) {
		convertBuffer[1] |= (ch & 0x60) >> 5;
		convertBuffer[2] = (ch & 0x1F) << 3;
	}
	void sevenBit3(unsigned char ch) {
		convertBuffer[2] |= (ch & 0x70) >> 4;
		convertBuffer[3] = (ch & 0xF) << 4;
	}
	void sevenBit4(unsigned char ch) {
		convertBuffer[3] |= (ch & 0x78) >> 3;
		convertBuffer[4] = (ch & 0x7) << 5;
	}
	void sevenBit5(unsigned char ch) {
		convertBuffer[4] |= (ch & 0x7C) >> 2;
		convertBuffer[5] = (ch & 0x3) << 6;
	}
	void sevenBit6(unsigned char ch) {
		convertBuffer[5] |= (ch & 0x7E) >> 1;
		convertBuffer[6] = (ch & 0x1) << 7;
	}
	void sevenBit7(unsigned char ch) {
		convertBuffer[6] |= (ch & 0x7F) >> 0;
	}

};
// add by qi up 

BOOL GetIMEINumber(char const *pSrc, char *pDst)
{
	std::string imei = pSrc;
	unsigned int pos1 = imei.find(":");
	if(pos1 != std::string::npos)
	{
		for(int i=pos1, j=0; i<imei.length(); i++) //将手机IMEI号放在数组IMEI中
		{
			if(pSrc[i] >= '0' && pSrc[i] <= '9')
			{
				pDst[j++] = pSrc[i];
				if(j >= 15)    //手机IMEI号为15位数字
				{
					return TRUE;
				}		
			}
			else
			{
				memset(pDst, 0, imei.length());
				j=0;
			}
		}
	}
	return FALSE;
}

ATCommandWarp::ATCommandWarp(): Util::Observer()
{
	m_pCom = NULL;
	m_strPhoneNumber = "";
	m_bgsm = true ;
	InitializeCriticalSection(&m_csAT);
	m_index = 0;
	m_iContact = 0 ;

	m_okstatus = okIdle;
	m_bInstance = false;

	m_bWaitCSQ = false;
	
	InitializeCriticalSection(&m_csCPBR);
	InitializeCriticalSection(&m_csTranch);
	InitializeCriticalSection(&m_csOk);

//	AfxBeginThread(SendMsgThread, this, THREAD_PRIORITY_NORMAL);
}

ATCommandWarp::~ATCommandWarp()
{
	if (bInit)
	{
		Off();
	}

	DeleteCriticalSection(&m_csAT); 
	DeleteCriticalSection(&m_csCPBR); 
	DeleteCriticalSection(&m_csTranch);
	DeleteCriticalSection(&m_csOk);
}

ATCommandWarp::ATCommandWarp(const ATCommandWarp& ATWarp)
{

}

UINT ATCommandWarp::SendMsgThread(LPVOID lParam) 
{	
	
	ATCommandWarp *pAT=(ATCommandWarp *)lParam;   // this    

	while (true)
	{	
		EnterCriticalSection(&(pAT->m_csTranch));
		size_t size = pAT->m_vMsg.size();
		Dprintf("size:%d:\r\n",size);

		if (size > 0)
		{
			SendMsg msg = pAT->m_vMsg[0];		
			pAT->m_vMsg.pop_front();
			size = pAT->m_vMsg.size();
			Dprintf("pop_front_size:%d:\r\n",size);
			LeaveCriticalSection(&(pAT->m_csTranch));

			if (msg.flag == pAT->m_okstatus && msg.flag != okNoHandle)
			{
				Util::ATCommand::Instance()->Write(msg.data,"");	
			
			}
			else
			{		
				for (;;)
				{
					static int count = 0 ;
					if (pAT->m_okstatus  == okIdle)
					{
						DWORD d = SuspendThread(Util::ATCommand::Instance()->serialPort_->readThread_->handle_);
						Dprintf("SuspendThread:%d\r\n",d);
						//EnterCriticalSection(&(pAT->m_csOk));
						Util::ATCommand::Instance()->Write(msg.data,"");
						pAT->m_okstatus = msg.flag;	
						//LeaveCriticalSection(&(pAT->m_csOk));
						d = ResumeThread(Util::ATCommand::Instance()->serialPort_->readThread_->handle_);
						Dprintf("ResumeThread:%d\r\n",d);
						count = 0;

						break;
					}
					else
					{			
						count++;		
						Sleep(10);
				//		Dprintf("Time = %d second\r\n",count*10/1000);
					}
				}
			
			}
		}
		else 
		{	
			LeaveCriticalSection(&(pAT->m_csTranch));
			Sleep(100);
		}
	}

} 

//ATCommandWarp* ATCommandWarp::GetATCommandWarp()
//{
//	static ATCommandWarp* at = 0;
//	if (at == 0)
//	{
//		at = new ATCommandWarp();
//	}
//	return at;
//}

void ATCommandWarp::Bind(Util::ComWarp* com)
{
	m_pCom = com;
}
int ATCommandWarp::Connect(char* pin,BOOL isSleepStart)
{
	if (!bInit)
	{
		int i = Init(isSleepStart);
		if (i == 1)
		{
			int o = On(pin,isSleepStart);
			if (o == 1)
			{
				bInit = true;
			}
			return o;
		}
			
		return 0;
	}	
	return 1;
}

/*bool ATCommandWarp::Command(char* cmd, int len, char* out ,int count,BOOL isWating )
{
	if(!m_pCom)
		return false;

	char ans[1024] = {0};      // 应答串 
	int ansLen = 1024;
	bool result = false;
	for (int i = 0; i < count; ++i)
	{
		if (strstr(ans, "OK") == NULL)
		{
			m_pCom->WriteComm(cmd, len);  
			if(isWating)
				Sleep(1000);
			memset(ans, 0, 1024);
			int len = m_pCom->ReadComm(ans, 1024);     
			
			if (out != NULL)
			{
				memcpy(out, ans, len);     
				out[len] = '\0';
			}
		}
		else
		{
			result = true;
			break;
		}
	}
	return result;
}
*/

/*bool ATCommandWarp::Command(char* cmd, int len, char* out ,int count,BOOL isWating )
{
	if(!m_pCom)
		return false;
	
	char temp[1024] = {0};//临时数据串
	int length = 0 ;
	int num = 0 ;
	bool result = false;

	m_pCom->WriteComm(cmd, len);//发指令  
	
	while (true)
	{
		Sleep(20);
		m_pCom->ReadComm(temp, 1024);
		if (strstr(temp, "OK") != NULL)
		{	
			if (out != NULL)
			{
				memcpy(out+length,temp,strlen(temp));
			}
			result = true;
			break;
		}
		else if (strstr(temp, "ERROR") != NULL)
		{	
			if (count > 0 && num < count)
			{
				m_pCom->WriteComm(cmd, len);//发指令
				num++;
			}	
			else
			{
				if (out != NULL)
				{
					memcpy(out+length,temp,strlen(temp));
				}
				result = false;
				break;
			}

		}
		else//没读到OK
		{	
			if (strlen(temp) > 0)
			{
				if (out != NULL)
				{
					memcpy(out+length,temp,strlen(temp));
					length += strlen(out);
					memset(temp,0,1024);
				}

			}
			Sleep(10);
		}
	}	
	return result;
}
*/

bool ATCommandWarp::Command(char* cmd, int len, char* out ,int count,BOOL isWating )
{
	if(!m_pCom)
		return false;
	
	char temp[1024] = {0};//临时数据串
	char ReadOut[1024] = {0};//读出来的数据
	int length = 0 ;
	int num = 0 ;
	bool result = false;
	
	m_pCom->WriteComm(cmd, len);//发指令  
	
	while (true)
	{
	//	Sleep(20);
		int n = m_pCom->ReadComm(temp, 1024);
		
		if ( n > 0)
		{
			memcpy(ReadOut+length,temp,strlen(temp));
			length = strlen(ReadOut);
			memset(temp,0,1024);
		}
	
		if (strstr(ReadOut, "OK") != NULL)
		{	
			result = true;
			break;
		}
		else if (strstr(ReadOut, "ERROR") != NULL)
		{	
			if (count > 0 && num < count)
			{	
				Dprintf("WriteCount: %d\r\n", num);
				m_pCom->WriteComm(cmd, len);//发指令
				num++;					
				memset(ReadOut,0,1024);
				length = 0 ;
				Dprintf("WriteOver\r\n");
			}	
			else
			{	
				Dprintf("ErrorBreak\r\n");
				result = false;
				break;
			}
			
		}

	}	

	if (out != NULL)
	{
		strcpy(out,ReadOut);
	}

	return result;
}

void ATCommandWarp::Command_2(char *cmd,int len,char *out,int outlen ,char *stop)
{
	m_pCom->WriteComm(cmd,len);

	char ans[1024] = {0};
	int	 anslen = 1024;
	int  dread = 0 ; 

	do 
	{	
		memset(ans,0,1024);
		dread = m_pCom->ReadComm(ans,anslen);
		if (dread)
		{
			strcat(out,ans);
		}
	} while (!strstr(ans,stop));

}

bool ATCommandWarp::Command_1(char* cmd, int len)
{
	if(!m_pCom)
		return false;
	
	bool result = true;
	m_pCom->WriteComm(cmd, len);  
	return result;
}

int ATCommandWarp::Transaction(char* req, int reqLen, char* res, int resLen,int nCount,bool bwait,char *condition)
{
	if(!m_pCom)
		return false;
	
	EnterCriticalSection(&m_csAT);
	int dwNumWrite = 0;
	while (dwNumWrite < reqLen)
	{
		dwNumWrite += m_pCom->WriteComm(req + dwNumWrite, reqLen - dwNumWrite);
	}
	
	// add by qi 2009_12_04
	if (bwait)
	{	
		char ans[1024] = {0};
		int	 anslen = 1024;
		int  dread = 0 ; 
		do 
		{	
			memset(ans,0,1024);
			dread = m_pCom->ReadComm(ans,anslen);
			if (dread)
			{
				strcat(res,ans);
			}

		}while(!strstr(res,condition) && !strstr(res,"ERROR"));// while (!strstr(res,"OK"));
		
		LeaveCriticalSection(&m_csAT);

		return 1;
	}
	//

	int dwNumRead = 0;
	int nnCount = 0;
	while (true)
	{
		int count = m_pCom->ReadComm(res + dwNumRead, resLen - dwNumRead);
		dwNumRead += count;
		if ((nnCount > nCount) ||((dwNumRead > 0) && (count == 0)) || (strstr(res, "OK") != NULL) || (strstr(res, "ERROR") != NULL) || (strstr(res, "\r\n") != NULL))    //
		{
			if(nnCount > nCount)
				Dprintf("CountOut %d\r", nnCount);
			break;
		}
		nnCount++;
	}
	LeaveCriticalSection(&m_csAT);  

	return dwNumRead;
}
static std::string gOldSendCmd = "";
void ATCommandWarp::Transaction_(char* req, int reqLen ,char const *condition,OKStatus flag,bool head)
{	

	if (m_bInstance)//串口打开才可以写数
	{	
// 		if (head)//优先出去
// 		{	
// 			std::string data = req ;
// 			Util::ATCommand::Instance()->Write(data,condition);
// 			return ;
// 		}

/*		EnterCriticalSection(&m_csTranch);
		SendMsg msg ;
		msg.data = req;
		msg.flag = flag;
		if (head)		
			m_vMsg.push_front(msg);		
		else
			m_vMsg.push_back(msg);
		LeaveCriticalSection(&m_csTranch);
*/
		
		extern VOID WriteMyLog_(char *ptr, int size);
		if (m_okstatus == flag && flag != okNoHandle)
		{
		//	Dprintf("flag same\r\n");
		//	WriteMyLog_("flag same",strlen("flag same"));
			
			std::string data = req;
			Util::ATCommand::Instance()->Write(data,condition);	
		}
		else 
		{	
		//	Dprintf("\r\nAT_Begin:%s\r\n",req);
		//	Dprintf("\r\nTransaction_Begin\r\n",strlen("Transaction_Begin"));
		//	WriteMyLog_("Transaction_Begin",strlen("Transaction_Begin"));
			
			if(m_okstatus == okNoHandle && gOldSendCmd.find("+COPS=?") != std::string::npos)
			{
				return;
			}
			EnterCriticalSection(&m_csTranch);
		//	Dprintf("\r\nTransaction_In\r\n");
		//	WriteMyLog_("Transaction_In",strlen("Transaction_In"));
		//	Dprintf("\r\nAT_In:%s\r\n",req);
			static int n = 0;
			n = 1;
			for (;;)
			{
				OKStatus co = m_okstatus;
				if (co == okIdle)
				{
				//	Dprintf("\r\nTransaction_Write\r\n");
				//	WriteMyLog_("Transaction_Write",strlen("Transaction_Write"));
				//	SuspendThread(Util::ATCommand::Instance()->serialPort_->readThread_->handle_);
					EnterCriticalSection(&m_csOk);
					if (!m_bWaitCSQ)//在收网络运营商时候需要等待
					{	
					//	WriteMyLog_("Transaction_CSQ",strlen("Transaction_CSQ"));
						m_bWaitCSQ = head;
						std::string data = req;
						Util::ATCommand::Instance()->Write(data,condition);
						gOldSendCmd = data;
					//	WriteMyLog_("Transaction_Flag",strlen("Transaction_Flag"));
						m_okstatus = flag;	
					}
					LeaveCriticalSection(&m_csOk);
				//	ResumeThread(Util::ATCommand::Instance()->serialPort_->readThread_->handle_);
					break;
				}
				else
				{	
					if (!m_bWaitCSQ)
					{
						int nOffCount = 10*10; //10*120;
						if(m_okstatus == okSmsSend)
							nOffCount = 10*40;
						if (!(n++%(nOffCount)))
						{	
							n = 1;
							char log[50];
							sprintf(log,"status:%d",(int)m_okstatus);
							WriteMyLog_(log,strlen(log));//记录一下当前的状态

							m_okstatus = okIdle;
					//		WriteMyLog_("AT_Sleep",strlen("AT_Sleep"));
						}
					}
	
					/*if (!(n++%(10*120)))
					{	
						n = 1;
						m_okstatus = okIdle;
						WriteMyLog_("AT_Sleep",strlen("AT_Sleep"));
					}*/					
					Sleep(100);
				}
				
			}
			LeaveCriticalSection(&m_csTranch);
		}
		
	}	
}

char gIMEI[32]	= {0};
char gCGMR[1024] = {0};
char gDDTM[32] = {0};
int gIsTDStatus = 0;    //1 only T, 2 T 优先, 0 g 优先
BOOL gIsFind3GNet = TRUE;
void GetAutoNet(char *ptr)
{
	char *DDTM = strstr(ptr, "^DDTM:");

	if(strstr(ptr, "^DDTM: 1,2"))
	{
		gIsTDStatus = 1;
	}
	else if(strstr(ptr, "^DDTM: 0,2"))
		gIsTDStatus = 2;
	else if(
		strstr(ptr, "^DDTM: 0,0"))
		gIsTDStatus = 0;
	else
		gIsTDStatus = 3;
}

int ATCommandWarp::Init(BOOL isSleepStart)
{
	char AT[] = "AT\r";// 测试GSM-MODEM的存在性
	char ATZ[] = "ATZ\r";//复位
	char ATE[] = "ATE0\r";// ECHO OFF  
	char CGMR[] = "AT+CGMR\r";
	char CMGF[] = "AT+CMGF=0\r";// PDU模式
	char DDTM[] = "AT^DDTM?\r";
	char CSCS[] = "AT+CSCS=\"UCS2\"\r";
	char CPMS[] = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
	char CMNI[] = "AT+CNMI=2,1,0,0,0\r";
	char ATV[] = "ATV1\r";//1 <CR><LF><verbose code><CR><LF> 0 <numeric code><CR>	"ATV0\r";返回“0\r”（ATV指令决定返回result code是数字格式还是字符格式，ATV0 返回为数字格式，其中0 表示OK，1 表示CONNECT，2 表示RING，3 表示NO CARRIER，4 表示ERROR
	char ATUSBPOWER[] = "AT^DUSBPOWER=1\r";
//	char DMBR[] = "AT^DMBR=4\r";//设置波特率
	char DUSBDEG1[] = "AT^DUSBDEG=1,1\r";//返回“0\r”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DUSBDEG2[] = "AT^DUSBDEG=2,1\r";//返回“0\r”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DUSBDEG3[] = "AT^DUSBDEG=3,1\r";//返回“0\r”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DUSBDEG4[] = "AT^DUSBDEG=4,1\r";//返回“0\r”,（此指令为开启EP1 通信口，第一个参数从1-4 对应EP1-EP4，第二个参数1 表示open，0 表示close；在启用某	个通信口做业务之前需通过此指令开启对应通信口）
	char DGPIOSM[] = "AT^DGPIOSM=1\r";//r”，返回“0\r”（, 此指令为APP与模块唤醒模式选择）
	char DSLP[] = "AT^DSLP=0,0\r";
	char DSQ[] = "AT^DSQ=1\r";//返回“0\r”,（此指令为开启网络信号分贝值的上报）；
	char CRC[] = "AT+CRC=1\r";//返回“0\r”,（此指令为开启来电后的扩展指示，可选	输入指令）；
	char DSCI[] = "AT^DSCI=1\r";//，返回“0\r”,（此指令为开启进度指示，可选输入指令）；
	char DCPI[] = "AT^DCPI=1\r";//返回“0\r”, （此指令为开启呼叫状态指示，以拨打语	音电话10086 为例，开启此上报指示后，其后跟随DCPI 的上报指示，
	//“^DCPI:1,0,0,1”为建立连接指示；“^DCPI:1,3,0,1” 为正在呼叫指示；
	//“^DCPI:1,4,0,1” 为信道分配指示；“^DCPI:1,5,0,1” 为描述进度指示；
	//“^DCPI:1,2,0,1”为被叫振铃指示；“^DCPI:1,6,0,1”为呼叫建立指示，可选输入指令）；

	char DVTTYPE[] = "AT^DVTTYPE=3\r";
	char DVTABLE[] = "AT^DVTABLE=1,1,0\r";

	char DAUDSCE[] = "AT^DAUDSCE=?\r";
	char DAUDSCE0[] = "AT^DAUDSCE=0\r";
	char DAUDSCE1[] = "AT^DAUDSCE=1\r";


	char GSN[]  = "AT+GSN\r";
	
	bool result = true;
	char ans[1024] = {0};      // 应答串    

 	if (!Command(AT, strlen(AT)))
	{
		return 0;
	}
	
//	if(!isSleepStart)
//	Command(DMBR, strlen(DMBR));//设置波特率

	Command(ATZ, strlen(ATZ));
	Command(GSN, strlen(GSN), gIMEI, 3, TRUE);//IMEI
	Command(DDTM, strlen(DDTM), gDDTM, 3, TRUE);
	GetAutoNet(gDDTM);

	Command(CGMR,strlen(CGMR),gCGMR);//软件版本

	//新增
	
	//add by qi 20100927
	char DDEBUG[]="AT^DDEBUG=2,5\r";
	Command(DDEBUG, strlen(DDEBUG));

	Command(ATUSBPOWER, strlen(ATUSBPOWER));
	Command(DUSBDEG1, strlen(DUSBDEG1));
	Command(DUSBDEG2, strlen(DUSBDEG2));
	Command(DUSBDEG3, strlen(DUSBDEG3));
	Command(DUSBDEG4, strlen(DUSBDEG4));

	if(!isSleepStart)
	{
		if (Command(ATE, strlen(ATE))
			/*&& Command(ATUSBPOWER, strlen(ATUSBPOWER))
			&& Command(DUSBDEG1, strlen(DUSBDEG1))
			&& Command(DUSBDEG2, strlen(DUSBDEG2))
			&& Command(DUSBDEG3, strlen(DUSBDEG3))
			&& Command(DUSBDEG4, strlen(DUSBDEG4))*/
			&& Command(DGPIOSM, strlen(DGPIOSM))

	//		&& Command(DSLP, strlen(DSLP))
	//		&& Command(DSQ, strlen(DSQ))

			//视频电话
			&& Command(CRC, strlen(CRC))
			&& Command(DSCI, strlen(DSCI))
			&& Command(DCPI, strlen(DCPI)
			/*&& Command(DDEBUG, strlen(DDEBUG))*/)
	//		&& Command(DVTTYPE, strlen(DVTTYPE))
	//		&& Command(DVTABLE, strlen(DVTABLE))
			
			)
		{	
			return 1;
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

void ATCommandWarp::AdjustAudio()
{
	char DAUDTABW[] = "AT^DAUDTABW=4,1,5,0,2,aaaaaaaa0001020001006842684244018501fd01ef02cc039b055e06ea09ee096d111d0fd91dfb133030a60bbd78bd78a60b3030fb13d91d1d0f6d11ee09eb095e069b05cc03ef02fd0185014401f2ff1700fdfffffff2ff27ffaa007c02e8fef3006dfe4bffcef6db094be4c26ec26e4be4db09cef64bff6dfef300e8fe7c02aa0027fff2fffffffdff1700010004013800004c545c646c0010fe07200001000a00a000ff7fe001fe170000fe1700000910ff0301002640264001000000ff7fff7f1405b80b280032535b7ffc7fd007ea51c400ff7f6400fe17ff7f0100ff0706017eff000000000000000000000000000000000000000000ff000000000000\r";
	char DAUDTABW1[] = "AT^DAUDTABW=4,1,5,0,0,aaaaaaaa0001000001006842684211005a00d500b601290353051b08460bb70e9c127d1877207f2dc5490e2ee47ee47e0e2ec5497f2d77207d189c12b70e460b1b0853052903b601d5005a001100f2ff1700fdfffffff2ff27ffaa007c02e8fef3006dfe4bffcef6db094be4c26ec26e4be4db09cef64bff6dfef300e8fe7c02aa0027fff2fffffffdff1700010004011f00004c545c646c0010fe07200001000a00a000ff7fe001fe170000fe1700000910ff0301002640264001000000ff7fff7f1405b80b280032535b7ffc7fd007ea51c400ff7f6400fe17ff7f0100ff0706017eff000000000000000000000000000000000000000000ff000000000000\r";
	FILE *file = fopen("/flashdrv/video.txt", "r+b");
	BOOL isflag = TRUE;
	if(file)
	{
		while(!feof(file))
		{
			char temp[1024] = {0};
			fgets(temp, 1024, file);
			if(strstr(temp, "AT^DAUDTABW=4,1,5,0,2") || strstr(temp, "AT^DAUDTABW=4,1,5,0,0"))
			{
				Command(temp, strlen(temp));
				isflag = FALSE;
			}
		}
	}

	if(isflag)
	{
		Command(DAUDTABW, strlen(DAUDTABW));			// CTA 测试
		Command(DAUDTABW1, strlen(DAUDTABW1));
	}
}

int ATCommandWarp::On(char* pin, BOOL isSleepStart)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	char CREG1[] = "AT+CREG=1\r";//返回“0\r”（此指令为开启网络上报，可选输入指令）；
	char CREG0[] = "AT+CREG=0\r";//返回“0\r”（此指令为开启网络上报，可选输入指令）；
	char CMER[] = "AT+CMER=2,0,0,2\r";//r”，返回“0\r”（此指令为开启高层协议栈事件CIEV：	<ind>,<value>上报）；
	char CFUN5[] = "AT+CFUN=5\r";//返回“0\r”，（此指令为激活USIM 卡）；
	char CFUN1[] = "AT+CFUN=1\r";//，返回“0\r”，（此指令为激活协议栈）；
	char COPS[] = "AT+COPS=0\r";//返回“0\r”，(此指令为搜网指令),此后LC6311 会返
	//回很多数据，直到返回“+CREG：1”表示开机成功（“^DACTI:2”表示当前接入技术是TD; “+CIEV：2,2”表示信号等级为2 级；“+CIEV：8,0”表示短
	//消息存储未满；“+CREG：2”表示搜网中；“+CREG：1”表示搜网成功并注
	//册成功。【说明：“+CREG:n”其中“n”的值可为0[未注册]，1[注册成功]，2[搜网]，3[网络拒绝注册]，4[网络注册状态未知]，5[漫游]，开机后只有当CREG
	//返回的是1 或5 时，才能做LC6311 所支持的业务】；鉴于对CREG：4 的协议
	//理解，建议当+CREG：4 时，APP 处理器在UI 显示为可提供服务状态，即等
	//同于+CREG：1或+CREG:5】）。

	char CMGF[] = "AT+CMGF=0\r";// PDU模式
	char CSCS[] = "AT+CSCS=\"GSM\"\r";//UCS2//
	char CPMS[] = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";

	char COPS2[] = "AT+COPS=2\r";//停止收网

	char *CMNI = NULL;

	char CPIN[] = "AT+CPIN?\r"; //PIN 鉴定 此处用来判断是否插入SIM卡
	char CNUM[] = "AT+CNUM\r";//读手机号码
	char CSCA[] = "AT+CSCA?\r";//读短信中心号码
	char CIMI[] = "AT+CIMI\r";  //IMSI号码，用于判断是否属于中国移动
	
	char DSTM[] = "AT^DSTM?\r";//^DSTM: 0,0,0 OK 表明当前的网络为G网
							   //若为^DSTM: 0,0,2　OK表明当前的网络为T网

	char DUSIMR[] = "AT^DUSIMR = 1\r";//报告SIM卡的状态。add by qi 20100406
	char CSMS[] = "AT+CSMS=0\r";
	
	char CGMR[] = "AT+CGMR\r";//软件版本号

	//test
//	char DAUDI_[] = "AT^DAUDI=1,5\r";
//	char DAUDI[] = "AT^DAUDI=?\r";
//	char DAUDSCC[] = "AT^DAUDSCC=4,5,0\r";
//	char DAUDSCS[] = "AT^DAUDSCS=1\r";

	bool bSimInitOk = false;//SIM卡是否初始话好
	bool result = true;
	bool bwin = false;
	char ans[1024] = {0};      // 应答串    
	
	BOOL bIsUnSIM = FALSE;
	bool bpinInput = false;
	static bool bsim = true ;

	if(isSleepStart)
		goto END;

	if (!Command(CREG1, strlen(CREG1)))
	{
		return 0;
	}

	AdjustAudio();
	PhoneVolume(3);
	
	memset(ans, 0, 1024);

//	if (!Command(CFUN5, strlen(CFUN5), ans)) // old	
//	if (!CommandFun5(CFUN5, strlen(CFUN5), ans))//add by qi 2009_08_28
	if (!CommandFun5_(CFUN5, strlen(CFUN5), ans))//add by qi 2009_08_28
	{	
		if (strstr(ans, "+CME ERROR: 11") != NULL || strstr(ans, "+CME ERROR: 12") != NULL )//在设置开机PIN 码时，APP端输入“AT+CFUN=5”，激活USIM 卡，返回的“＋CME		ERROR：11”表示“SIM PIN Required”；此时APP 端会弹出提示输入pin码的提示，		APP端输入PIN 码后，再执行激活协议栈的指令。
		{
			if (!CheckPin())
			{
				return 0 ;
			}
			bpinInput = true;
		}
		else if(strstr(ans, "+CME ERROR: 10") != NULL )	//未检测SIM卡
		{	
			Command(CFUN1, strlen(CFUN1));
			Command(COPS, strlen(COPS));
			ReportNet(FALSE);
			RegisterNewCom();
			bIsUnSIM = TRUE;
			bsim = false ;
			HandleSIM(false,true);
			return 4;
		}
		else
		{	
			Command(CFUN1, strlen(CFUN1));
			Command(COPS, strlen(COPS));
			RegisterNewCom();

			HandleSIM(false);
			return 4;

		}
	}
	
	gIsFind3GNet = TRUE;

	if (bsim)
	{	
		if (bpinInput)
		{
			HandleSIM(true,true);
		}
		else
		{
			HandleSIM(true);
		}
		
	}
	
//  6311  lxz 20090807

#define TD6311 1
#if(TD6311 == 1)
	if(!(Command(CFUN1, strlen(CFUN1))
		&& Command(COPS, strlen(COPS))))
	{
		return 0;
	}
#else
	if(!(Command(CFUN1, strlen(CFUN1))))
	{
		return 0;
	}
#endif
	
	extern BOOL g_bAutoSearchNet ;

//	ReportNet(false);
	memset(ans, 0, 1024);
	m_pCom->ReadComm(ans, 1024);
 	while ((strstr(ans, "+CREG: 1") == NULL) && (strstr(ans, "+CREG: 5") == NULL))
	{	
		//add by qi 20100826,判断SIM卡是否初始话好
		if (strstr(ans,"^DCALLPI") != NULL)
		{
			bSimInitOk = true;
		}

		if(bIsUnSIM && strstr(ans, "+CREG: 3") != NULL)//没有检测到SIM卡
		{
			return 3;
		}
		else if (!bIsUnSIM && (strstr(ans,"+CREG: 3") != NULL || strstr(ans,"+CREG: 0") != NULL))//有SIM卡但是也没收到网络
		{	
			g_bAutoSearchNet = false;
			ReportNet(false);
			RegisterNewCom();
			Sleep(50);
			return 4;
		}
		else if (!g_bAutoSearchNet)//不是自动收网跳出去
		{
		//	Command(COPS2,strlen(COPS2));
			m_pCom->CloseComm();
			Util::ATCommand::Instance()->RegisterObserver(this);
			m_bInstance = Util::ATCommand::Instance()->m_bPortOpen;
			//FindPLMN();   //lxz 20100605
			return 0;
		}
		if(!gIsFind3GNet)			//退出找网			//lxz 20100623
			return 3;
		m_pCom->ReadComm(ans, 1024);
		Sleep(10);
	}
	gIsFind3GNet = FALSE;
	
	//add by qi 20100601
	g_bAutoSearchNet = false;

	if (strstr(ans, "+CREG: 1") != NULL || strstr(ans, "+CREG: 5") != NULL)//收网成功
	{	
		bwin = true;
		ReportNet(true);	
	}

	//add by qi 20100826,判断SIM卡是否初始话好
	if (!bSimInitOk)
	{	
		int ncount = 0;//超时
		while (strstr(ans,"^DCALLPI") == NULL && ncount < 500 /*超时5秒退出*/)
		{	
			memset(ans,0,1024);
			m_pCom->ReadComm(ans, 1024);
			Sleep(10);
			ncount++;
		}
		
		extern	VOID WriteMyLog_(char *ptr, int size);
		char log[] = {0};
		sprintf(log,"nount:%d",ncount);
		WriteMyLog_(log,strlen(log));

	}
	//

END:
	if(isSleepStart)
	{
		bwin = true;
		
		//判断SIM卡状态
		memset(ans, 0, 1024);
		char DUSIMR[] = "AT^DUSIMR=2\r";
		Command(DUSIMR,strlen(DUSIMR),ans);
		bool bsim = true;
		std::string d(ans);
		size_t t ;
		if ((t = d.find("^DUSIMR: ")) != std::string::npos)
		{
			d = d.substr(t+9);
			if (d.size())
			{
				CString c = Util::StringOp::ToCString(d);
				bsim = (bool)atoi(c);
			}
		}

		if (!bsim)//在深度睡眠的时候没插SIM卡
		{
			ReportNet(FALSE);
			RegisterNewCom();
			HandleSIM(false);
			return 4;	
		}
		else
		{
			ReportNet(true);
			HandleSIM(true);
		}
	
	}

	//add by qi 20105022
	//char DDEBUG[]="AT^DDEBUG=2,5\r";
	//Command(DDEBUG, strlen(DDEBUG),NULL,1);

	Command(CMER,strlen(CMER));
	Command(DUSIMR,strlen(DUSIMR));//报告SIM卡状态

	//add by qi 20100508
	Command(CREG0, strlen(CREG0));
	Command(CMGF, strlen(CMGF));
	Command(CSCS, strlen(CSCS));

	//等待^DCALLPI
	Command(CPMS, strlen(CPMS),ans,20);

	if(main->m_pMessageSet->SmsReceipt())
	{
		CMNI = "AT+CNMI=2,1,0,1,1\r";
	}
	else
	{
		CMNI = "AT+CNMI=2,1,0,0,0\r";
	}
	Command(CMNI, strlen(CMNI));

//
//	Command(DAUDSCS,strlen(DAUDSCS));
//	Command(DAUDSCC,strlen(DAUDSCC));
//	Command(DAUDI_,strlen(DAUDI_));
//	Command(DAUDI,strlen(DAUDI));

// 	if (!(Command(CREG0, strlen(CREG0))
// 		&& Command(CMGF, strlen(CMGF))
// 		&& Command(CSCS, strlen(CSCS))
// 		&& Command(CPMS, strlen(CPMS),ans,20)
// 		&& Command(CMNI, strlen(CMNI))))
// 	{
// 		return 0;
// 	}
	
	Command(CSMS,strlen(CSMS));

	char cmd_[16];// 命令串
	sprintf(cmd_, "AT+VTD=%d\r", 4); // 生成命令
    Command(cmd_,strlen(cmd_));
	
	memset(ans, 0, 1024);	
	//Transaction(CSCA, strlen(CSCA), ans, 1024,0,true,"+CSCA:");
	if (Command(CSCA, strlen(CSCA), ans))
	{
		std::string addr = ans;
		if (addr.find("+CSCA:") != std::string::npos)
		{
			unsigned int pos1 = addr.find("\"");
			if (pos1 != std::string::npos)
			{
				unsigned int pos2 = addr.find("\"", pos1 + 1);
				if (pos2 != std::string::npos)
				{
					pos1 += 1;
					if (pos2 - pos1 > 0)
					{
						m_strSmsCentreAddress = addr.substr(pos1, pos2 - pos1);	
						extern VOID WriteLog(char *ptr);
						WriteLog((char *)m_strSmsCentreAddress.c_str());
						main->m_pMessageSet->SmsCenterAddress(m_strSmsCentreAddress);
						main->m_pMessageSet->Update();
					}
				}
			}
		}	
	}

	//获得当前的网络运营商
	GetOperator();

	//短息自注册
	std::string simID;
	SimIdentification(simID);
	
	char log[50] = {0};
	sprintf(log,"simID:%s",simID.c_str());
	extern	VOID WriteMyLog_(char *ptr, int size);
	WriteMyLog_(log,strlen(log));
	
	extern BOOL IsTestCTA();
	extern BOOL isKaInterfaceTest();
	if(!IsTestCTA() && !isKaInterfaceTest())
	{	
		if (bwin)
		{
			std::vector<boost::shared_ptr<Data::Register> > vresult = Data::Register::GetFromDatabase("");
			if (!(vresult.empty()))
			{
				if(simID != vresult[0]->terminalId)
				{
					if(main->m_pSMSWarp->SendRegistDM())//短信自注册
					{
						vresult[0]->terminalId = simID;
						vresult[0]->Update();
					}
				}
			}			
		}
		Sleep(100);
		UnconditionalTransferStatus();//无条件转移

	}

	RegisterNewCom();
	
//	PhoneSetCallWaiting(true);
//	CallLimit("AI",true);//呼入限制
//	CallLimit("AO",true);//呼出限制	

	return 1;

}

void ATCommandWarp::Off(void)
{
	char COPS[] = "AT+COPS=2\r";//返回“+CREG: 0\r\n0\r”（此指令为去激活网络）；
	char CFUN0[] = "AT+CFUN=0\r";//返回“0\r”（此指令为去激活协议栈;
	char CFUN6[] = "AT+CFUN=6\r";//返回“0\r”（此指令为去激活USIM 卡；
	char DSOFF[] = "AT^DSOFF\r";//返回“0\r”（此指令为模块软关机；
	
	Transaction_(COPS,strlen(COPS));
	Transaction_(CFUN0,strlen(CFUN0));
	Transaction_(CFUN6,strlen(CFUN6));
	Transaction_(DSOFF,strlen(DSOFF));

}


void ATCommandWarp::Update(std::string const& data)
{	
//	ParseCMGL(data);
	ParseDialUp(data);
	ParseCLCK(data);
	ParseCPBS(data);
	ParseCPBR(data);
	ParseCCFC(data);
	ParseCCWA(data);
	ParseReceipt(data);
//	ParseQUSIM(data);
	ParseDUSIMR(data);
	ParseCPIN(data);
	ParseCRSM(data);
	ParseCSCA(data);
	ParseDTUSATURC(data);
	ParseCOPS(data);
	ParseCOPL(data);
//	ParseCREG(data);
	ParseDSACT(data);
	ParseCPMS(data);
	ParseDRAP(data);
	ParseError(data);
	ParseOK(data);

}

void ATCommandWarp::ParseQUSIM(std::string const& data)
{
	if(data.find("^DUSIMR:") != std::string::npos )
	{
		char *p = (char*)data.c_str();
		p = strstr(p, "^DUSIMR:");
		if(p != NULL)
		{
			p += 9;
			std::string s = p;
			CString value = Util::StringOp::ToCString(s);
			int v = atoi(value);
			if(v == 1)
				g_isSimEnable = TRUE;  //SIM卡可用
			else
				g_isSimEnable = FALSE;
		}
	}

}

void ATCommandWarp::ParseReceipt(std::string const& data)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(data.find("+CDS: 26") != std::string::npos)
	{
		Data::Message* msg = new Data::Message;
		size_t pos = 0;
		char *subStr = (char*)data.c_str();
		char *len = strstr(subStr, "+CDS: 26");
		pos = len - subStr;
		subStr += pos;
		len = strstr(subStr, "\r\n");
		pos = len - subStr;
		subStr += pos;
		if(pos != NULL)
		{
			pos += 2;
			subStr += 2;
			main->m_pSMSWarp->gsmDecodeReceiptPdu(subStr, &main->m_pSMSWarp->m_SmsReceipt);
		}
		
		CString str;
		CString time;
		
		str += L"信息在";
		
		time = Util::StringOp::ToCString(string(&main->m_pSMSWarp->m_SmsReceipt.toDest[0],2).c_str());
		msg->timestamp.year = atoi(time);
		time = Util::StringOp::ToCString(string(&main->m_pSMSWarp->m_SmsReceipt.toDest[2],2).c_str());
		msg->timestamp.month = atoi(time);
		time = Util::StringOp::ToCString(string(&main->m_pSMSWarp->m_SmsReceipt.toDest[4],2).c_str());
		msg->timestamp.day = atoi(time);
		time = Util::StringOp::ToCString(string(&main->m_pSMSWarp->m_SmsReceipt.toDest[6],2).c_str());
		msg->timestamp.hour = atoi(time);
		time = Util::StringOp::ToCString(string(&main->m_pSMSWarp->m_SmsReceipt.toDest[8],2).c_str());
		msg->timestamp.minite = atoi(time);
		time = Util::StringOp::ToCString(string(&main->m_pSMSWarp->m_SmsReceipt.toDest[10],2).c_str());
		msg->timestamp.second = atoi(time);
		
		str += Util::StringOp::ToCString(msg->timestamp.ToStringFormat_());
		str += L"送达";
		
		string number = string(&main->m_pSMSWarp->m_SmsReceipt.reciveAddress[0], strlen(main->m_pSMSWarp->m_SmsReceipt.reciveAddress));
		string name = main->GetName(number);
		
		if (!name.empty())
		{
			str += Util::StringOp::ToCString(name);	
		}
		else
		{
			str += main->m_pSMSWarp->m_SmsReceipt.reciveAddress;
		}

		if(!FindVideoDlg(FALSE))
		{
			main->m_pDeleteTipDlg->SetTitle(str, 5000,0,font_18);
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}

		//add by qi 0827
		if (m_okstatus != okSmsSend && m_okstatus != okChld)
		{
			SetOkStatus(okIdle);
		}
		
	}
}

void ATCommandWarp::ParseDialUp(std::string const& data)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nOffSize = data.find("+CGEV: ME DEACT \"IP\",");
	if(nOffSize != std::string::npos)
	{
		if(data.find("\"\"") != std::string::npos)
		{
			::PostMessage(main->m_hWnd, WM_DIAL_RETRY, 0, 0);
		}
	}
}

void ATCommandWarp::ParseCLCK(std::string const& data)
{	
	CString c;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (data.find("+CLCK:") != std::string::npos)
	{	
		std::string s = data ;
		size_t t ;
		int status ;
		t = s.find("+CLCK:");
		if ( t != std::string::npos)
		{
			s = s.substr(t+7);
			c = Util::StringOp::ToCString(s);
			status = atoi(c);
			PostMessage(main->m_pSettingDlg->m_pPinSetDlg->m_hWnd,WM_BLOCK_OK,status,2);

		/*	if ( 1 == status)
			{	
				main->m_pSettingDlg->m_pPinSetDlg->m_bPin = TRUE;
				main->m_pSettingDlg->m_pPinSetDlg->m_MJPGList.SetUnitIsDownStatus(41,true);
				main->m_pSettingDlg->m_pPinSetDlg->m_MJPGList.SetUnitIsDownStatus(42,false);
			}
			else
			{	
				main->m_pSettingDlg->m_pPinSetDlg->m_bPin = FALSE;
				main->m_pSettingDlg->m_pPinSetDlg->m_MJPGList.SetUnitIsDownStatus(41,false);
				main->m_pSettingDlg->m_pPinSetDlg->m_MJPGList.SetUnitIsDownStatus(42,true);
			}
			main->m_pSettingDlg->m_pPinSetDlg->m_MJPGList.Invalidate();*/

		}


	//	PostMessage(main->m_hWnd,WM_CLCK,m_type,status);	
		
	}
}

void ATCommandWarp::ParseCPBS(std::string const& data)
{	
	CString c;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (data.find("+CPBS:") != std::string::npos)
	{	
		std::string s = data;
		int all = 0 ;
		size_t t ;
		t = s.find("\"SM\"");
		if ( t != std::string::npos)
		{
			s = s.substr(t);
			t = s.find(',');
			if ( t != std::string::npos)
			{
				s = s.substr(t+1);
				c = Util::StringOp::ToCString(s);

				m_nContactNum = atoi(c);//SIM中已经存的数目
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CONTACT_NUM,m_nContactNum,0);
				t = s.find(',');
				if ( t != std::string::npos)
				{
					s = s.substr(t+1);//总数
					c = Util::StringOp::ToCString(s);
					all = atoi(c);//SIM中总的数目
					PostMessage(main->m_pContactDlg->m_hWnd,WM_SIM_CAPACITY,all,0);					
				}

			}

		}
		else 
		{	
			if (s.find("FD") != std::string::npos)
			{
				static std::string d = "";
				d = data ;
				::PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_PSTN_LIST,(WPARAM)&d,0);

			}
		}

	}

}

/*void ATCommandWarp::ParseCPBR(std::string const& data)
{	

	if ( data.find("+CPBR:") != std::string::npos)
	{
		//反馈的信息的格式
		
// 			AT+CPBR= 0,2
// 			+CPBR: 1,"13512509353",129,"502A5DE5",0
// 			+CPBR: 2,"12452",129,"7F57603B",0
// 			OK
		
		

		Dprintf("CPBR: %s\r\n",(char *)data.c_str());
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;	
		static bool bfirst = true ;
		
		std::string s = data ;
		std::string name ;
		std::string phone ;
		std::vector <SIM_FORMAT> vsim ; 	
		size_t t = 0 ;
		
		while ( (t = s.find("+CPBR:") )!= std::string::npos)
		{
			++m_index;
			SIM_FORMAT sim ;
			s = s.substr(t+1);
			t = s.find(',');
			sim.id = s.substr(6,t-6);//卡的序号
			
			s = s.substr(t+1);	
			t = s.find(',');//找电话号码
			phone = s.substr(1,t-2);
			
			s = s.substr(t+1);
			t = s.find(',');//找姓名
			s = s.substr(t+1);
			t = s.find(',');
			name = s.substr(1,t-2);
			
			//转化GB2312
			char *des = new char[150];
			unsigned char *temp = new unsigned char[100]; 
			memset(temp,0,150);
			memset(des,0,100);
			if(m_bgsm)
			{
				int tlength = gsmString2Bytes(name.c_str(),temp,strlen(name.c_str()));
				DecodeGsm(temp,des,tlength);
			}
			else
			{

				USC2toGB2312((char*)name.c_str(),des,strlen(name.c_str()));			

			}
			
			sim.telnum = phone;
			sim.name = des ;
			vsim.push_back(sim);
			if (des)
			{
				delete []des;
			}

			if (temp)
			{
				delete []temp; 
			}

			main->InsertSimToContact(vsim);
			if (vsim.size())
			{
				vsim.clear();
			}
			Sleep(20);
			
		}		
		
	}


}*/

void ATCommandWarp::ParseCPBR(std::string const& data)
{	
	static std::string d = "" ;
	static std::string d_ = "";
	if ( data.find("+CPBR:") != std::string::npos)
	{
		//反馈的信息的格式
		/*
			AT+CPBR= 0,2
			+CPBR: 1,"13512509353",129,"502A5DE5",0
			+CPBR: 2,"12452",129,"7F57603B",0
			OK
		*/
		
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;	
		
		d += data;
		if (d.find("OK") != std::string::npos)
		{
			d_ = d;
			PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBR_DATA,(WPARAM)&d_,0);	
			d = "";
		}
	}
	else if(d.size() > 0)
	{
		if (data.find("OK") != std::string::npos)
		{
			d_ = d;
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;	
			PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBR_DATA,(WPARAM)&d_,0);	
			d = "";
		}
	}
}


void ATCommandWarp::ParseCCFC(std::string const& data)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString c;
	if (data.find("+CCFC:") != std::string::npos)
	{	
		int status ;
		std::string s = data ;
		size_t t ;
		t = s.find("+CCFC:");
		if ( t != std::string::npos)
		{
			s = s.substr(t+7);
			
			c = Util::StringOp::ToCString(s);
			status = atoi(c);
			
			std::string number("") ;
			if ( 1 == status)
			{
				t = s.find(",");
				if ( t != std::string::npos)
				{
					s = s.substr(t+1);
					t = s.find(",");
					if ( t != std::string::npos)
					{
						s = s.substr(t+2);
						t = s.find(",");
						if ( t != std::string::npos)
						{
							std::string number;
							number = s.substr(0,t-1);
							SendMessage(main->m_pSettingDlg->m_pCallSetDlg->m_hWnd,
								WM_CALLTRANSFER,status,(LPARAM)&number);
						}
					}

				}
			}
			else if ( 0 == status)
			{
				SendMessage(main->m_pSettingDlg->m_pCallSetDlg->m_hWnd,
					WM_CALLTRANSFER,status,0);
			}
			
		}
		SetOkStatus(okIdle);		
	}	
}

void ATCommandWarp::ParseCCWA(std::string const& data)
{	
	CString c;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (data.find("+CCWA:") != std::string::npos)
	{	
		int status = 0 ;
		std::string s = data;
		size_t t = s.find("+CCWA:");
		if ( t != std::string::npos)
		{
			s = s.substr(t+7);
			
			//检查后面的数据,如果还有数据不处理
			t = s.find(',');
			if (t != std::string::npos)
			{	
				if (1 == t)
				{
					c = Util::StringOp::ToCString(s);
					status = atoi(c);
					if ( status == 0 || status == 1)
					{
						PostMessage(main->m_pSettingDlg->m_pCallSetDlg->m_hWnd,WM_CALLTRANSFER,status,0);
					}
				}
			}

		}
		
	}

}

void ATCommandWarp::ParseAnswerData(char *src,bool bgsm)
{	
	//反馈的信息的格式
	/*
	AT+CPBR= 0,2
	+CPBR: 1,"13512509353",129,"502A5DE5",0
	+CPBR: 2,"12452",129,"7F57603B",0
	OK
	*/
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;	
	std::string s(src) ;
	std::string name ;
	std::string phone ;
	std::vector <SIM_FORMAT> vsim ; 	

	size_t t = 0 ;
	
	while ( (t = s.find("+CPBR:"))!= std::string::npos)
	{	
		SIM_FORMAT sim ;
		s = s.substr(t+1);
		t = s.find(',');
		
		if (t != std::string::npos)
		{
			sim.id = s.substr(6,t-6);//卡的序号			
			s = s.substr(t+1);
			t = s.find(',');//找电话号码
			if (t != std::string::npos)
			{
				phone = s.substr(1,t-2);				
				s = s.substr(t+1);
				t = s.find(',');//找姓名
				if (t != std::string::npos)
				{
					s = s.substr(t+1);
					t = s.find(',');					
					if (t != std::string::npos)
					{
						name = s.substr(1,t-2);						
						//转化GB2312
						char *des = new char[150];
						unsigned char *temp = new unsigned char[200]; 
						memset(temp,0,200);
						memset(des,0,150);
						if(bgsm)
						{
							int tlength = gsmString2Bytes(name.c_str(),temp,strlen(name.c_str()));
							DecodeGsm(temp,des,tlength);
						}
						else
						{
							USC2toGB2312((char*)name.c_str(),des,strlen(name.c_str()));		
						}
						
						sim.telnum = phone;
						sim.name = des ;

						vsim.push_back(sim);
						main->InsertSimToContact(vsim);
						if (vsim.size())
						{
							vsim.clear();
						}
						delete []des;
						delete []temp; 						
					}
						
				}
			
			}

		}

	}
	
}

void ATCommandWarp::ParseData(char const *src,std::vector<SIM_FORMAT> &vsim)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;	
	std::string s = src ;
	std::string name ;
	std::string phone ;
	
	size_t t = 0 ;
	
	while ( (t = s.find("+CPBR:"))!= std::string::npos)
	{	
		SIM_FORMAT sim ;
		s = s.substr(t+1);
		t = s.find(',');
		
		if (t != std::string::npos)
		{
			sim.id = s.substr(6,t-6);//卡的序号			
			s = s.substr(t+1);
			t = s.find(',');//找电话号码
			if (t != std::string::npos)
			{
				phone = s.substr(1,t-2);				
				s = s.substr(t+1);
				t = s.find(',');//找姓名
				if (t != std::string::npos)
				{
					s = s.substr(t+1);
					t = s.find(',');					
					if (t != std::string::npos)
					{
						name = s.substr(1,t-2);						
						//转化GB2312
						char *des = new char[150];
						unsigned char *temp = new unsigned char[200]; 
						memset(temp,0,200);
						memset(des,0,150);	
						
						USC2toGB2312((char*)name.c_str(),des,strlen(name.c_str()));		
												
						sim.telnum = phone;
						sim.name = des ;
						
						vsim.push_back(sim);	
						delete []des;
						delete []temp; 						
					}
					
				}
				
			}			
		}		
	}
}

void ATCommandWarp::ParseDUSIMR(std::string const& data)
{
	if (data.find("^DUSIMU: 1") != std::string::npos)//插入SIM卡
	{	
		g_isSimEnable = true;
		HandleSIM(true);
	}
	else if (data.find("^DUSIMU: 0") != std::string::npos)//未插入SIM
	{
		g_isSimEnable = false;
		HandleSIM(false);
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_NOSIM_WARNING,0,0);
	}	
}

void ATCommandWarp::ParseError(std::string const& data)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if (data.find("ERROR") != std::string::npos)
	{	
		if ( okDdtm == GetOkStatus())
		{	
			if (data.find("+CME ERROR: 3") != std::string::npos 
			|| data.find("+CME ERROR: 4") != std::string::npos)
			{
				main->m_pDeleteTipDlg->SetTitle(L"网络设置失败");
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			}
			SetOkStatus(okIdle);
		}
		else if (okDsatref == GetOkStatus())
		{	
			if (data.find("+CME ERROR: 3") != std::string::npos 
				|| data.find("+CME ERROR: 4") != std::string::npos)
			{
				main->m_pDeleteTipDlg->SetTitle(L"操作不支持");
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);	
			}
			SetOkStatus(okIdle);
		}			
		else if (GetOkStatus() == okOpenPin || GetOkStatus() == okClosePin ||
			GetOkStatus() == okChangepin || GetOkStatus() == okInputpin )
		{	
			if ( data.find("+CME ERROR: 12") != std::string::npos
			|| data.find("+CME ERROR: 316") != std::string::npos)//需要输入PUK
			{
				//输入PUK界面
				if (GetOkStatus() == okOpenPin)//在打开PIN码的操作过程中，连续三次PIN码错误，PIN自动开启
				{
					PostMessage(main->m_pSettingDlg->m_pPinSetDlg->m_hWnd,WM_BLOCK_OK,1,2);
				}
					
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PUK,0,10);
								
			}
			else if (GetOkStatus() == okOpenPin || GetOkStatus() == okClosePin)
			{
				PostMessage(main->m_pSettingDlg->m_pPinSetDlg->m_hWnd,WM_BLOCK_OK,0,0);
			}
			else //PIN码错误 
			{
				main->m_pDeleteTipDlg->SetTitle("PIN码输入错误");
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			}
					
		}
		else if ( GetOkStatus() == okPuk )//处理Puk
		{	
			main->m_pDeleteTipDlg->SetTitle("PUK码输入错误");
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
		else if ( GetOkStatus() == okPuk2)
		{
			main->m_pDeleteTipDlg->SetTitle("PUK2码输入错误");
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
		else if (GetOkStatus() == okChangepin2 || GetOkStatus() == okDsactDeactive 
					|| GetOkStatus() == okDsactActive || GetOkStatus() == okInputpin2)
		{
			if ( data.find("+CME ERROR: 18") != std::string::npos
				|| data.find("+CME ERROR: 318") != std::string::npos)//需要输入PUK2
			{
				//输入PUK界面
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PUK2,0,10);
				
			}
			else //PIN码错误 
			{
				main->m_pDeleteTipDlg->SetTitle("PIN2码输入错误");
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			}		
		}
		else if (GetOkStatus() == okCpbwAdd)
		{	
			SetOkStatus(okIdle);
			if (data.find("+CME ERROR: 17") != std::string::npos)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PIN2,0,3);
			}
			else if (data.find("+CME ERROR: 18") != std::string::npos)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PUK2,0,10);
			}
			else
			{
				PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBW_ADD,0,0);
			}
		
		}
		else if (GetOkStatus() == okCpbwChange)
		{	
			SetOkStatus(okIdle);
			if (data.find("+CME ERROR: 17") != std::string::npos)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PIN2,0,3);
			}
			else if (data.find("+CME ERROR: 18") != std::string::npos)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PUK2,0,10);
			}
			else
			{
				PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBW_CHANGE,0,0);
			}
			
		}
		else if (GetOkStatus() == okCpbwDelete)
		{	
			SetOkStatus(okIdle);
			PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBW_DELETE,0,0);			
		}
		else if (GetOkStatus() == okCrsm)
		{	
			main->m_pDeleteTipDlg->SetTitle("有效期设置失败");
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
		else if (GetOkStatus() == okCimi)
		{
			//sim没插入
			HandleSIM(false,1);
		}
		else if (GetOkStatus() == okCsqSearch || okCsqRegister == GetOkStatus())
		{
			m_bWaitCSQ = false;
			PostMessage(main->m_hWnd,WM_CSQ_REGISTER,0,0);
		}
		else if	(GetOkStatus() == okCpolDel)
		{
			PostMessage(main->m_pSettingDlg->m_pUPLNDlg->m_hWnd,WM_COPL_DEL,0,0);
		}
		else if	(GetOkStatus() == okCpolAdd)
		{
			PostMessage(main->m_pSettingDlg->m_pUPLNDlg->m_hWnd,WM_COPL_ADD,0,0);
		}
		else if	(GetOkStatus() == okCpolChange)
		{
			PostMessage(main->m_pSettingDlg->m_pUPLNDlg->m_hWnd,WM_COPL_CHANGE,0,0);
		}
		else if (GetOkStatus() == okCpbsSelect)
		{
			SetOkStatus(okIdle);
			PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBS_SELECT,0,0);
		}
		else if (GetOkStatus() == okChangecCsca)
		{
			main->m_pDeleteTipDlg->SetTitle(L"短信中心设置失败");
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);	
		}
		else if ( GetOkStatus() == okCnmiOpen || GetOkStatus() == okCnmiClose)//短息回执设置
		{
			main->m_pDeleteTipDlg->SetTitle(L"短息回执设置失败");
			main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		}
		else if (GetOkStatus() == okAtd)//
		{
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_ATD_ERROR,0,0);
		}
		else if(data.find("ERROR: 11") != std::string::npos)//输入PIN码
		{
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_APP_INPUT_PIN,0,3);
		}
		else if (okCcfcregister == GetOkStatus() || okCcfcCancel == GetOkStatus()
				|| okCcfcCheck == GetOkStatus() || okCcwaAble == GetOkStatus() || 
				okCcwaCancel == GetOkStatus() || okCcwaCheck == GetOkStatus() ||
				okCcfcUnconditionReg == GetOkStatus() ||
				okCcfcUnconditionCancel == GetOkStatus())
		{	
			SetOkStatus(okIdle);
			PostMessage(main->m_pSettingDlg->m_pCallSetDlg->m_hWnd,WM_CALLTRANSFER,2,0);//操作不允许				
		}
		else if(data.find("+CME ERROR: 100") != std::string::npos ||
				data.find("+HTCERROR:") != std::string::npos)
		{	
			if (okChld == GetOkStatus())
			{	
				main->m_pWarningNoFlashDlg->SetTitle(L"不能进行此操作!");
				main->m_pWarningNoFlashDlg->ShowWindow(SW_SHOW);
			}
			m_bWaitCSQ = false;
		}
			
		if (GetOkStatus() != okSmsSend)
		{
			SetOkStatus(okIdle);

		}
	}			
}

void ATCommandWarp::ParseOK(std::string const& data)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (data.find("OK") != std::string::npos )
	{
	//	Dprintf("Insert_OK\r\n");
	//	Dprintf("Status:%d\r\n",m_okstatus);
		switch (GetOkStatus())
		{	
			case okCimi:
				SetOkStatus(okIdle);
				HandleSIM(true,1);
				break;
			case okChld:
				SetOkStatus(okIdle);
				::PostMessage(main->m_pTelphoneDialDlg->m_hWnd,WM_CHLD_OK,0,0);
				break;
			case okCcfcUnconditionReg:
				{	
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pCallSetDlg->m_hWnd,WM_CALLTRANSFER,1,0);
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_UNCONDITIONALTRANSFER,1,0);
				}
				break;
			case okCcfcUnconditionCancel:
				{	
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pCallSetDlg->m_hWnd,WM_CALLTRANSFER,0,0);
					PostMessage(theApp.m_pMainWnd->m_hWnd,WM_UNCONDITIONALTRANSFER,0,0);
				}
				break;
			case okCcwaAble:
			case okCcfcregister:
				SetOkStatus(okIdle);
				PostMessage(main->m_pSettingDlg->m_pCallSetDlg->m_hWnd,WM_CALLTRANSFER,1,0);
				break;

			case okCcwaCancel:
			case okCcfcCancel:
				SetOkStatus(okIdle);
				PostMessage(main->m_pSettingDlg->m_pCallSetDlg->m_hWnd,WM_CALLTRANSFER,0,0);
				break;

			case okDdtm:
				SetOkStatus(okIdle);
				main->m_pDeleteTipDlg->SetTitle(L"网络设置成功,重启话机才生效!");
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				break;
			case okOpenPin://输入PIN
			case okClosePin:
				{	
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pPinSetDlg->m_hWnd,WM_BLOCK_OK,0,1);
				}
				break;
			case okPuk:
				{	
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("PUK输入正确");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
				break;

			case okPuk2:
				{	
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("PUK2输入正确");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
				break;
			case okInputpin:
				{	
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("PIN输入正确");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
				break;
			case okInputpin2:
				{	
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("PIN2输入正确");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
				break;
			
			case okChangepin:
				{
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("PIN码修改成功");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
				break;
			case okChangepin2:
				{
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("PIN2码修改成功");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				}
				break;
			case okCrsm:
				{
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("有效期设置成功");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					PostMessage(main->m_pSmsSettingDlg->m_hWnd,WM_VP_OK,0,0);
				}
				break;
			case okCnmiOpen:
				{
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle(L"短息回执设置成功");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					PostMessage(main->m_pSmsSettingDlg->m_hWnd,WM_CNMI_OPEN,1,0);
				}
				break;
			case okCnmiClose:
				{
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle(L"短息回执设置成功");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					PostMessage(main->m_pSmsSettingDlg->m_hWnd,WM_CNMI_CLOSE,1,0);
				}
				break;
			case okCpolDel:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pUPLNDlg->m_hWnd,WM_COPL_DEL,1,0);
				}
				break;
			case okCpolAdd:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pUPLNDlg->m_hWnd,WM_COPL_ADD,1,0);
				}
				break;
			case okCpolChange:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pUPLNDlg->m_hWnd,WM_COPL_CHANGE,1,0);
				}
				break;
			case okCpbsSelect:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBS_SELECT,1,0);
				}
				break;
			case okCpbwAdd:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBW_ADD,1,0);
				}
				break;
			case okCpbwChange:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBW_CHANGE,1,0);
				}
				break;
			case okCpbwDelete:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBW_DELETE,1,0);
				}
				break;
			case okDsactDeactive:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_hWnd,WM_DSACT_DEACTIVE,0,0);
				}
				break;

			case okDsactActive:
				{
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_hWnd,WM_DSACT_ACTIVE,0,0);
				}
				break;

			case okChangecCsca:
				{
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("短信中心设置成功");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					PostMessage(main->m_pSmsSettingDlg->m_hWnd,WM_CSCA_SET_OK,0,0);
				}
				break;
			case okCpbr:
				{	
					SetOkStatus(okIdle);
					PostMessage(main->m_pSettingDlg->m_pPstnNumberDlg->m_hWnd,WM_CPBR_OK,0,0);
				}
				break;
			case okCsqRegister:
				{
					SetOkStatus(okIdle);
					m_bWaitCSQ = false;
					main->m_pTipDlg->SetTitle("网络注册成功!");
					main->m_pTipDlg->ShowWindow_(SW_SHOW);
					
					main->m_pSettingDlg->m_pCallSetDlg->ShowWindow_(SW_HIDE);
					main->m_pSettingDlg->m_bManualSearchNet = true;

					main->m_pWarningNoFlashDlg->ShowWindow_(SW_HIDE);

					PostMessage(main->m_hWnd,WM_CSQ_REGISTER,1,0);

				}
				break;
			case okCsqSearch:
				{
					SetOkStatus(okIdle);
					main->m_pDeleteTipDlg->SetTitle("网络注册成功!");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					
					main->m_pSettingDlg->m_pCallSetDlg->ShowWindow_(SW_HIDE);
					main->m_pSettingDlg->m_bManualSearchNet = false;

				}
				break;
			case okDsatref:
				{
					if (data.find("^DTUSATURC:") != std::string::npos)
					{
						//有数据解析数据
						main->m_pCstkDlg->ParseSTK(data.c_str());
						main->m_pCstkDlg->ShowWindow_(SW_SHOW);
						main->m_pDeleteTipDlg->ShowWindow_(SW_HIDE);
						SetOkStatus(okIdle);
					}
					else
					{
						//没数据去查询数据
						char CSIM[] = "AT+CSIM=22,A010000006FFFFFFFFFFFF\r";
						Util::ATCommand::Instance()->Write(CSIM,"");
					//	CSIM();
					}
				}
				break;
			
			case okAtd:
			case okCcfcCheck:
			case okCcwaCheck:
			case okNoHandle:
				{
			//		Dprintf("Parse_OnHandle\r\n");
					SetOkStatus(okIdle);
				}
				break;

			case okCTA:
				SetOkStatus(okIdle);
				extern BOOL IsTestCTA();	
				extern void EditPrintf(char *ptr);
				if (!IsTestCTA())
				{
					EditPrintf((char *)data.c_str());					
				}
				break;

// 			case OkCsim:
// 				{
// 					m_okstatus = okIdle;
// 					if (data.find("^DTUSATURC:") != std::string::npos)
// 					{
// 						//有数据解析数据
// 						main->m_pCstkDlg->ParseSTK(data.c_str());
// 						
// 					}
// 					else
// 					{
// 						//没数据去查询数据
// 						//m_okstatus = OkCsim;
// 						//CSIM();
// 					}
// 				}
// 				break;

			default:
			//	m_okstatus = okIdle;
				break;
		}
	}
}

void ATCommandWarp::ParseCMGL(std::string const& data)
{	
	if (data.find("+CMGL: ") != std::string::npos )
	{		
		char *buff = new char[data.size() + 10];
		memset(buff,0,data.size() + 10);
		memcpy(buff,data.c_str(),data.size());	
		SMS::SMSWarp::GetSMSWarp()->ParseCMGL_(data.c_str());
		delete buff;
		buff = NULL;		
	}

}

void GetPinCount(char *ans, int &nPin1, int &nPin2, int &nPuk1, int &nPuk2)
{
	nPin1 = 3;
	nPin2 = 3;
	nPuk1 = 10;
	nPuk2 = 10;
	
	if(strstr(ans,"^DRAP:") != NULL)
	{
		std::string s = ans ;
		int l ;
		l = s.find("^DRAP:");
		if(l >= 0)
		{
			s = s.substr(l+7);
			nPin1 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
		}
		
		l = s.find(",");
		if(l >= 0)
		{
			s = s.substr(l+1);
			nPin2 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
		}
		
		l = s.find(",");
		if(l >= 0)
		{
			s = s.substr(l+1);
			nPuk1 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
		}
		
		l = s.find(",");
		if(l >= 0)
		{
			s = s.substr(l+1);
			nPuk2 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
		}
	}
}

void ATCommandWarp::ParseCPIN(std::string const& data)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (data.find("+CPIN: READY") != std::string::npos )//已经输入过PIN码
	{
		 main->m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowPinLockView();
		// int p1, p2, pu1, pu2;
		// GetPinCount((char *)data.c_str(), p1, p2, pu1, pu2);
		 main->m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->SetErrorCount(g_p1);
		 main->m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowWindow_(SW_SHOW);
	}	
	else if (data.find("+CPIN: SIM PIN") != std::string::npos)//需要输入PIN码
	{
	//	int p1, p2, pu1, pu2;
	//	GetPinCount((char *)data.c_str(), p1, p2, pu1, pu2);
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_APP_INPUT_PIN,0,g_p1);		
	}
	else if (data.find("+CPIN: SIM PUK") != std::string::npos)//需要输入PUK码
	{
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PUK,0,g_pu1);
	//	main->m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowPukInputView();
	//	main->m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->SetErrorCount(g_pu1);
	//	main->m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowWindow_(SW_SHOW);	
	}

}

void ATCommandWarp::ParseCRSM(std::string const&data)
{
	if ( data.find("+CRSM: ") != std::string::npos )
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

		std::string s = data ;
		size_t t = data.find(",");
		
		if (t != std::string::npos)//先把数据段保存起来
		{
			s = data.substr(t+1);
			t = s.find(",");			
			if (t != std::string::npos)
			{				
				s = s.substr(t+1);
				if (t != std::string::npos)
				{
					t = s.find("\r\n");				
					m_strSMSP = s.substr(0,t);//真正的数据包
					
					std::string svp = m_strSMSP.substr(m_strSMSP.size()-2,2);
					
					if (main->m_pSmsSettingDlg->m_vpState != CSMSSettingDlg::restorevp)
					{
						m_svp = svp;
					}

					unsigned char vp;
					SMS::SMSWarp::GetSMSWarp()->gsmString2Bytes(svp.c_str(),&vp,2);
					
					static CString cvp;
					int level ;
					if ( vp >= 0 && vp <= 143)
					{	
						level = 0 ;
						cvp.Format(L"%d分钟",(vp + 1)*5);//分钟
					}
					else if (vp > 143 && vp <= 167)
					{	
						level = 1 ;
						cvp.Format(L"%d小时",12 + (vp-143)/2);//小时
					}
					else if ( vp > 167 && vp <= 196)
					{	
						level = 2 ;
						cvp.Format(L"%d天",vp-166);//天
					}
					else if (vp > 196 && vp <= 255)
					{
						level = 3 ;
						cvp.Format(L"%d周",vp-192);//周
					}

					CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
					
					PostMessage(main->m_pSmsSettingDlg->m_hWnd,WM_SMS_VP,(WPARAM)&cvp,level);
					//main->m_pSmsSettingDlg->m_MJPGList.SetUnitText(63,cvp,true);
				}				
			}
		}
				
	}
}

void ATCommandWarp::ParseCSCA(std::string const&data)
{
	if (data.find("+CSCA:") != std::string::npos)
	{
		std::string s = data ;
		size_t t = s.find("+CSCA:");
		if (t != std::string::npos)
		{
			s = s.substr(t+8);
			t = s.find(",");
			if (t != std::string::npos)
			{
				s = s.substr(0,t-1);				

				CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;			
				static std::string d = "";
				d = s;
				::PostMessage(main->m_pSmsSettingDlg->m_hWnd,WM_SMS_CSCA,(WPARAM)&d,0);
			}
		}

	}
}

void ATCommandWarp::ParseDTUSATURC(std::string const&data)
{
	if (data.find("^DTUSATURC:") != std::string::npos)
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pCstkDlg->ParseSTK(data.c_str());
		main->m_pCstkDlg->ShowWindow_(SW_SHOW);
		main->m_pDeleteTipDlg->ShowWindow_(SW_HIDE);
		SetOkStatus(okIdle);
	}
}

void ATCommandWarp::ParseCOPS(std::string const&data)
{
	if (data.find("+COPS:") != std::string::npos)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pDeleteTipDlg->OnButtonDeleteTipCancel();
		main->m_pSettingDlg->m_pCallSetDlg->m_cmbPlmn.ResetContent();
		
		std::string oldOp;
		std::string s = data ;
		size_t t ;
		s = s.substr(7);
		t = s.find("(");
		while (t != std::string::npos)
		{	
			int status;
			int act;
			std::string olds;
			CString c ;
			olds = s.substr(t);
			s = s.substr(t+1);
			c =  Util::StringOp::ToCString(s);
			status = atoi(c);//网络运营商状态

			t = s.find("\",\"");
			if (t != std::string::npos)
			{
				s = s.substr(t+3);
				t = s.find("\",\"");

				if (t != std::string::npos)
				{
					s = s.substr(t+3);

					t = s.find(",");
					if (t != std::string::npos)
					{	
						s = s.substr(t+1);
						c =  Util::StringOp::ToCString(s);
						act = atoi(c);//网络类型 
						
						t = olds.find(")");
						if ( t != std::string::npos)
						{
							std::string d = olds.substr(0,t);
							std::string ct ;
							if ( 0 == act || 1 == act )//G网
							{
								ct = "(G网)";	
							}
							else if ( 2 == act)//T网
							{
								ct = "(T网)";	
							}
							d = d + ct + ")";  
							
							CString temp = Util::StringOp::ToCString(d);
							main->m_pSettingDlg->m_pCallSetDlg->m_cmbPlmn.AddString(temp);	
						}
					
						
					}
				}


			}
			t = s.find("(");
		}
	
		m_bWaitCSQ = false;
		main->m_pWarningNoFlashDlg->ShowWindow_(FALSE);
		main->m_pSettingDlg->m_pCallSetDlg->m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\网络运营商列表.xml");
		main->m_pSettingDlg->m_pCallSetDlg->m_uiType = 4;
		main->m_pSettingDlg->m_pCallSetDlg->SetCallSetParam();
		main->m_pSettingDlg->m_pCallSetDlg->ShowWindow_(SW_SHOW);
	}
}

void ATCommandWarp::ParseCREG(std::string const&data)
{
	
	if (data.find("+CREG: 1") != std::string::npos)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pDeleteTipDlg->SetTitle(L"搜网成功");
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}
	else if (data.find("+CREG: 3") != std::string::npos)
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pDeleteTipDlg->SetTitle(L"网络注册失败");
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);	
	}
}

void ATCommandWarp::ParseCOPL(std::string const&data)
{
	static std::string d = "";
	static std::string d_ = "";
	if (data.find("+CPOL: ") != std::string::npos && data.find("OK") != std::string::npos)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	//	d = data;
		if(d.size() > 0)
			d += data;
		else
			d = data;
		d_ = d;
		::PostMessage(main->m_pSettingDlg->m_pUPLNDlg->m_hWnd,WM_UPLMN_LIST,(WPARAM)&d_,0);
		d = "";
	}
	else if(data.find("+CPOL: ") != std::string::npos)
	{ 
		if(d.size() > 0)
			d += data;
		else
			d = data;
	}
	else if(d.size() > 0 && data.find("OK") != std::string::npos)
	{
		d_ = d;
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;	
		::PostMessage(main->m_pSettingDlg->m_pUPLNDlg->m_hWnd,WM_UPLMN_LIST,(WPARAM)&d_,0);
		d = "";
	}
}

void ATCommandWarp::ParseDSACT(std::string const&Data)
{
	if (Data.find("^DSACT: ") != std::string::npos)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		CString c ;
		std::string s = Data;
		size_t t = s.find("^DSACT: ");
		s = s.substr(t+8);
		c = Util::StringOp::ToCString(s);
		int status = atoi(c);
		PostMessage(main->m_pSettingDlg->m_hWnd,WM_FD_STATATUS,status,0);
	}
}

void ATCommandWarp::ParseDRAP(std::string const&Data)
{
	if (Data.find("^DRAP: ") != std::string::npos && g_IsFDNFindDrap)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		static std::string d = "";
		d = Data ;
		PostMessage(main->m_pSettingDlg->m_hWnd,WM_DRAP_DATA,WPARAM(&d),0);
	}
	else if(Data.find("^DRAP: ") != std::string::npos && !g_IsFDNFindDrap)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		
		g_p1 = g_p2 = g_pu1 = g_pu2 = -1;
		GetPinCount((char *)Data.c_str(), g_p1, g_p2, g_pu1, g_pu2);
		PostMessage(main->m_pSettingDlg->m_pPinSetDlg->m_hWnd,WM_DRAP_STATUS,0,m_DrapStatus);
	}
}

void ATCommandWarp::ParseCPMS(std::string const&data)
{
	if (data.find("+CPMS: ") != std::string::npos)
	{	
		uchar status = 0 ;
		uchar total ;
		size_t t = data.find("+CPMS: ");
		std::string s = data.substr(t+7);
		CString c ;
		c = Util::StringOp::ToCString(s);
		status = atoi(c);
		
		//找总数
		t = s.find(",");
		if ( t != std::string::npos)
		{
			s = s.substr(t+1);
			c = Util::StringOp::ToCString(s);
			total = atoi(c);
			
			if (status == total)//sim满了
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_FULL,0,0);	
			}
		}
	}
}

void ATCommandWarp::ParseTest(std::string const&data)
{
	if (data.find("^DCALLPI") != std::string::npos)//
	{
		CPMS();
	}
}

void ATCommandWarp::SetOkStatus(OKStatus oks)
{	
	EnterCriticalSection(&m_csOk);
	m_okstatus = oks;
	LeaveCriticalSection(&m_csOk);
}

OKStatus ATCommandWarp::GetOkStatus()
{	
	EnterCriticalSection(&m_csOk);
	OKStatus ok = m_okstatus;
	LeaveCriticalSection(&m_csOk);
	return ok;
} 


void ATCommandWarp::SmsWrite2Sim(int smsLength,int sta)
{
	char CMGW[100];
	memset(CMGW,0,100);
	sprintf(CMGW,"AT+CMGW = %d,%d\r",smsLength,sta);
//	Sleep(200);
	Transaction_(CMGW,strlen(CMGW),"",okSmsSend);
	Sleep(500);
}

void ATCommandWarp::SmsWrite2Sim(char const *smsPdu,int pduLength)
{	
	//	Util::ATCommand::Instance()->Write(smsPdu);
	Sleep(500);
	Transaction_((char *)smsPdu,pduLength,"",okSmsSend);
	Sleep(500);
}

void ATCommandWarp::SmsReadFromSim_(int sta)
{
	char CMGL[100];
	memset(CMGL,0,100);
	sprintf(CMGL,"AT+CMGL=%d\r",sta);
	
	switch (sta)
	{
		case 0:
			Dprintf("0\r\n");
			Transaction_(CMGL,strlen(CMGL),"",okCmglRead1);
			break;
		case 1:
			Dprintf("1\r\n");
			Transaction_(CMGL,strlen(CMGL),"",okCmglRead2);
			break;
		case 2:
			Dprintf("2\r\n");
			Transaction_(CMGL,strlen(CMGL),"",okCmglRead3);
			break;
		case 3:
			Dprintf("3\r\n");
			Transaction_(CMGL,strlen(CMGL),"",okCmglRead4);
			break;
	}
	
}

void ATCommandWarp::SmsReadFromSim(int sta)
{	
	char CMGL[100] = {0};
	char ans[1024] = {0};
	
	memset(CMGL,0,100);
	sprintf(CMGL,"AT+CMGL = %d\r",sta);
	
	EnterCriticalSection(&m_csAT);
	m_pCom->WriteComm(CMGL,strlen(CMGL));

	std::string data ;//全部的数据
	int  readLength;
	while (true)
	{
		Sleep(10);
		memset(ans,0,1024);
		readLength = m_pCom->ReadComm(ans,1024);//数据有可能多次读出来
		
		if (readLength > 0 )
		{	
			std::string s(ans);
			data += s;
		}

		if (data.find("OK") != std::string::npos)//只有OK返回
		{
			break;
		}
		else if (data.find("ERROR") != std::string::npos)
		{
			break;
		}
			
	}
	
	// 对数据解析
	Dprintf(L"\r\nParse_CMGL\r\n");
	
	SMS::SMSWarp::GetSMSWarp()->ParseCMGL_((char *)data.c_str());
	
	LeaveCriticalSection(&m_csAT);
	
}

/*void ATCommandWarp::SmsReadFromSim(int sta)
{	
	int  allLength = 0;//总的读出数据的长度
	int  size = 1024;//总的分配空间的大小
	int  haveReadLength = 0 ;
	char CMGL[100];
	char ans[1024];
	
	memset(CMGL,0,100);
	sprintf(CMGL,"AT+CMGL = %d\r",sta);
	
	EnterCriticalSection(&m_csAT);
	m_pCom->WriteComm(CMGL,strlen(CMGL));
	
	char *data = (char *)malloc(sizeof(char)*1024); //这里面读出来的数据有可能超过1024
	if (data == NULL)
	{
		return ;
	}
	memset(data,0,1024);
	//	Dprintf("Data Begin: %s\r\n",data);
	bool bok = false ;
	int  num = 0;
	int  readLength;
	while (true)
	{
		Sleep(30);
		memset(ans,0,1024);
		readLength = m_pCom->ReadComm(ans,1024);//数据有可能多次读出来
		allLength += readLength;
		if (allLength > size )//内存不够增加内存
		{	
			Dprintf("Realloc \r\n");
			data = (char *)realloc(data,sizeof(char)*(size+1024));
			if (data == NULL)
			{
				return ;
			}
			memset(data + size,0,1024);
			size += 1024;
		}
		
		if ( !bok && strstr(ans,"+CMGL:") != NULL)
		{	
			//	strcat(data,ans);
			memcpy(data,ans,readLength);
			haveReadLength = readLength;
			bok = true ;
			continue;
		}
		
		if (strstr(ans,"OK") != NULL && strstr(ans,"+CMGL:") == NULL)//只有OK返回
		{
			break;
		}
		
		if (!bok)
		{
			num++;
			if ( 5 == num)//5次没查出数据退出
			{
				break;
			}
		}
		else 
		{	
			if (readLength > 0)
			{	
				Dprintf("strcat data \r\n");
				memcpy(data + haveReadLength ,ans,readLength);
				haveReadLength += readLength;
				//	strcat(data,ans);
			}
			
			if ( 0 == readLength )
			{
				break ;
			}
		}
		
	}
	
	// 对数据解析
	Dprintf(L"\r\nParse_CMGL\r\n");
	//	Dprintf("Data:%s \r\n",data);
	
	SMS::SMSWarp::GetSMSWarp()->ParseCMGL_(data);
	
	free(data);
	data = NULL ;
	LeaveCriticalSection(&m_csAT);
	
}*/

void ATCommandWarp::SmsDeleteFromSim(int index , int delflag )
{
	/*
	0 index
	1 delete all readMessages
	2 delete read and sent messages
	3 delete read sent unsent messages
	4 delete all
	*/
	char CMGD[100];
	memset(CMGD,0,100);
	sprintf(CMGD,"AT+CMGD = %d,%d\r",index,delflag);

	Transaction_(CMGD,strlen(CMGD));
}

void ATCommandWarp::PinStatus()
{
	char CLCK[] = "AT+CLCK=\"SC\",2\r";
	Transaction_(CLCK,strlen(CLCK),"",okNoHandle);
}

void ATCommandWarp::CheckPuk()
{
	char CPIN[] = "AT+CPIN?\r";
	g_IsFDNFindDrap = FALSE;
	Drap();
	//char DRAP[] = "AT^DRAP\r";
	//Transaction_(DRAP,strlen(DRAP),"",okNoHandle);
	Sleep(30);
	Transaction_(CPIN,strlen(CPIN),"",okNoHandle);
}

void ATCommandWarp::AppInputPin(const char *pin)
{	
	char PIN[100];
	memset(PIN,0,100);
	sprintf(PIN,"AT+CPIN=\"%s\"\r",pin);
	Transaction_(PIN,strlen(PIN),"",okInputpin);
}

void ATCommandWarp::AppInputPin2(const char *pin2)
{
	char PIN2[100];
	memset(PIN2,0,100);
	sprintf(PIN2,"AT^DPIN2=\"%s\"\r",pin2);
	Transaction_(PIN2,strlen(PIN2),"",okInputpin2);
}

void ATCommandWarp::AppInputPuk(const char *puk,const char *newpin)
{
	//change by qi 20100909
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	char FUK[100];
	memset(FUK,0,100);
	sprintf(FUK,"AT+CPIN=\"%s\",\"%s\"\r",puk,newpin);
	
	if (main->m_bSearchNetOver)
	{
		Transaction_(FUK,strlen(FUK),"",okPuk);//输入PUK
	}
	else
	{
		InputPuk(puk,newpin);
	}
}

void ATCommandWarp::AppChangePin(const char *pin,const char *newpin)
{
	char PASSWORD[100];
	memset(PASSWORD,0,100);
	sprintf(PASSWORD,"AT+CPWD=\"SC\",\"%s\",\"%s\"\r",pin,newpin);
	Transaction_(PASSWORD,strlen(PASSWORD),"",okChangepin);
}

void ATCommandWarp::AppChangePin2(const char *pin,const char *newpin)
{
	char PASSWORD[100];
	memset(PASSWORD,0,100);
	sprintf(PASSWORD,"AT+CPWD=\"P2\",\"%s\",\"%s\"\r",pin,newpin);
	Transaction_(PASSWORD,strlen(PASSWORD),"",okChangepin2);
}
void ATCommandWarp::AppInputPuk2(const char *puk2,const char *newpin)
{
	char FUK2[100];
	memset(FUK2,0,100);
	sprintf(FUK2,"AT^DMSP=52,\"%s\",\"%s\"\r",puk2,newpin);   //PUK2---->PIN2
	Transaction_(FUK2,strlen(FUK2),"",okPuk2);
}

void ATCommandWarp::AppLockSim(const char *password)
{
	char CLCK[100];//SIM上锁
	memset(CLCK,0,100);
	sprintf(CLCK,"AT+CLCK=\"SC\",1,\"%s\"\r",password);
	Transaction_(CLCK,strlen(CLCK),"",okOpenPin);
}

void ATCommandWarp::AppUnlockSim(const char *password)
{
	char CLCK[100];//sim解锁
	memset(CLCK,0,100);
	sprintf(CLCK,"AT+CLCK=\"SC\",0,\"%s\"\r",password);
	Transaction_(CLCK,strlen(CLCK),"",okClosePin);
}

void ATCommandWarp::SetVP(int const vp)
{	
	char CRSM[200];
	memset(CRSM,0,200);
	int count = m_strSMSP.size()/2;
	std::string svp;
	switch(vp)
	{
	case 1:
		svp = "0B" ;//一小时
		break;
	case 2:
		svp = "A7" ;//一天
		break;
	case 3:
		svp = "AD" ;//一周
		break;
	case 4:
		svp = "FF";//最大时长
		break;
	}	

	std::string data = m_strSMSP.substr(0,m_strSMSP.size()-2);
	data = data + svp;
	
	sprintf(CRSM,"AT+CRSM=220,28482,1,4,%d,%s\r",count, data.c_str());
	Transaction_(CRSM,strlen(CRSM),"",okCrsm);

}

void ATCommandWarp::SetVP()
{
	char CRSM[200];
	memset(CRSM,0,200);
	int count = m_strSMSP.size()/2;	
	std::string data = m_strSMSP.substr(0,m_strSMSP.size()-2);
	data = data + m_svp;
	
	sprintf(CRSM,"AT+CRSM=220,28482,1,4,%d,%s\r",count, data.c_str());
	Transaction_(CRSM,strlen(CRSM));
}

void ATCommandWarp::GetVP()
{
	char CRSM[] = "AT+CRSM=178,28482,1,4,0\r"; //40
	Transaction_(CRSM,strlen(CRSM));	
}

void ATCommandWarp::ChangeCsca(char const *csca)
{
	char CSCA[50] ;
	memset(CSCA,0,50);
	sprintf(CSCA,"AT+CSCA=\"%s\"\r",csca);
	Transaction_(CSCA,strlen(CSCA),"",okChangecCsca);
}
void ATCommandWarp::GetCsca()
{
	char CSCA[] = "AT+CSCA?\r";
	Transaction_(CSCA,strlen(CSCA));
} 

void ATCommandWarp::CSCA()
{
	char CSCA[] = "AT+CSCA?\r";
	Transaction_(CSCA,strlen(CSCA));
}

void ATCommandWarp::SimIdentification(std::string &identification)
{
	char DCID[] = "AT^DCID\r";
	char ans[1024];
	
	Command(DCID,strlen(DCID),ans);
	if (strstr(ans,"^DCID: ") != NULL )
	{
		std::string s = strstr(ans,"^DCID: ")+ 8;
		size_t t = s.find("\"");
		
		if (t != std::string::npos)
		{
			identification = s.substr(0,t);
		}	
	}

/*	m_pCom->WriteComm(DCID,strlen(DCID));
	int  num = 0;	
	while (true)
	{
		Sleep(20);
		m_pCom->ReadComm(ans,1024);
		if (strstr(ans,"^DCID: ") != NULL )
		{
			std::string s = strstr(ans,"^DCID: ")+ 8;
			size_t t = s.find("\"");
			
			if (t != std::string::npos)
			{
				identification = s.substr(0,t);
			}
		
		}
		
		if (strstr(ans,"OK") != NULL || strstr(ans,"ERROR") != NULL)
		{
			break;	
		}

		num++;
		memset(ans,0,1024);

		if ( 5 == num)//5次没查出数据退出
		{
			break;
		}
	}*/
}

void ATCommandWarp::RegisterNewCom()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	extern IsTestCTA();
	if(!IsTestCTA())
	{
		main->RemoveSimSms();//移除SIM卡里的信息
		main->ReadSimSMS();
		
		main->RemoveSIMGroup();
		ReadSimContact();//读SIM卡里的信息
	}
	
	//设置波特率
	//char DMBR[] = "AT^DMBR=4\r";
	//Command(DMBR,strlen(DMBR));

	m_pCom->CloseComm();
	Util::ATCommand::Instance()->RegisterObserver(this);
	m_bInstance = Util::ATCommand::Instance()->m_bPortOpen;
	Sleep(30);

}

void ATCommandWarp::DSATREF_(int const mode)
{
	char DSATREF[20];
	sprintf(DSATREF,"AT^DSATREF=%d\r",mode);
	
	switch (mode)
	{
	case 0:
		Transaction_(DSATREF,strlen(DSATREF),"",okDsatref);
		break;
	case 4:
		Transaction_(DSATREF,strlen(DSATREF));
		break;
	}


	//a0120000xx
}

void ATCommandWarp::CSIM()
{	
	char CSIM[] = "AT+CSIM=22,A010000006FFFFFFFFFFFF\r";
	Transaction_(CSIM,strlen(CSIM),"",okCsim);	
}

void ATCommandWarp::CPMS()
{
	char CPMS[]="AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
	Transaction_(CPMS,strlen(CPMS));
}

void ATCommandWarp::UPLMN_Delete(int index)
{
	char CPOL[50];
	memset(CPOL,0,50);
	sprintf(CPOL,"AT+CPOL=%d\r",index);
	Transaction_(CPOL,strlen(CPOL),"",okCpolDel);
}

void ATCommandWarp::UPLMN_Add(char *oper,int gsm_act,int gsm_compact,int urtan_act)
{	
	char CPOL[50];
	char ans[1024] = {0};
	memset(CPOL,0,50);
	sprintf(CPOL,"AT+CPOL=,2,\"%s\",%d,%d,%d\r",oper,gsm_act,gsm_compact,urtan_act);
	Transaction_(CPOL,strlen(CPOL),"",okCpolAdd);
}

void ATCommandWarp::UPLMN_Change(int const index,char *oper,int gsm_act,int gsm_compact,int urtan_act)
{
	char CPOL[100];
	memset(CPOL,0,100);

	char ans[1024] = {0};
	sprintf(CPOL,"AT+CPOL=%d,2,\"%s\",%d,%d,%d\r",index,oper,gsm_act,gsm_compact,urtan_act);	
	Transaction_(CPOL,strlen(CPOL),"",okCpolChange);

}

void ATCommandWarp::UPLMN_Search()
{
	if(gIsFind3GNet)
	{
		gIsFind3GNet = FALSE;
		Sleep(500);
	}
	char CPOL[50];
	memset(CPOL,0,50);	
	char temp[1024] = {0};//临时数据串
	int length = 0 ;
	int num = 0 ;
	bool bok = false ;

	sprintf(CPOL,"AT+CPOL?\r");
	Transaction_(CPOL,strlen(CPOL));

}

void ATCommandWarp::CMMS()
{
	char CMMS[]="AT+CMMS=1\r";
	Transaction_(CMMS,strlen(CMMS),"",okSmsSend);
}

void ATCommandWarp::CMGF(int f)
{
	char CMGF[50];
	memset(CMGF,0,50);
	sprintf(CMGF,"AT+CMGF=%d\r",f);
	Transaction_(CMGF,strlen(CMGF));
}

void ATCommandWarp::SimStatus()
{
	char CIMI[] = "AT+CIMI\r";
	Transaction_(CIMI,strlen(CIMI),"",okCimi);
}

void ATCommandWarp::FindPLMN()
{
	char COPS[] = "AT+COPS=?\r";
	Transaction_(COPS,strlen(COPS),"",okNoHandle,true);
}

void ATCommandWarp::SearchPLMN()
{
	char COPS[] = "AT+COPS=0\r";
	Transaction_(COPS,strlen(COPS),"",okCsqSearch);
}

void ATCommandWarp::RegisterPLMN(char const *plmn,int const act)
{
	char COPS[100];
	sprintf(COPS,"AT+COPS=1,2,\"%s\",%d\r",plmn,act);
	Transaction_(COPS,strlen(COPS),"",okCsqRegister,true);

}

void ATCommandWarp::ClosePLMN()
{
	char COPS[] = "AT+COPS=2\r";
	Transaction_(COPS,strlen(COPS));
}

bool ATCommandWarp::GetInstance()
{
	return m_bInstance;
}

void  ATCommandWarp::Drap(DrapStatus ds )
{	
	m_DrapStatus = ds;
	char DRAP[]="AT^DRAP\r";
//	g_IsFDNFindDrap = TRUE;
	Transaction_(DRAP,strlen(DRAP));	
} 

extern BOOL gIsHandSet;
void ATCommandWarp::PhoneDial(char * number, BOOL isVideo)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	Sleep(50);
	PhoneDialTone(0, NULL);

//	if(!main->phone_->Hand())
//	{
//		extern void GNotifyDial(BOOL isDial);
//		GNotifyDial(TRUE);
//	}
//	gIsHandSet = FALSE;

	
	char ATD[] = "ATD";
	char CMD[1024];
	char number_[240] = {0};
	if(strlen(number) == 13 && ( memcmp(number, "861", 3) ==0 ))		//lxz 20100609
	{
		strcpy(number_, number+2);
	}
	else
		strcpy(number_, number);
	
	strcpy(CMD, ATD);
	strcat(CMD, number);
	if(isVideo)
	{
		char temp[] = "AT^DUSBPOWER=1\r";
		char r[64] = {0}; 
		//Transaction(temp, strlen(temp), r, 64);
		Transaction_(temp, strlen(temp));

		memset(CMD, 0, 1024);
		strcpy(CMD, "AT^DVTDIAL=");
		strcat(CMD, number);
		strcat(CMD, "\r");
	}
	else
		strcat(CMD, ";\r");

	//Transaction_(CMD, strlen(CMD));
	Transaction_(CMD, strlen(CMD),"",okAtd);
		
}

void ATCommandWarp::PhoneRedial(void)
{
	char ATDL[] = "ATDL;\r";
	Transaction_(ATDL, strlen(ATDL),"",okAtd);
	
}

int ATCommandWarp::PhoneState(void)
{	
	char CLCC[] = "AT+CLCC\r";
	Transaction_(CLCC, strlen(CLCC));
//	Transaction_(CLCC, strlen(CLCC),"",okIdle);
	return 1;
}

void ATCommandWarp::PhoneRing(char * number, int * type)
{
	/*
		+CLCC: 1,0,2,0,0,"10086",129\r\n
	*/

	char CLCC[] = "AT+CLCC\r";//注意：来电消息是主动上报的，这里还需要修改。 +CRING: VOICE\r\n+CLIP:"10086",161,"",,"",0\r\n
	Transaction_(CLCC,strlen(CLCC));

}

void ATCommandWarp::PhoneHangup(void)
{
//	extern void GNotifyDial(BOOL isDial);
//	GNotifyDial(FALSE);

	char ATH[] = "ATH\r";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\r”，3 表示“NO CARRIER”,为对端主动挂断
//	Transaction_(ATH, strlen(ATH));
	Transaction_(ATH, strlen(ATH));
}

void ATCommandWarp::PhoneAnswer(void)
{
	char ATA[] = "ATA\r";
	Transaction_(ATA, strlen(ATA));

}
void ATCommandWarp::PhoneSubDial(char number)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+VTS=%c\r", number); // 生成命令    

	Transaction_(cmd,strlen(cmd));

}


//电话功能
void ATCommandWarp::PhoneSetTrans(PhoneCallTRans calltrans)     //设置呼叫转移
{

}

void ATCommandWarp::CallTransferCancel(int type)
{
	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=%d,4\r",type);

	extern VOID WriteMyLog_(char *ptr, int size);
	if ( 0 == type)
	{	
		Transaction_(CCFC,strlen(CCFC),"",okCcfcUnconditionCancel);		
	}
	else	
	Transaction_(CCFC,strlen(CCFC),"",okCcfcCancel);
	
}

void ATCommandWarp::CallTransferStart(int type, std::string const number ,int second)
{
	char CCFC[100];
	memset(CCFC,0,100);
	
	char log[100];

	if (second > 0)
	{	
		sprintf(CCFC,"AT+CCFC=%d,3,\"%s\",,,,,%d\r",type,number.c_str(),second);		
	}
	else
	{	
		sprintf(CCFC,"AT+CCFC=%d,3,\"%s\"\r",type,number.c_str());
	}	
	
	if(0 == type)
	{	
		Transaction_(CCFC,strlen(CCFC),"",okCcfcUnconditionReg);		
	}
	else
	{	
		Transaction_(CCFC,strlen(CCFC),"",okCcfcregister);
	}
	
}

void ATCommandWarp::CallTransferStatus(int type)
{
	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=%d,2\r",type);
	Transaction_(CCFC,strlen(CCFC),"",okCcfcCheck);
	
}

void ATCommandWarp::UnconditionalTransferStatus()
{	
	char CCFC[] = "AT+CCFC=0,2\r";
	char ans[1024] = {0};
	Command(CCFC,strlen(CCFC),ans);

	if ( strstr(ans,"+CCFC: ") != NULL )
	{	
		std::string s = ans ;
		size_t t ;
		t = s.find("+CCFC:");
		
		if (t != std::string::npos)
		{
			s = s.substr(t+7);
			CString c ;
			c = Util::StringOp::ToCString(s);
			int flag = atoi(c);
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_UNCONDITIONALTRANSFER,flag,0);
			
		}		
	}

/*	m_pCom->WriteComm(CCFC, strlen(CCFC));

	bool bok = false ;
	int  num = 0;
	while (true)
	{
		Sleep(20);
		memset(ans,0,1024);
		m_pCom->ReadComm(ans,1024);
		if ( strstr(ans,"+CCFC: ") != NULL )
		{	
			std::string s = ans ;
			size_t t ;
			t = s.find("+CCFC:");
			
			if (t != std::string::npos)
			{
				s = s.substr(t+7);
				CString c ;
				c = Util::StringOp::ToCString(s);
				int flag = atoi(c);
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_UNCONDITIONALTRANSFER,flag,0);

			}
			
			bok = true;
			break;
		}
		
		num++;
		if ( 5 == num)//5次没查出数据退出
		{
			break;
		}		
	}*/
}

void ATCommandWarp::CallLimit(const char *fac,bool able)
{
	char CLCK[100];//SIM上锁
	memset(CLCK,0,100);
	
	char GCLCK[] = "AT+CLCK=?\r";
	Transaction_(GCLCK,strlen(GCLCK));

//	sprintf(CLCK,"AT+CLCK=\"%s\",%d,\"1234\",1\r",fac,(int)able);
//	sprintf(CLCK,"AT+CLCK=\"%s\",%d\r",fac,(int)able);
//	Transaction_(CLCK,strlen(CLCK));

	
}
void ATCommandWarp::PhoneSetCallLimit(PhoneCallLimit calllimit)    //设置呼叫受限
{
//	CallLimit("AI",calllimit.isCallin);//呼入限制
//	CallLimit("AO",calllimit.isCallout);//呼出限制	
	CallLimit("AI",true);//呼入限制
	CallLimit("AO",true);//呼出限制	

}

void ATCommandWarp::PhoneGetCallLimit(PhoneCallLimit &calllimit)    //获取呼叫受限设置
{	
	char CLCK[100];
	memset(CLCK,0,100);
	char fac[][5]={{"AI"},{"AO"}};
	for (int i = 0 ; i < 2;i++)
	{
		sprintf(CLCK,"AT+CLCK=\"%s\",2\r",fac[i]);		
		Transaction_(CLCK,strlen(CLCK));	
	}
}

bool ATCommandWarp::PhoneSetCallWaiting(bool able)//设置呼叫等待
{
	char CCWA[100];
	memset(CCWA,0,100);
	sprintf(CCWA,"AT+CCWA=1,%d\r",(int)able);
	if (able)
	{
		Transaction_(CCWA,strlen(CCWA),"",okCcwaAble);	
	}
	else
	{
		Transaction_(CCWA,strlen(CCWA),"",okCcwaCancel);	
	}
	return true ;
}

void ATCommandWarp::CHLDStatus()//获取呼叫等待设置
{
	char CCWA[] = "AT+CCWA=1,2\r";
	Transaction_(CCWA,strlen(CCWA),"",okCcwaCheck);

}

void ATCommandWarp::CHLD(int n)
{
	/*  
		n = 2,呼叫切换，n = 3,添加一个呼叫，以前是两个人通话，现在变成三个。
		其它的功能参照LC6311文档	
	*/
	char CHLD[100];
	memset(CHLD,0,100);
	sprintf(CHLD,"AT+CHLD=%d\r",n);
	Transaction_(CHLD,strlen(CHLD),"",okChld);

}

void ATCommandWarp::ReadSimContact()
{	
	int mac = 200;
	CString c ;
	char ans[1024] = {0}; // 应答串
	int ansLen = 1024;
	char cmd1[]	= "AT+CPBS = \"SM\"\r";//选择SIM存贮器
	memset(ans,0,1024);
	Transaction(cmd1, strlen(cmd1), ans, ansLen, 0,true);
		
	if (strstr(ans, "OK") == NULL)
	{
		Dprintf("SM NOT FOUND!");
		return ;
	}
		
	int l = 0 ;//SIM 中已经存在的电话的数目
	char cmd2[]	= "AT+CPBS?\r";//查询SIM状态
	memset(ans,0,1024);
	Transaction(cmd2, strlen(cmd2), ans, ansLen,0,true);
	if (strstr(ans, "+CPBS:") == NULL)
	{
		return ;
	}
	else
	{
		
		std::string s = ans;
		l = s.find("\"SM\"");
		if (l != std::string::npos)
		{
			s = s.substr(l);
			l = s.find(',');
			s = s.substr(l+1);
			c = Util::StringOp::ToCString(s);
			l = atoi(c);//SIM中已经存的数目
		}


	}
	
	char cmd3[16] ;//查询状态
	char *siminfo = NULL;//保存sim卡反馈回所有信息(字符串)
	if (l == 0)
	{
	//	siminfo = new char[1024];
	//	memset(siminfo,0,1024);
		return ;
	}
	else
	{
		siminfo = new char[mac*l];
		if (siminfo == NULL)
		{	
			return;
		}
		memset(siminfo,0,mac*l);
	}
	
	
	sprintf(cmd3,"AT+CPBR=0\r"); 

//	m_bgsm = true;	
//	if (CheckCSCS(siminfo))
//	{
		char UCS2[] = "AT+CSCS = \"UCS2\"\r";
		memset(ans,0,1024);
		Transaction(UCS2, strlen(UCS2), ans, ansLen,0,true);

		memset(siminfo,0,mac*l);
		Transaction(cmd3, strlen(cmd3), siminfo, mac*l,0,true);		
		m_bgsm = false;

//	}
	
	if (strstr(siminfo, "+CPBR:") != NULL)
	{
		// 分析返回的信息
		ParseAnswerData(siminfo,m_bgsm);
	}
	
	if (siminfo !=NULL)
	{
		delete []siminfo ;
	}
	
	//恢复编码集
	if (!m_bgsm)
	{
		char UCS2[] = "AT+CSCS = \"GSM\"\r";
		memset(ans,0,1024);
		Transaction(UCS2, strlen(UCS2), ans, ansLen);
	}
	
}

void ATCommandWarp::ReportSCE()
{
	char DAUDSCS[] = "AT^DAUDO?\r";
	Transaction_(DAUDSCS, strlen(DAUDSCS));
}

int gTelVolume = 3;
bool ATCommandWarp::PhoneVolume(unsigned int level)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	gTelVolume = level;
	
	char cmd0[30];       //命令串    
	char cmd1[30];       //命令串    
	char cmd2[30];       //命令串 
	char cmd4[30];       //命令串  
 
// 	sprintf(cmd0, "AT^DAUDSCC=4,%d,0\r", level); // 生成命令 
// 	Transaction_(cmd0, strlen(cmd0));

	sprintf(cmd1, "AT^DAUDO=1,%d\r", level); // 生成命令 
	sprintf(cmd2, "AT^DAUDO=2,%d\r", level); // 生成命令 

	sprintf(cmd4, "AT^DAUDI=1,%d\r", 5); // 生成命令   //level
	
	if( main->phone_->Hand())
	{	
		
		sprintf(cmd0, "AT^DAUDSCC=4,%d,0\r", level); // 生成命令 
		Transaction_(cmd0, strlen(cmd0));

		Transaction_(cmd1, strlen(cmd1));	
	}
	else
	{
		Transaction_(cmd2, strlen(cmd2));
	}

	Transaction_(cmd4, strlen(cmd4));

	return true;
	
}
void ATCommandWarp::PhoneMute(bool isMute)
{
	int mute = 0;
	if (isMute)
	{
		mute = 1;
	}
	else
	{
		mute = 0;
	}
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMUT=%d\r", mute); // 生成命令    
	
	Transaction_(cmd, strlen(cmd));

}
void ATCommandWarp::PhoneAutoAnswer(unsigned int second)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "ATS0=%d\r", second); // 生成命令    
	Transaction_(cmd, strlen(cmd));
}

void ATCommandWarp::PhoneNettype(void)			//LC6211
{
	char cmd[] = "AT^DACTI?\r";       // 命令串    
	Transaction_(cmd,strlen(cmd));
}

extern void GPlayDialTone(char *dialtone);
extern void GIsOpenMix(BOOL isOn);
void ATCommandWarp::PhoneDialTone(BOOL isOn, char *tone)
{
	char AT[64] = {0};
	bool result = true;
	char ans[1024] = {0};     
	int ansLen = 1024;
	if(!isOn)
	{
		GIsOpenMix(0);
		SendDialTone(NULL, (int)isOn);
	}
	else
	{	
		GIsOpenMix(1);
	//	GPlayDialTone(tone);    //lxz 20100609
		SendDialTone(tone, (int)isOn);		
	}

}

void ATCommandWarp::PhoneSignalQuality(void)
{
	char cmd[] = "AT+CSQ\r";       // 命令串    
	Transaction_(cmd, strlen(cmd));
}

//old
void ATCommandWarp::PhoneHandFree(bool isHandFree)
{		
	int hf = 0;
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (isHandFree)//免提
	{
		hf = 0;
	}
	else
	{
		#if(CTA_ONLY == 1)     //lxz 20100826
		hf = 4;
        #else
		hf = 0;
		#endif
	}
	
	char cmd[16];// 命令串    
	sprintf(cmd, "AT^DAUDSCS=%d\r", hf); // 生成命令	
	Transaction_(cmd, strlen(cmd));

	if (!main->phone_->Hand())
	{	
		hf = 2;
		char cmd[16];// 命令串   
		extern int gTelVolume;
		sprintf(cmd, "AT^DAUDO=%d,%d\r", hf, gTelVolume); //生成命令		
		Transaction_(cmd, strlen(cmd));	
	}
	else
	{
		#if(CTA_ONLY == 1)     //lxz 20100826
		;
		#else
		hf = 1;
		char cmd[16];// 命令串   
		extern int gTelVolume;
		sprintf(cmd, "AT^DAUDO=%d,%d\r", hf, gTelVolume); //生成命令		
		Transaction_(cmd, strlen(cmd));	
		#endif
	}

}

//new
/*void ATCommandWarp::PhoneHandFree(bool isHandFree)
{		
	int hf = 0;
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (isHandFree)//免提
	{
		hf = 0;
	}
	else
	{
		hf = 4;

	}

	char cmd[16];// 命令串    
	sprintf(cmd, "AT^DAUDSCS=%d\r", hf); // 生成命令	
	Transaction_(cmd, strlen(cmd));

	if (!main->phone_->Hand())
	{	
		hf = 2;
		char cmd[16];// 命令串   
		extern int gTelVolume;
		sprintf(cmd, "AT^DAUDO=%d,%d\r", hf, gTelVolume); //生成命令		
		Transaction_(cmd, strlen(cmd));	
		
		hf = 3;
		memset(cmd,0,16);
		sprintf(cmd, "AT^DAUDO=%d,%d\r", hf, 0); //生成命令		
		Transaction_(cmd, strlen(cmd));	

	}
}*/

/*void ATCommandWarp::PhoneHandFree(bool isHandFree)
{		
	int hf = 0;
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	char cmd[16];// 命令串    
	sprintf(cmd, "AT^DAUDSCS=%d\r", hf); //生成命令	
	Transaction_(cmd, strlen(cmd));
		
	if (!main->phone_->Hand())
	{	
		hf = 2;
		char cmd[16];       // 命令串   
		extern int gTelVolume;
		sprintf(cmd, "AT^DAUDO=%d,%d\r", hf, gTelVolume); //生成命令		
		Transaction_(cmd, strlen(cmd));	
	}
	else
	{
		hf = 1;
		char cmd[16];       // 命令串   
		extern int gTelVolume;
		sprintf(cmd, "AT^DAUDO=%d,%d\r", hf, gTelVolume); //生成命令		
		Transaction_(cmd, strlen(cmd));	
	}	
}*/

void ATCommandWarp::SetOutputGainLevel(int type)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (main->phone_->Hand())
	{
		if ( 0 == type)
		{
			char cmd[20] = {0};       // 命令串   
			sprintf(cmd, "AT^DAUDSCC=4,%d,0\r", 1); // 生成命令 	
			Transaction_(cmd, strlen(cmd));
		}
		else if (1 == type)
		{
			char cmd[20]={0};// 命令串
			extern int gTelVolume;
			sprintf(cmd, "AT^DAUDSCC=4,%d,0\r", gTelVolume); // 生成命令 
			Transaction_(cmd, strlen(cmd));
		}

	}
}

std::string ATCommandWarp::PhoneNumber(void)
{
	return m_strPhoneNumber;
}

void ATCommandWarp::SmsSend(int dataLength)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGS=%d\r", dataLength); // 生成命令    
	Transaction_(cmd,strlen(cmd),"",okSmsSend);
}

int ATCommandWarp::SmsSend(char* pdu, int pduLength, char* ans, int ansLength)
{	
	Transaction_(pdu, pduLength,"",okSmsSend);
	return 1;
}

int ATCommandWarp::SmsSend(int len, char* pdu, int pduLength, char* ans, int ansLength)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGS=%d\r", len); // 生成命令    
	int nLength = len;        // 串口收到的数据长度    
	char res[1024] = {0};      // 应答串 
	
	int reslen = 1024;
	m_pCom->WriteComm(cmd, strlen(cmd));
	int dwNumRead = 0;
	int nnCount = 0;
	while (true)
	{	
		//add by qi 20100325
		Sleep(10);
		int count = m_pCom->ReadComm(res + dwNumRead, reslen - dwNumRead);
		dwNumRead += count;
		if(strstr(res, "ERROR") != NULL)
		{
			nLength = 0;
			break;
		}
		
		if(strstr(res, "\r\n") != NULL && strstr(res, ">") != NULL)
			break;
		
		Sleep(100);
		if(nnCount++ > 0x8FFF)
		{
			nLength = 0;	
			break;
		}
	}
	
	strcpy(ans, res);
	if(nLength != 0)
	{
		char res_[1024] = {0};      // 应答串 
		int dwNumWrite = 0;
		while (dwNumWrite != pduLength)
		{
			dwNumWrite += m_pCom->WriteComm(pdu + dwNumWrite, pduLength - dwNumWrite);
		}
		int resLen = 1024;
		int dwNumRead = 0;
		int nnCount = 0;
		while (true)
		{	
			//add by qi 20100325
			Sleep(10);
			int count = m_pCom->ReadComm(res_ + dwNumRead, resLen - dwNumRead);
			dwNumRead += count;
			//	if ((nnCount > nCount) ||((dwNumRead > 0) && (count == 0)) || (strstr(res, "OK") != NULL) || (strstr(res, "ERROR") != NULL) || (strstr(res, "\r\n") != NULL))    //
			if((strstr(res_, "OK") != NULL) || (strstr(res_, "ERROR") != NULL) /*|| (strstr(res, "\r\n") != NULL) || ((dwNumRead > 0) && (count == 0))*/)
			{
				break;			
			}
			
			if(nnCount >= 0x8FFF)
			{
				break;
			}
			Sleep(100);
			nnCount++;
		}
		
		nLength = dwNumRead;
		strcpy(ans, res_);
	}
	return nLength;
}

int ATCommandWarp::SmsDelete(int index, char* ans, int ansLength)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGD=%d\r", index);    // 生成命令    
	
	//return Transaction(cmd, strlen(cmd), ans, ansLength);
	Transaction_(cmd,strlen(cmd));
	return 1;

}
int ATCommandWarp::SmsReadList(char* ans, int ansLength)
{
	char cmd[] = "AT+CMGL=0\r";
	Transaction_(cmd, strlen(cmd));
	return 1;
}

int ATCommandWarp::SmsReadResponse(void* pData, int nLength)
{
	return m_pCom->ReadComm(pData, nLength);
}

int ATCommandWarp::SmsWriteRequest(void* pData, int nLength)
{
	return m_pCom->WriteComm(pData, nLength);
}

std::string ATCommandWarp::SmsCenterAddress(void)
{
	return m_strSmsCentreAddress;
}
bool ATCommandWarp::SmsCenterAddress(std::string sca)
{
	char cmd[64] = {0};
	sprintf(cmd, "AT+CSCA=\"%s\",145\r", sca.c_str());
	char ans[1024] = {0};      // 应答串 
	int ansLen = 1024;
	int nLength = Transaction(cmd, strlen(cmd), ans, ansLen);
	return true;
}

// add by qi 2009_07_22
// old 
/*BOOL ATCommandWarp::CheckPin()
{
	BOOL flag = TRUE;
	char PIN[] = "AT+CPIN?\r" ;//查询sim卡状态
	char answer[1024];

//	EnterCriticalSection(&m_csAT);   //lxz 20100511
	m_pCom->WriteComm(PIN,strlen(PIN));
	bool bok = false ;
	int  num = 0;
	while (true)
	{
		Sleep(20);
		memset(answer,0,1024);
		m_pCom->ReadComm(answer,1024);
		if ( strstr(answer,"+CPIN: ") != NULL && strstr(answer,"OK") != NULL)
		{	
			if (strstr(answer,"+CPIN: SIM PIN") !=NULL)//需要pin1码
			{	
				HandlePin();
			}
			else if (strstr(answer,"+CPIN: SIM PUK") !=NULL)//需要PUK1码
			{	
				HandlePuk();
			}
			else if (strstr(answer,"+CPIN: SIM PUK2") !=NULL)//需要PUK2码
			{	
				HandlePuk2();
				flag = FALSE;
			}
			bok = true;
			break;
		}
		Sleep(500);
		num++;
		if ( 5 == num)//5次没查出数据退出
		{
			break;
		}		
	}
//	LeaveCriticalSection(&m_csAT);    //lxz 20100511

	return flag;
}*/

BOOL ATCommandWarp::CheckPin()
{
	BOOL flag = TRUE;
	char PIN[] = "AT+CPIN?\r" ;//查询sim卡状态
	char answer[1024] = {0};
	Command(PIN,strlen(PIN),answer);

	if ( strstr(answer,"+CPIN: ") != NULL && strstr(answer,"OK") != NULL)
	{	
		if (strstr(answer,"+CPIN: SIM PIN") !=NULL)//需要pin1码
		{	
			HandlePin();
		}
		else if (strstr(answer,"+CPIN: SIM PUK") !=NULL)//需要PUK1码
		{	
			HandlePuk();
		}
		else if (strstr(answer,"+CPIN: SIM PUK2") !=NULL)//需要PUK2码
		{	
			HandlePuk2();
			flag = FALSE;
		}
	}	
	
	return flag;
}
// old
/*void ATCommandWarp::GetPinErrorCount(int &nPin1, int &nPin2, int &nPuk1, int &nPuk2)
{
	nPin1 = 3;
	nPin2 = 3;
	nPuk1 = 10;
	nPuk2 = 10;

	if(gIsFind3GNet)
	{
		gIsFind3GNet = FALSE;
		Sleep(500);
	}

	char cmd[64] = {0};
	sprintf(cmd, "AT^DRAP\r");
	char ans[1024] = {0};      // 应答串 
	int ansLen = 1024;
	
	//Transaction(cmd, strlen(cmd), ans, ansLen);
	EnterCriticalSection(&m_csAT);   //lxz 20100511
	m_pCom->WriteComm(cmd, strlen(cmd));
	bool bok = false ;
	int  num = 0;
	while (true)
	{
		Sleep(20);
		memset(ans,0,1024);
		m_pCom->ReadComm(ans,1024);
		
		if(strstr(ans,"^DRAP:") != NULL)
		{
			std::string s = ans ;
			int l ;
			l = s.find("^DRAP:");
			if(l >= 0)
			{
				s = s.substr(l+7);
				nPin1 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
			}
			
			l = s.find(",");
			if(l >= 0)
			{
				s = s.substr(l+1);
				nPin2 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
			}
			
			l = s.find(",");
			if(l >= 0)
			{
				s = s.substr(l+1);
				nPuk1 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
			}
			
			l = s.find(",");
			if(l >= 0)
			{
				s = s.substr(l+1);
				nPuk2 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
			}
			break;
		}
		else if(strstr(ans,"ERROR") != NULL)
			break;
		if(num++ > 30)
			break;
		else
			Sleep(100);
	}
	LeaveCriticalSection(&m_csAT); 
	return;
}*/

void ATCommandWarp::GetPinErrorCount(int &nPin1, int &nPin2, int &nPuk1, int &nPuk2)
{
	nPin1 = 3;
	nPin2 = 3;
	nPuk1 = 10;
	nPuk2 = 10;
	
	if(gIsFind3GNet)
	{
		gIsFind3GNet = FALSE;
		Sleep(500);
	}
	
	char cmd[64] = {0};
	sprintf(cmd, "AT^DRAP\r");
	
	EnterCriticalSection(&m_csAT);   //lxz 20100511

	char ans[1024] = {0};      // 应答串 
	Command(cmd,strlen(cmd),ans);	
	if(strstr(ans,"^DRAP:") != NULL)
	{
		std::string s = ans ;
		int l ;
		l = s.find("^DRAP:");
		if(l >= 0)
		{
			s = s.substr(l+7);
			nPin1 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
		}
		
		l = s.find(",");
		if(l >= 0)
		{
			s = s.substr(l+1);
			nPin2 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
		}
		
		l = s.find(",");
		if(l >= 0)
		{
			s = s.substr(l+1);
			nPuk1 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
		}
		
		l = s.find(",");
		if(l >= 0)
		{
			s = s.substr(l+1);
			nPuk2 = Util::StringOp::ToInt(s, 10);    //atoi(s.c_str());
		}
	}
	else if(strstr(ans,"ERROR") != NULL)
	{
		Dprintf("Drap_error\r\n");
	}
	
	LeaveCriticalSection(&m_csAT); 
}

/*void ATCommandWarp::HandlePin()
{
	char ans[1024];//应答消息
	memset(ans,0,1024);

	int pin1 = -1;
	int pin2 = -1;
	int puk1 = -1;
	int puk2 = -1;

	GetPinErrorCount(pin1, pin2, puk1, puk2);
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT, 0, pin1);
	while (strstr(ans,"+CME ERROR: 12") == NULL)//连续三次pin码输入错误，sim将被锁住
	{	
		
		while(strstr(ans,"OK") == NULL && strstr(ans,"ERROR") == NULL )//等待用户输入pin码
		{	
			Sleep(100);
			extern BOOL g_isOFFing;
			if(!g_isOFFing)
			{
				m_pCom->ReadComm(ans,1024);	
			}
		}		
				
		if ( strstr(ans,"ERROR") != NULL)//输入pin码错误
		{	
			if (strstr(ans,"+CME ERROR: 12") == NULL)
			{
				pin1 = -1;
				GetPinErrorCount(pin1, pin2, puk1, puk2);
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_ERROR,pin1);
				memset(ans, 0, 1024);
			}
		}

		if (strstr(ans,"OK") != NULL && (strstr(ans,"ERROR") == NULL))//输入pin码正确
		{	
			//PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_OK,0);
			return ;	
		}
	}
	
	if (strstr(ans,"+CME ERROR: 12")!= NULL)//需要输入PUK码
	{	
		//add by qi 20100909
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pPasswordDlg->ShowWindow_(SW_HIDE);
		Sleep(30);
		//

		HandlePuk();
	}		
	
}*/

void ATCommandWarp::HandlePin()
{
	char ans[1024];//应答消息
	memset(ans,0,1024);

	std::string data("");
	
	int pin1 = -1;
	int pin2 = -1;
	int puk1 = -1;
	int puk2 = -1;
	
	GetPinErrorCount(pin1, pin2, puk1, puk2);
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT, 0, pin1);
	while (data.find("+CME ERROR: 12") == std::string::npos )//连续三次pin码输入错误，sim将被锁住
	{	
		
		while(data.find("OK") == std::string::npos &&
			data.find("+CME ERROR: 16") == std::string::npos && 
			data.find("+CME ERROR: 12") == std::string::npos)//等待用户输入pin码
		{	
			Sleep(10);
			extern BOOL g_isOFFing;
			if(!g_isOFFing)
			{
				m_pCom->ReadComm(ans,1024);	
				data += std::string(ans);
				memset(ans,0,1024);
			}
		}		
		
		if (data.find("+CME ERROR: 16") != std::string::npos)
		{
			pin1 = -1;
			GetPinErrorCount(pin1, pin2, puk1, puk2);
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_ERROR,pin1);
			memset(ans, 0, 1024);
			data="";
		}
		else if(data.find("OK") != std::string::npos)
		{
			return ;
		}
		
	}

	if (data.find("+CME ERROR: 12") != std::string::npos)//需要输入PUK码
	{	
		//add by qi 20100909
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pPasswordDlg->ShowWindow_(SW_HIDE);
		Sleep(30);
		
		HandlePuk();
		data = "";
	}
		
}

void ATCommandWarp::HandlePuk2()
{
	int num = 0 ;
	
	int pin1 = -1;
	int pin2 = -1;
	int puk1 = -1;
	int puk2 = -1;
	
	GetPinErrorCount(pin1, pin2, puk1, puk2);
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_PUK2,puk2);	

}

void ATCommandWarp::HandlePuk()
{
	int num = 0 ;

	int pin1 = -1;
	int pin2 = -1;
	int puk1 = -1;
	int puk2 = -1;
	
	GetPinErrorCount(pin1, pin2, puk1, puk2);
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PUK,0,puk1);
	//	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_PUK,puk1);	
	while ( num != 10)//如果输错PUK码累计10次，您的SIM卡将被永久锁住（SIM报废）需要重新换卡。
	{			
		char ans[1024];
		memset(ans,0,1024);
		while(strstr(ans,"OK") == NULL && strstr(ans,"ERROR")== NULL )//等待用户输入pin码
		{	
			Sleep(100);
			extern BOOL g_isOFFing;
			if(!g_isOFFing)
			{
				m_pCom->ReadComm(ans,1024);	
			}
		}		
		if (strstr(ans,"OK") != NULL)//输入PUK码正确
		{	
			//add by qi 20100909
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->
				m_pPinSetDlg->m_pPinInputDlg->ShowWindow_(SW_HIDE);
			return ;	
		}		
		if ( strstr(ans,"ERROR") != NULL)//输入PUK码错误
		{
			if (/*num > 0 &&*/ num < 9)
			{	
				num++;

				puk1 = -1;
				GetPinErrorCount(pin1, pin2, puk1, puk2);
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_INPUT_PUK,0,puk1);
				//	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_FUK_ERROR,puk1);
				memset(ans, 0, 1024);
			}
		}		
	}
	
	if (10 == num)//10次输入错误
	{			
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_FUK_LOCKED,0);		
	}
}

void ATCommandWarp::HandleSIM(bool binsert,bool bfresh)
{
	::PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SIM_MESSAGE,binsert,(int)(bfresh));
}

bool ATCommandWarp::CommandFun5(char* cmd, int len, char* out /* = NULL */, int count /* = 5 */)
{
	char ans[1024] = {0};// 应答串 
	int ansLen = 1024;
	bool result = false;

	for (int i = 0; i < count; ++i)
	{
		m_pCom->WriteComm(cmd, len);  
		Sleep(i * 1000);
		int len = m_pCom->ReadComm(ans, 1024);     
		
		if (out != NULL)
		{
			memcpy(out, ans, len);     
			out[len] = '\0';
		}

		if (strstr(ans,"+CME ERROR: 11") != NULL && strstr(ans,"OK") != NULL)//需要PIN码
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"+CME ERROR: 12") != NULL && strstr(ans,"OK") != NULL)//需要输入PUK码
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"+CME ERROR: 10") != NULL && strstr(ans,"OK") != NULL)//未检测到SIM卡
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"OK") != NULL)
		{
			result = true;
			break;
		}
	}
	return result;
}

// old
/*bool ATCommandWarp::CommandFun5_(char* cmd, int len,char* out)
{
	char ans[1024] = {0};// 应答串 
	int ansLen = 1024;
	bool result = false;
	
	m_pCom->WriteComm(cmd, len); 
	
	while (1)
	{
		m_pCom->ReadComm(ans, 1024);     	
		if (strstr(ans,"+CME ERROR: 11") != NULL )//需要PIN码
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"+CME ERROR: 12") != NULL )//需要输入PUK码
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"+CME ERROR: 10") != NULL )//未检测到SIM卡
		{
			result = false ;
			break;
		}
		else if (strstr(ans,"OK") != NULL)
		{
			result = true;
			break;
		}
		Sleep(30);
	}
	
	if ( out != NULL)
	{
		strcpy(out,ans);
	}

	return result ;
}*/

//new
bool ATCommandWarp::CommandFun5_(char* cmd, int len,char* out)
{
	char ans[1024] = {0};// 应答串 
	int ansLen = 1024;
	bool result = false;

	std::string data("");
	
	m_pCom->WriteComm(cmd, len); 
	
	while (1)
	{
		int ReadNum = m_pCom->ReadComm(ans, 1024);
		if (ReadNum > 0)
		{
			std::string d(ans);
			data += d;
		}

		if (data.find("+CME ERROR: 11") != std::string::npos )//需要PIN码
		{
			result = false ;
			break;
		}
		else if (data.find("+CME ERROR: 12") != std::string::npos )//需要输入PUK码
		{
			result = false ;
			break;
		}
		else if (data.find("+CME ERROR: 10") != std::string::npos )//未检测到SIM卡
		{
			result = false ;
			break;
		}
		else if (data.find("OK") != std::string::npos)
		{
			result = true;
			break;
		}
		Sleep(10);
	}
	
	if ( out != NULL)
	{
		strcpy(out,(char*)data.c_str());
	}
	
	return result ;
}

void ATCommandWarp::UnconditionalTransfer(bool able)
{
	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=0,%d\r",(int)able);
	Transaction_(CCFC,strlen(CCFC));
}
void ATCommandWarp::BusyTransfer(bool able)
{
	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=1,%d\r",int(able));
	Transaction_(CCFC,strlen(CCFC));
}

void ATCommandWarp::NoReplyTransfer(bool able)
{
	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=2,%d\r",(int)able);
	Transaction_(CCFC,strlen(CCFC));

}
void ATCommandWarp::NoReachTranfer(bool able)
{
	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=3,%d\r",(int)able);
	Transaction_(CCFC,strlen(CCFC));
}

void ATCommandWarp::InputPin(const char *pin)
{
	char PIN[100];
	memset(PIN,0,100);
	sprintf(PIN,"AT+CPIN=\"%s\"\r",pin);
	m_pCom->WriteComm(PIN,strlen(PIN));//输入pin码
}

void ATCommandWarp::InputPuk(const char *puk,const char *newpin)
{
	char FUK[100];
	memset(FUK,0,100);
	sprintf(FUK,"AT+CPIN=\"%s\",\"%s\"\r",puk,newpin);
	m_pCom->WriteComm(FUK,strlen(FUK));//输入sim卡解锁码和新的pin码
}

bool ATCommandWarp::InputPin2(const char *pin2)
{
	char PIN2[100];
	char ans[1024];
	memset(PIN2,0,100);
	memset(ans,0,1024);
	sprintf(PIN2,"AT^DPIN2=\"%s\"\r",pin2);
	Transaction(PIN2,strlen(PIN2),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{
		return false ;
	}
	return true ;
}

bool ATCommandWarp::InputPuk2(const char *puk2,const char *newpin)
{
	char FUK2[100];
	char ans[1024];
	memset(FUK2,0,100);
	memset(ans,0,1024);
	sprintf(FUK2,"AT^DPIN2=\"%s\",\"%s\"\r",puk2,newpin);
	Transaction(FUK2,strlen(FUK2),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{
		return false ;
	}
	return true ;
}


bool ATCommandWarp::LockSim(const char *password)
{
	char CLCK[100];//SIM上锁
	char ans[1024];
	memset(CLCK,0,100);
	memset(ans,0,1024);
	sprintf(CLCK,"AT+CLCK=\"SC\",1,\"%s\"\r",password);
	Transaction(CLCK,strlen(CLCK),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{	
		return false ;
	}
	return true ;

}
bool ATCommandWarp::UnlockSim(const char *password)
{
	char CLCK[100];//sim解锁
	char ans[1024];
	memset(CLCK,0,100);
	memset(ans,0,1024);
	sprintf(CLCK,"AT+CLCK=\"SC\",0,\"%s\"\r",password);
	Transaction(CLCK,strlen(CLCK),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{	
		return false ;
	}
	return true ;

}
bool ATCommandWarp::GetSimStatus(int &status)
{	
	CString c;
	char CLCK[] = "AT+CLCK=\"SC\",2\r";
	char ans[1024];
	memset(ans,0,1024);
	Transaction(CLCK,strlen(CLCK),ans,1024);
	if (strstr(ans,"OK") != NULL)
	{
		std::string s = ans ;
		int l ;
		l = s.find("+CLCK:");
		s = s.substr(l+7);
		c = Util::StringOp::ToCString(s);
		status = atoi(c);
		return true ;
	}
	return false ;

}
bool ATCommandWarp::ChangePin(const char *pin,const char *newpin)
{	
	//只有在锁住的情况下才能改变pin
	
	char PASSWORD[100];
	char ans[1024];
	memset(ans,0,1024);
	memset(PASSWORD,0,100);
	sprintf(PASSWORD,"AT+CPWD=\"SC\",\"%s\",\"%s\"\r",pin,newpin);
	Transaction(PASSWORD,strlen(PASSWORD),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{
		return false ;
	}
	
	return true ;
}
bool ATCommandWarp::ChangePin2(const char *pin,const char *newpin)
{
	char PASSWORD[100];
	char ans[1024];
	memset(PASSWORD,0,100);
	memset(ans,0,1024);
	sprintf(PASSWORD,"AT+PASSWORD=\"P2\",\"%s\",\"%s\"\r",pin,newpin);
	Transaction(PASSWORD,strlen(PASSWORD),ans,1024);
	if (strstr(ans,"ERROR") != NULL)
	{
		return false ;
	}
	return true ;
}
void ATCommandWarp::GetOperator()
{
	char CPOL[] = "AT+COPS?\r"; //AT+WOPN读取操作员名字,AT+CPOL
	char ans[1024];
	Command(CPOL,strlen(CPOL),ans);
	std::string str = ans ;
	size_t t;

	if ( (t = str.find("+COPS: ")) != std::string::npos)
	{	
		str = str.substr(t);
		t = str.find(',');
		if (t != std::string::npos)
		{
			str = str.substr(t+1);
			t	= str.find(',');
			if (t != std::string::npos)
			{
				str = str.substr(t+2);
				t = str.find('"');
				static std::string oprator ;
				oprator = str.substr(0,t);
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CURRENT_OPERATOR,(WPARAM)&oprator,0);
			}
		}
		
	}
}

int ATCommandWarp::GetSimCapacity()
{	
	int l = 0;
	int ansLen = 1024;
	char cmd[]	= "AT+CPBS?\r";//查询SIM状态
	Transaction_(cmd, strlen(cmd));
	return l;
}

bool ATCommandWarp::CheckCSCS(char *code)
{	
	int num = 0 ;
	size_t pos ;
	std::string s = code ;
	pos = s.find("+CPBR:");
	if (pos != std::string::npos)
	{			
		if ( s.find('?') != std::string::npos )
		{
			return true ;
		}		
	}
	return false ;

}

void ATCommandWarp::ResetGSM()
{
	//恢复编码集
	char UCS2[] = "AT+CSCS = \"GSM\"\r";
	Transaction_(UCS2, strlen(UCS2));	
}

void ATCommandWarp::ReadSimContent(int const nstart)
{
	char cmd3[16] ;//查询状态
	memset(cmd3,0,16);
//	sprintf(cmd3, "AT+CPBR=%d,%d\r", nstart,nstart);
	sprintf(cmd3, "AT+CPBR=%d,%d\r", 0,nstart);
	Transaction_(cmd3, strlen(cmd3));

}

void ATCommandWarp::ReportNet(bool bover)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	PostMessage(main->m_hWnd,WM_SEARCH_NET,WPARAM(bover),NULL);
}

void ATCommandWarp::DHUP(int const cid)
{
	char dhup[30];
	memset(dhup,0,30);
	sprintf(dhup,"AT^DHUP=%d,17\r",cid);
	Transaction_(dhup,strlen(dhup));
//	Transaction_(dhup,strlen(dhup));
}

void ATCommandWarp::DUSIMR()
{
	char DUSIMR[] = "AT^DUSIMR = 1\r";//报告SIM卡的状态
	Transaction_(DUSIMR,strlen(DUSIMR));
}

void ATCommandWarp::SimContactNum()
{
	char cmd1[]	= "AT+CPBS = \"SM\"\r";//选择SIM存贮器
	Transaction_(cmd1, strlen(cmd1));

	char cmd2[]	= "AT+CPBS?\r";//查询SIM状态
	Transaction_(cmd2, strlen(cmd2));
	
}


void  ATCommandWarp::Save2Sim(std::vector<SIM_FORMAT> vsim,bool bsave, BOOL isFDN)
{
	if (vsim.size() == 0)
	{
		return ;
	}

	char ans[1024] = {0}; // 应答串 
	int ansLen = 1024;

	char cmd2[1024]; //向电话本中添加或删除条目
	int length = 0 ;
	std::string name = vsim[0].name;
	for (int i = 0 ; i < vsim.size(); i++)
	{   
		char *pname = new char[200];//名字这么大空间足够了
		unsigned char *desname = new unsigned char[200];//
		memset(cmd2,0,1024);
		memset(pname,0,200);
		memset(desname,0,200);	
		
//		if (!m_bgsm)
//		{
		if(!isFDN)
		{
			char UCS2[] = "AT+CSCS = \"UCS2\"\r";
			Transaction_(UCS2, strlen(UCS2));
			GB2312toUSC2(vsim[i].name.c_str(),pname,vsim[i].name.size());//将名字转换成USC2格式
		}
		else
		{
			char HEX[] = "AT+CSCS= \"GSM\"\r";
			Transaction_(HEX, strlen(HEX));
			strcpy(pname, vsim[i].name.c_str());
		}
// 		}
// 		else
// 		{
// 			int dlength = EncodeGsm(vsim[i].name.c_str(),desname,strlen(vsim[i].name.c_str()));
// 			gsmBytes2String(desname,pname,dlength);	
// 		}
		
		if (bsave)//添加
		{
			sprintf(cmd2,"AT+CPBW=%d,\"%s\",129,\"%s\"\r",0,vsim[i].telnum.c_str(),pname);
			Transaction_(cmd2, strlen(cmd2),"",okCpbwAdd);
		}
		else//修改
		{
			sprintf(cmd2,"AT+CPBW=%s,\"%s\",129,\"%s\"\r",vsim[i].id.c_str(),vsim[i].telnum.c_str(),pname);	
			Transaction_(cmd2, strlen(cmd2),"",okCpbwChange);
		}
		
		if (!m_bgsm)//恢复下编码集
		{
			char HEX[] = "AT+CSCS= \"GSM\"\r";
			Transaction_(HEX, strlen(HEX));
		}
		
		delete[]pname;
		delete[]desname;
		Sleep(10);
	}
	
}

void ATCommandWarp::CMER()
{
	char CMER[] = "AT+CMER=2,0,0,2\r" ;
	Transaction_(CMER, strlen(CMER));
}

void ATCommandWarp::CSCS(char const *code)
{
	char CSCS[50];
	memset(CSCS,0,50);
	sprintf(CSCS,"AT+CSCS=\"%s\"\r",code);
	Transaction_(CSCS,strlen(CSCS));
}

void ATCommandWarp::CPBS(char const *storage)
{
	char CPBS[50];
	memset(CPBS,0,50);
	sprintf(CPBS,"AT+CPBS=\"%s\"\r",storage);

	if (strstr(storage,"FD"))
	{
		Transaction_(CPBS,strlen(CPBS),"",okCpbsSelect);
	}
	else
	{
		Transaction_(CPBS,strlen(CPBS));
	}
}

void ATCommandWarp::CPBS()
{
	char CPBS[] = "AT+CPBS?\r";	
	Transaction_(CPBS,strlen(CPBS),"",okCpbsCheck);
}

void ATCommandWarp::CPBR(int const index)
{
	char CPBR[100];
	memset(CPBR,0,100);
	sprintf(CPBR,"AT+CPBR=%d\r",index);
	Transaction_(CPBR,strlen(CPBR),"",okCpbr);
}

void ATCommandWarp::DMBR(int n)
{
	char DMBR[30] = {0};
	sprintf(DMBR,"AT^DMBR=%d\r",n);
	Transaction_(DMBR,strlen(DMBR));
}

void ATCommandWarp::SendDialTone(char *code, int isON)
{
	char DialTone[100];
	memset(DialTone,0,100);
	static BOOL gIsDialing = FALSE;
	if( isON == 1 )
	{
		if(gIsDialing || strstr(code, "dialing") > 0)
		{
			sprintf(DialTone,"AT^DAUDCTRL=%d\r",0);
			//Transaction_(DialTone);
		}
		char dialtone_[100];
		sprintf(dialtone_,"AT^DAUDCTRL=%d,\"%s\"\r",isON, code);
		strcat(DialTone, dialtone_);
	}
	else
	{
		sprintf(DialTone,"AT^DAUDCTRL=%d\r",isON);
		gIsDialing = FALSE;
	}
	Transaction_(DialTone, strlen(DialTone));

	if(code)
	{
		if(strstr(code, "dialing") > 0 || strstr(code, "hangup") > 0 )
		{
			gIsDialing = TRUE;
		}
		else
		{
			gIsDialing = FALSE;
		}
	}
}

void ATCommandWarp::DSACT(int const mode,char*pwd)
{
	char DSACT[100];
	memset(DSACT,0,100);
	
	switch (mode)
	{
	case 0: //关闭
		sprintf(DSACT,"AT^DSACT=\"FD\",%d,\"%s\"\r",mode,pwd);
		Transaction_(DSACT,strlen(DSACT),"",okDsactDeactive);
		break;
	case 1: //开启
		sprintf(DSACT,"AT^DSACT=\"FD\",%d,\"%s\"\r",mode,pwd);
		Transaction_(DSACT,strlen(DSACT),"",okDsactActive);
		break;
	case 2: //查询
		sprintf(DSACT,"AT^DSACT=\"FD\",%d\r",mode);
		Transaction_(DSACT,strlen(DSACT));
		break;
	}

}

void ATCommandWarp::SmsReceiptOpen(bool bopen)
{
	char CNMI[100]; 
	if (bopen)
	{	
		strcpy(CNMI,"AT+CNMI=2,1,0,1,1\r");
		Transaction_(CNMI,strlen(CNMI),"",okCnmiOpen);
	}
	else
	{	
		strcpy(CNMI,"AT+CNMI=2,1,0,0,0\r");
		Transaction_(CNMI,strlen(CNMI),"",okCnmiClose);

	}
	
}

void ATCommandWarp::DeleteSim(int id)
{
	char cmd2[1024]; //向电话本中添加或删除条目
	sprintf(cmd2,"AT+CPBW=%d\r",id);
	Transaction_(cmd2, strlen(cmd2),"",okCpbwDelete);
}

// 可打印字符串转换为字节数据    
// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}    
// 输入: pSrc - 源字符串指针    
//       nSrcLength - 源字符串长度    
// 输出: pDst - 目标数据指针    
// 返回: 目标数据长度    
int ATCommandWarp::gsmString2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)   
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
int ATCommandWarp::gsmBytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)   
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

//转化成7_bit   
int ATCommandWarp::EncodeGsm(const char *src,unsigned char *des,int srclength )
{
	Converter c;	
	int num = 0 ;
	int desLength = 0 ;
	unsigned char* temp = des;
	while( num < srclength)
	{	
		memset(c.convertBuffer,0,7);
		for (int j = 0; j < 7; ++j) 
		{
			if (num > srclength)
			{
				break ;
			}
			c.convertBuffer[j] = src[num++] ;
		}
		*des++ = c.sevenBit0();
		*des++ = c.sevenBit1();
		*des++ = c.sevenBit2();
		*des++ = c.sevenBit3();
		*des++ = c.sevenBit4();
		*des++ = c.sevenBit5();
		*des++ = c.sevenBit6();
		*des++ = c.sevenBit7();
		desLength += 8;
	}
	*des = '\0';
	return desLength;

}

//Decode 7_bit
int ATCommandWarp::DecodeGsm(const unsigned char *src,char *des,int srclength)
{	
	Converter c ;
	int num = 0 ;
	int deslength = 0 ;
	while (num < srclength)
	{	
		memset(c.convertBuffer,0,7);
		c.sevenBit0(src[num++]);
		deslength++ ;
		if (num < srclength)
		{
			c.sevenBit1(src[num++]);
			deslength++;
		}
		if (num < srclength)
		{
			c.sevenBit2(src[num++]);
			deslength++;
		}
		if (num < srclength)
		{
			c.sevenBit3(src[num++]);
			deslength++;
		}
		if (num < srclength)
		{
			c.sevenBit4(src[num++]);
			deslength++;
		}
		if (num < srclength)
		{
			c.sevenBit5 (src[num++]);
			deslength++;
		}		
		if (num < srclength)
		{
			c.sevenBit6 (src[num++]);
			deslength++;
		}		
		if (num < srclength)
		{
			c.sevenBit7 (src[num++]);
			deslength++;
		}
		
		for (int i = 0 ; i < 7; i++)
		{
			*des++ = c.convertBuffer[i];
		}
	}

	*des = '\0';
	return deslength;
}
void ATCommandWarp::GB2312toUSC2(const char *src,char *des,int srclength)
{
	int nDstlength ;
	WCHAR wh[128];//最多128个汉字名字
	unsigned char pdst[256];
	int it = 0  ;
	int num = 0 ;
	memset(wh,0,128*2);
	memset(pdst,0,256);
	nDstlength = ::MultiByteToWideChar(CP_ACP, 0, src, srclength, wh, srclength +1);
	for(int i = 0; i < nDstlength; i++) 
	{
		// 先输出高位字节
		pdst[it++] = wh[i] >> 8;
		// 后输出低位字节
		pdst[it++] = wh[i] & 0xff;

	}
	pdst[it] = '\0';//结束标志，自己加的
	
	i = 0 ;
	for ( i = 0 ; i < it ;i++)
	{
		sprintf(des+num*2,"%02x",pdst[i]);
		num++;
	}
	des[num*2+1] = '\0';
	
}
void ATCommandWarp::USC2toGB2312(char *src,char *des,int length)
{	

	char *endpos = 0 ;
	unsigned char hex[128*2];
	WCHAR wh[64*2];
	memset(hex,0,128*2);
	memset(wh,0,64*2);
	int it = 0 ;
	for (int i = 0 ; i < length ;i+=2)//把字符串转化成16进制的值
	{   
		char temp[3];
		temp[0] = src[i];
		temp[1] = src[i+1];
		temp[2] = '\0';
		hex[it++]  = ::strtol(temp,&endpos,16);
	}
	
	hex[it] = '\0' ;//标志,判断该hex是否结束
	int wt = 0;
	for (i = 0; i < it; i += 2) {
		wh[wt++] = (hex[i]<< 8) | (unsigned char)hex[i+1] ;
	}

	wh[wt] = '\0' ;	
	
	std::string s = Util::StringOp::FromCString(wh);
	strcpy(des,(char*)s.c_str());

//  UNICODE串-->字符串
//	int nDstLength = ::WideCharToMultiByte(CP_ACP, 0, wh, length/4, des, length,
//		NULL, NULL);
//  输出字符串加个结束符
//	des[nDstLength] = '\0';	


}

struct VideoThreadParam 
{
	Util::ComWarp * pVideoComm;
	BOOL			*pIsRunThread;
	BOOL            *pIsExitThread;
	pGetVideoData   GetVideoData; 
};

VideoThreadParam   gVideaoParam;

void GetVideoDataProc(void *pParam)
{
	VideoThreadParam *pVideoParam = (VideoThreadParam *)pParam;
	char data[1024];
	while(pVideoParam->pIsRunThread)
	{
		int len = pVideoParam->pVideoComm->ReadComm(data, 1024);
		pVideoParam->GetVideoData(data, len);
	}
	*pVideoParam->pIsExitThread = TRUE;
}

//开始视频
bool ATCommandWarp::StartVideoPhone(Util::ComWarp *pVideoCom)
{
	pVideoCom->OpenComm(L"COM4:");
	char *cmd = "AT^DVTCHL=1\r";
	pVideoCom->WriteComm(cmd, strlen(cmd));
	m_bIsVideoRun = TRUE;
	m_bIsVideoExit = FALSE;
	
	gVideaoParam.pVideoComm = pVideoCom;
	gVideaoParam.pIsRunThread = &m_bIsVideoRun;
	gVideaoParam.pIsExitThread = &m_bIsVideoExit;
	
	DWORD d;
	HANDLE pThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)GetVideoDataProc, (void *)&gVideaoParam, 0, &d );

	return TRUE;
}

//结束视频
bool ATCommandWarp::EndVideoPhone(Util::ComWarp *pVideoCom)
{
	m_bIsVideoRun = FALSE;
	while(!m_bIsVideoExit)
	{
		Sleep(5);
	}
	pVideoCom->CloseComm();
	return TRUE;
}

int ATCommandWarp::GetVideoData(void *pData, int length)
{
	return 0;
}

int ATCommandWarp::SendVideoData(void *pData, int length)
{
	int ret = gVideaoParam.pVideoComm->WriteComm(pData, length);
	return ret;
}

