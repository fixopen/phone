/******************************************************************************/
/*                                                                            */
/*  ��Ȩ����(c)  2001�����Ƽ�                                                 */
/*                                                                            */
/******************************************************************************/
/*  �� �� ��                                                                  */
/*             window.cpp                                                     */
/*                                                                            */
/*  �� �� ��                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  ��    ��                                                                  */
/*             �����Ƽ�E-PhoneII��������Դ�ļ�                              */
/*             HanWang Technology E-PhoneII Function Class Library source file*/
/*                                                                            */
/*  ��    ��                                                                  */
/*             CWnd�ඨ��                                                     */
/*             CCtrl�ඨ��                                                    */
/*             CFrameWnd�ඨ��                                                */
/*             CButton�ඨ��                                                  */
/*             CIconButton�ඨ��                                              */
/*             CDialog�ඨ��                                                  */
/*             CPages�ඨ�壨�ʻԣ�                                           */
/*             CRadioButton�ඨ�壨����ᣩ                                   */
/*             CAppWnd�ඨ��                                                  */
/*                                                                            */
/*  ���˵��                                                                  */
/*             �μ���E201���������ơ��ĵ�                                   */
/*                                                                            */
/*  ��    ��                                                                  */
/*             ����                                                           */
/*                                                                            */
/*  ��    ��                                                                  */
/*             2001-06-15                                                     */
/*                                                                            */
/*  ��ʷ��¼                                                                  */
/*             2001-08-01  ����CFrameWnd������Ϣ��������WM_DACTIVE          */
/*                         ��WM_ACITVE��Ϣ����                                */
/*                                                                            */
/*             2001-08-07  ����CIconButton��Createʱ��һ��BUG                 */
/*                                                                            */
/*             2001-08-23  ����CCheckButton������������                       */
/*                         CIconButton�����޸�ͼ��ӿ�                        */
/*                         �޸�CButton��OnPaint����                           */
/*                                                                            */
/*             2001-08-24  �޸�CDialog��DoModal����                           */
/*                                                                            */
/*             2001-09-24  CFrameWnd������OnRtc�麯���ӿ�                     */
/*                         �޸�CWnd�Ĺ��캯��                                 */
/*                                                                            */
/*             2001-09-26  CAppWnd������m_nWndIcon��Ա                        */
/*                         CAppWnd������Create����                            */
/*                         CAppWnd������GetWndIcon��SetWndIcon����            */
/*             2001-10-31  �޸�CMenuButton::OnPaint����������ᣩ             */
/*             2001-10-31  �޸� CAppWnd::PaintTitle() (byw)                   */
/*             2001-10-31  ���� CDialog::Create (�����)                      */
/*             2001-11-19  �޸� CPages::OnPaint���� (�����)                  */
/*             2001-12-07  ygr, modify DoSystemKeys()                         */
/*			   2001-12-24  hqf, ��ӽ�����������							  */
/*                                                                            */
/*		byw, 2001/12/26                                                       */
/*      modify DoSystemKeys(...)                                              */
/*                                                                            */
/******************************************************************************/


#include "window.h"
#include "datadrv.h"
#include "pswd.h"
#include "ring.h"
/*============================================================================*/
//  ��Ҫ�õ���ȫ�ֺ�������                                                    //
/*============================================================================*/

////////////////////////////////////////////////////////////////////////////////
//����:
//            ɾ��ָ������
extern BOOL DeleteWindow(CFrameWnd *pWnd, BOOL bRedrawBack=TRUE);

////////////////////////////////////////////////////////////////////////////////
//����:
//            ��ʾָ������
extern void ShowWindow(CFrameWnd * pwnd);

////////////////////////////////////////////////////////////////////////////////
//����:
//            ����ָ������
extern void HideWindow(CFrameWnd * pwnd);

////////////////////////////////////////////////////////////////////////////////
//����:
//            ��õ�ǰ�����
extern CFrameWnd * GetCurWindow();

////////////////////////////////////////////////////////////////////////////////////
int    g_iScreenStatus = SS_NORMAL;         // ��ǰ�Ƿ���Ļ����״̬

////////////////////////////////////////////////////////////////////////////////////
int     g_iCurDialogApp = -1;                 // ��ǰ�Ի������Ǹ�Ӧ����
////////////////////////////////////////////////////////////////////////////////
//���õ�ǰDialog���ĸ�Ӧ����
int SetCurrentDialogApp(int iCurDialogApp)
{
	int   iPrevApp = g_iCurDialogApp;
	g_iCurDialogApp = iCurDialogApp;
	return iPrevApp;
}

int GetCurrentDialogApp()
{
	return g_iCurDialogApp;
}
/*============================================================================*/
//  CWnd�ຯ������                                                            //
/*============================================================================*/

////////////////////////////////////////////////////////////////////////////////
//
CWnd::CWnd() 
{
	memset(m_strName, 0, WND_TITLE_LENGTH);
	m_pChild  = m_pParent =  m_pNextWnd = m_pPrevWnd = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//
CWnd:: ~CWnd()
{
}

////////////////////////////////////////////////////////////////////////////////
//
void  CWnd::SendMessage(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	WindowProcess(nMessage, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////
//
void  CWnd::SetWindowText(const char * strText)
{
  if(strText)
   hwstrncpy(m_strName, strText, WND_TITLE_LENGTH -1);
  else
   m_strName[0]=0; 
}

////////////////////////////////////////////////////////////////////////////////
//
void  CWnd::GetWindowText(char * pTextBuf, UINT16 nMaxCount) const
{
	hwstrncpy(pTextBuf, m_strName, nMaxCount-1);
	*(pTextBuf + nMaxCount - 1) = '\0';
}

////////////////////////////////////////////////////////////////////////////////
//
UINT32 CWnd::GetWindowStyle() const
{
	return m_dwStyle;
}

////////////////////////////////////////////////////////////////////////////////
//
void  CWnd::SetWindowStyle(const UINT32 dwStyle)
{
	m_dwStyle = dwStyle;
}

////////////////////////////////////////////////////////////////////////////////
//
CRect CWnd::GetWindowRect() const
{
   return m_sRect;
}

////////////////////////////////////////////////////////////////////////////////
//
CWnd * CWnd::GetNextWnd() const
{
	return m_pNextWnd;
}

////////////////////////////////////////////////////////////////////////////////
//
void CWnd::Paint()
{
  if(m_dwStyle & WS_VISIBLE)
  {
	  OnPaint(CDC()); 
  }
}

/*============================================================================*/
//  CCtrl�ຯ������                                                           //
/*============================================================================*/
BOOL CCtrl::Create(char *strName, const CRect &wndRect, CWnd * pParent, UINT32 dwStyle, UINT32 nCtrlId)
{
	if(!pParent)
		return FALSE;

	m_dwStyle  =  dwStyle|WS_CHILD;
	m_sRect    =  wndRect;
	m_pParent  =  pParent;
    m_nCtrlId  =  nCtrlId; 
    SetWindowText(strName);
	
	((CFrameWnd *)m_pParent)->AddChild(this); 

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
void CCtrl::Destroy()
{
    ((CFrameWnd *)m_pParent)->DeleteChild(this); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CCtrl::SendNotifyMessage(UINT32 nMessage, UINT32 lParam)
{
    ((CFrameWnd *)m_pParent)->SendMessage(nMessage, m_nCtrlId, lParam); 
}

/*============================================================================*/
//  CFrameWnd�ຯ������                                                       //
/*============================================================================*/

MESSAGE_ENTRY CFrameWnd :: stMessageEntry = {stMessageMap, NULL};
MESSAGE_MAP   CFrameWnd :: stMessageMap[] = {0,0,0,NULL};

extern CFrameWnd * g_pWindowList;

BOOL CFrameWnd::IsLegalWnd()
{
	CFrameWnd * pWnd=g_pWindowList;
	while(pWnd)
	{
		if(pWnd==this)
			return true;
		else
			pWnd=(CFrameWnd *)pWnd->m_pNextWnd ;
	}
	return false;
}

BOOL CFrameWnd::OnDestroy()
{
	return IsLegalWnd();
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CFrameWnd::Create(char *strName, UINT32 dwStyle, const CRect &wndRect, UINT32 nHelpId)
{
	SetWindowText(strName);
 
	m_dwStyle  =  dwStyle & ~WS_CHILD;   
	m_sRect    =  wndRect;
//	m_pChild   =  NULL;
	m_pParent  =  NULL;
	m_pCurCtrl =  NULL;  
	m_nHelpId  =  nHelpId;
	m_nWndType =  WND_TYPE_FRAME;

	

	if(!OnCreate())
		return FALSE;
	::AddToWindowList(this); // ygr ����˳��
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
void CFrameWnd::OnDactive()//CFrameWnd����Ӵ��ڼ��Ӵ��ڵ��Ӵ���WM_DACTIVE
{
	CCtrl *pChild = (CCtrl*)m_pChild;
	while(pChild)
	{
		pChild->SendMessage(WM_DACTIVE , 0 ,0);
		pChild = (CCtrl*)pChild->m_pNextWnd;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
CFrameWnd::CFrameWnd()
{
	m_pChild = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//
CFrameWnd::~CFrameWnd()
{
	Destroy();
}
////////////////////////////////////////////////////////////////////////////////
//
void CFrameWnd::Destroy()
{
	if(!OnDestroy())
		return;
	DestroyChildWnd();
	::DeleteWindow(this);
}

////////////////////////////////////////////////////////////////////////////////
//
void  CFrameWnd::DestroyChildWnd()
{
    CCtrl *pCtrl = (CCtrl *)m_pChild; 
//	CCtrl *pCtrl2;
	while(pCtrl)
	{
		pCtrl->Destroy(); 
		pCtrl = (CCtrl *)pCtrl->m_pNextWnd;
	}
//	m_pChild = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//
void CFrameWnd::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_dwStyle & WS_DISABLE)
		return;
	if(!IsLegalWnd())  //�����Ѳ���������!        hqf
		return;
	switch(nMessage)
	{
	case WM_ACTIVE:
		OnActive();
		break;
	case WM_DACTIVE:
		OnDactive();
		break;
	case WM_PENDOWN:
		DoPenDown(CPoint(HIWORD(wParam), LOWORD(wParam)));
		break;
	case WM_PENMOVE:
		DoPenMove(CPoint(HIWORD(wParam), LOWORD(wParam)));
		break;
	case WM_PENUP:
        DoPenUp();
		break;
	case WM_PAINT:
		Paint();
		break;
	case WM_CLOSE:
		Destroy();
		break;
	case WM_RTC:
		OnRtc(UINT16(wParam));
		break;
	default:
        ProcessMessageMap(nMessage, wParam, lParam);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void CFrameWnd::ProcessMessageMap(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	MESSAGE_ENTRY  *pMsgEntry = GetMessageEntry();
	while(pMsgEntry)
	{
		MESSAGE_MAP *pMsgUnit = pMsgEntry->messageMap;
		while(pMsgUnit->memberFxn)
		{
			if(nSrcId >= pMsgUnit->id1 && nSrcId <= pMsgUnit->id2 && pMsgUnit->message == nMessage)
			{
				(this->*(pMsgUnit->memberFxn))(nMessage, nSrcId, lParam); 
				   return;
			}
			pMsgUnit++;
		}
		pMsgEntry = pMsgEntry->parentMessageEntry; 
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void CFrameWnd::ShowWindow(UINT32 nCmdShow)
{
	switch(nCmdShow)
	{
	case SW_SHOW:
		::ShowWindow(this);
		break;
	case SW_HIDE:
		::HideWindow(this);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void CFrameWnd::AddChild(CCtrl *pChild)
{
	pChild->m_pNextWnd = m_pChild;
	if(m_pChild )
		m_pChild->m_pPrevWnd = pChild;
	m_pChild           = pChild;
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CFrameWnd::DeleteChild(CCtrl *pChild)// ֻ�Ǵ�������ɾ���Ӵ��ڣ������ͷ��ڴ档
{
	CCtrl * pFirst = (CCtrl *)m_pChild;

	if(pFirst == pChild)
	{
		m_pChild  = (CCtrl *)m_pChild->m_pNextWnd; 
		if(m_pChild)
			m_pChild->m_pPrevWnd = NULL;
		return TRUE;
	}

    while(pFirst && pFirst->m_pNextWnd != pChild)
		pFirst = (CCtrl *)pFirst->m_pNextWnd;

	if(pFirst == NULL)
		return FALSE;

	pFirst->m_pNextWnd = pChild->m_pNextWnd;
	if(pChild->m_pNextWnd)
		pChild->m_pNextWnd->m_pPrevWnd = pFirst;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
CCtrl * CFrameWnd::GetPenCtrl(CPoint & pt)
{
	CCtrl *pCtrl = (CCtrl *)m_pChild; 
	while(pCtrl )
	{
		if((pCtrl->m_dwStyle & WS_VISIBLE) && pt.InRect(pCtrl->m_sRect))
			return pCtrl;
		pCtrl = (CCtrl *)pCtrl->m_pNextWnd;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//
void  CFrameWnd::DoPenDown(CPoint &pt)
{
    CCtrl *pCtrl = GetPenCtrl(pt);
	if(pCtrl && m_pCurCtrl && (pCtrl == m_pCurCtrl))
	{
		pCtrl->SendMessage(WM_ACTIVE, (UINT32)m_pCurCtrl, 0);
		pCtrl->SendMessage(WM_PENDOWN, MAKELONG(pt.y, pt.x), 0);
	}
	else if(pCtrl && m_pCurCtrl)
	{ 
		UINT32 wParam = (UINT32) m_pCurCtrl;
		m_pCurCtrl->SendMessage(WM_DACTIVE, UINT32(pCtrl), 0);
		pCtrl->SendMessage(WM_ACTIVE, wParam , 0);
		pCtrl->SendMessage(WM_PENDOWN, MAKELONG(pt.y, pt.x), 0);
		m_pCurCtrl = pCtrl;
	}
	else if(pCtrl && !m_pCurCtrl)
	{
        pCtrl->SendMessage(WM_ACTIVE,  UINT32(m_pCurCtrl), 0);
		pCtrl->SendMessage(WM_PENDOWN, MAKELONG(pt.y, pt.x), 0);
		m_pCurCtrl = pCtrl;
	}
	else if(!pCtrl && m_pCurCtrl)
	{
       m_pCurCtrl->SendMessage(WM_DACTIVE, UINT32(pCtrl), 0);
	   m_pCurCtrl = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void  CFrameWnd::DoPenMove(CPoint &pt)
{
	CCtrl *pCtrl = GetPenCtrl(pt);
	if(pCtrl && m_pCurCtrl && pCtrl == m_pCurCtrl)
	{
	    pCtrl->SendMessage(WM_PENMOVE, MAKELONG(pt.y, pt.x), 0);
	}
	else if(pCtrl && m_pCurCtrl)
	{ 
		m_pCurCtrl->SendMessage(WM_PENMOVEOUT, MAKELONG(pt.y, pt.x), 0);
		m_pCurCtrl->SendMessage(WM_DACTIVE, UINT32(pCtrl), 0);
		pCtrl->SendMessage(WM_ACTIVE,    UINT32(m_pCurCtrl), 0);
		pCtrl->SendMessage(WM_PENMOVEIN, MAKELONG(pt.y, pt.x), 0);
		m_pCurCtrl = pCtrl;
	}
	else if(pCtrl && !m_pCurCtrl)
	{
        pCtrl->SendMessage(WM_ACTIVE,  0, 0);
		pCtrl->SendMessage(WM_PENMOVEIN, MAKELONG(pt.y, pt.x), 0);
		m_pCurCtrl = pCtrl;
	}
	else if(!pCtrl && m_pCurCtrl)
	{
        m_pCurCtrl->SendMessage(WM_PENMOVEOUT, MAKELONG(pt.y, pt.x), 0);
	    m_pCurCtrl->SendMessage(WM_DACTIVE, UINT32(pCtrl), 0);
	    m_pCurCtrl = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void  CFrameWnd::DoPenUp()
{ 
    if(m_pCurCtrl)
        m_pCurCtrl->SendMessage(WM_PENUP, 0, 0); 
}

///////////////////////////////////////////////////////////////////////////////
//         �������
void CFrameWnd::PaintControlBox(CDC &dc)
{
}

///////////////////////////////////////////////////////////////////////////////
//         ����ư�ť
void CFrameWnd::PaintTitle(CDC &dc)
{
   CRect r(m_sRect);
   r.bottom = r.top + WND_TITLE_HEIGHT-1;
   dc.SetBackColor(LGRAY);
   dc.EraseRect(r); 
   dc.SetPenColor(BLACK); 
   dc.DrawText(r.left +2, r.top + 1, m_strName);
   if(m_dwStyle & WS_CONTROLBOX)
   {
	   r.left = r.right - WND_TITLE_HEIGHT-1;
	   r.DeflateRect(1,1); 
	   dc.Draw3dButton(r, NULL); 
   }

   // temp, debug
/*   char	tempecho[32];
	long	templen;
	for (templen = 0; templen < 0x10000; templen ++)
		if (*((BYTE*)0x00100000 + templen) != 0xFF)
			break;
	extern NU_MEMORY_POOL  g_nuSystem_Memory_Pool;
	sprintf(tempecho, "%ld, %ld", g_nuSystem_Memory_Pool.words[11], 0x10000 - templen); 
	dc.TextOut(0, 0, tempecho);*/

}

////////////////////////////////////////////////////////////////////////////////
//
void CFrameWnd::OnPaint(CDC &dc)
{

//   dc.Reset();
   if(!(m_dwStyle & WS_TRANSPARENT))
	   dc.EraseRect(m_sRect);
   
   if(m_dwStyle & WS_BORDER)
   	     dc.FrameRect(m_sRect); //����
   if(m_dwStyle & WS_CAPTION)
	     PaintTitle(dc);
   PaintChildWnd();
}

////////////////////////////////////////////////////////////////////////////////
//
void CFrameWnd::PaintChildWnd()
{
	CCtrl *pCtrl = (CCtrl *)m_pChild; 
	while(pCtrl)
	{
		pCtrl->Paint(); 
		pCtrl = (CCtrl *)pCtrl->m_pNextWnd;
	}
}

/*============================================================================*/
//  CButton�ຯ������                                                         //
/*============================================================================*/
BOOL CButton::Create(char *strCaption,CRect &ctrlRect, CFrameWnd * pParent, UINT32 nCtrlId)
{
	m_bPressed = 0;
	return CCtrl::Create(strCaption, ctrlRect, pParent, WS_CHILD|WS_VISIBLE, nCtrlId); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CButton::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_dwStyle & WS_DISABLE)
		return;

    switch(nMessage)
    {
    case WM_PENMOVEIN:
    case WM_PENDOWN:
    case WM_PENMOVE:
        if(!m_bPressed)
        {
         CDC::InvertRect (m_sRect);
         m_bPressed = 1;
        }
        break;
    case WM_PENMOVEOUT:
        if(m_bPressed)
        {
         CDC::InvertRect (m_sRect);
         m_bPressed = 0;
        }
         break;
    case WM_PENUP:
        if(m_bPressed)
        {
        CDC::InvertRect (m_sRect); 
        m_bPressed = 0;
        SendNotifyMessage(CN_COMMAND, 0);
        }
        break; 
    }
}

////////////////////////////////////////////////////////////////////////////////
//
void CButton::OnPaint(CDC &dc)
{
	 dc.Draw3dButton(m_sRect, m_strName);  
}

/*============================================================================*/
//  CIconButton�ඨ��                                                         //
/*============================================================================*/
BOOL CIconButton::Create(char *strCaption, UINT32 nIconId, CRect &ctrlRect, CFrameWnd * pParent, UINT32 nCtrlId)
{
	if(!CButton::Create(strCaption, ctrlRect, pParent, nCtrlId))
		return FALSE;
	m_nIconId = nIconId;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
void CIconButton::SetIcon(UINT32 nIconId)
{
	m_nIconId  = nIconId;
}
////////////////////////////////////////////////////////////////////////////////
//
void CIconButton::OnPaint(CDC &dc)
{

	CRect r(m_sRect.left, m_sRect.top, m_sRect.right, m_sRect.bottom);
	dc.SetBackColor(LGRAY);
	dc.EraseRect(r);
	if (m_nIconId && m_nIconId <= BMPTOTAL){
		dc.PutIcon(r.left+1, (r.top + r.bottom)/2 - gICON_HEIGHT(m_nIconId)/2, m_nIconId);
		r.left += gICON_WIDTH(m_nIconId) + 1;
		r.top += 1;
	}
	else
		r.left += 1;
	dc.SetAlign(HALIGN_LEFT, VALIGN_MID);
	dc.DrawText(r, m_strName); 
	dc.Draw3dRect(m_sRect); 

/*	
	if(m_nIconId && m_nIconId <= BMPTOTAL)
	{
		INT16 x = (m_sRect.left + m_sRect.right)/2 + gICON_WIDTH(m_nIconId)/2; 
		INT16 y = (m_sRect.top + m_sRect.bottom)/2 - gICON_HEIGHT(m_nIconId)/2; 
		dc.SetBackColor(LGRAY);
	    dc.EraseRect(m_sRect);
		dc.PutIcon(m_sRect.left+1, y, m_nIconId);
		dc.SetAlign(HALIGN_LEFT, VALIGN_MID);
		CRect r(m_sRect.left + gICON_WIDTH(m_nIconId)+1, m_sRect.top+1, m_sRect.right, m_sRect.bottom); 
		dc.DrawText(r, m_strName); 
		dc.Draw3dRect(m_sRect); 
	} 
	else
		CButton::OnPaint(dc); 
*/
}

/*============================================================================*/
//  CMenuButton�ඨ��                                                         //
/*============================================================================*/
void CMenuButton::OnPaint(CDC &dc)
{
	dc.SetBackColor(LGRAY);
	dc.Draw3dButton(m_sRect, NULL); 
	CRect r(m_sRect.left+1 , m_sRect.top+1 ,m_sRect.right-8,m_sRect.bottom);
	dc.DrawText(r, m_strName);
	dc.PutIcon(m_sRect.right-8, m_sRect.top+1, ICON_SMDOWNARROW);
}

/*============================================================================*/
//  CLineEdit�ඨ��                                                           //
/*============================================================================*/
BOOL CLineEdit::Create(char * strInitText, INT32 nMaxCount, CRect &ctrRect, CFrameWnd *pParent, UINT32 nCtrlId)
{
	m_nDispChars = ctrRect.Width() / ASC_WIDTH;
	m_nMaxCount = nMaxCount;
	m_pEditBuf  = new char[nMaxCount];
	if(strInitText)
	{
		strcpy(m_pEditBuf, strInitText);
		m_nCharCount = strlen(strInitText);
	}
	else
	{
		*m_pEditBuf  = 0;
		m_nCharCount = 0;
	}
	m_nCursorPos = 0; // ��������
	return CCtrl::Create(NULL, ctrRect, pParent, WS_VISIBLE|WS_CHILD, nCtrlId); 
}

////////////////////////////////////////////////////////////////////////////////
//
void CLineEdit::WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	INT16 x = HIWORD(wParam) , y = LOWORD(wParam);
	switch(nMessage)
	{
	case WM_PENDOWN:case WM_PENMOVEIN:
		{
		if(m_nCharCount == 0)
			return;
		INT16 nCharPos = (x - m_sRect.left)/ASC_WIDTH;
		if(nCharPos + m_nCharCount <= m_nDispChars)
			m_nCursorPos = m_nCharCount-1;
		else
			m_nCursorPos = m_nDispChars  - nCharPos;
		if(m_nCursorPos <0 )
			m_nCursorPos = 0;
		ShowCursor();
		break;
		}
	case WM_PENUP:
		SendNotifyMessage(CN_COMMAND , 0);
		break;
	}
}

void CLineEdit::ShowCursor()
{
	::OpenCursor();
	::SetCursorAttr(2, CHARHEIGHT-1);
	::SetCursorXY(m_sRect.right - m_nCursorPos * ASC_WIDTH, m_sRect.top +1);
}
////////////////////////////////////////////////////////////////////////////////
//
void CLineEdit::OnPaint(CDC &dc)
{
	dc.EraseRect(m_sRect);
	dc.FrameRect(m_sRect);
	dc.SetAlign(HALIGN_RIGHT);
    if(m_nCharCount <= m_nDispChars)
	 dc.DrawText(m_sRect.right-1, m_sRect.top+1, m_pEditBuf);
	else
	 dc.DrawText(m_sRect.right-1, m_sRect.top+1, m_pEditBuf + (m_nCharCount-m_nDispChars)); 
	ShowCursor();
}
////////////////////////////////////////////////////////////////////////////////
//
void CLineEdit::Destroy()
{
	delete [] m_pEditBuf;
	::CloseCursor();
	CCtrl::Destroy(); 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CLineEdit::AppendChar(char ch)
{
	if(m_nCharCount + 1 >= m_nMaxCount)
		return FALSE;
	for(INT16 i = 0; i <= m_nCursorPos; i++)
		m_pEditBuf[m_nCharCount - i] = m_pEditBuf[m_nCharCount -1 -i];
	m_pEditBuf[m_nCharCount - m_nCursorPos] = ch;
//	m_nCharCount++;
	m_pEditBuf[++m_nCharCount]=0;
	OnPaint(CDC());
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
void CLineEdit::OnActive()
{
	ShowCursor();
}

////////////////////////////////////////////////////////////////////////////////
//
void CLineEdit::OnDactive()
{
	::CloseCursor();
}
////////////////////////////////////////////////////////////////////////////////
//
void CLineEdit::BackSpace()
{
	if(m_nCharCount - m_nCursorPos > 0)
	{
	 for(INT16 i = 0; i<= m_nCursorPos ; i++)
	 {
		 m_pEditBuf[m_nCharCount -1 - m_nCursorPos + i] = 
			 m_pEditBuf[m_nCharCount - m_nCursorPos + i];
	 }
	 m_pEditBuf[--m_nCharCount]=0;
	 if(m_nCharCount> 0 && m_nCursorPos >= m_nCharCount)
		 m_nCursorPos = m_nCharCount -1;
	 OnPaint(CDC());
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void CLineEdit::Clear()
{
	m_nCharCount=0;
	m_nCursorPos = 0;
	m_pEditBuf[m_nCharCount]=0;
	OnPaint(CDC());
}
////////////////////////////////////////////////////////////////////////////////
//
void CLineEdit::GetInputText(char * pBuf, INT32 nBufLen)
{
	hwstrncpy(pBuf, m_pEditBuf, nBufLen - 1);
	*(pBuf + nBufLen - 1) = '\0';
}

/*============================================================================*/
//  CCheckButton�ඨ��                                                        //
/*============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//
void CCheckButton::OnPaint(CDC &dc)
{
	UINT32 iconid = m_bChecked ? ICON_CHECKED : ICON_UNCHECKED;
	//dc.SetBackColor(LGRAY); 
	dc.PutIcon(m_sRect.left+1, m_sRect.top, iconid);
	CRect r = m_sRect;
	r.left += 16;
	dc.DrawText(r, m_strName); 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CCheckButton::Create(char *strText, CRect &ctrRect, CFrameWnd *pParent, BOOL bInitValue, UINT32 nCtrlId)
{
	m_bChecked = bInitValue;
	return CButton::Create(strText, ctrRect, pParent, nCtrlId); 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CCheckButton::GetValue()
{
	return m_bChecked;
}
////////////////////////////////////////////////////////////////////////////////
//
void CCheckButton::SetCheck(BOOL bChecked)
{
	if(bChecked != m_bChecked)
	{
		m_bChecked = bChecked;
		OnPaint(CDC());
	}
}
void CCheckButton::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(nMessage == WM_PENUP)
	{
       if(m_bPressed)
       {
        CDC::InvertRect (m_sRect); 
        m_bPressed = 0;
		m_bChecked = !m_bChecked;
		OnPaint(CDC());
        SendNotifyMessage(CN_COMMAND, 0);
       }
	}
	else
	  CButton::WindowProcess(nMessage, wParam, lParam); 
}

/*============================================================================*/
//  CRadioButton�ඨ��                                                        //
/*============================================================================*/
void CRadioItem::Reset()
{
	m_itemName[0] = '\0';
	m_itemRect.SetRect(0 , 0 , 0 , 0);
	m_bIsSel = FALSE;
	m_pNextItem= NULL;
}

void CRadioItem::Paint(CDC &dc)
{
	PaintRadio(dc, m_bIsSel);
	PaintText(dc);
}

void CRadioItem::PaintRadio(CDC &dc , BOOL bIsSel)
{
	if(bIsSel)
		dc.PutIcon(m_itemRect.left , m_itemRect.top , ICON_RADIOSEL);
	else
		dc.PutIcon(m_itemRect.left , m_itemRect.top , ICON_RADIOUNSEL);
}

void CRadioItem::PaintText(CDC &dc)
{
	CRect r(m_itemRect.left + 18 , m_itemRect.top , m_itemRect.right , m_itemRect.bottom);
	dc.DrawText(r, m_itemName);
}

void CRadioItem::SetSel(BOOL bIsSel)
{
	if(bIsSel == m_bIsSel)
		return;
	m_bIsSel = bIsSel;
	PaintRadio(CDC() , bIsSel);
}

CRadioButton::CRadioButton()
{
	
}

CRadioButton::~CRadioButton()
{
}

BOOL CRadioButton::Create(CRect &wndRect , UINT32 dwStyle , CWnd *pParent , UINT32 nCtrlID)
{
	if(!CCtrl::Create(NULL, wndRect , pParent , dwStyle ,nCtrlID))
		return FALSE;
	m_nSelItem= -1;
	m_nItems = 0;
	m_pFirstItem = m_pLastItem = NULL;
	m_nAlign = HORIZONTAL;
}
    
void CRadioButton::Destroy()
{
	CRadioItem *pItem;
	pItem = m_pFirstItem;
	while(pItem)
	{
		m_pFirstItem = pItem->m_pNextItem;
		delete pItem;
		pItem = m_pFirstItem;
	}
	m_pLastItem = NULL;
	CCtrl::Destroy();//��Ҫbug!!!
}

BOOL CRadioButton::AddButton(char *itemName)
{
	CRadioItem *pItemTemp = new CRadioItem;
	if(!pItemTemp)
		return FALSE;
	pItemTemp->Reset();
	hwstrncpy(pItemTemp->m_itemName , itemName , MAX_ITEM_LENGTH - 1);
	if(m_pLastItem==NULL)
		m_pFirstItem = m_pLastItem = pItemTemp;
	else
	{
		m_pLastItem->m_pNextItem = pItemTemp;
		m_pLastItem = pItemTemp;
	}
	m_nItems++;
}

BOOL CRadioButton::SetButtonText(INT16 nItem , char *strNewText ,BOOL bRedraw)
{
	if(nItem <0 || nItem >=m_nItems)
		return FALSE;
	CRadioItem *pItem = m_pFirstItem;
	for(int i = 0; i< nItem ;i++)
		pItem = pItem->m_pNextItem;
	strncpy(pItem->m_itemName , strNewText,MAX_ITEM_LENGTH );
	pItem->m_itemName[MAX_ITEM_LENGTH]= '\0';
	if(bRedraw)
		OnPaint(CDC());
}
		
CRadioItem *CRadioButton::GetItem(INT16 nItem)
{
	if(nItem <0 || nItem >=m_nItems)
		return NULL;
	CRadioItem *pItem = m_pFirstItem;
	for(int i = 0; i< nItem ;i++)
		pItem = pItem->m_pNextItem;
	return pItem;
}

void CRadioButton::SetSel(INT16 nItem , BOOL bRedraw)
{
	INT16 nSelItem;
	CRadioItem *pItem;
	nSelItem = 0;
	if(!bRedraw)
	{
		m_nSelItem = nItem;
		if(nItem <0 || nItem >=m_nItems)
			m_nSelItem = -1;
		return;
	}
	if(m_pFirstItem&& (m_pFirstItem->m_itemRect.Width()==0||
		m_pFirstItem->m_itemRect.Height()==0))
		return;
	if(nItem < 0 || nItem >= m_nItems)
	{
		for(pItem = m_pFirstItem; pItem; pItem= pItem->m_pNextItem)
		{
			if(nSelItem++== m_nSelItem)
				pItem->SetSel(FALSE);
		}
		m_nSelItem = -1;
	}
	else
	{
		if(nItem == m_nSelItem)
			return;
		for(pItem = m_pFirstItem; pItem; pItem= pItem->m_pNextItem, nSelItem++)
		{
			if(nSelItem == m_nSelItem)
				pItem->SetSel(FALSE);
			else if(nSelItem == nItem)
				pItem->SetSel(TRUE);

		}
		m_nSelItem = nItem;
	}
}
		
void CRadioButton::PaintItem(CRect r, CRadioItem *pItem)
{
	if(pItem==NULL)
		return;
	pItem->m_itemRect = r;
	pItem->Paint(CDC());
}

void CRadioButton::OnPaint(CDC &dc)
{
	int nItemWidth , nItemHeight ;
	int i;
	CRadioItem *pItem = m_pFirstItem;
	CRect r(m_sRect);
	r.DeflateRect(1, 1);

	if(m_nAlign == HORIZONTAL)
	{
		nItemWidth = r.Width()/m_nItems;
		nItemWidth = MAX(nItemWidth , 32);
		r.right = r.left + nItemWidth;
		i = 0;
		while(r.right <= m_sRect.right && pItem)
		{
			r.right -=1;// , r.bottom -= 1;
			PaintItem(r , pItem);	
			if(i++== m_nSelItem)
				pItem->SetSel();
			else
				pItem->SetSel(FALSE);
			pItem = pItem->m_pNextItem;
			r.left = r.right +1 , r.right  = r.left + nItemWidth;
		}
	}
	else
	{
		nItemHeight = r.Height()/m_nItems;
		nItemHeight = MAX(nItemHeight , RADITEM_HEIGHT);
		r.bottom = r.top + nItemHeight ;
		i =0;
		while(r.bottom <= m_sRect.bottom && pItem)
		{
			r.bottom -=1;
			PaintItem(r, pItem);
			if(i++== m_nSelItem)
				pItem->SetSel();
	    	else
				pItem->SetSel(FALSE);
	    	pItem = pItem->m_pNextItem;
			r.top = r.bottom +1 , r.bottom = r.top + nItemHeight;
		}
	}
}

void CRadioButton::WindowProcess(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	switch(nMessage)
	{
	case WM_PENDOWN:
	case WM_PENMOVEIN:
	case WM_PENMOVE:
		{
		 CPoint pt(HIWORD(wParam) , LOWORD(wParam));
		 DoPenDown(pt);
		 break;
		}
	default:
		break;
	}
}

void CRadioButton::DoPenDown(CPoint &pt)
{
	CRadioItem *pItem;
	INT16 nSelItem =0;
	for(pItem = m_pFirstItem ; pItem ;pItem= pItem->m_pNextItem)
	{
		if(pt.InRect(pItem->m_itemRect))
		{
			SetSel(nSelItem, TRUE);//ѡ�пؼ�nSelItem��������ѡ�С�
			SendNotifyMessage(CN_COMMAND , m_nSelItem);
			break;
		}
		nSelItem++;
	}
}		

/*============================================================================*/
//  CPages�ඨ��                                                         //
/*============================================================================*/

BOOL CPages::Create(char *strCaption, CRect &ctrlRect, CFrameWnd * pParent, UINT32 nCtrlId)
{
	m_bSelected = FALSE;
	return CButton::Create(strCaption, ctrlRect, pParent, nCtrlId);
}

void CPages::OnPaint(CDC &dc)//modified by hqf
{
//	dc.EraseRect(m_sRect); 
//	dc.TextOut(m_sRect.left+1, m_sRect.bottom -16, m_strName); 
	CButton::OnPaint(dc);
	if(m_bSelected)
	{
/*	  dc.MoveTo(m_sRect.left,  m_sRect.bottom); 
      dc.LineTo(m_sRect.left,  m_sRect.top); 
      dc.LineTo(m_sRect.right, m_sRect.top); 
	  dc.LineTo(m_sRect.right, m_sRect.bottom);
	  CRect r = m_sRect;
	  r.DeflateRect(1,1); 
	  dc.InvertRect(r); */
	  dc.InvertRect(m_sRect);//��ɫ
	}
/*	else
	{
	  dc.MoveTo(m_sRect.left, m_sRect.bottom);  
	  dc.LineTo(m_sRect.left, m_sRect.top);
	  dc.LineTo(m_sRect.right, m_sRect.top);
	  dc.LineTo(m_sRect.right, m_sRect.bottom);
	  dc.LineTo(m_sRect.left, m_sRect.bottom);  
	}*/
}
/*============================================================================*/
//  CLineButton�ඨ��                                                         //
/*============================================================================*/
void CLineButton::OnPaint(CDC &dc)
{
	dc.EraseRect(m_sRect);
	dc.DrawText(m_sRect, m_strName);  
	dc.SetLineStyle(1);
	dc.MoveTo(m_sRect.left,  m_sRect.bottom);
	dc.LineTo(m_sRect.right, m_sRect.bottom);   
}


/*============================================================================*/
//  CDialog�ඨ��                                                             //
/*============================================================================*/

BEGIN_MESSAGE_MAP(CDialog, CFrameWnd)
  //{{ define message map
  ON_MESSAGE(IDCANCEL, CN_COMMAND, OnCancel)
  ON_MESSAGE(IDOK,     CN_COMMAND, OnOk)
  ON_MESSAGE(IDYES,    CN_COMMAND, OnYes)
  ON_MESSAGE(IDNO,     CN_COMMAND, OnNo)
  //}}
END_MESSAGE_MAP

////////////////////////////////////////////////////////////////////////////////
//
CDialog::CDialog()
{
	m_bPutScreen = TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
CDialog::~CDialog()
{
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CDialog::Create(char *strName ,UINT32 dwStyle ,  const CRect &wndRect , 
		          BOOL bPutScreen, UINT32 nHelpId)
{
	m_bPutScreen = bPutScreen;
	SetCurrentDialogApp(::GetCurrentApp());
	return CFrameWnd::Create(strName , dwStyle , wndRect , 
		          nHelpId);
}
////////////////////////////////////////////////////////////////////////////////
//

BOOL CDialog::OnCreate()
{
    if(!CFrameWnd::OnCreate())
		return FALSE; 
	m_bExit = 0;
	m_nWndType = WND_TYPE_DIALOG;
	CRect ctrRect(m_sRect.right - WND_TITLE_HEIGHT +1, m_sRect.top, m_sRect.right, m_sRect.top + WND_TITLE_HEIGHT-1);
	if(!m_butCancel.Create(NULL, ICON_CANCEL, ctrRect, this, IDCANCEL))
		return FALSE;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
void CDialog::Destroy()
{
	if(!OnDestroy())
		return;
	DestroyChildWnd();
	if(m_bPutScreen)
	{
		::DeleteWindow(this, FALSE);
		CDC::PutScreen(m_pBkScrBuf); 
	}
	else
	{
		::DeleteWindow(this);  //  hqf 12/21
		SysFree(m_pBkScrBuf->pBuffer);
		m_pBkScrBuf->pBuffer = NULL;
		SysFree(m_pBkScrBuf);
		m_pBkScrBuf = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
UINT32 CDialog::DoModal()
{
  extern CFrameWnd * g_pCurWindow;
   if(g_pCurWindow)
   {
	   g_pCurWindow->SendMessage(WM_DACTIVE, 0, 0); 
	   g_pCurWindow = NULL;
   }
   m_pBkScrBuf = CDC::SaveScreen(m_sRect);
   ShowWindow();
   extern void EventLoop(UINT32 *);
   m_bExit = 0;
   EventLoop(&m_bExit);
   return m_nReturnCode;
}

////////////////////////////////////////////////////////////////////////////////
//
void CDialog::OnYes()
{
	m_bExit = 1;
	m_nReturnCode = IDYES;
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////
//
void CDialog::OnNo()
{
	m_bExit = 1;
	m_nReturnCode = IDNO;
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////
//
void CDialog::OnOk()
{
	m_bExit = 1;
	m_nReturnCode = IDOK;
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////
//
void CDialog::OnCancel()
{
	m_bExit = 1;
	m_nReturnCode = IDCANCEL;
    Destroy();
}

/*============================================================================*/
//  CAppWnd�ඨ��                                                             //
/*============================================================================*/

BEGIN_MESSAGE_MAP(CAppWnd, CFrameWnd)
  //{{
  ON_MESSAGE(IDCANCEL, CN_COMMAND, OnCancel)
  ON_MESSAGE(IDOK, CN_COMMAND, OnOk)
  //}}
END_MESSAGE_MAP

////////////////////////////////////////////////////////////////////////////////
//
void CAppWnd::OnCancel(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	Destroy(); 
} 
void CAppWnd::OnOk(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////
//
void CAppWnd::Destroy()
{
	if(!m_bForceToClose)
	{
		if(!OnDestroy())
			return;
	}
	else
		OnDestroy();
	DestroyChildWnd();
	::DeleteWindow(this);
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CAppWnd::Create(char *strName, UINT32 dwStyle, const CRect &wndRect, UINT32 nHelpId, UINT32 nWndIcon)
{
	m_nWndIcon = nWndIcon;
	m_bForceToClose = FALSE;
	return CFrameWnd::Create(strName, dwStyle, wndRect, nHelpId); 
}
////////////////////////////////////////////////////////////////////////////////
//
void CAppWnd::OnPaint(CDC &dc)
{
   if(!(m_dwStyle & WS_TRANSPARENT))
	   dc.EraseRect(m_sRect);
   
   if(m_dwStyle & WS_BORDER)
   	     dc.FrameRect(m_sRect); 

   if(m_dwStyle & WS_CAPTION)
	     PaintTitle(dc);
   PaintChildWnd();
} 
////////////////////////////////////////////////////////////////////////////////
//
void CAppWnd::PaintTitle(CDC &dc)
{
   CRect r(m_sRect);
   r.bottom = r.top + WND_TITLE_HEIGHT -1;
   if(m_nWndIcon)
   {
     dc.PutIcon(r.left+1, r.top+1, m_nWndIcon);
	 r.left += 16;
   }
   dc.SetBackColor(LGRAY);
   dc.EraseRect(r); 
   dc.SetPenColor(BLACK); 
   dc.DrawText(r.left +2, r.top +1, m_strName);
   m_obutCancel.OnPaint(CDC());  
/*
   // temp, debug
   char	tempecho[32];
	long	templen;
	for (templen = 0; templen < 0x10000; templen ++)
		if (*((BYTE*)0x00100000 + templen) != 0xFF)
			break;
	extern NU_MEMORY_POOL  g_nuSystem_Memory_Pool;
	sprintf(tempecho, "%ld, %ld", g_nuSystem_Memory_Pool.words[11], 0x10000 - templen); 
	dc.TextOut(0, 0, tempecho);
*/
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL CAppWnd::OnCreate()
{
	if(!CFrameWnd::OnCreate())
		return FALSE;
    m_nWndType = WND_TYPE_APP;
	CRect ctrRect(m_sRect.right - WND_TITLE_HEIGHT+1, m_sRect.top, m_sRect.right, m_sRect.top + WND_TITLE_HEIGHT -1);
	if(!m_obutCancel.Create(NULL, ICON_CANCEL, ctrRect, this, IDCANCEL))
		return FALSE;
	if(m_dwStyle & WS_OKBOX)
	{
		ctrRect.right = ctrRect.left -1 , ctrRect.left = ctrRect.right - WND_TITLE_HEIGHT+1;//���17
		if(!m_oBtnOk.Create(NULL , ICON_OK, ctrRect , this , IDOK))
			return FALSE;
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//end window.cpp









/******************************************************************************/
/*                                                                            */
/*  ��Ȩ����(c)  2001�����Ƽ�                                                 */
/*                                                                            */
/******************************************************************************/
/*  �� �� ��                                                                  */
/*             window.cpp                                                     */
/*                                                                            */
/*  �� �� ��                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  ��    ��                                                                  */
/*             �����Ƽ�E-PhoneII��������Դ�ļ�                              */
/*             HanWang Technology E-PhoneII Function Class Library source file*/
/*                                                                            */
/*  ��    ��                                                                  */
/*             ȫ�ִ�������ͷָ�붨��                                         */
/*             ����ȫ�ִ����б�ĺ�������                                     */
/*             �绰�����ඨ��                                                 */
/*             �����ඨ��                                                     */
/*                                                                            */
/*  ���˵��                                                                  */
/*             ��                                                             */
/*                                                                            */
/*  ��    ��                                                                  */
/*             ����                                                           */
/*                                                                            */
/*  ��    ��                                                                  */
/*             2001-06-15                                                     */
/*                                                                            */
/*  ��ʷ��¼                                                                  */
/*             2001-07-15    ���������ඨ��                                   */
/*                                                                            */
/*             2001-07-24    ���ӵ绰������                                   */
/*                                                                            */
/*             2001-08-01    �����򴰿ڷ���WM_ACTIVE��WM_DACTIVE��Ϣ          */
/*                                                                            */
/*             2001-08-23    ����HideWindowNotRedrawBack����                  */
/*             2001-11-12    �޸�HideWindow����������ᣩ                     */
/*             2001-11-21    �޸�TranslateMessage����������ᣩ               */
/******************************************************************************/
#include "hwfcl.h"
#include "phonedrv.h"
#include "messagebox.h"
#include "pinyin.h"
#include "record.h"
#include "phoneapp.h"
#include "alarm.h"
////////////////////////////////////////////////////////////////////////////////
//��ȫ������¼��ǰ���ĸ�Ӧ����,�Է�ֹ�ظ�����
//��ʼΪ0��ʾ��ǰ�κ�һ��Ӧ����
//ÿ��APPӦ������ʱ����Ѵ˱�����Ϊ��Ӧ��ֵ
//�˳�ʱ�Ѵ˱�����Ϊ,�ɽӿں���SetCurrentApp��ɶԴ˱������޸�
//
static UINT32       g_nCurApp = 2 ;    
CFrameWnd * g_pWindowList = NULL;  //���������ͷָ��
CFrameWnd * g_pDeskTop    = NULL;  //ָ������
CFrameWnd * g_pCurWindow  = NULL;         //ָ��ǰ����

//		STATUS status = NU_Create_Timer (&RecDelayStartTimer, "Record Timer", RecDelayTimerProccess, 122, 300, 300, NU_ENABLE_TIMER);
NU_TIMER	RecDelayStartTimer;		// ¼��������ʱ��

extern CFrameWnd * g_pPhoneWnd;
extern CAlarmDisplay *g_pAlarmDisplay;
extern CRecord *pRec;
extern void CardAppRun();		// ��Ƭ
extern void MemoAppRun();		// ����
extern void PictureAppRun();	// ���
extern void AlarmAppRun();		// ����
extern void HistoryAppRun();	// ͨ����¼
extern void GameAppRun();
extern void ToolAppRun();
extern void SettingsAppRun();	// ϵͳ����
extern void ClockAppRun();		// ʱ��
extern void ConvAppRun();		// ��λ����
extern void CalcAppRun();		// ������
extern void ReferenceAppRun();	// ����
extern void RecordBrowseAppRun();	// ���Բ鿴
extern void HelpAppRun(INT32 nHelpID);
extern void SMAppRun();             //����Ϣ
extern void ScrProtectAppRun();     //��������
extern void ExitScrProt();
//extern void DoWithAlarm();
extern BOOL	g_bIsCanReset ;//////�Ƿ������л�
/////////////////////////////////////////////////////////////////////

void RecDelayTimerProccess(UNSIGNED)
{
	UINT32	type;
	NU_Control_Timer(&RecDelayStartTimer, NU_DISABLE_TIMER);
	if (GetPhoneStatus())
		type = RECORD_LINE;
	else
		type = RECORD_MIC;
	PostSystemMessage(WM_RECORD_START,0, type, 0,true);
}


	
////////////////////////////////////////////////////////////////////////////////
//

void SetCurrentApp(UINT32 nAppId)
{
    g_nCurApp = nAppId;
}

UINT32 GetCurrentApp()
{
	return g_nCurApp;
}
////////////////////////////////////////////////////////////////////////////////
//
CFrameWnd * GetActiveWindow(CFrameWnd *pStartWnd)
{
	CFrameWnd *pWnd;
	if(pStartWnd == NULL)
		pWnd = g_pWindowList;
	else
		pWnd = pStartWnd;
	while(pWnd && !(pWnd->GetWindowStyle() & WS_VISIBLE))
		pWnd = (CFrameWnd *)pWnd->GetNextWnd();
	return pWnd;
}


////////////////////////////////////////////////////////////////////////////////
//
BOOL ReturnToDeskTop(BOOL bPaintDesktop)
{
	static BOOL bFirstIn = TRUE;
	if(bFirstIn)
		bFirstIn = FALSE;
	else
		return FALSE;
    CFrameWnd *pWnd = g_pWindowList;
    if(pWnd == g_pDeskTop)
    	  return FALSE;

    if(pWnd == g_pPhoneWnd)
		pWnd->ShowWindow(SW_HIDE); 
	
	while(pWnd)
	{
		if(pWnd != g_pPhoneWnd )
			pWnd->Visible(FALSE);  
    	pWnd = (CFrameWnd *)pWnd->GetNextWnd();
	}
    pWnd = g_pWindowList;
	while(pWnd)
	{
		if(pWnd != g_pDeskTop && pWnd != g_pPhoneWnd)
		{
			switch(pWnd->GetWndType())
			{
			case WND_TYPE_APP:
				  ((CAppWnd *)pWnd)->m_bForceToClose = TRUE;
				  ((CAppWnd*)pWnd)->Destroy();
				  ((CAppWnd *)pWnd)->m_bForceToClose = FALSE;
				  break;
			case WND_TYPE_FRAME:
				  pWnd->Destroy(); 
				  break;
			case WND_TYPE_DIALOG:
				  //pWnd->Destroy(); 
				  ((CDialog*)pWnd)->OnCancel();
				  break;
			}
		}
		pWnd = (CFrameWnd *)pWnd->GetNextWnd();
	}
    g_pDeskTop->Visible(TRUE);  
    g_pCurWindow =GetActiveWindow();

	if(bPaintDesktop)
	{
		g_pCurWindow->SendMessage(WM_ACTIVE, 0, 0); 
		g_pCurWindow->SendMessage(WM_PAINT,  0, 0); 
	}
//	else
//		g_pCurWindow->Visible(FALSE);  // ****hqf*****//
	bFirstIn = TRUE;
	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
//
void AddToWindowList(CFrameWnd * pWnd)
{
  pWnd->m_pNextWnd =  g_pWindowList;  //���������ͷָ��  CFrameWnd * g_pWindowList
  if(g_pWindowList)
	 g_pWindowList->m_pPrevWnd = pWnd;
  g_pWindowList = pWnd;
  if(g_pWindowList)
	g_pWindowList->m_pPrevWnd = NULL;
} 

////////////////////////////////////////////////////////////////////////////////
//
BOOL DeleteWindow(CFrameWnd *pWnd, BOOL bRedrawBackWnd)
{
	BOOL bPaintBefore =  (pWnd == g_pCurWindow) ;

	if(!DeleteWindowList(pWnd))
		return FALSE;

	g_pCurWindow =GetActiveWindow(); 

//	g_pCurWindow->m_pPrevWnd = NULL; 
	if(g_pCurWindow && bPaintBefore )
	{
		 g_pCurWindow->SendMessage(WM_ACTIVE, 0, 0); 
//		 if(bRedrawBackWnd)
//			g_pCurWindow->SendMessage(WM_PAINT,  0, 0); 
		 if(bRedrawBackWnd)
		 {
			 CRect r = g_pCurWindow->GetWindowRect();
			 if(r == DESK_TOP_RECT)
    			g_pCurWindow->SendMessage(WM_PAINT,  0, 0); 
			 else
			 {
				CWnd *pWndTemp = g_pCurWindow->m_pNextWnd;
				CRect r2;
				while(pWndTemp && pWndTemp->m_pNextWnd
						&& ((r2= pWndTemp->GetWindowRect()) != DESK_TOP_RECT))
					pWndTemp = pWndTemp->m_pNextWnd;
				if(pWndTemp)
				{
					while(pWndTemp)
					{
						if(pWndTemp->m_dwStyle & WS_VISIBLE)
							pWndTemp->SendMessage(WM_PAINT , 0 , 0);
						pWndTemp = pWndTemp->m_pPrevWnd;
					}
				}
				else
					g_pCurWindow->SendMessage(WM_PAINT,  0, 0); 
			 }
		 }
	}
} 
////////////////////////////////////////////////////////////////////////////////
//
BOOL DeleteWindowList(CFrameWnd *pWnd)
{
	CFrameWnd *pCurWnd = g_pWindowList;
	if(pCurWnd == pWnd)
	{
		g_pWindowList = (CFrameWnd *)pCurWnd->m_pNextWnd;
		if(g_pWindowList)
			g_pWindowList->m_pPrevWnd = NULL;
		return TRUE;
	}
	while(pCurWnd != NULL && pCurWnd->m_pNextWnd != pWnd)
		 pCurWnd = (CFrameWnd *)pCurWnd->m_pNextWnd;

	if(!pCurWnd)
		return FALSE;

	pCurWnd->m_pNextWnd = pWnd->m_pNextWnd;
	if(pWnd->m_pNextWnd)
		pWnd->m_pNextWnd->m_pPrevWnd = pCurWnd;

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
void ShowWindow(CFrameWnd * pWnd)
{
	if(pWnd && (pWnd != g_pCurWindow))
	{
		if(pWnd != g_pWindowList)
		{
			DeleteWindowList(pWnd);
			AddToWindowList(pWnd);
		}
		pWnd->SetWindowStyle(pWnd->GetWindowStyle() | WS_VISIBLE); 
		if(g_pCurWindow)
			g_pCurWindow->SendMessage(WM_DACTIVE, 0, 0); 
		g_pCurWindow = pWnd;
		::CloseCursor(); // hqf 2002/1/8
		g_pCurWindow->SendMessage(WM_ACTIVE , 0  ,0);
		g_pCurWindow->SendMessage(WM_PAINT, 0, 0); 
	}
}
////////////////////////////////////////////////////////////////////////////////
//
void HideWindow(CFrameWnd * pWnd)
{
	if(pWnd && pWnd != g_pDeskTop && pWnd == g_pCurWindow)
	{
       	pWnd->SetWindowStyle(pWnd->GetWindowStyle() & (~WS_VISIBLE)); 
		g_pCurWindow =GetActiveWindow(); 
		g_pCurWindow->SendMessage(WM_ACTIVE, 0, 0); //�ߵ��˴���
		g_pCurWindow->SendMessage(WM_PAINT, 0, 0);  //
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void HideWindowNotRedrawBack(CFrameWnd *pWnd)
{
	if(pWnd && pWnd != g_pDeskTop && pWnd == g_pCurWindow)
	{
       	pWnd->SetWindowStyle(pWnd->GetWindowStyle() & (~WS_VISIBLE));
		g_pCurWindow =GetActiveWindow(pWnd);
		g_pCurWindow->SendMessage(WM_ACTIVE, 0, 0);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void DispatchMessage(const SYSTEM_MESSAGE *lpMsg)
{
	  if(lpMsg->handle)
		 ((CWnd*)lpMsg->handle)->SendMessage(lpMsg->message, lpMsg->wparam, lpMsg->lparam); 
	  else
         g_pCurWindow->SendMessage(lpMsg->message, lpMsg->wparam, lpMsg->lparam); 
}

////////////////////////////////////////////////////////////////////////////////
//����Ӳ����

bool DoSystemKeys(UINT32 key)
{
	if(g_iScreenStatus != SS_NORMAL)
		return true;
	NU_Control_Timer(&RecDelayStartTimer, NU_DISABLE_TIMER);	// ��ֹ¼����ʱ��
//	if(g_pCurWindow && g_pCurWindow->GetWndType() == WND_TYPE_DIALOG)
//		return true;
	if(GetCurrentDialogApp()!= -1 && GetCurrentApp() != APPID_DESKTOP 
		&& GetCurrentDialogApp() != GetCurrentApp())
		return true;
//!!!------------12-7, ygr
//	if(key!=KEY_RECORD && key!=KEY_STOP)
	if(key!=KEY_STOP)
		if(GetRecordStatus())
			return true;
		else
			pRec->ShowWindow(SW_HIDE);
//-------------12-7


	/*
	*	bbj, 2001/12/26
	*/
	if ((key != KEY_FASTDIAL) && (g_pCurWindow == g_pPhoneWnd))
		g_pPhoneWnd->SendMessage(WM_KEYUP,KEY_FASTDIAL, 0);
	switch (key){
		case KEY_CATALOG:   //Ӧ���б�
			if (GetCurrentApp() == APPID_CATALOG
				&& g_pCurWindow == g_pDeskTop)
				return false;
			::SetCurrentApp(APPID_CATALOG);//����������������!!!
//			extern void AppListRun();
			if(!ReturnToDeskTop(TRUE))
				return TRUE;
//			AppListRun();
			::SetCurrentDialogApp(APPID_CATALOG);
			return true;
		case KEY_HELP:     //����
			if (GetCurrentApp() == APPID_HELP)
				return false;
			if(g_pCurWindow)
				HelpAppRun(g_pCurWindow->GetHelpID());
//			::SetCurrentDialogApp(APPID_HELP);
//			::MessageBox("������Ϣ��δװ��" , "����" , MB_OK|MB_ICONINFORMATION);
			return true;
		case KEY_CLOCK: // ���棨ʱ�ӣ�
			if (GetCurrentApp() == APPID_CLOCK)
				return false;
			::SetCurrentApp(APPID_CLOCK);//����������������!!!
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			::ClockAppRun();
			::SetCurrentDialogApp(APPID_CLOCK);
			return true;
		case KEY_ALARM: //����
			if (GetCurrentApp() == APPID_ALARM)
				return false;
			::SetCurrentApp(APPID_ALARM);//����������������!!!
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			AlarmAppRun();
			::SetCurrentDialogApp(APPID_ALARM);
			return true;
		case KEY_DOCUMENT: //����
			if (GetCurrentApp() == APPID_DOCUMENT)
				return false;
			::SetCurrentApp(APPID_DOCUMENT);//����������������!!!
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			ReferenceAppRun();
			::SetCurrentDialogApp(APPID_DOCUMENT);
			return true;
		case KEY_SETUP:    //ϵͳ����
			if (GetCurrentApp() == APPID_SETUP)
				return false;
			::SetCurrentApp(APPID_SETUP);//����������������!!!
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			SettingsAppRun();
			::SetCurrentDialogApp(APPID_SETUP);
			return true;
		case KEY_FASTDIAL: //�ٲ�
			//if (GetCurrentApp() == APPID_FASTDIAL)	// bbj ------------- ??? ��һ��
			//	return false;
			//::SetCurrentApp(APPID_FASTDIAL);//����������������!!!
			g_pPhoneWnd->SendMessage(WM_KEYUP,KEY_FASTDIAL, 0);
			//::SetCurrentDialogApp(APPID_FASTDIAL);	// bbj ------------- ??? ��һ��
			return true;
		case KEY_SMS:     //����Ϣ
			if (GetCurrentApp() == APPID_SMS && !g_bIsCanReset)
				return false;
			::SetCurrentApp(APPID_SMS);//����������������!!!
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			SMAppRun();
			::SetCurrentDialogApp(APPID_SMS);
			return true;
		case KEY_HISTORY: //ͨ����¼
			if (GetCurrentApp() == APPID_HISTORY)
				return false;
			::SetCurrentApp(APPID_HISTORY);
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			HistoryAppRun();
			::SetCurrentDialogApp(APPID_HISTORY);
            return true;  
		case KEY_CACULATOR: //������
			if (GetCurrentApp() == APPID_CALCULATOR)
				return false;
			::SetCurrentApp(APPID_CALCULATOR);
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			CalcAppRun();
			::SetCurrentDialogApp(APPID_CALCULATOR);
			return true;
		case KEY_PICTURE: //��ǩ
			if (GetCurrentApp() == APPID_PICTURE)
				return false;
			::SetCurrentApp(APPID_PICTURE);
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			PictureAppRun();
			::SetCurrentDialogApp(APPID_PICTURE);
			return true;
		case KEY_CARD:  //��Ƭ
			if (GetCurrentApp() == APPID_CARD)
				return false;
			::SetCurrentApp(APPID_CARD);
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			CardAppRun();
			::SetCurrentDialogApp(APPID_CARD);

			return true;
		case KEY_RECORD: //¼��
			if(pRec)
				pRec->SendMessage(WM_KEYUP,KEY_RECORD,0);
//			if (GetCurrentApp() == APPID_RECORD)
//				return false;
//			::SetCurrentApp(APPID_RECORD);
//			::SetCurrentDialogApp(APPID_RECORD);
//			ReturnToDeskTop(FALSE);
//			RepeatAppRun();
			return true;
		case KEY_STOP: //¼��ֹͣ
//			if (GetCurrentApp() != APPID_RECORD)
//				return false;
//			::SetCurrentApp(APPID_RECORD);// �����ԣ�����
//			ReturnToDeskTop(FALSE);
			return true;
		case KEY_PLAY: //ĩ������
			extern void AutoPlayNewRecord();
			if (GetCurrentApp() == APPID_RECPLAY) 
				return false;
			::SetCurrentApp(APPID_RECPLAY);
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			AutoPlayNewRecord();
			::SetCurrentDialogApp(APPID_RECPLAY);
			return true;
		case KEY_RECBROWSE: //�鿴����
			if (GetCurrentApp() == APPID_RECBROWSE)
				return false;
			::SetCurrentApp(APPID_RECBROWSE);
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			RecordBrowseAppRun();
			::SetCurrentDialogApp(APPID_RECBROWSE);
			return true;
		case KEY_MEMO:   //����
			if (GetCurrentApp() == APPID_MEMO)
				return false;
			::SetCurrentApp(APPID_MEMO);
			if(!ReturnToDeskTop(FALSE))
				return TRUE;
			MemoAppRun();
			::SetCurrentDialogApp(APPID_MEMO);
			return true;
		case KEY_CALIBRATON: //У׼
			if (GetCurrentApp() == APPID_RECORD || GetCurrentApp()== APPID_CALIBRATON)
				return false;
			::SetCurrentApp(APPID_CALIBRATON);
			void CalibrationPen(void);			
			::SetCurrentDialogApp(APPID_CALIBRATON);
			CalibrationPen();
			::SetCurrentApp();
			//extern void SaveSyatemOption(SYSTEM_OPTION *option);
			extern SYSTEM_OPTION g_SystemOption; 
			SaveSystemOption(&g_SystemOption);
			
			return TRUE;
		case KEY_TEST:  //ץͼ
			extern STATUS UART_Write(char *pData, int DataLen);
			UART_Write((char*)0x00180000, 6400);
			return true;
		case KEY_SUPERUSER:  //�����û�
//			SystemSleep();
			extern void ShowDebugInfo();
			ShowDebugInfo();
			return true;
	}
	return false;
}

/*
   �����������������ʱ������������Ϣ
   ����ֵ��TRUE��ʾ�˳������ɹ�
           FALSE��ʾ�˳��������ɹ�
*/

extern BOOL g_bPowerOn;
BOOL HandleExitScrProt(const SYSTEM_MESSAGE *lpMsg)
{
	if(g_bPowerOn )
		return FALSE;
	if(g_iScreenStatus == SS_LOCKED)
	{
		if(g_SystemOption.strScreenPassword[0] == '\0')
		{
			ExitScrProt();
			if (lpMsg->message != WM_PENUP)
				PostMessageToQueue((SYSTEM_MESSAGE *)lpMsg);
		}
		else 
		{
			g_iScreenStatus = SS_UNLOCKING;
			char strBuf[10];
			strcpy(strBuf, g_SystemOption.strScreenPassword);
			if(lpMsg->message == WM_MICOPEN || lpMsg->message == WM_PICKUP 
				|| lpMsg->message == WM_RING)
			{
				AutoOpenBackLight();		// auto open backlight
				if(g_pPhoneWnd)
				{
					g_pPhoneWnd->SendMessage(lpMsg->message, lpMsg->wparam, lpMsg->lparam); 
					if(pRec && lpMsg->message== WM_MICOPEN)//ygr
						pRec->SendMessage(lpMsg->message, lpMsg->wparam, lpMsg->lparam);
				}
			}
			if(::InputPassword(strBuf , 9 , INPUT_PSWD , "��������������")== IDOK)
			{
				ExitScrProt();
				if (lpMsg->message != WM_PENUP && lpMsg->message != WM_MICOPEN &&
					lpMsg->message != WM_PICKUP && lpMsg->message != WM_RING)
					PostMessageToQueue((SYSTEM_MESSAGE *)lpMsg);
			}
			else
				g_iScreenStatus = SS_LOCKED;
		}
		return TRUE;
	}
	else if(g_iScreenStatus == SS_UNLOCKING &&(lpMsg->message>= WM_KEY
		&& lpMsg->message <= WM_KEYKEEP ))//||( lpMsg->message == WM_MICOPEN ||lpMsg->message == WM_PICKUP)
		return TRUE;
	return FALSE;
}


BOOL TranslateMessage(const SYSTEM_MESSAGE *lpMsg)
{
    switch(lpMsg->message)
	{
	case WM_MICOPEN:   //����ժ�� 
	case WM_PICKUP:    //�ֱ�ժ��
	case WM_RING:      //���� wParam ָ��CallId���ַ���ָ��
		{
			::StopRing(FALSE);
			if(HandleExitScrProt(lpMsg))
				return TRUE;
		}
	case WM_PICKDOWN:  //�ֱ��һ�
	case WM_MICCLOSE:  //����һ� 
	case WM_FLASH:     //����
		{
		if(GetRecordStatus())//�绰�ر�¼��//ygr
			pRec->SendMessage(WM_RECORD_END, 0,0); 
		if(GetReplayStatus() )
			StopPlay();
		}
	case WM_SPEAKINGPICKUP:
	case WM_DIAL:      //����,����������ժ��״̬ʱ�İ���, wParam�����8bitΪ���ĺ���
	case WM_REDIAL:    //�ز� 
	case WM_MUTESTART: //������ʼ
	case WM_MUTEEND:   //��������
	case WM_SOFTDIAL:
	case WM_RINGEND:   //������� 
		{
		AutoOpenBackLight();		// auto open backlight
        if(g_pPhoneWnd)
		{
			g_pPhoneWnd->SendMessage(lpMsg->message, lpMsg->wparam, lpMsg->lparam); 
			if(pRec &&  (lpMsg->message==WM_MICOPEN ||
					lpMsg->message==WM_SPEAKINGPICKUP))//ygr
				pRec->SendMessage(lpMsg->message, lpMsg->wparam, lpMsg->lparam);
			return TRUE;
		}

		break;
		}
	case WM_AUTOREC_START:
		{
		if(WAVE_STORE_LEN-GetWaveUsed()<=7*8192 ||GetPhoneStatus())//���Կռ���
				return true;
		if(g_pPhoneWnd)
			g_pPhoneWnd->SendMessage(WM_RINGEND, 0, 0);
		SetSoftMicFlag(0);
		if(pRec)
			pRec->SendMessage(WM_RECORD_START, RECORD_AUTO, lpMsg->lparam);
		return true;
		}
	case WM_RECORD_START:
		if(pRec)
		{
			pRec->SendMessage(WM_RECORD_START, lpMsg->wparam, lpMsg->lparam);
			return true;
		}
		break;
	case WM_SOUND_CLOSE:
		{
		if(g_pAlarmDisplay)
			g_pAlarmDisplay->SendMessage(WM_SOUND_CLOSE, lpMsg->wparam, lpMsg->lparam);
		if(g_pCurWindow && g_pCurWindow!= g_pAlarmDisplay)
			g_pCurWindow->SendMessage(WM_SOUND_CLOSE, lpMsg->wparam, lpMsg->lparam);
		break;
		}
	case WM_REPLAY_END:
		if(pRec)
			pRec->SendMessage(lpMsg->message, lpMsg->wparam, lpMsg->lparam);
	case WM_RECORD_END:
		if(g_pAlarmDisplay)
			g_pAlarmDisplay->SendMessage(lpMsg->message, lpMsg->wparam, lpMsg->lparam);
		break;
		
	case WM_RTC:
		{
		AutoCloseBackLight();	// backlight auto power down
		if(g_pPhoneWnd)
			g_pPhoneWnd->SendMessage(WM_RTC , lpMsg->wparam , lpMsg->lparam);
		if(pRec)
			pRec->SendMessage(WM_RTC , lpMsg->wparam , lpMsg->lparam);
		CFrameWnd	*pwnd = ::GetActiveWindow();
		if(pwnd && pwnd != g_pPhoneWnd && pwnd != pRec)
			pwnd->SendMessage(WM_RTC , lpMsg->wparam , lpMsg->lparam);
		
		if(g_pAlarmDisplay)//cxd 12-31
			g_pAlarmDisplay->SendMessage(WM_RTC ,lpMsg->wparam , lpMsg->lparam);
		
		return TRUE;
		}
	case WM_ALARM: //������Ϣ
		{
		AutoOpenBackLight();		// auto open backlight
		//DoWithAlarm();
		if(g_pAlarmDisplay)
			g_pAlarmDisplay->SendMessage(WM_ALARM , 0, 0);
		if(HandleExitScrProt(lpMsg))   // hqf 2002-1-26
			return TRUE;
        return TRUE;
		}
	case WM_KEY:
		{
		if(g_iScreenStatus != SS_NORMAL)
			return TRUE;
		if(GetCurrentDialogApp()!= -1 && GetCurrentApp() != APPID_DESKTOP 
			&& GetCurrentDialogApp() != GetCurrentApp())
			return TRUE;

		if ((lpMsg->wparam == KEY_RECORD) && !GetRecordStatus())
		{
			if(g_bIsCanReset && GetCurrentApp() == APPID_SMS)
				return TRUE;
			if(pRec)
				pRec->SendMessage(WM_KEY,lpMsg->wparam , lpMsg->lparam);
			NU_Control_Timer(&RecDelayStartTimer, NU_DISABLE_TIMER);	// ����¼����ʱ��
			NU_Reset_Timer(&RecDelayStartTimer, RecDelayTimerProccess, 100, 100, NU_ENABLE_TIMER);
			return TRUE;
		}
        break;
		}
	case WM_KEYKEEP:
		if(g_iScreenStatus != SS_NORMAL)
			return TRUE;
		break;
	case WM_KEYUP:{
		AutoOpenBackLight();		// auto open backlight
		if(HandleExitScrProt(lpMsg))
			return TRUE;
		return DoSystemKeys(lpMsg->wparam);
	}
	case WM_PENDOWN : case WM_PENMOVE: 
	{
		AutoOpenBackLight();		// auto open backlight
		break;
	}
	case WM_PENUP:
		if(HandleExitScrProt(lpMsg))
			return TRUE;
		break;
	
	case WM_QUITSMS:
	{

		// TODO: ����Ϣģʽ�°�����,��������Ϣ���ڴ���
		if(g_bIsCanReset && GetCurrentApp() == APPID_SMS)
		{
			ReturnToDeskTop(TRUE);
			SetCurrentApp(APPID_CATALOG);
		}
		break;
	}
	}
	return FALSE;
}


/*===========================================================================*/
//  ������                                                                   //
/*===========================================================================*/
#include "listview.h"
class CDesktopButton : public CIconButton
{
public:
	virtual void OnPaint(CDC &dc);
};
void CDesktopButton::OnPaint(CDC &dc)
{
	dc.PutIcon(m_sRect.left+1, m_sRect.top, m_nIconId);
}


class CDeskTop:public CFrameWnd
{
protected:
	CDesktopButton  m_oMainButtons[12];
	//CListView  m_oMainView;
public:
    DECLARE_MESSAGE_MAP(CDeskTop)
	MESSAGE_HANDLE void OnListView(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	virtual BOOL OnCreate();
	virtual void OnPaint(CDC &dc);
};
#define  ID_APP1  62920
////////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CDeskTop, CFrameWnd)
  ON_MESSAGE_RANGE(ID_APP1, ID_APP1+12, CN_COMMAND, OnListView)
END_MESSAGE_MAP 
////////////////////////////////////////////////////////////////////////////////
//
#include "messagebox.h"
/*
                         ICON_CARD,     ICON_MEMORY, ICON_PAINT,
		                 ICON_HISTORY,  ICON_ALARM,  ICON_RECORD, 
						 ICON_CALENDAR, ICON_CALC,   ICON_GAME
						 */
BOOL SetCallIdMode(BOOL);
void CDeskTop::OnListView(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	switch(nSrcId - ID_APP1)
	{
	case 0:
		CardAppRun();
		::SetCurrentDialogApp(APPID_CARD);
		break;
	case 1: 

//		MessageBox("����Ϣ������δ��ͨ","����Ϣ", MB_OK|MB_ICONINFORMATION);
		SMAppRun();
		::SetCurrentDialogApp(APPID_SMS);
		break;
	case 2: 
		HistoryAppRun();
		::SetCurrentDialogApp(APPID_HISTORY);
		break;
	case 3:
		AlarmAppRun();
		::SetCurrentDialogApp(APPID_ALARM);
		break; 
	case 4:
		MemoAppRun();
		::SetCurrentDialogApp(APPID_MEMO);
		break;
	case 5:
		PictureAppRun();
		::SetCurrentDialogApp(APPID_PICTURE);
    	break;
	case 6:
		 ClockAppRun();
		 ::SetCurrentDialogApp(APPID_CLOCK);
		 break;
	case 7:
		RecordBrowseAppRun();
		::SetCurrentDialogApp(APPID_RECBROWSE);
		break;
	case 8:
   		CalcAppRun();
		::SetCurrentDialogApp(APPID_CALCULATOR);
		break;
	case 9:
		ReferenceAppRun();
		::SetCurrentDialogApp(APPID_DOCUMENT);
		break;
	case 10:
		SettingsAppRun();
		::SetCurrentDialogApp(APPID_SETUP);
		break;
	case 11:
		if (g_SystemOption.PowerDownTime == 0)
			MessageBox("���������á�ʹ����Ļ������������Ч��","��ʾ", MB_OK|MB_ICONINFORMATION);
		else
			ScrProtectAppRun();
//		MessageBox("����, ������չ����","����", MB_OK|MB_ICONINFORMATION);
		break;
	}
//	Destroy();
 }

void CDeskTop::OnPaint(CDC &dc)
{
	::CloseCursor();
	CFrameWnd::OnPaint(dc);
}
	
CDeskTop * g_pAppList=NULL;
void AppListRun()
{
	if(!g_pAppList)
		g_pAppList = new CDeskTop;
	::SetCurrentApp(APPID_CATALOG);
	g_pAppList->Create("", WS_VISIBLE, DESK_TOP_RECT, HELP_MAIN); 
	g_pAppList->ShowWindow();  


}

////////////////////////////////////////////////////////////////////////////////
//
BOOL CDeskTop::OnCreate()
{
#define WIDTH   (160/3)
#define	HEIGHT  (160/4)
	//char *text[]={"��Ƭ", "����", "����", "ͨ����¼", "��Ϸ", "����", 
	//	         "ϵͳ����", "Ӧ��1", "Ӧ��2", "Ӧ��3", "Ӧ��4", "Ӧ��5"};
	UINT32 aMainIcon[12]={
		ICON_DESKTOP_CARDCASE,
		ICON_DESKTOP_SMS,
		ICON_DESKTOP_HISTROY,
		ICON_DESKTOP_ALARM,
		ICON_DESKTOP_MEMO,
		ICON_DESKTOP_PICTURE,
		ICON_DESKTOP_CLOCK,
		ICON_DESKTOP_RECORD_BROWSE,
		ICON_DESKTOP_CALCULATOR,
		ICON_DESKTOP_DOCUMENT,
		ICON_DESKTOP_SETTINGS,
		ICON_DESKTOP_LOCKSCREEN,
	};

	CRect r(0,0, WIDTH-1, HEIGHT-1);
	int k=0;
	for(int i=0; i<4; i++)
	{
		r.left =0;
		r.right = r.left + WIDTH-1;
		for(int j=0; j<3; j++)
		{
			m_oMainButtons[k].Create(0, aMainIcon[k], r, this, ID_APP1 + k);
			k++;
			r.left = r.right +1;
			r.right = r.left + WIDTH -1;
		}
		r.top  = r.bottom +1;
		r.bottom = r.top +HEIGHT-1;
	}
	/*
	m_oMainView.Create(m_sRect, this, 503);
	m_oMainView.AppendItem("��Ƭ",     ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("����",     ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("����",     ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("ͨ����¼", ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("��Ϸ",     ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("����",     ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("ϵͳ����", ICON_ALBUM, NULL, ICON_SMALLALBUM); 

	m_oMainView.AppendItem("Ӧ��1", ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("Ӧ��2", ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("Ӧ��3", ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("Ӧ��4", ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("Ӧ��5", ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	m_oMainView.AppendItem("Ӧ��6", ICON_ALBUM, NULL, ICON_SMALLALBUM); 
	*/
#undef WIDTH
#undef HEIGHT
	return TRUE;
}

void InitWndSystem()
{
  
	g_pWindowList = NULL; 
    g_pCurWindow  = NULL; 
	g_pDeskTop =  new CDeskTop;
//	g_pDeskTop =  new CClockApp;

	extern NU_TIMER  RecDelayStartTimer;
	extern void RecDelayTimerProccess(UNSIGNED);
	STATUS status = NU_Create_Timer (&RecDelayStartTimer, "Record Timer", RecDelayTimerProccess, 122, 300, 300, NU_DISABLE_TIMER);

    g_pDeskTop->Create(NULL, WS_VISIBLE, DESK_TOP_RECT, HELP_MAIN); 
	extern void InitPhoneApp();
    InitPhoneApp();
    g_pDeskTop->ShowWindow(); 
	extern void ScrProtectAppRun();
	if(g_SystemOption.strScreenPassword[0] != '\0')
		ScrProtectAppRun();
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
//
void GameAppRun()
{
	//��Ϸ
	MessageBox("��Ϸ","��Ϸ", MB_OK|MB_ICONINFORMATION);
}
////////////////////////////////////////////////////////////////////////////////
//
void ToolAppRun()
{
	//����
	MessageBox("����", "����", MB_OK|MB_ICONINFORMATION);
}

////////////////////////////////////////////////////////////////////////////////
//end wndmanage.cpp