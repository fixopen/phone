// MultimediaPhoneDlg.cpp : implementation file
//


#include "stdafx.h"
#include <ntddndis.h>
#include "Pblmember.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"
#include "./View/ViewConstants.h"
#include "./Data/SkinStyle.h"
#include "./Util/socket.h"
#include "./cximageinc/ximage.h"
#include "./data/Message.h"
#include "./data/Otadata.h"
#include "./data/OtaMessage.h"
#include "./data/MmsMessage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "View/ContactDlg.h"
#include "View/InformationDlg.h"
#include "View/SettingDlg.h"
#include "View/ContactNewDlg.h"
#include "Data/OTAData.h"
#include "Data/LanguageResource.h"
#include "sip.h"

//#define WM_PLAYVIDEO	8002
#define  BMP_WIDTH			 800
#define  BMP_HEIGHT          420
#define  BMP_WIDTH1			 692
#define  BMP_HEIGHT1         296

#define  BMP_WIDTH2			 800
#define  BMP_HEIGHT2         57
#define  BMP_WIDTH3			 600  //535
#define  BMP_HEIGHT3         420
#define  BMP_WIDTH4			 800
#define  BMP_HEIGHT4         480
#define  BMP_WIDTH5			 535
#define  BMP_HEIGHT5         420
#define  BMP_WIDTH6			 800
#define  BMP_HEIGHT6         423
#define  BMP_WIDTH7			 800
#define  BMP_HEIGHT7         92
#define  BMP_WIDTH8			 440
#define  BMP_HEIGHT8         270
#define  BMP_WIDTH9			 317
#define  BMP_HEIGHT9         376
#define  BMP_WIDTH10		 317
#define  BMP_HEIGHT10        375
#define  BMP_WIDTH11		 317
#define  BMP_HEIGHT11        375
#define  BMP_WIDTH12		 440
#define  BMP_HEIGHT12        440
#define  BMP_WIDTH13		 440
#define  BMP_HEIGHT13        340

#define		G3_MODULOR		 1

//add by qi 2009_09_15
#define  DATE_SET_TIEMR     10 
	

#define  KEYTIMEOUT  60 * 60 * 1000
#define  RINGTIMEOUT  6 * 1000
extern  BOOL DetectDIR(TCHAR *sDir);
extern  BOOL g_isSimEnable;
extern  BOOL g_IsSignal;
extern VOID WriteMyLog_(char *ptr, int size);
extern int HttpProcesse(void *pParam);
extern BOOL g_isShift;
extern BOOL g_isAutoPlay;
extern int gBatteryLevel[];
extern int g_BatteryLevel;
extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);

extern bool g_bSendReport;
extern bool g_bReadReport;
std::string g_sRecipientAddress = "";

BOOL g_MemoryLeak = FALSE;
BOOL g_IsRun = FALSE;
BOOL g_bAdjustPanel = FALSE;
BOOL g_bAutoSearchNet = TRUE ;
static int timeCount = 0;
HANDLE g_hEvent = NULL;
BOOL g_bReplay = FALSE;
BOOL g_bStartring = FALSE;

extern VOID WriteMyLog_(char *ptr, int size);
extern BOOL FindVideoDlg(BOOL flag);	

CRITICAL_SECTION csCom;

void InsertTestData2DB()//往数据库中添加测试数据
{	
	//通讯录
	extern BOOL RandName(char chName[], int nBufSize);	//nBufSize >= 10
	extern BOOL RandPhone(char chPhone[12], int nBufSize);	//nBufSize >= 12

	const char *pCompany[] = {
		"龙族鲜花店",	
			"怡红快绿鲜花店",
			"送爽鲜花店",
			"阅色珠宝玉器",
			"童氏地缘珠宝",
			"宝之苑商贸有限公司",
			"靓伊爱尔工业饰品连锁(中国）总部",
			"颐和祥珠宝有限公司",
			"浙江温岭雪瑶珍珠养殖公司",
			"中鼎元珠宝有限公司",
			"聚天宝首饰行",
			"七色麻服装专卖店",
			"FUBU专卖店",
			"北京东圣高策商贸有限公司",
			"北京山之苑商贸有限公司",
			"天雅服装市场",
			"北京动物园天乐服装批发市场",
			"北京福昌隆服装店",
			"北京百衣百顺时装店",
			"北京瑞丽华服装店",
			"北京梅兰妮服装店",
			"品博苑",
			"丰泽园饭庄",
			"湘川鄂情",
			"嘉丰湘菜馆",
			"龙鼎湘江",
			"红色江山",
			"潭州酒楼",
			"湘鄂春",
			"洞庭楼"
	};		//30
	
	const char *pDepartment[] = {
		"董事长办公室",
			"总经理办公室",
			"人力资源部",
			"企管部",
			"技术部",
			"生产部",
			"销售部",
			"财务部",
			"后勤部",
			"基建设备部",
			"采购部"
	};

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nCount = 0  ;
	/*while (nCount < 3000)
	{
		::Sleep(100);
		std::auto_ptr<Data::Contact> pContact( new Data::Contact );
		
		char chName[16] = {0};
		char chPhone[16] = {0};
		RandName(chName, 16);
		RandPhone(chPhone, 16);
		
		pContact->alias(main->m_pContactNewDlg->GetAlias(chName));
		pContact->name(chName);
		pContact->mobilesTelephone(Data::TelephoneNumber(chPhone));
		
		int nIndex = rand()%30;
		pContact->company(pCompany[nIndex]);
		nIndex = rand()%11;
		pContact->department(pDepartment[nIndex]);
		
		pContact->Insert(); 
		nCount++;
	}
	
	//通话记录
	nCount = 0 ;
	std::string number[10] = 
		{
			"18811038275",
			"18811038275",
			"18811038175",
			"13520201043",
			"13520201241",
			"13520201241",
			"13520201241",
			"18811038175",
			"18811038175",
			"13520201043",
		};
	while (nCount < 3000)
	{
		::Sleep(100);
		std::auto_ptr<Data::ContactInfo> pContactInfo( new Data::ContactInfo );
		pContactInfo->startTime(CTime::GetCurrentTime());
		pContactInfo->type(Data::citInNo);
		pContactInfo->telephoneNumber(Data::TelephoneNumber(number[rand()%10]));
		pContactInfo->duration(3400);
		pContactInfo->contactId(1);

		char chName[16] = {0};
		RandName(chName, 16);
		pContactInfo->name(chName);

		pContactInfo->Insert();
		nCount++;
	}*/

	//短信
	nCount = 0 ;
	std::string address[7] = 
		{
			"18811038275",
			"18811038275",
			"18811038175",
			"13520201043",
			"13520201241",
			"13623223231",
			"13623223231",
		};

	while (nCount < 3000)
	{
		::Sleep(60);
		std::auto_ptr<Data::Message> pMessage( new Data::Message );
		
		pMessage->unicodeData = pCompany[rand()%20];
		
		pMessage->remote.address = address[rand()%7];
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		pMessage->timestamp.year = tm.wYear%100;
		pMessage->timestamp.month = tm.wMonth;
		pMessage->timestamp.day = tm.wDay;
		pMessage->timestamp.hour = tm.wHour;
		pMessage->timestamp.minite = tm.wMinute;
		pMessage->timestamp.second = tm.wSecond;
		
		pMessage->reference = 0;
		pMessage->uplevelProtocol = 200;	
				
		pMessage->state = Data::Message::sNoRead;
		if (nCount < 1000)//发件箱
		{
			pMessage->group = Data::Message::gSend;
		}
		else if (nCount >= 1000 && nCount < 2000)//收件箱
		{
			pMessage->group = Data::Message::gReceive;
		}
		else if (nCount >= 2000 && nCount < 3000)//草稿箱
		{
			pMessage->group = Data::Message::gUnSend;
		}	
		nCount++;
		pMessage->Insert();
	}
	
	//彩信

}

void CopyDirFiles()
{
	CString SrcDir = "/usbdisk";
	CString DesDir = "/flashdrv/my_photo";
	CString findFilename = SrcDir + "/*.*";
	//wprintf(_T("find file name is %s\n"), (LPCTSTR)findFilename);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		Dprintf("not find file\n");
	}
	else
	{
		bool finished = false;
		do
		{
			wchar_t wideToName[256];
			//	wchar_t root[256] = {0};
			//	mbstowcs(root, rootPath.c_str(), rootPath.length());
			wsprintf(wideToName, _T("%s/%s"), DesDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			//	wideToName =  
			wchar_t wideFromName[256] = {0};
			//	wchar_t usb[256] = {0};
			wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//wprintf(_T("create dir %s to %s\n"), wideFromName, wideToName);
				//CopyDirFiles(wideFromName, wideToName);
			}
			else
			{
				if (!::CopyFile(wideFromName, wideToName, false))
				{
					Dprintf("current error is %d.", GetLastError());
				}
//				wprintf(_T("copy %s to %s\n"), wideFromName, wideToName);
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					Dprintf("find end.\n");
					finished = true;
				}
				else
				{
					Dprintf("Couldn't find next file.\n");
				}
			}
		} while (!finished);
		FindClose(hFind);
	}
}

int ALLBMPMEMORY_MAX =	0;
int	g_ALLBMPMemoryCount = -1;
struct  ALLBmpMemoryInfo
{
	CString  sFileName;
	HDC		 hdc;
	HBITMAP  hbmp;
	HBITMAP  hOldbmp;
	CRect	 rect;
};

ALLBmpMemoryInfo *g_allBmpMemory;
void InitALLBmpMemoryInfo()
{

	MEMORYSTATUS   mem;     
	mem.dwLength=sizeof(mem);     
	GlobalMemoryStatus(&mem);     
    if( mem.dwAvailPhys >= 64*1024*1024)
	{ 
		ALLBMPMEMORY_MAX = 0;     //   15   remark 
		Dprintf("BMP Allac 30\r\n");
	}
	else
	{
		Dprintf("BMP Allac 0\r\n");
	}

	if(ALLBMPMEMORY_MAX > 0)
		g_allBmpMemory = new ALLBmpMemoryInfo[ALLBMPMEMORY_MAX];

	for(int i = 0; i < ALLBMPMEMORY_MAX; i++)
	{
		g_allBmpMemory[i].sFileName = "";
		g_allBmpMemory[i].hdc = NULL;
		g_allBmpMemory[i].rect = CRect(0, 0, 0 ,0);
		g_allBmpMemory[i].hbmp = NULL;
		g_allBmpMemory[i].hOldbmp = NULL;
	}
}

BOOL FindMemoryInfo(CString sAllFilename, int &inDexAllBmpMemeoryinfo)
{
	for(int i = 0; i <= g_ALLBMPMemoryCount; i++)
	{
		if(g_allBmpMemory[i].sFileName == sAllFilename)
		{
			inDexAllBmpMemeoryinfo = i;
			
		//	wprintf(g_allBmpMemory[i].sFileName);
//			Dprintf(" FindAllMemeory %d \r\n", i);
			
			return TRUE;
		}
	}
	return FALSE;
}

BOOL AddALLBmpMemoryInfo(CString sFileName, CRect rect, CDC *pdc)
{
	int n;
	if(FindMemoryInfo(sFileName, n))
		return FALSE;

	if(g_ALLBMPMemoryCount < (ALLBMPMEMORY_MAX - 1))
	{
		g_ALLBMPMemoryCount++;

		g_allBmpMemory[g_ALLBMPMemoryCount].sFileName = sFileName;
		g_allBmpMemory[g_ALLBMPMemoryCount].rect = rect;
		g_allBmpMemory[g_ALLBMPMemoryCount].hdc = CreateCompatibleDC(pdc->m_hDC); 
		
		BYTE  *lpBitmapBits = NULL;
		BITMAPINFO RGB24BitsBITMAPINFO; 
		ZeroMemory(&RGB24BitsBITMAPINFO, sizeof(BITMAPINFO));
		RGB24BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		RGB24BitsBITMAPINFO.bmiHeader.biWidth = rect.Width();
		RGB24BitsBITMAPINFO.bmiHeader.biHeight = rect.Height();
		RGB24BitsBITMAPINFO.bmiHeader.biPlanes = 1;
		RGB24BitsBITMAPINFO.bmiHeader.biBitCount = 24;
		
		g_allBmpMemory[g_ALLBMPMemoryCount].hbmp = CreateDIBSection(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, (BITMAPINFO*)&RGB24BitsBITMAPINFO, DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
		g_allBmpMemory[g_ALLBMPMemoryCount].hOldbmp = (HBITMAP)SelectObject(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, g_allBmpMemory[g_ALLBMPMemoryCount].hbmp);
		BitBlt(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, 0, 0, rect.Width(), rect.Height(), pdc->m_hDC, 0, 0, SRCCOPY);

	//	wprintf(sFileName);
		Dprintf(" AddAllMemeory %d \r\n", g_ALLBMPMemoryCount);
		
		return TRUE;
	}
	return FALSE;
}

BOOL AddALLBmpMemoryInfo(CString sFileName, CRect rect, HDC pdc)
{
	int n;
	if(FindMemoryInfo(sFileName, n))
		return FALSE;
	
	if(g_ALLBMPMemoryCount < (ALLBMPMEMORY_MAX - 1))
	{
		g_ALLBMPMemoryCount++;
		
		g_allBmpMemory[g_ALLBMPMemoryCount].sFileName = sFileName;
		g_allBmpMemory[g_ALLBMPMemoryCount].rect = rect;
		g_allBmpMemory[g_ALLBMPMemoryCount].hdc = CreateCompatibleDC(pdc); 
		
		BYTE  *lpBitmapBits = NULL;
		BITMAPINFO RGB24BitsBITMAPINFO; 
		ZeroMemory(&RGB24BitsBITMAPINFO, sizeof(BITMAPINFO));
		RGB24BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		RGB24BitsBITMAPINFO.bmiHeader.biWidth = rect.Width();
		RGB24BitsBITMAPINFO.bmiHeader.biHeight = rect.Height();
		RGB24BitsBITMAPINFO.bmiHeader.biPlanes = 1;
		RGB24BitsBITMAPINFO.bmiHeader.biBitCount = 24;
		
		g_allBmpMemory[g_ALLBMPMemoryCount].hbmp = CreateDIBSection(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, (BITMAPINFO*)&RGB24BitsBITMAPINFO, DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
		g_allBmpMemory[g_ALLBMPMemoryCount].hOldbmp = (HBITMAP)SelectObject(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, g_allBmpMemory[g_ALLBMPMemoryCount].hbmp);
		BitBlt(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, 0, 0, rect.Width(), rect.Height(), pdc, 0, 0, SRCCOPY);
		
		//	wprintf(sFileName);
		Dprintf(" AddAllMemeory %d \r\n", g_ALLBMPMemoryCount);
		
		return TRUE;
	}
	return FALSE;
}

BOOL ReleaseALLBmpMemoryInfo()
{
	for(int i = 0; i <= g_ALLBMPMemoryCount; i++)
	{
		SelectObject(g_allBmpMemory[i].hdc, g_allBmpMemory[i].hOldbmp);
		DeleteObject(g_allBmpMemory[i].hbmp);
		DeleteDC(g_allBmpMemory[i].hdc);
	}
	g_ALLBMPMemoryCount = -1;
	return TRUE;
}

#include   <INITGUID.h>   
IImagingFactory   *pImgFactory;

void InitImage()
{
	CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
}

void  DrawImage(LPCTSTR szFileImage, CDC *pdc, CRect rect, BOOL isOffset = FALSE) 
{ 

	//	unsigned int w = DMemprintf("DrawImage 0");
	CRect newRect = rect;
	if(isOffset)
	{
		newRect = CRect(0, 0, rect.Width(), rect.Height());
	}

	CString sFile = szFileImage;
	if(sFile.Find(L".bmp") != -1 || sFile.Find(L".BMP") != -1)
	{
		//读bmp文件头的数据  

		CFile   file;   
		if( !file.Open(szFileImage,  CFile::modeRead ) )   
		{     
			return;
		}  
		
		
		BITMAPFILEHEADER   bfhHeader;   
		file.Read(&bfhHeader,sizeof(BITMAPFILEHEADER));   
		if(bfhHeader.bfType!=((WORD)   ('M'<<8)|'B'))   
		{   
			::MessageBox(NULL,L"The   file   is   not   a   file!", L"warning",MB_OK);  
			file.Close();
			return;   
		}   
		if(bfhHeader.bfSize!=file.GetLength())     
		{   
			::MessageBox(NULL,L"The   BMP   file   header   error!", L"warning", MB_OK);   
			file.Close();
			return;   
		}   
		UINT   uBmpInfoLen=(UINT)   bfhHeader.bfOffBits-sizeof(BITMAPFILEHEADER);   
		LPBITMAPINFO   lpBitmap= (LPBITMAPINFO)   new   BYTE[uBmpInfoLen];  
		
		file.Read((LPVOID)   lpBitmap,uBmpInfoLen);   
		if((*   (LPDWORD)(lpBitmap))!=sizeof(BITMAPINFOHEADER))     
		{   
			::MessageBox(NULL,L"The   BMP   is   not   Windows   3.0   format!", L"warning",MB_OK);  
			file.Close();
			delete []lpBitmap;    //20090216 test
			return;   
		}     
		DWORD   dwBitlen=bfhHeader.bfSize   -   bfhHeader.bfOffBits;     
		LPVOID   lpBits;   
		//TRACE(L"file DC 1 %d\n", offset);
		
		/*
			如果背景图是 800*420，692*296，535*420，800*60，Unit框小于他们，则取出相应的BMP画出来.否则整个画
		*/
		if(((lpBitmap->bmiHeader.biWidth == BMP_WIDTH && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT) && (!(rect.Width() >= BMP_WIDTH && rect.Height() >= BMP_HEIGHT))) ||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH1 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT1) && (!(rect.Width() >= BMP_WIDTH1 && rect.Height() >= BMP_HEIGHT1)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH2 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT2) && (!(rect.Width() >= BMP_WIDTH2 && rect.Height() >= BMP_HEIGHT2))) ||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH3 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT3) && (!(rect.Width() >= BMP_WIDTH3 && rect.Height() >= BMP_HEIGHT3)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH5 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT5) && (!(rect.Width() >= BMP_WIDTH5 && rect.Height() >= BMP_HEIGHT5)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH4 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT4) && (!(rect.Width() >= BMP_WIDTH4 && rect.Height() >= BMP_HEIGHT4))))
		{
			;
		}
		else
		{
			BOOL flag = FALSE;
			if(BMP_WIDTH == lpBitmap->bmiHeader.biWidth || BMP_WIDTH1 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH2 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH3 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH5 == lpBitmap->bmiHeader.biWidth)
			{
				flag = TRUE;

				int index;;
				if(FindMemoryInfo(szFileImage, index))
				{
					if(index >= 0 && index < ALLBMPMEMORY_MAX)
					{
						BitBlt(pdc->m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);
						

						file.Close();
						return;
					}
				}
			}
		
			file.Close();
			delete []lpBitmap;    //20090216 test
			
			IImage   *pImage   =   NULL; 
			
			//	CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
			
			//   Create   the   imaging   factory. 
			if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
			{ 
				//   Load   the   image   from   the   JPG   file. 
				if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
				{ 
					//   Draw   the   image. 
					pImage->Draw(pdc->m_hDC, rect, NULL); 
					pImage->Release(); 
					
					if(flag)
						AddALLBmpMemoryInfo(szFileImage, rect, pdc);

					::Sleep(1);
				} 
				else
				{
					Dprintf("DrawImage error\n");
				}
				
				pImgFactory-> Release(); 
			}

			return;
		}
		//创建HBITMAP句柄   

		/*
		重新根据rect 创建位图信息
		*/

		int w = (((lpBitmap->bmiHeader.biWidth*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;
		int h = lpBitmap->bmiHeader.biHeight;
		lpBitmap->bmiHeader.biHeight = rect.Height();
		lpBitmap->bmiHeader.biWidth = rect.Width();

		CDC   dcMem;   
		dcMem.CreateCompatibleDC(pdc);   
		HBITMAP   hBmp = ::CreateDIBSection(dcMem.m_hDC, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
		if(lpBits == NULL)
		{
			//::MessageBox(NULL,L"Draw BMP no memory!", L"warning", MB_OK);  
			//重启机器

			extern VOID WriteLog(CHAR* str);
			char restartTimer[64];
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			sprintf(restartTimer, "memleak watchdog:%04d-%02d-%02d %02d:%02d:%02d \r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
//			WriteLog(restartTimer);

			file.Close();
			g_MemoryLeak = TRUE;
			delete []lpBitmap;    //20090216 test
			return;
		}

		//TRACE(L"file DC 2 %d\n", offset);
	
		/*
		file.ReadHuge(lpBits, dwBitlen);  
		*/
		int off = (h-rect.top-rect.Height())*w + rect.left*(lpBitmap->bmiHeader.biBitCount/8);

		file.Seek(off, CFile::current);
		int size = (((rect.Width()*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;   //rect.Width()*3;
		int offline = w - size; 
		
		for(int i = 0; i < rect.Height(); i++)
		{
		   	file.Read(lpBits, size); 
			LPBYTE lpbyte = (LPBYTE)lpBits;
			lpbyte += size;
			lpBits = (LPVOID)lpbyte;
			file.Seek(offline, CFile::current);
		}
		file.Close();       
		
		//TRACE(L"file DC 3 %d\n", offset);

		CBitmap   BmpImg;   
		BmpImg.Attach(hBmp);   
		
		CBitmap  *pOldBmp=dcMem.SelectObject(&BmpImg);   
	
		//TRACE(L"file DC 4 %d\n", offset);

    	//pdc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
		pdc->BitBlt(newRect.left, newRect.top, newRect.Width(), newRect.Height(), &dcMem, 0, 0, SRCCOPY);
	
		dcMem.SelectObject(pOldBmp); 
	
		DeleteObject(hBmp);

		BmpImg.Detach();
		dcMem.DeleteDC();
	
		delete []lpBitmap;    //20090216 test

		//TRACE(L"end file DC %d\n", offset);
		::Sleep(1);
	}
	else
	{

		IImage   *pImage   =   NULL; 
		
		//	CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
		
		//   Create   the   imaging   factory. 
		if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
		{ 
			//   Load   the   image   from   the   JPG   file. 
			if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
			{ 
				//   Draw   the   image. 
				
				pImage->Draw(pdc->m_hDC, rect, NULL); 
				pImage->Release(); 
				::Sleep(1);
			} 
			else
			{
				Dprintf("DrawImage error\n");
			}
			
			pImgFactory-> Release(); 
		} 

	}

}

void  DrawImage_HDC(LPCTSTR szFileImage, HDC pdc, CRect rect, BOOL isOffset = FALSE, VOID *lpParamBits = NULL) 
{ 
//	unsigned int w = DMemprintf("DrawImage 0");

	if(wcsstr(szFileImage,(L".png")) > 0)
	{
		CxImage *image_;
		image_ = new CxImage();
		if(image_)
		{
			//CDC *pdc = GetDC(); 
			bool ret = image_->Load(szFileImage, CXIMAGE_FORMAT_PNG);
			image_->Draw(pdc, rect);
			//ReleaseDC(pdc);
			delete image_;
		}
		return ;
	}

	CRect newRect = rect;
	if(isOffset)
	{
		newRect = CRect(0, 0, rect.Width(), rect.Height());
	}

	CString sFile = szFileImage;
	if(sFile.Find(L".bmp") != -1 || sFile.Find(L".BMP") != -1)
	{
		//读bmp文件头的数据  

		CFile   file;   
		if(!file.Open(szFileImage,  CFile::modeRead ) )   
		{     
			return;
		}  
			
		BITMAPFILEHEADER   bfhHeader;   
		file.Read(&bfhHeader,sizeof(BITMAPFILEHEADER));   
		if(bfhHeader.bfType!=((WORD)   ('M'<<8)|'B'))   
		{   
			::MessageBox(NULL,L"The   file   is   not   a   file!", L"warning",MB_OK);  
			file.Close();
			return;   
		}   
		if(bfhHeader.bfSize!=file.GetLength())     
		{   
			::MessageBox(NULL,L"The   BMP   file   header   error!", L"warning", MB_OK);   
			file.Close();
			return;   
		}   
		UINT   uBmpInfoLen=(UINT)   bfhHeader.bfOffBits-sizeof(BITMAPFILEHEADER);   
		LPBITMAPINFO   lpBitmap= (LPBITMAPINFO)   new   BYTE[uBmpInfoLen];  
		
		file.Read((LPVOID)   lpBitmap,uBmpInfoLen);   
		if((*   (LPDWORD)(lpBitmap))!=sizeof(BITMAPINFOHEADER))     
		{   
			::MessageBox(NULL,L"The   BMP   is   not   Windows   3.0   format!", L"warning",MB_OK);  
			file.Close();
			delete []lpBitmap;    //20090216 test
			return;   
		}     
		DWORD   dwBitlen=bfhHeader.bfSize   -   bfhHeader.bfOffBits;     
		LPVOID   lpBits;   
		
		/*
			如果背景图是 800*420，692*296，535*420，800*60，Unit框小于他们，则取出相应的BMP画出来.否则整个画
		*/
		if(((lpBitmap->bmiHeader.biWidth == BMP_WIDTH && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT) && (!(rect.Width() >= BMP_WIDTH && rect.Height() >= BMP_HEIGHT))) ||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH1 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT1) && (!(rect.Width() >= BMP_WIDTH1 && rect.Height() >= BMP_HEIGHT1)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH2 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT2) && (!(rect.Width() >= BMP_WIDTH2 && rect.Height() >= BMP_HEIGHT2))) ||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH3 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT3) && (!(rect.Width() >= BMP_WIDTH3 && rect.Height() >= BMP_HEIGHT3)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH5 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT5) && (!(rect.Width() >= BMP_WIDTH5 && rect.Height() >= BMP_HEIGHT5)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH6 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT6) && (!(rect.Width() >= BMP_WIDTH6 && rect.Height() >= BMP_HEIGHT6)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH7 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT7) && (!(rect.Width() >= BMP_WIDTH7 && rect.Height() >= BMP_HEIGHT7)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH8 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT8) && (!(rect.Width() >= BMP_WIDTH8 && rect.Height() >= BMP_HEIGHT8)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH9 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT9) && (!(rect.Width() >= BMP_WIDTH9 && rect.Height() >= BMP_HEIGHT9)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH10 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT10) && (!(rect.Width() >= BMP_WIDTH10 && rect.Height() >= BMP_HEIGHT10)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH11 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT11) && (!(rect.Width() >= BMP_WIDTH11 && rect.Height() >= BMP_HEIGHT11)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH12 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT12) && (!(rect.Width() >= BMP_WIDTH12 && rect.Height() >= BMP_HEIGHT12)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH13 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT13) && (!(rect.Width() >= BMP_WIDTH13 && rect.Height() >= BMP_HEIGHT13)))||\
			((lpBitmap->bmiHeader.biWidth == BMP_WIDTH4 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT4) && (!(rect.Width() >= BMP_WIDTH4 && rect.Height() >= BMP_HEIGHT4))))
		{
			;
		}
		else
		{
			BOOL flag = FALSE;
			if(BMP_WIDTH == lpBitmap->bmiHeader.biWidth || BMP_WIDTH1 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH2 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH3 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH5 == lpBitmap->bmiHeader.biWidth)
			{
				flag = TRUE;

				int index;
				if(FindMemoryInfo(szFileImage, index))
				{
					if(index >= 0 && index < ALLBMPMEMORY_MAX)
					{
						BitBlt(pdc, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);
						
						file.Close();
						return;
					}
				}

				if(lpParamBits)
				{

					int size = (((rect.Width()*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2; 
					file.Read(lpParamBits, size*rect.Height());
					file.Close();   
					delete []lpBitmap;    //20090216 test
					
				//	::Sleep(1);
					return;
				}

				////////////////////////////////////////////////////////////////////////////
				int w = (((lpBitmap->bmiHeader.biWidth*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;
				int h = lpBitmap->bmiHeader.biHeight;
				lpBitmap->bmiHeader.biHeight = rect.Height();
				lpBitmap->bmiHeader.biWidth = rect.Width();

				HDC dcMem = ::CreateCompatibleDC(pdc);   
				HBITMAP   hBmp = ::CreateDIBSection(pdc, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
				if(lpBits == NULL)
				{
					::MessageBox(NULL,L"Draw BMP no memory!", L"warning", MB_OK);  
					//重启机器
					//return;
				}

			
				int off = (h-rect.top-rect.Height())*w + rect.left*(lpBitmap->bmiHeader.biBitCount/8);

				file.Seek(off, CFile::current);
				int size = (((rect.Width()*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;   //rect.Width()*3;
				int offline = w - size; 
				
				file.Read(lpBits, size*rect.Height());
				file.Close();       				

				HBITMAP pOldBmp= (HBITMAP)::SelectObject(dcMem, hBmp);      //20090723
				
    			::BitBlt(pdc, newRect.left, newRect.top, newRect.Width(), newRect.Height(), dcMem, 0, 0, SRCCOPY); 
				if(flag)
					 AddALLBmpMemoryInfo(szFileImage, rect, pdc);//20090723
			
				::SelectObject(dcMem, pOldBmp);				//20090723
				::DeleteObject(hBmp);
				::DeleteDC(dcMem);							//20090723
				
				delete []lpBitmap;    //20090216 test
 				
 				if(flag)
				//::Sleep(1);
//				TRACE(L"DrawImage_HDC DDDDDDDDDDD =  %d\n", offset);

				return;
				///////////////////////////////////////////////////////////////////////////

				//wprintf(szFileImage);
				//Dprintf( " All Draw \n");
				if(FindMemoryInfo(szFileImage, index))
				{
					if(index >= 0 && index < ALLBMPMEMORY_MAX)
					{
						BitBlt(pdc, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);
						
						file.Close();
						return;
					}
				}
			}

			file.Close();
			delete []lpBitmap;    //20090216 test
			
			IImage  *pImage   =   NULL; 
			
			//	CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
			
			//   Create   the   imaging   factory. 
			if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
			{ 
				//   Load   the   image   from   the   JPG   file. 
				if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
				{ 
					//   Draw   the   image. 
					pImage->Draw(pdc, rect, NULL); 
					pImage->Release(); 

					if(flag)
						AddALLBmpMemoryInfo(szFileImage, rect, pdc);
				
					::Sleep(1);
				} 
				else
				{
					Dprintf("DrawImage error\n");
				}
				
				pImgFactory-> Release(); 
			}

			return;
		}
		//创建HBITMAP句柄   

		/*
		重新根据rect 创建位图信息
		*/

		int w = (((lpBitmap->bmiHeader.biWidth*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;
		int h = lpBitmap->bmiHeader.biHeight;
		lpBitmap->bmiHeader.biHeight = rect.Height();
		lpBitmap->bmiHeader.biWidth = rect.Width();

		HDC dcMem = ::CreateCompatibleDC(pdc);   
		HBITMAP   hBmp = ::CreateDIBSection(dcMem, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
		if(lpBits == NULL)
		{
			::MessageBox(NULL,L"Draw BMP no memory!", L"warning", MB_OK);  
			//重启机器

			//return;
		}
	
		/*
		file.ReadHuge(lpBits, dwBitlen);  
		*/
		int off = (h-rect.top-rect.Height())*w + rect.left*(lpBitmap->bmiHeader.biBitCount/8);

		file.Seek(off, CFile::current);
		int size = (((rect.Width()*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;   //rect.Width()*3;
		int offline = w - size; 
		
		for(int i = 0; i < rect.Height(); i++)
		{
		   	file.Read(lpBits, size); 
			LPBYTE lpbyte = (LPBYTE)lpBits;
			lpbyte += size;
			lpBits = (LPVOID)lpbyte;
			file.Seek(offline, CFile::current);
		}
		file.Close();       
		
		//TRACE(L"file DC 3 %d\n", offset);

		HBITMAP pOldBmp= (HBITMAP)::SelectObject(dcMem, hBmp);   
	
		
    	//pdc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
		::BitBlt(pdc, newRect.left, newRect.top, newRect.Width(), newRect.Height(), dcMem, 0, 0, SRCCOPY);
	
		::SelectObject(dcMem, pOldBmp); 
	
		::DeleteObject(hBmp);
		::DeleteDC(dcMem);
	
		delete []lpBitmap;    //20090216 test

		::Sleep(1);
	}
	else
	{

		IImage   *pImage   =   NULL; 
		
		//	CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
		
		//   Create   the   imaging   factory. 
		if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
		{ 
			//   Load   the   image   from   the   JPG   file. 
			if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
			{ 
				//   Draw   the   image. 
				
				pImage->Draw(pdc, rect, NULL); 
				pImage->Release(); 
				::Sleep(1);
			} 
			else
			{
				Dprintf("DrawImage error\n");
			}
			
			pImgFactory-> Release(); 
		} 

	}

}

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg dialog
CMultimediaPhoneDlg::CMultimediaPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultimediaPhoneDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitImage();
	n_bklightcount = 0;
	m_bIsHungOn = FALSE;
	m_bNetOkStatus = FALSE;
	m_bNetType = 0;
	m_nTELRigster = TELRIGSTER_UN;
	m_bIsDial = FALSE;
	m_sMobileNumber = "";
	m_isBattery = FALSE;
	m_isHaveBattery = TRUE;
	m_bNoDc = true;

	m_nBackLightStatus = 7;
	m_nBackLightValue = 7;
	m_bSearchNetWin = false ;

	m_bIsCloseLCD = false;
	m_isDialTimeout = 0;
	
	m_phoneLine[0].pFSM = new Util::FSM;
	m_phoneLine[1].pFSM = new Util::FSM;
	m_phoneLine[2].pFSM = new Util::FSM;

	m_phoneLine[0].LineStatusTimer = 101 ;
	m_phoneLine[1].LineStatusTimer = 102 ;

	for (int i = 0 ; i < 2;i++)
	{
		m_phoneLine[i].bFirwall = false;
		m_phoneLine[i].bHasCallID = false ;
		m_phoneLine[i].RingFilename = "";
		m_phoneLine[i].RingCount = 0 ;
		m_phoneLine[i].iIgnoreRingCount = 1;
		m_phoneLine[i].bRingPlayed = false ;
		m_phoneLine[i].b3gReject = false;
		m_phoneLine[i].telnum = "" ;
		m_phoneLine[i].b3gFisrtRing = false;
		m_phoneLine[i].Second = 0 ;
	}

	m_phoneLine[0].HangoffTimer = 110;
	m_phoneLine[1].HangoffTimer = 111;
	
	m_nline = 0 ;

	m_bInsertSim = false;
}

CMultimediaPhoneDlg::~CMultimediaPhoneDlg()
{
	for (std::map<int, CDialog*>::iterator i = panels_.begin(); i != panels_.end(); ++i)
		delete i->second;

	for (int j = 0 ; j < 3;j++)
	{
		if (m_phoneLine[j].pFSM)
		{
			delete m_phoneLine[j].pFSM;
		}
	}

}

void CMultimediaPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultimediaPhoneDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultimediaPhoneDlg, CDialog)
	//{{AFX_MSG_MAP(CMultimediaPhoneDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SOFTKEY, OnMainSoftKey)
	ON_MESSAGE(WM_GEN_EVENT, OnEvent)
	ON_MESSAGE(WM_DEVICECHANGE, OnDeviceChange)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg message handlers
extern BOOL watchdog_test;
void CMultimediaPhoneDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch (w)
	{
		case 1:
			break;

		case 3:
			break;

		case 4://未接电话
			if (m_pMainDlg->m_nUnTel > 0 )
			{
				m_pMainDlg->StopVideo(FALSE, 0, 0);
				m_pContactInfoDlg->ShowUnconnectItems();
				m_pContactInfoDlg->ShowWindow(SW_SHOW);
				CWnd *p = m_pContactInfoDlg;
				AddIcon(Allicon[3],p,false);
				AddDesktopBtn();
			}
			break;

		case 5://短信小图标
			 m_pMainDlg->doReadSMS();
			 break;

		case 6://防火墙小图标
			//	m_pMainDlg->m_firewalDlg_->ShowWindow(SW_SHOW);
			if (m_pSettingDlg->m_pSetting->isFirewall())
			{
				m_pMainDlg->StopVideo(FALSE, 0, 0);
				m_pSettingDlg->IniCtrlData();
				m_pSettingDlg->OnClickMJPG(103,0);
				m_pSettingDlg->ShowWindow(SW_SHOW);
				CWnd *p = m_pSettingDlg;
				AddIcon(Allicon[9],p,false);
				AddDesktopBtn();
			}
			break;

		case 7://日程小图标	
			if (m_pMainDlg->m_bIsAlarm)
			{	
				m_pMainDlg->StopVideo(FALSE, 0, 0);
			//	m_pMainDlg->m_mainLunarderDlg_->SetIn();
				m_pMainDlg->ShowTodayAlarm();
			//	CWnd *p = m_AlarmShowDlg;
			//	AddIcon(Allicon[7],p,false);
			//	AddDesktopBtn();

			}
			break;

		case 8:
		//	OnButtonSetting();
			break;
		case 10:
			Desktop();
			break;

		case 15:
			if(m_phoneLine[m_nline].pFSM->getCurrentState() > p3gsHangOff ||
				m_phoneLine[2].pFSM->getCurrentState() > pstnsHangOff)
			{	
				if (!m_pTelephoneDlg->IsWindowVisible())
				{
					m_pTelephoneDlg->ShowWindow_(TRUE);
					m_MJPGList.SetUnitIsShow(15,false,true);
					CWnd *p = m_pTelephoneDlg;
					AddIcon(Allicon[0],p,false);
					AddDesktopBtn();
				}
			}
			break;

		default:
			break;
	}

	m_MJPGList.Invalidate();
	
}

void  CMultimediaPhoneDlg::SetAPN(int apn)
{	
	extern void SetProxyOption(TCHAR *pIP, BOOL isProXY);
	m_nAPN.apn_type = (int)apn;
	if(apn == 0)			//CMWAP
	{
		strcpy(m_nAPN.dialnumber, m_pSettingDlg->m_pSetting->gprsDialnumber1_.c_str());
		sprintf(m_nAPN.proxy, "%s:%d", m_pSettingDlg->m_pSetting->gprsProxyIp1_.c_str(), m_pSettingDlg->m_pSetting->gprsProxyPort1_);
		strcpy(m_nAPN.http, m_pSettingDlg->m_pSetting->gprsHttp1_.c_str());
	}
	else if(apn == 1)
	{
		strcpy(m_nAPN.dialnumber, m_pSettingDlg->m_pSetting->gprsDialnumber2_.c_str());
		if(m_pSettingDlg->m_pSetting->gprsProxyIp2_ == "")
			strcpy(m_nAPN.proxy, "");
		else
			sprintf(m_nAPN.proxy, "%s:%d", m_pSettingDlg->m_pSetting->gprsProxyIp2_.c_str(), m_pSettingDlg->m_pSetting->gprsProxyPort2_);
		strcpy(m_nAPN.http, m_pSettingDlg->m_pSetting->gprsHttp2_.c_str());
	}
	else if(apn == 2)		//CMUSER1
	{
		strcpy(m_nAPN.dialnumber, m_pSettingDlg->m_pSetting->gprsDialnumber3_.c_str());
		if(m_pSettingDlg->m_pSetting->gprsProxyIp3_ == "")
		{
			strcpy(m_nAPN.proxy, "");
			SetProxyOption(NULL, FALSE);
		}
		else
		{
			sprintf(m_nAPN.proxy, "%s:%d", m_pSettingDlg->m_pSetting->gprsProxyIp3_.c_str(), m_pSettingDlg->m_pSetting->gprsProxyPort3_);
			TCHAR temp[32] = {0};
			mbstowcs(temp, m_nAPN.proxy, strlen(m_nAPN.proxy));
			SetProxyOption(temp, TRUE);
		}
		strcpy(m_nAPN.http, m_pSettingDlg->m_pSetting->gprsHttp3_.c_str());
	}
	else if(apn == 3)		//CMUSER2
	{
		strcpy(m_nAPN.dialnumber, m_pSettingDlg->m_pSetting->gprsDialnumber4_.c_str());
		if(m_pSettingDlg->m_pSetting->gprsProxyIp4_ == "")
		{
			strcpy(m_nAPN.proxy, "");
			SetProxyOption(NULL, FALSE);
		}
		else
		{
			sprintf(m_nAPN.proxy, "%s:%d", m_pSettingDlg->m_pSetting->gprsProxyIp4_.c_str(), m_pSettingDlg->m_pSetting->gprsProxyPort4_);
			TCHAR temp[32] = {0};
			mbstowcs(temp, m_nAPN.proxy, strlen(m_nAPN.proxy));
			SetProxyOption(temp, TRUE);
		}
		strcpy(m_nAPN.http, m_pSettingDlg->m_pSetting->gprsHttp4_.c_str());
	}

}

BOOL CMultimediaPhoneDlg::IsConnectNet()
{
	BOOL flag = TRUE;
	if(m_nTELRigster < TELRIGSTER_DIALING)
	{
		m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle("需要先拨号连接，请确定!",30000, 2);
		m_pDeleteTipDlg->ShowWindow_(TRUE);
		flag = FALSE;
	}
	else if(m_nTELRigster == TELRIGSTER_DIALING)
	{
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle("正在拨号连接，请稍候...",2000);
		m_pDeleteTipDlg->ShowWindow_(TRUE);
		flag = FALSE;
	}
	return flag;
}

BOOL CMultimediaPhoneDlg::IsUnConnectNet()
{
	if(m_nTELRigster >= TELRIGSTER_DIALED)
	{
		m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle("需要退出拨号连接吗?请确定!",30000, 3);
		m_pDeleteTipDlg->ShowWindow_(TRUE);
	}
	return TRUE;
}

static BOOL isFirstReg = TRUE;
void TdDoWithProc(void *p)
{	
	BOOL isSleepStart = FALSE;
	isSleepStart = (BOOL)*(BOOL *)p;
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	CNetStatusDlg::ADSLInit();
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while(1)
	{
		::Sleep(100);
		//pMainDlg->m_bIsDial = TRUE;

		if(pMainDlg->m_nTELRigster == TELRIGSTER_UN)
		{	
			bool static bSerach = false;
			BOOL haveNet ;
			if (!bSerach)//只收网一次
			{	
				bSerach = true;
				haveNet = pMainDlg->doSerachrTDNet(isSleepStart);
			}

			if(!haveNet)
			{			
				g_IsSignal = FALSE;
				//pMainDlg->SetTimer(0x131, 60*1000, NULL);  //隔60秒搜一次网
			}
			else
			{
				pMainDlg->KillTimer(0x131);
				g_IsSignal = TRUE;
			}

		}
		else if(pMainDlg->m_nTELRigster == TELRIGSTER_TD && pMainDlg->m_bIsDial)
		{
			if(pMainDlg->m_isDialTimeout == 1)
			{
				Sleep(10*1000);  //第一次超时后休眠10秒钟继续拨号
			}
// 			for(int i=0; i<3; i++)
// 			{
// 				pMainDlg->m_pSMSWarp->SignalQuality_();
// 				Sleep(500);
// 			}
			
			char *cApnName = NULL;
			if((DIALTYPE)pMainDlg->m_nAPN.apn_type == CMUSER1)
			{
				cApnName = (char *)pMainDlg->m_pSettingDlg->m_pSetting->gprsType3_.c_str();
			}
			else if((DIALTYPE)pMainDlg->m_nAPN.apn_type == CMUSER2)
			{
				cApnName = (char *)pMainDlg->m_pSettingDlg->m_pSetting->gprsType4_.c_str();
			}

			if(pMainDlg->m_isDialTimeout < 3)
			{
				int static queryNoCount = 0;
				if(g_IsSignal)  //有信号
				{
					queryNoCount = 0;
					pMainDlg->SetAPN(pMainDlg->m_pSettingDlg->m_pSetting->MmsReciveType());
					if(CNetStatusDlg::ADSLDial(pMainDlg->m_nAPN.dialnumber, "", "", (CWnd *)pMainDlg, (DIALTYPE)pMainDlg->m_nAPN.apn_type,cApnName))
					{
						pMainDlg->m_MJPGList.SetUnitBitmap(14, ".\\adv\\mjpg\\k5\\common\\gprs状态1.bmp", "", FALSE);
						pMainDlg->m_MJPGList.Invalidate();
						pMainDlg->m_nTELRigster = TELRIGSTER_DIALING;
						pMainDlg->SetTimer(0x125, 1000, NULL);   //wzx20100318
					}
					else
					{
						Sleep(1000);
					}
				}
				else
				{
					queryNoCount++;
					pMainDlg->m_MJPGList.SetUnitBitmap(14, "", "", TRUE);
					Sleep(5000);	//没有信号则休眠5秒钟
					if(queryNoCount == 3)
					{
				//		pMainDlg->SearchNetInit();
						if(g_isSimEnable)
						{
							pMainDlg->m_nTELRigster = TELRIGSTER_UN;
						}
						else
						{
							pMainDlg->m_pWarningNoFlashDlg->SetTitle(L"模块错误或SIM卡不可用");
							pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
							pMainDlg->m_nTELRigster = TELRIGSTER_UN;
							g_IsSignal = FALSE;
							break;
						}
						queryNoCount = 0;
					}
					continue;
				}
			}
			else
			{
				pMainDlg->m_MJPGList.SetUnitBitmap(14, "", "", TRUE);
			}
		}
		
		if(pMainDlg->m_nTELRigster >= TELRIGSTER_TD && pMainDlg->m_binitOver)
		{
			pMainDlg->SelMMSDataBase();
			pMainDlg->doWithDownLoad();   //test
		}
	}
}


BOOL  watchdog_MFC = TRUE;
void WatchDogProc(void *)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
//	Util::ClientSocket *netSocket = NULL;

	while(1)
	{
#ifdef _DEBUG
//	    pMainDlg->SetWatchDog();   //设置watchdog
#else
		if(!watchdog_MFC)
		{
			extern VOID WriteLog(CHAR* str);
			char restartTimer[64];
			SYSTEMTIME tm;
			GetLocalTime(&tm);
			DWORD value = DMemprintf("");
			sprintf(restartTimer, "watchdog_MFC IS false:%04d-%02d-%02d %02d:%02d:%02d %x\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, value);
//			WriteLog(restartTimer);
		}

		if((watchdog_MFC && (!g_MemoryLeak)) || (g_bAdjustPanel) || pMainDlg->m_bIsCloseLCD)
		{
			  Dprintf("Set watchdog flag--------\r\n");
//			  pMainDlg->SetWatchDog();   //设置watchdog   移到main窗口
			  Sleep(5);
//			  pMainDlg->SetWatchDog();   //设置watchdog   移到main窗口
			  Sleep(5);
//			  pMainDlg->SetWatchDog();   //设置watchdog   移到main窗口
			  Sleep(5);
			  watchdog_MFC = FALSE;
		}
#endif
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		if(tm.wHour == 0 && tm.wMinute == 0 && tm.wSecond < 8)
		{
			pMainDlg->SetAlarmList();
			Sleep(8000);
		}
		else
			Sleep(5000);

		static int gCCCC = 0;
		extern void GAskBatteryStatus();
		if(gCCCC++%3 == 0)
			GAskBatteryStatus();
	}
}

static BOOL gIsToLight;

Util::ATCommandWarp* GetATCommandWarp()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	return main->m_pATCommandWarp1;
}

void GIsOpenMix(BOOL isOn) 
{
     CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	 extern VOID WriteMyLog_(char *ptr, int size);
	
	 return ;//add by qi 20100702

	 if (!(main->OpenAllPort()))
	 {
		return;
	 }
	
	 if(!isOn)
	 {
		waveInMessage(0, 0x10000001,0,0);//speeker
		char *sound = "ATN'MIX=0'\r";
		Dprintf("Wav_Speeker_false\r\n");
		WriteMyLog_("Wav_Speeker_false",strlen("Wav_Speeker_false"));			

		//main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	 }
	 else//
	 {
		 if (!main->phone_->Hand())
		 {	 
			 WriteMyLog_("Wav_Speeker",strlen("Wav_Speeker"));			
			 Dprintf("Wav_Speeker\r\n");
			 waveInMessage(0, 0x10000001,0,0);//speeker
			 char *sound = "ATN'MIX=0'\r";//线路搭上去Speeker
		//	 main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
		 }
		 else 
		 {	
			 WriteMyLog_("Wav_LineOut",strlen("Wav_LineOut"));			
			 Dprintf("Wav_LineOut\r\n");
			 waveInMessage(0, 0x10000002,0,0);//lineout
			 char *sound = "ATN'MIX=1'\r";//线路搭上去LineOut
		//	 main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));

		 }
		
	//	 waveInMessage(0, 0x10000002,0,0);//lineout
	//	 char *sound = "ATN'MIX=1'\r";//混音
	//	 main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));

	 }
	 ::Sleep(10);

}

void GMute(BOOL isOn) 
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	//add by qi 20100831


	if (!(main->m_bATComm) || !(main->phone_->m_bOpen1Port))
	{
		return ;
	}	
	
	if(isOn)//手柄
	{	
		if ( !(main->phone_->Free() && !main->phone_->Hand()) && 
			!main->m_pMainDlg->m_mainMp3Dlg_->playerDlg_->player_->isPlaying_ &&
			!main->m_pMainDlg->m_mainVideoDlg_->playerDlg_->player_->isPlaying_&&
			!main->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic && !main->m_pSMSListDlg->m_pMmsReadDlg->m_isReadMid)
		{
			char *sound = "ATN'MUT=1'\r";//Speeker静音
			Dprintf("speeker off ATN'MUT=1\r\n");
			main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));	
		}

	}
	else
	{
		char *sound = "ATN'MUT=0'\r";//Speeker不静音
		Dprintf("\r\nspeeker open ATN'MUT=0 \r\n");
		main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	}

}

void GMute_() 
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			
	if (!(main->m_bATComm) || !(main->phone_->m_bOpen1Port))
	{
		return ;
	}	
		
	char *sound = "ATN'MUT=0'\r";//Speeker不静音
	main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));		
}

void PlayDialToneProc(void *p)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->phone_->StopRing(true,false);

	// add by qi 20100416
	//DWORD oldVoulme;
	//waveOutGetVolume(NULL,&oldVoulme);
	UINT16 volume[] = {0xFF00, 0xdd00, 0xbb00, 0xaa00, 0x8800, 0x6600, 0x4400,
		0x2200, 0x1600, 0x1000};
	int nVoulme = main->m_pSettingDlg->m_pSetting->phoneCallRingVolume_ - 1 ;
	DWORD newVoulme = (volume[nVoulme]|(volume[nVoulme]<<16));
	waveOutSetVolume(NULL,newVoulme);
	//add by qi over

	char *ptr = (char *)p;
	int offtime = 50;
	if(strlen(ptr) > 1)
		offtime = 30;
	WIN_Audio_close();
	while(*ptr != '\0')
	{		
 		if ( !main->phone_->Hand())
 		{
 			extern void GMute(BOOL isOn);
			GMute(FALSE);
 		}

		// add by qi 20100607
		//电话是挂断状态就没拨号音
		if (main->m_phoneLine[0].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHangOff &&
			main->m_phoneLine[1].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsHangOff )
		{
			return ;
		}
		//add by qi

		char filename[32];
		char c = *ptr;
		if(c == '*')
			c = 'A';
		sprintf(filename, "/hive/res/%c.wav", c);
		ptr++;
		CString s = filename;
		Set_WIN_Params(NULL, 44*1024, 16, 1);
		FILE *fl = fopen(filename, "r+b");
		if(fl)
		{
			char *temp = new char[200*1024];
			int len = fread(temp, sizeof(char), 200*1024, fl);
			WIN_Play_Samples(temp, len/8);
			//WIN_Play_Samples(temp, len/7);
			Dprintf(" play = %c\r\n",c);
			delete []temp;
		}
		Sleep(offtime);
		WIN_Audio_close();
		fclose(fl);
	}
	
	//add by qi 20100416,把声音设置回去
	//waveOutSetVolume(NULL,oldVoulme);
	
}

char gDiatone[64];
void GPlayDialTone(char *dialtone) 
{	
	extern BOOL IsTestCTA();
	if( IsTestCTA())
		return;

	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nVolume = main->m_pSettingDlg->m_pSetting->smsRingVolume_;
//	UINT16 volume[] = {0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000}; //(5-nVolume)*0xFFFF/5;
	UINT16 volume[] = {0xFF00, 0xdd00, 0xbb00, 0xaa00, 0x8800, 0x6600, 0x4400,
	0x2200, 0x1600, 0x1000};
	main->phone_->g_RingSound = (volume[nVolume]|(volume[nVolume]<<16));
	if(strstr(dialtone, "dialing") > 0)
	{
		main->phone_->StartRing(L"/hive/res/dial.wav", 0xFF);
	}
	else if(strstr(dialtone, "hangup") > 0)
	{
		main->phone_->StartRing(L"");
		GIsOpenMix(0);
	
	}
	else if(strlen(dialtone) > 0)
	{
		DWORD d;
		strcpy(gDiatone, dialtone);
	//	WIN_Audio_close();
		HANDLE pThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)PlayDialToneProc, (void *)gDiatone, 0, &d );
	}
// 	else
// 	{
// 		char filename[64] = {0};
// 		sprintf(filename, "/hive/res/%s.wav", dialtone);
// 		CString s = filename;
// 		main->phone_->StartRing((LPTSTR)(LPCTSTR)filename, 1);
// 	}

}
void GNotifyDial(BOOL isDial)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!(main->OpenAllPort()))
	{
		return;
	}
	
	if(!isDial)
	{
		char *sound = "ATN'TLK=0'\r";
		Dprintf("ATN'TLK = 0\r\n");
		main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	}
	else
	{
		char *sound = "ATN'TLK=1'\r";//我正在使用软件拨号
		Dprintf("ATN'TLK = 1\r\n");
		main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	}
	 ::Sleep(10);
}

//lxz sleep
void SetSleepFlag(BOOL isSleep)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	char sound[32];
	sprintf(sound, "ATN'SLP=%d'\r", isSleep);
	main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
}

//lxz sleep
void SetMPUOff()
{	
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	//切换到PSTN
	main->phone_->SWitchPhone3G(FALSE);
	
	//
	char DUSBDEG1[] = "AT^DUSBDEG=1,0\r";
	main->m_pATCommandWarp1->Transaction_(DUSBDEG1,strlen(DUSBDEG1),"");

	//USB去电
	char ATUSBPOWER[] = "AT^DUSBPOWER=0\r";
	main->m_pATCommandWarp1->Transaction_(ATUSBPOWER,strlen(ATUSBPOWER),"");
	
	//
	char CGACT[] = "AT+CGACT=0,1\r";
	main->m_pATCommandWarp1->Transaction_(CGACT,strlen(CGACT),"");

	//模块睡眠
	char Dslp[] = "AT^DSLP=0,1\r";
	main->m_pATCommandWarp1->Transaction_(Dslp,strlen(Dslp),"");
	
	//设置睡眠标志
	SetSleepFlag(1);
	
	WriteMyLog_("SetMPUOff",strlen("SetMPUOff"));

	char sound[32];
	sprintf(sound, "ATN'MPU=0'\r");
	main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
}

//lxz sleep
BOOL isSleepStart()
{
	//lxz 20100804
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	char flag = 0 ;
	if(main->GetSleepFlag(flag))
	{
		return (flag == 1);
	}
}

void GAskBatteryStatus()
{	
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
// 	if (!(main->OpenAllPort()))
// 	{
// 		return;
// 	}

	if (!(main->m_bATComm) || !(main->phone_->m_bOpen1Port))
	{
		return ;
	}
	
	char *status = "ATN'STA=?'\r";
	main->phone_->m_pRS232->WritePort((unsigned char *)status, strlen(status));
	::Sleep(10);
}

void GNotifyReady()
{	

	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!(main->OpenAllPort()))
	{
		return;
	}
	
	char *sound = "ATN'RDY=1'\r";
	main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	::Sleep(10);
	char *status = "ATN'STA=?'\r";
	main->phone_->m_pRS232->WritePort((unsigned char *)status, strlen(status));
	::Sleep(10);
}

BOOL g_isOFFing = FALSE;
void PowerOffProc(void)
{
	g_isOFFing = TRUE;
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->playervideo_->InitPlayer();
	main->playervideo_->SetOwner(NULL);
	main->playervideo_->PlayerFile("\\flashdrv\\关机.mp4");
	
	Sleep(4000);
	
	main->playervideo_->PausePlayer(true);
	extern Util::ATCommandWarp* GetATCommandWarp();
	Util::ATCommandWarp *pATCommanWarp = GetATCommandWarp();
	pATCommanWarp->Off();
	
	Sleep(50);
	char *txt = "ATN'RDY=2'\r";
	main->phone_->m_pRS232->WritePort((unsigned char *)txt, strlen(txt));
	
	while(1)
	{
		Sleep(100);
	}
}

void UiProc(void *param)
{	
	BOOL *flag = (BOOL *)param;

	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->playervideo_->InitPlayer();

	//add by qi 0630
	main->playervideo_->SetVolume(10);

	main->playervideo_->PlayerFile("\\flashdrv\\开机.mp4");

	
	//change by qi 20100608
// 	bool bpause = false;
// 	MEDIA_INFO info;
// 	while(*flag)
// 	{	
// 		Sleep(50);
// 		if (!bpause)
// 		{	
// 			main->playervideo_->GetVideoFileInfo(&info);
// 			Dprintf("nTick:%d\r\n",info.nTick);
// 			if (info.nTick >= 61000)
// 			{	
// 				bpause = true;
// 				main->playervideo_->PausePlayer(true);
// 			}
// 		}
// 	}
	//add over

//old
while(*flag)
	{	
		static bool isFirst = true;
		if(isFirst)
		{
			isFirst = false;
			Sleep(4500);
			main->playervideo_->PausePlayer(true);
		}
		Sleep(50);
	}
	
	main->playervideo_->ExitPlayer();
	
	//add by qi 0706
	main->playervideo_->SetVolume(2);

	main->m_binitOver = true;

}

BOOL isFirstGif;
BOOL CMultimediaPhoneDlg::OnInitDialog()
{
	//lxz test jpg
	CDialog::OnInitDialog();
	
	m_binitOver = false ;

	SetWindowText(L"Edo_Phone_Dlg");

	CImgCache::InitCache(m_hWnd);

	InitALLBmpMemoryInfo();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here
	//step1: global variable init

	//step2: global workthread init

	//step3: config data read and fill

	//step4: device and environment check and init

	//step5: init UI
	//检测SD ,USB
	InitSCLPort();
	m_bIsHaveUSB = DetectDIR(L"UsbDisk");
	m_bIsSD = DetectDIR(L"StorageCard");
//	SetTimer(IDT_DETECTUSB, 500, NULL);
	
	char buffer[1024];
	GetManuInfo(buffer, 1024);
	memcpy(&m_manuinfo, buffer, sizeof(MANUINFO));
	//add by qi 20100630
	memcpy(m_manuinfo.hardVer,"DT_MMP_MAIN11_TEL02",strlen("DT_MMP_MAIN11_TEL02"));
	//init player

	//change by qi 20100907
	//获得深度睡眠的标准
	bool bsleepFlag = false;
	if( 0 == VERSION_TYPE)
	{
		bsleepFlag = isSleepStart();
	}

	char log[30] = {0};
	sprintf(log,"bsleepFlag:%d",(int)bsleepFlag);
	WriteMyLog_(log,strlen(log));
	
	FILE *file = NULL;
	file = fopen("\\flashdrv\\res_dat\\config.txt", "r+b");
	if(file != NULL)
	{
		char config[32] = {0};
		std::vector<string> sv;
		for(int i=0; i<5; i++)
		{
			memset(config, 0, 32);
			if(fgets(config, 32, file) != NULL)
			{
				sv.push_back(config);
			}
		}
		for(i=0; i<5; i++)
		{
			if(sv.size() > 0)
			{
				std::string str = sv[i];
				int pos = str.find_first_of('=', 0);
				if(pos != std::string::npos)
				{
					str = str.substr(pos+2, str.size());
					CString c = Util::StringOp::ToCString(str);
					gBatteryLevel[i] = atoi(c);
				}
			}
		}
		fclose(file);
	}

	m_pMessageSet = Data::MessageSet::GetCurrentConfig();
	m_pTempMsgSet = m_pMessageSet;

	g_bSendReport = m_pMessageSet->MmsSendReport();
	g_bReadReport = m_pMessageSet->MmsReadReport();

	playervideo_ = new Player(NULL, mtVideo);
	playervideo_->SetPlayerOwner(this);

	playeraudio_ = new Player(NULL, mtAudio);
	playeraudio_->SetPlayerOwner(this);

	playerimage_ = new Player(NULL, mtImage);
	playerimage_->SetPlayerOwner(this);

	CDialog* panel; 
	
	//  change by qi 2009_09_15
	panel = new CInformationDlg();
	panel->Create(CInformationDlg::IDD);
	panels_[IDC_BUTTON_INFORMATION] = panel;

	m_vAllCwnd.push_back(panel);
	
	m_pSettingDlg = new CSettingDlg();
	panel = m_pSettingDlg;
	panel->Create(CSettingDlg::IDD);
 	panels_[IDC_BUTTON_SETTING] = panel;

	m_vAllCwnd.push_back(panel);

	m_pTipDlg = new CTipDlg();//提示框
	m_pTipDlg->Create(CTipDlg::IDD);
	m_vAllCwnd.push_back(m_pTipDlg);

	m_pContactDlg = new CContactDlg();
	m_pContactDlg->Create(CContactDlg::IDD);
	panel = m_pContactDlg;
	panels_[IDC_BUTTON_CONTACT] = panel;
	
	m_vAllCwnd.push_back(m_pContactDlg);

	m_pContactNewDlg = new CContactNewDlg(panels_[IDC_BUTTON_CONTACT]);
	panel = m_pContactNewDlg;
	panel->Create(CContactNewDlg::IDD);
	panels_[IDC_BUTTON_CONTACTNEW] = panel;
	
	m_vAllCwnd.push_back(m_pContactNewDlg);

#if(G3_MODULOR == 1)
 
	m_pComWarp1 = new Util::ComWarp();
	if (m_pComWarp1)
	{	
		//change by qi 20100907
		WCHAR COM[10] = {0};
		if ( 1 == VERSION_TYPE)//usb
		{
			wcscpy(COM,L"COM8:");
		}
		else
		{
			wcscpy(COM,L"COM2:");//uart	
		}

		//if ((m_bATComm = m_pComWarp1->OpenComm(L"COM8:")))
		if ((m_bATComm = m_pComWarp1->OpenComm(COM)))//57600
		{	
			sprintf(log,"OpenCom:%d",(int)m_bATComm);
			WriteMyLog_(log,strlen(log));

			m_pATCommandWarp1 = new Util::ATCommandWarp();
			if (m_pATCommandWarp1)
			{
				m_pATCommandWarp1->Bind(m_pComWarp1);
				m_pSMSWarp = SMS::SMSWarp::GetSMSWarp();
				m_pVideoComWarp = new Util::ComWarp();
				phone_ = Telephone::TelephoneWarp::GetTelephoneWarp();//new Telephone::TelephoneWarp();
				
				extern void SetSleepFlag(BOOL isSleep);
				SetSleepFlag(0);

				extern void GNotifyReady();
				GNotifyReady();

			}
		}

	}

#else
	m_pComWarp1 = new Util::ComWarp();
	m_pComWarp1->OpenComm(L"COM4:");
	m_pATCommandWarp1 = new Util::ATCommandWarp();
	m_pATCommandWarp1->Bind(m_pComWarp1);
	
	phone_ = Telephone::TelephoneWarp::GetTelephoneWarp();//new Telephone::TelephoneWarp();
	m_pSMSWarp = SMS::SMSWarp::GetSMSWarp();
#endif
	
	m_pComWarp7 = NULL;

	m_pTelephoneDlg = new CTelephoneDlg();
	m_pTelephoneDlg->Create(CTelephoneDlg::IDD);
	m_pTelephoneDlg->ShowWindow(SW_HIDE);
	
	m_vAllCwnd.push_back(m_pTelephoneDlg);

	InitializeCriticalSection(&csCom);
	DWORD watchdogThreadID = 0;
	HANDLE m_pThread1 = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)WatchDogProc, 0, 0, &watchdogThreadID );
	DWORD tdThreadID = 0;
	HANDLE m_pThread2;
	if(m_bATComm)
	{	
		//lxz sleep
		static BOOL isFLAG;
		isFLAG = bsleepFlag;//isSleepStart();
		m_pThread2 = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)TdDoWithProc, &isFLAG, 0, &tdThreadID );
	}
	else
	{
		SetTimer(0x126, 0, 0);
	}
	

	 DWORD uiThreadID = 0;
	 isFirstGif = TRUE;
	 HANDLE m_uiThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)UiProc, &isFirstGif, 0, &uiThreadID );
		
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 57), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\3g_telstatus.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 57));
	m_MJPGList.SetUnitBitmap(0,".\\adv\\mjpg\\k5\\common\\桌面.bmp","",FALSE);
	m_MJPGList.SetUnitIsShow(10,false,false);
	m_MJPGList.SetUnitIsShow(15,false,false);

	/*
	设置 三个 界面
	*/
	
	DWORD D = GetTickCount();

	m_pMainDlg = new CMainDlg();
	panel = m_pMainDlg;
	panel->Create(CMainDlg::IDD);
	panels_[IDC_BUTTON_MAIN] = panel;
	currentPanel_ = panel;
	
//	m_vAllCwnd.push_back(m_pMainDlg);

 	m_pContactInfoDlg = new CContactInfoDlg();
 	panel = m_pContactInfoDlg;
 	panel->Create(CContactInfoDlg::IDD);
 	panels_[IDC_BUTTON_CONTACTINFO] = panel;
	
	m_vAllCwnd.push_back(m_pContactInfoDlg);

 	m_pSoundDlg = new CSoundDlg();
 	panel = m_pSoundDlg;
 	panel->Create(CSoundDlg::IDD);
 	panels_[IDC_BUTTON_SOUND] = panel;
	
	m_vAllCwnd.push_back(m_pSoundDlg);


	m_pContactGroupDlg = new CContactGroupDlg(panels_[IDC_BUTTON_CONTACT]);
	panel = m_pContactGroupDlg;
	panel->Create(CContactGroupDlg::IDD);
	panels_[IDC_BUTTON_CONTACTGROUP] = panel;
	
	m_vAllCwnd.push_back(m_pContactGroupDlg);

	m_pDeleteTipDlg = new CDeleteTipDlg(this);
	m_pDeleteTipDlg->Create(CDeleteTipDlg::IDD);
	
	m_vAllCwnd.push_back(m_pDeleteTipDlg);

	m_pNetStatusDlg = new CNetStatusDlg(this);
	m_pNetStatusDlg->Create(CNetStatusDlg::IDD);

	m_pNotebookDlg = new CNotebookDlg(this);
	m_pNotebookDlg->Create(CNotebookDlg::IDD);
	
	m_vAllCwnd.push_back(m_pNotebookDlg);

	m_AlarmShowDlg = new CAlarmShowDlg(this);
	m_AlarmShowDlg->Create(CAlarmShowDlg::IDD);

	m_vAllCwnd.push_back(m_AlarmShowDlg);

	m_pWarningNoFlashDlg = new CWarningNoFlashDlg(this);
	m_pWarningNoFlashDlg->Create(CWarningNoFlashDlg::IDD);

	m_vAllCwnd.push_back(m_pWarningNoFlashDlg);

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);
	
	m_vAllCwnd.push_back(m_pPasswordDlg);


	m_pMMSDlg = new CMMSDlg();//写短信界面
	m_pMMSDlg->Create(CMMSDlg::IDD);

	m_vAllCwnd.push_back(m_pMMSDlg);

	m_pSMSListDlg = new C3GSMSListDlg();//收件箱
	m_pSMSListDlg->Create(C3GSMSListDlg::IDD);

	m_vAllCwnd.push_back(m_pSMSListDlg);
	m_vAllCwnd.push_back(m_pSMSListDlg->m_pSmsReadDlg);
	m_vAllCwnd.push_back(m_pSMSListDlg->m_pMmsReadDlg);
	m_vAllCwnd.push_back(m_pSMSListDlg->m_pNumberExtractDlg);
	m_vAllCwnd.push_back(m_pSMSListDlg->m_pSmsDetailDlg);
	m_vAllCwnd.push_back(m_pSMSListDlg->m_pStorageStatusDlg);

	//add by qi 2009_10_19
	m_pShiftFileDlg = new CShiftFile();
	m_pShiftFileDlg->Create(CShiftFile::IDD);
	m_pShiftFileDlg->ShowWindow(SW_HIDE);

	m_vAllCwnd.push_back(m_pShiftFileDlg);

	m_pDraftDlg	= new CDraftDlg();
	m_pDraftDlg->Create(CDraftDlg::IDD);
	m_pDraftDlg->ShowWindow(SW_HIDE);

	m_vAllCwnd.push_back(m_pDraftDlg);

	m_pSmsSettingDlg = new CSMSSettingDlg();
	m_pSmsSettingDlg->Create(CSMSSettingDlg::IDD);
	m_pSmsSettingDlg->ShowWindow(SW_HIDE);

	m_vAllCwnd.push_back(m_pSmsSettingDlg);

	m_pMmsSettingDlg = new CMMSSettingDlg();
	m_pMmsSettingDlg->Create(CMMSSettingDlg::IDD);
	m_pMmsSettingDlg->ShowWindow(SW_HIDE);

	m_vAllCwnd.push_back(m_pMmsSettingDlg);

	m_pTelphoneDialDlg = new CTelephoneDialDlg();
	m_pTelphoneDialDlg->Create(CTelephoneDialDlg::IDD);
	m_pTelphoneDialDlg->ShowWindow(SW_HIDE);
	
	m_vAllCwnd.push_back(m_pTelphoneDialDlg);

	m_pTelphoneRingDlg = new CTelephoneRingDlg();
	m_pTelphoneRingDlg->Create(CTelephoneRingDlg::IDD);
	m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);

//	m_vAllCwnd.push_back(m_pTelphoneRingDlg);

	m_pCstkDlg = new CSTKDlg();
	m_pCstkDlg->Create(CSTKDlg::IDD);
	m_pCstkDlg->ShowWindow_(SW_HIDE);

	m_vAllCwnd.push_back(m_pCstkDlg);

	
	DWORD dend = GetTickCount()- D;
	
	TRACE(L"DDDDDDDDDDDD = %d",dend);

	currentPanel_->ShowWindow(SW_SHOW);
	currentPanel_->MoveWindow(0,57,800,423);
		
	m_pFSM = new Util::FSM;
	AddFSMRules();
	m_uiKeyTimer = 1;
	m_uiRingTimer = 2;
	m_uiPSTNRingTimer = 3;
	
	//设置进入屏保
	SetScreenSaveTimer();
	SetTimer(IDT_CONTROLBACKLIGHT_TIME+1, 30*1000, NULL);
	
	CString icon = L".\\adv\\mjpg\\k5\\common\\桌面图标.bmp";
	AddIcon(icon,m_pMainDlg,true);
	m_MJPGList.SetUnitIsShow(10,false,true);

	//设置闹铃
	SetAlarmList();

	GMute(true);

	isFirstGif = FALSE;
	
	//插入数据
	//InsertTestData2DB();

//	m_binitOver = true ;

	std::vector<boost::shared_ptr<Data::MmsMessage> > result = Data::MmsMessage::GetFromDatabase("");
	for(int i = 0; i < result.size(); i++)
	{
		if(result[i]->state == Data::MmsMessage::stRepeatDownload)
		{
			if(result[i]->repeatCount < 2)
			{
				result[i]->state = Data::MmsMessage::stUnDownload;
				result[i]->Update();
			}
			else
			{
				result[i]->Remove();
			}
		}
	}

	std::string fileter = "type = ";
	fileter += Util::StringOp::FromInt(Data::MMSData::tpRepeatSend);
	std::vector<boost::shared_ptr<Data::MMSData> > mmsSendResult = Data::MMSData::GetFromDatabase(fileter);
	for(i = 0; i < mmsSendResult.size(); i++)
	{
		if(mmsSendResult[i]->type == Data::MMSData::tpRepeatSend)
		{
			if(mmsSendResult[i]->repeatCount < 2)
			{
				mmsSendResult[i]->type = Data::MMSData::tpReadySend;
			}
			else
			{
				mmsSendResult[i]->type = Data::MMSData::tpUnSend;
			}
			mmsSendResult[i]->Update();
		}
	}

//	SetTimer(1002,30*1000,NULL);//记录当前的时间

	return TRUE;  // return TRUE  unless you set the focus to a control

}

#define   NDISUIO_DEVICE_NAME                     TEXT( "UIO1: ") 
#define   FILE_ANY_ACCESS                             0 
#define   METHOD_BUFFERED                             0 
#define   FILE_DEVICE_NETWORK                     0x00000012 

#define   FSCTL_NDISUIO_BASE             FILE_DEVICE_NETWORK 

typedef   struct   _NDISUIO_QUERY_BINDING{ 
	ULONG       BindingIndex;                   //   0-based   binding   number 
	ULONG       DeviceNameOffset;   //   from   start   of   this   struct 
	ULONG       DeviceNameLength;   //   in   bytes 
	ULONG       DeviceDescrOffset;   //   from   start   of   this   struct 
	ULONG       DeviceDescrLength;   //   in   bytes 
	
}   NDISUIO_QUERY_BINDING,   *PNDISUIO_QUERY_BINDING; 

typedef   ULONG   NDIS_OID,   *PNDIS_OID; 

typedef   struct   _NDISUIO_QUERY_OID{ 
	NDIS_OID                 Oid; 
	PTCHAR       ptcDeviceName;   
	UCHAR                       Data[sizeof(ULONG)]; 
}   NDISUIO_QUERY_OID,   *PNDISUIO_QUERY_OID; 

//#define   CTL_CODE(DeviceType,Function,Method,Access)   (((DeviceType)<<16)|((Access)<<14)|((Function)<<2)|(Method)) 
#define   _NDISUIO_CTL_CODE(_Function,_Method,_Access)   CTL_CODE(FSCTL_NDISUIO_BASE,_Function, _Method,_Access) 

#define   IOCTL_NDISUIO_QUERY_BINDING    _NDISUIO_CTL_CODE(0x203,   METHOD_BUFFERED,   FILE_ANY_ACCESS) 

#define   IOCTL_NDISUIO_OPEN_DEVICE    _NDISUIO_CTL_CODE(0x200,   METHOD_BUFFERED,   FILE_ANY_ACCESS) 

#define   IOCTL_NDISUIO_QUERY_OID_VALUE      _NDISUIO_CTL_CODE(0x201,   METHOD_BUFFERED,   FILE_ANY_ACCESS) 

int CMultimediaPhoneDlg::CheckInternetStatus()
{
	if(m_pSettingDlg->m_pSetting->linkMode() == Data::lmDirect)   //网络直连
	{
		m_bNetType = 0;
		int ret = 0;
		HANDLE hDevice = INVALID_HANDLE_VALUE;
		
		hDevice =CreateFile(TEXT("UIO1:"), GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_ALWAYS, 0, NULL);
		
		
		if (hDevice == INVALID_HANDLE_VALUE)
		{
			Dprintf("Open Error!\r\n");
			return 0;
		}
		
		//--------------------------------------------------------------------- 
		//   retrieve   the   name   and   description   of   the   adapter. 
		//--------------------------------------------------------------------- 
		CHAR   chData[1024];   
		PNDISUIO_QUERY_BINDING   pBindInfo   =   (PNDISUIO_QUERY_BINDING)   chData;   
		pBindInfo-> BindingIndex   =   0;   
		int   length   =   sizeof   (NDISUIO_QUERY_BINDING);   
		DWORD   bytesconsumed   =0; 
		BOOL   bValue   =   DeviceIoControl   (hDevice,   
			IOCTL_NDISUIO_QUERY_BINDING,   
			pBindInfo,   
			length,   
			NULL,   
			1024,   
			&bytesconsumed,   
			NULL);   
		if   (!bValue)   
		{   
			CloseHandle(hDevice);
			return 0;   
		}   
		
		DWORD   dwReturnedBytes;   
		PTCHAR   ptcDeviceName   =  (PTCHAR)((PUCHAR)pBindInfo   +   pBindInfo-> DeviceNameOffset);   
		
		//--------------------------------------------------------------------- 
		//   Open   Device 
		//---------------------------------------------------------------------   
		DWORD   deErrCode   =   0;   
		bValue   =   DeviceIoControl(   hDevice,   
			IOCTL_NDISUIO_OPEN_DEVICE,   
			ptcDeviceName,   
			wcslen(ptcDeviceName)   *   sizeof(TCHAR),   
			NULL,   
			0,   
			&dwReturnedBytes,   
			NULL);   
		
		if   (0   ==   bValue)   
		{   
			deErrCode   =   GetLastError(); 
			CloseHandle(hDevice);
			return 0;   
		}   
		
		PNDISUIO_QUERY_OID   queryOid;   	
		PUCHAR   QueryBuffer[1024];   
		
		queryOid   =      (PNDISUIO_QUERY_OID)   QueryBuffer;   
		
		queryOid-> Oid   =   OID_GEN_MEDIA_CONNECT_STATUS;          //   Custom   OID;   
		queryOid-> ptcDeviceName   =   NULL;						//   You   dont   need   this   once   you   have   done   a   OPEN   Device.   
		
		
		bValue   =   DeviceIoControl   (hDevice,   
			IOCTL_NDISUIO_QUERY_OID_VALUE,   
			(LPVOID)   queryOid,   
			sizeof(NDISUIO_QUERY_OID)   +   sizeof   (DWORD),   
			(LPVOID)   queryOid,   
			sizeof(NDISUIO_QUERY_OID)   +   sizeof   (DWORD),   
			&bytesconsumed,   
			NULL);   
		
		if ( *(ULONG*)(queryOid->Data) == NdisMediaStateConnected)// Connected! 
		{
			//	Dprintf("Connected!\r\n");
			ret = 1;
		}
		else if ( *(ULONG*)(queryOid->Data) ==	NdisMediaStateDisconnected)
		{
			//	Dprintf("Disconnected!\r\n");
			ret = 0;
		}
		
		CloseHandle(hDevice); // What was the result?
		return ret;
	}
	else if(m_pSettingDlg->m_pSetting->linkMode() == Data::lmDial)  //网络拨号
	{
		m_bNetType = 1;
		int Statuscode;
		if(CNetStatusDlg::CheckADSLStatus(Statuscode))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	return 0 ;
}

void CMultimediaPhoneDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(154,212,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(154,212,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(154,212,255));
}
void CMultimediaPhoneDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(235,252,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(235,252,255));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(225,252,255));
}
void CMultimediaPhoneDlg::SwitchPanel_(int panelId)
{
	//lxz 20071218 stocks
	//if(panelId == IDC_BUTTON_CONTACT || panelId == IDC_BUTTON_CONTACT || panelId == IDC_BUTTON_CONTACTINFO || panelId == IDC_BUTTON_SOUND)
	//	return;

	//停止试听音乐
	m_pSettingDlg->StopTryRing();
	m_pContactGroupDlg->StopTryRing();
	m_pMainDlg->m_mainLunarderDlg_->StopTryRing();
	
	//关闭输入法
	SipShowIM(SIPF_OFF);
	if(m_pMainDlg->m_pWebDialog->IsWindowVisible())
	{
		m_pMainDlg->m_pWebDialog->ShowWindow_(SW_HIDE);
	}
	
	if(panelId == 0)
	{
		m_pMainDlg->m_mainVideoDlg_->OnExit_();
		if(m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible())
			m_pMainDlg->m_mainLunarderDlg_->OnExit_();
		SwitchPanel_(IDC_BUTTON_MAIN);
		return;
	}

	if(currentPanel_ == panels_[IDC_BUTTON_MAIN])//恢复播放消息
	{
		IsSendMessage(FALSE);
	}

	m_MJPGList.Invalidate();

}

void CMultimediaPhoneDlg::OnButtonSound() 
{
	SwitchPanel_(IDC_BUTTON_SOUND);	
}

void CMultimediaPhoneDlg::OnButtonInformation() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_INFORMATION);
	//test CEDialog
//	m_pTestCEDlg->ShowWindow(TRUE);
}

void CMultimediaPhoneDlg::OnButtonSetting() 
{
	// TODO: Add your control notification handler code here
	m_pSettingDlg->SetDiskInFo();
	SwitchPanel_(IDC_BUTTON_SETTING);	
}


void CMultimediaPhoneDlg::OnButtonIME() 
{
	// TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonLine() 
{
	// TODO: Add your control notification handler code here
//	if (m_pFSM->getCurrentState() != tsHangOff )
	if (m_phoneLine[m_nline].pFSM->getCurrentState() != p3gsHangOff )
	{
		m_pTelephoneDlg->ShowWindow_(TRUE);	
	}
}

BOOL CMultimediaPhoneDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CMultimediaPhoneDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	extern BOOL watchdog_MFC;
	watchdog_MFC = TRUE;   //设置watchdog生效
	static BOOL isBatteryTimer = FALSE;

	if ((message == WM_TEL_RING)
		|| (message == WM_TEL_HUNGON))
	{
		m_pContactInfoDlg->SendMessage(WM_TEL_INUSE, 0, 0);
		m_pSoundDlg->SendMessage(WM_TEL_INUSE, 0, 0);
		m_pSettingDlg->SendMessage(WM_TEL_INUSE, 0, 0);
	}

	switch (message) 
	{
	case WM_LBUTTONDOWN:
		if(!ReStoreBackLight())
			panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 3); //退出全屏
		break;

	case WM_APP + 88: //WM_PlayEnded:
		{
			static int gCount = 0;
			Dprintf("player %d file start!\n", gCount++);
		
		}
		break;

	case WM_APP+60:
		Dprintf("0:");
		break;

	case WM_TEL_RING:
		if(m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
		{
			SendOutEvnet(8025, 0);
		}
		HandleRing(wParam,lParam);
		break;

	case WM_TEL_HUNGOFF:
		HandleHungOff(wParam,lParam);
		break;

	case WM_TEL_HUNGON:
		if(m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
		{
			SendOutEvnet(8025, 0);
		}
		HandleHungOn(wParam,lParam);
		break;

	case WM_TEL_CALLIDEND:
		HandleCallIDEnd(wParam,lParam);
		break;

	case WM_TEL_KEYCODE:
		{	
			//add by qi 0716
			if (!OpenAllPort())
			{
				break;
			}
			
			//摘手柄按键的输出增益声音减小
			if (phone_->Hand() && m_phoneLine[m_nline].pFSM->getCurrentState() < p3gsConnected)
			{
				if (!m_pTelphoneDialDlg->m_bSetGainLevel)
				{	
					m_pATCommandWarp1->SetOutputGainLevel(0);
					m_pTelphoneDialDlg->m_bSetGainLevel = true;
				}
			}

			static char num[100] = {0};
			static int n = 0;
			num[n++] = wParam;
	
			if (strstr(num,"*#06#") != NULL)
			{	
				//IMEI
				extern char gIMEI[32];
				char imei[32] = {0};
				char *ptr = strstr(gIMEI, "GSN:");
				if(ptr)
				{
					char *ptr1 = (strstr(ptr, "\r\n"));
					if(ptr1)
						memcpy(imei, ptr+5, ptr1-ptr-5);
				}
				
				CString IM = Util::StringOp::ToCString(std::string(imei));
				m_pDeleteTipDlg->SetTitle(L"IMEI:"+IM,300000);
				m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				memset(num,0,100);
				n = 0;
				return 0;
			}

			if ( 100 == n)
			{
				memset(num,0,100);	
			}

			SendOutEvnet(WM_TEL_KEYCODE, 0);
			Dprintf("%x ", wParam);
			m_phoneLine[m_nline].pFSM->fireEvent(p3geKey,(void * const)wParam);
			if (m_pMainDlg->m_p3GSMSDlg->IsWindowVisible())
			{
				::SendMessage(m_pMainDlg->m_p3GSMSDlg->m_hWnd,WM_KEYDOWN,wParam,0);
			}
		}
		break;

	case WM_REDIAL:
		//add by qi 0716
		if (!OpenAllPort())
		{
			break;
		}

		if(m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
		{
			SendOutEvnet(8025, 0);
		}
		else
		{
			ReStoreBackLight();
			m_pTelephoneDlg->Redial();
		}
		break;

	case WM_MUTE:
		{	
			if (m_pTelephoneDlg->IsWindowVisible())
			{
				m_pTelephoneDlg->VolumeSwitch();
			}
		}
		break;

	case WM_RKEY:
		Dprintf("R KEY \n");
		break;

	case WM_SPEEDDIAL:
		{
			//add by qi 0716
			if (!OpenAllPort() || PstnActive())
			{
				break;
			}

			char c = wParam;
			std::string number ;
			if(m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
			{
				SendOutEvnet(8025, 0);
			}
			else
			{
				ReStoreBackLight();
				SendOutEvnet(8021, 0);
				if ( 1 == c)
				{  
					number = "12580";
				}
				else if ( 2 == c)
				{
					number = "10086";
				}
				m_pTelephoneDlg->DialContact(Data::TelephoneNumber(number));
				AddDesktopBtn();
			}

		}
		break;

	case WM_HUNGONTORECORD:
		Dprintf("RECORD ... \n");
		if (m_phoneLine[m_nline].pFSM->getCurrentState() == CMultimediaPhoneDlg::p3gsConnected)
		m_pTelephoneDlg->OnButtonTelephoneRecord();

		break;

	case WM_HUNGONINRECORD:
		Dprintf("HUNG ON IN RECORD \n");
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOnInRecord(); 
		break;

	case WM_BINGJIZHAIJI:
		Dprintf("bingjizhaiji 0---- \n");
		if (m_phoneLine[m_nline].pFSM->getCurrentState() == p3gsRing)
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HandleOff();
			Dprintf("bingjizhaiji 1---- \n");
		}		
		break;

	case WM_KEYDOWN:
		{
			if(wParam != VK_F9)
				ReStoreBackLight();
			SetScreenSaveTimer();

			if(playervideo_->isPlaying_ && m_pMainDlg->m_mainVideoDlg_->IsWindowVisible())
			{
				m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
			}
			else if(playerimage_->isPlaying_)
			{
				m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
			}
			else if(playeraudio_->isPlaying_ && m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible())
			{
				m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
			}

			if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSMSListDlg->
				m_pMmsReadDlg->IsWindowVisible())
			{
				((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSMSListDlg->
				m_pMmsReadDlg->SendMessage(WM_KEYDOWN, wParam, lParam);	
			}

		}
	case (WM_USER+1000):
//		Dprintf("InputWnd click\r\n");
		ReStoreBackLight();
		SetScreenSaveTimer();
		break;

	case WM_TEL_STATUS: //电话状态
		{	
			static int status = 0 ;
			if(wParam != TEL_SIGNALQUALITY && wParam != TEL_NETTYPE)
				m_pTelephoneDlg->SendMessage(WM_TEL_STATUS, wParam, lParam);
			else if(wParam == TEL_NETTYPE)    //网络状态
			{
				//网络状态
				if(lParam != 2)//G网
				{	
					status = 1 ;
				}
				else//T网
				{
					status = 2 ; 
				}
				
			}
			else if(wParam == TEL_SIGNALQUALITY && (m_nTELRigster >= TELRIGSTER_TD))
			{
				//刷新电量
				WCHAR txt[64] = {0};
				if ( 0 == lParam)//收不到网络
				{
					KillTimer(SEARCH_NET_TIMER);
					g_IsSignal = false;
					CString nonet = L".\\adv\\mjpg\\k5\\common\\png\\无网络.bmp" ;
					m_MJPGList.SetUnitBitmap(8,nonet,L"",true);
				}
				else
				{
					if ( 1 == status)
					{
						swprintf(txt, L".\\adv\\mjpg\\k5\\common\\png\\G网%d.bmp", lParam);
						m_MJPGList.SetUnitBitmap(8, txt, txt, TRUE);

					}
					else if( 2 == status )
					{
						swprintf(txt, L".\\adv\\mjpg\\k5\\common\\png\\T网%d.bmp", lParam);
						m_MJPGList.SetUnitBitmap(8, txt, txt, TRUE);
					}
					g_IsSignal = true;
				}

			}
		}
		break;

	case WM_CHANGE_BATTERY:
		{
			if(m_isHaveBattery && !isBatteryTimer)
			{
				char txt[64];
				sprintf(txt, ".\\adv\\mjpg\\k5\\common\\png\\电池%d.bmp", phone_->m_BatteryStatus.batteryProccess);
				m_MJPGList.SetUnitBitmap(11, txt, txt, TRUE);
			}
			else if(!m_isHaveBattery)
			{	
// 				bool bInvalidate = false; //add by qi 20100408
// 				if (m_binitOver)
// 				{
// 					bInvalidate = true ;	
// 				}
				m_MJPGList.SetUnitBitmap(11, L".\\adv\\mjpg\\k5\\common\\png\\无电池.bmp", L"", false);
			}
		}
		break;
	case WM_CHANGE_BATTERYOK:
		{
			KillTimer(0x120);
			isBatteryTimer = FALSE;
			char txt[64];
			sprintf(txt, ".\\adv\\mjpg\\k5\\common\\png\\电池%d.bmp", phone_->m_BatteryStatus.batteryProccess);
			m_MJPGList.SetUnitBitmap(11, txt, txt, FALSE);

			if (phone_->m_BatteryStatus.batteryType == BATTERY_DC)
			{
//				IsCloseNet(FALSE); // add by qi 20100406
				WriteMyLog_("OpenNet",strlen("OpenNet"));
				m_isBattery = FALSE;
				m_bNoDc = true;
				char *sChongdian = ".\\adv\\mjpg\\k5\\common\\外界电源.bmp";
				m_MJPGList.SetUnitBitmap(13, sChongdian, sChongdian, FALSE);
			}
			else if(!phone_->m_BatteryStatus.isBattery_DC)
			{	
				m_bNoDc = false;
//				IsCloseNet(TRUE); // add by qi 20100406
				WriteMyLog_("CloseNet",strlen("CloseNet"));

				m_MJPGList.SetUnitBitmap(13, L".\\adv\\mjpg\\k5\\common\\png\\无外界电源.bmp", L"", FALSE);
			}
			//m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
			//m_pDeleteTipDlg->SetPasswordModel(false);
			//m_pDeleteTipDlg->SetTitle("充电已完成!");
			//m_pDeleteTipDlg->ShowWindow_(TRUE);
			
			if(m_binitOver)
			m_MJPGList.Invalidate();
		}
		break;

	case WM_CHANGE_DCBATTERY:
		{
			if(phone_->m_BatteryStatus.typeChange == 1)
			{
				KillTimer(0x120);
				isBatteryTimer = FALSE;
				m_isBattery = TRUE;
				m_bNoDc = false;
				
				m_MJPGList.SetUnitBitmap(13, L".\\adv\\mjpg\\k5\\common\\png\\无外界电源.bmp", L"", FALSE);
				
				WriteMyLog_("battery",strlen("battery"));

				char txt[64];
				sprintf(txt, ".\\adv\\mjpg\\k5\\common\\png\\电池%d.bmp", phone_->m_BatteryStatus.batteryProccess);
				m_MJPGList.SetUnitBitmap(11, txt, txt, FALSE);

			}
			else if(phone_->m_BatteryStatus.typeChange == 0)
			{
				//m_pMainDlg->SendMessage(WM_PLAYVIDEO, 0, 0);
//				m_pDeleteTipDlg->SetTitle("正在使用电源供电!");
//				m_pDeleteTipDlg->ShowWindow_(TRUE);
				
				//wangzhenxing20100604
				ReStoreBackLight();   //插上电源时点亮屏幕
				char *sChongdian = ".\\adv\\mjpg\\k5\\common\\外界电源.bmp";
				m_MJPGList.SetUnitBitmap(13, sChongdian, sChongdian, FALSE);
				
				//add by qi 20100514
				static bool b = false;
				m_bNoDc = true;
//				if (b)
//				{	
//					IsReduceRreq(false);
//					IsCloseLCD(false);
//					IsCloseNet(false);
//				}
//				b = true;
				WriteMyLog_("Insert_Dianyuan",strlen("Insert_Dianyuan"));
				//add over

				if (m_isHaveBattery)
				{
					SetTimer(0x120, 500, NULL);
					isBatteryTimer = TRUE;
				}
				else
				{
					KillTimer(0x120);
					isBatteryTimer = FALSE;
				}
				m_isBattery = FALSE;
			}
			if(m_binitOver)
			m_MJPGList.Invalidate();
			SetControlBackLightTimer();
			phone_->m_BatteryStatus.typeChange = -1;
		}
		break;
	case WM_BATLOW:
		{
			int static count = 0;
			if(!phone_->m_BatteryStatus.isBattery_DC && g_BatteryLevel < gBatteryLevel[4])
			{
				if(0 == count)
				{
					BatteryLow();
					count = 1;
					SetTimer(0x129, 2*60*1000, NULL);
				}
			}
			else if(phone_->m_BatteryStatus.isBattery_DC || g_BatteryLevel > gBatteryLevel[4])
			{
				KillTimer(0x129);
				count = 0;
			}
		}
		break;
	case WM_ATCOMM_ERROR:
		{
			m_pTipDlg->SetTitle("TD 模块错误!", 10000);
			m_pTipDlg->ShowWindow_(TRUE);
		}
		break;
	case WM_SMS_SENDPRO:
		{
			SMS::SMSWarp::SM_PARAM *pParam = (SMS::SMSWarp::SM_PARAM *)wParam;
			CString s = "正在给 ";
			
			std::string number = pParam->TPA ;

			std::string name = GetName(number);

			if (!name.empty())
			{
				s += Util::StringOp::ToCString(name);	
			}
			else
			{
				s += pParam->TPA;
			}

			s += " 发送短信...";
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTipDlg->SetTitle(s, 40000);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTipDlg->ShowWindow_(TRUE);
		}
		break;

	case WM_SMS_STATUS:
		doWithSMS(wParam, lParam);
		break;
	case WM_RINGTIME:
		KillTimer(0x141);
		phone_->StartRing(L"");
		g_bStartring = FALSE;
		break;
	case WM_DELETESELITEM:		//阅读短信
		{
			if(wParam == 2)  //wangzhenxing20100531
			{
				//SetAPN((int)CMNET);
				SetAPN(m_pSettingDlg->m_pSetting->mmsType_);
				m_bIsDial = TRUE;
			}
			else if(wParam == 3)
			{
				Net3GHungOff();
			}
			else
			{
 				extern int g_iSmsID;
				
				//add by qi 0713
				extern VOID WriteMyLog_(char *ptr, int size);
				char log[50];
				sprintf(log,"smsID:%d",g_iSmsID);
				WriteMyLog_(log,strlen(log));

 				if(g_iSmsID >= 0)
 				{
					int index = g_iSmsID>>1;
					if(g_iSmsID & 1)//彩信
					{	
						sprintf(log,"mmsID:%d,index:%d",g_iSmsID,index);
						WriteMyLog_(log,strlen(log));

						m_pSMSListDlg->m_pMmsReadDlg->SetMMSInfo(index,RECV_TYPE);
						m_pSMSListDlg->RefreshList(MMS_TYPE);
						//if (!m_pSMSListDlg->m_pMmsReadDlg->IsWindowVisible())
						{
							m_pSMSListDlg->m_pMmsReadDlg->ShowWindow(SW_SHOW);
							CWnd *p = m_pSMSListDlg->m_pMmsReadDlg;
							AddIcon(Allicon[1],p,false);
						}

					}
					else
					{	
						sprintf(log,"smsID:%d,index:%d",g_iSmsID,index);
						WriteMyLog_(log,strlen(log));

						m_pSMSListDlg->m_pSmsReadDlg->SetSMSInfo(index,RECV_TYPE);
						m_pSMSListDlg->RefreshList(SMS_TYPE);
					//	if (!m_pSMSListDlg->m_pSmsReadDlg->IsWindowVisible())
						{
							m_pSMSListDlg->m_pSmsReadDlg->ShowWindow(SW_SHOW);
							CWnd *p = m_pSMSListDlg->m_pSmsReadDlg;
							AddIcon(Allicon[1],p,false);
						}

					}
					AddDesktopBtn();
 					g_iSmsID = -1;
 				}
				else if(-2 == g_iSmsID) //确定接收彩信（接收方式为手动时）
				{  
					//wangzhenxing20100127
					m_MmsMsg->Insert();
					m_isDialTimeout = 0;  //每次新收到彩信是置为0
					IsSendMessage(TRUE);
					SetEvent(g_hEvent);
					delete m_MmsMsg;
					m_MmsMsg = NULL;
				}
			}
			break;
		}
	case 0xCCCD:			//RASDIAL error
		OnRasErrorCode(wParam, lParam);
		break;

	case WM_PIN_INPUT:   //输入PIN
		{
			if(wParam == PIN_ERROR || wParam == 0)
			{
				m_pPasswordDlg->SettingType(CHECK_PINPASSWORD);
				m_pPasswordDlg->SetErrorCount(lParam);
				m_pPasswordDlg->ShowWindow_(SW_SHOW);
			}
			else if(wParam == PIN_PUK || wParam == PIN_FUK_ERROR)
			{
				m_pPasswordDlg->SettingType(CHECK_PUKPASSWORD);
				m_pPasswordDlg->SetErrorCount(lParam);
				m_pPasswordDlg->ShowWindow_(SW_SHOW);
			}
		}
		break;

	case WM_SIM_MESSAGE:
		{
			CString nosim = L".\\adv\\mjpg\\k5\\common\\png\\sim卡1.bmp" ;
			CString sim = L".\\adv\\mjpg\\k5\\common\\png\\sim卡.bmp";
			if (wParam)
			{
				if (lParam == 0)
				{
					m_MJPGList.SetUnitBitmap(12,sim,L"",false);
				}
				else
				m_MJPGList.SetUnitBitmap(12,sim,L"",true);
				
				m_bInsertSim = true ;
			}
			else
			{
				if (lParam == 0)
				{
					m_MJPGList.SetUnitBitmap(12,nosim,L"",false);
				}
				else
				m_MJPGList.SetUnitBitmap(12,nosim,L"",true);
				m_bInsertSim = false ;
			}
			
			//启动收网
			if (!m_bSearchNetWin && m_bInsertSim)
			{
				SetTimer(SEARCH_NET_TIMER,300,NULL);
			}

		}
		break;
	case WM_SEARCH_NET:
		{
			if ((bool)wParam)//收网成功
			{
				m_bSearchNetWin = true;
				KillTimer(SEARCH_NET_TIMER);
				CString net = L".\\adv\\mjpg\\k5\\common\\png\\T网0.bmp" ;
				m_MJPGList.SetUnitBitmap(8,net,L"",true);
			}
			else 
			{	
				CString nonet = L".\\adv\\mjpg\\k5\\common\\png\\无网络.bmp" ;
				m_bSearchNetWin = false;
				m_MJPGList.SetUnitBitmap(8,nonet,L"",false);

			}

		}
		break;

	case WM_VK_F:
		
		//add by qi 0716
		if (!OpenAllPort() || PstnActive())
		{
			break;
		}

		if(m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
		{
			SendOutEvnet(8025, 0);
		}
		else
		{
			ReStoreBackLight();
			BOOL flag1 = m_pSettingDlg->m_pSetting->isAdmin();
			if(flag1 && !m_pSettingDlg->m_bLogin)
			{
				m_pPasswordDlg->SettingType(CHECK_SUPPERPASSWORD);
				std::string strTemp = m_pSettingDlg->m_pSetting->adminPassword();
				m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
				m_pPasswordDlg->SetHWnd(this->m_hWnd);
				m_pPasswordDlg->ShowWindow_(SW_SHOW);
			}
			else
			{
				VK_F(wParam);
			}
		}
		break;

	case WM_RECEIVE_BOX:
		//add by qi 0716
		if (!OpenAllPort())
		{
			break;
		}

		if(m_pMainDlg->m_mainScreenSaveDlg_->m_isScreenPlay)
		{
			SendOutEvnet(8025, 0);
		}
		else
		{
			ReStoreBackLight();
			m_pMainDlg->StopVideo(FALSE, 0, 0);
			m_pMainDlg->m_p3GSMSDlg->OnClickMJPG(2,0);
			AddDesktopBtn();
		}
		break;
	case CHECK_SUPPERPASSWORD:
		if(1 == wParam)
		{
			m_pSettingDlg->m_bLogin = TRUE;
			VK_F(wParam);
		}
		break;
	case WM_APP_INPUT_PIN:
		{	
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowPinInputView();
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->SetErrorCount(lParam);
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowWindow_(SW_SHOW);
		}
		break;

	case WM_INPUT_PUK:
		{	
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowPukInputView();
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->SetErrorCount(lParam);
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowWindow_(SW_SHOW);
		}
		break;
	case WM_INPUT_PIN2:
		{	
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->m_nType = 8;
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowPin2InputView();
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->SetErrorCount(lParam);
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowWindow_(SW_SHOW);
		}
		break;

	case WM_INPUT_PUK2:
		{	
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowPuk2InputView();
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->SetErrorCount(lParam);
			m_pSettingDlg->m_pPinSetDlg->m_pPinInputDlg->ShowWindow_(SW_SHOW);
		}
		break;

// 	case WM_SMS_CSCA:
// 		{
// 			std::string *s = (std::string*)(wParam);
// 			m_pSmsSettingDlg->m_edtSmsc.SetWindowText(Util::StringOp::ToCString(*s));	
// 		}
// 		break;
	case WM_DELETE_MESSAGE:
		{
			m_pSMSWarp->gsmDeleteMessage((int)wParam, NULL);
			if(1 == (int)lParam)  //发送报告
			{
				if(g_sRecipientAddress.substr(0, 3) == "+86")
				{
					g_sRecipientAddress = g_sRecipientAddress.substr(3);
				}
				else if(g_sRecipientAddress.substr(0, 2) == "86")
				{
					g_sRecipientAddress = g_sRecipientAddress.substr(2);
				}
				std::string name = g_sRecipientAddress;
				std::vector<boost::shared_ptr<Data::Contact> > tempContact;
				tempContact = m_pTelephoneDlg->FindCallContact(g_sRecipientAddress);
				if(!tempContact.empty())
				{
					name = tempContact[0]->name();
				}
				CString title = L"彩信息已送达到" + Util::StringOp::ToCString(name);
				if(!FindVideoDlg(FALSE))
				{
					m_pTipDlg->SetTitle(title);
					m_pTipDlg->ShowWindow_(TRUE);
				}
			}
		}
		break;
	case WM_UNCONDITIONALTRANSFER:
		{	
			if ( 1 == wParam)
			{	
				m_bUnconditonSet = true;
			}
			else
			{	
				m_bUnconditonSet = false ;
			}

		}
		break;
	case WM_POWER_OFF:
		{
			DWORD powerOffThreadID = 0;
			static BOOL gIsFirstOFF = TRUE;
			if(gIsFirstOFF)
			{
				HANDLE m_pwrThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)PowerOffProc, 0, 0, &powerOffThreadID );
				gIsFirstOFF = FALSE;
			}
			break;
		}
		break;
	case WM_SEND_0X82_WIN:
		{	
			//在PSTN振铃状态去检查0x82指令是否发成功
			if (m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsRing)
			{
				m_pTelephoneDlg->m_bSend0x82Win = true;
			}
		}
		break;
	case WM_TELCALL_TIME:
		{
			CString sTitle_;
			sTitle_.Format(_T("本次通话的时长为: %d分-%d秒."), wParam/60, wParam%60);
			m_pWarningNoFlashDlg->SetTitle(sTitle_, 2000);
			m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		}
		break;
	case WM_DIAL_RETRY:
		{
			m_pWarningNoFlashDlg->SetTitle(L"拨号连接失败,稍后重试!",2000);

			m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			timeCount = 0;
			KillTimer(0x125);
		//	m_pNetStatusDlg->ADSLHungUp();			//lxz 20100706
			m_MJPGList.SetUnitBitmap(14, "", "", TRUE);
			m_MJPGList.Invalidate();
			m_isDialTimeout++;
			m_nTELRigster = TELRIGSTER_TD;

			break;
		}
	case WM_SMS_FULL:
		{
			FindVideoDlg(TRUE);
			m_pWarningNoFlashDlg->SetTitle(L"短消息存储已满!");
			m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		}
		break;
	case WM_NOSIM_WARNING:
		{
			FindVideoDlg(TRUE);
			m_pWarningNoFlashDlg->SetTitle(L"SIM卡已被拔出,请重启话机!");
			m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		}
		break;
	case WM_CSQ_REGISTER://重新收网成功
		{
			if (1 == wParam)
			{
				if (!g_IsSignal)//以前没信号
				{
					static bool bBind = false ;		
					if (!bBind)
					{
						bBind = true ;
						phone_->Bind(m_pATCommandWarp1);
						
						m_pSMSWarp->Register();
						m_pSMSWarp->Bind(m_pATCommandWarp1);//启动发送线程
						
						m_pATCommandWarp1->SimContactNum();//取出sim卡里的信息电话和姓名					
						m_pATCommandWarp1->CPMS();
						m_pATCommandWarp1->DUSIMR();
						
					}
					
					Dprintf("3G 搜网 %d Ok\r\n");				
					m_nTELRigster = TELRIGSTER_TD;				
					g_IsSignal = true ;

				}
							
			}
			else//失败
			{	
				m_pDeleteTipDlg->OnButtonDeleteTipCancel();
				m_pWarningNoFlashDlg->SetTitle(L"网络注册失败");
				m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				
				m_pSettingDlg->m_pCallSetDlg->ShowWindow_(SW_HIDE);

				g_IsSignal = false;	
			}
		}
		break;
	case WM_CURRENT_OPERATOR:
		{
			std::string *s = (std::string *)wParam;
			CString oper = Util::StringOp::ToCString(*s);
			CString filename = L"";
			if ( 0 == oper.Compare(L"中国移动") || 0 == oper.Compare(L"46000")
				|| 0 == oper.Compare(L"46002") || 0 == oper.Compare(L"46007"))
			{
				filename = L".\\adv\\mjpg\\k5\\common\\operator_yd.bmp";
			}
			else if ( 0 == oper.Compare(L"中国联通") || 0 == oper.Compare(L"46001"))
			{
				filename = L".\\adv\\mjpg\\k5\\common\\operator_lt.bmp";
			}
			else if ( 0 == oper.Compare(L"中国电信") || 0 == oper.Compare(L"46003"))
			{
				filename = L".\\adv\\mjpg\\k5\\common\\operator_dx.bmp";
			}
			m_MJPGList.SetUnitBitmap(20,filename,L"",true);
		}
		break;
	case WM_ALL_PHONE_NUM:
		{
			m_pTelephoneDlg->m_nAllPhoneNum = wParam ;
		}
		break;
	case WM_PRINTF_DATA:
		{	
			std::string *d = (std::string *)wParam ;
			extern IsTestCTA();
			//	if(IsTestCTA())
			{
				extern void EditPrintf(char *ptr);
				EditPrintf((char *)(d->c_str()));
 			}
		}
		break;
	case  WM_ATD_ERROR:
		{
			m_pWarningNoFlashDlg->SetTitle(L"电话拨打失败");
			m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			m_pTelephoneDlg->OnClickMJPG(1000,0);
		}
		break;
	case WM_SIM_STORAGE:
		{
			//检查下SIM卡存储是否已满
			m_pATCommandWarp1->CPMS();
		}
		break;
	case WM_CSCS:
		{
			m_pATCommandWarp1->CSCS("GSM");	
		}
		break;
	case WM_SMS_SEND_LENGTH:
		{
			m_pATCommandWarp1->SmsSend(wParam);
		}
		break;
	case WM_SMS_SEND:
		{
			std::string *data = (std::string *)wParam;
			m_pATCommandWarp1->SmsSend((char *)data->c_str(),data->size(),NULL,16384);
		}
		break;
	case WM_SMS_WRITE_LENGTH:
		{
			m_pATCommandWarp1->SmsWrite2Sim(wParam,3);
		}
		break;
	case WM_SMS_WRITE_TO_SIM:
		{
			std::string *data = (std::string *)wParam;
			m_pATCommandWarp1->SmsWrite2Sim((char *)data->c_str(),data->size());	
		}
		break;
	case WM_CHAR:
	//	VK_ESCAPE
		{
			int i = 0;
			i++;
			Dprintf("WM_CHAR %x %x\r\n", wParam, lParam);
		}
		break;

	case WM_C0M8_ERROR:
		{	
			Util::RS232 m_pRS232;
			bool b = m_pRS232.OpenPort(8);
			char log[50];
			sprintf(log,"open:%d",(int)b);

			extern VOID WriteMyLog_(char *ptr, int size);
			WriteMyLog_(log,strlen(log));
			
			Util::ATCommand::Instance()->serialPort_->Bind(m_pRS232.Handle());
			
		}
		break;

	case WM_CONTACT_NUM:
		break;
	
	default:
		break;
	}

	if (message == WM_TEL_HUNGOFF)
	{
		m_pContactInfoDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
		m_pSoundDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
		m_pSettingDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CMultimediaPhoneDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CMultimediaPhoneDlg::AddFSMRules(void)
{	
	//3g
	m_phoneLine[0].pFSM->registerRule(p3gsHangOff,p3geHangOn,p3gsHangOn,
		(void (__cdecl *const )(void *)) g3HangOn);
	m_phoneLine[0].pFSM->registerRule(p3gsHangOff,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
	m_phoneLine[0].pFSM->registerRule(p3gsHangOff,p3geCallID,p3gsRing,
		(void (__cdecl *const )(void *)) g3CallID);

	m_phoneLine[0].pFSM->registerRule(p3gsHangOn,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);	
	m_phoneLine[0].pFSM->registerRule(p3gsHangOn,p3geKey,p3gsKey,
		(void (__cdecl *const )(void *)) g3Key);
	m_phoneLine[0].pFSM->registerRule(p3gsHangOn,p3geDialing,p3gsDialing,
		(void (__cdecl *const )(void *)) g3Dialing);
	m_phoneLine[0].pFSM->registerRule(p3gsHangOn,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
	m_phoneLine[0].pFSM->registerRule(p3gsHangOn,p3geCallID,p3gsRing,
		(void (__cdecl *const )(void *)) g3CallID);

	m_phoneLine[0].pFSM->registerRule(p3gsKey,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[0].pFSM->registerRule(p3gsKey,p3geKey,p3gsKey,
		(void (__cdecl *const )(void *)) g3Key);
	m_phoneLine[0].pFSM->registerRule(p3gsKey,p3geDialing,p3gsDialing,
		(void (__cdecl *const )(void *)) g3Dialing);
	m_phoneLine[0].pFSM->registerRule(p3gsKey,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
	m_phoneLine[0].pFSM->registerRule(p3gsKey,p3geCallID,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);

	m_phoneLine[0].pFSM->registerRule(p3gsDialing,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[0].pFSM->registerRule(p3gsDialing,p3geConnected,p3gsConnected,
		(void (__cdecl *const )(void *)) g3Contect);//对方接听

	m_phoneLine[0].pFSM->registerRule(p3gsAltering,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[0].pFSM->registerRule(p3gsAltering,p3geConnected,p3gsConnected,
		(void (__cdecl *const )(void *)) g3Contect);//对方接听


	m_phoneLine[0].pFSM->registerRule(p3gsRing,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[0].pFSM->registerRule(p3gsRing,p3geHangOn,p3gsConnected,
		(void (__cdecl *const )(void *)) g3Contect);//我接听
	m_phoneLine[0].pFSM->registerRule(p3gsRing,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
//	m_phoneLine[0].pFSM->registerRule(p3gsRing,p3geCallID,p3gsRing,
//		(void (__cdecl *const )(void *)) g3Ring);

	m_phoneLine[0].pFSM->registerRule(p3gsConnected,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[0].pFSM->registerRule(p3gsConnected,p3geKey,p3gsConnected,
		(void (__cdecl *const )(void *)) g3SubDial);
	m_phoneLine[0].pFSM->registerRule(p3gsConnected,p3geHold,p3gsHold,
		(void (__cdecl *const )(void *)) g3Hold);

	m_phoneLine[0].pFSM->registerRule(p3gsHold,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);//
	m_phoneLine[0].pFSM->registerRule(p3gsHold,p3geConnected,p3gsConnected,
		(void (__cdecl *const )(void *)) g3Contected);

	m_phoneLine[0].pFSM->registerRule(p3gsPstn,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);//
	m_phoneLine[0].pFSM->registerRule(p3gsPstn,p3geCallID,p3gsRing,
		(void (__cdecl *const )(void *)) g3CallID);
	m_phoneLine[0].pFSM->registerRule(p3gsPstn,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);//
	
	m_phoneLine[0].pFSM->setStartState(p3gsHangOff);

	//3g_2
	m_phoneLine[1].pFSM->registerRule(p3gsHangOff,p3geHangOn,p3gsHangOn,
		(void (__cdecl *const )(void *)) g3HangOn);
	m_phoneLine[1].pFSM->registerRule(p3gsHangOff,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
	m_phoneLine[1].pFSM->registerRule(p3gsHangOff,p3geCallID,p3gsRing,
		(void (__cdecl *const )(void *)) g3CallID);
	
	m_phoneLine[1].pFSM->registerRule(p3gsHangOn,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);	
	m_phoneLine[1].pFSM->registerRule(p3gsHangOn,p3geKey,p3gsKey,
		(void (__cdecl *const )(void *)) g3Key);
	m_phoneLine[1].pFSM->registerRule(p3gsHangOn,p3geDialing,p3gsDialing,
		(void (__cdecl *const )(void *)) g3Dialing);
	m_phoneLine[1].pFSM->registerRule(p3gsHangOn,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
	m_phoneLine[1].pFSM->registerRule(p3gsHangOn,p3geCallID,p3gsRing,
		(void (__cdecl *const )(void *)) g3CallID);
	
	m_phoneLine[1].pFSM->registerRule(p3gsKey,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[1].pFSM->registerRule(p3gsKey,p3geKey,p3gsKey,
		(void (__cdecl *const )(void *)) g3Key);
	m_phoneLine[1].pFSM->registerRule(p3gsKey,p3geDialing,p3gsDialing,
		(void (__cdecl *const )(void *)) g3Dialing);
	m_phoneLine[1].pFSM->registerRule(p3gsKey,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
	m_phoneLine[1].pFSM->registerRule(p3gsKey,p3geCallID,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
	
	m_phoneLine[1].pFSM->registerRule(p3gsDialing,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[1].pFSM->registerRule(p3gsDialing,p3geConnected,p3gsConnected,
		(void (__cdecl *const )(void *)) g3Contect);//对方接听
	
	m_phoneLine[1].pFSM->registerRule(p3gsAltering,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[1].pFSM->registerRule(p3gsAltering,p3geConnected,p3gsConnected,
		(void (__cdecl *const )(void *)) g3Contect);//选路


	m_phoneLine[1].pFSM->registerRule(p3gsRing,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[1].pFSM->registerRule(p3gsRing,p3geHangOn,p3gsConnected,
		(void (__cdecl *const )(void *)) g3Contect);//我接听
	m_phoneLine[1].pFSM->registerRule(p3gsRing,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);
//	m_phoneLine[1].pFSM->registerRule(p3gsRing,p3geCallID,p3gsRing,
//		(void (__cdecl *const )(void *)) g3Ring);
	
	m_phoneLine[1].pFSM->registerRule(p3gsConnected,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);
	m_phoneLine[1].pFSM->registerRule(p3gsConnected,p3geKey,p3gsConnected,
		(void (__cdecl *const )(void *)) g3SubDial);
	m_phoneLine[1].pFSM->registerRule(p3gsConnected,p3geHold,p3gsHold,
		(void (__cdecl *const )(void *)) g3Hold);
	
	m_phoneLine[1].pFSM->registerRule(p3gsHold,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);//
	m_phoneLine[1].pFSM->registerRule(p3gsHold,p3geConnected,p3gsConnected,
		(void (__cdecl *const )(void *)) g3Contected);
	
	m_phoneLine[1].pFSM->registerRule(p3gsPstn,p3geRing,p3gsRing,
		(void (__cdecl *const )(void *)) g3Ring);//
	m_phoneLine[1].pFSM->registerRule(p3gsPstn,p3geCallID,p3gsRing,
		(void (__cdecl *const )(void *)) g3CallID);
	m_phoneLine[1].pFSM->registerRule(p3gsPstn,p3geHangOff,p3gsHangOff,
		(void (__cdecl *const )(void *)) g3HangOff);//
	
	m_phoneLine[1].pFSM->setStartState(p3gsHangOff);
	
	//pstn
	m_phoneLine[2].pFSM->registerRule(pstnsHangOff,pstne3gRing,pstns3gRing,
		(void (__cdecl *const )(void *)) pstn3gRing);
	m_phoneLine[2].pFSM->registerRule(pstnsHangOff,pstneRing,pstnsRing,
		(void (__cdecl *const )(void *)) pstnRing);
	m_phoneLine[2].pFSM->registerRule(pstnsHangOff,pstneCallID,pstnsRing,
		(void (__cdecl *const )(void *)) pstnCallID);

	m_phoneLine[2].pFSM->registerRule(pstns3gRing,pstneHangOff,pstnsHangOff,
		(void (__cdecl *const )(void *)) pstnHangOff);
	m_phoneLine[2].pFSM->registerRule(pstns3gRing,pstneRing,pstnsRing,
		(void (__cdecl *const )(void *)) pstnRing);
	m_phoneLine[2].pFSM->registerRule(pstns3gRing,pstneCallID,pstnsRing,
		(void (__cdecl *const )(void *)) pstnCallID);

	m_phoneLine[2].pFSM->registerRule(pstnsRing,pstneHangOff,pstnsHangOff,
		(void (__cdecl *const )(void *)) pstnHangOff);
	m_phoneLine[2].pFSM->registerRule(pstnsRing,pstneHangOn,pstnsConnected,
		(void (__cdecl *const )(void *)) pstnContect);
	m_phoneLine[2].pFSM->registerRule(pstnsRing,pstneRing,pstnsRing,
		(void (__cdecl *const )(void *)) pstnRing);
	m_phoneLine[2].pFSM->registerRule(pstnsRing,pstneCallID,pstnsRing,
		(void (__cdecl *const )(void *)) pstnCallID);
	
	m_phoneLine[2].pFSM->registerRule(pstnsConnected,pstneHangOff,pstnsHangOff,
		(void (__cdecl *const )(void *)) pstnHangOff);
	m_phoneLine[2].pFSM->registerRule(pstnsConnected,pstneHold,pstnsHold,
		(void (__cdecl *const )(void *)) pstnHold);

	m_phoneLine[2].pFSM->registerRule(pstnsHold,pstneHangOff,pstnsHangOff,
		(void (__cdecl *const )(void *)) pstnHangOff);
	m_phoneLine[2].pFSM->registerRule(pstnsHold,pstneConnected,pstnsConnected,
		(void (__cdecl *const )(void *)) pstnContect);

	m_phoneLine[2].pFSM->setStartState(pstnsHangOff);
	
	
}

void CMultimediaPhoneDlg::HangOff(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->OnTimer(0x123);
}

void CMultimediaPhoneDlg::HangOn(void* param)
{
	//::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);
	//::SetTimer((theApp.m_pMainWnd)->m_hWnd, 0x124, 50, NULL);		//摘机

}

void CMultimediaPhoneDlg::Key(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer, KEYTIMEOUT, 0);
	
//	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Key_(param); old 

	// add by qi 2009_11_05 new	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelphoneDialDlg->Key_(param);

}

void CMultimediaPhoneDlg::SubKey(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->SubKey_(param);
}

void CMultimediaPhoneDlg::Connect(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Connect_(param);
}

void CMultimediaPhoneDlg::Ring(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Ring_(param);
}

void CMultimediaPhoneDlg::CallID(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->CallID_(param);
}

void CMultimediaPhoneDlg::OnTimer(UINT nIDEvent) 
{
	//lxz test 20090309
	if(nIDEvent == 0x999)
	{
		KillTimer(0x999);
		SetTimer(0x998, 15000, NULL);
		PostMessage(WM_TEL_RING, 0, 0);
		PostMessage(WM_TEL_RING, 0, 0);
		PostMessage(WM_TEL_RING, 0, 0);
		PostMessage(WM_TEL_RING, 0, 0);
	}
	else if(nIDEvent == 1002)
	{
		char restartTimer[64];
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		sprintf(restartTimer, "current_time:%04d-%02d-%02d %02d:%02d:%02d\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
		WriteMyLog_(restartTimer,strlen(restartTimer));
		
	}
	else if(nIDEvent == 0x998)
	{
		PostMessage(WM_TEL_HUNGOFF, 0, 0);
		KillTimer(0x998);
		SetTimer(0x999, 1000, NULL);
	}
	else if(nIDEvent == 0x126)
	{
		KillTimer(0x126);
		PostMessage(WM_ATCOMM_ERROR, 0, 0);
	}

	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer)
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
		//m_pFSM->fireEvent(teConnect, 0);
	}
	else if (nIDEvent == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer)
	{
		Dprintf("no_ring_come");
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
		int line ;
		if (m_phoneLine[0].pFSM->getCurrentState() == p3gsRing )
		{	
			line = 0 ;
			m_phoneLine[0].pFSM->fireEvent(p3geHangOff, (void *)line);
		}
		else if (m_phoneLine[1].pFSM->getCurrentState() == p3gsRing )
		{	
			line = 1 ;
			m_phoneLine[1].pFSM->fireEvent(p3geHangOff, (void *)line);
		}
	}
	else if (nIDEvent == m_uiPSTNRingTimer)
	{
		KillTimer(m_uiPSTNRingTimer);
		if (m_phoneLine[2].pFSM->getCurrentState() == pstnsRing)
		{
			int line = 3;
			m_phoneLine[2].pFSM->fireEvent(pstneHangOff,(void *)line);
		}

	}
	else if(nIDEvent == IDT_CONTROLBACKLIGHT_TIME+1)
	{
		//lxz test 数据库 内存泄露

		extern VOID WriteLog(CHAR* str);
		char restartTimer[64];
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		DWORD value = DMemprintf("");
		sprintf(restartTimer, "mem:%04d-%02d-%02d %02d:%02d:%02d %x\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, value);
//		WriteLog(restartTimer);
		
		//刷新闹铃
		//m_pMainDlg->FindTodayAlarm();   //20090221   移到设置闹铃列表中

		gIsToLight = FALSE;
		SetNightControlBackLightTimer();

	}

	else if(nIDEvent == IDT_CONTROLBACKLIGHT_TIME)//进入黑屏
	{
		KillTimer(IDT_CONTROLBACKLIGHT_TIME);
		if(!IsMediaPlay())
		{
			SetTimer(0x121, 500, NULL);
			
			if (!m_bNoDc && m_pSettingDlg->m_pSetting->isDeepSleep
				&& m_bSearchNetOver &&  0 == VERSION_TYPE )//没插电源
			{
				SetTimer(0x132, 1*15*1000, NULL);    //只有当电池供电的时候才进入深度睡眠   //lxz 20100804
			}
			m_nBackLightValue = 7 - m_pSettingDlg->m_pSetting->blackLightValue();
		}
	}
	else if(nIDEvent == IDT_MAIN_SAVESCREEN_TIME)//进入屏保
	{
		//lxz 20080623
		//return;

		CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
		HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
		BOOL flag2 = ::IsWindowVisible(hWnd);
		BOOL flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
		if(flag2 && flag3)
			return;

		KillTimer(IDT_MAIN_SAVESCREEN_TIME);
		int type = (int)m_pSettingDlg->m_pSetting->screenSaverContent();
		pWnd->m_mainScreenSaveDlg_->SetSaveScreenType(type, TRUE);//1 图片屏保

	}
	else if(nIDEvent == IDT_ALARMTIME)		//设置闹铃
	{
		static int nOldAlarmID = 0;
		
		KillTimer(IDT_ALARMTIME);
		boost::shared_ptr<Data::Scheduler> m_result = Data::Scheduler::GetFromDatabaseById(m_nAlarmID);
		nOldAlarmID  = m_nAlarmID;
		SetAlarmList();
		if(m_result)
		{
			ReStoreBackLight();

			std::string content = m_result->what();
			std::string ring = "/flashdrv/my_ring/"; 
			ring += m_result->ring();
			CString content_ = "";
			content_ += content.c_str();
			CTime tm = m_result->startTime();
			CString sTime;
			sTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
			m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
		//	m_AlarmShowDlg->SetTxt(nOldAlarmID, sTime, "标题: ", content_, ring);
			m_AlarmShowDlg->SetTxt(nOldAlarmID, sTime, content_, ring);
			m_AlarmShowDlg->ShowWindow_(SW_SHOW);
		}
	}
	else if(nIDEvent == 0x123)		//挂机
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x123);

		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
		
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(NULL);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SendOutEvnet(WM_TEL_HUNGOFF, 1);//未接接听，自动关闭窗口时模拟挂机事件

	}
	else if(nIDEvent == 0x124)		//摘机
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(NULL);

	}
	else if(nIDEvent == 0x125)
	{
		// wzx20100318
		if(m_nTELRigster == TELRIGSTER_DIALED)
		{
			timeCount = 0;
			m_isDialTimeout = 0;
			KillTimer(nIDEvent);
		}
		else
		{
			timeCount++;
		}
		
		if(timeCount > 100)
		{
			timeCount = 0;
			KillTimer(nIDEvent);
			m_pNetStatusDlg->ADSLHungUp();
			m_MJPGList.SetUnitBitmap(14, "", "", TRUE);
			m_isDialTimeout++;
		}
	}
	
	else if(nIDEvent == 0x120)
	{
		static unsigned int index = 0;
		char txt[64];
		sprintf(txt, ".\\adv\\mjpg\\k5\\common\\png\\电池%d.bmp", index++);
		m_MJPGList.SetUnitBitmap(11, txt, txt, TRUE);
		if(index > 5)
		{
			index = 0;
		}
	}
	else if(nIDEvent == 0x132)	//进入深度睡眠   //lxz 20100804
	{
		SetMPUOff();
	}
	else if(nIDEvent == 0x121)
	{
		if(m_nBackLightValue > 0 && m_nBackLightStatus != 0)
		{
			m_nBackLightValue--;
			SetBackLight(m_nBackLightValue);
		}
		else
		{
			m_nBackLightValue = 7 - m_pSettingDlg->m_pSetting->blackLightValue();
			KillTimer(0x121);
			
			//add by qi 20100514
//			if (!m_bNoDc)
//			{
//				IsCloseLCD(TRUE);
//				IsCloseNet(TRUE);
//				IsReduceRreq(TRUE); //影响系统功能
//				WriteMyLog_("ReduceReq",strlen("ReduceReq"));
//			}

		}
	}
	else if(nIDEvent == 0x122)
	{
		int v = 7 - m_pSettingDlg->m_pSetting->blackLightValue();
		if(m_nBackLightValue <= v)
		{
			Dprintf("Open BackLight Level %d\r\n", m_nBackLightValue);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(m_nBackLightValue);
			m_nBackLightValue++;
		}
		else
		{
			KillTimer(0x122);
		}
	}
	else if(nIDEvent == 0x127)
	{
		HWND hwnd = ::FindWindow(TEXT("VMREADER_DaTang"), NULL);
		static int count = 0;
		if(hwnd == NULL)
		{
			count++;
			if(count == 2)
			{
				IsUnConnectNet();
				count = 0;
			}
		}
	}
	else if (nIDEvent == 0x128)//网路不能被注册
	{	
		KillTimer(0x128);
		m_pWarningNoFlashDlg->SetTitle(L"网络不能被注册");
		m_pWarningNoFlashDlg->ShowWindow_(TRUE);

	}
	else if(nIDEvent == 0x129)
	{
		BatteryLow();
	}
	else if(nIDEvent == 0x130)
	{
		KillTimer(nIDEvent);
		m_MJPGList.Invalidate(false);
	}
	else if(nIDEvent == 0x141)
	{
		KillTimer(nIDEvent);
		g_bStartring = FALSE;
	}
	else if (nIDEvent == SEARCH_NET_TIMER)
	{
		static int net = 0 ;
		WCHAR txt[64];
		swprintf(txt, L".\\adv\\mjpg\\k5\\common\\png\\T网%d.bmp", net);

		m_MJPGList.SetUnitBitmap(8,txt,txt,true);
		net++;
		if ( net > 5)
		{
			net = 0;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CMultimediaPhoneDlg::OnMainSoftKey(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 2:
		OnButtonIME();
		break;
	case 3:
	//	OnButtonLine();
		break;
	case 4:
		break;
	case 5:
		break;

	case 6:
		break;
		
	case 7:
		OnButtonSound();
		break;
	case 8:
		OnButtonInformation();
		break;
	case 9:
		OnButtonSetting();
		break;
	}

}


//振铃 摘机 按键等外部事情发送给 屏保 MP3 VIDEO 听留言窗口
void CMultimediaPhoneDlg::SendOutEvnet(WPARAM w, LPARAM l)
{
	SendMessage(WM_GEN_EVENT);
	CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
	switch(w)
	{
		case WM_TEL_KEYCODE:
			{
				if(pWnd->m_mainScreenSaveDlg_->m_isScreenPlay)
				{
					pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, l);
				}
			}
			break;
		case WM_TEL_RING:
		case WM_TEL_HUNGON:
			{
				pWnd->StopVideo(TRUE, 0, 0);
			}
			break;
		case WM_TEL_HUNGOFF:
			{	
				if (!FindActiveLine())//没有活动的路，恢复播放
				{
					IsSendMessage(TRUE);
				}
				
			}
			break;
		case WM_TOUCH_CLICK:
			{
			}
			break;
	}
}

void CMultimediaPhoneDlg::OnEvent(WPARAM w, LPARAM l)
{
	if(w == 1)
		SendOutEvnet(WM_TOUCH_CLICK, 0);
	SetScreenSaveTimer();
}

BOOL CMultimediaPhoneDlg::SetNightControlBackLightTimer()
{
	BOOL ret = FALSE;
	BOOL flag2 = m_pSettingDlg->m_pSetting->isNightControlBlackLight();
	BOOL flag1 = m_pSettingDlg->m_pSetting->isContrlBlackLight();
	int t1 = m_pSettingDlg->m_pSetting->nightControlBlackLightStartTime();
	int t2 = m_pSettingDlg->m_pSetting->nightControlBlackLightEndTime();
	
	SYSTEMTIME sTime;
	int sStartTime[] = {18, 19, 20, 21, 22};
	int sEndTime[] = {5, 6, 7, 8, 9};
	GetLocalTime(&sTime);
	if((sTime.wHour >= sStartTime[t1] && sTime.wHour < 24) || (sTime.wHour < sEndTime[t2] && sTime.wHour >= 0))
	{
		if(flag2 || m_isBattery)
		{
			CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
			if(pWnd == NULL)
				return ret;
			HWND hWnd0 = ::FindWindow(L"csplayer_win0", L"csplayer window0");
			HWND hWnd2 = ::FindWindow(L"csplayer_win2", L"csplayer window2");
			BOOL isVisible0 = ::IsWindowVisible(hWnd0);
			BOOL isVisible2 = ::IsWindowVisible(hWnd2);
			BOOL flag0 = pWnd->m_mainVideoDlg_->IsWindowVisible();
			BOOL flag2 = pWnd->m_mainPhotoDlg_->IsWindowVisible();
			if((isVisible0 && flag0) || (isVisible2 && flag2) || g_isShift)
				return ret;

			SetTimer(IDT_CONTROLBACKLIGHT_TIME, 30*1000, NULL);   
			ret = TRUE;
		}
		else
		{
			ret = FALSE;
			KillTimer(IDT_CONTROLBACKLIGHT_TIME);
		}
		return ret;
	}
	else if(!gIsToLight && !m_isBattery)
	{
		gIsToLight = TRUE;
		if(!flag1)				//黑屏唤醒一次
		{
			KillTimer(IDT_CONTROLBACKLIGHT_TIME);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->ReStoreBackLight();
		}
	}
	return ret;
}

void CMultimediaPhoneDlg::SetControlBackLightTimer()
{
	KillTimer(IDT_CONTROLBACKLIGHT_TIME);
	BOOL flag1 = m_pSettingDlg->m_pSetting->isContrlBlackLight();
	if(m_isBattery || flag1)
	{
		CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
		if(pWnd == NULL)
			return;
		if(IsMediaPlay())
			return;
	//  chnage by qi 20100521
		int nTimer_[] = {10*1000, 20*1000, 30*1000, 40*1000};
		int nTimer = m_pSettingDlg->m_pSetting->contrlBlackLightWaitTime();
		
		//add by qi 20100514
		bool static b = false;
		if (!m_bNoDc && b)
		{
			nTimer_[nTimer] = 10*1000;
			WriteMyLog_("NoDC",strlen("NoDC"));
		}
		b = true;

		SetTimer(IDT_CONTROLBACKLIGHT_TIME, nTimer_[nTimer], NULL);
	//	Dprintf("Set ControlBackLightTimer %d\r\n", nTimer[nTimer_]);
	}
}

BOOL CMultimediaPhoneDlg::CancelBalckLightSaveTimer()
{
//	Dprintf("CancelBalckLightSaveTimer\r\n");
	KillTimer(IDT_MAIN_SAVESCREEN_TIME);
	KillTimer(IDT_CONTROLBACKLIGHT_TIME);
	return TRUE;
}

BOOL CMultimediaPhoneDlg::IsMediaPlay()
{
	if(m_phoneLine[0].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff  ||
		m_phoneLine[1].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff ||
		m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsHangOff)
	{
		return TRUE;
	}
	else if(m_pSMSWarp != NULL && m_pSMSWarp->IsSendingSms())//正在发送短信
	{	
		return TRUE;		
	}
	else if(m_pMainDlg->m_mainMp3Dlg_->m_IsPlay == 1 || m_pMainDlg->m_mainVideoDlg_->m_IsPlay == 1)
	{
		return TRUE;
	}
	else if(g_isAutoPlay || m_pSMSListDlg->m_pMmsReadDlg->m_isReadMid) //是否自动播放相册和midi音乐
	{
		return TRUE;
	}
	else if(g_isShift)  //是否正在转移文件
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CMultimediaPhoneDlg::IsSendMessage(BOOL flag)
{
	if(m_vIcon.size() > 0)
	{
		CString str = m_vIcon[m_vIcon.size()-1].second;
		if((str == Allicon[5]) || (str == Allicon[6]))
		{
			m_pMainDlg->StopVideo(FALSE, 1, 0);
		}

		if(flag)
		{
			m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
		}
	}
}

void CMultimediaPhoneDlg::BatteryLow()
{
	m_pMainDlg->StopVideo(TRUE, 0, 0);

	if(m_pMainDlg->m_mainMp3Dlg_->m_IsPlay == 1)
	{
		m_pWarningNoFlashDlg->SetTitle("电池电量低! 已停止音乐播放!");
	}
	else if(m_pMainDlg->m_mainVideoDlg_->m_IsPlay == 1)
	{
		m_pWarningNoFlashDlg->SetTitle("电池电量低! 已停止影院播放!");
		m_pMainDlg->m_mainVideoDlg_->OnExit();
	}
	else if(g_isAutoPlay) //是否自动播放相册和midi音乐
	{
		m_pWarningNoFlashDlg->SetTitle("电池电量低! 已停止相册播放!");
		m_pMainDlg->m_mainPhotoDlg_->OnExit();
	}
	else if(m_pSMSListDlg->m_pMmsReadDlg->m_isReadMid)
	{
		m_pWarningNoFlashDlg->SetTitle("电池电量低! 请停止播放彩信!");
	}
	else if(g_isShift)  //是否正在转移文件
	{
		m_pWarningNoFlashDlg->SetTitle("电池电量低! 请停止转移文件播放!");
	}
	else
	{
		m_pWarningNoFlashDlg->SetTitle("电池电量低! 系统将进入待机模式!");
	}

	m_pWarningNoFlashDlg->ShowWindow_(TRUE);

	//电量低的提示音 add bu qi 20100811
	TCHAR filename[] = L"\\flashdrv\\电量不足请充电.mp3" ;
	phone_->StartRing(filename,2);

}

BOOL CMultimediaPhoneDlg::IsCloseLCD(BOOL flag)
{
	
	if(flag)
	{
		m_bIsCloseLCD = TRUE;	
	}
	else
	{
		m_bIsCloseLCD = FALSE;
	}
//	return TRUE;
	
	
	//关闭LCDC和时钟
	HDC hdc = ::GetDC(NULL);
	int EscCode;
	if(flag)
	{
		EscCode = 1;
		ExtEscape(hdc, 6147, sizeof(EscCode),  (LPSTR)&EscCode, 0, 0);
		m_bIsCloseLCD = TRUE;	
	}
	else
	{
		EscCode = 0;
		ExtEscape(hdc, 6147, sizeof(EscCode),  (LPSTR)&EscCode, 0, 0);
		m_bIsCloseLCD = FALSE;
	}
	::ReleaseDC(NULL, hdc);
	return TRUE;
}

BOOL CMultimediaPhoneDlg::IsCloseNet(BOOL flag)
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		Dprintf("SetNetPower %d\n", flag);
		
		unsigned long size;
		DWORD inBuff;
		if(flag)
			inBuff=1;
		else
			inBuff=2;
		DWORD value;
		if(DeviceIoControl(m_hPort, SCL_NET_POWERCTL, &inBuff, sizeof(DWORD), &value, sizeof(DWORD), &size, NULL))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMultimediaPhoneDlg::IsReduceRreq(BOOL flag)
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		Dprintf("SetReduceRreq %d\n", flag);
		
		unsigned long size;
		DWORD inBuff;
		if(flag)
		{
			inBuff=96000000;
		}
		else
		{
			inBuff=384000000;
		}
		DWORD value;
		if(DeviceIoControl(m_hPort, SCL_SET_CPU_FREQ, &inBuff, sizeof(DWORD), &value, sizeof(DWORD), &size, NULL))
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMultimediaPhoneDlg::ReStoreBackLight()
{
	Dprintf("Start Open LCD!\r\n");
	KillTimer(0x121);
	KillTimer(0x132);    

	int blackValue = m_nBackLightStatus;
	
	//恢复背光
	int v = 7 - m_pSettingDlg->m_pSetting->blackLightValue();
	if(v == 0)
	{
		v = 1;
	}
	
	if(v != m_nBackLightStatus)
	{
		m_nBackLightValue = m_nBackLightStatus;
		SetTimer(0x122, 300, NULL);

//		if (!m_bNoDc)//无外接电源
//		{
//			IsReduceRreq(FALSE);
//			IsCloseLCD(FALSE);
//			IsCloseNet(FALSE);
//			WriteMyLog_("NoReduceRreq",strlen("NoReduceRreq"));
//			Dprintf("Open LCD Success!\r\n");
//		}
		SetScreenSaveTimer();

		Dprintf("Open BackLight OK!\r\n");
	}

	if(blackValue == 0)    //为黑
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CMultimediaPhoneDlg::SetScreenSaveTimer()
{

//	lxz test 20080623
//	return;

//	Dprintf("UnSet ControlBackLightTimer\r\n");
	SetControlBackLightTimer();

	CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
	if(!pWnd)
		return;

	HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
	HWND hWnd1 = ::FindWindow(L"csplayer_win2", L"csplayer window2");

 	HWND hIEWnd = ::FindWindow(TEXT("iExplore"), NULL);
	
	KillTimer(IDT_MAIN_SAVESCREEN_TIME);
	BOOL flag1 = m_pSettingDlg->m_pSetting->isUseScreenSaver();
	BOOL flag2 = FALSE;
	if(hWnd)
		flag2 = ::IsWindowVisible(hWnd);
	BOOL flag3 = FALSE;
	if(pWnd->m_mainVideoDlg_)
		flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
	BOOL flag4 = FALSE;

	if (m_phoneLine[m_nline].pFSM != NULL)
	{
		if(m_phoneLine[m_nline].pFSM->getCurrentState() != CMultimediaPhoneDlg::p3gsHangOff)
		flag4 = TRUE;
	}

	if(flag1)
	{
		CTimeSpan nTimer = m_pSettingDlg->m_pSetting->screenSaverDuration();
		int nSecond = nTimer.GetTotalSeconds();
		if(flag2 && flag3)    //video 
		{
			Dprintf("UnSet ScreenSave Timer \r\n");
		}
		else if(::IsWindowVisible(hWnd1))   //电子相册
		{
			Dprintf("UnSet ScreenSave Timer \r\n");
		}
 		else if(hIEWnd && ::IsWindowVisible(hIEWnd))   //浏览器
 		{
 			Dprintf("UnSet ScreenSave Timer \r\n");
 		}
		else if(flag4)  //电话正在使用
		{
			Dprintf("UnSet ScreenSave Timer \r\n");
		}
		else
		{
			SetTimer(IDT_MAIN_SAVESCREEN_TIME, nSecond*1000, NULL);
			static int gTimerCount = 0;
	//		Dprintf("Set ScreenSave Timer = %d \r\n", gTimerCount++);
		}
	}
	else
	{
	//	Dprintf("UnSet ScreenSave Timer \r\n");
	}
}

void CMultimediaPhoneDlg::KillScreenSaveTimer()
{
	KillTimer(IDT_MAIN_SAVESCREEN_TIME);
//	Dprintf("Kill ScreenSave Timer \r\n");
}

void CMultimediaPhoneDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
	Dprintf("USB do with %d %d\r\n", w, l);
	if(g_IsRun)
	{
		if(m_pSettingDlg->m_copyfileDlg)
			m_pSettingDlg->m_copyfileDlg->SendMessage(WM_USB_MSG, w, 0);
		if(m_pMainDlg->m_p3GHomePicDlg)
			m_pMainDlg->m_p3GHomePicDlg->SendMessage(WM_USB_MSG, w, 0);
		if(m_pMainDlg->m_p3GHomeJoyDlg)
			m_pMainDlg->m_p3GHomeJoyDlg->SendMessage(WM_USB_MSG, w, 0);
		if(m_pMainDlg->m_p3GHomeMovieDlg)
			m_pMainDlg->m_p3GHomeMovieDlg->SendMessage(WM_USB_MSG, w, 0);
	}
}

////////SCL////////////////
/////SCL////////////////////////
void CMultimediaPhoneDlg::InitSCLPort()
{
	m_hPort = CreateFile (_T("SCL1:"), // Pointer to the name of the port
		GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
		0,            // Share mode
		NULL,         // Pointer to the security attribute
		CREATE_ALWAYS,// Create New File as any case
		0,            // Port attributes
		NULL);
}

BOOL CMultimediaPhoneDlg::GetManuInfo(char *pBuffer, int length)
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		//Get Information;
		unsigned long size;
		UINT nIn;
		if(DeviceIoControl(m_hPort, SCL_GET_PRODUCT_INFO, &nIn, 0, pBuffer, 512, &size, NULL))
		{
			return TRUE;
		}
	}
	return FALSE;
}

bool CMultimediaPhoneDlg::GetSleepFlag(char &flag)
{
	unsigned long size;
	UINT nIn;
	if(DeviceIoControl(m_hPort, SCL_GET_SleepFlag, &nIn, 0, &flag, sizeof(char), &size, NULL))
	{
		return true;
	}
	return false;
}

//设置watchdog
BOOL CMultimediaPhoneDlg::SetWatchDog()
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		unsigned long size;
		UINT value;
		UINT nIn;
		if(DeviceIoControl(m_hPort, SCL_ENABLE_WATCHDOG, &nIn, 0, &value, sizeof(UNIT), &size, NULL))
		{
			return TRUE;
		}
	}
	return FALSE;
}

//设置ADSL电源开关
BOOL CMultimediaPhoneDlg::SetADSLPower(BOOL isOn)
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		Dprintf("SetADSLPower %d\n", isOn);
		
		unsigned long size;
		DWORD inBuff;
		if(isOn)
			inBuff=1;
		else
			inBuff=2;
		DWORD value;
		if(DeviceIoControl(m_hPort, SCL_ADSL_POWERCTL, &inBuff, sizeof(DWORD), &value, sizeof(DWORD), &size, NULL))
		{
			return TRUE;
		}
	}
	return FALSE;
}

//设置系统重启
BOOL CMultimediaPhoneDlg::SetReStart()
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		unsigned long size;
		UINT value;
		UINT nIn;
		if(DeviceIoControl(m_hPort, SCL_SYSTEM_RESET, &nIn, 0, &value, sizeof(UINT32), &size, NULL))
		{
			return TRUE;
		}
	}
	return FALSE;
}

//关闭打开背光
BOOL CMultimediaPhoneDlg::SetBackLight(int isType)
{
	if(m_nBackLightStatus != isType)
	{
		if(isType == 6)
		{
			int i=0;
			i++;
		}
		//todo: 设置
		if (m_hPort != INVALID_HANDLE_VALUE)
		{
			unsigned long size;
			BACKLIGHTCMD blightcmd;
			blightcmd.bkCmd = 0x01;
			blightcmd.bkLevel = isType;
			UINT value;
			if(DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(UINT), &size, NULL))
			{
				Dprintf("Level %d set is OK!!!\r\n", isType);
				m_nBackLightStatus = isType;
				return TRUE;
			}
			else
			{
				Dprintf("Level %d set Error!!!\r\n", isType);
			}
		}
	}
	return FALSE;
}

BOOL CMultimediaPhoneDlg::SetBackLight_(int level)
{
	if(m_nBackLightStatus != level)
	{
		//todo: 设置
		if (m_hPort != INVALID_HANDLE_VALUE)
		{
			unsigned long size;
			BACKLIGHTCMD blightcmd;
			blightcmd.bkCmd = 0x01;
			blightcmd.bkLevel = level;
			UINT value;
			if(DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(UINT), &size, NULL))
			{
				Dprintf("Level %d set is OK!!!\r\n", level);
				return TRUE;
			}
			else
			{
				Dprintf("Level %d set Error!!!\r\n", level);
			}
		}
	}
	return FALSE;
}

//读取背光的光敏值
#define OLDVALUE_MAX   3
UINT32 CMultimediaPhoneDlg::GetBackLightValue()
{
	int ret;
	static int oldblValue[OLDVALUE_MAX] = {0, 0, 0};
	static int oldcount = 0;
	//todo: 设置
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		unsigned long size;
		BACKLIGHTCMD blightcmd;
		blightcmd.bkCmd = 0x02;
		blightcmd.bkLevel = 0;
		DWORD value;
		if(DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(DWORD), &size, NULL))
		{
		//	Dprintf("back light %d\n", value);	
			oldblValue[oldcount++%OLDVALUE_MAX] = value;
			BOOL flag = FALSE;
			for(int i = 0; i < OLDVALUE_MAX; i++)
			{
				if(m_nBackLightStatus == 0)   //黑
				{
					ret = 7;
					if(oldblValue[i] >= 5000)    //5000
					{
						ret = 0;    //黑
						break;
					}
				}
				else
				{
					ret = 0;
					if(oldblValue[i] < 5000)    //50000
					{
						ret = 7;    //亮
						break;
					}
				}
			}
		}
	}

	return ret;
}

BOOL CMultimediaPhoneDlg::EnryRun()
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		unsigned long size;
		ENCRYCMD cmd;
		cmd.bkCmd = 0x01;
		cmd.bkRandom = rand();
		DWORD ret;
		if(DeviceIoControl(m_hPort, SCL_ENCRYGPT_VERIFY, &cmd, sizeof(ENCRYCMD), &ret, sizeof(DWORD), &size, NULL))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}

UINT32 CMultimediaPhoneDlg::GetEnry()
{
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		unsigned long size;
		ENCRYCMD cmd;
		cmd.bkCmd = 0x02;
		UINT32 value;
		if(DeviceIoControl(m_hPort, SCL_ENCRYGPT_VERIFY, &cmd, sizeof(ENCRYCMD), &value, sizeof(UINT32), &size, NULL))
		{
			return value;
		}
	}
	return 0;
}


UINT32 CMultimediaPhoneDlg::GetRawBackLightValue()
{
	int ret = 0;
	//todo: 设置
	if (m_hPort != INVALID_HANDLE_VALUE)
	{
		unsigned long size;
		BACKLIGHTCMD blightcmd;
		blightcmd.bkCmd = 0x02;
		blightcmd.bkLevel = 0;
		DWORD value;
		if(DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(DWORD), &size, NULL))
		{
			ret = value;
	//		Dprintf("back light %d\n", value);	
		}
	}

	return ret;
}
//////////////////end SCL/////////////////////////

//alarm setting
void CMultimediaPhoneDlg::SetAlarmList()
{
	//	return;
	
	KillTimer(IDT_ALARMTIME);
	
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CTime time = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, 0, 0, 0);
	
	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(),time.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(),time.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";
	std::string order = "startTime";
	std::vector<boost::shared_ptr<Data::Scheduler> > m_result = Data::Scheduler::GetFromDatabaseByOrder(filter, order); 
	if (!m_result.empty())
	{
		for(int i = 0; i < m_result.size(); i++)
		{
			CTime tm = m_result[i]->startTime();
			GetLocalTime(&curtime);
			CTime nowtm = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
			if(m_result[i]->tipsType() && (nowtm < tm))
			{
				m_nAlarmID = m_result[i]->id();
				int timeout = (tm.GetHour() - curtime.wHour)*3600 + (tm.GetMinute() - curtime.wMinute)*60 + (tm.GetSecond() - curtime.wSecond);
				SetTimer(IDT_ALARMTIME, timeout*1000, NULL);
				break;
			}
		}
	}

	//查找今天没有看过过期的闹铃
	m_pMainDlg->FindTodayAlarm();    //定时器中查找会有内存泄露.   //20090221
}
/*
10658066    OTA特服号
1065806601  远程文字留言
1065806612  家庭理财 

1065806611  便民信息
1065806651  健康信息
1065806661  教育信息
1065806621  社区信息
*/

#define SPECODE_MAX   6
char *gpChaSpecod[] = {"1065806601", "1065806612", "1065806611", "1065806651", "1065806661", "1065806621"};
//	

int  g_iSmsID = -1;

//int type = 0;       短信
//			 1;		  家庭百事通
//           2;       来电振铃
void CMultimediaPhoneDlg::SMSSpecRing(int type, CString s)
{
	if(0 == m_nBackLightStatus)    //wangzhenxing20100527
	{
		ReStoreBackLight();
	}

	UINT16 volume[] = {0xFF00, 0xdd00, 0xbb00, 0xaa00, 0x8800, 0x6600, 0x4400,
	0x2200, 0x1600, 0x1000};
	if(type == 0)
	{
		if(m_pSettingDlg->m_pSetting->isSmsRing_)
		{
			CFileStatus status;
			CString filename = "/flashdrv/my_ring/甜蜜.wav";
			if(CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_pSettingDlg->m_pSetting->smsRingFilename_)),status))
			{
				filename = m_pSettingDlg->m_pSetting->smsRingFilename_.c_str();
			}

			int nVolume = m_pSettingDlg->m_pSetting->smsRingVolume_ - 1; //change by qi 20100617
 			phone_->g_RingSound = (volume[nVolume]|(volume[nVolume]<<16));
			phone_->StartRing((LPTSTR)(LPCTSTR)filename, 1);
			
			KillTimer(0x141);
			g_bStartring = TRUE;
			SetTimer(0x141, 4000, NULL);
		}
	}
	else if(type == 1)
	{
		CFileStatus status;
		CString filename = "/flashdrv/my_ring/甜蜜.wav";
		if(CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_pSettingDlg->m_pSetting->specodeRingFilename_)),status))
		{
			filename = m_pSettingDlg->m_pSetting->specodeRingFilename_.c_str();
		}
		
		int nVolume = m_pSettingDlg->m_pSetting->specodeRingVolume_;
		phone_->g_RingSound = (volume[nVolume]|(volume[nVolume]<<16));
		phone_->StartRing((LPTSTR)(LPCTSTR)filename, 1);
	}
	else if(type == 2)	//振铃
	{
	//	int nVolume = m_pSettingDlg->m_pSetting->soundVolume();
		int nVolume = m_pSettingDlg->m_pSetting->phoneCallRingVolume_ - 1;//change by qi 20100617
		phone_->g_RingSound = (volume[nVolume]|(volume[nVolume]<<16));
		phone_->StartRing((LPTSTR)(LPCTSTR)s);
	}
}

void CMultimediaPhoneDlg::doWithSMS(WPARAM wParam, LPARAM lParam)
{
	if(wParam == WM_SMS_SMS)    //短信
	{
		Data::Message* pMsg = (Data::Message *)lParam;
		BOOL flag = FALSE;
	//	for(int i = 0 ; i < SPECODE_MAX; i++)
		std::string sCode = m_pSettingDlg->m_pSetting->speCode1_;
		if(pMsg->remote.address.substr(0, 6) == sCode.substr(0, 6))
		{
		//	if(pMsg->remote.address == gpChaSpecod[i])
			{
				if(pMsg->remote.address == m_pSettingDlg->m_pSetting->speCode3_)   //家庭留言
				{
					pMsg->group = Data::Message::gReMoteSMS;
					pMsg->Insert();
					g_iSmsID = (pMsg->id() << 1);
					//phone_->StartRing(L"/flashdrv/my_ring/甜蜜.wav", 1);

					SMSSpecRing(1, "");
					
					if (!m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible() && 
						!m_pMainDlg->m_mainVideoDlg_->IsWindowVisible()&&
						!playervideo_->isPlaying_ && !playerimage_->isPlaying_)
					{
						m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
						m_pDeleteTipDlg->SetPasswordModel(false);
						m_pDeleteTipDlg->SetTitle("接收到一条家庭留言,是否阅读?");
						m_pDeleteTipDlg->ShowWindow_(TRUE);
					}

				}
				else
				{
					BOOL f =TRUE;

					if(f)
					{
						Data::MultimediaDownload *result = new Data::MultimediaDownload();
						
						result->header.opCode = "";
						result->header.version = "";
						result->header.timestamp = pMsg->timestamp.ToCTimeStamp();
						result->multimediaInfos.type = Data::MultimediaDownload::MultimediaInfo::tText;
						result->multimediaInfos.spCode = pMsg->remote.address;
						result->multimediaInfos.subject = Util::StringOp::FromTimestampFormat(result->header.timestamp);
						result->multimediaInfos.filename = "";
						result->multimediaInfos.weatherCode = 0;
						result->multimediaInfos.content = pMsg->unicodeData;
						result->multimediaInfos.isRead = false;
						
						if(pMsg->remote.address.substr(0, 8) != "10658153")
						{
							result->Insert();
							//	int i = 0;
							//	i++;
						}
						else
						{
							int i =0;
							i++;
						}

						SMSSpecRing(1, "");
					}
				}
				flag = TRUE;
			}
		}

		if(!flag)
		{
			//普通短信
			if(pMsg->remote.address.substr(0, 8) != "10658153")
			{
				
				BOOL f =TRUE;
				if(pMsg->total > 1)
				{
					pMsg->group = Data::Message::gExternSms;//家庭百事通的超长短信
					pMsg->Insert();
					
					std::string sfilter = "[group] = " + Util::StringOp::FromInt(Data::Message::gExternSms); 
					sfilter += " AND [transactionId] = '";
					sfilter += pMsg->transactionId;
					sfilter += "'";
					std::vector<boost::shared_ptr<Data::Message> >vMessageCurrentResult = Data::Message::GetFromDatabase(sfilter);
					int i = vMessageCurrentResult.size();
					if(vMessageCurrentResult.size() == pMsg->total)	//短信收满
					{
						std::string content = "";
						for(int i = 0; i < pMsg->total; i++)
						{
							for(int j = 0; j < pMsg->total; j++ )
							{
								if(vMessageCurrentResult[j]->no == i+1)
								{
								 	content += vMessageCurrentResult[j]->unicodeData;
								}
							}
						}
						for(i = 0; i < pMsg->total; i++)
						{
							vMessageCurrentResult[i]->Remove();
						}
						pMsg->unicodeData = content;
					}
					else
						f = FALSE;
				}

				if(f)
				{
					pMsg->group = Data::Message::gReceive;
					m_pMainDlg->m_p3GSMSDlg->SaveSmsData(pMsg);
					//pMsg->Insert();

					g_iSmsID = (pMsg->id()<<1);

					//add by qi 0713
					extern VOID WriteMyLog_(char *ptr, int size);
					char log[50];
					sprintf(log,"Recive_smsID:%d,index:%d",g_iSmsID,pMsg->id());
					WriteMyLog_(log,strlen(log));
					
					SMSSpecRing(0, "");

					if (!m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible() && 
						!m_pMainDlg->m_mainVideoDlg_->IsWindowVisible()&&
						!playervideo_->isPlaying_ && !playerimage_->isPlaying_ )
					{
						m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
						m_pDeleteTipDlg->SetPasswordModel(false);
						m_pDeleteTipDlg->SetTitle("接收到一条短信,是否阅读?");
						m_pDeleteTipDlg->ShowWindow_(TRUE);

						//刷新下彩信列表
						m_pSMSListDlg->RefreshList(SMS_TYPE);

					}
					::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
				}

			}
		}
	
		delete pMsg;

	}
	else if(wParam == WM_SMS_MMS)	//彩信
	{//wangzhenxing20100128
		m_MmsMsg = (Data::MmsMessage*)lParam;
		
		std::string number = m_MmsMsg->remote.address;
		if(number.substr(0, 3) == "+86")
		{
			number = number.substr(3);
		}
		else if(number.substr(0, 2) == "86")
		{
			number = number.substr(2);
		}
		std::vector<boost::shared_ptr<Data::Contact> > tempContact;
		tempContact = m_pTelephoneDlg->FindCallContact(number);
		if(!tempContact.empty())  //名片信息里有此号码
		{
			MMSReciveTip(m_MmsMsg);
		}
		else
		{
			int anonymous = m_pMessageSet->AnonymousMMSReceive();
			switch(anonymous)
			{
			case 1: //拒接
				break;
			case 2:  //接收
				MMSReciveTip(m_MmsMsg);
				break;
			case 3:  //每次询问
				{
					m_pMainDlg->StopVideo(FALSE, 0, 0);  //如果匿名信息每次询问，则强制提示
					
					m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
					m_pDeleteTipDlg->SetTitle("收到一条匿名彩信!是否接收?");
					m_pDeleteTipDlg->ShowWindow_(TRUE);
					
					g_iSmsID = -2;

					extern VOID WriteMyLog_(char *ptr, int size);
					WriteMyLog_("每次询问",strlen("每次询问"));

				}
				break;
			default:
				break;
			}
		}
	}
	else if(wParam == WM_SMS_OTA)	//OTA
	{
		Data::OtaMessage* pMsg = (Data::OtaMessage *)lParam;
		pMsg->Insert();
/*
		extern void StartHttp(char *url, int type = 0, int dwtype = 0, char *lfilename = NULL, char *cIMEI = NULL, char *cTerminalId = NULL);
		
		if(pMsg->instruction == Data::tMenuManager)				    //菜单维护
		{
		//	StartHttp((char *)pMsg->url.c_str(), 0, 2);
		}
		else if(pMsg->instruction == Data::tWeather)				//天气预报
		{
			StartHttp((char *)pMsg->url.c_str(), 0, 1);
		}
		else if(pMsg->instruction == Data::tMultimediaDownload)	    //媒体下载
		{
			StartHttp((char *)pMsg->url.c_str(), 0, 0);
		}
		*/
	}
	else if(wParam == WM_SMS_SENDRESULT)
	{	
		if(lParam == 1)	//短信发送成功
		{
			if(!FindVideoDlg(FALSE))
			{
				m_pTipDlg->SetTitle("短信发送成功!");
				m_pTipDlg->ShowWindow_(TRUE);
			}
		}
		else if(lParam > 1)//短信发送失败
		{

			CString tip = L"发送失败";
		/*	switch (lParam)
			{
			case GSM_UNKOMN:
				tip = L"未知错误";
				break;
			case GSM_NET_ERROR:
				tip = L"网络超时";
				break;
			case GSM_ERR:
				tip = L"发送失败";
				break;
			default:
				tip = L"发送失败";
				break;
			}*/
			if(!FindVideoDlg(FALSE))
			{
				m_pTipDlg->SetTitle(tip);
				m_pTipDlg->ShowWindow_(TRUE);
			}
		}

		m_pSMSListDlg->RefreshList(SMS_TYPE);

	}
}

void CMultimediaPhoneDlg::SetPreADSLDial()
{
	return;    //不启用提前拨号  20100906 lxz

	m_bIsDial = TRUE;
	SetEvent(g_hEvent);
}

void CMultimediaPhoneDlg::UnSetPreADSLDial()
{
	return; //不启用提前拨号  20100906 lxz

	m_MJPGList.SetUnitBitmap(14, "", "", TRUE);
	m_MJPGList.Invalidate();
	
	if(m_nTELRigster >= TELRIGSTER_DIALED)
	{
		CNetStatusDlg::ADSLHungUp();
		m_MJPGList.SetUnitBitmap(14, "", "", TRUE);
		m_MJPGList.Invalidate();
		Sleep(3000);
		SetAPN(m_pSettingDlg->m_pSetting->mmsType_);
		m_nTELRigster = TELRIGSTER_TD;
	}
}

void CMultimediaPhoneDlg::MMSReciveTip(Data::MmsMessage *mms)
{
	SMSSpecRing(0, "");
	
	if(m_pMessageSet->IsAutoReceive())
	{
		mms->Insert();
		m_isDialTimeout = 0;

		if(!FindVideoDlg(FALSE))
		{
			//wangzhenxing20100604
			m_pTipDlg->SetTitle(L"接收一条彩信通知!");
			m_pTipDlg->ShowWindow_(TRUE);
		}

		SetEvent(g_hEvent);
		delete m_MmsMsg;
		m_MmsMsg = NULL;
	}
	else
	{
		FindVideoDlg(TRUE);  //如果是手动接收，则强制提示
		m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		m_pDeleteTipDlg->SetTitle("收到一条彩信!是否接收?");
		m_pDeleteTipDlg->ShowWindow_(TRUE);
		
		extern VOID WriteMyLog_(char *ptr, int size);
		WriteMyLog_("每次询问1",strlen("每次询问1"));
		g_iSmsID = -2;
	}
}

#include <Raserror.h>
#include <Ras.h>
//网络拨号
void CMultimediaPhoneDlg::OnRasErrorCode(WPARAM w, LPARAM l)
{
	int result = w;
	char txt[64];
	sprintf(txt, "Ras code %d %d %d\r\n", result, w, l);
	DWriteDat("", (UINT8 *)txt, strlen(txt));
	if(result >= RASCS_Interactive && result <= RASCS_PasswordExpired)
		result = result - RASCS_Interactive + 19;
	else if(result >= RASCS_Connected && result <= RASCS_Disconnected)
		result = result - RASCS_Connected + 23;

	if(result > 25)
		result = 25;
	
	if(result == 23)   //连接上
	{
		m_MJPGList.SetUnitBitmap(14, ".\\adv\\mjpg\\k5\\common\\gprs状态.bmp", "", TRUE);
		m_nTELRigster = TELRIGSTER_DIALED;
	}
	else if(result == 24)   //未连接
	{
		m_nTELRigster = TELRIGSTER_TD;
	}
	extern char *adsl_status[];
	Dprintf("Dial : %s \r\n", adsl_status[result]);
}

//3G挂断
void CMultimediaPhoneDlg::Net3GHungOff()
{
	if(m_nTELRigster >= TELRIGSTER_DIALED)
	{
		CNetStatusDlg::ADSLHungUp();
		m_MJPGList.SetUnitBitmap(14, "", "", TRUE);
		m_MJPGList.Invalidate();
		Sleep(3000);
	//	SetAPN((int)CMWAP);
		SetAPN(m_pSettingDlg->m_pSetting->mmsType_);
	}
	m_nTELRigster = TELRIGSTER_TD;
	if(0)				//检测是否需要永远拨号
	{
		m_bIsDial = TRUE;
	}
	else
	{
		m_bIsDial = FALSE;
	}
//	SetAPN((int)CMWAP);
}

//搜网需要的条件
void CMultimediaPhoneDlg::SearchNetInit()
{
	char QUSIM[] = "AT^DUSIMR=2\r";   //查询SIM卡是否可用
	m_pATCommandWarp1->Transaction_(QUSIM, strlen(QUSIM));
}

//手机搜网
BOOL CMultimediaPhoneDlg:: doSerachrTDNet(BOOL isSleepStart)
{
	DWORD	dwStart  =  GetTickCount();
	phone_->Bind_(m_pATCommandWarp1);
	m_pSMSWarp->Bind_(m_pATCommandWarp1);
	
	BOOL isNetOK = FALSE;
	m_bSearchNetOver = false ;
	int ret = m_pATCommandWarp1->Connect("",isSleepStart);	
	m_bSearchNetOver = true ;

	if (ret == 1)    //1 成功    
	{	
		static bool bBind = false ;		
		if (!bBind)
		{
			bBind = true ;
			phone_->Bind(m_pATCommandWarp1);
			
			m_pSMSWarp->Register();
			m_pSMSWarp->Bind(m_pATCommandWarp1);
		}

		DWORD offset = GetTickCount() - dwStart;   
	   	Dprintf("3G 搜网 %d Ok\r\n", offset);

		m_nTELRigster = TELRIGSTER_TD;
		
		m_pSMSWarp->SetOTANumber(m_pSettingDlg->m_pSetting->speCode11_);//设置OTA号码		
		
		m_pATCommandWarp1->SimContactNum();//取出sim卡里的信息电话和姓名

		m_pATCommandWarp1->CPMS();
		m_pATCommandWarp1->DUSIMR();

		isNetOK = TRUE;

	}
	else if(ret == 2) //需要注册码
	{
		isNetOK = FALSE;
	}
	else if(ret == 3) //未插卡
	{
		isNetOK = FALSE;
	}
	else if (ret == 4)//有卡，未检测到网路
	{
		//add by qi 0812
		phone_->Bind(m_pATCommandWarp1);		
		m_pSMSWarp->Register();
		m_pSMSWarp->Bind(m_pATCommandWarp1);
		
		
		m_pATCommandWarp1->SimContactNum();//取出sim卡里的信息电话和姓名

		FindVideoDlg(TRUE);
		SetTimer(0x128,3000,NULL);
		
		KillTimer(SEARCH_NET_TIMER);
		PostMessage(WM_SEARCH_NET,0,0);
		isNetOK = FALSE;
	}
	return isNetOK;
}

//手机注册
void CMultimediaPhoneDlg:: doRegisterTel()
{
	std::vector<boost::shared_ptr<Data::Register> > result = Data::Register::GetFromDatabase(""); 

	Dprintf("doRegisterTel 0\r\n");
	HttpParam *pHttpParam = new HttpParam;
	pHttpParam->type = 1;
	pHttpParam->apn_type = m_nAPN.apn_type;
	pHttpParam->apn_proxy = m_nAPN.proxy;
	if(result.size() > 0)
	{
		pHttpParam->cIMEI = (char *)result[0]->IMEI.c_str();
	//	pHttpParam->cTerminalId = (char *)result[0]->terminalId.c_str();
	}
	else
	{
		pHttpParam->cIMEI = "1234";
	//	pHttpParam->cTerminalId = "1234567890";
	}
	pHttpParam->cTerminalId = (char *)m_sMobileNumber.c_str();
	Dprintf("doRegisterTel 1\r\n");

	pHttpParam->url = "http://211.138.198.106:8888/";
	if(HttpProcesse(pHttpParam) == 0)
	{
		m_nTELRigster = TELRIGSTER_REG;
		isFirstReg = FALSE;
//		AfxMessageBox(L"注册到宽连十方平台成功!");
	}
//	else
	//	m_nTELRigster = TELRIGSTER_REG;
}

BOOL CMultimediaPhoneDlg::GetIs3GDialed()
{	
	
	if(m_nTELRigster >= TELRIGSTER_DIALED && m_nAPN.apn_type == m_pSettingDlg->m_pSetting->mmsType_)
		return TRUE;
	else if(m_nTELRigster >= TELRIGSTER_DIALED)
	{
		Net3GHungOff();
		return FALSE;
	}
	else
	{
	//	SetAPN((int)CMWAP);
		SetAPN(m_pSettingDlg->m_pSetting->mmsType_);
		return FALSE;
	}
	
	return FALSE;
}

void CMultimediaPhoneDlg::ParseSmil(CString FilePath, MMS::MMSWarp::MMS_SubmitRequest &r)
{
	CString str = FilePath + L"*" ;
	WIN32_FIND_DATA fd;
	HANDLE hfind;
	hfind	  =	 FindFirstFile(str,&fd);
	if(hfind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
			{ 
				;//如果是文件夹
			}
			else
			{ 
				//如果是文件
				CString strfl = fd.cFileName ;
				
				if ( strfl.Find(L".smil") > 0 || strfl.Find(L".smi") > 0 || strfl.Find(L".SMIL") > 0 || strfl.Find(L".SMI") > 0)
				{
					//解析smil	
					CString file = FilePath + strfl;
					MMS::MMSWarp::GetMMSWarp()->DecodeSmil(Util::StringOp::FromCString(file).c_str(), r.pars, r.layout);
					return;
				}
			}
		}
		while(FindNextFile(hfind,&fd));
	}
	
	MMS::MMSWarp::GetMMSWarp()->AddRootLayout(&r, 176, 216);
	MMS::MMSWarp::GetMMSWarp()->AddRegion(&r, MMS::MMSWarp::rtText, 0, 0, 176, 144);
	MMS::MMSWarp::GetMMSWarp()->AddRegion(&r, MMS::MMSWarp::rtImage, 0, 144, 176, 72);
	
	CString sI = "";
	CString sT = "";
	CString sV = "";
	CString sA = "";
	hfind	  =	 FindFirstFile(str,&fd);
	if(hfind !=	 INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
			{ 
				;//如果是文件夹
			}
			else
			{ 
				//如果是文件
				CString strfl = fd.cFileName ;				
				if ( strfl.Find(L".bmp") > 0 || strfl.Find(L".BMP") > 0 || strfl.Find(L".jpg") > 0 || strfl.Find(L".JPG") > 0\
					|| strfl.Find(L".gif") > 0 || strfl.Find(L".GIF") > 0 )
				{
					sI = FilePath + strfl;
				}
				else if ( strfl.Find(L".txt") > 0 || strfl.Find(L".TXT") > 0)
				{
					sT = FilePath + strfl;
				}
				else if ( strfl.Find(L".rm") > 0 || strfl.Find(L".RM") > 0 || strfl.Find(L".wav") > 0 || strfl.Find(L".WAV") > 0\
					|| strfl.Find(L".MID") > 0 || strfl.Find(L".mid") > 0 )
				{
					sA = FilePath + strfl;
				}
				else if ( strfl.Find(L".3gp") > 0 || strfl.Find(L".3GP") > 0 || strfl.Find(L".mp4") > 0 || strfl.Find(L".MP4") > 0)
				{
					//解析smil	
					sV = FilePath + strfl;
				}
			}
		}
		while(FindNextFile(hfind,&fd));
	}
	MMS::MMSWarp::GetMMSWarp()->AddPar(&r, Util::StringOp::FromCString(sI).c_str(), Util::StringOp::FromCString(sT).c_str(), Util::StringOp::FromCString(sA).c_str(), Util::StringOp::FromCString(sV).c_str());
	return;

}

void CMultimediaPhoneDlg:: doWithDownLoad()
{
	if(isFirstGif)
	{
		Sleep(100);
		return;
	}
	
	WaitForSingleObject(g_hEvent, INFINITE);//////

	BOOL flag = FALSE;
	std::string fileter = "type = ";
	fileter += Util::StringOp::FromInt(Data::MMSData::tpReadySend);
	std::vector<boost::shared_ptr<Data::MMSData> > mmsSendResult = Data::MMSData::GetFromDatabase(fileter);
	for(int i = 0; i < mmsSendResult.size(); i++)
	{
		CString path = Util::StringOp::ToCString(mmsSendResult[i]->SavePath);
		if(!DetectDIR((TCHAR *)path.GetBuffer(128)))
		{
			if(!FindVideoDlg(FALSE))
			{
				m_pWarningNoFlashDlg->SetTitle(L"路径不存在!无法发送");
				m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			}
			mmsSendResult[i]->Remove();
			return;
		}
		if(m_isDialTimeout >= 3)
		{
			FindVideoDlg(TRUE);
			m_pWarningNoFlashDlg->SetTitle(L"拨号连接超时,下次开机将重发");
			m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			mmsSendResult[i]->type = Data::MMSData::tpRepeatSend;//下次开机重发
			mmsSendResult[i]->repeatCount++;
			mmsSendResult[i]->Update();
			m_bIsDial = FALSE;
			break;
		}

		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CTime ct = CTime(tm);
		unsigned int year = mmsSendResult[i]->localSendTime.year+2000;
		unsigned int month = mmsSendResult[i]->localSendTime.month;
		unsigned int day = mmsSendResult[i]->localSendTime.day;
		unsigned int hour = mmsSendResult[i]->localSendTime.hour;
		unsigned int minite = mmsSendResult[i]->localSendTime.minite;
		unsigned int second = mmsSendResult[i]->localSendTime.second;
		CTime st = CTime(year, month, day, hour, minite, second);
		CTimeSpan sp;
		if(ct >= st)
		{
			sp = ct - st;
		}
		else
		{
			sp = st - ct;
		}
		unsigned int time = sp.GetTotalSeconds();
		if((mmsSendResult[i]->TimeOfExpiry != 0) && (time >= mmsSendResult[i]->TimeOfExpiry))  //查看彩信是否过期
		{
			FindVideoDlg(TRUE);
			m_pWarningNoFlashDlg->SetTitle(L"本条彩信已过期,无法发送");
			m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			mmsSendResult[i]->type = Data::MMSData::tpUnSend;
			mmsSendResult[i]->isRead = false; //存到草稿箱
			mmsSendResult[i]->Update();
			continue;
		}
		
		if(!GetIs3GDialed())
		{
			m_bIsDial = TRUE;
			return;
		}
		
		flag = TRUE;
		
		//彩信的发送
		std::string sendError = "";
		std::string sendOk = "";
		std::string number_;
		std::string number = mmsSendResult[i]->RecipientAddress;

		if(number.empty())
		{
			CString path = mmsSendResult[i]->SavePath.c_str();
			if(DetectDIR((LPTSTR)(LPCTSTR)path))
			{
				DeleteDirectory(path);
			}
			mmsSendResult[i]->Remove();   //如果号码为空则删除
			continue;
		}

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
				MMS::MMSWarp::MMS_SubmitRequest r;
				
				r.TransactionID = mmsSendResult[i]->TransactionId;
				r.Subject = Util::StringOp::ToUTF8(mmsSendResult[i]->Subject);
				r.RecipientAddress = number_;
				r.TimeOfExpiry = mmsSendResult[i]->TimeOfExpiry;
				CString sPath = mmsSendResult[i]->SavePath.c_str();
				
				ParseSmil(sPath, r);
				
				char *buf = NULL;
				if(buf == NULL)
					buf = new char[1024*1024];
				
				memset(buf, 0, 1024*1024);
				int len = 0;
				r.SenderAddress = mmsSendResult[i]->SenderAddress;
				len = MMS::MMSWarp::GetMMSWarp()->EncodeSubmitRequest(&r, buf);
				
				HttpParam *pHttpParam = new HttpParam;
				pHttpParam->type = 2;//发彩信
				pHttpParam->apn_type = m_nAPN.apn_type;
				pHttpParam->apn_proxy = m_nAPN.proxy;
				pHttpParam->pData = buf;
				pHttpParam->dataLentg = len;
				pHttpParam->url = m_nAPN.http; 
				
				int ret = HttpProcesse(pHttpParam);
				delete []buf;
				if(ret == 0)
				{
					if(!FindVideoDlg(FALSE))
					{
						m_pTipDlg->SetTitle(L"彩信发送成功!", 5000);
						m_pTipDlg->ShowWindow_(TRUE);
					}
					
					sendOk += number_;
					sendOk += ";";
				}
				else
				{
					char txt[64];
					char* ResponseStatus[] = {"", "未知错误", "服务拒绝", "消息格式错误", "发送地址错误", "消息未发现", "网络错误", "消息内容错误", "不支持的消息"};
					if(ret > 0 && ret < 8)
						sprintf(txt, "彩信发送失败", ResponseStatus[ret]);
					else
						strcpy(txt, "彩信发送失败");
					CString s = txt;
					if(!FindVideoDlg(FALSE))
					{
						m_pTipDlg->SetTitle(s,5000);
						m_pTipDlg->ShowWindow_(TRUE);
					}

					sendError += number_;
					sendError += ";";
				}
				Sleep(50);
			}
			else
			{
				if(sendError != "" && sendOk != "")
				{
					boost::shared_ptr<Data::MMSData> errorData = boost::shared_ptr<Data::MMSData>(new Data::MMSData);
					*errorData = *mmsSendResult[i];
					mmsSendResult[i]->type = Data::MMSData::tpSend;
					mmsSendResult[i]->RecipientAddress = sendOk;
					mmsSendResult[i]->isRead = true;
					errorData->RecipientAddress = sendError;
					errorData->isRead = false;
					errorData->type = Data::MMSData::tpSend;
					errorData->Insert();
				}
				else if(sendError != "")
				{
					mmsSendResult[i]->type = Data::MMSData::tpSend;
					mmsSendResult[i]->isRead = false;//表示发送失败
				}
				else if(sendOk != "")
				{
					mmsSendResult[i]->type = Data::MMSData::tpSend;
					mmsSendResult[i]->isRead = true; //表示发送成功
				}

				mmsSendResult[i]->Update();
			}

			if(flag)
			{
				if(!m_pMessageSet->SaveSendMessage())
				{
					CString path = mmsSendResult[i]->SavePath.c_str();
					if(DetectDIR((LPTSTR)(LPCTSTR)path))
					{
						DeleteDirectory(path);
					}
					mmsSendResult[i]->Remove();
				}
				break;
			}
		}while(true);
	}
	//下载彩信
	extern BOOL IsTestCTA();
	if (!IsTestCTA())
	{
		std::vector<boost::shared_ptr<Data::MmsMessage> > result_ = Data::MmsMessage::GetFromDatabase(""); 
		for(i = 0; i < result_.size(); i++)
		{
			if(result_[i]->state == Data::MmsMessage::stUnDownload)
			{
				ULONGLONG m ;
				ULONGLONG f ;
				ULONGLONG nleft ;
				GetDiskStatus(m,f,nleft);
				if(nleft == 0 )
				{
					int static tipCount = 0;
					if(tipCount > 0)
					{
						Sleep(20*1000);
					}
					else
					{
						tipCount = 1;
					}
					FindVideoDlg(TRUE);
					m_pWarningNoFlashDlg->SetTitle(L"空间不足!无法接收彩信");
					m_pWarningNoFlashDlg->ShowWindow_(TRUE);
					return;
				}

				if(m_isDialTimeout >= 2)
				{
					FindVideoDlg(TRUE);
					m_pWarningNoFlashDlg->SetTitle(L"拨号连接超时!无法接收彩信");
					m_pWarningNoFlashDlg->ShowWindow_(TRUE);
					result_[i]->state = Data::MmsMessage::stDownloadedError;
					result_[i]->Update();
					m_bIsDial = FALSE;
					break;
				}

				SYSTEMTIME tm;
				GetLocalTime(&tm);
				CTime ct = CTime(tm);
				unsigned int year = result_[i]->localReciveTime.year+2000;
				unsigned int month = result_[i]->localReciveTime.month;
				unsigned int day = result_[i]->localReciveTime.day;
				unsigned int hour = result_[i]->localReciveTime.hour;
				unsigned int minite = result_[i]->localReciveTime.minite;
				unsigned int second = result_[i]->localReciveTime.second;
				CTime st = CTime(year, month, day, hour, minite, second);
				CTimeSpan sp;
				if(ct >= st)
				{
					sp = ct - st;
				}
				else
				{
					sp = st - ct;
				}
				unsigned int time = sp.GetTotalSeconds();
				if((result_[i]->timeOfExpiry != 0) && (time >= result_[i]->timeOfExpiry) )  //查看彩信是否过期
				{
					FindVideoDlg(TRUE);
					m_pWarningNoFlashDlg->SetTitle(L"本条彩信已过期,无法接收");
					m_pWarningNoFlashDlg->ShowWindow_(TRUE);
					result_[i]->Remove();
					continue;
				}

				if(!GetIs3GDialed())
				{
					m_bIsDial = TRUE;
					return;
				}

				flag = TRUE;
				HttpParam *pHttpParam = new HttpParam;
				pHttpParam->type = 0;			
				pHttpParam->dwType = 10;		//彩信下载

				char sTemp[512] = {0};
				char *pUrl = (char *)result_[i]->url.c_str();
				char *sId = (char *)result_[i]->transactionId.c_str();
				if(!strstr(pUrl, sId))
				{
					int index = 0;
					for(int i_1 = 0; i_1 < strlen((char *)result_[i]->url.c_str()); i_1++)
					{
						if(*pUrl == '/')
							index = i_1;
						pUrl++;
					}
					strncpy(sTemp, (char *)result_[i]->url.c_str(), index+1);
					strcat(sTemp, (char *)result_[i]->transactionId.c_str());
					pHttpParam->url = sTemp;
				}
				else
				{
					pHttpParam->url = (char *)result_[i]->url.c_str();
				}			
				pHttpParam->apn_type = m_nAPN.apn_type;
				pHttpParam->apn_proxy = m_nAPN.proxy;

				
				if(result_[i]->downloadCount == 1)
				{
					if(!FindVideoDlg(FALSE))
					{
						m_pDeleteTipDlg->SetTitle(L"正在重新下载彩信", 5000);
						m_pDeleteTipDlg->ShowWindow_(TRUE);
					}
				}

				int index;
				Dprintf("彩信下载 \r\n");
				WriteMyLog_("彩信下载",strlen("彩信下载"));
				if((index = HttpProcesse(pHttpParam)) >= 0)
				{
					g_iSmsID = ((index<<1)+1);

					//add by qi 0713
					extern VOID WriteMyLog_(char *ptr, int size);
					char log[50];
					sprintf(log,"Recive_mmsID:%d,index:%d",g_iSmsID,index);
					WriteMyLog_(log,strlen(log));

					result_[i]->state = Data::MmsMessage::stParsed;
					result_[i]->Update();
					if(!FindVideoDlg(FALSE))
					{
						m_pDeleteTipDlg->SetTitle(L"彩信接收成功!", 5000);
						m_pDeleteTipDlg->SetHWnd(m_hWnd);
						m_pDeleteTipDlg->ShowWindow_(TRUE);
					}
					
					if(g_bSendReport)
					{
						//向服务器发送消息，已经收到彩信
						char *buf = NULL;
						if(buf == NULL)
							buf = new char[1024];
						
						memset(buf, 0, 1024);
						int len = 0;
						len = MMS::MMSWarp::GetMMSWarp()->EncodeNotificationResponse(result_[i]->transactionId, 0x81, buf);
						
						HttpParam *pHttpParam = new HttpParam;
						pHttpParam->type = 11;			//发彩信接受成功
						pHttpParam->apn_type = m_nAPN.apn_type;
						pHttpParam->apn_proxy = m_nAPN.proxy;
						pHttpParam->pData = buf;
						pHttpParam->dataLentg = len;
						pHttpParam->url = m_nAPN.http; 
						
						Dprintf("彩信回执\r\n");
						WriteMyLog_("彩信回执",strlen("彩信回执"));
						int ret = HttpProcesse(pHttpParam);
					}
					
					//刷新下彩信列表
					m_pSMSListDlg->RefreshList(MMS_TYPE);

				}
				else
				{	
					Dprintf("接受失败\r\n");
					WriteMyLog_("接受失败",strlen("接受失败"));
					if(!FindVideoDlg(FALSE))
					{
						m_pTipDlg->SetTitle(L"彩信接收失败!", 10000);
						m_pTipDlg->ShowWindow_(TRUE);
					}
					result_[i]->state = Data::MmsMessage::stDownloadedError;
					result_[i]->Update();
				}

			}

			if(result_[i]->state == Data::MmsMessage::stDownloadedError)
			{
				if(result_[i]->downloadCount < 2)
				{
					result_[i]->state = Data::MmsMessage::stUnDownload;
					result_[i]->downloadCount++;
					Sleep(10000);
				}
				else 
				{
					result_[i]->state = Data::MmsMessage::stRepeatDownload;
					result_[i]->repeatCount++;
				}
				result_[i]->Update();
			}

			if(result_[i]->state == Data::MmsMessage::stParsed)
			{
				result_[i]->Remove();
			}
		}
	}

	if(m_nAPN.apn_type == m_pSettingDlg->m_pSetting->mmsType_ && flag)//判断是否需要永远连接
	{
		Net3GHungOff();
		Sleep(3000);
//		SetAPN((int)CMWAP);
		SetAPN(m_pSettingDlg->m_pSetting->mmsType_);

	}
}

/************************************************************************/
/*						add by qi down                                   */
/************************************************************************/
void CMultimediaPhoneDlg::SaveDataToContact(const std::vector<Util::ATCommandWarp::SIM_FORMAT> vsim)
{
	boost::shared_ptr<Data::Contact> pContact;
	pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
	for (int i = 0; i < vsim.size() ;i++)
	{
		pContact->name(vsim[i].name);
		pContact->alias(GetAlias(vsim[i].name));//别名
		pContact->mobilesTelephone(Data::TelephoneNumber(vsim[i].telnum));
		pContact->groupId(0);
		pContact->Insert();
	}

}
void CMultimediaPhoneDlg::GetDataFromContact(std::vector<Util::ATCommandWarp::SIM_FORMAT> &vsim)
{ 
	std::vector< boost::shared_ptr<Data::Contact> > vfromcontact;
 	if (Data::Contact::GetDataCount("") > 0)
 	{
		vfromcontact = Data::Contact::GetFromDatabase("", Data::dDown, Data::Contact::GetCurrentId() + 1);
		int a = vfromcontact.size();
		for (int i = 0 ; i < vfromcontact.size() ;i++)
		{   
			Util::ATCommandWarp::SIM_FORMAT sim ;
			sim.name   =  vfromcontact[i]->name();//名字
			sim.telnum = /*(Data::TelephoneNumber)*/(vfromcontact[i]->mobilesTelephone()).number();//电话
			vsim.push_back(sim);
		}
 	}
} 

void CMultimediaPhoneDlg::InsertSimToContact(const std::vector<Util::ATCommandWarp::SIM_FORMAT> vsim)
{	
		
	//把SIM 信息插入到 Contact-sim卡组里
	for (int i = 0 ; i < vsim.size() ; i++)
	{
		boost::shared_ptr<Data::Contact> pContact;
		pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
		pContact->name(vsim[i].name);
		
		//std::string name = vsim[i].name ;

		pContact->alias(GetAlias(pContact->name()));
		pContact->mobilesTelephone(Data::TelephoneNumber(vsim[i].telnum));	

		pContact->worksTelephone(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
		pContact->mobilesTelephone2(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
		pContact->worksTelephone2(Data::TelephoneNumber(Util::StringOp::FromCString(L"")));
		pContact->email(Util::StringOp::FromCString(L""));
		pContact->homesite(Util::StringOp::FromCString(L""));
		pContact->company(Util::StringOp::FromCString(L""));
		pContact->companyAddress(Util::StringOp::FromCString(L""));
		pContact->companyPostCode(Util::StringOp::FromCString(L""));
		pContact->department(Util::StringOp::FromCString(L""));
		pContact->duty(Util::StringOp::FromCString(L""));
		pContact->memo(vsim[i].id);
		pContact->type(Data::ctNormal);
		pContact->groupId(m_nSIMID);
		pContact->Insert();

		Sleep(10);
				
	}

}

void CMultimediaPhoneDlg::RemoveSIMGroup()
{
	//SIM组里有信息，先删除掉
	m_nSIMID = -1;
	std::vector<boost::shared_ptr<Data::ContactGroup> > result;
	if (Data::ContactGroup::GetDataCount("") > 0)//查找
	{
		result = Data::ContactGroup::GetFromDatabase("");
		for (int i = 0; i < result.size(); ++i)
		{
			if( "SIM卡" == result[i]->name())
			{
				m_nSIMID = result[i]->id();
				m_pContactDlg->SetSimID(m_nSIMID);
				break;
			}
		}
	}
	
	if(m_nSIMID == -1)//新建
	{
		//创建SIM卡类别
		boost::shared_ptr<Data::ContactGroup> m_pContactGroup = boost::shared_ptr<Data::ContactGroup>(new Data::ContactGroup);
		m_pContactGroup->name("SIM卡");
		m_pContactGroup->ringName("默认");
		m_pContactGroup->tipName("默认");
		m_pContactGroup->Insert();
		m_nSIMID = m_pContactGroup->id();
		m_pContactDlg->SetSimID(m_nSIMID);
	}

	std::vector<boost::shared_ptr<Data::Contact> > result1 = Data::Contact::GetFromDatabase("groupId = " + Util::StringOp::FromInt(m_nSIMID));
	if (!result1.empty())
	{
		for(int i = 0; i < result1.size(); i++)
			result1[i]->Remove();
	}

}

void CMultimediaPhoneDlg::VK_F(WPARAM w)
{
	if (m_bSearchNetOver)
	{	
		std::vector<std::pair<std::string, std::string> > m = m_pSettingDlg->m_pSetting->speedDials();
		if (m[w-1].second != "" /*&& m_phoneLine[m_nline].pFSM->getCurrentState() == p3gsHangOn*/)
		{
			m_pMainDlg->StopVideo(TRUE, 0, 0);
			m_pTelephoneDlg->DialContact(Data::TelephoneNumber(m[w-1].second));
 		}
		else
		{
			m_pMainDlg->StopVideo(FALSE, 0, 0);

			m_pSettingDlg->OnClickMJPG(126,0);
			m_pSettingDlg->ShowWindow(SW_SHOW);
			AddIcon(Allicon[9],m_pSettingDlg,false);
		}
		AddDesktopBtn();
	}
}

void CMultimediaPhoneDlg::AddAudio(bool badd)
{	
	HWND hWnd1 = ::FindWindow(L"csplayer_win1", L"csplayer window1");
	HWND hWnd0 = ::FindWindow(L"csplayer_win0", L"csplayer window0");
	if ( m_pTelephoneDlg->IsWindowVisible() )
	{
		m_pTelephoneDlg->HandleAudio(badd);
	}
	else if(IsWindow(hWnd1) || m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic)
	{
		m_pMainDlg->m_mainMp3Dlg_->HandleAudio(badd);				
	}
	else if (IsWindow(hWnd0) && m_pMainDlg->m_mainVideoDlg_->IsWindowVisible())
	{
		m_pMainDlg->m_mainVideoDlg_->HandleAudio(badd);
	}
	
}

std::string CMultimediaPhoneDlg::GetAlias(std::string name)
{
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex == NULL)
	{
		return "";
	}
	
	std::string alias;
	for (int i = 0; i < name.size(); ++i)
	{
		if ((unsigned char)name[i] > (unsigned char)127)
		{
			unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i+1] - 0xa1);
			
			if(index >= 0 && index < 6768)
			{
				alias += m_pContactNewDlg->m_pPinYinIndex[index];	
			}
			else
			{
			}
			++i;
		}
		else
		{
			if (name[i] >= 'A' && name[i] <= 'Z')
			{
				alias += char(name[i] + 32);
			}
			else
			{
				alias += name[i];
			}
		}
	}
	return alias;
}

std::string CMultimediaPhoneDlg::GetName(std::string number)
{
	
	if (number == "" )
	{
		return "";
	}
	
	if ( number.length() > 6)
	{
		if ( 0 == (number.substr(0,2)).compare("86"))
		{
			number = number.substr(2);
		}
		else if ( 0 == (number.substr(0,3)).compare("+86"))
		{
			number = number.substr(3);
		}
	}

	std::string filter;
	if (number.length() < 7)
	{
		filter += "mobilesTelephone = '";
		filter += (number);
		filter += "'";
		filter += " OR mobilesTelephone2 = '";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone = '";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone2 = '";
		filter += (number);
		filter += "'";
	}
	else
	{
		filter += "mobilesTelephone LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR mobilesTelephone2 LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone LIKE '%";
		filter += (number);
		filter += "'";
		filter += " OR worksTelephone2 LIKE '%";
		filter += (number);
		filter += "'";
	}
	
//	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabaseByTypeOffsetLength(filter,0,1);
	if (!result.empty())
	{
		return result[0]->name();
	}
	return "";

	/*if (number == "" || number.size() < 8)
	{
		return "";
	}
	
	if ( 0 == (number.substr(0,2)).compare("86"))
	{
		number = number.substr(2);
	}
	else if ( 0 == (number.substr(0,3)).compare("+86"))
	{
		number = number.substr(3);
	}


	std::string filter;

	filter = "mobilesTelephone Like '%";
	filter += Data::TelephoneNumber(number).ToString();
	filter += "' OR mobilesTelephone2 Like '%";
	filter += Data::TelephoneNumber(number).ToString();
	filter += "' OR worksTelephone Like '%";
	filter += Data::TelephoneNumber(number).ToString();
	filter += "' OR worksTelephone2 Like '%";
	filter += Data::TelephoneNumber(number).ToString();
	filter += "'";
	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
	if (!result.empty())
	{
		return result[0]->name();
	}
	return "";*/


}

void CMultimediaPhoneDlg::AddIcon(CString icon,CWnd *pcwnd,bool bjudge )
{	
	if (!bjudge)
	{	
// 		CString Ticon = m_vIcon[m_vIcon.size() -1] ;
// 		if ( 0 == Ticon.Compare(icon))
// 		{
// 			return ;
// 		}
		
		std::vector<std::pair<CWnd*,CString> >::iterator it = m_vIcon.begin();

		for ( it ; it < m_vIcon.end();it++)
		{
			if ((*it).first == pcwnd)//相同的窗口，去掉该窗口图标
			{	
				m_vIcon.erase(it);
				break;
			}
		}
	}
	std::pair<CWnd*,CString> p(pcwnd,icon) ;
	m_vIcon.push_back(p);//链接图标	
	m_MJPGList.SetUnitBitmap(0,icon,L"",true);
	
}

void CMultimediaPhoneDlg::Desktop()
{
	//停止试听音乐
	m_pSettingDlg->StopTryRing();
	m_pContactGroupDlg->StopTryRing();
	m_pMainDlg->m_mainLunarderDlg_->StopTryRing();
	
	if (m_pMainDlg->m_p3GHomeMovieDlg->IsWindowVisible())
	{
		m_pMainDlg->m_p3GHomeMovieDlg->OnExit_();
	}

	if (m_pMainDlg->m_p3GHomePicDlg->IsWindowVisible())
	{
		m_pMainDlg->m_p3GHomePicDlg->OnExit_();
	}

	if (m_pMainDlg->m_p3GHomeJoyDlg->IsWindowVisible())
	{
		m_pMainDlg->m_p3GHomeJoyDlg->OnExit_();
	}
	
	if (playervideo_->isPlaying_)
	{	
		m_pMainDlg->m_p3GHomeMovieDlg->OnExit_();
		m_pMainDlg->m_mainVideoDlg_->OnExit_();
	}
	if (playerimage_->isPlaying_)
	{
		m_pMainDlg->m_p3GHomePicDlg->OnExit_();
		m_pMainDlg->m_mainPhotoDlg_->OnExit_();
	}
	if(playeraudio_->isPlaying_ || m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic)
	{
		m_pMainDlg->m_p3GHomeJoyDlg->OnExit_();
		m_pMainDlg->m_mainMp3Dlg_->OnExit();
	}
	
	if(m_pSMSListDlg->m_pMmsReadDlg->m_isReadMid)
	{
		m_pMMSDlg->StopAudio();
		m_pSMSListDlg->m_pMmsReadDlg->m_isReadMid = FALSE;
	}

	if (m_vIcon.size())
	{
		m_vIcon.clear();
	}

	CString icon = L".\\adv\\mjpg\\k5\\common\\桌面图标.bmp";
	AddIcon(icon,m_pMainDlg,true);

	//把短息里的内容保存到草稿箱
	if (m_pMainDlg->m_p3GSMSDlg->IsWindowVisible())
	{
		m_pMainDlg->m_p3GSMSDlg->OnClickMJPG(1000,0);
	}
	
	//把彩信里的内容保存到草稿箱
	if (m_pMMSDlg->IsWindowVisible())
	{
		m_pMMSDlg->OnClickMJPG(1000,0);	
	}
	
	//电话的处理
	if (m_pTelphoneDialDlg->IsWindowVisible())
	{
		m_pTelephoneDlg->OnClickMJPG(1000,0);
	}

	if (m_pTelephoneDlg->IsWindowVisible())//电话隐藏
	{
		m_pTelephoneDlg->OnButtonTelephoneHide();
	}

	for (int i = 0 ; i < m_vAllCwnd.size() ;i++)
	{
		if (m_vAllCwnd[i])
		{   			
			m_vAllCwnd[i]->ShowWindow(SW_HIDE);		
		}
	}

	m_pMainDlg->HideAllWindow();
//	m_pMainDlg->ShowWindow(SW_SHOW);

	//判断这个时候电话的状态

	/*if (m_phoneLine[0].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsDialing ||
		m_phoneLine[1].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsDialing ||
		m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected)
	{
		m_MJPGList.SetUnitIsShow(15,true,true);
	}*/

	m_MJPGList.SetUnitIsShow(10,false,true);
	m_MJPGList.Invalidate();

}

bool CMultimediaPhoneDlg::GetSimStatus()
{
	return m_bInsertSim ;
}


void CMultimediaPhoneDlg::g3HangOn(void* param)
{	
// 	::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);
// 	::SetTimer((theApp.m_pMainWnd)->m_hWnd, 0x124, 50, NULL);//摘机

	// change by qi 20100519
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(NULL);

}

void CMultimediaPhoneDlg::g3HangOff(void* param)
{	
	//int type = (int)param;
//	if ( 1 == (int)param )//只处理3G相关的数据
//	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pTelephoneDlg->G3HangOff((int)param);
		
		//pstn也在振铃
		if (main->m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsRing)
		{
			if (main->m_pTelphoneRingDlg->IsWindowVisible())
			{
				main->PopIcon(main->m_pTelphoneRingDlg);

			//	main->m_pTelphoneRingDlg->ShowWindow_(SW_HIDE);
			//	main->PopbackIcon();
			}
		}
		main->m_pTelephoneDlg->SwitchAnother3G();
//	}
//	else
//	{
//		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->OnTimer(0x123);	
//	}

}

void CMultimediaPhoneDlg::g3Key(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer, KEYTIMEOUT, 0);	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelphoneDialDlg->Key_(param);	
}

void CMultimediaPhoneDlg::g3Dialing(void* param)
{
		
}

void CMultimediaPhoneDlg::g3Ring(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);
	
// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->
// 		m_phoneLine[2].pFSM->setStartState(pstns3gRing);

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Ring_(param);
	
}

void CMultimediaPhoneDlg::g3CallID(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->CallID_(param);	
}

void CMultimediaPhoneDlg::g3Contect(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Connect_(param);

}

void CMultimediaPhoneDlg::g3Contected(void* param)
{

}

void CMultimediaPhoneDlg::g3SubDial(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->SubKey_(param);
}

void CMultimediaPhoneDlg::g3Active(void* param)
{
	
}

void CMultimediaPhoneDlg::g3Hold(void* param)
{
	
}

void CMultimediaPhoneDlg::pstn3gRing(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->
		m_phoneLine[2].pFSM->setStartState(pstns3gRing);
}

void CMultimediaPhoneDlg::pstnRing(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiPSTNRingTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiPSTNRingTimer, RINGTIMEOUT, 0);
	
// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->
// 		m_phoneLine[0].pFSM->setStartState(p3gsPstn);

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->pstnRing_(param);
	
}

void CMultimediaPhoneDlg::pstnHangOff(void* param)
{	
	Dprintf("PSTN HangOff \n");

	if ( 3 == (int)param )//只处理pstn的功能
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->PstnHangOff_(NULL);
	}
	//else
	//{
	//	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->PstnHangOff_(NULL);
	//	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(NULL);
	//}

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SendOutEvnet(WM_TEL_HUNGOFF, 1);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_bIsHungOn = FALSE;

}

void CMultimediaPhoneDlg::pstnContect(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->PstnContect_(param);

}

void CMultimediaPhoneDlg::pstnHold(void* param)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->
		m_phoneLine[2].pFSM->setStartState(pstnsHold);
}

void CMultimediaPhoneDlg::pstnCallID(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiPSTNRingTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiPSTNRingTimer, RINGTIMEOUT, 0);	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->pstnCallID_(param);

}

void CMultimediaPhoneDlg::HandleHungOn(WPARAM wParam,LPARAM lParam)
{
	if (!OpenAllPort())
	{
		return ;
	}
	
	ReStoreBackLight();

	extern VOID WriteMyLog_(char *ptr, int size);
	WriteMyLog_("nHandleHungOn",strlen("nHandleHungOn"));			

	Dprintf("\nHandleHungOn :%d\n",wParam);
	m_bIsHungOn = TRUE;
	
	if (m_phoneLine[0].pFSM->getCurrentState() == p3gsRing &&
		m_phoneLine[1].pFSM->getCurrentState() == p3gsHangOff &&
		m_phoneLine[2].pFSM->getCurrentState() == pstnsRing)//3g_1 Ring,3g_2 挂断状态 and pstn Ring
	{
		if ( 1 == lParam)//接3G
		{
			Telephone::TelephoneWarp::GetTelephoneWarp()->PSTNHangoff();
			phone_->SWitchPhone3G(TRUE);
			m_phoneLine[wParam-1].pFSM->fireEvent(p3geHangOn,((void * const)(wParam-1)));
		}
		else//接PSTN
		{
			phone_->SWitchPhone3G(FALSE);
			phone_->PSTNHangOn();
	//		m_phoneLine[2].pFSM->fireEvent(pstneHangOn,NULL);
		}

	}
	else if (m_phoneLine[m_nline].pFSM->getCurrentState() == p3gsConnected && 
		m_phoneLine[(m_nline+1)%2].pFSM->getCurrentState() == p3gsHangOff && 
		m_phoneLine[2].pFSM->getCurrentState() == pstnsRing)//3g_1 contected,3g_2 Hangoff,Pstn Ring
	{	
			phone_->SWitchPhone3G(FALSE);
	//		m_phoneLine[2].pFSM->fireEvent(pstneHangOn,NULL);
	}
	else if (m_phoneLine[0].pFSM->getCurrentState() == p3gsHangOff&& 
		m_phoneLine[1].pFSM->getCurrentState() == p3gsHangOff&&
		m_phoneLine[2].pFSM->getCurrentState() == pstnsRing)//pstn 接听
	{
		phone_->SWitchPhone3G(FALSE);
		if ( 2 == lParam)//手柄摘机
		{
			m_phoneLine[2].pFSM->fireEvent(pstneHangOn,((void * const)wParam));
		} 
		else 
		{
			phone_->PSTNHangOn();
		}
		//change by qi 20100831
		//m_phoneLine[2].pFSM->fireEvent(pstneHangOn,((void * const)wParam));

	}
	else if (m_phoneLine[0].pFSM->getCurrentState() == p3gsHangOff && 
		m_phoneLine[1].pFSM->getCurrentState() == p3gsHangOff && 
		m_phoneLine[2].pFSM->getCurrentState() == pstnsHangOff)//3g 摘机拨打
	{	
		
		Telephone::TelephoneWarp::GetTelephoneWarp()->PSTNHangoff();
		phone_->SWitchPhone3G(TRUE);

		char log[10];
		sprintf(log,"wParam:%d",wParam);
		WriteMyLog_(log,strlen(log));

		m_phoneLine[0].pFSM->fireEvent(p3geHangOn,((void * const)(wParam-1)));

		WriteMyLog_("Teldial",strlen("Teldial"));

	}
	else if (m_phoneLine[wParam-1].pFSM->getCurrentState() == p3gsRing && 
		m_phoneLine[wParam%2].pFSM->getCurrentState() == p3gsHangOff && 
		m_phoneLine[2].pFSM->getCurrentState() == pstnsHangOff)//3g 接听
	{	
		Telephone::TelephoneWarp::GetTelephoneWarp()->PSTNHangoff();
		phone_->SWitchPhone3G(TRUE);
		m_phoneLine[wParam-1].pFSM->fireEvent(p3geHangOn,((void * const)(wParam-1)));		
	}

}

void CMultimediaPhoneDlg::HandleCallIDEnd(WPARAM wParam ,LPARAM lParam)
{	
	Dprintf("CALL ID \n");
	if (lParam == 3)
	{
		m_phoneLine[(int)lParam-1].pFSM->fireEvent(pstneCallID,(void * const)wParam);
	}
	else
	{
		m_phoneLine[(int)lParam-1].pFSM->fireEvent(p3geCallID,(void * const)wParam);
	}
	
}

void CMultimediaPhoneDlg::HandleRing(WPARAM wParam ,LPARAM lParam)
{
	//停止试听音乐
	m_pSettingDlg->StopTryRing();
	m_pContactGroupDlg->StopTryRing();
	m_pMainDlg->m_mainLunarderDlg_->StopTryRing();	
	SendOutEvnet(WM_TEL_RING, 0);
	Dprintf("RING \n");
	if ( 3 == lParam )
	{
		m_phoneLine[(int)lParam-1].pFSM->fireEvent(pstneRing,(void * const)(lParam-1));
	}
	else
	{
		m_phoneLine[(int)lParam-1].pFSM->fireEvent(p3geRing,(void * const)(lParam-1));
	}

}

void CMultimediaPhoneDlg::HandleHungOff(WPARAM wParam ,LPARAM lParam)
{	
	// add 20100419
	
	WriteMyLog_("Handle HUNG Begin",strlen("Handle HUNG Begin"));

	if (!OpenAllPort())
	{
		return ;
	}

	Dprintf("Handle HUNG OFF \r\n");
	WriteMyLog_("Handle HUNG OFF",strlen("Handle HUNG OFF"));

	if ( 3 != lParam && lParam > 0  )//3g off
	{
		m_phoneLine[lParam-1].pFSM->fireEvent(p3geHangOff,(void * const)(lParam-1));
	}
	else if ( 3 == lParam ) //pstn off
	{
		m_phoneLine[2].pFSM->fireEvent(pstneHangOff,(void * const)(lParam-1));//
	}
 	else// 0 == lParam 全部挂断
 	{	
		//pstn不在振铃
		if (m_phoneLine[2].pFSM->getCurrentState() != CMultimediaPhoneDlg::pstnsRing)
		{	
			Dprintf("pstn_Ring\r\n");
			int line ;
			if (m_phoneLine[2].pFSM->getCurrentState() != pstnsHangOff)
			{
				int p = 3;
				m_phoneLine[2].pFSM->fireEvent(pstneHangOff,(void *)p);
			}
			
			if (m_phoneLine[0].pFSM->getCurrentState() != p3gsHangOff || 
				m_phoneLine[1].pFSM->getCurrentState() != p3gsHangOff)
			{
				m_pTelephoneDlg->G3AllHangOff();
				m_pTelephoneDlg->CommonDataInit();
			}
		}
		else
		{
			Dprintf("pstn_noRing\r\n");
			m_pTelephoneDlg->G3AllHangOff();
			phone_->Hangup();
			m_pTelephoneDlg->ActivePstn();	
		}

// 		line = 1;
// 		if (m_phoneLine[1].pFSM->getCurrentState() != p3gsHangOff)
// 		{
// 			m_phoneLine[1].pFSM->fireEvent(p3geHangOff,(void*)line);
// 		}
// 		
// 		line = 0;
// 		if (m_phoneLine[0].pFSM->getCurrentState() != p3gsHangOff)
// 		{
// 			m_phoneLine[0].pFSM->fireEvent(p3geHangOff,(void*)line);
// 		}

 	}

	SendOutEvnet(WM_TEL_HUNGOFF, 1);
	m_bIsHungOn = FALSE;

}

void CMultimediaPhoneDlg::AddDesktopBtn()
{	
	CString desktop_fore = L".\\adv\\mjpg\\k5\\common\\桌面按钮_fore.png";
	CString desktop_back = L".\\adv\\mjpg\\k5\\common\\桌面按钮_back.png";
	m_MJPGList.SetUnitBitmap(10,desktop_fore,desktop_back,false);
	m_MJPGList.SetUnitIsShow(10,true,true);
}

bool CMultimediaPhoneDlg::FindIdleLine(void)
{	
	int line = -1 ;
	for (int i = 0 ; i < 2; i++)
	{
		if (m_phoneLine[i].pFSM->getCurrentState() == p3gsHangOff)
		{
			line =  i ;//可用线路
			break;
		}
	}
	
	if ( -1 == line)//没有线路可用
	{
		m_pDeleteTipDlg->SetTitle(L"线路忙.......",3000);
		m_pDeleteTipDlg->ShowWindow(true);
		return false ;
	}
	else
	{
		m_nline = line;
	}
	return true;

}

bool CMultimediaPhoneDlg::FindActiveLine()
{
	bool bactive = false;
	for (int i = 0 ; i < 2 ;i++)
	{
		if (m_phoneLine[i].pFSM->getCurrentState() >= p3gsDialing)
		{
			bactive = true;
		}
	}
	
	if (m_phoneLine[2].pFSM->getCurrentState() != pstnsHangOff)
	{
		bactive = true;
	}
	
	return bactive ;

}

void CMultimediaPhoneDlg::Find3gLineNum(int &g3Num)
{	
	int num = 0  ;
	for (int i = 0 ; i < 2 ;i++)
	{
		if (m_phoneLine[i].pFSM->getCurrentState() != p3gsHangOff)
		{
			num++;
		}
	}
	g3Num = num;
}

void CMultimediaPhoneDlg::Swtich2AnotherLine()
{
	for (int i = 0 ; i < 2 ; i++)
	{	
		if ( i != m_nline)
		{
			if (m_phoneLine[i].pFSM->getCurrentState() != p3gsHangOff)
			{
				m_nline = i;
			}
		}	
	}

// 	if (m_phoneLine[2].pFSM->getCurrentState() != pstnsHangOff)
// 	{
// 		m_nline = 2;
// 	}

}

bool CMultimediaPhoneDlg::PstnActive()
{
	if ( m_phoneLine[2].pFSM->getCurrentState() != pstnsHangOff)
	{
		m_pWarningNoFlashDlg->SetTitle(L"PSTN正在通话!");
		m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return true;
	}
	return false;
}


void CMultimediaPhoneDlg::ReleasesOneLine(int const line)
{
	m_pATCommandWarp1->CHLD(line);
}

void CMultimediaPhoneDlg::RemoveSimSms()
{
	std::string filter ;
	filter = "[reference] = 1";
	Data::Message::Remove(filter);
}

void CMultimediaPhoneDlg::ReadSimSMS()
{
	m_pATCommandWarp1->SmsReadFromSim(0);
	m_pATCommandWarp1->SmsReadFromSim(1);
	m_pATCommandWarp1->SmsReadFromSim(2);
	m_pATCommandWarp1->SmsReadFromSim(3);
}

bool CMultimediaPhoneDlg::OpenAllPort()
{
	if (!m_bATComm ||!(m_pATCommandWarp1->GetInstance())|| !(phone_->m_bOpen1Port))
	{
		return false;
	}	
	return true;
}

CWnd *CMultimediaPhoneDlg::GetIconCurrentWnd()
{
	if (m_vIcon.size())
	{
		CWnd* pWnd = m_vIcon[m_vIcon.size()-1].first;
		if(!pWnd)
			pWnd = m_pMainDlg;
		return pWnd;
	}

}

void CMultimediaPhoneDlg::PopbackIcon(CWnd *p)
{
	CString icon;
	bool bfresh = false;
// 	m_vIcon.pop_back();
// 	if ( 0 == m_vIcon.size())
// 	{
// 		CString icon = L".\\adv\\mjpg\\k5\\common\\桌面图标.bmp";
// 		AddIcon(icon,m_pMainDlg,true);	
// 	}
// 	else
	{	

		if (p != NULL)
		{
			std::vector<std::pair<CWnd*,CString> >::iterator it = m_vIcon.begin();
			for ( it ; it < m_vIcon.end();it++)
			{
				if ((*it).first == p)//相同的窗口，去掉该窗口图标
				{	
					m_vIcon.erase(it);
					break;
				}
			}
		}
		else
		{
			m_vIcon.pop_back();
		}
		
		
		if ( 0 == m_vIcon.size())
		{
		 	CString icon = L".\\adv\\mjpg\\k5\\common\\桌面图标.bmp";
		 	AddIcon(icon,m_pMainDlg,true);	
 		}
				
		icon = m_vIcon[m_vIcon.size()-1].second;
		m_MJPGList.SetUnitBitmap(0,icon,L"",bfresh);		

	}

	if ( 1 == m_vIcon.size())
	{	
		m_MJPGList.SetUnitBitmap(10,L"",L"",bfresh);
	//	m_MJPGList.SetUnitIsShow(10,false,true);
	}
	
	m_MJPGList.Invalidate(false);

}

void CMultimediaPhoneDlg::PopIcon(CWnd *c)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	c->ShowWindow(SW_HIDE);
	main->PopbackIcon(c);
}

void CMultimediaPhoneDlg::SelMMSDataBase()
{
	std::string fileter = "type = ";
	fileter += Util::StringOp::FromInt(Data::MMSData::tpReadySend);
	std::vector<boost::shared_ptr<Data::MMSData> > mmsSendResult = Data::MMSData::GetFromDatabase(fileter);
	fileter = "state = ";
	fileter += Util::StringOp::FromInt(Data::MmsMessage::stUnDownload);
	std::vector<boost::shared_ptr<Data::MmsMessage> > result = Data::MmsMessage::GetFromDatabase(fileter);
	
	if(mmsSendResult.size() > 0 || result.size() > 0)
	{
		SetEvent(g_hEvent);
	}
	else
	{
		ResetEvent(g_hEvent);
	}
}

bool CMultimediaPhoneDlg::IsDiskFull()
{
	//判断磁盘空间
	ULONGLONG m ;
	ULONGLONG f ;
	ULONGLONG nleft ;
	extern void GetDiskStatus(ULONGLONG &free,ULONGLONG &total,ULONGLONG &uleft);
	GetDiskStatus(m,f,nleft);//1012517216

	if(nleft == 0 )//101177344
	{
		m_pWarningNoFlashDlg->SetTitle("磁盘空间不足,请先删除!");
		m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return true;
	}
	return false ;
}


/************************************************************************/
/*						add by qi up                                     */
/************************************************************************/

void PickupFunc()
{
	Dprintf("\nPickupFunc\n");
}
void HandsfreeFunc()
{
	Dprintf("\nHandsfreeFunc\n");
}
void HangupFunc()
{
	Dprintf("\nHangupFunc\n");
}
void KeypadDialFunc(unsigned char const& keyCode)
{
	Dprintf("\nKeypadDialFunc=%d\n", keyCode);
}
void ConnectFunc()
{
	Dprintf("\nConnectFunc\n");
}
void RingInFunc(Data::TelephoneNumber number)
{
	Dprintf("\nRingInFunc=%s\n", number.number().c_str());
}
void RemoteHangupFunc()
{
	Dprintf("\nRemoteHangupFunc\n");
}
void TimeoutFunc()
{
	Dprintf("\nTimeoutFunc\n");
}
void ErrorFunc()
{
	Dprintf("\nErrorFunc\n");
}


