// fastdialsdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\multimediaphone.h"
#include "..\multimediaphoneDlg.h"
#include "RingSetdlg.h"
#include "../Data/LanguageResource.h"
#include "../Data/SkinStyle.h"
#include "sip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRingSetDlg dialog


CRingSetDlg::CRingSetDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CRingSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRingSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_Data = NULL;
}


void CRingSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRingSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRingSetDlg, CCEDialog)
	//{{AFX_MSG_MAP(CRingSetDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_OK, OnButtonFastDialsOk)
	ON_BN_CLICKED(IDC_BUTTON_FASTDIALS_CANCEL, OnButtonFastDialsCancel)
	ON_MESSAGE(WM_COMMBOX_CLICKED, OnComboSelect)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

void CRingSetDlg::OnComboSelect(WPARAM w, LPARAM l)
{
	UINT16 volume[] = {0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000}; //(5-nVolume)*0xFFFF/5;
	CString s = "";
	int nVolume;
	UINT32 v;

	if(w == IDC_COMBOBOX_SETTING_RING || w == IDC_COMBOBOX_SETTING_RINGV)
	{
		m_cmbRing.GetWindowText(s);
		nVolume =m_cmbRingVolume.GetCurSel();
	}
	else if(w == IDC_COMBOBOX_SETTING_SMSRING || w == IDC_COMBOBOX_SETTING_SMSRINGV)
	{
		m_cmbSMSRing.GetWindowText(s);
		nVolume =m_cmbSMSRingVolume.GetCurSel();
	}
	else if(w == IDC_COMBOBOX_SETTING_SPECRING || w == IDC_COMBOBOX_SETTING_SPECRINGV)
	{
		m_cmbSpecRing.GetWindowText(s);
		nVolume =m_cmbSpecRingVolume.GetCurSel();
	}

	if(s != "")
	{
		CString sFile = "/flashdrv/my_ring/";
		sFile += s;
		v = volume[nVolume]|(volume[nVolume]<<16);
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->g_RingSound = v;
		((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing((LPTSTR)(LPCTSTR)sFile, 1);
	}

}
/////////////////////////////////////////////////////////////////////////////
// CRingSetDlg message handlers
void CRingSetDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->StartRing(L"");
	switch(w)
	{
	case 100:
		OnButtonFastDialsOk();
		break;
	case 101:
		OnButtonFastDialsCancel();
		break;
	}
}

BOOL CRingSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str = "";
	m_chbEnableRing.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(33+54, 66+62, 33+54+35, 66+62+32), this, IDC_CHECK_SETTING_RING);
	m_chbEnableRing.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
//	SetButtonDefaultColor(&m_chbEnableRing);
	m_cmbRing.Create(WS_CHILD|WS_VISIBLE, CRect(195+54, 101+62-10, 195+54+196, 101+62+190), this, IDC_COMBOBOX_SETTING_RING);
	m_cmbRingVolume.Create(WS_CHILD|WS_VISIBLE, CRect(414+54, 90+62, 414+54+136, 90+62+190), this, IDC_COMBOBOX_SETTING_RINGV);

	m_chbEnableSMSRing.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(33+54, 134+62, 33+54+35, 134+62+32), this, IDC_CHECK_SETTING_RING);
	m_chbEnableSMSRing.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	//	SetButtonDefaultColor(&m_chbEnableRing);
	m_cmbSMSRing.Create(WS_CHILD|WS_VISIBLE, CRect(195+54, 171+62-10, 195+54+196, 171+62+190), this, IDC_COMBOBOX_SETTING_SMSRING);
	m_cmbSMSRingVolume.Create(WS_CHILD|WS_VISIBLE, CRect(414+54, 160+62, 414+54+136, 160+62+190), this, IDC_COMBOBOX_SETTING_SMSRINGV);

	m_chbEnableSpecRing.Create(str, WS_CHILD|WS_VISIBLE|BS_CHECKBOX, CRect(33+54, 206+62, 33+54+35, 206+62+32), this, IDC_CHECK_SETTING_RING);
	m_chbEnableSpecRing.SetIcon(IDI_ICON_CHECK1, CSize(32, 32), IDI_ICON_CHECK0, CSize(32, 32));
	//	SetButtonDefaultColor(&m_chbEnableRing);
	m_cmbSpecRing.Create(WS_CHILD|WS_VISIBLE, CRect(195+54, 241+62-10, 195+54+196, 241+62+190), this, IDC_COMBOBOX_SETTING_SPECRING);
	m_cmbSpecRingVolume.Create(WS_CHILD|WS_VISIBLE, CRect(414+54, 230+62, 414+54+136, 230+62+190), this, IDC_COMBOBOX_SETTING_SPECRINGV);
	
	CString sRingVolume[5] = {"最大", "较大", "中间", "较小", "最小"};
	m_cmbRingVolume.ResetContent();
	m_cmbRingVolume.AddString(sRingVolume[0]);
	m_cmbRingVolume.AddString(sRingVolume[1]);
	m_cmbRingVolume.AddString(sRingVolume[2]);
	m_cmbRingVolume.AddString(sRingVolume[3]);
	m_cmbRingVolume.AddString(sRingVolume[4]);

	m_cmbSMSRingVolume.ResetContent();
	m_cmbSMSRingVolume.AddString(sRingVolume[0]);
	m_cmbSMSRingVolume.AddString(sRingVolume[1]);
	m_cmbSMSRingVolume.AddString(sRingVolume[2]);
	m_cmbSMSRingVolume.AddString(sRingVolume[3]);
	m_cmbSMSRingVolume.AddString(sRingVolume[4]);

	m_cmbSpecRingVolume.ResetContent();
	m_cmbSpecRingVolume.AddString(sRingVolume[0]);
	m_cmbSpecRingVolume.AddString(sRingVolume[1]);
	m_cmbSpecRingVolume.AddString(sRingVolume[2]);
	m_cmbSpecRingVolume.AddString(sRingVolume[3]);
	m_cmbSpecRingVolume.AddString(sRingVolume[4]);
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\铃声设置.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRingSetDlg::OnButtonFastDialsOk()
{
	if(m_Data)
	{
		CString str;
		UINT16 volume[] = {0xFF00, 0xcc00, 0x8800, 0x4400, 0x1000}; //(5-nVolume)*0xFFFF/5;
		CString s;
		int nVolume;

		if (m_chbEnableRing.GetCheck() == BST_CHECKED)
		{
		 	m_Data->isUseSpecRing(true);
		}
		else
		{
			m_Data->isUseSpecRing(false);
	 	}
		m_cmbRing.GetWindowText(str);
		s = "/flashdrv/my_ring/";
		s += str;
		m_Data->defaultRingFilename(Util::StringOp::FromCString(s));
		nVolume = m_cmbRingVolume.GetCurSel();
		m_Data->soundVolume(nVolume);

		if (m_chbEnableSMSRing.GetCheck() == BST_CHECKED)
		{
			m_Data->isSmsRing_ = true;
		}
		else
		{
			m_Data->isSmsRing_ = false;
		}
		m_cmbSMSRing.GetWindowText(str);
		s = "/flashdrv/my_ring/";
		s += str;
		m_Data->smsRingFilename_ = Util::StringOp::FromCString(s);
		nVolume = m_cmbSMSRingVolume.GetCurSel();
		m_Data->smsRingVolume_ = nVolume;

		if (m_chbEnableSpecRing.GetCheck() == BST_CHECKED)
		{
			m_Data->isSpecodeRing_ = true;
		}
		else
		{
			m_Data->isSpecodeRing_ = false;
		}
		m_cmbSpecRing.GetWindowText(str);
		s = "/flashdrv/my_ring/";
		s += str;
		m_Data->specodeRingFilename_ = Util::StringOp::FromCString(s);
		nVolume = m_cmbSpecRingVolume.GetCurSel();
		m_Data->specodeRingVolume_ = nVolume;
		
		m_Data->Update();
	}
	
	SipShowIM(SIPF_OFF);

	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);
}

void CRingSetDlg::OnButtonFastDialsCancel()
{
	SipShowIM(SIPF_OFF);
	ShowWindow_(FALSE);
	SipShowIM(SIPF_OFF);
}

void CRingSetDlg::SetRingLst(TCHAR *dir)
{
	m_cmbRing.ResetContent();
	m_cmbSMSRing.ResetContent();
	m_cmbSpecRing.ResetContent();
	
	CString sDir = "/flashdrv/my_ring/*.*";
	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	
	//sDir += "*.*";
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
			m_cmbRing.AddString(FindFileData.cFileName);
			m_cmbSMSRing.AddString(FindFileData.cFileName);
			m_cmbSpecRing.AddString(FindFileData.cFileName);
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

		}
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			//printf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, 128);
			filename[i] = '\0';
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
			{
				m_cmbRing.AddString(FindFileData.cFileName);
				m_cmbSMSRing.AddString(FindFileData.cFileName);
				m_cmbSpecRing.AddString(FindFileData.cFileName);
			}
			else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			//	m_ringList.InsertItem(ncount++, FindFileData.cFileName, 2);
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

	return;
}

void CRingSetDlg::SetRingSetParam(boost::shared_ptr<Data::Setting> data)
{
	m_Data = data;
	extern int FindFileEx(CString s, CString &sPath, CString &sFilename);
	CString s;
	CString filename;
	CString path;
	int nVolume;

	SetRingLst(L"");

	if (m_Data->isUseSpecRing())
	{
		m_chbEnableRing.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnableRing.SetCheck(BST_UNCHECKED);
	}
	s = Util::StringOp::ToCString(m_Data->defaultRingFilename());
	filename = "";
	FindFileEx(s, path, filename);
	m_cmbRing.SetWindowText_(filename);
	nVolume = m_Data->soundVolume();
	m_cmbRingVolume.SetCurSel(nVolume);

	if (m_Data->isSmsRing_)
	{
		m_chbEnableSMSRing.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnableSMSRing.SetCheck(BST_UNCHECKED);
	}
	s = Util::StringOp::ToCString(m_Data->smsRingFilename_);
	filename = "";
	FindFileEx(s, path, filename);
	m_cmbSMSRing.SetWindowText_(filename);
	nVolume = m_Data->smsRingVolume_;
	m_cmbSMSRingVolume.SetCurSel(nVolume);

	if (m_Data->isSpecodeRing_)
	{
		m_chbEnableSpecRing.SetCheck(BST_CHECKED);
	}
	else
	{
		m_chbEnableSpecRing.SetCheck(BST_UNCHECKED);
	}
	s = Util::StringOp::ToCString(m_Data->specodeRingFilename_);
	filename = "";
	FindFileEx(s, path, filename);
	m_cmbSpecRing.SetWindowText_(filename);
	nVolume = m_Data->specodeRingVolume_;
	m_cmbSpecRingVolume.SetCurSel(nVolume);
}
