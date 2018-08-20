// MJPGStatic.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "MJPGStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMJPGStatic
extern void DrawImage(LPCTSTR szFileImage, CDC *pdc, CRect rect);
//globle data
CString gLangName[] = {".\\adv\\mjpg\\中文\\playcfg.xml", ".\\adv\\mjpg\\英文\\playcfg.xml", ".\\adv\\mjpg\\日文\\playcfg.xml", ".\\adv\\mjpg\\韩文\\playcfg.xml"};
CString gAdvPlayList = ".\\adv\\video\\advplaylist.xml";
CString gFontName[] = {"宋体", "黑体", "Arial", "Times New Roman"};
UINT gFontWeights[] = {FW_NORMAL, FW_BOLD};
UINT gFontSize[] = {10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
UINT gFontHeight[] = {1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30};
COLORREF gFontColor[] = {RGB(0, 0, 0), RGB(255, 255, 255), RGB(255, 0, 0), RGB(0, 0, 255), RGB(0, 100, 0), RGB(255, 255, 0)};
UINT gFontAlign[] = {DT_WORDBREAK|DT_NOPREFIX|DT_EDITCONTROL, DT_WORDBREAK|DT_NOPREFIX|DT_EDITCONTROL, DT_WORDBREAK|DT_NOPREFIX|DT_EDITCONTROL};

CMJPGStatic::CMJPGStatic()
{
}

CMJPGStatic::~CMJPGStatic()
{
}


BEGIN_MESSAGE_MAP(CMJPGStatic, CStatic)
	//{{AFX_MSG_MAP(CMJPGStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
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
	if(rt.Width() ==0 || (rt.Width()-MULITI_RECT_OFFSET*2) < height)
		return 0;
	TEXTMETRIC tm;
	int nCount = (rt.Width()-MULITI_RECT_OFFSET*2)/(height/2);
	if(nCount <= 0)
		return 0;
	int nLineCount = 0; 
	int nLength = s.GetLength();
	int n= 0;
	int nChar = 0;
	for(int i = 0; i < nLength; i++)
	{
		if(n == 0)
		{
			if(nLineCount >= (LineCount - 1))
				return nLineCount;
			pCount[nLineCount].nStart = i;
			nChar = 0;
		}

		CString t = s.Mid(i, 1);
		if(t == L"\r")
		{
			if(n == 0)			//处理连续回车
			{
				pCount[nLineCount].nCount = n;
				n = 0;
				nLineCount++;

				continue;
			}
			pCount[nLineCount].nCount = n;
			n = 0;
			nLineCount++;
		}
		
		else if(t == L"\n")
		{
			if(n == 0)
				continue;
			pCount[nLineCount].nCount = n;
			n = 0;
			nLineCount++;
		}

		else if(i == nLength-1)
		{
			pCount[nLineCount].nCount = n+1;
			n = 0;
			nLineCount++;
		}
		else 
		{
			TCHAR c = s.GetAt(i);
			if(c&0xFF00)  //双字节
			{
				nChar += 2;
				if(nChar >nCount)						//多一个字节换行
				{
					if(!IsBiaodianFuhao(c))			   //符号显示在上一行上
					{
						i--;							//i需要退一个字符
					}
					else
					{
						i++;
						n++;
						if(IsBiaodianFuhao(s.GetAt(i)))
						{
							n++;
							i++;
						}
					}
					pCount[nLineCount].nCount = n;     //自动折行了，不能加1;
					n = 0;
					nLineCount++;
				}
				else if(nChar == nCount)				//正好换行
				{
					if(IsBiaodianFuhao(s.GetAt(i+1)))			//符号显示在上一行上
					{
						n++;
						i++;
						if(IsBiaodianFuhao(s.GetAt(i)))
						{
							n++;
							i++;
							if(IsBiaodianFuhao(s.GetAt(i)))
							{
								n++;
								i++;
							}
						}
					}

					pCount[nLineCount].nCount = n+1;   
					n = 0;
					nLineCount++;
				}
				else
				{
					n++;
				}
			}
			else	//单字节
			{
				nChar += 1;
				if(nChar == nCount)
				{
					if(IsBiaodianFuhao(s.GetAt(i+1)))			//符号显示在上一行上
					{
						n++;
						i++;
					}

					pCount[nLineCount].nCount = n+1;
					n = 0;
					nLineCount++;
				}
				else
				{
					n++;
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
	if(i >= (nCount-1))
	{
		return s1;
	}
	else
	{
		int index = pVarry[i].nStart + pVarry[i].nCount-1;
		TCHAR temp = s.GetAt(index);
		TCHAR temp1 = s.GetAt(index+1);
		if(((temp >= L'a' && temp <= 'z') || (temp >= L'A' && temp <= 'Z')) && ((temp1 >= L'a' && temp1 <= 'z') || (temp1 >= L'A' && temp1 <= 'Z')))
		{
			s1 += "-";
		}
	}
	return s1;
}

void CMJPGStatic::DrawMultiLine(CDC *pdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1)
{
	int nPixel1 = nPixel;
	
	int nRectLine = rt.Height()/nPixel1;
	if(nRectLine == 0)
		nRectLine = 1;
	int nTxtLine = LineCount;
	int nShowLine = nTxtLine;
	BOOL bIsExtentRect = FALSE;
	if(nTxtLine > nRectLine)
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
			for(int i = 0; i < nShowLine; i++)
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
			int offset = (rt_.Height()-nShowLine*nPixel1)/2;
			for(int i = 0; i < nShowLine; i++)
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
			int offset = rt_.Height()-nShowLine*nPixel1;
			for(int i = 0; i < nShowLine; i++)
			{
				CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
				CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1+offset, rt_.right, rt_.top+(i+1)*nPixel1+offset);
				pdc->DrawText(s1, &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
			}
			break;
		}
	case middle_top:
		{
			for(int i = 0; i < nShowLine; i++)
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
			int offset = (rt_.Height()-nShowLine*nPixel1)/2;
			for(int i = 0; i < nShowLine; i++)
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

void CMJPGStatic::DrawUnit(UNIT *pUnit, CDC *pdc)
{
	pdc->SetBkMode(TRANSPARENT);
	
	//贴背景图
	if(pUnit->m_bgFilename != "")
		DrawImage(ToFileDir(pUnit->m_bgFilename), pdc, pUnit->m_Rect);

	//画文字
	if(pUnit->m_sContent == "")
		return;
//	int ret = DMemprintf("Font 0");
	int nLanguage = DEFAULT_CHARSET;
	TCHAR code = pUnit->m_sContent.GetAt(0);
	if((code >= 0x3130 && code <= 0x318f) || code >= 0xAC00 && code <= 0xD7A3)
		nLanguage = HANGEUL_CHARSET;

	VERIFY(m_Font.CreateFont(
		gFontSize[pUnit->m_FontSize],                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
	/*	gFontWeights[pUnit->m_FontWeights]*/FW_NORMAL,       // nWeight
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
/*
	int ret1 = DMemprintf("Font 1");
	if(ret1 != ret)
	{
		Dprintf("Font = %d\n", ret-ret1);
		CString s = pUnit->m_bgFilename;
		char txt[128];
		SYSTEMTIME curtime;
		GetLocalTime(&curtime);
		gNNCount += (ret-ret1);
		sprintf(txt, " %02d:%02d:%02d %d %d %d\n", curtime.wHour, curtime.wMinute, curtime.wSecond, ret-ret1, gNNCount, ret1);
		s += txt;
		char data[256];
		memset(data, 0, 256);
		wcstombs(data, s.GetBuffer(256), 256);
		gFile = fopen("/flashdrv/test.txt", "a+");
		fwrite(data, strlen(data), sizeof(char), gFile);
		fclose(gFile);
	}
*/
}

void CMJPGStatic::DrawMJPGPage(CString sFile)
{
	CDC *pdc = GetDC();
	//	int ret = DMemprintf("MJPGPage 0");
	if(m_currentMJPGList)
		delete m_currentMJPGList;
	//	DMemprintf("MJPGPage 1");
	m_currentMJPGList = ::Util::XmlParser::ParseFileToMJPGList_(sFile);
	if(m_currentMJPGList == NULL)
	{
		Dprintf("m_currentMJPGList == NULL\n");
//		OnTimer(IDT_CHANGTO_MAINMJPG);   //lxz
	}
	//	DMemprintf("MJPGPage 2");
	
	if(m_currentMJPGList)
	{
#if 1
		m_sCurrentLinkFile = sFile;
		//移到该窗口类中
		/*
		CDC memDC;
		CBitmap bmp;
		memDC.CreateCompatibleDC(pdc); 
		bmp.CreateCompatibleBitmap(pdc, m_rtMJPG.Width(), m_rtMJPG.Height());   
		CBitmap *oldbmp = memDC.SelectObject(&bmp);   
		*/
		
		DWORD  s = GetTickCount(); 
		DWORD   dwStart   =   GetTickCount(); 	
		if(m_currentMJPGList->bgfilename != "")
			DrawImage(ToFileDir(m_currentMJPGList->bgfilename), &m_memDC, m_rtMJPG);
		else
			m_memDC.FillSolidRect(&m_rtMJPG, RGB(255, 255, 255));
		DWORD offset = GetTickCount() - dwStart;   
		TRACE(L"BG %d\n", offset);
		
		for (int i = 0; i < m_currentMJPGList->items.size(); ++i)
		{
			DWORD  s = GetTickCount(); 
			DWORD   dwStart   =   GetTickCount(); 
			MJPGItem *item = m_currentMJPGList->items[i];
			DrawUnit(&item->unitparam, &m_memDC);
			DWORD offset = GetTickCount() - dwStart;   
			TRACE(L"Unit %d\n", offset);
		}
		pdc->BitBlt(m_rtMJPG.left, m_rtMJPG.top, m_rtMJPG.Width(), m_rtMJPG.Height(), &m_memDC, 0, 0, SRCCOPY);
		//移到全局中
		// 		memDC.SelectObject(&oldbmp);
		// 		memDC.DeleteDC();   
		// 		bmp.DeleteObject();
#else
		m_sCurrentLinkFile = sFile;
		DWORD  s = GetTickCount(); 
		DWORD   dwStart   =   GetTickCount(); 	
		DrawImage(ToFileDir(m_currentMJPGList->bgfilename), pdc, m_rtMJPG);
		DWORD offset = GetTickCount() - dwStart;   
		TRACE(L"BG %d\n", offset);
		
		for (int i = 0; i < m_currentMJPGList->items.size(); ++i)
		{
			DWORD  s = GetTickCount(); 
			DWORD   dwStart   =   GetTickCount(); 
			MJPGItem *item = m_currentMJPGList->items[i];
			DrawUnit(&item->unitparam, pdc);
			DWORD offset = GetTickCount() - dwStart;   
			TRACE(L"Unit %d\n", offset);
		}
#endif
	}
	/*
	int ret1 = DMemprintf("MJPGPage 3");
	if(ret1 != ret)
	{
	//wprintf(L"%s\n", sFile);
	Dprintf("MJPGPage = %d\n", ret-ret1);
	CString s = sFile;
	char txt[128];
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	gNNCount += (ret-ret1);
	sprintf(txt, " %02d:%02d:%02d %d %d %d\n", curtime.wHour, curtime.wMinute, curtime.wSecond, ret-ret1, gNNCount, ret1);
	s += txt;
	char data[256];
	memset(data, 0, 256);
	wcstombs(data, s.GetBuffer(256), 256);
	gFile = fopen("/flashdrv/test.txt", "a+");
	fwrite(data, strlen(data), sizeof(char), gFile);
	fclose(gFile);
	}
	*/
	ReleaseDC(pdc);
} 

CString	CMJPGStatic::ToFileDir(CString filename)
{
	CString sDirName = "";
	if(filename == "")
		return sDirName;
	sDirName = "\\flashdrv";
	sDirName += filename.Mid(1);
	return sDirName;
}


void CMJPGStatic ::InitMemDC() 
{
	CDC *pdc = GetDC();
	m_memDC.CreateCompatibleDC(pdc); 
	m_bmp.CreateCompatibleBitmap(pdc, 480, 204);   
	m_memDC.SelectObject(&m_bmp);   
	ReleaseDC(pdc);
}

void CMJPGStatic::SetCurrentLinkFile(CString s)
{
	m_sCurrentLinkFile = ToFileDir(s);
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
	if(nIndex >= 0)
	{
		m_nIndexSelectUnit = nIndex;
		InvertRect(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Rect);
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
	if(m_nIndexSelectUnit >= 0)
	{
		InvertRect(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Rect);
		if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_link) //link
		{
			CString sLink = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
			if(sLink != "")
			{
				sLink = ToFileDir(sLink);
				DrawMJPGPage(sLink);
			}
			m_nIndexSelectUnit = -1;
		}
	}
	CStatic::OnLButtonUp(nFlags, point);
}

int CMJPGStatic::FindUnitIndex(CPoint pt)
{
	int ret = -1;
	
	if(m_currentMJPGList)
	{
		for(int i = 0; i < m_currentMJPGList->items.size(); i++)
		{
			if(m_currentMJPGList->items[i]->unitparam.m_Rect.PtInRect(pt))
			{
				if(m_currentMJPGList->items[i]->unitparam.m_UnitType >= unit_link)
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

void CMJPGStatic::InvertRect_HDC(HDC hdc, CRect rt)
{
	InvertRect(hdc, &rt);
}


void CMJPGStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	static BOOL isFLAG = TRUE;
	if(isFLAG)
	{
		isFLAG = FALSE;
		InitMemDC();
	}
	DrawMJPGPage(m_sCurrentLinkFile);
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}
