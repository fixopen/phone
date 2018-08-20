// contactgroupdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\MultimediaPhone.h"
#include "contactgroupdlg.h"
#include "../Data/LanguageResource.h"
//#include "ContactDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/SkinStyle.h"
#include "sip.h"

// add by qi 2009_09_27
#include "ContactDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContactGroupDlg dialog


CString delfore		=	L".\\adv\\mjpg\\k5\\common\\通讯录\\删除按钮_fore.png" ;
CString delback		=	L".\\adv\\mjpg\\k5\\common\\通讯录\\删除按钮_back.png" ;

CContactGroupDlg::CContactGroupDlg(CWnd* pParent /*=NULL*/)
	:CCEDialog(CContactGroupDlg::IDD, pParent)
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


BEGIN_MESSAGE_MAP(CContactGroupDlg, CCEDialog)
	//{{AFX_MSG_MAP(CContactGroupDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_OK, OnButtonContactGroupOk)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_CANCEL, OnButtonContactGroupCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_DELETE, OnButtonContactGroupDelete)
	ON_BN_CLICKED(IDC_SETTING_RINGSELECT, OnRingSelect)
	ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTRING, OnRingLst)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_LISTCTRL_CLICK, OnListCltrlClick)
	ON_MESSAGE(WM_STOPTRYRING, OnStopTryRing)
END_MESSAGE_MAP()

void CContactGroupDlg::OnListCltrlClick(WPARAM w, LPARAM l)
{
	LRESULT ret;
	if(w == IDC_SETTING_LSTRING)
		OnRingLst(NULL, &ret);
}

void CContactGroupDlg::StopTryRing()
{
	if(m_MJPGList.GetUnitIsDownStatus(5))
	{
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StopRing();
	}
}
/////////////////////////////////////////////////////////////////////////////
// CContactGroupDlg message handlers
void CContactGroupDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	if(w != 10)
		StopTryRing();

	switch (w)
	{
	case 1001://确定
		OnButtonContactGroupOk();
		break;
	case 1000://取消
		OnButtonContactGroupCancel();
		break;
	case 1002://删除
		OnButtonContactGroupDelete();
		break;

	case 10:
		{
			if(!m_MJPGList.GetUnitIsDownStatus(5))
			{
				CString   str;  
				m_ringEdit.GetWindowText(str);
				if(str.Compare(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))) != 0)
				{
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->OnTimer(1002); //SendMessage(WM_OUTEVENT, 0, 0);
					
					CString s = m_ringDir;
					s += str;
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SetMsgWnd(this);
					((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)s, 1);
					m_MJPGList.SetUnitIsDownStatus(5, TRUE);
					m_MJPGList.SetUnitIsShow(5, TRUE);
				}
			}
			else
			{
				StopTryRing();
			}
		}
		break;
	}
}

void CContactGroupDlg::OnStopTryRing(WPARAM w, LPARAM l)
{
	if(m_MJPGList.GetUnitIsDownStatus(5))
	{
		m_MJPGList.SetUnitIsDownStatus(5, FALSE);
		m_MJPGList.SetUnitIsShow(5, TRUE);

		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->m_mainMp3Dlg_->SendMessage(WM_OUTEVENT, 0, 1);
	}
}

BOOL CContactGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	std::string strTemp;
	CString str;
	
	int xbegin,ybegin;
	int width,height;
	xbegin = 132+190;
	ybegin = 111+105;
	width  = 226;
	height = 32;
	m_edtName.Create(WS_CHILD|WS_VISIBLE, 
		CRect(xbegin, ybegin, xbegin+width, ybegin+height), this, IDC_EDIT_CONTACTGROUP_NAME);
	m_edtName.SetLimitText(15);
	
	ybegin = 158+105;
	m_ringEdit.Create(WS_CHILD|WS_VISIBLE,
		CRect(xbegin, ybegin, xbegin+167, ybegin+height), this, 0xFFFF);
	m_ringEdit.SetIsAutoInput(FALSE);
	
	//按钮
	m_ringButton.Create(L"",Data::g_comboxBMPID[0][Data::g_skinstyle],
		Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE,
		CRect(xbegin+167,ybegin,xbegin+167+58,ybegin+height+7), this, IDC_SETTING_RINGSELECT);
	
	m_ringList.Create(WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER,
		CRect(xbegin,ybegin+38,xbegin+226,ybegin+38+150), this, IDC_SETTING_LSTRING, TRUE, 1);
	m_ringList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], 
		Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_ringList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 508-194-32);
	m_ringList.ShowWindow_(SW_HIDE);

	m_ringStatic.Create(L"", WS_CHILD, CRect(xbegin-2,ybegin+34-3,xbegin-2+226+8,ybegin+34-3+156), this);
	m_ringStatic.SetBorder(TRUE);

	m_cmbSoundTip.Create(WS_CHILD|WS_VISIBLE, 
		CRect(xbegin-4, 205+105-5, xbegin+width, 205+105-5+153), this, IDC_COMBOBOX_CONTACTGROUP_SOUNDTIP);
	
	m_pImageList1 = new CImageList();
	m_pImageList1->Create(32, 32, ILC_COLOR32|ILC_MASK, 6, 6);   
	CBitmap bm;
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[1][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[0][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[2][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[3][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[4][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	bm.LoadBitmap(Data::g_listctrlSelectBMPID[5][Data::g_skinstyle]);
	m_pImageList1->Add(&bm, RGB(255, 0, 255)); 
	bm.DeleteObject();
	m_ringList.SetImageList(m_pImageList1, LVSIL_SMALL);
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(190, 105,190+440, 105+270), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\新建类别.xml");
	m_MJPGList.SetMJPGRect(CRect(190, 105, 190+440, 105+270));
	
	MoveWindow(0,0,800,480);

	m_pContactGroup = boost::shared_ptr<Data::ContactGroup>();
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
		return;
	}
	
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
	
	//不能新建SIM卡组
	if ( 0 == name.compare("SIM卡"))
	{
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pWarningNoFlashDlg->SetTitle("不能新建SIM组别");
		return ;
	}

	m_pContactGroup->name(name);
	
	CString   str;  
	m_ringEdit.GetWindowText(str);
	
	/*
	if (str.GetLength() == 0)
	{
	str = Data::LanguageResource::Get(Data::RI_CARD_DEFAULT).c_str();
	}
	*/

	if(str.Compare(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))) != 0)
	{
		CString s = m_ringDir;
		s += str;
		str = s;
	}
	//str  --------->ringname
	
	m_pContactGroup->ringName(Util::StringOp::FromCString(str));
	
	
	m_cmbSoundTip.GetLBText(m_cmbSoundTip.GetCurSel(), str);
	
	int index = m_cmbSoundTip.GetItemData(m_cmbSoundTip.GetCurSel());
	if (index == 0)
	{
		str = csFlashRecordPath + str;
	}
	else if (index == 1)
	{
		str = csStorageCardRecordPath + str;
	}
	m_pContactGroup->tipName(Util::StringOp::FromCString(str));
	
	bool isInsert = false;
	if (m_pContactGroup->id() == 0)
	{
		std::string filter = "name = '";
		filter += name;
		filter += "'";
		std::vector<boost::shared_ptr<Data::ContactGroup> > result;
		result = Data::ContactGroup::GetFromDatabase(filter);
		if (!result.empty())
		{
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetType(1);     //提示该类别已存在
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return;
		}
		m_pContactGroup->Insert();
		isInsert = true;
	}
	else
	{
		m_pContactGroup->Update();
	}


//	int sel = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->GetTypeListSelected();
//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();
	
//	if ((sel > 0) && isInsert)
//	{
//		++sel;
//	}
//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->SetTypeListSelected(sel);
//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
	
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ClearUp();
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ClearUpGroup();
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->FromGroupDataBase();
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowUpGroupInfo();


	ShowWindow(FALSE);
	SipShowIM(SIPF_OFF);
	
	m_ringList.ShowWindow_(SW_HIDE);
	m_ringStatic.ShowWindow(SW_HIDE);
}

void CContactGroupDlg::OnButtonContactGroupCancel() 
{
 	m_ringList.ShowWindow_(SW_HIDE);
 	m_ringStatic.ShowWindow(SW_HIDE);
//	ShowWindow(FALSE);

	ShowWindow_(SW_HIDE);
	SipShowIM(SIPF_OFF);
}

void CContactGroupDlg::OnButtonContactGroupDelete() 
{
	// TODO: Add your control notification handler code here
	if (m_pContactGroup->id() > 0)
	{
	//	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTGROUP);
	//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
		
		CString title = "确认删除该组别吗?";
		int count = 1 ;
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(title,0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessMax(count);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(SW_SHOW);
	}

	m_ringList.ShowWindow_(SW_HIDE);
 	m_ringStatic.ShowWindow(SW_HIDE);
}

void CContactGroupDlg::SetContactGroup(boost::shared_ptr<Data::ContactGroup> contactgroup)
{	
	CString editType = L".\\adv\\mjpg\\k5\\common\\通讯录\\编辑类别.bmp" ;
	CString newType  = L".\\adv\\mjpg\\k5\\common\\通讯录\\新建类别.bmp" ;

	m_pContactGroup = contactgroup;
//	AddRingTipItems();
	std::string strTemp;
	if (contactgroup->id() == 0)
	{
//		strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);
// 		CString str = strTemp.c_str();
// 		m_sticTitle.SetWindowText(str);
		m_edtName.SetWindowText(_T(""));
//		m_btnDelete.ShowWindow(FALSE);
//		m_MJPGList.SetUnitIsShow(3, TRUE);
//		m_MJPGList.SetUnitIsShow(4, FALSE);
		
		m_MJPGList.SetUnitBitmap(4,newType,L"",false);
		m_MJPGList.SetUnitBitmap(1002,L"",L"",false);
		m_MJPGList.Invalidate();

	}
	else
	{
//		strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
// 		CString str = strTemp.c_str();
// 		m_sticTitle.SetWindowText(str);
		m_edtName.SetWindowText(Util::StringOp::ToCString(contactgroup->name()));
//		m_btnDelete.ShowWindow(TRUE);
//		m_MJPGList.SetUnitIsShow(4, TRUE);
//		m_MJPGList.SetUnitIsShow(3, FALSE);

		m_MJPGList.SetUnitBitmap(4,editType,L"",false);
		m_MJPGList.SetUnitBitmap(1002,delfore,delback,false);
		m_MJPGList.Invalidate();

	}
		

	CString ringName = Util::StringOp::ToCString(contactgroup->ringName()) ;
	CFileStatus   status;
	memset(m_ringDir, 0, 128*2);
	if (ringName.GetLength() > 0 && CFile::GetStatus(ringName, status))
	{
		if(ringName.Compare(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT))) == 0)
			m_ringEdit.SetWindowText(ringName);
		else
		{
			CString s = ringName;
			int nStart = 0;
			int n = s.Find(_T("/"), nStart);
			while(n != -1)
			{	nStart = n+1;
			n = s.Find(_T("/"), nStart);
			}
			CString filename = s.Mid(nStart);
			CString path = s.Mid(0, nStart);
			memcpy(m_ringDir, (TCHAR *)(LPCTSTR)path/*.GetBuffer(256)*/, path.GetLength()*2);
			m_ringEdit.SetWindowText(filename);
		}
	}
	else
	{
		m_ringEdit.SetWindowText(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));
	}

	m_cmbSoundTip.ResetContent();
	m_cmbSoundTip.AddString(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));	
	m_cmbSoundTip.SetItemData(0, -1);
	std::vector<boost::shared_ptr<Data::SoundSegment> > sounds = Data::SoundSegment::GetFromDatabase("type = 2 AND isTip = 1");
	int pos = 0;
	for (int i = 0; i < sounds.size(); ++i)
	{
		pos = sounds[i]->filename().rfind('\\');
		m_cmbSoundTip.AddString(Util::StringOp::ToCString(sounds[i]->filename().substr(pos + 1)));
		if (sounds[i]->filename().substr(0, pos + 1) == ssFlashRecordPath)
		{
			m_cmbSoundTip.SetItemData(i + 1, 0);
		}
		else if(sounds[i]->filename().substr(0, pos + 1) == "\\flashdrv\\MY_RECORD\\")
		{
			m_cmbSoundTip.SetItemData(i + 1, 0);
		}
		else if(sounds[i]->filename().substr(0, pos + 1) == "\\flashdrv\\my_record\\")
		{
			m_cmbSoundTip.SetItemData(i + 1, 0);
		}
		else
		{
			m_cmbSoundTip.SetItemData(i + 1, 1);
		}
	}

	pos = contactgroup->tipName().rfind('\\');
	strTemp = contactgroup->tipName().substr(pos + 1);

	int index = m_cmbSoundTip.SelectString(0, Util::StringOp::ToCString(strTemp));
	if (index == CB_ERR)
	{
//		m_cmbSoundTip.SelectString(0, Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));
		m_cmbSoundTip.SetCurSel(0);
	}


}

void CContactGroupDlg::OnRingLst(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_ringList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int index = m_ringList.GetNextSelectedItem (pos);
		CString s;
		CString s1 = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM));
		s = m_ringList.GetItemText(index, 0);
		if(s.Compare(_T("storagecard")) == 0)
		{
			SetRingLst(_T("/StorageCard/"));
		}
		//默认
		else if(s == Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)))
		{
			m_ringEdit.SetWindowText(s);
			m_ringList.ShowWindow_(SW_HIDE);
			m_ringStatic.ShowWindow(SW_HIDE);
		}

		//上一级
		else if(s.Compare(s1) == 0)
		{
			//	m_chDir
			CString sDir = m_ringDir;
			int n = sDir.Find(_T("/"));
			UINT8 old[16];
			int i = 0;
			while(n != -1)
			{
				old[i++] = n;
				n += 1;
				n = sDir.Find(_T("/"), n);
			}
			sDir = sDir.Mid(0, old[i-2]+1);
			if(sDir == _T("/"))
				SetRingLst(L"");
			else
				SetRingLst((TCHAR *)(LPCTSTR)sDir/*.GetBuffer(128)*/);
		}
		
		else
		{
			int n = s.Find(_T(".mp3"));
			int n1 = s.Find(_T(".MP3"));
			int n2 = s.Find(_T(".wav"));
			int n3 = s.Find(_T(".WAV"));
			if((n1 == -1) && (n == -1) && (n2 == -1) && (n3 == -1))
			{
				CString sDir = m_ringDir;
				sDir += s; 
				sDir += "/";
				SetRingLst((TCHAR *)(LPCTSTR)sDir/*.GetBuffer(128)*/);
			}
			else
			{
				m_ringEdit.SetWindowText(s);
				m_ringList.ShowWindow_(SW_HIDE);
				m_ringStatic.ShowWindow(SW_HIDE);
			}
		}
	}
	
	*pResult = 0;
}

void CContactGroupDlg::OnRingSelect()
{
	if(m_ringStatic.IsWindowVisible())
	{
		m_ringList.ShowWindow_(SW_HIDE);
		m_ringStatic.ShowWindow(SW_HIDE);
	}
	else
	{
		SetRingLst(L"");
		m_ringList.ShowWindow_(SW_SHOW);
		m_ringStatic.ShowWindow(SW_SHOW);
	}
}

extern BOOL DetectDIR(TCHAR *sDir);
void CContactGroupDlg::SetRingLst(TCHAR *dir)
{
	memset(m_ringDir, 0, 128*2);
	memcpy(m_ringDir, dir, wcslen(dir)*2);
	m_ringList.DeleteAllItems();
	m_ringList.SetScrollPos(0, TRUE);
	int ncount = 0;
	if(wcslen(dir) == 0)
	{
		if(DetectDIR((LPTSTR)(LPCTSTR)csStarageCard))
			m_ringList.InsertItem(ncount++, _T("storagecard"), 3);
		m_ringList.InsertItem(ncount++, Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)), 5);
		memcpy(m_ringDir, _T("/flashdrv/my_ring/"), wcslen(_T("/flashdrv/my_ring/"))*2);
	}
	
	else
	{
		CString s = Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM));
		m_ringList.InsertItem(ncount++, s, 4);
		memcpy(m_ringDir, dir, wcslen(dir)*2);
	}
	
	CString sDir = m_ringDir;
	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	
	sDir += "*.*";
	hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return;
	} 
	else 
	{
		char filename[128];
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
		{
			m_ringList.InsertItem(ncount, FindFileData.cFileName, 5);
			m_ringList.SetItemData(ncount++, 0);
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString sS;
			CString s = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
			sS = m_ringList.GetItemText(0, 0);
			if(sS == s)
				m_ringList.InsertItem(1, FindFileData.cFileName, 2);
			else 
				m_ringList.InsertItem(0, FindFileData.cFileName, 2);
			ncount++;
		}
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
			{
				m_ringList.InsertItem(ncount, FindFileData.cFileName, 5);
				m_ringList.SetItemData(ncount++, 0);
			}
			else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				CString sS;
				CString s = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
				sS = m_ringList.GetItemText(0, 0);
				if(sS == s)
					m_ringList.InsertItem(1, FindFileData.cFileName, 2);
				else 
					m_ringList.InsertItem(0, FindFileData.cFileName, 2);
				ncount++;
			}
		}
		
		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);//关闭查找句柄
		} 
		else 
		{
			return;
		}
	}
	m_ringList.SetScrollRagle(TRUE);
	return;
}

LRESULT CContactGroupDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_DELETESELITEM:
	{
		std::string filter = "groupId = " + Util::StringOp::FromInt(m_pContactGroup->id());
		Data::Contact::Remove(filter);
		m_pContactGroup->Remove();

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(1);

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ClearUp();

		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ClearUpGroup();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->FromGroupDataBase();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowUpGroupInfo();

		ShowWindow(FALSE);
	}
	break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
