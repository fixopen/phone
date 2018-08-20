// CEListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CEListCtrl.h"
#include "../Data/SkinStyle.h"
#include "../resource.h"

#include "./MultimediaPhone.h"
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
	m_nLineHeight = 34;
	m_nPage = 0;
}

CCEListCtrl::~CCEListCtrl()
{
}

void CCEListCtrl ::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

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
	    //奇数行   
        if(pLVCD->nmcd.dwItemSpec % 2)   
            pLVCD->clrTextBk = m_RGB1;   
        //偶数行   
        else  
            pLVCD->clrTextBk = m_RGB2;   
 
// 		POSITION pos = GetFirstSelectedItemPosition();
// 		if (pos != NULL)
// 		{
// 			int index = GetNextSelectedItem (pos);
// 			if(index   ==  pLVCD->nmcd.dwItemSpec)     //当重画的项为选中项时，重设文字和背景色 
// 			{ 
// 				pLVCD->clrText = RGB(255, 0, 0);
// 			}
// 		}

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

//
// 	EnsureVisible(pos,   TRUE);
// 	SetExtendedStyle(   LVS_EX_FULLROWSELECT   );     //设置高亮显示属性   
// 	SetFocus();     //set   list   focus   
//  	SetItemState(pos,   LVIS_SELECTED,   LVIS_SELECTED);   //选中行
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
		//m_scollbar_.Invalidate();
	}
}

BOOL CCEListCtrl ::Create(DWORD dwStyle, CRect &rect, CWnd *pParentWnd, UINT nID, BOOL bIsScroll, int Scroll_style, BOOL isSelfScroll)
{

	m_bIsScroll = bIsScroll;
	m_bIsSelfScroll = isSelfScroll;
	int  nStyle = dwStyle | LVS_SHOWSELALWAYS;
	if(!m_bIsScroll)
	{
		nStyle |= LVS_NOSCROLL;
	}

	BOOL ret = CListCtrl::Create(nStyle, rect, pParentWnd, nID);
	
	VERIFY(m_font.CreateFont(
	   m_nLineHeight,                        // nHeight
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
	   _T("新宋体")));                 // lpszFacename
    SetFont(&m_font);

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

		::SetWindowPos(m_scollbar_.m_hWnd, HWND_TOP, rt.left, rt.top, rt.Width(), rt.Height(), SWP_NOACTIVATE);
	}
	else
	{
		m_scollbar_.Create(L"", WS_CHILD|WS_GROUP, CRect(0, 0, 0, 0), pParentWnd, 0);
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
}
/*

BOOL CCEListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	BOOL ret = CListCtrl::PreTranslateMessage(pMsg);
	if(pMsg->message == WM_LBUTTONDOWN)   
	{    
		if(m_bIsScroll)
		{
		//	SetTimer(IDT_CLICKLIST_TIMER, 100, NULL);
		}
	}
	return ret;
}
*/

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
	CListCtrl::OnTimer(nIDEvent);
}

BEGIN_MESSAGE_MAP(CCEListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CCEListCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
		ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
		ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
		ON_WM_TIMER()
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
	else if(message == WM_SCROLL_EDO)
	{
		if(wParam == SB_THUMBPOSITION)
		{
			int nPos = GetScrollPos(SB_VERT); 
			int nPage;
			int nY;
			lParam -= 1;
			if(nPos < lParam)
			{
				
				nPage = (lParam - nPos)/m_nPage;
				nY = (lParam - nPos)%m_nPage;
				for(int i = 0; i < nPage; i++)
					SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0 );
				for(i = 0; i < nY; i++)
					SendMessage(WM_VSCROLL, SB_LINEDOWN, 0 );
			}
			else
			{
				nPage = (nPos -lParam)/m_nPage;
				nY = (nPos - lParam)%m_nPage;
				for(int i = 0; i < nPage; i++)
					SendMessage(WM_VSCROLL, SB_PAGEUP, 0 );
				for(i = 0; i < nY; i++)
					SendMessage(WM_VSCROLL, SB_LINEUP, 0 );
			}
		}
		else if(wParam == SB_PAGEUP)
		{
			SendMessage(WM_VSCROLL, SB_PAGEUP, 0 );
		}
		else if(wParam == SB_PAGEDOWN)
		{
			SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0 );
		}
		else if(wParam == SB_LINEUP)
		{
			SendMessage(WM_VSCROLL, SB_LINEUP, 0 );
		}
		else if(wParam == SB_LINEDOWN)
		{
			SendMessage(WM_VSCROLL, SB_LINEDOWN, 0 );
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

BOOL CCEListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_LBUTTONDOWN)
	{
		Dprintf("ListCtrl pendown\r\n");
		theApp.GetMainWnd()->SendMessage(WM_NOPENUP, 0, 0);
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
		Dprintf("ListCtrl penup\r\n");
	}
	return CListCtrl::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////////////////////////////////////////
// CCEListCtrl message handlers
