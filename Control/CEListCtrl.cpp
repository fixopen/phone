// CEListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CEListCtrl.h"
#include "../Data/SkinStyle.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCEListCtrl

CCEListCtrl::CCEListCtrl()
{
	m_RGB1 = Data::g_listctrlBackRGB1[Data::g_skinstyle];
	m_RGB2 = Data::g_listctrlBackRGB2[Data::g_skinstyle];
	m_nLineHeight = 20 ;
	m_nFontHeight = 20 ;
	m_nPage = 0;
}

CCEListCtrl::~CCEListCtrl()
{

}

void CCEListCtrl ::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
/*	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);    
	LVITEM lvi = {0}; 
	lvi.mask = LVIF_STATE;//|LVIF_IMAGE; 
	lvi.stateMask = LVIS_FOCUSED | LVIS_SELECTED ; 
	lvi.iItem = lpDrawItemStruct->itemID; 
	BOOL bGet = GetItem(&lvi);
	
	//高亮显示
	
	BOOL bHighlight =((lvi.state & LVIS_DROPHILITED)||((lvi.state & LVIS_SELECTED) && 
		((GetFocus() == this)|| (GetStyle() & LVS_SHOWSELALWAYS))));
	
	// 画文本背景 
	// 每行的颜色
	CRect rcBack = lpDrawItemStruct->rcItem; 
	pDC->SetBkMode(TRANSPARENT); 
	if( bHighlight ) //如果被选中
	{ 
		//pDC->SetTextColor(RGB(255,255,255)); //文本为白色
		pDC->FillRect(rcBack, &CBrush(RGB(0,0,255))); 
	} 
	else 
	{ 
		//pDC->SetTextColor(RGB(0,0,0));    
		if (lpDrawItemStruct->itemID%2) //奇数
		{
			pDC->FillRect(rcBack, &CBrush(m_RGB1)); 
	
		}
		else
		{
			pDC->FillRect(rcBack, &CBrush(m_RGB2)); 
		}
	} 

	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE) 
	{ 
		//写文本 
		CString szText; 
		int nCollumn = GetHeaderCtrl()->GetItemCount();//列数
		for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++) 
		{ 
			//循环得到文本 
			CRect rcItem; 
			if ( !GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem )) 
				continue; 
			
			//画图标
			LVITEM lv = {0}; 
			lv.iItem = lpDrawItemStruct->itemID;
			lv.mask  = LVIF_IMAGE|LVIF_STATE;
			GetItem(&lv);
			
			//获得图标信息
			int nTop ; 
			IMAGEINFO info;
			CImageList*  pImageList = GetImageList(LVSIL_SMALL);
			pImageList->GetImageInfo(lv.iImage,&info);
			CRect cr1 = lpDrawItemStruct->rcItem ; //行高
			CRect cr2 = info.rcImage;
			nTop =  lpDrawItemStruct->rcItem.top + (cr1.Height() - cr2.Height())/2; 
			pImageList->Draw(pDC,lv.iImage,\
				CPoint(0,nTop),LVSIL_SMALL);
			// 
			
			//画文本
			szText = GetItemText( lpDrawItemStruct->itemID, i );
			
			rcItem.right -= 1; 
			pDC->DrawText(szText, lstrlen(szText), &rcItem, DT_LEFT    
				
				| DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
			
		} 
	}
   */
}
void CCEListCtrl ::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//类型安全转换   
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);   
    *pResult = 0;   
       
    //指定列表项绘制前后发送消息   
    if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)   
    {   
        *pResult = CDRF_NOTIFYITEMDRAW;   
    }   
    else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)   
    {   
		///////////失去焦点选中行还保持以前颜色///////////
		int    nItem = static_cast <int>( pLVCD->nmcd.dwItemSpec ); 
		
		BOOL bSelect = FALSE; 
		POSITION pos = GetFirstSelectedItemPosition(); 
		POSITION pos1 = pos;
		while(pos) 
		{ 
			int index = GetNextSelectedItem(pos); 
			if(index==nItem) 
			{ 
				bSelect = TRUE; 
				break; 
			}
			if(index == -1)
			{
				if(nItem == 0)
					bSelect = TRUE;
			}
		}

//		modify by yy 20111028
// 		if(pos1 == 0)
// 		{
// 			if(nItem == 0)
// 				bSelect = TRUE;
// 		}

		if(bSelect) 
		{ 
			pLVCD->clrTextBk = RGB(0,0,255); //Set the text to red 
		} 
		///////////////////////////////end//////////////
		
		else
		{
			//奇数行   
			if(pLVCD->nmcd.dwItemSpec % 2)   
				pLVCD->clrTextBk = m_RGB1;   
			//偶数行   
			else  
            pLVCD->clrTextBk = m_RGB2; 

		}
     
        //继续   
        *pResult = CDRF_DODEFAULT;   
    
	}   

}


void CCEListCtrl ::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void CCEListCtrl::SetListColor(COLORREF RGB1, COLORREF RGB2)
{
	m_RGB1 = RGB1;
	m_RGB2 = RGB2;
}

void CCEListCtrl::SetScrollPos(int pos, BOOL redraw)
{
	m_scollbar_.SetPos(pos, redraw);
}

void CCEListCtrl::SetScrollRagle(BOOL redraw)
{
	if (m_bIsSelfScroll)
	{
		int nMax = this->GetItemCount();
		m_scollbar_.SetRangle(0, nMax, m_nPage);
	}
	SetScrollPos(0, redraw);
	if(redraw)
	{
		m_scollbar_.ShowWindow(SW_SHOW);	
	}
}


BOOL CCEListCtrl ::Create(DWORD dwStyle, CRect &rect, CWnd *pParentWnd, UINT nID, BOOL bIsScroll, int Scroll_style, BOOL isSelfScroll)
{

	m_bIsScroll = bIsScroll;
	m_bIsSelfScroll = isSelfScroll;
	int  nStyle = dwStyle | LVS_SINGLESEL;
	if(!m_bIsScroll)
	{
		nStyle |= LVS_NOSCROLL;
	}

	BOOL ret = CListCtrl::Create(nStyle, rect, pParentWnd, nID);
	
	VERIFY(m_Font.CreateFont(
	   m_nFontHeight,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   ANSI_CHARSET,              // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   _T("新宋体")));            // lpszFacename
    SetFont(&m_Font);

	//设置滚动条
	if(m_bIsScroll)
	{
		m_nPage = rect.Height()/m_nLineHeight;

		CRect rt ;
		GetWindowRect(&rt);
		pParentWnd->ScreenToClient(&rt);

		rt.left = rt.right-SCROLL_ARROW_WIDTH+1;
		m_scollbar_.Create(L"", WS_CHILD|WS_VISIBLE|WS_GROUP, rt, pParentWnd, 0);
		if (isSelfScroll)
		{
			m_scollbar_.SetParam(Scroll_style, 0, 0, m_nPage, this);
		}
		else
		{
			m_scollbar_.SetParam(Scroll_style, 0, 0, m_nPage, pParentWnd);
		}

		::SetWindowPos(m_scollbar_.m_hWnd, HWND_TOPMOST, rt.left, rt.top, rt.Width(), rt.Height(), 0);
	}
	return ret;
}

void CCEListCtrl::OnClicked() 
{
	// TODO: Add your control notification handler code here
	if(m_bIsScroll)
	{
		if (m_bIsSelfScroll)
		{
			int n = GetScrollPos(SB_VERT);
			m_scollbar_.SetPos(n);
		}
	}

	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		m_nListIndex = GetNextSelectedItem (pos);
	}
}

BOOL CCEListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
		if(pMsg->message == WM_LBUTTONDOWN)
		{
			m_nListIndex = -1;
			BOOL ret =	CListCtrl::PreTranslateMessage(pMsg);
			
			POSITION pos = GetFirstSelectedItemPosition();
			if (pos != NULL)
			{
				int index = GetNextSelectedItem (pos);
				if(index >= 0 )//&& m_nListIndex != index)
				{
					GetParent()->PostMessage(WM_LISTCTRL_CLICK, GetDlgCtrlID(), 0);
				}
			}
			else
				SetTimer(1, 50, NULL);
			return ret;
		}
		
		if(pMsg->message == WM_LBUTTONUP)
		{
		}
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CCEListCtrl::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == IDT_CLICKLIST_TIMER)
	{
		KillTimer(IDT_CLICKLIST_TIMER);
		int n = GetScrollPos(SB_VERT);
		m_scollbar_.SetPos(n);
	}
	else if(nIDEvent == 1)
	{
		KillTimer(1);
		GetParent()->PostMessage(WM_LISTCTRL_CLICK, GetDlgCtrlID(), 0);
	}
}

BEGIN_MESSAGE_MAP(CCEListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCEListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_MEASUREITEM_REFLECT()
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
		ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
		ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
		ON_WM_TIMER()
		ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCEListCtrl ::ShowWindow_(int nCmdShow)
{
	ShowWindow(nCmdShow);
	m_scollbar_.ShowWindow(nCmdShow);
}

LRESULT CCEListCtrl ::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret =  CWnd::WindowProc(message, wParam, lParam);

	if(message == WM_LBUTTONDOWN)
	{
		if(m_bIsScroll)
		{
			if (m_bIsSelfScroll)
			{
				SetTimer(IDT_CLICKLIST_TIMER, 100, NULL);
			}
		}
	}
	return ret;
}

void CCEListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	// TODO: Add your control notification handler code here
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		m_nListIndex = GetNextSelectedItem (pos);
	}
	*pResult = 0;
}

void  CCEListCtrl::SetItemHeight() 
{ 		
	if (GetSafeHwnd()) 
	{ 
		CRect   rc; 
		GetWindowRect(   &rc   );	
		WINDOWPOS   wp; 
		wp.hwnd   =   m_hWnd; 
		wp.cx   =   rc.Width(); 
		wp.cy   =   rc.Height(); 
		wp.flags   =   SWP_NOACTIVATE   |   SWP_NOMOVE   | \
			SWP_NOOWNERZORDER   |   SWP_NOZORDER; 
		SendMessage(WM_WINDOWPOSCHANGED,   0,   (LPARAM)&wp); 
	} 
}

void CCEListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if(m_nLineHeight > 0)
		lpMeasureItemStruct-> itemHeight   =   m_nLineHeight; 
}


/////////////////////////////////////////////////////////////////////////////
// CCEListCtrl message handlers
