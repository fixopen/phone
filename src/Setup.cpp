#include "sysSetup.h"
#include "pswd.h"
#include "pendrv.h"
#include "ring.h"
#include "phoneapp.h"
#include "sms.h"
//
//const char SystemVersion[] = "2002/1/4, V1.81";

////////////////// history /////////////////////
//
//	2001/12/06
//		hqf, modify CSystemSetup::OnDestroy() 
//	2001/12/06
//		byw, modify CSystemSetup::ResetSystemOption()
//  2001/12/06 
//		hqf, modify CAutoShutDown::PaintOther()
//  2001/12/10 
//		hqf, modify CSystemSetup::ResetSystemOption()
//	byw, 2001/12/25
//		modify void CSystemSetup::SetSystemTime()
//		modify void CSystemSetup::SetSystemDate()
//		modify void COutLine::OnPaint()


int   g_iEnterScrTime = 0;
void ConvStrToPswd(char *strSrc, char *strPswd)//把字符串都转成'*'
{
	short iLen = strlen(strSrc);
	for(short i = 0;i< iLen; i++)
		strPswd[i] = '*';
	strPswd[iLen] = '\0';
}


//***************下面是系统设置主界面窗口的定义*********************//
#define     IDC_BTN_TIME            3499
#define     IDC_BTN_DATE            3500
#define     IDC_BTN_RECORD          3501
#define     IDC_BTN_RING            3502
#define     IDC_BTN_FORBIDDIAL      3503
#define     IDC_BTN_OUTLINE         3504
#define     IDC_BTN_PASSWORD        3505
#define     IDC_BTN_NOTBOTHER       3506
#define     IDC_BTN_IP              3507
#define     IDC_BTN_AGENT           3508
#define     IDC_BTN_AUTOSHUT        3509
#define     IDC_BTN_MEMORY          3510
#define     IDC_BTN_ADJUST          3511


/*
*	变量定义
*/
SYSTEM_OPTION g_SystemOption;

BOOL g_bOutWndDel = FALSE;
BOOL g_bAutoShutWndDel = FALSE;

BEGIN_MESSAGE_MAP(CSystemSetup, CAppWnd)
  ON_MESSAGE_RANGE(IDC_BTN_TIME, IDC_BTN_ADJUST, CN_COMMAND, OnBtnCmd)
END_MESSAGE_MAP 

CSystemSetup::CSystemSetup()
{
	m_pDateTimeWnd = NULL;	
	m_pRecordWnd = NULL;
//	m_pPswdWnd = NULL;
	m_pRingWnd = NULL;
	m_pForbidWnd = NULL;
	m_pOutLineWnd= NULL;
	m_pOnlyAccept = NULL;
	m_pIPWnd = NULL;
	//m_pCallId = NULL;
	m_pAutoShut = NULL;
	m_pMemInfo = NULL;
	m_pAgentInfo = NULL;
}

CSystemSetup::~CSystemSetup()
{
}

// temp, debug
//extern void tempecho(int num);

void  CSystemSetup::ResetSystemOption()
{

	LoadSystemOption(&g_SystemOption);

	/* 
	*	初始化默认值
	*/
	if (g_SystemOption.nSetupErasedFlag != 0x55AA00FF){
		
		memset(&g_SystemOption ,0 , sizeof(SYSTEM_OPTION));
		g_SystemOption.nSetupErasedFlag = 0x55AA00FF;
		
		g_SystemOption.bPhoneWndPlayAni = 1;	// 使用电话窗口动画
		g_SystemOption.nNoInterrept = 0;		// 不使用免打扰
		//g_SystemOption.strOnlyAcceptTel[ONLYACCEPT_COUNT][21];
		g_SystemOption.nDefaultRingID = 0;
		g_SystemOption.nHardwareRingVolume = RINGVOLUME_HIGH;
		//g_SystemOption.strOutLineHead[OUTLINE_HEAD_LEN + 1];
		//g_SystemOption.strLimitNumber[FORBID_COUNT][PHONE_NUMBER_INPUT_LEN + 1];
		g_SystemOption.nRecordRingCount = 10000;
		g_SystemOption.nDefaultIP = 0;          //默认IP帐号的索引
		//g_SystemOption.iiDialNum[DIALRULE_COUNT];
		//g_SystemOption.LocalAreaCode[8];
		g_SystemOption.KxNom = -139;
		g_SystemOption.KxDenom = -1167;
		g_SystemOption.KyNom = -139;
		g_SystemOption.KyDenom = 1054;
		g_SystemOption.Dx = -76;
		g_SystemOption.Dy = 185;
		g_SystemOption.PowerDownTime = 0;
		g_SystemOption.sTimeType = 1;           // 24小时制
		//g_SystemOption.strScreenPassword[9];  //屏保密码
		//g_SystemOption.Version[128];			// 版本信息(包括软件版本，硬件版本，安装日期，代理商代号)

		// 初始化IP帐号
		for(int i = 0; i< DIALRULE_COUNT; i++) 
		{
			strcpy(g_SystemOption.iiDialNum[i].strPostfix0, "1");
			strcpy(g_SystemOption.iiDialNum[i].strPostfix1, "#");
			strcpy(g_SystemOption.iiDialNum[i].strPostfix2, "#");
			strcpy(g_SystemOption.iiDialNum[i].strPostfix3, "#");
		}

		// 初始化版本信息
		strcpy(g_SystemOption.Version, "SVer 1.87");
		
		//初始化安装日期、代理商信息等
		g_SystemOption.SetupDate.year = 2002 , g_SystemOption.SetupDate.month = 3;
		g_SystemOption.SetupDate.day = 1;
//		::GetSysTime(&g_SystemOption.SetupDate);
		strcpy(g_SystemOption.SeriesNumber, "1111-11111111");
		strcpy(g_SystemOption.VendorInfo, "试用版");
		
		/////初始化短消息
//		extern void InitSMSApp();
//		InitSMSApp ( );

		//////////////// SMS ///////////////////////

		SaveSystemOption ( &g_SystemOption );
	}
	
}

BOOL CSystemSetup::OnDestroy()
{
	/*
	*	Save settings to FLASH
	*/
//	SaveSystemOption(&g_SystemOption);

	if(m_pDateTimeWnd)
	{
		delete m_pDateTimeWnd;
		m_pDateTimeWnd = NULL;
	}
	if(m_pRecordWnd)
	{
		delete m_pRecordWnd;
		m_pRecordWnd = NULL;
	}
	if(m_pRingWnd)
	{
		delete m_pRingWnd;
		m_pRingWnd = NULL;
	}
	if(m_pForbidWnd)
	{
		delete m_pForbidWnd;
		m_pForbidWnd = NULL;
	}
	if(m_pOutLineWnd)
	{
		delete m_pOutLineWnd;
		g_bOutWndDel = TRUE;
		m_pOutLineWnd = NULL;
	}
	if(m_pOnlyAccept)
	{
		delete m_pOnlyAccept;
		m_pOnlyAccept = NULL;
	}
	if(m_pIPWnd)
	{
		delete m_pIPWnd;
		m_pIPWnd = NULL;
	}
	if(m_pAgentInfo)
	{
		delete m_pAgentInfo;
		m_pAgentInfo = NULL;
	}
/*	
	if(m_pCallId)
	{
		delete m_pCallId;
		m_pCallId = NULL;
	}
*/
	if(m_pAutoShut)
	{
		delete m_pAutoShut;
		g_bAutoShutWndDel = TRUE;
		m_pAutoShut = NULL;
	}
	if(m_pMemInfo)
	{
		delete m_pMemInfo;
		m_pMemInfo = NULL;
	}
	::SetCurrentApp();
	return CAppWnd::OnDestroy();
}
BOOL CSystemSetup::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(m_sRect.left +8 , m_sRect.top +22 , m_sRect.left + 74 , m_sRect.top + 39);
	m_oBtnDate.Create("时间" , r, this , IDC_BTN_TIME);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnPswd.Create("留言" , r, this , IDC_BTN_RECORD);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnForbidDial.Create("禁拨" , r, this , IDC_BTN_FORBIDDIAL);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnNotBother.Create("只接电话" , r, this , IDC_BTN_NOTBOTHER);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnAdjust.Create("手写校正" , r, this , IDC_BTN_ADJUST);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnMemory.Create("内存使用" , r, this , IDC_BTN_MEMORY);

	r.left = r.right + 10 , r.right = r.left + 66;
	r.top = m_sRect.top + 22 , r.bottom = m_sRect.top + 39;
	m_oBtnTime.Create("日期" , r, this , IDC_BTN_DATE);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnRing.Create("振铃" , r, this , IDC_BTN_RING);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnOutLine.Create("外线区号" , r, this , IDC_BTN_OUTLINE);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnIP.Create("IP电话" , r, this , IDC_BTN_IP);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnAutoShut.Create("屏幕保护" , r, this , IDC_BTN_AUTOSHUT);
	r.top = r.bottom + 6 , r.bottom = r.top + 17;
	m_oBtnAgent.Create("关于..." , r, this , IDC_BTN_AGENT);
//	r.top = r.bottom + 6 , r.bottom = r.top + 17;
//	m_oBtnDisp.Create("系统密码" , r, this , IDC_BTN_PASSWORD);
	return TRUE;
}

void CSystemSetup::SetSystemTime()
{
/*	if(!m_pDateTimeWnd)
		m_pDateTimeWnd= new CDateTimeSet;
	m_pDateTimeWnd->Create("日期时间", WS_VISIBLE|WS_CAPTION , DESK_TOP_RECT , 0);
	m_pDateTimeWnd->ShowWindow();*/
	INT16		nHour , nMinute, nType;
	DateTime	SysTime;
	
	::GetSysTime(&SysTime);
	nHour = SysTime.hour;
	nMinute = SysTime.minute;
	nType = g_SystemOption.sTimeType;
	if(::TimeInputBox("时间设置", nHour ,nMinute,nType, HELP_SETUP_TIME)==IDOK)
	{
		SysTime.hour = nHour , SysTime.minute = nMinute, SysTime.second = 0;
		::SetSysTime(&SysTime);
		g_SystemOption.sTimeType = nType;
		extern void Alarm_SetAlarm(); // added 12 03
		Alarm_SetAlarm();
	}
}

void CSystemSetup::SetSystemDate()
{
	INT16		nYear, nMonth, nDay;
	DateTime	SysTime;
	
	::GetSysTime(&SysTime);
	nYear = SysTime.year;
	nMonth = SysTime.month;
	nDay = SysTime.day;
	if(::DateInputBox("日期设置", nYear ,nMonth, nDay,HELP_SETUP_DATE)==IDOK)
	{
		SysTime.year = nYear , SysTime.month = nMonth;
		SysTime.day = nDay;
		::SetSysTime(&SysTime);
		extern void Alarm_SetAlarm(); // added 12 03
		Alarm_SetAlarm();// 可能有问题
	}
}

void CSystemSetup::SetRecord()
{
	if(!m_pRecordWnd)
		 m_pRecordWnd = new CRecordSet;
	 m_pRecordWnd->Create("留言设置" , WS_VISIBLE|WS_CAPTION|WS_OKBOX  , DESK_TOP_RECT , HELP_SETUP_RECORD);
	 m_pRecordWnd->ShowWindow();
}

void CSystemSetup::SetScreenPswd()
{
/*	 if(!m_pPswdWnd)
		 m_pPswdWnd = new CPswdSet;
	 m_pPswdWnd->Create("系统密码" , WS_VISIBLE|WS_CAPTION|WS_OKBOX  , DESK_TOP_RECT , 0);
	 m_pPswdWnd->ShowWindow();*/
}

void CSystemSetup::SetDefaultRing()
{
	if(!m_pRingWnd)
		m_pRingWnd = new CRingSet;
	m_pRingWnd->Create("振铃设置", WS_VISIBLE|WS_CAPTION|WS_OKBOX  , DESK_TOP_RECT , HELP_SETUP_RING);
    m_pRingWnd->ShowWindow();
}

void CSystemSetup::SetForbidDial()
{
	if(!m_pForbidWnd)
		m_pForbidWnd = new CForbidDial;
	m_pForbidWnd->Create("禁拨设置", WS_VISIBLE|WS_CAPTION|WS_OKBOX  , DESK_TOP_RECT , HELP_SETUP_DIALSTOP);
	m_pForbidWnd->ShowWindow();
}

void CSystemSetup::SetOutLine()
{
	if(!m_pOutLineWnd)
		m_pOutLineWnd = new COutLine;
	m_pOutLineWnd->Create("外线区号设置", WS_VISIBLE|WS_CAPTION|WS_OKBOX , DESK_TOP_RECT , HELP_SETUP_EXTERIOR_LINE);
	m_pOutLineWnd->ShowWindow();
}

/*
void CSystemSetup::SetCallIDMode()
{
	if(!m_pCallId)
		m_pCallId = new CCallID;
	m_pCallId->Create("来电显示" ,WS_VISIBLE|WS_CAPTION|WS_OKBOX , DESK_TOP_RECT , 0);
	m_pCallId->ShowWindow();
}
*/
void CSystemSetup::SetAutoShutTime()
{
	if(!m_pAutoShut)
		m_pAutoShut = new CAutoShutdown;
	m_pAutoShut->Create("屏幕保护设置" ,WS_VISIBLE|WS_CAPTION|WS_OKBOX , DESK_TOP_RECT , HELP_SETUP_LOCK);
	m_pAutoShut->ShowWindow();
}

void CSystemSetup::ShowMemInfo()
{
	if(!m_pMemInfo)
		m_pMemInfo = new CMemoryInfo;
	m_pMemInfo->Create("内存使用状况" ,WS_VISIBLE|WS_CAPTION, DESK_TOP_RECT , HELP_SETUP_MEM);
	m_pMemInfo->ShowWindow();
}

void CSystemSetup::ShowAgentInfo()
{
	if(!m_pAgentInfo)
		m_pAgentInfo = new CAgentInfo;
#ifdef  DEBUG
/*	short id  = 0;
	HISTORY his ;
	memset(&his , 0, sizeof(HISTORY));
	strcpy(his.Name,"记录测试");
	strcpy(his.Number, "00000000") ;
	strcpy(his.AreaCode	 , "000") ;
	CARD   card = { 0 , 0, 0, 0, "测试", ""};
	CARDCASE  cc = { 0 , 0, 0, "无" , "" , 0 , ""};
	MEMO  memo = {0 , "无" , "" , "测试" , 0};
	ALARM alarm ; 
	memset(&alarm , 0, sizeof(ALARM));
	alarm.Flag = 0;
	alarm.When.day = 2000;
	alarm.When.month = 1;
	alarm.When.day = 1;
	alarm.When.hour = 0 ;
	alarm.When.minute = 1;
	alarm.When.second = 0;
	alarm.MinBef=ALARM_ONE;
	alarm.Music=1;
	strcpy(alarm.Title, "测试");
	strcpy(alarm.Todo, "");

	FASTDIAL fd = {0, 0 , 0, 0, "无" , "86241818"};
//	CONTENT content;
//	MODEL  model;

	short i = GetHisrotyUsed();
	while(id >=0 && i < 256)
	{
		id = ::AppendHistory(&his);
		i++;
	}

	id = 0;
	i = ::GetCardUsed();
	while(id >= 0 && i < 1024)
	{
		id = ::AppendCard(&card);
		i++;
	}

	id = 0;
	i = ::GetCardCaseUsed();
	while(id >= 0 && i < 128)
	{
		id = ::AppendCardCase(&cc);
		i++;
	}

	id = 0;
	i = ::GetMemoUsed();
	while(id >= 0 && i++ < 128)
		id = ::AppendMemo(&memo);

	id = 0;
	i = ::GetAlarmUsed();
	while(id >= 0 && i++ < 128)
		id = ::AppendAlarm(&alarm);

	id = 0;
	i = 0;
	while(id >= 0 && i++ < 64)
	{
		id = ::AppendFastDial(&fd);
		fd.Sort = id +1;
	}
*/	
	/*WAVEHEAD* wh = (WAVEHEAD *)WAVE_STORE_ADDR;
	memset( wh,0,sizeof(WAVEHEAD) );
	wh->Flag =WAVE_MIC; wh->Length = WAVE_STORE_LEN -4-sizeof(WAVEHEAD) ; // 录音满
	extern WAVEHEAD *	*pWaveTail;
	*pWaveTail = (WAVEHEAD*)((long)*pWaveTail + sizeof(WAVEHEAD) + *pWaveTail->Length);*/
#endif

	m_pAgentInfo->Create("关于e风智能电话" ,WS_VISIBLE|WS_CAPTION, DESK_TOP_RECT , HELP_SETUP_ABOUT);
	m_pAgentInfo->ShowWindow();
}

void CSystemSetup::SetOnlyAccept()
{
	if(!m_pOnlyAccept)
		m_pOnlyAccept = new CAcceptCall;
	m_pOnlyAccept->Create("只接电话设置", WS_VISIBLE|WS_CAPTION|WS_OKBOX , DESK_TOP_RECT , HELP_SETUP_ONLY_RING);
	m_pOnlyAccept->ShowWindow();
}

void CSystemSetup::SetIPPhone()
{
	if(!m_pIPWnd)
		m_pIPWnd = new CIPPhone;
	m_pIPWnd->Create("IP电话设置", WS_VISIBLE|WS_CAPTION|WS_OKBOX , DESK_TOP_RECT , HELP_SETUP_IP);
	m_pIPWnd->ShowWindow();
}

void CSystemSetup::OnBtnCmd(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	switch(wParam)
	{
	case IDC_BTN_TIME: //设置时间
		SetSystemTime();
		break;
	case IDC_BTN_DATE: //设置日期
		SetSystemDate();
		break;
	case IDC_BTN_RECORD://留言设置
		SetRecord();
		break;
	case IDC_BTN_PASSWORD: //屏保密码
		SetScreenPswd();
		break;
	case IDC_BTN_RING: //振铃设置
		SetDefaultRing();
		break;
	case IDC_BTN_FORBIDDIAL: //禁拨设置
		SetForbidDial();
		break;
	case IDC_BTN_OUTLINE: //外线
		SetOutLine();
		break;
	case IDC_BTN_NOTBOTHER: //只接电话
		SetOnlyAccept();
		break;
    case IDC_BTN_IP:  //IP电话设置
		SetIPPhone();
		break;
//	case IDC_BTN_DISP:   //来电显示制式
//		SetCallIDMode();
//		break;
	case IDC_BTN_AUTOSHUT: //自动关机
		SetAutoShutTime();
		break;
	case IDC_BTN_MEMORY: //内存使用状况
		ShowMemInfo();
		break;
	case IDC_BTN_ADJUST: //触摸屏校正
		CalibrationPen();
		SaveSystemOption(&g_SystemOption);
		break;
	case IDC_BTN_AGENT:
		ShowAgentInfo();
	default:
		break;
	}
}

//***************下面是时间日期设置窗口的定义部分*********************//
#define IDC_BTNTIME             3550
#define IDC_BTNDATE             3551
         
BEGIN_MESSAGE_MAP(CDateTimeSet, CAppWnd)
  ON_MESSAGE(IDC_BTNDATE, CN_COMMAND, OnDateInput)
  ON_MESSAGE(IDC_BTNTIME, CN_COMMAND, OnTimeInput)
END_MESSAGE_MAP 

BOOL CDateTimeSet::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(m_sRect.left +30, m_sRect.top+60 , m_sRect.left +123 ,  m_sRect.top +79);
	m_oBtnTime.Create("时间设置",0 , r, this , IDC_BTNTIME);
	r.top = r.bottom + 27 , r.bottom = r.top +19;
	m_oBtnDate.Create("日期设置",0 , r, this , IDC_BTNDATE);
	return TRUE;
}

void CDateTimeSet::OnTimeInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	INT16 nHour , nMinute, nType;
	DateTime SysTime;
	if(::TimeInputBox("设置时间", nHour ,nMinute,nType,HELP_SETUP_TIME)==IDOK)
	{
		::GetSysTime(&SysTime);
		SysTime.hour = nHour , SysTime.minute = nMinute, SysTime.second = 0;
		::SetSysTime(&SysTime);
		g_SystemOption.sTimeType = nType;
		extern void Alarm_SetAlarm(); // added 12 03
		Alarm_SetAlarm();
	}
}

void CDateTimeSet::OnDateInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	INT16 nYear , nMonth , nDay;
	DateTime SysTime;
	if(::DateInputBox("设置日期", nYear ,nMonth, nDay,HELP_SETUP_DATE)==IDOK)
	{
		::GetSysTime(&SysTime);
		SysTime.year = nYear , SysTime.month = nMonth;
		SysTime.day = nDay;
		::SetSysTime(&SysTime);
		extern void Alarm_SetAlarm(); // added 12 03
		Alarm_SetAlarm();
	}
}

//***************下面是留言设置窗口的定义部分*********************//
#define    IDC_COM_COUNT                         3515
#define    IDC_BTN_GREET                         3516
#define    IDC_CHECK_AUTOREC                     3517
BEGIN_MESSAGE_MAP(CRecordSet, CAppWnd)
  ON_MESSAGE(IDC_BTN_GREET, CN_COMMAND, OnRecordGreet)
  ON_MESSAGE(IDC_CHECK_AUTOREC, CN_COMMAND, OnAutoRec)
END_MESSAGE_MAP 

CRecordSet::CRecordSet()
{
	m_pGreetSet = NULL;
}

CRecordSet::~CRecordSet()
{
}

BOOL CRecordSet::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	char strRec[5];
	INT16 i;
	BOOL  bUseAutoRec;
	CRect r(m_sRect.left +4 , m_sRect.top + 30 , m_sRect.left +120, m_sRect.top + 50);
	bUseAutoRec = (g_SystemOption.nRecordRingCount > 100? FALSE : TRUE); 
	m_oBtnIsUse.Create("启用留言功能" , r, this, bUseAutoRec , IDC_CHECK_AUTOREC);
	/*if(g_SystemOption.nRecordRingCount> 100)
		sprintf(strRec , " %d" , 3);
	else
		sprintf(strRec , " %d" , g_SystemOption.nRecordRingCount +1);*/
	r.SetRect(m_sRect.left +37 , m_sRect.top + 60 , m_sRect.left +77, m_sRect.top + 79);
	m_oRingCount.Create(NULL , r , 74, this , IDC_COM_COUNT);
	for(i=4 ; i<= 8; i++)
	{
		sprintf(strRec ," %d" ,i);
		m_oRingCount.AppendItem(strRec, 0);
	}
	if(g_SystemOption.nRecordRingCount <= 8)
		m_oRingCount.SetCurSel(g_SystemOption.nRecordRingCount - 4);
	else
		m_oRingCount.SetCurSel(0);

	if(bUseAutoRec)
		m_oRingCount.SetWindowStyle(m_oRingCount.GetWindowStyle() | WS_VISIBLE);
	else
		m_oRingCount.SetWindowStyle(m_oRingCount.GetWindowStyle() & ~WS_VISIBLE);

	r.SetRect(m_sRect.left +25 , m_sRect.top + 109 , m_sRect.left +109, m_sRect.top + 128);
	m_oBtnGreet.Create("录制问候语" , r , this , IDC_BTN_GREET);
	return TRUE;
}

BOOL CRecordSet::OnDestroy()
{
	if(m_pGreetSet)
	{
		delete m_pGreetSet;
		m_pGreetSet = NULL;
	}
	return CAppWnd::OnDestroy();
}

void CRecordSet::OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_oBtnIsUse.GetValue())
	{
		CPopListItem *pItem;
		pItem = m_oRingCount.GetItem(m_oRingCount.GetCurSel()); 
//		char strTimes[3];
//		m_oRingCount.GetWindowText(strTimes , 2);
		g_SystemOption.nRecordRingCount = pItem->strText[1] - 0x31;
	}
	else
	{
		g_SystemOption.nRecordRingCount = 1000;
	}
	SaveSystemOption(&g_SystemOption);
	CAppWnd::OnOk(nMessage, wParam, lParam);
}
void CRecordSet::PaintOther(CDC &dc)
{
	if(m_oBtnIsUse.GetValue())
	{
		dc.TextOut(m_sRect.left +5 , m_sRect.top +60 , "振铃");
		dc.TextOut(m_sRect.left +80 , m_sRect.top +60 , "声提示留言");
	}
	else
	{
		CRect r(m_sRect.left +5 , m_sRect.top + 60 , m_sRect.left + 37,
                m_sRect.top + 79);
		dc.EraseRect(r);
		r.SetRect(m_sRect.left +80 , m_sRect.top +60 ,m_sRect.right , m_sRect.top +79);
		dc.EraseRect(r);
	}
}

void CRecordSet::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	dc.Reset();
	PaintOther(dc);	
}
//显示留言录制窗口
void CRecordSet::OnRecordGreet(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(!m_pGreetSet)
		m_pGreetSet = new CGreeting;
	if(!m_pGreetSet->Create("录制问候语",WS_CAPTION|WS_OKBOX, DESK_TOP_RECT , HELP_SETUP_RECORD_WELCOME))
	{
		::MessageBox("空间不足，无法录制问候语！", "录制问候语" ,MB_OK|MB_ICONINFORMATION);
		return ;
	}
	m_pGreetSet->ShowWindow();
}

void CRecordSet::OnAutoRec(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_oBtnIsUse.GetValue())
	{
		m_oRingCount.SetWindowStyle(m_oRingCount.GetWindowStyle() | WS_VISIBLE);
		m_oRingCount.OnPaint(CDC());
		PaintOther(CDC());
	}
	else
	{
		m_oRingCount.SetWindowStyle(m_oRingCount.GetWindowStyle() & ~WS_VISIBLE);
		PaintChildWnd();
		CRect r= m_oRingCount.GetWindowRect();
		CDC dc;
		dc.Reset();
		dc.EraseRect(r);
		PaintOther(dc);
	}
}

//***************下面是振铃设置窗口的定义部分*********************//
#define  IDC_COM_RING                  3530
#define  IDC_BTN_LISTEN                3531
#define  IDC_RAD_VOLUME                3532
#define  IDC_BTN_DEL	               3533

#define RING_DROPHEIGHT         72
BEGIN_MESSAGE_MAP(CRingSet, CAppWnd)
  ON_MESSAGE(IDC_BTN_LISTEN, CN_COMMAND, OnListenTest)
  ON_MESSAGE(IDC_BTN_DEL , CN_COMMAND, OnDelRing)
  ON_MESSAGE(IDC_COM_RING, CN_COMMAND, OnRingSel)
END_MESSAGE_MAP 

BOOL CRingSet::OnDelRing()
{
	short nRingID ;
	short nSel = m_oComRing.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox("普通铃声不能删除！" , "提示" , MB_OK|MB_ICONSTOP);
		return FALSE;
	}
	CPopListItem *pItem = m_oComRing.GetItem(nSel);
	nRingID = pItem->exData;
	if(pItem)
	{
		short RingIDBuf[RING_MAX_COUNT];
		int total=0;
		total=FindRing(RingIDBuf, RING_MAX_COUNT);

		//     hqf        2002-1-31
		// TODO: 搜索所有用到振铃的地方，如果正在使用，则提示不能删除
		short nUserType , nUserID;
		char strTip[64] = "";
		if(RingInUse(nRingID , &nUserType, &nUserID))
		{
			switch(nUserType)
			{
			case DATA_CARD:
				{
					CARD *pCard = ::GetCardPointer(nUserID);
					sprintf(strTip , "名片“%s”中正在使用，无法删除！" , pCard->Name);
					break;
				}
			case DATA_ALARM:
				{
					ALARM *pAlarm = GetAlarmPointer(nUserID);
					sprintf(strTip , "提醒中正在使用，无法删除！",pAlarm->Title);
					break;
				}
			case DATA_CARDCASE:
				{
					CARDCASE *pCase = GetCardCasePointer(nUserID);
					pCase->RingID = RingIDBuf[nSel != total ? nSel : 0];
					break;
				}
			}
		}
		else 
		{
			for(short i =0; i < 4; i++)
			{ 
				if(g_SystemOption.SMSStatus[i].iRingID == nRingID)
				{
					if(i != 3)
						sprintf(strTip , "短消息信箱%d中正使用，无法删除！",i+1); 
					else
						strcpy(strTip , "短消息公共信箱中正使用，无法删除！"); 
					break;
				}
			}
		}
		if(strTip[0] != '\0')
		{
			::MessageBox(strTip , "删除振铃" , MB_OK|MB_ICONSTOP);
			return FALSE;
		}

									
		// TODO: 如果等于系统振铃，则提问删除，确认后，删除之，系统振铃变为删除的下一条振铃
		if(g_SystemOption.nDefaultRingID == nRingID)
		{
			if(::MessageBox("是默认振铃，确定删除吗！", "提示",MB_YESNO|MB_ICONQUESTION)== IDNO)
				return FALSE;
			if(IsLegalWnd())
			{
				if(::DeleteRing(nRingID))
				{
					m_oComRing.DeleteItem(nSel);
					if(m_oComRing.GetTotalItems() == 1)
					{
						m_oComRing.SetCurSel(0);
						m_oComRing.OnPaint(CDC());
						::MessageBox("系统铃声变为普通铃声，所有特色振铃和只接电话都将失效！", "提示",MB_OK|MB_ICONINFORMATION);
						return TRUE;
					}
					else
					{
						if(nSel != total)
						{
							g_SystemOption.nDefaultRingID = RingIDBuf[nSel];
							m_oComRing.SetCurSel(nSel);
						}
						else
						{
							g_SystemOption.nDefaultRingID = RingIDBuf[0];
							m_oComRing.SetCurSel(1);
						}
					}
					m_oComRing.OnPaint(CDC());
					return TRUE;
				}
				else
				{
					::MessageBox("系统保留铃声，无法删除！", "提示",MB_OK|MB_ICONSTOP);
					return FALSE;
				}
			}
		}
		// DONE
		

		if(MessageBox( "真的要删除这首振铃吗？","提示",MB_YESNO|MB_ICONQUESTION)==IDYES)
		{
			if(DeleteRing(nRingID))
			{
				m_oComRing.DeleteItem(nSel);
				if(m_oComRing.GetTotalItems() == 1)
				{
					m_oComRing.SetCurSel(0);
					m_oComRing.OnPaint(CDC());
					::MessageBox("系统铃声变为普通铃声，所有特色振铃和只接电话都将失效！", "提示",MB_OK|MB_ICONINFORMATION);
					return TRUE;
				}
				else
				{
					if(nSel != total)
						m_oComRing.SetCurSel(nSel);
					else
						m_oComRing.SetCurSel(1);
				}
				m_oComRing.OnPaint(CDC());
				return TRUE;
			}
			else
			{
				::MessageBox("系统保留铃声，无法删除！", "提示",MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
		}
		else 
			return FALSE;
	}
	return FALSE;
}

BOOL CRingSet::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
//	CRect r(m_sRect.right - 16*2 +1, m_sRect.top ,m_sRect.right -16 ,m_sRect.top +WND_TITLE_HEIGHT);
//	m_oBtnOk.Create(NULL , ICON_OK , r , this ,IDOK);
	CRect r;
	r.SetRect(m_sRect.left + 16 , m_sRect.top + 47 , m_sRect.right -18 ,m_sRect.top + 66);
	m_oComRing.Create(NULL , r, RING_DROPHEIGHT, this, IDC_COM_RING);
	short pRingBuf[RING_MAX_COUNT];
	int nCount = ::FindRing(pRingBuf , RING_MAX_COUNT);
//	::GetPhoneOption(&CSystemSetup::g_SystemOption);
	m_oComRing.AppendItem("普通铃声" , 0 , -1);
	for(int i = 0; i< nCount ; i++)
	{
		m_oComRing.AppendItem(::GetRingPointer(pRingBuf[i])->Name , 0 , pRingBuf[i]);
		if(pRingBuf[i]== g_SystemOption.nDefaultRingID)
			m_oComRing.SetCurSel(i + 1);	//byw
	}
	if( g_SystemOption.nDefaultRingID ==-1)
		m_oComRing.SetCurSel(0);

	/*r.top = r.bottom + 20 , r.bottom = r.top + 18;
	UINT32 dwStyle = (g_SystemOption.nDefaultRingID == -1? WS_VISIBLE: 0);
	m_oRadVolume.Create(r ,dwStyle , this , IDC_RAD_VOLUME);
	m_oRadVolume.AddButton("低");
	m_oRadVolume.AddButton("高");
	m_oRadVolume.SetSel(g_SystemOption.nHardwareRingVolume == RINGVOLUME_LOW? 0:1);*/
	
	r.DeflateRect(38 , 0);
	r.top = r.bottom + 70 , r.bottom = r.top + 18;
//	r.left += 16;
	r.MoveTo(r.left+32,r.top);
	m_bIsListening = FALSE;
	m_oBtnListenTest.Create("试听" , r , this , IDC_BTN_LISTEN);

	r.MoveTo(r.left- 60,r.top);
	m_oBtnDel.Create("删除" , r , this , IDC_BTN_DEL);
}

void CRingSet::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	dc.SetBackColor();
	dc.TextOut(m_sRect.left +16 ,m_sRect.top + 26, "默认振铃");
}

void CRingSet::OnRingSel(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(lParam == 0)
	{
		//m_oRadVolume.SetWindowStyle(m_oRadVolume.GetWindowStyle() | WS_VISIBLE);
		//m_oRadVolume.SetSel(g_SystemOption.nHardwareRingVolume == RINGVOLUME_LOW? 0:1);
		//m_oRadVolume.OnPaint(CDC());
		::MessageBox("使用普通铃声，所有特色振铃和只接电话都将失效！", "提示",MB_OK|MB_ICONINFORMATION);
	}
}

void CRingSet::OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	short pRingBuf[RING_MAX_COUNT];
	int nCount = ::FindRing(pRingBuf , RING_MAX_COUNT);
	INT16 nSel = m_oComRing.GetCurSel();
	if(nSel > nCount)
		return;
	if(nSel > 0){
		g_SystemOption.nDefaultRingID = pRingBuf[nSel - 1];
		DisableHardwareRing();
		//SetRingVolume(RINGVOLUME_HIGH);
	}
	else{
		g_SystemOption.nDefaultRingID = -1;
		EnableHardwareRing();
		g_SystemOption.nHardwareRingVolume = RINGVOLUME_HIGH;
		//SetRingVolume(g_SystemOption.nHardwareRingVolume);
	}
	//::SetPhoneOption(&g_SystemOption);
	SaveSystemOption(&g_SystemOption);
	CAppWnd::OnOk(nMessage, wParam, lParam);
}

void CRingSet::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(nMessage == WM_SOUND_CLOSE)
	{
		m_oBtnListenTest.SetWindowText("试听");
		m_oBtnListenTest.OnPaint(CDC());
		m_bIsListening = FALSE;
	}
	else
		CFrameWnd::WindowProcess(nMessage , wParam , lParam);
}

BOOL CRingSet::OnDestroy()
{
	if(!CAppWnd::OnDestroy())
		return FALSE;
	if(m_bIsListening)
	{
		INT16 status = ::GetPhoneWndStatus();
		if(status == -1 || status == 0)
		{
			::StopRing();
			m_bIsListening = FALSE;
		}
	}
	return TRUE;
}


//试听振铃
void CRingSet::OnListenTest(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	short nRingID ;
	short nSel = m_oComRing.GetCurSel();
	if(nSel == 0)
	{
		::MessageBox("普通铃声无法试听！" , "提示" , MB_OK|MB_ICONINFORMATION);
		return;
	}
	CPopListItem *pItem = m_oComRing.GetItem(nSel);
	if(pItem)
	{
		nRingID = pItem->exData;
		INT16 status = ::GetPhoneWndStatus();
		if(!m_bIsListening)
		{
			if(status == -1 || status == 0)
			{
				sndPlayMusic(nRingID, true, 1);
				m_bIsListening = TRUE;
				m_oBtnListenTest.SetWindowText("停止");
				m_oBtnListenTest.OnPaint(CDC());
			}
		}
		else
		{
			if(status == -1 || status == 0)
			{
				::StopRing();
				m_bIsListening = FALSE;
				m_oBtnListenTest.SetWindowText("试听");
				m_oBtnListenTest.OnPaint(CDC());
			}
		}
	}
}

//**********下面是禁拨号码窗口类的定义*************************//
#define   IDC_GRIDNUM                    3540
BEGIN_MESSAGE_MAP(CForbidDial, CAppWnd)
  ON_MESSAGE(IDC_GRIDNUM, CN_GRIDSEL, OnGridClick)
END_MESSAGE_MAP 

BOOL CForbidDial::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(m_sRect.left +18 , m_sRect.top +49 , m_sRect.right , m_sRect.bottom );
	if(!m_oGridNum.Create(this , r , WS_VISIBLE|TBS_SPACELINE , 6 , 1 , IDC_GRIDNUM))
		return FALSE;
	for(int i = 0; i< FORBID_COUNT ;i++)
		m_oGridNum.SetUnitText(i+1 , 1 , g_SystemOption.strLimitNumber[i]);
	return TRUE;
}

void CForbidDial::OnGridClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	char strNum[PHONE_NUMBER_INPUT_LEN + 1];
	INT16 iRow = LOWORD(lParam) , iCol = HIWORD(lParam);
	m_oGridNum.GetUnitText(iRow , iCol , strNum , PHONE_NUMBER_INPUT_LEN);
	if(::NumInputBox("请输入号码",strNum , PHONE_NUMBER_INPUT_LEN)==IDOK)
		m_oGridNum.SetUnitText(iRow , iCol , strNum , 0 , TRUE);
}

void CForbidDial::OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	char strNum[8];
	for(int i= 0; i< FORBID_COUNT ; i++)
	{
		m_oGridNum.GetUnitText(i+1, 1 , strNum , 8);
		strcpy(g_SystemOption.strLimitNumber[i] , strNum);
	}
	SaveSystemOption(&g_SystemOption);
	CAppWnd::OnOk(nMessage, wParam, lParam);
}

void CForbidDial::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	CRect r(m_sRect.left , m_sRect.top +18 , m_sRect.right , m_sRect.top +51);
	dc.SetBackColor();
	dc.DrawText(r, "下面数字开头的号码禁止拨出:");
	INT16 x, y;
	x = m_sRect.left , y = m_sRect.top + 52;
	char pIndex[3];
	for(INT16 i =0; i< FORBID_COUNT ; i++)
	{
		sprintf(pIndex , "%d.", i+1);
		dc.TextOut(x, y, pIndex);
		y += 17;
	}
	dc.SetLineStyle(1);
	dc.MoveTo(m_sRect.left + 17 , y-3 );
	dc.LineTo(m_sRect.right -1 , y-3);
}

//***************下面是外线设置主界面窗口的定义*********************//

#define  IDC_COM_HEADER                           3550
#define  IDC_BTN_AREACODE                         3551
BEGIN_MESSAGE_MAP(COutLine, CAppWnd)
  ON_MESSAGE(IDC_COM_HEADER, CN_COMMAND, OnComSel)
  ON_MESSAGE(IDC_BTN_AREACODE, CN_COMMAND, OnAreaCodeInput)
END_MESSAGE_MAP 


BOOL COutLine::OnCreate()
{
	char strOutLine[OUTLINE_HEAD_LEN +1];

	if(!CAppWnd::OnCreate())
		return FALSE;
	g_bOutWndDel = FALSE;
	strcpy(m_strHeader, g_SystemOption.strOutLineHead);
	CRect r(m_sRect.left +40 , m_sRect.top +49 , m_sRect.left +121 , m_sRect.top +68);
	if (m_strHeader[0] == '\0')
		strcpy(strOutLine, "直拨");
	else
		strcpy(strOutLine, m_strHeader);
	if(!m_oComHeader.Create(strOutLine, r , 74 , this , IDC_COM_HEADER))
		return FALSE;
	m_oComHeader.AppendItem("0", 0, 0);
	m_oComHeader.AppendItem("9", 0, 0);
	m_oComHeader.AppendItem("直拨", 0, 0);
	m_oComHeader.AppendItem("其他", 0, 0);
	r.top += 66 , r.bottom = r.top + 18;
	
	strcpy(m_strAreaCode , g_SystemOption.LocalAreaCode);
	m_oBtnLocalAreaCode.Create(m_strAreaCode, r, this ,IDC_BTN_AREACODE);
	
	return TRUE;
}

void COutLine::OnAreaCodeInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	char strTemp[8] = "";
	strcpy(strTemp , m_strAreaCode);
	if(::NumInputBox("输入本地区号", strTemp, 5)==IDOK)
	{
		if(IsLegalWnd() && !g_bOutWndDel) {
			m_oBtnLocalAreaCode.SetWindowText(strTemp);
			m_oBtnLocalAreaCode.OnPaint(CDC());
			strcpy(m_strAreaCode,strTemp);
		}
	}
}

void COutLine::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	dc.Reset();
	dc.TextOut(m_sRect.left +48 , m_sRect.top +25 , "外线先拨");
	dc.TextOut(m_sRect.left +48 , m_sRect.top +91 , "本地区号");
}

void COutLine::OnComSel(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	char strHeader[OUTLINE_HEAD_LEN + 1];
	strHeader[0] = '\0';
	switch (lParam){
		case 0:
			strcpy(m_strHeader, "0");
			break;
		case 1:
			strcpy(m_strHeader, "9");
			break;
		case 2:
			//strcpy(m_strHeader, "直拨");
			m_strHeader[0] = '\0';
			break;	
		case 3:
			//strcpy(strHeader, m_strHeader);	//byw, 2002/1/31
			m_strHeader[0] = '\0';
			if(::NumInputBox("请输入外线字头", strHeader , OUTLINE_HEAD_LEN + 1)==IDOK){
				if(IsLegalWnd() && !g_bOutWndDel)
					strcpy(m_strHeader, strHeader);
			}
			break;
	}
	if(IsLegalWnd() && !g_bOutWndDel) {
		if (m_strHeader[0] == '\0')
			strcpy(strHeader, "直拨");
		else
			strcpy(strHeader, m_strHeader);
		m_oComHeader.SetWindowText(strHeader);
		if (lParam == 3)
			m_oComHeader.OnPaint(CDC());
	}
}
		
void COutLine::OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	strcpy(g_SystemOption.strOutLineHead, m_strHeader);
	strcpy(g_SystemOption.LocalAreaCode, m_strAreaCode);
	SaveSystemOption(&g_SystemOption);
	CAppWnd::OnOk(nMessage, wParam, lParam);
}
	
//**********下面是只接电话窗口类的定义*************************//		
#define IDC_GRID_TEL                   3560           
BEGIN_MESSAGE_MAP(CAcceptCall, CAppWnd)
  ON_MESSAGE(IDC_GRID_TEL, CN_GRIDSEL, OnGridClick)
END_MESSAGE_MAP 

BOOL CAcceptCall::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(m_sRect.left, m_sRect.top + 19 , m_sRect.right , m_sRect.bottom);
	if(!m_oGridTel.Create(this , r, WS_VISIBLE|TBS_SPACELINE , 8 , 2,IDC_GRID_TEL))
		return FALSE;
	m_bChecked = g_SystemOption.nNoInterrept;
	UINT16 pColsWidth[] = {18 , 0};
	m_oGridTel.SetColsWidth(pColsWidth);
	m_oGridTel.SetUnitText(1, 1,NULL , m_bChecked ?ICON_CHECKED : ICON_UNCHECKED);
	m_oGridTel.SetUnitText(1, 2, "只下列号码振铃", 0);
    for(INT16 i = 0 ;i< ONLYACCEPT_COUNT; i++)
	{
		m_oGridTel.SetUnitText(i+2 , 1, NULL, ICON_CHOICEFROMCARD);
		m_oGridTel.SetUnitText(i+2 , 2, g_SystemOption.strOnlyAcceptTel[i], 0);
	}
    return TRUE;
}

void CAcceptCall::OnGridClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	INT16 iRow = LOWORD(lParam), iCol= HIWORD(lParam);
	if(iRow==1 )
	{
		m_oGridTel.SetUnitIcon(1, 1, m_bChecked? ICON_UNCHECKED:ICON_CHECKED, TRUE);
		m_bChecked = !m_bChecked;
	}
	else 
	{ 
		char strTel[21];
//		strTel[0] ='\0';
		if(iCol==1)
		{
			//extern short SelectTelNum(char *strTel , short nBufLen , char & cTelType );
			extern short SelectTelNum(char *strTel , short nBufLen , CARDFIELD &nCardField);
			CARDFIELD nCardField;
			if(SelectTelNum(strTel , 21, nCardField)!=-1)
				m_oGridTel.SetUnitText(iRow , 2, strTel , 0 , TRUE);
		}
		else
		{
			m_oGridTel.GetUnitText(iRow ,2 , strTel , 20); 
			if(::NumInputBox("请输入号码", strTel , 20)==IDOK)
				m_oGridTel.SetUnitText(iRow , 2, strTel , 0 , TRUE);
		}
	}
}

void CAcceptCall::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	dc.Reset();
	dc.SetLineStyle(1);
	dc.MoveTo(m_sRect.left, m_sRect.bottom - 4);
	dc.LineTo(m_sRect.right , m_sRect.bottom -4);
}

void CAcceptCall::OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	for(INT16 i =1 ; i <= ONLYACCEPT_COUNT;i++)
		m_oGridTel.GetUnitText(i+1 , 2,g_SystemOption.strOnlyAcceptTel[i-1], 21);
	g_SystemOption.nNoInterrept = (m_bChecked? 1: 0);
	SaveSystemOption(&g_SystemOption);
	CAppWnd::OnOk(nMessage, wParam, lParam);
}
	
//**********下面是IP话窗口类的定义*************************//
#define           IDC_PAGE_IP1                             3590
#define           IDC_PAGE_IP2                             3591
#define           IDC_PAGE_IP3                             3592
#define           IDC_PAGE_IP4                             3593
#define           IDC_PAGE_IP5                             3594
#define           IDC_BTN_HEADER                           3595                          
#define           IDC_BTN_ACCOUNT                          3596                          
#define           IDC_BTN_PSWD                             3597                          
#define           IDC_BTN_DELAY1                           3598
#define           IDC_BTN_DELAY2                           3599
#define           IDC_BTN_DELAY3                           3600
#define           IDC_BTN_DELAY4                           3601
#define           IDC_RAD_DEF                              3602
BEGIN_MESSAGE_MAP(CIPPhone, CAppWnd)
  ON_MESSAGE_RANGE(IDC_PAGE_IP1, IDC_PAGE_IP5, CN_COMMAND, OnPagesClick)
  ON_MESSAGE_RANGE(IDC_BTN_HEADER, IDC_BTN_DELAY4, CN_COMMAND, OnInfosClick)
  ON_MESSAGE(IDC_RAD_DEF, CN_COMMAND, OnRadClick)
END_MESSAGE_MAP 

BOOL CIPPhone::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	memcpy(&m_ipInfo , &g_SystemOption.iiDialNum , sizeof(m_ipInfo));
	//把系统设置信息拷到BUF中
	m_nCurIP = m_nDefIP = g_SystemOption.nDefaultIP;
#define PAGE_HEIGHT                      16
#define PAGE_WIDTH                       31
	//下面创建IP页签控件
	CRect r(m_sRect.left , m_sRect.top +18 , m_sRect.left + PAGE_WIDTH-1, 
				m_sRect.top +18 +PAGE_HEIGHT -1);
	short i;
	char  strText[10];
	for(i =0;i < 5 ;i++)
	{
		sprintf(strText, "IP%d" , i+1);
		m_oPageip[i].Create(strText,  r, this , IDC_PAGE_IP1+ i);
		r.left = r.right +1 ,r.right = r.left +PAGE_WIDTH;
	}
    m_oPageip[m_nDefIP].SetSelected(TRUE);
#undef PAGE_HEIGHT
#undef PAGE_WIDTH
	//下面创建帐号等控件
	r.SetRect(m_sRect.left+ 45 , m_sRect.top +55, m_sRect.left +113,
		m_sRect.top + 72);
	m_oBtnInfo[0].Create(m_ipInfo[m_nDefIP].strHead , r , this , IDC_BTN_HEADER);
	r.top = r.bottom +3 , r.bottom = r.top +17;
	m_oBtnInfo[1].Create(m_ipInfo[m_nDefIP].strAccount , r , this , IDC_BTN_HEADER + 1);
	r.top = r.bottom +3 , r.bottom = r.top +17;
	char strPswd[7];
	::ConvStrToPswd(m_ipInfo[m_nDefIP].strPassword , strPswd);
	m_oBtnInfo[2].Create(strPswd , r , this , IDC_BTN_HEADER + 2);
	
	//下面创建延时控件
	r.SetRect(m_sRect.left+ 125 , m_sRect.top +55, m_sRect.right -1,
		m_sRect.top + 72);
	char *ppTemp[]= { m_ipInfo[m_nDefIP].strPostfix0,m_ipInfo[m_nDefIP].strPostfix1,
					  m_ipInfo[m_nDefIP].strPostfix2,m_ipInfo[m_nDefIP].strPostfix3
	};

	for(i =0 ;i<4 ;i++)
	{
		m_oBtnDelay[i].Create(ppTemp[i] , r , this , IDC_BTN_DELAY1 + i);
		r.top = r.bottom +3 , r.bottom = r.top +17;
	}
    //下面创建radio控件
	r.SetRect(m_sRect.left + 47,  m_sRect.top +135, m_sRect.left +143,
		m_sRect.top + 153);
    if(!m_oRadDef.Create(r ,WS_VISIBLE, this , IDC_RAD_DEF))
		return FALSE;
	m_oRadDef.AddButton("设为默认");
	m_oRadDef.SetSel(0);
	
	return TRUE;
}
	
void CIPPhone::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	dc.Reset();
	dc.PutIcon(m_sRect.left , m_sRect.top +33 , ICON_IPRULE);
	char *ppTip[]= 
	{
		"拨号:" ,"卡号:" , "密码:" , "号码:自动得到" , "默认:"
	};
	CPoint pt(m_sRect.left +3 , m_sRect.top +55);
	INT16 i;
    for(i= 0; i<5; i++)
	{
		dc.TextOut(pt.x, pt.y, ppTip[i]);
		pt.y += 20;
	}
	pt.x = m_sRect.left +114 , pt.y = m_sRect.top + 55;
	for(i= 0; i< 4; i++)
	{
		dc.TextOut(pt.x, pt.y, "+");
		pt.y += 20;
	}
    
}

void CIPPhone::LoadIPOption(INT16 iIndex)
{
	if(iIndex <0 || iIndex >= 5)
		return;
	m_oBtnInfo[0].SetWindowText(m_ipInfo[iIndex].strHead);
	m_oBtnInfo[1].SetWindowText(m_ipInfo[iIndex].strAccount);
	char strTemp[IP_PASSWORD_LEN];
	::ConvStrToPswd(m_ipInfo[iIndex].strPassword , strTemp);
	m_oBtnInfo[2].SetWindowText(strTemp);
	m_oBtnDelay[0].SetWindowText(m_ipInfo[iIndex].strPostfix0);
	m_oBtnDelay[1].SetWindowText(m_ipInfo[iIndex].strPostfix1);
	m_oBtnDelay[2].SetWindowText(m_ipInfo[iIndex].strPostfix2);
	m_oBtnDelay[3].SetWindowText(m_ipInfo[iIndex].strPostfix3);
	if(iIndex == m_nDefIP)
		m_oRadDef.SetSel(0);
	else
		m_oRadDef.SetSel(-1);
}

void CIPPhone::OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	memcpy(&g_SystemOption.iiDialNum , &m_ipInfo,sizeof(m_ipInfo));
	g_SystemOption.nDefaultIP = m_nDefIP;
	SaveSystemOption(&g_SystemOption);
	CAppWnd::OnOk(nMessage, wParam, lParam);
}
//点击IP页签处理函数
void CIPPhone::OnPagesClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
     wParam -= IDC_PAGE_IP1;
	 if(wParam == m_nCurIP)
		 return;
	 for(INT16 i=0;i< 5 ; i++)
	 {
		 if(i== wParam)
			 m_oPageip[i].SetSelected(TRUE);
		 else
			 m_oPageip[i].SetSelected(FALSE);
	 }
	 LoadIPOption(wParam);
	 m_nCurIP = wParam;
	 OnPaint(CDC());
}

//设为默认IP处理函数
void CIPPhone::OnRadClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_nCurIP != m_nDefIP)
	{
		m_oRadDef.SetSel(0);
		m_nDefIP = m_nCurIP;
	}
}
	
void CIPPhone::OnInfosClick(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	wParam -= IDC_BTN_HEADER;
	switch(wParam)
	{
	case 0: //ip的号码(17910等)
		{
		if(::NumInputBox("请输入IP号码",m_ipInfo[m_nCurIP].strHead , IP_DIALIN_LEN)==IDOK)
		{
			m_oBtnInfo[0].SetWindowText(m_ipInfo[m_nCurIP].strHead);
			m_oBtnInfo[0].OnPaint(CDC());
		}
		else
			m_oBtnInfo[0].GetWindowText(m_ipInfo[m_nCurIP].strHead, IP_DIALIN_LEN);
		break;
		}
	case 1: //帐号
		{
		if(::NumInputBox("请输入IP卡号",m_ipInfo[m_nCurIP].strAccount , IP_CARDNUM_LEN)==IDOK)
		{
			m_oBtnInfo[1].SetWindowText(m_ipInfo[m_nCurIP].strAccount);
			m_oBtnInfo[1].OnPaint(CDC());
		}
		else
			m_oBtnInfo[1].GetWindowText(m_ipInfo[m_nCurIP].strAccount,IP_CARDNUM_LEN);
		break;

		}
	case 2: //密码
		{
		char strTemp[IP_PASSWORD_LEN];
		strcpy(strTemp, m_ipInfo[m_nCurIP].strPassword);
		strTemp[0]= '\0';
		if(::NumInputBox("请输入IP密码",strTemp , IP_PASSWORD_LEN)==IDOK)
		{
			char strPswd[7];
			strcpy(m_ipInfo[m_nCurIP].strPassword, strTemp);
			::ConvStrToPswd(m_ipInfo[m_nCurIP].strPassword,strPswd);
			m_oBtnInfo[2].SetWindowText(strPswd);
			m_oBtnInfo[2].OnPaint(CDC());
		}
		else
			m_oBtnInfo[2].GetWindowText(m_ipInfo[m_nCurIP].strPassword,IP_PASSWORD_LEN);
		break;
		}
	case 3:  //提示语言
		{
		if(::NumInputBox("请输入提示语言",m_ipInfo[m_nCurIP].strPostfix0 , IP_DELAY_LEN)==IDOK)
		{
			m_oBtnDelay[0].SetWindowText(m_ipInfo[m_nCurIP].strPostfix0);
			m_oBtnDelay[0].OnPaint(CDC());
		}
		else
			m_oBtnDelay[0].GetWindowText(m_ipInfo[m_nCurIP].strPostfix0, IP_DELAY_LEN);

		break;
		}
	case 4:  //延时1
		{
		if(::NumInputBox("请输入结束符",m_ipInfo[m_nCurIP].strPostfix1 , IP_DELAY_LEN)==IDOK)
		{
			m_oBtnDelay[1].SetWindowText(m_ipInfo[m_nCurIP].strPostfix1);
			m_oBtnDelay[1].OnPaint(CDC());
		}
		else
			m_oBtnDelay[1].GetWindowText(m_ipInfo[m_nCurIP].strPostfix1, IP_DELAY_LEN);
		break;
		}
	case 5: //延时2
		{
		if(::NumInputBox("请输入结束符",m_ipInfo[m_nCurIP].strPostfix2 , IP_DELAY_LEN)==IDOK)
		{
			m_oBtnDelay[2].SetWindowText(m_ipInfo[m_nCurIP].strPostfix2);
			m_oBtnDelay[2].OnPaint(CDC());
		}
		else
			m_oBtnDelay[2].GetWindowText(m_ipInfo[m_nCurIP].strPostfix2, IP_DELAY_LEN);
		break;
		}
	case 6: //结束符
		{
		if(::NumInputBox("请输入结束符",m_ipInfo[m_nCurIP].strPostfix3 , IP_DELAY_LEN)==IDOK)
		{
			m_oBtnDelay[3].SetWindowText(m_ipInfo[m_nCurIP].strPostfix3);
			m_oBtnDelay[3].OnPaint(CDC());
		}
		else
			m_oBtnDelay[3].GetWindowText(m_ipInfo[m_nCurIP].strPostfix3, IP_DELAY_LEN);	
		break;
		}
	}
}

//**********下面是来电显示窗口类的定义*************************//
/*
#define      IDC_RAD_MODE                      3570
BOOL CCallID::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(m_sRect.left +18 , m_sRect.top + 60 , m_sRect.left +100 , m_sRect.top +97);
	if(!m_oRadMode.Create(r , WS_VISIBLE , this , IDC_RAD_MODE))
		return FALSE;
	m_oRadMode.AddButton("FSK制式");
	m_oRadMode.AddButton("DTMF制式");
	m_oRadMode.SetSel(g_SystemOption.iCallInMode);
	m_oRadMode.SetAlign(VIRTICAL);
	return TRUE;
}

void CCallID::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	dc.SetBackColor();
	dc.TextOut(m_sRect.left +15 , m_sRect.top +27 , "本地来电显示制式");
}

void CCallID::OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	g_SystemOption.iCallInMode = m_oRadMode.GetSel();
	SaveSystemOption(&g_SystemOption);
	CAppWnd::OnOk(nMessage, wParam, lParam);
}
*/   
//**********下面是自动关机窗口类的定义*************************//
#define  IDC_TIME_BAR             3580
#define  IDC_BTN_ADD              3581
#define  IDC_BTN_DEC              3582
#define  IDC_BTN_USE              3583
#define  IDC_BTNPSWD              3520                

BEGIN_MESSAGE_MAP(CAutoShutdown, CAppWnd)
  ON_MESSAGE(IDC_BTN_ADD, CN_COMMAND, OnTimeAdd)
  ON_MESSAGE(IDC_BTN_DEC, CN_COMMAND, OnTimeDec)  
  ON_MESSAGE(IDC_BTN_USE, CN_COMMAND, OnUseSwitch)
  ON_MESSAGE(IDC_TIME_BAR, CS_PROCESS, OnProcessMove)
  ON_MESSAGE(IDC_BTNPSWD, CN_COMMAND, OnPswdInput)
END_MESSAGE_MAP 

BOOL CAutoShutdown::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(m_sRect.left +7 , m_sRect.top + 20 , m_sRect.right -20 , m_sRect.top + 38);
	UINT32 nIconID = (g_SystemOption.PowerDownTime ? ICON_CHECKED :ICON_UNCHECKED);
	m_bUsed = (g_SystemOption.PowerDownTime ? TRUE:FALSE);
	m_oBtnUse.Create("使用屏幕保护", r , this ,m_bUsed ,IDC_BTN_USE);
	r.SetRect(m_sRect.left +5 , m_sRect.top + 74 , m_sRect.left +135 , m_sRect.top + 93);
	if(!m_oTimeBar.Create(1 , 30,1 , r , this ,CS_PENMOVE|CS_BIAOCHI, IDC_TIME_BAR ))//ygr modify CProcesbar::Create 
		return FALSE;
	m_oTimeBar.SetCurValue(g_SystemOption.PowerDownTime);
	r.SetRect(m_sRect.left +98, m_sRect.top +96 , m_sRect.left +133 , m_sRect.top +115);
	m_oBtnAdd.Create("增加" , r, this , IDC_BTN_ADD);
	r.right = r.left - 58 , r.left = r.right - 35;
	m_oBtnDec.Create("减少" , r, this , IDC_BTN_DEC);
	r.SetRect(m_sRect.left +20 , m_sRect.top + 136 , m_sRect.right - 26 , m_sRect.top + 152);
	char strPswd[9];
	strcpy(m_strPswd , g_SystemOption.strScreenPassword);
	::ConvStrToPswd(g_SystemOption.strScreenPassword , strPswd);
	m_oBtnPswd.Create(strPswd, r , this , IDC_BTNPSWD);

	if(!m_bUsed) // 不使用自动关机
	{
		m_oTimeBar.SetWindowStyle(m_oTimeBar.GetWindowStyle() & ~WS_VISIBLE);
		m_oBtnAdd.SetWindowStyle(m_oBtnAdd.GetWindowStyle() & ~WS_VISIBLE);
		m_oBtnDec.SetWindowStyle(m_oBtnDec.GetWindowStyle() & ~WS_VISIBLE);
		m_oBtnPswd.SetWindowStyle(m_oBtnPswd.GetWindowStyle() & ~WS_VISIBLE);
	}
	g_bAutoShutWndDel = FALSE;
	return TRUE;
}

void CAutoShutdown::PaintOther(CDC &dc)
{
	dc.SetBackColor();
	if(m_oBtnUse.GetValue())
	{
		dc.TextOut(m_sRect.left +5 , m_sRect.top+ 39 , "进入屏幕保护时间");
		char strMinu[11];
		CRect r(m_sRect.left + 40 , m_sRect.top + 57 , m_sRect.right -1 , m_sRect.top + 73);
		dc.EraseRect(r);
		sprintf(strMinu , "% 2d分钟" , m_oTimeBar.GetCurValue());
		dc.TextOut(m_sRect.left + 40 , m_sRect.top +57 , strMinu);
		dc.TextOut(m_sRect.left + 5 , m_sRect.top + 118 , "设定屏幕保护密码");		
	}
	else
	{
		CRect r(m_sRect.left , m_sRect.top + 39 , m_sRect.right , m_sRect.bottom);
		dc.EraseRect(r);
	}
}

void CAutoShutdown::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	PaintOther(dc);
}

void CAutoShutdown::OnTimeAdd(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
//	if(m_bUsed)
		m_oTimeBar.SetCurValue(m_oTimeBar.GetCurValue()+1, TRUE);
	PaintOther(CDC());
}

void CAutoShutdown::OnTimeDec(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
//	if(m_bUsed)
		m_oTimeBar.SetCurValue(m_oTimeBar.GetCurValue()-1, TRUE);
	PaintOther(CDC());
}

void CAutoShutdown::OnOk(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_oBtnUse.GetValue())
		g_SystemOption.PowerDownTime = m_oTimeBar.GetCurValue();
	else
		g_SystemOption.PowerDownTime = 0;
	strcpy(g_SystemOption.strScreenPassword , m_strPswd);
	SaveSystemOption(&g_SystemOption);
	CAppWnd::OnOk(nMessage, wParam, lParam);
}

void CAutoShutdown::OnUseSwitch(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
//	m_oBtnUse.OnPaint(CDC());
	if(m_oBtnUse.GetValue())
	{
		m_oTimeBar.SetWindowStyle(m_oTimeBar.GetWindowStyle() | WS_VISIBLE);
		m_oBtnAdd.SetWindowStyle(m_oBtnAdd.GetWindowStyle() | WS_VISIBLE);
		m_oBtnDec.SetWindowStyle(m_oBtnDec.GetWindowStyle() | WS_VISIBLE);
		m_oBtnPswd.SetWindowStyle(m_oBtnDec.GetWindowStyle() | WS_VISIBLE);
	}
	else
	{
		m_oTimeBar.SetWindowStyle(m_oTimeBar.GetWindowStyle() & ~WS_VISIBLE);
		m_oBtnAdd.SetWindowStyle(m_oBtnAdd.GetWindowStyle() & ~WS_VISIBLE);
		m_oBtnDec.SetWindowStyle(m_oBtnDec.GetWindowStyle() & ~WS_VISIBLE);
		m_oBtnPswd.SetWindowStyle(m_oBtnDec.GetWindowStyle() & ~WS_VISIBLE);
	}
	m_bUsed = !m_bUsed;
	OnPaint(CDC());
}

void CAutoShutdown::OnPswdInput(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	char strInput[9];
	char strDisp[9];
	strcpy(strInput, m_strPswd);
	if(strInput[0])
	{
		if(::InputPassword(strInput, 9 , 1,"请输入旧密码")==IDOK)
			if(::InputPassword(strInput, 9 , 0)==IDOK)
			{
				if(IsLegalWnd() && !g_bAutoShutWndDel)
				{
					strcpy(m_strPswd , strInput);
					::ConvStrToPswd(strInput , strDisp);
					m_oBtnPswd.SetWindowText(strDisp);
					m_oBtnPswd.OnPaint(CDC());
				}
			}
	}
	else
	{
		if(::InputPassword(strInput, 9 , 0)==IDOK)
		{
			if(IsLegalWnd() && !g_bAutoShutWndDel) 
			{
				strcpy(m_strPswd, strInput);
				::ConvStrToPswd(strInput , strDisp);
				m_oBtnPswd.SetWindowText(strDisp);
				m_oBtnPswd.OnPaint(CDC());
			}
		}
	}
}

void CAutoShutdown::OnProcessMove(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	m_oTimeBar.SetCurValue(lParam, TRUE);
	PaintOther(CDC());
}

//**********下面是内存使用窗口类的定义*************************//
BOOL CMemoryInfo::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	char strInfo[8][25];
	char strTotalInfo[513] = "";
	short nCaseUsed , nCardUsed , nMemUsed ,
		nAlarmUsed , nHisUsed, nSmsUsed,nRingUsed , nWaveUsed, nWaveMaxSec; //, nTemp;
	//short pCommonBuf[CARD_MAX_COUNT+CARDCASE_MAX_COUNT];
	nCardUsed = GetCardUsed();
	nCaseUsed = GetCardCaseUsed();
	nAlarmUsed = GetAlarmUsed();
	nHisUsed = GetHisrotyUsed();
	nRingUsed = GetRingUsed();
	nMemUsed = GetMemoUsed();
	nWaveMaxSec = WAVE_STORE_LEN/(8192*60);
	nWaveUsed = GetWaveUsed() * 100/WAVE_STORE_LEN;
	nSmsUsed = GetSmsUsed();

	sprintf(strInfo[0] , "名片夹 %3d个 %3d",CARDCASE_MAX_COUNT, nCaseUsed);
	sprintf(strInfo[1] , "  名片%3d张 %3d",CARD_MAX_COUNT, nCardUsed);
	sprintf(strInfo[2] , "  记事 %3d个 %3d",MEMO_MAX_COUNT, nMemUsed);
	sprintf(strInfo[3] , "  提醒 %3d个 %3d",ALARM_MAX_COUNT, nAlarmUsed);
	sprintf(strInfo[4] , "  通话 %3d个 %3d",HISTORY_MAX_COUNT, nHisUsed);
	sprintf(strInfo[5] , "短消息 %3d条 %3d",SMS_MAX_COUNT, nSmsUsed);
	sprintf(strInfo[6] , "  铃声 %3d个 %3d",RING_MAX_COUNT, nRingUsed);
	sprintf(strInfo[7] , "  录音 %3d分  %d%%",nWaveMaxSec, nWaveUsed);
	for(short i = 0 ; i< 8; i++)
	{
//		strcat(strTotalInfo, "--*--*--*--*--*--\r\n");
		strcat(strTotalInfo, strInfo[i]);
		strcat(strTotalInfo , "\r\n");
	}
	strTotalInfo[512] = '\0';
	// TODO:加短消息
	CRect r(m_sRect.left +1 , m_sRect.top + WND_TITLE_HEIGHT+ 20, 
		             m_sRect.right ,m_sRect.bottom-1);
	m_oStaticInfo.Create(strTotalInfo, r , this , WS_VISIBLE , 
		0, 513 , NULL);
}

void CMemoryInfo::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	CRect dispRect(m_sRect.left +1 , m_sRect.top + WND_TITLE_HEIGHT+1, 
		             m_sRect.right -1 ,m_sRect.top + WND_TITLE_HEIGHT + 18);
	dc.Reset();
//	dc.FrameRect(dispRect);
//	dc.Draw3dRect(dispRect , GRAY , GRAY);

//	dispRect.DeflateRect(1, 1);
	dispRect.bottom = dispRect.top + 17;
	dc.SetBackColor(LGRAY);
	dc.EraseRect(dispRect);
	dc.DrawText(dispRect,"项目   最大  已使用");
}

//**********下面是代理商信息窗口类的定义*************************//
void CAgentInfo::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	CRect dispRect(m_sRect.left +1 , m_sRect.top +28, 
		             m_sRect.right -1 , m_sRect.top + 46 );
	dc.Reset();
//	dc.FrameRect(dispRect);
//	dispRect.DeflateRect(1, 1);
//	dispRect.bottom = dispRect.top + 34;
//	dc.DrawText(dispRect , "EP201       汉王科技        版权所有");
	dc.DrawText(dispRect , "汉王科技 版权所有");

	dispRect.top = dispRect.bottom +1 ,  dispRect.bottom = m_sRect.bottom;
	dc.SetBackColor(LGRAY);
	dc.EraseRect(dispRect);
	dc.Draw3dRect(dispRect , GRAY , GRAY);
	dispRect.DeflateRect(1, 1);
	dc.Draw3dRect(dispRect , GRAY , WHITE);
	dispRect.DeflateRect(1, 1);
//	dc.FrameRect(dispRect);
	
	
	char strSoftVerInfo[84] = "";
	char strPhoneVerInfo[84] = "";
	char strDate[64] = "";
	char strSerialNum[128] = "";
	char strAgentInfo[160] = "";

	char *strPhone = ::GetPhoneVersion();
	sprintf(strSoftVerInfo, "版本号:%s", g_SystemOption.Version);
	sprintf(strPhoneVerInfo, "       PVer %s",strPhone );
	sprintf(strDate, "  日期:%4d/%02d/%02d", g_SystemOption.SetupDate.year,g_SystemOption.SetupDate.month,
		  g_SystemOption.SetupDate.day);
	sprintf(strSerialNum, "序列号:%s", g_SystemOption.SeriesNumber);
	sprintf(strAgentInfo, "代理商:%s", g_SystemOption.VendorInfo);

	char *strInfo[] = {strSoftVerInfo, strPhoneVerInfo, strDate, strSerialNum , strAgentInfo};

//	dc.SetBackColor();
	CPoint pt(dispRect.left +2 , dispRect.top +2);
	for(short i =0 ;i< 5; i++)
	{
		dc.TextOut(pt.x ,pt.y , strInfo[i]);
		pt.y += 17;
	}

	/*
	*	计算代码区校验和
	*/
	UINT32		sum = 0, addr;

	for(addr = CODE_START_ADDR; addr < CODE_END_ADDR; addr += 4)
		sum += *(UINT32*)addr;
	char strTemp[64];
	sprintf(strTemp , "校验和:%08X", sum);
	dc.TextOut(pt.x ,pt.y , strTemp);
}

	
static CSystemSetup *g_pSetupWnd= NULL;
void SettingsAppRun()
{
	if(!g_pSetupWnd)
		g_pSetupWnd = new CSystemSetup;
    ::SetCurrentApp(APPID_SETUP);
	g_pSetupWnd->Create("系统设置",  WS_VISIBLE| WS_CAPTION , DESK_TOP_RECT,HELP_SETUP);
	g_pSetupWnd->ShowWindow();
}

//**********下面是调试信息主窗口类的定义*************************//
#define  IDC_BTN_BURN                3590 
#define  IDC_BTN_CALLID              3591 

BEGIN_MESSAGE_MAP(CDebugMainWnd, CAppWnd)
  ON_MESSAGE(IDC_BTN_BURN, CN_COMMAND, OnBurn)
  ON_MESSAGE(IDC_BTN_CALLID, CN_COMMAND, OnCallID)  
END_MESSAGE_MAP 

BOOL CDebugMainWnd::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	CRect r(m_sRect.left + 48 , m_sRect.top + 64, m_sRect.left + 113,
		 m_sRect.top + 85);
	m_oBtnCallID.Create("来电显示", r , this, IDC_BTN_CALLID);
	r.top = r.bottom + 24, r.bottom = r.top + 21;
	m_oBtnBurn.Create("烧录", r , this, IDC_BTN_BURN);
	m_pCallIDWnd = NULL;
	return TRUE;
}

BOOL CDebugMainWnd::OnDestroy()
{
	CAppWnd::OnDestroy();
	if(m_pCallIDWnd)
	{
		delete m_pCallIDWnd;
		m_pCallIDWnd = NULL;
	}
	return TRUE;
}

void CDebugMainWnd::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	dc.Reset();
	char strTip[64];
	sprintf(strTip , "已重启%d次" , g_SystemOption.nResetTimes);
	dc.TextOut(m_sRect.left + 38 , m_sRect.top + 30, strTip);
}

void CDebugMainWnd::OnCallID(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(!m_pCallIDWnd)
		m_pCallIDWnd = new CCallIDInfo;
	m_pCallIDWnd->Create(NULL , WS_CAPTION | WS_VISIBLE , DESK_TOP_RECT);
	m_pCallIDWnd->ShowWindow();
}

void CDebugMainWnd::OnBurn(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	// TODO: 加入程晓东接口
}

//**********下面是来电显示调试信息窗口类的定义*************************//

int GetCallIDItems()
{
	UINT32		addr;
	int iItems = 0;
	for (addr = 0x01300000; addr < 0x01400000; addr += 1024){
		if ((*(UINT16 *)addr) >= 1901 && (*(UINT16 *)addr) <= 2050)
			iItems++;
	}
	return iItems;
}

BOOL GetSpecifyCallID(int iItem, CallIDInfo &callid)
{
	UINT32		addr;
	int iTotalItems , j, i;
	iTotalItems = GetCallIDItems();
	if(iTotalItems == 0)
		return FALSE;

	if(iItem >= iTotalItems)
		iItem = iTotalItems -1;
	else if(iItem < 0)
		iItem = 0;
	
	for (addr = 0x01300000, j= 0; addr < 0x01400000; addr += 1024){
		if ((*(UINT16 *)addr) >= 1901 && (*(UINT16 *)addr) <= 2050)
			if(j++ == iItem)
			{
				BYTE *pTemp = (BYTE *)addr;
				memcpy(&callid.dt , pTemp,sizeof(DateTime));
				pTemp += sizeof(DateTime);
				i =0;
				while((*pTemp)!= 0xFF && i < PHONE_CALLIDBUF_SEZE*3 -1 )
				{
					BYTE bt = *pTemp;
					callid.strCallID[i] = ((bt >> 4) > 9? ((bt>> 4) + 0x37 ):
											((bt>> 4) + 0x30 ));
					callid.strCallID[i+1] = ((bt & 0x0F) > 9? ((bt & 0x0F) + 0x37 ):
											((bt & 0x0F) + 0x30 ));
					callid.strCallID[i+2] = '-';
					pTemp++ , i+=3;
				}
				callid.strCallID[i] = '\0';
				return TRUE;
			}
	}
	return FALSE;
}

#define  IDC_BTN_PREVITEM              3592 
#define  IDC_BTN_NEXTITEM              3593 
#define  IDC_STC_ITEM	               3594 
#define  IDC_TXT_CONTENT               3595 

BEGIN_MESSAGE_MAP(CCallIDInfo, CAppWnd)
  ON_MESSAGE(IDC_BTN_PREVITEM, CN_COMMAND, OnPrevItem)
  ON_MESSAGE(IDC_BTN_NEXTITEM, CN_COMMAND, OnNextItem)  
  ON_MESSAGE(IDC_STC_ITEM, CN_COMMAND, OnSpecifyItem)  
END_MESSAGE_MAP 

BOOL CCallIDInfo::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	char strTip[32];
	m_iItems = GetCallIDItems();
	m_iCurItem = 0;
	CRect r(m_sRect.left + 1 , m_sRect.top + 141, m_sRect.left + 74,
		 m_sRect.bottom -1);

	m_oBtnSpecifyItem.Create(NULL, r , this, IDC_STC_ITEM);
	r.left = r.right +1, r.right = r.left + 41;
	m_oBtnPrevItem.Create(NULL , ICON_PREVITEM , r, this, IDC_BTN_PREVITEM);

	r.left = r.right +1, r.right = m_sRect.right;
	m_oBtnNextItem.Create(NULL , ICON_NEXTITEM , r, this, IDC_BTN_NEXTITEM);
	r.SetRect(m_sRect.left , m_sRect.top + WND_TITLE_HEIGHT +1,
		m_sRect.right -1,	 m_sRect.top + 140);
	m_oTxtCallID.Create(NULL, r, this, WS_VISIBLE, IDC_TXT_CONTENT,
		        PHONE_CALLIDBUF_SEZE * 3, NULL);
	return TRUE;
}

void CCallIDInfo::OnPaint(CDC &dc)
{
	char strTemp[32];
	if(m_iItems != GetCallIDItems())
		m_iItems = GetCallIDItems();

	if(m_iItems == 0)
		strcpy(strTemp , "0/0");
	else
		sprintf(strTemp, "%d/%d", m_iCurItem +1 , m_iItems);
	m_oBtnSpecifyItem.SetWindowText(strTemp);

	CallIDInfo callid;
	if(GetSpecifyCallID(m_iCurItem, callid))
	{
		m_oTxtCallID.SetWindowText(callid.strCallID, FALSE);
		sprintf(m_strName , "%d-%d-%d,%d:%d:%d", callid.dt.year,callid.dt.month,
				callid.dt.day,callid.dt.hour,callid.dt.minute,callid.dt.second);
		int iLen = strlen(m_strName);
		strncpy(m_strName , m_strName + 2,  iLen -2);
		m_strName[iLen -2] ='\0';
	}
	else
	{
		m_oTxtCallID.SetWindowText("无来电记录", FALSE);
		m_strName[0] = '\0';
	}
	CAppWnd::OnPaint(dc);
}

/*          hqf     
	过滤掉除了0～9外的其他字符        
*/

void FilterIllegalChars(const char *strSrc, char *strDest)
{
	int iLen = strlen(strSrc);
	int i , j;
	for(i=0, j=0 ;i < iLen; i++)
		if(strSrc[i] >= 0x30 && strSrc[i] <= 0x39)
			strDest[j++] = strSrc[i];
	strDest[j] = '\0';
}

extern double ConvStrtonum(char *pStr);

void CCallIDInfo::OnSpecifyItem(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	char strInput[5], strLegal[5];
	if(m_iItems == 0)
		return;
	if(NumInputBox("请输入记录序号",strInput , 5) == IDOK)
	{
		FilterIllegalChars(strInput,strLegal);
		int nTemp = ConvStrtonum(strLegal);
		if(nTemp > m_iItems)
			m_iCurItem = m_iItems -1;
		else if(nTemp <=0 )
			m_iCurItem = 1;
		else
			m_iCurItem = nTemp -1;
		OnPaint(CDC());
	}
}

void CCallIDInfo::OnPrevItem(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_iCurItem > 0)
	{
		m_iCurItem--;
		OnPaint(CDC());
	}
}

void CCallIDInfo::OnNextItem(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	if(m_iCurItem < m_iItems -1)
	{
		m_iCurItem++;
		OnPaint(CDC());
	}
}

static CDebugMainWnd *g_pDebugWnd = NULL;
void ShowDebugInfo()
{
	if(!g_pDebugWnd)
		g_pDebugWnd = new CDebugMainWnd;
	g_pDebugWnd->Create("调试信息", WS_VISIBLE|WS_NORMAL, DESK_TOP_RECT);
	g_pDebugWnd->ShowWindow();
}