// SoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "SoundDlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/Data.h"
#include "../Data/ContactInfo.h"
#include "../Data/Contact.h"
#include "../Data/SkinStyle.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SoundSegment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TITLE_NUM	  5 

int g_iRecordCount = 0;
extern int g_iContactCount;
/////////////////////////////////////////////////////////////////////////////
// CSoundDlg dialog

CSoundDlg::CSoundDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundDlg::IDD, pParent)
	, SoundTotal(30000)
	, PageSize(7)
{
	//{{AFX_DATA_INIT(CSoundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundDlg, CDialog)
	//{{AFX_MSG_MAP(CSoundDlg)
		ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CLICK, IDC_LIST_SOUND_TYPE, OnClickListType)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SOUND_LIST, OnClickListList)
	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_SOUND, OnButtonSound)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_SETTIP, OnButtonSetTip)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DETAIL_TIP,OnButtonDetailTIP)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_CONTACT,OnButtonContact)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundDlg message handlers

BOOL CSoundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
    ////////在录音播放的时候，作为只播放一次录音的开关//////
	m_bPopPasswordDlg=true;

	m_lsType.m_nLineHeight = 25;
	m_lsType.m_nFontHeight = 16;
	m_lsType.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|\
		LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER,\
		CRect(6, 27*VHEIGHT, 80, 198*VHEIGHT),\
		this, IDC_LIST_SOUND_TYPE, FALSE);
	m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	m_lsType.InsertColumn(0, _T("Type"), LVCFMT_LEFT, 76);

	m_pImageList = new CImageList();
	m_pImageList->Create(16, m_lsType.m_nLineHeight, ILC_COLOR32|ILC_MASK, 3, 2);   
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP1);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP24);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP25);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP32);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();

	m_lsType.SetImageList(m_pImageList, LVSIL_SMALL);
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_RECORD);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 1);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_SOUND);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 2);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 3);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 0);
	m_lsType.SetScrollRagle(FALSE);

	m_lsList.m_nLineHeight = 27 ;
	m_lsList.m_nFontHeight = 18 ;
	m_lsList.Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|\
		LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER,\
		CRect(81, 27*VHEIGHT, 416, 198*VHEIGHT), this, IDC_LIST_SOUND_LIST, TRUE, 0, FALSE);
	m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
//	m_lsList.InsertColumn(0, _T("Sound"), LVCFMT_LEFT, 86);
	m_lsList.InsertColumn(0, _T("NameNO"), LVCFMT_LEFT, 170);
	m_lsList.InsertColumn(1, _T("Time"), LVCFMT_LEFT, 90);
	m_lsList.InsertColumn(2, _T("Duration"), LVCFMT_LEFT, 55);


	m_pImageList = new CImageList();
	m_pImageList->Create(16, m_lsList.m_nLineHeight, ILC_COLOR32|ILC_MASK, 2, 2);   
	bm.LoadBitmap(IDB_BITMAP23);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	bm.LoadBitmap(IDB_BITMAP26);
	m_pImageList->Add(&bm, RGB(255, 0, 255));
	bm.DeleteObject();
	m_lsList.SetImageList(m_pImageList, LVSIL_SMALL);


	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY);
	str = strTemp.c_str();
	m_btnSound.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 26*VHEIGHT, 471, 45*VHEIGHT), this, IDC_BUTTON_SOUND_SOUND);
//	m_btnSound.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD);
	str = strTemp.c_str();
	m_btnRecord.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 46*VHEIGHT, 471, 65*VHEIGHT), this, IDC_BUTTON_SOUND_RECORD);
//	m_btnRecord.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);
	//strTemp = Data::LanguageResource::Get(Data::RI_SOUND_CONTACT);
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_CONTACT);
	str = strTemp.c_str();
	m_btncontact.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 66*VHEIGHT, 471, 85*VHEIGHT), this, IDC_BUTTON_SOUND_CONTACT);
//	m_btnSearch.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 86*VHEIGHT, 471, 105*VHEIGHT), this, IDC_BUTTON_SOUND_SEARCH);
//	m_btnSearch.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP);
	str = strTemp.c_str();
	m_btnSetTip.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 106*VHEIGHT, 471, 125*VHEIGHT), this, IDC_BUTTON_SOUND_SETTIP);
//	m_btnProtect.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 126*VHEIGHT, 471, 145*VHEIGHT), this, IDC_BUTTON_SOUND_DELETE);
	//	m_btnDelete.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	strTemp=Data::LanguageResource::Get(Data::DETAIL_TIP);
    str=strTemp.c_str();
	m_btnDetail.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 146*VHEIGHT, 471, 165*VHEIGHT), this, IDC_BUTTON_DETAIL_TIP);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.Create(str, Data::g_buttonRectBMPID[0][Data::g_skinstyle], Data::g_buttonRectBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(417, 178*VHEIGHT, 471, 198*VHEIGHT), this, IDC_BUTTON_SOUND_CLOSE);
	m_btnClose.SetBackRGB(Data::g_allFramBackRGB[Data::g_skinstyle]);

	m_sticPanel.Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(416, 26*VHEIGHT, 472, 198*VHEIGHT), this);
	m_sticPanel.SetColor(RGB(0, 0, 0), Data::g_allFrameInFrameLine1RGB[0][Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204*VHEIGHT), this, 2);
	
	TextStruct ts[TITLE_NUM];
	memset(ts, 0, sizeof(TextStruct) * TITLE_NUM);
	
	ts[0].txtRect = CRect(0, 0, 80, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
/*	
	ts[1].txtRect = CRect(81, 0, 160, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_SOUND).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_SOUND).length());
*/	
	ts[1].txtRect = CRect(101, 0, 260, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[1].sTxt,\
		Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).length());
	
	ts[2].txtRect = CRect(260, 0, 340, 20*VHEIGHT);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_LEFT | DT_BOTTOM;
	memcpy(ts[2].sTxt,\
		Data::LanguageResource::Get(Data::RI_RECORD_TIME).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_TIME).length());
	
	ts[3].txtRect = CRect(340, 0, 396, 20*VHEIGHT);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt,\
		Data::LanguageResource::Get(Data::RI_RECORD_DURATION).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_DURATION).length());
	
	ts[4].txtRect = CRect(417, 0, 475, 20*VHEIGHT);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, \
		Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(),\
		Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
	
	m_sticBackground.SetTextStruct(ts, TITLE_NUM);

    m_pDetailTip=new CDetailTip(this);
	m_pDetailTip->Create(CDetailTip::IDD);

	m_pPlaySoundDlg = new CPlaySoundDlg(this);
	m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);
	m_pPlaySoundDlg->SetModel(0);

	m_pSoundSearchDlg = new CSoundSearchDlg(this);
	m_pSoundSearchDlg->Create(CSoundSearchDlg::IDD);

	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);

	m_pRecordSoundDlg = new CRecordSoundDlg(this);
	m_pRecordSoundDlg->Create(CRecordSoundDlg::IDD);

    m_psoundTipDlg=new CTIP(this);
 	m_psoundTipDlg->Create(CTIP::IDD);

	m_bTelephoneInUse = false;
    
	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	
	m_iType = 0;
	setButtonName();
	ShowItemsInList();

	g_iRecordCount = Data::SoundSegment::GetDataCount("");
	
	//currentPage=1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundDlg::SetButtonDefaultColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CSoundDlg::SetButtonSelectedColor(CCEButtonST* button)
{
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_IN, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_OUT, RGB(248,214,147));
	button->SetColor(CCEButtonST::BTNST_COLOR_BK_FOCUS, RGB(248,214,147));
}

void CSoundDlg::OnClickListType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	setButtonName();
	ShowItemsInList();
	*pResult = 0;
}

void CSoundDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	LRESULT ret;
	if(w == IDC_LIST_SOUND_TYPE)
		OnClickListType(NULL, &ret);
	else if(w == IDC_LIST_SOUND_LIST)
		OnClickListList(NULL, &ret);
}

void CSoundDlg::OnClickListList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CSoundDlg::OnButtonSound() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();

	std::vector<boost::shared_ptr<Data::SoundSegment> > result;

	std::string soundFileName;

	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);  
		
		pos = m_lsType.GetFirstSelectedItemPosition();

		std::string filter=m_sListSearchFilter;

        filter = "id = ";
  
   	    filter += Util::StringOp::FromInt(m_lsList.GetItemData(index));
	
	    result = Data::SoundSegment::GetFromDatabase(filter);
          
		if(!result.empty())
		{
			soundFileName=(std::string)result[0]->filename();
		}
        
		m_pPlaySoundDlg->SetSound(m_lsType.GetNextSelectedItem(pos), m_lsList.GetItemData(index), 0, m_sListSearchFilter);
        
		std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->\
			m_pSettingDlg->m_pSetting->playRecordPassword();

		if(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isPlayProtect() 
 		&& !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin&&\
		m_bPopPasswordDlg && !strTemp.empty())
		{     
			//add by qi 20111104
			//密码为空不用输入密码
			m_pPasswordDlg->SetType(CHECK_PASSWORD);
			m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
			m_pPasswordDlg->SetHWnd(this->m_hWnd);
			m_pPasswordDlg->ShowWindow(SW_SHOW);
		}
		else
		{   
			m_pPlaySoundDlg->StartPlay(FALSE);
			m_pPlaySoundDlg->ShowWindow(TRUE);
		}
	}
	else
	{
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;

	}
}

void CSoundDlg::OnButtonRecord()
{
	if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff))
		return;
	m_lsType.SetItemState(2, LVIS_SELECTED, LVIS_SELECTED);
	setButtonName();
	m_pRecordSoundDlg->ShowWindow_(TRUE);

}

void CSoundDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
    m_pSoundSearchDlg->SetSertchCondition(m_iType);
	m_pSoundSearchDlg->ShowWindow(TRUE);
}

void CSoundDlg::OnButtonSetTip() 
{
	std::string strTemp=Data::LanguageResource::Get(Data::RI_ERROR);
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	
	if (pos == NULL)
	{	
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}

	pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{	
		int index = m_lsList.GetNextSelectedItem(pos);
		if (m_iType == 2)
		{
			if (m_vCurrentResult[index]->type() == Data::sstLocalRecord)
			{	
				//change by qi 20110624 以前是10秒，现在50秒
				if ((m_vCurrentResult[index]->duration() < 10 * 5))
				{  
					if (m_vCurrentResult[index]->isTip())
					{
						m_vCurrentResult[index]->isTip(!m_vCurrentResult[index]->isTip());
						m_vCurrentResult[index]->tipName("");
						m_vCurrentResult[index]->Update();
						ShowArrayInList(m_vCurrentResult);   
					}
					else
					{
						m_vCurrentResult[index]->isTip(!m_vCurrentResult[index]->isTip());
						m_vCurrentResult[index]->tipName(Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP));
						m_vCurrentResult[index]->Update();
						ShowArrayInList(m_vCurrentResult);
					}
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->UpdateTips();
				}
				else
				{	
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(false);//提示录音数据超出10秒
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(strTemp);
					std::string str1;
					str1=Data::LanguageResource::Get(Data::RI_TIME_TIP);
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(str1.c_str());
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);//?
				}
			}
			else
			{
				//
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(false);     //提示提示语应该为本地录音
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(strTemp);
				std::string str1;
				str1=Data::LanguageResource::Get(Data::RI_LOCAL_TIP);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(str1.c_str());
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			}
		}
		else
		{   
			m_psoundTipDlg->setTypeIndex(m_iType);
			m_psoundTipDlg->setTipIndex(index);

			//change by qi 20111104
			//传名字			
			m_psoundTipDlg->setCurrentTIp(m_vCurrentResult[index]->name());			
			m_psoundTipDlg->setm_vCurrentResult(m_vCurrentResult);
			m_psoundTipDlg->ShowWindow_(TRUE);

		}
	}			
}

void CSoundDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isAdmin() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
		{	
			std::string pw = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->adminPassword();
			if (!pw.empty())
			{	
				//add by qi 20111104
				//密码为空不用输入密码
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(true);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
			}
		}

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
		std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
	}
	else
	{
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}
}

void CSoundDlg::OnButtonClose()
{
	ShowItemsInList();
	ShowWindow(FALSE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAIN);
}

void CSoundDlg::ShowItemsInList(void)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		return;
	}

	std::string filter;
	int type = m_lsType.GetNextSelectedItem(pos);
	if(type != 3)
		m_iType = type;
	
	if (type == 0)
	{	
		m_sListFilter ="type = " + Util::StringOp::FromInt(Data::sstTeleRecord);
		filter = m_sListFilter;
	}
	else if (type == 1)
	{
		m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstLeaveWord);
		filter = m_sListFilter;
	}
	else if (type == 2)
	{
		m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstLocalRecord);

		filter = m_sListFilter;
	}
	else if (type == 3)
	{
		if (m_sListSearchFilter == "")
		{
			m_lsList.DeleteAllItems();

			return;
		}
		filter = m_sListSearchFilter;
	}

	if (g_iRecordCount > 0)
	{
		m_vCurrentResult = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, Data::SoundSegment::GetCurrentId() + 1, PageSize);
		int count = Data::SoundSegment::GetDataCount(filter);
		m_lsList.m_scollbar_.SetRangle(0, count, PageSize);
		ShowArrayInList(m_vCurrentResult, type);	
		m_lsList.SetScrollRagle(TRUE);
		m_lsList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
	else
	{
		m_lsList.DeleteAllItems();
	}
}

void CSoundDlg::ScrollItemsInList(int step)
{
	if (m_vCurrentResult.empty())
	{
		return;
	}
	std::string filter = m_sListFilter;
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int type = m_lsType.GetNextSelectedItem(pos);

		if (type == m_lsType.GetItemCount() - 1)
		{
			filter = m_sListSearchFilter;
		}
	}
	
	std::vector<boost::shared_ptr<Data::SoundSegment> > temp;
	int currentID = 0;
	
	if (step == SB_LINEUP)
	{
		currentID = m_vCurrentResult[0]->id();
		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dUp, currentID, 1);
		if (!temp.empty())
		{
			m_vCurrentResult.insert(m_vCurrentResult.begin(),temp[0]);
			if (m_vCurrentResult.size() > PageSize)
			{
				m_vCurrentResult.pop_back();
			}
		}
	}
	else if (step == SB_LINEDOWN)
	{
		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();
		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, currentID, 1);
		if (!temp.empty())
		{
			m_vCurrentResult.push_back(temp[0]);
			if (m_vCurrentResult.size() > PageSize)
			{
				m_vCurrentResult.erase(m_vCurrentResult.begin());
			}
		}
	}
	else if (step == SB_PAGEUP)
	{
		currentID = m_vCurrentResult[0]->id();
		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dUp, currentID, PageSize);
		if (temp.size() < PageSize)
		{
			temp.insert(temp.end(), m_vCurrentResult.begin(), m_vCurrentResult.begin() + PageSize - temp.size());
		}
		m_vCurrentResult = temp;
	}
	else if(step == SB_PAGEDOWN)
	{
		currentID = m_vCurrentResult[m_vCurrentResult.size() - 1]->id();

		temp = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, currentID, PageSize);
		if (temp.size() < PageSize)
		{
			m_vCurrentResult.insert(m_vCurrentResult.end(), temp.begin(), temp.end());
			if (m_vCurrentResult.size() > PageSize)
			{
				m_vCurrentResult.erase(m_vCurrentResult.begin(), m_vCurrentResult.begin() + m_vCurrentResult.size() - PageSize);
			}
		}
		else
		{
			m_vCurrentResult = temp;
		}
	}
	ShowArrayInList(m_vCurrentResult);		
}

void CSoundDlg::ShowArrayInList(std::vector<boost::shared_ptr<Data::SoundSegment> > array, int type)
{
	int iSel = 0;

	POSITION pos = m_lsList.GetFirstSelectedItemPosition(); 
	
	if (pos != NULL)
	{   
		iSel = m_lsList.GetNextSelectedItem(pos);   
	}

	m_lsList.DeleteAllItems();

	CString temp;

	for (int i = 0; i < (array).size(); ++i)
	{	
		/*std::string s = array[i]->filename();
		
		int index = s.rfind("\\") + 1;
		s = s.substr(index);
		temp = Util::StringOp::ToCString(s);

		if (!array[i]->played())
		{
			m_lsList.InsertItem(i, temp, 0);
		}
		else
		{
			m_lsList.InsertItem(i, temp, 1);
		}*/

		if (array[i]->type() == 2)
		{
			temp  = Util::StringOp::ToCString(array[i]->tipName());
			CString name = Util::StringOp::ToCString(array[i]->name());
			
			//add by qi 20111104
			//显示内容为："提示语+(名字)"
			if (!temp.IsEmpty() && !name.IsEmpty())
			{
				temp += L"(";
				temp += name;
				temp += L")";
			}
			else if (temp.IsEmpty() && !name.IsEmpty())
			{
				temp = name;
			}
		
			
			//m_lsList.SetItemText(i, 1, temp);
		}
		else
		{   
			//change by qi 20111104
			if (array[i]->name() != "")
			{
				temp = (array[i]->name() + \
					"(" + array[i]->telephoneNumber().number() + ")").c_str();
			}
			else
			{
				temp = array[i]->telephoneNumber().number().c_str();
			}

			//m_lsList.SetItemText(i, 1, temp);
		}
		
		//new 
		if (!array[i]->played())
		{
			m_lsList.InsertItem(i, temp, 0);
		}
		else
		{
			m_lsList.InsertItem(i, temp, 1);
		}
		//

		temp.Format(_T("%02d-%02d %d:%02d"),
									array[i]->startTime().GetMonth(),
									array[i]->startTime().GetDay(),
									array[i]->startTime().GetHour(),
									array[i]->startTime().GetMinute());
		
		//  old 
		//	m_lsList.SetItemText(i, 2, temp);
		m_lsList.SetItemText(i, 1, temp);

		if (array[i]->duration() >= 3600)
		{
			temp.Format(_T("%d:%02d:%02d"), \
				array[i]->duration() / 3600 ,\
				array[i]->duration() % 3600 / 60, array[i]->duration() % 60);
		}
		else
		{
			temp.Format(_T("%02d:%02d"), \
				array[i]->duration() / 60, array[i]->duration() % 60);
		}
	
	//  old 
	//	m_lsList.SetItemText(i, 3, temp);
		m_lsList.SetItemText(i, 2, temp);

		m_lsList.SetItemData(i, array[i]->id());
	}

	if (!array.empty())
	{
		if (iSel > array.size() - 1)
		{
			iSel = array.size() - 1;
		}

	   m_lsList.SetItemState(iSel, LVIS_SELECTED | \
		   LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CSoundDlg::DeleteSelectedItem(void)
{
	POSITION pos = m_lsList.GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		int index = m_lsList.GetNextSelectedItem (pos);
		std::string filename = m_vCurrentResult[index]->filename();

		if (!m_vCurrentResult[index]->played())
		{
			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_nRecordCount--;
			::PostMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 0, 0);
		}
		bool isTip = m_vCurrentResult[index]->isTip();
		int ciId = m_vCurrentResult[index]->contactInfoId();
		m_vCurrentResult[index]->Remove();
		--g_iRecordCount;
		std::string filter;
		filter = "id = " + Util::StringOp::FromInt(ciId);
		std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase(filter);
		if (!result.empty())
		{
			filter = "contactInfoId = ";
			filter += Util::StringOp::FromInt(ciId);
			
			std::vector<boost::shared_ptr<Data::SoundSegment> > result2 = Data::SoundSegment::GetFromDatabase(filter);
			if (result2.empty())
			{
				result[0]->isSound(false);

				result[0]->Update();

				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ShowItemsInList();
			}
		}
		
		CFileStatus   status;

		if (CFile::GetStatus(LPCTSTR(Util::StringOp::ToCString(filename)),status))
		{
			try
			{
				CFile::Remove(LPCTSTR(Util::StringOp::ToCString(filename)));
			}
			catch (CFileException* pEx)
			{
				pEx->Delete();
			}
		}

		ShowItemsInList();		

		if (isTip)
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->UpdateTips();
		}
	}
}

void CSoundDlg::ShowSearchResultInList(std::vector<boost::shared_ptr<Data::SoundSegment> > array)
{
	m_vSearchResult = array;

	POSITION pos = m_lsType.GetFirstSelectedItemPosition();
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);  
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED); 
	}
    m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	ShowArrayInList(array);	
}

void CSoundDlg::Search(std::string filter)
{
	m_sListSearchFilter = filter;

	POSITION pos = m_lsType.GetFirstSelectedItemPosition();   
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);   
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(m_lsType.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	ShowItemsInList();
}

void CSoundDlg::SetTypeListSelected(int index)
{
	POSITION pos = m_lsType.GetFirstSelectedItemPosition(); 
	while (pos != NULL)
	{   
		int iSel = m_lsType.GetNextSelectedItem(pos);
		m_lsType.SetItemState(iSel, 0, LVIS_SELECTED);   
	}
    m_lsType.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}
void CSoundDlg::OnButtonContact()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	std::string str;
	str=Data::LanguageResource::Get(Data::RI_ERROR);
	POSITION pos=m_lsType.GetFirstSelectedItemPosition();

    if (pos==NULL)
	{	
		return;
	}

	pos=m_lsList.GetFirstSelectedItemPosition();
	
	if (pos!=NULL)
	{   
		int isel=m_lsList.GetNextSelectedItem(pos);
		if (m_iType == 2)
		{	
			//add by qi 20111104
			//修改文件名
			m_psoundTipDlg->setTipIndex(isel);
			m_psoundTipDlg->setTypeIndex(m_iType);
			m_psoundTipDlg->setCurrentTIp(m_vCurrentResult[isel]->name());
			m_psoundTipDlg->setm_vCurrentResult(m_vCurrentResult);
			m_psoundTipDlg->ShowWindow_(TRUE);
			//
		}
		else
		{
			int id=m_lsList.GetItemData(isel);
			m_vResult = Data::SoundSegment::GetFromDatabase("id = " + Util::StringOp::FromInt(id));
			if (!m_vResult.empty())
			{
				int id = m_vResult[0]->contactId();
				if (id>0)
				{
					std::vector<boost::shared_ptr<Data::Contact> > result =Data::Contact::GetFromDatabase("id = "+Util::StringOp::FromInt(id));
					if (!result.empty())
					{
						main->m_pContactNewDlg->SetContact(result[0]);
						main->m_pContactNewDlg->ShowWindow(TRUE);
					}
					else
					{
						main->m_pDeleteTipDlg->SetPasswordModel(false);
						main->m_pDeleteTipDlg->SetTitle(str.c_str());
						main->m_pDeleteTipDlg->setM_bIsNewContact(true);
						main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_NO_CONTACT)));
						main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					}
				} 
				else
				{
					if (g_iContactCount < main->m_pContactDlg->ContactTotal)
					{
						boost::shared_ptr<Data::Contact> contact(new Data::Contact);
						contact->id(0);
						std::string number = m_vResult[0]->telephoneNumber().number();
						if (number.size() > 8)
						{
							contact->mobilesTelephone(number);
						}
						else
						{
							contact->worksTelephone(number);
						}
						
						if(m_vResult[0]->tipName() != "")
						{
							contact->name(m_vResult[0]->tipName());
						}
						else if(m_vResult[0]->name() != "")
						{
							contact->name(m_vResult[0]->tipName());
						}
						
						main->SwitchPanel_(IDC_BUTTON_CONTACT);
						main->m_pContactNewDlg->SetContact(contact);
						main->m_pContactNewDlg->ShowWindow(TRUE);
					}
					else
					{
						main->m_pDeleteTipDlg->SetPasswordModel(false);
						main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
						main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_CONTACT_TIP)));
						main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
					}
				}		 
			}
		}
	}
	else
	{
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}
}

void CSoundDlg::SaveSoundSegment(boost::shared_ptr<Data::SoundSegment> ss)
{
	if(ss->duration() == 0)
	{
		ss->Remove();
		return;
	}

	if (ss->id() == 0)
	{   
		ss->Insert();
	}
	else
	{          
		ss->Update();
	}

	if(ss)  //wzx20101217
	{
		ss = boost::shared_ptr<Data::SoundSegment>();
	}

	++g_iRecordCount;

	if (g_iRecordCount > SoundTotal)
	{   
		std::string str;
		str=Data::LanguageResource::Get(Data::RI_CARDINFO_WARNING);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(false);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(str.c_str());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_SOUND_TIP)));
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
		
		std::vector<boost::shared_ptr<Data::SoundSegment> > temp = Data::SoundSegment::GetFromDatabase("", Data::dUp, 0, 1);
		if (!temp.empty())
		{
			temp[0]->Remove();
		}
		--g_iRecordCount;
	}

	ShowItemsInList();
}

LRESULT CSoundDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	switch (message)
	{
	case WM_DELETESELITEM:
		DeleteSelectedItem();
		break;
	case WM_SETTINGPASSWORD:
		main->m_pSettingDlg->m_pSetting->soundProtectPassword(std::string((char*)wParam, lParam));
		main->m_pSettingDlg->m_pSetting->Update();
		break;
	case WM_VSCROLL:
		ScrollItemsInList(wParam);
		break;
	case WM_CHECKPASSWORD:	
         SetTimer(50,800,0);
    	break;
	case WM_TEL_INUSE:
		if (!m_bTelephoneInUse)
		{
			m_bTelephoneInUse = true;
			m_pPlaySoundDlg->CloseSound();
			m_pRecordSoundDlg->CloseSound();
		}
		break;
	case WM_TEL_NOUSE:
		m_bTelephoneInUse = false;
		break;
	case WM_LOCALRECORD:
		OnButtonRecord();
		break;
	case WM_PLAYLEAVEWORD:  
	    if (main->m_pSoundDlg->IsWindowVisible())
		{
		    POSITION pos = m_lsList.GetFirstSelectedItemPosition();
			if (pos != NULL)
			{   
				int index = m_lsList.GetNextSelectedItem (pos);
				pos = m_lsType.GetFirstSelectedItemPosition();
				m_pPlaySoundDlg->SetModel(0);
				m_pPlaySoundDlg->SetSound(m_lsType.GetNextSelectedItem(pos), m_lsList.GetItemData(index), 0, m_sListSearchFilter);
				std::string strTemp = main->m_pSettingDlg->m_pSetting->playRecordPassword();
				if (main->m_pSettingDlg->m_pSetting->isPlayProtect() 
					&& !main->m_pSettingDlg->m_bLogin && \
					m_bPopPasswordDlg && !strTemp.empty()) //add by qi 20111104
				{
					//defect 123
					//m_bPopPasswordDlg=false;
					
					m_pPlaySoundDlg->setonlynew(FALSE);
					//==============================
					m_pPasswordDlg->SetType(CHECK_PASSWORD);
							
					m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
					
					m_pPasswordDlg->SetHWnd(this->m_hWnd);
					
					m_pPasswordDlg->ShowWindow(SW_SHOW);	
				}
				else
				{   
					m_pPlaySoundDlg->SetAutoClose(TRUE);				
					m_pPlaySoundDlg->StartPlay(FALSE);					
					m_pPlaySoundDlg->ShowWindow(TRUE); 				
				}
			}
		}
		else if (main->m_pSoundDlg->m_pRecordSoundDlg->IsWindowVisible())
		{
			m_pRecordSoundDlg->OnBtRecordplay();
		}
		break;
	case WM_CHANGELANGUAGE:
        ChangeLanguage();
		break;
	case WM_KEYDOWN:

		if (main->m_pSoundDlg->IsWindowVisible())
		{
			int i;
			POSITION pos=m_lsList.GetFirstSelectedItemPosition();

			if (pos!=NULL)
			{
				iSel =m_lsList.GetNextSelectedItem(pos);
				i=iSel;
				if (wParam=='U')
				{  
					i--;

					m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

					m_lsList.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
					
					if (i==-1)
					{
						ScrollItemsInList(SB_LINEUP);
						int nPos = m_lsList.m_scollbar_.GetPos();
						nPos--;
						m_lsList.m_scollbar_.SetPos(nPos);
						m_lsList.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
					}
				}
				else if (wParam=='D')
				{   
					i++;
					m_lsList.SetExtendedStyle(m_lsList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
					m_lsList.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
					
					if (i==m_lsList.GetItemCount())
					{
						ScrollItemsInList(SB_LINEDOWN);
						
						int nPos = m_lsList.m_scollbar_.GetPos();
						if (nPos<m_lsList.m_scollbar_.m_nMax-i)
						{
							nPos++;
							m_lsList.m_scollbar_.SetPos(nPos);
							m_lsList.SetItemState(m_lsList.GetItemCount()-1,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); 	
						}	
					}

				}
			}
			m_lsList.Invalidate();
		}
	 break;	
	case WM_CANCEL:
       OnButtonClose();
		break;
		
	case WM_DELETE:
	if (main->m_pSoundDlg->IsWindowVisible())
		{
			if (main->m_pDeleteTipDlg->IsWindowVisible())
			{
				return 0;
			}else
			{
			if(m_lsList.GetItemCount()>0)
			{
				POSITION pos=m_lsList.GetFirstSelectedItemPosition();
				if (pos!=NULL)
				{
					if (main->m_pSettingDlg->m_pSetting->isAdmin() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
					{	
						std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
						if (!pw.empty())
						{	
							//add by qi 20111104
							//密码为空不用输入密码
							main->m_pDeleteTipDlg->SetPasswordModel(true);
							main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
						}
					}
					iSel =m_lsList.GetNextSelectedItem(pos);                
					main->m_pDeleteTipDlg->SetHWnd(m_hWnd);
					std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_TIP);
					main->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
					main->m_pDeleteTipDlg->ShowWindow(TRUE);
				}
			}
		   }
		}
		break;
	case WM_ITYPE:
        if (main->m_pSoundDlg->IsWindowVisible())
		{
            POSITION pos = m_lsType.GetFirstSelectedItemPosition();
			if (pos != NULL)
			{
				int type = m_lsType.GetNextSelectedItem(pos);
				type++;
				m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
				m_lsType.SetItemState(type,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				if (type==4)
				{
					type=0;
					m_lsType.SetExtendedStyle(m_lsType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
					m_lsType.SetItemState(type,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
				}
			}
			setButtonName();
			ShowItemsInList();
		    m_lsType.Invalidate();
		}
		break;
	case WM_SEARCH:
        if (main->m_pSoundDlg->IsWindowVisible())
		{
			m_pSoundSearchDlg->ShowWindow(TRUE);
		}
		break;

	case WM_NEWCONTACT:
		if (main->m_pSoundDlg->IsWindowVisible())
		{
			if (g_iContactCount < main->m_pContactDlg->ContactTotal)
			{
				boost::shared_ptr<Data::Contact> contact(new Data::Contact);
				contact->id(0);
				
				if(!m_vResult.empty())
				{
					std::string number = m_vResult[0]->telephoneNumber().number();
					if (number.size() > 8)
					{
						contact->mobilesTelephone(number);
					}
					else
					{
						contact->worksTelephone(number);
					}
					
					if(m_vResult[0]->tipName() != "")
					{
						contact->name(m_vResult[0]->tipName());
					}
					else if(m_vResult[0]->name() != "")
					{
						contact->name(m_vResult[0]->tipName());
					}
				}
				main->SwitchPanel_(IDC_BUTTON_CONTACT);
				main->m_pContactNewDlg->SetContact(contact);
				main->m_pContactNewDlg->ShowWindow(TRUE);
			}
			else
			{
				main->m_pDeleteTipDlg->SetPasswordModel(false);
				main->m_pDeleteTipDlg->SetTitle(Data::LanguageResource::Get(Data::RI_ERROR));
				main->m_pDeleteTipDlg->SetDelTip(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_SYSTEM_CONTACT_TIP)));
				main->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
			}
		}
		break;
    case WM_KEYNEWCONTACT:
		OnButtonContact();
		break;
	case WM_SHOWLIST:
		ShowItemsInList();
		break;
	case WM_TEL_KEYCODE:
		if(!(main->m_pDeleteTipDlg->IsWindowVisible()||
			m_pRecordSoundDlg->IsWindowVisible()||m_pSoundSearchDlg->IsWindowVisible()||
			m_pDetailTip->IsWindowVisible()||m_psoundTipDlg->IsWindowVisible()||
			m_pPlaySoundDlg->IsWindowVisible()||m_pPasswordDlg->IsWindowVisible()||
			main->m_pMainDlg->m_mainScreenSaveDlg_->IsWindowVisible()))
		{
			std::string c;
			c=wParam;
			findfilter+=c;
	        SetTimer(IDT_SEARCHER_TIME,500,NULL);
		}
	 break;
	}		
	return CDialog::WindowProc(message, wParam, lParam);
}
void CSoundDlg::OnTimer(UINT nIDEvent)
{
	std::string filter;
	if (IDT_SEARCHER_TIME==nIDEvent)
	{
		KillTimer(IDT_SEARCHER_TIME);
		filter += "(telephoneNumber Like '%";		
	
		filter += Data::TelephoneNumber(findfilter).ToString();
	
		filter += "%')";
		Search(filter);
	}
	findfilter="";
	if (50==nIDEvent)
	{
        KillTimer(50);
		m_pPlaySoundDlg->SetAutoClose(TRUE);
		m_pPlaySoundDlg->ShowWindow(TRUE);
		m_pPlaySoundDlg->StartPlay(m_pPlaySoundDlg->onlenew_);
	}
	CDialog::OnTimer(nIDEvent);
	}
void CSoundDlg::ChangeLanguage()
{
	
	m_lsType.DeleteAllItems();
	std::string strTemp;
	CString str;
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_SOUND);
	str = strTemp.c_str();
	m_lsType.InsertItem(0, str, 1);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_RECORD);
	str = strTemp.c_str();
	m_lsType.InsertItem(1, str, 2);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD);
	str = strTemp.c_str();
	m_lsType.InsertItem(2, str, 3);


	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_SEARCHRESULT);
	str = strTemp.c_str();
	m_lsType.InsertItem(3, str, 0);

	m_lsType.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	ShowItemsInList();
	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY);
	str = strTemp.c_str();
	m_btnSound.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LOCALRECORD);
	str = strTemp.c_str();
	m_btnRecord.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_FINDBTN);
	str = strTemp.c_str();
	m_btnSearch.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP);
	str = strTemp.c_str();
	m_btnSetTip.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_RECORD_DELETE);
	str = strTemp.c_str();
	m_btnDelete.SetWindowText(str);

	
	strTemp = Data::LanguageResource::Get(Data::DETAIL_TIP);
	str = strTemp.c_str();
	m_btnDetail.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_PLAY_CLOSE);
	str = strTemp.c_str();
	m_btnClose.SetWindowText(str);

	strTemp = Data::LanguageResource::Get(Data::RI_SOUND_CONTACT);
	str = strTemp.c_str();
	m_btncontact.SetWindowText(str);

	TextStruct ts[6];
	memset(ts, 0, sizeof(TextStruct) * 6);
	
	ts[0].txtRect = CRect(0, 0, 80, 20*VHEIGHT);
	ts[0].txtFontSize = 16;
	ts[0].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[0].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TYPE).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TYPE).length());
	
	ts[1].txtRect = CRect(81, 0, 160, 20*VHEIGHT);
	ts[1].txtFontSize = 16;
	ts[1].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[1].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_SOUND).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_SOUND).length());
	
	ts[2].txtRect = CRect(160, 0, 260, 20*VHEIGHT);
	ts[2].txtFontSize = 16;
	ts[2].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[2].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_NAMENO).length());
	
	ts[3].txtRect = CRect(260, 0, 340, 20*VHEIGHT);
	ts[3].txtFontSize = 16;
	ts[3].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[3].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_TIME).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_TIME).length());
	
	ts[4].txtRect = CRect(340, 0, 396, 20*VHEIGHT);
	ts[4].txtFontSize = 16;
	ts[4].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[4].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_DURATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_DURATION).length());
	
	ts[5].txtRect = CRect(417, 0, 475, 20*VHEIGHT);
	ts[5].txtFontSize = 16;
	ts[5].sAlign = DT_CENTER | DT_BOTTOM;
	memcpy(ts[5].sTxt, Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).c_str(), Data::LanguageResource::Get(Data::RI_RECORD_OPERATION).length());
	
	m_sticBackground.SetTextStruct(ts, 6);
}

void CSoundDlg::OnButtonDetailTIP()
{   
	POSITION pos=m_lsType.GetFirstSelectedItemPosition();
	
    if (pos==NULL)
	{
		return;
	}
	int type=m_lsType.GetNextSelectedItem(pos);
	
	pos=m_lsList.GetFirstSelectedItemPosition();
	
	if (pos!=NULL)
	{   
		int index=m_lsList.GetNextSelectedItem(pos);
		m_pDetailTip->setTipIndex(index);
		CString RecordCaption=m_lsList.GetItemText(index,0);
		CString Name;
		
		if(2 == type)
		{
			Name = m_lsList.GetItemText(index,1);
		}
		else
		{
			if (m_vCurrentResult[index]->tipName() != "")
			{
				Name = m_lsList.GetItemText(index,1);
				if(m_vCurrentResult[index]->name() != "")
				{
					Name += "(";
					Name += Util::StringOp::ToCString(m_vCurrentResult[index]->name());
					Name += ")";
				}
			}
			else
			{
				Name = Util::StringOp::ToCString(m_vCurrentResult[index]->name());
			}
		}
		m_pDetailTip->setTypeIndex(type);
        m_pDetailTip->setTIp(false);
		CString Number=Util::StringOp::ToCString(m_vCurrentResult[index]->telephoneNumber().number());
		CString StartTime=m_lsList.GetItemText(index,2);
		CString Duration=m_lsList.GetItemText(index,3);
		m_pDetailTip->setDetialContent(Name,Number,StartTime,Duration,RecordCaption,true);
		m_pDetailTip->setHwnd(this->m_hWnd);
		m_pDetailTip->setm_vCurrentResult(m_vCurrentResult);
		m_pDetailTip->ShowWindow_(TRUE);		
	}
	else
	{
		// add by yy 20111027
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::string strTemp = Data::LanguageResource::Get(Data::RI_CHOOSEITEM);
        main->m_pCCWarningDlg->SetDelTip(strTemp.c_str());		
		main->m_pCCWarningDlg->ShowWindow_(SW_SHOW);
		return;
	}
}

void CSoundDlg::setButtonName()
{   
	POSITION pos = m_lsType.GetFirstSelectedItemPosition();

	if (pos == NULL)
	{
		return;
	}
	std::string strTemp;
	CString str;
	int type = m_lsType.GetNextSelectedItem(pos);

	if (type==0)
	{  
        strTemp=Data::LanguageResource::Get(Data::RI_SOUND_CONTACT);
		str=strTemp.c_str();
		m_btncontact.SetWindowText(str);

		strTemp=Data::LanguageResource::Get(Data::RT_BUTTON_MODIFY);
		str=strTemp.c_str();
        m_btnSetTip.SetWindowText(str);

	}
	else if (type==1)
	{
		strTemp=Data::LanguageResource::Get(Data::RI_SOUND_CONTACT);
		str=strTemp.c_str();
		m_btncontact.SetWindowText(str);
		
		strTemp=Data::LanguageResource::Get(Data::RT_BUTTON_MODIFY);
		str=strTemp.c_str();
        m_btnSetTip.SetWindowText(str);

	}
	else if (type==2)
	{    
		strTemp=Data::LanguageResource::Get(Data::RT_BUTTON_MODIFY);
		str=strTemp.c_str();
		m_btncontact.SetWindowText(str);

		strTemp=Data::LanguageResource::Get(Data::RI_SOUND_LEAVEWORDTIP);
		str=strTemp.c_str();
		m_btnSetTip.SetWindowText(str);
	}
}

