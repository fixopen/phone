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
#define  BMP_HEIGHT2         60
#define  BMP_WIDTH3			 600  //535
#define  BMP_HEIGHT3         420
#define  BMP_WIDTH4			 800
#define  BMP_HEIGHT4         480
#define  BMP_WIDTH5			 535
#define  BMP_HEIGHT5         420

#define G3_MODULOR		1
	
#define  KEYTIMEOUT  60 * 60 * 1000
#define  RINGTIMEOUT  6 * 1000
extern  BOOL DetectDIR(TCHAR *sDir);
BOOL g_MemoryLeak = FALSE;
BOOL g_IsRun = FALSE;
BOOL g_bAdjustPanel = FALSE;
CRITICAL_SECTION csCom;

extern int HttpProcesse(void *pParam);
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
#include   <imaging.h> 
IImagingFactory   *pImgFactory;

void InitImage()
{
	CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
//	CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory);
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
		DWORD   dwStart   =   GetTickCount(); 	

		CFile   file;   
		if(   !file.Open(szFileImage,  CFile::modeRead ) )   
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
		DWORD offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 1 %d\n", offset);
		dwStart   =   GetTickCount();
		
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

				//wprintf(szFileImage);
				//Dprintf( " All Draw \n");

				dwStart = GetTickCount();
				int index;;
				if(FindMemoryInfo(szFileImage, index))
				{
					if(index >= 0 && index < ALLBMPMEMORY_MAX)
					{
						BitBlt(pdc->m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);
						
						offset = GetTickCount() - dwStart;   
// 						if(flag)
// 							Dprintf("Draw ALL BMP %d\n", offset);

// 						unsigned int w1 = DMemprintf("DrawImage 1");
// 						if(w != w1)
// 							Dprintf("w != w1\r\n");

						file.Close();
						return;
					}
				}
			}
			dwStart = GetTickCount();
		
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

			offset = GetTickCount() - dwStart;   
// 			if(flag)
// 				Dprintf("Draw ALL BMP %d\n", offset);
			//TRACE(L"Draw ALL BMP %d\n", offset);
		
// 			unsigned int w1 = DMemprintf("DrawImage 1");
// 			if(w != w1)
// 				Dprintf("w != w1\r\n");
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
			WriteLog(restartTimer);

			file.Close();
			g_MemoryLeak = TRUE;
			delete []lpBitmap;    //20090216 test
			return;
		}

		offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 2 %d\n", offset);
		dwStart   =   GetTickCount();
	
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
		
		offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 3 %d\n", offset);
		dwStart   =   GetTickCount();

		CBitmap   BmpImg;   
		BmpImg.Attach(hBmp);   
		
		CBitmap  *pOldBmp=dcMem.SelectObject(&BmpImg);   
	
		offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 4 %d\n", offset);
		dwStart   =   GetTickCount(); 

    	//pdc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
		pdc->BitBlt(newRect.left, newRect.top, newRect.Width(), newRect.Height(), &dcMem, 0, 0, SRCCOPY);
	
		dcMem.SelectObject(pOldBmp); 
	
		DeleteObject(hBmp);

		BmpImg.Detach();
		dcMem.DeleteDC();
	
		delete []lpBitmap;    //20090216 test

		offset = GetTickCount() - dwStart;   
		//TRACE(L"end file DC %d\n", offset);
		::Sleep(1);
	}
	else
	{
		DWORD   dwStart   =   GetTickCount(); 	

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
		int offset = GetTickCount() - dwStart;  
	//	wprintf(szFileImage);
	//	Dprintf(" Draw ALL BMP %d\n", offset);
		//	CoUninitialize(); 

	}
// 	unsigned int w1 = DMemprintf("DrawImage 1");
// 	if(w != w1)
// 		Dprintf("w != w1\r\n");
}

void  DrawImage_HDC(LPCTSTR szFileImage, HDC pdc, CRect rect, BOOL isOffset = FALSE, VOID *lpParamBits = NULL) 
{ 
//	unsigned int w = DMemprintf("DrawImage 0");

	if(wcsstr(szFileImage,(L".png")) > 0)
	{
		DWORD   dwStart1   =   GetTickCount(); 
		CxImage *image_;
		image_ = new CxImage();
		if(image_)
		{
			//CDC *pdc = GetDC(); 
			bool ret = image_->Load(szFileImage, CXIMAGE_FORMAT_PNG);
			DWORD   dwStart2   =   GetTickCount(); 
			image_->Draw(pdc, rect);
			DWORD offset2 = GetTickCount() - dwStart2;   
			TRACE(L"draw png %d\n", offset2);
			//	ReleaseDC(pdc);
			delete image_;
		}
		DWORD offset1 = GetTickCount() - dwStart1;   
		TRACE(L"png %d\n", offset1);
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
		DWORD   dwStart   =   GetTickCount(); 	

		CFile   file;   
		if(   !file.Open(szFileImage,  CFile::modeRead ) )   
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
		DWORD offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 1 %d\n", offset);
		dwStart   =   GetTickCount();
		
		
		//	如果背景图是 800*420，692*296，535*420，800*60，Unit框小于他们，则取出相应的BMP画出来.否则整个画
		
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
			if(BMP_WIDTH == lpBitmap->bmiHeader.biWidth || BMP_WIDTH1 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH2 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH3 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH5 == lpBitmap->bmiHeader.biWidth)
			{
				flag = TRUE;

				int index;
				dwStart = GetTickCount();
				if(FindMemoryInfo(szFileImage, index))
				{
					if(index >= 0 && index < ALLBMPMEMORY_MAX)
					{
						BitBlt(pdc, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);
						
						offset = GetTickCount() - dwStart;   
						if(flag)
							Dprintf("Draw ALL BMP %d\n", offset);	
						// 						unsigned int w1 = DMemprintf("DrawImage 1");
						// 						if(w != w1)
						// 							Dprintf("w != w1\r\n");
						
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
					offset = GetTickCount() - dwStart;   
					
					if(flag)
						Dprintf("Draw ALL BMP %d\n", offset);
				//	::Sleep(1);
					return;
				}

				////////////////////////////////////////////////////////////////////////////
				DWORD dwStart1 = GetTickCount();
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

// 				offset = GetTickCount() - dwStart;   
// 				TRACE(L"file DC 2 %d\n", offset);
 				dwStart   =   GetTickCount();
			
				int off = (h-rect.top-rect.Height())*w + rect.left*(lpBitmap->bmiHeader.biBitCount/8);

				file.Seek(off, CFile::current);
				int size = (((rect.Width()*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;   //rect.Width()*3;
				int offline = w - size; 
				
				file.Read(lpBits, size*rect.Height());
				file.Close();       
				
 				offset = GetTickCount() - dwStart;   
 				TRACE(L"file DC 3 %d\n", offset);
 				dwStart   =   GetTickCount();

				HBITMAP pOldBmp= (HBITMAP)::SelectObject(dcMem, hBmp);      //20090723
				
 				offset = GetTickCount() - dwStart;   
 				TRACE(L"file DC 4 %d\n", offset);
 				dwStart   =   GetTickCount(); 

    			::BitBlt(pdc, newRect.left, newRect.top, newRect.Width(), newRect.Height(), dcMem, 0, 0, SRCCOPY); 
				if(flag)
					 AddALLBmpMemoryInfo(szFileImage, rect, pdc);//20090723
			
				offset = GetTickCount() - dwStart;   
				TRACE(L"file DC 5 %d\n", offset);
 				dwStart   =   GetTickCount();

				::SelectObject(dcMem, pOldBmp);				//20090723
				::DeleteObject(hBmp);
				::DeleteDC(dcMem);									//20090723
				
				delete []lpBitmap;    //20090216 test

				offset = GetTickCount() - dwStart1;   
 				
 				if(flag)
 					Dprintf("Draw ALL BMP %d\n", offset);
				::Sleep(1);
				return;
				///////////////////////////////////////////////////////////////////////////

				//wprintf(szFileImage);
				//Dprintf( " All Draw \n");
				dwStart = GetTickCount();
				if(FindMemoryInfo(szFileImage, index))
				{
					if(index >= 0 && index < ALLBMPMEMORY_MAX)
					{
						BitBlt(pdc, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);
						
						offset = GetTickCount() - dwStart;   
						if(flag)
							Dprintf("Draw ALL BMP %d\n", offset);
						
						// 						unsigned int w1 = DMemprintf("DrawImage 1");
						// 						if(w != w1)
						// 							Dprintf("w != w1\r\n");
						
						file.Close();
						return;
					}
				}
			}
			dwStart = GetTickCount();
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

			offset = GetTickCount() - dwStart;   
// 			if(flag)
// 				Dprintf("Draw ALL BMP %d\n", offset);
			//TRACE(L"Draw ALL BMP %d\n", offset);
		
// 			unsigned int w1 = DMemprintf("DrawImage 1");
// 			if(w != w1)
// 				Dprintf("w != w1\r\n");
			return;
		}
		//创建HBITMAP句柄   

		
		//重新根据rect 创建位图信息
		

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

		offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 2 %d\n", offset);
		dwStart   =   GetTickCount();
	
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
		
		offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 3 %d\n", offset);
		dwStart   =   GetTickCount();

		HBITMAP pOldBmp= (HBITMAP)::SelectObject(dcMem, hBmp);   
	
		offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 4 %d\n", offset);
		dwStart   =   GetTickCount(); 

    	//pdc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
		::BitBlt(pdc, newRect.left, newRect.top, newRect.Width(), newRect.Height(), dcMem, 0, 0, SRCCOPY);
	
		::SelectObject(dcMem, pOldBmp); 
	
		::DeleteObject(hBmp);
		::DeleteDC(dcMem);
	
		delete []lpBitmap;    //20090216 test

		offset = GetTickCount() - dwStart;   
		//TRACE(L"end file DC %d\n", offset);
		::Sleep(1);
	}
	else
	{
		DWORD   dwStart   =   GetTickCount(); 	

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
		int offset = GetTickCount() - dwStart;  
	//	wprintf(szFileImage);
	//	Dprintf(" Draw ALL BMP %d\n", offset);
		//	CoUninitialize(); 

	}
// 	unsigned int w1 = DMemprintf("DrawImage 1");
// 	if(w != w1)
// 		Dprintf("w != w1\r\n");
}

void  DrawImage_HDC(LPCTSTR szFileImage, CRect imageRect, HDC pdc, CRect hdcRect, VOID *lpParamBits = NULL) 
{ 
//	unsigned int w = DMemprintf("DrawImage 0");

	if(wcsstr(szFileImage,(L".png")) > 0)
	{
		DWORD   dwStart1   =   GetTickCount(); 
		CxImage *image_;
		image_ = new CxImage();
		if(image_)
		{
			//CDC *pdc = GetDC(); 
			bool ret = image_->Load(szFileImage, CXIMAGE_FORMAT_PNG);
			DWORD   dwStart2   =   GetTickCount(); 
			image_->Draw(pdc, hdcRect);
			DWORD offset2 = GetTickCount() - dwStart2;   
			TRACE(L"draw png %d\n", offset2);
			//	ReleaseDC(pdc);
			delete image_;
		}
		DWORD offset1 = GetTickCount() - dwStart1;   
		TRACE(L"png %d\n", offset1);
		return;
	}

	CString sFile = szFileImage;
	if(sFile.Find(L".bmp") != -1 || sFile.Find(L".BMP") != -1)
	{
		//读bmp文件头的数据  
		DWORD   dwStart   =   GetTickCount(); 	

		CFile   file;   
		if(   !file.Open(szFileImage,  CFile::modeRead ) )   
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
		if((*   (LPDWORD)(lpBitmap))!=sizeof(BITMAPINFOHEADER) || (lpBitmap->bmiHeader.biBitCount != 16))     
		{   
		//	::MessageBox(NULL,L"The   BMP   is   not   Windows   3.0   format!", L"warning",MB_OK);  
			file.Close();
			delete []lpBitmap;    //20090216 test
			return;   
		}     
		DWORD   dwBitlen=bfhHeader.bfSize   -   bfhHeader.bfOffBits;     
		LPVOID   lpBits;   
		DWORD offset = GetTickCount() - dwStart;   
		//TRACE(L"file DC 1 %d\n", offset);
		dwStart   =   GetTickCount();
		
		int w = (((lpBitmap->bmiHeader.biWidth*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;
		int h = lpBitmap->bmiHeader.biHeight;
		lpBitmap->bmiHeader.biHeight = imageRect.Height();
		lpBitmap->bmiHeader.biWidth = imageRect.Width();
		//需要内部准备一个memdc，再画到外部的dc
		if(lpParamBits == NULL)
		{
			HDC dcMem = ::CreateCompatibleDC(pdc);   
			HBITMAP   hBmp = ::CreateDIBSection(dcMem, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
			if(lpBits == NULL)
			{
				::MessageBox(NULL,L"Draw BMP no memory!", L"warning", MB_OK);  
				//return;
			}

			offset = GetTickCount() - dwStart;   
			//TRACE(L"file DC 2 %d\n", offset);
			dwStart   =   GetTickCount();
		
			int off = (h-imageRect.top-imageRect.Height())*w + imageRect.left*(lpBitmap->bmiHeader.biBitCount/8);

			file.Seek(off, CFile::current);
			int size = (((imageRect.Width()*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;   //rect.Width()*3;
			int offline = w - size; 
			
			if(off == 0 && offline == 0)
			{
				file.Read(lpBits, size*imageRect.Height());
			}
			else
			{
				for(int i = 0; i < imageRect.Height(); i++)
				{
					file.Read(lpBits, size); 
					LPBYTE lpbyte = (LPBYTE)lpBits;
					lpbyte += size;
					lpBits = (LPVOID)lpbyte;
					file.Seek(offline, CFile::current);
				}
			}
			
			file.Close();       
			
			offset = GetTickCount() - dwStart;   
			//TRACE(L"file DC 3 %d\n", offset);
			dwStart   =   GetTickCount();

			HBITMAP pOldBmp= (HBITMAP)::SelectObject(dcMem, hBmp);   
		
			offset = GetTickCount() - dwStart;   
			//TRACE(L"file DC 4 %d\n", offset);
			dwStart   =   GetTickCount(); 

    		//pdc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
			::BitBlt(pdc, hdcRect.left, hdcRect.top, hdcRect.Width(), hdcRect.Height(), dcMem, 0, 0, SRCCOPY);
		
			::SelectObject(dcMem, pOldBmp); 
		
			::DeleteObject(hBmp);
			::DeleteDC(dcMem);
		}
		//直接画到外部的dc的 lpParamBits上
		else
		{
			int off = (h-imageRect.top-imageRect.Height())*w + imageRect.left*(lpBitmap->bmiHeader.biBitCount/8);
			
			file.Seek(off, CFile::current);
			int size = (((imageRect.Width()*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;   //rect.Width()*3;
			int offline = w - size; 
			
			int w_ = (((800*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;
			int off_ = (480-hdcRect.top-hdcRect.Height())*w_ + hdcRect.left*(lpBitmap->bmiHeader.biBitCount/8);
						
			lpBits = (LPBYTE)lpParamBits+off_;

			if(off_ == 0 && w_ == w)
			{
				file.Read(lpBits, size*imageRect.Height());
			}
			else
			{
				for(int i = 0; i < hdcRect.Height(); i++)
				{
					file.Read(lpBits, size); 
					LPBYTE lpbyte = (LPBYTE)lpBits;
					lpbyte += w_;
					lpBits = (LPVOID)lpbyte;
					file.Seek(offline, CFile::current);
				}
			}
			file.Close();       
		}
		
		delete []lpBitmap;    //20090216 test

		offset = GetTickCount() - dwStart;   
		//TRACE(L"end file DC %d\n", offset);
		::Sleep(1);
	}
	else
	{
		DWORD   dwStart   =   GetTickCount(); 	

		IImage   *pImage   =   NULL; 
		
		//	CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
		
		//   Create   the   imaging   factory. 
		if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
		{ 
			//   Load   the   image   from   the   JPG   file. 
			if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
			{ 
				//   Draw   the   image. 
				
				pImage->Draw(pdc, hdcRect, NULL); 
				pImage->Release(); 
				::Sleep(1);
			} 
			else
			{
				Dprintf("DrawImage error\n");
			}
			
			pImgFactory-> Release(); 
		} 
		int offset = GetTickCount() - dwStart;  
	//	wprintf(szFileImage);
	//	Dprintf(" Draw ALL BMP %d\n", offset);
		//	CoUninitialize(); 

	}
// 	unsigned int w1 = DMemprintf("DrawImage 1");
// 	if(w != w1)
// 		Dprintf("w != w1\r\n");
}

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg dialog
CMultimediaPhoneDlg::CMultimediaPhoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultimediaPhoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultimediaPhoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitImage();
	n_bklightcount = 0;
	m_bIsHungOn = FALSE;
	m_bNetOkStatus = FALSE;
	m_bNetType = 0;
	m_nTELRigster = TELRIGSTER_UN;
	m_bIsDial = FALSE;
	m_sMobileNumber = "";
}

CMultimediaPhoneDlg::~CMultimediaPhoneDlg()
{
	for (std::map<int, CDialog*>::iterator i = panels_.begin(); i != panels_.end(); ++i)
		delete i->second;
}

void CMultimediaPhoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultimediaPhoneDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

//  	DDX_Control(pDX, IDC_BUTTON_TIME, m_btnTime);
//  	DDX_Control(pDX, IDC_BUTTON_IME, m_btnIME);
//  	DDX_Control(pDX, IDC_BUTTON_LINE, m_btnLine);
//  	DDX_Control(pDX, IDC_BUTTON_MAIN, m_btnMain);
//  	DDX_Control(pDX, IDC_BUTTON_CONTACT, m_btnContact);
//  	DDX_Control(pDX, IDC_BUTTON_CONTACTINFO, m_btnContactInfo);
//  	DDX_Control(pDX, IDC_BUTTON_SOUND, m_btnSound);
//  	DDX_Control(pDX, IDC_BUTTON_INFORMATION, m_btnInformation);
//  	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btnSetting);
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
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickToggle)
	/*
	ON_BN_CLICKED(IDC_BUTTON_TIME, OnButtonTime)
	ON_BN_CLICKED(IDC_BUTTON_IME, OnButtonIME)
	ON_BN_CLICKED(IDC_BUTTON_LINE, OnButtonLine)
	ON_BN_CLICKED(IDC_BUTTON_MAIN, OnButtonMain)
	ON_BN_CLICKED(IDC_BUTTON_CONTACT, OnButtonContact)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTINFO, OnButtonContactinfo)
	ON_BN_CLICKED(IDC_BUTTON_SOUND, OnButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_INFORMATION, OnButtonInformation)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnButtonSetting)
	*/
END_MESSAGE_MAP()

/*
void IpConfigRenew()
{
	PROCESS_INFORMATION processInfo;
	STARTUPINFO lpStartupInfo; // 用于在创建子进程时设置各种属性 
	
	memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
	
	lpStartupInfo.cb = sizeof(lpStartupInfo);
	
	lpStartupInfo.dwX = 0;
	lpStartupInfo.dwY = 0;
	lpStartupInfo.dwXSize = 0;
	lpStartupInfo.dwYSize = 0;
	lpStartupInfo.wShowWindow= SW_SHOWNORMAL; 
	lpStartupInfo.dwFlags= 1|2|4;
	
	memset(&processInfo, 0, sizeof(processInfo));
	CString s = "/renew";
	if (!CreateProcess(L"ipconfig.exe", s, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL,  0, &processInfo))
	{
		
	}

	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
}
*/
void CMultimediaPhoneDlg::OnClickToggle(WPARAM w, LPARAM l)
{
	switch (w)
	{
		case 3:
			OnButtonMain();
			//test 20090602
			//	CNetStatusDlg::ADSLDial(m_nAPN.dialnumber, "", "", (CWnd *)this, (DIALTYPE)m_nAPN.apn_type);			
			break;
		case 4:
			OnButtonContact();
			
			//test 20090602
			//	Net3GHungOff();
			
			break;
		case 5:
			OnButtonContactinfo();
			break;
		case 6:
			OnButtonSound();
			break;
		case 7:
			OnButtonInformation();
			break;
		case 8:
			OnButtonSetting();
			break;
		default:
			break;
	}
	if(w >= 3)
	{
		m_MJPGList.SetUnitIsDownStatus(2, FALSE);
		m_MJPGList.SetUnitIsDownStatus(3, FALSE);
		m_MJPGList.SetUnitIsDownStatus(4, FALSE);
		m_MJPGList.SetUnitIsDownStatus(5, FALSE);
		m_MJPGList.SetUnitIsDownStatus(6, FALSE);
		m_MJPGList.SetUnitIsDownStatus(7, FALSE);
		m_MJPGList.SetUnitIsDownStatus(w-1, TRUE);
		m_MJPGList.Invalidate();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg message handlers
extern BOOL watchdog_test;
void CMultimediaPhoneDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch (w)
	{
		case 1:
		//	watchdog_test = !watchdog_test;
			m_pNetStatusDlg->SetType(m_bNetType, m_bNetOkStatus);		//网络状态
			m_pNetStatusDlg->ShowWindow_(SW_SHOW);
			break;
		case 2:
			OnButtonLine();		//电话状态
			break;
		case 3:
			OnButtonMain();
			//test 20090602
		//	CNetStatusDlg::ADSLDial(m_nAPN.dialnumber, "", "", (CWnd *)this, (DIALTYPE)m_nAPN.apn_type);			
			break;
		case 4:
			OnButtonContact();
			
			//test 20090602
		//	Net3GHungOff();

			break;
		case 5:
			OnButtonContactinfo();
			break;
		case 6:
			OnButtonSound();
			break;
		case 7:
			OnButtonInformation();
			break;
		case 8:
			OnButtonSetting();
			break;
	}
	if(w >= 3)
	{
		m_MJPGList.SetUnitIsDownStatus(2, FALSE);
		m_MJPGList.SetUnitIsDownStatus(3, FALSE);
		m_MJPGList.SetUnitIsDownStatus(4, FALSE);
		m_MJPGList.SetUnitIsDownStatus(5, FALSE);
		m_MJPGList.SetUnitIsDownStatus(6, FALSE);
		m_MJPGList.SetUnitIsDownStatus(7, FALSE);
		m_MJPGList.SetUnitIsDownStatus(w-1, TRUE);
		m_MJPGList.Invalidate();
	}
}

void  CMultimediaPhoneDlg::SetAPN(int apn)
{
	m_nAPN.apn_type = (int)apn;
	if(apn == 0)			//CMWAP
	{
		strcpy(m_nAPN.dialnumber, m_pSettingDlg->m_pSetting->gprsDialnumber1_.c_str());
		sprintf(m_nAPN.proxy, "%s:%d", m_pSettingDlg->m_pSetting->gprsProxyIp1_.c_str(), m_pSettingDlg->m_pSetting->gprsProxyPort1_);
		strcpy(m_nAPN.http, m_pSettingDlg->m_pSetting->gprsHttp1_.c_str());
	}
	else
	{
		strcpy(m_nAPN.dialnumber, m_pSettingDlg->m_pSetting->gprsDialnumber2_.c_str());
		if(m_pSettingDlg->m_pSetting->gprsProxyIp2_ == "")
			strcpy(m_nAPN.proxy, "");
		else
			sprintf(m_nAPN.proxy, "%s:%d", m_pSettingDlg->m_pSetting->gprsProxyIp2_.c_str(), m_pSettingDlg->m_pSetting->gprsProxyPort2_);
		strcpy(m_nAPN.http, m_pSettingDlg->m_pSetting->gprsHttp2_.c_str());
	}
}

BOOL CMultimediaPhoneDlg::IsConnectNet()
{
	BOOL flag = TRUE;
	if(m_nTELRigster < TELRIGSTER_DIALING)
	{
		m_pDeleteTipDlg->type = 1;
		m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle("需要先拨号连接，请确定!", 30000);
		m_pDeleteTipDlg->ShowWindow_(TRUE);
		flag = FALSE;
	}
	else if(m_nTELRigster == TELRIGSTER_DIALING)
	{
		m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle("正在拨号连接，请稍候...", 2000);
		m_pDeleteTipDlg->ShowWindow_(TRUE);
		flag = FALSE;
	}
	return flag;
}

BOOL CMultimediaPhoneDlg::IsUnConnectNet()
{
	if(m_nTELRigster >= TELRIGSTER_DIALED)
	{
		m_pDeleteTipDlg->type = 2;
		m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle("需要退出拨号连接吗?请确定!", 30000);
		m_pDeleteTipDlg->ShowWindow_(TRUE);
	}
	return TRUE;
}

static BOOL isFirstReg = TRUE;
void TdDoWithProc()
{
	/*
	char *pBuf = new char[1024*64];
	memset(pBuf, 0, 64*1024);
	FILE *fl = fopen("/flashdrv/log.dat", "r+b");
	if(fl)
	{
		int size = fread(pBuf, sizeof(char), 64*1024, fl);
		fclose(fl);
		Data::MMSData* pMMsData = MMS::MMSWarp::GetMMSWarp()->DecodeMessage((UINT8 *)pBuf, size);
		pMMsData->Insert();
		delete pMMsData;
	}
	*/

	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	pMainDlg->SetAPN((int)CMNET);
	while(1)
	{
	//	break;

		::Sleep(50);
		//pMainDlg->m_bIsDial = TRUE;

		if(pMainDlg->m_nTELRigster == TELRIGSTER_UN)
		{
			pMainDlg->doSerachrTDNet();
		}

		else if(pMainDlg->m_nTELRigster == TELRIGSTER_TD && pMainDlg->m_bIsDial)
		{
// 			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"正在拨号连接...", FALSE);
// 			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(theApp.m_pMainWnd->m_hWnd);
// 			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

			if(CNetStatusDlg::ADSLDial(pMainDlg->m_nAPN.dialnumber, "", "", (CWnd *)pMainDlg, (DIALTYPE)pMainDlg->m_nAPN.apn_type))
			{
				pMainDlg->m_MJPGList.SetUnitBitmap(12, ".\\adv\\mjpg\\k1\\common\\3G\\gprs状态1.bmp", "", TRUE);
				pMainDlg->m_nTELRigster = TELRIGSTER_DIALING;
			}
			else
			{
				Sleep(1000);
			}	
		}
		else if(pMainDlg->m_nTELRigster == TELRIGSTER_DIALED /*&& isFirstReg*/)
		{
			//Dprintf("doRegisterTel :\r\n");
			//pMainDlg->doRegisterTel();    //LXZ 20090618k
		}
	//	else if(pMainDlg->m_nTELRigster == TELRIGSTER_REG)
		if(pMainDlg->m_nTELRigster >= TELRIGSTER_TD)
		{
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
			WriteLog(restartTimer);
		}

		if((watchdog_MFC && (!g_MemoryLeak)) || (g_bAdjustPanel))
		{
			  pMainDlg->SetWatchDog();   //设置watchdog   移到main窗口
			  Sleep(5);
			  pMainDlg->SetWatchDog();   //设置watchdog   移到main窗口
			  Sleep(5);
			  pMainDlg->SetWatchDog();   //设置watchdog   移到main窗口
			  Sleep(5);
			  watchdog_MFC = FALSE;
		}
#endif
		/*
		int val = pMainDlg->GetBackLightValue();

		//pMainDlg->SetBackLight(val);
		//Sleep(1000);
		//Dprintf("light %d\n", val);   //7 亮    0 黑
		if(val == 0)		
		{
			if(pMainDlg->n_bklightcount <= 0)
				pMainDlg->SetBackLight(val);
			else
				pMainDlg->SetBackLight(7);
			pMainDlg->n_bklightcount--;
			if(pMainDlg->n_bklightcount > 30)
				pMainDlg->n_bklightcount = 0;
		}
		else
			pMainDlg->SetBackLight(val);
		*/
// 		if(netSocket)
// 		{
// 			netSocket->close();
// 		}
// 		netSocket = new Util::ClientSocket();
// 		netSocket->connect("127.0.0.1", 21);
// 		if(netSocket->getErrorState() != Util::Socket::esConnectionError)
// 		{
// 			pMainDlg->m_bNetOkStatus = TRUE;
// 		}
// 		else
// 			pMainDlg->m_bNetOkStatus = FALSE;

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
void CMultimediaPhoneDlg::TestDB()
{
/*	Data::MMSData::Remove("");
	boost::shared_ptr<Data::MMSData> pMMsData = boost::shared_ptr<Data::MMSData> (new Data::MMSData);

	pMMsData->SenderAddress = "13601371701";
	pMMsData->Subject = "结婚请柬";
	pMMsData->DateAndTime = 360000;
	pMMsData->SavePath = "/flashdrv/3g/file/20090602161404/128x128/";
	pMMsData->isRead = FALSE;
	pMMsData->type = Data::MMSData::tpReceive;
	pMMsData->Insert();
	
	pMMsData->SenderAddress = "13601371711";
	pMMsData->Subject = "结婚请柬";
	pMMsData->DateAndTime = 360000;
	pMMsData->SavePath = "/flashdrv/3g/file/20090602161404/128x128/";
	pMMsData->isRead = FALSE;
	pMMsData->type = Data::MMSData::tpReceive;
	pMMsData->Insert();
	*/
	
//	Data::MultimediaDownload::Remove("");
/*
	Data::WeatherPicture::Remove("");
	boost::shared_ptr<Data::WeatherPicture> pWeatherPic = boost::shared_ptr<Data::WeatherPicture> (new Data::WeatherPicture);
	
	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\小雪.png";
	pWeatherPic->content = "小雪";
	pWeatherPic->code = "900022";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\中雪.png";
	pWeatherPic->content = "中雪";
	pWeatherPic->code = "900032";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\大雪.png";
	pWeatherPic->content = "大雪";
	pWeatherPic->code = "900042";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\暴雪.png";
	pWeatherPic->content = "暴雪";
	pWeatherPic->code = "900052";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\冰雹.png";
	pWeatherPic->content = "冰雹";
	pWeatherPic->code = "900072";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\小雨.png";
	pWeatherPic->content = "小雨";
	pWeatherPic->code = "900082";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\中雨.png";
	pWeatherPic->content = "中雨";
	pWeatherPic->code = "900092";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\大雨.png";
	pWeatherPic->content = "大雨";
	pWeatherPic->code = "900102";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\暴雨.png";
	pWeatherPic->content = "暴雨";
	pWeatherPic->code = "900112";
	pWeatherPic->Insert();


	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\雨夹雪.png";
	pWeatherPic->content = "雨夹雪";
	pWeatherPic->code = "900132";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\阴.png";
	pWeatherPic->content = "阴";
	pWeatherPic->code = "900142";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\多云.png";
	pWeatherPic->content = "多云";
	pWeatherPic->code = "900152";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\晴.png";
	pWeatherPic->content = "晴";
	pWeatherPic->code = "900162";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\阴转多云.png";
	pWeatherPic->content = "阴转多云";
	pWeatherPic->code = "900172";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\晴转多云.png";
	pWeatherPic->content = "晴转多云";
	pWeatherPic->code = "900182";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\阵雨.png";
	pWeatherPic->content = "阵雨";
	pWeatherPic->code = "900242";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\雷雨.png";
	pWeatherPic->content = "雷雨";
	pWeatherPic->code = "900252";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\雷阵雨.png";
	pWeatherPic->content = "雷阵雨";
	pWeatherPic->code = "900262";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\强雷电.png";
	pWeatherPic->content = "强雷电";
	pWeatherPic->code = "900272";
	pWeatherPic->Insert();

	pWeatherPic->filename = "\\flashdrv\\3g\\weather\\雾.png";
	pWeatherPic->content = "雾";
	pWeatherPic->code = "900322";
	pWeatherPic->Insert();
	

	
	Data::Register::Remove("");
	boost::shared_ptr<Data::Register> pRegister = boost::shared_ptr<Data::Register> (new Data::Register);
	pRegister->terminalVersion = "1.0.0";
	pRegister->IMEI = "1234";
	pRegister->terminalId = "18851154007";
	pRegister->Insert();

	Data::RegisterReply::Remove("");
	boost::shared_ptr<Data::RegisterReply> pRegisterReply = boost::shared_ptr<Data::RegisterReply> (new Data::RegisterReply);
	pRegisterReply->result = Data::RegisterReply::rSuccess;
	pRegisterReply->areaCode = "南京";
	pRegisterReply->description = "南京";
	pRegisterReply->Insert();
	

	Data::Weather::Remove("");
	*/

// 	Data::OtaMessage::Remove("");
// 	boost::shared_ptr<Data::OtaMessage> pOtaMessage = boost::shared_ptr<Data::OtaMessage> (new Data::OtaMessage);
// 	
/*
	pOtaMessage->instruction = Data::tMenuManager;
	pOtaMessage->state = Data::OtaMessage::stUnDownload;
	pOtaMessage->url = "http://211.138.198.106:8888/ota?id=7434";
	pOtaMessage->Insert();
*/
/*
	pOtaMessage->instruction = Data::tWeather;
	pOtaMessage->state = Data::OtaMessage::stUnDownload;
	pOtaMessage->url = "http://211.138.198.106:8888/ota?id=7341";
	pOtaMessage->Insert();
*/
// 	pOtaMessage->instruction = Data::tMultimediaDownload;
// 	pOtaMessage->state = Data::OtaMessage::stUnDownload;
// 	pOtaMessage->url = "http://211.138.198.106:8888/ota?id=7432";
// 	pOtaMessage->Insert();
// 
// 	pOtaMessage->instruction = Data::tMultimediaDownload;
// 	pOtaMessage->state = Data::OtaMessage::stUnDownload;
// 	pOtaMessage->url = "http://211.138.198.106:8888/ota?id=7433";
// 	pOtaMessage->Insert();
	

	// 1065806651
/*
	Data::MenuManager::Remove("");
	boost::shared_ptr<Data::MenuManager> m_pMenuManager = boost::shared_ptr<Data::MenuManager> (new Data::MenuManager) ;
	
	for(int i = 0; i <9; i++)
	{
		char *spcod[] = {"1065805002", "1111", "1065805011", "1065805021", "1065805051", "1065805061", "1065805012", "3333", "1065805003"};
		char txt[24];
		sprintf(txt, "10000%d", i);
		m_pMenuManager->opType = (Data::MenuManager::MenuOperator)1;
		m_pMenuManager->menuInfos.menuLevel = 1; 
		m_pMenuManager->menuInfos.parentId = 4; 
		m_pMenuManager->menuInfos.menuId = 400+i; 
		m_pMenuManager->menuInfos.type = (Data::MenuManager::MenuInfo::Type)1 ; 
		m_pMenuManager->menuInfos.showOrder = 4+i; 
		m_pMenuManager->menuInfos.spCode = spcod[i]; 
		m_pMenuManager->menuInfos.pageCount = 0; 
		m_pMenuManager->menuInfos.imageName = ""; 
		m_pMenuManager->menuInfos.imageContent = ""; 
		m_pMenuManager->menuInfos.url = ""; 
		sprintf(txt, "", i);
        m_pMenuManager->menuInfos.menuName = txt; 
		m_pMenuManager->Insert();	
	}
*/
/*
	for(i = 0; i < 14; i++)
	{
		char txt[24];
		sprintf(txt, "20000%d", i);
		m_pMenuManager->opType = (Data::MenuManager::MenuOperator)1;
		m_pMenuManager->menuInfos.menuLevel =2; 
		m_pMenuManager->menuInfos.parentId = 2004; 
		m_pMenuManager->menuInfos.menuId = 3000+i; 
		m_pMenuManager->menuInfos.type = (Data::MenuManager::MenuInfo::Type)1 ; 
		m_pMenuManager->menuInfos.showOrder = 1+i; 
		m_pMenuManager->menuInfos.spCode = txt; 
		m_pMenuManager->menuInfos.pageCount = 0; 
		m_pMenuManager->menuInfos.imageName = "/flashdrv/3g/menu/1.jpg"; 
		m_pMenuManager->menuInfos.imageContent = ""; 
		m_pMenuManager->menuInfos.url = ""; 
		sprintf(txt, "二级菜单%d", i);
        m_pMenuManager->menuInfos.menuName = txt; 
		m_pMenuManager->Insert();	
	}
*/
  /*
 	boost::shared_ptr<Data::MenuManager> m_pMenuManager = boost::shared_ptr<Data::MenuManager> (new Data::MenuManager) ;

	std::vector<boost::shared_ptr<Data::MenuManager> > m_result = Data::MenuManager::GetFromDatabase(""); 
	int nSize = m_result.size();
	if(nSize > 0)
	{
		for(int i = 0; i < nSize; i++)
			m_pMenuManager = m_result[i];
	}
	*/

	/*
	boost::shared_ptr<Data::MultimediaDownload> m_pMediaDownload = boost::shared_ptr<Data::MultimediaDownload> (new Data::MultimediaDownload) ;
	
	for(int i = 0; i < 14; i++)
	{
		char txt[24];
		sprintf(txt, "列表%d", i);

		m_pMediaDownload->multimediaInfos.type = (Data::MultimediaDownload::MultimediaInfo::Type)3;
		m_pMediaDownload->multimediaInfos.spCode = "200000";
		m_pMediaDownload->multimediaInfos.subject = txt;
		m_pMediaDownload->multimediaInfos.content ="	for(i = 0; i < 14; i++)\
		{\
			char txt[24];\
			m_pMenuManager->menuLevel =2; \
			m_pMenuManager->parentId = 2004; \
			m_pMenuManager-> menuId = 3000+i; \
			m_pMenuManager->type = (Data::Type)1 ; \
			m_pMenuManager->showOrder = 1+i; \
			m_pMenuManager->spCode = txt; \
			m_pMenuManager->Insert();\
	}\
	  \
	   \
	  for(i = 0; i < 14; i++)\
	  {\
	  char txt[24];\
	  m_pMenuManager->menuLevel =2; \
	  m_pMenuManager->parentId = 2004; \
	  m_pMenuManager-> menuId = 3000+i; \
	  m_pMenuManager->type = (Data::Type)1 ; \
	  m_pMenuManager->showOrder = 1+i; \
	  m_pMenuManager->spCode = txt; \
	  m_pMenuManager->Insert();\
	}"	
	;
		m_pMediaDownload->Insert();	
	}
	for(i = 0; i < 23; i++)
	{
		char txt[24];
			
		m_pMediaDownload->multimediaInfos.type = (Data::MultimediaDownload::MultimediaInfo::Type)1;
		m_pMediaDownload->multimediaInfos.spCode = "200001";
		sprintf(txt, "/flashdrv/3g/file/car%d.jpg", i%5);
		m_pMediaDownload->multimediaInfos.filename = txt;
		sprintf(txt, "列表%d", i);
		m_pMediaDownload->multimediaInfos.subject = txt;
		m_pMediaDownload->multimediaInfos.content ="";
		m_pMediaDownload->Insert();	
	}
	std::vector<boost::shared_ptr<Data::MultimediaDownload> > m_result1 = Data::MultimediaDownload::GetFromDatabase(""); 
	int nSize = m_result1.size();
	if(nSize > 0)
	{
		for(i= 0; i < nSize; i++)
			m_pMediaDownload = m_result1[i];
	}
	*/

}

Util::ATCommandWarp* GetATCommandWarp()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	return main->m_pATCommandWarp1;
}

void GIsOpenMix(BOOL isOn) 
{
     CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	 if(!isOn)
	 {
		waveInMessage(0, 0x10000001,0,0);
		char *sound = "ATN'MIX=0'\r";
		main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	 }
	 else
	 {
		 waveInMessage(0, 0x10000002,0,0);
		 char *sound = "ATN'MIX=1'\r";
		 main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	 }
	 ::Sleep(10);
}
void PlayDialToneProc(void *p)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	char *ptr = (char *)p;
	int offtime = 200;
	if(strlen(ptr) > 1)
		offtime = 150;
	while(*ptr != '\0')
	{
		char filename[32];
		sprintf(filename, "/hive/res/%c.wav", *ptr);
		ptr++;
		CString s = filename;
		main->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
		Sleep(offtime);
	}
	main->phone_->StartRing(L"");
	GIsOpenMix(0);
}

char gDiatone[64];
void GPlayDialTone(char *dialtone) 
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int nVolume = main->m_pSettingDlg->m_pSetting->smsRingVolume_;
	UINT16 volume[] = {0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000}; //(5-nVolume)*0xFFFF/5;
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
	if(!isDial)
	{
		char *sound = "ATN'TLK=0'\r";
		main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	}
	else
	{
		char *sound = "ATN'TLK=1'\r";
		main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	}
	 ::Sleep(10);
}
void GAskBatteryStatus()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	char *status = "ATN'STA=?'\r";
	main->phone_->m_pRS232->WritePort((unsigned char *)status, strlen(status));
	::Sleep(10);
}
void GNotifyReady()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	char *sound = "ATN'RDY=1'\r";
	main->phone_->m_pRS232->WritePort((unsigned char *)sound, strlen(sound));
	::Sleep(10);
	char *status = "ATN'STA=?'\r";
	main->phone_->m_pRS232->WritePort((unsigned char *)status, strlen(status));
	::Sleep(10);
}
BOOL CMultimediaPhoneDlg::OnInitDialog()
{
	//lxz test jpg
	CDialog::OnInitDialog();
	
	SetWindowText(L"Edo_Phone_Dlg");
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

	//init player
	//other wnd
	playervideo_ = new Player(NULL, mtVideo);
	playervideo_->SetPlayerOwner(this);

	playeraudio_ = new Player(NULL, mtAudio);
	playeraudio_->SetPlayerOwner(this);

	playerimage_ = new Player(NULL, mtImage);
	playerimage_->SetPlayerOwner(this);

//	m_pPhone = new Logical::Phone(PickupFunc, HandsfreeFunc, HangupFunc, KeypadDialFunc, ConnectFunc, RingInFunc, RemoteHangupFunc, TimeoutFunc, ErrorFunc);
//	m_pPhone->start();
	//短信 电话

	/*
	m_pComWarp = new Util::ComWarp();
	m_pComWarp->OpenComm(L"COM4:");
	m_pATCommandWarp = new Util::ATCommandWarp();
	m_pATCommandWarp->Bind(m_pComWarp);
	phone_ = Telephone::TelephoneWarp::GetTelephoneWarp();//new Telephone::TelephoneWarp();
	m_pSMSWarp = SMS::SMSWarp::GetSMSWarp();
	*/

#if(G3_MODULOR == 1)
 	m_pComWarp1 = new Util::ComWarp();
 	m_bATComm = m_pComWarp1->OpenComm(L"COM8:");
 	m_pATCommandWarp1 = new Util::ATCommandWarp();
 	m_pATCommandWarp1->Bind(m_pComWarp1);
	m_pSMSWarp = SMS::SMSWarp::GetSMSWarp();

	m_pVideoComWarp = new Util::ComWarp();
// 	
 	phone_ = Telephone::TelephoneWarp::GetTelephoneWarp();//new Telephone::TelephoneWarp();
	extern void GNotifyReady();
	GNotifyReady();

#else
	m_pComWarp1 = new Util::ComWarp();
	m_pComWarp1->OpenComm(L"COM4:");
	m_pATCommandWarp1 = new Util::ATCommandWarp();
	m_pATCommandWarp1->Bind(m_pComWarp1);
	
	phone_ = Telephone::TelephoneWarp::GetTelephoneWarp();//new Telephone::TelephoneWarp();
	m_pSMSWarp = SMS::SMSWarp::GetSMSWarp();
#endif
	

//	simdevice_ = new SIMDevice(NULL);

	//mainVideoDlg_ = new CMainVideoDlg();
	//mainVideoDlg_->Create(CMainVideoDlg::IDD);
	//playerDlg_ = new CPlayerDlg();
	//playerDlg_->Create(CPlayerDlg::IDD);
/*
	ICONKEYSTRUCT softkeyval_[9];
	softkeyval_[0].sKeyRect = CRect(3, 9, 40, 24);
	softkeyval_[0].nKeyValue = 1;
	softkeyval_[0].nTextId = 0;
	softkeyval_[0].nBitmapId = 0;
	softkeyval_[0].nAlign = IMAGE_LEFT;
	softkeyval_[0].bToggle = FALSE;

	softkeyval_[1].sKeyRect = CRect(44, 9, 59, 24);
	softkeyval_[1].nKeyValue = 2;
	softkeyval_[1].nTextId = 0;
	softkeyval_[1].nBitmapId = IDB_BITMAP_MAININPUT;
	softkeyval_[1].nAlign = IMAGE_LEFT;
	softkeyval_[1].bToggle = FALSE;

	softkeyval_[2].sKeyRect = CRect(63, 9, 78, 24);
	softkeyval_[2].nKeyValue = 3;
	softkeyval_[2].nTextId = 0;
	softkeyval_[2].nBitmapId = IDB_BITMAP_MAINTEL;
	softkeyval_[2].nAlign = IMAGE_LEFT;
	softkeyval_[2].bToggle = FALSE;

	softkeyval_[3].sKeyRect = CRect(102, 4, 166, 28);
	softkeyval_[3].nKeyValue = 4;
	softkeyval_[3].nTextId = Data::RI_MAIN_MAINBTN;
	softkeyval_[3].nBitmapId = IDB_BITMAP_MAINDESKTOP;
	softkeyval_[3].nAlign = IMAGE_LEFT;
	softkeyval_[3].bToggle = TRUE;

	softkeyval_[4].sKeyRect = CRect(169, 4, 229+1, 28);
	softkeyval_[4].nKeyValue = 5;
	softkeyval_[4].nTextId = Data::RI_MAIN_CARDCASEBTN;
	softkeyval_[4].nBitmapId = IDB_BITMAP_MAINCARD;
	softkeyval_[4].nAlign = IMAGE_LEFT;
	softkeyval_[4].bToggle = TRUE;

	softkeyval_[5].sKeyRect = CRect(232, 4, 292+1, 28);
	softkeyval_[5].nKeyValue = 6;
	softkeyval_[5].nTextId = Data::RI_MAIN_CALLLISTBTN;
	softkeyval_[5].nBitmapId = IDB_BITMAP_MAINCALLLIST;
	softkeyval_[5].nAlign = IMAGE_LEFT;
	softkeyval_[5].bToggle = TRUE;

	softkeyval_[6].sKeyRect = CRect(295, 4, 355+1, 28);
	softkeyval_[6].nKeyValue = 7;
	softkeyval_[6].nTextId = Data::RI_MAIN_SOUND;
	softkeyval_[6].nBitmapId = IDB_BITMAP_MAINRECORD;
	softkeyval_[6].nAlign = IMAGE_LEFT;
	softkeyval_[6].bToggle = TRUE;

	softkeyval_[7].sKeyRect = CRect(358, 4, 416+3, 28);
	softkeyval_[7].nKeyValue = 8;
	softkeyval_[7].nTextId = Data::RI_MAIN_STOCKINFOBTN;
	softkeyval_[7].nBitmapId = IDB_BITMAP_MAININFO;
	softkeyval_[7].nAlign = IMAGE_LEFT;
	softkeyval_[7].bToggle = TRUE;

	softkeyval_[8].sKeyRect = CRect(421, 4, 479, 28);
	softkeyval_[8].nKeyValue = 9;
	softkeyval_[8].nTextId = Data::RI_MAIN_SYSTTEMSETTING;
	softkeyval_[8].nBitmapId = IDB_BITMAP_MAINSETTING;
	softkeyval_[8].nAlign = IMAGE_LEFT;
	softkeyval_[8].bToggle = TRUE;
	
	m_StatusKey.Create(IDB_BITMAP_MAIN, IDB_BITMAP_MAINTOGGLE, 3, CPoint(0, 0), 9,  softkeyval_, CRect(0, 204, 479, 233), this, WS_VISIBLE|WS_CHILD, IDC_BUTTON_TIME);
	SetSkinStyle();
	m_StatusKey.SetLeftTimer();
	*/
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 420, 800, 480), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\3g_telstatus.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 420, 800, 480));
//	m_MJPGList.SetUnitIsDownStatus(2, TRUE);
	m_MJPGList.SetUnitIsDownStatus(2, TRUE);

	m_MJPGList.SetUnitIsDisable(0, FALSE);
	m_MJPGList.SetUnitIsDisable(1, TRUE);

	/*
	m_btnTime.Create(str, WS_CHILD|WS_VISIBLE, CRect(0, 205, 41, 234), this, IDC_BUTTON_TIME);
	m_btnIME.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(41, 205, 65, 234), this, IDC_BUTTON_IME);
	m_btnIME.SetIcon(IDI_ICON_IME, CSize(16, 16));
	m_btnLine.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(65, 205, 89, 234), this, IDC_BUTTON_LINE);
	m_btnLine.SetIcon(IDI_ICON_LINE, CSize(16, 16));

	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_MAINBTN);
	str = strTemp.c_str();		
	m_btnMain.Create(str, WS_CHILD|WS_VISIBLE, CRect(91, 205, 154, 234), this, IDC_BUTTON_MAIN);
	m_btnMain.SetWindowText(str);
	m_btnMain.SetIcon(IDI_ICON_DESK, CSize(16, 16));
	
	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_CARDCASEBTN);
	str = strTemp.c_str();
	m_btnContact.Create(str, WS_CHILD|WS_VISIBLE, CRect(156, 205, 219, 234), this, IDC_BUTTON_CONTACT);
	m_btnContact.SetWindowText(str);
	m_btnContact.SetIcon(IDI_ICON_CONTACT, CSize(16, 16));
	
	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_CALLLISTBTN);
	str = strTemp.c_str();
	m_btnContactInfo.Create(str, WS_CHILD|WS_VISIBLE, CRect(221, 205, 284, 234), this, IDC_BUTTON_CONTACTINFO);
	m_btnContactInfo.SetWindowText(str);
	m_btnContactInfo.SetIcon(IDI_ICON_CONTACTINFO, CSize(16, 16));

	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_SOUND);
	str = strTemp.c_str();
	m_btnSound.Create(str, WS_CHILD|WS_VISIBLE, CRect(286, 205, 349, 234), this, IDC_BUTTON_SOUND);
	m_btnSound.SetWindowText(str);
	m_btnSound.SetIcon(IDI_ICON_SOUND, CSize(16, 16));

	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_STOCKINFOBTN);
	str = strTemp.c_str();
	m_btnInformation.Create(str, WS_CHILD|WS_VISIBLE, CRect(351, 205, 414, 234), this, IDC_BUTTON_INFORMATION);
	m_btnInformation.SetWindowText(str);
	m_btnInformation.SetIcon(IDI_ICON_INFO, CSize(16, 16));

	strTemp = Data::LanguageResource::Get(Data::RI_MAIN_SYSTTEMSETTING);
	str = strTemp.c_str();
	m_btnSetting.Create(str, WS_CHILD|WS_VISIBLE, CRect(416, 205, 480, 234), this, IDC_BUTTON_SETTING);
	m_btnSetting.SetWindowText(str);
	m_btnSetting.SetIcon(IDI_ICON_SETTING, CSize(16, 16));
	*/

	CDialog* panel; 
	panel = new CInformationDlg();
	panel->Create(CInformationDlg::IDD);
	panels_[IDC_BUTTON_INFORMATION] = panel;

	m_pSettingDlg = new CSettingDlg();
	panel = m_pSettingDlg;
	panel->Create(CSettingDlg::IDD);
	panels_[IDC_BUTTON_SETTING] = panel;
	

	/*
	设置 三个 界面
	*/
	m_pTelephoneDlg = new CTelephoneDlg();
	m_pTelephoneDlg->Create(CTelephoneDlg::IDD);

	m_pMainDlg = new CMainDlg();
	panel = m_pMainDlg;
	panel->Create(CMainDlg::IDD);
	panels_[IDC_BUTTON_MAIN] = panel;
	currentPanel_ = panel;

	m_pContactDlg = new CContactDlg();
	m_pContactDlg->Create(CContactDlg::IDD);
	panel = m_pContactDlg;
	panels_[IDC_BUTTON_CONTACT] = panel;

	m_pContactInfoDlg = new CContactInfoDlg();
	panel = m_pContactInfoDlg;
	panel->Create(CContactInfoDlg::IDD);
	panels_[IDC_BUTTON_CONTACTINFO] = panel;

	m_pSoundDlg = new CSoundDlg();
	panel = m_pSoundDlg;
	panel->Create(CSoundDlg::IDD);
	panels_[IDC_BUTTON_SOUND] = panel;

	m_pContactNewDlg = new CContactNewDlg(panels_[IDC_BUTTON_CONTACT]);
	panel = m_pContactNewDlg;
	panel->Create(CContactNewDlg::IDD);
	panels_[IDC_BUTTON_CONTACTNEW] = panel;

	m_pContactGroupDlg = new CContactGroupDlg(panels_[IDC_BUTTON_CONTACT]);
	panel = m_pContactGroupDlg;
	panel->Create(CContactGroupDlg::IDD);
	panels_[IDC_BUTTON_CONTACTGROUP] = panel;


	m_pDeleteTipDlg = new CDeleteTipDlg(this);
	m_pDeleteTipDlg->Create(CDeleteTipDlg::IDD);

	m_pNotebookDlg = new CNotebookDlg(this);
	m_pNotebookDlg->Create(CNotebookDlg::IDD);

	m_AlarmShowDlg = new CAlarmShowDlg(this);
	m_AlarmShowDlg->Create(CAlarmShowDlg::IDD);

	m_pWarningNoFlashDlg = new CWarningNoFlashDlg(this);
	m_pWarningNoFlashDlg->Create(CWarningNoFlashDlg::IDD);

	m_pNetStatusDlg = new CNetStatusDlg(this);
	m_pNetStatusDlg->Create(CNetStatusDlg::IDD);

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

/*
 	m_mainLunarderDlg1_ = new CLunarderDlg(panels_[IDC_BUTTON_MAINNOTE]);
	panel = m_mainLunarderDlg1_;
	panel->Create(CContactGroupDlg::IDD);
	panels_[IDC_BUTTON_MAINNOTE] = panel;
*/
	/*
	m_pHuangLiDlg_ = new CHuangliDlg;
	m_pHuangLiDlg_->Create(CHuangliDlg::IDD, this);
	m_pHuangLiDlg_->ShowWindow(SW_HIDE);
	*/

/*
	buttons_[IDC_BUTTON_TIME] = &m_btnTime;
	buttons_[IDC_BUTTON_IME] = &m_btnIME;s
	buttons_[IDC_BUTTON_LINE] = &m_btnLine;
	buttons_[IDC_BUTTON_MAIN] = &m_btnMain;
	buttons_[IDC_BUTTON_CONTACT] = &m_btnContact;
	buttons_[IDC_BUTTON_CONTACTINFO] = &m_btnContactInfo;
	buttons_[IDC_BUTTON_SOUND] = &m_btnSound;
	buttons_[IDC_BUTTON_INFORMATION] = &m_btnInformation;
	buttons_[IDC_BUTTON_SETTING] = &m_btnSetting;

	for (std::map<int, CCEButtonST*>::iterator iter = buttons_.begin(); iter != buttons_.end(); ++iter)
	{
		SetButtonDefaultColor((*iter).second);
	}
	*/

	currentPanel_->ShowWindow(SW_SHOW);
//	m_pBtnCurrent = &m_btnMain;
//	SetButtonSelectedColor(m_pBtnCurrent);

	//and so on
	//m_btnLine.EnableWindow(FALSE);
	m_pFSM = new Util::FSM;
	AddFSMRules();
	m_uiKeyTimer = 1;
	m_uiRingTimer = 2;
	
	//设置进入屏保
	SetScreenSaveTimer();
	SetTimer(IDT_CONTROLBACKLIGHT_TIME+1, 60*1000, NULL);   //20090221   lxztest  60* 
	SetTimer(0x125, 1000, NULL);   //20090221

	//是否启动背光   lxz 20081223
//	if(!SetNightControlBackLightTimer())
//		ReStoreBackLight();

	//设置闹铃
	SetAlarmList();
	//查找今天没有看过过期的闹铃
	//m_pMainDlg->FindTodayAlarm();  移到设置当天闹铃列表中

	//lxz test 20090309
	//SetTimer(0x999, 50000, NULL);

	InitializeCriticalSection(&csCom);
	DWORD watchdogThreadID = 0;
	HANDLE m_pThread1 = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)WatchDogProc, 0, 0, &watchdogThreadID );
	DWORD tdThreadID = 0;
	HANDLE m_pThread2;
	if(m_bATComm)
	{
		m_pThread2 = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)TdDoWithProc, 0, 0, &tdThreadID );
	}
	else
	{
		SetTimer(0x126, 0, 0);
	}
	TestDB();
	/*
	if (m_pATCommandWarp1->Connect(""))
	{
		phone_->Bind(m_pATCommandWarp1);
		
		m_pSMSWarp = SMS::SMSWarp::GetSMSWarp();
		m_pSMSWarp->Bind(m_pATCommandWarp1);
	}
	*/

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

#define   CTL_CODE(DeviceType,Function,Method,Access)   (((DeviceType)<<16)|((Access)<<14)|((Function)<<2)|(Method)) 
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
			m_pNetStatusDlg->m_bADSLISConnnect = TRUE;
			return 1;
		}
		else
		{
			m_pNetStatusDlg->m_bADSLISConnnect = FALSE;
			return 0;
		}
	}
	
}

void CMultimediaPhoneDlg::SetNetTelStatus()
{
	static CString gsNet = "";
	static CString gsTelName = "";
	static CString gsChongdian = "";
	CString sTel = ".\\adv\\mjpg\\k1\\common\\电话1.bmp";
	CString sNet = ".\\adv\\mjpg\\k1\\common\\网络1.bmp";
	CString sChongdian = ".\\adv\\mjpg\\k1\\common\\网络1.bmp";
	static int gnTel = 0;
	static int gnNet = 0;
	static int gnChongdian = 0;

	if (m_pFSM->getCurrentState() != tsHangOff /*&& m_pTelephoneDlg->m_MJPGList.GetUnitText(100) != ""*/)   //摘机
	{
		if(!m_pTelephoneDlg->IsWindowVisible())
		{
			m_MJPGList.SetUnitIsDisable(1, FALSE);
			if(gnTel++%2)
				sTel = ".\\adv\\mjpg\\k1\\common\\电话1.bmp";
			else
				sTel = ".\\adv\\mjpg\\k1\\common\\电话2.bmp";
		}
	}
	else
	{
		m_MJPGList.SetUnitIsDisable(1, TRUE);
	}
	if(gsTelName != sTel)
	{
		m_MJPGList.SetUnitBitmap(1, sTel, "", TRUE);
		gsTelName = sTel;
	}

	if (phone_->m_BatteryStatus.isCharge)   //外接电源
	{
		
		m_MJPGList.SetUnitIsDisable(13, FALSE);
		if(gnChongdian++%2)
			sChongdian = ".\\adv\\mjpg\\k1\\common\\3g\\外界电源.bmp";
		else
			sChongdian = "";
	}
	else
	{
		sChongdian = "";
		m_MJPGList.SetUnitIsDisable(13, TRUE);
	}
	if(gsChongdian != sChongdian)
	{
		m_MJPGList.SetUnitBitmap(13, sChongdian, "", TRUE);
		gsChongdian = sChongdian;
	}

	//20090429 lxz
	return;

	m_bNetOkStatus = CheckInternetStatus();
	if(m_bNetOkStatus)
	{
		if(gnNet++%2)
			sNet = ".\\adv\\mjpg\\k1\\common\\网络1.bmp";
		else
			sNet = ".\\adv\\mjpg\\k1\\common\\网络2.bmp";
	}

	if(gsTelName != sTel)
	{
		m_MJPGList.SetUnitBitmap(1, sTel, "", TRUE);
		gsTelName = sTel;
	}
	if(gsNet != sNet)
	{
		m_MJPGList.SetUnitBitmap(0, sNet, "", TRUE);
		gsNet = sNet;
	}
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
//		SwitchPanel_(oldcurrentPanelID_);
		return;
	}
/*
	oldcurrentPanelID_ = currentPanelID_;
	currentPanelID_ = panelId;
*/
	
	if(currentPanel_ == panels_[IDC_BUTTON_MAIN])	//停止播放
	{
		panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 0);
	}

	if(1)					//lxz 20090512
//	if (panels_[panelId] != currentPanel_)    //lxz 20090512
	{
		/*CWnd *pWnd = */
	//	CWnd *pWnd = currentPanel_->GetFocus();
	//	pWnd->SendMessage(WM_KILLFOCUS, 0, 0);
		
		/*
		SetButtonDefaultColor(m_pBtnCurrent);
		m_pBtnCurrent = buttons_[panelId];
		SetButtonSelectedColor(m_pBtnCurrent);
		*/
		if(currentPanel_ == panels_[IDC_BUTTON_MAIN])
			((CMainDlg *)currentPanel_)->ShowWindow_(SW_HIDE);
		else if(currentPanel_ == panels_[IDC_BUTTON_INFORMATION])    //20071220 stocks
			((CInformationDlg*)currentPanel_)->ShowWindow_(SW_HIDE);
		else
			currentPanel_->ShowWindow(SW_HIDE);
		currentPanel_ = panels_[panelId];

		if(currentPanel_ != panels_[IDC_BUTTON_MAIN])   //lxz 20081206切换到主窗口很慢
			::SetFocus(currentPanel_->m_hWnd);

		if(currentPanel_ == m_pSettingDlg)
			m_pSettingDlg->IniCtrlData();
		if(currentPanel_ == panels_[IDC_BUTTON_MAIN])
			((CMainDlg *)currentPanel_)->ShowWindow_(SW_SHOW);
		else if(currentPanel_ == panels_[IDC_BUTTON_INFORMATION])   //20071220 stocks
			((CInformationDlg*)currentPanel_)->ShowWindow_(SW_SHOW);
		else
			currentPanel_->ShowWindow(SW_SHOW);
	/*
		if (panelId == IDC_BUTTON_MAIN)
		{
			m_StatusKey.SetToggleKey(3);
		}
		if (panelId == IDC_BUTTON_CONTACT)
		{
			m_StatusKey.SetToggleKey(4);
		}
		if (panelId == IDC_BUTTON_CONTACTINFO)
		{
			m_StatusKey.SetToggleKey(5);
		}
		if (panelId == IDC_BUTTON_SOUND)
		{
			m_StatusKey.SetToggleKey(6);
		}
		if (panelId == IDC_BUTTON_INFORMATION)
		{
			m_StatusKey.SetToggleKey(7);
		}
		*/
	}
	else if(currentPanel_ == panels_[IDC_BUTTON_MAIN])  //多次点击桌面
	{
		//lxz  20080623
		m_pMainDlg->SetMainMenu();
	}

	if(currentPanel_ == panels_[IDC_BUTTON_MAIN])	//恢复播放消息
	{
		panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 1);
	}

	///////////////
	m_MJPGList.SetUnitIsDownStatus(2, FALSE);
	m_MJPGList.SetUnitIsDownStatus(3, FALSE);
	m_MJPGList.SetUnitIsDownStatus(4, FALSE);
	m_MJPGList.SetUnitIsDownStatus(5, FALSE);
	m_MJPGList.SetUnitIsDownStatus(6, FALSE);
	m_MJPGList.SetUnitIsDownStatus(7, FALSE);
	if(panelId <= IDC_BUTTON_SETTING)
		m_MJPGList.SetUnitIsDownStatus(panelId-1001, TRUE);
	else if(panelId == IDC_BUTTON_SETTING)
		m_MJPGList.SetUnitIsDownStatus(7, FALSE);
	else
		m_MJPGList.SetUnitIsDownStatus(2, FALSE);
	m_MJPGList.Invalidate();
	///////////////
}

void CMultimediaPhoneDlg::OnButtonMain() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_MAIN);
}

void CMultimediaPhoneDlg::OnButtonContact() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_CONTACT);
}

void CMultimediaPhoneDlg::OnButtonContactinfo() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_CONTACTINFO);
	//lxz 20080617
//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearUnconnectCount();
}

void CMultimediaPhoneDlg::OnButtonSound() 
{
	// TODO: Add your control notification handler code here
	SwitchPanel_(IDC_BUTTON_SOUND);	
	//lxz 20080617
//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearRecordCount();
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

void CMultimediaPhoneDlg::OnButtonTime() 
{
	// TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonIME() 
{
	// TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonLine() 
{
	// TODO: Add your control notification handler code here
	if (m_pFSM->getCurrentState() != tsHangOff )
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
		
// 			HWND hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); //
// 			if(hWnd)
// 			{
// 				CRect rt;
// 				playerimage_->owner_->GetWindowRect(&rt);
// 			
// 				HDC hdc = ::GetDC(hWnd);
// 				HDC hdc1 = m_pMainDlg->m_mainPhotoDlg_->GetDC()->m_hDC;
// 				BitBlt(hdc1, rt.left, rt.top, rt.Width(), rt.Height(), hdc, 0, 0, 0);
// 				::ReleaseDC(hWnd, hdc);
// 			}
			
				
	//		Sleep(100);
		//	panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 4); //播放下一个文件
		}
		break;
	case WM_APP+60:
		Dprintf("0:");
		break;
	case WM_TEL_RING:
		//停止试听音乐
		m_pSettingDlg->StopTryRing();
		m_pContactGroupDlg->StopTryRing();
		m_pMainDlg->m_mainLunarderDlg_->StopTryRing();
	
		//ReStoreBackLight();    //移到防火墙后  lxz 20090304
		SendOutEvnet(WM_TEL_RING, 0);
		Dprintf("RING \n");
		m_pFSM->fireEvent(teRing, (void * const)wParam);
		break;
	case WM_TEL_HUNGOFF:
		Dprintf("HUNG OFF \n");
		m_pFSM->fireEvent(teHangOff, (void * const)wParam);
		SendOutEvnet(WM_TEL_HUNGOFF, 1);
		m_bIsHungOn = FALSE;
		break;
	case WM_TEL_HUNGON:
		Dprintf("HUNG ON 0\n");
		
		//停止试听音乐  
// 		m_pSettingDlg->StopTryRing();
// 		m_pContactGroupDlg->StopTryRing();
// 		m_pMainDlg->m_mainLunarderDlg_->StopTryRing();

		ReStoreBackLight();
		Dprintf("HUNG ON 1\n");
		m_bIsHungOn = TRUE;
		m_pFSM->fireEvent(teHangOn, (void * const)wParam);
		SendOutEvnet(WM_TEL_HUNGON, 0);
		break;
	case WM_TEL_CALLIDEND:
		Dprintf("CALL ID \n");
		m_pFSM->fireEvent(teCallID, (void * const)wParam);
		break;
	case WM_TEL_KEYCODE:
		SendOutEvnet(WM_TEL_KEYCODE, 0);
		Dprintf("%x ", wParam);
		m_pFSM->fireEvent(teKey, (void * const)wParam);
		break;
	case WM_REDIAL:
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Redial();
		break;
	case WM_MUTE:
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Mute();
		break;
	case WM_RKEY:
		Dprintf("R KEY \n");
		break;
	case WM_SPEEDDIAL:
		{
			/*     //lxz test 20090923
			char c = wParam;
			if (m_pFSM->getCurrentState() == tsHangOn)
			{
				std::map<char, std::string> m = m_pSettingDlg->m_pSetting->speedDials();
				if (m[c] != "")
				{
					m_pTelephoneDlg->DialContact(Data::TelephoneNumber(m[c]));
				}
			}
			*/
			char c = wParam;
		//	HWND hwnd = ::GetForegroundWindow();
			HWND hwnd = ::GetActiveWindow();
			if(hwnd)
			{
				HWND hMjpg = ::GetDlgItem(hwnd, 10086);
				if(hMjpg)
				{
					if( c == 1)
					{
						::SendMessage(hMjpg, WM_KEYDOWN, CANCEL_KEY, lParam);	
					}
					else if(c == 2)
					{
						::SendMessage(hMjpg, WM_KEYDOWN, UP_KEY, lParam);
					}
					else if(c == 3)
					{
						::SendMessage(hMjpg, WM_KEYDOWN, OK_KEY, lParam);
					}
					else if(c == 4)
					{
						::SendMessage(hMjpg, WM_KEYDOWN, LEFT_KEY, lParam);
					}
					else if(c == 6)
					{
						::SendMessage(hMjpg, WM_KEYDOWN, RIGHT_KEY, lParam);
					}
					else if(c == 8)
					{
						::SendMessage(hMjpg, WM_KEYDOWN, DOWN_KEY, lParam);
					}
				}
			}
		}
		break;
	case WM_HUNGONTORECORD:
		Dprintf("RECORD ... \n");
		if (m_pFSM->getCurrentState() == tsConnect)
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->OnButtonTelephoneRecord();
		break;
	case WM_HUNGONINRECORD:
		Dprintf("HUNG ON IN RECORD \n");
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOnInRecord(); 
		break;
	case WM_BINGJIZHAIJI:
		Dprintf("bingjizhaiji 0---- \n");
		if (m_pFSM->getCurrentState() == tsRing)
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
				playerimage_->owner_->SendMessage(WM_KEYDOWN, wParam, lParam);
			}
			else if(playeraudio_->isPlaying_ && m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible())
			{
				m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
			}
			break;
		}
	case WM_KEYUP:
		{
			int i = 0;
			i++;
		}
	case (WM_USER+1000):
//		Dprintf("InputWnd click\r\n");
		ReStoreBackLight();
		SetScreenSaveTimer();
		break;
	case WM_TEL_STATUS:     //电话状态
		if(wParam != TEL_SIGNALQUALITY && wParam != TEL_NETTYPE)
			m_pTelephoneDlg->SendMessage(WM_TEL_STATUS, wParam, lParam);
		else if(wParam == TEL_NETTYPE && (m_nTELRigster >= TELRIGSTER_TD))    //网络状态
		{
			//刷新信号
			char txt[64] = {0};
			if(lParam != 2)
			{
				if(m_MJPGList.GetUnitIsShow(20) == FALSE)
				{
					m_MJPGList.SetUnitIsShow(20, TRUE);
					sprintf(txt, ".\\adv\\mjpg\\k1\\common\\3g\\G信号.bmp");
					m_MJPGList.SetUnitBitmap(20, txt, txt, TRUE);
				}	
			}
			else
			{
				if(m_MJPGList.GetUnitIsShow(20) == TRUE)
				{
					m_MJPGList.SetUnitIsShow(20, FALSE);
					m_MJPGList.SetUnitBitmap(20, txt, txt, TRUE);
					m_MJPGList.SetUnitIsShow(10, TRUE);
				}
			}
		}
		else if(wParam == TEL_SIGNALQUALITY && (m_nTELRigster >= TELRIGSTER_TD))
		{
			//刷新信号
			char txt[64];
			sprintf(txt, ".\\adv\\mjpg\\k1\\common\\3g\\信号%d.bmp", lParam);
			m_MJPGList.SetUnitBitmap(10, txt, txt, TRUE);
		}
		break;
	case WM_CHANGE_BATTERY:
		{
		//	if(!phone_->m_BatteryStatus.isCharge)
			{
				if(phone_->m_BatteryStatus.batteryType == 0)
				{
					m_MJPGList.SetUnitBitmap(13, "", "", TRUE);
				}
				else
				{
					char *sChongdian = ".\\adv\\mjpg\\k1\\common\\3g\\外界电源.bmp";
					m_MJPGList.SetUnitBitmap(13, sChongdian, sChongdian, TRUE);
				}
				char txt[64];
				sprintf(txt, ".\\adv\\mjpg\\k1\\common\\3g\\电量%d.bmp", phone_->m_BatteryStatus.batteryProccess);
				m_MJPGList.SetUnitBitmap(11, txt, txt, TRUE);
			}
		}
		break;
	case WM_CHANGE_BATTERYOK:
		{
			char txt[64];
			sprintf(txt, ".\\adv\\mjpg\\k1\\common\\3g\\电量%d.bmp", phone_->m_BatteryStatus.batteryProccess);
			m_MJPGList.SetUnitBitmap(11, txt, txt, TRUE);

			m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
			m_pDeleteTipDlg->SetPasswordModel(false);
			m_pDeleteTipDlg->SetTitle("充电已完成!");
			m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
		break;
	case WM_ATCOMM_ERROR:
		{
			m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
			m_pDeleteTipDlg->SetPasswordModel(false);
			m_pDeleteTipDlg->SetTitle("TD 模块错误!", 10000);
			m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
	case WM_SMS_SENDPRO:
		{
			SMS::SMSWarp::SM_PARAM *pParam = (SMS::SMSWarp::SM_PARAM *)wParam;
			CString s = "正在给 ";
			s += pParam->TPA;
			s += " 发送短信...";
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(s, FALSE);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
		break;
	case WM_SMS_STATUS:
		doWithSMS(wParam, lParam);
		break;
	case WM_DELETESELITEM:		//阅读短信
		{
			if(wParam == 1)
			{
				m_bIsDial = TRUE;
			}
			else if(wParam == 2)
			{
				Net3GHungOff();
			}
			else
			{
 				extern int g_iSmsID;
 				if(g_iSmsID >= 0)
 				{
					int index = g_iSmsID>>1;
					if(g_iSmsID & 1)			//彩信
					{
						C3GSMSDetailDlg *pWnd_ = m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
						pWnd_->initDataBase(MMS_READ, index, FALSE);
 						pWnd_->ShowWindow(SW_SHOW);
					}
					else
					{
						C3GSMSDetailDlg *pWnd_ = m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
 						pWnd_->initDataBase(SMS_READ, index, FALSE);
 						pWnd_->ShowWindow(SW_SHOW);
					}
 					g_iSmsID = -1;
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
				m_pPasswordDlg->SetType(CHECK_PINPASSWORD);
				m_pPasswordDlg->SetHWnd(this->m_hWnd);
				m_pPasswordDlg->ShowWindow_(SW_SHOW);
			}
			else if(wParam == PIN_PUK || wParam == PIN_FUK_ERROR)
			{
				m_pPasswordDlg->SetType(CHECK_PUKPASSWORD);
				m_pPasswordDlg->SetHWnd(this->m_hWnd);
				m_pPasswordDlg->ShowWindow_(SW_SHOW);
			}
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

void CMultimediaPhoneDlg::EnableLine(BOOL bEnable)
{
	//lxz 20080620
//	m_StatusKey.SetEnableKey(2, bEnable);
//	m_btnLine.EnableWindow(bEnable);
}

void CMultimediaPhoneDlg::AddFSMRules(void)
{
	m_pFSM->registerRule(tsHangOff, teHangOn, tsHangOn, (void (__cdecl *const )(void *)) HangOn);
	m_pFSM->registerRule(tsHangOff, teRing, tsRing, (void (__cdecl *const )(void *)) Ring);

	m_pFSM->registerRule(tsHangOff, teCallID, tsRing, (void (__cdecl *const )(void *)) CallID);

	m_pFSM->registerRule(tsRing, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
	m_pFSM->registerRule(tsRing, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsRing, teRing, tsRing, (void (__cdecl *const )(void *)) Ring);
	m_pFSM->registerRule(tsRing, teCallID, tsRing, (void (__cdecl *const )(void *)) CallID);	
//  m_pFSM->registerRule(tsRing, teCallID, tsCallID, (void (__cdecl *const )(void *)) CallID);	
// 
//  m_pFSM->registerRule(tsCallID, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
//  m_pFSM->registerRule(tsCallID, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
// 	m_pFSM->registerRule(tsCallID, teRing, tsCallID, (void (__cdecl *const )(void *)) Ring);

	m_pFSM->registerRule(tsConnect, teCallID, tsConnect, (void (__cdecl *const )(void *)) CallID);
	m_pFSM->registerRule(tsConnect, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);

	m_pFSM->registerRule(tsConnect, teKey, tsConnect, (void (__cdecl *const )(void *)) SubKey);

	m_pFSM->registerRule(tsHangOn, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsHangOn, teKey, tsKey, (void (__cdecl *const )(void *)) Key);
	m_pFSM->registerRule(tsKey, teConnect, tsConnect, (void (__cdecl *const )(void *)) Connect);//拨打时，超时即为接通
	m_pFSM->registerRule(tsKey, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
	m_pFSM->registerRule(tsKey, teKey, tsKey, (void (__cdecl *const )(void *)) Key);
		
	m_pFSM->setStartState(tsHangOff);
}

void CMultimediaPhoneDlg::HangOff(void* param)
{
//	::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x123);
//	::SetTimer((theApp.m_pMainWnd)->m_hWnd, 0x123, 50, NULL);    //挂机   lxz
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->OnTimer(0x123);

// 	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(FALSE);
// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(param);
// 
// 	//未接接听，自动关闭窗口时模拟挂机事件
// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SendOutEvnet(WM_TEL_HUNGOFF, 1);
}

void CMultimediaPhoneDlg::HangOn(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, 0x124, 50, NULL);		//摘机

// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
// 	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(param);
}

void CMultimediaPhoneDlg::Key(void* param)
{
	::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
	::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer, KEYTIMEOUT, 0);
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Key_(param);
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

	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
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
		m_pFSM->fireEvent(teConnect, 0);
	}
	else if (nIDEvent == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer)
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiRingTimer);
// 		if ((m_pFSM->getCurrentState() == tsCallID) || (m_pFSM->getCurrentState() == tsRing))
		if (m_pFSM->getCurrentState() == tsRing)
		{
			m_pFSM->fireEvent(teHangOff, 0);
		}
	}

	else if(nIDEvent == IDT_CONTROLBACKLIGHT_TIME+1)
	{
		//lxz test 数据库 内存泄露
	//	SetAlarmList();
	//	m_pMainDlg->FindTodayAlarm();
	//	std::vector<boost::shared_ptr<Data::Scheduler> > result = Data::Scheduler::GetFromDatabase("");
/*
		std::string order = "startTime";
    	std::string filter = "startTime BETWEEN '20090201000000' AND '20090201235959'";
		std::vector<boost::shared_ptr<Data::Scheduler> > result = Data::Scheduler::GetFromDatabaseByOrder(filter, order); 
*/
//		std::vector<boost::shared_ptr<Data::Contact> > lResult = Data::Contact::GetFromDatabase("");

		extern VOID WriteLog(CHAR* str);
		char restartTimer[64];
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		DWORD value = DMemprintf("");
		sprintf(restartTimer, "mem:%04d-%02d-%02d %02d:%02d:%02d %x\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, value);
		WriteLog(restartTimer);
		
		//刷新闹铃
		//m_pMainDlg->FindTodayAlarm();   //20090221   移到设置闹铃列表中

		gIsToLight = FALSE;
		SetNightControlBackLightTimer();
	}

	else if(nIDEvent == IDT_CONTROLBACKLIGHT_TIME)		//进入黑屏
	{
		KillTimer(IDT_CONTROLBACKLIGHT_TIME);
		
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(0);
		Dprintf("Black BackLight\r\n");
	}
	else if(nIDEvent == IDT_MAIN_SAVESCREEN_TIME)		//进入屏保
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
		pWnd->m_mainScreenSaveDlg_->SetSaveScreenType(type, TRUE);	  //1 图片屏保   0  时间屏保
		pWnd->SendMessage(WM_CHANGEWINDOW, (WPARAM)6, SW_SHOW);   //切换到屏保
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
			CString content_ = "内容: ";
			content_ += content.c_str();
			CTime tm = m_result->startTime();
			CString sTime;
			sTime.Format(_T("时间: %04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
			m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
			m_AlarmShowDlg->SetTxt(nOldAlarmID, sTime, "标题: ", content_, ring);
			m_AlarmShowDlg->ShowWindow_(SW_SHOW);
		}
	}
	else if(nIDEvent == 0x123)		//挂机
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x123);

		::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(FALSE);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(NULL);
		
		//未接接听，自动关闭窗口时模拟挂机事件
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SendOutEvnet(WM_TEL_HUNGOFF, 1);
	}
	else if(nIDEvent == 0x124)		//摘机
	{
		::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);

		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(NULL);
	}
	else if(nIDEvent == 0x125)
	{
		SetNetTelStatus();   //20090221
	}

	/* 改成消息检测
	else if(nIDEvent == IDT_DETECTUSB)	//检测SD, USB
	{
		BOOL bIsHaveUSB = DetectDIR(L"UsbDisk");
		BOOL bIsSD = DetectDIR(L"StorageCard");
		int flag = 0;
		if(bIsHaveUSB != m_bIsHaveUSB)
		{
			if(bIsHaveUSB)
				flag = 0x8000;
			else 
				flag = 0x8004;
			m_bIsHaveUSB = bIsHaveUSB;
		}

		else if( bIsSD != m_bIsSD)
		{
			if(bIsSD)
				flag = 0x8000;
			else 
				flag = 0x8004;
			m_bIsSD = bIsSD;
		}
		if(flag != 0)
		{
			m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_DEVICECHANGE, flag, 0);
			m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_DEVICECHANGE, flag, 0);
			m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_DEVICECHANGE, flag, 0);
		}
	}
	*/
	CDialog::OnTimer(nIDEvent);
}

void CMultimediaPhoneDlg::OnMainSoftKey(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1:
		OnButtonTime();
		break;
	case 2:
		OnButtonIME();
		break;
	case 3:
		OnButtonLine();
		break;
	case 4:
		OnButtonMain();
		break;
	case 5:
		OnButtonContact();
		break;
	case 6:
		OnButtonContactinfo();
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

void CMultimediaPhoneDlg::SetSkinStyle()
{
	/*
	m_StatusKey.m_pKeys[1].nBitmapId = Data::g_mainstatusIMEBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[2].nBitmapId = Data::g_mainstatusTELBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[3].nBitmapId = Data::g_mainstatusDesktopBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[4].nBitmapId = Data::g_mainstatusCardBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[5].nBitmapId = Data::g_mainstatusCalllistBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[6].nBitmapId = Data::g_mainstatusRecordBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[7].nBitmapId = Data::g_mainstatusInfoBmpID[Data::g_skinstyle];
	m_StatusKey.m_pKeys[8].nBitmapId = Data::g_mainstatusSettingBmpID[Data::g_skinstyle];

	m_StatusKey.m_BitmapToggleID = Data::g_mainstatusToggleBmpID[Data::g_skinstyle];
	m_StatusKey.m_BitmapID = Data::g_mainstatusMainBmpID[Data::g_skinstyle];
	m_StatusKey.m_clrBack = Data::g_mainstatusBackRGB[Data::g_skinstyle];
	*/
}

//振铃 摘机 按键等外部事情发送给 屏保 MP3 VIDEO 听留言窗口
void CMultimediaPhoneDlg::SendOutEvnet(WPARAM w, LPARAM l)
{
	SendMessage(WM_GEN_EVENT);
	CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
	BOOL isShow = !pWnd->m_mainScreenSaveDlg_->IsWindowVisible();
	switch(w)
	{
		case WM_TEL_KEYCODE:
			{
				pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
			}
			break;
		case WM_TEL_RING:
		case WM_TEL_HUNGON:
			{
				pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
				pWnd->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, w, l);
				pWnd->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, w, l);
				pWnd->m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, w, l);
			}
			break;
		case WM_TEL_HUNGOFF:
			{
				pWnd->m_mainVideoDlg_->SendMessage(WM_OUTEVENT, w, l);
				pWnd->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, w, l);
				pWnd->m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, w, l);
				
			}
			break;
		case WM_TOUCH_CLICK:
			{
	//			pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
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
	if(flag2)
	{
		CTime t1 = m_pSettingDlg->m_pSetting->nightControlBlackLightStartTime();
		CTime t2 = m_pSettingDlg->m_pSetting->nightControlBlackLightEndTime();
		
		SYSTEMTIME sTime;
		GetLocalTime(&sTime);
		CTime tm = CTime(sTime);
		CTime tStart = CTime(tm.GetYear(), tm.GetMonth(), tm.GetDay(), t1.GetHour(), t1.GetMinute(), t1.GetSecond());
		CTime tEnd = CTime(tm.GetYear(), tm.GetMonth(), tm.GetDay(), t2.GetHour(), t2.GetMinute(), t2.GetSecond());
		if(tStart > tEnd)
		{
			if(tm >= tStart || tm <= tEnd)
			{
				SetTimer(IDT_CONTROLBACKLIGHT_TIME, 30*1000, NULL);   
				Dprintf("Set NightControlBackLightTimer %d\r\n", 30000);
				ret = TRUE;
				return ret;
			}
		}
		else 
		{
			if(tm > tStart && tm < tEnd)
			{
				SetTimer(IDT_CONTROLBACKLIGHT_TIME, 30*1000, NULL);   
				Dprintf("Set NightControlBackLightTimer %d\r\n", 30000);
				ret = TRUE;
				return ret;
			}
		}
		
		if(!gIsToLight)
		{
			gIsToLight = TRUE;	
			KillTimer(IDT_CONTROLBACKLIGHT_TIME);
			if(!flag1)				//黑屏唤醒一次
			{
				int v = m_pSettingDlg->m_pSetting->blackLightValue();
				v = 7-v;
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(v);
				Dprintf("Night BackLight to Light\r\n");
			}
		}
	}
	return ret;
}

void CMultimediaPhoneDlg::SetControlBackLightTimer()
{
	KillTimer(IDT_CONTROLBACKLIGHT_TIME);
	BOOL flag1 = m_pSettingDlg->m_pSetting->isContrlBlackLight();
	if(flag1)
	{
		CMainDlg *pWnd = (CMainDlg *)GetPanel(IDC_BUTTON_MAIN);
		if(pWnd == NULL)
			return;
		HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
		BOOL flag2 = ::IsWindowVisible(hWnd);
		BOOL flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
		if(flag2 && flag3)
			return;

		int nTimer_[] = {60*1000, 5*60*1000, 10*60*1000, 30*60*1000};
		int nTimer = m_pSettingDlg->m_pSetting->contrlBlackLightWaitTime();
		SetTimer(IDT_CONTROLBACKLIGHT_TIME, nTimer[nTimer_], NULL);
		Dprintf("Set ControlBackLightTimer %d\r\n", nTimer[nTimer_]);
	}
}

BOOL CMultimediaPhoneDlg::CancelBalckLightSaveTimer()
{
//	Dprintf("CancelBalckLightSaveTimer\r\n");
	KillTimer(IDT_MAIN_SAVESCREEN_TIME);
	KillTimer(IDT_CONTROLBACKLIGHT_TIME);
	return TRUE;
}
BOOL CMultimediaPhoneDlg::ReStoreBackLight()
{
	if(m_nBackLightStatus == 0)    //为黑
	{
		SetScreenSaveTimer();

		//恢复背光
		int v = m_pSettingDlg->m_pSetting->blackLightValue();
		v = 7-v;
		if(v == 0)
			v = 1;
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SetBackLight(v);
		return TRUE;
	}
	return FALSE;
}

void CMultimediaPhoneDlg::SetScreenSaveTimer()
{
	//lxz test 20080623
	//return;

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
	if(m_pFSM->getCurrentState() != tsHangOff)
		flag4 = TRUE;
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
		if(m_pMainDlg->m_mainPhotoDlg_)
			m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_USB_MSG, w, 0);
		if(m_pMainDlg->m_mainMp3Dlg_)
			m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_USB_MSG, w, 0);
		if(m_pMainDlg->m_mainVideoDlg_)
			m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_USB_MSG, w, 0);
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
				m_nBackLightStatus = isType;
				return TRUE;
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
	UINT16 volume[] = {0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000}; //(5-nVolume)*0xFFFF/5;
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

			int nVolume = m_pSettingDlg->m_pSetting->smsRingVolume_;
 			phone_->g_RingSound = (volume[nVolume]|(volume[nVolume]<<16));
			phone_->StartRing((LPTSTR)(LPCTSTR)filename, 1);
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
		int nVolume = m_pSettingDlg->m_pSetting->soundVolume();
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

					m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
					m_pDeleteTipDlg->SetPasswordModel(false);
					m_pDeleteTipDlg->SetTitle("接受到一条家庭留言,是否阅读?");
					m_pDeleteTipDlg->ShowWindow_(TRUE);
				}
				else
				{
					BOOL f =TRUE;
// 					if(pMsg->total > 1)
// 					{
// 						pMsg->group = Data::Message::gExternSms;		//家庭百事通的超长短信
// 						pMsg->Insert();
// 						
// 						std::string sfilter = "[group] = " + Util::StringOp::FromInt(Data::Message::gReMoteSMS); 
// 						sfilter += "AND transactionId = ";
// 						sfilter += pMsg->transactionId;
// 						std::vector<boost::shared_ptr<Data::Message> >vMessageCurrentResult = Data::Message::GetFromDatabase(sfilter);
// 						if(vMessageCurrentResult.size() == pMsg->total)			//短信收满
// 						{
// 							std::string content = "";
// 							for(int i = 0; i < pMsg->total; i++)
// 							{
// 								for(int j = 0; j < pMsg->total; j++ )
// 								{
// 									if(vMessageCurrentResult[j]->no == i+1)
// 									{
// 										content += vMessageCurrentResult[j]->unicodeData;
// 									}
// 								}
// 							}
// 							for(i = 0; i < pMsg->total; i++)
// 							{
// 								vMessageCurrentResult[i]->Remove();
// 							}
// 							pMsg->unicodeData = content;
// 						}
// 						else
// 							f = FALSE;
// 					}
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
							int i;
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
				/*
				pMsg->Insert();
				g_iSmsID = pMsg->id();
				SMSSpecRing(0, "");
				m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
				m_pDeleteTipDlg->SetPasswordModel(false);
				m_pDeleteTipDlg->SetTitle("接受到一条短信,是否阅读?");
				m_pDeleteTipDlg->ShowWindow_(TRUE);
				::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
				*/
				
				BOOL f =TRUE;
				if(pMsg->total > 1)
				{
					pMsg->group = Data::Message::gExternSms;		//家庭百事通的超长短信
					pMsg->Insert();
					
					std::string sfilter = "[group] = " + Util::StringOp::FromInt(Data::Message::gExternSms); 
					sfilter += " AND [transactionId] = '";
					sfilter += pMsg->transactionId;
					sfilter += "'";
					std::vector<boost::shared_ptr<Data::Message> >vMessageCurrentResult = Data::Message::GetFromDatabase(sfilter);
					int i = vMessageCurrentResult.size();
					if(vMessageCurrentResult.size() == pMsg->total)			//短信收满
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
					pMsg->Insert();
					g_iSmsID = (pMsg->id()<<1);
					SMSSpecRing(0, "");
					m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
					m_pDeleteTipDlg->SetPasswordModel(false);
					m_pDeleteTipDlg->SetTitle("接受到一条短信,是否阅读?");
					m_pDeleteTipDlg->ShowWindow_(TRUE);
					::SendMessage(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
				}

			}
		}
	
		delete pMsg;
	}
	else if(wParam == WM_SMS_MMS)	//彩信
	{
		Data::MmsMessage* pMsg = (Data::MmsMessage *)lParam;
		pMsg->Insert();
		SMSSpecRing(0, "");

		m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		m_pDeleteTipDlg->SetPasswordModel(false);
		m_pDeleteTipDlg->SetTitle("正在接受一条彩信!");
		m_pDeleteTipDlg->ShowWindow_(TRUE);

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
		if(lParam == 1)		//短信发送成功
		{
			m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
			m_pDeleteTipDlg->SetPasswordModel(false);
			m_pDeleteTipDlg->SetTitle("短信发送成功!");
			m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
		else		//短信发送失败
		{
			m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
			m_pDeleteTipDlg->SetPasswordModel(false);
			m_pDeleteTipDlg->SetTitle("短信发送失败!");
			m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
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
	//	Dprintf("Ras code %d %d %d\r\n", result, w, l);
	DWriteDat("", (UINT8 *)txt, strlen(txt));
	if(result >= RASCS_Interactive && result <= RASCS_PasswordExpired)
		result = result - RASCS_Interactive + 19;
	else if(result >= RASCS_Connected && result <= RASCS_Disconnected)
		result = result - RASCS_Connected + 23;
	
	if(result > 25)
		result = 25;
	
	if(result == 23)   //连接上
	{
		m_MJPGList.SetUnitBitmap(12, ".\\adv\\mjpg\\k1\\common\\3G\\gprs状态.bmp", "", TRUE);
		m_nTELRigster = TELRIGSTER_DIALED;
	}
	else if(result == 24)   //未连接
	{
		m_nTELRigster = TELRIGSTER_TD;
		if(0)			//是否需要继续拨号  姜丹 13917967645   69
			m_bIsDial = FALSE;

// 		m_pDeleteTipDlg->SetTitle(L"拨号上网不成功...", FALSE);
// 		m_pDeleteTipDlg->SetHWnd(m_hWnd);
// 		m_pDeleteTipDlg->ShowWindow_(TRUE);
	
		if(m_nAPN.apn_type == (int)(CMWAP))
		{
			std::string fileter = "type = ";
			fileter += Util::StringOp::FromInt(Data::MMSData::tpReadySend);
			std::vector<boost::shared_ptr<Data::MMSData> > mmsSendResult = Data::MMSData::GetFromDatabase(fileter);
			for(int i = 0; i < mmsSendResult.size(); i++)
			{
				mmsSendResult[i]->type = Data::MMSData::tpUnSend;
				mmsSendResult[i]->Update();
			}
		}
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
		m_MJPGList.SetUnitBitmap(12, "", "", TRUE);
		Sleep(3000);
		SetAPN((int)CMWAP);
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

//手机搜网
void CMultimediaPhoneDlg:: doSerachrTDNet()
{
	DWORD	dwStart   =   GetTickCount();
	phone_->Bind_(m_pATCommandWarp1);
	m_pSMSWarp->Bind_(m_pATCommandWarp1);

	int ret = m_pATCommandWarp1->Connect("");

	phone_->Bind(m_pATCommandWarp1);
	m_pSMSWarp->Bind(m_pATCommandWarp1);

	if (ret == 1)    //1 成功    
	{
		DWORD offset = GetTickCount() - dwStart;   
	   	Dprintf("3G 搜网 %d Ok\r\n", offset);

	//	phone_->Bind(m_pATCommandWarp1);
		//	m_pSMSWarp->Bind(m_pATCommandWarp1);

		m_nTELRigster = TELRIGSTER_TD;

		m_pSMSWarp->SetOTANumber(m_pSettingDlg->m_pSetting->speCode11_);			//设置OTA号码
		
		m_sMobileNumber = phone_->GetNumber();
// 		if(m_sMobileNumber == "")    //lxz 20090827
// 		{
// 			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"请设置SIM卡的本机手机号码!", 15000);
// 			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
// 			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
// 		}
		
		//add by qi 2009_08_03

// 		std::vector<Util::ATCommandWarp::SIM_FORMAT> vGsim;
// 		m_pATCommandWarp1->ReadSimPhoneInfo(vGsim);//取出sim卡里的信息电话和姓名
// 		InsertSimToContact(vGsim);
// 		m_pContactDlg->ShowItemsInList();


//		AfxMessageBox(L"搜索TD网络成功!");
	}
	else if(ret == 2) //2   需要注册码
	{

	}
	else if(ret == 3)   //未插卡
	{
		m_nTELRigster = TELRIGSTER_FAILED;

	}
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

// 	if(m_nTELRigster >= TELRIGSTER_DIALED)
// 		return TRUE;
// 	return FALSE;
	

	
	if(m_nTELRigster >= TELRIGSTER_DIALED && m_nAPN.apn_type == (int)CMWAP)
		return TRUE;
	else if(m_nTELRigster >= TELRIGSTER_DIALED)
	{
		Net3GHungOff();
		return FALSE;
	}
	else
	{
		SetAPN((int)CMWAP);
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
				/*
				else if ( strfl.Find(L".bmp") > 0 || strfl.Find(L".BMP") > 0 || strfl.Find(L".jpg") > 0 || strfl.Find(L".JPG") > 0\
					|| strfl.Find(L".gif") > 0 || strfl.Find(L".GIF") > 0 )
				{
					//解析smil	
					sV = FilePath + strfl;
				}
				*/
			}
		}
		while(FindNextFile(hfind,&fd));
	}
	MMS::MMSWarp::GetMMSWarp()->AddPar(&r, Util::StringOp::FromCString(sI).c_str(), Util::StringOp::FromCString(sT).c_str(), Util::StringOp::FromCString(sA).c_str());
	return;
}

void CMultimediaPhoneDlg:: doWithDownLoad()
{
	/*std::string filter = "state = '" + Util::StringOp::FromInt(Date::OtaMessage::stUnDownload) + "'";*/

	//
	BOOL flag = FALSE;
	std::string fileter = "type = ";
	fileter += Util::StringOp::FromInt(Data::MMSData::tpReadySend);
	std::vector<boost::shared_ptr<Data::MMSData> > mmsSendResult = Data::MMSData::GetFromDatabase(fileter);
	for(int i = 0; i < mmsSendResult.size(); i++)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"正在发送彩信...", 2000);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

		if(!GetIs3GDialed())
		{
			m_bIsDial = TRUE;
			return;
		}

		flag = TRUE;

		//彩信的发送

		std::string number_;
		std::string number = mmsSendResult[i]->RecipientAddress;
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
				CString s = "正在给 ";
				s += number.c_str();
				s += " 发送彩信...";
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(s, 5000);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

				MMS::MMSWarp::MMS_SubmitRequest r;
				
				r.TransactionID = mmsSendResult[i]->TransactionId;
				r.Subject = Util::StringOp::ToUTF8(mmsSendResult[i]->Subject);
				r.RecipientAddress = number_;
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
				pHttpParam->type = 2;			//发彩信
				pHttpParam->apn_type = m_nAPN.apn_type;
				pHttpParam->apn_proxy = m_nAPN.proxy;
				pHttpParam->pData = buf;
				pHttpParam->dataLentg = len;
				pHttpParam->url = m_nAPN.http; 
				
				extern int HttpProcesse(void *pParam);
				int ret = HttpProcesse(pHttpParam);
				delete []buf;
				if(ret == 0)
				{
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"彩信发送成功!", 5000);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
					mmsSendResult[i]->type = Data::MMSData::tpSend;
					mmsSendResult[i]->Update();
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
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(s, FALSE);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
					mmsSendResult[i]->type = Data::MMSData::tpUnSend;
					mmsSendResult[i]->Update();
				}
				Sleep(50);
			}
			
			if(flag)
				break;
		}while(true);

		/*
		MMS::MMSWarp::MMS_SubmitRequest r;
		
		r.TransactionID = mmsSendResult[i]->TransactionId;
		r.Subject = Util::StringOp::ToUTF8(mmsSendResult[i]->Subject);
		r.RecipientAddress = mmsSendResult[i]->RecipientAddress;
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
		pHttpParam->type = 2;			//发彩信
		pHttpParam->apn_type = m_nAPN.apn_type;
		pHttpParam->apn_proxy = m_nAPN.proxy;
		pHttpParam->pData = buf;
		pHttpParam->dataLentg = len;
		pHttpParam->url = m_nAPN.http; 
		
		extern int HttpProcesse(void *pParam);
		int ret = HttpProcesse(pHttpParam);
		delete []buf;
		if(ret == 0)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"彩信发送成功!", 5000);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
			mmsSendResult[i]->type = Data::MMSData::tpSend;
			mmsSendResult[i]->Update();
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
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(s, FALSE);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
			mmsSendResult[i]->type = Data::MMSData::tpUnSend;
			mmsSendResult[i]->Update();
		}
		*/
	}

	//下载OTA
	std::vector<boost::shared_ptr<Data::OtaMessage> > result = Data::OtaMessage::GetFromDatabase(""); 
	for(i = 0; i < result.size(); i++)
	{
		if(result[i]->state == Data::OtaMessage::stUnDownload)
		{
			if(!GetIs3GDialed())
			{
				m_bIsDial = TRUE;
				return;
			}
			flag = TRUE;
			HttpParam *pHttpParam = new HttpParam;
			pHttpParam->type = 0;
			pHttpParam->url = (char *)result[i]->url.c_str();
			pHttpParam->apn_type = m_nAPN.apn_type;
			pHttpParam->apn_proxy = m_nAPN.proxy;
			
			//http 下载
			if(result[i]->instruction == Data::tMenuManager)				    //菜单维护
			{
				pHttpParam->dwType = 2;
			}
			else if(result[i]->instruction == Data::tWeather)					//天气预报
			{
				pHttpParam->dwType = 1;
			}
			else if(result[i]->instruction == Data::tMultimediaDownload)	    //媒体下载
			{
				pHttpParam->dwType = 0;
			}
			if(HttpProcesse(pHttpParam) == 0)
			{
				result[i]->state = Data::OtaMessage::stParsed;
				result[i]->Update();
				SMSSpecRing(1, "");
			}
			else
			{
				result[i]->state = Data::OtaMessage::stDownloadedError;
				result[i]->Update();
			}
		}
		else if(result[i]->state == Data::OtaMessage::stParsed)
		{
			result[i]->Remove();
		}
	}

	//1528001948

	//下载彩信
	std::vector<boost::shared_ptr<Data::MmsMessage> > result_ = Data::MmsMessage::GetFromDatabase(""); 
	for(i = 0; i < result_.size(); i++)
	{
		if(result_[i]->state == Data::MmsMessage::stUnDownload)
		{
			if(!GetIs3GDialed())
			{
				m_bIsDial = TRUE;
				return;
			}
			flag = TRUE;
			HttpParam *pHttpParam = new HttpParam;
			pHttpParam->type = 0;			
			pHttpParam->dwType = 10;		//彩信下载
			pHttpParam->url = (char *)result_[i]->url.c_str();
			pHttpParam->apn_type = m_nAPN.apn_type;
			pHttpParam->apn_proxy = m_nAPN.proxy;
			
			int index;
			if((index = HttpProcesse(pHttpParam)) >= 0)
			{
				g_iSmsID = ((index<<1)+1);

				result_[i]->state = Data::MmsMessage::stParsed;
				result_[i]->Update();

				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"彩信接受成功!", 5000);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

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
				
				extern int HttpProcesse(void *pParam);
				int ret = HttpProcesse(pHttpParam);
			}
			else
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"彩信接受失败!", 10000);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

				result_[i]->state = Data::MmsMessage::stDownloadedError;
				result_[i]->Update();
			}
		}
		else if(result_[i]->state == Data::MmsMessage::stParsed)
		{
			result_[i]->Remove();
		}
	}

	if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_nAPN.apn_type == (int)CMWAP && flag)		//判断是否需要永远连接
	{
		Net3GHungOff();
		Sleep(3000);
		SetAPN((int)CMNET);
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
		pContact->mobilesTelephone(Data::TelephoneNumber(vsim[i].telnum));
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
	m_nSIMID = -1;
	std::vector<boost::shared_ptr<Data::ContactGroup> > result;
	if (Data::ContactGroup::GetDataCount("") > 0)//查找
	{
		result = Data::ContactGroup::GetFromDatabase("");
		for (int i = 0; i < result.size(); ++i)
		{
			if(CString(result[i]->name().c_str()) == L"SIM名片")
			{
				m_nSIMID = result[i]->id();
				break;
			}
		}
	}
	
	if(m_nSIMID == -1)
	{
		//创建SIM卡类别
		boost::shared_ptr<Data::ContactGroup> m_pContactGroup = boost::shared_ptr<Data::ContactGroup>(new Data::ContactGroup);
		m_pContactGroup->name("SIM名片");
		m_pContactGroup->ringName("默认");
		m_pContactGroup->tipName("默认");
		m_pContactGroup->Insert();
		m_nSIMID = m_pContactGroup->id();
	}
	
	//SIM组里有信息，先删除掉
	std::vector<boost::shared_ptr<Data::Contact> > result1 = Data::Contact::GetFromDatabase("groupId = " + Util::StringOp::FromInt(m_nSIMID));
	if (!result1.empty())
	{
		for(int i = 0; i < result1.size(); i++)
			result1[i]->Remove();
	}
	
	//把SIM 信息插入到 Contact-sim卡组里
	for (int i = 0 ; i < vsim.size() ; i++)
	{
		boost::shared_ptr<Data::Contact> pContact;
		pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
		pContact->name(vsim[i].name);
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
		pContact->memo(Util::StringOp::FromCString(L""));
		pContact->type(Data::ctNormal);
		pContact->groupId(m_nSIMID);
		pContact->Insert();	
	}
//	m_procbar.SetPos(0);
//	m_procbar.ShowWindow(SW_SHOW);

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
				alias += ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex[index];	
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


