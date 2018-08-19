#include "stdafx.h"
#include "DIBSectionCE.h"

namespace Control {
    // Use C++ exception handling instead of structured on non-CE platforms.
#ifndef _WIN32_WCE  
#undef TRY
#undef CATCH
#undef END_CATCH
#define TRY try
#define CATCH(ex_class, ex_object) catch(ex_class* ex_object)
#define END_CATCH
#endif  // _WIN32_WCE

    // Standard colours
    RGBQUAD CDIBSectionCE::ms_StdColours[] = {
        { 0x00, 0x00, 0x00, 0 },    // First 2 will be palette for monochrome bitmaps
        { 0xFF, 0xFF, 0xFF, 0 },

        { 0x00, 0x00, 0xFF, 0 },    // First 16 will be palette for 16 colour bitmaps
        { 0x00, 0xFF, 0x00, 0 },
        { 0x00, 0xFF, 0xFF, 0 },
        { 0xFF, 0x00, 0x00, 0 },
        { 0xFF, 0x00, 0xFF, 0 },
        { 0xFF, 0xFF, 0x00, 0 },

        { 0x00, 0x00, 0x80, 0 },
        { 0x00, 0x80, 0x00, 0 },
        { 0x00, 0x80, 0x80, 0 },
        { 0x80, 0x00, 0x00, 0 },
        { 0x80, 0x00, 0x80, 0 },
        { 0x80, 0x80, 0x00, 0 },
        { 0x80, 0x80, 0x80, 0 },

        { 0xC0, 0xC0, 0xC0, 0 },
    };


    /////////////////////////////////////////////////////////////////////////////
    // CE DIBSection global functions

    UINT CEGetDIBColorTable(HDC hdc, UINT uStartIndex, UINT cEntries, RGBQUAD *pColors);

    /////////////////////////////////////////////////////////////////////////////
    // CDIBSectionCE static functions

    // 
    // --- In?: nBitsPerPixel - bits per pixel
    // --- Out : 
    // --- Returns :The number of colours for this colour depth
    // --- Effect : Returns the number of color table entries given the number
    //              of bits per pixel of a bitmap
    /*static*/ int CDIBSectionCE::NumColorEntries(int nBitsPerPixel) 
    {
        int nColors = 0;

        switch (nBitsPerPixel) 
        {
        case 1:  nColors = 2;   break;
#ifdef _WIN32_WCE
        case 2:  nColors = 4;   break;   // winCE only       
#endif
        case 4:  nColors = 16;  break;
        case 8:  nColors = 256; break;
        case 16:
        case 24:
        case 32: nColors = 0;   break;   // 16,24 or 32 bpp have no color table

        default:
            ASSERT(FALSE);
        }

        return nColors;
    }

    // 
    // --- In?: nWidth - image width in pixels
    //           nBitsPerPixel - bits per pixel
    // --- Out :
    // --- Returns : Returns the number of storage bytes needed for each scanline 
    //               in the bitmap
    // --- Effect : 
    /*static*/ int CDIBSectionCE::BytesPerLine(int nWidth, int nBitsPerPixel)
    {
        int nBytesPerLine = nWidth * nBitsPerPixel;
        nBytesPerLine = ( (nBytesPerLine + 31) & (~31) ) / 8;

        return nBytesPerLine;
    }

#ifndef DIBSECTION_NO_PALETTE

    // 
    // --- In?: palette - reference to a palette object which will be filled
    //           nNumColours - number of colour entries to fill
    // --- Out :
    // --- Returns : TRUE on success, false otherwise
    // --- Effect : Creates a halftone color palette independant of screen color depth.
    //              palette will be filled with the colors, and nNumColours is the No.
    //              of colors to file. If nNumColoursis 0 or > 256, then 256 colors are used.
    /*static*/ BOOL CDIBSectionCE::CreateHalftonePalette(CPalette& palette, int nNumColours)
    {
        palette.DeleteObject();

        int nNumStandardColours = sizeof(ms_StdColours) / sizeof(ms_StdColours[0]);
        int nIndex = 0;
        int nNumEntries = nNumColours;
        if (nNumEntries <= 0 || nNumEntries > 256)
            nNumEntries = 256;

        PALETTEINFO PalInfo;                   
        PalInfo.palNumEntries = (WORD) nNumEntries;

        // The standard colours (16)
        for (int i = 0; i < nNumStandardColours; i++)
        {
            if (nIndex >= nNumEntries) 
                break;

            PalInfo.palPalEntry[nIndex].peRed   = ms_StdColours[i].rgbRed;
            PalInfo.palPalEntry[nIndex].peGreen = ms_StdColours[i].rgbGreen;
            PalInfo.palPalEntry[nIndex].peBlue  = ms_StdColours[i].rgbBlue;
            PalInfo.palPalEntry[nIndex].peFlags = 0;
            nIndex++;
        }

        // A colour cube (6 divisions = 216)
        for (int blue = 0; blue <= 5; blue++)
            for (int green = 0; green <= 5; green++)
                for (int red = 0; red <= 5; red++)
                {
                    if (nIndex >= nNumEntries)
                        break;

                    PalInfo.palPalEntry[nIndex].peRed   = (BYTE) ((red*255)/5);
                    PalInfo.palPalEntry[nIndex].peGreen = (BYTE) ((green*255)/5);
                    PalInfo.palPalEntry[nIndex].peBlue  = (BYTE) ((blue*255)/5);
                    PalInfo.palPalEntry[nIndex].peFlags = 0;
                    nIndex++;
                }

                // A grey scale (24 divisions)
                for (int grey = 0; grey <= 23; grey++)
                {
                    if (nIndex >= nNumEntries) 
                        break;

                    PalInfo.palPalEntry[nIndex].peRed   = (BYTE) (grey*255/23);
                    PalInfo.palPalEntry[nIndex].peGreen = (BYTE) (grey*255/23);
                    PalInfo.palPalEntry[nIndex].peBlue  = (BYTE) (grey*255/23);
                    PalInfo.palPalEntry[nIndex].peFlags = 0;
                    nIndex++;
                }

                return palette.CreatePalette((LPLOGPALETTE) PalInfo);
    }
#endif // DIBSECTION_NO_PALETTE


    /////////////////////////////////////////////////////////////////////////////
    // CDIBSectionCE

    CDIBSectionCE::CDIBSectionCE()
    {
        m_hBitmap     = NULL;
        m_hOldBitmap  = NULL;
        m_bReuseMemDC = FALSE;

        DeleteObject(); // This will initialise to a known state - ie. empty
    }

    CDIBSectionCE::~CDIBSectionCE()
    {
        DeleteObject();
    }

    // --- In?:
    // --- Out :
    // --- Returns :
    // --- Effect : Resets the object to an empty state, and frees all memory used.
    void CDIBSectionCE::DeleteObject()
    {
        // Unselect the bitmap out of the memory DC before deleting bitmap
        ReleaseMemoryDC(TRUE);

        if (m_hBitmap)
            ::DeleteObject(m_hBitmap);
        m_hBitmap = NULL;
        m_ppvBits = NULL;

#ifndef DIBSECTION_NO_PALETTE
        m_Palette.DeleteObject();
#endif

        memset(&m_DIBinfo, 0, sizeof(m_DIBinfo));

        m_iColorDataType = DIB_RGB_COLORS;
        m_iColorTableSize = 0;
    }

    /////////////////////////////////////////////////////////////////////////////
    // CDIBSectionCE diagnostics

#ifdef _DEBUG
    void CDIBSectionCE::AssertValid() const
    {
        ASSERT(m_hBitmap);

        DIBSECTION ds;
        DWORD dwSize = GetObject( m_hBitmap, sizeof(DIBSECTION), &ds );
        ASSERT(dwSize == sizeof(DIBSECTION));

        ASSERT(0 <= m_iColorTableSize && m_iColorTableSize <= 256);

        CObject::AssertValid();
    }

    //void CDIBSectionCE::Dump(CDumpContext& dc) const
    //{
    //    CObject::Dump(dc);
    //}

#endif //_DEBUG


    /////////////////////////////////////////////////////////////////////////////
    // CDIBSectionCE operations

    // --- In?: pDC - Pointer to a device context
    //           ptDest - point at which the topleft corner of the image is drawn
    // --- Out :
    // --- Returns : TRUE on success
    // --- Effect : Draws the image 1:1 on the device context
    BOOL CDIBSectionCE::Draw(CDC* pDC, CPoint ptDest, BOOL bForceBackground /*=FALSE*/) 
    { 
        if (!m_hBitmap)
            return FALSE;

        CSize size = GetSize();
        CPoint ptOrigin = CPoint(0,0);

        // Create a memory DC compatible with the destination DC
        CDC* pMemDC = GetMemoryDC(pDC, FALSE);
        if (!pMemDC)
            return FALSE;

#ifndef DIBSECTION_NO_PALETTE
        // Select and realize the palette
        CPalette* pOldPalette = NULL;
        if (m_Palette.m_hObject && UsesPalette(pDC))
        {
            pOldPalette = pDC->SelectPalette(&m_Palette, bForceBackground);
            pDC->RealizePalette();
        }
#endif // DIBSECTION_NO_PALETTE

        BOOL bResult = pDC->BitBlt(ptDest.x, ptDest.y, size.cx, size.cy, pMemDC, 
            ptOrigin.x, ptOrigin.y, SRCCOPY);

#ifndef DIBSECTION_NO_PALETTE
        if (pOldPalette)
            pDC->SelectPalette(pOldPalette, FALSE);
#endif // DIBSECTION_NO_PALETTE

        ReleaseMemoryDC();
        return bResult;
    }

    // --- In?: pDC - Pointer to a device context
    //           ptDest - point at which the topleft corner of the image is drawn
    //           size - size to stretch the image
    // --- Out :
    // --- Returns : TRUE on success
    // --- Effect : Stretch draws the image to the desired size on the device context
    BOOL CDIBSectionCE::Stretch(CDC* pDC, CPoint ptDest, CSize size,
        BOOL bForceBackground /*=FALSE*/) 
    { 
        if (!m_hBitmap)
            return FALSE;

        CPoint ptOrigin = CPoint(0,0);
        CSize imagesize = GetSize();

        // Create a memory DC compatible with the destination DC
        CDC* pMemDC = GetMemoryDC(pDC, FALSE);
        if (!pMemDC)
            return FALSE;

#ifndef DIBSECTION_NO_PALETTE
        // Select and realize the palette
        CPalette* pOldPalette = NULL;
        if (m_Palette.m_hObject && UsesPalette(pDC))
        {
            pOldPalette = pDC->SelectPalette(&m_Palette, bForceBackground);
            pDC->RealizePalette();
        }
#endif // DIBSECTION_NO_PALETTE

#ifndef _WIN32_WCE
        pDC->SetStretchBltMode(COLORONCOLOR);
#endif // _WIN32_WCE

        BOOL bResult = pDC->StretchBlt(ptDest.x, ptDest.y, size.cx, size.cy, 
            pMemDC, 
            ptOrigin.x, ptOrigin.y, imagesize.cx, imagesize.cy, 
            SRCCOPY);

#ifndef DIBSECTION_NO_PALETTE
        if (pOldPalette)
            pDC->SelectPalette(pOldPalette, FALSE);
#endif // DIBSECTION_NO_PALETTE

        ReleaseMemoryDC();
        return bResult;
    }

    //////////////////////////////////////////////////////////////////////////////
    // Setting the bitmap...

    // --- In?: nIDResource - resource ID
    // --- Out :
    // --- Returns : Returns TRUE on success, FALSE otherwise
    // --- Effect : Initialises the bitmap from a resource. If failure, then object is
    //              initialised back to an empty bitmap.
    BOOL CDIBSectionCE::SetBitmap(UINT nIDResource)
    {
        return SetBitmap(MAKEINTRESOURCE(nIDResource));
    }

    // --- In?: lpszResourceName - resource name
    // --- Out :
    // --- Returns : Returns TRUE on success, FALSE otherwise
    // --- Effect : Initialises the bitmap from a resource. If failure, then object is
    //              initialised back to an empty bitmap.
    BOOL CDIBSectionCE::SetBitmap(LPCTSTR lpszResourceName)
    {
        HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetResourceHandle(), 
            lpszResourceName,
            IMAGE_BITMAP, 
            0,0, 
#ifdef _WIN32_WCE
            0
#else
            LR_CREATEDIBSECTION
#endif
            );

        if (!hBmp) 
        {
            TRACE0("Unable to LoadImage");
            return FALSE;
        }

        BOOL bResult = SetBitmap(hBmp);
        ::DeleteObject(hBmp);
        return bResult;
    }

    // --- In?: lpBitmapInfo - pointer to a BITMAPINFO structure
    //           lpBits - pointer to image bits
    // --- Out :
    // --- Returns : Returns TRUE on success, FALSE otherwise
    // --- Effect : Initialises the bitmap using the information in lpBitmapInfo to determine
    //              the dimensions and colours, and the then sets the bits from the bits in
    //              lpBits. If failure, then object is initialised back to an empty bitmap.
    BOOL CDIBSectionCE::SetBitmap(LPBITMAPINFO lpBitmapInfo, LPVOID lpBits)
    {
        DeleteObject();

        if (!lpBitmapInfo || !lpBits)
            return FALSE;

        HDC hDC = NULL;
        TRY {
            BITMAPINFOHEADER& bmih = lpBitmapInfo->bmiHeader;

            // Compute the number of colours in the colour table
            m_iColorTableSize = NumColorEntries(bmih.biBitCount);

            DWORD dwBitmapInfoSize = sizeof(BITMAPINFO) + m_iColorTableSize*sizeof(RGBQUAD);

            // Copy over BITMAPINFO contents
            memcpy(&m_DIBinfo, lpBitmapInfo, dwBitmapInfoSize);

            // Should now have all the info we need to create the sucker.
            //TRACE(_T("Width %d, Height %d, Bits/pixel %d, Image Size %d\n"),
            //      bmih.biWidth, bmih.biHeight, bmih.biBitCount, bmih.biSizeImage);

            // Create a DC which will be used to get DIB, then create DIBsection
            hDC = ::GetDC(NULL);
            if (!hDC) 
            {
                TRACE0("Unable to get DC\n");
                AfxThrowResourceException();
            }

            m_hBitmap = CreateDIBSection(hDC, (const BITMAPINFO*) m_DIBinfo,
                m_iColorDataType, &m_ppvBits, NULL, 0);
            ::ReleaseDC(NULL, hDC);
            if (!m_hBitmap)
            {
                TRACE0("CreateDIBSection failed\n");
                AfxThrowResourceException();
            }

            DWORD dwImageSize = m_DIBinfo.bmiHeader.biSizeImage;
            if (dwImageSize == 0)
            {
                int nBytesPerLine = BytesPerLine(lpBitmapInfo->bmiHeader.biWidth, 
                    lpBitmapInfo->bmiHeader.biBitCount);
                dwImageSize = nBytesPerLine * lpBitmapInfo->bmiHeader.biHeight;
            }

#ifndef _WIN32_WCE
            // Flush the GDI batch queue 
            GdiFlush();
#endif

            memcpy(m_ppvBits, lpBits, dwImageSize);

#ifndef DIBSECTION_NO_PALETTE
            if (!CreatePalette())
            {
                TRACE0("Unable to create palette\n");
                AfxThrowResourceException();
            }
#endif // DIBSECTION_NO_PALETTE
        }
        CATCH (CException, e)
        {
            e->Delete();
            _ShowLastError();
            if (hDC) 
                ::ReleaseDC(NULL, hDC);
            DeleteObject();
            return FALSE;
        }
        END_CATCH

            return TRUE;
    }

    // --- In?: hBitmap - handle to image
    //           pPalette - optional palette to use when setting image
    // --- Out :
    // --- Returns : Returns TRUE on success, FALSE otherwise
    // --- Effect : Initialises the bitmap from the HBITMAP supplied. If failure, then
    //              object is initialised back to an empty bitmap.
    BOOL CDIBSectionCE::SetBitmap(HBITMAP hBitmap, CPalette* pPalette /*= NULL*/)
    {
        DeleteObject();

        if (!hBitmap)
            return FALSE;

        // Get dimensions of bitmap
        BITMAP bm;
        if (!::GetObject(hBitmap, sizeof(bm),(LPVOID)&bm))
            return FALSE;
        bm.bmHeight = abs(bm.bmHeight);

        CWindowDC dc(NULL);
#ifndef DIBSECTION_NO_PALETTE
        CPalette* pOldPalette = NULL;
#endif // DIBSECTION_NO_PALETTE

        TRY {
            m_iColorTableSize = NumColorEntries(bm.bmBitsPixel);

            // Initialize the BITMAPINFOHEADER in m_DIBinfo
            BITMAPINFOHEADER& bih = m_DIBinfo.bmiHeader;
            bih.biSize          = sizeof(BITMAPINFOHEADER);
            bih.biWidth         = bm.bmWidth;
            bih.biHeight        = bm.bmHeight;
            bih.biPlanes        = 1;                // Must always be 1 according to docs
            bih.biBitCount      = bm.bmBitsPixel;
            bih.biCompression   = BI_RGB;
            bih.biSizeImage     = BytesPerLine(bm.bmWidth, bm.bmBitsPixel) * bm.bmHeight;
            bih.biXPelsPerMeter = 0;
            bih.biYPelsPerMeter = 0;
            bih.biClrUsed       = 0;
            bih.biClrImportant  = 0;

            GetColorTableEntries(dc.GetSafeHdc(), hBitmap);

#ifndef DIBSECTION_NO_PALETTE
            // If we have a palette supplied, then set the palette (and hance DIB color
            // table) using this palette
            if (pPalette)
                SetPalette(pPalette);

            if (m_Palette.GetSafeHandle())
            {
                pOldPalette = dc.SelectPalette(&m_Palette, FALSE);
                dc.RealizePalette();
            }
#endif // DIBSECTION_NO_PALETTE

            // Create it!
            m_hBitmap = CreateDIBSection(dc.m_hDC, 
                (const BITMAPINFO*) m_DIBinfo,
                m_iColorDataType,
                &m_ppvBits, 
                NULL, 0);
#ifndef DIBSECTION_NO_PALETTE
            if (pOldPalette)
                dc.SelectPalette(pOldPalette, FALSE);
            pOldPalette = NULL; 
#endif // DIBSECTION_NO_PALETTE

            if (!m_hBitmap)
            {
                TRACE0("Unable to CreateDIBSection\n");
                AfxThrowResourceException();
            }

#ifndef DIBSECTION_NO_PALETTE
            // If palette was supplied then create a palette using the entries in the DIB
            // color table.
            if (!pPalette)
                CreatePalette();
#endif // DIBSECTION_NO_PALETTE

            // Need to copy the supplied bitmap onto the newly created DIBsection
            CDC memDC, CopyDC;
            if (!CopyDC.CreateCompatibleDC(&dc) || !memDC.CreateCompatibleDC(&dc)) 
            {
                TRACE0("Unable to create compatible DC's\n");
                AfxThrowResourceException();
            }

#ifndef DIBSECTION_NO_PALETTE
            if (m_Palette.GetSafeHandle())
            {
                memDC.SelectPalette(&m_Palette, FALSE);  memDC.RealizePalette();
                CopyDC.SelectPalette(&m_Palette, FALSE); CopyDC.RealizePalette();
            }
#endif // DIBSECTION_NO_PALETTE

#ifndef _WIN32_WCE
            // Flush the GDI batch queue 
            GdiFlush();
#endif

            HBITMAP hOldMemBitmap  = (HBITMAP) SelectObject(memDC.m_hDC,  hBitmap);
            HBITMAP hOldCopyBitmap = (HBITMAP) SelectObject(CopyDC.m_hDC, m_hBitmap);

            CopyDC.BitBlt(0,0, bm.bmWidth, bm.bmHeight, &memDC, 0,0, SRCCOPY);

            SelectObject(memDC.m_hDC, hOldMemBitmap);
            SelectObject(CopyDC.m_hDC, hOldCopyBitmap);

#ifndef DIBSECTION_NO_PALETTE
            if (m_Palette.GetSafeHandle())
            {
                memDC.SelectStockObject(DEFAULT_PALETTE);
                CopyDC.SelectStockObject(DEFAULT_PALETTE);
            }
#endif // DIBSECTION_NO_PALETTE
        }
        CATCH (CException, e)
        {
            e->Delete();
            _ShowLastError();
#ifndef DIBSECTION_NO_PALETTE
            if (pOldPalette)
                dc.SelectPalette(pOldPalette, FALSE);
#endif // DIBSECTION_NO_PALETTE
            DeleteObject();
            return FALSE;
        }
        END_CATCH

            return TRUE;
    }


    //////////////////////////////////////////////////////////////////////////////
    // Persistance...

    // --- In?: lpszFileName - image filename
    // --- Out :
    // --- Returns : Returns TRUE on success, FALSE otherwise
    // --- Effect : Loads the bitmap from a bitmap file with the name lpszFileName. 
    //              If failure, then object is initialised back to an empty bitmap.
    BOOL CDIBSectionCE::Load(LPCTSTR lpszFileName)
    {
        CFile file;
        if (!file.Open(lpszFileName, CFile::modeRead))
            return FALSE;

        // Get the current file position.  
        DWORD dwFileStart = file.GetPosition();

        // The first part of the file contains the file header.
        // This will tell us if it is a bitmap, how big the header is, and how big 
        // the file is. The header size in the file header includes the color table.
        BITMAPFILEHEADER BmpFileHdr;
        int nBytes;
        nBytes = file.Read(&BmpFileHdr, sizeof(BmpFileHdr));
        if (nBytes != sizeof(BmpFileHdr)) 
        {
            TRACE0("Failed to read file header\n");
            return FALSE;
        }

        // Check that we have the magic 'BM' at the start.
        if (BmpFileHdr.bfType != DS_BITMAP_FILEMARKER)
        {
            TRACE0("Not a bitmap file\n");
            return FALSE;
        }

        // Read the header (assuming it's a DIB). 
        DIBINFO	BmpInfo;
        nBytes = file.Read(&BmpInfo, sizeof(BITMAPINFOHEADER)); 
        if (nBytes != sizeof(BITMAPINFOHEADER)) 
        {
            TRACE0("Failed to read BITMAPINFOHEADER\n");
            return FALSE;
        }

        // Check that we have a real Windows DIB file.
        if (BmpInfo.bmiHeader.biSize != sizeof(BITMAPINFOHEADER))
        {
            TRACE0(" File is not a Windows DIB\n");
            return FALSE;
        }

        // See how big the color table is in the file (if there is one).  
        int nColors = NumColorEntries(BmpInfo.bmiHeader.biBitCount);
        if (nColors > 0) 
        {
            // Read the color table from the file.
            int nColorTableSize = nColors * sizeof(RGBQUAD);
            nBytes = file.Read(BmpInfo.ColorTable(), nColorTableSize);
            if (nBytes != nColorTableSize) 
            {
                TRACE0("Failed to read color table\n");
                return FALSE;
            }
        }

        // So how big the bitmap surface is.
        int nBitsSize = BmpFileHdr.bfSize - BmpFileHdr.bfOffBits;

        // Allocate the memory for the bits and read the bits from the file.
        BYTE* pBits = (BYTE*) malloc(nBitsSize);
        if (!pBits) 
        {
            TRACE0("Out of memory for DIB bits\n");
            return FALSE;
        }

        // Seek to the bits in the file.
        file.Seek(dwFileStart + BmpFileHdr.bfOffBits, CFile::begin);

        // read the bits
        nBytes = file.Read(pBits, nBitsSize);
        if (nBytes != nBitsSize) 
        {
            TRACE0("Failed to read bits\n");
            free(pBits);
            return FALSE;
        }

        // Everything went OK.
        BmpInfo.bmiHeader.biSizeImage = nBitsSize;

        if (!SetBitmap((LPBITMAPINFO) BmpInfo, pBits))
        {
            TRACE0("Failed to set bitmap info\n");
            free(pBits);
            return FALSE;
        }

        free(pBits);

        return TRUE;
    }

    // --- In?: lpszFileName - image filename
    // --- Out :
    // --- Returns : Returns TRUE on success, FALSE otherwise
    // --- Effect : Saves the image to file.
    BOOL CDIBSectionCE::Save(LPCTSTR lpszFileName)
    {
        BITMAPFILEHEADER   hdr;
        LPBITMAPINFOHEADER lpbi = GetBitmapInfoHeader();

        if (!lpbi || !lpszFileName)
            return FALSE;

        CFile file;
        if (!file.Open(lpszFileName, CFile::modeWrite|CFile::modeCreate))
            return FALSE;

        DWORD dwBitmapInfoSize = sizeof(BITMAPINFO) + m_iColorTableSize*sizeof(RGBQUAD);
        DWORD dwFileHeaderSize = dwBitmapInfoSize + sizeof(hdr);

        // Fill in the fields of the file header 
        hdr.bfType       = DS_BITMAP_FILEMARKER;
        hdr.bfSize       = dwFileHeaderSize + lpbi->biSizeImage;
        hdr.bfReserved1  = 0;
        hdr.bfReserved2  = 0;
        hdr.bfOffBits    = dwFileHeaderSize;

        // Write the file header 
        file.Write(&hdr, sizeof(hdr));

        // Write the DIB header
        file.Write(lpbi, dwBitmapInfoSize);

        // Write DIB bits
        file.Write(GetDIBits(), lpbi->biSizeImage);

        return TRUE;
    }

    /////////////////////////////////////////////////////////////////////////////
    // CDIBSectionCE palette stuff

#ifndef DIBSECTION_NO_PALETTE

    // --- In?:
    // --- Out :
    // --- Returns : TRUE on success
    // --- Effect : Creates the palette from the DIBSection's color table. Assumes 
    //              m_iColorTableSize has been set and the DIBsection m_hBitmap created
    BOOL CDIBSectionCE::CreatePalette()
    {
//        m_Palette.DeleteObject();
//
//        if (!m_hBitmap)
//            return FALSE;
//
//        // Create a 256 color halftone palette if there is no color table in the DIBSection
//        if (m_iColorTableSize == 0)
//            return CreateHalftonePalette(m_Palette, 256);
//
//        // Get space for the colour entries
//        RGBQUAD *pRGB = new RGBQUAD[m_iColorTableSize];
//        if (!pRGB)
//            return CreateHalftonePalette(m_Palette, m_iColorTableSize);
//
//        HDC hDC = ::GetDC(NULL);
//        if (!hDC)
//        {
//            delete [] pRGB;
//            return FALSE;
//        }
//
//        // Create a memory DC compatible with the current DC
//        CDC MemDC;
//        MemDC.CreateCompatibleDC(CDC::FromHandle(hDC));
//        if (!MemDC.GetSafeHdc())
//        {
//            delete [] pRGB;
//            ::ReleaseDC(NULL, hDC);
//            return CreateHalftonePalette(m_Palette, m_iColorTableSize);
//        }
//        ::ReleaseDC(NULL, hDC);
//
//        HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(MemDC.GetSafeHdc(), m_hBitmap);
//        if (!hOldBitmap)
//        {
//            delete [] pRGB;
//            return CreateHalftonePalette(m_Palette, m_iColorTableSize);
//        }
//
//        // Get the colours used. WinCE does not support GetDIBColorTable so if you
//        // are using this on a CE device with palettes, then you need to replace
//        // the call with code that manually gets the color table from the m_DIBinfo structure.
//#ifdef _WIN32_WCE
//        int nColours = ::CEGetDIBColorTable(MemDC.GetSafeHdc(), 0, m_iColorTableSize, pRGB);
//#else
//        int nColours = ::GetDIBColorTable(MemDC.GetSafeHdc(), 0, m_iColorTableSize, pRGB);
//#endif
//
//        // Clean up
//        ::SelectObject(MemDC.GetSafeHdc(), hOldBitmap);
//
//        if (!nColours)   // No colours retrieved => the bitmap in the DC is not a DIB section
//        {
//            delete [] pRGB;
//            return CreateHalftonePalette(m_Palette, m_iColorTableSize);
//        }   
//
//        // Create and fill a LOGPALETTE structure with the colours used.
//        PALETTEINFO PaletteInfo;
//        PaletteInfo.palNumEntries = (WORD) m_iColorTableSize;
//
//        int i = 0;
//        for (; i < nColours; i++)
//        {
//            PaletteInfo.palPalEntry[i].peRed   = pRGB[i].rgbRed;
//            PaletteInfo.palPalEntry[i].peGreen = pRGB[i].rgbGreen;
//            PaletteInfo.palPalEntry[i].peBlue  = pRGB[i].rgbBlue;
//            PaletteInfo.palPalEntry[i].peFlags = 0;
//        }
//        for (; i < (int) m_iColorTableSize; i++)
//        {
//            PaletteInfo.palPalEntry[i].peRed   = 0;
//            PaletteInfo.palPalEntry[i].peGreen = 0;
//            PaletteInfo.palPalEntry[i].peBlue  = 0;
//            PaletteInfo.palPalEntry[i].peFlags = 0;
//        }
//
//        delete [] pRGB;
//
//        // Create Palette!
//        return m_Palette.CreatePalette(&PaletteInfo);
        return 0;
    }

    // --- In?: pPalette - new palette to use
    // --- Out :
    // --- Returns : TRUE on success
    // --- Effect : Sets the current palette used by the image from the supplied CPalette,
    //              and sets the color table in the DIBSection
    BOOL CDIBSectionCE::SetPalette(CPalette* pPalette)
    {
        m_Palette.DeleteObject();

        if (!pPalette)
            return FALSE;

        UINT nColours = pPalette->GetEntryCount();
        if (nColours <= 0 || nColours > 256)
            return FALSE;

        // Get palette entries
        PALETTEINFO pi;
        pi.palNumEntries = (WORD) pPalette->GetPaletteEntries(0, nColours, (LPPALETTEENTRY) pi);

        // TODO: If pi.palNumEntries < m_iColorTableSize, then fill in blanks with 0's

        return SetLogPalette(&pi);
    }

    // --- In?: pLogPalette - new palette to use
    // --- Out :
    // --- Returns : TRUE on success
    // --- Effect : Sets the current palette used by the image from the supplied LOGPALETTE
    BOOL CDIBSectionCE::SetLogPalette(LOGPALETTE* pLogPalette)
    {
        ASSERT(pLogPalette->palVersion == (WORD) 0x300);
        UINT nColours = pLogPalette->palNumEntries;
        if (nColours <= 0 || nColours > 256)
        {
            CreatePalette();
            return FALSE;
        }

        // Create new palette
        m_Palette.DeleteObject();
        if (!m_Palette.CreatePalette(pLogPalette))
        {
            CreatePalette();
            return FALSE;
        }

        if (m_iColorTableSize == 0)
            return TRUE;

        // Set the DIB colours
        RGBQUAD RGBquads[256]; 
        for (UINT i = 0; i < nColours; i++)
        {
            RGBquads[i].rgbRed   = pLogPalette->palPalEntry[i].peRed;
            RGBquads[i].rgbGreen = pLogPalette->palPalEntry[i].peGreen;
            RGBquads[i].rgbBlue  = pLogPalette->palPalEntry[i].peBlue;
            RGBquads[i].rgbReserved = 0;
        }

        return FillDIBColorTable(nColours, RGBquads);
    }

    // --- In?: nNumColours - number of colours to set
    //           pRGB - colours to fill
    // --- Out :
    // --- Returns : Returns TRUE on success
    // --- Effect : Sets the colours used by the image. Only works if # colours <= 256
    BOOL CDIBSectionCE::FillDIBColorTable(UINT nNumColours, RGBQUAD *pRGB)
    {
        if (!pRGB || !nNumColours || !m_iColorTableSize || nNumColours > 256)
            return FALSE;

        // get the number of colors to return per BITMAPINFOHEADER docs
        UINT nColors;
        LPBITMAPINFOHEADER pBmih = GetBitmapInfoHeader();
        if (pBmih->biClrUsed)
            nColors = pBmih->biClrUsed;
        else
            nColors = 1 << (pBmih->biBitCount*pBmih->biPlanes);

        // Initialize the loop variables
        nColors = min(nNumColours, nColors);

        LPRGBQUAD pColorTable = GetColorTable();
        for (UINT iColor = 0; iColor < nColors; iColor++)
        {
            pColorTable[iColor].rgbReserved = 0;
            pColorTable[iColor].rgbBlue     = pRGB[iColor].rgbBlue;
            pColorTable[iColor].rgbRed      = pRGB[iColor].rgbRed;
            pColorTable[iColor].rgbGreen    = pRGB[iColor].rgbGreen;
        }

        return TRUE;
    }

#endif // DIBSECTION_NO_PALETTE


    // --- In?: hdc     - the Device Context in which the DIBSection is selected
    //           hBitmap - the bitmap whose solor entries are to be queried
    //           lpbi    - a pointer to a BITMAPINFO structure that will have it's
    //                     color table filled.
    // --- Out :
    // --- Returns : the number of colors placed in the color table
    // --- Effect : This function is a replacement for GetDIBits, in that it retrieves 
    //              (or synthesizes) the color table from the given bitmap, and stores 
    //              the values in the BITMAPINFO structure supplied.
    UINT CDIBSectionCE::GetColorTableEntries(HDC hdc, HBITMAP hBitmap)
    {
        UINT nColorTableSize = m_iColorTableSize;
        if (!nColorTableSize)
            return 0;

        // Fill the color table with the colours from the bitmap's color table
        LPRGBQUAD pColorTable = GetColorTable();

        // Get the color table from the HBITMAP and copy them over.
        UINT nCount;
        RGBQUAD* pRGB = new RGBQUAD[nColorTableSize];
        if (pRGB)
        {
            HBITMAP hOldBitmap = (HBITMAP) SelectObject(hdc, hBitmap);
            nCount = CEGetDIBColorTable(hdc, 0, nColorTableSize, pRGB);
            SelectObject(hdc, hOldBitmap);
            if (nCount)
            {
                nColorTableSize = nCount;
                memcpy(pColorTable, pRGB, nCount*sizeof(RGBQUAD));
            }
        }
        delete [] pRGB;

        // Didn't work - so synthesize one.
        if (!nCount)
        {       
            int nNumStandardColours = sizeof(ms_StdColours) / sizeof(ms_StdColours[0]);
            UINT nIndex = 0;

            // The standard colours (16)
            int i = 0;
            for (; i < nNumStandardColours; i++)
            {
                if (nIndex >= nColorTableSize) 
                    break;

                memcpy( &(pColorTable[i]), &(ms_StdColours[i]), sizeof(RGBQUAD) );         

                nIndex++;
            }

            // A colour cube (6 divisions = 216)
            for (int blue = 0; blue <= 5; blue++)
                for (int green = 0; green <= 5; green++)
                    for (int red = 0; red <= 5; red++)
                    {
                        if (nIndex >= nColorTableSize)
                            break;

                        pColorTable[i].rgbRed   = (BYTE) ((red*255)/5);
                        pColorTable[i].rgbGreen = (BYTE) ((green*255)/5);
                        pColorTable[i].rgbBlue  = (BYTE) ((blue*255)/5);
                        pColorTable[i].rgbReserved = 0;
                        nIndex++;
                    }

                    // A grey scale (24 divisions)
                    for (int grey = 0; grey <= 23; grey++)
                    {
                        if (nIndex >= nColorTableSize) 
                            break;

                        pColorTable[i].rgbRed   = (BYTE) (grey*255/23);
                        pColorTable[i].rgbGreen = (BYTE) (grey*255/23);
                        pColorTable[i].rgbBlue  = (BYTE) (grey*255/23);
                        pColorTable[i].rgbReserved = 0;
                        nIndex++;
                    }
        }

        return nColorTableSize;
    }

    /**********************************************************************
    This function is from the MS KB article "HOWTO: Get the Color Table of 
    a DIBSection in Windows CE".

    PARAMETERS:
    HDC - the Device Context in which the DIBSection is selected
    UINT - the index of the first color table entry to retrieve
    UINT - the number of color table entries to retrieve
    RGBQUAD - a buffer large enough to hold the number of RGBQUAD
    entries requested

    RETURNS:
    UINT - the number of colors placed in the buffer

    ***********************************************************************/
    UINT CEGetDIBColorTable(HDC hdc, UINT uStartIndex, UINT cEntries,
        RGBQUAD *pColors)
    {   
        if (pColors == NULL)
            return 0;                       // No place to put them, fail

        // Get a description of the DIB Section
        HBITMAP hDIBSection = (HBITMAP) GetCurrentObject( hdc, OBJ_BITMAP );

        DIBSECTION ds;
        DWORD dwSize = GetObject( hDIBSection, sizeof(DIBSECTION), &ds );

        if (dwSize != sizeof(DIBSECTION))
            return 0;                      // Must not be a DIBSection, fail

        if (ds.dsBmih.biBitCount > 8)
            return 0;                      // Not Palettized, fail

        // get the number of colors to return per BITMAPINFOHEADER docs
        UINT cColors;
        if (ds.dsBmih.biClrUsed)
            cColors = ds.dsBmih.biClrUsed;
        else
            cColors = 1 << (ds.dsBmih.biBitCount*ds.dsBmih.biPlanes);

        // Create a mask for the palette index bits for 1, 2, 4, and 8 bpp
        WORD wIndexMask = (0xFF << (8 - ds.dsBmih.biBitCount)) & 0x00FF;

        // Get the pointer to the image bits
        LPBYTE pBits = (LPBYTE) ds.dsBm.bmBits;

        // Initialize the loop variables
        cColors = min( cColors, cEntries );
        BYTE OldPalIndex = *pBits;

        UINT TestPixelY;
        if (ds.dsBmih.biHeight > 0 )
            // If button up DIB, pBits points to last row
            TestPixelY = ds.dsBm.bmHeight-1;
        else
            // If top down DIB, pBits points to first row
            TestPixelY = 0;

        for (UINT iColor = uStartIndex; iColor < cColors; iColor++)
        {
            COLORREF    rgbColor;

            // Set the palette index for the test pixel,
            // modifying only the bits for one pixel
            *pBits = (iColor << (8 - ds.dsBmih.biBitCount)) |
                (*pBits & ~wIndexMask);

            // now get the resulting color
            rgbColor = GetPixel( hdc, 0, TestPixelY );

            pColors[iColor - uStartIndex].rgbReserved = 0;
            pColors[iColor - uStartIndex].rgbBlue = GetBValue(rgbColor);
            pColors[iColor - uStartIndex].rgbRed = GetRValue(rgbColor);
            pColors[iColor - uStartIndex].rgbGreen = GetGValue(rgbColor);
        }

        // Restore the test pixel
        *pBits = OldPalIndex;

        return cColors;
    }


    // --- In?: pDC - device context to use when calling CreateCompatibleDC
    //           bSelectPalette - if TRUE, the current palette will be preselected
    // --- Out :
    // --- Returns : A pointer to a memory DC
    // --- Effect : Creates a memory DC and selects in the current bitmap so it can be
    //              modified using the GDI functions. Only one memDC can be created for
    //              a given CDIBSectionCE object. If you have a memDC but wish to recreate it
    //              as compatible with a different DC, then call ReleaseMemoryDC first.
    //              If the memory DC has already been created then it will be recycled.
    //              Note that if using this in an environment where the colour depth of
    //              the screen may change, then you will need to set "m_bReuseMemDC" to FALSE
    CDC* CDIBSectionCE::GetMemoryDC(CDC* pDC /*=NULL*/, BOOL bSelectPalette /*=TRUE*/)
    {
        if (!m_bReuseMemDC)
            ReleaseMemoryDC(TRUE);
        else if (m_MemDC.GetSafeHdc())   // Already created?
        {
#ifndef _WIN32_WCE
            // Flush the GDI batch queue 
            GdiFlush();
#endif
            return &m_MemDC;
        }

        // Create a memory DC compatible with the given DC
        if (pDC)
            m_MemDC.CreateCompatibleDC(pDC);
        else
        {
            HDC hDC = ::GetDC(NULL);    // screen DC
            if (!hDC) return FALSE;
            m_MemDC.CreateCompatibleDC(CDC::FromHandle(hDC));
            ::ReleaseDC(NULL, hDC);
        }
        if (!m_MemDC.GetSafeHdc())
            return NULL;

        // Select in the bitmap
        m_hOldBitmap = (HBITMAP) ::SelectObject(m_MemDC.GetSafeHdc(), m_hBitmap);
        if (!m_hOldBitmap)
        {
            m_MemDC.DeleteDC();
            return NULL;
        }

#ifndef DIBSECTION_NO_PALETTE
        // Select in the palette
        if (bSelectPalette && UsesPalette(&m_MemDC))
        {
            // Palette should already have been created - but just in case...
            if (!m_Palette.GetSafeHandle())
                CreatePalette();

            m_pOldPalette = m_MemDC.SelectPalette(&m_Palette, FALSE);
            m_MemDC.RealizePalette();
        }
        else
            m_pOldPalette = NULL;
#endif // DIBSECTION_NO_PALETTE

#ifndef _WIN32_WCE
        // Flush the GDI batch queue 
        GdiFlush();
#endif // _WIN32_WCE

        return &m_MemDC;
    }

    // --- In?: bForceRelease - if TRUE, then the memory DC is forcibly released
    // --- Out :
    // --- Returns : TRUE on success
    // --- Effect : Selects out the current bitmap and deletes the mem dc. If bForceRelease 
    //              is FALSE, then the DC release will not actually occur. This is provided 
    //              so you can have
    //
    //                 GetMemoryDC(...)
    //                 ... do something
    //                 ReleaseMemoryDC()
    //
    //               bracketed calls. If m_bReuseMemDC is subsequently set to FALSE, then 
    //               the same code fragment will still work.
    BOOL CDIBSectionCE::ReleaseMemoryDC(BOOL bForceRelease /*=FALSE*/)
    {
        if ( !m_MemDC.GetSafeHdc() || (m_bReuseMemDC && !bForceRelease) )
            return TRUE; // Nothing to do

#ifndef _WIN32_WCE
        // Flush the GDI batch queue 
        GdiFlush();
#endif // _WIN32_WCE

        // Select out the current bitmap
        if (m_hOldBitmap)
            ::SelectObject(m_MemDC.GetSafeHdc(), m_hOldBitmap);
        m_hOldBitmap = NULL;

#ifndef DIBSECTION_NO_PALETTE
        // Select out the current palette
        if (m_pOldPalette)
            m_MemDC.SelectPalette(m_pOldPalette, FALSE);
        m_pOldPalette = NULL;
#endif // DIBSECTION_NO_PALETTE

        // Delete the memory DC
        return m_MemDC.DeleteDC();
    }


#ifdef _DEBUG
    // Makes trace windows a little bit more informative...
    void CDIBSectionCE::_ShowLastError()
    {
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,    
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
            (LPTSTR) &lpMsgBuf, 0, NULL);
        TRACE1("Last error: %s\n", lpMsgBuf);
        LocalFree(lpMsgBuf);
    }
#else
    void CDIBSectionCE::_ShowLastError() {}
#endif
}
