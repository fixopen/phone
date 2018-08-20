// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GHomeJoyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gPlayIndex;
extern CString g_destDir;
extern CString g_srcDir;
extern CString g_showDir;
extern BOOL DetectDIR(TCHAR *sDir);
extern BOOL DetectFile(TCHAR *sDir);
extern float GetFileSize(TCHAR *sFile);
extern float GetDirSize(TCHAR *sDir, float &fSize);
/////////////////////////////////////////////////////////////////////////////
// C3GHomeJoyDlg dialog

C3GHomeJoyDlg::C3GHomeJoyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GHomeJoyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GHomeJoyDlg)
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
	m_uiType = 0;
	m_cardType2 = 0;
	m_cardType3 = 0;
	m_sListSearchFilter = "";
	m_sListFilter = "";
	m_soundType = -1;
}


void C3GHomeJoyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GHomeJoyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GHomeJoyDlg, CDialog)
	//{{AFX_MSG_MAP(C3GHomeJoyDlg)
	ON_MESSAGE(WM_MJPGTOGGLE, OnClickMJPG)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_MESSAGE(WM_USB_MSG, OnDeviceChange)
	ON_MESSAGE(WM_RENAME, OnRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GHomeJoyDlg message handlers

BOOL C3GHomeJoyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	m_pPlaySoundDlg = new CPlaySoundDlg(this);
	m_pPlaySoundDlg->Create(CPlaySoundDlg::IDD);
	m_pPlaySoundDlg->SetModel(0);
	
	m_pSoundSearchDlg = new CSoundSearchDlg(this);
	m_pSoundSearchDlg->Create(CSoundSearchDlg::IDD);
	
	m_pPasswordDlg = new CPasswordDlg(this);
	m_pPasswordDlg->Create(CPasswordDlg::IDD);
	
	m_pRecordSoundDlg = new CRecordSoundDlg(this);
	m_pRecordSoundDlg->Create(CRecordSoundDlg::IDD);
	
	m_pRenameDlg = new CRenameDlg(this);
	m_pRenameDlg->Create(CRenameDlg::IDD);
	
	m_bTelephoneInUse = false;

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\����\\����.xml");
	
	MoveWindow(0, 57, 800, 423);
	SetUnitFont();

	m_MJPGList.SetUnitIsDisable(2, TRUE);
	m_MJPGList.SetUnitIsDisable(3, TRUE);
	m_MJPGList.SetUnitIsShow(1, TRUE);
	m_MJPGList.SetUnitIsShow(10, TRUE);
	m_MJPGList.SetUnitIsShow(11, TRUE);
	m_MJPGList.SetUnitIsShow(1000, TRUE);
	m_MJPGList.SetUnitIsShow(1001, TRUE);
	
	ShowItemsInList(m_uiType);
	PostMessage(WM_USB_MSG, 0x8000, 0);
	PostMessage(WM_USB_MSG, 0x8000, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GHomeJoyDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	int index = 0;
	switch(w)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
		m_isSelectAll = FALSE;
		m_uiType = w;
		ShowItemsInList(w);
		break;
	case 5:			//ѡ��ǰҳ��
		break;
	case 7:			//�Ϸ�
		PageUp();
		break;
	case 8:			//�·�
		PageDown();
		break;
	case 10:      //����
		{
			HWND hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1");
			if(IsWindow(hWnd))
			{
				pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnMp3Btn();
				pMainDlg->m_pMainDlg->m_mainMp3Dlg_->playerDlg_->ShowWindow(SW_SHOW);
			}
			else
			{
				FindSelectPage();
				if (m_firstSelPage > 0)
				{
					if((0 == m_uiType) || (2 == m_uiType) || (3 == m_uiType))
					{
						pMainDlg->m_pMainDlg->m_mainMp3Dlg_->OnOpenFile();
						pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnMp3Btn();
						OnPlayer();
					}
					else if(1 == m_uiType)
					{
						PlaySound();
					}
				}
			}
			break;
		}
	case 12:      //ת���ļ�
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
			}
			OnShiftFile();
		}
		break;
	case 11:	  //ɾ��
		FindSelectPage();
		if (m_firstSelPage > 0)
		{
			OnClickDelete();
		}
		break;
	case 13:		//��ʾ��
		OnButtonSetTip();
		break;
	case 14:		//��ʼ����¼��
		OnButtonRecord();
		break;
	case 15:		//������
		for (index = 0; index < m_fileCount; index++)
		{
			if (1 == m_isFileSelect[index])
			{
				std::string filename = m_vCurrentResult[index]->name();	
				m_pRenameDlg->SetWndParam(this, index, filename.c_str());
				m_pRenameDlg->ShowWindow_(SW_SHOW);
			}
		}
		break;
	case 20:
		m_soundType = 0xFF;
		m_sListFilter = "";
		ShowRecord(m_sListFilter);
		break;
	case 21:
		m_soundType = 0;
		m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstLeaveWord);
		ShowRecord(m_sListFilter);
		break;
	case 22:
		m_soundType = 1;
		m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstTeleRecord);
		ShowRecord(m_sListFilter);
		break;
	case 23:
		m_soundType = 2;
		m_sListFilter = "type = " + Util::StringOp::FromInt(Data::sstLocalRecord);
		ShowRecord(m_sListFilter);
		break;
	case 24:
		m_soundType = 3;
		OnButtonSearch();
		if (m_sListSearchFilter == "")
		{
			return;
		}
 		m_sListFilter = m_sListSearchFilter;
		ShowRecord(m_sListFilter);
		break;
	case 50:
		OnSelectAll();
		break;
	case 100:      //ѡ��
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
	case 101:          //����ͼ��   
	case 201:
	case 301:
	case 401:
	case 501:
		break;
	case 102:
	case 202:
	case 302:
	case 402:
	case 502:
		break;
	case 103:		  //��ϵ����Ϣ	 
	case 203:
	case 303:
	case 403:
	case 503:
		break;
	case 1000:     //����	
		OnExit_();
		break;
	case 1001:    //ȷ��
		break;
	default:
		break;
	}
}

void C3GHomeJoyDlg::SetUnitFont()
{
	m_MJPGList.SetUnitText(4, _T("��ǰҳ"), TRUE);
	m_MJPGList.SetUnitText(52, _T("�ļ�/�ļ���"), TRUE);

	for (int i=4; i<=6; i++)
	{
		m_MJPGList.SetUnitFont(i, font_14);
		m_MJPGList.SetUnitColor(i,font_white,TRUE);
	}
	for (i=51; i<=56; i++)
	{
		m_MJPGList.SetUnitFont(i, font_18);
		m_MJPGList.SetUnitColor(i,font_white,TRUE);
	}
	for (i=1; i<=5; i++)
	{
		int index = i*100;
		for (int j=2; j<=5; j++)
		{
			m_MJPGList.SetUnitFont(index+j, font_18);
			m_MJPGList.SetUnitColor(index+j,font_white,TRUE);
		}
		m_MJPGList.SetUnitFont(index+20, font_18);
		m_MJPGList.SetUnitColor(index+20,font_white,TRUE);
	}
}

void C3GHomeJoyDlg::AddFileMp3()
{
	int dirCount = m_fileDir.size();
	std::vector<CString>::iterator dir;
	std::vector<int>::iterator idir;
	for (int i=0; i<dirCount; i++)
	{
		dir = m_fileMp3.begin();
		idir = m_isFileSelect.begin();
		m_fileMp3.insert(dir, m_fileDir[i]);
		m_isFileSelect.insert(idir, 0);
	}
}

void C3GHomeJoyDlg::SetInitShow()
{
	m_fileCount = m_fileDir.size() + m_fileMp3.size();
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
	m_MJPGList.SetUnitText(6, str, TRUE);
	str.Empty();
	str.Format(L"%d",m_currentPage);
	m_MJPGList.SetUnitText(5, str, TRUE);

	if (m_currentPage <= 1)   //����һ����ʾʱ�Ϸ���ť������
	{
		m_MJPGList.SetUnitIsDisable(7, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDisable(7, FALSE); 
	}
	if (m_fileCount <= m_pageSize)		//������С��6ʱ�����·���ť������
	{
			m_MJPGList.SetUnitIsDisable(8, TRUE);
	}
	else
	{
			m_MJPGList.SetUnitIsDisable(8, FALSE); 
	}
}

void C3GHomeJoyDlg::ShowItemsInList(int type)
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
		SetPlayList(_T("/FlashDrv/MY_MUSIC/"), type);
	}
	else if (type == 1)
	{
		ShowRecord("");
		memset(m_chDir, 0, 128*2);
		memcpy(m_chDir,L"/FlashDrv/my_record/", wcslen(_T("/FlashDrv/my_record/"))*2) ;
	}
	else if (type == 2)
	{
		if (1 == m_cardType2)
		{
			SetPlayList(_T("/usbdisk/"), type);
		}
		else if (2 == m_cardType2)
		{
			SetPlayList(_T("/storagecard/"), type);
		}
	}
	else if (type == 3)
	{
		if (1 == m_cardType3)
		{
			SetPlayList(_T("/usbdisk/"), type);
		}
		else if (2 == m_cardType3)
		{
			SetPlayList(_T("/storagecard/"), type);
		}
	}
	
	for (int i = 0; i < 4; ++i)
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
	AddFileMp3();
	SetUnitIsAvailable(type);   //����ҳǩת��ʱ��״̬�仯
	ShowArrayInList();
}

void C3GHomeJoyDlg::ShowArrayInList()
{
	OnClearAll();
	int index = 100;
	if (m_fileCount > 0)
	{
		CString temp;
		float fileSize;
		for (int i=(m_currentPage - 1)*m_pageSize; i<m_currentPage*m_pageSize; i++)
		{
			fileSize = 0;
			if (m_fileMp3.begin() + i < m_fileMp3.end())
			{
				CString path = m_chDir + m_fileMp3[i];

				m_UnitStatus[index/100-1] = TRUE;
				SetShowStatus(index, TRUE);
				m_MJPGList.SetUnitText(index+2, m_fileMp3[i], FALSE);
				if(i<m_fileDir.size())
				{

					m_MJPGList.SetUnitBitmap(index, ".\\adv\\mjpg\\k5\\common\\png\\�ļ���.png", L".\\adv\\mjpg\\k5\\common\\png\\�ļ���.png", FALSE);
					fileSize = GetDirSize((LPTSTR)(LPCTSTR)path, fileSize);
					fileSize /= (1024*1024);
					if(m_uiType != 1)
					{
						if(fileSize < 1.0 && fileSize > 0.0000001)
						{
							fileSize *= 1024;
							temp.Format(_T("%0.1fK"), fileSize);
						}
						else if(fileSize >= -0.0000001 && fileSize <= 0.0000001)
						{
							temp = L"0�ֽ�";
						}
						else
						{
							temp.Format(_T("%0.1fM"), fileSize);
						}
						m_MJPGList.SetUnitText(index+20, temp, FALSE);
					}
				}
				else
				{
					fileSize = GetFileSize((LPTSTR)(LPCTSTR)path);
					fileSize /= (1024*1024);
					m_MJPGList.SetUnitBitmap(index, L"", L"", FALSE);
					if(1 == m_uiType)
					{
						m_MJPGList.SetUnitIsShow(index+1, TRUE, FALSE);
						if (!m_vCurrentResult[i]->played())
						{
							m_MJPGList.SetUnitIsDownStatus(index+1, TRUE);
						}
						else
						{
							m_MJPGList.SetUnitIsDownStatus(index+1, FALSE);
						}
						if (m_vCurrentResult[i]->name() != "")
						{
							temp = (m_vCurrentResult[i]->name() + m_vCurrentResult[i]->telephoneNumber().number()).c_str();
						}
						else
						{
							temp = m_vCurrentResult[i]->telephoneNumber().number().c_str();
						}
						m_MJPGList.SetUnitText(index+3, temp, FALSE);
						
						temp.Format(_T("%04d-%02d-%02d %02d:%02d"),
							m_vCurrentResult[i]->startTime().GetYear(),
							m_vCurrentResult[i]->startTime().GetMonth(),
							m_vCurrentResult[i]->startTime().GetDay(),
							m_vCurrentResult[i]->startTime().GetHour(),
							m_vCurrentResult[i]->startTime().GetMinute());
						m_MJPGList.SetUnitText(index+5, temp, FALSE);
						
						if (m_vCurrentResult[i]->duration() >= 3600)
						{
							temp.Format(_T("%d:%02d:%02d"), m_vCurrentResult[i]->duration() / 3600 ,m_vCurrentResult[i]->duration() % 3600 / 60, m_vCurrentResult[i]->duration() % 60);
						}
						else
						{
							temp.Format(_T("%02d:%02d"), m_vCurrentResult[i]->duration() / 60, m_vCurrentResult[i]->duration() % 60);
						}
						m_MJPGList.SetUnitText(index+4, temp, FALSE);
					}
					else
					{
						if(fileSize < 1.0 && fileSize > 0.0000001)
						{
							fileSize *= 1024;
							temp.Format(_T("%0.1fK"), fileSize);
						}
						else if(fileSize >= -0.0000001 && fileSize <= 0.0000001)
						{
							temp = L"0�ֽ�";
						}
						else
						{
							temp.Format(_T("%0.1fM"), fileSize);
						}
						m_MJPGList.SetUnitText(index+20, temp, FALSE);
					}
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
	}
	m_MJPGList.Invalidate();
}

void C3GHomeJoyDlg::OnClearAll()
{
	for (int i=1; i<=m_pageSize; i++)
	{
		int index = i*100;
		m_MJPGList.SetUnitIsShow(index+1, FALSE);
		for (int j=2; j<=5; j++)
		{
			m_MJPGList.SetUnitText(index+j, L"", FALSE);
		}
		m_MJPGList.SetUnitText(i*100+20, L"", FALSE);
	}
	m_MJPGList.Invalidate();
}

void C3GHomeJoyDlg::SetShowStatus(int unitNO, BOOL status)
{
	if (m_UnitStatus[unitNO/100-1])
	{
		if (status)  //����Ϊǰ��
		{
			m_MJPGList.SetUnitIsDisable(unitNO, FALSE);
			m_MJPGList.SetUnitIsDownStatus(unitNO, FALSE);
		}
		else    //����Ϊ��
		{
			m_MJPGList.SetUnitIsDisable(unitNO, FALSE);
			m_MJPGList.SetUnitIsDownStatus(unitNO, TRUE);
		}
	}
	else
	{
		m_MJPGList.SetUnitIsDownStatus(unitNO, FALSE);
		m_MJPGList.SetUnitIsDisable(unitNO, TRUE);
		for (int i=unitNO+1; i<=unitNO+5; i++)
		{
			m_MJPGList.SetUnitText(i, L"", FALSE);
			m_MJPGList.SetUnitIsDisable(i, TRUE);
		}
		m_MJPGList.SetUnitText(unitNO+20, L"", FALSE);
		m_MJPGList.SetUnitIsDisable(unitNO+20, TRUE);
	}
	
	m_MJPGList.Invalidate();
}

int C3GHomeJoyDlg::SetPlayList(TCHAR *dir, int type)
{
	g_srcDir = L"";
	g_srcDir = dir;

	BOOL DetectDIR(TCHAR *sDir);
	if(!m_fileMp3.empty())
	{
		m_fileMp3.clear();
		m_fileDir.clear();
		m_isFileSelect.clear();
	}
	memset(m_chDir, 0, 128*2);
	int ncount = 0;
	
	if(1 == type)
	{
		CString s = Data::LanguageResource::Get(Data::RI_COMN_TOBOTTOM).c_str();
	}
	
	memcpy(m_chDir, g_srcDir, wcslen(g_srcDir)*2);
	
	CString sDir = m_chDir;
	WIN32_FIND_DATA FindFileData;			//�����ļ�ʱҪʹ�õ����ݽṹ
	HANDLE hFind = INVALID_HANDLE_VALUE;	//������Ҿ��
	
	sDir += "*.*";
	hFind = FindFirstFile(sDir, &FindFileData);//ʹ��FindFirstFile��������ʼ�ļ�����
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return 0;
	} 
	else 
	{
		char filename[128];
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".Mp3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
		{
			m_fileMp3.push_back(FindFileData.cFileName);
			m_isFileSelect.push_back(0);
			ncount++;
		}
		else if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			m_fileDir.push_back(FindFileData.cFileName);
			ncount++;
		}
		
		//������ѭ��ʹ��FindNextFile�����������ļ�
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".Mp3")||strstr(filename, ".wav")||strstr(filename, ".WAV"))
			{
				m_fileMp3.push_back(FindFileData.cFileName);
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
			FindClose(hFind);//�رղ��Ҿ��
		} 
		else 
		{
			//Dprintf ("FindNextFile error. Error is %u\n", dwError);
			return 0;
		}
	}
	return ncount;	
}

void C3GHomeJoyDlg::ShowRecord(std::string filter)
{
	m_fileMp3.clear();
	m_isFileSelect.clear();
	m_vCurrentResult.clear();
	m_fileCount = 0;
	if (Data::SoundSegment::GetDataCount("") > 0)
	{
		m_vCurrentResult = Data::SoundSegment::GetFromDatabase(filter, Data::dDown, Data::SoundSegment::GetCurrentId() + 1);
		m_fileCount = Data::SoundSegment::GetDataCount(filter);
		for (int i=0; i<m_vCurrentResult.size(); i++)
		{
			std::string s= m_vCurrentResult[i]->filename();
			s = s.substr(s.length() - 12, 6);
			m_fileMp3.push_back(s.c_str());
			m_isFileSelect.push_back(0);
		}
	}
	ShowArrayInList();
}

void C3GHomeJoyDlg::OnSelectAll()
{
	//wangzhenxing1019
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
		}
		for (int i=100; i<=500; i+=100)
		{
			SetShowStatus(i, !m_isSelectAll);
		}
	}
}

void C3GHomeJoyDlg::OnButtonCheck(int uintNO)
{
	int index = (m_currentPage-1)*m_pageSize + m_offSet;
	if (m_fileCount > 0)
	{
		if(index >= m_fileDir.size())
		{
			if (m_isFileSelect[index] == 0)
			{
				m_isFileSelect[index] = 1;
				SetShowStatus(uintNO, FALSE);
			}
			else
			{
				m_isFileSelect[index] = 0;
				SetShowStatus(uintNO, TRUE);
			}
		}
		else
		{
			CString dir = m_chDir+m_MJPGList.GetUnitText(uintNO+2)+"/";
			SetPlayList(dir.GetBuffer(dir.GetLength()), m_uiType);
			SetInitShow();
			AddFileMp3();
			ShowArrayInList();
		}
	}
}

void C3GHomeJoyDlg::PageUp()
{
	m_currentPage--;
	
	if (m_currentPage <= 1)
	{
		m_currentPage = 1;
		m_MJPGList.SetUnitIsDisable(7, TRUE);
	}
	if (m_currentPage*m_pageSize < m_fileCount)
	{
		m_MJPGList.SetUnitIsDisable(8, FALSE);
	}
	
	CString str;
	str.Format(L"%d", m_currentPage);
	m_MJPGList.SetUnitText(5, str, TRUE);
	str.Empty();
	
	OnClearAll();
	ShowArrayInList();
}

void C3GHomeJoyDlg::PageDown()
{
	m_currentPage++;   
	
	if (m_currentPage > 1)
	{
		m_MJPGList.SetUnitIsDisable(7, FALSE);
	}
	if (m_currentPage >= m_pageCount) 
	{
		m_MJPGList.SetUnitIsDisable(8, TRUE);
	}
	
	CString str;
	str.Format(L"%d", m_currentPage);
	m_MJPGList.SetUnitText(5, str, TRUE);
	str.Empty();
	
	ShowArrayInList(); 
}

void C3GHomeJoyDlg::OnClickDelete()
{
	if (0 == m_firstSelPage)
		return;
	int count = 0;
	for(int i=(m_firstSelPage-1)*m_pageSize; i<m_lastSelPage*m_pageSize; i++)
	{
		if(1 == m_isFileSelect[i] && i >= m_fileDir.size())
			count++;
	}
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetTitle(L"ȷ��Ҫɾ����ѡ�ļ���", 0);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(this->GetSafeHwnd());
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessMax(count);
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isAdmin() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPasswordModel(true);
	}
	std::string pw = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->adminPassword();
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetPassword(Util::StringOp::ToCString(pw));
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
}
extern void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE);
void C3GHomeJoyDlg::DeleteSelectedItem()
{
	size_t pageStartIndex = m_fileDir.size();
	std::vector<int>::iterator checkItem = m_isFileSelect.begin() + pageStartIndex;
	std::vector<CString>::iterator checkFile = m_fileMp3.begin() + pageStartIndex;
	std::vector< boost::shared_ptr< Data::SoundSegment> >::iterator m_vResult = m_vCurrentResult.begin() + pageStartIndex;
	int delCount = 0;
	for (int i=pageStartIndex; (checkItem != m_isFileSelect.end())
		&& (checkItem < (m_isFileSelect.begin() + m_lastSelPage * m_pageSize));)
	{
		if (*checkItem == 1)
		{
			CString s;
			if (1 == m_uiType)
			{
				s = m_chDir + m_fileMp3[i] + L"ST.SPX";
			}
			else
			{
				s = m_chDir + m_fileMp3[i];
			}

			if (m_vCurrentResult.size() > 0)
			{
				(*m_vResult)->Remove();
				m_vCurrentResult.erase(m_vResult);
			}

			DeleteFile(s);
			int ret = GetLastError();
			if(0 == GetLastError() || 2 == GetLastError())
			{
				delCount++;
				checkFile = m_fileMp3.erase(checkFile);
				checkItem = m_isFileSelect.erase(checkItem);
			}
			else
			{
				++checkItem;
				++checkFile;
				++m_vResult;
				++i;
			}
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetProcessPos(delCount);
		}
		else
		{
			++checkItem;
			++checkFile;
			++m_vResult;
			++i;
		}
	}

	//������ǰ�ܼ�¼��
	m_fileCount = m_fileMp3.size();
	if (0 == m_fileCount)
	{
		m_MJPGList.SetUnitText(6, "0", TRUE);
		m_MJPGList.SetUnitText(5, "0", TRUE);
		m_MJPGList.SetUnitIsDisable(8, TRUE);
		m_MJPGList.SetUnitIsDisable(7, TRUE);
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
		m_MJPGList.SetUnitText(6, str, TRUE);
		if (m_fileCount <= m_pageSize)
		{
			m_MJPGList.SetUnitIsDisable(8, TRUE);
		}
		
		if ((m_currentPage - 1) * m_pageSize >= m_fileMp3.size())
			m_currentPage = m_pageCount;
		str.Empty();
		str.Format(L"%d",m_currentPage);
		m_MJPGList.SetUnitText(5, str, TRUE);
		
		//��鵱ǰҳ�����
		if (m_currentPage == m_pageCount)
		{
			m_MJPGList.SetUnitIsDisable(8, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(8, FALSE);
		}
		if (m_currentPage == 1)
		{
			m_MJPGList.SetUnitIsDisable(7, TRUE);
		}
		else
		{
			m_MJPGList.SetUnitIsDisable(7, FALSE);
		}
	}
	ShowArrayInList();
}

void C3GHomeJoyDlg::FindSelectPage()
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

void C3GHomeJoyDlg::OnShiftFile()
{
	g_destDir = L"";
	g_showDir = L"";

	if (0 == m_firstSelPage)
		return;
	if(!DetectDIR(_T("\\StorageCard")) && !DetectDIR(_T("\\UsbDisk")))
	{
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetTitle(_T("������ƶ��豸"));
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetHWnd(this->GetSafeHwnd());
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
		return;
	}

	if (0 == m_uiType)
	{
		if (DetectDIR(L"/usbdisk"))
		{
			g_destDir = L"/UsbDisk/";
			g_showDir = L"U��";
		}
		else if (DetectDIR(L"/storagecard"))
		{
			g_destDir = L"/StorageCard/";
			g_showDir = L"SD��";
		}
	}
	else if (2 == m_uiType)
	{
		if (1 == m_cardType2)
		{
			g_showDir = L"U��";
		}
		else if (2 == m_cardType2)
		{
			g_showDir = L"SD��";
		}
		g_destDir = _T("/FlashDrv/MY_MUSIC/");
	}
	else if (3 == m_uiType)
	{
		if (1 == m_cardType3)
		{
			g_showDir = L"U��";
		}
		else if (2 == m_cardType3)
		{
			g_showDir = L"SD��";
		}
		g_destDir = _T("/FlashDrv/MY_MUSIC/");
	}

	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
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
			int n = wcstombs( filename, (LPTSTR)(LPCTSTR)m_fileMp3[loop], 128);
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

void C3GHomeJoyDlg::SetUnitIsAvailable(int type)
{
	if ((type == 0) || (type == 2) || (type == 3))
	{
		m_MJPGList.SetUnitText(51, L"", FALSE);
		m_MJPGList.SetUnitText(53, L"", FALSE);
		m_MJPGList.SetUnitText(55, L"", FALSE);
		m_MJPGList.SetUnitText(56, L"", FALSE);
		m_MJPGList.SetUnitIsDisable(51, TRUE);
		m_MJPGList.SetUnitIsDisable(53, TRUE);
		m_MJPGList.SetUnitIsDisable(55, TRUE);
		m_MJPGList.SetUnitIsDisable(56, TRUE);
		
		m_MJPGList.SetUnitIsDisable(54, FALSE);
		m_MJPGList.SetUnitText(54, _T("�ļ���С"), FALSE);

		m_MJPGList.SetUnitRect(102, CRect(140,174,340,199));
		m_MJPGList.SetUnitRect(202, CRect(140,217,340,242));
		m_MJPGList.SetUnitRect(302, CRect(140,260,340,285));
		m_MJPGList.SetUnitRect(402, CRect(140,303,340,328));
		m_MJPGList.SetUnitRect(502, CRect(140,346,340,371));
		
		for (int i=1; i<=m_pageSize; i++)
		{
			int index = i*100;
			m_MJPGList.SetUnitIsShow(index+1, FALSE, FALSE);
			for (int j=3; j<=5; j++)
			{
				m_MJPGList.SetUnitIsDisable(index+j, TRUE);
			}
			m_MJPGList.SetUnitIsDisable(index+20, FALSE);
		}
		m_MJPGList.SetUnitIsShow(12, TRUE, FALSE);
		for (i=13; i<=15;i++)
		{
			m_MJPGList.SetUnitIsShow(i, FALSE, FALSE);
		}
		for (i=20; i<25; i++)
		{
			m_MJPGList.SetUnitIsShow(i, FALSE, FALSE);
		}
	}
	else if (type == 1)
	{
		m_MJPGList.SetUnitText(54, L"", FALSE);
		m_MJPGList.SetUnitIsDisable(54, TRUE);
		m_MJPGList.SetUnitIsDisable(51, FALSE);
		m_MJPGList.SetUnitIsDisable(53, FALSE);
		m_MJPGList.SetUnitIsDisable(55, FALSE);
		m_MJPGList.SetUnitIsDisable(56, FALSE);
		
		m_MJPGList.SetUnitText(51, _T("״̬"), FALSE);
		m_MJPGList.SetUnitText(53, _T("����/����"), FALSE);
		m_MJPGList.SetUnitText(55, _T("ʱ��"), FALSE);
		m_MJPGList.SetUnitText(56, _T("ʱ��"), FALSE);
		
		m_MJPGList.SetUnitRect(102, CRect(140,174,235,199));
		m_MJPGList.SetUnitRect(202, CRect(140,217,235,242));
		m_MJPGList.SetUnitRect(302, CRect(140,260,235,285));
		m_MJPGList.SetUnitRect(402, CRect(140,303,235,328));
		m_MJPGList.SetUnitRect(502, CRect(140,346,235,371));
	
		m_MJPGList.SetUnitIsShow(12, FALSE, FALSE);
		for (int i=13; i<=15;i++)
		{
			m_MJPGList.SetUnitIsShow(i, TRUE, FALSE);
		}

		for (i=20; i<25; i++)
		{
			m_MJPGList.SetUnitIsShow(i, TRUE, FALSE);
		}

		for (i=1; i<=m_pageSize; i++)
		{
			int index = i*100;
			m_MJPGList.SetUnitIsShow(index+1, TRUE, FALSE);
			for (int j=3; j<=5; j++)
			{
				m_MJPGList.SetUnitIsDisable(index+j, FALSE);
			}
			m_MJPGList.SetUnitIsDisable(index+20, TRUE);
		}
	}
	m_MJPGList.Invalidate();
}

LRESULT C3GHomeJoyDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
		SetPlayList(m_chDir, m_uiType);
		SetInitShow();
		AddFileMp3();
		ShowArrayInList();
		break;
	};
	return CDialog::WindowProc(message, wParam, lParam);
}

void C3GHomeJoyDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
  	if (w == 0x8000) //insert
	{
		::Sleep(500);
		if (m_MJPGList.GetUnitIsDisable(2))
		{
			if(DetectDIR(_T("/usbdisk")))
			{
				m_MJPGList.SetUnitIsDisable(2, FALSE);
				m_cardType2 = 1;
				m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/����/U��_fore.BMP", "./ADV/MJPG/k5/common/����/U��_back.BMP", TRUE);
			}
			else if(DetectDIR(_T("/storagecard")))
			{
				m_MJPGList.SetUnitIsDisable(2, FALSE);
				m_cardType2 = 2;
				m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/����/SD��_fore.BMP", "./ADV/MJPG/k5/common/����/SD��_back.BMP", TRUE);
			}
		}
		else if (m_MJPGList.GetUnitIsDisable(3))
		{
			if(DetectDIR(_T("/usbdisk")) && (m_cardType2 != 1))
			{
				m_MJPGList.SetUnitIsDisable(3, FALSE);
				m_cardType3 = 1;
				m_MJPGList.SetUnitBitmap(3, "./ADV/MJPG/k5/common/����/U��_fore.BMP", "./ADV/MJPG/k5/common/����/U��_back.BMP", TRUE);
			}
			else if(DetectDIR(_T("/storagecard")) && (m_cardType2 != 2))
			{
				m_MJPGList.SetUnitIsDisable(3, FALSE);
				m_cardType3 = 2;
				m_MJPGList.SetUnitBitmap(3, "./ADV/MJPG/k5/common/����/SD��_fore.BMP", "./ADV/MJPG/k5/common/����/SD��_back.BMP", TRUE);
			}
		}
	}
	else if (w == 0x8004) //remove
	{
		::Sleep(1000);
		if(DetectDIR(_T("/usbdisk")))
		{
			m_cardType2 = 1;
			m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/����/U��_fore.BMP", "./ADV/MJPG/k5/common/����/U��_back.BMP", TRUE);
		}
		else if(DetectDIR(_T("/storagecard")))
		{
			m_cardType2 = 2;
			m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/����/SD��_fore.BMP", "./ADV/MJPG/k5/common/����/SD��_back.BMP", TRUE);
		}
		else
		{
			m_MJPGList.SetUnitBitmap(2, L"", L"", TRUE);
			m_MJPGList.SetUnitIsDisable(2, TRUE);
			m_cardType2 = 0;
		}
		m_MJPGList.SetUnitBitmap(3, L"", L"", TRUE);
		m_MJPGList.SetUnitIsDisable(3, TRUE);
		m_cardType3 = 0;
		OnClearAll();
		m_uiType = 0;
		ShowItemsInList(m_uiType);
	}
}

void C3GHomeJoyDlg::ShowWindow_(int nCmdShow)
{
	ShowItemsInList(m_uiType);
	ShowWindow(nCmdShow);
}

void C3GHomeJoyDlg::OnExit_()
{
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	pMainDlg->PopbackIcon();

	m_isSelectAll = FALSE;
	m_uiType = 0;
	m_MJPGList.SetUnitIsDownStatus(50, FALSE);
	for (int i=100; i<=500; i+=100)
	{
		m_MJPGList.SetUnitIsDownStatus(i, FALSE);
	}
	for (i=0; i<m_isFileSelect.size(); i++)
	{
		m_isFileSelect[i] = 0;
	}
}

void C3GHomeJoyDlg::OnPlayer(int index)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->CancelBalckLightSaveTimer();
	
	main->m_pMainDlg->m_mainMp3Dlg_->m_MP3List.clear();
	main->m_pMainDlg->m_mainMp3Dlg_->m_ShowList.clear();
	int nFileSelected = 0;
	int loop = 0;
	if (m_fileDir.size() > 0)
	{
		loop = m_fileDir.size();
	}
	for(; loop < m_fileCount; loop++)
	{
		if(m_isFileSelect[loop] == 1)
		{
			char filename[128];
			int n = wcstombs( filename, (LPTSTR)(LPCTSTR)m_fileMp3[loop], 128);
			filename[n] = '\0';
			
			char file_dir[128];
			n = wcstombs( file_dir, m_chDir, 128);
			file_dir[n] = '\0';
			
			char txt[128];
			sprintf(txt, "%s%s",file_dir, filename);
			
			main->m_pMainDlg->m_mainMp3Dlg_->m_MP3List.push_back(txt);
			main->m_pMainDlg->m_mainMp3Dlg_->m_ShowList.push_back(m_fileMp3[loop]);
			nFileSelected++;
		}
	}
	
	main->m_pMainDlg->m_mainMp3Dlg_->CalculatePage(nFileSelected);
	main->m_pMainDlg->m_mainMp3Dlg_->ShowArrayInList(main->m_pMainDlg->m_mainMp3Dlg_->m_ShowList);
	
	if(nFileSelected > 0)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->playeraudio_->SetImageList(main->m_pMainDlg->m_mainMp3Dlg_->m_MP3List, index);
		m_MJPGList.Invalidate();
	}
	gPlayIndex = 1;
}

void C3GHomeJoyDlg::PlaySound()
{
	m_pPlaySoundDlg->SetSound(m_soundType, m_vCurrentResult[0]->id(), 0, m_sListFilter);
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTempSetting->isPlayProtect() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
	{
		m_pPasswordDlg->SetType(CHECK_PLAYPASSWORD);
		std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTempSetting->playRecordPassword();
		m_pPasswordDlg->SetOldPassWord((char *)strTemp.c_str());
		m_pPasswordDlg->SetHWnd(this->m_hWnd);
		m_pPasswordDlg->ShowWindow_(SW_SHOW);	
	}
	else
	{
		m_pPlaySoundDlg->StartPlay(false);
		m_pPlaySoundDlg->ShowWindow_(TRUE);
	}
}

void C3GHomeJoyDlg::OnButtonRecord()
{
	if ((((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff))
		return;

 	m_pRecordSoundDlg->ShowWindow_(TRUE);
}

void C3GHomeJoyDlg::OnButtonSetTip() 
{
	for (int i=0; i<m_fileCount; i++)
	{
		if (1 == m_isFileSelect[i])
		{
			if ((m_vCurrentResult[i]->type() == Data::sstLocalRecord))
			{
				if ((m_vCurrentResult[i]->duration() <= 10))
				{
					m_vCurrentResult[i]->isTip(!m_vCurrentResult[i]->isTip());
					m_vCurrentResult[i]->Update();
					//ShowArrayInList();
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pTipDlg->ShowWindow(SW_SHOW);
				}
				else
				{
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetType(2);     //��ʾ¼�����ݳ���10��
					((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				}
			}
			else
			{
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetType(3);     //��ʾ��ʾ��Ӧ��Ϊ����¼��
				((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			}
		}
	}
}

void C3GHomeJoyDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	m_pSoundSearchDlg->ShowWindow(TRUE);
}

void C3GHomeJoyDlg::Search(std::string filter)
{
	m_sListSearchFilter = filter;
}

void C3GHomeJoyDlg::OnRename(WPARAM w, LPARAM l)
{
	CString s;
	m_pRenameDlg->m_edtNewName.GetWindowText(s);
	std::string sName = Util::StringOp::FromCString(s);
	m_vCurrentResult[w]->name(sName);
	m_vCurrentResult[w]->Update();
	
	if(m_vCurrentResult[w]->telephoneNumber().number() != "")
	{
		std::string s1;
		if(sName != "")
			s1 = "(" + m_vCurrentResult[w]->telephoneNumber().number() + ")";
		else
			s1 = m_vCurrentResult[w]->telephoneNumber().number();
		s += s1.c_str();
	}
	m_MJPGList.SetUnitText(((w%m_pageSize)+1)*100+3, s, TRUE);
}