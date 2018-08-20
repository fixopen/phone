// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\MultimediaPhoneDlg.h"
#include "..\multimediaphone.h"
#include "PinInputDlg.h"
#include "../Util/ATCommandWarp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg dialog


CPinInputDlg::CPinInputDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPinInputDlg::IDD, pParent)
{
	m_nType = 0;
}


void CPinInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPinSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPinInputDlg, CCEDialog)
	//{{AFX_MSG_MAP(CPinSetDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPinSetDlg message handlers
void CPinInputDlg::OnClickMJPG(WPARAM w, LPARAM l)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch(w)
	{
		case 41:
		case 42:
			break;

		case 1000://取消
			if (!main->m_bSearchNetOver)
			{
				return;
			}
			main->m_pSettingDlg->m_pPinSetDlg->SetPinStatus();
			ShowWindow_(SW_HIDE);
			break;

		case 1001://确定
			OnbtnOK();
			break;
	}
}

void CPinInputDlg::SetErrorCount(int nCount)
{
	if(nCount > 0)
	{
		m_nErrorCount = nCount;
		char temp[24]; 
		sprintf(temp, "剩余%d次", nCount);
		CString s = temp;
		m_MJPGList.SetUnitIsShow(30, TRUE, FALSE);
		m_MJPGList.SetUnitText(30, s, TRUE);
	}
}

BOOL CPinInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int xtop = 190 ;
	int ytop = 105 ;
	int xbegin = 185 ;
	int ybegin = 94 ;
	int width = 200, height = 36;
	int internal = 19;
	
	ybegin = ytop+ybegin;
	m_edt1Number.Create(WS_CHILD|WS_VISIBLE, CRect(xtop+xbegin, ybegin, xtop+xbegin+width, ybegin+height), this, IDC_EDIT_PINNUMBER);
	m_edt1Number.SetLimitText(8);
	m_edt1Number.SetLimitDiagital();
	
	ybegin = ybegin+internal+height ;
	m_edt2Number.Create(WS_CHILD|WS_VISIBLE, CRect(xtop+xbegin, ybegin, xtop+xbegin+width, ybegin+height), this, IDC_EDIT_PINNUMBER1);
	m_edt2Number.SetLimitText(8);
	m_edt2Number.SetLimitDiagital();
	
	ybegin = ybegin+internal+height ;
	m_edt3Number.Create(WS_CHILD|WS_VISIBLE, CRect(xtop+xbegin, ybegin, xtop+xbegin+width, ybegin+height), this, IDC_EDIT_PINNUMBER2);
	m_edt3Number.SetLimitText(8);
	m_edt3Number.SetLimitDiagital();

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(xtop, ytop, xtop+440, ytop+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\Pin码输入.xml");
	m_MJPGList.SetMJPGRect(CRect(xtop, ytop, xtop+440, ytop+270));

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CPinInputDlg::OnbtnOK()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch (m_nType)
	{	
		case 1://pin加锁还是解锁
			{
				CString pin;
				m_edt2Number.GetWindowText(pin);
				
				//判断PIN码的长度
				if (pin.GetLength() < 4)
				{	
					main->m_pWarningNoFlashDlg->SetTitle(L"PIN码至少4位");
					main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
					return;
				}
				//

				bool b1 = main->m_pSettingDlg->m_pPinSetDlg->m_MJPGList.GetUnitIsDownStatus(41);//PIN关掉
				if (!b1)
				{
					main->m_pATCommandWarp1->AppUnlockSim(Util::StringOp::FromCString(pin).c_str());

				}
				else
				{
					main->m_pATCommandWarp1->AppLockSim(Util::StringOp::FromCString(pin).c_str());
				}
			}
			break;
		case 2://修改Pin码
			{
				 CString pin;
				 CString newPin,RepPin;
				 m_edt1Number.GetWindowText(pin);
				 m_edt2Number.GetWindowText(newPin);
				 m_edt3Number.GetWindowText(RepPin);
				 if (newPin != RepPin )//两次输入的PIN码不相同
				 {
					 main->m_pDeleteTipDlg->SetTitle(L"PIN码不一致，请重新输入");
					 main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					 return ;
				 }
				 main->m_pATCommandWarp1->AppChangePin(Util::StringOp::FromCString(pin).c_str(),
					 Util::StringOp::FromCString(newPin).c_str());

			}
			break;

		case 3://puk输入
			{
				 CString puk;
				 CString number1,number2;
				 m_edt1Number.GetWindowText(puk);
				 m_edt2Number.GetWindowText(number1);
				 m_edt3Number.GetWindowText(number2);
				 if (puk.IsEmpty() || number2.IsEmpty() || number1.IsEmpty())
				 {	
					return;
				 }

				 if (number1 != number2 )//两次输入的PIN码不相同
				 {
					main->m_pDeleteTipDlg->SetTitle(L"PIN码不一致，请重新输入");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					return ;
				 }
				
				 main->m_pATCommandWarp1->AppInputPuk(Util::StringOp::FromCString(puk).c_str(),
					 Util::StringOp::FromCString(number1).c_str());
				 				
				 //add by qi 20100909
				 if (!main->m_bSearchNetOver)
				 {
					return ;
				 }
				 main->m_pSettingDlg->m_pPinSetDlg->SetPinStatus();
			}
			break;
		case 4:
			{
				 CString pin2;
				 CString number1,number2;
				 m_edt1Number.GetWindowText(pin2);
				 m_edt2Number.GetWindowText(number1);
				 m_edt3Number.GetWindowText(number2);
				 if (number1 != number2 )//两次输入的PIN码不相同
				 {
					 main->m_pDeleteTipDlg->SetTitle(L"PIN2码不一致，请重新输入");
					 main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					 return ;
				 }
				 main->m_pATCommandWarp1->AppChangePin2(Util::StringOp::FromCString(pin2).c_str(),
					 Util::StringOp::FromCString(number1).c_str());

			}
			break;

		case 5://puk2输入
			{
				CString puk;
				CString number1,number2;
				m_edt1Number.GetWindowText(puk);
				m_edt2Number.GetWindowText(number1);
				m_edt3Number.GetWindowText(number2);
				if (number1 != number2 )//两次输入的PIN码不相同
				{
					main->m_pDeleteTipDlg->SetTitle(L"PIN码不一致，请重新输入");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					return ;
				}
				main->m_pATCommandWarp1->AppInputPuk2(Util::StringOp::FromCString(puk).c_str(),
					 Util::StringOp::FromCString(number1).c_str());
			}
			break;

		case 6://pin输入
			{
				CString pin;
				m_edt2Number.GetWindowText(pin);
				main->m_pATCommandWarp1->AppInputPin(Util::StringOp::FromCString(pin).c_str());	

			}
			break;

		case 7://FD开关锁
			{
				CString pwd;
				m_edt2Number.GetWindowText(pwd);
				bool b1 = main->m_pSettingDlg->m_MJPGList.GetUnitIsDownStatus(216);//FD关掉
				if (b1)
				{
					main->m_pATCommandWarp1->DSACT(0,(char *)Util::StringOp::FromCString(pwd).c_str());
					
				}
				else
				{
					main->m_pATCommandWarp1->DSACT(1,(char *)Util::StringOp::FromCString(pwd).c_str());
				}

			}
			break;

		case 8://PIN2输入
			{
				CString pin2;
				m_edt2Number.GetWindowText(pin2);
				main->m_pATCommandWarp1->AppInputPin2(Util::StringOp::FromCString(pin2).c_str());	
			}
			break;
	}
	ShowWindow_(SW_HIDE);
}

void CPinInputDlg::ShowPinLockView()
{
	//输入PIN码
	m_nType = 1;
	EditClear();
	m_edt1Number.ShowWindow(SW_HIDE);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_HIDE);

	m_MJPGList.SetUnitText(2,L"请输入PIN码",true);
	
	m_MJPGList.SetUnitIsShow(2,true,true);
	m_MJPGList.SetUnitIsShow(1,false,true);
	m_MJPGList.SetUnitIsShow(3,false,true);

	m_edt1Number.SetPasswordChar('*');
	m_edt2Number.SetPasswordChar('*');
	m_edt3Number.SetPasswordChar('*');
	
}

void CPinInputDlg::ShowPukInputView()
{
	m_nType = 3;
	EditClear();

	m_MJPGList.SetUnitText(1,L"请输入PUK",true);
	m_MJPGList.SetUnitText(2,L"新的PIN码",true);
	m_MJPGList.SetUnitText(3,L"重复PIN码",true);

	m_MJPGList.SetUnitIsShow(1,true,true);
	m_MJPGList.SetUnitIsShow(2,true,true);
	m_MJPGList.SetUnitIsShow(3,true,true);

	m_edt1Number.ShowWindow(SW_SHOW);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_SHOW);

	m_edt1Number.SetPasswordChar('*');
	m_edt2Number.SetPasswordChar('*');
	m_edt3Number.SetPasswordChar('*');
}

void CPinInputDlg::ChangePinView()
{	
	//改变PIN码
	m_nType = 2;
	EditClear();
	m_MJPGList.SetUnitText(1,L"输入PIN码",true);
	m_MJPGList.SetUnitText(2,L"新的PIN码",true);
	m_MJPGList.SetUnitText(3,L"重复PIN码",true);
	
	m_MJPGList.SetUnitIsShow(1,true,true);	
	m_MJPGList.SetUnitIsShow(2,true,true);
	m_MJPGList.SetUnitIsShow(3,true,true);

	m_edt1Number.ShowWindow(SW_SHOW);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_SHOW);

	m_edt1Number.SetPasswordChar('*');
	m_edt2Number.SetPasswordChar('*');
	m_edt3Number.SetPasswordChar('*');

}

void CPinInputDlg::ChangePin2View()
{
	//修改PIN2码
	m_nType = 4;
	EditClear();
	m_MJPGList.SetUnitText(1,L"输入PIN2码",true);
	m_MJPGList.SetUnitText(2,L"新的PIN2码",true);
	m_MJPGList.SetUnitText(3,L"重复PIN2码",true);
	
	m_MJPGList.SetUnitIsShow(1,true,true);	
	m_MJPGList.SetUnitIsShow(2,true,true);
	m_MJPGList.SetUnitIsShow(3,true,true);
	
	m_edt1Number.ShowWindow(SW_SHOW);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_SHOW);
	
	m_edt1Number.SetPasswordChar('*');
	m_edt2Number.SetPasswordChar('*');
	m_edt3Number.SetPasswordChar('*');
}

void CPinInputDlg::ShowPuk2InputView()
{
	m_nType = 5;
	EditClear();

	m_MJPGList.SetUnitText(1,L"请输入PUK2",true);
	m_MJPGList.SetUnitText(2,L"新的PIN2码",true);
	m_MJPGList.SetUnitText(3,L"重复PIN2码",true);
	
	m_MJPGList.SetUnitIsShow(1,true,true);
	m_MJPGList.SetUnitIsShow(2,true,true);
	m_MJPGList.SetUnitIsShow(3,true,true);
	
	m_edt1Number.ShowWindow(SW_SHOW);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_SHOW);
	
	m_edt1Number.SetPasswordChar('*');
	m_edt2Number.SetPasswordChar('*');
	m_edt3Number.SetPasswordChar('*');
}

void CPinInputDlg::ShowPinInputView()
{
	//输入PIN码
	m_nType = 6;
	EditClear();
	m_edt1Number.ShowWindow(SW_HIDE);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_HIDE);
	
	m_MJPGList.SetUnitText(2,L"请输入PIN码",true);
	
	m_MJPGList.SetUnitIsShow(2,true,true);
	m_MJPGList.SetUnitIsShow(1,false,true);
	m_MJPGList.SetUnitIsShow(3,false,true);
	
	m_edt1Number.SetPasswordChar('*');
	m_edt2Number.SetPasswordChar('*');
	m_edt3Number.SetPasswordChar('*');
}

void CPinInputDlg::ShowPin2InputView()
{
	//输入密码
	EditClear();
	m_edt1Number.ShowWindow(SW_HIDE);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_HIDE);
	
	m_MJPGList.SetUnitText(2,L"请输入PIN2",true);
	
	m_MJPGList.SetUnitIsShow(2,true,true);
	m_MJPGList.SetUnitIsShow(1,false,true);
	m_MJPGList.SetUnitIsShow(3,false,true);
	
	m_edt1Number.SetPasswordChar('*');
	m_edt2Number.SetPasswordChar('*');
	m_edt3Number.SetPasswordChar('*');
}

void CPinInputDlg::EditClear()
{
	m_edt1Number.SetWindowText(L"");
	m_edt2Number.SetWindowText(L"");
	m_edt3Number.SetWindowText(L"");

	m_MJPGList.SetUnitIsShow(30, FALSE, FALSE);
}