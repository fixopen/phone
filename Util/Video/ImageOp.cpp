#include <objbase.h>
#include <INITGUID.h>

#include "ImageOp.h"
#include "../Text/StringOp.h"
#include "inc/cximage/ximage.h"

namespace Util {
    namespace Video {
        std::map<std::wstring, HDC> ImageOp::images_;
        //std::map<std::wstring, HBITMAP> ImageOp::masks_;
        std::map<std::wstring, HDC> ImageOp::masks_;

        void ImageOp::DrawImage(HDC const destination, RECT const& dstBounds, std::wstring const& sourceFilename, SIZE const& imageSize, RECT const& srcBounds) {
            DWORD typeId = CxImage::GetTypeIdFromName(findExtension_(sourceFilename.c_str()));
            CxImage img;
            img.Load(sourceFilename.c_str(), typeId);
            img.Crop(srcBounds);
            //img.Blt(destination, dstPosition.x, dstPosition.y);
            img.Stretch(destination, dstBounds);
            //int imageIndex = findCacheIndex_(sourceFilename);
            //if (imageIndex == -1) {
            //    imageIndex = addToImageCache_(sourceFilename, destination, imageSize);
            //}
            ////MaskBlt(destination, x, y, width, heigth, imageCacheDirectory[imageIndex].dc, x, y, bitmap, maskWidth, maskHeight, op);
            //BitBlt(destination,
            //    dstBounds.left, dstBounds.top, dstBounds.right - dstBounds.left, dstBounds.bottom - dstBounds.top,
            //    imageCacheDirectory[imageIndex].dc,
            //    srcBounds.left, srcBounds.top,
            //    SRCCOPY);
        }

        HDC const ImageOp::GetMaskImage(std::wstring const& maskFilename, SIZE const& maskSize) {
            if (masks_.find(maskFilename) == masks_.end()) {
                drawMask_(maskFilename, maskSize);
            }
            return masks_[maskFilename];
        }

        void ImageOp::PrepareImage(std::map<std::wstring, SIZE> const& images) {
            IImagingFactory* const factory = beforeDraw_();
            for (std::map<std::wstring, SIZE>::const_iterator i = images.begin(); i != images.end(); ++i) {
                draw_(factory, i->first, i->second);
            }
            afterDraw_(factory);
        }

        HDC const ImageOp::GetImage(std::wstring const& imageFilename, SIZE const& imageSize) {
            if (images_.find(imageFilename) == images_.end()) {
                drawImage_(imageFilename, imageSize);
            }
            return images_[imageFilename];
        }

        void ImageOp::drawImage_(std::wstring const& imageFilename, SIZE const& imageSize) {
            ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
            ::IImagingFactory* imageFactory = 0;
            ::CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&imageFactory);
            if (imageFactory) {
                IImage* image = 0;
                imageFactory->CreateImageFromFile(imageFilename.c_str(), &image);
                if (image) {
                    HDC dc = ::CreateCompatibleDC(0);
                    HWND desktop = ::GetDesktopWindow();
                    HDC desktopDc = ::GetDC(desktop);
                    HBITMAP bitmap = ::CreateCompatibleBitmap(desktopDc, imageSize.cx, imageSize.cy);
                    ::ReleaseDC(desktop, desktopDc);
                    if (!bitmap) {
                        DWORD errorCode = ::GetLastError();
                        DWORD ec = errorCode;
                    }
                    ::SelectObject(dc, bitmap);
                    RECT bounds;
                    bounds.left = 0;
                    bounds.top = 0;
                    bounds.right = imageSize.cx;
                    bounds.bottom = imageSize.cy;
                    HRESULT r = image->Draw(dc, &bounds, 0);
                    if (r != S_OK) {
                        DWORD errorCode = ::GetLastError();
                        DWORD ec = errorCode;
                    }
                    image->Release();
                    images_.insert(std::make_pair(imageFilename, dc));
                } else {
                    ;//image creation error
                }
                imageFactory->Release();
            } else {
                ;//image factory creation error
            }
            ::CoUninitialize();
        }

        void ImageOp::drawMask_(std::wstring const& imageFilename, SIZE const& imageSize) {
            //HBITMAP bitmap = (HBITMAP)::LoadImage(NULL, maskFilename.c_str(), IMAGE_BITMAP, 0, 0, 0);
            FILE* file = fopen(Util::Text::StringOp::ToUTF8(imageFilename).c_str(), "rb");
            if (file) {
                BITMAPFILEHEADER bitmapFileHeader;
                fread(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), file);
                UINT bitmapInfoLength = (UINT)bitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
                BITMAPINFO* bitmapInfo = (BITMAPINFO*)malloc(bitmapInfoLength);
                fread((void*)bitmapInfo, 1, bitmapInfoLength, file);
                //BITMAPINFOHEADER bitmapInfoHeader;
                //fread(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), file);
                HDC dc = ::CreateCompatibleDC(0);
                void* bits = 0;
                HBITMAP bitmap = ::CreateDIBSection(dc, bitmapInfo, DIB_RGB_COLORS, &bits, NULL, 0);
                size_t currentPos = ftell(file);
                fseek(file, 0, SEEK_END);
                size_t lastPos = ftell(file);
                if (bits) {
                    fseek(file, currentPos, SEEK_SET);
                    fread(bits, 1, lastPos - currentPos, file);
                    ::SelectObject(dc, static_cast<HGDIOBJ>(bitmap));
                    free(bitmapInfo);
                    masks_.insert(std::make_pair(imageFilename, dc));
                }
                fclose(file);
            }
            /*
            CFile file;
            if (file.Open(maskFilename.c_str(), CFile::modeRead)) {
            BITMAPFILEHEADER bitmapFileHeader;
            file.Read(&bitmapFileHeader, sizeof(BITMAPFILEHEADER));
            ULONGLONG fileSize = file.GetLength();
            if ((bitmapFileHeader.bfType == (WORD)(('M' << 8) | 'B'))
            && (bitmapFileHeader.bfSize == fileSize)) {
            BITMAPINFOHEADER bitmapInfoHeader;
            file.Read(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));
            if (bitmapInfoHeader.biSize == sizeof(BITMAPINFOHEADER)) {
            HDC dc = ::CreateCompatibleDC(0);
            UINT bitmapInfoLength = (UINT)bitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
            BITMAPINFO* bitmapInfo = (BITMAPINFO*)new BYTE[bitmapInfoLength];
            file.Read((LPVOID)bitmapInfo, bitmapInfoLength);
            void* bits = 0;
            HBITMAP bitmap = ::CreateDIBSection(dc, bitmapInfo, DIB_RGB_COLORS, &bits, NULL, 0);
            masks_.insert(std::make_pair(maskFilename, bitmap));
            }
            }
            file.Close();
            }
            */
        }

        IImagingFactory* const ImageOp::beforeDraw_() {
            IImagingFactory* result = 0;
            ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
            IImagingFactory* imageFactory = 0;
            ::CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&imageFactory);
            if (imageFactory) {
                result = imageFactory;
            }
            return result;
        }

        void ImageOp::draw_(IImagingFactory* const imageFactory, std::wstring const& imageFilename, SIZE const& imageSize) {
            IImage* image = 0;
            imageFactory->CreateImageFromFile(imageFilename.c_str(), &image);
            if (image) {
                HDC dc = ::CreateCompatibleDC(0);
                HWND desktop = ::GetDesktopWindow();
                HDC desktopDc = ::GetDC(desktop);
                HBITMAP bitmap = ::CreateCompatibleBitmap(desktopDc, imageSize.cx, imageSize.cy);
                ::ReleaseDC(desktop, desktopDc);
                if (!bitmap) {
                    DWORD errorCode = ::GetLastError();
                    DWORD ec = errorCode;
                }
                ::SelectObject(dc, bitmap);
                RECT bounds;
                bounds.left = 0;
                bounds.top = 0;
                bounds.right = imageSize.cx;
                bounds.bottom = imageSize.cy;
                HRESULT r = image->Draw(dc, &bounds, 0);
                if (r != S_OK) {
                    DWORD errorCode = ::GetLastError();
                    DWORD ec = errorCode;
                }
                image->Release();
                images_.insert(std::make_pair(imageFilename, dc));
            }
        }

        void ImageOp::afterDraw_(IImagingFactory* const imageFactory) {
            imageFactory->Release();
            ::CoUninitialize();
        }

        wchar_t const* const ImageOp::findExtension_(wchar_t const* const name) {
            int len = wcslen(name);
            for (int i = len - 1; i >= 0; --i) {
                if (name[i] == '.') {
                    return (wchar_t*)(name + i + 1);
                }
            }
            return (wchar_t*)(name + len);
        }
    }
}
#if 0

//std::vector<ImageOp::ImageCacheEntry> ImageOp::imageCacheDirectory;
//void ImageOp::DrawImage(HDC const destination, RECT const& dstBounds, std::wstring const& sourceFilename, SIZE const& imageSize, RECT const& srcBounds) {
//    //DWORD typeId = CxImage::GetTypeIdFromName(findExtension_((LPCTSTR)sourceFilename));
//    //CxImage img;
//    //img.Load((LPCTSTR)sourceFilename, typeId);
//    //img.Crop(sourcePosition);
//    ////img.Blt(destination, destPosition.x, destPosition.y);
//    //img.Stretch(destination, destPosition);
//    int imageIndex = findCacheIndex_(sourceFilename);
//    if (imageIndex == -1) {
//        imageIndex = addToImageCache_(sourceFilename, destination, imageSize);
//    }
//    //MaskBlt(destination, x, y, width, heigth, imageCacheDirectory[imageIndex].dc, x, y, bitmap, maskWidth, maskHeight, op);
//    BitBlt(destination,
//        dstBounds.left, dstBounds.top, dstBounds.right - dstBounds.left, dstBounds.bottom - dstBounds.top,
//        imageCacheDirectory[imageIndex].dc,
//        srcBounds.left, srcBounds.top,
//        SRCCOPY);
//}

//HDC const ImageOp::GetImage(std::wstring const& filename, SIZE const& imageSize, HDC const hDC) {
//    HDC result = 0;
//    if (findCacheIndex_(filename) == -1) {
//        ImageCacheEntry memoryInfo;
//        memoryInfo.filename = filename;
//        memoryInfo.dc = CreateCompatibleDC(hDC);
//        result = memoryInfo.dc;
//        memoryInfo.bitmap = (HBITMAP)LoadImage(NULL, filename.c_str(), IMAGE_BITMAP, 0, 0, 0);
//        if (memoryInfo.bitmap == NULL) {
//            DWORD errorCode = GetLastError();
//            TCHAR errorString[1024] = {0};
//            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, errorString, sizeof errorString, 0);
//            MessageBox(NULL, errorString, L"warning", MB_OK);
//        } else {
//            memoryInfo.oldBitmap = (HBITMAP)SelectObject(memoryInfo.dc, memoryInfo.bitmap);
//            imageCacheDirectory.push_back(memoryInfo);
//        }
//    } else {
//        ::MessageBox(NULL, L"has exist!", L"warning", MB_OK);
//    }
//    return result;
//}

//int const ImageOp::findCacheIndex_(std::wstring const& filename) {
//    int result = -1;
//    for (size_t i = 0; i < imageCacheDirectory.size(); ++i) {
//        if (filename == imageCacheDirectory[i].filename) {
//            result = i;
//            break;
//        }
//    }
//    return result;
//}

//int const ImageOp::addToImageCache_(std::wstring const& filename, HDC const dc, SIZE const& imageSize) {
//    int result = -1;
//    CoInitializeEx(NULL, COINIT_MULTITHREADED);
//    IImagingFactory* imageFactory = 0;
//    CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&imageFactory);
//    if (imageFactory) {
//        IImage* image = 0;
//        imageFactory->CreateImageFromFile(filename.c_str(), &image);
//        if (image) {
//            ImageCacheEntry imageCacheEntry;
//            imageCacheEntry.filename = filename;
//            imageCacheEntry.dc = CreateCompatibleDC(0/*dc*/);
//            imageCacheEntry.bitmap = CreateCompatibleBitmap(dc, imageSize.cx, imageSize.cy);
//            SelectObject(imageCacheEntry.dc, imageCacheEntry.bitmap);
//            RECT bounds;
//            bounds.left = 0;
//            bounds.top = 0;
//            bounds.right = imageSize.cx;
//            bounds.bottom = imageSize.cy;
//            HRESULT r = image->Draw(imageCacheEntry.dc, &bounds, 0);
//            if (r != S_OK) {
//                DWORD errorCode = GetLastError();
//            }
//            image->Release();
//            imageCacheDirectory.push_back(imageCacheEntry);
//            result = imageCacheDirectory.size() - 1;
//        } else {
//            ;//image creation error
//        }
//        imageFactory->Release();
//    } else {
//        ;//image factory creation error
//    }
//    CoUninitialize();
//    return result;
//}

//SIZE const ImageOp::getImageSize_(std::wstring const& filename) {
//    SIZE result;
//    return result;
//}
void f() {
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
        dwStart   =   GetTickCount();

        if(((lpBitmap->bmiHeader.biWidth == BMP_WIDTH && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT) && (!(rect.Width() >= BMP_WIDTH && rect.Height() >= BMP_HEIGHT))) ||\
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH1 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT1) && (!(rect.Width() >= BMP_WIDTH1 && rect.Height() >= BMP_HEIGHT1)))||\
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH2 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT2) && (!(rect.Width() >= BMP_WIDTH2 && rect.Height() >= BMP_HEIGHT2))) ||\
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH3 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT3) && (!(rect.Width() >= BMP_WIDTH3 && rect.Height() >= BMP_HEIGHT3)))||\
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH4 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT4) && (!(rect.Width() >= BMP_WIDTH4 && rect.Height() >= BMP_HEIGHT4))))
        {
            ;
        }
        else
        {
            BOOL flag = FALSE;
            if(BMP_WIDTH == lpBitmap->bmiHeader.biWidth || BMP_WIDTH1 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH2 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH3 == lpBitmap->bmiHeader.biWidth)
            {
                flag = TRUE;

                dwStart = GetTickCount();
                int index;;
                if(FindMemoryInfo(szFileImage, index))
                {
                    if(index >= 0 && index < ALLBMPMEMORY_MAX)
                    {
                        BitBlt(pdc->m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), g_allBmpMemory[index].hdc, 0, 0, SRCCOPY);

                        offset = GetTickCount() - dwStart;   
                        file.Close();
                        return;
                    }
                }
            }
            dwStart = GetTickCount();

            file.Close();
            delete []lpBitmap;    //20090216 test

            IImage   *pImage   =   NULL; 

            if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
            { 
                if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
                { 
                    pImage->Draw(pdc->m_hDC, rect, NULL); 
                    pImage->Release(); 

                    if(flag)
                        AddALLBmpMemoryInfo(szFileImage, rect, pdc);

                    ::Sleep(1);
                } 
                else
                {
                }

                pImgFactory-> Release(); 
            }

            offset = GetTickCount() - dwStart;   
            return;
        }
        int w = (((lpBitmap->bmiHeader.biWidth*lpBitmap->bmiHeader.biBitCount)+31)>>5)<<2;
        int h = lpBitmap->bmiHeader.biHeight;
        lpBitmap->bmiHeader.biHeight = rect.Height();
        lpBitmap->bmiHeader.biWidth = rect.Width();

        CDC   dcMem;   
        dcMem.CreateCompatibleDC(pdc);   
        HBITMAP   hBmp = ::CreateDIBSection(dcMem.m_hDC, lpBitmap, DIB_RGB_COLORS, &lpBits, NULL, 0);
        if(lpBits == NULL)
        {
            extern VOID WriteLog(CHAR* str);
            char restartTimer[64];
            SYSTEMTIME tm;
            GetLocalTime(&tm);
            sprintf(restartTimer, "memleak watchdog:%04d-%02d-%02d %02d:%02d:%02d \r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
            WriteLog(restartTimer);

            file.Close();
            g_MemoryLeak = TRUE;
            ::MessageBox(NULL,L"Draw BMP no memory!", L"warning", MB_OK);  
            delete []lpBitmap;    //20090216 test
            return;
        }

        offset = GetTickCount() - dwStart;   
        dwStart   =   GetTickCount();

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
        dwStart   =   GetTickCount();

        CBitmap   BmpImg;   
        BmpImg.Attach(hBmp);   

        CBitmap  *pOldBmp=dcMem.SelectObject(&BmpImg);   

        offset = GetTickCount() - dwStart;   
        dwStart   =   GetTickCount(); 

        pdc->BitBlt(newRect.left, newRect.top, newRect.Width(), newRect.Height(), &dcMem, 0, 0, SRCCOPY);

        dcMem.SelectObject(pOldBmp); 

        DeleteObject(hBmp);

        BmpImg.Detach();
        dcMem.DeleteDC();

        delete []lpBitmap;

        offset = GetTickCount() - dwStart;   
        ::Sleep(1);
    }
    else
    {
        DWORD   dwStart   =   GetTickCount(); 	

        IImage   *pImage   =   NULL; 

        if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
        { 
            if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
            { 
                pImage->Draw(pdc->m_hDC, rect, NULL); 
                pImage->Release(); 
                ::Sleep(1);
            } 
            else
            {
            }

            pImgFactory-> Release(); 
        } 
        int offset = GetTickCount() - dwStart;  
    }


    CRect newRect = rect;
    if(isOffset)
    {
        newRect = CRect(0, 0, rect.Width(), rect.Height());
    }

    CString sFile = szFileImage;
    if(sFile.Find(L".bmp") != -1 || sFile.Find(L".BMP") != -1)
    {
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
            delete []lpBitmap;
            return;   
        }     
        DWORD   dwBitlen=bfhHeader.bfSize   -   bfhHeader.bfOffBits;     
        LPVOID   lpBits;   
        DWORD offset = GetTickCount() - dwStart;   
        dwStart   =   GetTickCount();

        /*
        如果背景图是 800*420，692*296，535*420，800*60，Unit框小于他们，则取出相应的BMP画出来.否则整个画
        */
        if(((lpBitmap->bmiHeader.biWidth == BMP_WIDTH && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT) && (!(rect.Width() >= BMP_WIDTH && rect.Height() >= BMP_HEIGHT))) ||\
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH1 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT1) && (!(rect.Width() >= BMP_WIDTH1 && rect.Height() >= BMP_HEIGHT1)))||\
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH2 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT2) && (!(rect.Width() >= BMP_WIDTH2 && rect.Height() >= BMP_HEIGHT2))) ||\
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH3 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT3) && (!(rect.Width() >= BMP_WIDTH3 && rect.Height() >= BMP_HEIGHT3)))||\
            ((lpBitmap->bmiHeader.biWidth == BMP_WIDTH4 && lpBitmap->bmiHeader.biHeight == BMP_HEIGHT4) && (!(rect.Width() >= BMP_WIDTH4 && rect.Height() >= BMP_HEIGHT4))))
        {
            ;
        }
        else
        {
            BOOL flag = FALSE;
            if(BMP_WIDTH == lpBitmap->bmiHeader.biWidth || BMP_WIDTH1 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH2 == lpBitmap->bmiHeader.biWidth ||BMP_WIDTH3 == lpBitmap->bmiHeader.biWidth)
            {
                flag = TRUE;
            }
            dwStart = GetTickCount();

            file.Close();
            delete []lpBitmap;

            IImage   *pImage   =   NULL; 
            if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
            { 
                if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
                { 
                    pImage->Draw(pdc, rect, NULL); 
                    pImage->Release(); 

                    ::Sleep(1);
                } 
                else
                {
                }

                pImgFactory-> Release(); 
            }

            offset = GetTickCount() - dwStart;   
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
            //重启机器
            extern VOID WriteLog(CHAR* str);
            char restartTimer[64];
            SYSTEMTIME tm;
            GetLocalTime(&tm);
            sprintf(restartTimer, "memleak watchdog:%04d-%02d-%02d %02d:%02d:%02d \r\n", tm.wYear, tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond);
            WriteLog(restartTimer);

            file.Close();
            g_MemoryLeak = TRUE;
            ::MessageBox(NULL,L"Draw BMP no memory!", L"warning", MB_OK);  
            delete []lpBitmap;
            return;
        }

        offset = GetTickCount() - dwStart;   
        dwStart   =   GetTickCount();
        int off = (h-rect.top-rect.Height())*w + rect.left*(lpBitmap->bmiHeader.biBitCount/8);

        if(off < 0)
            off = 0;
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
        dwStart   =   GetTickCount();

        HBITMAP pOldBmp= (HBITMAP)::SelectObject(dcMem, hBmp);   

        offset = GetTickCount() - dwStart;   
        dwStart   =   GetTickCount(); 

        ::BitBlt(pdc, newRect.left, newRect.top, newRect.Width(), newRect.Height(), dcMem, 0, 0, SRCCOPY);

        ::SelectObject(dcMem, pOldBmp); 

        ::DeleteObject(hBmp);
        ::DeleteDC(dcMem);

        delete []lpBitmap;

        offset = GetTickCount() - dwStart;   
        ::Sleep(1);
    }
    else
    {
        DWORD   dwStart   =   GetTickCount(); 	
        IImage   *pImage   =   NULL; 
        if (SUCCEEDED(CoCreateInstance   (CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void   **)&pImgFactory))) 
        { 
            if (SUCCEEDED(pImgFactory-> CreateImageFromFile(szFileImage, &pImage))) 
            { 
                pImage->Draw(pdc, rect, NULL); 
                pImage->Release(); 
                ::Sleep(1);
            } 
            else
            {
            }
            pImgFactory-> Release(); 
        } 
        int offset = GetTickCount() - dwStart;  
    }
}
namespace {
    struct ImageCacheEntry {
        std::wstring filename;
        HDC hDC;
        HBITMAP bitmap;
        HBITMAP oldBitmap;
    };
    std::vector<ImageCacheEntry> imageCacheDirectory;

    int findCacheIndex_(std::wstring const& filename) {
        int result = -1;
        for (size_t i = 0; i < imageCacheDirectory.size(); ++i) {
            if (filename == imageCacheDirectory[i].filename) {
                result = i;
                break;
            }
        }
        return result;
    }

    void AddToMemory_(std::wstring const& filename, HDC const hDC) {
        if (findCacheIndex_(filename) == -1) {
            ImageCacheEntry memoryInfo;
            memoryInfo.filename = filename;
            memoryInfo.hDC = CreateCompatibleDC(hDC);
            memoryInfo.bitmap = (HBITMAP)LoadImage(NULL, filename.c_str(), IMAGE_BITMAP, 0, 0, 0);
            if (memoryInfo.bitmap == NULL) {
                DWORD errorCode = GetLastError();
                TCHAR errorString[1024] = {0};
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, errorString, sizeof errorString, 0);
                MessageBox(NULL, errorString, L"warning", MB_OK);
            } else {
                memoryInfo.oldBitmap = (HBITMAP)SelectObject(memoryInfo.hDC, memoryInfo.bitmap);
            }
        } else {
            ::MessageBox(NULL, L"has exist!", L"warning", MB_OK);
        }
    }
}
bool const LoadImageFile(std::wstring const& filename, SIZE const& size, HDC const dc, RECT const& bounds) {
    bool result = false;
    //打开文件
    HANDLE file = CreateFile(filename.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (file != INVALID_HANDLE_VALUE) {
        //取得文件大小
        DWORD fileSize = GetFileSize(file, NULL);
        if ((DWORD)-1 != fileSize) {
            //按文件大小分配内存
            HGLOBAL memoryHandle = GlobalAlloc(GMEM_MOVEABLE, fileSize);
            if (NULL != memoryHandle) {
                //锁定内存
                void* data = GlobalLock(memoryHandle);
                if (NULL != data) {
                    //读取图像文件
                    DWORD fileRead = 0;
                    BOOL read = ReadFile(file, data, fileSize, &fileRead, NULL);
                    if (FALSE != read) {
                        //从已分配内存生成IStream流
                        IStream* stream = NULL;
                        HRESULT hr = CreateStreamOnHGlobal(memoryHandle, TRUE, &stream);
                        if (SUCCEEDED(hr)) {
                            IPicture* picture;
                            hr = OleLoadPicture(stream, fileSize, FALSE, IID_IPicture, (LPVOID*)&picture);
                            if (SUCCEEDED(hr)) {
                                OLE_XPOS_HIMETRIC sx = bounds.left;
                                OLE_YPOS_HIMETRIC sy = bounds.top;
                                OLE_XSIZE_HIMETRIC logicalWidth;
                                OLE_YSIZE_HIMETRIC logicalHeight;
                                picture->get_Width(&logicalWidth);
                                picture->get_Height(&logicalHeight);
                                if (SUCCEEDED(picture->Render(dc->m_hDC, sx, sy, bounds.Width(), bounds.Height(), sx, logicalHeight - sy, logicalWidth, -logicalHeight, NULL))) {
                                    result = true;
                                } else {
                                    MessageBox(NULL, _T("显示操作失败"), L"警告", MB_OK);
                                    picture->Release();
                                }
                            } else if (picture != NULL) {
                                MessageBox(NULL, _T("加载操作失败"), L"警告", MB_OK);
                                picture->Release();
                            } else {
                                MessageBox(NULL, _T("加载操作失败"), L"警告", MB_OK);
                            }
                            //释放IStream流
                            stream->Release();
                        } else {
                            MessageBox(NULL, _T("生成流操作失败"), L"警告", MB_OK);
                        }
                    } else {
                        MessageBox(NULL, _T("读文件出错"), L"警告", MB_OK);
                    }
                    //解锁内存
                    GlobalUnlock(memoryHandle);
                } else {
                    MessageBox(NULL, _T("无法锁定内存"), L"警告", MB_OK);
                    //释放内存
                    GlobalFree(memoryHandle);
                }
            } else {
                MessageBox(NULL, _T("内存不足，无法分配足够内存"), L"警告", MB_OK);
            }
        } else {
            MessageBox(NULL, _T("图像文件是空的"), L"警告", MB_OK);
        }
        //关闭文件
        CloseHandle(file);
    } else {
        CString error;
        error.Format(_T("%s无法被打开"), filename);
        MessageBox(NULL, error, L"警告", MB_OK);
    }
    return result;
}
void DrawImage(std::wstring const& filename, HDC const hDC, CRect const& srcBounds, CRect const& dstBounds) {
    CFile file;
    if (file.Open((LPCTSTR)filename, CFile::modeRead)) {
        BITMAPFILEHEADER bitmapFileHeader;
        file.Read(&bitmapFileHeader, sizeof(BITMAPFILEHEADER));
        ULONGLONG fileSize = file.GetLength();
        if ((bitmapFileHeader.bfType == (WORD)(('M' << 8) | 'B'))
            && (bitmapFileHeader.bfSize == fileSize)) {
                BITMAPINFOHEADER bitmapInfoHeader;
                file.Read(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));
                if (bitmapInfoHeader.biSize == sizeof(BITMAPINFOHEADER)) {
                    LONG width = bitmapInfoHeader.biWidth;
                    LONG height = bitmapInfoHeader.biHeight;
                    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, fileSize);
                    if (hGlobal) {
                        LPVOID data = GlobalLock(hGlobal);
                        file.SeekToBegin();
                        file.Read(data, fileSize);
                        GlobalUnlock(hGlobal);
                        IStream* stream;
                        CreateStreamOnHGlobal(hGlobal, TRUE, &stream);
                        IPicture* picture;
                        if (SUCCEEDED(OleLoadPicture(stream, fileSize, TRUE, IID_IPicture, (LPVOID*)&picture))) {
                            OLE_XSIZE_HIMETRIC logicalWidth;
                            OLE_YSIZE_HIMETRIC logicalHeight;
                            picture->get_Width(&logicalWidth);
                            picture->get_Height(&logicalHeight);
                            OLE_XPOS_HIMETRIC logicalX = srcBounds.left * logicalWidth / width;
                            OLE_YPOS_HIMETRIC logicalY = srcBounds.top * logicalHeight / height;
                            if (SUCCEEDED(picture->Render(hDC, dstBounds.left, dstBounds.top, dstBounds.Width(), dstBounds.Height(), logicalX, logicalHeight - logicalY, logicalWidth, -logicalHeight, NULL))) {
                                //ok. finally
                            } else {
                                ::MessageBox(NULL, L"render image failure", L"warning", MB_OK);
                            }
                            picture->Release();
                        } else {
                            ::MessageBox(NULL, L"load picture failure", L"warning", MB_OK);
                        }
                        stream->Release();
                        //GlobalFree(hGlobal);
                    } else {
                        ::MessageBox(NULL, L"momory alloc failure", L"warning", MB_OK);
                    }
                } else {
                    ::MessageBox(NULL, L"file format is not Windows 3.0!", L"warning", MB_OK);
                }
        } else {
            ::MessageBox(NULL, L"file format is not BMP!", L"warning", MB_OK);
        }
        file.Close();
    } else {
        ::MessageBox(NULL, L"open file failure!", L"warning", MB_OK);
    }
    int index = findIndex_(filename);
    if (index == -1) {
        AddToMemory_(filename, hDC);
    }
    BitBlt(hDC, dstBounds.left, dstBounds.top, dstBounds.Width(), dstBounds.Height(), Memory[index].hDC, srcBounds.left, srcBounds.top, SRCCOPY);
}
CFile file;
CFileException ex;   
if (file.Open((LPCTSTR)filename, CFile::modeRead, &ex)) {
    BITMAPFILEHEADER bitmapFileHeader;
    file.Read(&bitmapFileHeader, sizeof(BITMAPFILEHEADER));
    ULONGLONG fileSize = file.GetLength();
    if ((bitmapFileHeader.bfType == (WORD)(('M' << 8) | 'B'))
        && (bitmapFileHeader.bfSize == fileSize)) {
            BITMAPINFOHEADER bitmapInfoHeader;
            file.Read(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));
            if (bitmapInfoHeader.biSize == sizeof(BITMAPINFOHEADER)) {
                LONG width = bitmapInfoHeader.biWidth;
                LONG height = bitmapInfoHeader.biHeight;
                int index = findIndex_(filename);
                if (index != -1) {
                    BitBlt(hDC, dstBounds.left, dstBounds.top, dstBounds.Width(), dstBounds.Height(), Memory[index].hDC, srcBounds.left, srcBounds.top, SRCCOPY);
                } else {
                    CoInitializeEx(NULL, COINIT_MULTITHREADED);
                    IImagingFactory* imageFactory = 0;
                    CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void**)&imageFactory);
                    IImage* image = NULL;
                    if (SUCCEEDED(imageFactory->CreateImageFromFile((LPCTSTR)filename, &image))) {
                        image->Draw(hDC, dstBounds, NULL);
                        image->Release();
                        //@@AddToMemory_(filename, srcBounds, hDC);
                        ::Sleep(1);
                    } else {
                        ::MessageBox(NULL, L"DrawImage error", L"warning", MB_OK);
                    }
                    imageFactory->Release();
                    CoUninitialize();
                }
            } else {
                ::MessageBox(NULL, L"file format is not Windows 3.0!", L"warning", MB_OK);
            }
    } else {
        ::MessageBox(NULL, L"file format is not BMP!", L"warning", MB_OK);
    }
    file.Close();
} else {
    TCHAR szError[1024] = {0};
    int error = GetLastError();
    ex.GetErrorMessage(szError, 1024);
    ::MessageBox(NULL, szError, L"warning", MB_OK);
}

BITMAPINFOHEADER bitmapInfoHeader;
memset(&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
bitmapInfoHeader.biWidth = bounds.Width();
bitmapInfoHeader.biHeight = bounds.Height();
bitmapInfoHeader.biPlanes = 1;
bitmapInfoHeader.biBitCount = 24;
BYTE* bitmapBits = NULL;
memoryInfo.bitmap = CreateDIBSection(memoryInfo.hDC, (BITMAPINFO*)&bitmapInfoHeader, DIB_RGB_COLORS, (void **)&bitmapBits, NULL, 0);
if (bitmapBits) {
    int w = (((bitmapInfoHeader.biWidth * bitmapInfoHeader.biBitCount) + 31) >> 5) << 2;
    int h = bitmapInfoHeader.biHeight;
    bitmapInfoHeader.biHeight = bounds.Height();
    bitmapInfoHeader.biWidth = bounds.Width();

    CFile file;
    file.Open((LPCTSTR)filename, CFile::modeRead);
    int offset = (h - bounds.top - bounds.Height()) * w + bounds.left * (bitmapInfoHeader.biBitCount / 8);
    file.Seek(offset, CFile::current);
    int size = (((bounds.Width() * bitmapInfoHeader.biBitCount) + 31) >> 5) << 2;
    int offline = w - size;

    for (int i = 0; i < bounds.Height(); ++i) {
        file.Read(bitmapBits, size);
        bitmapBits += size;
        file.Seek(offline, CFile::current);
    }
    file.Close();

    //CDC memoryDC;
    //memoryDC.CreateCompatibleDC(hDC);
    //HBITMAP bitmap = ::CreateDIBSection(memoryDC.m_hDC, &bitmapInfoHeader, DIB_RGB_COLORS, &bitmapBits, NULL, 0);
    //CBitmap bitmapImage;
    //bitmapImage.Attach(bitmap);
    //CBitmap* oldBitmap = memoryDC.SelectObject(&bitmapImage);
    //hDC->BitBlt(newRect.left, newRect.top, newRect.Width(), newRect.Height(), &memoryDC, 0, 0, SRCCOPY);
    //memoryDC.SelectObject(oldBitmap);
    //delete[] bitmapBits;
    //DeleteObject(bitmap);
    //memoryInfo.oldBitmap = (HBITMAP)SelectObject(memoryInfo.hDC, memoryInfo.bitmap);
    //BitBlt(memoryInfo.hDC, 0, 0, bounds.Width(), bounds.Height(), hDC, 0, 0, SRCCOPY);
    Memory.push_back(memoryInfo);
} else {
    ::MessageBox(NULL, L"Draw BMP no memory!", L"warning", MB_OK);
}
#endif
