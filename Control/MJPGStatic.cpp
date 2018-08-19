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

#include "Cache.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LCD_WIDTH	480
#define LCD_HEIGHT  214
/////////////////////////////////////////////////////////////////////////////
// CMJPGStatic
extern void DrawImage(LPCTSTR szFileImage, CDC *pdc, CRect rect, BOOL isOffset = FALSE);
extern void DrawImage_HDC(LPCTSTR szFileImage, HDC pdc, CRect rect, BOOL isOffset = FALSE, VOID *lpBits = NULL);
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
	m_currentMJPGList = NULL;
	m_nIndexSelectUnit = -1;
	m_SetUnitNo = -1;
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
	else if(nChar == LEFT_KEY || nChar == VK_NUMPAD2)
	{
		int rt = FindRowFocusUnit(TOLEFT);
		if(rt != -1)
		{
			SetFocusUnit(TOLEFT, rt);
			Invalidate();
			//SetUnitIsShow(rt, TRUE, TRUE);
		}
	}
	else if(nChar == RIGHT_KEY || nChar == VK_NUMPAD3)
	{
		int rt = FindRowFocusUnit(TORIGHT);
		if(rt != -1)
		{
			SetFocusUnit(TORIGHT, rt);
			Invalidate();
			//SetUnitIsShow(rt, TRUE, TRUE);
		}
	}
	else if(nChar == UP_KEY || nChar == VK_NUMPAD0)
	{
		int rt = FindColFocusUnit(TOUP);
		if(rt >= 0)
		{
			SetFocusUnit(TOUP, rt);
			Invalidate();
			//SetUnitIsShow(rt, TRUE, TRUE);
		}
		else if(rt == -2)
			GetParent()->PostMessage(WM_CLICKMJPG_TOAPP, (WPARAM)UP_KEY, GetDlgCtrlID());
	}
	else if(nChar == DOWN_KEY || nChar == VK_NUMPAD1)
	{
		int rt = FindColFocusUnit(TODOWN);
		if(rt >= 0)
		{
			SetFocusUnit(TODOWN, rt);
			Invalidate();
			//SetUnitIsShow(rt, TRUE, TRUE);
		}
		else if(rt == -3)
			GetParent()->PostMessage(WM_CLICKMJPG_TOAPP, (WPARAM)DOWN_KEY, GetDlgCtrlID());
	}
	else if(nChar == OK_KEY || nChar == VK_NUMPAD5)
	{
		if(m_nFocusUnit >= 0)
			GetParent()->PostMessage(WM_CLICKMJPG_TOAPP, (WPARAM)m_nFocusUnit, GetDlgCtrlID());
	}
	else if(nChar == CANCEL_KEY || nChar == VK_NUMPAD4)
	{
		GetParent()->PostMessage(WM_CLICKMJPG_TOAPP, (WPARAM)CANCEL_KEY, GetDlgCtrlID());
	}
	else if(nChar == 0x88)
		GetParent()->PostMessage(WM_CLICKMJPG_TOAPP, (WPARAM)1000, GetDlgCtrlID());
	CStatic::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMJPGStatic::OnTimer(UINT nID)
{
	if(nID == 10086)
	{
		KillTimer(10086);
		::SetCursor(::LoadCursorW(NULL, IDC_HAND));
	}
//	CMJPGStatic::OnTimer(nID);	
} 
BEGIN_MESSAGE_MAP(CMJPGStatic, CStatic)
	//{{AFX_MSG_MAP(CMJPGStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
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
	/*
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
	*/
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
						if((i < nLength) && IsBiaodianFuhao(s.GetAt(i)))
						{
							n++;
							i++;
						}

						i--; //正好符号换行多加了一个字符 lxz 20081105
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
						if((i < nLength) && IsBiaodianFuhao(s.GetAt(i)))
						{
							n++;
							i++;
							if((i < nLength) && IsBiaodianFuhao(s.GetAt(i)))
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
					if(IsBiaodianFuhao(s.GetAt(i+1)))//符号显示在上一行上
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
		if(index < 0)
			index = 0;
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
	DrawMultiLine(pdc->m_hDC, s, nPixel, nFontSize, rt, pVarry, LineCount, nAglin1);
}

void CMJPGStatic::DrawMultiLine(HDC hdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1)
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
			}
			break;
		}
	case right_top:
		{
			for(int i = 0; i < nShowLine; i++)
			{
				CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
				CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1, rt_.right, rt_.top+(i+1)*nPixel1);
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
			}
			break;
		}
		
	}
}

void CMJPGStatic::DrawUnit(HDC hdc, UNIT *pUnit, BOOL isSelected, CRect & rcPaint)
{
	ASSERT (pUnit);
	
// 	if (!pUnit->m_pUnitWnd)
// 		return;

	if (!pUnit->m_bDirty && !pUnit->m_bIsShow) // 不可见的非Dirty对象不需要重绘
		return;
		
	CRect rcTmp;
	BOOL bIntersect = rcTmp.IntersectRect(&rcPaint, &pUnit->m_Rect); //是否有无效区域以外的内容
	
	if (!pUnit->m_bDirty && !bIntersect) //非Dirty对象，且未处于无效区域内，则不需要重画。
		return;	

	CString sAllJpg = L"";
	CString sUintJpg = L"";
	//确定图片
	if(!pUnit->m_bIsShow)	// 不可见对象应绘制窗体背景
	{
		sUintJpg = L"";
		sAllJpg = m_currentMJPGList->bgfilename;
	}
	else if(isSelected || pUnit->m_bIsDownStatus)
	{
		sUintJpg = pUnit->m_bgFilename_down;
		sAllJpg = m_currentMJPGList->bgfilename_down;
	}
	else
	{
		sUintJpg = pUnit->m_bgFilename;
		sAllJpg = m_currentMJPGList->bgfilename;
	}
	
	//add 20100921
	if(pUnit->m_nSiererNO == m_nFocusUnit)
	{		
		DrawRectLine(NULL, pUnit->m_Rect);
	}
	//
	
	if(sUintJpg != "")	// 对象有自己的图片的话，绘制该图片
	{	
		HDC hdcUnit ;

		//add by qi 20100920
		//画PNG
		if (sUintJpg.Find(L".png") > 0 || sUintJpg.Find(L".PNG") > 0)
		{
			CxImage *image_;
			image_ = new CxImage();
			if(image_)
			{
				CString filename = ToFileDir(sUintJpg);
				bool ret = image_->Load(filename.GetBuffer(256), CXIMAGE_FORMAT_PNG);
				
				hdcUnit = CImgCache::GetHDC(m_currentMJPGList->bgfilename);
				::BitBlt(hdc, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), 
					hdcUnit, pUnit->m_Rect.left, pUnit->m_Rect.top, SRCCOPY);

				if (ret)
				{	
					image_->Draw(hdc, pUnit->m_Rect);
				}
				delete image_;
			}	
		}
		else
		{	
			UINT BmpWidth;
			UINT BmpHeight;

			HDC hdcUnit = CImgCache::GetHDC(sUintJpg, &BmpWidth, &BmpHeight);
			
			::BitBlt(hdc, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), 
 			hdcUnit, 0, 0, SRCCOPY);
			
			if (pUnit->m_Rect.Width() != BmpWidth 
				|| pUnit->m_Rect.Height() != BmpHeight)
			{
				if (pUnit->m_Rect.Width() > BmpWidth)
				{
					HDC hdcBg = CImgCache::GetHDC(m_currentMJPGList->bgfilename);
					::BitBlt(hdc, pUnit->m_Rect.left + BmpWidth, pUnit->m_Rect.top, 
						pUnit->m_Rect.Width() - BmpWidth, pUnit->m_Rect.Height(), 
						hdcBg, pUnit->m_Rect.left + BmpWidth, pUnit->m_Rect.top, SRCCOPY);
				}
				if (pUnit->m_Rect.Height() > BmpHeight)
				{
					HDC hdcBg = CImgCache::GetHDC(m_currentMJPGList->bgfilename);
					::BitBlt(hdc, pUnit->m_Rect.left, pUnit->m_Rect.top + BmpHeight, 
						pUnit->m_Rect.Width(), pUnit->m_Rect.Height() - BmpHeight, 
						hdcBg, pUnit->m_Rect.left, pUnit->m_Rect.top + BmpHeight, SRCCOPY);
				}
			}
		}

	}
	else 
	{
		CRect rc;
		rc.UnionRect(&pUnit->m_Rect, rcPaint);
		BOOL bAllIn = (rc == rcPaint);
		if (!bAllIn || sAllJpg != m_currentMJPGList->bgfilename) //无效区域内的默认背景已经被重画过
		{
			if (!sAllJpg.IsEmpty())
			{
				HDC hdcBg = CImgCache::GetHDC(sAllJpg);
				::BitBlt(hdc, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), 
					hdcBg, pUnit->m_Rect.left, pUnit->m_Rect.top, SRCCOPY);
			}
			else
			{
				HDC hdcBg = CImgCache::GetHDC(m_currentMJPGList->bgfilename);
				::BitBlt(hdc, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), 
					hdcBg, pUnit->m_Rect.left, pUnit->m_Rect.top, NOTSRCCOPY);
			}
		}
	}

	//画文字
	if(!pUnit->m_bIsShow || pUnit->m_sContent == "")
		return;

	CTextCacheKey key;
	key.BoxHeight = pUnit->m_Rect.Height();
	key.BoxWidth = pUnit->m_Rect.Width();
	key.FontColor = gFontColor[pUnit->m_FontColor];
	key.FontItalic = pUnit->m_bFontItalic;
	key.FontName = gFontName[pUnit->m_Font];
	key.FontSize = gFontSize[pUnit->m_FontSize];
	key.FontUnderLine = pUnit->m_bFontUnLine;
	key.FontWeights = gFontWeights[pUnit->m_FontWeights];
	key.FontHeight = gFontHeight[pUnit->m_FontHeight];
	
	key.FontLanguage = DEFAULT_CHARSET;
	TCHAR code = pUnit->m_sContent.GetAt(0);
	if((code >= 0x3130 && code <= 0x318f) || code >= 0xAC00 && code <= 0xD7A3)
		key.FontLanguage = HANGEUL_CHARSET;
	
	key.Text = pUnit->m_sContent;
	key.FontAlign = pUnit->m_FontAlign;

	// 首先去缓冲区中查找
	HDC hdcText = CTextCache::GetCachedHDC(key);
	if (hdcText)	
	{
		//找到缓冲对象
		::BitBlt(hdc, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(),
			hdcText, 0, 0, SRCAND);
	}
	else
	{
		// 尝试建立缓冲项
		hdcText = CTextCache::CreateEmptyHDC(key, hdc);
		if (hdcText)
		{
			// 创建成功,在缓冲dc中绘制
			CRect rc(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
			DrawUnitText(hdcText, rc, key);
			// 复制到目标dc
			::BitBlt(hdc, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(),
				hdcText, 0, 0, SRCAND);
		}
		else
		{
			// 现有缓冲策略禁止缓冲该项，直接绘制
			DrawUnitText(hdc, pUnit->m_Rect, key);
		}
	}

}
void CMJPGStatic::DrawUnitText(HDC hdc, CRect & rc, CTextCacheKey & key)
{
	CFont font;
	VERIFY(font.CreateFont(
		key.FontSize,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		key.FontWeights,//FW_NORMAL,       // nWeight
		key.FontItalic,                     // bItalic
		key.FontUnderLine,                     // bUnderline
		0,                         // cStrikeOut
		key.FontLanguage,                 // DEFAULT_CHARSET,//HANGUL_CHARSET,           // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		key.FontName));  
	
	HFONT hOldFont = (HFONT)::SelectObject(hdc, font.m_hObject);
	::SetTextColor(hdc, key.FontColor);
	
	MULTILINEINFO pVarry[100];
	int n = GetCStringMuliteLine(key.Text, key.FontSize, rc, &pVarry[0], 100);	
	DrawMultiLine(hdc, key.Text, key.FontSize + key.FontHeight, key.FontSize, rc, &pVarry[0], n, key.FontAlign);
	
	::SelectObject(hdc, hOldFont);
	font.DeleteObject();
}


extern void TransparentBlt2( HDC hdcDest,      // 目标DC
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


CString	CMJPGStatic::ToFileDir(CString filename)
{
	CString sDirName = "";
	if(filename == "" || filename.GetAt(0) != _T('.'))
		return filename;
	sDirName = "\\flashdrv";
	sDirName += filename.Mid(1);
	return sDirName;
}

/*
void CMJPGStatic ::CreateMemDC() 
{
	CDC *pdc = GetDC();
	m_pMemDC = new CDC();
	m_pBmp = new CBitmap(); 
	m_pMemDC->CreateCompatibleDC(pdc); 
	m_pBmp->CreateCompatibleBitmap(pdc, m_ClientRect.Width(), m_ClientRect.Height());   
	m_pOldbmp = m_pMemDC->SelectObject(m_pBmp);   
	ReleaseDC(pdc);
}

void CMJPGStatic ::ReleaseMemDC() 
{
	m_pMemDC->SelectObject(m_pOldbmp);
	m_pBmp->DeleteObject();
	delete m_pMemDC;
	delete m_pBmp;
}
*/

void CMJPGStatic ::CreateMemDC_HDC(CString sFilename) 
{
	//   20090723
	HDC pdc = ::GetDC(m_hWnd);
	m_hMemDC = ::CreateCompatibleDC(pdc); 
	m_plpBits = NULL;
	CFile   file;   
	if( file.Open(sFilename,  CFile::modeRead ) )   
	{     
		BITMAPFILEHEADER   bfhHeader;   
		file.Read(&bfhHeader,sizeof(BITMAPFILEHEADER));   
		if(bfhHeader.bfType ==((WORD)   ('M'<<8)|'B'))   
		{   		 
			if(bfhHeader.bfSize == file.GetLength())     
			{   
				UINT   uBmpInfoLen=(UINT)   bfhHeader.bfOffBits-sizeof(BITMAPFILEHEADER);   
				LPBITMAPINFO   lpBitmap= (LPBITMAPINFO)   new   BYTE[uBmpInfoLen];  
				
				file.Read((LPVOID)   lpBitmap,uBmpInfoLen);   
				if((* (LPDWORD)(lpBitmap))==sizeof(BITMAPINFOHEADER))     
				{   
					DWORD   dwBitlen=bfhHeader.bfSize   -   bfhHeader.bfOffBits;     
					lpBitmap->bmiHeader.biHeight = m_ClientRect.Height();
					lpBitmap->bmiHeader.biWidth = m_ClientRect.Width();
					
					m_hBmp = ::CreateDIBSection(pdc, lpBitmap, DIB_RGB_COLORS, &m_plpBits, NULL, 0);
					if(m_plpBits)
					{
						m_hOldBmp = (HBITMAP)::SelectObject(m_hMemDC, m_hBmp);   
						::ReleaseDC(m_hWnd, pdc);
						file.Close();
						delete []lpBitmap; 
						return;
					}
				}
				else
					delete []lpBitmap;    //20090216 test
			}
		}	
		file.Close();
	}
	
	m_hBmp = ::CreateCompatibleBitmap(pdc, m_ClientRect.Width(), m_ClientRect.Height());
	m_hOldBmp = (HBITMAP)::SelectObject(m_hMemDC, m_hBmp);   
	::ReleaseDC(m_hWnd, pdc);	
}

void CMJPGStatic ::ReleaseMemDC_HDC()  
{
	::SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBmp);
	DeleteDC(m_hMemDC);
}

void CMJPGStatic::SetCurrentLinkFile(CString s)
{
	//lxz 20091130
	if(ToFileDir(s) == m_sCurrentLinkFile)
		return;
	m_nIndexSelectUnit = -1;
	m_sCurrentLinkFile = ToFileDir(s);
	if(m_currentMJPGList)
		delete m_currentMJPGList;
	//	DMemprintf("MJPGPage 1");
	
//	DWORD dwStart = GetTickCount();
	m_currentMJPGList = ::Util::XmlParser::ParseFileToMJPGList_(m_sCurrentLinkFile);
//	DWORD offset = GetTickCount() - dwStart;   
//	TRACE(L"ParseFileToMJPGList_ %s:%d\n", s, offset);
	InitFocusUnit();

	if(m_currentMJPGList)
	{
		m_bDirty = FALSE;

		m_nIsCrollIndex = -1;
		m_nIsCrollCount = 0;
		for(int i = 0; i < m_currentMJPGList->items.size(); i++)
		{
			if(m_currentMJPGList->items[i]->unitparam.m_Ctrltype > ctrl_unit)
			{
				int nstyle = 0;
				if(m_currentMJPGList->items[i]->unitparam.m_bIsShow)
					nstyle = WS_VISIBLE;
				switch(m_currentMJPGList->items[i]->unitparam.m_Ctrltype)
				{
				case ctrl_edit:
					{
					
					}
					break;
				case ctrl_chechbutton:
					{
						/*
						CCEButtonST *pButton = new CCEButtonST();
						if(pButton)
						{
							pButton->Create(m_currentMJPGList->items[i]->unitparam.m_sContent, WS_CHILD|BS_CHECKBOX|nstyle, m_currentMJPGList->items[i]->unitparam.m_Rect, this, m_currentMJPGList->items[i]->unitparam.m_nSiererNO, 24);
							pButton->SetIcon(IDI_ICON_24CHECK1, CSize(24, 24), IDI_ICON_24CHECH0, CSize(24, 24));
							m_currentMJPGList->items[i]->unitparam.m_pUnitWnd = (CWnd *)pButton;
						}
						*/
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
			if(m_currentMJPGList->items[i]->unitparam.m_nIsScroll && m_currentMJPGList->items[i]->unitparam.m_bIsShow && m_nIsCrollIndex == -1)
			{
				m_nIsCrollIndex = m_currentMJPGList->items[i]->unitparam.m_nIsScroll;
			}
			if(m_currentMJPGList->items[i]->unitparam.m_nIsScroll)
				m_nIsCrollCount = m_currentMJPGList->items[i]->unitparam.m_nIsScroll;
		}

		if(m_nIsCrollIndex == 1)
		{
			SetUnitIsShow(1001, FALSE);
		}
		if(m_nIsCrollCount == 1)
		{
			SetUnitIsShow(1002, FALSE);
		}
	}
}

void CMJPGStatic::CacheSelf()
{
	ASSERT (m_currentMJPGList);

	if (!m_currentMJPGList->bgfilename.IsEmpty())
		CImgCache::GetHDC(m_currentMJPGList->bgfilename);
	if (!m_currentMJPGList->bgfilename_down.IsEmpty())
		CImgCache::GetHDC(m_currentMJPGList->bgfilename_down);

	for (int i = 0; i < m_currentMJPGList->items.size(); ++i)
	{
		MJPGItem * item = m_currentMJPGList->items[i];
		// 仅缓存普通状态的图，压下状态等用到再读取(一般都不大)。
		if (!item->unitparam.m_bgFilename.IsEmpty())
			CImgCache::GetHDC(item->unitparam.m_bgFilename);
	}	
}

//int upordown 0 up, 1 down.
void CMJPGStatic::ScrollUnit(int upordown, int unitcount)
{
	/*
	if(upordown == 0)  //上翻
	{
		if(m_nIsCrollIndex == 0)
			return;
		int max = m_nIsCrollIndex - unitcount;
		if(max < 0)
			max = 0;
		for(int i = max; i <= m_nIsCrollIndex-1; i++)
		{
			if(m_currentMJPGList->items[i]->unitparam.m_bIsScroll)
			{
				m_nIsCrollIndex = i;
				break;
			}
		}
	}
	else		//下翻
	{
		int max = m_nIsCrollIndex + unitcount;
		if(max > (m_currentMJPGList->items.size()-1))
			max = m_currentMJPGList->items.size()-1;
		if(max >= m_nIsCrollCount)
			return;
		for(int i = m_nIsCrollIndex+1; i <= max; i++)
		{
			if(!m_currentMJPGList->items[i]->unitparam.m_bIsScroll)
			{
				m_nIsCrollIndex = (i-1);
				break;
			}
			else
				m_nIsCrollIndex = i;
		}
	}
	for(int i = 0; i < m_currentMJPGList->items.size(); i++)
	{
		if(m_currentMJPGList->items[i]->unitparam.m_nIsScroll)
		{
			if(i >= m_nIsCrollIndex && i < (m_nIsCrollIndex+unitcount))
			{
				m_currentMJPGList->items[i]->unitparam.m_bIsShow = TRUE;
			
				if(m_currentMJPGList->items[i]->unitparam.m_pUnitWnd)
					m_currentMJPGList->items[i]->unitparam.m_pUnitWnd->ShowWindow(SW_SHOW);
			}
			else
			{
				m_currentMJPGList->items[i]->unitparam.m_bIsShow = FALSE;
				if(m_currentMJPGList->items[i]->unitparam.m_pUnitWnd)
					m_currentMJPGList->items[i]->unitparam.m_pUnitWnd->ShowWindow(SW_HIDE);
			}
		}
	}
	*/
	if(upordown == 0)  //上翻
	{
		if(m_nIsCrollIndex == 1)
			return;
		m_nIsCrollIndex--;

		if(m_nIsCrollIndex == 1)
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
		if(m_nIsCrollIndex == m_nIsCrollCount)
			return;
		m_nIsCrollIndex++;
		if(m_nIsCrollIndex == m_nIsCrollCount)
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
	for(int i = 0; i < m_currentMJPGList->items.size(); i++)
	{
		if(m_currentMJPGList->items[i]->unitparam.m_nIsScroll == m_nIsCrollIndex)
		{
			m_currentMJPGList->items[i]->unitparam.m_bIsShow = TRUE;
				
			if(m_currentMJPGList->items[i]->unitparam.m_pUnitWnd)
				m_currentMJPGList->items[i]->unitparam.m_pUnitWnd->ShowWindow(SW_SHOW);
		}
		else if(m_currentMJPGList->items[i]->unitparam.m_nIsScroll)
		{
			m_currentMJPGList->items[i]->unitparam.m_bIsShow = FALSE;
			if(m_currentMJPGList->items[i]->unitparam.m_pUnitWnd)
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

void CMJPGStatic::InvalidateItem(UINT nIndex)
{
	if (!IsWindowVisible())
		return;
	if (!m_bDirty)
	{
		// 这里的无效仅用于触发WM_PAINT事件，矩形区域设的很小以避免不必要的绘制
		CRect rc(m_currentMJPGList->items[nIndex]->unitparam.m_Rect);
		rc.left = rc.left + rc.Width() / 2;
		rc.top = rc.top + rc.Height() / 2;
		rc.right = rc.left + 1;
		rc.bottom = rc.top + 1;
		InvalidateRect(&rc);

		m_bDirty = TRUE;
	}
	m_currentMJPGList->items[nIndex]->unitparam.m_bDirty = TRUE;
}

int CMJPGStatic::GetIndexByUnitNo(int UnitNO)
{
	for(int i = 0; i < m_currentMJPGList->items.size(); i++)
		if(m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
			return i;
	//ASSERT (0); //没有找到指定序号的对象。

	return -1;
}
/////////////////////////////////////////////////////////////////////////////
// CMJPGStatic message handlers

void CMJPGStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	extern BOOL watchdog_MFC;
	watchdog_MFC = TRUE;   //设置watchdog生效

	// TODO: Add your message handler code here and/or call default
	int nIndex = FindUnitIndex(point);
	m_oldMJPGList = m_currentMJPGList;
	m_oldLinkFile = m_sCurrentLinkFile;
	if(nIndex >= 0)
	{
		
		int nOldFocus = m_nFocusUnit;
		
		int nNo = m_currentMJPGList->items[nIndex]->unitparam.m_nSiererNO; 
		SetClickUnit(nNo);
				
		if(nOldFocus != m_nFocusUnit)
		{
			m_nFocusUnit = -1;
	//		InvalidateRect(GetUnitRect(nOldFocus));
	//		::Sleep(10);		
		}
		else
		{
			m_nFocusUnit = nOldFocus;
		}
				
		if(m_currentMJPGList->items[nIndex]->unitparam.m_bIsDisable)   //不响应消息
			return;

		m_nIndexSelectUnit = nIndex;
		//InvertRect(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Rect);
		if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Ctrltype == ctrl_unittoggle)
		{
		//	if(!m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_bIsDownStatus)
			{
		//      change by qi 201009
		//	    InvalidateItem(nIndex);
		//		DrawUnitStatus_HDC(&m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam, 1);
				
				if(GetParent())
				{
					if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_click) //click
					{
						CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
						if(sDial != "")
						{	
							//add 20100921
							m_nIndexSelectUnit = -1;

							std::string str = Util::StringOp::FromCString(sDial);
							CString s_ = str.c_str();
							int code = atoi(s_);
							GetParent()->PostMessage(WM_MJPGTOGGLE, (WPARAM)code, GetDlgCtrlID());
						}
					}
				}
			}
		}
		else
		InvalidateItem(nIndex);
		//	DrawUnitStatus_HDC(&m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam, 1);
		
		if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_click)
		{
			CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
			if(sDial != "")
			{	
				std::string str = Util::StringOp::FromCString(sDial);
				CString s_ = str.c_str();
				int code = atoi(s_);
				GetParent()->PostMessage(WM_CLICKMJPG_TOAPP1, (WPARAM)code, GetDlgCtrlID());
			}
		}
		/*
		
		if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_link) //link
		{
			CString sLink = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
			if(sLink != "")
			{
				sLink = ToFileDir(sLink);
				SetCurrentLinkFile(sLink);
				DrawMJPGPage(sLink);
				m_nIndexSelectUnit = -1;
			}
		//	m_nIndexSelectUnit = -1;
		}
		else if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_click) //link
		{
			CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
			if(sDial != "")
			{
				std::string str = Util::StringOp::FromCString(sDial);
				int code = atoi(str.c_str());
				if(code == 2)
				{
					Dprintf("test\n");
				}
				GetParent()->PostMessage(WM_CLICKMJPG_TOAPP, (WPARAM)code, GetDlgCtrlID());
				return;
			}
		}
	//	m_nIndexSelectUnit = -1;
	*/

	}
	else 
	{
		m_nIndexSelectUnit = -1;
	}
	CStatic::OnLButtonDown(nFlags, point);
}

void CMJPGStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_nIndexSelectUnit >= 0)
	{
		if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Ctrltype == ctrl_unittoggle)
		{
			CStatic::OnMouseMove(nFlags, point);
		}

		if(IsWindowVisible())
			if(GetParent()->IsWindowVisible())
			{
				//CRect rt = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Rect;
				if(!m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Rect.PtInRect(point))
				{
					InvalidateItem(m_nIndexSelectUnit);
	//				DrawUnitStatus_HDC(&m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam, 0);
					if(GetParent())
						GetParent()->SendMessage(WM_MJPGMOVE, m_nIndexSelectUnit, 0);
					m_nIndexSelectUnit = -1;
				}
			}
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CMJPGStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	Dprintf("test up %\n", m_nIndexSelectUnit);
	
	if(m_oldLinkFile != m_sCurrentLinkFile)
	{
		m_nIndexSelectUnit = -1;
		return;
	}

	if(m_nIndexSelectUnit >= 0)
	{	
		int nNo = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_nSiererNO; 
		int nOldFocus = m_nFocusUnit;
		SetClickUnit(nNo);
		//InvertRect(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Rect);
		if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_Ctrltype == ctrl_unittoggle)
		{
			CStatic::OnLButtonUp(nFlags, point);
			return;
		}
		
		if(IsWindowVisible() && GetParent()->IsWindowVisible())
		InvalidateItem(m_nIndexSelectUnit);

		//lxz 20100701 remove
		/*
		if(IsWindowVisible())
		if(IsWindowVisible() && GetParent()->IsWindowVisible())
			{
				if(nOldFocus != m_nFocusUnit)
				{
					Invalidate();
				}
				else
					DrawUnitStatus_HDC(&m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam, 0);
			}
		*/	

		if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_link) //link
		{
			CString sLink = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
			if(sLink != "")
			{
				sLink = ToFileDir(sLink);
				SetCurrentLinkFile(sLink);
				Invalidate();
			//	DrawMJPGPage_HDC(sLink);
			}
			m_nIndexSelectUnit = -1;
		}
		else if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_click) //click
		{
			CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
			if(sDial != "")
			{
				std::string str = Util::StringOp::FromCString(sDial);
				CString s_ = str.c_str();
				int code = atoi(s_);
				if(code == 2)
				{
					Dprintf("test\n");
				}
				GetParent()->PostMessage(WM_CLICKMJPG_TOAPP, (WPARAM)code, GetDlgCtrlID());
			}
		}
		else if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_pageup)		//上翻
		{
			CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
			if(sDial != "")
			{
				std::string str = Util::StringOp::FromCString(sDial);
				CString s_ = str.c_str();
				int code = atoi(s_);
				if(code>0)
					ScrollUnit(0, code);
			}
		}
		
		else if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == uint_pagedown)	//下翻
		{
			CString sDial = m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent;
			if(sDial != "")
			{
				std::string str = Util::StringOp::FromCString(sDial);
				CString s_ = str.c_str();
				int code = atoi(s_);
				if(code>0)
					ScrollUnit(1, code);
			}
		}
		else if(m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitType == unit_outdial)
		{
			//软件拨号
			
// 			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
// 			main->phone_->HungOn();
// 			Sleep(1000);
// 			char telcode[128];
// 			memset(telcode, 0, 128);
// 			wcstombs(telcode, (LPTSTR)(LPCTSTR)m_currentMJPGList->items[m_nIndexSelectUnit]->unitparam.m_UnitContent/*.GetBuffer(64)*/, 64);
// 			Dprintf("Dial %s\n ", telcode);
				
		}

		m_nIndexSelectUnit = -1;
	}
	// LButtonUp后很可能有耗时的后续事件发生，不在这里立刻更新的话，按钮可能一直保持压下的状态。
	UpdateWindow();
	CStatic::OnLButtonUp(nFlags, point);
}

int CMJPGStatic::FindUnitIndex(CPoint pt)
{
	int ret = -1;
	
	if(m_currentMJPGList)
	{
		for(int i = 0; i < m_currentMJPGList->items.size(); i++)
		{
			if(m_currentMJPGList->items[i]->unitparam.m_Rect.PtInRect(pt) && m_currentMJPGList->items[i]->unitparam.m_bIsShow)   //可见
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
	::InvertRect(hdc, &rt);
}

void CMJPGStatic::OnPaint() 
{
//	DMemprintf("MJPGStatic0 ");
    ASSERT (m_currentMJPGList);
	
	RECT rc;
	// 获取窗口被无效的区域
	GetUpdateRect(&rc);
	CRect rcPaint(rc);
// 	
	CDC * pDC = GetDC();


	CRect rt1 = m_currentMJPGList->m_Rect;
	SetMJPGRect(rt1);
	
	// 获取后台缓冲
	HDC hBackBuffer = CImgCache::GetBackBuffer();

	// 首先将无效区域的背景重绘至后台缓冲
	//L"\\Flashdrv\\adv\\mjpg\\k5\\common\\png\\3g_status.bmp.bmp"
	HDC hdcBg = CImgCache::GetHDC(m_currentMJPGList->bgfilename);
	//HDC hdcBg = CImgCache::GetHDC(L"\\Flashdrv\\adv\\mjpg\\k5\\common\\png\\主界面1.bmp")
	ASSERT(hdcBg);
	::BitBlt(hBackBuffer, rcPaint.left, rcPaint.top, rcPaint.Width(), rcPaint.Height(), hdcBg, rcPaint.left, rcPaint.top, SRCCOPY);

	int i;
	// 更新不可见对象至后台缓冲。
	for (i = 0; i < m_currentMJPGList->items.size(); ++i)
	{
		MJPGItem * item = m_currentMJPGList->items[i];
		if (!item->unitparam.m_bIsShow)
			DrawUnit(hBackBuffer, &item->unitparam, (m_nIndexSelectUnit == i), rcPaint);
	}
	// 更新可见对象至后台缓冲。
	for (i = 0; i < m_currentMJPGList->items.size(); ++i)
	{
		MJPGItem * item = m_currentMJPGList->items[i];
		if (item->unitparam.m_bIsShow)
			DrawUnit(hBackBuffer, &item->unitparam, (m_nIndexSelectUnit == i), rcPaint);
	}
	// 首先将无效区域由后台缓冲绘制到前台
	::BitBlt(pDC->m_hDC, rcPaint.left, rcPaint.top, rcPaint.Width(), rcPaint.Height(), hBackBuffer, rcPaint.left, rcPaint.top, SRCCOPY);

	// 逐个元素绘制到前台
	for (i = 0; i < m_currentMJPGList->items.size(); ++i)
	{
		MJPGItem *item = m_currentMJPGList->items[i];
		
		CRect rcUnit;
		//仅重画无效区域之外的Dirty对象
		if (item->unitparam.m_bDirty)
		{
			rcUnit.UnionRect(&rcPaint, &item->unitparam.m_Rect);
			if (rcUnit != rcPaint) // 一个对象有无效区域以外内容的话整体重画
			{
				rcUnit = item->unitparam.m_Rect;
				::BitBlt(pDC->m_hDC, rcUnit.left, rcUnit.top, rcUnit.Width(), rcUnit.Height(), hBackBuffer, rcUnit.left, rcUnit.top, SRCCOPY);
			}
			item->unitparam.m_bDirty = FALSE;
		}
	}
	m_bDirty = FALSE;

	ReleaseDC(pDC);
	ValidateRect(NULL);
}

BOOL CMJPGStatic::GetUnitIsDownStatus(int UnitNO)
{
	if(!m_currentMJPGList)
		return FALSE;

	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;
		
	return m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus;

}
CRect CMJPGStatic::GetUnitRect(int UnitNO)
{
	if(!m_currentMJPGList)
		return CRect(0, 0, 0, 0);
	for(int i = 0; i < m_currentMJPGList->items.size(); i++)
	{
		if(m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
		{
			return m_currentMJPGList->items[i]->unitparam.m_Rect;
		}
	}
	return CRect(0, 0, 0, 0);;
}

BOOL CMJPGStatic::SetUnitIsDownStatus(int UnitNO, BOOL isDown)
{
	if(!m_currentMJPGList)
		return FALSE;

	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;

	if (m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus != isDown)
	{
		InvalidateItem(i);
		m_currentMJPGList->items[i]->unitparam.m_bIsDownStatus = isDown;
	}
	return TRUE;
}

BOOL CMJPGStatic:: SetUnitIsShowNOResh(int UnitNO, BOOL isShow)
{
	if(!m_currentMJPGList)
		return FALSE;

	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;

	if (m_currentMJPGList->items[i]->unitparam.m_bIsShow != isShow)
	{
		InvalidateItem(i);
		m_currentMJPGList->items[i]->unitparam.m_bIsShow = isShow;
	}
	return TRUE;
}

BOOL CMJPGStatic::SetUnitIsShow(int UnitNO, BOOL isShow, BOOL isInvalidate)
{
	if(!m_currentMJPGList)
		return FALSE;
	
	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;

	if (m_currentMJPGList->items[i]->unitparam.m_bIsShow != isShow)
	{
		InvalidateItem(i);
		m_currentMJPGList->items[i]->unitparam.m_bIsShow = isShow;
	}
	return TRUE;
}


BOOL CMJPGStatic::SetUnitFont(int UnitNO, TEXTSIZE nFont)
{
	if(!m_currentMJPGList)
		return FALSE;
	for(int i = 0; i < m_currentMJPGList->items.size(); i++)
	{
		if(m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
		{
			m_currentMJPGList->items[i]->unitparam.m_FontSize = nFont;		
			return TRUE;
		}
	}
	return FALSE;
}

CString CMJPGStatic::GetUnitText(int UnitNO)
{
	CString s = "";
	if(!m_currentMJPGList)
		return s;
	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return s;

	return m_currentMJPGList->items[i]->unitparam.m_sContent;
}

BOOL CMJPGStatic:: GetUnitIsShow(int UnitNO)
{
	if(!m_currentMJPGList)
		return FALSE;
	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;

	return m_currentMJPGList->items[i]->unitparam.m_bIsShow;
}


BOOL CMJPGStatic::SetUnitBitmap(int UnitNO, CString filename_up, CString filename_down, BOOL isDraw, BOOL isInvalidate)
{
	if(!m_currentMJPGList)
		return FALSE;
	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;

	if (m_currentMJPGList->items[i]->unitparam.m_bgFilename != filename_up
		|| m_currentMJPGList->items[i]->unitparam.m_bgFilename_down != filename_down)
	{
		InvalidateItem(i);
		m_currentMJPGList->items[i]->unitparam.m_bgFilename = filename_up;
		m_currentMJPGList->items[i]->unitparam.m_bgFilename_down = filename_down;
	}
	return TRUE;
}

BOOL CMJPGStatic::GetUnitIsDisable(int UnitNO)
{
	if(!m_currentMJPGList)
		return TRUE;
	for(int i = 0; i < m_currentMJPGList->items.size(); i++)
	{
		if(m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
		{
			return m_currentMJPGList->items[i]->unitparam.m_bIsDisable;
		}
	}
	return TRUE;
}

BOOL CMJPGStatic::SetUnitIsDisable(int UnitNO, BOOL isDisable)
{
	if(!m_currentMJPGList)
		return FALSE;
	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;
	if (m_currentMJPGList->items[i]->unitparam.m_bIsDisable != isDisable)
	{
		InvalidateItem(i);
		m_currentMJPGList->items[i]->unitparam.m_bIsDisable = isDisable;
	}
	return TRUE;
}

BOOL CMJPGStatic::SetUnitIsTranslate(int UnitNO, BOOL isTrans)
{
	if(!m_currentMJPGList)
		return FALSE;
	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;

	if (m_currentMJPGList->items[i]->unitparam.m_bIsTranslate != isTrans)
	{
		InvalidateItem(i);
		m_currentMJPGList->items[i]->unitparam.m_bIsTranslate = isTrans;
	}
	return TRUE;
}

BOOL CMJPGStatic::SetUnitText(int UnitNO, CString text, BOOL isDraw, BOOL isInvalidate)
{
	if(!m_currentMJPGList)
		return FALSE;
	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;
	if (m_currentMJPGList->items[i]->unitparam.m_sContent != text)
	{
		InvalidateItem(i);
		m_currentMJPGList->items[i]->unitparam.m_sContent = text;
	}
	return TRUE;
}

BOOL CMJPGStatic::SetUnitRect(int UnitNO, CRect rt)
{
	if(!m_currentMJPGList)
		return FALSE;
	for (int i=0; i<m_currentMJPGList->items.size(); i++)
	{
		if(m_currentMJPGList->items[i]->unitparam.m_nSiererNO == UnitNO)
		{
			m_currentMJPGList->items[i]->unitparam.m_Rect = rt;
			InvalidateRect(m_currentMJPGList->items[i]->unitparam.m_Rect);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMJPGStatic::SetUnitColor(int UnitNO, TEXTCOLOR cl, BOOL isDraw, BOOL isInvalidate)
{
	if(!m_currentMJPGList)
		return FALSE;
	int i = GetIndexByUnitNo(UnitNO);
	if (i < 0)
		return FALSE;

	if (m_currentMJPGList->items[i]->unitparam.m_FontColor != cl)
	{
		InvalidateItem(i);
		m_currentMJPGList->items[i]->unitparam.m_FontColor = cl;
	}
	return TRUE;
}

void CMJPGStatic::DrawMultiLine_HDC(HDC hdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1)
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_CENTER|DT_VCENTER);
			}
			break;
		}
	case right_top:
		{
			for(int i = 0; i < nShowLine; i++)
			{
				CString s1 = AdjustEnglishWordShow(s, pVarry, i, nShowLine); //s.Mid(pVarry[i].nStart, pVarry[i].nCount);
				CRect rt1 = CRect(rt_.left, rt_.top+i*nPixel1, rt_.right, rt_.top+(i+1)*nPixel1);
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_RIGHT|DT_VCENTER);
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
				::DrawText(hdc, s1, s1.GetLength(), &rt1, DT_SINGLELINE|DT_WORDBREAK|DT_LEFT|DT_VCENTER);
			}
			break;
		}
		
	}
}
static int gnCount = 0;
void CMJPGStatic::DrawRectLine(HDC hdc, CRect rt)
{
	::SetCursorPos(rt.left+rt.Width()/2, rt.top+rt.Height()/2 + 57);
	return;

	int nCount = 1;// gnCount%2;
// 	if(gnCount > 10)
// 	{
// 		KillTimer(10086);
// 		nCount = 1;
// 		gnCount = 0;
// 	}
	if(nCount > 0)
	{
		CPen pen(PS_SOLID, 1, RGB(3, 230, 226));
		HGDIOBJ oldpen = ::SelectObject(hdc, pen.m_hObject);
		POINT pt[4];
		POINT pt1;
		CRect rt_ = rt;
		int type = 1;
	
		for(int i = 0; i < 5; i++)
		{
			if(i != 2)
				continue;
			if(type)
			{
				if(i == 2)
				{
				//	CPen pen(PS_SOLID, 2, RGB(1, 231, 228));
					CPen pen(PS_SOLID, 2, RGB(172, 172, 172));
					::SelectObject(hdc, pen.m_hObject);
				}
				else if(i == 1 || i == 3)
				{
				//	CPen pen(PS_SOLID, 2, RGB(17, 180, 177));
					CPen pen(PS_SOLID, 2, RGB(172, 172, 172));
					::SelectObject(hdc, pen.m_hObject);
				}
				else
				{
				//	CPen pen(PS_SOLID, 2, RGB(44, 109, 111));
					CPen pen(PS_SOLID, 2, RGB(172, 172, 172));
					::SelectObject(hdc, pen.m_hObject);
				}
			}
			else
			{
				if(i == 2)
				{
					CPen pen(PS_SOLID, 2, RGB(255, 3, 5));
					::SelectObject(hdc, pen.m_hObject);
				}
				else if(i == 1 || i == 3)
				{
					CPen pen(PS_SOLID, 2, RGB(205, 5, 7));
					::SelectObject(hdc, pen.m_hObject);
				}
				else
				{
					CPen pen(PS_SOLID, 2, RGB(159, 3, 5));
					::SelectObject(hdc, pen.m_hObject);
				}
			}
			
			
			rt_.DeflateRect(1, 1);
			
			/*
			pt[0].x = rt_.left;
			pt[0].y = rt_.top;
			::MoveToEx(hdc, pt[0].x, pt[0].y, &pt1);
			pt[1].x = rt_.left;
			pt[1].y = rt_.bottom;
			::LineTo(hdc, pt[1].x, pt[1].y);
		
			pt[2].x = rt_.right;
			pt[2].y = rt_.bottom;
			::LineTo(hdc, pt[2].x, pt[2].y);
		
			pt[3].x = rt_.right;
			pt[3].y = rt_.top;
			::LineTo(hdc, pt[3].x, pt[3].y);
		
			::LineTo(hdc, pt[0].x, pt[0].y);
			*/
			pt[0].x = rt_.left;
			pt[0].y = rt_.bottom;
			::MoveToEx(hdc, pt[0].x, pt[0].y, &pt1);
			pt[1].x = rt_.right;
			pt[1].y = rt_.bottom;
			::LineTo(hdc, pt[1].x, pt[1].y);
		}
	
		::SelectObject(hdc, &oldpen);
	}
	gnCount++;
}

/*
void DrawRectLine(HDC hdc, CRect rt)
{
	CPen pen(PS_SOLID, 1, RGB(3, 230, 226));
	HGDIOBJ oldpen = ::SelectObject(hdc, pen.m_hObject);
	POINT pt[4];
	POINT pt1;
	CRect rt_ = rt;
	int type = 1;
	for(int i = 0; i < 5; i++)
	{
		if(type)
		{
			if(i == 2)
			{
				CPen pen(PS_SOLID, 2, RGB(1, 231, 228));
				::SelectObject(hdc, pen.m_hObject);
			}
			else if(i == 1 || i == 3)
			{
				CPen pen(PS_SOLID, 2, RGB(17, 180, 177));
				::SelectObject(hdc, pen.m_hObject);
			}
			else
			{
				CPen pen(PS_SOLID, 2, RGB(44, 109, 111));
				::SelectObject(hdc, pen.m_hObject);
			}
		}
		else
		{
			if(i == 2)
			{
				CPen pen(PS_SOLID, 2, RGB(255, 3, 5));
				::SelectObject(hdc, pen.m_hObject);
			}
			else if(i == 1 || i == 3)
			{
				CPen pen(PS_SOLID, 2, RGB(205, 5, 7));
				::SelectObject(hdc, pen.m_hObject);
			}
			else
			{
				CPen pen(PS_SOLID, 2, RGB(159, 3, 5));
				::SelectObject(hdc, pen.m_hObject);
			}
		}
		

		rt_.DeflateRect(1, 1);
		pt[0].x = rt_.left;
		pt[0].y = rt_.top;
		::MoveToEx(hdc, pt[0].x, pt[0].y, &pt1);
		pt[1].x = rt_.left;
		pt[1].y = rt_.bottom;
		::LineTo(hdc, pt[1].x, pt[1].y);
		pt[2].x = rt_.right;
		pt[2].y = rt_.bottom;
		::LineTo(hdc, pt[2].x, pt[2].y);
		pt[3].x = rt_.right;
		pt[3].y = rt_.top;
		::LineTo(hdc, pt[3].x, pt[3].y);
		::LineTo(hdc, pt[0].x, pt[0].y);	
	}
	::SelectObject(hdc, &oldpen);
}
*/

//int nUintStatus  0  up  1 down
void CMJPGStatic::DrawUnitStatus_HDC(UNIT *pUnit, int nUintStatus)
{
	if(!pUnit)
		return;

	if(!pUnit->m_bIsShow)
		return;


	//贴背景图
	CString sAllJpg = "";
	CString sUintJpg = "";
	if(pUnit->m_bIsDownStatus)
	{
		sUintJpg = pUnit->m_bgFilename_down;
		sAllJpg = m_currentMJPGList->bgfilename_down;
	}
	else
	{
		if(nUintStatus == 0)   //up
		{
			sUintJpg = pUnit->m_bgFilename;
			sAllJpg = m_currentMJPGList->bgfilename;
		}
		else if(nUintStatus == 1) //down
		{
			sUintJpg = pUnit->m_bgFilename_down;
			sAllJpg = m_currentMJPGList->bgfilename_down;
		}
	}
	
	if(sUintJpg != "" || sAllJpg != "")
	{
		HDC pdc = ::GetDC(m_hWnd);
		HDC pMemDC1 = ::CreateCompatibleDC(pdc); 
		HBITMAP pBmp1 = ::CreateCompatibleBitmap(pdc, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
		HBITMAP pOldbmp1 = (HBITMAP)SelectObject(pMemDC1, pBmp1);

		::SetBkMode(pMemDC1, TRANSPARENT);
		if(sUintJpg != "")
		{
			//为了支持透明色，先拷贝背景 李新志20090514
			if(sUintJpg.Find(L".png") > 0 || sUintJpg.Find(L".PNG") > 0)
			{
				if(sAllJpg == "")
					sAllJpg = m_currentMJPGList->bgfilename;
				if(sAllJpg != "")
					DrawImage_HDC(ToFileDir(sAllJpg), pMemDC1, CRect(pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.right, pUnit->m_Rect.bottom), TRUE);
			}
			
			//小图
			DrawImage_HDC(ToFileDir(sUintJpg), pMemDC1, CRect(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height()));

		}
		else if(sAllJpg != "")
		{
			//test lxz 20080703
			if(m_currentMJPGList->bgfilename_down.Find(L".bmp") != -1 || m_currentMJPGList->bgfilename_down.Find(L".BMP") != -1)
				DrawImage_HDC(ToFileDir(sAllJpg), pMemDC1, pUnit->m_Rect, TRUE);
			else
			{
				HDC pMemDC = ::CreateCompatibleDC(pdc); 
				HBITMAP pBmp = ::CreateCompatibleBitmap(pdc, m_ClientRect.Width(), m_ClientRect.Height());
				HBITMAP pOldbmp = (HBITMAP)SelectObject(pMemDC, pBmp);
				DrawImage_HDC(ToFileDir(sAllJpg), pMemDC, m_ClientRect);
				::BitBlt(pMemDC1, 0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), pMemDC, pUnit->m_Rect.left, pUnit->m_Rect.top, SRCCOPY);
				::SelectObject(pMemDC, pOldbmp);
				::DeleteObject(pBmp);
				::DeleteDC(pMemDC);
			}
		}
		else
		{
			InvertRect_HDC(pMemDC1, CRect(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height()));
		}
		
		//画文字
		if(pUnit->m_nSiererNO == m_nFocusUnit)
		{
//			CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
//			HGDIOBJ oldpen = ::SelectObject(pMemDC1, pen.m_hObject);

// 			POINT pt[4];
// 			POINT pt1;
// 			CRect rt_ = pUnit->m_Rect;
// 			rt_.DeflateRect(1, 1);
// 			pt[0].x = rt_.left;
// 			pt[0].y = rt_.top;
// 			::MoveToEx(pMemDC1, pt[0].x, pt[0].y, &pt1);
// 			pt[1].x = rt_.left;
// 			pt[1].y = rt_.bottom;
// 			::LineTo(pMemDC1, pt[1].x, pt[1].y);
// 			pt[2].x = rt_.right;
// 			pt[2].y = rt_.bottom;
// 			::LineTo(pMemDC1, pt[2].x, pt[2].y);
// 			pt[3].x = rt_.right;
// 			pt[3].y = rt_.top;
// 			::LineTo(pMemDC1, pt[3].x, pt[3].y);
// 			::LineTo(pMemDC1, pt[0].x, pt[0].y);

//			::SelectObject(pMemDC1, &oldpen);
			
			if(!nUintStatus)
			{
				CRect rt(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
				DrawRectLine( pMemDC1, rt);
			}		
		}
		if(pUnit->m_sContent != "")
		{
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
			
			HFONT pFont = (HFONT)SelectObject(pMemDC1, m_Font.m_hObject);
			::SetTextColor(pMemDC1, gFontColor[pUnit->m_FontColor]);
			
			MULTILINEINFO pVarry[100];
			CRect rt = CRect(0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height());
			int n = GetCStringMuliteLine(pUnit->m_sContent, gFontSize[pUnit->m_FontSize], rt, &pVarry[0], 100);	
			DrawMultiLine_HDC(pMemDC1, pUnit->m_sContent, gFontSize[pUnit->m_FontSize]+gFontHeight[pUnit->m_FontHeight], gFontSize[pUnit->m_FontSize], rt, &pVarry[0], n, pUnit->m_FontAlign);
			
			::SelectObject(pMemDC1, pFont);
			m_Font.DeleteObject();
		}

		::BitBlt(pdc, pUnit->m_Rect.left, pUnit->m_Rect.top, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), pMemDC1, 0, 0, SRCCOPY);
		::SelectObject(pMemDC1, pOldbmp1);
		::DeleteObject(pBmp1);
		::DeleteDC(pMemDC1);
		::ReleaseDC(m_hWnd, pdc);

	}
	else
	{
		HDC pdc = ::GetDC(m_hWnd);
		InvertRect_HDC(pdc, pUnit->m_Rect);

		if(pUnit->m_nSiererNO == m_nFocusUnit)
		{
// 			CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
// 			HGDIOBJ oldpen = ::SelectObject(pdc, pen.m_hObject);
// 			POINT pt[4];
// 			POINT pt1;
// 			CRect rt_ = pUnit->m_Rect;
// 			rt_.DeflateRect(1, 1);
// 			pt[0].x = rt_.left;
// 			pt[0].y = rt_.top;
// 			::MoveToEx(pdc, pt[0].x, pt[0].y, &pt1);
// 			pt[1].x = rt_.left;
// 			pt[1].y = rt_.bottom;
// 			::LineTo(pdc, pt[1].x, pt[1].y);
// 			pt[2].x = rt_.right;
// 			pt[2].y = rt_.bottom;
// 			::LineTo(pdc, pt[2].x, pt[2].y);
// 			pt[3].x = rt_.right;
// 			pt[3].y = rt_.top;
// 			::LineTo(pdc, pt[3].x, pt[3].y);
// 			::LineTo(pdc, pt[0].x, pt[0].y);
// 			::SelectObject(pdc, &oldpen);

			DrawRectLine( pdc, pUnit->m_Rect);
		}

		if(pUnit->m_sContent != "")
		{
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
			
			HFONT pFont = (HFONT)::SelectObject(pdc, m_Font.m_hObject);
			::SetTextColor(pdc, gFontColor[pUnit->m_FontColor]);
			
			MULTILINEINFO pVarry[100];
			int n = GetCStringMuliteLine(pUnit->m_sContent, gFontSize[pUnit->m_FontSize], pUnit->m_Rect, &pVarry[0], 100);	
			DrawMultiLine_HDC(pdc, pUnit->m_sContent, gFontSize[pUnit->m_FontSize]+gFontHeight[pUnit->m_FontHeight], gFontSize[pUnit->m_FontSize], pUnit->m_Rect, &pVarry[0], n, pUnit->m_FontAlign);
			
			::SelectObject(pdc, pFont);

			m_Font.DeleteObject();
		}
		::ReleaseDC(m_hWnd, pdc);
	}
//	DMemprintf("DrawUnitStatus 1 ");
}

void CMJPGStatic::DrawGroup_HDC(int *nNO, int nCount, BOOL isShow)
{
	CPoint p1 = CPoint(800, 480);
	CPoint p2 = CPoint(0, 0);

	for(int i = 0; i < nCount; i++)
	{
		int nIndex = SetUnitIsShow(nNO[i], isShow, FALSE);
		if(nIndex >= 0)
		{
			CPoint p1_ = CPoint(m_currentMJPGList->items[nIndex]->unitparam.m_Rect.left, m_currentMJPGList->items[nIndex]->unitparam.m_Rect.top);
			CPoint p2_ = CPoint(m_currentMJPGList->items[nIndex]->unitparam.m_Rect.right, m_currentMJPGList->items[nIndex]->unitparam.m_Rect.bottom);
			if(p1.x>p1_.x)
				p1.x = p1_.x;
			if(p1.y>p1_.y)
				p1.y = p1_.y;
			if(p2.x<p2_.x)
				p2.x = p2_.x;
			if(p2.y<p2_.y)
				p2.y = p2_.y;
		}
	}
	CRect rt = CRect(p1.x, p1.y, p2.x, p2.y);
	InvalidateRect(rt);
}

void CMJPGStatic::DrawUnit_HDC(UNIT *pUnit, HDC pdc)
{
	if(!pUnit)
		return;

	if(!pUnit->m_bIsShow)
		return;
	if(pUnit->m_pUnitWnd)
		return;

	::SetBkMode(pdc, TRANSPARENT);
	
	//贴背景图
	if(!pUnit->m_bIsDownStatus)
	{
		if(pUnit->m_bgFilename != L"")
		{
			//if(!pUnit->m_bIsTranslate)
			DrawImage_HDC(ToFileDir(pUnit->m_bgFilename), pdc, pUnit->m_Rect);
		}
	}
	else
	{
		if(pUnit->m_bgFilename_down != "")
		{
			//if(!pUnit->m_bIsTranslate)
			DrawImage_HDC(ToFileDir(pUnit->m_bgFilename_down), pdc, pUnit->m_Rect);
		}
		else if(m_currentMJPGList->bgfilename_down != "")
		{
			//test lxz 20080703
			//去扣图
			if(m_currentMJPGList->bgfilename_down.Find(L".bmp") != -1 || m_currentMJPGList->bgfilename_down.Find(L".BMP") != -1)
				DrawImage_HDC(ToFileDir(m_currentMJPGList->bgfilename_down), pdc, pUnit->m_Rect);
			else
			{
				HDC pMemDC = ::CreateCompatibleDC(pdc); 
				HBITMAP pBmp = ::CreateCompatibleBitmap(pdc, m_ClientRect.Width(), m_ClientRect.Height());
				HBITMAP pOldbmp = (HBITMAP)SelectObject(pMemDC, pBmp);
				DrawImage_HDC(ToFileDir(m_currentMJPGList->bgfilename_down), pMemDC, m_ClientRect);
				::BitBlt(pdc, 0, 0, pUnit->m_Rect.Width(), pUnit->m_Rect.Height(), pMemDC, pUnit->m_Rect.left, pUnit->m_Rect.top, SRCCOPY);
				::SelectObject(pMemDC, pOldbmp);
				::DeleteObject(pBmp);
				::DeleteDC(pMemDC);
			}
		}
	}

	if(pUnit->m_nSiererNO == m_nFocusUnit)
	{
// 		CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
// 		HGDIOBJ oldpen = ::SelectObject(pdc, pen.m_hObject);
// 		POINT pt[4];
// 		POINT pt1;
// 		CRect rt_ = pUnit->m_Rect;
// 		rt_.DeflateRect(1, 1);
// 		pt[0].x = rt_.left;
// 		pt[0].y = rt_.top;
// 		::MoveToEx(pdc, pt[0].x, pt[0].y, &pt1);
// 		pt[1].x = rt_.left;
// 		pt[1].y = rt_.bottom;
// 		::LineTo(pdc, pt[1].x, pt[1].y);
// 		pt[2].x = rt_.right;
// 		pt[2].y = rt_.bottom;
// 		::LineTo(pdc, pt[2].x, pt[2].y);
// 		pt[3].x = rt_.right;
// 		pt[3].y = rt_.top;
// 		::LineTo(pdc, pt[3].x, pt[3].y);
// 		::LineTo(pdc, pt[0].x, pt[0].y);
// 		::SelectObject(pdc, &oldpen);
  		DrawRectLine( pdc, pUnit->m_Rect);
	}

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
	
	HFONT pFont = (HFONT)::SelectObject(pdc, m_Font.m_hObject);
	::SetTextColor(pdc, gFontColor[pUnit->m_FontColor]);

	MULTILINEINFO pVarry[100];
	CRect rt = pUnit->m_Rect;
	int n = GetCStringMuliteLine(pUnit->m_sContent, gFontSize[pUnit->m_FontSize], rt, &pVarry[0], 100);	
	DrawMultiLine_HDC(pdc, pUnit->m_sContent, gFontSize[pUnit->m_FontSize]+gFontHeight[pUnit->m_FontHeight], gFontSize[pUnit->m_FontSize], rt, &pVarry[0], n, pUnit->m_FontAlign);
	
	::SelectObject(pdc, pFont);
	m_Font.DeleteObject();
}

void CMJPGStatic::DrawMJPGPage_HDC(CString sFile)
{
	//	int ret = DMemprintf("MJPGPage 0");

	if(m_currentMJPGList == NULL)
	{
		Dprintf("m_currentMJPGList == NULL\n");
	//	CString s = ".\\adv\\mjpg\\main\\中文\\m1.xml";
	//	DrawMJPGPage(ToFileDir(s));
		return;
	}
	//	DMemprintf("MJPGPage 2");
	/*
	if(sFile.Find(L"\\main\\中文\\m1.xml") != -1)
		GetParent()->SendMessage(WM_MJPGSHOWHALF, 1);
	else
		GetParent()->SendMessage(WM_MJPGSHOWHALF, 0);
		*/
//	DWORD  s = GetTickCount(); 
	
	CRect rt1 = m_currentMJPGList->m_Rect;
	SetMJPGRect(rt1);
//	MoveWindow(m_rtMJPG, FALSE);
	m_ClientRect = CRect(0, 0, rt1.Width(), rt1.Height());
	CreateMemDC_HDC(ToFileDir(m_currentMJPGList->bgfilename));
	HDC pdc = ::GetDC(m_hWnd);
	
	if(m_currentMJPGList)
	{
#if 1
		m_sCurrentLinkFile = sFile;
		//移到该窗口类中
		/*
		CDC memDC;
		CBitmap bmp;
		memDC.CreateCompatibleDC(pdc); 
		bmp.CreateCompatibleBitmap(pdc, m_ClientRect.Width(), m_ClientRect.Height());   
		CBitmap *oldbmp = memDC.SelectObject(&bmp);   
		*/
		
		if(m_currentMJPGList->bgfilename != "")
		{
			DrawImage_HDC(ToFileDir(m_currentMJPGList->bgfilename), m_hMemDC, m_ClientRect, FALSE, m_plpBits);
		}
		else
		{
			CBrush bBr = RGB(255, 255, 255); 
			::FillRect(m_hMemDC, &m_ClientRect, (HBRUSH)bBr.m_hObject);
		}
				
		int size_ = m_currentMJPGList->items.size();
		for (int i = 0; i < size_; ++i)
		//for (int i = size_-1; i >= 0; --i)
		{
 			MJPGItem *item = m_currentMJPGList->items[i];
 			DrawUnit_HDC(&item->unitparam, m_hMemDC);
		}

		::BitBlt(pdc, m_ClientRect.left, m_ClientRect.top, m_ClientRect.Width(), m_ClientRect.Height(), m_hMemDC, 0, 0, SRCCOPY);
		
		//移到全局中
		// 		memDC.SelectObject(&oldbmp);
		// 		memDC.DeleteDC();   
		// 		bmp.DeleteObject();
#else
		m_sCurrentLinkFile = sFile;
		DrawImage_HDC(ToFileDir(m_currentMJPGList->bgfilename), pdc, m_ClientRect);
		//TRACE(L"BG %d\n", offset);
		
		//int size_ = m_currentMJPGList->items.size();
		for (int i = 0; i < m_currentMJPGList->items.size(); ++i)
		//for (int i = size_-1; i >= 0; --i)
		{
			MJPGItem *item = m_currentMJPGList->items[i];
			DrawUnit_HDC(&item->unitparam, pdc);
			//TRACE(L"Unit %d\n", offset);
		}
#endif
	}
	/*
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
	::ReleaseDC(m_hWnd, pdc);
	ReleaseMemDC_HDC();
	
	/*
	if(m_currentMJPGList)
	{
		delete m_currentMJPGList;
		m_currentMJPGList = NULL;
	}
	*/
} 

BOOL CMJPGStatic::InitFocusUnit()
{
	m_nFocusUnit = -1;

	//lxz 20100701 remove
	//return TRUE;

	m_nRowFocusUnit = -1;
	m_nColFocusUnit = -1;
	if(m_currentMJPGList)
	{
		CString s = m_currentMJPGList->m_sTab;
		if(!s.IsEmpty())
		{
			int nStart = s.Find(L"(", 0);
			int nEnd;
			if(nStart >= 0)
			{
				nEnd = s.Find(L",", 0);
				if(nEnd)
				{
					CString sRow = s.Mid(nStart+1, nEnd-nStart-1);
					m_nRowFocusUnit = Util::StringOp::ToInt(sRow);
					m_nFocusUnit = m_nRowFocusUnit;
				}
			}
			nStart = s.Find(L"[", 0);
			if(nStart >= 0)
			{
				nEnd = s.Find(L",", nStart);
				if(nEnd)
				{
					CString sCol = s.Mid(nStart+1, nEnd-nStart-1);
					m_nColFocusUnit = Util::StringOp::ToInt(sCol);
				}
			}
		}
	}
	return TRUE;
}

int CMJPGStatic::FindRowFocusUnit(FOCUSDIRECT direct)
{
	int ret = -1;
	if(m_nRowFocusUnit != -1)
	{
		CString s = m_currentMJPGList->m_sTab;
		if(m_currentMJPGList)
		{
			if(!s.IsEmpty())
			{
				int nStart = s.Find(L"(", 0);
				int nEnd = s.Find(L")", nStart);
				int nCount = 0;
				int Id[100];
				CString sIndex = "";
				for(int i = nStart+1; i <= nEnd; i++)
				{
					CString sTemp = s.Mid(i, 1);	
					if(sTemp == L"," || sTemp == L")")
					{
						int no = Util::StringOp::ToInt(sIndex,10);
						if(GetUnitIsShow(no) && (!GetUnitIsDisable(no)))
							Id[nCount++] = no;
						sIndex = "";
					}
					else if(sTemp != L"(" && sTemp != L" ")
					{
						sIndex += sTemp;
					}
				}
				for(i = 0; i < nCount; i++)
				{
					if(Id[i] == m_nRowFocusUnit)
						break;
				}
// 				if(m_SetUnitNo > 0)  //wangzhenxing20100603
// 				{
// 					if(m_SetUnitNo <= nCount)
// 					{
// 						ret = Id[m_SetUnitNo-1];
// 						m_SetUnitNo = -1;
// 					}
// 				}
// 				else
				{
					if(i == nCount)
						ret = Id[0];
					else
					{
						if(direct == TOLEFT)  //向左
						{
							if(i == 0)
								i = nCount-1;
							else
								i--;
							ret = Id[i];
						}
						else				//向右
						{
							i++;
							if(i >= nCount)
								i = 0;
							ret = Id[i];
						}
					}
				}
			}
		}
	}
	return ret;
}

int CMJPGStatic::FindColFocusUnit(FOCUSDIRECT direct)
{
	int ret = -1;
	if(m_nColFocusUnit != -1)
	{
		CString s = m_currentMJPGList->m_sTab;
		if(m_currentMJPGList)
		{
			if(!s.IsEmpty())
			{
				int nStart = s.Find(L"[", 0);
				int nEnd = s.GetLength();
				int nCount = 0;
				int rCount = 0;
				int Id[20][100];
				memset(&Id[0][0], 0xFF, sizeof(int)*(2000));
				CString sIndex = "";
				for(int i = nStart+1; i <= nEnd; i++)
				{
					CString sTemp = s.Mid(i, 1);	
					if(sTemp == L"," || sTemp == L"]")
					{
						int no = Util::StringOp::ToInt(sIndex, 10);
						if(GetUnitIsShow(no) && (!GetUnitIsDisable(no)))
							Id[rCount][nCount++] = no;
						sIndex = "";
						if(sTemp == L"]")
						{
							nCount = 0;
							rCount++;
						}
					}
					else if(sTemp != L"[" && sTemp != L" ")
					{
						sIndex += sTemp;
					}
				}
				for(int j = 0; j < rCount; j++)
					for(i = 0; i < 100; i++)
					{
						if(Id[j][i] == m_nColFocusUnit)
							goto TAB;
						else if(Id[j][i] == -1)
							break;
					}
TAB:	    	if(j == rCount)
					ret = Id[0][0];
				else
				{
				    nCount = 0;
					for(int j_ = 0; j_ < 100; j_++)
					{
						if(Id[j][j_] != -1)
							nCount++;
						else
							break;
					}
					if(direct == TOUP)  //向上
					{
						if(i == 0)
							return -2;
						else
							i--;
						ret = Id[j][i];
					}
					else				//向下
					{
						i++;
						if(i >= nCount)
							return -3;
						i = i%nCount;
						ret = Id[j][i];
					}
				}
			}
		}
	}
	return ret;
}

BOOL CMJPGStatic::SetClickUnit(int noUnit)
{
	//lxz 20100701 remove
	//m_nFocusUnit = -1;
	//return TRUE;
	
	CString s = Util::StringOp::FromInt(noUnit).c_str();
	CString s_ = m_currentMJPGList->m_sTab;
	int nStart = s_.Find(L"[");
	if(nStart >= 0)
	{
		int nStart_;
		int nStart1_;
		CString s1 = s  + ",";
		CString s2 = s  + "]";
	//	nStart_ = s_.Find(s1, nStart);
	//	nStart1_ = s_.Find(s2, nStart);
		nStart_ = s_.Find(s1, nStart);
		if(nStart_ > 0)
		{
			TCHAR c= s_.GetAt(nStart_-1);
			if(c != L'[' && c != L',')
				nStart_ = -1;
		}
		nStart1_ = s_.Find(s2, nStart);
		if(nStart1_ > 0)
		{
			TCHAR c= s_.GetAt(nStart1_-1);
			if(c != L'[' && c != L',')
				nStart1_ = -1;
			}
		if(nStart_>=0 || nStart1_>=0)
		{
			m_nRowFocusUnit = noUnit;
			m_nFocusUnit = noUnit;
			return TRUE;
		}
	}


		nStart = s_.Find(L"(");
		int nEnd = s_.Find(L")");
		if(nStart >= 0)
		{
			int nStart_;
			int nStart1_;
			CString s1 = s  + ",";
			CString s2 = s  + ")";
			nStart_ = s_.Find(s1, nStart);
			if(nStart_ > 0)
			{
				TCHAR c= s_.GetAt(nStart_-1);
				if(c != L'(' && c != L',')
					nStart_ = -1;
			}
			nStart1_ = s_.Find(s2, nStart);
			if(nStart1_ > 0)
			{
				TCHAR c= s_.GetAt(nStart1_-1);
				if(c != L'(' && c != L',')
					nStart1_ = -1;
			}
			if((nStart_>=0 && (nStart_ < nEnd)) ||(nStart1_>=0 && (nStart1_ < nEnd)))
			{
				m_nColFocusUnit = noUnit;
				m_nFocusUnit = noUnit;
				return TRUE;
			}
		}
		return TRUE;
}

BOOL CMJPGStatic::SetFocusUnit(FOCUSDIRECT direct, int noUnit)
{
//	SetTimer(10086, 500, NULL);

	SetTimer(10086, 400, NULL);
	::SetCursor(::LoadCursorW(NULL, IDC_WAIT));

	//lxz 20100701 remove
	//m_nFocusUnit = -1;
	//return TRUE;

	gnCount = 0;
	CString s = Util::StringOp::FromInt(noUnit).c_str();
	if(direct == TOLEFT || direct == TORIGHT)
	{
		m_nRowFocusUnit = noUnit; 
		CString s_ = m_currentMJPGList->m_sTab;
		int nStart = s_.Find(L"[");
		if(nStart >= 0)
		{
			int nStart_;
			int nStart1_;
			CString s1 = s  + ",";
			CString s2 = s  + "]";
		//	nStart_ = s_.Find(s1, nStart);
		//	nStart1_ = s_.Find(s2, nStart);
			nStart_ = s_.Find(s1, nStart);
			if(nStart_ > 0)
			{
				TCHAR c= s_.GetAt(nStart_-1);
				if(c != L'[' && c != L',')
					nStart_ = -1;
			}
			nStart1_ = s_.Find(s2, nStart);
			if(nStart1_ > 0)
			{
				TCHAR c= s_.GetAt(nStart1_-1);
				if(c != L'[' && c != L',')
					nStart1_ = -1;
			}
			if(nStart_ || nStart1_)
				m_nColFocusUnit = m_nRowFocusUnit;
		}
	}
	else if(direct == TOUP || direct == TODOWN)
	{
		m_nColFocusUnit = noUnit; 
		CString s_ = m_currentMJPGList->m_sTab;
		int nStart = s_.Find(L"(");
		int nEnd = s_.Find(L")");
		if(nStart >= 0)
		{
			int nStart_;
			int nStart1_;
			CString s1 = s  + ",";
			CString s2 = s  + ")";
			//	nStart_ = s_.Find(s1, nStart);
			//	nStart1_ = s_.Find(s2, nStart);
			nStart_ = s_.Find(s1, nStart);
			if(nStart_ > 0)
			{
				TCHAR c= s_.GetAt(nStart_-1);
				if(c != L'(' && c != L',')
					nStart_ = -1;
			}
			nStart1_ = s_.Find(s2, nStart);
			if(nStart1_ > 0)
			{
				TCHAR c= s_.GetAt(nStart1_-1);
				if(c != L'(' && c != L',')
					nStart1_ = -1;
			}
			if((nStart_>=0 && (nStart_ < nEnd)) ||(nStart1_>=0 && (nStart1_ < nEnd)))
				m_nRowFocusUnit = m_nColFocusUnit;
		}
	}
	m_nFocusUnit = noUnit;
	return TRUE;
}

void CMJPGStatic::InsertAdressCharacter(CString &ac)
{	
	int size = ac.GetLength();
 	for (int i = 0 ; i < size ;i++)
 	{
		if (ac.GetAt(i) == '&')
		{
			ac.Insert(i,'&');
			i +=1;
			size +=1;
		}
 	}
}