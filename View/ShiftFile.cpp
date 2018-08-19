// ShiftFile.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "ShiftFile.h"
#include "../Pblmember.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString g_destDir;
extern CString g_srcDir;
extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
// extern void CopyDirFiles(TCHAR *src, TCHAR *des, BOOL bOverflow, DWORD &freeBytes);
// extern BOOL DetectDIR(TCHAR *sDir);
// extern BOOL DetectFile(TCHAR *sDir);
// extern float GetFileSize(TCHAR *sFile);

BOOL g_isShift = FALSE;
/////////////////////////////////////////////////////////////////////////////
// CShiftFile dialog

int DetectDir(TCHAR *filename)
{
	BOOL isDirEx = DetectDIR(filename);
	BOOL isFileEx = DetectFile(filename);
	if (isDirEx)
	{
		return 1;
	}
	if (isFileEx)
	{
		return 2;
	}
	return 0;
}

DWORD WINAPI ShiftFileProc(LPVOID lpParameter)   // thread data
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CShiftFile *shiftDlg = (CShiftFile*)lpParameter;
	CString destStr;
	CString srcStr;
	DWORD freeBytes = shiftDlg->m_freeBytes.QuadPart;
	int err = 0;
	for (int i=0; i<shiftDlg->m_fileList.size(); i++)
	{
		if (!g_isShift)
		{
			shiftDlg->KillTimer(0x10);
			shiftDlg->ShowWindow_(SW_HIDE);
			::SendMessage(shiftDlg->m_handle, WM_SHIFTSELITEM, 0, 0);
			return 1;
		}

		CString s;
		destStr = g_destDir + shiftDlg->m_fileList[i];
		srcStr = g_srcDir + shiftDlg->m_fileList[i];
		if (1 == DetectDir((LPTSTR)(LPCTSTR)srcStr))
		{
			CreateDirectoryW(destStr, NULL);
			CopyDirectoryFiles((LPTSTR)(LPCTSTR)srcStr, (LPTSTR)(LPCTSTR)destStr, freeBytes, TRUE);
			if(0 == freeBytes)
			{
				main->m_pWarningNoFlashDlg->SetTitle(L"存储空间不足！转移失败！");
				main->m_pWarningNoFlashDlg->SetHWnd(shiftDlg->m_handle);
				main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
				break;
			}
			err = GetLastError();
			shiftDlg->m_currentFile++;
			shiftDlg->m_shiftProc->SetPos(shiftDlg->m_currentFile);
			DeleteDirectory(srcStr);
		}
		else if (2 == DetectDir((LPTSTR)(LPCTSTR)srcStr))
		{
			DWORD fileBytes = GetFileSize((LPTSTR)(LPCTSTR)srcStr);
			if(freeBytes > fileBytes)
			{
				freeBytes -= fileBytes;
				BOOL ret = CopyFile(srcStr, destStr, FALSE);
				err = GetLastError();
				if(ret)
				{
					shiftDlg->m_currentFile++;
					shiftDlg->m_shiftProc->SetPos(shiftDlg->m_currentFile);
					DeleteFile(srcStr);
				}
			}
			else
			{
				freeBytes = 0;

				main->m_pWarningNoFlashDlg->SetTitle(L"存储空间不足！转移失败！");
				main->m_pWarningNoFlashDlg->SetHWnd(shiftDlg->m_handle);
				main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
				break;
			}
		}

		s.Format(L"%d", shiftDlg->m_currentFile);
		shiftDlg->m_MJPGList.SetUnitText(4, s, TRUE);
	}
	shiftDlg->m_currentFile = 1;
	shiftDlg->KillTimer(0x10);
	shiftDlg->ShowWindow_(SW_HIDE);
	::SendMessage(shiftDlg->m_handle, WM_SHIFTSELITEM, 0, 0);

	return 0;
}

CShiftFile::CShiftFile(CWnd* pParent /*=NULL*/)
	: CCEDialog(CShiftFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShiftFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bCheckStatus = FALSE;
}

void CShiftFile::DoDataExchange(CDataExchange* pDX)
{
	CCEDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShiftFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShiftFile, CCEDialog)
	//{{AFX_MSG_MAP(CShiftFile)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShiftFile message handlers

BOOL CShiftFile::OnInitDialog() 
{
	CCEDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_shiftProc = new CCEProcessBar();
	m_shiftProc->Create(WS_VISIBLE|WS_CHILD, CRect(32,213,323,245), this, 0xFF10);
	m_shiftProc->SetPos(0);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0,0,440,270), this);
	m_MJPGList.SetCurrentLinkFile(L".\\adv\\mjpg\\k5\\中文\\转移文件.xml");
	
	MoveWindow(CRect(180, 110, 620, 380));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

extern BOOL DetectDIR(TCHAR *sDir);
void CShiftFile::OnClickMJPG(WPARAM w, LPARAM l)
{	
	switch(w)
	{
	case 10:      //本地到SD卡或者USB
		if (!m_MJPGList.GetUnitIsDownStatus(10))
		{
			m_MJPGList.SetUnitIsDownStatus(10, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(10, FALSE);
		}
//		GetDiskFreeSpaceEx(_T("\\StorageCard"), &m_freeBytes, &m_totalBytes, &m_totalFreeBytes);

		GetDiskFreeSpaceEx(_T("\\UsbDisk"), &m_freeBytes, &m_totalBytes, &m_totalFreeBytes);
		break;
	case 20:     //USB或者SD卡到本地
		if (!m_MJPGList.GetUnitIsDownStatus(20))
		{
			m_MJPGList.SetUnitIsDownStatus(20, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(20, FALSE);
		}
		GetDiskFreeSpaceEx(_T("\\Flashdrv"), &m_freeBytes, &m_totalBytes, &m_totalFreeBytes);
		break;
	case 1000:   //返回
		OnClickCancel();
		break;
	case 1001:    //确定
		if ((L"" != g_destDir) && (L"" != g_srcDir))
		{
			if(!DetectDIR(_T("\\StorageCard")) && !DetectDIR(_T("\\UsbDisk")))
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetTitle(_T("请插入移动设备"));
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			}
			else
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"确定要转移所选文件吗？", 0);
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
			}
		}
		break;
	}
	
}

void CShiftFile::SetHWnd(HWND handle)
{
	m_handle = handle;
}

void CShiftFile::OnClickCancel()
{
	g_isShift = FALSE;
	ShowWindow_(SW_HIDE);
}

void CShiftFile::OnClickOk()
{
	g_isShift = TRUE;
	HANDLE hThread;
// 	hThread = CreateThread(NULL, 0, ShiftFileProc, this, 0, NULL);
// 	CloseHandle(hThread);	

	SetTimer(0x10, 1000, NULL);
	Sleep(1000);
}

void CShiftFile::OnTimer(UINT nIDEvent)
{
	if (0x10 == nIDEvent)
	{
		if (!m_MJPGList.GetUnitIsDisable(11))
		{
			m_MJPGList.SetUnitIsDownStatus(11, !m_bCheckStatus);
			m_MJPGList.SetUnitIsShow(11, TRUE);
		}
		else if (!m_MJPGList.GetUnitIsDisable(21))
		{
			m_MJPGList.SetUnitIsDownStatus(21, !m_bCheckStatus);
			m_MJPGList.SetUnitIsShow(21, TRUE);
		}

		m_bCheckStatus = !m_bCheckStatus;
	}
}

LRESULT CShiftFile::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DELETESELITEM:
		OnClickOk();
		break;
	default:
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}
