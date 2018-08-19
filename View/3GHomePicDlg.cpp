// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GHomePicDlg.h"

#ifdef _DEBUG
#define new 
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3GHomePicDlg dialog
BOOL g_isAutoPlay = FALSE;
CString g_destDir = L"";
CString g_srcDir = L"";
CString g_showDir = L"";
extern BOOL g_isShift;
extern BOOL DetectDIR(TCHAR *sDir);
extern BOOL DetectFile(TCHAR *sDir);
extern double GetFileSize(TCHAR *sFile);
extern double GetDirSize(TCHAR *sDir, float &fSize);

C3GHomePicDlg::C3GHomePicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GHomePicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GHomePicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pageCount = 0;
	m_fileCount = 0;
	m_currentPage = 0;
	m_firstSelPage = 0;
	m_lastSelPage = 0;
	m_isSelectAll = FALSE;
	m_pageSize = 5;
	m_offSet = 0;
	m_cardType1 = 0;
	m_cardType2 = 0;
	m_callType = 0;
	m_selCount = 0;
}

void C3GHomePicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GHomePicDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GHomePicDlg, CDialog)
	//{{AFX_MSG_MAP(C3GHomePicDlg)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GHomePicDlg message handlers

BOOL C3GHomePicDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\相册.xml");

	MoveWindow(0, 57, 800, 423);
	SetUnitFont();
	m_MJPGList.SetUnitText(8, L"当前页", TRUE);
	m_MJPGList.SetUnitIsDisable(1, TRUE);
	m_MJPGList.SetUnitIsDisable(2, TRUE);

	m_uiType = 0;
	ShowItemsInList(m_uiType);
	PostMessage(WM_USB_MSG, 0x8000, 0);
	PostMessage(WM_USB_MSG, 0x8000, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GHomePicDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 0:
	case 1:
	case 2:
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
		m_isSelectAll = FALSE;
		m_firstSelPage = 0;
		m_lastSelPage = 0;
		m_uiType = w;
		m_selCount = 0;
		ShowItemsInList(w);
		break;
	case 3:			//选择当前页数
		break;
	case 5:			//上翻
		PageUp();
		break;
	case 6:			//下翻
		PageDown();
		break;
	case 10:      //播放
		FindSelectPage();
		if (m_firstSelPage > 0)
		{
			pMainDlg->m_pMainDlg->m_mainPhotoDlg_->OnOpenFile();
			pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnPhotoBtn(1);
			OnPlayer();
			pMainDlg->AddIcon(Allicon[6],pMainDlg->m_pMainDlg->m_mainVideoDlg_,false);		//lxz 20100528
		}
		else
		{
			pMainDlg->m_pTipDlg->SetTitle("请选择要播放的文件!");
			pMainDlg->m_pTipDlg->ShowWindow_(TRUE);
		}
		break;
	case 11:      //转移文件
		if(!g_isShift)
		{
			FindSelectPage();
			if (m_firstSelPage > 0)
			{
				if (0 == m_uiType)
				{
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(10, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(10, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(11, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(11, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(20, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(20, TRUE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(21, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(21, TRUE);
					pMainDlg->m_pShiftFileDlg->OnClickMJPG(10, 0);
				}
				else
				{
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(20, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(20, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(21, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(21, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(10, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(10, TRUE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDownStatus(11, FALSE);
					pMainDlg->m_pShiftFileDlg->m_MJPGList.SetUnitIsDisable(11, TRUE);
					pMainDlg->m_pShiftFileDlg->OnClickMJPG(20, 0);
				}
				OnShiftFile();
			}
			else
			{
				pMainDlg->m_pTipDlg->SetTitle("请选择要转移的文件!");
				pMainDlg->m_pTipDlg->ShowWindow_(TRUE);
			}
		}
		else
		{
			pMainDlg->m_pShiftFileDlg->ShowWindow_(TRUE);
		}
		break;
	case 12:	  //删除
		FindSelectPage();
		if (m_firstSelPage > 0)
		{
			OnClickDelete();
		}
		else
		{
			pMainDlg->m_pTipDlg->SetTitle("请选择要删除的文件!");
			pMainDlg->m_pTipDlg->ShowWindow_(TRUE);
		}
		break;
	case 50:
		OnSelectAll();
		break;
	case 57:
		if(m_sPreDir.size() > 0)
		{
			CString dir = m_sPreDir[m_sPreDir.size()-1];
			m_sPreDir.pop_back();
			SetPlayList(dir.GetBuffer(dir.GetLength()));
			SetInitShow();
			AddFilePicture();
			ShowArrayInList(m_filePicture);
		}
		break;
	case 100:      //选择
	case 200:
	case 300:
	case 400:
	case 500:
		if (m_UnitStatus[w/100-1])
		{
			m_offSet = w/100 -1;
			OnButtonCheck(w);
		}
		break;
	case 1000:     //返回
		OnExit_();
		break;
	case 1001:    //确定
		OnOK_();
		break;
	default:
		break;
	}
}

void C3GHomePicDlg::SetUnitFont()
{
	for (int i=3; i<=4; i++)
	{
		m_MJPGList.SetUnitFont(i, font_14);
		m_MJPGList.SetUnitColor(i,font_white,TRUE);
	}
	
	m_MJPGList.SetUnitFont(8, font_14);
	m_MJPGList.SetUnitColor(8,font_white,TRUE);
	
	for (i=1; i<=5; i++)
	{
		m_MJPGList.SetUnitFont(i*100+1, font_18);
		m_MJPGList.SetUnitColor(i*100+1,font_white,TRUE);
		m_MJPGList.SetUnitFont(i*100+2, font_18);
		m_MJPGList.SetUnitColor(i*100+2,font_white,TRUE);
	}
}

void C3GHomePicDlg::SetInitShow()
{
	m_fileCount = m_fileDir.size() + m_filePicture.size();
	int count = m_fileCount;
	if (0 == m_fileCount)
	{
		m_currentPage = 0;
	}
	else
	{
		m_currentPage = 1;
	}
	if (0 == count%m_pageSize)
	{
		m_pageCount = count/m_pageSize;
	}
	else
	{
		m_pageCount = count/m_pageSize + 1;
	}

	CString str;
	str.Format(L"/%d",m_pageCount);
	m_MJPGList.SetUnitText(4, str, TRUE);
	str.Empty();
	str.Format(L"%d",m_currentPage);
	m_MJPGList.SetUnitText(3, str, TRUE);

	if (m_currentPage <= 1)   //当第一次显示时上翻按钮不可用
	{
		m_MJPGList.SetUnitIsDisable(5, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDisable(5, FALSE); 
	}
	if (m_fileCount <= m_pageSize)		//当总数小于6时设置下翻按钮不可用
	{
			m_MJPGList.SetUnitIsDisable(6, TRUE);
	}
	else
	{
			m_MJPGList.SetUnitIsDisable(6, FALSE); 
	}
}

void C3GHomePicDlg::ShowItemsInList(int type)
{
	if (type == -1)
	{
		type = m_uiType;
	}
	else
	{
		m_uiType = type;
	}

	if (type == 0)
	{
		SetPlayList(_T("/FlashDrv/MY_PHOTO/"));
	}
	else if (type == 1)
	{
		if (1 == m_cardType1)
		{
			SetPlayList(_T("/usbdisk/"));
		}
		else if (2 == m_cardType1)
		{
			SetPlayList(_T("/storagecard/"));
		}
	}
	else if (type == 2)
	{
		if (1 == m_cardType2)
		{
			SetPlayList(_T("/usbdisk/"));
		}
		else if (2 == m_cardType2)
		{
			SetPlayList(_T("/storagecard/"));
		}
	}
	
	for (int i = 0; i < 3; ++i)
	{
		if (i == m_uiType)
		{
			m_MJPGList.SetUnitIsDownStatus(i, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDownStatus(i, FALSE);
		}
	}

	m_MJPGList.Invalidate();

	SetInitShow();
	AddFilePicture();
	
	ShowArrayInList(m_filePicture);
}

void C3GHomePicDlg::ShowArrayInList(std::vector<CString> fileName)
{
	OnClearAll();

	CString dir = m_chDir;
	CString preDir = L"";
	if(m_sPreDir.size() > 0)
	{
		preDir = m_sPreDir[m_sPreDir.size()-1];
	}

	if(preDir != dir && preDir != L"")
	{
		m_MJPGList.SetUnitIsDisable(57, FALSE);
		m_MJPGList.SetUnitIsShow(57, TRUE, FALSE);
	}
	else
	{
		m_MJPGList.SetUnitIsDisable(57, TRUE);
		m_MJPGList.SetUnitIsShow(57, FALSE, FALSE);
	}

	int index = 100;
	if (m_fileCount > 0)
	{
		CString temp;
		double fileSize;
		for (int i=(m_currentPage - 1)*m_pageSize; i<m_currentPage*m_pageSize; i++)
		{
			fileSize = 0;
			if (fileName.begin() + i < fileName.end())
			{
				CString path = m_chDir + m_filePicture[i];

				m_UnitStatus[index/100-1] = TRUE;
				SetShowStatus(index, TRUE);
				m_MJPGList.SetUnitText(index+1, m_filePicture[i], FALSE);
				if(i<m_fileDir.size())
				{
					m_MJPGList.SetUnitBitmap(index, ".\\adv\\mjpg\\k5\\common\\png\\文件夹.bmp", L".\\adv\\mjpg\\k5\\common\\png\\文件夹.bmp", FALSE);
					m_MJPGList.SetUnitText(index+2, L"", FALSE);
				}
				else
				{
					m_MJPGList.SetUnitBitmap(index, L"", L"", FALSE);

					fileSize = GetFileSize((LPTSTR)(LPCTSTR)path);
					fileSize /= (1024*1024);
					if(fileSize < 1.0 && fileSize > 0.0000001)
					{
						fileSize *= 1024;
						fileSize += 0.05;
						temp.Format(_T("%0.1fK"), fileSize);
					}
					else if(fileSize >= -0.0000001 && fileSize <= 0.0000001)
					{
						temp = L"0字节";
					}
					else
					{
						temp.Format(_T("%0.1fM"), fileSize);
					}
					m_MJPGList.SetUnitText(index+2, temp, FALSE);

					if (1 == m_isFileSelect[i])
					{
						SetShowStatus(index, FALSE); 
					}
				}
			}
			else
			{
				m_MJPGList.SetUnitBitmap(index, L"", L"", FALSE);
				m_UnitStatus[index/100-1] = FALSE;
				SetShowStatus(index, TRUE);
			}

			index += 100;
		}
	}
	else
	{
		for (int i=1; i<=m_pageSize; i++)
		{
			m_MJPGList.SetUnitBitmap(i*100, L"", L"", FALSE);
			m_UnitStatus[i-1] = FALSE;
			SetShowStatus(i*100, TRUE);
		}
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
	}

	m_MJPGList.Invalidate();
}

void C3GHomePicDlg::OnClearAll()
{
	for (int i=1; i<=m_pageSize; i++)
	{
		m_MJPGList.SetUnitText(i*100+1, L"", FALSE);
		m_MJPGList.SetUnitText(i*100+2, L"", FALSE);
	}
	m_MJPGList.Invalidate();
}

void C3GHomePicDlg::SetShowStatus(int unitNO, BOOL status)
{
	if (m_UnitStatus[unitNO/100-1])
	{
		if (status)  //设置为前景
		{
			m_MJPGList.SetUnitIsDisable(unitNO, FALSE);
			m_MJPGList.SetUnitIsDownStatus(unitNO, FALSE);
		}
		else    //设置为后景
		{
			m_MJPGList.SetUnitIsDisable(unitNO, FALSE);
			m_MJPGList.SetUnitIsDownStatus(unitNO, TRUE);
		}
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(unitNO, FALSE);
		m_MJPGList.SetUnitIsDisable(unitNO, TRUE);
		
		m_MJPGList.SetUnitText(unitNO+1, L"", FALSE);
		m_MJPGList.SetUnitText(unitNO+2, L"", FALSE);
		m_MJPGList.SetUnitIsDisable(unitNO+1, TRUE);
		m_MJPGList.SetUnitIsDisable(unitNO+2, TRUE);
	}
	
	m_MJPGList.Invalidate();
}

int C3GHomePicDlg::SetPlayList(TCHAR *dir)
{
	g_srcDir = L"";
	g_srcDir = dir;

	if(!m_filePicture.empty())
	{
		m_filePicture.clear();
		m_fileDir.clear();
		m_isFileSelect.clear();
	}

	memset(m_chDir, 0, 128*2);
	int ncount = 0;
	memcpy(m_chDir, g_srcDir, wcslen(g_srcDir)*2);
	
	CString sDir = m_chDir;
	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
	
	sDir += "*.*";
	hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return 0;
	} 
	else 
	{
		char filename[128];
		//Dprintf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".bmp")||strstr(filename, ".BMP")||strstr(filename, ".jpg")||strstr(filename, ".JPG"))
		{
			m_filePicture.push_back(FindFileData.cFileName);
			m_isFileSelect.push_back(0);
			ncount++;
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			m_fileDir.push_back(FindFileData.cFileName);
			ncount++;
		}
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".bmp")||strstr(filename, ".BMP")||strstr(filename, ".jpg")||strstr(filename, ".JPG")||strstr(filename, ".png")||strstr(filename, ".PNG"))
			{
				m_filePicture.push_back(FindFileData.cFileName);
				m_isFileSelect.push_back(0);
				ncount++;
			}
			else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				m_fileDir.push_back(FindFileData.cFileName);
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
			//Dprintf ("FindNextFile error. Error is %u\n", dwError);
			return 0;
		}
	}
	return ncount;	
}

void C3GHomePicDlg::AddFilePicture()
{
	int dirCount = m_fileDir.size();
	std::vector<CString>::iterator dir;
	std::vector<int>::iterator idir;
	for (int i=0; i<dirCount; i++)
	{
		dir = m_filePicture.begin();
		idir = m_isFileSelect.begin();
		m_filePicture.insert(dir, m_fileDir[i]);
		m_isFileSelect.insert(idir, 0);
	}
}

void C3GHomePicDlg::SetScreenPicture()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString filename = m_chDir;
	main->m_pMainDlg->m_mainPhotoDlg_->m_PhotoList.clear();
	if(main->m_pMainDlg->m_mainPhotoDlg_->m_ScreenPhotoList.size() > 0)
	{
		main->m_pMainDlg->m_mainPhotoDlg_->m_PhotoList = main->m_pMainDlg->m_mainPhotoDlg_->m_ScreenPhotoList;
	}
	else
	{
		for(int i=m_fileDir.size(); i<m_isFileSelect.size(); i++)
		{
			CString filename = m_chDir + m_filePicture[i];
			main->m_pMainDlg->m_mainPhotoDlg_->m_PhotoList.push_back(filename);
			filename.Empty();
		}
	}
}

void C3GHomePicDlg::OnSelectAll()
{
	//wangzhenxing0923
	if (m_fileCount > 0)
	{
		if (!m_isSelectAll)
		{
			m_isSelectAll = TRUE;
			m_firstSelPage = 1;
			m_lastSelPage = m_pageCount;
			m_MJPGList.SetUnitIsDownStatus(50, TRUE);
			for (int i=0; i<m_fileCount; i++)
			{
				m_isFileSelect[i] = 1;
			}
			m_selCount = m_isFileSelect.size();
		}
		else
		{
			m_isSelectAll = FALSE;
			m_firstSelPage = 0;
			m_lastSelPage = 0;
			m_MJPGList.SetUnitIsDownStatus(50, FALSE);
			for (int i=0; i<m_fileCount; i++)
			{
				m_isFileSelect[i] = 0;
			}
			m_selCount = 0;
		}
		for (int i=100; i<=500; i+=100)
		{
			SetShowStatus(i, !m_isSelectAll);
		}
	}
	else
	{
		m_isSelectAll = FALSE;
		m_firstSelPage = 0;
		m_lastSelPage = 0;
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
		m_MJPGList.Invalidate();
	}
}

void C3GHomePicDlg::OnButtonCheck(int uintNO)
{
	int index = (m_currentPage-1)*m_pageSize + m_offSet;
	if (m_fileCount > 0)
	{
		if(index >= m_fileDir.size())
		{
			if (m_isFileSelect[index] == 0)
			{
				m_isFileSelect[index] = 1;
				m_selCount++;
				if(m_selCount == m_isFileSelect.size())
				{
					m_isSelectAll = TRUE;
					m_firstSelPage = 1;
					m_lastSelPage = m_pageCount;
					m_MJPGList.SetUnitIsDownStatus(50, TRUE);
				}
				SetShowStatus(uintNO, FALSE);
			}
			else
			{
				m_isFileSelect[index] = 0;
				m_selCount--;
				if(m_selCount < m_isFileSelect.size())
				{
					m_isSelectAll = FALSE;
					m_firstSelPage = 0;
					m_lastSelPage = 0;
					m_MJPGList.SetUnitIsDownStatus(50, FALSE);
				}
				SetShowStatus(uintNO, TRUE);
			}
		}
		else
		{
			m_sPreDir.push_back(m_chDir);
			CString dir = m_chDir+m_MJPGList.GetUnitText(uintNO+1)+"/";
			SetPlayList(dir.GetBuffer(dir.GetLength()));
			SetInitShow();
			AddFilePicture();
			ShowArrayInList(m_filePicture);
		}
	}
}

void C3GHomePicDlg::FindSelectPage()
{
	BOOL find = FALSE;
	if (!m_isSelectAll)
	{
		for (int i=0; i<m_fileCount; i++)
		{
			if (1 == m_isFileSelect[i])
			{
				if (0 == m_firstSelPage)
				{
					m_firstSelPage = i/m_pageSize + 1;
				}
				m_lastSelPage = i/m_pageSize + 1;
				find = TRUE;
			}
		}
		if (!find)
		{
			m_firstSelPage = 0;
			m_lastSelPage = 0;
		}
	}
}

void C3GHomePicDlg::PageUp()
{
	m_currentPage--;
	
	if (m_currentPage <= 1)
	{
		m_currentPage = 1;
		m_MJPGList.SetUnitIsDisable(5, TRUE);
	}
	if (m_currentPage*m_pageSize < m_fileCount)
	{
		m_MJPGList.SetUnitIsDisable(6, FALSE);
	}
	
	CString str;
	str.Format(L"%d", m_currentPage);
	m_MJPGList.SetUnitText(3, str, TRUE);
	str.Empty();
	
	ShowArrayInList(m_filePicture);
}

void C3GHomePicDlg::PageDown()
{
	m_currentPage++;   
	
	if (m_currentPage > 1)
	{
		m_MJPGList.SetUnitIsDisable(5, FALSE);
	}
	if (m_currentPage*m_pageSize > m_fileCount)  
	{
		m_MJPGList.SetUnitIsDisable(6, TRUE);
		for (int i=m_pageSize; i>m_fileCount-(m_currentPage-1)*m_pageSize; i--)
		{
			int row = i*100;
			m_MJPGList.SetUnitIsDownStatus(row, FALSE);
			OnClearAll();
		}
	}
	else if (m_currentPage*m_pageSize == m_fileCount)
	{
		m_MJPGList.SetUnitIsDisable(6, TRUE);
	}
	
	CString str;
	str.Format(L"%d", m_currentPage);
	m_MJPGList.SetUnitText(3, str, TRUE);
	str.Empty();
	
	OnClearAll();
	ShowArrayInList(m_filePicture); 
}

void C3GHomePicDlg::OnClickDelete()
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	if (0 == m_firstSelPage)
		return;
	int count = 0;
	for(int i=(m_firstSelPage-1)*m_pageSize; i<m_fileCount; i++)
	{
		if(1 == m_isFileSelect[i] && i >= m_fileDir.size())
			count++;
	}

	CString str;
	str.Format(L"%d", count);
	str = L"是否删除已选中的" + str +L"个文件?";
	main->m_pDeleteTipDlg->SetTitle(str, 0);
	main->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	main->m_pDeleteTipDlg->SetProcessMax(count);
	if (main->m_pSettingDlg->m_pSetting->isAdmin() && !main->m_pSettingDlg->m_bLogin)
	{
		main->m_pDeleteTipDlg->SetPasswordModel(true);
	}
	std::string pw = main->m_pSettingDlg->m_pSetting->adminPassword();
	main->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
	main->m_pDeleteTipDlg->ShowWindow_(TRUE);
}
extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
void C3GHomePicDlg::DeleteSelectedItem()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	size_t pageStartIndex = m_fileDir.size();
	std::vector<int>::iterator checkItem = m_isFileSelect.begin() + pageStartIndex;
	std::vector<CString>::iterator checkFile = m_filePicture.begin() + pageStartIndex;
	int delCount = 0;
	for (int i=pageStartIndex; (checkItem != m_isFileSelect.end())
		&& (checkItem < (m_isFileSelect.begin() + m_lastSelPage * m_pageSize));)
	{
		if (*checkItem == 1)
		{
			CString s= m_chDir + m_filePicture[i];
			DeleteFile(s);
			if(0 == GetLastError())
			{
				checkFile = m_filePicture.erase(checkFile);
				checkItem = m_isFileSelect.erase(checkItem);
			}
			else
			{
				++checkItem;
				++checkFile;
				++i;

				main->m_pWarningNoFlashDlg->SetTitle(L"无法删除该文件!请确定文件是否受保护!");
				main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			}

			delCount++;
			main->m_pDeleteTipDlg->SetProcessPos(delCount);
		}
		else
		{
			++checkItem;
			++checkFile;
			++i;
		}
	}

	//从新求当前总记录数
	m_fileCount = m_filePicture.size();
	if (0 == m_fileCount)
	{
		m_MJPGList.SetUnitText(3, "0", TRUE);
		m_MJPGList.SetUnitText(4, "/0", TRUE);
		m_MJPGList.SetUnitIsDisable(5, TRUE);
		m_MJPGList.SetUnitIsDisable(6, TRUE);
		m_pageCount = 0;
		m_currentPage = 0;
	}
	else
	{
		if (0 == m_fileCount%m_pageSize)
		{
			m_pageCount = m_fileCount/m_pageSize;
		}
		else
		{
			m_pageCount = m_fileCount/m_pageSize + 1;
		}
		CString str;
		str.Format(L"/%d", m_pageCount);
		m_MJPGList.SetUnitText(4, str, TRUE);
		if (m_fileCount <= m_pageSize)
		{
			m_MJPGList.SetUnitIsDisable(6, TRUE);
		}
		if ((m_currentPage - 1) * m_pageSize >= m_filePicture.size())
			m_currentPage = m_pageCount;
		str.Empty();
		str.Format(L"%d",m_currentPage);
		m_MJPGList.SetUnitText(3, str, TRUE);
		//检查当前页的情况
		if (m_currentPage == m_pageCount)
		{
			m_MJPGList.SetUnitIsDisable(6, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(6, FALSE);
		}
		if (m_currentPage == 1)
		{
			m_MJPGList.SetUnitIsDisable(5, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(5, FALSE);
		}
	}
	ShowArrayInList(m_filePicture);
}

void C3GHomePicDlg::OnShiftFile()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	g_destDir = L"";
	g_showDir = L"";

	if (0 == m_firstSelPage)
		return;
	if(!DetectDIR(_T("\\StorageCard")) && !DetectDIR(_T("\\UsbDisk")))
	{
		main->m_pWarningNoFlashDlg->SetTitle(_T("请插入移动设备"));
		main->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
		main->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
		return;
	}
	
	if (0 == m_uiType)
	{
		if (DetectDIR(L"/usbdisk"))
		{
			g_destDir = L"/UsbDisk/";
			g_showDir = L"U盘";
		}
		else if (DetectDIR(L"/storagecard"))
		{
			g_destDir = L"/StorageCard/";
			g_showDir = L"SD卡";
		}
	}
	else if (1 == m_uiType)
	{
		if (1 == m_cardType1)
		{
			g_showDir = L"U盘";
		}
		else if (2 == m_cardType1)
		{
			g_showDir = L"SD卡";
		}

		g_destDir = L"/FlashDrv/MY_PHOTO/";
	}
	else if (2 == m_uiType)
	{
		if (1 == m_cardType2)
		{
			g_showDir = L"U盘";
		}
		else if (2 == m_cardType2)
		{
			g_showDir = L"SD卡";
		}

		g_destDir = L"/FlashDrv/MY_PHOTO/";
	}

	main->CancelBalckLightSaveTimer();
	main->m_pShiftFileDlg->m_shiftProc->SetPos(0);
	main->m_pShiftFileDlg->SetHWnd(this->GetSafeHwnd());
	main->m_pShiftFileDlg->ShowWindow_(TRUE);
	
	main->m_pShiftFileDlg->m_fileList.clear();
	int nFileSelected = 0;
	for(int loop = 0; loop < m_fileCount; loop++)
	{
		if(m_isFileSelect[loop] == 1)
		{
			char filename[128];
			int n = wcstombs( filename, (LPTSTR)(LPCTSTR)m_filePicture[loop], 128);
			filename[n] = '\0';
			main->m_pShiftFileDlg->m_fileList.push_back(filename);
			nFileSelected++;
		}
	}

	CString s;
	s.Format(L"/%d", nFileSelected);
	main->m_pShiftFileDlg->m_MJPGList.SetUnitText(5, s, FALSE);
	main->m_pShiftFileDlg->m_MJPGList.SetUnitText(4, L"0", FALSE);
	main->m_pShiftFileDlg->m_MJPGList.SetUnitText(12, g_showDir, FALSE);
	main->m_pShiftFileDlg->m_MJPGList.SetUnitText(22, g_showDir, FALSE);
	main->m_pShiftFileDlg->m_MJPGList.Invalidate();
	main->m_pShiftFileDlg->m_fileCount = nFileSelected;
	main->m_pShiftFileDlg->m_currentFile = 0;
	main->m_pShiftFileDlg->m_shiftProc->SetParam(0, 0, nFileSelected, 1);
}

LRESULT C3GHomePicDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch (message)
	{
	case WM_DELETESELITEM:
		DeleteSelectedItem();
		break;
	case WM_SHIFTSELITEM:
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
		m_isSelectAll = FALSE;
		SetPlayList(m_chDir);
		SetInitShow();
		AddFilePicture();
		ShowArrayInList(m_filePicture);
		break;
	};
	return CDialog::WindowProc(message, wParam, lParam);
}

void C3GHomePicDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
	if (w == 0x8000) //insert
	{
		::Sleep(3000);
		if (m_MJPGList.GetUnitIsDisable(1))
		{
			if(DetectDIR(_T("/usbdisk")))
			{
				m_MJPGList.SetUnitIsDisable(1, FALSE);
				m_cardType1 = 1;
				m_MJPGList.SetUnitBitmap(1, "./ADV/MJPG/k5/common/相册/U盘_fore.BMP", "./ADV/MJPG/k5/common/相册/U盘_back.BMP", TRUE);
			}
			else if(DetectDIR(_T("/storagecard")))
			{
				m_MJPGList.SetUnitIsDisable(1, FALSE);
				m_cardType1 = 2;
				m_MJPGList.SetUnitBitmap(1, "./ADV/MJPG/k5/common/相册/SD卡_fore.BMP", "./ADV/MJPG/k5/common/相册/SD卡_back.BMP", TRUE);
			}
		}
		else if (m_MJPGList.GetUnitIsDisable(2))
		{
			if(DetectDIR(_T("/usbdisk")) && (m_cardType1 != 1))
			{
				m_MJPGList.SetUnitIsDisable(2, FALSE);
				m_cardType2 = 1;
				m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/相册/U盘_fore.BMP", "./ADV/MJPG/k5/common/相册/U盘_back.BMP", TRUE);
			}
			else if(DetectDIR(_T("/storagecard")) && (m_cardType1 != 2))
			{
				m_MJPGList.SetUnitIsDisable(2, FALSE);
				m_cardType2 = 2;
				m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/相册/SD卡_fore.BMP", "./ADV/MJPG/k5/common/相册/SD卡_back.BMP", TRUE);
			}
		}
	}
	else if (w == 0x8004) //remove
	{
		::Sleep(1000);
		if(DetectDIR(_T("/usbdisk")))
		{
			m_cardType1 = 1;
			m_MJPGList.SetUnitBitmap(1, "./ADV/MJPG/k5/common/相册/U盘_fore.BMP", "./ADV/MJPG/k5/common/相册/U盘_back.BMP", TRUE);
		}
		else if(DetectDIR(_T("/storagecard")))
		{
			m_cardType1 = 2;
			m_MJPGList.SetUnitBitmap(1, "./ADV/MJPG/k5/common/相册/SD卡_fore.BMP", "./ADV/MJPG/k5/common/相册/SD卡_back.BMP", TRUE);
		}
		else
		{
			m_MJPGList.SetUnitBitmap(1, L"", L"", TRUE);
			m_MJPGList.SetUnitIsDisable(1, TRUE);
			m_cardType1 = 0;
		}
		m_MJPGList.SetUnitBitmap(2, L"", L"", TRUE);
		m_MJPGList.SetUnitIsDisable(2, TRUE);
		m_cardType2 = 0;
		m_uiType = 0;
		OnClearAll();
		ShowItemsInList(m_uiType);
	}
}

void C3GHomePicDlg::ShowWindow_(int nCmdShow, BOOL isBtnShow)
{
	m_sPreDir.clear();
	ShowItemsInList(m_uiType);
	ShowWindow(nCmdShow);
	if(isBtnShow)
	{
		SetTimer(0x11, 200, NULL);
	}
	else
	{
		m_MJPGList.SetUnitIsShow(10, FALSE);
		m_MJPGList.SetUnitIsShow(11, FALSE);
		m_MJPGList.SetUnitIsShow(12, FALSE);
	}
}

void C3GHomePicDlg::OnTimer(UINT nIDEvent)
{
	if(0x11 == nIDEvent)
	{
		KillTimer(0x11);
		m_MJPGList.SetUnitIsShow(10, TRUE);
		m_MJPGList.SetUnitIsShow(11, TRUE);
		m_MJPGList.SetUnitIsShow(12, TRUE);
	}
}

void C3GHomePicDlg::OnOK_()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	CString filename = m_chDir;
	for(int i=m_fileDir.size(); i<m_isFileSelect.size(); i++)
	{
		if(1 == m_isFileSelect[i])
		{
			filename += m_filePicture[i];
			break;
		}
	}
	if(1 == m_callType)
	{
		main->m_pMMSDlg->InsertPicture(filename);
		main->m_pMMSDlg->m_nSelVideo = 0;
	}
	m_callType = 0;
	OnExit_();
}

void C3GHomePicDlg::OnExit_()
{
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE); 

	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	pMainDlg->PopbackIcon();
	
	m_selCount = 0;//add by qi 20100914

	m_isSelectAll = FALSE;
	m_uiType = 0;
	m_sPreDir.clear();
	m_MJPGList.SetUnitIsDownStatus(50, FALSE);
	for (int i=100; i<=500; i+=100)
	{
		m_MJPGList.SetUnitIsDownStatus(i, FALSE);
	}
	for (i=0; i<m_isFileSelect.size(); i++)
	{
		m_isFileSelect[i] = 0;
	}
	if(1 == m_callType)
	{
		if(pMainDlg->m_pMMSDlg->m_nSelVideo == 0)
			pMainDlg->m_pMMSDlg->m_nSelVideo = 1;
		else
			pMainDlg->m_pMMSDlg->m_nSelVideo = 0;
	}
	m_callType = 0;
}

void C3GHomePicDlg::OnPlayer(int index)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->CancelBalckLightSaveTimer();
	
	main->m_pMainDlg->m_mainPhotoDlg_->m_PhotoList.clear();
	int nFileSelected = 0;
	int loop = 0;
	if (m_fileDir.size() > 0)
	{
		loop = m_fileDir.size()-1;
	}
	for(; loop < m_fileCount; loop++)
	{
		if(m_isFileSelect[loop] == 1)
		{
			char filename[128];
			int n = wcstombs( filename, (LPTSTR)(LPCTSTR)m_filePicture[loop], 128);
			filename[n] = '\0';
			
			char file_dir[128];
			n = wcstombs( file_dir, m_chDir, 128);
			file_dir[n] = '\0';
			
			char txt[128];
			sprintf(txt, "%s%s",file_dir, filename);
			
			main->m_pMainDlg->m_mainPhotoDlg_->m_PhotoList.push_back(txt);
			nFileSelected++;
		}
	}
	
	CString s;
	s.Format(L"/%d", nFileSelected);
	main->m_pMainDlg->m_mainPhotoDlg_->m_MJPGList.SetUnitText(31, s, FALSE);
	main->m_pMainDlg->m_mainPhotoDlg_->m_MJPGList.SetUnitText(30, L"1", FALSE);
	main->m_pMainDlg->m_mainPhotoDlg_->m_selectFileCount = nFileSelected;
	main->m_pMainDlg->m_mainPhotoDlg_->m_selectCurrentFile = 1;

	if(nFileSelected != 0)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->playerimage_->SetImageList(main->m_pMainDlg->m_mainPhotoDlg_->m_PhotoList, index);
		m_MJPGList.Invalidate();

		//调用播放窗口
		if(main->m_pMainDlg->m_mainPhotoDlg_->playerDlg_)
		{
			main->m_pMainDlg->m_mainPhotoDlg_->playerDlg_->SetParam("", mtImage, FALSE);
			main->m_pMainDlg->m_mainPhotoDlg_->playerDlg_->ReSetWindowsRect(CRect(110, 88, 552, 385));
			main->m_pMainDlg->m_mainPhotoDlg_->playerDlg_->ShowWindow(SW_SHOW);
			main->m_pMainDlg->m_mainPhotoDlg_->playerDlg_->player_->PlayerImage();
			g_isAutoPlay = TRUE;
		}
	}
	main->m_pMainDlg->m_mainPhotoDlg_->playerDlg_->SetResumeTimer();
	main->m_pMainDlg->m_mainPhotoDlg_->m_MJPGList.SetUnitIsDownStatus(3, TRUE);
	main->m_pMainDlg->m_mainPhotoDlg_->SetTimer(0x101, 500, NULL);
}
