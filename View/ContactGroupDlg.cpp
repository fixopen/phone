// contactgroupdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\MultimediaPhone.h"
#include "contactgroupdlg.h"
#include "../Data/LanguageResource.h"
#include "ContactDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactGroupDlg dialog


CContactGroupDlg::CContactGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContactGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContactGroupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContactGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContactGroupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContactGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CContactGroupDlg)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_OK, OnButtonContactGroupOk)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_CANCEL, OnButtonContactGroupCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_DELETE, OnButtonContactGroupDelete)
	ON_BN_CLICKED(IDC_BUTTON_PLAYRING, OnButtonPlayRing)
	ON_BN_CLICKED(IDC_BUTTON_PLAYLEAVEWORD, OnButtonPlayLeaveWord)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContactGroupDlg message handlers

BOOL CContactGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
	
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TYPENAME);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(112, 69*VHEIGHT, 170, 87*VHEIGHT), this);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_RING);
	str = strTemp.c_str();
	m_sticRing.Create(str, WS_CHILD|WS_VISIBLE, CRect(112, 90*VHEIGHT, 170, 106*VHEIGHT), this);
	m_sticRing.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticRing.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_SOUNDTIP);
	str = strTemp.c_str();
	m_sticSoudTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(112, 115*VHEIGHT, 170, 131*VHEIGHT), this);
	m_sticSoudTip.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticSoudTip.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
	str = strTemp.c_str();
	m_btnPlayRing.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(294, 90*VHEIGHT, 350, 90*VHEIGHT+23), this, IDC_BUTTON_PLAYRING);
	m_btnPlayRing.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
	str = strTemp.c_str();
	m_btnPlayLeaveWord.Create(str, Data::g_buttonArcBMPSETTINGID[0][Data::g_skinstyle], Data::g_buttonArcBMPSETTINGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(294, 115*VHEIGHT, 350, 115*VHEIGHT+23), this, IDC_BUTTON_PLAYLEAVEWORD);
	m_btnPlayLeaveWord.SetBackRGB(Data::g_allFramInRectBackRGB[Data::g_skinstyle]);

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(174, 69*VHEIGHT, 288, 87*VHEIGHT), this, IDC_EDIT_CONTACTGROUP_NAME);
	m_edtName.SetLimitText(15);
	m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(174, 90, 290, 230), this, IDC_COMBOBOX_CONTACTGROUP_RING);
	m_cmbSoundTip.Create(WS_CHILD|WS_VISIBLE, CRect(174, 115, 290, 235), this, IDC_COMBOBOX_CONTACTGROUP_SOUNDTIP);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 41*VHEIGHT, 471, 61*VHEIGHT-2), this, IDC_BUTTON_CONTACTGROUP_OK);
	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 66*VHEIGHT, 471, 86*VHEIGHT), this, IDC_BUTTON_CONTACTGROUP_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETETYPEBTN);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 91*VHEIGHT, 471, 111*VHEIGHT), this, IDC_BUTTON_CONTACTGROUP_DELETE);
	m_btnDelete.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 1);
		
	m_pContactGroup = boost::shared_ptr<Data::ContactGroup>();
	
	m_iSBell=false;
	m_iTotalSeconds=0;
	m_iCurrentSecond=0;
	m_pOggCodec = new OggCodec();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContactGroupDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactGroupDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CContactGroupDlg::AddRingTipItems(void)
{

}

void CContactGroupDlg::OnButtonContactGroupOk() 
{
	// TODO: Add your control notification handler code here
	CString s;
	m_edtName.GetWindowText(s);
	if (s.IsEmpty())
	{	
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CONTACTGROUPNEW);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}
	ShowWindow(FALSE);

	//关闭输入法
	KeyBoardOff();
	
	if (!m_pContactGroup)
	{
		m_pContactGroup = boost::shared_ptr<Data::ContactGroup>(new Data::ContactGroup);
	}
	
	std::string name = Util::StringOp::FromCString(s);
	
	for (int i = 0; i < name.length(); ++i)
	{
		if (name[i] == '\'')
		{
			name[i] = '\"';
		}
	}

	m_pContactGroup->name(name);

 	CString str;
	m_cmbRing.GetLBText(m_cmbRing.GetCurSel(), str);
	if (m_cmbRing.GetCurSel() > 0)
	{
		str = csFlashRingPath + str;
	}
	m_pContactGroup->ringName(Util::StringOp::FromCString(str));
	m_cmbSoundTip.GetLBText(m_cmbSoundTip.GetCurSel(), str);

	if (m_cmbSoundTip.GetCurSel() > 0)
	{
		int index = m_cmbSoundTip.GetItemData(m_cmbSoundTip.GetCurSel());

		if (index == 0)
		{   
	       	if ((str.Right(4).CompareNoCase(_T(".mp3")) == 0)
					||(str.Right(4).CompareNoCase(_T(".wav")) == 0))
			{
               str = csHiveTipPath + str;
			}
			else
			{
				str = csFlashRecordPath + str;  //本地录音文件
			}
		}
		else if (index == 1)
		{  
		   str = csStorageCardRecordPath + str;
		}
		else
		{   
		  	str = csHiveTipPath + str;		
		}
	}
	m_pContactGroup->tipName(Util::StringOp::FromCString(str));

	bool isInsert = false;
	if (m_pContactGroup->id() == 0)
	{
		m_pContactGroup->Insert();

		isInsert = true;
	}
	else
	{      
		m_pContactGroup->Update();
	}

	((CContactDlg*)m_pParentWnd)->ShowTypeItems();
	((CContactDlg*)m_pParentWnd)->ShowItemsInList();
}

void CContactGroupDlg::OnButtonContactGroupCancel() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);
	//关闭输入法
	KeyBoardOff();
}

void CContactGroupDlg::OnButtonContactGroupDelete() 
{
	// TODO: Add your control notification handler code here
	if (m_pContactGroup->id() > 0)
	{
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTGROUP);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
}

void CContactGroupDlg::SetContactGroup(boost::shared_ptr<Data::ContactGroup> contactgroup)
{
	m_pContactGroup = contactgroup;
	std::string strTemp;
	if (contactgroup->id() == 0)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPESTATIC);
		m_edtName.SetWindowText(_T(""));
		m_btnDelete.ShowWindow(FALSE);
	}
	else
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPESTATIC);
		m_edtName.SetWindowText(Util::StringOp::ToCString(contactgroup->name()));
		m_btnDelete.ShowWindow(TRUE);
	}
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	ts[0].txtRect = CRect(8, 0, 100, 20);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, strTemp.c_str(), strTemp.length());
	m_sticBackground.SetTextStruct(ts, 1);
	CString ringName = Util::StringOp::ToCString(contactgroup->ringName());
	m_cmbRing.ResetContent();
	m_cmbRing.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));	
	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	
	hFind = FindFirstFile(csFlashRingPath + _T("*.*"), &FindFileData);//使用FindFirstFile函数来开始文件查找
	
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CString filename;
		filename =  FindFileData.cFileName;
		if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
			|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
		{
			m_cmbRing.AddString(filename);	
		}
		
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			filename =  FindFileData.cFileName;
			if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
				|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
			{
				m_cmbRing.AddString(filename);	
			}
		}

		FindClose(hFind);
	}
	ringName = ringName.Right(ringName.GetLength() - ringName.ReverseFind('\\') - 1);
	int i = 0;
	CString txt;
	for (i = 0; i < m_cmbRing.GetCount(); i++)
	{
		m_cmbRing.GetLBText(i, txt);
		if (ringName == txt)
		{
			m_cmbRing.SetCurSel(i);
			break;
		}
		else
		{
			m_cmbRing.SetCurSel(0);
		}
	}

	m_cmbSoundTip.ResetContent();
	m_cmbSoundTip.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));	
	hFind = FindFirstFile(csHiveTipPath + _T("*.*"), &FindFileData);//使用FindFirstFile函数来开始文件查找
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		CString filename;
		filename =  FindFileData.cFileName;
		if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
			|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
		{
			m_cmbSoundTip.AddString(filename);	
		}
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			filename =  FindFileData.cFileName;
			if ((filename.Right(4).CompareNoCase(_T(".mp3")) == 0)
				|| (filename.Right(4).CompareNoCase(_T(".wav")) == 0))
			{
				m_cmbSoundTip.InsertString(-1,filename);
			}
		}
		FindClose(hFind);
	}

	for (i = 0; i < m_cmbSoundTip.GetCount(); i++)
	{
		m_cmbSoundTip.SetItemData(i, 2);
	}
	if (Data::SoundSegment::GetDataCount("type = 2 AND isTip = 1") > 0)
	{
		std::vector<boost::shared_ptr<Data::SoundSegment> > sounds =\
			Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1");
		int pos = 0;
		for (i = 0; i < sounds.size(); ++i)
		{	
			//add by qi 20110803
			CString tipsDir;//提示语的目录
			//

			pos = sounds[i]->filename().rfind('\\');
			m_cmbSoundTip.InsertString(-1,\
				Util::StringOp::ToCString(sounds[i]->filename().substr(pos + 1)));
			
			//add by qi 20110803
			tipsDir = Util::StringOp::ToCString(sounds[i]->filename().substr(0,pos+1));
			m_vGroupTipsDir.push_back(tipsDir);
			m_cmbSoundTip.SetItemData(m_cmbSoundTip.GetCount()-1,i);
			//
//			change by qi 20110803
// 			if (sounds[i]->filename().substr(0, pos + 1) == ssFlashRecordPath)
// 			{
// 				m_cmbSoundTip.SetItemData(i + 1, 0);
// 			}
// 			else
// 			{
// 				m_cmbSoundTip.SetItemData(i + 1, 1);
// 			}
		}
	}

    int pos = contactgroup->tipName().rfind('\\');

	CString tipName = Util::StringOp::ToCString(contactgroup->tipName().substr(pos + 1));

	for (i = 0; i < m_cmbSoundTip.GetCount(); i++)
	{
		m_cmbSoundTip.GetLBText(i, txt);

		if (tipName == txt)
		{
			m_cmbSoundTip.SetCurSel(i);

			break;
		}
		else
		{
			m_cmbSoundTip.SetCurSel(0);
		}
	}
}
LRESULT CContactGroupDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DELETESELITEM:
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pDeleteTipDlg->SetPasswordModel(false);
		main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_DELETE);
		main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		KillTimer(99);
		SetTimer(99, 50, NULL);
	}
	break;
	case WM_CHANGELANGUAGE:
		ChangeLanguage();
    break;
	case WM_OK:
		OnButtonContactGroupOk();
		break;
	case WM_CANCEL:
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactGroupDlg->IsWindowVisible())
		{
             OnButtonContactGroupCancel(); 
		}
    break;
	case WM_DELETE:
		OnButtonContactGroupDelete();
		break;
	case WM_STOPTRYRING:
		SetButtoName();
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

void CContactGroupDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str;
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_TYPENAME);
	str = strTemp.c_str();
    m_sticName.SetWindowText(str);

    strTemp = Data::LanguageResource::Get(Data::RI_CARD_RING);
	str = strTemp.c_str();
    m_sticRing.SetWindowText(str);
   
	strTemp = Data::LanguageResource::Get(Data::RI_CARD_SOUNDTIP);
	str = strTemp.c_str();
	m_sticSoudTip.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
    m_btnOk.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
    m_btnCancel.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETETYPEBTN);
	str = strTemp.c_str();
    m_btnDelete.SetWindowText(str);
}

void CContactGroupDlg::OnButtonPlayRing()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (main->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff)     
		 return;

	m_iSBell=true;  
	std::string strTemp;
	CString str;
	if(m_bPlaying)
    {
		m_pOggCodec->StopDecode(m_bPlaying);
		main->phone_->StopRing(m_bPlaying);
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
		str = strTemp.c_str();
		m_btnPlayRing.SetWindowText(str);
		
		m_bPlaying=false;
	}
	else
	{
		m_pOggCodec->StopDecode(!m_bPlaying);
		main->phone_->StopRing(!m_bPlaying);
		
		CString s;
		m_cmbRing.GetWindowText(s);
		if (s.IsEmpty())
			return;
		if(s == L"默认")
		{
			s = Util::StringOp::ToCString(main->m_pSettingDlg->m_pSetting->defaultRingFilename());
		}
		else
		{
			s = csFlashRingPath + s;
		}

		CFileStatus status;
		if (CFile::GetStatus(LPCTSTR(s),status))
		{
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_STOP);
			str = strTemp.c_str();
			m_btnPlayRing.SetWindowText(str);

			main->phone_->SetMsgWnd(this);
			main->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
			m_bPlaying=true;
		}
	}
}

void CContactGroupDlg::OnButtonPlayLeaveWord()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if ((main->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff))
		return;
	std::string strTemp;
	CString str;
	if (m_bPlayPause)
	{
		m_pOggCodec->StopDecode(m_bPlayPause);
		main->phone_->StopRing(m_bPlayPause);
		
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
		str = strTemp.c_str();
		m_btnPlayLeaveWord.SetWindowText(str);
		
		m_bPlayPause = false;
	}
    else
    {	
		m_pOggCodec->StopDecode(!m_bPlayPause);
		main->phone_->StopRing(!m_bPlayPause);
		
		CString s;
		m_cmbSoundTip.GetWindowText(s);
		if (s.IsEmpty() || (m_cmbSoundTip.GetCurSel() == LB_ERR))
			return;
		
		if ((s.Right(4).CompareNoCase(_T(".mp3")) == 0)
			|| (s.Right(4).CompareNoCase(_T(".wav")) == 0))
		{  
			int index = m_cmbSoundTip.GetItemData(m_cmbSoundTip.GetCurSel());

			if (index == 0)
			{
				s = csFlashRecordPath + s;
			}
			else if (index == 1)
			{
				s = csStorageCardRecordPath + s;
			}
			else
			{
				s = csHiveTipPath + s;
			}
		}
		else
		{
			if(s == L"默认")
			{
				s = Util::StringOp::ToCString(\
					main->m_pSettingDlg->m_pSetting->defaultTipFilename());
			}
			else
			{	
				int index = m_cmbSoundTip.GetItemData(m_cmbSoundTip.GetCurSel());
				s = m_vGroupTipsDir[index] + s;

				//change by qi 20110803
				//s = csFlashRecordPath + s;
			}
		}
		
		CFileStatus status;
		if (CFile::GetStatus(LPCTSTR(s),status))
		{
			strTemp = Data::LanguageResource::Get(Data::RI_SETTING_STOP);
			str = strTemp.c_str();
			m_btnPlayLeaveWord.SetWindowText(str);   //当文件能播放时才设置按钮文字
			m_bPlayPause=true;

			if((s.Right(4).CompareNoCase(_T(".spx")) == 0)||(s.Right(4).CompareNoCase(_T(".spd")) == 0))
			{   
				if (m_pOggCodec->StartDecode(Util::StringOp::FromCString(s)))
				{ 
					KillTimer(100);
					SetTimer(100, 1000, NULL);  
				}
			}
			else
			{   
				main->phone_->SetMsgWnd(this);
				main->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
			}
		}
	}
}

void CContactGroupDlg::SetButtoName()
{
	std::string strTemp;
	CString str;
	if (m_iSBell)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
		str = strTemp.c_str();
		m_btnPlayRing.SetWindowText(str);
		m_bPlaying=false;	
	}
	else
	{   
		strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);
		str = strTemp.c_str();
		m_btnPlayLeaveWord.SetWindowText(str);
		m_bPlayPause = false;
	}

	m_iSBell=false;
}

void CContactGroupDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==100)
	{ 
		if(m_bPlayPause)
		{  
			++m_iCurrentSecond;
            if (m_iCurrentSecond>m_iTotalSeconds)
            { 	
				m_bPlayPause = false;
            }
		}
		else
		{   
			std::string strTemp = Data::LanguageResource::Get(Data::RI_SETTING_LISTEN);	
			CString	str = strTemp.c_str();	
			m_btnPlayLeaveWord.SetWindowText(str);
			m_bPlayPause=false;
			m_iCurrentSecond=0;
			KillTimer(100);
		}
	}
	else if(nIDEvent == 99)  //wzx 20110106
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		KillTimer(nIDEvent);

		int id = m_pContactGroup->id();
		std::string filter;
		filter = "groupId = " + Util::StringOp::FromInt(id);
		std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
		if (!result.empty())
		{
			Data::Contact::ExecCommand("begin;");
			for (std::vector<boost::shared_ptr<Data::Contact> >::iterator iter = result.begin(); iter != result.end(); ++iter)
			{
				(*iter)->groupId(0);
				(*iter)->Update();
			}
			Data::Contact::ExecCommand("commit;");
		}
		m_pContactGroup->Remove();
		((CContactDlg*)m_pParentWnd)->ShowTypeItems();		
		((CContactDlg*)m_pParentWnd)->ShowItemsInList();
		//操作已成功
		main->m_pDeleteTipDlg->SetPasswordModel(false);
		main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
		std::string str1 = Data::LanguageResource::Get(Data::RI_DELETETIP_OPSUCCESS);//是否将Hive和FlashDrv下的文件删除
		main->m_pDeleteTipDlg->SetDelTip(str1.c_str());
		main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);

		ShowWindow(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}