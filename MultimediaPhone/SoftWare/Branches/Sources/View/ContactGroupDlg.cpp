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
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_OK, OnButtonContactGroupOk)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_CANCEL, OnButtonContactGroupCancel)
	ON_BN_CLICKED(IDC_BUTTON_CONTACTGROUP_DELETE, OnButtonContactGroupDelete)
	ON_BN_CLICKED(IDC_SETTING_RINGSELECT, OnRingSelect)
	ON_NOTIFY(NM_CLICK, IDC_SETTING_LSTRING, OnRingLst)
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
	m_sticName.Create(str, WS_CHILD|WS_VISIBLE, CRect(112, 69, 170, 87), this);
	m_sticName.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticName.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_RING);
	str = strTemp.c_str();
	m_sticRing.Create(str, WS_CHILD|WS_VISIBLE, CRect(112, 90, 170, 106), this);
	m_sticRing.SetColor(RGB(0, 0, 0), Data::g_allFramInRectBackRGB[Data::g_skinstyle]);
	m_sticRing.SetAlign(SS_CENTER);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_SOUNDTIP);
	str = strTemp.c_str();
	m_sticSoudTip.Create(str, WS_CHILD, CRect(6, 65, 66, 85), this);
	m_sticSoudTip.SetColor(RGB(0, 0, 0), RGB(255, 255, 255));

	m_edtName.Create(WS_CHILD|WS_VISIBLE, CRect(174, 69, 288, 87), this, IDC_EDIT_CONTACTGROUP_NAME);

	//m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(174, 90, 288, 200), this, IDC_COMBOBOX_CONTACTGROUP_RING);
	m_ringEdit.Create(WS_CHILD|WS_VISIBLE, CRect(174,90,272,108), this, 0xFFFF);
	m_ringEdit.SetIsAutoInput(FALSE);
	m_ringButton.Create(L"", Data::g_comboxBMPID[0][Data::g_skinstyle], Data::g_comboxBMPID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(273,90,288,106), this, IDC_SETTING_RINGSELECT);
	m_ringList.Create(WS_CHILD|LVS_REPORT|LVS_NOCOLUMNHEADER|LVS_NOSORTHEADER, CRect(174,110,289,194), this, IDC_SETTING_LSTRING, TRUE, 1);
	m_ringList.SetListColor(Data::g_listctrlBackRGB1[Data::g_skinstyle], Data::g_listctrlBackRGB2[Data::g_skinstyle]);
	m_ringList.InsertColumn(0, _T("Filename"), LVCFMT_LEFT, 95);
	m_ringList.ShowWindow_(SW_HIDE);
	
	m_pImageList1 = new CImageList();
	m_pImageList1->Create(12, 12, ILC_COLOR32|ILC_MASK, 6, 6);   
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
	
	m_ringStatic.Create(L"", WS_CHILD, CRect(173,109,290,195), this);
	m_ringStatic.SetBorder(TRUE);

	m_cmbSoundTip.Create(WS_CHILD, CRect(66, 65, 188, 165), this, IDC_COMBOBOX_CONTACTGROUP_SOUNDTIP);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_OKBTN);
	str = strTemp.c_str();
	m_btnOk.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 41, 471, 61), this, IDC_BUTTON_CONTACTGROUP_OK);
	m_btnOk.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_COMN_CANCELBTN);
	str = strTemp.c_str();
	m_btnCancel.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 66, 471, 86), this, IDC_BUTTON_CONTACTGROUP_CANCEL);
	m_btnCancel.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	strTemp = Data::LanguageResource::Get(Data::RI_CARD_DELETETYPEBTN);
	str = strTemp.c_str();
	m_btnDelete.Create(str, Data::g_buttonArcBMPALLDIALOGID[0][Data::g_skinstyle], Data::g_buttonArcBMPALLDIALOGID[1][Data::g_skinstyle], WS_CHILD|WS_VISIBLE, CRect(415, 91, 471, 111), this, IDC_BUTTON_CONTACTGROUP_DELETE);
	m_btnDelete.SetBackRGB(Data::g_allFramAngleBackLineRGB[Data::g_skinstyle]);

	m_sticBackground.Create(CRect(0, 0, 480, 204), this, 1);
		
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
	m_ringList.ShowWindow_(SW_HIDE);
	m_ringStatic.ShowWindow(SW_HIDE);
	ShowWindow(FALSE);
	(CMultimediaPhoneDlg*)theApp.inputDlg_->ShowWindow(FALSE);

	if (!m_pContactGroup)
	{
		m_pContactGroup = boost::shared_ptr<Data::ContactGroup>(new Data::ContactGroup);
	}

	CString s;
	m_edtName.GetWindowText(s);
	m_pContactGroup->name(Util::StringOp::FromCString(s));

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
	int sel = ((CContactDlg*)m_pParentWnd)->GetTypeListSelected();
	((CContactDlg*)m_pParentWnd)->ShowTypeItems();

	if ((sel > 0) && isInsert)
	{
		++sel;
	}
	((CContactDlg*)m_pParentWnd)->SetTypeListSelected(sel);
}

void CContactGroupDlg::OnButtonContactGroupCancel() 
{
	// TODO: Add your control notification handler code here
	m_ringList.ShowWindow_(SW_HIDE);
	m_ringStatic.ShowWindow(SW_HIDE);
	ShowWindow(FALSE);
	(CMultimediaPhoneDlg*)theApp.inputDlg_->ShowWindow(FALSE);
}

void CContactGroupDlg::OnButtonContactGroupDelete() 
{
	// TODO: Add your control notification handler code here
	std::string strTemp = Data::LanguageResource::Get(Data::RI_DELETETIP_CONTACTGROUP);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_pParentWnd->GetSafeHwnd());
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow(TRUE);
}

void CContactGroupDlg::SetContactGroup(boost::shared_ptr<Data::ContactGroup> contactgroup)
{
	m_pContactGroup = contactgroup;
//	AddRingTipItems();
	std::string strTemp;
	if (contactgroup->id() == 0)
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_NEWTYPEBTN);
// 		CString str = strTemp.c_str();
// 		m_sticTitle.SetWindowText(str);
		m_edtName.SetWindowText(_T(""));
		m_btnDelete.ShowWindow(FALSE);
	}
	else
	{
		strTemp = Data::LanguageResource::Get(Data::RI_CARD_EDITTYPEBTN);
// 		CString str = strTemp.c_str();
// 		m_sticTitle.SetWindowText(str);
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

	CString ringName = Util::StringOp::ToCString(contactgroup->ringName()) ;

	if (ringName.GetLength() > 0)
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
			m_ringEdit.SetWindowText(filename);
		}
	}
	else
	{
		m_ringEdit.SetWindowText(Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));
	}

	if (contactgroup->tips())
	{
		int index = m_cmbSoundTip.SelectString(0, Util::StringOp::ToCString(contactgroup->tips()->filename()));
		if (index == CB_ERR)
		{
			m_cmbSoundTip.SelectString(0, Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));
		}

	}
	else
	{
		m_cmbSoundTip.SelectString(0, Util::StringOp::ToCString(Data::LanguageResource::Get(Data::RI_CARD_DEFAULT)));
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
		if(s.Compare(_T("storage card")) == 0)
		{
			SetRingLst(_T("/storage card/"));
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
				SetRingLst(sDir.GetBuffer(128));
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
				SetRingLst(sDir.GetBuffer(128));
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
		if(DetectDIR(_T("/storage card")))
			m_ringList.InsertItem(ncount++, _T("storage card"), 3);
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
		//		printf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
		{
			m_ringList.InsertItem(ncount, FindFileData.cFileName, 5);
			m_ringList.SetItemData(ncount++, 0);
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			m_ringList.InsertItem(ncount++, FindFileData.cFileName, 2);
		}
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			//printf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
			{
				m_ringList.InsertItem(ncount, FindFileData.cFileName, 5);
				m_ringList.SetItemData(ncount++, 0);
			}
			else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				m_ringList.InsertItem(ncount++, FindFileData.cFileName, 2);
			}
		}
		
		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);//关闭查找句柄
		} 
		else 
		{
			//			printf ("FindNextFile error. Error is %u\n", dwError);
			return;
		}
	}
	m_ringList.SetScrollRagle(TRUE);
	return;
}