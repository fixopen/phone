// telephonedlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "telephonedlg.h"
#include "../Data/LanguageResource.h"
#include "../MultimediaPhoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg dialog


CTelephoneDlg::CTelephoneDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CTelephoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTelephoneDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTelephoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTelephoneDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTelephoneDlg, CCEDialog)
	//{{AFX_MSG_MAP(CTelephoneDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_HIDE, OnButtonTelephoneHide)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_NOTE, OnButtonTelephoneNote)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_RECORD, OnButtonTelephoneRecord)
	ON_BN_CLICKED(IDC_BUTTON_TELEPHONE_HANDLE, OnButtonTelephoneHandle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg message handlers

BOOL CTelephoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CenterWindow(GetDesktopWindow());	// center to the hpc screen
	int X = 35;
	int Y = 40;
	std::string strTemp = Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC);
	CString str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this, IDC_STATIC_TELEPHONE_TITLE);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(170);

	str = _T("");
	m_sticNameNo.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 25 + Y, 300 + X, 45 + Y), this, IDC_STATIC_TELEPHONE_NAMENO);
	SetStaticDefaultColor(&m_sticNameNo);
	m_sticNameNo.SetFontSize(18);   

	m_sticCompany.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 45 + Y, 300 + X, 60 + Y), this, IDC_STATIC_TELEPHONE_COMPANY);
	SetStaticDefaultColor(&m_sticCompany);

	m_sticDepartment.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 60 + Y, 200 + X, 75 + Y), this, IDC_STATIC_TELEPHONE_DEPARTMENT);
	SetStaticDefaultColor(&m_sticDepartment);

	m_sticDuty.Create(str, WS_CHILD|WS_VISIBLE, CRect(25 + X, 75 + Y, 200 + X, 90 + Y), this, IDC_STATIC_TELEPHONE_DUTY);
	SetStaticDefaultColor(&m_sticDuty);


	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDING);
	str = strTemp.c_str();
	m_sticRecordCaption.Create(str, WS_CHILD, CRect(226 + X, 75 + Y, 280 + X, 95 + Y), this);
	SetStaticDefaultColor(&m_sticRecordCaption);

	str = _T("");
	m_sticRecord.Create(str, WS_CHILD, CRect(282 + X, 75 + Y, 338 + X, 95 + Y), this);
	SetStaticDefaultColor(&m_sticRecord);

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_DURATION);
	str = strTemp.c_str();
	m_sticDurationCaption.Create(str, WS_CHILD, CRect(226 + X, 100 + Y, 280 + X, 120 + Y), this);
	SetStaticDefaultColor(&m_sticDurationCaption);

	str = _T("");
	m_sticDuration.Create(str, WS_CHILD, CRect(282 + X, 100 + Y, 338 + X, 120 + Y), this);
	SetStaticDefaultColor(&m_sticDuration);


	strTemp = Data::LanguageResource::Get(Data::RI_TEL_HIDEBTN);
	str = strTemp.c_str();
	m_btnHide.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_TELEPHONE_HIDE);
	m_btnHide.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnHide.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnHide.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_NOTEBTN);
	str = strTemp.c_str();
	m_btnNote.Create(str, WS_CHILD, CRect(340 + X, 50 + Y, 402 + X, 70 + Y), this, IDC_BUTTON_TELEPHONE_NOTE);
	m_btnNote.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnNote.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnNote.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN);
	str = strTemp.c_str();
	m_btnRecord.Create(str, WS_CHILD, CRect(340 + X, 75 + Y, 402 + X, 95 + Y), this, IDC_BUTTON_TELEPHONE_RECORD);
	m_btnRecord.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnRecord.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnRecord.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN);
	str = strTemp.c_str();
	m_btnHandle.Create(str, WS_CHILD, CRect(340 + X, 100 + Y, 402 + X, 120 + Y), this, IDC_BUTTON_TELEPHONE_HANDLE);
	m_btnHandle.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnHandle.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnHandle.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	m_btnRecordTip.Create(_T(""), WS_CHILD, CRect(206 + X, 75 + Y, 226 + X, 95 + Y), this, IDC_BUTTON_TELEPHONE_RECORDTIP);
	m_btnRecordTip.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(255,255,255));
	m_btnRecordTip.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(255,255,255));
	m_btnRecordTip.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(255,255,255));
	m_btnRecordTip.SetIcon(IDI_ICON2, CSize(12, 12));

	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 125 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));
	
	m_strRecord = CString(Data::LanguageResource::Get(Data::RI_TEL_RECORDEBTN).c_str());
	m_strStopRecord = CString(Data::LanguageResource::Get(Data::RI_TEL_STOPRECORDEBTN).c_str());
	m_strHangOn = CString(Data::LanguageResource::Get(Data::RI_TEL_ACCEPTBTN).c_str());
	m_strHangOff = CString(Data::LanguageResource::Get(Data::RI_TEL_HUNGOFFBTN).c_str());
	m_uiTelephoneTimer = 0;
	m_uiTelephoneSecond = 0;
	m_uiRecordTimer = 0;
	m_uiRecordSecond = 0;
	m_uiRingCount = 0;
	m_uiCurrentContactID = 0;
	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
	m_pNoteDlg = new CNoteDlg(this);
	m_pNoteDlg->Create(CNoteDlg::IDD);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTelephoneDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
	cestatic->SetColor(RGB(0, 0, 0), RGB(255,255,255));
}

void CTelephoneDlg::OnButtonTelephoneHide()
{
	ShowWindow(FALSE);
}

void CTelephoneDlg::OnButtonTelephoneNote()
{
	m_pNoteDlg->ShowWindow(TRUE);
}

void CTelephoneDlg::OnButtonTelephoneRecord()
{
	CString caption;
	m_btnRecord.GetWindowText(caption);
	if (caption == m_strRecord)
	{
		m_btnRecordTip.ShowWindow(TRUE);
		m_sticRecordCaption.ShowWindow(TRUE);
		m_sticRecord.ShowWindow(TRUE);
		m_btnRecord.SetWindowText(m_strStopRecord);

		m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>(new Data::SoundSegment);
		CTime t;
		t = CTime::GetCurrentTime();
		CString filename;
		filename.Format(_T("%d%d%d"), t.GetHour(), t.GetMinute(), t.GetSecond());
		m_spSoundSegment->filename(Util::StringOp::FromCString(filename));
		m_spSoundSegment->type(Data::sstTeleRecord);
		m_uiRecordTimer = SetTimer(2, 1000, 0);

		boost::shared_ptr<Data::ContactInfoSoundSegmentMap> csMap(new Data::ContactInfoSoundSegmentMap);
		csMap->startTime(t);
		m_vCISSMap.push_back(csMap);
	}
	else
	{
		m_btnRecordTip.ShowWindow(FALSE);
		m_sticRecordCaption.ShowWindow(FALSE);
		m_sticRecord.ShowWindow(FALSE);
		m_sticRecord.SetWindowText(_T(""));
		m_btnRecord.SetWindowText(m_strRecord);

		KillTimer(m_uiRecordTimer);
		m_uiRecordTimer = 0;
		m_spSoundSegment->duration(Util::TimeSpan(m_uiRecordSecond));
		m_spSoundSegment->Insert();
		if (!m_vCISSMap.empty())
		{
			m_vCISSMap[m_vCISSMap.size() - 1]->soundSegmentId(m_spSoundSegment->id());
		}

		m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
		m_uiRecordSecond = 0;
	}
}

void CTelephoneDlg::OnButtonTelephoneHandle()
{
	CString caption;
	m_btnHandle.GetWindowText(caption);
	if (caption == m_strHangOn)
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOn();
	}
	else 
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->HungOff();
	}	
}

void CTelephoneDlg::HangOff_(void* param)
{
	KillTimer(m_uiTelephoneTimer);
	m_uiTelephoneTimer = 0;

	if (m_spContactInfo)// != boost::shared_ptr<Data::ContactInfo>()
	{
		m_spContactInfo->duration(Util::TimeSpan(m_uiTelephoneSecond));
		m_spContactInfo->Insert();
	}

	CString caption;
	m_btnRecord.GetWindowText(caption);
	if (caption == m_strStopRecord)
	{
		OnButtonTelephoneRecord();
	}

	if (!m_vCISSMap.empty())
	{
		for (std::vector<boost::shared_ptr<Data::ContactInfoSoundSegmentMap> >::iterator iter = m_vCISSMap.begin(); iter != m_vCISSMap.end(); ++iter)
		{
			(*iter)->contactInfoId(m_spContactInfo->id());
			(*iter)->Insert();
		}
	}
	m_vCISSMap.clear();

	m_spContactInfo = boost::shared_ptr<Data::ContactInfo>();
	m_uiTelephoneSecond = 0;
// 	m_spSoundSegment = boost::shared_ptr<Data::SoundSegment>();
//  m_uiRecordTimer = 0;
//  m_uiRecordSecond = 0;
	m_uiRingCount = 0;
	m_uiCurrentContactID = 0;

	if (m_pNoteDlg->IsWindowVisible())
	{
		m_pNoteDlg->ShowWindow(FALSE);
		//save pictures;
	}
	
	m_sticTitle.SetWindowText(CString(Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str()));
	m_sticDurationCaption.ShowWindow(FALSE);
	m_sticDuration.SetWindowText(_T(""));
	m_sticDuration.ShowWindow(FALSE);
	m_btnNote.ShowWindow(FALSE);
 	m_btnRecord.ShowWindow(FALSE);
	m_btnHandle.SetWindowText(m_strHangOn);
	m_btnHandle.ShowWindow(FALSE);
	m_sticNameNo.SetWindowText(_T(""));
	m_sticCompany.SetWindowText(_T(""));
	m_sticDepartment.SetWindowText(_T(""));
	m_sticDuty.SetWindowText(_T(""));

	ShowWindow(FALSE);
}

void CTelephoneDlg::HangOn_(void* param)
{
	//add ring  lxz 2007.10.18
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->phone_->StopRing();

	m_sticTitle.SetWindowText(CString(Data::LanguageResource::Get(Data::RI_TEL_DIALTELSTC).c_str()));
	m_btnHandle.SetWindowText(m_strHangOff);
	m_btnHandle.ShowWindow(TRUE);
	ShowWindow(TRUE);
}

void CTelephoneDlg::Key_(void* param)
{
	CString number;
	m_sticNameNo.GetWindowText(number);
	number += char(param);
	m_sticNameNo.SetWindowText(number);

	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->direction(Data::dOut);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->isConnect(false);
	}
	m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
}

void CTelephoneDlg::SubKey_(void* param)
{
	CString number;
	m_sticNameNo.GetWindowText(number);
	number += char(param);
	m_sticNameNo.SetWindowText(number);
}

void CTelephoneDlg::Connect_(void* param)
{
	m_sticTitle.SetWindowText(CString(Data::LanguageResource::Get(Data::RI_TEL_TELINGSTC).c_str()));
	m_sticDurationCaption.ShowWindow(TRUE);
	m_sticDuration.ShowWindow(TRUE);
	m_btnNote.ShowWindow(TRUE);
	m_btnRecord.ShowWindow(TRUE);
	m_btnHandle.SetWindowText(m_strHangOff);
	m_btnHandle.ShowWindow(TRUE);
	m_sticDuration.ShowWindow(TRUE);
	m_sticDuration.SetWindowText(_T(""));
	if (m_spContactInfo->direction() == Data::dOut)
	{
		CString number;
		m_sticNameNo.GetWindowText(number);
		m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		CallID_(0);
	}
	m_spContactInfo->isConnect(true);
	m_uiTelephoneTimer = SetTimer(1, 1000, 0);
}

void CTelephoneDlg::Ring_(void* param)
{
	if (!IsWindowVisible())
	{
		//add ring  lxz 2007.10.18
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->phone_->SetSoundValueRing(-1000);
		main->phone_->StartRing(L"/flashdrv/my_ring/rock.wav");


		m_sticTitle.SetWindowText(CString(Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str()));
		m_btnHandle.SetWindowText(m_strHangOn);
		m_btnHandle.ShowWindow(TRUE);
		ShowWindow(TRUE);
	}

	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->direction(Data::dIn);
		m_spContactInfo->isConnect(false);
	}
	++m_uiRingCount;
}

void CTelephoneDlg::Code_(void* param)
{
	CString number;
	m_sticNameNo.GetWindowText(number);
	CString c;
	c.Format(_T("%d"), UINT(param));
	number += c;
	m_sticNameNo.SetWindowText(number);

	m_spContactInfo->telephoneNumber(Util::StringOp::FromCString(number));
}

void CTelephoneDlg::CallID_(void* param)
{
	CString number;
	m_sticNameNo.GetWindowText(number);
	if (number.IsEmpty())
	{
		return;
	}
	
	std::string filter;
	if (m_uiCurrentContactID > 0)
	{
		filter = "id = ";
		filter += Util::StringOp::FromInt(m_uiCurrentContactID);
	}
	else
	{
		filter += "mobilesTelephone = '";
		filter += Util::StringOp::FromCString(number);
		filter += "'";
		filter += " OR mobilesTelephone2 = '";
		filter += Util::StringOp::FromCString(number);
		filter += "'";
		filter += " OR worksTelephone = '";
		filter += Util::StringOp::FromCString(number);
		filter += "'";
		filter += " OR worksTelephone2 = '";
		filter += Util::StringOp::FromCString(number);
		filter += "'";
	}

	std::vector<boost::shared_ptr<Data::Contact> > result = Data::Contact::GetFromDatabase(filter);
	if (!result.empty())
	{
		ShowContact(result[0]);
	}
}

void CTelephoneDlg::ShowContact(boost::shared_ptr<Data::Contact> contact)
{
  	m_spContactInfo->contactId(contact->id());
	m_sticNameNo.SetWindowText(CString(contact->name().c_str()));
	m_sticCompany.SetWindowText(CString(contact->company().c_str()));
	m_sticDepartment.SetWindowText(CString(contact->department().c_str()));
	m_sticDuty.SetWindowText(CString(contact->duty().c_str()));
}

void CTelephoneDlg::DialContact(std::string number, int contactId)
{
	m_uiCurrentContactID = contactId;
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->phone_->DialNumber((char*)number.c_str());
	m_sticNameNo.SetWindowText(CString(number.c_str()));
	if (!IsWindowVisible())
	{
		m_sticTitle.SetWindowText(CString(Data::LanguageResource::Get(Data::RI_TEL_NEWTELSTC).c_str()));
		m_btnHandle.SetWindowText(m_strHangOn);
		m_btnHandle.ShowWindow(TRUE);
		ShowWindow(TRUE);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->EnableLine(TRUE);
	}
	
	if (!m_spContactInfo)
	{
		m_spContactInfo = boost::shared_ptr<Data::ContactInfo>(new Data::ContactInfo);
		m_spContactInfo->direction(Data::dOut);
		m_spContactInfo->startTime(CTime::GetCurrentTime());
		m_spContactInfo->isConnect(false);
		m_spContactInfo->telephoneNumber(number);
	}

	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->setStartState(CMultimediaPhoneDlg::tsKey);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->fireEvent(CMultimediaPhoneDlg::teConnect, 0);
}

void CTelephoneDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		++m_uiTelephoneSecond;
		CTimeSpan ts(m_uiTelephoneSecond);
		CString duration;
		if (m_uiTelephoneSecond > 3600)
		{
			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
		}
		else
		{
			duration.Format(_T("%d:%02d"), ts.GetMinutes(),ts.GetSeconds());
		}
		m_sticDuration.SetWindowText(duration);
	}
	else if (nIDEvent == 2)
	{
		++m_uiRecordSecond;
		CTimeSpan ts(m_uiRecordSecond);
		CString duration;
		if (m_uiRecordSecond > 3600)
		{
			duration.Format(_T("%d:%02d:%02d"), ts.GetHours(), ts.GetMinutes(),ts.GetSeconds());
		}
		else
		{
			duration.Format(_T("%d:%02d"), ts.GetMinutes(),ts.GetSeconds());
		}
		m_sticRecord.SetWindowText(duration);
	}
	
	CCEDialog::OnTimer(nIDEvent);
}
