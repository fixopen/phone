// MJPGStatic.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "MJPGStatic.h"
#include "CEBtnST.h"
#include "CEComboBox_.h"
#include "CERadioButton.h"
#include "CELineEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void DrawImage(LPCTSTR szFileImage, CDC* pdc, CRect rect, BOOL isOffset = FALSE);

namespace Control {
#define LCD_WIDTH	480
#define LCD_HEIGHT  214
    /////////////////////////////////////////////////////////////////////////////
    // CMJPGStatic
    //globle data
    CString gLangName[] = {
        _T(".\\adv\\mjpg\\中文\\playcfg.xml"),
        _T(".\\adv\\mjpg\\英文\\playcfg.xml"),
        _T(".\\adv\\mjpg\\日文\\playcfg.xml"),
        _T(".\\adv\\mjpg\\韩文\\playcfg.xml")
    };
    CString gAdvPlayList = _T(".\\adv\\video\\advplaylist.xml");
    CString gFontName[] = {
        _T("宋体"),
        _T("黑体"),
        _T("Arial"),
        _T("Times New Roman")
    };
    UINT gFontWeights[] = {
        FW_NORMAL,
        FW_BOLD
    };
    UINT gFontSize[] = {
        10,
        12,
        14,
        16,
        18,
        20,
        22,
        24,
        26,
        28,
        30
    };
    UINT gFontHeight[] = {
        1,
        2,
        4,
        6,
        8,
        10,
        12,
        14,
        16,
        18,
        20,
        22,
        24,
        26,
        28,
        30
    };
    COLORREF gFontColor[] = {
        RGB(0, 0, 0),
        RGB(255, 255, 255),
        RGB(255, 0, 0),
        RGB(0, 0, 255),
        RGB(0, 100, 0),
        RGB(255, 255, 0)
    };
    UINT gFontAlign[] = {
        DT_WORDBREAK|DT_NOPREFIX|DT_EDITCONTROL,
        DT_WORDBREAK|DT_NOPREFIX|DT_EDITCONTROL,
        DT_WORDBREAK|DT_NOPREFIX|DT_EDITCONTROL
    };

    CMJPGStatic::CMJPGStatic()
    {
        m_currentMJPGList = NULL;
        m_nIndexSelectUnit = -1;
    }

    CMJPGStatic::~CMJPGStatic()
    {
    }

    void CMJPGStatic::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
    {
        if(nChar == 'U' || nChar == 'D')
        {
            CWnd *pWnd = GetParent();
            if(pWnd)
                pWnd->SendMessage(WM_KEYDOWN, nChar, 0);
        }
        CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
    }

    BEGIN_MESSAGE_MAP(CMJPGStatic, CStatic)
        //{{AFX_MSG_MAP(CMJPGStatic)
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_PAINT()
        ON_WM_KEYDOWN()
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    BOOL IsBiaodianFuhao(TCHAR c)
    {
        if(c == L',' || c == L'，'\
            || c == L'.' || c == L'。'\
            || c == L'!' || c == L'！'\
            || c == L':' || c == L'：'\
            || c == L';' || c == L'；'\
            || c == L'.' || c == L'。'\
            || c == L'"' || c == L'“'\
            || c == L'”'\
            || c == L'\'' || c == L'‘'\
            || c == L'’'\
            || c == L'、'\
            || c == L'\r')
            return TRUE;
        return FALSE;
    }

#define MULITI_RECT_OFFSET 3
    int CMJPGStatic::GetCStringMuliteLine(CString s, int height, CRect rt, MULTILINEINFO *pCount, int LineCount)
    {
        if (rt.Width() == 0 || (rt.Width() - MULITI_RECT_OFFSET * 2) < height)
            return 0;
        TEXTMETRIC tm;
        int nCount = (rt.Width() - MULITI_RECT_OFFSET * 2) / (height / 2);
        if (nCount <= 0)
            return 0;
        int nLineCount = 0; 
        int nLength = s.GetLength();
        int n = 0;
        int nChar = 0;
        for (int i = 0; i < nLength; ++i)
        {
            if (n == 0)
            {
                if (nLineCount >= (LineCount - 1))
                    return nLineCount;
                pCount[nLineCount].nStart = i;
                nChar = 0;
            }

            CString t = s.Mid(i, 1);
            if (t == L"\r")
            {
                if (n == 0)			//处理连续回车
                {
                    pCount[nLineCount].nCount = n;
                    n = 0;
                    ++nLineCount;
                    continue;
                }
                pCount[nLineCount].nCount = n;
                n = 0;
                ++nLineCount;
            }
            else if (t == L"\n")
            {
                if (n == 0)
                    continue;
                pCount[nLineCount].nCount = n;
                n = 0;
                ++nLineCount;
            }
            else if (i == nLength-1)
            {
                pCount[nLineCount].nCount = n + 1;
                n = 0;
                ++nLineCount;
            }
            else 
            {
                TCHAR c = s.GetAt(i);
                if (c & 0xFF00)  //双字节
                {
                    nChar += 2;
                    if (nChar > nCount)						//多一个字节换行
                    {
                        if (!IsBiaodianFuhao(c))			   //符号显示在上一行上
                        {
                            --i;							//i需要退一个字符
                        }
                        else
                        {
                            ++i;
                            ++n;
                            if (IsBiaodianFuhao(s.GetAt(i)))
                            {
                                ++n;
                                ++i;
                            }
                            --i;   //正好符号换行多加了一个字符   lxz 20081105
                        }
                        pCount[nLineCount].nCount = n;     //自动折行了，不能加1;
                        n = 0;
                        ++nLineCount;
                    }
                    else if (nChar == nCount)				//正好换行
                    {
                        if (IsBiaodianFuhao(s.GetAt(i + 1)))			//符号显示在上一行上
                        {
                            ++n;
                            ++i;
                            if (IsBiaodianFuhao(s.GetAt(i)))
                            {
                                ++n;
                                ++i;
                                if (IsBiaodianFuhao(s.GetAt(i)))
                                {
                                    ++n;
                                    ++i;
                                }
                            }
                        }

                        pCount[nLineCount].nCount = n + 1;   
                        n = 0;
                        ++nLineCount;
                    }
                    else
                    {
                        ++n;
                    }
                }
                else	//单字节
                {
                    nChar += 1;
                    if (nChar == nCount)
                    {
                        if (IsBiaodianFuhao(s.GetAt(i + 1)))			//符号显示在上一行上
                        {
                            ++n;
                            ++i;
                        }

                        pCount[nLineCount].nCount = n + 1;
                        n = 0;
                        ++nLineCount;
                    }
                    else
                    {
                        ++n;
                    }
                }
            }
        }
        return nLineCount;
    }

    void TrimRinghtEnterChar(CString &s)
    {
        s.TrimRight(L"\r\n");
    }


    CString AdjustEnglishWordShow(CString s,  MULTILINEINFO *pVarry, int i, int nCount)
    {
        CString s1 = s.Mid(pVarry[i].nStart, pVarry[i].nCount);
        if (i >= (nCount - 1))
        {
            return s1;
        }
        else
        {
            int index = pVarry[i].nStart + pVarry[i].nCount - 1;
            TCHAR temp = s.GetAt(index);
            TCHAR temp1 = s.GetAt(index+1);
            if (((temp >= L'a' && temp <= 'z') || (temp >= L'A' && temp <= 'Z')) && ((temp1 >= L'a' && temp1 <= 'z') || (temp1 >= L'A' && temp1 <= 'Z')))
            {
                s1 += "-";
            }
        }
        return s1;
    }

    void CMJPGStatic::DrawMultiLine(CDC *pdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1)
    {
        int nPixel1 = nPixel;

        int nRectLine = rt.Height() / nPixel1;
        if(nRectLine == 0)
            nRectLine = 1;
        int nTxtLine = LineCount;
        int nShowLine = nTxtLine;
        BOOL bIsExtentRect = FALSE;
        if (nTxtLine > nRectLine)
        {
            bIsExtentRect = TRUE;
            nShowLine = nRectLine;
        }

        CRect rt_ = rt;
        rt_.DeflateRect(MULITI_RECT_OFFSET, 0);
        switch(nAglin1)
        {
        case left_top:
            {
                rt.right += 8;
                for (int i = 0; i < nShowLine; ++i)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1, rt_.right, rt_.top+(i+1)*nPixel1);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
                }
                break;
            }
        case left_center:
            {
                rt.right += 8;
                int offset = (rt_.Height() - nShowLine * nPixel1) / 2;
                for (int i = 0; i < nShowLine; ++i)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    s1.TrimRight(L"\r\n");
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1+offset, rt_.right, rt_.top+(i+1)*nPixel1+offset);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
                }
                break;
            }
        case left_bottom:
            {
                rt.right += 8;
                int offset = rt_.Height() - nShowLine * nPixel1;
                for (int i = 0; i < nShowLine; ++i)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1+offset, rt_.right, rt_.top+(i+1)*nPixel1+offset);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
                }
                break;
            }
        case middle_top:
            {
                for (int i = 0; i < nShowLine; ++i)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    s1.TrimRight(L"\r\n");
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1, rt_.right, rt_.top+(i+1)*nPixel1);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
                }
                break;
            }
        case middle_center:
            {
                int offset = (rt_.Height() - nShowLine * nPixel1) / 2;
                for (int i = 0; i < nShowLine; ++i)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    s1.TrimRight(L"\r\n");
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1+offset, rt_.right, rt_.top+(i+1)*nPixel1+offset);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
                }
                break;
            }
        case middle_bottom:
            {
                int offset = rt_.Height()-nShowLine*nPixel1;
                for(int i = 0; i < nShowLine; i++)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    s1.TrimRight(L"\r\n");
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1+offset, rt_.right, rt_.top+(i+1)*nPixel1+offset);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
                }
                break;
            }
        case right_top:
            {
                for(int i = 0; i < nShowLine; i++)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1, rt_.right, rt_.top+(i+1)*nPixel1);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
                }
                break;
            }
        case right_center:
            {
                int offset = (rt_.Height()-nShowLine*nPixel1)/2;
                for(int i = 0; i < nShowLine; i++)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    s1.TrimRight(L"\r\n");
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1+offset, rt_.right, rt_.top+(i+1)*nPixel1+offset);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
                }
                break;
            }
        case right_bottom:
            {
                int offset = rt_.Height()-nShowLine*nPixel1;
                for(int i = 0; i < nShowLine; i++)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1+offset, rt_.right, rt_.top+(i+1)*nPixel1+offset);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
                }
                break;
            }
        default:
            {
                rt.right += 8;
                for(int i = 0; i < nShowLine; i++)
                {
                    CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
                    CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1, rt_.right, rt_.top+(i+1)*nPixel1);
                    pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
                }
                break;
            }

        }
    }

    extern void TransparentBlt2(
        HDC hdcDest,      // 目标DC
        int nXOriginDest,   // 目标X偏移
        int nYOriginDest,   // 目标Y偏移
        int nWidthDest,     // 目标宽度
        int nHeightDest,    // 目标高度
        HDC hdcSrc,         // 源DC
        int nXOriginSrc,    // 源X起点
        int nYOriginSrc,    // 源Y起点
        int nWidthSrc,      // 源宽度
        int nHeightSrc,     // 源高度
        UINT crTransparent  // 透明色,COLORREF类型
        );

    void CMJPGStatic::DrawUnit(UNIT *pUnit, CDC *pdc)
    {
        if (!pUnit)
            return;

        if (!pUnit->m_bIsShow)
            return;
        if (pUnit->m_pUnitWnd)
            return;

        pdc->SetBkMode(TRANSPARENT);

        //贴背景图
        if(!pUnit->m_bIsDownStatus)
        {
            if(pUnit->m_bgFilename != "")
            {
                if(!pUnit->m_bIsTranslate)
                    ::DrawImage(ToFileDir(pUnit->m_bgFilename), pdc, pUnit->m_Rect);
                else
                {
                    CDC *pMemDC = new CDC();
                    CBitmap *pBmp = new CBitmap(); 
                    //	CDC *pdc_ = GetDC();
                    pMemDC->CreateCompatibleDC(pdc); 
                    pBmp->CreateCompatibleBitmap(pMemDC, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
                    CBitmap *pOldbmp = pMemDC->SelectObject(pBmp);
                    CRect rt1 = CRect(0, 0, pUnit->m_Rect.Width()-1, pUnit->m_Rect.Height()-1);
                    ::DrawImage(ToFileDir(pUnit->m_bgFilename), pMemDC, rt1);

                    TransparentBlt2(pdc->m_hDC, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), pMemDC->m_hDC, 0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), RGB(255, 0, 255));

                    pMemDC->SelectObject(pOldbmp);
                    pBmp->DeleteObject();
                    //	ReleaseDC(pdc_);
                    delete pMemDC;
                    delete pBmp;

                }
            }
        }
        else
        {
            if (pUnit->m_bgFilename_down != "")
            {
                if(!pUnit->m_bIsTranslate)
                    ::DrawImage(ToFileDir(pUnit->m_bgFilename_down), pdc, pUnit->m_Rect);
                else
                {
                    CDC *pMemDC = new CDC();
                    CBitmap *pBmp = new CBitmap(); 
                    CDC *pdc_ = GetDC();
                    pMemDC->CreateCompatibleDC(pdc_); 
                    pBmp->CreateCompatibleBitmap(pMemDC, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
                    CBitmap *pOldbmp = pMemDC->SelectObject(pBmp);
                    CRect rt1 = CRect(0, 0, pUnit->m_Rect.Width()-1, pUnit->m_Rect.Height()-1);
                    ::DrawImage(ToFileDir(pUnit->m_bgFilename_down), pMemDC, rt1);

                    TransparentBlt2(pdc->m_hDC, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), pMemDC->m_hDC, 0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), RGB(255, 0, 255));

                    pMemDC->SelectObject(pOldbmp);
                    pBmp->DeleteObject();
                    ReleaseDC(pdc_);
                    delete pMemDC;
                    delete pBmp;

                }
            }
            else if(m_currentMJPGList->bgfilename_down != "")
            {
                //test lxz 20080703
                if(m_currentMJPGList->bgfilename_down.Find(L".bmp") != -1 || m_currentMJPGList->bgfilename_down.Find(L".BMP") != -1)
                    ::DrawImage(ToFileDir(m_currentMJPGList->bgfilename_down), pdc, pUnit->m_Rect);
                else
                {
                    CDC *pMemDC = new CDC();
                    CBitmap *pBmp = new CBitmap(); 
                    CDC *pdc_ = GetDC();
                    pMemDC->CreateCompatibleDC(pdc_); 
                    pBmp->CreateCompatibleBitmap(pdc_, m_ClientRect.Width(), m_ClientRect.Height());
                    CBitmap *pOldbmp = pMemDC->SelectObject(pBmp);
                    ::DrawImage(ToFileDir(m_currentMJPGList->bgfilename_down), pMemDC, m_ClientRect);
                    pdc->BitBlt(pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), pMemDC, pUnit->m_Rect.left, pUnit->m_Rect.top, SRCCOPY);
                    pMemDC->SelectObject(pOldbmp);
                    pBmp->DeleteObject();
                    ReleaseDC(pdc_);
                    delete pMemDC;
                    delete pBmp;
                }
            }
        }

        //画文字
        if (pUnit->m_sContent == "")
            return;
        int nLanguage = DEFAULT_CHARSET;
        TCHAR code = pUnit->m_sContent.GetAt(0);
        if((code >= 0x3130 && code <= 0x318f) || code >= 0xAC00 && code <= 0xD7A3)
            nLanguage = HANGEUL_CHARSET;

        VERIFY(m_Font.CreateFont(
            gFontSize[pUnit->m_FontSize],                        // nHeight
            0,                         // nWidth
            0,                         // nEscapement
            0,                         // nOrientation
            gFontWeights[pUnit->m_FontWeights],//FW_NORMAL,       // nWeight
            pUnit->m_bFontItalic,                     // bItalic
            pUnit->m_bFontUnLine,                     // bUnderline
            0,                         // cStrikeOut
            nLanguage,                 // DEFAULT_CHARSET,//HANGUL_CHARSET,           // nCharSet
            OUT_DEFAULT_PRECIS,        // nOutPrecision
            CLIP_DEFAULT_PRECIS,       // nClipPrecision
            DEFAULT_QUALITY,           // nQuality
            DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
            gFontName[pUnit->m_Font]));                 // lpszFacename

        CFont *pFont = pdc->SelectObject(&m_Font);
        pdc->SetTextColor(gFontColor[pUnit->m_FontColor]);

        MULTILINEINFO pVarry[100];
        CRect rt = pUnit->m_Rect;
        int n = GetCStringMuliteLine(pUnit->m_sContent, gFontSize[pUnit->m_FontSize], rt, &pVarry[0], 100);	
        DrawMultiLine(pdc, pUnit->m_sContent, gFontSize[pUnit->m_FontSize]+gFontHeight[pUnit->m_FontHeight], gFontSize[pUnit->m_FontSize], rt, &pVarry[0], n, pUnit->m_FontAlign);

        pdc->SelectObject(pFont);
        m_Font.DeleteObject();
    }

    void CMJPGStatic::DrawMJPGPage(CString sFile)
    {
        if (m_currentMJPGList == NULL)
        {
            return;
        }
        CreateMemDC();
        CRect rt1 = m_currentMJPGList->m_Rect;
        SetMJPGRect(rt1);
        //MoveWindow(m_rtMJPG, FALSE);
        m_ClientRect = CRect(0, 0, rt1.Width(), rt1.Height());
        CDC *pdc = GetDC();

        if(m_currentMJPGList)
        {
#if 1
            m_sCurrentLinkFile = sFile;
            DWORD  s = GetTickCount(); 
            DWORD   dwStart   =   GetTickCount(); 	
            if(m_currentMJPGList->bgfilename != "")
                ::DrawImage(ToFileDir(m_currentMJPGList->bgfilename), m_pMemDC, m_ClientRect);
            else
                m_pMemDC->FillSolidRect(&m_ClientRect, RGB(255, 255, 255));
            DWORD offset = GetTickCount() - dwStart;   
            //TRACE(L"BG %d\n", offset);

            for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
            {
                DWORD  s = GetTickCount(); 
                DWORD   dwStart   =   GetTickCount(); 
                MJPGItem *item = m_currentMJPGList->items[i];
                DrawUnit(&item->unitparam, m_pMemDC);
                DWORD offset = GetTickCount() - dwStart;   
                //TRACE(L"Unit %d\n", offset);
            }
            pdc->BitBlt(m_ClientRect.left, m_ClientRect.top, m_ClientRect.Width(), m_ClientRect.Height(), m_pMemDC, 0, 0, SRCCOPY);
#else
            m_sCurrentLinkFile = sFile;
            DWORD  s = GetTickCount(); 
            DWORD   dwStart   =   GetTickCount(); 	
            DrawImage(ToFileDir(m_currentMJPGList->bgfilename), pdc, m_ClientRect);
            DWORD offset = GetTickCount() - dwStart;   
            //TRACE(L"BG %d\n", offset);

            for (int i = 0; i < m_currentMJPGList->items.size(); ++i)
            {
                DWORD  s = GetTickCount(); 
                DWORD   dwStart   =   GetTickCount(); 
                MJPGItem *item = m_currentMJPGList->items[i];
                DrawUnit(&item->unitparam, pdc);
                DWORD offset = GetTickCount() - dwStart;   
                //TRACE(L"Unit %d\n", offset);
            }
#endif
        }
    } 

    CString	CMJPGStatic::ToFileDir(CString filename)
    {
        CString sDirName;
        if (filename == _T("") || filename.GetAt(0) != _T('.'))
            return filename;
        sDirName = _T("C:");
        sDirName += filename.Mid(1);
        return sDirName;
    }

    void CMJPGStatic::CreateMemDC() 
    {
        CDC *pdc = GetDC();
        m_pMemDC = new CDC();
        m_pBmp = new CBitmap(); 
        m_pMemDC->CreateCompatibleDC(pdc); 
        m_pBmp->CreateCompatibleBitmap(pdc, m_ClientRect.Width(), m_ClientRect.Height());   
        m_pOldbmp = m_pMemDC->SelectObject(m_pBmp);   
        ReleaseDC(pdc);
    }

    void CMJPGStatic::ReleaseMemDC() 
    {
        m_pMemDC->SelectObject(m_pOldbmp);
        m_pBmp->DeleteObject();
        delete m_pMemDC;
        delete m_pBmp;
    }


    void CMJPGStatic::SetCurrentLinkFile(CString s)
    {
        m_sCurrentLinkFile = ToFileDir(s);
        if (m_currentMJPGList)
            delete m_currentMJPGList;
        m_currentMJPGList = ::Util::XmlParser::ParseFileToMJPGList_(m_sCurrentLinkFile);

        if (m_currentMJPGList)
        {
            m_nIsCrollIndex = -1;
            m_nIsCrollCount = 0;
            for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
            {
                if (m_currentMJPGList->items[i]->unitparam.m_Ctrltype > ctrl_unit)
                {
                    int nstyle = 0;
                    if (m_currentMJPGList->items[i]->unitparam.m_bIsShow)
                        nstyle = WS_VISIBLE;
                    switch (m_currentMJPGList->items[i]->unitparam.m_Ctrltype)
                    {
                    case ctrl_edit:
                        {

                        }
                        break;
                    case ctrl_chechbutton:
                        {

                        }
                        break;
                    case ctrl_radiobutton:
                        {

                        }
                        break;
                    case ctrl_commbox:
                        {

                        }
                        break;
                    default:
                        break;
                    }
                }
                if (m_currentMJPGList->items[i]->unitparam.m_nIsScroll && m_currentMJPGList->items[i]->unitparam.m_bIsShow && m_nIsCrollIndex == -1)
                {
                    m_nIsCrollIndex = m_currentMJPGList->items[i]->unitparam.m_nIsScroll;
                }
                if (m_currentMJPGList->items[i]->unitparam.m_nIsScroll)
                    m_nIsCrollCount = m_currentMJPGList->items[i]->unitparam.m_nIsScroll;
            }

            if (m_nIsCrollIndex == 1)
            {
                SetUnitIsShow(1001, FALSE);
            }
            if (m_nIsCrollCount == 1)
            {
                SetUnitIsShow(1002, FALSE);
            }
        }
    }

    //int upordown 0 up, 1 down.
    void CMJPGStatic::ScrollUnit(int upordown, int unitcount)
    {
        if (upordown == 0)  //上翻
        {
            if (m_nIsCrollIndex == 1)
                return;
            m_nIsCrollIndex--;

            if (m_nIsCrollIndex == 1)
            {
                SetUnitIsShow(1001, FALSE);
                SetUnitIsShow(1002, TRUE);
            }
            else
            {
                SetUnitIsShow(1001, TRUE);
                SetUnitIsShow(1002, TRUE);
            }
        }
        else		//下翻
        {
            if (m_nIsCrollIndex == m_nIsCrollCount)
                return;
            ++m_nIsCrollIndex;
            if (m_nIsCrollIndex == m_nIsCrollCount)
            {
                SetUnitIsShow(1002, FALSE);
                SetUnitIsShow(1001, TRUE);
            }
            else
            {
                SetUnitIsShow(1001, TRUE);
                SetUnitIsShow(1002, TRUE);
            }
        }
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); i++)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nIsScroll == m_nIsCrollIndex)
            {
                m_currentMJPGList->items[i]->unitparam.m_bIsShow = TRUE;

                if (m_currentMJPGList->items[i]->unitparam.m_pUnitWnd)
                    m_currentMJPGList->items[i]->unitparam.m_pUnitWnd->ShowWindow(SW_SHOW);
            }
            else if (m_currentMJPGList->items[i]->unitparam.m_nIsScroll)
            {
                m_currentMJPGList->items[i]->unitparam.m_bIsShow = FALSE;
                if (m_currentMJPGList->items[i]->unitparam.m_pUnitWnd)
                    m_currentMJPGList->items[i]->unitparam.m_pUnitWnd->ShowWindow(SW_HIDE);
            }
        }
        Invalidate();
    }

    void CMJPGStatic::PreSubclassWindow() 
    {
        // TODO: Add your specialized code here and/or call the base class
        DWORD dwStyle = GetStyle();
        ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

        CStatic::PreSubclassWindow();
    }


    /////////////////////////////////////////////////////////////////////////////
    // CMJPGStatic message handlers

    void CMJPGStatic::OnLButtonDown(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        int nIndex = FindUnitIndex(point);
        m_oldMJPGList = m_currentMJPGList;
        m_oldLinkFile = m_sCurrentLinkFile;
        if (nIndex >= 0)
        {
            if (m_currentMJPGList->items[nIndex]->unitparam.m_bIsDisable)   //不响应消息
                return;

            m_nIndexSelectUnit = nIndex;
            //InvertRect(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Rect);
            DrawUnitStatus(&m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam, 1);
        }
        else 
        {
            m_nIndexSelectUnit = -1;
        }
        CStatic::OnLButtonDown(nFlags, point);
    }

    void CMJPGStatic::OnLButtonUp(UINT nFlags, CPoint point) 
    {
        // TODO: Add your message handler code here and/or call default
        //	Dprintf("test up %\n", m_nIndexSelectUnit);

        if (m_oldLinkFile != m_sCurrentLinkFile)
        {
            m_nIndexSelectUnit = -1;
            return;
        }

        if (m_nIndexSelectUnit >= 0)
        {
            //InvertRect(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Rect);
            if (IsWindowVisible())
                if (GetParent()->IsWindowVisible())
                    DrawUnitStatus(&m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam, 0);


            if (m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_link) //link
            {
                CString sLink = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
                if(sLink != "")
                {
                    sLink = ToFileDir(sLink);
                    SetCurrentLinkFile(sLink);
                    DrawMJPGPage(sLink);
                }
                m_nIndexSelectUnit = -1;
            }
            else if (m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_click) //click
            {
                CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
                if (sDial != "")
                {
                    std::string str = Util::StringOp::FromCString(sDial);
                    //CString s_ = str.c_str();
                    int code = atoi(str.c_str());
                    if(code == 2)
                    {
                        //Dprintf("test\n");
                    }
                    GetParent()->PostMessage(WM_CLICKMJPG_TOAPP, (WPARAM)code, GetDlgCtrlID());
                }
            }
            else if (m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_pageup)		//上翻
            {
                CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
                if (sDial != "")
                {
                    std::string str = Util::StringOp::FromCString(sDial);
                    //CString s_ = str.c_str();
                    int code = atoi(str.c_str());
                    if (code > 0)
                        ScrollUnit(0, code);
                }
            }
            else if (m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == uint_pagedown)	//下翻
            {
                CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
                if (sDial != "")
                {
                    std::string str = Util::StringOp::FromCString(sDial);
                    //CString s_ = str.c_str();
                    int code = atoi(str.c_str());
                    if (code > 0)
                        ScrollUnit(1, code);
                }
            }
            else if (m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_outdial)
            {
                //软件拨号

                CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
                main->phone_->HungOn();
                Sleep(1000);
                char telcode[128];
                memset(telcode, 0, 128);
                wcstombs(telcode, m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent.GetBuffer(64), 64);
                //Dprintf("Dial %s\n ", telcode);
                //	main->phone_->DialNumber(telcode);

            }

            m_nIndexSelectUnit = -1;
        }
        CStatic::OnLButtonUp(nFlags, point);
    }

    int CMJPGStatic::FindUnitIndex(CPoint pt)
    {
        int ret = -1;

        if (m_currentMJPGList)
        {
            for (std::size_t i = 0; i < m_currentMJPGList->items.size(); i++)
            {
                if (m_currentMJPGList->items[i]->unitparam.m_Rect.PtInRect(pt) && m_currentMJPGList->items[i]->unitparam.m_bIsShow)   //可见
                {
                    if (m_currentMJPGList->items[i]->unitparam.m_UnitType >= unit_link)
                    {
                        ret = i;
                        break;
                    }
                }
            }
        }
        return ret;
    }

    void CMJPGStatic::InvertRect(CRect rt)
    {
        CDC *pdc = GetDC();
        pdc->InvertRect(&rt);
        ReleaseDC(pdc);
    }

    void CMJPGStatic::OnPaint() 
    {
        //	DMemprintf("MJPGStatic0 ");
        CPaintDC dc(this); // device context for painting
        DrawMJPGPage(m_sCurrentLinkFile);
    }

    BOOL CMJPGStatic::GetUnitIsDownStatus(int UnitNO)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                if (m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus )
                    return TRUE;
            }
        }
        return FALSE;
    }

    BOOL CMJPGStatic::SetUnitIsDownStatus(int UnitNO, BOOL isDown)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if(m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus = isDown;
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL CMJPGStatic:: SetUnitIsShow(int UnitNO, BOOL isShow, BOOL isInvalidate)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                m_currentMJPGList->items[i]->unitparam.m_bIsShow = isShow;
                if (isInvalidate)
                    InvalidateRect(m_currentMJPGList->items[i]->unitparam.m_Rect);
                else
                    DrawUnitStatus(&m_currentMJPGList->items[i]->unitparam, m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus);
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL CMJPGStatic:: GetUnitIsShow(int UnitNO)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                return m_currentMJPGList->items[i]->unitparam.m_bIsShow;
            }
        }
        return FALSE;
    }


    BOOL CMJPGStatic::SetUnitBitmap(int UnitNO, CString filename_up, CString filename_down, BOOL isDraw, BOOL isInvalidate)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                //	if(filename_up != "")
                m_currentMJPGList->items[i]->unitparam.m_bgFilename = filename_up;
                //	if(filename_down != "")
                m_currentMJPGList->items[i]->unitparam.m_bgFilename_down = filename_down;
                if (isDraw)
                {
                    if (isInvalidate)
                        InvalidateRect(m_currentMJPGList->items[i]->unitparam.m_Rect);
                    else
                        DrawUnitStatus(&m_currentMJPGList->items[i]->unitparam, m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus);
                }
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL CMJPGStatic::SetUnitIsDisable(int UnitNO, BOOL isDisable)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                m_currentMJPGList->items[i]->unitparam.m_bIsDisable = isDisable;
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL CMJPGStatic::SetUnitIsTranslate(int UnitNO, BOOL isTrans)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                m_currentMJPGList->items[i]->unitparam.m_bIsTranslate = isTrans;
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL CMJPGStatic::SetUnitText(int UnitNO, CString text, BOOL isDraw, BOOL isInvalidate)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                m_currentMJPGList->items[i]->unitparam.m_sContent = text;
                if (isDraw)
                {
                    if (isInvalidate)
                        InvalidateRect(m_currentMJPGList->items[i]->unitparam.m_Rect);
                    else
                        DrawUnitStatus(&m_currentMJPGList->items[i]->unitparam, m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus);
                }				
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL CMJPGStatic::SetUnitColor(int UnitNO, TEXTCOLOR cl, BOOL isDraw, BOOL isInvalidate)
    {
        if (!m_currentMJPGList)
            return FALSE;
        for (std::size_t i = 0; i < m_currentMJPGList->items.size(); ++i)
        {
            if (m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
            {
                m_currentMJPGList->items[i]->unitparam.m_FontColor = cl;
                if (isDraw)
                {
                    if (isInvalidate)
                        InvalidateRect(m_currentMJPGList->items[i]->unitparam.m_Rect);
                    else
                        DrawUnitStatus(&m_currentMJPGList->items[i]->unitparam, m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus);
                }				
                return TRUE;
            }
        }
        return FALSE;
    }

    //int nUintStatus  0  up  1 down
    void CMJPGStatic::DrawUnitStatus(UNIT *pUnit, int nUintStatus)
    {
        if (!pUnit)
            return;

        if (!pUnit->m_bIsShow)
            return;

        DWORD dwStart = GetTickCount(); 	

        //贴背景图
        CString sAllJpg;
        CString sUintJpg;
        if (pUnit->m_bIsDownStatus)
        {
            sUintJpg = pUnit->m_bgFilename_down;
            sAllJpg = m_currentMJPGList->bgfilename_down;
        }
        else
        {
            if (nUintStatus == 0)   //up
            {
                sUintJpg = pUnit->m_bgFilename;
                sAllJpg = m_currentMJPGList->bgfilename;
            }
            else if (nUintStatus == 1) //down
            {
                sUintJpg = pUnit->m_bgFilename_down;
                sAllJpg = m_currentMJPGList->bgfilename_down;
            }
        }

        if (sUintJpg != "" || sAllJpg != "")
        {
            CDC *pdc = GetDC();
            CDC *pMemDC1 = new CDC();
            CBitmap *pBmp1 = new CBitmap(); 
            //	CDC *pdc_ = GetDC();
            pMemDC1->CreateCompatibleDC(pdc); 
            pBmp1->CreateCompatibleBitmap(pdc, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
            CBitmap *pOldbmp1 = pMemDC1->SelectObject(pBmp1);
            pMemDC1->SetBkMode(TRANSPARENT);

            if (sUintJpg != "")
            {
                //	DrawImage(ToFileDir(sUintJpg), pdc, pUnit->m_Rect);
                if (!pUnit->m_bIsTranslate)
                    ::DrawImage(ToFileDir(sUintJpg), pMemDC1, CRect(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height()));
                else
                {
                    CDC *pMemDC = new CDC();
                    CBitmap *pBmp = new CBitmap(); 
                    CDC *pdc_ = GetDC();
                    pMemDC->CreateCompatibleDC(pdc_); 
                    pBmp->CreateCompatibleBitmap(pMemDC, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
                    CBitmap *pOldbmp = pMemDC->SelectObject(pBmp);
                    CRect rt1 = CRect(0, 0, pUnit->m_Rect.Width()-1, pUnit->m_Rect.Height()-1);
                    ::DrawImage(ToFileDir(sUintJpg), pMemDC, rt1);

                    TransparentBlt2(pMemDC1->m_hDC, rt1.left, rt1.top, rt1.Width(), rt1.Height(), pMemDC->m_hDC, 0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), RGB(255, 0, 255));

                    pMemDC->SelectObject(pOldbmp);
                    pBmp->DeleteObject();
                    ReleaseDC(pdc_);
                    delete pMemDC;
                    delete pBmp;

                }
            }
            else if (sAllJpg != "")
            {
                //test lxz 20080703
                if (m_currentMJPGList->bgfilename_down.Find(L".bmp") != -1 || m_currentMJPGList->bgfilename_down.Find(L".BMP") != -1)
                    ::DrawImage(ToFileDir(sAllJpg), pMemDC1, pUnit->m_Rect, TRUE);
                else
                {
                    CDC *pMemDC = new CDC();
                    CBitmap *pBmp = new CBitmap(); 
                    pMemDC->CreateCompatibleDC(pdc); 
                    pBmp->CreateCompatibleBitmap(pdc, m_ClientRect.Width(), m_ClientRect.Height());
                    CBitmap *pOldbmp = pMemDC->SelectObject(pBmp);
                    ::DrawImage(ToFileDir(sAllJpg), pMemDC, m_ClientRect);
                    pMemDC1->BitBlt(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), pMemDC, pUnit->m_Rect.left, pUnit->m_Rect.top, SRCCOPY);
                    pMemDC->SelectObject(pOldbmp);
                    pBmp->DeleteObject();
                    delete pMemDC;
                    delete pBmp;
                }
            }
            else
            {
                pMemDC1->InvertRect(CRect(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height()));
                //		DMemprintf("DrawUnitStatus 1 ");
            }

            DWORD offset = GetTickCount() - dwStart;   
            //	TRACE(L"DrawUnit Status %d\n", offset);

            //画文字
            if (pUnit->m_sContent != "")
            {
                //	int ret = DMemprintf("Font 0");
                int nLanguage = DEFAULT_CHARSET;
                TCHAR code = pUnit->m_sContent.GetAt(0);
                if ((code >= 0x3130 && code <= 0x318f) || code >= 0xAC00 && code <= 0xD7A3)
                    nLanguage = HANGEUL_CHARSET;

                VERIFY(m_Font.CreateFont(
                    gFontSize[pUnit->m_FontSize],                        // nHeight
                    0,                         // nWidth
                    0,                         // nEscapement
                    0,                         // nOrientation
                    gFontWeights[pUnit->m_FontWeights],//FW_NORMAL,       // nWeight
                    pUnit->m_bFontItalic,                     // bItalic
                    pUnit->m_bFontUnLine,                     // bUnderline
                    0,                         // cStrikeOut
                    nLanguage,                 // DEFAULT_CHARSET,//HANGUL_CHARSET,           // nCharSet
                    OUT_DEFAULT_PRECIS,        // nOutPrecision
                    CLIP_DEFAULT_PRECIS,       // nClipPrecision
                    DEFAULT_QUALITY,           // nQuality
                    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                    gFontName[pUnit->m_Font]));                 // lpszFacename

                CFont *pFont = pMemDC1->SelectObject(&m_Font);
                pMemDC1->SetTextColor(gFontColor[pUnit->m_FontColor]);

                MULTILINEINFO pVarry[100];
                CRect rt = CRect(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
                int n = GetCStringMuliteLine(pUnit->m_sContent, gFontSize[pUnit->m_FontSize], rt, &pVarry[0], 100);	
                DrawMultiLine(pMemDC1, pUnit->m_sContent, gFontSize[pUnit->m_FontSize]+gFontHeight[pUnit->m_FontHeight], gFontSize[pUnit->m_FontSize], rt, &pVarry[0], n, pUnit->m_FontAlign);

                pMemDC1->SelectObject(pFont);
                m_Font.DeleteObject();
            }

            pdc->BitBlt(pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), pMemDC1, 0, 0, SRCCOPY);
            pMemDC1->SelectObject(pOldbmp1);
            pBmp1->DeleteObject();
            ReleaseDC(pdc);
            delete pMemDC1;
            delete pBmp1;
        }
        else
        {
            CDC *pdc = GetDC();
            pdc->InvertRect(pUnit->m_Rect);
            if (pUnit->m_sContent != "")
            {
                //	int ret = DMemprintf("Font 0");
                int nLanguage = DEFAULT_CHARSET;
                TCHAR code = pUnit->m_sContent.GetAt(0);
                if ((code >= 0x3130 && code <= 0x318f) || code >= 0xAC00 && code <= 0xD7A3)
                    nLanguage = HANGEUL_CHARSET;

                VERIFY(m_Font.CreateFont(
                    gFontSize[pUnit->m_FontSize],                        // nHeight
                    0,                         // nWidth
                    0,                         // nEscapement
                    0,                         // nOrientation
                    gFontWeights[pUnit->m_FontWeights],//FW_NORMAL,       // nWeight
                    pUnit->m_bFontItalic,                     // bItalic
                    pUnit->m_bFontUnLine,                     // bUnderline
                    0,                         // cStrikeOut
                    nLanguage,                 // DEFAULT_CHARSET,//HANGUL_CHARSET,           //nCharSet
                    OUT_DEFAULT_PRECIS,        // nOutPrecision
                    CLIP_DEFAULT_PRECIS,       // nClipPrecision
                    DEFAULT_QUALITY,           // nQuality
                    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                    gFontName[pUnit->m_Font]));                 // lpszFacename

                CFont *pFont = pdc->SelectObject(&m_Font);
                pdc->SetTextColor(gFontColor[pUnit->m_FontColor]);

                MULTILINEINFO pVarry[100];
                int n = GetCStringMuliteLine(pUnit->m_sContent, gFontSize[pUnit->m_FontSize], pUnit->m_Rect, &pVarry[0], 100);	
                DrawMultiLine(pdc, pUnit->m_sContent, gFontSize[pUnit->m_FontSize]+gFontHeight[pUnit->m_FontHeight], gFontSize[pUnit->m_FontSize], pUnit->m_Rect, &pVarry[0], n, pUnit->m_FontAlign);

                pdc->SelectObject(pFont);
                m_Font.DeleteObject();
            }
            ReleaseDC(pdc);
        }
        //	DMemprintf("DrawUnitStatus 1 ");
    }
}
