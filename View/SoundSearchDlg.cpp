// soundsearchdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "soundsearchdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SoundSegment.h"
#include "../Data/ContactInfo.h"
#include "../Data/Contact.h"
#include "SoundDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundSearchDlg dialog


CSoundSearchDlg::CSoundSearchDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CSoundSearchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSoundSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundSearchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundSearchDlg, CCEDialog)
	//{{AFX_MSG_MAP(CSoundSearchDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SOUNDSEARCH_CANCEL, OnButtonSearchSoundCancel)
	ON_BN_CLICKED(IDC_BUTTON_SOUNDSEARCH_SEARCH, OnButtonSearchSoundSearch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundSearchDlg message handlers

BOOL CSoundSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	int X = 35;
	int Y = 40;
	std::string strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	CString str = strTemp.c_str();
	m_btnSearch.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 25 + Y, 402 + X, 45 + Y), this, IDC_BUTTON_SOUNDSEARCH_SEARCH);
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnSearch.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, WS_CHILD|WS_VISIBLE, CRect(340 + X, 100 + Y, 402 + X, 120 + Y), this, IDC_BUTTON_SOUNDSEARCH_CANCEL);
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(115,210,138));
	m_btnCancel.SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(115,210,138));

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_FIND_TITLE);
	str = strTemp.c_str();
	m_sticTitle.Create(str, WS_CHILD|WS_VISIBLE, CRect(0 + X, 0 + Y, 410 + X, 20 + Y), this);
	m_sticTitle.SetColor(RGB(0, 0, 0), RGB(115,210,138));
	m_sticTitle.SetLeftMargin(180);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIP);
	str = strTemp.c_str();
	m_sticTip.Create(str, WS_CHILD|WS_VISIBLE, CRect(16 + X, 25 + Y, 340 + X, 40 + Y), this);
	SetStaticDefaultColor(&m_sticTip);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIME);
	str = strTemp.c_str();
	m_sticTimeRange.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 50 + Y, 88 + X, 65 + Y), this);
	SetStaticDefaultColor(&m_sticTimeRange);
	
	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIMEFROM);
	str = strTemp.c_str();
	m_sticFrom.Create(str, WS_CHILD|WS_VISIBLE, CRect(88 + X, 50 + Y, 105 + X, 65 + Y), this);
	SetStaticDefaultColor(&m_sticFrom);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_TIMETO);
	str = strTemp.c_str();
	m_sticTo.Create(str, WS_CHILD|WS_VISIBLE, CRect(203 + X, 50 + Y, 220 + X, 65 + Y), this);
	SetStaticDefaultColor(&m_sticTo);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NAME);
	str = strTemp.c_str();
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 75 + Y, 88 + X, 90 + Y), this);
	SetStaticDefaultColor(&m_sticName);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_FIND_NUMBER);
	str = strTemp.c_str();
	m_sticNumber.Create(str, WS_CHILD|WS_VISIBLE, CRect(32 + X, 100 + Y, 88 + X, 115 + Y), this);
	SetStaticDefaultColor(&m_sticNumber);

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(94 + X, 75 + Y, 320 + X, 95 + Y), this, IDC_EDIT_SOUNDSEARCH_NAME);
	m_edtName.SetLimitText(15);

	m_edtNumber.Create(WS_CHILD|WS_VISIBLE, CRect(94 + X, 100 + Y, 320 + X, 120 + Y), this, IDC_EDIT_SOUNDSEARCH_NUMBER);
	m_edtNumber.SetLimitText(15);

	m_dtFrom.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(110 + X, 50 + Y, 204 + X, 65 + Y), this, IDC_DATETIME_SOUNDSEARCH_FROM);
	m_dtTo.Create(WS_VISIBLE|WS_CHILD|DTS_SHORTDATEFORMAT, CRect(225 + X, 50 + Y, 319 + X, 65 + Y), this, IDC_DATETIME_SOUNDSEARCH_TO);
	
	m_sticBackground.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(X, Y, 410 + X, 125 + Y), this);
	m_sticBackground.SetColor(RGB(255,255,255), RGB(255,255,255));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundSearchDlg::SetStaticDefaultColor(CCEStatic* cestatic)
{
// 	cestatic->SetColor(RGB(0, 0, 0), RGB(194,235,149));
	cestatic->SetColor(RGB(0, 0, 0), RGB(255,255,255));
}

void CSoundSearchDlg::OnButtonSearchSoundCancel() 
{
	// TODO: Add your control notification handler code here
 	ShowWindow(FALSE);
}

void CSoundSearchDlg::OnButtonSearchSoundSearch() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);

	std::vector<boost::shared_ptr<Data::SoundSegment> > searchResult;

    CTime dateFrom;
    CTime dateTo;
	m_dtFrom.GetTime(dateFrom);
	m_dtTo.GetTime(dateTo);
	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", dateFrom.GetYear(), dateFrom.GetMonth(),dateFrom.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", dateTo.GetYear(), dateTo.GetMonth(),dateTo.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";
	std::vector<boost::shared_ptr<Data::ContactInfoSoundSegmentMap> > cissmResult;
	std::vector<int> soundSegmentIds;

	CString csTel;
	m_edtNumber.GetWindowText(csTel);
	CString csName;
	m_edtName.GetWindowText(csName);

	cissmResult = Data::ContactInfoSoundSegmentMap::GetFromDatabase(filter);
	for (std::vector<boost::shared_ptr<Data::ContactInfoSoundSegmentMap> >::iterator iter = cissmResult.begin(); iter != cissmResult.end(); ++iter)
	{
		if (csTel.IsEmpty() && csName.IsEmpty())
		{
			soundSegmentIds.push_back((*iter)->soundSegmentId());
		}
		else if (!csTel.IsEmpty() && csName.IsEmpty())
		{
			std::string flt = "id = " + Util::StringOp::FromInt((*iter)->contactInfoId());
			flt += " AND telephoneNumber Like '%";
			flt += Util::StringOp::FromCString(csTel);
			flt += "%'";
			std::vector<boost::shared_ptr<Data::ContactInfo> > ciResult;
			ciResult = Data::ContactInfo::GetFromDatabase(flt);
			if (!ciResult.empty())
			{
				soundSegmentIds.push_back((*iter)->soundSegmentId());
			}
		}
		else if (csTel.IsEmpty() && !csName.IsEmpty())
		{
			std::string flt = "id = " + Util::StringOp::FromInt((*iter)->contactInfoId());
			std::vector<boost::shared_ptr<Data::ContactInfo> > ciResult;
			ciResult = Data::ContactInfo::GetFromDatabase(flt);
			if (!ciResult.empty())
			{
				if (ciResult[0]->contact())
				{
					if (ciResult[0]->contact()->name().find(Util::StringOp::FromCString(csName)) != std::string.npos)
					{
						soundSegmentIds.push_back((*iter)->soundSegmentId());
					}
				}
			}
		}
		else
		{
			std::string flt = "id = " + Util::StringOp::FromInt((*iter)->contactInfoId());
			flt += " AND telephoneNumber Like '%";
			flt += Util::StringOp::FromCString(csTel);
			flt += "%'";
			std::vector<boost::shared_ptr<Data::ContactInfo> > ciResult;
			ciResult = Data::ContactInfo::GetFromDatabase(flt);
			if (!ciResult.empty())
			{
				if (ciResult[0]->contact())
				{
					if (ciResult[0]->contact()->name().find(Util::StringOp::FromCString(csName)) != std::string.npos)
					{
						soundSegmentIds.push_back((*iter)->soundSegmentId());
					}
				}
			}
		}
	}

	for (std::vector<int>::iterator i = soundSegmentIds.begin(); i != soundSegmentIds.end(); ++i)
	{
		std::string f = "id = " + Util::StringOp::FromInt(*i);
		std::vector<boost::shared_ptr<Data::SoundSegment> > temp;
		temp = Data::SoundSegment::GetFromDatabase(f);
		for (std::vector<boost::shared_ptr<Data::SoundSegment> >::iterator it = temp.begin(); it != temp.end(); ++it)
		{
			searchResult.push_back(*it);
		}
	}
 	((CSoundDlg*)m_pParentWnd)->ShowSearchResultInList(searchResult);
}