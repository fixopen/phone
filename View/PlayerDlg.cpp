// PlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PlayerDlg.h"
#include "../MultimediaPhoneDlg.h"
#include "../MultimediaPhone.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include<algorithm>
/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog

#define PHOTOSHOW_TIME		5*1000
CPlayerDlg::CPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayerDlg)
		// NOTE: the ClassWizard will add member initialization here
	memset(m_chVideoFilename, 0, 128);
	type_ = 0;
	isActiveMode_ = 0;
	//}}AFX_DATA_INIT
}


void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER+100, OnReceiveMsg)
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg message handlers
void CPlayerDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(!main->player_->type_ && isActiveMode_) //图片 && 手动
	{
		if(nChar == 'U')			//page up
		{
			main->player_->Up();	
		}
		else if(nChar == 'D')		//page down
		{
			main->player_->Down();
		}
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HWND hWnd;
	CDialog::OnLButtonDown(nFlags, point);
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(!main->player_->type_)
	{
		main->m_pMainDlg->SendMessage(WM_PLAYPHOTO, 3);  //退出全屏
	//	GetParent()->GetParent()->SendMessage(WM_PLAYPHOTO, 3);  //退出全屏
	}
	
/*
	CCheckDlg* main = (CCheckDlg*)theApp.m_pMainWnd;
	if(!main->player_->type_)
	{
		main->player_->StopImage();
	}
	else
	{
		main->player_->ExitPlayer();
		hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
		::ShowWindow(hWnd, SW_HIDE);
		main->settingDlg_->ShowWindow(SW_SHOW);
	}
//	::ShowCursor(TRUE);
	EndDialog(0);
*/
}

BOOL CPlayerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// SetCursorPos
	// TODO: Add extra initialization here
	//main->player_ = new Player();
	/*
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if(!main->player_->isActiveMode_)  //屏保
	{
		SetTimer(0x10002, 2*1000, NULL);
	}*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlayerDlg::OnReceiveMsg(WPARAM w, LPARAM l)
{
	/*
	HWND hWnd;
	CCheckDlg* main = (CCheckDlg*)theApp.m_pMainWnd;
	if (main->player_->isActiveMode_ == false)
	{
		if(main->player_->type_ == 0)
			main->player_->StopImage();
		else
		{
			main->player_->ExitPlayer();
			hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
				::ShowWindow(hWnd, SW_HIDE);
				main->settingDlg_->ShowWindow(SW_SHOW);
		}
//		::ShowCursor(TRUE);
		EndDialog(0);
	}
	else
	{
		UINT nChar = w;
		switch (nChar)
		{
		case Phone::UP:
			if(main->player_->type_ == 0)
				main->player_->Up();
			else
			{
				main->player_->ExitPlayer();
				hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
				::ShowWindow(hWnd, SW_HIDE);
				main->settingDlg_->ShowWindow(SW_SHOW);
//				::ShowCursor(TRUE);
				EndDialog(0);
			}

			break;
		case Phone::DOWN:
			if(main->player_->type_ == 0)
			{
				main->player_->Down();
			}
			else
			{
				main->player_->ExitPlayer();
//				::ShowCursor(TRUE);
				EndDialog(0);
				hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
				::ShowWindow(hWnd, SW_HIDE);
				main->settingDlg_->ShowWindow(SW_SHOW);
			}
			break;
		case Phone::BS:
			if(main->player_->type_ == 0)
				main->player_->StopImage();
			else
			{
				main->player_->ExitPlayer();
////			::ShowCursor(TRUE);
				EndDialog(0);
				hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
				::ShowWindow(hWnd, SW_HIDE);
				main->settingDlg_->ShowWindow(SW_SHOW);
			}
			break;
		default:
			break;
		}
	}
	*/
}

void CPlayerDlg::OnOk(void)
{
}

void CPlayerDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	HWND hWnd;
	/*
	// TODO: Add your message handler code here and/or call default
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if (main->player_->isActiveMode_ == false)
	{
		if(main->player_->type_ == 0)
			main->player_->StopImage();
		else
		{
			main->player_->ExitPlayer();
			hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
			::ShowWindow(hWnd, SW_HIDE);
			main->settingDlg_->ShowWindow(SW_SHOW);
		}
//		::ShowCursor(TRUE);
		EndDialog(0);
	}
	else
	{
		switch (nChar)
		{
		case Phone::BS:
			if(main->player_->type_ == 0)
				main->player_->StopImage();
			else
			{
				main->player_->ExitPlayer();
				hWnd = ::FindWindow(L"csplayer_win", L"csplayer window"); //
				::ShowWindow(hWnd, SW_HIDE);
				main->settingDlg_->ShowWindow(SW_SHOW);
			}

	//			::ShowCursor(TRUE);
			EndDialog(0);
			break;
		default:
			break;
		}
	}
	*/
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CPlayerDlg::AddImageList(char *picDir[], int ncount)
{
	std::vector<CString> list1;
	for(int i = 0; i < ncount; i++)
	{
		if(i == 0)
			list1 = SetImageList_(picDir[i]);
		else
		{
			std::vector<CString> list = SetImageList_(picDir[i]);
			list1.insert(list1.end(), list.begin(), list.end());
		}
	}
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->player_->SetImageList(list1);
}

BOOL CPlayerDlg::SetImageListForScreenSave(char *FileName)
{
	FILE *file;
	if((file = fopen(FileName, "r+b")) != NULL)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		std::vector<CString> filelist;
		char name[128];
		memset(name, 0, 128);
		while(!feof(file))
		{
			if(fgets(name, 128, file))
			{
				int n = strlen(name);
				name[n-2] = '\0';
				CString sf = name;
				filelist.push_back(sf);
			}
		}
		fclose(file);
		std::sort(filelist.begin(), filelist.end());
		main->player_->SetImageList(filelist);
		return TRUE;
	}
	return FALSE;
}

void CPlayerDlg::SetImageList(char *DIR)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->player_->SetImageList(SetImageList_(DIR));
}

std::vector<CString> CPlayerDlg::SetImageList_(char *DIR)
{
	std::vector<CString> filelist;

	WIN32_FIND_DATA FindFileData;			//查找文件时要使用的数据结构
	HANDLE hFind = INVALID_HANDLE_VALUE;	//定义查找句柄
//	char filename[MAX_PATH]; 
	int ncount = 0;

	char totalfilename[128];
	sprintf(totalfilename, "%s*.*", DIR);
	CString sDir = totalfilename;
	hFind = FindFirstFile(sDir, &FindFileData);//使用FindFirstFile函数来开始文件查找
		
	if (hFind == INVALID_HANDLE_VALUE) 
	{
//	  printf ("Inval  return 0\n;");
	  return filelist;
	} 
	else 
	{
		char filename[128];
//		printf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
		filename[i] = '\0';
		if(strstr(filename, ".jpg")||strstr(filename, ".JPG")||strstr(filename, ".bmp")||strstr(filename, ".BMP"))
		{
			sprintf(totalfilename, "%s%s", DIR, filename);
			CString sf = totalfilename;
			filelist.push_back(sf);
		}
	
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
//			printf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".jpg")||strstr(filename, ".JPG")||strstr(filename, ".bmp")||strstr(filename, ".BMP"))
			{
				sprintf(totalfilename, "%s%s", DIR, filename);
				CString sf = totalfilename;
				filelist.push_back(sf);
			}
		}

		std::sort(filelist.begin(), filelist.end());

		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);//关闭查找句柄
		} 
		else 
		{
//			printf ("FindNextFile error. Error is %u\n", dwError);
			return filelist;
		}
	}
	
	return filelist;
}

void CPlayerDlg::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
	CDialog::OnPaint();

	//临时加上相框 20071217 by lxz
	CRect rt;
	GetClientRect(&rt);
	if(rt.Width() >= 479 && rt.Height() >= 233)
	{
		CxImage *pImage = new CxImage();
		if(pImage)
		{
			bool ret = pImage->Load(TEXT("/flashdrv/a4.jpg"), CXIMAGE_FORMAT_JPG);
			if(ret)
			{
				CDC *pdc = GetDC();
				pImage->Draw(pdc->m_hDC, 0, 0, 480, 234, 0, TRUE);
				ReleaseDC(pdc);
			}
		}
		delete pImage;
	}
	//

	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(!main->player_->type_)
	{
		main->player_->PlayerImage();
	}
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CPlayerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0x10002)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		if(!main->player_->type_ && IsWindowVisible() && main->player_->owner_ == this)
		{
			main->player_->Down();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CPlayerDlg:: OnOK()
{
}

void CPlayerDlg:: OnCancel()
{
}

void CPlayerDlg:: SetParam(char *videoFile, int isVideo , BOOL isSaveScreen)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->player_->isActiveMode_ = !isSaveScreen;
	main->player_->type_ = isVideo;
	if(isVideo != 0)
	{
		main->player_->isVideoType = isVideo;
		//如果进入视频，则需要停止播放MP3音乐
		if(main->player_->isPlayingVideo && main->player_->isVideoType == 2)
			main->player_->ExitPlayer(TRUE);   
	}

	type_ = isVideo;
	isActiveMode_ = !isSaveScreen;
	main->player_->SetOwner(this);
	if(strlen(videoFile) != 0)
		strcpy(m_chVideoFilename, videoFile);

	if(!isActiveMode_)  //屏保
	{
		if(!main->player_->type_)	//图片
			SetTimer(0x10002, PHOTOSHOW_TIME, NULL);
	}
}

void CPlayerDlg:: ReSetWindowsRect(CRect rt)
{
	MoveWindow(&rt, FALSE);

	//临时加上相框 20071217 by lxz
	CRect rt1;
	GetClientRect(&rt1);
	if(rt1.Width() >= 479 && rt1.Height() >= 234)
	{
		CxImage *pImage = new CxImage();
		if(pImage)
		{
			bool ret = pImage->Load(TEXT("/flashdrv/a4.jpg"), CXIMAGE_FORMAT_JPG);
			if(ret)
			{
				CDC *pdc = GetDC();
				pImage->Draw(pdc->m_hDC, 0, 0, 480, 234, 0, TRUE);
				ReleaseDC(pdc);
			}
		}
		delete pImage;
	}
}

void CPlayerDlg:: Show()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	main->player_->SetOwner(this);
	ShowWindow(SW_SHOW);

	if(type_)
	{
		main->player_->InitPlayer();
		main->player_->PlayerFile(m_chVideoFilename);
	}
}

void CPlayerDlg:: Hide()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	ShowWindow(SW_HIDE);
	if(main->player_->type_)
	{
//		main->player_->ExitPlayer();
	}
}

void CPlayerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if(bShow)
	{
		Show();
	}
	else
	{
		Hide();
	}
}

void CPlayerDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

    if( nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
        Show();
    else if(nState == WA_INACTIVE)
       int i = 0;
}

void CPlayerDlg::SetResumeTimer()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(!main->player_->type_)	//图片
	SetTimer(0x10002, PHOTOSHOW_TIME, NULL);
}

void CPlayerDlg:: StopTimer()
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(!main->player_->type_)	//图片
		KillTimer(0x10002);
}
