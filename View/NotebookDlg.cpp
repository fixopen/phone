// NotebookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "NotebookDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotebookDlg dialog


CNotebookDlg::CNotebookDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNotebookDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotebookDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsOPenTel = FALSE;
}


void CNotebookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotebookDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotebookDlg, CDialog)
	//{{AFX_MSG_MAP(CNotebookDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NOTE_OK, OnButtonNoteOk)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_PREV, OnButtonNotePrev)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_NEXT, OnButtonNoteNext)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_CLEAR, OnButtonNoteClear)
	ON_BN_CLICKED(IDC_BUTTON_NOTE_CLOSE, OnButtonNoteClose)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotebookDlg message handlers

BOOL CNotebookDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_WriteStatic.Create(L"", WS_VISIBLE|WS_CHILD, CRect(33, 66, 548, 393), this, 8950);
	m_WriteStatic.SetWrite(_T("/hive/my_note/1.bmp"));

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\便签.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));
	MoveWindow(0,57,800,423);
	m_MJPGList.SetUnitIsDownStatus(200,TRUE);

	m_nIndex = 0;

	char txt[64];
	sprintf(txt, "%d/20 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());
	CString s = txt;
	m_MJPGList.SetUnitFont(205, font_14);
	m_MJPGList.SetUnitText(205, txt, FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNotebookDlg::OnClickMJPG(WPARAM w, LPARAM l)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch (w)
	{
	case 100:
		main->m_pMainDlg->m_mainCalucaterDlg_->ShowWindow_(SW_SHOW);
		main->PopIcon(this);
		main->AddIcon(Allicon[8],main->m_pMainDlg->m_mainCalucaterDlg_,false);
		break;
	case 200:
		break;
	case 202:
		OnButtonNotePrev();
		break;
	case 203:
		OnButtonNoteNext();
		break;
// 	case 210:
// 		OnButtonNoteOk();
// 		break;
// 	case 211:
// 		OnButtonNoteClose();
// 		break;
	case 212:
		OnButtonNoteClear();
		break;
	case 300://SIM卡
		{

			if (!main->GetSimStatus())
			{
				main->m_pWarningNoFlashDlg->SetTitle("没插入SIM卡!");
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				return ;
			}

			if (main->m_phoneLine[0].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsDialing ||
				main->m_phoneLine[1].pFSM->getCurrentState() >= CMultimediaPhoneDlg::p3gsDialing ||
				main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsConnected )
			{
				main->m_pWarningNoFlashDlg->SetTitle(L"操作不允许");
				main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				return;
			}
			else
			{
				main->m_pCstkDlg->SetCmdID(-1, 0, FALSE);
				main->m_pDeleteTipDlg->SetTitle(L"正在查询信息",20000);
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			}
			
		}
		break;

	case 1000:
		OnButtonNoteClose();
		main->PopbackIcon();
		break;

	case 1001:
		OnButtonNoteOk();
		main->PopbackIcon();
		break;

	default:
		break;
	}
}

void CNotebookDlg::SetPageIndex()
{
	char txt[64];
	sprintf(txt, "%d/20 %s", m_nIndex+1, Data::LanguageResource::Get(Data::RI_NOTE_PAGE).c_str());
	CString s = txt;

	m_MJPGList.SetUnitText(205, txt, TRUE);

}

void CNotebookDlg::OnButtonNoteOk()
{
	m_bIsOPenTel = FALSE;
// 	m_WriteStatic.GetWrite(filename[m_nIndex]);
	std::string filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
	ShowWindow(SW_HIDE);
}

void CNotebookDlg::OnButtonNotePrev()
{
	std::string filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.GetWrite(filename[m_nIndex]);
	m_nIndex--;
	if(m_nIndex < 0)
		m_nIndex = 19;
	
	filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.SetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
// 	m_WriteStatic.SetWrite(filename[m_nIndex]);
	
	SetPageIndex();
}

void CNotebookDlg::OnButtonNoteNext()
{
	std::string filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.GetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.GetWrite(filename[m_nIndex]);
	m_nIndex++;
	if(m_nIndex > 19)
		m_nIndex = 0;
	filename = "/hive/my_note/";
	filename += Util::StringOp::FromInt(m_nIndex + 1);
	filename += ".bmp";
	m_WriteStatic.SetWrite((LPTSTR)(LPCTSTR)Util::StringOp::ToCString(filename));
//	m_WriteStatic.SetWrite(filename[m_nIndex]);
	
	SetPageIndex();
}

void CNotebookDlg::OnButtonNoteClear()
{
	m_WriteStatic.ClearDC();
}

void CNotebookDlg::OnButtonNoteClose()
{
	m_bIsOPenTel = FALSE;
	ShowWindow(SW_HIDE);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SetStatusAll(FALSE);
}

void CNotebookDlg::ShowWindow_(int nCmdShow)
{
	ShowWindow(nCmdShow);
}