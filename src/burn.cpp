
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
	
	if(InputBox((char *)"��¼����", (char *)szPasswordStr, 8, INIT_WRITE,FALSE)==IDOK)
	{
		if(strcmp((char*)szPasswordStr,"hanwang")!=0)
		{	
			::MessageBox("��¼������󣡲��ܽ�����¼������","����",MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}	
	else
		return FALSE;

	if(!CAppWnd::Create("��¼����", WS_CAPTION|WS_VISIBLE, DESK_TOP_RECT, 0))
		return FALSE;
	
	return TRUE;
}

BOOL CBurnWnd::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;

	if(!m_butStart.Create("��¼", CRect(40, 120, 120, 140 ), this, ID_START))
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
// ��  �ܣ� ����ͨ��UART���봦��BOOTSTRAPģʽ�ı�һ̨EPHONE���Ӳ���
// ��  ���� ��
// ����ֵ��
//  0     ���ӳɹ�
//  1     �޷���ADS������
//  2     19200  ������ͨ�����ɹ�
//  3     38400  ������ͨ�����ɹ� 
//  4     115200 ������ͨ�����ɹ� 
// 
int ConnectADS()
{
   DispInfo("������E-Phone����...");
   if(!TestAds(FALSE))
	  return 1;

   SendData("\nFFFFF2010180");
   delay(100);
  
   ::SetPortBaudRate(19200);
   delay(100);

   DispInfo("���Բ���19200������ͨѶ...");
   if(!TestAds(FALSE))
	    return 2;

   SendData("\nFFFFF9020100");
   delay(100);
   
   DispInfo("���Բ���38400������ͨѶ...");
   ::SetPortBaudRate(38400); 
   delay(100);

   if(!TestAds(TRUE))
   	   return 3;

   SendData("\nFFFFF9030138");
   delay(100);

   DispInfo("���Բ���115200������ͨѶ...");
   ::SetPortBaudRate(115200); 
   delay(100);

   if(!TestAds(TRUE))
   		return 4;	   
   
   DispInfo("���ӳɹ�!");
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

const int  STEPLEN=16;   //һ�η��͵��ֽ���
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
//unsigned long nSourceFileLen;           //Դ�ļ�����
bool SendTextFile( unsigned char *pSourceBuf,unsigned long nSourceFileLen)
{
	
	unsigned long nReceiveCount = 0;        //���յ����ֽ���
	unsigned long ErrCount      = 0;        //������
	unsigned long NonDataCount  = 0;        //δ�յ����Ĵ���
	unsigned long Sendcur       = 0;        //�ѷ����ֽ���
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
#define BOOTADDR		(unsigned long)0x10A0000		//����.dat
#define BOOTLEN			(unsigned long)33532			//����.dat
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
#define PROGRAMFILEADDR	(unsigned long)0x01000000		//����
#define PROGRAMFILELEN	(unsigned long)0x90000			//����


int BurnMain()
{ 

	InitSerialPort(TRUE);
    if(ConnectADS() != 0)
	{
		DispInfo("�޷���Ŀ��E-Phone����,���������Ƿ���ȷ,�����������,������Ŀ��E-Phone������...");
		InitSerialPort(FALSE);
		return 0;
	}
	//����
	DispInfo("��E-Phone���ӳɹ������ڿ�ʼ������������...");
	if(!SendTextFile((UINT8 *)BOOTADDR, BOOTLEN))
		goto Burn_Error;

	//
	DispInfo("�ȴ�������������...");
    delay(800);

    //�رմ����ж�
	OUTWORD(USTCNT, 0xE100);

	//����.dat
    DispInfo("������¼ģ��..."); 
	if(!SendBinFile((UINT8 *)BOOTADDR, BOOTADDR,BOOTLEN ))
		goto Burn_Error;

	//e201rom.dat
	DispInfo("�����ֿ⼰��д�ֵ�..."); 
	if(!SendBinFile((UINT8 *)ROMFILEADDR,ROMFILEADDR,ROMFILELEN))
		goto Burn_Error;

	//e201icon.dat
	DispInfo("����ͼ������..."); 
	if(!SendBinFile((UINT8 *)ICONFILEADDR, ICONFILEADDR,ICONFILELEN))
		goto Burn_Error;

	//e201ring.dat
	DispInfo("������������..."); 
	if(!SendBinFile((UINT8 *)RINGFILEADDR, RINGFILEADDR,RINGFILELEN))
		goto Burn_Error;

	//doc.dat
	DispInfo("�����������������..."); 
	if(!SendBinFile((UINT8 *)DOCFILEADDR, DOCFILEADDR,DOCFILELEN))
		goto Burn_Error;

	//greet.dat
	DispInfo("�����ʺ�����������..."); 
    if(!SendBinFile((UINT8 *)GREETFILEADDR, GREETFILEADDR,GREETFILELEN))
		goto Burn_Error;


	//����
	DispInfo("�������..."); 
	if(!SendBinFile((UINT8 *)PROGRAMFILEADDR, PROGRAMFILEADDR,PROGRAMFILELEN))
		goto Burn_Error;


	DispInfo("��¼��ɣ�������Ŀ��������ϵͳ���ü��У���!");
	InitSerialPort(FALSE); 
	return 1;


Burn_Error:
	DispInfo("����!"); 
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
		DispInfo("ע����¼��ʼ�����֮ǰ�޷�ֹͣ!");
	}
}

unsigned long GetCheckSum()
{
	unsigned long dwCheckSum=0;
	unsigned long *pdwData;
	unsigned long i;

	//����.dat
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

	//����
	pdwData=(unsigned long*)PROGRAMFILEADDR;
	for(i=0;i<PROGRAMFILELEN;i+=4)
		dwCheckSum+=*pdwData++;
	return dwCheckSum;	
}