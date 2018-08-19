// YHTelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../multimediaphone.h"
#include "../MultimediaPhoneDlg.h"
#include "3GDetailReadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDT_SHOWWEATHER_ICON	0xFF00

int FindFileEx(CString s, CString &sPath, CString &sFilename)
{
	int nStart = 0;
	int n = s.Find(_T("/"), nStart);
	while(n != -1)
	{
		nStart = n+1;
		n = s.Find(_T("/"), nStart);
	}
	sFilename = s.Mid(nStart);
	sPath = s.Mid(0, nStart);
	return 0;
}


//   删除目录   
void DeleteDirectory(CString SrcDir, BOOL isShow = TRUE)   
{   
	CString findFilename = SrcDir;
	if(SrcDir.Right(1) == "/" || SrcDir.Right(1) == "\\" )
		findFilename += "*.*";
	else
		findFilename += "\\*.*";
	/*   lxz 20080621
	if(isShow)
	wprintf(_T("find file name is %s\n"), (LPCTSTR)findFilename);
	*/
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile((LPCTSTR)findFilename, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		Dprintf("not find file\n");
	}
	else
	{
		bool finished = false;
		do
		{
			::Sleep(0);
			wchar_t wideFromName[256] = {0};
			wsprintf(wideFromName, _T("%s/%s"), SrcDir, (LPCTSTR)FindFileData.cFileName); //findFileName
			
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
			/*  lxz 20080621
			if(isShow)
			wprintf(_T("into dir %s \n"), wideFromName);
				*/
				if(isShow)
					wprintf(_T("Delete %s...... \n"), wideFromName);
				DeleteDirectory(wideFromName, isShow);
			}
			else
			{
				if (!DeleteFile(wideFromName))
				{
					Dprintf("current error is %d.", GetLastError());
				}
				/* lxz 20080621
				if(isShow)
				wprintf(_T("delete %s\n"), wideFromName);
				*/
			}
			
			if (!FindNextFile(hFind, &FindFileData))
			{
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					Dprintf("find end.\n");
					finished = true;
				}
				else
				{
					Dprintf("Couldn't find next file.\n");
				}
			}
		} while (!finished);
		FindClose(hFind);
	}
	RemoveDirectory(SrcDir);
	/* lxz 20080621
	if(isShow)
	wprintf(_T("delete dir %s\n"), SrcDir);
	*/
}

/////////////////////////////////////////////////////////////////////////////
// C3GDetailReadDlg dialog

/*
void AddMenuLink(Menu3G *pMenu, MenuLink *pParentMenu)
{
	pParentMenu	
}

void RemoveMenuLink(MenuLink *MenuLink, MenuLink *pMenuLink)
{
			
}
*/

C3GDetailReadDlg::C3GDetailReadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(C3GDetailReadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(C3GDetailReadDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pMenuLink = NULL;
}


void C3GDetailReadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3GDetailReadDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3GDetailReadDlg, CDialog)
	//{{AFX_MSG_MAP(C3GDetailReadDlg)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPG)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DELETESELITEM, OnDeleteItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3GDetailReadDlg message handlers
/*
void C3GDetailReadDlg::OnTimer(UINT nIDEvent)
{
	if(IsWindowVisible())
	{
		if(nIDEvent == 1)
		{
			
		}
	}
	CDialog::OnTimer(nIDEvent);
}
*/

void C3GDetailReadDlg:: OnDeleteItem(WPARAM w, LPARAM l)
{
	doDelete();
}

BOOL C3GDetailReadDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_onlyEdit.Create(WS_CHILD|ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | ES_READONLY , CRect(285, 68, 775, 320), this, 0xFFFF);
	m_MMSShow.Create(WS_CHILD, CRect(285, 68, 775, 320), this, 0, 0);

	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k1\\中文\\3g_家庭百事通阅读.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));

	m_onlyEdit.MoveWindow(m_MJPGList.GetUnitRect(16), FALSE);
//	m_MMSShow.MoveWindow(m_MJPGList.GetUnitRect(16), FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3GDetailReadDlg::OnClickMJPG(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg *pMainDlg = ((CMultimediaPhoneDlg*)(theApp.m_pMainWnd));
	switch(w)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		clickmenu(w-1);
		break;
	case 6:
		doPre();
		break;
	case 7:
		doNext();
		break;
	case 8:			//转发
		break;
	case 9:			//回复
		break;
	case 10:		//回电
		break;
	case 11:		//删除
		{
			//doDelete();
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetHWnd(m_hWnd);
			std::string strTemp = ".\\adv\\mjpg\\k1\\common\\确定删除吗.bmp";
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->SetDelTip(strTemp.c_str());
			((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pDeleteTipDlg->ShowWindow_(TRUE);
		}
		break;
	case 12:
		if(m_onlyEdit.IsWindowVisible())
		{
			m_onlyEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
			m_onlyEdit.Invalidate();
		}
		break;
	case 13:
		if(m_onlyEdit.IsWindowVisible())
		{
			m_onlyEdit.SendMessage(WM_VSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
			m_onlyEdit.Invalidate();
		}
		break;
	case 100:
		OnExit();
	default:
		break;
	}
}

void C3GDetailReadDlg::inittitle(char *titleTxt, char *titleIconFilename)
{
	CString s = titleTxt;
	s += "列表";
	m_MJPGList.SetUnitText(15, titleTxt, FALSE, TRUE);
	m_MJPGList.SetUnitText(14, s, FALSE, TRUE);
	m_MJPGList.SetUnitBitmap(13, titleIconFilename, "", FALSE, TRUE);
}

void C3GDetailReadDlg::initmenu(int menutype, std::string spCode, int nOffset)
{
	//
	std::vector<boost::shared_ptr<Data::MenuManager> > pMenuManagerresult;
	std::string filter;
	if(m_nMenuType == 0)		//天气预报
	{
		filter = "spCode = '";
		filter += "11111";
		filter += "'";
	}
	else
		filter = spCode;
	pMenuManagerresult = Data::MenuManager::GetFromDatabase(filter);
	if(pMenuManagerresult.size() > 0)
		inittitle((char *)pMenuManagerresult[0]->menuInfos.menuName.c_str(), (char *)pMenuManagerresult[0]->menuInfos.imageName.c_str());
	else
		inittitle("定制消息", "定制消息");


	m_nMenuType = menutype;
	m_nMenuCount = 0;
	if(nOffset == 0)
		m_nMenuIndex = 0;
	m_nOffset = nOffset;
	m_filter = spCode;

	for(int i = 0; i < 5; i++)
	{
		m_MJPGList.SetUnitIsShow(i+8, FALSE, FALSE);
	}
	if(m_nMenuType == 0)		//天气预报
	{
		m_pWeatherdateresult = Data::Weather::GetFromDatabaseByTypeOffsetLength("", m_nOffset, 6);
		if(m_pWeatherdateresult.size() > 0)
		{
			m_nMenuCount = m_pWeatherdateresult.size();
			for(int i= 0; i < 5 && i < m_nMenuCount; i++)
			{
				m_MJPGList.SetUnitIsShow(i+8, TRUE, FALSE);
				
				char txt[32];
				sprintf(txt, "%s %04d年%d月%d日", m_pWeatherdateresult[i]->weatherInfos.area.c_str(), m_pWeatherdateresult[i]->weatherInfos.datetime.GetYear(), m_pWeatherdateresult[i]->weatherInfos.datetime.GetMonth(), m_pWeatherdateresult[i]->weatherInfos.datetime.GetDay());

				m_MJPGList.SetUnitText(i+8, txt, FALSE, TRUE);
			}
			ShowDetailRead(0, FALSE);
		}
	}
	else
	{
		m_pMediaFileresult = Data::MultimediaDownload::GetFromDatabaseByTypeOffsetLength(m_filter, nOffset, 6); 
		if(m_pMediaFileresult.size() > 0)
		{
			m_nMenuCount = m_pMediaFileresult.size();
			for(int i= 0; i < 5 && i < m_nMenuCount; i++)
			{
				m_MJPGList.SetUnitIsShow(i+8, TRUE, FALSE);
				m_MJPGList.SetUnitText(i+8, m_pMediaFileresult[i]->multimediaInfos.subject.c_str(), FALSE, TRUE);
			}
			ShowDetailRead(0, FALSE);
		}
	}
	m_MJPGList.Invalidate();
}

int C3GDetailReadDlg::clickmenu(int index)
{
	ShowDetailRead(index, TRUE);
	return 0;
}

void C3GDetailReadDlg::ShowDetailRead(int i, int flag)
{
//	m_MMSShow.ShowWindow(SW_HIDE);
	m_nClickIndex = i;
	m_onlyEdit.ShowWindow(SW_HIDE);
	m_MMSShow.ShowWindow(SW_HIDE);
	for(int j = 0; j < 5; j++)
		m_MJPGList.SetUnitIsDownStatus(8+j, FALSE);
	if(m_nMenuType == 0)			//天气
	{
		m_onlyEdit.ShowWindow(SW_SHOW);
		
	//	std::string filter = "datetime = '" + Util::StringOp::FromTimestamp(m_pWeatherdateresult[i]->weatherInfos.datetime);
	//	filter += "'";
	//	m_pWeatherresult = Data::Weather::GetFromDatabase(filter);
		std::string weather = "";
	//	for(int i = 0; i < m_pWeatherresult.size(); i++)
		{
			/*
			Util::Timestamp datetime;
			std::string area; //Area
			std::string wind; //Wind
			std::string temperature; //TemperaTure
			int code; //WeatherCode
            std::string description; //OtherInfo
			*/
			char txt[32];
			sprintf(txt, "%04d年%d月%d日", m_pWeatherdateresult[i]->weatherInfos.datetime.GetYear(), m_pWeatherdateresult[i]->weatherInfos.datetime.GetMonth(), m_pWeatherdateresult[i]->weatherInfos.datetime.GetDay());
			weather += txt;
			weather += "\r\n";
			weather += m_pWeatherdateresult[i]->weatherInfos.area;
			weather += "\r\n风力:";
			weather += m_pWeatherdateresult[i]->weatherInfos.wind;
			weather += "\r\n温度:";
			weather += m_pWeatherdateresult[i]->weatherInfos.temperature;
			weather += "\r\n天气:";
			std::string fil = "code = " +  Util::StringOp::FromInt(m_pWeatherdateresult[i]->weatherInfos.code);
			m_pWeatherPicture = Data::WeatherPicture::GetFromDatabase(fil);
		/*	if(m_pWeatherPicture.size() > 0)
			{
				weather += m_pWeatherPicture[0]->content;
				weather += "\r\n";
			}
			*/
			weather += m_pWeatherdateresult[i]->weatherInfos.description;
			weather += "\r\n\r\n";
		}
		CString str = weather.c_str();
		m_onlyEdit.SetWindowText(str);
		SetTimer(IDT_SHOWWEATHER_ICON, 20, NULL);
	}
	else
	{
		if(m_pMediaFileresult[i]->multimediaInfos.type == Data::MultimediaDownload::MultimediaInfo::Type::tText)
		{
			//todo 文字
			m_onlyEdit.ShowWindow(SW_SHOW);
			CString str = (char *)m_pMediaFileresult[i]->multimediaInfos.content.c_str();
			m_onlyEdit.SetWindowText(str);
			//m_MJPGList.SetUnitText(16, m_pMediaFileresult[i]->multimediaInfos.content.c_str(), TRUE, TRUE);
		}
		else if(m_pMediaFileresult[i]->multimediaInfos.type == Data::MultimediaDownload::MultimediaInfo::Type::tImage)
		{
			//todo 图片
			m_MJPGList.SetUnitBitmap(16, m_pMediaFileresult[i]->multimediaInfos.filename.c_str(), "", flag, TRUE);
		}
		else if(m_pMediaFileresult[i]->multimediaInfos.type == Data::MultimediaDownload::MultimediaInfo::Type::tMMS)
		{
			//todo 彩信
			CString s = m_pMediaFileresult[i]->multimediaInfos.filename.c_str();

			CString filename = "";
			CString path = "";
			
			FindFileEx(s, path, filename);
			if(path != "")
			{
				m_MMSShow.FindFileSmil(path.GetBuffer(256));
				m_MMSShow.ShowWindow(SW_SHOW);
			}
		}
		m_pMediaFileresult[i]->multimediaInfos.isRead = true;
		m_pMediaFileresult[i]->Update();
	}
	m_MJPGList.SetUnitIsDownStatus(i+8, TRUE);
	m_MJPGList.Invalidate();
}


void C3GDetailReadDlg::doNext()
{
	if(m_nMenuCount > 5)
	{
		m_nMenuIndex++;
		initmenu(m_nMenuType, m_filter, m_nMenuIndex*5+1);
	}
}

void C3GDetailReadDlg::doDelete()
{
	if(m_nMenuType == 0)    //天气预报
	{
		int size = m_pWeatherdateresult.size();
		m_pWeatherdateresult[m_nClickIndex]->Remove();
		if(size == 1)
		{
			m_nOffset -= 5;
			if(m_nOffset < 0)
				m_nOffset = 0;
		}
	}
	else					//其他
	{
		int size = m_pMediaFileresult.size();
		m_pMediaFileresult[m_nClickIndex]->Remove();
		//删除目录
		CString sFilename =  m_pMediaFileresult[m_nClickIndex]->multimediaInfos.filename.c_str();
		if(sFilename != "")
		{
			CString path = "";
			CString filename = "";
			FindFileEx(sFilename, path, filename);
			if(path != "")
			{
				DeleteDirectory(path, FALSE);
			}
		}
		if(size == 1)
		{
			m_nOffset -= 5;
			if(m_nOffset < 0)
				m_nOffset = 0;
		}
	}
	initmenu(m_nMenuType, m_filter, m_nOffset);
}

void C3GDetailReadDlg::doPre()
{
	if(m_nMenuIndex > 0)
	{
		m_nMenuIndex--;
		initmenu(m_nMenuType, m_filter, m_nMenuIndex*5+1);
	}
}

void C3GDetailReadDlg::OnExit()
{
	m_MJPGList.SetUnitText(16, "", FALSE, TRUE);
	m_MJPGList.SetUnitBitmap(16, "", "", FALSE, TRUE);
	m_onlyEdit.SetWindowText(L"");
	m_MMSShow.ShowWindow(SW_HIDE);
	m_onlyEdit.ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
}

void C3GDetailReadDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == IDT_SHOWWEATHER_ICON)
	{
		KillTimer(IDT_SHOWWEATHER_ICON);
		if(m_pWeatherPicture.size() > 0)
		{
			std::string filename = m_pWeatherPicture[0]->filename;
			HDC pdc = ::GetDC(m_onlyEdit.m_hWnd);
			extern void DrawImage_HDC(LPCTSTR szFileImage, HDC pdc, CRect rect, BOOL isOffset = FALSE, VOID *lpBits = NULL);
			CRect rt = m_MJPGList.GetUnitRect(16);
			rt.left = rt.Width() - 128;
			rt.right = rt.left + 128;
			rt.top = 0;
			rt.bottom = rt.top + 128;
			CString sF = (char *)filename.c_str();
			DrawImage_HDC(sF, pdc, rt);
			::ReleaseDC(m_onlyEdit.m_hWnd, pdc);
		}
	}
}
/*
void C3GDetailReadDlg::InitMenuLink()
{
	
}

void C3GDetailReadDlg::FreeMenuLink()
{
	if(m_pMenuLink)
	{
		
	}
}
*/