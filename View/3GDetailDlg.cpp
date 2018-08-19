// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GDetailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GDetailDlg dialog

/*
void AddMenuLink(Menu3G *pMenu, MenuLink *pParentMenu)
{
	pParentMenu	
}

void RemoveMenuLink(MenuLink *MenuLink, MenuLink *pMenuLink)
{
			
}
*/

C3GDetailDlg::C3GDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GDetailDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GDetailDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pMenuLink = NULL;
}


void C3GDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GDetailDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GDetailDlg, CDialog)
	//{{AFX_MSG_MAP(C3GDetailDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GDetailDlg message handlers
/*
void C3GDetailDlg::OnTimer(UINT nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == 1)
		{
			
		}
	}
	CDialog::OnTimer(nIDEvent);
}
*/

BOOL C3GDetailDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 600, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\3g_家庭百事通.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 600, 420));
//	SetTimer(1, 1000, NULL);

	m_p3GDetailReadDlg = new C3GDetailReadDlg;
	m_p3GDetailReadDlg->Create(C3GDetailReadDlg::IDD, this);

	initmenu(4);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GDetailDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 2:
		m_p3GDetailReadDlg->initmenu(2, "", 0);
		m_p3GDetailReadDlg->ShowWindow(SW_SHOW);
		break;
	case 8:
		m_p3GDetailReadDlg->initmenu(8, "", 0);
		m_p3GDetailReadDlg->ShowWindow(SW_SHOW);
		break;

	case 1:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 9:
		clickmenu(m_nMenuIndex+w-1);
		break;
	case 101:
		doPre();
		break;
	case 102:
		doNext();
		break;
	case 100:
		OnExit();
	default:
		break;
	}
}

//初始化菜单，parentId = 0 第一级菜单,家庭百事通的主界面
void C3GDetailDlg::initmenu(int parentId)
{
	if(parentId == 4)   //一级菜单
		m_nMenuLevel = 1;
	else 
		m_nMenuLevel = 2;
	memset(m_Menu3G, 0, sizeof(Menu3G)*9);
	m_nMenuCount = 0;
	m_nMenuIndex = 0;
	char txt[64];
	sprintf(txt, "parentId = %d", parentId );
	std::string filter = txt;
	m_pMenuManagerresult = Data::MenuManager::GetFromDatabaseByOrder(filter, "showOrder"); 
	for(int i = 0; i < 9; i++)
	{
		m_MJPGList.SetUnitIsShow(i, FALSE, FALSE);
		m_MJPGList.SetUnitIsShow(10+i, FALSE, FALSE);
	}

	if(m_pMenuManagerresult.size() > 0)
	{
		m_nMenuCount = m_pMenuManagerresult.size();
		for(int i= 0; i < 9 && i < m_nMenuCount; i++)
		{
			m_MJPGList.SetUnitIsShow(i, TRUE, FALSE);
		    m_MJPGList.SetUnitIsShow(10+i, TRUE, FALSE);
			m_MJPGList.SetUnitBitmap(i, m_pMenuManagerresult[i]->menuInfos.imageName.c_str(),  m_pMenuManagerresult[i]->menuInfos.specImageName.c_str(), FALSE, TRUE);
			m_MJPGList.SetUnitText(i+10, m_pMenuManagerresult[i]->menuInfos.menuName.c_str(), FALSE, TRUE);
		}
		m_MJPGList.SetUnitIsShow(101, FALSE, FALSE);
		m_MJPGList.SetUnitIsShow(102, FALSE, FALSE);
		if(m_nMenuCount > 9)
		{
			m_MJPGList.SetUnitIsShow(102, TRUE, FALSE);
		}
	}
	m_MJPGList.Invalidate();
}

//处理菜单的点击，如果有二级菜单，则初始化二级菜单到m_Menu3G
int C3GDetailDlg::clickmenu(int index)
{
	int nIndex = m_nMenuIndex + index;
	if(nIndex <= m_nMenuCount)
	{
		int parentId = m_pMenuManagerresult[nIndex]->menuInfos.menuId;
		char txt[64];
		sprintf(txt, "parentId = %d", parentId );
		if(Data::MenuManager::GetFromDatabase(txt, Data::dNull, 0, 1).size() > 0)
			initmenu(parentId);
		else
		{
			//进入明细
			//m_p3GDetailReadDlg->inittitle((char *)m_pMenuManagerresult[nIndex]->menuInfos.menuName.c_str(), (char *)m_pMenuManagerresult[nIndex]->menuInfos.imageName.c_str());
			std::string spCode = "spCode = '";
			spCode += m_pMenuManagerresult[nIndex]->menuInfos.spCode;
			spCode += "'";
			int type = 1;
			if(m_pMenuManagerresult[nIndex]->menuInfos.spCode == ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pSettingDlg->m_pSetting->speCode1_)    //天气预报
			{
				type = 0;
				spCode = " DISTINCT datetime ";
			}
			m_p3GDetailReadDlg->initmenu(type, spCode, 0);
			m_p3GDetailReadDlg->ShowWindow(SW_SHOW);
		}
	}
	return 0;
}


void C3GDetailDlg::doNext()
{
	m_nMenuIndex += 9;
	for(int i = 0; i < 9; i++)
	{
		m_MJPGList.SetUnitIsShow(i, FALSE, FALSE);
		m_MJPGList.SetUnitIsShow(10+i, FALSE, FALSE);
	}
	
	if(m_nMenuCount > 0)
	{
		int j = 0;
		for(int i= m_nMenuIndex; i < (m_nMenuIndex+9) && i < m_nMenuCount; i++)
		{
			m_MJPGList.SetUnitIsShow(j, TRUE, FALSE);
			m_MJPGList.SetUnitIsShow(10+j, TRUE, FALSE);
			m_MJPGList.SetUnitBitmap(j, m_pMenuManagerresult[i]->menuInfos.imageName.c_str(), m_pMenuManagerresult[i]->menuInfos.specImageName.c_str(), FALSE , TRUE);
			m_MJPGList.SetUnitText(j+10, m_pMenuManagerresult[i]->menuInfos.menuName.c_str(), FALSE , TRUE);
			j++;
		}
		m_MJPGList.SetUnitIsShow(101, TRUE, FALSE);
		m_MJPGList.SetUnitIsShow(102, FALSE, FALSE);
		if(m_nMenuCount > (m_nMenuIndex+9))
		{
			m_MJPGList.SetUnitIsShow(102, TRUE, FALSE);
		}
	}
	m_MJPGList.Invalidate();
}

void C3GDetailDlg::doPre()
{
	m_nMenuIndex -= 9;
	for(int i = 0; i < 9; i++)
	{
		m_MJPGList.SetUnitIsShow(i, FALSE, FALSE);
		m_MJPGList.SetUnitIsShow(10+i, FALSE, FALSE);
	}
	
	if(m_nMenuCount > 0)
	{
		int j = 0;
		for(int i= m_nMenuIndex; i < (m_nMenuIndex+9) && i < m_nMenuCount; i++)
		{
			m_MJPGList.SetUnitIsShow(j, TRUE, FALSE);
			m_MJPGList.SetUnitIsShow(10+j, TRUE, FALSE);
			m_MJPGList.SetUnitBitmap(j, m_pMenuManagerresult[i]->menuInfos.imageName.c_str(), m_pMenuManagerresult[i]->menuInfos.specImageName.c_str(), FALSE, TRUE);
			m_MJPGList.SetUnitText(j+10, m_pMenuManagerresult[i]->menuInfos.menuName.c_str(), FALSE, TRUE);
			j++;
		}
		m_MJPGList.SetUnitIsShow(101, FALSE, FALSE);
		m_MJPGList.SetUnitIsShow(102, TRUE, FALSE);
		if(m_nMenuIndex >= 9)
		{
			m_MJPGList.SetUnitIsShow(101, TRUE, FALSE);
		}
	}
	m_MJPGList.Invalidate();
}

void C3GDetailDlg::OnExit()
{
	if(m_nMenuLevel==2 &&m_nMenuCount > 0)
	{
		int parentId = m_pMenuManagerresult[m_nMenuIndex]->menuInfos.parentId;
		char txt[64];
		sprintf(txt, "selfId = %d", parentId );

		std::vector<boost::shared_ptr<Data::MenuManager> > m_result = Data::MenuManager::GetFromDatabase(txt, Data::dNull, 0, 1); 
		if(m_result.size() > 0)
		{
			parentId = m_result[0]->menuInfos.parentId;
			sprintf(txt, "parentId = %d", parentId );
			if(Data::MenuManager::GetFromDatabase(txt, Data::dNull, 0, 1).size() > 0)
				initmenu(parentId);
		}
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}
/*
void C3GDetailDlg::InitMenuLink()
{
	
}

void C3GDetailDlg::FreeMenuLink()
{
	if(m_pMenuLink)
	{
		
	}
}
*/