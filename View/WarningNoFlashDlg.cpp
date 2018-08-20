// AlarmShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "WarningNoFlashDlg.h"
#include "../Pblmember.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarningNoFlashDlg dialog


CWarningNoFlashDlg::CWarningNoFlashDlg(CWnd* pParent /*=NULL*/)
	: CCEDialog(CWarningNoFlashDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarningNoFlashDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWarningNoFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CCEDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarningNoFlashDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWarningNoFlashDlg, CCEDialog)
	//{{AFX_MSG_MAP(CWarningNoFlashDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarningNoFlashDlg message handlers

BOOL CWarningNoFlashDlg::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	m_procbarSound.Create(WS_CHILD, CRect(57 + 54, 190 + 62, 522 + 54, 217 + 62), this, 0xFF10);
	m_procbarSound.SetParam(0, 0, 4, 1);
	m_procbarSound.SetPos(0);

	// TODO: Add extra initialization here
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(54, 62, 746, 358), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\����\\�����ռ䲻��.xml");
	m_MJPGList.SetMJPGRect(CRect(54, 62, 746, 358));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWarningNoFlashDlg::RemoveThread(void *type_)
{
	::sleep(0);
	int count = 0;
	CString text;
	int type = *(int *)type_;
	if(type == 6)
	{
		Data::Contact::Remove("");
		Data::ContactGroup::Remove("");
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->SetTypeListSelected(0);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
		count = Data::Contact::GetDataCount("");
		text.Format(_T("%d"), count);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_sticContactSize.SetWindowText(text);
		
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowTypeItems();
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactDlg->ShowItemsInList();
	}
	else if (type == 7)
	{
		Data::ContactInfo::Remove("");
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ShowItemsInList(-1);
		count = Data::ContactInfo::GetDataCount("");
		text.Format(_T("%d"), count);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_sticContactInfoSize.SetWindowText(text);
		::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 1, 0);
	}
	else if (type == 8)
	{
		DeleteFiles((LPTSTR)(LPCTSTR)CString(csFlashRecordPath + _T("*")));
		if (DetectDIR((LPTSTR)(LPCTSTR)csStorageCardRecordPath))
		{
			DeleteFiles((LPTSTR)(LPCTSTR)CString(csStorageCardRecordPath + _T("*")));
		}
		Data::SoundSegment::Remove("");
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSoundDlg->ShowItemsInList(-1);
		count = Data::SoundSegment::GetDataCount("");
		text.Format(_T("%d"), count);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_sticSoundSize.SetWindowText(text);
		
		std::vector<boost::shared_ptr<Data::ContactInfo> > result = Data::ContactInfo::GetFromDatabase("isSound = 1");
		if (!result.empty())
		{
			int index = 0;
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->m_procbarSound.SetParam(0, 0, result.size(), 1);
			for (std::vector<boost::shared_ptr<Data::ContactInfo> >::iterator iter = result.begin(); iter != result.end(); ++iter)
			{
				(*iter)->isSound(false);
				(*iter)->Update();
				index++;
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->m_procbarSound.SetPos(index);
			}
		}
		
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pContactInfoDlg->ShowItemsInList(-1);
		::SendMessage(((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pMainDlg->GetSafeHwnd(), WM_TELNOTIFY, 2, 0);
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->OnClickMJPG(0, 0);
}

void CWarningNoFlashDlg::SetType(int type)
{
	m_MJPGList.SetUnitIsShow(3, FALSE);
	m_procbarSound.ShowWindow(SW_HIDE);
	m_nType = type;
	if(type == 0)   //����������
	{
		m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\����������.bmp", ".\\adv\\mjpg\\k1\\common\\����������.bmp", FALSE);
	}
	else if(type == 1)		//������Ѵ���
	{
		m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\������Ѵ���.bmp", ".\\adv\\mjpg\\k1\\common\\������Ѵ���.bmp", FALSE);
	}
	else if(type == 2)     //��¼��ʱ������10��
	{
		m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\¼��ʱ������.bmp", ".\\adv\\mjpg\\k1\\common\\¼��ʱ������.bmp", FALSE);
	}
	else if(type == 3)		//��ʾ��ӦΪ����¼��
	{
		m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\ӦΪ����¼��.bmp", ".\\adv\\mjpg\\k1\\common\\ӦΪ����¼��.bmp", FALSE);
	}
 	else if(type == 4)      //��ʾ�Ƿ�ȡ������Ա��¼
 	{
 		m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\��Ҫ����Ա��¼.bmp", ".\\adv\\mjpg\\k1\\common\\ȡ������Ա��¼.bmp", FALSE);
 	}
	else if(type == 5)      //��ʾ�Ƿ�ȡ������Ա��¼
	{
		m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\û����������Ա����.bmp", ".\\adv\\mjpg\\k1\\common\\û����������Ա����.bmp", FALSE);
 	}
	else if(type == 6 || type == 7 || type == 8)     //��ʾ����ɾ���ļ������Ժ�
	{
		if(type == 8)
		{
			m_MJPGList.SetUnitIsShow(3, TRUE);
			m_procbarSound.ShowWindow(SW_SHOW);
		}
		m_MJPGList.SetUnitIsShow(2, FALSE);
		m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\����ɾ��.bmp", ".\\adv\\mjpg\\k1\\common\\����ɾ��.bmp", FALSE);
		SetTimer(1, 2000, NULL);
		SetTimer(2, 500, NULL);
	}
	else
	{
		m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\�����ռ䲻��.bmp", ".\\adv\\mjpg\\k1\\common\\�����ռ䲻��.bmp", FALSE);
	}
}
void CWarningNoFlashDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	m_MJPGList.SetUnitIsShow(3, FALSE);
	m_procbarSound.ShowWindow(SW_HIDE);

	ShowWindow_(SW_HIDE);
	SetType(0xFF);
	KillTimer(1);
	KillTimer(2);
	m_MJPGList.SetUnitIsShow(2, TRUE);
}

void CWarningNoFlashDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 1)
	{
		KillTimer(1);
		RemoveThread(&m_nType);
		//HANDLE hRemoveThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RemoveThread, &m_nType, 0, NULL);
		//CloseHandle(hRemoveThread);
	}
	else if(nIDEvent == 2)
	{
		/*
		static int gOldCount = 0;
		if((gOldCount%3) == 1)
			m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\����ɾ��1.bmp", ".\\adv\\mjpg\\k1\\common\\����ɾ��1.bmp", TRUE);
		else if((gOldCount%3) == 2)
			m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\����ɾ��2.bmp", ".\\adv\\mjpg\\k1\\common\\����ɾ��2.bmp", TRUE);
		else
			m_MJPGList.SetUnitBitmap(1, ".\\adv\\mjpg\\k1\\common\\����ɾ��.bmp", ".\\adv\\mjpg\\k1\\common\\����ɾ��.bmp", TRUE);
		gOldCount++;
		*/
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->CancelBalckLightSaveTimer();
	}
	CCEDialog::OnTimer(nIDEvent);
}
