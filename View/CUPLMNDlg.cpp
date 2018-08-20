// RenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "CUPLMNDlg.h"
#include "Pblmember.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg dialog


CUPLMNDlg::CUPLMNDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CUPLMNDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUPLMNDlg::DoDataExchange(CDataExchange* pDX)
{
	CCEDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenameDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


void CUPLMNDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	switch(w)
	{
	case 1:
		{
			OnbuttonOK();
		}
		break;
	case 2:
		ShowWindow_(SW_HIDE);
		break;
	case 20: // 添加
		Add();
		break;
	case 21: // 修改
		Change();
		break;
	case 22: //删除
		Delete();
		break;
	}
}

BEGIN_MESSAGE_MAP(CUPLMNDlg, CCEDialog)
	//{{AFX_MSG_MAP(CRenameDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_COMMBOX_CLICKED, OnSelectUPLMN)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenameDlg message handlers

BOOL CUPLMNDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	
	m_operate = search;

	int xbegin = 54;
	int ybegin = 52;

	int ytop = 81;
	int xtop = 290;
	
	m_cEditOper.Create(WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL , CRect(xbegin+xtop, ybegin+ytop, xbegin+xtop+100, ybegin+ytop+30), this, 0xFFFF);
	
	ybegin += 30 + 10;
	m_cEditGsm_Act.Create(WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL , CRect(xbegin+xtop, ybegin+ytop, xbegin+xtop+100, ybegin+ytop+30), this, 0xFFFF);
	m_cEditGsm_Act.SetLimitDiagital(true);
	m_cEditGsm_Act.SetLimitText(1);

	ybegin += 30 + 10;
	m_cEditGsm_Compact.Create(WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL , CRect(xbegin+xtop, ybegin+ytop, xbegin+xtop+100, ybegin+ytop+30), this, 0xFFFF);
	m_cEditGsm_Compact.SetLimitDiagital(true);
	m_cEditGsm_Compact.SetLimitText(1);

	ybegin += 30 + 10;
	m_cEditUtran_Act.Create(WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL , CRect(xbegin+xtop, ybegin+ytop, xbegin+xtop+100, ybegin+ytop+30), this, 0xFFFF);
	m_cEditUtran_Act.SetLimitDiagital(true);
	m_cEditUtran_Act.SetLimitText(1);
	
	ybegin = 52;
	m_cUPLMNList.Create(WS_CHILD|WS_VISIBLE, CRect(xbegin+40, ybegin+ytop, xbegin+40+140, ybegin+400), this, IDC_COMBOBOX_UPLMN);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(xbegin, ybegin, xbegin + 692, ybegin+296), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\Uplmn列表.xml");
	m_MJPGList.SetMJPGRect(CRect(xbegin, ybegin, xbegin + 692, ybegin+296));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUPLMNDlg::ShowUplmn()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (m_vUplmnList.size())
	{
		m_vUplmnList.clear();
	}
	m_cUPLMNList.ResetContent();
	char ans[1024] = {0};
	main->m_pATCommandWarp1->UPLMN_Search();

}

void CUPLMNDlg::ParseUPLMN(char *u)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::string s = u ;
	if (s.find("ERROR") != std::string::npos)
	{
		//错误
	}
	else
	{
		while(s.find("+CPOL: ") != std::string::npos)
		{
			size_t t = s.find("+CPOL: ");
			CString c;
			UPLMN up;

			s = s.substr(t+7);
			c = Util::StringOp::ToCString(s);
			up.index = (uchar)atoi(c);//索引
			CString str;
			str.Format(L"%d",up.index);
			m_cUPLMNList.InsertString(-1,str);//插入列表中
			
			t = s.find(',');
			if (t != std::string::npos)
			{
				s = s.substr(t+1);
				c = Util::StringOp::ToCString(s);
				up.format = (uchar)atoi(c);//类型
				
				t = s.find(',');
				if (t != std::string::npos)
				{
					s = s.substr(t+1);

					t = s.find(',');
					if (t != std::string::npos)
					{
						std::string s1;
						s1 = s.substr(1,t-2);
						up.oper = s1;//网络运营商
						s = s.substr(1);

						t = s.find(',');
						if (t != std::string::npos)
						{
							s = s.substr(t+1);
							c = Util::StringOp::ToCString(s);
							up.gsm_act = (uchar)atoi(c);//gsm_act

							t = s.find(',');
							if (t != std::string::npos)
							{
								s = s.substr(t+1);
								c = Util::StringOp::ToCString(s);
								up.gsm_compact = (uchar)atoi(c);//gsm_compact
								
								t = s.find(',');
								if (t != std::string::npos)
								{
									s = s.substr(t+1);
									c = Util::StringOp::ToCString(s);
									up.utran_act = (uchar)atoi(c);//类型

								}
							}
							
						}
					}
						

				}
			}

			m_vUplmnList.push_back(up);//添加到列表
			
		}
	}
}


void CUPLMNDlg::OnSelectUPLMN(WPARAM w, LPARAM l)
{
	if(w == IDC_COMBOBOX_UPLMN)
	{
		int nSel = m_cUPLMNList.GetCurSel();
		SetUplmn(nSel);
		ReadOnly(true);
	}
}

void CUPLMNDlg::SetUplmn(int index)
{	
	if (m_vUplmnList.size())
	{
		m_cEditOper.SetWindowText(Util::StringOp::ToCString(m_vUplmnList[index].oper));
		CString c ;
		c.Format(L"%d",m_vUplmnList[index].gsm_act);
		m_cEditGsm_Act.SetWindowText(c);
		
		c.Format(L"%d",m_vUplmnList[index].gsm_compact);
		m_cEditGsm_Compact.SetWindowText(c);

		c.Format(L"%d",m_vUplmnList[index].utran_act);
		m_cEditUtran_Act.SetWindowText(c);
		
		m_cUPLMNList.SetCurSel(index);
	}
}

void CUPLMNDlg::Add()
{
	m_cEditOper.SetWindowText(L"");
	m_cEditGsm_Act.SetWindowText(L"");
	m_cEditGsm_Compact.SetWindowText(L"");
	m_cEditUtran_Act.SetWindowText(L"");
	
	ReadOnly(false);
	m_operate = add;
}

void CUPLMNDlg::Change()
{
	ReadOnly(false);
	m_operate = change;
}

void CUPLMNDlg::Delete()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	int cur = m_cUPLMNList.GetCurSel();
	main->m_pATCommandWarp1->UPLMN_Delete(cur+1);
}

void CUPLMNDlg::OnbuttonOK()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	CString c ;
	CString temp;
	int index ;
	int gsm_act;
	int gsm_compact;
	int urtan_act ;
	
	m_cEditOper.GetWindowText(c);
	m_cEditGsm_Act.GetWindowText(temp);
	gsm_act = atoi(temp);
	
	m_cEditGsm_Compact.GetWindowText(temp);
	gsm_compact = atoi(temp);
	
	m_cEditUtran_Act.GetWindowText(temp);
	urtan_act = atoi(temp);

	switch (m_operate)
	{
	case add:
		{
			main->m_pATCommandWarp1->UPLMN_Add((char *)Util::StringOp::FromCString(c).c_str(),
				gsm_act,gsm_compact,urtan_act);

		}
		break;
	case change:
		{	
			index = m_cUPLMNList.GetCurSel();
			main->m_pATCommandWarp1->UPLMN_Change(index+1,(char *)Util::StringOp::FromCString(c).c_str(),
				gsm_act,gsm_compact,urtan_act);
		}
		break;
	}
}

void CUPLMNDlg::DelList(int index)
{
	std::vector<UPLMN>::iterator it = m_vUplmnList.begin();
	if (m_vUplmnList.size() > index)
	{
		m_vUplmnList.erase(it+index);
	}
}

void CUPLMNDlg::ReadOnly(bool b)
{
	m_cEditOper.SetReadOnly(b);
	m_cEditGsm_Act.SetReadOnly(b);
	m_cEditGsm_Compact.SetReadOnly(b);
	m_cEditUtran_Act.SetReadOnly(b);
}

LRESULT CUPLMNDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch (message)
	{	
		case WM_UPLMN_LIST:
			{
				std::string *data = (std::string *)wParam; 
				std::string pdata = data->c_str();
				ParseUPLMN((char *)pdata.c_str());
				SetUplmn(0);
				ReadOnly(true);
				ShowWindow_(SW_SHOW);
			}	
			break;
		case WM_COPL_DEL:
			{
				if (1 == wParam)//删除成功
				{	
					int cur = m_cUPLMNList.GetCurSel();
					DelList(cur);
					m_cUPLMNList.DeleteString(cur);
					if (m_vUplmnList.size())
					{
						SetUplmn(0);
					}
					else
					{
						m_cUPLMNList.ResetContent();
						m_cEditOper.SetWindowText(L"");
						m_cEditGsm_Act.SetWindowText(L"");
						m_cEditGsm_Compact.SetWindowText(L"");
						m_cEditUtran_Act.SetWindowText(L"");
						
					}
					main->m_pDeleteTipDlg->SetTitle(L"删除成功");
				}
				else
				{
					main->m_pDeleteTipDlg->SetTitle(L"删除失败");
				}
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);


			}
			break;
		case WM_COPL_ADD:
			{	
				if (1 == wParam)
				{
					main->m_pDeleteTipDlg->SetTitle(L"添加成功");
					ShowWindow_(SW_HIDE);
				}
				else
				{
					main->m_pDeleteTipDlg->SetTitle(L"添加失败");
				}
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			}
			break;

		case WM_COPL_CHANGE:
			{	
				int index = m_cUPLMNList.GetCurSel();
				if (1 == wParam)
				{	
					int gsm_act;
					int gsm_compact;
					int urtan_act ;
					CString temp ;
					CString c ;
					m_cEditOper.GetWindowText(c);
					m_cEditGsm_Act.GetWindowText(temp);
					gsm_act = atoi(temp);
					
					m_cEditGsm_Compact.GetWindowText(temp);
					gsm_compact = atoi(temp);
					
					m_cEditUtran_Act.GetWindowText(temp);
					urtan_act = atoi(temp);

					m_vUplmnList[index].oper = Util::StringOp::FromCString(c);			
					m_vUplmnList[index].gsm_act = gsm_act;
					m_vUplmnList[index].gsm_compact = gsm_compact ;
					m_vUplmnList[index].utran_act = urtan_act ;
					
					SetUplmn(index);
					main->m_pDeleteTipDlg->SetTitle(L"修改成功");
				}
				else
				{
					main->m_pDeleteTipDlg->SetTitle(L"修改失败");
				}
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			}
			break;
	}
	return CWnd::WindowProc(message, wParam, lParam);

}