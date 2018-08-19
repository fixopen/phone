// PSTNNumberDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PSTNNumberDlg.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../resource.h"

#define IDC_COMBOBOX_PSTNL		101
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPSTNNumberDlg dialog


CPSTNNumberDlg::CPSTNNumberDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPSTNNumberDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPSTNNumberDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPSTNNumberDlg::DoDataExchange(CDataExchange* pDX)
{
	CCEDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPSTNNumberDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPSTNNumberDlg, CCEDialog)
	//{{AFX_MSG_MAP(CPSTNNumberDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_COMMBOX_CLICKED, OnSelectPSTN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPSTNNumberDlg message handlers

BOOL CPSTNNumberDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();

	int xbegin = 54;
	int ybegin = 52+20;
	
	m_edtName.Create(WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL , CRect(xbegin+235, ybegin+96, xbegin+235+160, ybegin+96+30), this, 0xFFFF);
	
	m_edtNumber.Create(WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL , CRect(xbegin+235, ybegin+144, xbegin+235+160, ybegin+144+30), this, 0xFFFF);
	m_edtNumber.SetLimitDiagital(true);
	m_edtNumber.SetLimitText(13);

	m_cmbPSTNList.Create(WS_CHILD|WS_VISIBLE, CRect(xbegin+25, ybegin+96, xbegin+25+150, ybegin+420), this, IDC_COMBOBOX_PSTNL);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(xbegin, ybegin, xbegin + 692, ybegin+296), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\固话列表.xml");
	m_MJPGList.SetMJPGRect(CRect(xbegin, ybegin, xbegin + 692, ybegin+296));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPSTNNumberDlg::OnClickMJPG(WPARAM w, LPARAM l)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch(w)
	{
	case 1:
		OnbuttonOK();
		main->m_pATCommandWarp1->CPBS("SM");
		break;
	case 2:
		m_nOper = 0;
		main->m_pATCommandWarp1->CPBS("SM");
		ShowWindow_(SW_HIDE);
		break;
	case 20: // 全部删除
		DeleteAll();
		break;
	case 21: // 修改
		Change();
		break;
	case 22: //删除
		Delete();
		break;
	case 23: //新增
		Add();
		break;
	}
}

void CPSTNNumberDlg::OnSelectPSTN(WPARAM w, LPARAM l)
{
	if(w == IDC_COMBOBOX_PSTNL)
	{
		int nSel = m_cmbPSTNList.GetCurSel();
		SetPSTNList(nSel);
		ReadOnly(true);
	}
}

void CPSTNNumberDlg::SetPSTNList(int index)
{	
	if (vFDNList.size() > index)
	{
		m_cmbPSTNList.SetCurSel(index);
		m_edtName.SetWindowText(Util::StringOp::ToCString(vFDNList[index].name));
		m_edtNumber.SetWindowText(Util::StringOp::ToCString(vFDNList[index].telnum));
	}
}

void CPSTNNumberDlg::Change()
{
	ReadOnly(false);
	m_nOper = 1;
}

void CPSTNNumberDlg::Add()
{
	ReadOnly(false);
	m_edtName.SetWindowText(L"");
	m_edtNumber.SetWindowText(L"");
	m_cmbPSTNList.SetCurSel(-1);
	m_cmbPSTNList.SetWindowText_(L"");
	m_nOper = 2;
}

void CPSTNNumberDlg::Delete()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (vFDNList.size())
	{
		m_nOper = 3;
		int cur = m_cmbPSTNList.GetCurSel();
		main->m_pATCommandWarp1->DeleteSim(Util::StringOp::ToInt(vFDNList[cur].id));
	}

}

void CPSTNNumberDlg::OnbuttonOK()
{
	std::vector< Util::ATCommandWarp::SIM_FORMAT > vsim ;
	Util::ATCommandWarp::SIM_FORMAT sim ;
	bool bsave ;
	CString sName;
	CString sTel;
	m_edtName.GetWindowText(sName);
	m_edtNumber.GetWindowText(sTel);
	
	sim.id = "0";
	sim.name = ::Util::StringOp::FromCString(sName);
	sim.telnum = ::Util::StringOp::FromCString(sTel);
		
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(m_nOper == 1)   //修改
	{	
		bsave = false;
		int cur = m_cmbPSTNList.GetCurSel();
		if (cur > -1)
		{
			int index = Util::StringOp::ToInt(vFDNList[cur].id);
			sim.id = Util::StringOp::FromInt(index);
		}
		else
		{
			main->m_pWarningNoFlashDlg->SetTitle(L"没有可更改的项");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		}

	}
	else if(m_nOper == 2)//添加
	{	
		bsave = true;
		sim.id = "0";
	}	
	else
	{
		ShowWindow_(SW_HIDE);
		return;
	}

	vsim.push_back(sim);
	main->m_pATCommandWarp1->Save2Sim(vsim,bsave, TRUE);

}

void CPSTNNumberDlg::ReadOnly(bool b)
{
	m_edtName.SetReadOnly(b);
	m_edtNumber.SetReadOnly(b);
}

void CPSTNNumberDlg::DeleteAll()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
	if (vFDNList.size())
	{	
		m_nOper = 4;
		m_nIndex = 0 ;
		main->m_pATCommandWarp1->DeleteSim(Util::StringOp::ToInt(vFDNList[0].id));
	}
}

void CPSTNNumberDlg::ShowPSTNList()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	vFDNList.clear();
	m_cmbPSTNList.ResetContent();
	m_edtName.SetWindowText(L"");
	m_edtNumber.SetWindowText(L"");
	main->m_pATCommandWarp1->CSCS("UCS2");
	main->m_pATCommandWarp1->CPBS("FD");
}

LRESULT CPSTNNumberDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch (message)
	{
	case WM_CPBS_SELECT: //选择贮存器
		{
			if (wParam)//成功
			{
				//main->m_pATCommandWarp1->CPBS();//查询SIM卡里的名片
				m_nOper = 5;
				main->m_pATCommandWarp1->CPBR(0);

			}
		}
		break;

	case WM_PSTN_LIST:
		{
			std::string *data = (std::string *)wParam ;
			std::string s = *data;
			size_t t ;
			if (s.find("FD") != std::string::npos)
			{
				t = s.find("FD");
				s = s.substr(t);
				t = s.find(",");
				if ( t != std::string::npos)
				{
					s = s.substr(t+1);
				//	m_nContactNum = atoi((char *)s.c_str());
				//	main->m_pATCommandWarp1->CPBR(0);
				}
			}
		}
		break;

	case WM_CPBR_DATA:
		{
			std::string *data = (std::string *)wParam ;
			main->m_pATCommandWarp1->ParseData(data->c_str(),vFDNList);

			for(int i = 0; i < vFDNList.size(); i++)
			{
				m_cmbPSTNList.AddString(Util::StringOp::ToCString(vFDNList[i].id));
				if(i == 0)
				{
					m_cmbPSTNList.SetCurSel(0);
					m_edtName.SetWindowText(Util::StringOp::ToCString(vFDNList[0].name));
					m_edtNumber.SetWindowText(Util::StringOp::ToCString(vFDNList[0].telnum));
				}
			}

			if (vFDNList.size())
			{
				ReadOnly(true);
			}
			
			if (!IsWindowVisible())
			{
				ShowWindow_(SW_SHOW);				
			}

			*data = "";
			main->m_pATCommandWarp1->CSCS("GSM");

		}
		break;
	case WM_CPBR_OK:
		{
			if (5 == m_nOper)
			{	
				if (vFDNList.size())
				{
					m_cmbPSTNList.SetCurSel(0);
				}
				else
				{
					ReadOnly(true);
				}

				if (!IsWindowVisible())
				{
					ShowWindow_(SW_SHOW);
				}
				m_nOper = 0 ;
			}
		}
		break;

	case WM_CPBW_ADD:
		{	
			CString title;
			if ( 2 == m_nOper)
			{	

				if (1 == wParam)
				{
					title = L"添加成功";
				}
				else
				{
					title = L"添加失败";
				}
				main->m_pDeleteTipDlg->SetTitle(title);
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				Sleep(1000);
				ShowPSTNList();
				m_nOper = 0 ;

			}
		}
		break;
	
	case WM_CPBW_CHANGE:
		{	
			CString title;
			if ( 1 == m_nOper)
			{	
				if (1 == wParam)
				{
					title = L"修改成功";
				}
				else
				{
					title = L"修改失败";
				}
				main->m_pDeleteTipDlg->SetTitle(title);
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				Sleep(1000);
				ShowPSTNList();
				m_nOper = 0 ;
			}
		}
		break;

	case WM_CPBW_DELETE:
		{	
			CString title;
			if ( 3 == m_nOper)
			{	
				if (1 == wParam)
				{
					title = L"删除成功";
				}
				else
				{
					title = L"删除失败";
				}
				main->m_pDeleteTipDlg->SetTitle(title);
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				Sleep(1000);
				ShowPSTNList();
				m_nOper = 0 ;

			}
			else if ( 4 == m_nOper)
			{
				if (1 == wParam)
				{						
					if (m_nIndex < vFDNList.size())
					{
						m_nIndex++; 
						main->m_pATCommandWarp1->DeleteSim(Util::StringOp::ToInt(vFDNList[m_nIndex].id));
						break;
					}
					else
					{
						title = L"删除成功";
					}
				}
				else
				{
					title = L"删除成功";
				}

				main->m_pDeleteTipDlg->SetTitle(title);
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
				Sleep(1000);
				ShowPSTNList();
				m_nOper = 0 ;

			}
		}
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
	
}