//#include "StdAfx.h"

#undef IMPORT
#define EXPORT
#include "ImageOperator.h"

#include "../inc/cximage/ximage.h"

#include <vector>
#include <INITGUID.h>
#include <imaging.h>

namespace Util {
    void ImageOperator::DrawImage(HDC destination, RECT destPosition, CString sourceFilename, RECT sourcePosition)
    {
        DWORD typeId = CxImage::GetTypeIdFromName(findExtension_((LPCTSTR)sourceFilename));
        CxImage img;
        img.Load((LPCTSTR)sourceFilename, typeId);
        img.Crop(sourcePosition);
        //img.Blt(destination, destPosition.x, destPosition.y);
        img.Stretch(destination, destPosition);
    }

    void ImageOperator::DrawImage(HDC destination, RECT destPosition, CString sourceFilename)
    {
        DWORD typeId = CxImage::GetTypeIdFromName(findExtension_((LPCTSTR)sourceFilename));
        CxImage img;
        img.Load((LPCTSTR)sourceFilename, typeId);
        //img.Blt(destination, destPosition.x, destPosition.y);
        img.Stretch(destination, destPosition);
    }

    void ImageOperator::DrawImage(HDC destination, CString sourceFilename, RECT sourcePosition)
    {
        DWORD typeId = CxImage::GetTypeIdFromName(findExtension_((LPCTSTR)sourceFilename));
        CxImage img;
        img.Load((LPCTSTR)sourceFilename, typeId);
        img.Crop(sourcePosition);
        img.Blt(destination);
    }

    void ImageOperator::DrawImage(HDC destination, CString sourceFilename)
    {
        DWORD typeId = CxImage::GetTypeIdFromName(findExtension_((LPCTSTR)sourceFilename));
        CxImage img;
        img.Load((LPCTSTR)sourceFilename, typeId);
        img.Blt(destination);
    }

    TCHAR* ImageOperator::findExtension_(TCHAR const* name)
    {
        int len = wcslen(name);
        for (int i = len - 1; i >= 0; --i) {
            if (name[i] == '.') {
                return (TCHAR*)(name + i + 1);
            }
        }
        return (TCHAR*)(name + len);
    }

    namespace {
        struct MemoryInfo {
            CString filename;
            HDC hDC;
            HBITMAP bitmap;
            HBITMAP oldBitmap;
        };
        std::vector<MemoryInfo> Memory;

        int findIndex_(CString filename) {
            int result = -1;
            for (size_t i = 0; i < Memory.size(); ++i) {
                if (filename == Memory[i].filename) {
                    result = i;
                    break;
                }
            }
            return result;
        }

        void AddToMemory_(CString filename, HDC hDC) {
            if (findIndex_(filename) == -1) {
                MemoryInfo memoryInfo;
                memoryInfo.filename = filename;
                memoryInfo.hDC = CreateCompatibleDC(hDC);
                memoryInfo.bitmap = (HBITMAP)LoadImage(NULL, (LPCTSTR)filename, IMAGE_BITMAP, 0, 0, 0);
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

        void DrawImage(CString filename, HDC hDC, CRect srcBounds, CRect dstBounds) {
#if 0
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
#else
            int index = findIndex_(filename);
            if (index == -1) {
                AddToMemory_(filename, hDC);
            }
            BitBlt(hDC, dstBounds.left, dstBounds.top, dstBounds.Width(), dstBounds.Height(), Memory[index].hDC, srcBounds.left, srcBounds.top, SRCCOPY);
#endif
        }
    }
}
#if 0
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
