/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             pswd.cpp                                                       */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技密码输入窗口类的源文件                                 */
/*             HanWang Technology E-PhoneII Password Inputput Application     */
/*               class  file												  */
/*                                                                            */
/*  组    成                                                                  */
/*             密码输入窗口定义                                               */
/*                                                                            */
/*  设计说明                                                                  */
/*             略                                                             */
/*                                                                            */
/*  作    者                                                                  */
/*             郝庆丰                                                         */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-09-05                                                     */
/*                                                                            */
/*  修改记录                                                                  */
/*             2001-10-25                                                     */
/*             修改了“回删键”处e理函数的一个bug（OnSoftKey)                 */
/******************************************************************************/
#include "pswd.h"
#include "messagebox.h"

BOOL  CPswdEdit::Create(char *strName, const CRect wndRect, CWnd * pParent, UINT32 dwStyle, 
					UINT32 nCtrlId, INT32 iBuffLen,char * strTitle, CWritePad *pConnectedCtrl)
{
	m_bShowCursor = TRUE;
	return CEdit::Create(strName , wndRect , pParent , dwStyle, nCtrlId, iBuffLen , 
		                  strTitle , pConnectedCtrl);
}

void CPswdEdit::OnPaint(CDC &dc)
{
	CEdit::OnPaint(dc );
	if(!m_bShowCursor)
		::CloseCursor();
}
			

/////////////////////////////////////////////////////////////////
//下面是密码输入窗口类的定义
extern BOOL CalcuRectValue(CRect *pRectArray , int iRows , int iCols , CRect rectTotal , 
					short xInterval , short yInterval);
#define IDC_EDIT1                 431
#define IDC_EDIT2                 432
#define IDC_SOFTKEY               433
#define IDC_OK			          IDOK
#define IDC_CANCEL                IDCANCEL
#define PSWD_LEN                  9

BEGIN_MESSAGE_MAP(CPsWdInput, CDialog)

ON_MESSAGE(IDC_EDIT1 , CN_COMMAND , OnEditClick)
ON_MESSAGE(IDC_EDIT2 , CN_COMMAND , OnEditClick)
ON_MESSAGE(IDC_SOFTKEY , WM_KEY , OnSoftKey)

END_MESSAGE_MAP

BOOL CPsWdInput::Create(char *strName , char *strOrigPswd, UINT32 dwStyle , const CRect &wndRect,
		     UINT32 nHelpId , char flag)
{
	m_cFlag = flag;
	if(strOrigPswd)
		strcpy(m_strOrigPswd , strOrigPswd);
	return CDialog::Create(strName , WS_CAPTION|WS_BORDER|dwStyle , wndRect,TRUE, nHelpId);
		 
}

void CPsWdInput::OnActive()
{
	CDialog::OnActive();
//	::OpenCursor();
	m_pCurEdit->DrawCursor();
}

void CPsWdInput::OnDactive()
{
	CDialog::OnDactive();
	::CloseCursor(); 
}

BOOL CPsWdInput::OnCreate()
{
	if(!CDialog::OnCreate())
		return FALSE;
	CRect r;
	m_strPswd1[0]= m_strPswd2[0]= '\0';
	m_pPswd1[0]= m_pPswd2[0]= '\0';
	if(m_cFlag == INPUT_PSWD)
	{
		r.SetRect(m_sRect.left +2 , m_sRect.top + 52, m_sRect.right - 2 , m_sRect.top + 68);
		if(!m_oEdit1.Create(m_pPswd1 , r, this, WS_VISIBLE|WS_CHILD , 
			 IDC_EDIT1, PSWD_LEN))
		return FALSE;
		m_pCurEdit = &m_oEdit1;
	}
	else
	{
		r.SetRect(m_sRect.left +2 , m_sRect.top + 36, m_sRect.right - 2 , m_sRect.top + 52);
		if(!m_oEdit1.Create(m_pPswd1 , r, this, WS_VISIBLE|WS_CHILD , 
			 IDC_EDIT1, PSWD_LEN))
		return FALSE;
		r.top = r.bottom + 20; r.bottom = r.top + 16;
		if(!m_oEdit2.Create(m_pPswd2 , r, this, WS_VISIBLE|WS_CHILD , 
			 IDC_EDIT2, PSWD_LEN))
		return FALSE;
		m_pCurEdit = &m_oEdit1;
		
	}

	if(m_cFlag == INPUT_PSWD)
		r.top = m_sRect.bottom - 70 , r.bottom = m_sRect.bottom -10;
	else
		r.top = m_sRect.bottom - 61 , r.bottom = m_sRect.bottom-1 ;

	r.left = 1; r.right = 159 ;
	CRect  rectCharKey[40];
    CalcuRectValue(rectCharKey, 4 ,10 , r, 1, 1);
	rectCharKey[37].right = rectCharKey[39].right;
	rectCharKey[37].bottom = rectCharKey[39].bottom;
	UINT16 pLowKeyValue[] = { '1', '2', '3','4', '5', '6','7','8','9','0',
		                  'q','w', 'e','r', 't', 'y', 'u','i','o', 'p',
						  'a' , 's', 'd', 'f', 'g', 'h','j', 'k', 'l', ' ',
						  'z', 'x', 'c', 'v', 'b','n', 'm' ,CE_BACK_UP ,
						};
    if(!m_oLowKey.Create(ICON_PSWDLOWKEY  , 38,  rectCharKey, pLowKeyValue , this, IDC_SOFTKEY))
		return FALSE; 

	r.SetRect(m_sRect.right - WND_TITLE_HEIGHT *2+1 , m_sRect.top , 
		m_sRect.right - WND_TITLE_HEIGHT , m_sRect.top + WND_TITLE_HEIGHT -1);
	m_oBtnOk.Create(NULL , ICON_OK , r , this , IDOK);
//	r.top = r.bottom + 4 , r.bottom = r.top + 20;
//	r.left = m_sRect.left + 24 , r.right = r.left + 40;
//	m_oBtnOk.Create("确定", r, this , IDC_OK );
//	r.left = r.right + 50 , r.right = r.left + 40;
//	m_oBtnCancel.Create("取消", r, this , IDC_CANCEL );
	return TRUE;
}

void CPsWdInput::OnPaint(CDC &dc)
{
	CDialog::OnPaint(dc);
	dc.Reset();
	if(m_cFlag == INPUT_PSWD)
		dc.TextOut(m_sRect.left + 2 , m_sRect.top + WND_TITLE_HEIGHT + 11 , "请输入您的密码:");
	else
	{
		dc.TextOut(m_sRect.left + 2 , m_sRect.top + WND_TITLE_HEIGHT , "请输入您的密码:");
		dc.TextOut(m_sRect.left + 2, m_sRect.top + 53 , "请再次输入您的密码:");
	}
}
void CPsWdInput::OnSoftKey(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
//此处待继续编写。
{
	short len;
	if(lParam!= SHIFT_KEY&& lParam != CE_BACK_UP)
	{
		m_pCurEdit->SendMessage(WM_KEY , '*' , 0);
		if(m_pCurEdit== &m_oEdit1)
		{
			if((len = strlen(m_strPswd1)) < 8)
			{
			   m_strPswd1[len] = (char)lParam;
			   m_strPswd1[len+1]= '\0';
			}
		}
		else
		{
			if((len = strlen(m_strPswd2)) < 8)
			{
			   m_strPswd2[len] = (char)lParam;
			   m_strPswd2[len+1]= '\0';
			}
		}
	}
	else if(lParam == CE_BACK_UP)
	{
		m_pCurEdit->SendMessage(WM_KEY , CE_BACK_UP , 0);
		if(m_pCurEdit == &m_oEdit1)
		{
			if((len = strlen(m_strPswd1)) >0 )
			    m_strPswd1[len-1] = '\0';
		}
		else
		{
			if((len = strlen(m_strPswd2)) >0 )
			    m_strPswd2[len-1] = '\0';
		}
	}
	
	/* internal debug interface */
	extern void _password_filter(char *password);
	_password_filter(m_strPswd1);
}

void CPsWdInput::OnEditClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	if(lParam == IDC_EDIT1)
		m_pCurEdit= &m_oEdit1;
	else
		m_pCurEdit = &m_oEdit2;
}

void CPsWdInput::OnOk()
{
	::CloseCursor();
	if(m_cFlag== SET_PSWD)
	{
		if(strcmp(m_strPswd1 , m_strPswd2 ))
		{
			::MessageBox("两次输入密码不一致！", "提示", MB_OK|MB_ICONSTOP);
			if(IsLegalWnd()) // 防止界面刷新混乱
			{
				m_strPswd1[0] = m_strPswd2[0] = '\0';
				m_oEdit2.SetWindowText(m_strPswd2);
				m_oEdit1.SetWindowText(m_strPswd1);
				m_pCurEdit = &m_oEdit1;
			}
			return;
		}
		::MessageBox("密码设置成功！", "提示" , MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		if(strcmp(m_strOrigPswd , m_strPswd1))
		{
			::MessageBox("密码输入错误！", "提示" , MB_OK | MB_ICONSTOP);
			if(IsLegalWnd()) // 防止界面刷新混乱
			{
				m_strPswd1[0] = '\0';
				m_oEdit1.SetWindowText(m_strPswd1);
			}
			return;
		}
	}
	CDialog::OnOk();
}

UINT32  InputPassword(char *passwordBuf , short nBufLen , 
					  char flag, char *strCaption)
{
	CPsWdInput psInput;
	char title[27];
	if(flag&& !strCaption )
		strcpy(title , "输入密码");
	else if(!strCaption)
		strcpy(title , "输入新密码");
	else{
		hwstrncpy(title, strCaption, 26);
		title[26] = '\0';
	}

	psInput.Create(title , passwordBuf , WS_VISIBLE, CRect(0 , 0, 159 , 159 ), HELP_INPUT_PASSWORD , flag);
	if(psInput.DoModal()== IDOK)
	{
		hwstrncpy(passwordBuf , psInput.m_strPswd1, nBufLen-1);
		passwordBuf[nBufLen-1] = '\0';
		return IDOK;
	}
	passwordBuf[0] = '\0';
	return IDCANCEL;
}



