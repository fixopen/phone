// MultimediaPhoneDlg.cpp : implementation file
//
#include "stdafx.h"
#include <ntddndis.h>
#include "Pblmember.h"
#include "MultimediaPhone.h"
#include "MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "inc/ximage.h"
#include "Util/socket.h"
#include "View/ContactDlg.h"
#include "View/InformationDlg.h"
#include "View/SettingDlg.h"
#include "View/ContactNewDlg.h"
#include "View/ViewConstants.h"
#include "Data/OTAData.h"
#include "Data/LanguageResource.h"
#include "Data/SkinStyle.h"
#include "data/Message.h"
#include "data/Otadata.h"
#include "data/OtaMessage.h"
#include "data/MmsMessage.h"
#include "sip.h"

//#define WM_PLAYVIDEO  8002
#define  BMP_WIDTH           800
#define  BMP_HEIGHT          420
#define  BMP_WIDTH1          692
#define  BMP_HEIGHT1         296

#define  BMP_WIDTH2          800
#define  BMP_HEIGHT2         60
#define  BMP_WIDTH3          600  //535
#define  BMP_HEIGHT3         420
#define  BMP_WIDTH4          800
#define  BMP_HEIGHT4         480
#define  BMP_WIDTH5          535
#define  BMP_HEIGHT5         420
#define  BMP_WIDTH6          800
#define  BMP_HEIGHT6         423
#define  BMP_WIDTH7          800
#define  BMP_HEIGHT7         92
#define  BMP_WIDTH8          440
#define  BMP_HEIGHT8         270
#define  BMP_WIDTH9          317
#define  BMP_HEIGHT9         376
#define  BMP_WIDTH10         317
#define  BMP_HEIGHT10        375
#define  BMP_WIDTH11         317
#define  BMP_HEIGHT11        375
#define  BMP_WIDTH12         440
#define  BMP_HEIGHT12        440

#define     G3_MODULOR       1

//add by qi 2009_09_15
#define  DATE_SET_TIEMR     10  


#define  KEYTIMEOUT  60 * 60 * 1000
#define  RINGTIMEOUT  6 * 1000
extern  BOOL DetectDIR(TCHAR* sDir);
BOOL g_MemoryLeak = FALSE;
BOOL g_IsRun = FALSE;
BOOL g_bAdjustPanel = FALSE;
CRITICAL_SECTION csCom;

extern int HttpProcesse(void* pParam);
void CopyDirFiles() {
    CString SrcDir = usbdisk_dir;

    CString DesDir = ptchLocalPhoto;
    CString findFilename = SrcDir + "/*.*";
    //wprintf(_T("find file name is %s\n"), (LPCTSTR)findFilename);
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        Dprintf("not find file\n");
    } else {
        bool finished = false;
        do {
            wchar_t wideToName[256];
            //  wchar_t root[256] = {0};
            //  mbstowcs(root, rootPath.c_str(), rootPath.length());
            wsprintf(wideToName, _T("%s/%s"), DesDir, (LPCTSTR)FindFileData.cFileName); //findFileName
            //  wideToName =  
            wchar_t wideFromName[256] = {
                0
            };
            //  wchar_t usb[256] = {0};
            wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName

            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                //wprintf(_T("create dir %s to %s\n"), wideFromName, wideToName);
                //CopyDirFiles(wideFromName, wideToName);
            } else {
                if (!::CopyFile(wideFromName, wideToName, false)) {
                    Dprintf("current error is %d.", GetLastError());
                }
                //wprintf(_T("copy %s to %s\n"), wideFromName, wideToName);
            }

            if (!FindNextFile(hFind, &FindFileData)) {
                if (GetLastError() == ERROR_NO_MORE_FILES) {
                    Dprintf("find end.\n");
                    finished = true;
                } else {
                    Dprintf("Couldn't find next file.\n");
                }
            }
        } while (!finished);
        FindClose(hFind);
    }
}

int ALLBMPMEMORY_MAX = 0;
int g_ALLBMPMemoryCount = -1;
struct  ALLBmpMemoryInfo {
    CString sFileName;
    HDC hdc;
    HBITMAP hbmp;
    HBITMAP hOldbmp;
    CRect rect;
};

ALLBmpMemoryInfo* g_allBmpMemory;
void InitALLBmpMemoryInfo() {
    MEMORYSTATUS mem;     
    mem.dwLength = sizeof(mem);     
    GlobalMemoryStatus(&mem);     
    if (mem.dwAvailPhys >= 64 * 1024 * 1024) {
        ALLBMPMEMORY_MAX = 0;     //   15   remark 
        Dprintf("BMP Allac 30\r\n");
    } else {
        Dprintf("BMP Allac 0\r\n");
    }

    if (ALLBMPMEMORY_MAX > 0) {
        g_allBmpMemory = new ALLBmpMemoryInfo[ALLBMPMEMORY_MAX];
    }

    for (int i = 0; i < ALLBMPMEMORY_MAX; i++) {
        g_allBmpMemory[i].sFileName = "";
        g_allBmpMemory[i].hdc = NULL;
        g_allBmpMemory[i].rect = CRect(0, 0, 0, 0);
        g_allBmpMemory[i].hbmp = NULL;
        g_allBmpMemory[i].hOldbmp = NULL;
    }
}

BOOL FindMemoryInfo(CString sAllFilename, int& inDexAllBmpMemeoryinfo) {
    for (int i = 0; i <= g_ALLBMPMemoryCount; i++) {
        if (g_allBmpMemory[i].sFileName == sAllFilename) {
            inDexAllBmpMemeoryinfo = i;

            //  wprintf(g_allBmpMemory[i].sFileName);
            //          Dprintf(" FindAllMemeory %d \r\n", i);

            return TRUE;
        }
    }
    return FALSE;
}

BOOL AddALLBmpMemoryInfo(CString sFileName, CRect rect, CDC* pdc) {
    int n;
    if (FindMemoryInfo(sFileName, n)) {
        return FALSE;
    }

    if (g_ALLBMPMemoryCount < (ALLBMPMEMORY_MAX - 1)) {
        g_ALLBMPMemoryCount++;

        g_allBmpMemory[g_ALLBMPMemoryCount].sFileName = sFileName;
        g_allBmpMemory[g_ALLBMPMemoryCount].rect = rect;
        g_allBmpMemory[g_ALLBMPMemoryCount].hdc = CreateCompatibleDC(pdc->m_hDC); 

        BYTE* lpBitmapBits = NULL;
        BITMAPINFO RGB24BitsBITMAPINFO; 
        ZeroMemory(&RGB24BitsBITMAPINFO, sizeof(BITMAPINFO));
        RGB24BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        RGB24BitsBITMAPINFO.bmiHeader.biWidth = rect.Width();
        RGB24BitsBITMAPINFO.bmiHeader.biHeight = rect.Height();
        RGB24BitsBITMAPINFO.bmiHeader.biPlanes = 1;
        RGB24BitsBITMAPINFO.bmiHeader.biBitCount = 24;

        g_allBmpMemory[g_ALLBMPMemoryCount].hbmp = CreateDIBSection(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, (BITMAPINFO *)&RGB24BitsBITMAPINFO, DIB_RGB_COLORS, (void**)&lpBitmapBits, NULL, 0);
        g_allBmpMemory[g_ALLBMPMemoryCount].hOldbmp = (HBITMAP)SelectObject(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, g_allBmpMemory[g_ALLBMPMemoryCount].hbmp);
        BitBlt(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, 0, 0, rect.Width(), rect.Height(), pdc->m_hDC, 0, 0, SRCCOPY);

        //  wprintf(sFileName);
        Dprintf(" AddAllMemeory %d \r\n", g_ALLBMPMemoryCount);

        return TRUE;
    }
    return FALSE;
}

BOOL AddALLBmpMemoryInfo(CString sFileName, CRect rect, HDC pdc) {
    int n;
    if (FindMemoryInfo(sFileName, n)) {
        return FALSE;
    }

    if (g_ALLBMPMemoryCount < (ALLBMPMEMORY_MAX - 1)) {
        g_ALLBMPMemoryCount++;

        g_allBmpMemory[g_ALLBMPMemoryCount].sFileName = sFileName;
        g_allBmpMemory[g_ALLBMPMemoryCount].rect = rect;
        g_allBmpMemory[g_ALLBMPMemoryCount].hdc = CreateCompatibleDC(pdc); 

        BYTE* lpBitmapBits = NULL;
        BITMAPINFO RGB24BitsBITMAPINFO; 
        ZeroMemory(&RGB24BitsBITMAPINFO, sizeof(BITMAPINFO));
        RGB24BitsBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        RGB24BitsBITMAPINFO.bmiHeader.biWidth = rect.Width();
        RGB24BitsBITMAPINFO.bmiHeader.biHeight = rect.Height();
        RGB24BitsBITMAPINFO.bmiHeader.biPlanes = 1;
        RGB24BitsBITMAPINFO.bmiHeader.biBitCount = 24;

        g_allBmpMemory[g_ALLBMPMemoryCount].hbmp = CreateDIBSection(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, (BITMAPINFO *)&RGB24BitsBITMAPINFO, DIB_RGB_COLORS, (void**)&lpBitmapBits, NULL, 0);
        g_allBmpMemory[g_ALLBMPMemoryCount].hOldbmp = (HBITMAP)SelectObject(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, g_allBmpMemory[g_ALLBMPMemoryCount].hbmp);
        BitBlt(g_allBmpMemory[g_ALLBMPMemoryCount].hdc, 0, 0, rect.Width(), rect.Height(), pdc, 0, 0, SRCCOPY);

        //  wprintf(sFileName);
        Dprintf(" AddAllMemeory %d \r\n", g_ALLBMPMemoryCount);

        return TRUE;
    }
    return FALSE;
}

BOOL ReleaseALLBmpMemoryInfo() {
    for (int i = 0; i <= g_ALLBMPMemoryCount; i++) {
        SelectObject(g_allBmpMemory[i].hdc, g_allBmpMemory[i].hOldbmp);
        DeleteObject(g_allBmpMemory[i].hbmp);
        DeleteDC(g_allBmpMemory[i].hdc);
    }
    g_ALLBMPMemoryCount = -1;
    return TRUE;
}

#include   <INITGUID.h>   
#include   <imaging.h> 
IImagingFactory* pImgFactory;

void InitImage() {
    CoInitializeEx(NULL, COINIT_MULTITHREADED); 
    //  CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory);
}

void DrawImage(LPCTSTR szFileImage, CDC* pdc, CRect rect, BOOL isOffset = FALSE) {
    //  unsigned int w = DMemprintf("DrawImage 0");
    CRect newRect = rect;
    if (isOffset) {
        newRect = CRect(0, 0, rect.Width(), rect.Height());
    }

    CString sFile = szFileImage;
    if (sFile.Find(L".bmp") != -1 || sFile.Find(L".BMP") != -1) {
        //��bmp�ļ�ͷ������  

        CFile file;   
        if (!file.Open(szFileImage, CFile::modeRead)) {
            return;
        }  

        BITMAPFILEHEADER bfhHeader;   
        file.Read(&bfhHeader, sizeof(BITMAPFILEHEADER));   
        if (bfhHeader.bfType != ((WORD)('M' << 8) | 'B')) {
            ::MessageBox(NULL, L"The   file   is   not   a   file!", L"warning", MB_OK);  
            file.Close();
            return;
        }   
        if (bfhHeader.bfSize != file.GetLength()) {
            ::MessageBox(NULL, L"The   BMP   file   header   error!", L"warning", MB_OK);   
            file.Close();
            return;
        }   
        UINT uBmpInfoLen = (UINT)bfhHeader.bfOffBits - sizeof(BITMAPFILEHEADER);   
        LPBITMAPINFO lpBitmap = (LPBITMAPINFO)   new   BYTE[uBmpInfoLen];  

        file.Read((LPVOID)lpBitmap, uBmpInfoLen);   
        if ((*(LPDWORD)(lpBitmap)) != sizeof(BITMAPINFOHEADER)) {
            ::MessageBox(NULL, L"The   BMP   is   not   Windows   3.0   format!", L"warning", MB_OK);  
            file.Close();
            delete[]lpBitmap;    //20090216 test
            return;
        }     
        DWORD dwBitlen = bfhHeader.bfSize - bfhHeader.bfOffBits;     
        LPVOID lpBits;   
        //TRACE(L"file DC 1 %d\n", offset);

        /*
        �������ͼ�� 800*420��692*296��535*420��800*60��Unit��С�����ǣ���ȡ����Ӧ��BMP������.����������
        */
        if (((lpBitmap->bmiHeader.biWidth == BMP_WIDTH && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT) && (!(rect.Width() >= BMP_WIDTH && rect.Height() >= BMP_HEIGHT))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH1 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT1) && (!(rect.Width() >= BMP_WIDTH1 && rect.Height() >= BMP_HEIGHT1))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH2 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT2) && (!(rect.Width() >= BMP_WIDTH2 && rect.Height() >= BMP_HEIGHT2))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH3 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT3) && (!(rect.Width() >= BMP_WIDTH3 && rect.Height() >= BMP_HEIGHT3))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH5 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT5) && (!(rect.Width() >= BMP_WIDTH5 && rect.Height() >= BMP_HEIGHT5))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH4 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT4) && (!(rect.Width() >= BMP_WIDTH4 && rect.Height() >= BMP_HEIGHT4)))) {
            ;
        } else {
            BOOL flag = FALSE;
            if (BMP_WIDTH == lpBitmap->bmiHeader.biWidth || BMP_WIDTH1 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH2 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH3 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH5 == lpBitmap->bmiHeader.biWidth) {
                flag = TRUE;

                int index;;
                if (FindMemoryInfo(szFileImage, index)) {
                    if (index >= 0 && index < ALLBMPMEMORY_MAX) {
                        BitBlt(pdc->m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);


                        file.Close();
                        return;
                    }
                }
            }

            file.Close();
            delete[]lpBitmap;    //20090216 test

            IImage* pImage = NULL; 

            //  CoInitializeEx(NULL,   COINIT_MULTITHREADED); 

            //   Create   the   imaging   factory. 
            if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImgFactory))) {
                //   Load   the   image   from   the   JPG   file. 
                if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) {
                    //   Draw   the   image. 
                    pImage->Draw(pdc->m_hDC, rect, NULL); 
                    pImage->Release(); 

                    if (flag) {
                        AddALLBmpMemoryInfo(szFileImage, rect, pdc);
                    }

                    ::Sleep(1);
                } else {
                    Dprintf("DrawImage error\n");
                }

                pImgFactory-> Release();
            }

            return;
        }
        //����HBITMAP���   

        /*
        ���¸���rect ����λͼ��Ϣ
        */

        int w = (((lpBitmap->bmiHeader.biWidth* lpBitmap->bmiHeader.biBitCount) + 31) >> 5) << 2;
        int h = lpBitmap->bmiHeader.biHeight;
        lpBitmap->bmiHeader.biHeight = rect.Height();
        lpBitmap->bmiHeader.biWidth = rect.Width();

        CDC dcMem;   
        dcMem.CreateCompatibleDC(pdc);   
        HBITMAP hBmp = ::CreateDIBSection(dcMem.m_hDC, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
        if (lpBits == NULL) {
            //::MessageBox(NULL,L"Draw BMP no memory!", L"warning", MB_OK);  
            //��������

            extern VOID WriteLog(CHAR * str);
            char restartTimer[64] = {
                0
            };
            SYSTEMTIME tm;
            GetLocalTime(&tm);
            sprintf_s(restartTimer, 64, "memleak watchdog:%04d-%02d-%02d %02d:%02d:%02d \r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
            WriteLog(restartTimer);

            file.Close();
            g_MemoryLeak = TRUE;
            delete[]lpBitmap;    //20090216 test
            return;
        }

        //TRACE(L"file DC 2 %d\n", offset);

        /*
        file.ReadHuge(lpBits, dwBitlen);  
        */
        int off = (h - rect.top - rect.Height()) * w + rect.left*(lpBitmap->bmiHeader.biBitCount / 8);

        file.Seek(off, CFile::current);
        int size = (((rect.Width() * lpBitmap->bmiHeader.biBitCount) + 31) >> 5) << 2;   //rect.Width()*3;
        int offline = w - size; 

        for (int i = 0; i < rect.Height(); i++) {
            file.Read(lpBits, size); 
            LPBYTE lpbyte = (LPBYTE)lpBits;
            lpbyte += size;
            lpBits = (LPVOID)lpbyte;
            file.Seek(offline, CFile::current);
        }
        file.Close();       

        //TRACE(L"file DC 3 %d\n", offset);

        CBitmap BmpImg;   
        BmpImg.Attach(hBmp);   

        CBitmap* pOldBmp = dcMem.SelectObject(&BmpImg);   

        //TRACE(L"file DC 4 %d\n", offset);

        //pdc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
        pdc->BitBlt(newRect.left, newRect.top, newRect.Width(), newRect.Height(), &dcMem, 0, 0, SRCCOPY);

        dcMem.SelectObject(pOldBmp); 

        DeleteObject(hBmp);

        BmpImg.Detach();
        dcMem.DeleteDC();

        delete[]lpBitmap;    //20090216 test

        //TRACE(L"end file DC %d\n", offset);
        ::Sleep(1);
    } else {
        IImage* pImage = NULL; 

        //  CoInitializeEx(NULL,   COINIT_MULTITHREADED); 

        //   Create   the   imaging   factory. 
        if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImgFactory))) {
            //   Load   the   image   from   the   JPG   file. 
            if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) {
                //   Draw   the   image. 

                pImage->Draw(pdc->m_hDC, rect, NULL); 
                pImage->Release(); 
                ::Sleep(1);
            } else {
                Dprintf("DrawImage error\n");
            }

            pImgFactory-> Release();
        }
    }
}

void DrawImage_HDC(LPCTSTR szFileImage, HDC pdc, CRect rect, BOOL isOffset = FALSE, VOID* lpParamBits = NULL) {
    //unsigned int w = DMemprintf("DrawImage 0");

    if (wcsstr(szFileImage, (L".png")) > 0) {
        //@@
        return;
        CxImage* image_;
        image_ = new CxImage();
        if (image_) {
            //CDC *pdc = GetDC(); 
            bool ret = image_->Load(szFileImage, CXIMAGE_FORMAT_PNG);
            image_->Draw(pdc, rect);
            //  ReleaseDC(pdc);
            delete image_;
        }
        return;
    }

    CRect newRect = rect;
    if (isOffset) {
        newRect = CRect(0, 0, rect.Width(), rect.Height());
    }

    CString sFile = szFileImage;
    if (sFile.Find(L".bmp") != -1 || sFile.Find(L".BMP") != -1) {
        //��bmp�ļ�ͷ������  

        CFile file;   
        if (!file.Open(szFileImage, CFile::modeRead)) {
            return;
        }  

        BITMAPFILEHEADER bfhHeader;   
        file.Read(&bfhHeader, sizeof(BITMAPFILEHEADER));   
        if (bfhHeader.bfType != ((WORD)('M' << 8) | 'B')) {
            ::MessageBox(NULL, L"The   file   is   not   a   file!", L"warning", MB_OK);  
            file.Close();
            return;
        }   
        if (bfhHeader.bfSize != file.GetLength()) {
            ::MessageBox(NULL, L"The   BMP   file   header   error!", L"warning", MB_OK);   
            file.Close();
            return;
        }   
        UINT uBmpInfoLen = (UINT)bfhHeader.bfOffBits - sizeof(BITMAPFILEHEADER);   
        LPBITMAPINFO lpBitmap = (LPBITMAPINFO)new BYTE[uBmpInfoLen];  

        file.Read((LPVOID)lpBitmap, uBmpInfoLen);   
        if ((*(LPDWORD)(lpBitmap)) != sizeof(BITMAPINFOHEADER)) {
            ::MessageBox(NULL, L"The   BMP   is   not   Windows   3.0   format!", L"warning", MB_OK);  
            file.Close();
            delete[]lpBitmap;    //20090216 test
            return;
        }     
        DWORD dwBitlen = bfhHeader.bfSize - bfhHeader.bfOffBits;     
        LPVOID lpBits;   

        /*
        �������ͼ�� 800*420��692*296��535*420��800*60��Unit��С�����ǣ���ȡ����Ӧ��BMP������.����������
        */
        if (((lpBitmap->bmiHeader.biWidth == BMP_WIDTH && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT) && (!(rect.Width() >= BMP_WIDTH && rect.Height() >= BMP_HEIGHT))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH1 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT1) && (!(rect.Width() >= BMP_WIDTH1 && rect.Height() >= BMP_HEIGHT1))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH2 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT2) && (!(rect.Width() >= BMP_WIDTH2 && rect.Height() >= BMP_HEIGHT2))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH3 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT3) && (!(rect.Width() >= BMP_WIDTH3 && rect.Height() >= BMP_HEIGHT3))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH5 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT5) && (!(rect.Width() >= BMP_WIDTH5 && rect.Height() >= BMP_HEIGHT5))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH6 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT6) && (!(rect.Width() >= BMP_WIDTH6 && rect.Height() >= BMP_HEIGHT6))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH7 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT7) && (!(rect.Width() >= BMP_WIDTH7 && rect.Height() >= BMP_HEIGHT7))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH8 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT8) && (!(rect.Width() >= BMP_WIDTH8 && rect.Height() >= BMP_HEIGHT8))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH9 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT9) && (!(rect.Width() >= BMP_WIDTH9 && rect.Height() >= BMP_HEIGHT9))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH10 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT10) && (!(rect.Width() >= BMP_WIDTH10 && rect.Height() >= BMP_HEIGHT10))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH11 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT11) && (!(rect.Width() >= BMP_WIDTH11 && rect.Height() >= BMP_HEIGHT11))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH12 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT12) && (!(rect.Width() >= BMP_WIDTH12 && rect.Height() >= BMP_HEIGHT12))) || \
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH4 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT4) && (!(rect.Width() >= BMP_WIDTH4 && rect.Height() >= BMP_HEIGHT4)))) {
            ;
        } else {
            BOOL flag = FALSE;
            if (BMP_WIDTH == lpBitmap->bmiHeader.biWidth || BMP_WIDTH1 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH2 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH3 == lpBitmap->bmiHeader.biWidth || BMP_WIDTH5 == lpBitmap->bmiHeader.biWidth) {
                flag = TRUE;

                int index;
                if (FindMemoryInfo(szFileImage, index)) {
                    if (index >= 0 && index < ALLBMPMEMORY_MAX) {
                        BitBlt(pdc, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);

                        file.Close();
                        return;
                    }
                }

                if (lpParamBits) {
                    int size = (((rect.Width() * lpBitmap->bmiHeader.biBitCount) + 31) >> 5) << 2; 
                    file.Read(lpParamBits, size * rect.Height());
                    file.Close();   
                    delete[]lpBitmap;    //20090216 test

                    //  ::Sleep(1);
                    return;
                }

                ////////////////////////////////////////////////////////////////////////////
                int w = (((lpBitmap->bmiHeader.biWidth * lpBitmap->bmiHeader.biBitCount) + 31) >> 5) << 2;
                int h = lpBitmap->bmiHeader.biHeight;
                lpBitmap->bmiHeader.biHeight = rect.Height();
                lpBitmap->bmiHeader.biWidth = rect.Width();

                HDC dcMem = ::CreateCompatibleDC(pdc);   
                HBITMAP hBmp = ::CreateDIBSection(pdc, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
                if (lpBits == NULL) {
                    ::MessageBox(NULL, L"Draw BMP no memory!", L"warning", MB_OK);  
                    //��������
                    //return;
                }


                int off = (h - rect.top - rect.Height()) * w + rect.left * (lpBitmap->bmiHeader.biBitCount / 8);

                file.Seek(off, CFile::current);
                int size = (((rect.Width() * lpBitmap->bmiHeader.biBitCount) + 31) >> 5) << 2;   //rect.Width()*3;
                int offline = w - size; 

                file.Read(lpBits, size * rect.Height());
                file.Close();                       

                HBITMAP pOldBmp = (HBITMAP)::SelectObject(dcMem, hBmp);      //20090723

                ::BitBlt(pdc, newRect.left, newRect.top, newRect.Width(), newRect.Height(), dcMem, 0, 0, SRCCOPY); 
                if (flag) {
                    AddALLBmpMemoryInfo(szFileImage, rect, pdc);
                }//20090723

                ::SelectObject(dcMem, pOldBmp);             //20090723
                ::DeleteObject(hBmp);
                ::DeleteDC(dcMem);                          //20090723

                delete[]lpBitmap;    //20090216 test

                if (flag) {
                    //::Sleep(1);
                    //TRACE(L"DrawImage_HDC DDDDDDDDDDD =  %d\n", offset);

                    return;
                }
                ///////////////////////////////////////////////////////////////////////////

                //wprintf(szFileImage);
                //Dprintf( " All Draw \n");
                if (FindMemoryInfo(szFileImage, index)) {
                    if (index >= 0 && index < ALLBMPMEMORY_MAX) {
                        BitBlt(pdc, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);

                        file.Close();
                        return;
                    }
                }
            }

            file.Close();
            delete[]lpBitmap;    //20090216 test

            IImage* pImage = NULL; 

            //  CoInitializeEx(NULL,   COINIT_MULTITHREADED); 

            //   Create   the   imaging   factory. 
            if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImgFactory))) {
                //   Load   the   image   from   the   JPG   file. 
                if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) {
                    //   Draw   the   image. 
                    pImage->Draw(pdc, rect, NULL); 
                    pImage->Release(); 

                    if (flag) {
                        AddALLBmpMemoryInfo(szFileImage, rect, pdc);
                    }

                    ::Sleep(1);
                } else {
                    Dprintf("DrawImage error\n");
                }

                pImgFactory-> Release();
            }

            return;
        }
        //����HBITMAP���   

        /*
        ���¸���rect ����λͼ��Ϣ
        */

        int w = (((lpBitmap->bmiHeader.biWidth * lpBitmap->bmiHeader.biBitCount) + 31) >> 5) << 2;
        int h = lpBitmap->bmiHeader.biHeight;
        lpBitmap->bmiHeader.biHeight = rect.Height();
        lpBitmap->bmiHeader.biWidth = rect.Width();

        HDC dcMem = ::CreateCompatibleDC(pdc);   
        HBITMAP hBmp = ::CreateDIBSection(dcMem, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
        if (lpBits == NULL) {
            ::MessageBox(NULL, L"Draw BMP no memory!", L"warning", MB_OK);  
            //��������

            //return;
        }

        /*
        file.ReadHuge(lpBits, dwBitlen);  
        */
        int off = (h - rect.top - rect.Height()) * w + rect.left * (lpBitmap->bmiHeader.biBitCount / 8);

        file.Seek(off, CFile::current);
        int size = (((rect.Width() * lpBitmap->bmiHeader.biBitCount) + 31) >> 5) << 2;   //rect.Width()*3;
        int offline = w - size; 

        for (int i = 0; i < rect.Height(); ++i) {
            file.Read(lpBits, size); 
            LPBYTE lpbyte = (LPBYTE)lpBits;
            lpbyte += size;
            lpBits = (LPVOID)lpbyte;
            file.Seek(offline, CFile::current);
        }
        file.Close();       

        //TRACE(L"file DC 3 %d\n", offset);

        HBITMAP pOldBmp = (HBITMAP)::SelectObject(dcMem, hBmp);   


        //pdc->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
        ::BitBlt(pdc, newRect.left, newRect.top, newRect.Width(), newRect.Height(), dcMem, 0, 0, SRCCOPY);

        ::SelectObject(dcMem, pOldBmp); 

        ::DeleteObject(hBmp);
        ::DeleteDC(dcMem);

        delete[]lpBitmap;    //20090216 test

        ::Sleep(1);
    } else {
        IImage* pImage = NULL; 

        //  CoInitializeEx(NULL,   COINIT_MULTITHREADED); 
        //   Create   the   imaging   factory. 
        if (SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&pImgFactory))) {
            //   Load   the   image   from   the   JPG   file. 
            if (SUCCEEDED(pImgFactory->CreateImageFromFile(szFileImage, &pImage))) {
                //   Draw   the   image. 

                pImage->Draw(pdc, rect, NULL); 
                pImage->Release(); 
                ::Sleep(1);
            } else {
                Dprintf("DrawImage error\n");
            }

            pImgFactory->Release();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg dialog
CMultimediaPhoneDlg::CMultimediaPhoneDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMultimediaPhoneDlg::IDD, pParent) {
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

CMultimediaPhoneDlg::~CMultimediaPhoneDlg() {
    for (std::map<int, CDialog*>::iterator i = panels_.begin(); i != panels_.end(); ++i) {
        delete i->second;
    }
}

void CMultimediaPhoneDlg::DoDataExchange(CDataExchange* pDX) {
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

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg message handlers
extern BOOL watchdog_test;
LRESULT CMultimediaPhoneDlg::OnClickMJPG(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    switch (w) {
    case 1:
        break;

    case 2:
        OnButtonLine();     //�绰״̬
        break;
    case 3:
        OnButtonMain();
        //test 20090602
        //CNetStatusDlg::ADSLDial(m_nAPN.dialnumber, "", "", (CWnd *)this, (DIALTYPE)m_nAPN.apn_type);          
        break;

    case 4:
        //δ�ӵ绰
        m_pContactInfoDlg->ShowUnconnectItems();
        m_pContactInfoDlg->ShowWindow(SW_SHOW);
        AddIcon(Allicon[3]);
        break;

    case 5:
        //����Сͼ��
        m_pMainDlg->doReadSMS();
        break;

    case 6:
        //����ǽСͼ��
        //  m_pMainDlg->m_firewalDlg_->ShowWindow(SW_SHOW);
        m_pSettingDlg->IniCtrlData();
        m_pSettingDlg->OnClickMJPG(103, 0);
        m_pSettingDlg->ShowWindow(SW_SHOW);
        AddIcon(Allicon[9]);
        break;

    case 7:
        //�ճ�Сͼ��                        
        m_pMainDlg->m_mainLunarderDlg_->ShowWindow_(SW_SHOW);
        break;

    case 8:
        OnButtonSetting();
        break;
    case 10:
        Desktop();
        break;
    case 15:
        if (m_pFSM->getCurrentState() > tsHangOff) {
            if (!m_pTelephoneDlg->IsWindowVisible()) {
                m_pTelephoneDlg->ShowWindow_(TRUE);
                AddIcon(Allicon[0]);
            }
        }
        break;

    default:
        break;
    }

    m_MJPGList.Invalidate();
    return result;
}

void CMultimediaPhoneDlg::SetAPN(int apn) {
    m_nAPN.apn_type = (int)apn;
    if (apn == 0) {          //CMWAP
        strcpy_s(m_nAPN.dialnumber, 24, m_pSettingDlg->m_pSetting->gprsDialnumber1_.c_str());
        sprintf_s(m_nAPN.proxy, 32, "%s:%d", m_pSettingDlg->m_pSetting->gprsProxyIp1_.c_str(), m_pSettingDlg->m_pSetting->gprsProxyPort1_);
        strcpy_s(m_nAPN.http, 64, m_pSettingDlg->m_pSetting->gprsHttp1_.c_str());
    } else {
        strcpy_s(m_nAPN.dialnumber, 24, m_pSettingDlg->m_pSetting->gprsDialnumber2_.c_str());
        if (m_pSettingDlg->m_pSetting->gprsProxyIp2_ == "") {
            strcpy_s(m_nAPN.proxy, 32, "");
        } else {
            sprintf_s(m_nAPN.proxy, 32, "%s:%d", m_pSettingDlg->m_pSetting->gprsProxyIp2_.c_str(), m_pSettingDlg->m_pSetting->gprsProxyPort2_);
        }
        strcpy_s(m_nAPN.http, 64, m_pSettingDlg->m_pSetting->gprsHttp2_.c_str());
    }
}

BOOL CMultimediaPhoneDlg::IsConnectNet() {
    BOOL flag = TRUE;
    if (m_nTELRigster < TELRIGSTER_DIALING) {
        m_pDeleteTipDlg->m_ntype = 1;
        m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
        m_pDeleteTipDlg->SetPasswordModel(false);
        m_pDeleteTipDlg->SetTitle("��Ҫ�Ȳ������ӣ���ȷ��!", 30000);
        m_pDeleteTipDlg->ShowWindow_(TRUE);
        flag = FALSE;
    } else if (m_nTELRigster == TELRIGSTER_DIALING) {
        m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
        m_pDeleteTipDlg->SetPasswordModel(false);
        m_pDeleteTipDlg->SetTitle("���ڲ������ӣ����Ժ�...", 2000);
        m_pDeleteTipDlg->ShowWindow_(TRUE);
        flag = FALSE;
    }
    return flag;
}

BOOL CMultimediaPhoneDlg::IsUnConnectNet() {
    if (m_nTELRigster >= TELRIGSTER_DIALED) {
        m_pDeleteTipDlg->m_ntype = 2;
        m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
        m_pDeleteTipDlg->SetPasswordModel(false);
        m_pDeleteTipDlg->SetTitle("��Ҫ�˳�����������?��ȷ��!", 30000);
        m_pDeleteTipDlg->ShowWindow_(TRUE);
    }
    return TRUE;
}

static BOOL isFirstReg = TRUE;
void TdDoWithProc() {
    CMultimediaPhoneDlg* pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    pMainDlg->SetAPN((int)CMNET);
    while (true) {
        //  break;

        ::Sleep(50);
        //pMainDlg->m_bIsDial = TRUE;

        if (pMainDlg->m_nTELRigster == TELRIGSTER_UN) {
            pMainDlg->doSerachrTDNet();
        } else if (pMainDlg->m_nTELRigster == TELRIGSTER_TD && pMainDlg->m_bIsDial) {
            //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"���ڲ�������...", FALSE);
            //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(theApp.m_pMainWnd->m_hWnd);
            //((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

            if (CNetStatusDlg::ADSLDial(pMainDlg->m_nAPN.dialnumber, "", "", (CWnd *)pMainDlg, (DIALTYPE)pMainDlg->m_nAPN.apn_type)) {
                pMainDlg->m_MJPGList.SetUnitBitmap(14, ".\\adv\\mjpg\\k5\\common\\gprs״̬1.bmp", "", TRUE);
                pMainDlg->m_nTELRigster = TELRIGSTER_DIALING;
            } else {
                Sleep(1000);
            }
        } else if (pMainDlg->m_nTELRigster == TELRIGSTER_DIALED /*&& isFirstReg*/) {
            //Dprintf("doRegisterTel :\r\n");
            //pMainDlg->doRegisterTel();    //LXZ 20090618k
        }
        //  else if(pMainDlg->m_nTELRigster == TELRIGSTER_REG)
        if (pMainDlg->m_nTELRigster >= TELRIGSTER_TD) {
            pMainDlg->doWithDownLoad();   //test
        }
    }
}


BOOL watchdog_MFC = TRUE;
void WatchDogProc(void*) {
    CMultimediaPhoneDlg* pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
    //Util::ClientSocket *netSocket = NULL;

    while (true) {
#ifdef _DEBUG
        //pMainDlg->SetWatchDog();   //����watchdog
#else
        if (!watchdog_MFC) {
            extern VOID WriteLog(CHAR * str);
            char restartTimer[64] = {
                0
            };
            SYSTEMTIME tm;
            GetLocalTime(&tm);
            DWORD value = DMemprintf("");
            sprintf_s(restartTimer, 64, "watchdog_MFC IS false:%04d-%02d-%02d %02d:%02d:%02d %x\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, value);
            WriteLog(restartTimer);
        }

        if ((watchdog_MFC && (!g_MemoryLeak)) || (g_bAdjustPanel)) {
            pMainDlg->SetWatchDog();   //����watchdog   �Ƶ�main����
            Sleep(5);
            pMainDlg->SetWatchDog();   //����watchdog   �Ƶ�main����
            Sleep(5);
            pMainDlg->SetWatchDog();   //����watchdog   �Ƶ�main����
            Sleep(5);
            watchdog_MFC = FALSE;
        }
#endif
        /*
        int val = pMainDlg->GetBackLightValue();
        //pMainDlg->SetBackLight(val);
        //Sleep(1000);
        //Dprintf("light %d\n", val);   //7 ��    0 ��
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
        //      if(netSocket)
        //      {
        //          netSocket->close();
        //      }
        //      netSocket = new Util::ClientSocket();
        //      netSocket->connect("127.0.0.1", 21);
        //      if(netSocket->getErrorState() != Util::Socket::esConnectionError)
        //      {
        //          pMainDlg->m_bNetOkStatus = TRUE;
        //      }
        //      else
        //          pMainDlg->m_bNetOkStatus = FALSE;

        SYSTEMTIME tm;
        GetLocalTime(&tm);
        if (tm.wHour == 0 && tm.wMinute == 0 && tm.wSecond < 8) {
            pMainDlg->SetAlarmList();
            Sleep(8000);
        } else {
            Sleep(5000);
        }

        static int gCCCC = 0;
        extern void GAskBatteryStatus();
        if (gCCCC++ % 3 == 0) {
            GAskBatteryStatus();
        }
    }
}

static BOOL gIsToLight;
void CMultimediaPhoneDlg::TestDB() {
}

Util::ATCommandWarp* GetATCommandWarp() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    return main->m_pATCommandWarp1;
}

void GIsOpenMix(BOOL isOn) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    if (!isOn) {
        waveInMessage(0, 0x10000001, 0, 0);
        char* sound = "ATN'MIX=0'\r";
        main->phone_->m_pRS232->WritePort((unsigned char*)sound, strlen(sound));
    } else {
          waveInMessage(0, 0x10000002, 0, 0);
        char* sound = "ATN'MIX=1'\r";
          main->phone_->m_pRS232->WritePort((unsigned char*)sound, strlen(sound));
    }
    ::Sleep(10);
}

void PlayDialToneProc(void* p) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    char* ptr = (char*)p;
    int offtime = 50;
    if (strlen(ptr) > 1) {
        offtime = 30;
    }
    Util::Audio::WIN_Audio_close();
    while (*ptr != '\0') {
        main->phone_->StartRing(L"");
        char filename[32] = {
            0
        };
        char c = *ptr;
        if (c == '*') {
            c = 'A';
        }
        sprintf_s(filename, 32, hive_res_dir, c);
        ptr++;
        CString s = filename;
        Util::Audio::Set_WIN_Params(NULL, 44 * 1024, 16, 1);
        FILE* fl = 0;
        fopen_s(&fl, filename, "r+b");
        if (fl) {
            char* temp = new char[200 * 1024];
            int len = fread(temp, sizeof(char), 200 * 1024, fl);
            Util::Audio::WIN_Play_Samples(temp, len / 8);
            fclose(fl);
            delete[]temp;
        }
        //  main->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
        Sleep(offtime);
        Util::Audio::WIN_Audio_close();
        //  main->phone_->StartRing(L"");
    }
    //  main->phone_->StartRing(L"");

    //  GIsOpenMix(0);
}

char gDiatone[64];
void GPlayDialTone(char* dialtone) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    int nVolume = main->m_pSettingDlg->m_pSetting->smsRingVolume_;
    UINT16 volume[] = {
        0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000
    }; //(5-nVolume)*0xFFFF/5;
    main->phone_->g_RingSound = (volume[nVolume] | (volume[nVolume] << 16));
    if (strstr(dialtone, "dialing") > 0) {
        main->phone_->StartRing(hive_res_dial_file, 0xFF);
    } else if (strstr(dialtone, "hangup") > 0) {
        main->phone_->StartRing(L"");
        GIsOpenMix(0);
    } else if (strlen(dialtone) > 0) {
        DWORD d;
          strcpy_s(gDiatone, 64, dialtone);
        HANDLE pThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayDialToneProc, (void*)gDiatone, 0, &d);
    }
}

void GNotifyDial(BOOL isDial) {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    if (!isDial) {
        char* sound = "ATN'TLK=0'\r";
        main->phone_->m_pRS232->WritePort((unsigned char*)sound, strlen(sound));
    } else {
        char* sound = "ATN'TLK=1'\r";
          main->phone_->m_pRS232->WritePort((unsigned char*)sound, strlen(sound));
    }
    ::Sleep(10);
}

void GAskBatteryStatus() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    char* status = "ATN'STA=?'\r";
    //@@main->phone_->m_pRS232->WritePort((unsigned char*)status, strlen(status));
    ::Sleep(10);
}

void GNotifyReady() {
    CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
    char* sound = "ATN'RDY=1'\r";
    main->phone_->m_pRS232->WritePort((unsigned char*)sound, strlen(sound));
    ::Sleep(10);
    char* status = "ATN'STA=?'\r";
    main->phone_->m_pRS232->WritePort((unsigned char*)status, strlen(status));
    ::Sleep(10);
}

void UiProc(void* param) {
    BOOL* flag = (BOOL*)param;
    int i = 0;
    HDC hdc = ::GetDC(0);
    CRect rt = CRect(0, 0, 800, 480);
    CBrush br = RGB(0, 0, 0);
    ::FillRect(hdc, &rt, (HBRUSH)br.m_hObject);
    CGIFShow* cGifShow;
    cGifShow = new CGIFShow(hdc);

    if (cGifShow->Load(poweron_gif)) {
        CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
        cGifShow->SetMainWnd(main);
        cGifShow->SetPosition(0, 0);
        cGifShow->Play();
    }
    while (*flag) {
        ::Sleep(100);
    }
    cGifShow->Stop();
    ::ReleaseDC(0, hdc);
    delete cGifShow;
}

BOOL isFirstGif;
BOOL CMultimediaPhoneDlg::OnInitDialog() {
    //lxz test jpg
    CDialog::OnInitDialog();

    SetWindowText(L"Edo_Phone_Dlg");
    InitALLBmpMemoryInfo();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon

    CenterWindow(GetDesktopWindow());   // center to the hpc screen

    // TODO: Add extra initialization here
    //step1: global variable init

    //step2: global workthread init

    //step3: config data read and fill

    //step4: device and environment check and init

    //step5: init UI
    //���SD ,USB
    InitSCLPort();
    m_bIsHaveUSB = DetectDIR(L"UsbDisk");
    m_bIsSD = DetectDIR(L"StorageCard");
    //  SetTimer(IDT_DETECTUSB, 500, NULL);

    char buffer[1024];
    GetManuInfo(buffer, 1024);
    memcpy(&m_manuinfo, buffer, sizeof(MANUINFO));
    //init player
    //other wnd

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

    //  m_pPhone = new Logical::Phone(PickupFunc, HandsfreeFunc, HangupFunc, KeypadDialFunc, ConnectFunc, RingInFunc, RemoteHangupFunc, TimeoutFunc, ErrorFunc);
    //  m_pPhone->start();
    //���� �绰

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
    if (m_pComWarp1) {
        if ((m_bATComm = m_pComWarp1->OpenComm(L"COM8:"))) {
            m_pATCommandWarp1 = new Util::ATCommandWarp();
            if (m_pATCommandWarp1) {
                m_pATCommandWarp1->Bind(m_pComWarp1);
                m_pSMSWarp = SMS::SMSWarp::GetSMSWarp();
                m_pVideoComWarp = new Util::ComWarp();
                phone_ = Telephone::TelephoneWarp::GetTelephoneWarp();//new Telephone::TelephoneWarp();

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

    InitializeCriticalSection(&csCom);
    DWORD watchdogThreadID = 0;
    HANDLE m_pThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WatchDogProc, 0, 0, &watchdogThreadID);
    DWORD tdThreadID = 0;
    HANDLE m_pThread2;
    if (m_bATComm) {
        m_pThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TdDoWithProc, 0, 0, &tdThreadID);
    } else {
        SetTimer(0x126, 0, 0);
    }

    DWORD uiThreadID = 0;
    isFirstGif = TRUE;
    HANDLE m_uiThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UiProc, &isFirstGif, 0, &uiThreadID);

    //  simdevice_ = new SIMDevice(NULL);

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

    //  change by qi 2009_09_15
    //  m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 420, 800, 480), this);
    //  m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\����\\3g_telstatus.xml");
    //  m_MJPGList.SetMJPGRect(CRect(0, 420, 800, 480));
    //  m_MJPGList.SetUnitIsDownStatus(2, TRUE);

    m_MJPGList.Create(L"", WS_VISIBLE | WS_CHILD, CRect(0, 0, 800, 57), this);
    m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\����\\3g_telstatus.xml");
    m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 57));
    m_MJPGList.SetUnitBitmap(0, ".\\adv\\mjpg\\k5\\common\\����.bmp", "", FALSE);
    m_MJPGList.SetUnitIsShow(10, false, false);

    //  m_MJPGList.SetUnitBitmap(10,".\\adv\\mjpg\\k5\\common\\png\\���水ť.png","",FALSE);
    //  m_MJPGList.SetUnitIsDownStatus(2, TRUE);

    //  change by qi 2009_09_15
    //  m_MJPGList.SetUnitIsDownStatus(2, TRUE);

    //  change by qi 2009_09_15
    //  m_MJPGList.SetUnitIsDisable(0, FALSE);
    //  m_MJPGList.SetUnitIsDisable(1, TRUE);


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

    /*
    ���� ���� ����
    */

    DWORD D = GetTickCount();

    m_pTelephoneDlg = new CTelephoneDlg();
    m_pTelephoneDlg->Create(CTelephoneDlg::IDD);
    m_pTelephoneDlg->ShowWindow(SW_HIDE);

    m_vAllCwnd.push_back(m_pTelephoneDlg);

    m_pMainDlg = new CMainDlg();
    panel = m_pMainDlg;
    panel->Create(CMainDlg::IDD);
    panels_[IDC_BUTTON_MAIN] = panel;
    currentPanel_ = panel;

    m_vAllCwnd.push_back(m_pMainDlg);

    m_pContactDlg = new CContactDlg();
    m_pContactDlg->Create(CContactDlg::IDD);
    panel = m_pContactDlg;
    panels_[IDC_BUTTON_CONTACT] = panel;

    m_vAllCwnd.push_back(m_pContactDlg);

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

    m_pContactNewDlg = new CContactNewDlg(panels_[IDC_BUTTON_CONTACT]);
    panel = m_pContactNewDlg;
    panel->Create(CContactNewDlg::IDD);
    panels_[IDC_BUTTON_CONTACTNEW] = panel;

    m_vAllCwnd.push_back(m_pContactNewDlg);

    m_pContactGroupDlg = new CContactGroupDlg(panels_[IDC_BUTTON_CONTACT]);
    panel = m_pContactGroupDlg;
    panel->Create(CContactGroupDlg::IDD);
    panels_[IDC_BUTTON_CONTACTGROUP] = panel;

    m_vAllCwnd.push_back(m_pContactGroupDlg);

    m_pDeleteTipDlg = new CDeleteTipDlg(this);
    m_pDeleteTipDlg->Create(CDeleteTipDlg::IDD);

    m_vAllCwnd.push_back(m_pDeleteTipDlg);


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


    m_pMMSDlg = new CMMSDlg();//д���Ž���
    m_pMMSDlg->Create(CMMSDlg::IDD);

    m_vAllCwnd.push_back(m_pMMSDlg);

    m_pSMSListDlg = new C3GSMSListDlg();//�ռ���
    m_pSMSListDlg->Create(C3GSMSListDlg::IDD);

    m_vAllCwnd.push_back(m_pSMSListDlg);

    //add by qi 2009_10_19
    m_pShiftFileDlg = new CShiftFile();
    m_pShiftFileDlg->Create(CShiftFile::IDD);
    m_pShiftFileDlg->ShowWindow(SW_HIDE);

    m_vAllCwnd.push_back(m_pShiftFileDlg);

    m_pDraftDlg = new CDraftDlg();
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
    m_pTelphoneRingDlg->ShowWindow(SW_HIDE);

    m_vAllCwnd.push_back(m_pTelphoneRingDlg);

    DWORD dend = GetTickCount() - D;

    TRACE(L"DDDDDDDDDDDD = %d", dend);
    /*
    m_mainLunarderDlg1_ = new CLunarderDlg(panels_[IDC_BUTTON_MAINNOTE]);
    panel = m_mainLunarderDlg1_;
    panel->Create(CContactGroupDlg::IDD);
    panels_[IDC_BUTTON_MAINNOTE] = panel;
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

    //  change by qi 2009_09_15
    currentPanel_->ShowWindow(SW_SHOW);
    currentPanel_->MoveWindow(0, 57, 800, 423);

    //  m_pBtnCurrent = &m_btnMain;
    //  SetButtonSelectedColor(m_pBtnCurrent);

    // add by qi 2009_09_15  
    //  SetDateTime(false);
    //  SetTimer(DATE_SET_TIEMR,1000,NULL);

    //and so on
    //m_btnLine.EnableWindow(FALSE);
    m_pFSM = new Util::FSM;
    AddFSMRules();
    m_uiKeyTimer = 1;
    m_uiRingTimer = 2;

    //���ý�������
    SetScreenSaveTimer();
    SetTimer(IDT_CONTROLBACKLIGHT_TIME + 1, 60 * 1000, NULL);   //20090221   lxztest  60* 
    SetTimer(0x125, 1000, NULL);   //20090221

    CString icon = L".\\adv\\mjpg\\k5\\common\\����ͼ��.bmp";
    AddIcon(icon);
    m_MJPGList.SetUnitIsShow(10, false, true);

    //  �Ƿ��������� lxz 20081223
    //  if(!SetNightControlBackLightTimer())
    //  ReStoreBackLight();

    //��������
    SetAlarmList();
    //���ҽ���û�п������ڵ�����
    //m_pMainDlg->FindTodayAlarm();  �Ƶ����õ��������б���

    //lxz test 20090309
    //SetTimer(0x999, 50000, NULL);

    TestDB();
    /*
    if (m_pATCommandWarp1->Connect(""))
    {
    phone_->Bind(m_pATCommandWarp1);
    m_pSMSWarp = SMS::SMSWarp::GetSMSWarp();
    m_pSMSWarp->Bind(m_pATCommandWarp1);
    }
    */

    isFirstGif = FALSE;
    return TRUE;  // return TRUE  unless you set the focus to a control
}

#define   NDISUIO_DEVICE_NAME                     TEXT( "UIO1: ") 
#define   FILE_ANY_ACCESS                             0 
#define   METHOD_BUFFERED                             0 
#define   FILE_DEVICE_NETWORK                     0x00000012 

#define   FSCTL_NDISUIO_BASE             FILE_DEVICE_NETWORK 

typedef struct _NDISUIO_QUERY_BINDING {
    ULONG BindingIndex;                   //   0-based   binding   number 
    ULONG DeviceNameOffset;   //   from   start   of   this   struct 
    ULONG DeviceNameLength;   //   in   bytes 
    ULONG DeviceDescrOffset;   //   from   start   of   this   struct 
    ULONG DeviceDescrLength;   //   in   bytes 
} NDISUIO_QUERY_BINDING, *PNDISUIO_QUERY_BINDING; 

typedef ULONG NDIS_OID, *PNDIS_OID; 

typedef struct _NDISUIO_QUERY_OID {
    NDIS_OID Oid; 
    PTCHAR ptcDeviceName;   
    UCHAR Data[sizeof(ULONG)];
} NDISUIO_QUERY_OID, *PNDISUIO_QUERY_OID; 

#define CTL_CODE(DeviceType,Function,Method,Access) (((DeviceType)<<16)|((Access)<<14)|((Function)<<2)|(Method)) 
#define _NDISUIO_CTL_CODE(_Function,_Method,_Access) CTL_CODE(FSCTL_NDISUIO_BASE,_Function, _Method,_Access) 

#define IOCTL_NDISUIO_QUERY_BINDING _NDISUIO_CTL_CODE(0x203, METHOD_BUFFERED, FILE_ANY_ACCESS) 

#define IOCTL_NDISUIO_OPEN_DEVICE _NDISUIO_CTL_CODE(0x200, METHOD_BUFFERED, FILE_ANY_ACCESS) 

#define IOCTL_NDISUIO_QUERY_OID_VALUE _NDISUIO_CTL_CODE(0x201, METHOD_BUFFERED, FILE_ANY_ACCESS) 

int CMultimediaPhoneDlg::CheckInternetStatus() {
    if (m_pSettingDlg->m_pSetting->linkMode() == Data::lmDirect) {  //����ֱ��
        m_bNetType = 0;
        int ret = 0;
        HANDLE hDevice = INVALID_HANDLE_VALUE;

        hDevice = CreateFile(TEXT("UIO1:"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);


        if (hDevice == INVALID_HANDLE_VALUE) {
            Dprintf("Open Error!\r\n");
            return 0;
        }

        //--------------------------------------------------------------------- 
        //   retrieve   the   name   and   description   of   the   adapter. 
        //--------------------------------------------------------------------- 
        CHAR chData[1024];   
        PNDISUIO_QUERY_BINDING pBindInfo = (PNDISUIO_QUERY_BINDING)chData;   
        pBindInfo-> BindingIndex = 0;   
        int length = sizeof(NDISUIO_QUERY_BINDING);   
        DWORD bytesconsumed = 0; 
        BOOL bValue = DeviceIoControl(hDevice, IOCTL_NDISUIO_QUERY_BINDING, pBindInfo, length, NULL, 1024, &bytesconsumed, NULL);   
        if (!bValue) {
            CloseHandle(hDevice);
            return 0;
        }   

        DWORD dwReturnedBytes;   
        PTCHAR ptcDeviceName = (PTCHAR)((PUCHAR)pBindInfo + pBindInfo-> DeviceNameOffset);   

        //--------------------------------------------------------------------- 
        //   Open   Device 
        //---------------------------------------------------------------------   
        DWORD deErrCode = 0;   
        bValue = DeviceIoControl(hDevice, IOCTL_NDISUIO_OPEN_DEVICE, ptcDeviceName, wcslen(ptcDeviceName) * sizeof(TCHAR), NULL, 0, &dwReturnedBytes, NULL);   

        if (0 == bValue) {
            deErrCode = GetLastError(); 
            CloseHandle(hDevice);
            return 0;
        }   

        PNDISUIO_QUERY_OID queryOid;    
        PUCHAR QueryBuffer[1024];   

        queryOid = (PNDISUIO_QUERY_OID)QueryBuffer;   

        queryOid-> Oid = OID_GEN_MEDIA_CONNECT_STATUS;          //   Custom   OID;   
        queryOid-> ptcDeviceName = NULL;                        //   You   dont   need   this   once   you   have   done   a   OPEN   Device.   


        bValue = DeviceIoControl(hDevice, IOCTL_NDISUIO_QUERY_OID_VALUE, (LPVOID)queryOid, sizeof(NDISUIO_QUERY_OID) + sizeof(DWORD), (LPVOID)queryOid, sizeof(NDISUIO_QUERY_OID) + sizeof(DWORD), &bytesconsumed, NULL);   

        if (*(ULONG *)(queryOid->Data) == NdisMediaStateConnected) { // Connected! 
            //  Dprintf("Connected!\r\n");
            ret = 1;
        } else if (*(ULONG *)(queryOid->Data) == NdisMediaStateDisconnected) {
            //  Dprintf("Disconnected!\r\n");
            ret = 0;
        }

        CloseHandle(hDevice); // What was the result?
        return ret;
    } else if (m_pSettingDlg->m_pSetting->linkMode() == Data::lmDial) { //���粦��
        m_bNetType = 1;
        int Statuscode;
        if (CNetStatusDlg::CheckADSLStatus(Statuscode)) {
            return 1;
        } else {
            return 0;
        }
    }
    return 0;
}

void CMultimediaPhoneDlg::SetNetTelStatus() {
    static CString gsNet = "";
    static CString gsTelName = "";
    static CString gsChongdian = "";
    CString sTel = ".\\adv\\mjpg\\k1\\common\\�绰1.bmp";
    CString sNet = ".\\adv\\mjpg\\k1\\common\\����1.bmp";
    CString sChongdian = ".\\adv\\mjpg\\k1\\common\\����1.bmp";
    static int gnTel = 0;
    static int gnNet = 0;
    static int gnChongdian = 0;

    if (m_pFSM->getCurrentState() != tsHangOff /*&& m_pTelephoneDlg->m_MJPGList.GetUnitText(100) != ""*/) {  //ժ��
        if (!m_pTelephoneDlg->IsWindowVisible()) {
            m_MJPGList.SetUnitIsDisable(1, FALSE);
            if (gnTel++ % 2) {
                sTel = ".\\adv\\mjpg\\k1\\common\\�绰1.bmp";
            } else {
                sTel = ".\\adv\\mjpg\\k1\\common\\�绰2.bmp";
            }
        }
    } else {
        //  m_MJPGList.SetUnitIsDisable(1, TRUE);
    }
    if (gsTelName != sTel) {
        //  m_MJPGList.SetUnitBitmap(1, sTel, "", TRUE);
        gsTelName = sTel;
    }

    if (phone_->m_BatteryStatus.isCharge) {  //��ӵ�Դ
        m_MJPGList.SetUnitIsDisable(13, FALSE);
        if (gnChongdian++ % 2) {
            sChongdian = ".\\adv\\mjpg\\k1\\common\\3g\\����Դ.bmp";
        } else {
            sChongdian = "";
        }
    } else {
        sChongdian = "";
        m_MJPGList.SetUnitIsDisable(13, TRUE);
    }
    if (gsChongdian != sChongdian) {
        m_MJPGList.SetUnitBitmap(13, sChongdian, "", TRUE);
        gsChongdian = sChongdian;
    }

    //20090429 lxz
    return;

    m_bNetOkStatus = CheckInternetStatus();
    if (m_bNetOkStatus) {
        if (gnNet++ % 2) {
            sNet = ".\\adv\\mjpg\\k1\\common\\����1.bmp";
        } else {
            sNet = ".\\adv\\mjpg\\k1\\common\\����2.bmp";
        }
    }

    if (gsTelName != sTel) {
        //  m_MJPGList.SetUnitBitmap(1, sTel, "", TRUE);
        gsTelName = sTel;
    }
    if (gsNet != sNet) {
        m_MJPGList.SetUnitBitmap(0, sNet, "", TRUE);
        gsNet = sNet;
    }
}

void CMultimediaPhoneDlg::SetButtonDefaultColor(CCEButtonST* button) {
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(154, 212, 255));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(154, 212, 255));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(154, 212, 255));
}

void CMultimediaPhoneDlg::SetButtonSelectedColor(CCEButtonST* button) {
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(235, 252, 255));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(235, 252, 255));
    button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(225, 252, 255));
}

void CMultimediaPhoneDlg::SwitchPanel_(int panelId) {
    //lxz 20071218 stocks
    //if(panelId == IDC_BUTTON_CONTACT || panelId == IDC_BUTTON_CONTACT || panelId == IDC_BUTTON_CONTACTINFO || panelId == IDC_BUTTON_SOUND)
    //  return;

    //ֹͣ��������
    m_pSettingDlg->StopTryRing();
    m_pContactGroupDlg->StopTryRing();
    m_pMainDlg->m_mainLunarderDlg_->StopTryRing();

    //�ر����뷨
    SipShowIM(SIPF_OFF);
    if (m_pMainDlg->m_pWebDialog->IsWindowVisible()) {
        m_pMainDlg->m_pWebDialog->ShowWindow_(SW_HIDE);
    }

    if (panelId == 0) {
        m_pMainDlg->m_mainVideoDlg_->OnExit_();
        if (m_pMainDlg->m_mainLunarderDlg_->IsWindowVisible()) {
            m_pMainDlg->m_mainLunarderDlg_->OnExit_();
        }
        SwitchPanel_(IDC_BUTTON_MAIN);
        //      SwitchPanel_(oldcurrentPanelID_);
        return;
    }

    if (currentPanel_ == panels_[IDC_BUTTON_MAIN]) { //ֹͣ����
        panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 0);
    }

    //  if (panels_[panelId] != currentPanel_)    //lxz 20090512
    if (true) { //lxz 20090512
        if (currentPanel_ == panels_[IDC_BUTTON_MAIN]) {
            ((CMainDlg *)currentPanel_)->ShowWindow_(SW_HIDE);
        } else if (currentPanel_ == panels_[IDC_BUTTON_INFORMATION]) {   //20071220 stocks
            ((CInformationDlg *)currentPanel_)->ShowWindow_(SW_HIDE);
        } else {
            currentPanel_->ShowWindow(SW_HIDE);
        }
        currentPanel_ = panels_[panelId];

        if (currentPanel_ != panels_[IDC_BUTTON_MAIN]) {  //lxz 20081206�л��������ں���
            ::SetFocus(currentPanel_->m_hWnd);
        }

        if (currentPanel_ == m_pSettingDlg) {
            m_pSettingDlg->IniCtrlData();
        }
        if (currentPanel_ == panels_[IDC_BUTTON_MAIN]) {
            ((CMainDlg *)currentPanel_)->ShowWindow_(SW_SHOW);
        } else if (currentPanel_ == panels_[IDC_BUTTON_INFORMATION]) {  //20071220 stocks
            ((CInformationDlg *)currentPanel_)->ShowWindow_(SW_SHOW);
        } else {
            currentPanel_->ShowWindow(SW_SHOW);
        }
    } else if (currentPanel_ == panels_[IDC_BUTTON_MAIN]) { //��ε������
        m_pMainDlg->SetMainMenu();
    }

    if (currentPanel_ == panels_[IDC_BUTTON_MAIN]) { //�ָ�������Ϣ
        panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 1);
    }


    //  if(panelId <= IDC_BUTTON_SETTING)
    //      m_MJPGList.SetUnitIsDownStatus(panelId-1001, TRUE);
    //  else if(panelId == IDC_BUTTON_SETTING)
    //      m_MJPGList.SetUnitIsDownStatus(7, FALSE);
    //  else
    //      m_MJPGList.SetUnitIsDownStatus(2, FALSE);
    m_MJPGList.Invalidate();
    ///////////////
}

void CMultimediaPhoneDlg::OnButtonMain() {
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_MAIN);
}

void CMultimediaPhoneDlg::OnButtonContact() {
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_CONTACT);
}

void CMultimediaPhoneDlg::OnButtonContactinfo() {
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_CONTACTINFO);
    //lxz 20080617
    //  ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearUnconnectCount();
}

void CMultimediaPhoneDlg::OnButtonSound() {
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_SOUND); 
    //lxz 20080617
    //  ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearRecordCount();
}

void CMultimediaPhoneDlg::OnButtonInformation() {
    // TODO: Add your control notification handler code here
    SwitchPanel_(IDC_BUTTON_INFORMATION);
    //test CEDialog
    //  m_pTestCEDlg->ShowWindow(TRUE);
}

void CMultimediaPhoneDlg::OnButtonSetting() {
    // TODO: Add your control notification handler code here
    m_pSettingDlg->SetDiskInFo();
    SwitchPanel_(IDC_BUTTON_SETTING);
}

void CMultimediaPhoneDlg::OnButtonTime() {
    // TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonIME() {
    // TODO: Add your control notification handler code here
}

void CMultimediaPhoneDlg::OnButtonLine() {
    // TODO: Add your control notification handler code here
    if (m_pFSM->getCurrentState() != tsHangOff) {
        m_pTelephoneDlg->ShowWindow_(TRUE);
    }
}

BOOL CMultimediaPhoneDlg::PreTranslateMessage(MSG* pMsg) {
    // TODO: Add your specialized code here and/or call the base class

    return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CMultimediaPhoneDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
    extern BOOL watchdog_MFC;
    watchdog_MFC = TRUE;   //����watchdog��Ч

    if ((message == WM_TEL_RING) || (message == WM_TEL_HUNGON)) {
        m_pContactInfoDlg->SendMessage(WM_TEL_INUSE, 0, 0);
        m_pSoundDlg->SendMessage(WM_TEL_INUSE, 0, 0);
        m_pSettingDlg->SendMessage(WM_TEL_INUSE, 0, 0);
    }

    switch (message) {
    case WM_LBUTTONDOWN:
        if (!ReStoreBackLight()) {
            panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 3);
        } //�˳�ȫ��
        break;

    case WM_APP + 88:
        //WM_PlayEnded:
        {
            static int gCount = 0;
            Dprintf("player %d file start!\n", gCount++);

            //          HWND hWnd = ::FindWindow(L"csplayer_win2", L"csplayer window2"); //
            //          if(hWnd)
            //          {
            //              CRect rt;
            //              playerimage_->owner_->GetWindowRect(&rt);
            //          
            //              HDC hdc = ::GetDC(hWnd);
            //              HDC hdc1 = m_pMainDlg->m_mainPhotoDlg_->GetDC()->m_hDC;
            //              BitBlt(hdc1, rt.left, rt.top, rt.Width(), rt.Height(), hdc, 0, 0, 0);
            //              ::ReleaseDC(hWnd, hdc);
            //          }


            //      Sleep(100);
            //  panels_[IDC_BUTTON_MAIN]->SendMessage(WM_PLAYVIDEO, 4); //������һ���ļ�
        }
        break;
    case WM_APP+60:
        Dprintf("0:");
        break;
    case WM_TEL_RING:
        //ֹͣ��������
        m_pSettingDlg->StopTryRing();
        m_pContactGroupDlg->StopTryRing();
        m_pMainDlg->m_mainLunarderDlg_->StopTryRing();

        //ReStoreBackLight();    //�Ƶ�����ǽ��  lxz 20090304
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

        //ֹͣ��������  
        //      m_pSettingDlg->StopTryRing();
        //      m_pContactGroupDlg->StopTryRing();
        //      m_pMainDlg->m_mainLunarderDlg_->StopTryRing();

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
        ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->Redial();
        break;
    case WM_MUTE:
        ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->Mute();
        break;
    case WM_RKEY:
        Dprintf("R KEY \n");
        break;
    case WM_SPEEDDIAL:
        {
            char c = wParam;
            if (m_pFSM->getCurrentState() == tsHangOn) {
                std::vector<std::pair<std::string, std::string> > m = m_pSettingDlg->m_pSetting->speedDials();
                if (m[c].second != "") {
                    m_pTelephoneDlg->DialContact(Data::TelephoneNumber(m[c].second));
                }
            }
        }
        break;
    case WM_HUNGONTORECORD:
        Dprintf("RECORD ... \n");
        if (m_pFSM->getCurrentState() == tsConnect) {
            ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->OnButtonTelephoneRecord();
        }
        break;
    case WM_HUNGONINRECORD:
        Dprintf("HUNG ON IN RECORD \n");
        ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOnInRecord(); 
        break;
    case WM_BINGJIZHAIJI:
        Dprintf("bingjizhaiji 0---- \n");
        if (m_pFSM->getCurrentState() == tsRing) {
            ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->HandleOff();
            Dprintf("bingjizhaiji 1---- \n");
        }

        break;
    case WM_KEYDOWN:
        {
            if (wParam != VK_F9) {
                ReStoreBackLight();
            }
            SetScreenSaveTimer();

            if (playervideo_->isPlaying_ && m_pMainDlg->m_mainVideoDlg_->IsWindowVisible()) {
                m_pMainDlg->m_mainVideoDlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
            } else if (playerimage_->isPlaying_) {
                  m_pMainDlg->m_mainPhotoDlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
            } else if (playeraudio_->isPlaying_ && m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible()) {
                  m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_KEYDOWN, wParam, lParam);
            }
        }
    case (WM_USER+1000):
        //      Dprintf("InputWnd click\r\n");
        ReStoreBackLight();
        SetScreenSaveTimer();
        break;
    case WM_TEL_STATUS:
        //�绰״̬
        {
            static int status = 0 ;
            if (wParam != TEL_SIGNALQUALITY && wParam != TEL_NETTYPE) {
                m_pTelephoneDlg->SendMessage(WM_TEL_STATUS, wParam, lParam);
            } else if (wParam == TEL_NETTYPE) {   //����״̬
                  //����״̬
                if (lParam != 2) { //G��
                    status = 1 ;
                } else { //T��
                    status = 2 ;
                }
            } else if (wParam == TEL_SIGNALQUALITY && (m_nTELRigster >= TELRIGSTER_TD)) {
                  //ˢ�µ���
                char txt[64] = {
                    0
                };
                if (1 == status) {
                    sprintf_s(txt, 64, ".\\adv\\mjpg\\k5\\common\\png\\G��%d.png", lParam);
                } else if (2 == status) {
                    sprintf_s(txt, 64, ".\\adv\\mjpg\\k5\\common\\png\\T��%d.png", lParam);
                }
                m_MJPGList.SetUnitBitmap(8, txt, txt, TRUE);
            }
        }
        break;

    case WM_CHANGE_BATTERY:
        {
            //if(!phone_->m_BatteryStatus.isCharge)
            {
                if (phone_->m_BatteryStatus.batteryType == 0) { //û���Դ
                    m_MJPGList.SetUnitBitmap(13, "", "", TRUE);
                } else {
                    char* sChongdian = ".\\adv\\mjpg\\k5\\common\\����Դ.bmp";
                    m_MJPGList.SetUnitBitmap(13, sChongdian, sChongdian, TRUE);
                }

                char txt[64] = {
                    0
                };
                sprintf_s(txt, 64, ".\\adv\\mjpg\\k5\\common\\png\\���%d.png", phone_->m_BatteryStatus.batteryProccess);
                m_MJPGList.SetUnitBitmap(11, txt, txt, TRUE);
            }
        }
        break;
    case WM_CHANGE_BATTERYOK:
        {
            char txt[64] = {
                0
            };
            sprintf_s(txt, 64, ".\\adv\\mjpg\\k5\\common\\png\\���%d.png", phone_->m_BatteryStatus.batteryProccess);
            m_MJPGList.SetUnitBitmap(11, txt, txt, TRUE);

            m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
            m_pDeleteTipDlg->SetPasswordModel(false);
            m_pDeleteTipDlg->SetTitle("��������!");
            m_pDeleteTipDlg->ShowWindow_(TRUE);
        }
        break;

    case WM_ATCOMM_ERROR:
        {
            m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
            m_pDeleteTipDlg->SetPasswordModel(false);
            m_pDeleteTipDlg->SetTitle("TD ģ�����!", 10000);
            m_pDeleteTipDlg->ShowWindow_(TRUE);
        }
        break;
    case WM_SMS_SENDPRO:
        {
            SMS::SMSWarp::SM_PARAM* pParam = (SMS::SMSWarp::SM_PARAM*)wParam;
            CString s = "���ڸ� ";

            std::string number = pParam->TPA ;
            if (0 == (number.substr(0, 2)).compare("86")) {
                number = number.substr(2);
            }
            std::string name = GetName(number);

            if (!name.empty()) {
                s += Util::StringOp::ToCString(name);
            } else {
                s += pParam->TPA;
            }

            s += " ���Ͷ���...";
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(s);
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
            ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
        }
        break;

    case WM_SMS_STATUS:
        doWithSMS(wParam, lParam);
        break;

    case WM_DELETESELITEM:
        //�Ķ�����
        {
            if (wParam == 1) {
                SetAPN((int)CMNET);
                m_bIsDial = TRUE;
            } else if (wParam == 2) {
                Net3GHungOff();
            } else {
                extern int g_iSmsID;
                if (g_iSmsID >= 0) {
                    int index = g_iSmsID >> 1;
                    if (g_iSmsID & 1) { //����
                        //  C3GSMSDetailDlg *pWnd_ = m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
                        //  pWnd_->initDataBase(MMS_READ, index, FALSE);                    
                        //  pWnd_->ShowWindow(SW_SHOW);

                        m_pSMSListDlg->m_pMmsReadDlg->SetMMSInfo(index, RECV_TYPE);
                        m_pSMSListDlg->m_pMmsReadDlg->ShowWindow(SW_SHOW);
                        AddIcon(Allicon[1]);
                    } else {
                          //  C3GSMSDetailDlg *pWnd_ = m_pMainDlg->m_p3GSMSDlg->m_pSMSDetailDlg;
                          //  pWnd_->initDataBase(SMS_READ, index, FALSE);
                          //  pWnd_->ShowWindow(SW_SHOW);

                          m_pSMSListDlg->m_pSmsReadDlg->SetSMSInfo(index, RECV_TYPE);
                          m_pSMSListDlg->m_pSmsReadDlg->ShowWindow(SW_SHOW);
                          AddIcon(Allicon[1]);
                    }
                    g_iSmsID = -1;
                }
            }
            break;
        }
    case 0xCCCD:
        //RASDIAL error
        OnRasErrorCode(wParam, lParam);
        break;
    case WM_PIN_INPUT:
        //����PIN
        {
            if (wParam == PIN_ERROR || wParam == 0) {
                m_pPasswordDlg->SetType(CHECK_PINPASSWORD);
                m_pPasswordDlg->SetHWnd(this->m_hWnd);
                m_pPasswordDlg->ShowWindow_(SW_SHOW);
            } else if (wParam == PIN_PUK || wParam == PIN_FUK_ERROR) {
                m_pPasswordDlg->SetType(CHECK_PUKPASSWORD);
                m_pPasswordDlg->SetHWnd(this->m_hWnd);
                m_pPasswordDlg->ShowWindow_(SW_SHOW);
            }
        }
        break;
    case WM_SIM_MESSAGE:
        {
            CString nosim = L".\\adv\\mjpg\\k5\\common\\png\\sim��1.png" ;
            CString sim = L".\\adv\\mjpg\\k5\\common\\png\\sim��.png";
            if (wParam) {
                m_MJPGList.SetUnitBitmap(12, sim, L"", true);
                m_bInsertSim = true ;
            } else {
                m_MJPGList.SetUnitBitmap(12, nosim, L"", true);
                m_bInsertSim = false ;
            }
        }
        break;

    case WM_CHAR:
        //  VK_ESCAPE
        {
            int i = 0;
            i++;
            Dprintf("WM_CHAR %x %x\r\n", wParam, lParam);
        }
        break;
    default:
        break;
    }

    if (message == WM_TEL_HUNGOFF) {
        m_pContactInfoDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
        m_pSoundDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
        m_pSettingDlg->SendMessage(WM_TEL_NOUSE, 0, 0);
    }

    return CWnd::WindowProc(message, wParam, lParam);
}

void CMultimediaPhoneDlg::OnLButtonDown(UINT nFlags, CPoint point) {
    // TODO: Add your message handler code here and/or call default

    CDialog::OnLButtonDown(nFlags, point);
}

void CMultimediaPhoneDlg::EnableLine(BOOL bEnable) {
    //lxz 20080620
    //  m_StatusKey.SetEnableKey(2, bEnable);
    //  m_btnLine.EnableWindow(bEnable);
}

void CMultimediaPhoneDlg::AddFSMRules(void) {
    //  m_pFSM->registerRule(tsRing, teHangOn, tsConnect, (void (__cdecl *const )(void *)) HangOn);
    //  m_pFSM->registerRule(tsCallID, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);

    m_pFSM->registerRule(tsHangOff, teHangOn, tsHangOn, (void (__cdecl * const)(void*)) HangOn);
    m_pFSM->registerRule(tsHangOff, teRing, tsRing, (void (__cdecl * const)(void*)) Ring);

    m_pFSM->registerRule(tsHangOff, teCallID, tsRing, (void (__cdecl * const)(void*)) CallID);

    m_pFSM->registerRule(tsRing, teHangOn, tsConnect, (void (__cdecl * const)(void*)) Connect);
    m_pFSM->registerRule(tsRing, teHangOff, tsHangOff, (void (__cdecl * const)(void*)) HangOff);
    m_pFSM->registerRule(tsRing, teRing, tsRing, (void (__cdecl * const)(void*)) Ring);
    m_pFSM->registerRule(tsRing, teCallID, tsRing, (void (__cdecl * const)(void*)) CallID); 
    //  m_pFSM->registerRule(tsRing, teCallID, tsCallID, (void (__cdecl *const )(void *)) CallID);  

    //  m_pFSM->registerRule(tsCallID, teHangOn, tsConnect, (void (__cdecl *const )(void *)) Connect);
    //  m_pFSM->registerRule(tsCallID, teHangOff, tsHangOff, (void (__cdecl *const )(void *)) HangOff);
    //  m_pFSM->registerRule(tsCallID, teRing, tsCallID, (void (__cdecl *const )(void *)) Ring);

    m_pFSM->registerRule(tsConnect, teCallID, tsConnect, (void (__cdecl * const)(void*)) CallID);
    m_pFSM->registerRule(tsConnect, teHangOff, tsHangOff, (void (__cdecl * const)(void*)) HangOff);

    m_pFSM->registerRule(tsConnect, teKey, tsConnect, (void (__cdecl * const)(void*)) SubKey);

    m_pFSM->registerRule(tsHangOn, teHangOff, tsHangOff, (void (__cdecl * const)(void*)) HangOff);
    m_pFSM->registerRule(tsHangOn, teKey, tsKey, (void (__cdecl * const)(void*)) Key);
    m_pFSM->registerRule(tsKey, teConnect, tsConnect, (void (__cdecl * const)(void*)) Connect);//����ʱ����ʱ��Ϊ��ͨ
    m_pFSM->registerRule(tsKey, teHangOff, tsHangOff, (void (__cdecl * const)(void*)) HangOff);
    m_pFSM->registerRule(tsKey, teKey, tsKey, (void (__cdecl * const)(void*)) Key);

    m_pFSM->setStartState(tsHangOff);
}

void CMultimediaPhoneDlg::HangOff(void* param) {
    //  ::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x123);
    //  ::SetTimer((theApp.m_pMainWnd)->m_hWnd, 0x123, 50, NULL);    //�һ�   lxz
    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->OnTimer(0x123);

    //  ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_uiKeyTimer);
    //  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(FALSE);
    //  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(param);
    // 
    //  //δ�ӽ������Զ��رմ���ʱģ��һ��¼�
    //  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->SendOutEvnet(WM_TEL_HUNGOFF, 1);
}

void CMultimediaPhoneDlg::HangOn(void* param) {
    ::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);
    ::SetTimer((theApp.m_pMainWnd)->m_hWnd, 0x124, 50, NULL);       //ժ��

    //  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
    //  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(param);
}

void CMultimediaPhoneDlg::Key(void* param) {
    ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiKeyTimer);
    ::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiKeyTimer, KEYTIMEOUT, 0);

    //  ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pTelephoneDlg->Key_(param); old 
    // add by qi 2009_11_05 new
    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelphoneDialDlg->Key_(param);
}

void CMultimediaPhoneDlg::SubKey(void* param) {
    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->SubKey_(param);
}

void CMultimediaPhoneDlg::Connect(void* param) {
    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->Connect_(param);
}

void CMultimediaPhoneDlg::Ring(void* param) {
    ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiRingTimer);
    ::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);

    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->EnableLine(TRUE);
    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->Ring_(param);
}

void CMultimediaPhoneDlg::CallID(void* param) {
    ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiRingTimer);
    ::SetTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiRingTimer, RINGTIMEOUT, 0);

    ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->CallID_(param);
}

void CMultimediaPhoneDlg::OnTimer(UINT nIDEvent) {
    //lxz test 20090309
    if (nIDEvent == 0x999) {
        KillTimer(0x999);
        SetTimer(0x998, 15000, NULL);
        PostMessage(WM_TEL_RING, 0, 0);
        PostMessage(WM_TEL_RING, 0, 0);
        PostMessage(WM_TEL_RING, 0, 0);
        PostMessage(WM_TEL_RING, 0, 0);
    } else if (nIDEvent == 0x998) {
        PostMessage(WM_TEL_HUNGOFF, 0, 0);
        KillTimer(0x998);
        SetTimer(0x999, 1000, NULL);
    } else if (nIDEvent == 0x126) {
        KillTimer(0x126);
        PostMessage(WM_ATCOMM_ERROR, 0, 0);
    }

    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiKeyTimer) {
        ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiKeyTimer);
        m_pFSM->fireEvent(teConnect, 0);
    } else if (nIDEvent == ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiRingTimer) {
          ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiRingTimer);
          //if ((m_pFSM->getCurrentState() == tsCallID) || (m_pFSM->getCurrentState() == tsRing))
        if (m_pFSM->getCurrentState() == tsRing) {
            m_pFSM->fireEvent(teHangOff, 0);
        }
    } else if (nIDEvent == IDT_CONTROLBACKLIGHT_TIME + 1) {
        //lxz test ���ݿ� �ڴ�й¶
        //  SetAlarmList();
        //  m_pMainDlg->FindTodayAlarm();
        //  std::vector<boost::shared_ptr<Data::Scheduler> > result = Data::Scheduler::GetFromDatabase("");
        /*
        std::string order = "startTime";
        std::string filter = "startTime BETWEEN '20090201000000' AND '20090201235959'";
        std::vector<boost::shared_ptr<Data::Scheduler> > result = Data::Scheduler::GetFromDatabaseByOrder(filter, order); 
        */
        //      std::vector<boost::shared_ptr<Data::Contact> > lResult = Data::Contact::GetFromDatabase("");

        extern VOID WriteLog(CHAR * str);
        char restartTimer[64] = {
            0
        };
        SYSTEMTIME tm;
        GetLocalTime(&tm);
        DWORD value = DMemprintf("");
        sprintf_s(restartTimer, 64, "mem:%04d-%02d-%02d %02d:%02d:%02d %x\r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, value);
        WriteLog(restartTimer);

        //ˢ������
        //m_pMainDlg->FindTodayAlarm();   //20090221   �Ƶ����������б���

        gIsToLight = FALSE;
        SetNightControlBackLightTimer();
    } else if (nIDEvent == IDT_CONTROLBACKLIGHT_TIME) {      //�������
        KillTimer(IDT_CONTROLBACKLIGHT_TIME);

        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetBackLight(0);
        Dprintf("Black BackLight\r\n");
    } else if (nIDEvent == IDT_MAIN_SAVESCREEN_TIME) {       //��������
          //lxz 20080623
          //return;

        CMainDlg* pWnd = (CMainDlg*)GetPanel(IDC_BUTTON_MAIN);
        HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
        BOOL flag2 = ::IsWindowVisible(hWnd);
        BOOL flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
        if (flag2 && flag3) {
            return;
        }

        KillTimer(IDT_MAIN_SAVESCREEN_TIME);
        int type = (int)m_pSettingDlg->m_pSetting->screenSaverContent();
        pWnd->m_mainScreenSaveDlg_->SetSaveScreenType(type, TRUE);    //1 ͼƬ����   0  ʱ������
        pWnd->SendMessage(WM_CHANGEWINDOW, (WPARAM)6, SW_SHOW);   //�л�������
    } else if (nIDEvent == IDT_ALARMTIME) {      //��������
        static int nOldAlarmID = 0;

        KillTimer(IDT_ALARMTIME);
        boost::shared_ptr<Data::Scheduler> m_result = Data::Scheduler::GetFromDatabaseById(m_nAlarmID);
        nOldAlarmID = m_nAlarmID;
        SetAlarmList();
        if (m_result) {
            ReStoreBackLight();

            std::string content = m_result->what();
            std::string ring = my_ring_dir_a; 
            ring += m_result->ring();
            CString content_ = "����: ";
            content_ += content.c_str();
            CTime tm = m_result->startTime();
            CString sTime;
            sTime.Format(_T("ʱ��: %04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
            m_pMainDlg->m_pWebDialog->SendMessage(WM_KILLWEBSHOW, 1, 0);
            m_AlarmShowDlg->SetTxt(nOldAlarmID, sTime, "����: ", content_, ring);
            m_AlarmShowDlg->ShowWindow_(SW_SHOW);
        }
    } else if (nIDEvent == 0x123) {      //�һ�
          ::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x123);

          ::KillTimer((theApp.m_pMainWnd)->m_hWnd, ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_uiKeyTimer);
          ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->EnableLine(FALSE);
          ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOff_(NULL);

          //δ�ӽ������Զ��رմ���ʱģ��һ��¼�
          ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->SendOutEvnet(WM_TEL_HUNGOFF, 1);
    } else if (nIDEvent == 0x124) {      //ժ��
          ::KillTimer((theApp.m_pMainWnd)->m_hWnd, 0x124);

          ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->EnableLine(TRUE);
          ((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pTelephoneDlg->HangOn_(NULL);
    } else if (nIDEvent == 0x125) {
        //  change by qi 2009_12_02
        //  SetNetTelStatus();   //20090221
    }


    /* �ĳ���Ϣ���
    else if(nIDEvent == IDT_DETECTUSB)  //���SD, USB
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

LRESULT CMultimediaPhoneDlg::OnMainSoftKey(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    switch (w) {
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
    return result;
}

void CMultimediaPhoneDlg::SetSkinStyle() {
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

//���� ժ�� �������ⲿ���鷢�͸� ���� MP3 VIDEO �����Դ���
LRESULT CMultimediaPhoneDlg::SendOutEvnet(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    SendMessage(WM_GEN_EVENT);
    CMainDlg* pWnd = (CMainDlg*)GetPanel(IDC_BUTTON_MAIN);
    BOOL isShow = !pWnd->m_mainScreenSaveDlg_->IsWindowVisible();
    switch (w) {
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
            //pWnd->m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, w, isShow);
        }
        break;
    }
    return result;
}

LRESULT CMultimediaPhoneDlg::OnEvent(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    if (w == 1) {
        SendOutEvnet(WM_TOUCH_CLICK, 0);
    }
    SetScreenSaveTimer();
    return result;
}

BOOL CMultimediaPhoneDlg::SetNightControlBackLightTimer() {
    BOOL ret = FALSE;
    BOOL flag2 = m_pSettingDlg->m_pSetting->isNightControlBlackLight();
    BOOL flag1 = m_pSettingDlg->m_pSetting->isContrlBlackLight();
    if (flag2) {
        CTime t1 = m_pSettingDlg->m_pSetting->nightControlBlackLightStartTime();
        CTime t2 = m_pSettingDlg->m_pSetting->nightControlBlackLightEndTime();

        SYSTEMTIME sTime;
        GetLocalTime(&sTime);
        CTime tm = CTime(sTime);
        CTime tStart = CTime(tm.GetYear(), tm.GetMonth(), tm.GetDay(), t1.GetHour(), t1.GetMinute(), t1.GetSecond());
        CTime tEnd = CTime(tm.GetYear(), tm.GetMonth(), tm.GetDay(), t2.GetHour(), t2.GetMinute(), t2.GetSecond());
        if (tStart > tEnd) {
            if (tm >= tStart || tm <= tEnd) {
                SetTimer(IDT_CONTROLBACKLIGHT_TIME, 30 * 1000, NULL);   
                Dprintf("Set NightControlBackLightTimer %d\r\n", 30000);
                ret = TRUE;
                return ret;
            }
        } else {
            if (tm > tStart && tm < tEnd) {
                SetTimer(IDT_CONTROLBACKLIGHT_TIME, 30 * 1000, NULL);   
                Dprintf("Set NightControlBackLightTimer %d\r\n", 30000);
                ret = TRUE;
                return ret;
            }
        }

        if (!gIsToLight) {
            gIsToLight = TRUE;  
            KillTimer(IDT_CONTROLBACKLIGHT_TIME);
            if (!flag1) {            //��������һ��
                int v = m_pSettingDlg->m_pSetting->blackLightValue();
                v = 7 - v;
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetBackLight(v);
                Dprintf("Night BackLight to Light\r\n");
            }
        }
    }
    return ret;
}

void CMultimediaPhoneDlg::SetControlBackLightTimer() {
    KillTimer(IDT_CONTROLBACKLIGHT_TIME);
    BOOL flag1 = m_pSettingDlg->m_pSetting->isContrlBlackLight();
    if (flag1) {
        CMainDlg* pWnd = (CMainDlg*)GetPanel(IDC_BUTTON_MAIN);
        if (pWnd == NULL) {
            return;
        }
        HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
        BOOL flag2 = ::IsWindowVisible(hWnd);
        BOOL flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
        if (flag2 && flag3) {
            return;
        }

        int nTimer_[] = {
            60 * 1000, 5 * 60 * 1000, 10 * 60 * 1000, 30 * 60 * 1000
        };
        int nTimer = m_pSettingDlg->m_pSetting->contrlBlackLightWaitTime();
        SetTimer(IDT_CONTROLBACKLIGHT_TIME, nTimer[nTimer_], NULL);
        Dprintf("Set ControlBackLightTimer %d\r\n", nTimer[nTimer_]);
    }
}

BOOL CMultimediaPhoneDlg::CancelBalckLightSaveTimer() {
    //  Dprintf("CancelBalckLightSaveTimer\r\n");
    KillTimer(IDT_MAIN_SAVESCREEN_TIME);
    KillTimer(IDT_CONTROLBACKLIGHT_TIME);
    return TRUE;
}

BOOL CMultimediaPhoneDlg::ReStoreBackLight() {
    if (m_nBackLightStatus == 0) {   //Ϊ��
        SetScreenSaveTimer();

        //�ָ�����
        int v = m_pSettingDlg->m_pSetting->blackLightValue();
        v = 7 - v;
        if (v == 0) {
            v = 1;
        }
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->SetBackLight(v);
        return TRUE;
    }
    return FALSE;
}

void CMultimediaPhoneDlg::SetScreenSaveTimer() {
    //  lxz test 20080623
    //  return;

    //  Dprintf("UnSet ControlBackLightTimer\r\n");
    SetControlBackLightTimer();

    CMainDlg* pWnd = (CMainDlg*)GetPanel(IDC_BUTTON_MAIN);
    if (!pWnd) {
        return;
    }

    HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
    HWND hWnd1 = ::FindWindow(L"csplayer_win2", L"csplayer window2");

    HWND hIEWnd = ::FindWindow(TEXT("iExplore"), NULL);

    KillTimer(IDT_MAIN_SAVESCREEN_TIME);
    BOOL flag1 = m_pSettingDlg->m_pSetting->isUseScreenSaver();
    BOOL flag2 = FALSE;
    if (hWnd) {
        flag2 = ::IsWindowVisible(hWnd);
    }
    BOOL flag3 = FALSE;
    if (pWnd->m_mainVideoDlg_) {
        flag3 = pWnd->m_mainVideoDlg_->IsWindowVisible();
    }
    BOOL flag4 = FALSE;
    if (m_pFSM->getCurrentState() != tsHangOff) {
        flag4 = TRUE;
    }
    if (flag1) {
        CTimeSpan nTimer = m_pSettingDlg->m_pSetting->screenSaverDuration();
        int nSecond = (int)nTimer.GetTotalSeconds();
        if (flag2 && flag3) {   //video 
            Dprintf("UnSet ScreenSave Timer \r\n");
        } else if (::IsWindowVisible(hWnd1)) {  //�������
            Dprintf("UnSet ScreenSave Timer \r\n");
        } else if (hIEWnd && ::IsWindowVisible(hIEWnd)) {  //�����
            Dprintf("UnSet ScreenSave Timer \r\n");
        } else if (flag4) { //�绰����ʹ��
            Dprintf("UnSet ScreenSave Timer \r\n");
        } else {
            SetTimer(IDT_MAIN_SAVESCREEN_TIME, nSecond * 1000, NULL);
            static int gTimerCount = 0;
            //Dprintf("Set ScreenSave Timer = %d \r\n", gTimerCount++);
        }
    } else {
        //  Dprintf("UnSet ScreenSave Timer \r\n");
    }
}

void CMultimediaPhoneDlg::KillScreenSaveTimer() {
    KillTimer(IDT_MAIN_SAVESCREEN_TIME);
    //  Dprintf("Kill ScreenSave Timer \r\n");
}

LRESULT CMultimediaPhoneDlg::OnDeviceChange(WPARAM w, LPARAM l) {
    LRESULT result = 0;
    Dprintf("USB do with %d %d\r\n", w, l);
    if (g_IsRun) {
        if (m_pSettingDlg->m_copyfileDlg) {
            m_pSettingDlg->m_copyfileDlg->SendMessage(WM_USB_MSG, w, 0);
        }
        if (m_pMainDlg->m_p3GHomePicDlg) {
            m_pMainDlg->m_p3GHomePicDlg->SendMessage(WM_USB_MSG, w, 0);
        }
        if (m_pMainDlg->m_p3GHomeJoyDlg) {
            m_pMainDlg->m_p3GHomeJoyDlg->SendMessage(WM_USB_MSG, w, 0);
        }
        if (m_pMainDlg->m_p3GHomeMovieDlg) {
            m_pMainDlg->m_p3GHomeMovieDlg->SendMessage(WM_USB_MSG, w, 0);
        }
    }
    return result;
}

////////SCL////////////////
/////SCL////////////////////////
void CMultimediaPhoneDlg::InitSCLPort() {
    m_hPort = CreateFile(_T("SCL1:"), // Pointer to the name of the port
        GENERIC_READ | GENERIC_WRITE, // Access (read-write) mode
        0,            // Share mode
        NULL,         // Pointer to the security attribute
        CREATE_ALWAYS,// Create New File as any case
        0,            // Port attributes
        NULL);
}

BOOL CMultimediaPhoneDlg::GetManuInfo(char* pBuffer, int length) {
    if (m_hPort != INVALID_HANDLE_VALUE) {
        //Get Information;
        unsigned long size;
        UINT nIn;
        if (DeviceIoControl(m_hPort, SCL_GET_PRODUCT_INFO, &nIn, 0, pBuffer, 512, &size, NULL)) {
            return TRUE;
        }
    }
    return FALSE;
}

//����watchdog
BOOL CMultimediaPhoneDlg::SetWatchDog() {
    if (m_hPort != INVALID_HANDLE_VALUE) {
        unsigned long size;
        UINT value;
        UINT nIn;
        if (DeviceIoControl(m_hPort, SCL_ENABLE_WATCHDOG, &nIn, 0, &value, sizeof(UNIT), &size, NULL)) {
            return TRUE;
        }
    }
    return FALSE;
}

//����ADSL��Դ����
BOOL CMultimediaPhoneDlg::SetADSLPower(BOOL isOn) {
    if (m_hPort != INVALID_HANDLE_VALUE) {
        Dprintf("SetADSLPower %d\n", isOn);

        unsigned long size;
        DWORD inBuff;
        if (isOn) {
            inBuff = 1;
        } else {
            inBuff = 2;
        }
        DWORD value;
        if (DeviceIoControl(m_hPort, SCL_ADSL_POWERCTL, &inBuff, sizeof(DWORD), &value, sizeof(DWORD), &size, NULL)) {
            return TRUE;
        }
    }
    return FALSE;
}

//����ϵͳ����
BOOL CMultimediaPhoneDlg::SetReStart() {
    if (m_hPort != INVALID_HANDLE_VALUE) {
        unsigned long size;
        UINT value;
        UINT nIn;
        if (DeviceIoControl(m_hPort, SCL_SYSTEM_RESET, &nIn, 0, &value, sizeof(UINT32), &size, NULL)) {
            return TRUE;
        }
    }
    return FALSE;
}

//�رմ򿪱���
BOOL CMultimediaPhoneDlg::SetBackLight(int isType) {
    if (m_nBackLightStatus != isType) {
        //todo: ����
        if (m_hPort != INVALID_HANDLE_VALUE) {
            unsigned long size;
            BACKLIGHTCMD blightcmd;
            blightcmd.bkCmd = 0x01;
            blightcmd.bkLevel = isType;
            UINT value;
            if (DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(UINT), &size, NULL)) {
                m_nBackLightStatus = isType;
                return TRUE;
            }
        }
    }
    return FALSE;
}

//��ȡ����Ĺ���ֵ
#define OLDVALUE_MAX   3
UINT32 CMultimediaPhoneDlg::GetBackLightValue() {
    int ret;
    static int oldblValue[OLDVALUE_MAX] = {
        0, 0, 0
    };
    static int oldcount = 0;
    //todo: ����
    if (m_hPort != INVALID_HANDLE_VALUE) {
        unsigned long size;
        BACKLIGHTCMD blightcmd;
        blightcmd.bkCmd = 0x02;
        blightcmd.bkLevel = 0;
        DWORD value;
        if (DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(DWORD), &size, NULL)) {
            //  Dprintf("back light %d\n", value);  
            oldblValue[oldcount++ % OLDVALUE_MAX] = value;
            BOOL flag = FALSE;
            for (int i = 0; i < OLDVALUE_MAX; i++) {
                if (m_nBackLightStatus == 0) {  //��
                    ret = 7;
                    if (oldblValue[i] >= 5000) {   //5000
                        ret = 0;    //��
                        break;
                    }
                } else {
                    ret = 0;
                    if (oldblValue[i] < 5000) {   //50000
                        ret = 7;    //��
                        break;
                    }
                }
            }
        }
    }

    return ret;
}

BOOL CMultimediaPhoneDlg::EnryRun() {
    if (m_hPort != INVALID_HANDLE_VALUE) {
        unsigned long size;
        ENCRYCMD cmd;
        cmd.bkCmd = 0x01;
        cmd.bkRandom = rand();
        DWORD ret;
        if (DeviceIoControl(m_hPort, SCL_ENCRYGPT_VERIFY, &cmd, sizeof(ENCRYCMD), &ret, sizeof(DWORD), &size, NULL)) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
    return FALSE;
}

UINT32 CMultimediaPhoneDlg::GetEnry() {
    if (m_hPort != INVALID_HANDLE_VALUE) {
        unsigned long size;
        ENCRYCMD cmd;
        cmd.bkCmd = 0x02;
        UINT32 value;
        if (DeviceIoControl(m_hPort, SCL_ENCRYGPT_VERIFY, &cmd, sizeof(ENCRYCMD), &value, sizeof(UINT32), &size, NULL)) {
            return value;
        }
    }
    return 0;
}


UINT32 CMultimediaPhoneDlg::GetRawBackLightValue() {
    int ret = 0;
    //todo: ����
    if (m_hPort != INVALID_HANDLE_VALUE) {
        unsigned long size;
        BACKLIGHTCMD blightcmd;
        blightcmd.bkCmd = 0x02;
        blightcmd.bkLevel = 0;
        DWORD value;
        if (DeviceIoControl(m_hPort, SCL_ADJUST_BACKLIGHT, &blightcmd, sizeof(BACKLIGHTCMD), &value, sizeof(DWORD), &size, NULL)) {
            ret = value;
            //Dprintf("back light %d\n", value);  
        }
    }

    return ret;
}
//////////////////end SCL/////////////////////////

//alarm setting
void CMultimediaPhoneDlg::SetAlarmList() {
    //  return;

    KillTimer(IDT_ALARMTIME);

    SYSTEMTIME curtime;
    GetLocalTime(&curtime);
    CTime time = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, 0, 0, 0);

    std::string filter = "startTime BETWEEN '";
    char buff[32];
    memset(buff, 0, 32);
    sprintf_s(buff, 32, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(), time.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
    filter += buff;
    filter += "' AND '";
    memset(buff, 0, 32);
    sprintf_s(buff, 32, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(), time.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
    filter += buff;
    filter += "'";
    std::string order = "startTime";
    std::vector<boost::shared_ptr<Data::Scheduler> > m_result = Data::Scheduler::GetFromDatabaseByOrder(filter, order); 
    if (!m_result.empty()) {
        for (size_t i = 0; i < m_result.size(); i++) {
            CTime tm = m_result[i]->startTime();
            GetLocalTime(&curtime);
            CTime nowtm = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
            if (m_result[i]->tipsType() && (nowtm < tm)) {
                m_nAlarmID = m_result[i]->id();
                int timeout = (tm.GetHour() - curtime.wHour) * 3600 + (tm.GetMinute() - curtime.wMinute) * 60 + (tm.GetSecond() - curtime.wSecond);
                SetTimer(IDT_ALARMTIME, timeout * 1000, NULL);
                break;
            }
        }
    }

    //���ҽ���û�п������ڵ�����
    m_pMainDlg->FindTodayAlarm();    //��ʱ���в��һ����ڴ�й¶.   //20090221
}
/*
10658066    OTA�ط���
1065806601  Զ����������
1065806612  ��ͥ��� 

1065806611  ������Ϣ
1065806651  ������Ϣ
1065806661  ������Ϣ
1065806621  ������Ϣ
*/

#define SPECODE_MAX   6
char* gpChaSpecod[] = {
    "1065806601", "1065806612", "1065806611", "1065806651", "1065806661", "1065806621"
};
//  

int g_iSmsID = -1;

//int type = 0;       ����
//           1;       ��ͥ����ͨ
//           2;       ��������
void CMultimediaPhoneDlg::SMSSpecRing(int type, CString s) {
    UINT16 volume[] = {
        0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000
    }; //(5-nVolume)*0xFFFF/5;
    if (type == 0) {
        if (m_pSettingDlg->m_pSetting->isSmsRing_) {
            CFileStatus status;
            CString filename = my_ring_dir;
            filename += "/����.wav";
            if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_pSettingDlg->m_pSetting->smsRingFilename_)), status)) {
                filename = m_pSettingDlg->m_pSetting->smsRingFilename_.c_str();
            }

            int nVolume = m_pSettingDlg->m_pSetting->smsRingVolume_;
            phone_->g_RingSound = (volume[nVolume] | (volume[nVolume] << 16));
            phone_->StartRing((LPTSTR)(LPCTSTR)filename, 1);
        }
    } else if (type == 1) {
        CFileStatus status;
        CString filename = my_ring_dir;
          filename += "/����.wav";

        if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(m_pSettingDlg->m_pSetting->specodeRingFilename_)), status)) {
            filename = m_pSettingDlg->m_pSetting->specodeRingFilename_.c_str();
        }

        int nVolume = m_pSettingDlg->m_pSetting->specodeRingVolume_;
          phone_->g_RingSound = (volume[nVolume] | (volume[nVolume] << 16));
          phone_->StartRing((LPTSTR)(LPCTSTR)filename, 1);
    } else if (type == 2) {  //����
        int nVolume = m_pSettingDlg->m_pSetting->soundVolume();
        phone_->g_RingSound = (volume[nVolume] | (volume[nVolume] << 16));
        phone_->StartRing((LPTSTR)(LPCTSTR)s);
    }
}

LRESULT CMultimediaPhoneDlg::doWithSMS(WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    if (wParam == WM_SMS_SMS) {   //����
        Data::Message* pMsg = (Data::Message*)lParam;
        BOOL flag = FALSE;
        //  for(int i = 0 ; i < SPECODE_MAX; i++)
        std::string sCode = m_pSettingDlg->m_pSetting->speCode1_;
        if (pMsg->remote.address.substr(0, 6) == sCode.substr(0, 6)) {
            //  if(pMsg->remote.address == gpChaSpecod[i])
            {
                if (pMsg->remote.address == m_pSettingDlg->m_pSetting->speCode3_) {  //��ͥ����
                    pMsg->group = Data::Message::gReMoteSMS;
                    pMsg->Insert();
                    g_iSmsID = (pMsg->id() << 1);

                    SMSSpecRing(1, "");

                    if (!m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible() && !m_pMainDlg->m_mainVideoDlg_->IsWindowVisible()) {
                        m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
                        m_pDeleteTipDlg->SetPasswordModel(false);
                        m_pDeleteTipDlg->SetTitle("���ܵ�һ����ͥ����,�Ƿ��Ķ�?");
                        m_pDeleteTipDlg->ShowWindow_(TRUE);
                    }
                } else {
                    BOOL f = TRUE;

                    if (f) {
                        Data::MultimediaDownload* result = new Data::MultimediaDownload();

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

                        if (pMsg->remote.address.substr(0, 8) != "10658153") {
                            result->Insert();
                            //  int i = 0;
                            //  i++;
                        } else {
                            int i = 0;
                            i++;
                        }

                        SMSSpecRing(1, "");
                    }
                }
                flag = TRUE;
            }
        }
        if (!flag) {
            //��ͨ����
            if (pMsg->remote.address.substr(0, 8) != "10658153") {
                BOOL f = TRUE;
                if (pMsg->total > 1) {
                    pMsg->group = Data::Message::gExternSms;        //��ͥ����ͨ�ĳ�������
                    pMsg->Insert();

                    std::string sfilter = "[group] = " + Util::StringOp::FromInt(Data::Message::gExternSms); 
                    sfilter += " AND [transactionId] = '";
                    sfilter += pMsg->transactionId;
                    sfilter += "'";
                    std::vector<boost::shared_ptr<Data::Message> >vMessageCurrentResult = Data::Message::GetFromDatabase(sfilter);
                    int i = vMessageCurrentResult.size();
                    if (vMessageCurrentResult.size() == pMsg->total) {           //��������
                        std::string content = "";
                        for (int i = 0; i < pMsg->total; i++) {
                            for (int j = 0; j < pMsg->total; j++) {
                                if (vMessageCurrentResult[j]->no == i + 1) {
                                    content += vMessageCurrentResult[j]->unicodeData;
                                }
                            }
                        }
                        for (int i = 0; i < pMsg->total; i++) {
                            vMessageCurrentResult[i]->Remove();
                        }
                        pMsg->unicodeData = content;
                    } else {
                        f = FALSE;
                    }
                }

                if (f) {
                    pMsg->group = Data::Message::gReceive;
                    pMsg->Insert();
                    g_iSmsID = (pMsg->id() << 1);
                    SMSSpecRing(0, "");

                    if (!m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible() && !m_pMainDlg->m_mainVideoDlg_->IsWindowVisible()) {
                        m_pDeleteTipDlg->SetHWnd(GetSafeHwnd());
                        m_pDeleteTipDlg->SetPasswordModel(false);
                        m_pDeleteTipDlg->SetTitle("���ܵ�һ������,�Ƿ��Ķ�?");
                        m_pDeleteTipDlg->ShowWindow_(TRUE);
                    }
                    ::SendMessage(((CMultimediaPhoneDlg *)(theApp.m_pMainWnd))->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 3, 0);
                }
            }
        }

        delete pMsg;
    } else if (wParam == WM_SMS_MMS) {   //����
        Data::MmsMessage* pMsg = (Data::MmsMessage*)lParam;
        pMsg->Insert();
        SMSSpecRing(0, "");

        if (!m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible() && !m_pMainDlg->m_mainVideoDlg_->IsWindowVisible()) {
            m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
            m_pDeleteTipDlg->SetPasswordModel(false);
            m_pDeleteTipDlg->SetTitle("���ڽ���һ������!");
            m_pDeleteTipDlg->ShowWindow_(TRUE);
        }
    } else if (wParam == WM_SMS_OTA) {   //OTA
        Data::OtaMessage* pMsg = (Data::OtaMessage*)lParam;
        pMsg->Insert();
          /*
          extern void StartHttp(char *url, int type = 0, int dwtype = 0, char *lfilename = NULL, char *cIMEI = NULL, char *cTerminalId = NULL);
          if(pMsg->instruction == Data::tMenuManager)                 //�˵�ά��
          {
          //  StartHttp((char *)pMsg->url.c_str(), 0, 2);
          }
          else if(pMsg->instruction == Data::tWeather)                //����Ԥ��
          {
          StartHttp((char *)pMsg->url.c_str(), 0, 1);
          }
          else if(pMsg->instruction == Data::tMultimediaDownload)     //ý������
          {
          StartHttp((char *)pMsg->url.c_str(), 0, 0);
          }
          */
    } else if (wParam == WM_SMS_SENDRESULT) {
        if (lParam == 1) {   //���ŷ��ͳɹ�
            m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
            m_pDeleteTipDlg->SetPasswordModel(false);
            m_pDeleteTipDlg->SetTitle("���ŷ��ͳɹ�!");
            m_pDeleteTipDlg->ShowWindow_(TRUE);
        } else {     //���ŷ���ʧ��
            m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
            m_pDeleteTipDlg->SetPasswordModel(false);
            m_pDeleteTipDlg->SetTitle("���ŷ���ʧ��!");
            m_pDeleteTipDlg->ShowWindow_(TRUE);
        }
    }
    return result;
}

#include <Raserror.h>
#include <Ras.h>
//���粦��
LRESULT CMultimediaPhoneDlg::OnRasErrorCode(WPARAM w, LPARAM l) {
    LRESULT r = 0;
    int result = w;
    char txt[64] = {
        0
    };
    sprintf_s(txt, 64, "Ras code %d %d %d\r\n", result, w, l);
    //Dprintf("Ras code %d %d %d\r\n", result, w, l);
    DWriteDat("", (UINT8 *)txt, strlen(txt));
    if (result >= RASCS_Interactive && result <= RASCS_PasswordExpired) {
        result = result - RASCS_Interactive + 19;
    } else if (result >= RASCS_Connected && result <= RASCS_Disconnected) {
        result = result - RASCS_Connected + 23;
    }

    if (result > 25) {
        result = 25;
    }

    if (result == 23) {  //������
        m_MJPGList.SetUnitBitmap(14, ".\\adv\\mjpg\\k5\\common\\gprs״̬.bmp", "", TRUE);
        m_nTELRigster = TELRIGSTER_DIALED;
    } else if (result == 24) {  //δ����
        m_nTELRigster = TELRIGSTER_TD;
        if (0) { //�Ƿ���Ҫ��������  ���� 13917967645   69
            m_bIsDial = FALSE;
        }

          //      m_pDeleteTipDlg->SetTitle(L"�����������ɹ�...", FALSE);
          //      m_pDeleteTipDlg->SetHWnd(m_hWnd);
          //      m_pDeleteTipDlg->ShowWindow_(TRUE);

        if (m_nAPN.apn_type == (int)(CMWAP)) {
            std::string fileter = "type = ";
            fileter += Util::StringOp::FromInt(Data::MMSData::tpReadySend);
            std::vector<boost::shared_ptr<Data::MMSData> > mmsSendResult = Data::MMSData::GetFromDatabase(fileter);
            for (size_t i = 0; i < mmsSendResult.size(); i++) {
                mmsSendResult[i]->type = Data::MMSData::tpUnSend;
                mmsSendResult[i]->Update();
            }
        }
    }
    extern char* adsl_status[];
    Dprintf("Dial : %s \r\n", adsl_status[result]);
    return r;
}

//3G�Ҷ�
void CMultimediaPhoneDlg::Net3GHungOff() {
    if (m_nTELRigster >= TELRIGSTER_DIALED) {
        CNetStatusDlg::ADSLHungUp();
        m_MJPGList.SetUnitBitmap(12, "", "", TRUE);
        Sleep(3000);
        SetAPN((int)CMWAP);
    }
    m_nTELRigster = TELRIGSTER_TD;
    if (0) {             //����Ƿ���Ҫ��Զ����
        m_bIsDial = TRUE;
    } else {
        m_bIsDial = FALSE;
    }
    //  SetAPN((int)CMWAP);
}

//�ֻ�����
void CMultimediaPhoneDlg:: doSerachrTDNet() {
    DWORD dwStart = GetTickCount();
    phone_->Bind_(m_pATCommandWarp1);
    m_pSMSWarp->Bind_(m_pATCommandWarp1);

    int ret = m_pATCommandWarp1->Connect("");   

    static bool bBind = false ;

    if (!bBind) {
        bBind = true ;
        phone_->Bind(m_pATCommandWarp1);
        m_pSMSWarp->Bind(m_pATCommandWarp1);
    }

    if (ret == 1) {   //1 �ɹ�    
        DWORD offset = GetTickCount() - dwStart;   
        Dprintf("3G ���� %d Ok\r\n", offset);

        m_nTELRigster = TELRIGSTER_TD;

        //      m_pSMSWarp->SetCenterAddress(m_pSettingDlg->m_pSetting->speCode12_);        //���ö������ĺ���
        m_pSMSWarp->SetOTANumber(m_pSettingDlg->m_pSetting->speCode11_);            //����OTA����

        m_sMobileNumber = phone_->GetNumber();
        //      if(m_sMobileNumber == "")    //lxz 20090827
        //      {
        //          ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"������SIM���ı����ֻ�����!", 15000);
        //          ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
        //          ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
        //      }

        //      add by qi 2009_08_03
        //      std::vector<Util::ATCommandWarp::SIM_FORMAT> vGsim;
        //      Util::ATCommandWarp::SIM_FORMAT sim;
        //      sim.name = "С��";
        //      sim.telnum ="13681209927" ;
        //      vGsim.push_back(sim);
        //      m_pATCommandWarp1->Save2Sim(vGsim);
        //      m_pATCommandWarp1->ReadSimPhoneInfo(vGsim);//ȡ��sim�������Ϣ�绰������
        //      InsertSimToContact(vGsim);
        //      m_pContactDlg->ShowItemsInList();
    } else if (ret == 2) { //2   ��Ҫע����
    } else if (ret == 3) { //δ�忨
        //  m_nTELRigster = TELRIGSTER_FAILED;
    }
}

//�ֻ�ע��
void CMultimediaPhoneDlg:: doRegisterTel() {
    std::vector<boost::shared_ptr<Data::Register> > result = Data::Register::GetFromDatabase(""); 

    Dprintf("doRegisterTel 0\r\n");
    HttpParam* pHttpParam = new HttpParam;
    pHttpParam->type = 1;
    pHttpParam->apn_type = m_nAPN.apn_type;
    pHttpParam->apn_proxy = m_nAPN.proxy;
    if (result.size() > 0) {
        pHttpParam->cIMEI = (char*)result[0]->IMEI.c_str();
        //  pHttpParam->cTerminalId = (char *)result[0]->terminalId.c_str();
    } else {
        pHttpParam->cIMEI = "1234";
        //  pHttpParam->cTerminalId = "1234567890";
    }
    pHttpParam->cTerminalId = (char*)m_sMobileNumber.c_str();
    Dprintf("doRegisterTel 1\r\n");

    pHttpParam->url = "http://211.138.198.106:8888/";
    if (HttpProcesse(pHttpParam) == 0) {
        m_nTELRigster = TELRIGSTER_REG;
        isFirstReg = FALSE;
        //      AfxMessageBox(L"ע�ᵽ����ʮ��ƽ̨�ɹ�!");
    }
    //  else
    //  m_nTELRigster = TELRIGSTER_REG;
}

BOOL CMultimediaPhoneDlg::GetIs3GDialed() {
    if (m_nTELRigster >= TELRIGSTER_DIALED && m_nAPN.apn_type == (int)CMWAP) {
        return TRUE;
    } else if (m_nTELRigster >= TELRIGSTER_DIALED) {
        Net3GHungOff();
        return FALSE;
    } else {
        SetAPN((int)CMWAP);
        return FALSE;
    }

    return FALSE;
}

void CMultimediaPhoneDlg::ParseSmil(CString FilePath, MMS::MMSWarp::MMS_SubmitRequest& r) {
    CString str = FilePath + L"*" ;
    WIN32_FIND_DATA fd;
    HANDLE hfind;
    hfind = FindFirstFile(str, &fd);
    if (hfind != INVALID_HANDLE_VALUE) {
        do {
            if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
                ;//������ļ���
            } else {
                  //������ļ�
                CString strfl = fd.cFileName ;

                if (strfl.Find(L".smil") > 0 || strfl.Find(L".smi") > 0 || strfl.Find(L".SMIL") > 0 || strfl.Find(L".SMI") > 0) {
                    //����smil  
                    CString file = FilePath + strfl;
                    MMS::MMSWarp::GetMMSWarp()->DecodeSmil(Util::StringOp::FromCString(file).c_str(), r.pars, r.layout);
                    return;
                }
            }
        } while (FindNextFile(hfind, &fd));
    }

    MMS::MMSWarp::GetMMSWarp()->AddRootLayout(&r, 176, 216);
    MMS::MMSWarp::GetMMSWarp()->AddRegion(&r, MMS::MMSWarp::rtText, 0, 0, 176, 144);
    MMS::MMSWarp::GetMMSWarp()->AddRegion(&r, MMS::MMSWarp::rtImage, 0, 144, 176, 72);

    CString sI = "";
    CString sT = "";
    CString sV = "";
    CString sA = "";
    hfind = FindFirstFile(str, &fd);
    if (hfind != INVALID_HANDLE_VALUE) {
        do {
            if (fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
                ;//������ļ���
            } else {
                  //������ļ�
                CString strfl = fd.cFileName ;              
                if (strfl.Find(L".bmp") > 0 || strfl.Find(L".BMP") > 0 || strfl.Find(L".jpg") > 0 || strfl.Find(L".JPG") > 0\
                    || strfl.Find(L".gif") > 0 || strfl.Find(L".GIF") > 0) {
                    sI = FilePath + strfl;
                } else if (strfl.Find(L".txt") > 0 || strfl.Find(L".TXT") > 0) {
                      sT = FilePath + strfl;
                } else if (strfl.Find(L".rm") > 0 || strfl.Find(L".RM") > 0 || strfl.Find(L".wav") > 0 || strfl.Find(L".WAV") > 0\
                           || strfl.Find(L".MID") > 0 || strfl.Find(L".mid") > 0) {
                      sA = FilePath + strfl;
                }
                  /*
                  else if ( strfl.Find(L".bmp") > 0 || strfl.Find(L".BMP") > 0 || strfl.Find(L".jpg") > 0 || strfl.Find(L".JPG") > 0\
                  || strfl.Find(L".gif") > 0 || strfl.Find(L".GIF") > 0 )
                  {
                  //����smil  
                  sV = FilePath + strfl;
                  }
                  */
            }
        } while (FindNextFile(hfind, &fd));
    }
    MMS::MMSWarp::GetMMSWarp()->AddPar(&r, Util::StringOp::FromCString(sI).c_str(), Util::StringOp::FromCString(sT).c_str(), Util::StringOp::FromCString(sA).c_str());
    return;
}

void CMultimediaPhoneDlg:: doWithDownLoad() {
    /*std::string filter = "state = '" + Util::StringOp::FromInt(Date::OtaMessage::stUnDownload) + "'";*/

    //
    BOOL flag = FALSE;
    std::string fileter = "type = ";
    fileter += Util::StringOp::FromInt(Data::MMSData::tpReadySend);
    std::vector<boost::shared_ptr<Data::MMSData> > mmsSendResult = Data::MMSData::GetFromDatabase(fileter);
    for (size_t i = 0; i < mmsSendResult.size(); i++) {
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"���ڷ��Ͳ���...", 2000);
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
        ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);

        if (!GetIs3GDialed()) {
            m_bIsDial = TRUE;
            return;
        }

        flag = TRUE;

        //���ŵķ���

        std::string number_;
        std::string number = mmsSendResult[i]->RecipientAddress;
        do {
            BOOL flag = FALSE; 
            int nIndex = number.find(",");
            if (nIndex < 0) {
                nIndex = number.find(";");
            }
            if (nIndex < 0) {
                flag = TRUE;
                number_ = number;
            } else {
                  number_ = number.substr(0, nIndex);
                  number = number.substr(nIndex + 1, number.length() - (nIndex + 1));
            }

            if (number_ != "") {
                CString s = "���ڸ� ";

                if (0 == number_.substr(0, 2).compare("86")) {
                    number_ = number.substr(2);
                }
                std::string name = GetName(number);

                if (!name.empty()) {
                    s += Util::StringOp::ToCString(name);
                } else {
                    s += number_.c_str();
                }

                s += " ���Ͳ���...";
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(s, 5000);
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

                MMS::MMSWarp::MMS_SubmitRequest r;

                r.TransactionID = mmsSendResult[i]->TransactionId;
                r.Subject = Util::StringOp::ToUTF8(mmsSendResult[i]->Subject);
                r.RecipientAddress = number_;
                CString sPath = mmsSendResult[i]->SavePath.c_str();

                ParseSmil(sPath, r);

                char* buf = NULL;
                if (buf == NULL) {
                    buf = new char[1024 * 1024];
                }

                memset(buf, 0, 1024 * 1024);
                int len = 0;
                r.SenderAddress = mmsSendResult[i]->SenderAddress;
                len = MMS::MMSWarp::GetMMSWarp()->EncodeSubmitRequest(&r, buf);

                HttpParam* pHttpParam = new HttpParam;
                pHttpParam->type = 2;           //������
                pHttpParam->apn_type = m_nAPN.apn_type;
                pHttpParam->apn_proxy = m_nAPN.proxy;
                pHttpParam->pData = buf;
                pHttpParam->dataLentg = len;
                pHttpParam->url = m_nAPN.http; 

                extern int HttpProcesse(void*pParam);
                int ret = HttpProcesse(pHttpParam);
                delete[]buf;
                if (ret == 0) {
                    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"���ŷ��ͳɹ�!", 5000);
                    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
                    ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

                    mmsSendResult[i]->type = Data::MMSData::tpSend;
                    mmsSendResult[i]->isRead = true; //��ʾ���ͳɹ�
                    mmsSendResult[i]->Update();
                } else {
                    char txt[64] = {
                        0
                    };
                    char* ResponseStatus[] = {
                        "", "δ֪����", "����ܾ�", "��Ϣ��ʽ����", "���͵�ַ����", "��Ϣδ����", "�������", "��Ϣ���ݴ���", "��֧�ֵ���Ϣ"
                    };
                    if (ret > 0 && ret < 8) {
                        sprintf_s(txt, 64, "���ŷ���ʧ��", ResponseStatus[ret]);
                    } else {
                        strcpy_s(txt, 64, "���ŷ���ʧ��");
                    }
                    CString s = txt;
                      ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(s, false);
                      ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
                      ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

                      mmsSendResult[i]->type = Data::MMSData::tpUnSend;//�ݸ�
                      //mmsSendResult[i]->type = Data::MMSData::tpSend;//�ݸ�
                      mmsSendResult[i]->Update();
                }
                Sleep(50);
            }

            if (flag) {
                break;
            }
        } while (true);

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
        pHttpParam->type = 2;           //������
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
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"���ŷ��ͳɹ�!", 5000);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
        mmsSendResult[i]->type = Data::MMSData::tpSend;
        mmsSendResult[i]->Update();
        }
        else
        {
        char txt[64];
        char* ResponseStatus[] = {"", "δ֪����", "����ܾ�", "��Ϣ��ʽ����", "���͵�ַ����", "��Ϣδ����", "�������", "��Ϣ���ݴ���", "��֧�ֵ���Ϣ"};
        if(ret > 0 && ret < 8)
        sprintf(txt, "���ŷ���ʧ��", ResponseStatus[ret]);
        else
        strcpy(txt, "���ŷ���ʧ��");
        CString s = txt;
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(s, FALSE);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
        ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
        mmsSendResult[i]->type = Data::MMSData::tpUnSend;
        mmsSendResult[i]->Update();
        }
        */
    }

    //����OTA
    std::vector<boost::shared_ptr<Data::OtaMessage> > result = Data::OtaMessage::GetFromDatabase(""); 
    for (size_t i = 0; i < result.size(); i++) {
        if (result[i]->state == Data::OtaMessage::stUnDownload) {
            if (!GetIs3GDialed()) {
                m_bIsDial = TRUE;
                return;
            }
            flag = TRUE;
            HttpParam* pHttpParam = new HttpParam;
            pHttpParam->type = 0;
            pHttpParam->url = (char*)result[i]->url.c_str();
            pHttpParam->apn_type = m_nAPN.apn_type;
            pHttpParam->apn_proxy = m_nAPN.proxy;

            //http ����
            if (result[i]->instruction == Data::tMenuManager) {                  //�˵�ά��
                pHttpParam->dwType = 2;
            } else if (result[i]->instruction == Data::tWeather) {                   //����Ԥ��
                  pHttpParam->dwType = 1;
            } else if (result[i]->instruction == Data::tMultimediaDownload) {    //ý������
                pHttpParam->dwType = 0;
            }
            if (HttpProcesse(pHttpParam) == 0) {
                result[i]->state = Data::OtaMessage::stParsed;
                result[i]->Update();
                SMSSpecRing(1, "");
            } else {
                result[i]->state = Data::OtaMessage::stDownloadedError;
                result[i]->Update();
            }
        } else if (result[i]->state == Data::OtaMessage::stParsed) {
            result[i]->Remove();
        }
    }

    //1528001948

    //���ز���
    std::vector<boost::shared_ptr<Data::MmsMessage> > result_ = Data::MmsMessage::GetFromDatabase(""); 
    for (size_t i = 0; i < result_.size(); i++) {
        if (result_[i]->state == Data::MmsMessage::stUnDownload) {
            if (!GetIs3GDialed()) {
                m_bIsDial = TRUE;
                return;
            }
            flag = TRUE;
            HttpParam* pHttpParam = new HttpParam;
            pHttpParam->type = 0;           
            pHttpParam->dwType = 10;        //��������
            pHttpParam->url = (char*)result_[i]->url.c_str();
            pHttpParam->apn_type = m_nAPN.apn_type;
            pHttpParam->apn_proxy = m_nAPN.proxy;

            int index;
            if ((index = HttpProcesse(pHttpParam)) >= 0) {
                g_iSmsID = ((index << 1) + 1);

                result_[i]->state = Data::MmsMessage::stParsed;
                result_[i]->Update();

                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"���Ž��ܳɹ�!", 5000);
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
                ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

                //�������������Ϣ���Ѿ��յ�����
                char* buf = NULL;
                if (buf == NULL) {
                    buf = new char[1024];
                }

                memset(buf, 0, 1024);
                int len = 0;
                len = MMS::MMSWarp::GetMMSWarp()->EncodeNotificationResponse(result_[i]->transactionId, 0x81, buf);

                HttpParam* pHttpParam = new HttpParam;
                pHttpParam->type = 11;          //�����Ž��ܳɹ�
                pHttpParam->apn_type = m_nAPN.apn_type;
                pHttpParam->apn_proxy = m_nAPN.proxy;
                pHttpParam->pData = buf;
                pHttpParam->dataLentg = len;
                pHttpParam->url = m_nAPN.http; 

                extern int HttpProcesse(void*pParam);
                int ret = HttpProcesse(pHttpParam);
            } else {
                  ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"���Ž���ʧ��!", 10000);
                  ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
                  ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);

                  result_[i]->state = Data::MmsMessage::stDownloadedError;
                  result_[i]->Update();
            }
        } else if (result_[i]->state == Data::MmsMessage::stParsed) {
            result_[i]->Remove();
        }
    }

    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_nAPN.apn_type == (int)CMWAP && flag) {     //�ж��Ƿ���Ҫ��Զ����
        Net3GHungOff();
        Sleep(3000);
        SetAPN((int)CMWAP);
    }
}

/************************************************************************/
/*                      add by qi down                                   */
/************************************************************************/
void CMultimediaPhoneDlg::SaveDataToContact(const std::vector<Util::ATCommandWarp::SIM_FORMAT> vsim) {
    boost::shared_ptr<Data::Contact> pContact;
    pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
    for (size_t i = 0; i < vsim.size() ; i++) {
        pContact->name(vsim[i].name);
        pContact->mobilesTelephone(Data::TelephoneNumber(vsim[i].telnum));
        pContact->groupId(0);
        pContact->Insert();
    }
}

void CMultimediaPhoneDlg::GetDataFromContact(std::vector<Util::ATCommandWarp::SIM_FORMAT>& vsim) {
    std::vector<boost::shared_ptr<Data::Contact> > vfromcontact;
    if (Data::Contact::GetDataCount("") > 0) {
        vfromcontact = Data::Contact::GetFromDatabase("", Data::dDown, Data::Contact::GetCurrentId() + 1);
        int a = vfromcontact.size();
        for (size_t i = 0 ; i < vfromcontact.size() ; i++) {
            Util::ATCommandWarp::SIM_FORMAT sim ;
            sim.name = vfromcontact[i]->name();//����
            sim.telnum = /*(Data::TelephoneNumber)*/(vfromcontact[i]->mobilesTelephone()).number();//�绰
            vsim.push_back(sim);
        }
    }
}

void CMultimediaPhoneDlg::InsertSimToContact(const std::vector<Util::ATCommandWarp::SIM_FORMAT> vsim) {
    m_nSIMID = -1;
    std::vector<boost::shared_ptr<Data::ContactGroup> > result;
    if (Data::ContactGroup::GetDataCount("") > 0) { //����
        result = Data::ContactGroup::GetFromDatabase("");
        for (size_t i = 0; i < result.size(); ++i) {
            if ("SIM��" == result[i]->name()) {
                m_nSIMID = result[i]->id();
                m_pContactDlg->SetSimID(m_nSIMID);
                break;
            }
        }
    }

    if (m_nSIMID == -1) { //ֻ�ܽ���һ��
        //����SIM�����
        boost::shared_ptr<Data::ContactGroup> m_pContactGroup = boost::shared_ptr<Data::ContactGroup>(new Data::ContactGroup);
        m_pContactGroup->name("SIM��");
        m_pContactGroup->ringName("Ĭ��");
        m_pContactGroup->tipName("Ĭ��");
        m_pContactGroup->Insert();
        m_nSIMID = m_pContactGroup->id();
        m_pContactDlg->SetSimID(m_nSIMID);
    }

    //SIM��������Ϣ����ɾ����
    std::vector<boost::shared_ptr<Data::Contact> > result1 = Data::Contact::GetFromDatabase("groupId = " + Util::StringOp::FromInt(m_nSIMID));
    if (!result1.empty()) {
        for (size_t i = 0; i < result1.size(); i++) {
            result1[i]->Remove();
        }
    }

    //��SIM ��Ϣ���뵽 Contact-sim������
    for (size_t i = 0 ; i < vsim.size() ; i++) {
        boost::shared_ptr<Data::Contact> pContact;
        pContact = boost::shared_ptr<Data::Contact>(new Data::Contact);
        pContact->name(vsim[i].name);

        std::string name = vsim[i].name ;

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
    }
}

std::string CMultimediaPhoneDlg::GetAlias(std::string name) {
    if (((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex == NULL) {
        return "";
    }

    std::string alias;
    for (size_t i = 0; i < name.size(); ++i) {
        if ((unsigned char)name[i] > (unsigned char)127) {
            unsigned int index = ((unsigned char)name[i] - 0xb0) * 94 + ((unsigned char)name[i + 1] - 0xa1);

            if (index >= 0 && index < 6768) {
                alias += ((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pContactNewDlg->m_pPinYinIndex[index];
            } else {
            }
            ++i;
        } else {
            if (name[i] >= 'A' && name[i] <= 'Z') {
                alias += char(name[i] + 32);
            } else {
                alias += name[i];
            }
        }
    }
    return alias;
}

std::string CMultimediaPhoneDlg::GetName(std::string number) {
    if (number == "") {
        return number;
    }
    std::string filter;
    filter += "mobilesTelephone = '";
    filter += Data::TelephoneNumber(number).ToString();
    filter += "' OR mobilesTelephone2 = '";
    filter += Data::TelephoneNumber(number).ToString();
    filter += "' OR worksTelephone = '";
    filter += Data::TelephoneNumber(number).ToString();
    filter += "' OR worksTelephone2 = '";
    filter += Data::TelephoneNumber(number).ToString();
    filter += "'";
    std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
    if (!result.empty()) {
        return result[0]->name();
    }
    return "";
}

void CMultimediaPhoneDlg::AddIcon(CString icon) {
    m_vIcon.push_back(icon);//����ͼ��  
    m_MJPGList.SetUnitBitmap(0, icon, L"", true);
}

void CMultimediaPhoneDlg::Desktop() {
    //ֹͣ��������
    m_pSettingDlg->StopTryRing();
    m_pContactGroupDlg->StopTryRing();
    m_pMainDlg->m_mainLunarderDlg_->StopTryRing();

    if (m_pMainDlg->m_mainPhotoDlg_->IsWindowVisible()) {
        m_pMainDlg->m_p3GHomePicDlg->OnExit_();
        m_pMainDlg->m_mainPhotoDlg_->OnExit_();
    }

    if (m_pMainDlg->m_mainMp3Dlg_->IsWindowVisible()) {
        m_pMainDlg->m_p3GHomeJoyDlg->OnExit_();
        m_pMainDlg->m_mainMp3Dlg_->OnExit_(true);
    }

    if (m_pMainDlg->m_mainVideoDlg_->IsWindowVisible()) {
        m_pMainDlg->m_p3GHomeMovieDlg->OnExit_();
        m_pMainDlg->m_mainVideoDlg_->OnExit_();
    }

    if (m_vIcon.size()) {
        m_vIcon.clear();
    }

    CString icon = L".\\adv\\mjpg\\k5\\common\\����ͼ��.bmp";
    AddIcon(icon);

    for (size_t i = 0 ; i < m_vAllCwnd.size() ; i++) {
        if (m_vAllCwnd[i]) {
            m_vAllCwnd[i]->ShowWindow(SW_HIDE);
        }
    }

    m_pMainDlg->HideAllWindow();
    m_pMainDlg->ShowWindow(SW_SHOW);

    m_MJPGList.SetUnitIsShow(10, false, true);
}

bool CMultimediaPhoneDlg::GetSimStatus() {
    return !!m_bInsertSim;
}

void CMultimediaPhoneDlg::AddDesktopBtn() {
    CString desktop_fore = L".\\adv\\mjpg\\k5\\common\\���水ť_fore.png";
    CString desktop_back = L".\\adv\\mjpg\\k5\\common\\����ͼ��_back.png";
    m_MJPGList.SetUnitBitmap(10, desktop_fore, desktop_back, false);
    m_MJPGList.SetUnitIsShow(10, true, true);
}

void CMultimediaPhoneDlg::PopbackIcon() {
    CString icon;   
    m_vIcon.pop_back();
    if (0 == m_vIcon.size()) {
        CString icon = L".\\adv\\mjpg\\k5\\common\\����ͼ��.bmp";
        AddIcon(icon);
    } else {
        icon = m_vIcon[m_vIcon.size() - 1];
        m_MJPGList.SetUnitBitmap(0, icon, L"", true);
    }

    if (1 == m_vIcon.size()) {
        m_MJPGList.SetUnitBitmap(10, L"", L"", true);
        m_MJPGList.SetUnitIsShow(10, false, true);
    }
}

/************************************************************************/
/*                      add by qi up                                     */
/************************************************************************/

void PickupFunc() {
    Dprintf("\nPickupFunc\n");
}

void HandsfreeFunc() {
    Dprintf("\nHandsfreeFunc\n");
}

void HangupFunc() {
    Dprintf("\nHangupFunc\n");
}

void KeypadDialFunc(unsigned char const & keyCode) {
    Dprintf("\nKeypadDialFunc=%d\n", keyCode);
}

void ConnectFunc() {
    Dprintf("\nConnectFunc\n");
}

void RingInFunc(Data::TelephoneNumber number) {
    Dprintf("\nRingInFunc=%s\n", number.number().c_str());
}

void RemoteHangupFunc() {
    Dprintf("\nRemoteHangupFunc\n");
}

void TimeoutFunc() {
    Dprintf("\nTimeoutFunc\n");
}

void ErrorFunc() {
    Dprintf("\nErrorFunc\n");
}
