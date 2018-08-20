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

		case 1000://ȡ��
			if (!main->m_bSearchNetOver)
			{
				return;
			}
			main->m_pSettingDlg->m_pPinSetDlg->SetPinStatus();
			ShowWindow_(SW_HIDE);
			break;

		case 1001://ȷ��
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
		sprintf(temp, "ʣ��%d��", nCount);
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
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\����\\Pin������.xml");
	m_MJPGList.SetMJPGRect(CRect(xtop, ytop, xtop+440, ytop+270));

	return TRUE;  // return TRUE unless you set the focus to a control

}

void CPinInputDlg::OnbtnOK()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch (m_nType)
	{	
		case 1://pin�������ǽ���
			{
				CString pin;
				m_edt2Number.GetWindowText(pin);
				
				//�ж�PIN��ĳ���
				if (pin.GetLength() < 4)
				{	
					main->m_pWarningNoFlashDlg->SetTitle(L"PIN������4λ");
					main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
					return;
				}
				//

				bool b1 = main->m_pSettingDlg->m_pPinSetDlg->m_MJPGList.GetUnitIsDownStatus(41);//PIN�ص�
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
		case 2://�޸�Pin��
			{
				 CString pin;
				 CString newPin,RepPin;
				 m_edt1Number.GetWindowText(pin);
				 m_edt2Number.GetWindowText(newPin);
				 m_edt3Number.GetWindowText(RepPin);
				 if (newPin != RepPin )//���������PIN�벻��ͬ
				 {
					 main->m_pDeleteTipDlg->SetTitle(L"PIN�벻һ�£�����������");
					 main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					 return ;
				 }
				 main->m_pATCommandWarp1->AppChangePin(Util::StringOp::FromCString(pin).c_str(),
					 Util::StringOp::FromCString(newPin).c_str());

			}
			break;

		case 3://puk����
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

				 if (number1 != number2 )//���������PIN�벻��ͬ
				 {
					main->m_pDeleteTipDlg->SetTitle(L"PIN�벻һ�£�����������");
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
				 if (number1 != number2 )//���������PIN�벻��ͬ
				 {
					 main->m_pDeleteTipDlg->SetTitle(L"PIN2�벻һ�£�����������");
					 main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					 return ;
				 }
				 main->m_pATCommandWarp1->AppChangePin2(Util::StringOp::FromCString(pin2).c_str(),
					 Util::StringOp::FromCString(number1).c_str());

			}
			break;

		case 5://puk2����
			{
				CString puk;
				CString number1,number2;
				m_edt1Number.GetWindowText(puk);
				m_edt2Number.GetWindowText(number1);
				m_edt3Number.GetWindowText(number2);
				if (number1 != number2 )//���������PIN�벻��ͬ
				{
					main->m_pDeleteTipDlg->SetTitle(L"PIN�벻һ�£�����������");
					main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					return ;
				}
				main->m_pATCommandWarp1->AppInputPuk2(Util::StringOp::FromCString(puk).c_str(),
					 Util::StringOp::FromCString(number1).c_str());
			}
			break;

		case 6://pin����
			{
				CString pin;
				m_edt2Number.GetWindowText(pin);
				main->m_pATCommandWarp1->AppInputPin(Util::StringOp::FromCString(pin).c_str());	

			}
			break;

		case 7://FD������
			{
				CString pwd;
				m_edt2Number.GetWindowText(pwd);
				bool b1 = main->m_pSettingDlg->m_MJPGList.GetUnitIsDownStatus(216);//FD�ص�
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

		case 8://PIN2����
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
	//����PIN��
	m_nType = 1;
	EditClear();
	m_edt1Number.ShowWindow(SW_HIDE);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_HIDE);

	m_MJPGList.SetUnitText(2,L"������PIN��",true);
	
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

	m_MJPGList.SetUnitText(1,L"������PUK",true);
	m_MJPGList.SetUnitText(2,L"�µ�PIN��",true);
	m_MJPGList.SetUnitText(3,L"�ظ�PIN��",true);

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
	//�ı�PIN��
	m_nType = 2;
	EditClear();
	m_MJPGList.SetUnitText(1,L"����PIN��",true);
	m_MJPGList.SetUnitText(2,L"�µ�PIN��",true);
	m_MJPGList.SetUnitText(3,L"�ظ�PIN��",true);
	
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
	//�޸�PIN2��
	m_nType = 4;
	EditClear();
	m_MJPGList.SetUnitText(1,L"����PIN2��",true);
	m_MJPGList.SetUnitText(2,L"�µ�PIN2��",true);
	m_MJPGList.SetUnitText(3,L"�ظ�PIN2��",true);
	
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

	m_MJPGList.SetUnitText(1,L"������PUK2",true);
	m_MJPGList.SetUnitText(2,L"�µ�PIN2��",true);
	m_MJPGList.SetUnitText(3,L"�ظ�PIN2��",true);
	
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
	//����PIN��
	m_nType = 6;
	EditClear();
	m_edt1Number.ShowWindow(SW_HIDE);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_HIDE);
	
	m_MJPGList.SetUnitText(2,L"������PIN��",true);
	
	m_MJPGList.SetUnitIsShow(2,true,true);
	m_MJPGList.SetUnitIsShow(1,false,true);
	m_MJPGList.SetUnitIsShow(3,false,true);
	
	m_edt1Number.SetPasswordChar('*');
	m_edt2Number.SetPasswordChar('*');
	m_edt3Number.SetPasswordChar('*');
}

void CPinInputDlg::ShowPin2InputView()
{
	//��������
	EditClear();
	m_edt1Number.ShowWindow(SW_HIDE);
	m_edt2Number.ShowWindow(SW_SHOW);
	m_edt3Number.ShowWindow(SW_HIDE);
	
	m_MJPGList.SetUnitText(2,L"������PIN2",true);
	
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