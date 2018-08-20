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

#define  BITSPERSAMPLE   16
#define  SAMPLESPERSEC   44100
#define  CHANNELS        1
char buf[16384];
char out[8192];

typedef enum __WAVEDEV_MSG__
{
	WAV_SPEAKER_ONLY = 0x10000001,
	WAV_LINEOUT_ONLY = 0x10000002,
	WAV_SPEAKER_LINEOUT = 0x10000003,
	WAV_SNDEFFECT_POP	= 0x20000000,
	WAV_SNDEFFECT_3D	= 0x20000001,
	WAV_SNDEFFECT_CLASSIC	= 0x20000002,
	WAV_SNDEFFECT_JAZZ		= 0x20000003,
	WAV_LINEIN_MIC			= 0x30000001,
	WAV_LOCAL_MIC			= 0x30000002
} WAVDEV_MESSAGE;

/////////////////////////////////////////////////////////////////////////////
// CPlaySoundDlg dialog


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
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_NEXT, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_PREV, OnButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDDLG_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_CHECK_SOUNDDLG_PLAYCONTINUE, OnCheckPlaycontinue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlaySoundDlg message handlers

BOOL CPlaySoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	//CenterWindow(GetDesktopWindow());	// center to the hpc screen

	std::string strTemp;
	CString str;
	
	m_sticNameNO.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 76, 340, 92), this);
	m_sticNameNO.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticSoundName.Create(str, WS_CHILD|WS_VISIBLE, CRect(62, 98, 230, 114), this);
	m_sticSoundName.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);

	m_sticSoundDuration.Create(str, WS_CHILD|WS_VISIBLE, CRect(232, 98, 338, 114), this);
	m_sticSoundDuration.SetColor(RGB(0, 0, 0), Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_sticSoundDuration.SetAlign(SS_RIGHT);	

	m_procbarSound.Create(WS_CHILD|WS_VISIBLE, CRect(62, 123, 338, 139), this, 0xFF10);
	m_procbarSound.SetParam(0, 0, 50, 1);
	m_procbarSound.SetPos(0);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CONTINUE);
	str = strTemp.c_str();
	m_chkPlayContinue.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(62, 148, 338, 164), this, IDC_CHECK_SOUNDDLG_PLAYCONTINUE);
	m_chkPlayContinue.SetIcon(IDI_ICON_CHECK1, CSize(12, 12), IDI_ICON_CHECK0, CSize(12, 12));
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_chkPlayContinue.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, Data::g_partFrameInRectBackRGB[Data::g_skinstyle]);
	m_chkPlayContinue.SetCheck(BST_CHECKED);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 78, 434, 98), this, IDC_BUTTON_SOUNDDLG_CLOSE);
	m_btnClose.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PREV);
	str = strTemp.c_str();
	m_btnPrev.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 103, 434, 123), this, IDC_BUTTON_SOUNDDLG_PREV);
	m_btnPrev.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_TEXT);
	str = strTemp.c_str();
	m_btnNext.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 128, 434, 148), this, IDC_BUTTON_SOUNDDLG_NEXT);
	m_btnNext.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);
	
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_PLAY);
	str = strTemp.c_str();
	m_btnPlay.Create(str, Data::g_buttonArcBMPPARTDILOAGID[0][Data::g_skinstyle], Data::g_buttonArcBMPPARTDILOAGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(378, 153, 434, 173), this, IDC_BUTTON_SOUNDDLG_PLAY);
	m_btnPlay.SetBackRGB(Data::g_partFrameMainBackRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(34, 40, 448, 184), this, 1);
	
	TextStruct ts[1];
	memset(ts, 0, sizeof(TextStruct) * 1);
	
	ts[0].txtRect = CRect(8, 0, 100, 20);
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
		if (type == 2)
		{
			filter = searchFilter;
		}
		else
		{
			filter = "type = ";
			filter += Util::StringOp::FromInt(type);
		}
		std::vector<boost::shared_ptr<Data::SoundSegment> > result;
		if (dir == 0)
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
			//m_sticTitle;
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
			m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));
			m_sFilePath = result[0]->filename().substr(0, result[0]->filename().length() - 10);
			m_sticSoundName.SetWindowText(Util::StringOp::ToCString(result[0]->filename().substr(result[0]->filename().length() - 10)));
			m_iTotalSeconds = result[0]->duration().GetTotalSeconds();
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
			if (!result.empty())
			{
				int soundCount = result[0]->GetSoundsCount();
				if (soundCount > 0)
				{
					m_iCurrentID = result[0]->id();
					filter = "contactInfoId = ";
					filter += Util::StringOp::FromInt(result[0]->id());
					
					m_vSoundResult = Data::SoundSegment::GetFromDatabase(filter);
					if (!m_vSoundResult.empty())
					{
						m_iCurrentSound = m_vSoundResult.size() - 1;
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
						m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));
						m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename().substr(0, m_vSoundResult[m_iCurrentSound]->filename().length() - 10);
						m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
						m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration().GetTotalSeconds();
						m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
					}
				}
			}
		}
		else
		{
			filter += " AND isSound = 1";

			while (true)
			{
				result = Data::ContactInfo::GetFromDatabase(filter, Data::Direction(dir), id, 1);
				if (!result.empty())
				{
					int soundCount = result[0]->GetSoundsCount();
					if (soundCount > 0)
					{
						m_iCurrentID = result[0]->id();
						filter = "contactInfoId = ";
						filter += Util::StringOp::FromInt(result[0]->id());
						
						m_vSoundResult = Data::SoundSegment::GetFromDatabase(filter);
						if (!m_vSoundResult.empty())
						{
							m_iCurrentSound = m_vSoundResult.size() - 1;
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
							m_sticNameNO.SetWindowText(Util::StringOp::ToCString(nameNo));
							m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename().substr(0, m_vSoundResult[m_iCurrentSound]->filename().length() - 10);
							m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
							m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration().GetTotalSeconds();
							m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
							m_bHasNext = true;
							break;
						}						
					}
					continue;
				}
				break;
			}
		}
	}
	ShowSeconds();
}

void CPlaySoundDlg::SetModel(int model)
{
	m_iModel = model;
}

void CPlaySoundDlg::ShowSeconds(void)
{
	Util::TimeSpan total(m_iTotalSeconds);
	Util::TimeSpan current(m_iCurrentSecond);
	CString strTime;
	if (m_iTotalSeconds < 3600)
	{
		strTime.Format(_T("%02d:%02d/%02d:%02d"), current.GetMinutes(), current.GetSeconds(), total.GetMinutes(), total.GetSeconds());
	}
	else
	{
		strTime.Format(_T("%02d:%02d:%02d/%02d:%02d:%02d"), current.GetHours(), current.GetMinutes(), current.GetSeconds(), total.GetHours(), total.GetMinutes(), total.GetSeconds());
	}
	m_sticSoundDuration.SetWindowText(strTime);
}

void CPlaySoundDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();
	m_procbarSound.SetPos(0);
	m_iCurrentSecond = 0;
	m_iTotalSeconds = 0;
	m_sticNameNO.SetWindowText(_T(""));
	m_sticSoundName.SetWindowText(_T(""));
	m_sticSoundDuration.SetWindowText(_T(""));
	m_vSoundResult.clear();
	m_iCurrentSound = 0;
	ShowWindow_(FALSE);
}

void CPlaySoundDlg::OnButtonNext() 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();
	m_procbarSound.SetPos(0);
	m_iCurrentSecond = 0;
	m_bHasNext = false;
		
	if (m_iModel == 1)
	{
		--m_iCurrentSound;
		if (m_vSoundResult.size() > 1 && m_iCurrentSound >= 0)
		{
			m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename().substr(0, m_vSoundResult[m_iCurrentSound]->filename().length() - 10);
			m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
			m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration().GetTotalSeconds();
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
// 	OnButtonPlay();
}

void CPlaySoundDlg::OnButtonPrev() 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();
	m_procbarSound.SetPos(0);
	m_iCurrentSecond = 0;
	if (m_iModel == 1)
	{
		++m_iCurrentSound;
		if (m_vSoundResult.size() > 1 && m_iCurrentSound < m_vSoundResult.size())
		{
			m_sFilePath = m_vSoundResult[m_iCurrentSound]->filename().substr(0, m_vSoundResult[m_iCurrentSound]->filename().length() - 10);
			m_sticSoundName.SetWindowText(Util::StringOp::ToCString(m_vSoundResult[m_iCurrentSound]->filename().substr(m_vSoundResult[m_iCurrentSound]->filename().length() - 10)));
			m_iTotalSeconds = m_vSoundResult[m_iCurrentSound]->duration().GetTotalSeconds();
			m_procbarSound.SetParam(0, 0, m_iTotalSeconds, 1);
			ShowSeconds();
			return;
		}
		else
		{
			m_iCurrentSound = m_vSoundResult.size() - 1;
		}
	}
		
	SetSound(m_iType, m_iCurrentID, 2, m_sSearchFilter);
// 	OnButtonPlay();
}

void CPlaySoundDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();
	m_procbarSound.SetPos(0);
	m_iCurrentSecond = 0;
	ShowSeconds();
	CString filename;
	m_sticSoundName.GetWindowText(filename);
	filename = Util::StringOp::ToCString(m_sFilePath) + filename;
	if (!filename.IsEmpty())
	{
		CFileStatus   status;
		if (CFile::GetStatus(LPCTSTR(filename),status))
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)LPCTSTR(filename), 1);
			SetTimer(1, 1000, 0);
		}
	}
}

void CPlaySoundDlg::OnCheckPlaycontinue() 
{
	// TODO: Add your control notification handler code here
}

void CPlaySoundDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		++m_iCurrentSecond;
		if (m_iCurrentSecond <= m_iTotalSeconds)
		{
			m_procbarSound.SetPos(m_iCurrentSecond);
			ShowSeconds();
		}
		else
		{
			KillTimer(1);
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();
			m_procbarSound.SetPos(0);
			m_iCurrentSecond = 0;
			ShowSeconds();
			
			if(m_chkPlayContinue.GetCheck() == BST_CHECKED)
			{
				OnButtonNext();
				if (m_bHasNext)
				{
					OnButtonPlay();
				}
			}
		}
		break;
	}
	
	CCEDialog::OnTimer(nIDEvent);
}
