
/////////////////////////////////////////////////////////////////////////////
// The one and only application object

#include "hwfcl.h"
#include "uart.h"
#include "vz328.h"
#include "Messagebox.h" 
extern void InitSerialPort(BOOL OpenFlag);
extern int SendData(char *pData);
extern int RecieveData(char *pBuff, int nBufLen);
extern BOOL SetPortBaudRate(UINT16 nNewRate);
 
extern void ClearRecieveBuf();
extern int FastSendData(char *pData);
extern int SendBinData(char *pData, int len);
extern void delay(INT32 millisecond);
extern BOOL TestAds(BOOL );

class CBurnWnd : public CAppWnd
{
private:
	CButton m_butStart;
public:
	DECLARE_MESSAGE_MAP(CBurnWnd)
public:
    MESSAGE_HANDLE void  OnStart(UINT32 nMessage,UINT32 nDocButtonID, UINT32 lParam);
	virtual	 BOOL OnCreate();
	virtual	 BOOL Create();
};

#define ID_START  30001
BEGIN_MESSAGE_MAP(CBurnWnd,CAppWnd)
ON_MESSAGE(ID_START, CN_COMMAND , OnStart)
END_MESSAGE_MAP 

BOOL CBurnWnd::Create()
{
	char szPasswordStr[50];
	memset(szPasswordStr,0x00,50);
	
	if(InputBox((char *)"烧录密码", (char *)szPasswordStr, 8, INIT_WRITE,FALSE)==IDOK)
	{
		if(strcmp((char*)szPasswordStr,"hanwang")!=0)
		{	
			::MessageBox("烧录密码错误！不能进行烧录操作！","错误",MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}	
	else
		return FALSE;

	if(!CAppWnd::Create("烧录工具", WS_CAPTION|WS_VISIBLE, DESK_TOP_RECT, 0))
		return FALSE;
	
	return TRUE;
}

BOOL CBurnWnd::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;

	if(!m_butStart.Create("烧录", CRect(40, 120, 120, 140 ), this, ID_START))
		return FALSE;

	return TRUE;
}

void  CBurnWnd::OnStart(UINT32 nMessage,UINT32 nDocButtonID, UINT32 lParam)
{    
     int BurnMain(); 
	 BurnMain(); 
}

////////////////////////////////////////////////////////////////////////////////
//
void DispInfo(char * pInfo)
{
	CDC		dc;
	CRect	r(10, 30, 140, 90);
	dc.EraseRect(r);
	dc.DrawText(r, pInfo);
}

struct CPrgBar
{
	CRect   rec;
	UINT32  length;
	UINT32  current;

    CPrgBar(UINT32 len)
	{
		length = len;
		current = 0;
		rec.SetRect(5,92,145,105); 
	}
    void Paint()
	{
      CDC dc;
	  dc.EraseRect(rec);
	  //dc.Draw3dRect(rec, GRAY, WHITE);
	  dc.FrameRect(rec); 
	  PaintCur();
   
	}
    void PaintCur()
	{
		CDC dc;
		dc.SetBackColor(GRAY); 
		if(length !=0)
		{
		 INT16 ypos = (current * rec.Width()) / length + rec.left+2;
		 if(ypos > rec.right-2)
			 ypos = rec.right-2;
		 dc.EraseRect(rec.left+2, rec.top+2, ypos, rec.bottom-2); 
		}
	}

	void SetLength(UINT32 newlength)
	{
		length = newlength;
	}
	void SetCurrent(UINT32 cur)
	{
		current = cur;
		PaintCur();
	}

};
////////////////////////////////////////////////////////////////////////////////
// 功  能： 本机通过UART口与处于BOOTSTRAP模式的别一台EPHONE连接测试
// 参  数： 无
// 返回值：
//  0     连接成功
//  1     无法与ADS板连接
//  2     19200  波特率通读不成功
//  3     38400  波特率通读不成功 
//  4     115200 波特率通读不成功 
// 
int ConnectADS()
{
   DispInfo("正在与E-Phone连接...");
   if(!TestAds(FALSE))
	  return 1;

   SendData("\nFFFFF2010180");
   delay(100);
  
   ::SetPortBaudRate(19200);
   delay(100);

   DispInfo("尝试采用19200波特率通讯...");
   if(!TestAds(FALSE))
	    return 2;

   SendData("\nFFFFF9020100");
   delay(100);
   
   DispInfo("尝试采用38400波特率通讯...");
   ::SetPortBaudRate(38400); 
   delay(100);

   if(!TestAds(TRUE))
   	   return 3;

   SendData("\nFFFFF9030138");
   delay(100);

   DispInfo("尝试采用115200波特率通讯...");
   ::SetPortBaudRate(115200); 
   delay(100);

   if(!TestAds(TRUE))
   		return 4;	   
   
   DispInfo("连接成功!");
   return 0;
}

int GetLine(unsigned char *pBuf, unsigned char *pSourceBuf,int cur, UINT32 maxlen)
{
	while(pSourceBuf[cur] && pSourceBuf[cur] != '\n' && cur<maxlen)
		*pBuf++ = pSourceBuf[cur++];

	if(cur < maxlen)
	    *pBuf++ = pSourceBuf[cur++];

	*pBuf = 0;
	return cur;
}

bool StrRcmp(unsigned char *str1, unsigned char *str2, int len)
{
	int i;
	for(i=0; i<len ; i++, str1++, str2++)
	{
		if(*str1 != *str2)
			return FALSE;
	} 
	return (i == 0) ? FALSE : TRUE;
}

inline void WriteUartByte(UINT8 byte)
{
	while ((INWORD(UTX) & BIT10) != 0)
		;

	OUTWORD(UTX, BIT11 | byte);
}

inline UINT8 GetUartByte()
{
	while((INBYTE(URX) & BIT5) == 0)
		;
	return INBYTE(URX+1);
}

const int  STEPLEN=16;   //一次发送的字节数
void ClearUartBuf()
{
	UINT8 data;
	while ((INBYTE(URX) & BIT5))
		data = INBYTE(URX+1);
}

bool SendBinBuff(unsigned char *pSourceBuf,unsigned long nSourceFileLen)
{
    char SendDataCountStr[50];
	unsigned long  i;
    UINT32 step = nSourceFileLen/40;
	if(step == 0)
		step = 1;
	
	CPrgBar  bar(nSourceFileLen);
	bar.Paint();

	for(i =0; i< nSourceFileLen; i++)
	{
		WriteUartByte(*pSourceBuf++);
	//	if(GetUartByte() != *pSourceBuf)
	//		break;
		if((i % step) == 0)
			bar.SetCurrent(i); 
	}
	bar.SetCurrent(i); 
	return (i >= nSourceFileLen);
}

bool SendBinFile(unsigned char *pSourceData,unsigned long addr,unsigned long nSourceFileLen)
{   

	unsigned long buf[2] = {addr, nSourceFileLen};
   
	if(!SendBinBuff((UINT8 *)buf, 8))
		return 0;
	if(!SendBinBuff(pSourceData, nSourceFileLen))
		return 0;

	return 1;

}
//unsigned long nSourceFileLen;           //源文件长度
bool SendTextFile( unsigned char *pSourceBuf,unsigned long nSourceFileLen)
{
	
	unsigned long nReceiveCount = 0;        //接收到的字节数
	unsigned long ErrCount      = 0;        //出错数
	unsigned long NonDataCount  = 0;        //未收到数的次数
	unsigned long Sendcur       = 0;        //已发送字节数
	unsigned char pReceiveBuf[514], pBuf[514];
	char SendDataCountStr[50];

	CPrgBar  bar(nSourceFileLen);
	bar.Paint();
	
	ClearRecieveBuf();
     while(nReceiveCount < nSourceFileLen)
	{ 
		if(Sendcur < nSourceFileLen)
		{
			Sendcur=GetLine(pBuf, pSourceBuf,Sendcur, nSourceFileLen);
			FastSendData((char *)pBuf);
		}
		unsigned long Datalen = (RecieveData((char *)pReceiveBuf, 512)); 
		pReceiveBuf[Datalen]=0;

		if(Datalen==0 && NonDataCount>0x800)
			 break;
		else if(Datalen == 0)
		{
		   NonDataCount++; 
		   continue;
		}
		NonDataCount=0;

		if(!StrRcmp(pSourceBuf+nReceiveCount, pReceiveBuf, Datalen))
		{
			if(ErrCount>=2)
				break;
			delay(50);
			ClearRecieveBuf();	
			while(pSourceBuf[nReceiveCount]!='\n'&&nReceiveCount)
				nReceiveCount--;
			if(pSourceBuf[nReceiveCount]=='\n')
				nReceiveCount++;
			Sendcur=nReceiveCount;
			ErrCount++;
			continue;
		}

		nReceiveCount+=Datalen;
		ErrCount=0;

		if((nReceiveCount % 32) == 0)
			bar.SetCurrent(nReceiveCount); 
	}
	 bar.SetCurrent(nSourceFileLen); 
	if(nReceiveCount < nSourceFileLen-3)
	 return 0;

	return 1;
}
#define BOOTADDR		(unsigned long)0x10A0000		//引子.dat
#define BOOTLEN			(unsigned long)33532			//引子.dat
#define ROMFILEADDR		(unsigned long)0x010F0000		//e201rom.dat
#define ROMFILELEN		(unsigned long)1104160			//e201rom.dat
#define ICONFILEADDR	(unsigned long)0x01200000		//e201icon.dat
#define ICONFILELEN		(unsigned long)65680			//e201icon.dat
#define RINGFILEADDR	(unsigned long)0x01220000		//e201ring.dat
#define RINGFILELEN		(unsigned long)16384			//e201ring.dat
#define DOCFILEADDR		(unsigned long)0x012B6000		//doc.dat
#define DOCFILELEN		(unsigned long)109278			//doc.dat
#define GREETFILEADDR	(unsigned long)0x012D6000		//greet.dat
#define GREETFILELEN	(unsigned long)34182			//greet.dat
#define PROGRAMFILEADDR	(unsigned long)0x01000000		//程序
#define PROGRAMFILELEN	(unsigned long)0x90000			//程序


int BurnMain()
{ 

	InitSerialPort(TRUE);
    if(ConnectADS() != 0)
	{
		DispInfo("无法与目标E-Phone连接,请检查联线是否正确,如果联线无误,请重启目标E-Phone后重试...");
		InitSerialPort(FALSE);
		return 0;
	}
	//引子
	DispInfo("与E-Phone连接成功，现在开始传输引导程序...");
	if(!SendTextFile((UINT8 *)BOOTADDR, BOOTLEN))
		goto Burn_Error;

	//
	DispInfo("等待引导程序运行...");
    delay(800);

    //关闭串口中断
	OUTWORD(USTCNT, 0xE100);

	//引子.dat
    DispInfo("传输烧录模块..."); 
	if(!SendBinFile((UINT8 *)BOOTADDR, BOOTADDR,BOOTLEN ))
		goto Burn_Error;

	//e201rom.dat
	DispInfo("传输字库及手写字典..."); 
	if(!SendBinFile((UINT8 *)ROMFILEADDR,ROMFILEADDR,ROMFILELEN))
		goto Burn_Error;

	//e201icon.dat
	DispInfo("传输图标数据..."); 
	if(!SendBinFile((UINT8 *)ICONFILEADDR, ICONFILEADDR,ICONFILELEN))
		goto Burn_Error;

	//e201ring.dat
	DispInfo("传输振铃数据..."); 
	if(!SendBinFile((UINT8 *)RINGFILEADDR, RINGFILEADDR,RINGFILELEN))
		goto Burn_Error;

	//doc.dat
	DispInfo("传输帮助和资料数据..."); 
	if(!SendBinFile((UINT8 *)DOCFILEADDR, DOCFILEADDR,DOCFILELEN))
		goto Burn_Error;

	//greet.dat
	DispInfo("传输问候语语音数据..."); 
    if(!SendBinFile((UINT8 *)GREETFILEADDR, GREETFILEADDR,GREETFILELEN))
		goto Burn_Error;


	//程序
	DispInfo("传输程序..."); 
	if(!SendBinFile((UINT8 *)PROGRAMFILEADDR, PROGRAMFILEADDR,PROGRAMFILELEN))
		goto Burn_Error;


	DispInfo("烧录完成，请重启目标机后进入系统设置检查校验和!");
	InitSerialPort(FALSE); 
	return 1;


Burn_Error:
	DispInfo("错误!"); 
   	InitSerialPort(FALSE); 
	return 0;
}

CBurnWnd * g_pBurnApp = NULL;

#define APPID_BURN   100000
void BurnAppRun()
{
	if(!g_pBurnApp)
		g_pBurnApp = new CBurnWnd;
	::SetCurrentApp(APPID_BURN);
	if(g_pBurnApp->Create())
	{	
		g_pBurnApp->ShowWindow();
		DispInfo("注：烧录开始后，完成之前无法停止!");
	}
}

unsigned long GetCheckSum()
{
	unsigned long dwCheckSum=0;
	unsigned long *pdwData;
	unsigned long i;

	//引子.dat
	pdwData=(unsigned long*)BOOTADDR;
	for(i=0;i<BOOTLEN;i+=4)
		dwCheckSum+=*pdwData++;
	
	//e201rom.dat
	pdwData=(unsigned long*)ROMFILEADDR;
	for(i=0;i<ROMFILELEN;i+=4)
		dwCheckSum+=*pdwData++;
	
	//e201icon.dat
	pdwData=(unsigned long*)ICONFILEADDR;
	for(i=0;i<ICONFILELEN;i+=4)
		dwCheckSum+=*pdwData++;

	//e201ring.dat
	pdwData=(unsigned long*)RINGFILEADDR;
	for(i=0;i<RINGFILELEN;i+=4)
		dwCheckSum+=*pdwData++;

	//doc.dat
	pdwData=(unsigned long*)DOCFILEADDR;
	for(i=0;i<DOCFILELEN;i+=4)
		dwCheckSum+=*pdwData++;

	//greet.dat
	pdwData=(unsigned long*)GREETFILEADDR;
	for(i=0;i<GREETFILELEN;i+=4)
		dwCheckSum+=*pdwData++;

	//程序
	pdwData=(unsigned long*)PROGRAMFILEADDR;
	for(i=0;i<PROGRAMFILELEN;i+=4)
		dwCheckSum+=*pdwData++;
	return dwCheckSum;	
}