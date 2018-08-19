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

extern BOOL g_isShift;
extern int gPlayIndex;
extern CString g_destDir;
extern CString g_srcDir;
extern CString g_showDir;
extern BOOL DetectDIR(TCHAR *sDir);
extern BOOL DetectFile(TCHAR *sDir);
extern double GetFileSize(TCHAR *sFile);
extern double GetDirSize(TCHAR *sDir, float &fSize);
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
	m_playType = -1;
	m_cardType2 = 0;
	m_cardType3 = 0;
	m_sListSearchFilter = "";
	m_sListFilter = "";
	m_soundType = -1;
	m_callType = 0;
	m_playFileSize = 0;
	m_selCount = 0;
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

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this,10086);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\音乐.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));

	MoveWindow(0, 57, 800, 423);
	SetUnitFont();

	m_MJPGList.SetUnitIsDisable(2, TRUE);
	m_MJPGList.SetUnitIsDisable(3, TRUE);
	m_MJPGList.SetUnitIsDisable(4, TRUE);//add by qi 20100613
	m_MJPGList.SetUnitFont(9, font_14);//add by qi 20100613

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
	HWND hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1");
	int index = 0;
	switch(w)
	{
	case 0:
	case 2:
	case 3:
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
		m_isSelectAll = FALSE;
		m_firstSelPage = 0;
		m_lastSelPage = 0;
		m_uiType = w;
		m_selCount = 0;
		ShowItemsInList(w);
		break;
	case 5:			//选择当前页数
		break;
	case 7:			//上翻
		PageUp();
		break;
	case 8:			//下翻
		PageDown();
		break;
	case 10:      //播放
		{
			if(IsWindow(hWnd) || pMainDlg->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic)
			{
				pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnMp3Btn();
				pMainDlg->m_pMainDlg->m_mainMp3Dlg_->playerDlg_->ShowWindow(SW_SHOW);

				pMainDlg->AddIcon(Allicon[4],pMainDlg->m_pMainDlg->m_mainMp3Dlg_,false);		//lxz 20100528
			}
			else
			{
				FindSelectPage();
				if (m_firstSelPage > 0)
				{
					if((0 == m_uiType) || (2 == m_uiType) || (3 == m_uiType))
					{
						m_playType = m_uiType;   //将播放的音乐所在的页签记下

						pMainDlg->m_pMainDlg->m_mainMp3Dlg_->OnOpenFile();
						pMainDlg->m_pMainDlg->m_mainmenuDlg_->OnMp3Btn();
						OnPlayer();

						pMainDlg->AddIcon(Allicon[4],pMainDlg->m_pMainDlg->m_mainMp3Dlg_,false);		//lxz 20100528
					}
					else if(1 == m_uiType)
					{
						PlaySound();
					}
				}
				else
				{
					pMainDlg->m_pTipDlg->SetTitle("请选择要播放的文件!");
					pMainDlg->m_pTipDlg->ShowWindow_(TRUE);
				}
			}
			break;
		}
	case 12:      //转移文件
		if(!g_isShift)
		{
			FindSelectPage();
			if (m_firstSelPage > 0)
			{
				if((hWnd == NULL && !pMainDlg->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic) || m_uiType != m_playType)
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
					pMainDlg->m_pWarningNoFlashDlg->SetTitle("正在播放音乐,无法转移!");
					pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
				}
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
	case 11:	  //删除
		FindSelectPage();
		if (m_firstSelPage > 0)
		{
			if((hWnd == NULL && !pMainDlg->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic) || m_uiType != m_playType)
			{
				OnClickDelete();
			}
			else
			{
				pMainDlg->m_pWarningNoFlashDlg->SetTitle("正在播放音乐,无法删除!");
				pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			}
		}
		else
		{
			pMainDlg->m_pTipDlg->SetTitle("请选择要删除的文件!");
			pMainDlg->m_pTipDlg->ShowWindow_(TRUE);
		}
		break;
	case 13:		//提示语
		OnButtonSetTip();
		break;
	case 14:		//开始本地录音
		OnButtonRecord();
		break;
	case 15:		//重命名
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
		if((hWnd == NULL && !pMainDlg->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic) || m_uiType != m_playType)
		{
			OnSelectAll();
		}
		break;
	case 57:
		if(m_sPreDir.size() > 0)
		{
			CString dir = m_sPreDir[m_sPreDir.size()-1];
			m_sPreDir.pop_back();
			SetPlayList(dir.GetBuffer(dir.GetLength()), m_uiType);
			SetInitShow();
			AddFileMp3();
			ShowArrayInList();
		}
		break;
	case 100:      //选择
	case 200:
	case 300:
	case 400:
	case 500:
		{
			if((hWnd == NULL && !pMainDlg->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic)
				|| m_callType == 1 || m_uiType != m_playType)
			{
				if (m_UnitStatus[w/100-1])
				{
					m_offSet = w/100 -1;
					OnButtonCheck(w);
				}
			}
			else
			{
				pMainDlg->m_pWarningNoFlashDlg->SetTitle("正在播放,若需重新选择,请暂停音乐!");
				pMainDlg->m_pWarningNoFlashDlg->ShowWindow_(TRUE);
			}
		}
		break;
	case 1000: //返回	
		OnExit_();
		break;
	case 1001: //确定
		InsertAudio2Mms();
		break;
	default:
		break;
	}
}

void C3GHomeJoyDlg::SetUnitFont()
{
	m_MJPGList.SetUnitText(9, _T("当前页"), TRUE);
	m_MJPGList.SetUnitText(52, _T("文件/文件夹"), TRUE);

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

	if (m_currentPage <= 1)   //当第一次显示时上翻按钮不可用
	{
		m_MJPGList.SetUnitIsDisable(7, TRUE);
	}
	else
	{
		m_MJPGList.SetUnitIsDisable(7, FALSE); 
	}
	if (m_fileCount <= m_pageSize)		//当总数小于6时设置下翻按钮不可用
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
	SetUnitIsAvailable(type);   //设置页签转换时的状态变化
	ShowArrayInList();
}

void C3GHomeJoyDlg::ShowArrayInList()
{
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
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
			if (m_fileMp3.begin() + i < m_fileMp3.end())
			{
				CString path = m_chDir + m_fileMp3[i];

				m_UnitStatus[index/100-1] = TRUE;
				SetShowStatus(index, TRUE);
				m_MJPGList.SetUnitText(index+2, m_fileMp3[i], FALSE);
				if(i<m_fileDir.size())
				{
					m_MJPGList.SetUnitBitmap(index, ".\\adv\\mjpg\\k5\\common\\png\\文件夹.bmp", L".\\adv\\mjpg\\k5\\common\\png\\文件夹.bmp", FALSE);
					
					if(m_uiType != 1)
					{
						m_MJPGList.SetUnitText(index+20, L"", FALSE);
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
						HWND hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1");
						if(IsWindow(hWnd) || main->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic)
						{
						//	if(1 == m_callType)
						//	{
								if(m_fileMp3[i] == main->m_pMainDlg->m_mainMp3Dlg_->m_ShowList[gPlayIndex-1])
								{
									fileSize = m_playFileSize/(1024*1024);
								}
						//	}
						}

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
		m_MJPGList.SetUnitIsDownStatus(50, FALSE);
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
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".Mp3")
			||strstr(filename, ".wav")||strstr(filename, ".WAV")
			||strstr(filename, ".mid")||strstr(filename, ".MID"))
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
		
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".mp3")||strstr(filename, ".MP3")||strstr(filename, ".Mp3")
				||strstr(filename, ".wav")||strstr(filename, ".WAV")
				||strstr(filename, ".mid")||strstr(filename, ".MID"))
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
				m_selCount--;
				if(m_selCount < m_isFileSelect.size())
				{
					m_isSelectAll = FALSE;
					m_firstSelPage = 0;
					m_lastSelPage = 0;
					m_MJPGList.SetUnitIsDownStatus(50, FALSE);
				}
				m_isFileSelect[index] = 0;
				SetShowStatus(uintNO, TRUE);
			}
		}
		else
		{
			m_sPreDir.push_back(m_chDir);
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
void C3GHomeJoyDlg::DeleteSelectedItem()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
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
				checkFile = m_fileMp3.erase(checkFile);
				checkItem = m_isFileSelect.erase(checkItem);
			}
			else
			{
				++checkItem;
				++checkFile;
				++m_vResult;
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
			++m_vResult;
			++i;
		}
	}

	//从新求当前总记录数
	m_fileCount = m_fileMp3.size();
	if (0 == m_fileCount)
	{
		m_MJPGList.SetUnitText(6, "/0", TRUE);
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
		
		//检查当前页的情况
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
		if(m_playType != m_uiType)
		{
			m_firstSelPage = 0;
		}
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
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));

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
		g_destDir = _T("/FlashDrv/MY_MUSIC/");
	}
	else if (3 == m_uiType)
	{
		if (1 == m_cardType3)
		{
			g_showDir = L"U盘";
		}
		else if (2 == m_cardType3)
		{
			g_showDir = L"SD卡";
		}
		g_destDir = _T("/FlashDrv/MY_MUSIC/");
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
		m_MJPGList.SetUnitText(54, _T("文件大小"), FALSE);

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
		
		m_MJPGList.SetUnitText(51, _T("状态"), FALSE);
		m_MJPGList.SetUnitText(53, _T("姓名/号码"), FALSE);
		m_MJPGList.SetUnitText(55, _T("时长"), FALSE);
		m_MJPGList.SetUnitText(56, _T("时间"), FALSE);
		
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
	CMultimediaPhoneDlg *main = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	HWND hWnd = ::FindWindow(L"csplayer_win1", L"csplayer window1");

	switch (message)
	{
	case WM_DELETESELITEM:
		if((hWnd == NULL && !main->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic) || m_uiType != m_playType)
		{
			DeleteSelectedItem();
		}
		break;
	case WM_SHIFTSELITEM:
		if((hWnd == NULL && !main->m_pMainDlg->m_mainMp3Dlg_->m_isMidiMusic) || m_uiType != m_playType)
		{
			m_MJPGList.SetUnitIsDownStatus(50, FALSE);
			m_isSelectAll = FALSE;
			SetPlayList(m_chDir, m_uiType);
			SetInitShow();
			AddFileMp3();
			ShowArrayInList();
		}
		break;
	};
	return CDialog::WindowProc(message, wParam, lParam);
}

void C3GHomeJoyDlg::OnDeviceChange(WPARAM w, LPARAM l)
{
  	if (w == 0x8000) //insert
	{
		::Sleep(2000);
		if (m_MJPGList.GetUnitIsDisable(2))
		{
			if(DetectDIR(_T("/usbdisk")))
			{
				m_MJPGList.SetUnitIsDisable(2, FALSE);
				m_cardType2 = 1;
				m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/音乐/U盘_fore.BMP", "./ADV/MJPG/k5/common/音乐/U盘_back.BMP", TRUE);
			}
			else if(DetectDIR(_T("/storagecard")))
			{
				m_MJPGList.SetUnitIsDisable(2, FALSE);
				m_cardType2 = 2;
				m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/音乐/SD卡_fore.BMP", "./ADV/MJPG/k5/common/音乐/SD卡_back.BMP", TRUE);
			}
		}
		else if (m_MJPGList.GetUnitIsDisable(3))
		{
			if(DetectDIR(_T("/usbdisk")) && (m_cardType2 != 1))
			{
				m_MJPGList.SetUnitIsDisable(3, FALSE);
				m_cardType3 = 1;
				m_MJPGList.SetUnitBitmap(3, "./ADV/MJPG/k5/common/音乐/U盘_fore.BMP", "./ADV/MJPG/k5/common/音乐/U盘_back.BMP", TRUE);
			}
			else if(DetectDIR(_T("/storagecard")) && (m_cardType2 != 2))
			{
				m_MJPGList.SetUnitIsDisable(3, FALSE);
				m_cardType3 = 2;
				m_MJPGList.SetUnitBitmap(3, "./ADV/MJPG/k5/common/音乐/SD卡_fore.BMP", "./ADV/MJPG/k5/common/音乐/SD卡_back.BMP", TRUE);
			}
		}
	}
	else if (w == 0x8004) //remove
	{
		::Sleep(1000);
		if(DetectDIR(_T("/usbdisk")))
		{
			m_cardType2 = 1;
			m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/音乐/U盘_fore.BMP", "./ADV/MJPG/k5/common/音乐/U盘_back.BMP", TRUE);
		}
		else if(DetectDIR(_T("/storagecard")))
		{
			m_cardType2 = 2;
			m_MJPGList.SetUnitBitmap(2, "./ADV/MJPG/k5/common/音乐/SD卡_fore.BMP", "./ADV/MJPG/k5/common/音乐/SD卡_back.BMP", TRUE);
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

void C3GHomeJoyDlg::ShowWindow_(int nCmdShow,int bbtnShow )
{
	if(m_callType != 1)
	{
		OnExit_();
	}
	
	// add by qi 20100302
	m_MJPGList.SetUnitIsShow(10,bbtnShow,false);
	m_MJPGList.SetUnitIsShow(11,bbtnShow,false);
	m_MJPGList.SetUnitIsShow(12,bbtnShow,false);
	
	m_selCount = 0;
	m_sPreDir.clear();
	ShowItemsInList(m_uiType);
	ShowWindow(nCmdShow);
	m_MJPGList.Invalidate();
}

void C3GHomeJoyDlg::OnExit_()
{
	GetParent()->SendMessage(WM_CHANGEWINDOW, (WPARAM)this, (LPARAM)SW_HIDE);
	
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	pMainDlg->PopbackIcon();

	m_selCount = 0;
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
			if((m_fileMp3[loop].Right(4) != L".mid") && (m_fileMp3[loop].Right(4) != L".MID"))
			{
				nFileSelected++;
			}
		}
	}
	
	main->m_pMainDlg->m_mainMp3Dlg_->CalculatePage(main->m_pMainDlg->m_mainMp3Dlg_->m_MP3List.size());
	main->m_pMainDlg->m_mainMp3Dlg_->ShowArrayInList(main->m_pMainDlg->m_mainMp3Dlg_->m_ShowList);
	
	if(nFileSelected > 0)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->playeraudio_->SetImageList(main->m_pMainDlg->m_mainMp3Dlg_->m_MP3List, index);
	}
	
	main->m_pMainDlg->m_mainMp3Dlg_->SetTimer(0x101, 200, NULL);

	m_MJPGList.Invalidate();
	gPlayIndex = 1;
}

void C3GHomeJoyDlg::PlaySound()
{
	m_pPlaySoundDlg->SetSound(m_soundType, m_vCurrentResult[0]->id(), 0, m_sListFilter);
	if (((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->isPlayProtect() && !((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin)
	{
		m_pPasswordDlg->SettingType(CHECK_PLAYPASSWORD);
		std::string strTemp = ((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_pSetting->playRecordPassword();
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
					//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetType(2);     //提示录音数据超出10秒
					//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
				}
			}
			else
			{
				//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->SetType(3);     //提示提示语应该为本地录音
				//((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
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

void C3GHomeJoyDlg::InsertAudio2Mms()
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(1 == m_callType)
	{	
		CString filename = m_chDir;
		for(int i=m_fileDir.size(); i<m_isFileSelect.size(); i++)
		{
			if(1 == m_isFileSelect[i])
			{
				filename += m_fileMp3[i];
				break;
			}
		}
		
		main->m_pMMSDlg->InsertAudio(filename);
		
		main->PopbackIcon();
		ShowWindow(false);
		
	}	
	m_callType = 0;
}