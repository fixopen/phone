#include "stdafx.h"
#include "ATCommandWarp.h"

#include "resource.h"
#include "../Multimediaphone.h"
#include "../MultimediaPhoneDlg.h"

using Util::ATCommandWarp;

bool ATCommandWarp::bInit = false;

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

ATCommandWarp::ATCommandWarp(): Util::Observer()
{
	m_pCom = NULL;
	m_strPhoneNumber = "";
	m_bgsm = true ;
	InitializeCriticalSection(&m_csAT);
	m_index = 0;
	m_iContact = 0 ;
	
	InitializeCriticalSection(&m_csCPBR);

}

ATCommandWarp::~ATCommandWarp()
{
	if (bInit)
	{
		Off();
	}
	DeleteCriticalSection(&m_csAT); 

	DeleteCriticalSection(&m_csCPBR); 
}

ATCommandWarp::ATCommandWarp(const ATCommandWarp& ATWarp)
{

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
int ATCommandWarp::Connect(char* pin)
{
	if (!bInit)
	{
		int i = Init();
		if (i == 1)
		{
			int o = On(pin);
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

bool ATCommandWarp::Command(char* cmd, int len, char* out /* = NULL */, int count /* = 5 */,BOOL isWating /* = FALSE */)
{
	if(!m_pCom)
		return false;

	char ans[1024] = {0};      // 应答串 
	int ansLen = 1024;
	bool result = false;
// 	m_pCom->WriteComm(cmd, len);   
// 	m_pCom->ReadComm(ans, 1024);   
//	Transaction(cmd, len, ans, ansLen);
	for (int i = 0; i < count; ++i)
	{
		if (strstr(ans, "OK") == NULL)
		{
			m_pCom->WriteComm(cmd, len);  
		//	if(count != 1)              //lxz 20090825
		//	    Sleep(i * 1000);
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

int ATCommandWarp::Transaction(char* req, int reqLen, char* res, int resLen,int nCount,bool bwait)
{
	if(!m_pCom)
		return false;
	
	EnterCriticalSection(&m_csAT);
	int dwNumWrite = 0;
	while (dwNumWrite != reqLen)
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

		} while (!strstr(res,"OK"));
		
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
void ATCommandWarp::Transaction_(char* req, int reqLen ,char const *condition)
{	
	std::string data = req;
	Util::ATCommand::Instance()->Write(data,condition);		
}

int ATCommandWarp::Init(void)
{
	char AT[] = "AT\r";// 测试GSM-MODEM的存在性
	char ATZ[] = "ATZ\r";//复位
	char ATE[] = "ATE0\r";// ECHO OFF  
	char CGMR[] = "AT+CGMR\r";
	char CMGF[] = "AT+CMGF=0\r";// PDU模式
	char CSCS[] = "AT+CSCS=\"UCS2\"\r";
	char CPMS[] = "AT+CPMS=\"SM\",\"SM\",\"SM\"\r";
	char CMNI[] = "AT+CNMI=2,1,0,0,0\r";
	char ATV[] = "ATV1\r";//1 <CR><LF><verbose code><CR><LF> 0 <numeric code><CR>	"ATV0\r";返回“0\r”（ATV指令决定返回result code是数字格式还是字符格式，ATV0 返回为数字格式，其中0 表示OK，1 表示CONNECT，2 表示RING，3 表示NO CARRIER，4 表示ERROR
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
//	char DAUDSCS[] = "AT^DAUDSCS=0\r";
	
	bool result = true;
	char ans[1024] = {0};      // 应答串    

 	if (!Command(AT, strlen(AT)))
	{
		return 0;
	}
	
	Command(ATZ, strlen(ATZ));

	
	if (Command(ATE, strlen(ATE))
		&& Command(CGMR, strlen(CGMR))
		&& Command(DUSBDEG1, strlen(DUSBDEG1))
		&& Command(DUSBDEG2, strlen(DUSBDEG2))
		&& Command(DUSBDEG3, strlen(DUSBDEG3))
		&& Command(DUSBDEG4, strlen(DUSBDEG4))
		&& Command(DGPIOSM, strlen(DGPIOSM))

//		&& Command(DSLP, strlen(DSLP))
//		&& Command(DSQ, strlen(DSQ))

		//视频电话
		&& Command(CRC, strlen(CRC))
		&& Command(DSCI, strlen(DSCI))
		&& Command(DCPI, strlen(DCPI))
//		&& Command(DAUDSCS, strlen(DAUDSCS))
//		&& Command(DVTTYPE, strlen(DVTTYPE))
//		&& Command(DVTABLE, strlen(DVTABLE))
		
		)
	{	
		return 1;
	}
	return 0;
}

int ATCommandWarp::On(char* pin)
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
	char CMNI[] = "AT+CNMI=2,1,0,0,0\r";

	char CPIN[] = "AT+CPIN?\r"; //PIN 鉴定 此处用来判断是否插入SIM卡
	char CNUM[] = "AT+CNUM\r";//读手机号码
	char CSCA[] = "AT+CSCA?\r";//读短信中心号码
	
	char DSTM[] = "AT^DSTM?\r";//^DSTM: 0,0,0 OK 表明当前的网络为G网
							   //若为^DSTM: 0,0,2　OK表明当前的网络为T网


	bool result = true;
	char ans[1024] = {0};      // 应答串    

	if (!Command(CREG1, strlen(CREG1)))
	{
		return 0;
	}
	
//	PhoneVolume(3);
//	Command(CMER, strlen(CMER));
	memset(ans, 0, 1024);
	BOOL bIsUnSIM = FALSE;
	static bool bsim = true ;
//	if (!Command(CFUN5, strlen(CFUN5), ans)) // old	
	if (!CommandFun5(CFUN5, strlen(CFUN5), ans))//add by qi 2009_08_28
	{	
		if (strstr(ans, "+CME ERROR: 11") != NULL || strstr(ans, "+CME ERROR: 12") != NULL )//在设置开机PIN 码时，APP端输入“AT+CFUN=5”，激活USIM 卡，返回的“＋CME		ERROR：11”表示“SIM PIN Required”；此时APP 端会弹出提示输入pin码的提示，		APP端输入PIN 码后，再执行激活协议栈的指令。
		{
			if (!CheckPin())
			{
				return 0 ;
			}
		}
		else if(strstr(ans, "+CME ERROR: 10") != NULL )	//未检测SIM卡
		{	
			HandleSIM(false);
			Command(CFUN1, strlen(CFUN1));
			Command(COPS, strlen(COPS));
			main->RemoveSIMGroup();
			m_pCom->CloseComm();
			Util::ATCommand::Instance()->RegisterObserver(this);
			bIsUnSIM = TRUE;
			bsim = false ;
			return 3;
		}
		else
		{	
			HandleSIM(false);
			main->RemoveSIMGroup();
			m_pCom->CloseComm();
			Util::ATCommand::Instance()->RegisterObserver(this);

			Command(CFUN1, strlen(CFUN1));
			Command(COPS, strlen(COPS));
			return 3;

		}
	}
	
	if (bsim)
	{	
		HandleSIM(true);
	}
	
// 	char CLCK[100];//SIM上锁
// 	memset(CLCK,0,100);	
// 	sprintf(CLCK,"AT+CLCK=\"%s\",%d\r","SC",2);
// 	Command(CLCK, strlen(CLCK));

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
	memset(ans, 0, 1024);
	m_pCom->ReadComm(ans, 1024);
 	while ((strstr(ans, "+CREG: 1") == NULL) && (strstr(ans, "+CREG: 5") == NULL))
	{
		if(bIsUnSIM && strstr(ans, "+CREG: 3"))
			return 3;
		m_pCom->ReadComm(ans, 1024);
	}

	if (!(Command(CREG0, strlen(CREG0))
		&& Command(CMGF, strlen(CMGF))
		&& Command(CSCS, strlen(CSCS))
		&& Command(CPMS, strlen(CPMS))
		&& Command(CMNI, strlen(CMNI))))
	{
		return 0;
	}

	char cmd_[16];// 命令串
	sprintf(cmd_, "AT+VTD=%d\r", 4); // 生成命令
    Command(cmd_,strlen(cmd_));

//	m_pCom->WriteComm(cmd_, strlen(cmd_));	
//	PhoneVolume(1);

	memset(ans, 0, 1024);
	if (Command(CSCA, strlen(CSCA), ans))
	{
		std::string addr = ans;
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
				}
			}
		}	
	}

	memset(ans, 0, 1024);
	if (Command(CNUM, strlen(CNUM), ans))
	{
		std::string number = ans;
		unsigned int pos1 = number.find(",");
		if (pos1 != std::string::npos)
		{
			unsigned int pos2 = number.find(",", pos1 + 1);
			if (pos2 != std::string::npos)
			{
				pos1 += 2;
				pos2 -= 1;
				if (pos2 - pos1 > 0)
				{
					m_strPhoneNumber = number.substr(pos1, pos2 - pos1);
				}
			}
		}
	}

// 	memset(ans, 0, 1024);
// 	Command(DSTM, strlen(DSTM), ans);
// 	if ( strstr(ans,"^DSTM:") != NULL)
// 	{
// 		if (strstr(ans,"0,0,0") != NULL)//G网
// 		{
// 			 char* DSQ = "AT^DSQ =1\r";
// 			 Command(DSQ,strlen(DSQ),ans);	 
// 		}
// 	}

	main->RemoveSIMGroup();
//	ReadSimContact();//读SIM卡里的信息
	m_pCom->CloseComm();
	Util::ATCommand::Instance()->RegisterObserver(this);

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
	ParseDAUDO(data);
	ParseCLCK(data);
	ParseCPBS(data);
//	ParseCPBR(data);
	ParseCCFC(data);
	ParseCCWA(data);

}

void ATCommandWarp::ParseDAUDO(std::string const& data)
{	
	if (data.find( "^DAUDO:") != std::string::npos )
	{
		if(data.find( "DAUDO: 1") != std::string::npos )   
		{
			m_nDAUDO =  1;
		}
		
		m_nDAUDO = 2;
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
		s = s.substr(t+7);
		c = Util::StringOp::ToCString(s);
		status = atoi(c);

		PostMessage(main->m_pSettingDlg->m_hWnd,WM_CLCK,m_type,status);		 	
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
		s = s.substr(t);
		t = s.find(',');
		s = s.substr(t+1);
		c = Util::StringOp::ToCString(s);

		m_nContactNum = atoi(c);//SIM中已经存的数目
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_CONTACT_NUM,m_nContactNum,0);
		t = s.find(',');
		s = s.substr(t+1);//总数
		c = Util::StringOp::ToCString(s);
		all = atoi(c);//SIM中总的数目
		PostMessage(main->m_pContactDlg->m_hWnd,WM_SIM_CAPACITY,all,0);
		
// 		char UCS2[] = "AT+CSCS = \"UCS2\"\r";
// 		Transaction_(UCS2, strlen(UCS2));
// 		Sleep(50);
// 		ReadSimContent(m_nContactNum);

	}

}
void ATCommandWarp::ParseCPBR(std::string const& data)
{	
	EnterCriticalSection(&m_csCPBR);

	if ( data.find("+CPBR:") != std::string::npos)
	{
		//反馈的信息的格式
		/*
			AT+CPBR= 0,2
			+CPBR: 1,"13512509353",129,"502A5DE5",0
			+CPBR: 2,"12452",129,"7F57603B",0
			OK
		*/
		

		Dprintf("CPBR: %s\r\n",(char *)data.c_str());
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;	
		static bool bfirst = true ;
		
//		if ( bfirst)//第一次不解析
//		{
// 			if ( CheckCSCS((char* )data.c_str()))//有问号换种编码集
// 			{
// 				char UCS2[] = "AT+CSCS = \"UCS2\"\r";
// 				Transaction_(UCS2, strlen(UCS2));
// 				m_bgsm = false ;
// 			}
// 			bfirst = false;
// 			ReadSimContent(1);
// 			return;

//		}
		
/*		if ( CheckCSCS((char* )data.c_str()))//有问号换种编码集
		{
// 			if (bfirst)
// 			{	
// 				char UCS2[] = "AT+CSCS = \"UCS2\"\r";
// 				Transaction_(UCS2, strlen(UCS2));
// 
// 				m_bgsm = false ;
// 				bfirst = false;
// 			}

			size_t t = 0 ;
			std::string s = data ;

			while ( (t = s.find("+CPBR:") ) != std::string::npos)
			{
				m_iContact++ ;
				
				Dprintf("BBBBBBBBBBBB = : %d,%d\r\n",m_iContact,m_nContactNum);
				if (m_iContact == m_nContactNum)
				{	
					Dprintf("CCCCCCCCCCCC ");
					
					char UCS2[] = "AT+CSCS = \"UCS2\"\r";
					Transaction_(UCS2, strlen(UCS2));
					Sleep(100);
					ReadSimContent(m_nContactNum);
					bfirst = false;
					m_bgsm = false ;

					LeaveCriticalSection(&m_csCPBR);  
					return;
				}
				s = s.substr(t+6);
			}
			LeaveCriticalSection(&m_csCPBR);  
			return;

		}
*/

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
	LeaveCriticalSection(&m_csCPBR);  


}
void ATCommandWarp::ParseCCFC(std::string const& data)
{	
	CString c;
	if (data.find("+CCFC:") != std::string::npos)
	{	
		std::string s = data ;
		size_t t ;
		t = s.find("+CCFC:");
		s = s.substr(t+7);

		if ( 0 == m_nCCFCtype)
		{	
			c = Util::StringOp::ToCString(s);
			m_calltrans.isUncondify = atoi(c);
			if (m_calltrans.isUncondify)
			{
				if ( m_calltrans.teccode.empty() )
				{
					size_t phone = s.find('"');
					if (phone != std::string::npos)
					{
						s = s.substr(phone+1);
						phone = s.find('"');
						if (phone !=std::string::npos)
						{
							s = s.substr(0,phone);
							if (s.substr(0,2) == "86")
							{
								s = s.substr(2);
							}					
							m_calltrans.teccode = s;						
						}
					}
				}
				
				return ;
			}

		}

		if ( 0 == m_calltrans.isUncondify)
		{
			if ( 1 == m_nCCFCtype)
			{	
				c = Util::StringOp::ToCString(s);
				m_calltrans.isBusy = atoi(c);	
			}
			
			if ( 2 == m_nCCFCtype)
			{		
				c = Util::StringOp::ToCString(s);
				m_calltrans.isNoAswer = atoi(c);	
			}
			
			if ( 3 == m_nCCFCtype)
			{	
				c = Util::StringOp::ToCString(s);
				m_calltrans.isNoTel = atoi(c);	//无法接通
			}
			
			// 查找号码
			if (m_calltrans.teccode.empty())
			{
				size_t phone = s.find('"');
				if (phone != std::string::npos)
				{
					s = s.substr(phone+1);
					phone = s.find('"');
					if (phone !=std::string::npos)
					{
						s = s.substr(0,phone);
						if (s.substr(0,2) == "86")
						{
							s = s.substr(2);
						}					
						m_calltrans.teccode = s;						
					}
				}
			}

		}
	
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
		s = s.substr(t+7);
		c = Util::StringOp::ToCString(s);
		status = atoi(c);
		PostMessage(main->m_pSettingDlg->m_hWnd,WM_CCFC,status,0);		
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
	std::string s = src ;
	std::string name ;
	std::string phone ;
	std::vector <SIM_FORMAT> vsim ; 	

	size_t t = 0 ;
	
	while ( (t = s.find("+CPBR:"))!= std::string::npos)
	{	
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
		unsigned char *temp = new unsigned char[200]; 
		memset(temp,0,150);
		memset(des,0,200);
		if(bgsm)
		{
			int tlength = gsmString2Bytes(name.c_str(),temp,strlen(name.c_str()));
			DecodeGsm(temp,des,tlength);
		}
		else
		{
			USC2toGB2312((char*)name.c_str(),des,strlen(name.c_str()));		
		}
		
		Dprintf("AAAAAAAAA = %s",des);
		Dprintf("AAAAAAAAA = %s\r\n",sim.id.c_str());
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


extern BOOL gIsHandSet;
void ATCommandWarp::PhoneDial(char * number, BOOL isVideo)
{
	Sleep(50);
	PhoneDialTone(0, NULL);
	if(!gIsHandSet)
	{
		extern void GNotifyDial(BOOL isDial);
		GNotifyDial(TRUE);
	}
	gIsHandSet = FALSE;

	
	char ATD[] = "ATD";
	char CMD[1024];
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

	Transaction_(CMD, strlen(CMD));
		
}

void ATCommandWarp::PhoneRedial(void)
{
	char ATDL[] = "ATDL;\r";
	Transaction_(ATDL, strlen(ATDL));
	
}

int ATCommandWarp::PhoneState(void)
{
	char CLCC[] = "AT+CLCC\r";

	Transaction_(CLCC, strlen(CLCC));
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
	extern void GNotifyDial(BOOL isDial);
	GNotifyDial(FALSE);

	char ATH[] = "ATH\r";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\r”，3 表示“NO CARRIER”,为对端主动挂断
	Transaction_(ATH, strlen(ATH));
}

void ATCommandWarp::PhoneAnswer(void)
{
	char ATA[] = "ATA\r";//对于挂断AT指令，AT＋CHUP（挂断当前激活CS链路）和AT＋CHLD 也可作为可选AT指令。被动挂断，则LC6311 会给APP 上报“3\r”，3 表示“NO CARRIER”,为对端主动挂断	
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
	char CCFC[100];
	memset(CCFC,0,100);
	
	//注册释放
	for (int i = 0 ; i < 4 ;i++)
	{
		sprintf(CCFC,"AT+CCFC=%d,4\r",i);
		Transaction_(CCFC,strlen(CCFC));
	}
	
	//注册
	if ( 1 == calltrans.isUncondify)//注册无条件转移
	{
		//开通或者取消功能
		sprintf(CCFC,"AT+CCFC=%d,3,\"%s\"\r",0,calltrans.teccode);
		Transaction_(CCFC,strlen(CCFC));

	}
	else
	{
		for (int i = 1 ; i < 4; i++)
		{
			memset(CCFC,0,100);
			if (2 == i)// 无应答转移，默认时间是20秒
			{			
				sprintf(CCFC,"AT+CCFC=%d,3,\"%s\",,,,%d\r",i,calltrans.teccode,20);	
				if ( 1 == calltrans.isNoAswer)
				{
					Transaction_(CCFC,strlen(CCFC));
				}
			}
			else
			{
				sprintf(CCFC,"AT+CCFC=%d,3,\"%s\"\r",i,calltrans.teccode);
				if (1 == i)//遇忙
				{
					if (1 == calltrans.isBusy)
					{
						Transaction_(CCFC,strlen(CCFC));
					}
				}
				
				if (3 == i)//不能接通
				{
					if (1 == calltrans.isNoTel)
					{
						Transaction_(CCFC,strlen(CCFC));
					}
				}
			}
			
		}
	}

}

void ATCommandWarp::CallTransferCancel(int type)
{
	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=%d,4\r",type);
	Transaction_(CCFC,strlen(CCFC));
	
	CallTransferStatus(type);

}

void ATCommandWarp::CallTransferStart(int type, std::string const number ,int second)
{
	char CCFC[100];
	memset(CCFC,0,100);
	if ( second > 0)
	{
		sprintf(CCFC,"AT+CCFC=%d,3,\"%s\",,,,%d\r",type,number,second);		
	}
	else
	{
		sprintf(CCFC,"AT+CCFC=%d,3,\"%s\"\r",type,number);
	}

	Transaction_(CCFC,strlen(CCFC));
	
	CallTransferStatus(type);
}

void ATCommandWarp::CallTransferStatus(int type)
{
	char CCFC[100];
	memset(CCFC,0,100);
	sprintf(CCFC,"AT+CCFC=%d,2\r",type);
	Transaction_(CCFC,strlen(CCFC));
	
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
	Transaction_(CCWA,strlen(CCWA));	
	return true ;
}

bool ATCommandWarp::PhoneGetCallWaiting(int &status)//获取呼叫等待设置
{
	char CCWA[] = "AT+CCWA=1,2\r";
	Transaction_(CCWA,strlen(CCWA));

	return false ;
}

void ATCommandWarp::PhoneCallSwitch(int n)
{
	/*  
		n = 2,呼叫切换，n = 3,添加一个呼叫，以前是两个人通话，现在变成三个。
		其它的功能参照LC6311文档	
	*/
	char CHLD[100];
	memset(CHLD,0,100);
	sprintf(CHLD,"AT+CHLD=%d\r",n);
	Transaction_(CHLD,strlen(CHLD));

}

void ATCommandWarp::ReadSimContact()
{	
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
	
	int l;//SIM 中已经存在的电话的数目
	char cmd2[]	= "AT+CPBS?\r";//查询SIM状态
	memset(ans,0,1024);
	Transaction(cmd2, strlen(cmd2), ans, ansLen,true);
	if (strstr(ans, "OK") == NULL)
	{
		return ;
	}
	else
	{
		
		std::string s = ans;
		l = s.find("\"SM\"");
		s = s.substr(l);
		l = s.find(',');
		s = s.substr(l+1);
		c = Util::StringOp::ToCString(s);
		l = atoi(c);//SIM中已经存的数目

	}
	
	char cmd3[16] ;//查询状态
	char *siminfo ;//保存sim卡反馈回所有信息(字符串)
	if (l == 0)
	{
		siminfo = new char[1024];
		memset(siminfo,0,1024);
	}
	else
	{
		siminfo = new char[60*l];
		memset(siminfo,0,60*l);
	}
	
	sprintf(cmd3,"AT+CPBR=0,%d\r",l); 
	Transaction(cmd3, strlen(cmd3), siminfo, 60*l,0,true);
	m_bgsm = true;
	
	if (CheckCSCS(siminfo))
	{
		char UCS2[] = "AT+CSCS = \"UCS2\"\r";
		memset(ans,0,1024);
		Transaction(UCS2, strlen(UCS2), ans, ansLen,0,true);

		memset(siminfo,0,60*l);
		Transaction(cmd3, strlen(cmd3), siminfo, 60*l,0,true);		
		m_bgsm = false;

	}
	
	if (strstr(siminfo, "OK") != NULL)
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
// 	char DAUDSCC[100] ;
// 	memset(DAUDSCC,0,100);
// 	sprintf(DAUDSCC, "AT^DAUDSCC=0,%d\r", 5);

	char cmd1[16];       // 命令串    
	char cmd2[16];       // 命令串 
	char cmd4[16];       // 命令串  

	char DAUDSCS[] = "AT^DAUDSCS=0\r";
	
	sprintf(cmd1, "AT^DAUDO=1,%d\r", level); // 生成命令 
	sprintf(cmd2, "AT^DADDO=2,%d\r", level); // 生成命令 

	sprintf(cmd4, "AT^DAUDI=1,%d\r", level); // 生成命令 
	
	Transaction_(DAUDSCS,strlen(DAUDSCS));

//	ReportSCE();
//	Sleep(1000);

	if( main->phone_->Hand())
	{	
		Transaction_(cmd1, strlen(cmd1));
		Transaction_(cmd4, strlen(cmd4));
	
	}
	else
		Transaction_(cmd2, strlen(cmd2));

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
		//strcpy(AT, "AT^DAUDCTRL=0\r");
		//Command_1(AT, strlen(AT));
		GIsOpenMix(0);
	}
	else
	{
	//	if(strlen(tone) < 2)
		{
//			PhoneDialTone(0, NULL);
		}
//		strcpy(AT, "AT^DAUDCTRL=1,\"");
//		strcat(AT, tone);
//		Command_1(AT, strlen(AT));
		
		 GIsOpenMix(1);
		 GPlayDialTone(tone);

	}

}

void ATCommandWarp::PhoneSignalQuality(void)
{
	char cmd[] = "AT+CSQ\r";       // 命令串    
	Transaction_(cmd, strlen(cmd));
}

void ATCommandWarp::PhoneHandFree(bool isHandFree)
{
		
	int hf = 0;
	if (isHandFree)
	{
		hf = 2;
	}
	else
	{
		hf = 1;
	}

	char cmd[16];       // 命令串   
	extern int gTelVolume;
	sprintf(cmd, "AT^DAUDO=%d,%d\r", hf, gTelVolume); // 生成命令
	
 // sprintf(cmd, "AT^DAUDSCC=%d,%d\r", hf, gTelVolume); // 生成命令    
	
	Transaction_(cmd, strlen(cmd));	

}

std::string ATCommandWarp::PhoneNumber(void)
{
	return m_strPhoneNumber;
}

void ATCommandWarp::SmsSend(int dataLength)
{
	char cmd[16];       // 命令串    
	sprintf(cmd, "AT+CMGS=%d\r", dataLength); // 生成命令    

	Transaction_(cmd,strlen(cmd));
	
}

int ATCommandWarp::SmsSend(char* pdu, int pduLength, char* ans, int ansLength)
{
//	return Transaction(pdu, pduLength, ans, ansLength, 0xFFFF);
	Transaction_(pdu, pduLength);
	return 1;

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
	char cmd[] = "AT+CMGL=4\r";

//	return Transaction(cmd, strlen(cmd), ans, ansLength);
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
bool ATCommandWarp::CheckPin()
{	
	bool result = false ;
	char PIN[] = "AT+CPIN?\r" ;//查询sim卡状态
	char answer[1024];
	memset(answer,0,1024);
	if (Command(PIN,strlen(PIN),answer))
	{
		if (strstr(answer,"+CPIN: SIM PIN") !=NULL)//需要pin1码
		{	
			result = HandlePin();
		}
		else if (strstr(answer,"+CPIN: SIM PUK") !=NULL)//需要PUK1码
		{	
			result = HandlePuk();
		}
	}
	return result ;
}

bool  ATCommandWarp::HandlePin()
{
	char ans[1024];//应答消息
	memset(ans,0,1024);
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,0,0);
	while (strstr(ans,"+CME ERROR: 12") == NULL)//连续三次pin码输入错误，sim将被锁住
	{			
		while(strstr(ans,"OK") == NULL && strstr(ans,"ERROR") == NULL )//等待用户输入pin码
		{	
			Sleep(100);
			m_pCom->ReadComm(ans,1024);	
		}		
		if (strstr(ans,"OK") != NULL)//输入pin码正确
		{	
			return true;	
		}
		
		if ( strstr(ans,"ERROR") != NULL)//输入pin码错误
		{	
			if (strstr(ans,"+CME ERROR: 12") == NULL)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_ERROR,0);
				memset(ans, 0, 1024);
			}
		}
		
	}
	
	if (strstr(ans,"+CME ERROR: 12")!= NULL)//需要输入PUK码
	{	
		return false ;
	//	HandlePuk();
	}	
	return true ;
}
bool ATCommandWarp::HandlePuk()
{
	int num = 0 ;
	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_PUK,0);	
	while ( num != 10)//如果输错PUK码累计10次，您的SIM卡将被永久锁住（SIM报废）需要重新换卡。
	{			
		char ans[1024];
		memset(ans,0,1024);
		while(strstr(ans,"OK") == NULL && strstr(ans,"ERROR")== NULL )//等待用户输入pin码
		{	
			Sleep(100);
			m_pCom->ReadComm(ans,1024);	
		}		
		if (strstr(ans,"OK") != NULL)//输入PUK码正确
		{	
			return true;	
		}		
		if ( strstr(ans,"ERROR") != NULL)//输入PUK码错误
		{
			if (num > 0 && num < 9)
			{	
				num++;
				PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_FUK_ERROR,0);
				memset(ans, 0, 1024);
			}
		}		
	}
	
	if (10 == num)//10次输入错误
	{			
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_PIN_INPUT,PIN_FUK_LOCKED,0);		
	}
	return false ;

}

void ATCommandWarp::HandleSIM(bool binsert)
{
	::PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SIM_MESSAGE,binsert,0);
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

bool ATCommandWarp::CommandFun5_(char* cmd, int len,char* out)
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
	}
	
	if ( out != NULL)
	{
		strcpy(out,ans);
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
	sprintf(PIN2,"AT+CPIN2=\"%s\"\r",pin2);
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
	sprintf(FUK2,"AT+CPIN2=\"%s\",\"%s\"\r",puk2,newpin);
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
bool ATCommandWarp::GetOperator(std::string &opeator)
{
	char CPOL[] = "AT+COPS?\r"; //AT+WOPN读取操作员名字,AT+CPOL
	char ans[1024];
	Transaction(CPOL,strlen(CPOL),ans,1024);
	if (strstr(ans,"OK") != NULL)
	{
		std::string str = ans ;
		int l ;
		l	= str.find(',');
		str = str.substr(l+1);
		l	= str.find(',');
		str = str.substr(l+2);
		l = str.find('"');
		opeator = str.substr(0,l);
		return true ;
	}
	return false ;
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

void ATCommandWarp::SimContactNum()
{
	char cmd1[]	= "AT+CPBS = \"SM\"\r";//选择SIM存贮器
	Transaction_(cmd1, strlen(cmd1));

	char cmd2[]	= "AT+CPBS?\r";//查询SIM状态
	Transaction_(cmd2, strlen(cmd2));
	
}


void  ATCommandWarp::Save2Sim(std::vector<SIM_FORMAT> vsim)
{
	if (vsim.size() == 0)
	{
		return ;
	}

	char ans[1024] = {0}; // 应答串 
	int ansLen = 1024;

	char cmd2[1024]; //向电话本中添加或删除条目
	int length = 0 ;
	for (int i = 0 ; i < vsim.size(); i++)
	{   
		char *pname = new char[100];//名字这么大空间足够了
		unsigned char *desname = new unsigned char[100];//
		memset(cmd2,0,1024);
		memset(pname,0,200);
		memset(desname,0,200);	
		
		if (!m_bgsm)
		{
			char UCS2[] = "AT+CSCS = \"UCS2\"\r";
			Transaction_(UCS2, strlen(UCS2));
			GB2312toUSC2(vsim[i].name.c_str(),pname,strlen(vsim[i].name.c_str()));//将名字转换成USC2格式
		}
		else
		{
			int dlength = EncodeGsm(vsim[i].name.c_str(),desname,strlen(vsim[i].name.c_str()));
			gsmBytes2String(desname,pname,dlength);	
		}

		sprintf(cmd2,"AT+CPBW=%d,\"%s\",129,\"%s\"\r",0,vsim[i].telnum.c_str(),pname);		
		Transaction_(cmd2, strlen(cmd2));
		
		if (!m_bgsm)//恢复下编码集
		{
			char HEX[] = "AT+CSCS= \"GSM\"\r";
			Transaction_(HEX, strlen(HEX));
		}
		
		delete[]pname;
		delete[]desname;
	}
	
}

void ATCommandWarp::DeleteSim(int id)
{
	char cmd2[1024]; //向电话本中添加或删除条目
	sprintf(cmd2,"AT+CPBW=%d\r",id);
	Transaction_(cmd2, strlen(cmd2));

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
	pdst[it] = '\n';//结束标志，自己加的
	
	i = 0 ;
	while (pdst[i] !='\n')//把值转成相应的字符串
	{
		sprintf(des+num*2,"%02x",pdst[i]);
		num++;
		i++;
	}
	des[(num-1)*2+1] = '\0';
	
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
