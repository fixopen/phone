// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../MultimediaPhone.h"
#include "../MultimediaPhoneDlg.h"
#include "../Data/LanguageResource.h"
#include "../Logical/LunarderDate.h"
#include "../Data/SkinStyle.h"
#include "MainDlg.h"
#include "../data/otadata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

#define TIMER_MAINDLG		0xFFF3
extern BOOL DetectDIR(TCHAR *sDir);

CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	m_mainmenuDlg_ = NULL;
	m_nUnTel = 0;
	m_nLeaveSound = 0;
	m_bIsFireWall = FALSE;
	m_bIsAlarm = FALSE;

	m_nSMSCount = 0;
	m_nMMSCount = 0;
	m_nUnSMS = 0;
	m_nSMSLeaveCount = 0;
	m_nRssCount = 0;
	//}}AFX_DATA_INIT
}

CMainDlg::~CMainDlg()
{
	if(m_mainmenuDlg_)
		delete m_mainmenuDlg_;
	m_mainmenuDlg_ = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	//}}AFX_DATA_MAP
	/*
	DDX_Control(pDX, IDC_STATIC_LINE, m_LineStatic);
	DDX_Control(pDX, IDC_STATIC_TODAY, m_TodayStatic);
	DDX_Control(pDX, IDC_STATIC_TELSTATUS, m_TelStatusStatic);
	DDX_Control(pDX, IDC_STATIC_CALLWALL, m_CallWallStatic);
	DDX_Control(pDX, IDC_STATIC_TIME, m_TimeStatic);
	*/
}

void CMainDlg ::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
	if(nState)
	{
	/*	OnShowCallWallStatic();
		OnShowNoteStatic();
		OnShowTelStatusStatic();
		*/
	}
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_CHANGEWINDOW, OnChangeWindow)
	ON_MESSAGE(WM_PLAYVIDEO, OnPlayVideo)
	ON_MESSAGE(WM_PLAYPHOTO, OnPlayPhoto)
	ON_MESSAGE(WM_STATIC_CLICK, OnStaticClick)
	ON_MESSAGE(WM_CLICKMJPG_TOAPP, OnClickMJPGToApp)
	ON_MESSAGE(WM_MJPGSHOWHALF, OnMJPGShowHalf)
	ON_WM_ACTIVATE()
	ON_MESSAGE(WM_CHECKPASSWORD, OnSetAdmin)
	ON_MESSAGE(WM_DELETESELITEM, OnUnSetAdmin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers
void CMainDlg::SetCtrlEnable(BOOL flag)
{
//	m_LineStatic.EnableWindow(flag);
//	m_TodayStatic.EnableWindow(flag);
	/*
	m_TelStatusStatic.EnableWindow(flag);
	m_CallWallStatic.EnableWindow(flag);
	m_TimeStatic.EnableWindow(flag);
	*/
}

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_pWebDialog = new CWebDialog;
	m_pWebDialog->Create(CWebDialog::IDD);
	m_pWebDialog->ShowWindow_(SW_HIDE);

	m_mainmenuDlg_ = new CMainMenuDlg;
	m_mainmenuDlg_->Create(CMainMenuDlg::IDD, this);
	m_mainmenuDlg_->ShowWindow_(SW_HIDE);
	m_currentWnd = (CWnd *)m_mainmenuDlg_;
		
	m_mainVideoDlg_ = new CMainVideoDlg;
	m_mainVideoDlg_->Create(CMainVideoDlg::IDD, this);
	m_mainVideoDlg_->ShowWindow(SW_HIDE);
	
	m_mainPhotoDlg_ = new CMainPhotoDlg;
	m_mainPhotoDlg_->Create(CMainPhotoDlg::IDD, this);
	m_mainPhotoDlg_->ShowWindow(SW_HIDE);
	
	m_mainMp3Dlg_ = new CMainMp3Dlg;
	m_mainMp3Dlg_->Create(CMainMp3Dlg::IDD, this);
	m_mainMp3Dlg_->ShowWindow(SW_HIDE);
	
	m_mainScreenSaveDlg_ = new CScreenSaveDlg;
	m_mainScreenSaveDlg_->Create(CScreenSaveDlg::IDD);
	m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
	
	m_mainCalucaterDlg_ = new CCalculaterDlg;
	m_mainCalucaterDlg_->Create(CCalculaterDlg::IDD, this);
	m_mainCalucaterDlg_->ShowWindow(SW_HIDE);

	m_mainLunarderDlg_ = new CLunarderDlg;
	m_mainLunarderDlg_->Create(CLunarderDlg::IDD, this);
	m_mainLunarderDlg_->ShowWindow(SW_HIDE);
	
	
	m_firewalDlg_ = new CFireWallDlg();
	m_firewalDlg_->Create(CFireWallDlg::IDD, this);
	m_firewalDlg_->ShowWindow(SW_HIDE);

	m_p3GHomePicDlg = new C3GHomePicDlg();
	m_p3GHomePicDlg->Create(C3GHomePicDlg::IDD, this);
	m_p3GHomePicDlg->ShowWindow(SW_HIDE);

	m_p3GHomeJoyDlg = new C3GHomeJoyDlg();
	m_p3GHomeJoyDlg->Create(C3GHomeJoyDlg::IDD, this);
	m_p3GHomeJoyDlg->ShowWindow(SW_HIDE);

	m_p3GHomeMovieDlg = new C3GHomeMovieDlg();
	m_p3GHomeMovieDlg->Create(C3GHomeJoyDlg::IDD, this);
	m_p3GHomeMovieDlg->ShowWindow(SW_HIDE);

	m_p3GSMSDlg = new CSMSDlg();
	m_p3GSMSDlg->Create(CSMSDlg::IDD, this);
	m_p3GSMSDlg->ShowWindow(SW_HIDE);

	
// add by qi 2009_09_14

//  change by qi 2009_09_14
//	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 420), this);
//	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\3g_桌面.xml");
//	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 420));

	// add by qi 2009_09_14
	m_MJPGList.Create(L"", WS_VISIBLE|WS_CHILD, CRect(0, 0, 800, 423), this);
	m_MJPGList.SetCurrentLinkFile(".\\adv\\mjpg\\k5\\中文\\3g_桌面.xml");
	m_MJPGList.SetMJPGRect(CRect(0, 0, 800, 423));


	// change by qi 2009_09_15
	SetDateTime(FALSE);

	SetTimer(1, 1000, NULL);
	SetTimer(2, 5000, NULL);
	SetTimer(4, 1000, NULL);

	OnShowNoteStatic();
	OnShowCallWallStatic();
	OnShowTelStatusStatic(0, 0);
	OnShowTelStatusStatic(3, 0);

//	OnTimer(2);

	SetPhotoList();
	SetWeather();
	SetUpInfo();


//	for(int i = 12; i < 20; i++)
//	m_MJPGList.SetUnitIsTranslate(i, TRUE);

//	m_FrameStatic.Create(CRect(0, 0, 480, 204), this, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CMainDlg::doReadWeather()			//天气预报
{
	int type = 0;
	std::string spCode = " DISTINCT datetime ";
}

void CMainDlg::doReadRss()
{
	if(m_nRssCount > 0)
	{
		std::string spCode = "spCode = '";
		spCode += rssFileresult[0]->multimediaInfos.spCode;
		spCode += "'";
		int type = 1;
	}

}

void CMainDlg::doReadSMS()
{
	CSMSDlg *pWnd_ = m_p3GSMSDlg;
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd ;
	
	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_pMainDlg->SendMessage(WM_PLAYVIDEO, 0, 0);    //暂停视频   重复发送了暂停信息

	if(m_nMMSCount > 0)
	{	
		//  change by qi 2009_11_16
		
		main->m_pSMSListDlg->m_pMmsReadDlg->SetMMSInfo(mmsFileresult[0]->id(),RECV_TYPE);
//		mmsFileresult[0]->isRead = TRUE;
//		mmsFileresult[0]->Update();
		main->m_pSMSListDlg->m_pMmsReadDlg->ShowWindow(SW_SHOW);

		main->AddIcon(Allicon[1]);

	}
	else if(m_nSMSCount > 0)
	{
		main->m_pSMSListDlg->m_pSmsReadDlg->SetSMSInfo(smsFileresult[0]->id(),RECV_TYPE);
	//	smsFileresult[0]->state = Data::Message::sReaded;
	//	smsFileresult[0]->Update();
		main->m_pSMSListDlg->m_pSmsReadDlg->ShowWindow(SW_SHOW);

		main->AddIcon(Allicon[1]);

	}
	else
	{
//		pWnd_->m_pSMSListDlg->initType(RECV_TYPE, SMS_TYPE);
//		pWnd_->m_pSMSListDlg->ShowWindow(SW_SHOW);
	}
}

void CMainDlg::doReadLeaveHome()
{
	CSMSDlg *pWnd_ = m_p3GSMSDlg;
	if(m_nSMSLeaveCount > 0)
	{
		pWnd_->m_pSMSDetailDlg->initDataBase(SMS_READ, smsLeaveresult[0]->id(), FALSE);
		smsLeaveresult[0]->state = Data::Message::sReaded;
		smsLeaveresult[0]->Update();
		pWnd_->m_pSMSDetailDlg->ShowWindow(SW_SHOW);
	}
	else
	{
//		pWnd_->m_pSMSListDlg->initType(HOME_RECORD_TYPE, HOME_TYPE);
//		pWnd_->m_pSMSListDlg->ShowWindow(SW_SHOW);
	}
}

void CMainDlg::SetUpInfo(BOOL isDraw)
{
	//add by qi 2009_09_15
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;

	std::string filter = "isRead = 0";
	rssFileresult = Data::MultimediaDownload::GetFromDatabase(filter, Data::dNull, 0, 1); 
	m_nRssCount = rssFileresult.size();

	filter = "[group] = " + Util::StringOp::FromInt(Data::Message::gReceive);
	filter += " AND state = ";
	filter += Util::StringOp::FromInt(Data::Message::sNoRead);
	smsFileresult = Data::Message::GetFromDatabase(filter, Data::dNull, 0, 1); 
	m_nSMSCount = smsFileresult.size();

	filter = "[type] = " + Util::StringOp::FromInt(Data::MMSData::tpReceive);
	filter += " AND isRead = ";
	filter += Util::StringOp::FromInt(0);
	mmsFileresult = Data::MMSData::GetFromDatabase(filter, Data::dNull, 0, 1);
	m_nMMSCount = mmsFileresult.size();


	static int nFresh = 0;

	CString sRss = ".\\adv\\mjpg\\k5\\common\\png\\我的订阅.bmp";
	CString sTel = ".\\adv\\mjpg\\k5\\common\\png\\电话小标.png";
	CString sTel1 = ".\\adv\\mjpg\\k5\\common\\png\\电话小标1.png";

	CString sSms = ".\\adv\\mjpg\\k5\\common\\png\\未读留言.png";
	CString sSms1 = ".\\adv\\mjpg\\k5\\common\\png\\未读留言1.png";

	if(nFresh++ % 2)
	{
		if(m_nRssCount > 0)
			sRss = "";
		if(m_nSMSCount > 0 || m_nMMSCount > 0)
			sSms1 = "";
		if(m_nUnTel)
			sTel1 = "";
	}
	
	static CString gsTel = "";
	static CString gsSMS = "";
	static CString gsFireWall = "";
	
	if(gsTel != sTel1)
	{	
		//add by qi 2009_09_16
		main->m_MJPGList.SetUnitBitmap(4,sTel1,"",isDraw);
		gsTel = sTel1;
	}

	if(gsSMS != sSms1)
	{			
		//add by qi 2009_09_16
		main->m_MJPGList.SetUnitBitmap(5, sSms1, "", isDraw);
		gsSMS = sSms1;
	}

}

void CMainDlg::SetWeather()
{
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CTime time = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, 0, 0, 0);

	std::vector<boost::shared_ptr<Data::RegisterReply> > registReplyresult = Data::RegisterReply::GetFromDatabase(""); 
	std::string area = "南京";
	if(registReplyresult.size() > 0)
	{
		area = registReplyresult[0]->areaCode.c_str();
	}

	std::string filter = "area = '";
	filter += area;
	filter += "' AND datetime = '";
	filter += Util::StringOp::FromTimestamp(time);
	filter += "'";
	std::vector<boost::shared_ptr<Data::Weather> > weaherResult = Data::Weather::GetFromDatabase(filter); 
	CString s = area.c_str();
	CString s1 = "今天\r\n";
	if(weaherResult.size() > 0)
	{
		std::string fil = "code = " +  Util::StringOp::FromInt(weaherResult[0]->weatherInfos.code);
		std::vector<boost::shared_ptr<Data::WeatherPicture> > pWeatherPicture = Data::WeatherPicture::GetFromDatabase(fil);
		if(pWeatherPicture.size() > 0)
		{
			m_MJPGList.SetUnitBitmap(200, (char *)pWeatherPicture[0]->filename.c_str(), "", TRUE);
		}
		s += weaherResult[0]->weatherInfos.wind.c_str();
		s += "\r\n";
		s += weaherResult[0]->weatherInfos.description.c_str();
		s += "\r\n";
		s += weaherResult[0]->weatherInfos.temperature.c_str();

		s1 += weaherResult[0]->weatherInfos.temperature.c_str();
		s1 += "\r\n";
		s1 += weaherResult[0]->weatherInfos.description.c_str();
	}
	else
	{
		s += "\r\n天气\r\n无定制";
		s1 += "天气\r\n无定制";
	}
	m_MJPGList.SetUnitText(201, s, TRUE);
	m_MJPGList.SetUnitText(202, s1, TRUE);

	time += CTimeSpan(1, 0, 0, 0);
	filter = "area = '";
	filter += area;
	filter += "' AND datetime = '";
	filter += Util::StringOp::FromTimestamp(time);
	filter += "'";
	s1 = "明天\r\n";
	weaherResult = Data::Weather::GetFromDatabase(filter); 
	if(weaherResult.size() > 0)
	{
		s1 += weaherResult[0]->weatherInfos.temperature.c_str();
		s1 += "\r\n";
		s1 += weaherResult[0]->weatherInfos.description.c_str();
	}
	else
	{
		s1 += "天气\r\n无定制";
	}
	m_MJPGList.SetUnitText(203, s1, TRUE);

	time += CTimeSpan(1, 0, 0, 0);
	filter = "area = '";
	filter += area;
	filter += "' AND datetime = '";
	filter += Util::StringOp::FromTimestamp(time);
	filter += "'";
	s1 = "后天\r\n";
    weaherResult = Data::Weather::GetFromDatabase(filter); 
	if(weaherResult.size() > 0)
	{
		s1 += weaherResult[0]->weatherInfos.temperature.c_str();
		s1 += "\r\n";
		s1 += weaherResult[0]->weatherInfos.description.c_str();
	}
	else
	{
		s1 += "天气\r\n无定制";
	}
	m_MJPGList.SetUnitText(204, s1, TRUE);

}

BOOL CMainDlg::ShowTodayAlarm()
{
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CTime time = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, 0, 0, 0);
	
	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(),time.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(),time.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";
	std::string order = "startTime";
	std::vector<boost::shared_ptr<Data::Scheduler> > m_result = Data::Scheduler::GetFromDatabaseByOrder(filter, order); 
	if (!m_result.empty())
	{
		for(int i = 0; i < m_result.size(); i++)
		{
			CTime tm = m_result[i]->startTime();
			GetLocalTime(&curtime);
			CTime nowtm = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
			if(m_result[i]->tipsType() == Data::ttAlarmSound && (nowtm > tm))
			{
				std::string content = m_result[i]->what();
				std::string ring = "/flashdrv/my_ring/"; 
				ring += m_result[i]->ring();
				CString content_ = "内容: ";
				content_ += content.c_str();
				CTime tm = m_result[i]->startTime();
				CString sTime;
				sTime.Format(_T("时间: %04d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
				m_result[i]->tipsType(Data::ttRead);
				m_result[i]->Update();
				((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_AlarmShowDlg->SetTxt(m_result[i]->id(), sTime, "标题: ", content_, "", FALSE);
				((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_AlarmShowDlg->ShowWindow_(SW_SHOW);
				return TRUE;
			}
		}
	}
	return FALSE;
}

// // 
BOOL CMainDlg::FindTodayAlarm()
{
	m_bIsAlarm = FALSE;

	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CTime time(curtime.wYear, curtime.wMonth, curtime.wDay, 0, 0, 0);
	
	std::string filter = "startTime BETWEEN '";
	char buff[32];
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d000000", time.GetYear(), time.GetMonth(),time.GetDay());//, dateFrom.GetHour(), dateFrom.GetMinute(), dateFrom.GetSecond());
	filter += buff;
	filter += "' AND '";
	memset(buff, 0, 32);
	sprintf(buff, "%04d%02d%02d235959", time.GetYear(), time.GetMonth(),time.GetDay());//, dateTo.GetHour(), dateTo.GetMinute(), dateTo.GetSecond());
	filter += buff;
	filter += "'";
 	std::string order = "startTime";
//	std::string filter = "startTime BETWEEN '20090201000000' AND '20090201235959'";
	std::vector<boost::shared_ptr<Data::Scheduler> > m_result = Data::Scheduler::GetFromDatabaseByOrder(filter, order); 
	if (!m_result.empty())
	{
		for(int i = 0; i < m_result.size(); i++)
		{
			CTime tm = m_result[i]->startTime();
			GetLocalTime(&curtime);
			CTime nowtm = CTime(curtime.wYear, curtime.wMonth, curtime.wDay, curtime.wHour, curtime.wMinute, curtime.wSecond);
			if(m_result[i]->tipsType() == Data::ttAlarmSound && (nowtm > tm))
			{
				m_bIsAlarm = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CMainDlg::SetStatusAll(BOOL flag)
{
	  
// 	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
// 	main->m_MJPGList.SetUnitIsDownStatus(2, FALSE);
// 	main->m_MJPGList.SetUnitIsDownStatus(3, FALSE);
// 	main->m_MJPGList.SetUnitIsDownStatus(4, FALSE);
// 	main->m_MJPGList.SetUnitIsDownStatus(5, FALSE);
// 	main->m_MJPGList.SetUnitIsDownStatus(6, FALSE);
// 	main->m_MJPGList.SetUnitIsDownStatus(7, FALSE);

//	if(!flag)
	//  change by qi 2009_09_16
	//	main->m_MJPGList.SetUnitIsDownStatus(2, TRUE);
//	main->m_MJPGList.Invalidate();
}

void CMainDlg::HideAllWindow()
{
	m_pWebDialog->ShowWindow(SW_HIDE);	
	m_mainmenuDlg_->ShowWindow(SW_HIDE);	

	m_mainVideoDlg_->ShowWindow(SW_HIDE);	
	m_mainPhotoDlg_->ShowWindow(SW_HIDE);	

	m_mainMp3Dlg_->ShowWindow(SW_HIDE);	
	m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);	

	m_mainCalucaterDlg_->ShowWindow(SW_HIDE);	
	m_firewalDlg_->ShowWindow(SW_HIDE);	

	m_mainLunarderDlg_->ShowWindow(SW_HIDE);	
		
	m_p3GHomePicDlg->ShowWindow(SW_HIDE);	
	m_p3GHomeJoyDlg->ShowWindow(SW_HIDE);
	
	m_p3GHomeMovieDlg->ShowWindow(SW_HIDE);	
	m_p3GSMSDlg->ShowWindow(SW_HIDE);
	
	
}

void CMainDlg::SetDateTime(BOOL isDraw)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;

	CString sTime;
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	char time[40];
	sprintf(time,"%02d:%02d:%02d",curtime.wHour,curtime.wMinute,curtime.wSecond);
	sTime = time ;
	main->m_MJPGList.SetUnitText(2,sTime,isDraw);
	main->m_MJPGList.SetUnitFont(2,font_18);
	main->m_MJPGList.SetUnitColor(2,font_white,isDraw);

	int nWeekDay = Logical::LunarderDate::WeekDay(curtime.wYear, curtime.wMonth, curtime.wDay);
	char txt[64];
	sprintf(txt, "%04d-%02d-%02d", curtime.wYear, curtime.wMonth, curtime.wDay);
	sTime = txt;
	static CString oldTime = "";
	
	main->m_MJPGList.SetUnitText(1, sTime, isDraw);//年月日
	main->m_MJPGList.SetUnitFont(1,font_18);
	main->m_MJPGList.SetUnitColor(1,font_white,isDraw);
	oldTime = sTime;
	sprintf(txt, "%s", Data::LanguageResource::Get(Data::RI_COMN_SUNSTC+nWeekDay).c_str());
	main->m_MJPGList.SetUnitText(3, txt, isDraw);//星期几
	main->m_MJPGList.SetUnitFont(3,font_18);
	main->m_MJPGList.SetUnitColor(3,font_white,isDraw);
	sTime = txt;
	
}

//add function
//获取未接电话个数
int	CMainDlg::GetMissedTelInfo()
{
	//todo :: Missed finish!
	return 3;
}
//获取留言电话个数
int	CMainDlg::GetRecordedTelInfo()
{
	//todo :: Missed finish!
	return 2;
}

//获取是否设置启动防火墙
BOOL CMainDlg::GetCallWallInfo()
{
	//todo :: Missed finish!
	return TRUE;
}

void CMainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting message
}

int CMainDlg::SetPhotoList()
{
	m_PhotoList.clear();
	CString sDir = "\\flashdrv\\my_show\\";
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
		//		Dprintf ("First file name is %s\n", FindFileData.cFileName);
		int i = wcstombs( filename, FindFileData.cFileName, 128);
		filename[i] = '\0';
		if(strstr(filename, ".bmp")||strstr(filename, ".BMP") || strstr(filename, ".jpg")||strstr(filename, ".JPG"))
		{
			//对文件的大小经行限制 < 1024K
			if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1024*1024)
			{
				CString s = "\\flashdrv\\\\my_show\\";
				s += FindFileData.cFileName;
				m_PhotoList.push_back(s);
			}
		}
	
		//以下是循环使用FindNextFile函数来查找文件
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			//			Dprintf ("Next file name is %s\n", FindFileData.cFileName);
			i = wcstombs( filename, FindFileData.cFileName, /*wcslen(FindFileData.cFileName)*/128);
			filename[i] = '\0';
			if(strstr(filename, ".bmp")||strstr(filename, ".BMP") || strstr(filename, ".jpg")||strstr(filename, ".JPG"))
			{
				//对文件的大小经行限制 < 500K
				if(((FindFileData.nFileSizeHigh * MAXDWORD) + FindFileData.nFileSizeLow) < 1200*1024)
				{
					CString s = "\\flashdrv\\my_show\\";
					s += FindFileData.cFileName;
					m_PhotoList.push_back(s);
				}
			}
		}
		
		DWORD dwError = GetLastError();
		if (dwError == ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);//关闭查找句柄
		} 
		else 
		{
			//			Dprintf ("FindNextFile error. Error is %u\n", dwError);
			return 0;
		}
	}
	return 1;	
}

void CMainDlg::OnTimer(UINT nIDEvent)
{	
#ifdef _DEBUG
	
#else
	if(nIDEvent == 4)
	{
		extern BOOL watchdog_MFC;
		watchdog_MFC = TRUE;
	}
	
#endif
	extern BOOL g_IsRun;
    if(!g_IsRun)
		g_IsRun = TRUE;
// 	extern BOOL g_bAdjustPanel;
// 	if(g_bAdjustPanel)
//		g_bAdjustPanel = FALSE;

	
	if(nIDEvent == 1)
	{
		SetDateTime(TRUE);
		SetUpInfo(TRUE);
	}
	else if(nIDEvent == 2)//右边的广告区
	{

	}

	
	CDialog::OnTimer(nIDEvent);
}

void CMainDlg::OnChangeWindow(WPARAM w, LPARAM l)
{
	if(l == SW_HIDE)
	{
		if((CWnd *)w != m_mainmenuDlg_)
		{
			if(w != 0xFFFFFFFF)
				((CWnd *)w)->ShowWindow(SW_HIDE);
			m_mainmenuDlg_->SetVideo();
			if((CWnd *)w != (CWnd *)m_mainScreenSaveDlg_ || (m_currentWnd == m_mainmenuDlg_ && IsWindowVisible()))
			{
				if((CWnd *)w == (CWnd *)m_mainScreenSaveDlg_)
					m_mainmenuDlg_->ShowWindow_(SW_SHOW, 1);
				else
					m_mainmenuDlg_->ShowWindow_(SW_SHOW);
		//		m_currentWnd = (CWnd *)m_mainmenuDlg_ ;
			}
			else if((CWnd *)w == (CWnd *)m_mainScreenSaveDlg_ && m_currentWnd == m_mainPhotoDlg_ )
			{
				//将playDlg设置成photoDlg
				((CMultimediaPhoneDlg *)GetParent())->playerimage_->SetOwner(m_mainPhotoDlg_->playerDlg_);
			}
			else if((CWnd *)w == (CWnd *)m_mainScreenSaveDlg_)   //不回到主界面
			{
				m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
			//	return;
			}

			//lxz 0531
	    	m_mainmenuDlg_->ShowWindow_(SW_HIDE);

			m_mainVideoDlg_->playerDlg_->ShowWindow(SW_HIDE);
			m_mainVideoDlg_->ShowWindow(SW_HIDE);
			m_mainPhotoDlg_->playerDlg_->StopTimer();
			m_mainPhotoDlg_->playerDlg_->ShowWindow(SW_HIDE);
		    m_mainPhotoDlg_->ShowWindow(SW_HIDE);
			m_mainMp3Dlg_->ShowWindow(SW_HIDE);
			m_mainLunarderDlg_->ShowWindow(SW_HIDE);
			m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
			m_mainCalucaterDlg_->ShowWindow(SW_HIDE);
			m_pWebDialog->ShowWindow_(SW_HIDE);	
			m_MJPGList.ShowWindow(SW_SHOW);
			
			if((CWnd *)w != (CWnd *)m_mainScreenSaveDlg_ && (w != 6))    //屏保不用刷新，下边的状态条
				SetStatusAll(FALSE);
		}
	}
	else if(l == SW_SHOW)
	{
		/*
		if(m_currentWnd == m_mainVideoDlg_)
			m_mainVideoDlg_->OnExit_();
		else if(m_currentWnd != m_mainScreenSaveDlg_ )
			m_currentWnd->ShowWindow(SW_HIDE);
		*/
		m_mainmenuDlg_->ShowWindow_(SW_HIDE);
		if(w == 0)	//显示桌面影院
		{
			//m_mainVideoDlg_->SetVideo("/flashdrv/my_video/playlist.pls");
			//m_mainVideoDlg_->ShowWindow(SW_SHOW);
			//m_currentWnd = (CWnd *)m_mainVideoDlg_ ;

			m_mainVideoDlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainVideoDlg_;

		}
		else if(w == 1)	//显示Photo
		{
			//	m_mainPhotoDlg_->SetPhoto();
			//	m_mainPhotoDlg_->ShowWindow(SW_SHOW);
			
			//wangzhenxing20091011
			m_mainPhotoDlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainPhotoDlg_ ;
		}
		else if(w == 2)	//显示Mp3
		{
			m_mainMp3Dlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainMp3Dlg_ ;
		}
		else if(w == 3)		//显示日历
		{
			m_mainLunarderDlg_->ShowWindow(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainLunarderDlg_ ;
		}
		else if(w == 4)
		{
			m_mainCalucaterDlg_->ShowWindow_(SW_SHOW);
			m_currentWnd = (CWnd *)m_mainCalucaterDlg_ ;
		}
		else if(w == 5)	//显示屏保
		{
			//0530 lxz
			//	m_pWebDialog->SetPlayList(_T("\\storagecard\\"), 1);
			//	m_pWebDialog->ShowWindow(SW_SHOW);
			//	m_mainScreenSaveDlg_->ShowWindow(SW_SHOW);
			//	StartWeb();
		}
		else if(w == 6) //显示屏保
		{
			//lxz 20090207
			if(m_currentWnd == m_mainVideoDlg_)
				m_mainVideoDlg_->OnExit_();
			else if(m_currentWnd == m_mainPhotoDlg_)
				m_mainPhotoDlg_->OnExit_();

			m_mainScreenSaveDlg_->ShowWindow(SW_SHOW);
		}
	}
}

void CMainDlg::OnPlayPhoto(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(m_mainPhotoDlg_->IsWindowVisible())
	{	
	//	if(!main->playerimage_->type_)		//图片
		{
			if(w == 2)	//全屏播放
			{
				if(main->playerimage_->isPlaying_ )
				{
					m_mainPhotoDlg_->SetAllScreenPlayer(TRUE);
				}
			}
			else if(w == 3) //恢复播放
			{
				if(main->playerimage_->isPlaying_)
				{
					m_mainPhotoDlg_->SetAllScreenPlayer(FALSE);
				}
			}
		}
	}
	else if(m_mainScreenSaveDlg_->IsWindowVisible())
	{
		if(w == 3)	//退出屏保
		{
			if(main->playerimage_->isPlaying_ )
			{
				m_mainScreenSaveDlg_->OnExit();
			}
		}
	}
}

void CMainDlg::SetTimer_(BOOL flag)
{
	if(flag)
	{
		//开启定时器
		SetTimer(1, 1000, NULL);
		SetTimer(2, 5000, NULL);
	}
	else
	{
		KillTimer(1);
		KillTimer(2);
	}
}
void CMainDlg::OnPlayVideo(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	{

		if(w == 0  )//暂停视频
		{
			//视频暂停
			if(main->playervideo_->isPlaying_)
				m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
			if(main->playerimage_->isPlaying_)
				m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
			if(m_mainPhotoDlg_->IsWindowVisible())
				m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 0);
		}
		else if(w ==  1)//播放视频
		{
			{
				if(m_mainMp3Dlg_->IsWindowVisible())
				{
					//main->playeraudio_->InitPlayer();
				}
				else if(m_mainVideoDlg_->IsWindowVisible())
				{
					m_mainVideoDlg_->SendMessage(WM_OUTEVENT, 0, 1);						
				}
				else if(m_mainPhotoDlg_->IsWindowVisible())
				{
					m_mainPhotoDlg_->SendMessage(WM_OUTEVENT, 0, 1);						
				}
			}
		}
		else if(w == 2)	//全屏播放
		{
			if(main->playervideo_->isPlaying_)
			{
				main->playervideo_->SetAllScreenPlayer(TRUE);
			}
			if(main->playerimage_->isPlaying_)
				main->playerimage_->SetAllScreenPlayer(TRUE);
		}
		else if(w == 3)	//退出全屏
		{
			if(m_mainScreenSaveDlg_->playerDlg_->player_->owner_ == m_mainScreenSaveDlg_->playerDlg_ && m_mainScreenSaveDlg_->playerDlg_->player_->isPlaying_)
				m_mainScreenSaveDlg_->SendMessage(WM_OUTEVENT, 0, 0);
			if(main->playervideo_->isPlaying_)
			{
				main->playervideo_->SetAllScreenPlayer(FALSE);
			}
			if(m_mainPhotoDlg_->playerDlg_->player_->owner_ == m_mainPhotoDlg_->playerDlg_  && main->playerimage_->isPlaying_)
				m_mainPhotoDlg_->SetAllScreenPlayer(FALSE);
		}
		else if(w == 4 ) //文件开始播放
		{
			//有问题
			/*
			if(m_mainVideoDlg_->IsWindowVisible() )//|| m_mainVideoDlg_->IsWindowVisible())	//视频
			{
				m_mainVideoDlg_->StartOpenNewFile();
			}
			else if(m_mainMp3Dlg_->IsWindowVisible())
			{
				m_mainMp3Dlg_->StartOpenNewFile();
			}
			*/
		}
	}
}

void CMainDlg::OnStaticClick(WPARAM w, LPARAM l)
{
	if(w == IDC_STATIC_CALLWALL)		//防火墙
	{
		m_firewalDlg_->OnStart();
		m_firewalDlg_->ShowWindow(TRUE);
	}
	else if(w == IDC_STATIC_TELSTATUS)	//未接电话、留言电话
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
		//todo: 进入未读电话记录
		main->m_pContactInfoDlg->ShowUnconnectItems();
		main->m_pContactInfoDlg->ShowWindow(SW_SHOW);
		//lxz 20080617
	//	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pTelephoneDlg->ClearUnconnectCount();
	}
	else if(w == IDC_STATIC_NOTETATUS)		//今天是否有记事
	{	
		SYSTEMTIME curtime;
		GetLocalTime(&curtime);
//		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_mainLunarderDlg1_->SetIn();		//是否需要暂停视频播放
//		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_mainLunarderDlg1_->SetData(curtime.wYear, curtime.wMonth, curtime.wDay);
//		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->SwitchPanel_(IDC_BUTTON_MAINNOTE);
	}

}

void CMainDlg::OnShowNoteStatic()
{
	/*
	SYSTEMTIME curtime;
	GetLocalTime(&curtime);
	CString s;
	if(!m_mainLunarderDlg_->m_lunarder.IsHaveNote(curtime.wYear, curtime.wMonth, curtime.wDay, s))
	{
		s = Data::LanguageResource::Get(Data::RI_MAIN_NONOTE).c_str(); 
	}
	m_TodayNoteStatic.SetWindowText(s);
	*/
}

void CMainDlg::OnShowCallWallStatic()
{
	/*
	CString s;
	int type = m_firewalDlg_->GetFirewallType();
	if(type == -1)
	{
		s = Data::LanguageResource::Get(Data::RI_MAIN_PROTECTEDWALL_CLOSE_STC).c_str();
	}
	else
	{
		char txt[128];
		const char *strtemp[] = {\
		Data::LanguageResource::Get(Data::RI_FIREWALL_BLACKLIST).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_CONTACT).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_VIP).c_str(),\
		Data::LanguageResource::Get(Data::RI_FIREWALL_ALL).c_str()\
		};
		//sprintf(txt, "%s(%s)", Data::LanguageResource::Get(Data::RI_MAIN_PROTECTEDWALL_OPEN_STC).c_str(), strtemp[type]);
		sprintf(txt, "%s", strtemp[type]);
		s = txt;
	}
	m_CallWallStatic.SetWindowText(s);
	*/
}

void CMainDlg::OnShowTelStatusStatic(WPARAM wParam, LPARAM lParam)
{
	//todo: 获取未接电话条数
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	if(wParam == 0)
	{
		m_nUnTel = main->m_pTelephoneDlg->GetUnconnectCount();
		m_nLeaveSound = main->m_pTelephoneDlg->GetRecordCount();

	}
	else if(wParam == 1)    //通话记录
	{
		if(lParam == 0)     //清空
		{
			m_nUnTel = 0;
		}
		else if(lParam == 1)   //增加一条
		{
			m_nUnTel++;
		}
		else if(lParam == 2)   //减少一条
		{
			m_nUnTel--;
			if(m_nUnTel < 0)
				m_nUnTel = 0;
		}
	}
	else if(wParam == 2)	//语音记录
	{
		if(lParam == 0)     //清空
		{
			m_nLeaveSound = 0;	
		}
		else if(lParam == 1)   //增加一条
		{
			m_nLeaveSound++;
		}
		else if(lParam == 2)   //减少一条
		{
			m_nLeaveSound--;
			if(m_nLeaveSound < 0)
				m_nLeaveSound = 0;
		}
	}

	else  if(wParam == 3)    //短信
	{
		std::string filter;
			
		filter = "[group] = " + Util::StringOp::FromInt(Data::Message::gReceive);
		filter += " AND state = ";
		filter += Util::StringOp::FromInt(Data::Message::sNoRead);
		smsFileresult = Data::Message::GetFromDatabase(filter); 
		m_nSMSCount = smsFileresult.size();
		
		filter = "[type] = " + Util::StringOp::FromInt(Data::MMSData::tpReceive);
		filter += " AND isRead = ";
		filter += Util::StringOp::FromInt(0);
		mmsFileresult = Data::MMSData::GetFromDatabase(filter);
		m_nMMSCount = mmsFileresult.size();
		m_nUnSMS = m_nSMSCount + m_nMMSCount;
	}
	
}

LRESULT CMainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case WM_TELNOTIFY:
		OnShowCallWallStatic();
		OnShowNoteStatic();
		OnShowTelStatusStatic(wParam, lParam);
		break;
	case WM_FIRENOTIFY:
		OnShowCallWallStatic();
		break;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CMainDlg::ShowWindow_(int nCmdShow)
{
	SetTimer_(FALSE);

	//主界面来回切换 main不显示bug
	if(m_currentWnd == m_mainmenuDlg_ /* && nCmdShow == SW_SHOW*/)
	{
//		m_mainmenuDlg_->ShowWindow_(nCmdShow);
	}
	else if(m_currentWnd == m_mainVideoDlg_)
	{
		HWND hWnd = ::FindWindow(L"csplayer_win0", L"csplayer window0");
		BOOL flag2 = ::IsWindowVisible(hWnd);
		if(flag2 && nCmdShow == SW_SHOW)
			m_mainVideoDlg_->playerDlg_->ShowWindow(nCmdShow);
		else if(nCmdShow == SW_HIDE)
			m_mainVideoDlg_->playerDlg_->ShowWindow(nCmdShow);
		m_mainVideoDlg_->ShowWindow(nCmdShow);
	}
	else if(m_currentWnd == m_mainPhotoDlg_)
	{
		m_mainPhotoDlg_->playerDlg_->ShowWindow(nCmdShow);
		m_mainPhotoDlg_->ShowWindow(nCmdShow);
	}

	 
	DWORD dwStart   =   GetTickCount();
		
	ShowWindow(nCmdShow);


	//显示 MJPG
	if(nCmdShow == SW_SHOW)
	{
		m_mainmenuDlg_->ShowWindow_(SW_HIDE);
		m_mainVideoDlg_->ShowWindow(SW_HIDE);
		m_mainPhotoDlg_->playerDlg_->ShowWindow(SW_HIDE);
		m_mainPhotoDlg_->ShowWindow(SW_HIDE);
		m_mainMp3Dlg_->ShowWindow(SW_HIDE);
		m_mainLunarderDlg_->ShowWindow(SW_HIDE);
		m_mainScreenSaveDlg_->ShowWindow(SW_HIDE);
		m_mainCalucaterDlg_->ShowWindow(SW_HIDE);
		m_pWebDialog->ShowWindow_(SW_HIDE);	
		((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_HIDE);
		m_MJPGList.ShowWindow(SW_SHOW);
	}
	SetTimer_(TRUE);
}

void CMainDlg::SetMainMenu()
{
	((CMultimediaPhoneDlg *)theApp.m_pMainWnd)->m_pNotebookDlg->ShowWindow(SW_HIDE);
	m_firewalDlg_->ShowWindow(SW_HIDE);
	if(m_currentWnd == m_mainVideoDlg_)
		m_mainVideoDlg_->OnExit_();
	else if(m_currentWnd == m_mainMp3Dlg_)
		m_mainMp3Dlg_->OnExit_(FALSE);
	else if(m_currentWnd == m_mainPhotoDlg_)
		m_mainPhotoDlg_->OnExit_();
	else if(m_currentWnd == m_mainLunarderDlg_)
	{
		m_mainLunarderDlg_->OnExit_();
		ShowWindow(SW_SHOW);
	}
	else if(m_currentWnd == m_mainCalucaterDlg_)
		m_mainCalucaterDlg_->OnExit_();
//	else if(m_currentWnd == m_pWebDialog)
//	{
//		m_pWebDialog->OnButtonClose();
//	}
	else if(m_MJPGList.IsWindowVisible())
	{
		//	CString s = ".\\adv\\mjpg\\k1\\中文\\桌面.xml";
		//	m_MJPGList.SetCurrentLinkFile(s);
		//	SetDateTime(FALSE);
		//	Invalidate();
		//m_MJPGList.DrawMJPGPage_HDC(s);
	}
}

void CMainDlg::OnClickMJPGToApp(WPARAM w, LPARAM l)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
	CString icon;
	switch (w)
	{
	case 1:	//电话
			if(main->m_pFSM->getCurrentState() != CMultimediaPhoneDlg::tsHangOff)
			{
				main->m_pTelephoneDlg->ShowWindow_(TRUE);
				icon = Allicon[0];
			}
			else
			{	
		
		//		extern void GNotifyDial(BOOL isDial);
		//		GNotifyDial(1);
				main->m_pTelphoneDialDlg->m_bSoftware = true ;
				main->SendMessage(WM_TEL_HUNGON);

			}
			//icon = L".\\adv\\mjpg\\k5\\common\\电话\\电话图标.bmp";
			break;

	case 2:	//信息
			m_p3GSMSDlg->ShowWindow_(SW_SHOW);
			icon = Allicon[1];
			break;

	case 3:	//通讯录
			main->m_pContactDlg->ShowRightBtn(true);
			main->m_pContactDlg->ShowWindow_();
			icon = Allicon[2];

			break;

	case 4:	//通话记录
			main->m_pContactInfoDlg->ShowRightBtn(true);
			main->m_pContactInfoDlg->ShowWindow_();
			icon = Allicon[3];
			break;

	case 5:	//音乐
			m_p3GHomeJoyDlg->ShowWindow_(SW_SHOW);
			icon = Allicon[4];
			break;

	case 6:	//家庭影院								
			m_p3GHomeMovieDlg->ShowWindow_(SW_SHOW);			
			icon = Allicon[5];
			break;

	case 7:	//家庭相册
			m_p3GHomePicDlg->ShowWindow_(SW_SHOW);
			icon = Allicon[6];
			break;

	case 8:	//计算器
			m_mainmenuDlg_->OnCalculBtn();	//发消息
			icon = Allicon[7];
			break;

	case 9:	//日程提醒
			//	m_mainmenuDlg_->OnLunarderBtn();
			m_mainLunarderDlg_->ShowWindow_(SW_SHOW);
			icon = Allicon[8];
			break;

	case 10: //设置
			main->m_pSettingDlg->ShowWindow_(SW_SHOW);
			icon = Allicon[9];		
			break;

	case 11: 
			{
				m_mainmenuDlg_->OnCalculBtn();
			}
			break;

	case 12:
			{
			main->m_pSettingDlg->ShowWindow(SW_SHOW);
			}
			break;

	default:
		break;
	}

	if (!icon.IsEmpty())
	{
		main->AddDesktopBtn();
		main->AddIcon(icon);//添加图标	
	}

	m_MJPGList.m_nIndexSelectUnit = -1;

}

void CMainDlg::ShowRightCtrl(int nCmdShow)
{
	/*
	m_TimeStatic.ShowWindow(nCmdShow);
	m_TodayNoteStatic.ShowWindow(nCmdShow);
	m_CallWallStatic.ShowWindow(nCmdShow);
	m_TelStatusStatic.ShowWindow(nCmdShow);
	*/
}

void CMainDlg::OnMJPGShowHalf(WPARAM w, LPARAM l)
{
	if(w == 0)
		ShowRightCtrl(SW_HIDE);
	else
		ShowRightCtrl(SW_SHOW);

}

void CMainDlg::OnSetAdmin(WPARAM w, LPARAM l)
{
	if(w == 1)
	{
		m_MJPGList.SetUnitIsDownStatus(170, TRUE);
		SetStatusAll(FALSE);
		((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin = TRUE;
	}
}

void CMainDlg::OnUnSetAdmin(WPARAM w, LPARAM l)
{
	m_MJPGList.SetUnitIsDownStatus(170, FALSE);
	SetStatusAll(FALSE);
	((CMultimediaPhoneDlg*)theApp.m_pMainWnd)->m_pSettingDlg->m_bLogin = FALSE;
}

void  CMainDlg::StartWeb()
{
	MEMORYSTATUS   mem;     
	mem.dwLength=sizeof(mem);     
	GlobalMemoryStatus(&mem);     
//     if( mem.dwAvailPhys < 32*1024*1024)
// 	{
// 		return;
// 	}

	if(m_pWebDialog)
	{
		CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
		main->CancelBalckLightSaveTimer();
		m_pWebDialog->ShowWindow_(SW_SHOW);
	}

// 	PROCESS_INFORMATION processInfo;
// 	STARTUPINFO lpStartupInfo; // 用于在创建子进程时设置各种属性 
// 	memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
// 
// 	lpStartupInfo.cb = sizeof(lpStartupInfo);
// 	lpStartupInfo.dwX = 0;
// 	lpStartupInfo.dwY = 0;
// 	lpStartupInfo.dwXSize = 800;
// 	lpStartupInfo.dwYSize = 420;
// 	lpStartupInfo.wShowWindow= SW_MINIMIZE; 
// //	lpStartupInfo.dwFlags= 1|2|4;
// 	
// 
// //	memset(&processInfo, 0, sizeof(processInfo));
// 	if (!CreateProcess(L"\\windows\\iesample.exe", L"http://www.google.com", NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &lpStartupInfo, &processInfo))
// 	{
// 	//	GetWindowThreadProcessId
// 	//	::FindWindow()
// 	}
// 	HWND hwnd = ::FindWindow(NULL, L"窗口标题");
// //	HWND hwnd = ::GetWin(processInfo.dwProcessId);
//   //  Sleep(1000); 
//     ::MoveWindow(hwnd, 0,  0,   800,   420,   TRUE); 
// 
// 
// 	CloseHandle(processInfo.hThread);
// 	CloseHandle(processInfo.hProcess);
// 	return;

	/*

	PROCESS_INFORMATION processInfo;
// 	CString exeFilename = "\\windows\\presviewer.exe";
// 	CString filename = "\\storagecard\\礼仪.ppt";
	CString exeFilename = "\\windows\\docviewer.exe";
	CString filename = "\\storagecard\\上机测试题.doc";

	STARTUPINFO lpStartupInfo; // 用于在创建子进程时设置各种属性 
	memset(&lpStartupInfo, 0, sizeof(lpStartupInfo));
	lpStartupInfo.cb = sizeof(lpStartupInfo);
	lpStartupInfo.dwX = 20;
	lpStartupInfo.dwY = 20;
	lpStartupInfo.dwXSize = 200;
	lpStartupInfo.dwYSize = 200;
	lpStartupInfo.wShowWindow=SW_SHOW; 
	lpStartupInfo.dwFlags= 1|2|4;
	
	if (!CreateProcess(exeFilename, filename, NULL, NULL, NULL, CREATE_NEW_CONSOLE, NULL, NULL, &lpStartupInfo, &processInfo))
	{
		printf("应用程序启动失败，请原谅给您带来的不便，请尽快联系设备供应商!\n");
		//::MessageBox(NULL, _T("启动程序失败，请稍后再试……"), _T("程序启动失败"), 0);
	}
	CloseHandle(processInfo.hThread);
	CloseHandle(processInfo.hProcess);
	*/
}
