// playsounddlg.cpp : implementation file
//
#include "stdafx.h"
#include "..\MultimediaPhone.h"
#include "playsounddlg.h"
#include "../Data/LanguageResource.h"
#include "ContactInfoDlg.h"
#include "SoundDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

DWORD volume[]={0x10001000,0x44004400,0x88008800,0xcc00cc00,0xff00ff00};
CPlaySoundDlg::CPlaySoundDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CPlaySoundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlaySoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CPlaySoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlaySoundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlaySoundDlg, CCEDialog)
	//{{AFX_MSG_MAP(CPlaySoundDlg)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CHANGELANGUAGE,OnChangeLanguage)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_CHECK_SOUNDDLG_PLAYCONTINUE, OnCheckPlaycontinue)
	ON_MESSAGE(WM_PROCESS_POS, OnPlaySeek)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaySoundDlg message handlers

BOOL CPlaySoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
//	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	std::string strTemp;
	CString str;
	
	m_sticNameNO.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 98*VHEIGHT, 230, 114*VHEIGHT), this);
	m_sticNameNO.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticSoundName.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 76*VHEIGHT, 340, 92*VHEIGHT), this);
	m_sticSoundName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticSoundDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(232, 98*VHEIGHT, 338, 114*VHEIGHT), this);
	m_sticSoundDuration.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticSoundDuration.SetAlign(SS_RIGHT);	

	m_procbarSound.Create(WS_CHILD|WS_VISIBLE, CRect(62, 123*VHEIGHT, 338, 139*VHEIGHT), this, IDC_PBAR_SOUNDDLG_PROCESS);
	m_procbarSound.SetParam(0, 0, 50, 1);
	m_procbarSound.SetPos(0);
	m_procbarSound.SetDisable(true);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CONTINUE);
	str = strTemp.c_str();
	m_chkPlayContinue.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(62, 148*VHEIGHT, 240, 164*VHEIGHT), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_chkPlayContinue.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_chkPlayContinue.SetCheck(BST_UNCHECKED);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78*VHEIGHT, 434, 98*VHEIGHT), this, IDC_BUTTON_SOUNDDLG_CLOSE);
	m_btnClose.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PREV);
	str = strTemp.c_str();
	m_btnPrev.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103*VHEIGHT, 434, 123*VHEIGHT), this, IDC_BUTTON_SOUNDDLG_PREV);
	m_btnPrev.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TEXT);
	str = strTemp.c_str();
	m_btnNext.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 128*VHEIGHT, 434, 148*VHEIGHT-1), this, IDC_BUTTON_SOUNDDLG_NEXT);
	m_btnNext.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
	str = strTemp.c_str();
	m_btnPlay.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 153*VHEIGHT, 434, 173*VHEIGHT), this, IDC_BUTTON_SOUNDDLG_PLAY);
	m_btnPlay.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40*VHEIGHT, 448, 184*VHEIGHT), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);

	m_iCurrentSound = 0;
	m_iModel = 0;
	
	m_iCurrentID = 0;
	m_iTotalSeconds = 0;
	m_iCurrentSecond = 0;
	m_bHasNext = false;
	m_bAutoClose=FALSE;
	m_bPlaying = false;
	m_bPlayPause = false;
	m_bOnlyNew  = false;
	m_pOggCodec = new OggCodec();
    m_nSoundindex = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->soundVolume();
    waveOutSetVolume(NULL,volume[m_nSoundindex]);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//void CPlaySoundDlg::SetSound(int listID, int offset)
void CPlaySoundDlg::SetSound(int type, int id, int dir, std::string searchFilter)
{
	m_iType = type;

	m_sSearchFilter = searchFilter;

	if (m_iModel == 0)
	{
		std::string filter;

		if (type == 3)//查找结果
		{
			filter = searchFilter;
		}
		else
		{
			filter = "type = ";

			filter += Util::StringOp::FromInt(type);

			if (type == 0 && m_bOnlyNew)//有未读留言
			{
				filter += " AND played = 0";//在录音列表播放跳过这句
			}

		}
		std::vector<boost::shared_ptr<Data::SoundSegment> > result;

		if (dir == 0)//单条
		{
			filter = "id = ";

			filter += Util::StringOp::FromInt(id);

			result = Data::SoundSegment::GetFromDatabase(filter);
		}
		else
		{
			result = Data::SoundSegment::GetFromDatabase(filter, Data::Direction(dir), id, 1);
		}

		if (!result.empty())
		{			
			m_iCurrentID = result[0]->id();
			id = m_iCurrentID;
			std::string nameNo;
			if (result[0]->name() != "")
			{
				nameNo = result[0]->name();
				nameNo = nameNo + "(" + result[0]->telephoneNumber().number() + ")";
			}
			else
			{
				nameNo = result[0]->telephoneNumber().number();
			}
			
			CString fileName = Util::StringOp::ToCString(result[0]->filename());
			CFileStatus status;
			if (CFile::GetStatus(LPCTSTR(fileName),status))
			{
				if(!nameNo.empty())
					m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));
			}
			else
			{
				std::string str = Data::LanguageResource::Get(Data::RI_SOUND_NO_FILE);
				m_sticNameNO.SetWindowText(Util::StringOp::ToCString(str));
			}
			
			int index = result[0]->filename().rfind('\\') + 1;
			m_sFilePath = result[0]->filename().substr(0, index);
			m_sticSoundName.SetWindowText(Util::StringOp::ToCString(result[0]->filename().substr(index)));
			m_sticSoundName.Invalidate();
			m_iTotalSeconds = result[0]->duration();
			m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
			m_bHasNext = true;
		}
	}
	else if (m_iModel == 1)
	{		
		std::string filter;
		if (type == 3)
		{
			filter = searchFilter;
		}
		else
		{
			filter = "type = ";
			filter += Util::StringOp::FromInt(type);
		}

		std::vector<boost::shared_ptr<Data::ContactInfo> > result;
		if (dir == 0)
		{
			filter = "id = ";
			filter += Util::StringOp::FromInt(id);
			result = Data::ContactInfo::GetFromDatabase(filter);
		}
		else
		{
			filter += " AND isSound = 1";
			result = Data::ContactInfo::GetFromDatabase(filter, Data::Direction(dir), id, 1);
		}

		if (!result.empty())
		{
			m_iCurrentID = result[0]->id();
			filter = "contactInfoId = ";
			filter += Util::StringOp::FromInt(result[0]->id());
			m_vSoundResult = Data::SoundSegment::GetFromDatabase(filter);
			if (!m_vSoundResult.empty())
			{
				m_iCurrentSound = 0;
				std::string nameNo;
				if (m_vSoundResult[0]->name() != "")
				{
					nameNo = m_vSoundResult[0]->name();
					nameNo = nameNo + "(" + m_vSoundResult[0]->telephoneNumber().number() + ")";

				}
				else
				{
					nameNo = m_vSoundResult[0]->telephoneNumber().number();
				}

				CString fileName = Util::StringOp::ToCString(m_vSoundResult[0]->filename());
				CFileStatus status;
				if (CFile::GetStatus(LPCTSTR(fileName),status))
				{
					if(!nameNo.empty())
						m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));
				}
				else
				{
					std::string str = Data::LanguageResource::Get(Data::RI_SOUND_NO_FILE);
					m_sticNameNO.SetWindowText(Util::StringOp::ToCString(str));
				}

				int index = m_vSoundResult[m_iCurrentSound]->filename().rfind('\\') + 1;
				m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename().substr(0, index);
				m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(index)));
				m_sticSoundName.Invalidate();
				m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration();
				m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
			}
			else
			{
				m_sticSoundName.SetWindowText(_T(""));
				m_iTotalSeconds = 0;
			}
			m_bHasNext = true;
		}
	}
	ShowSeconds();
}

void CPlaySoundDlg::SetModel(int model)
{
	m_iModel = model;
}
void CPlaySoundDlg::SetAutoClose(bool bAutoClose)
{
   m_bAutoClose=bAutoClose;
}
void CPlaySoundDlg::leaveplay()
{
	OnButtonPlay();
}
void CPlaySoundDlg::SetPlayFlag(bool m_bPlaying_)
{
	m_bPlaying=m_bPlaying_;
}
void CPlaySoundDlg::ShowSeconds(void)
{
	CString strTime;
	if (m_iTotalSeconds < 3600)
	{
		strTime.Format(_T("%02d:%02d/%02d:%02d"), m_iCurrentSecond / 60, m_iCurrentSecond % 60, m_iTotalSeconds / 60, m_iTotalSeconds % 60);
	}
	else
	{
		strTime.Format(_T("%02d:%02d:%02d/%02d:%02d:%02d"), m_iCurrentSecond / 3600, m_iCurrentSecond % 3600 / 60, m_iCurrentSecond % 60, m_iTotalSeconds / 3600, m_iTotalSeconds % 3600 / 60, m_iTotalSeconds % 60);
	}
	m_sticSoundDuration.SetWindowText(strTime);
	m_sticSoundDuration.Invalidate();
}

void CPlaySoundDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here

	ShowWindow_(FALSE);

	KillTimer(1);

	if (m_bPlayPause)
	{
		std::string strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);

		CString str = strTemp.c_str();

		m_btnPlay.SetWindowText(str);

		m_bPlayPause = false;			
	}

	m_pOggCodec->StopDecode(true);
	m_bPlaying = false;
	m_procbarSound.SetPos(0);
	m_iCurrentSecond = 0;
	m_iTotalSeconds = 0;
	m_sticNameNO.SetWindowText(_T(""));
	m_sticSoundName.SetWindowText(_T(""));
	m_sticSoundDuration.SetWindowText(_T(""));
	m_vSoundResult.clear();
	m_iCurrentSound = 0;
	m_bOnlyNew = false;

	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList();
}

CCEStatic* CPlaySoundDlg::getM_sticSoundName()
{
	return &m_sticNameNO;
}
void CPlaySoundDlg::OnButtonPrev() 
{
	// TODO: Add your control notification handler code here
	StopPlay();
	m_bHasNext = false;

	if (m_iModel == 1)
	{
		--m_iCurrentSound;

		if (m_vSoundResult.size() > 1 && m_iCurrentSound >= 0)
		{
			int index = m_vSoundResult[m_iCurrentSound]->filename().rfind('\\') + 1;
			m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename().substr(0, index);
			m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(index)));
			m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration();
			m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
			ShowSeconds();
			m_bHasNext = true;

			return;
		}
		else
		{
			m_iCurrentSound = 0;

			m_bHasNext = false;
		}
	}

	SetSound(m_iType, m_iCurrentID, 1, m_sSearchFilter);
}
void CPlaySoundDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	StopPlay();
	m_bHasNext = false;

	if (m_iModel == 1)//?
	{
		++m_iCurrentSound;

		if (m_vSoundResult.size() > 1 && m_iCurrentSound < m_vSoundResult.size())
		{
			int index = m_vSoundResult[m_iCurrentSound]->filename().rfind('\\') + 1;
			m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename().substr(0, index);
			m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(index)));
			m_sticSoundName.Invalidate();
			m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration();
			m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
			ShowSeconds();
			m_bHasNext = true;
			return;
		}

		else
		{
			m_iCurrentSound = m_vSoundResult.size() - 1;
		}
	}	
	SetSound(m_iType, m_iCurrentID, 2, m_sSearchFilter);
}

void CPlaySoundDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	std::string str;
	CString temp;
	if (m_bPlaying)
	{
		std::string strTemp;
		CString str;

		if (m_bPlayPause)
		{
			strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
			m_bPlayPause = false;			
		}
		else
		{
			strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PAUSE);
			m_bPlayPause = true;
		}

		str = strTemp.c_str();
		m_btnPlay.SetWindowText(str);
		m_pOggCodec->PauseDecode(!m_bPlayPause);
		
	}
	else
	{
		CString filename;
		m_sticSoundName.GetWindowText(filename);
		CString path = Util::StringOp::ToCString(m_sFilePath) + filename;
		CFileStatus status;
		if (!filename.IsEmpty() && CFile::GetStatus(LPCTSTR(path),status))
		{
			filename = path;
			if (m_pOggCodec->StartDecode(\
			Util::StringOp::FromCString(filename)))//开始播放
			{
				SetTimer(1, 1000, 0);

				std::string strTemp;
				CString str;

				strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PAUSE);
				str = strTemp.c_str();
				m_btnPlay.SetWindowText(str);
				m_bPlaying = true;
				m_bPlayPause = true;
				m_procbarSound.SetDisable(false);
			}

			std::vector<boost::shared_ptr<Data::SoundSegment> > result = Data::SoundSegment::GetFromDatabase("filename = '" + Util::StringOp::FromCString(filename) + "'");	

			if (!result.empty())
			{
				for (int i = 0; i < result.size(); ++i)
				{
					if (!result[i]->played())//没有播放
					{
						if (((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_nRecordCount > 0)
						{
							((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_nRecordCount--;

							((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->PostMessage(WM_SHOWLIST,0,0);

							::PostMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
						}
					}

					result[i]->played(true);

					result[i]->Update();
				 
				}
			}
		}
		else
		{
			if(m_chkPlayContinue.GetCheck() == BST_CHECKED)
			{		
				OnButtonNext();
				if (m_bHasNext)
				{
					Sleep(200);
					PostMessage(WM_PLAYSOUND, 0, 1);
				}
			}
		}
	}
}

void CPlaySoundDlg::OnCheckPlaycontinue() 
{
	// TODO: Add your control notification handler code here
}

void CPlaySoundDlg::OnPlaySeek(WPARAM w, LPARAM l)
{
	if ((int)l == IDC_PBAR_SOUNDDLG_PROCESS)
	{
		if (m_bPlaying)
		{
			CString filename;
			m_sticSoundName.GetWindowText(filename);
			std::string fn = Util::StringOp::FromCString(filename);
			
			if (fn.find("HQ") != std::string::npos)
			{
				w -= (w % 2);
			}
			else
			{
				w -= (w % 5);
			}
			if (m_pOggCodec->DecodeSeek(w))
			{
				m_iCurrentSecond = w;
			}
			else
			{
				m_iCurrentSecond = m_iTotalSeconds;
			}
		}
	}
}

void CPlaySoundDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		if (!m_bPlayPause)
		{
			return;
		}
		++m_iCurrentSecond;
		if (m_iCurrentSecond <= m_iTotalSeconds)
		{
			m_procbarSound.SetPos(m_iCurrentSecond);
			ShowSeconds();
		}
		else
		{
			KillTimer(1);
			StopPlay();
			
			if(m_chkPlayContinue.GetCheck() == BST_CHECKED)
			{
				OnButtonNext();
				if (m_bHasNext)
				{
					PostMessage(WM_PLAYSOUND, 0, 1);
				}
			}
		}
		break;
	case 2:
		KillTimer(2);
		OnButtonPlay();
		break;
	}
	
	CCEDialog::OnTimer(nIDEvent);
}

void CPlaySoundDlg::CloseSound(void)
{
	StopPlay();
}

void CPlaySoundDlg::StartPlay(bool onlyNew)
{
	if (onlyNew)
	{
		m_bOnlyNew = true;
		m_chkPlayContinue.SetCheck(BST_CHECKED);
	}
	
	OnButtonPlay();
}

void CPlaySoundDlg::StopPlay(void)
{
	KillTimer(1);
	m_pOggCodec->StopDecode(true);
	m_bPlaying = false;
	m_bPlayPause = false;			
	std::string strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
	CString str = strTemp.c_str();
	m_btnPlay.SetWindowText(str);
	m_procbarSound.SetPos(0);
	m_procbarSound.SetDisable(true);
	m_iCurrentSecond = 0;
	ShowSeconds();
	Sleep(200);
}
LRESULT CPlaySoundDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_PLAYSOUND:
		if(lParam == 1)
		{
			SetTimer(2, 300, NULL);
		}
		break;
	case WM_KEYDOWN:						
		if(wParam == 'D')
		{
			OnButtonNext();
		}else if(wParam == 'U')
		{
			OnButtonPrev();
		}		
		break;
	case WM_CANCEL:
		OnButtonClose();
		break;
	case WM_BTLEFT:
			if(m_bPlaying)
			{
				m_nSoundindex--;
				if(m_nSoundindex  < 0)
					m_nSoundindex = 0;
			}
			waveOutSetVolume(NULL, volume[m_nSoundindex]);

		break;
	case WM_BTRIGHT:
			if(m_bPlaying)
			{
				m_nSoundindex++;
				if(m_nSoundindex > 4)
					m_nSoundindex = 4;
			}
			waveOutSetVolume(NULL, volume[m_nSoundindex]);

		break;
	case WM_PLAYLEAVEWORD:
		OnButtonPlay();
		break;
		
		}
	return CDialog::WindowProc(message,wParam,lParam);
}
LRESULT CPlaySoundDlg::OnChangeLanguage(WPARAM wParam,LPARAM lParam)
{
ChangeLanguage();
return 0;
}

// void CPlaySoundDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
// {
// 	
//  CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
// }

void CPlaySoundDlg::setonlynew(bool gopaly)
{
 onlenew_=gopaly;
}

void CPlaySoundDlg::ChangeLanguage()
{
	std::string strTemp;
	CString str;

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CONTINUE);
	str = strTemp.c_str();
	m_chkPlayContinue.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PREV);
	str = strTemp.c_str();
	m_btnPrev.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TEXT);
	str = strTemp.c_str();
	m_btnNext.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
	str = strTemp.c_str();
	m_btnPlay.SetWindowText(str);

	TextStruct ts[1];
   
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE).c_str(), Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TITLE).length());
	
	m_sticBackground.SetTextStruct(ts, 1);

}