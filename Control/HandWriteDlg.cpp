// InputDlg.cpp : implementation file
//
#include "stdafx.h"
#include "../resource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhone.h"
#include "HandWriteDlg.h"

#define HANDWRITE_RECT_WIDTH    300 
#define HANDWRITE_RECT_HEIGHT   250

#define INPUTSOFTKEYRECT	CRect(0, 0, HANDWRITE_RECT_WIDTH, HANDWRITE_RECT_HEIGHT)

#define VOIPINPUT_HAND_IDC			0xFC05

#define VOIPINPUT_DELETE_VALUE		VK_BACK		//0xFF00
#define VOIPINPUT_SPACE_VALUE		VK_SPACE	//0xFF03
#define VOIPINPUT_ENTER_VALUE		VK_RETURN	//0xFF04
#define VOIPINPUT_MOVE_LEFT         0xFF03		
#define VOIPINPUT_MOVE_RIGHT        0xFF04		
#define VOIPINPUT_HIDE_VALUE		0xFF05
#define VOIPINPUT_WRITE_VALUE		0xFF06
#define VOIPINPUT_CHANGE_VALUE		0x1000       

#define HOUXUAN_LOCAL				0xFF0B

#define ARROW_LEFT					(HOUXUAN_LOCAL+6)
#define ARROW_RIGHT					(ARROW_LEFT+1)

#define HAND_REG1_VALUE				0xFD00
#define HAND_REG2_VALUE				0xFD01
#define HAND_REG3_VALUE				0xFD02
#define HAND_REG4_VALUE				0xFD03
#define HAND_REG5_VALUE				0xFD04
#define HAND_REG6_VALUE				0xFD05
#define HAND_REG7_VALUE				0xFD06
#define HAND_REG8_VALUE				0xFD07
#define HAND_REG9_VALUE				0xFD08
#define HAND_REG10_VALUE			0xFD09

#define FRESH_HAND_TIMER			1
#define WINDOW_TOP					22
#define HZ_SHOW_LEN					6						

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void FreeWriteLineBuff();
extern void InitWriteLineBuff();
/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog

//////////////////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#include "stdio.h"
#else
#endif


extern void PutIconPartial(HDC pdc, int x, int y, int iNO, CRect r);
extern void PutIconPartial(CDC *pdc, int x, int y, int iNO, CRect r);

CHandWriteDlg::CHandWriteDlg(CWnd* pParent /*=NULL*/)
: CDialog(CHandWriteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset(m_chSxShowBuff, 0, sizeof(m_chSxShowBuff));
	m_pOwnerEdit = NULL;

	m_Font.CreateFont(
		20,                        // nHeight
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
	   _T("宋体"));    
}


void CHandWriteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHandWriteDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDlg)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SOFTKEY, OnSoftKey)
	ON_MESSAGE(WM_SOFTKEY_REG, OnSoftKeyReg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
//	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg message handlers

BOOL CHandWriteDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bDigital = false;
	//设置手写的区域
	CreateCtrl();
	
	MoveWindow(0,WINDOW_TOP,260,250);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHandWriteDlg::SetOwnerEdit(CEdit *pEdit)
{
	m_pOwnerEdit = pEdit;
}

void CHandWriteDlg::CreateCtrl()
{
	int  nSize = 12  ;  
	CRect   r2[12]   ;
	UINT16 vSoftKeyH[] = {HAND_REG1_VALUE, HAND_REG2_VALUE,\
		HAND_REG3_VALUE, HAND_REG4_VALUE,HAND_REG5_VALUE,HAND_REG6_VALUE,
		VOIPINPUT_DELETE_VALUE,VOIPINPUT_SPACE_VALUE,\
		VOIPINPUT_MOVE_LEFT,VOIPINPUT_MOVE_RIGHT,VOIPINPUT_CHANGE_VALUE,\
		VOIPINPUT_HIDE_VALUE,\
	}; //40
	
	int xBegin   = 56 ;
	int yBegin   = 5  ;
	int xWdith   = 33 ;
	int yHeight  = 25 ;
	for(int i = 0; i < nSize; i++)
	{	
		if (i < HZ_SHOW_LEN) //汉字显示区域
		{	
			xWdith   = 33 ;
			yHeight  = 25 ;
			r2[i] = CRect(xBegin,yBegin,xBegin+xWdith,yBegin+yHeight);
			xBegin += xWdith;
		}
		else if (  i >=HZ_SHOW_LEN && i < nSize)
		{
			if (i == HZ_SHOW_LEN)
			{
				yBegin = 0 ;
			}

			if (i == nSize-1)
			{
				yBegin  = 206;
			}
					
			yHeight =  41 ;	
			xWdith  =  48 ;
			xBegin  =  1  ;
			r2[i] = CRect(xBegin,yBegin,xBegin+xWdith,yBegin+yHeight);
			yBegin += yHeight+1;
		}
	}

	if( !(m_oSoftkeyHand.Create(Data::g_inputWriteBMPID[Data::g_skinstyle],\
		CPoint(0,0), nSize, r2, vSoftKeyH, INPUTSOFTKEYRECT, this, 0,\
		VOIPINPUT_HAND_IDC)) )
        return ;
	
	xBegin  = 55;
	yBegin  = 35;
	xWdith  = 200;
	yHeight = 211;
	m_oSoftkeyHand.SetHandWriteRect(CRect(xBegin,\
		yBegin, xBegin+xWdith, yBegin+yHeight), CRect(0, 0, 0, 0));
	m_oSoftkeyHand.ModifyStyle(0, SS_NOTIFY);	

	return ;
}

void CHandWriteDlg::hide(BOOL flag)
{
	FreeWriteLineBuff();
	
	m_pOwnerEdit = NULL;
	m_bIsShow = FALSE;
	ShowWindow(SW_HIDE);
}

void CHandWriteDlg::show(BOOL flag)
{
	InitWriteLineBuff();
	m_bIsShow = TRUE;

	CRect rt;
	GetWindowRect(&rt);
	::SetWindowPos(m_hWnd, HWND_TOPMOST,\
		rt.left, WINDOW_TOP, rt.Width(), rt.Height(), 0);
	ShowWindow(SW_SHOW);

}

void CHandWriteDlg:: hideorshow()
{
	if(m_bIsShow)
		hide();
	else
		show();
}

BOOL CHandWriteDlg::MoveTo(int x)
{
	CRect myRect;
	GetClientRect(&myRect);
	if (x == LEFT)
	{
		MoveWindow(0, WINDOW_TOP, myRect.Width(), myRect.Height(), true);

	}
	else
	{
		MoveWindow(220, WINDOW_TOP, myRect.Width(), myRect.Height(), true);
	}
	//刷新字
	SetTimer(FRESH_HAND_TIMER,100,NULL);
	return TRUE;
}


void CHandWriteDlg::OnSoftKeyHand(NMHDR_*hm, LRESULT*index)
{
	int wParam = hm->wParam;
	if(wParam >= HAND_REG1_VALUE && wParam <= HAND_REG10_VALUE)
	{
		if(m_pOwnerEdit)
		{
			char *ch = m_chSxShowBuff+(wParam - HAND_REG1_VALUE)*2;
			char txt[3];
			txt[0] = *ch;
			txt[1] = *(ch+1);
			txt[2] = 0;

			wchar_t wch[2] = {0};
			memcpy(wch, txt, 2);
		//	memset(wch, 0, sizeof(wch));
		//	mbstowcs( wch, txt, strlen(txt));
			
			int n, n1;
			m_pOwnerEdit->GetSel(n, n1);
			m_pOwnerEdit->SetSel(n-1, n);
			if (m_bDigital)
			{
				if (wParam>=48 && wParam<=57)
				{
					m_pOwnerEdit->ReplaceSel(wch);
				}
			}
			else
			{
			   m_pOwnerEdit->ReplaceSel(wch);
			}
		}
	}
	else
	{
		if(m_pOwnerEdit)
		{
			char txt[3];
			txt[0] = wParam;
			txt[1] = 0;
			txt[2] = 0;

			wchar_t wch[2];
			memset(wch, 0, sizeof(wch));
			mbstowcs( wch, txt, strlen(txt) );

			if(wParam == VK_RETURN || wParam == VK_BACK)
			{
				m_pOwnerEdit->SendMessage(WM_CHAR, wParam, 1);
			}
			else if (m_bDigital)
			{
				if (wParam>=48 &&wParam<=57)
				{
					m_pOwnerEdit->ReplaceSel(wch);
				}
				
			}
			else
			{
				m_pOwnerEdit->ReplaceSel(wch);
			}
		}
	}
}


void CHandWriteDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == FRESH_HAND_TIMER)
	{
		KillTimer(nIDEvent);
		OnPaint();
	}
	CDialog::OnTimer(nIDEvent);
}

void CHandWriteDlg ::OnSoftKey(WPARAM w, LPARAM l)
{
	NMHDR_ hm = {this, CN_COMMAND, w, (UINT32)0};

	if(w == VOIPINPUT_HIDE_VALUE)//hide 
	{
		hide();
		return;
	}
	else if(w == VOIPINPUT_MOVE_LEFT)//move
	{	
		MoveTo(LEFT);		
		return ;
	}
	else if (w == VOIPINPUT_MOVE_RIGHT)
	{	
		MoveTo(RIGHT);
		return;
	}
	else if (w == VOIPINPUT_CHANGE_VALUE)//切换
	{
		//
		theApp.inputDlg_->show();
		theApp.inputDlg_->SetOwnerEdit(m_pOwnerEdit);
		theApp.inputDlg_->SetLimitDiagital(m_bDigital);
		hide();
		return ;
	}
	
	OnSoftKeyHand(&hm, NULL);	
}


void CHandWriteDlg::OnSoftKeyReg(WPARAM w, LPARAM l)
{ 
	memset(m_chSxShowBuff, 0, sizeof(m_chSxShowBuff));	
	memcpy(m_chSxShowBuff, (void *)w, l*2);
	
	if(m_pOwnerEdit)
	{
		char *ch = m_chSxShowBuff;
		char txt[3];
		txt[0] = *ch;
		txt[1] = *(ch+1);
		txt[2] = 0;
		
		wchar_t wch[2] = {0};
		memcpy(wch, txt, 2);
		
		//	memset(wch, 0, sizeof(wch));
		//	mbstowcs( wch, txt, strlen(txt));
		////zzw10820//////////////////
		if (m_bDigital)
		{
			if (w>=48 && w<=57)
			{
				m_pOwnerEdit->ReplaceSel(wch);
			}
		}
		else
		{
			m_pOwnerEdit->ReplaceSel(wch);
		}
		
		m_pOwnerEdit->SetFocus();
		m_pOwnerEdit->Invalidate();
	}
	//Invalidate();
	OnPaint();
}


void CHandWriteDlg::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	CDialog::OnPaint();
    	
	CDC *pDC = m_oSoftkeyHand.GetDC();
	HFONT hOldFont = (HFONT)::SelectObject(pDC->m_hDC,m_Font.m_hObject); 
	CRect r = CRect(m_oSoftkeyHand.m_pKeys[0].sKeyRect.left+1, \
		m_oSoftkeyHand.m_pKeys[0].sKeyRect.top+1,\
		m_oSoftkeyHand.m_pKeys[HZ_SHOW_LEN-1].sKeyRect.right-1,\
		m_oSoftkeyHand.m_pKeys[HZ_SHOW_LEN-1].sKeyRect.bottom-1);

	PutIconPartial(pDC, r.left,r.top, Data::g_inputWriteBMPID[Data::g_skinstyle], r);
	CRect rA[HZ_SHOW_LEN];
	for(int i = 0; i < HZ_SHOW_LEN; i++)
	{
		rA[i] = m_oSoftkeyHand.m_pKeys[i].sKeyRect;
		rA[i].DeflateRect(1,1);
		char txt[3];
		txt[0] = m_chSxShowBuff[i*2];
		txt[1] = m_chSxShowBuff[i*2+1];;
		txt[2] = '\0';

		wchar_t wch[128] = {0};
		memcpy(wch, txt, 2);

	//	memset(wch, 0, sizeof(wch));
	//	mbstowcs( wch, txt, strlen(txt) );
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(wch, rA[i], DT_WORDBREAK | DT_CENTER);	
	}
	//
	::SelectObject(pDC->m_hDC,hOldFont);
	ReleaseDC(pDC);
	
	if(m_pOwnerEdit)
		m_pOwnerEdit->SetFocus();
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CHandWriteDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pOwnerEdit)
		m_pOwnerEdit->SetFocus();

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CHandWriteDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL ret = CDialog::PreTranslateMessage(pMsg);
	if(pMsg->message == WM_LBUTTONDOWN)   
	{         
		if(m_pOwnerEdit)
		{
			m_pOwnerEdit->SetFocus();
			m_pOwnerEdit->Invalidate();
		}
	}
	else if(pMsg->message == WM_LBUTTONUP)   
	{         
		if(m_pOwnerEdit)
		{
			m_pOwnerEdit->SetFocus();
			m_pOwnerEdit->Invalidate();
		}
	}
	return ret;
}
