#ifndef _ALARM_H
#define _ALARM_H
#include "datadrv.h"
#include "datetime.h"
#include "grid.h"
#include "menu.h"
#include "softKey.h"
#include "jishi.h"
#define     ALARM_INDEX_KIND_TODAY  (BIT3|BIT4)     //索引类型－－当天
#define ALARM_DAILY_NOWEEK   0x10//周末不提醒
class CAlarm
{
private:   
	
	//取提醒时间－－天重复
	void CAlarm::GetAlarmWithDayRepeat(CDate NowDate);
	//取提醒时间－－周重复
	void CAlarm::GetAlarmWithWeekRepeat(CDate NowDate);
	//取提醒时间－－月重复
	void CAlarm::GetAlarmWithMonthRepeat(CDate NowDate);
	//取提醒时间－－年重复
	void CAlarm::GetAlarmWithYearRepeat(CDate NowDate);
public :
	CAlarm();
	void Reset();
	~CAlarm(){};
public:
	UINT16 Repeat;
	short TimeBef;//提前时间
	short WeekDay;
	//short TimeLater;//延后时间
	short Music;     //响铃设置
	DateTime  When; //时间
	DateTime  AlarmTime;//提醒时间
	DateTime  EventTime;
	char  Todo[ALARM_TODO_LEN+1];
	char  Phone[ALARM_TITLE_LEN + 1];
	long  Reserved;
	long  RecordID;
	UINT16 ID;//所引号
public:
	DateTime GetRightTimeByMinute(DateTime OldTime,int nTimeOffset);
	BOOL Load(UINT16 AlarmID);//装载
	BOOL Save();         //保存
	BOOL Modify();       //修改
	BOOL Delete();       //删除
	void Init(CDate NowDate);
	
	BOOL bIsTodayAlarm(CDate NowTime);//判断是否今天提醒
	//判断事件时间是否在今天
	BOOL bEvenTimeIsToday(CDate NowDate);
	//判断事件时间是否在今天--不包含提前时间
	BOOL IsToday();
	BOOL bIsThisMonthAlarm(CDate NowTime);//判断是否本月提醒
	//取得提醒日期字符串
	void GetEventDateStr(char *szUnitText,BOOL Flag=TRUE);
	//取得提醒时间串
	void GetTimeStr(char* szUnitText);
	//取的提醒类型
	void GetAlarmKindStr(char* AlarmKindStr);
	//取得提前时间串
	void GetTimeBefStr(char *szUnitText);
	DateTime GetAlarmTime();//取提醒时间
	DateTime GetEventTime();//取事件时间
	void GetEventDateTime(INT16 &nYear,INT16 &nMonth,INT16 &nDay,INT16 &nHour,INT16 &nMinute);
	CAlarm & operator = (const CAlarm &srcAlarm);
	bool operator==(CAlarm const &Alarm);
};
//------------------------------------------------------------------
//------------------------------------------------------------------
//设置提醒部分
//------------------------------------------------------------------
//------------------------------------------------------------------
#define ALARM_ALARMDISPLAY_MAX   10
class CAlarmList
{
private:
	int AlarmCount;//提醒个数
	int LaterAlarmCount;//延后提醒的个数
	UINT8 MonthList[31];//提醒月表

	DateTime AlarmTime;//提醒时间????

	CAlarm AlarmList[ALARM_ALARMDISPLAY_MAX+1];//提醒队列--提醒时间相同只保存提醒的ID
	CAlarm LaterAlarmList[ALARM_ALARMDISPLAY_MAX+1];//延后的提醒队列
public :
	CAlarmList()
	{
		AlarmCount=0;
		LaterAlarmCount=0;
	};
	~CAlarmList(){};
	
private:
	BOOL InsertToAlarmList(CAlarm Alarm);
public:
	int GetAlarmCount();
	int GetLaterAlarmCount();
	BOOL DeleteFormLaterAlarmList(UINT16 ID);
	BOOL InsertToLaterAlarmList(CAlarm Alarm);
	UINT8* GetMonthList(CDate NowDate);//获得提醒队列
	CAlarm *GetAlarmList();
	int FindAlarmList();//设置提醒队列
	void FindMonthAlarmList(CDate NowDate);//取得提醒月表
	void SetAlarm();   //设置提醒时间
	
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//提醒索引部分
//---------------------------------------------------------------
//---------------------------------------------------------------

struct AlarmIndex
{
	DateTime EventTime;
	UINT16  AlarmID;

};
class CAlarmIndexList
{
private:
	AlarmIndex DayAlarmIndex[ALARM_MAX_COUNT+1];
	int AlarmIndexCount;
	
	UINT8 IndexKind;
	
public:
	CAlarmIndexList()
	{
		AlarmIndexCount=0;
		IndexKind=0;
		ModifyFlag=FALSE;
	};
	~CAlarmIndexList(){};
public:
	CDate IndexDate;
	BOOL ModifyFlag;//修改标志
	void Init(CDate NowDate,UINT8 nKind)
	{
		AlarmIndexCount=0;
		IndexDate=NowDate;
		IndexKind=nKind;
		ModifyFlag=FALSE;
	};
	AlarmIndex* GetAlarmIndex();
	CDate GetAlarmIndexDate();
	int GetAlarmIndexCount();
	UINT8 GetAlarmIndexKind();
	void Insert(CAlarm Alarm);
	void Delete(UINT16 AlarmID);
	void DeleteAll()
		{
			AlarmIndexCount=0;
			ModifyFlag=FALSE;
		};
	void Modify(CAlarm Alarm);
	int FindIndexList();//取得某日提醒索引表
	//判断该提醒是否已经存在
	BOOL IsAlarmExist(CAlarm AlarmRecord);
};
class CAlarmDisplay :public CFrameWnd
{
private:
	
	CAlarm AlarmDisplayList[ALARM_ALARMDISPLAY_MAX+1];//显示提醒队列
	ScreenBuf         * m_pBackScrBuf;//保存屏幕
	CCombox  LaterTimeBox;     //延后时间组合框
	CStatic  AlarmTextLable;   //显示提醒文字
	CButton  OkButton;
	CButton  CancelButton;
	BOOL DisplayFlag;
	BOOL RingFlag;//响铃标志
	BOOL RecordFlag;//播放录音标志
	int UnDispAlarmCount;             //显示提醒的个数
	
	void DrawAlarmTime(DateTime EventTime,CDC &dc);
	//显示提醒信息
	BOOL DrawAlarmInfo(CDC dc);

	//从提醒显示队列中删除一个提醒
	BOOL DeleteAlarm(CAlarm *pAlarmList,int *AlarmCount,UINT16 AlarmID);
	//向提醒显示队列中插入一个提醒
	void InsertAlarm(CAlarm AlarmRecord);

	//向指定提醒显示队列中插入一个提醒
	void InsertAlarmToList(CAlarm *pAlarmList,int *AlarmCount,CAlarm AlarmRecord);

	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	
public:
	CAlarmDisplay()
	{
		UnDispAlarmCount=0;
		DisplayFlag=FALSE;
		RingFlag=FALSE;
		RecordFlag=FALSE;//播放录音标志
	};
	~CAlarmDisplay(){};
	virtual BOOL     Create();
	virtual void PaintTitle(CDC &dc);
	virtual void ShowWindow(UINT32 nCmdShow);
	//设置显示提醒队列
	BOOL SetAlarmDispList();
	BOOL IsDisplay(){return DisplayFlag;};

	int GetNotDispAlarmCount(){return UnDispAlarmCount;};

	virtual  BOOL OnCreate();
	void OnOk();
	virtual  void OnPaint(CDC &dc);
	void OnCancel();
	virtual void Destroy();
	virtual void OnDactive();
    DECLARE_MESSAGE_MAP(CAlarmDisplay);
	MESSAGE_HANDLE void OnAlarm(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	MESSAGE_HANDLE void OnRecord(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam);
	//处理提醒消息 
};


//---------------------------------------------------------------
//---------------------------------------------------------------
//新建提醒窗口－－修改
//---------------------------------------------------------------
//---------------------------------------------------------------
class CCreateNewAlarm :public CAppWnd 
{
private:
	BOOL ModifyFlag;//增加或修改标志 TRUE:modify FALSE :new
	BOOL SaveFlag;//true:save,false:not save;
	BOOL RingFlag;//true:正在振铃，false:当前没有播放振铃
	BOOL ExitFlag;//TRUE:save;  FALSE: cancel;
	CAlarm AlarmRecord;
	CIconButton SaveButton;//保存退出按钮
	CGrid NewAlarmGrid;
	CPopUpList  MusicBox;     //音乐组合框
	CPopUpList  WeekBox;     //音乐组合框
	void SetDisplayText();
	char WndTitle[60];//窗口标题
	void SetWndTitle();
	char *GetWndTitle(){ return WndTitle;};
	//存储一条提醒
	void SaveAlarm();
public:
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	void OnGridComboxClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);

	void OnListSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	void OnWeekListSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
public:
	DECLARE_MESSAGE_MAP()
	BOOL Create(UINT16 nRepeatKind,UINT16 AlarmID,BOOL Flag);//增加
	virtual void OnCancel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnSave(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	virtual void OnDactive();
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//提醒浏览窗口－－浏览
//---------------------------------------------------------------
//---------------------------------------------------------------
class CAlarmView :public CAppWnd 
{
private:
	CAlarm AlarmRecord;
	CButton ModifyButton;//修改按钮
	CButton DeleteButton;//删除按钮
	CGrid AlarmViewGrid;
	CCreateNewAlarm *ModifyAlarmWnd;
	BOOL RingFlag;
	void SetDisplayText();
public:
	virtual BOOL OnCreate();
	virtual void OnActive();
	virtual BOOL OnDestroy();
	virtual void OnDactive();
	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
public:
	DECLARE_MESSAGE_MAP()
	BOOL Create(UINT16 AlarmID);
	MESSAGE_HANDLE void OnModify(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnDelete(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//分类索引窗口
//---------------------------------------------------------------
//---------------------------------------------------------------
class CRepeatFindList :public CAppWnd 
{
private:
	UINT16 nFindKind;
	CButton NewButton;
	CButton DeleteButton;
	CGrid ListGrid;
	CCreateNewAlarm *CreateAlarmWnd;
	CAlarmView      *AlarmViewWnd;
	void GetWindowTitleAndHelpID(UINT16 nKind,char *WndTitle,UINT32 *pHelpID);
protected:
	virtual BOOL OnCreate();
	virtual void OnActive();
	virtual BOOL OnDestroy();
public:
	DECLARE_MESSAGE_MAP()
	void SetDisplayText();
	BOOL Create(UINT16 nKind);
	virtual void OnPaint(CDC &dc);
	MESSAGE_HANDLE void OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnCreatNewAlarm(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnDeleteAlarm(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//今日索引窗口
//---------------------------------------------------------------
//---------------------------------------------------------------
class CTodayAlarmList :public CAppWnd 
{
private:
	CDate NowDate;
	CIconButton NextDayButton;
	CIconButton PrevDayButton;
	CAlarmView  *AlarmViewWnd;
	CGrid ListGrid;
	void SetDisplayText();
	//先是当前日期
	void DisplayDate(CDC &dc);
protected:
	virtual BOOL OnCreate();

	virtual BOOL OnDestroy();
	virtual void OnActive();
public:
	BOOL bMonthFlag;
	DECLARE_MESSAGE_MAP()
	BOOL Create(CDate Date);
	CDate GetNowDate();
	void SetNowDate(CDate Date);
	virtual void OnPaint(CDC &dc);
	MESSAGE_HANDLE void OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnPrevDay(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnNextDay(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//提醒日历控件
//---------------------------------------------------------------
//---------------------------------------------------------------

class CAlarmCalendar :public CCalendar
{
private:
	UINT8 *pAlarmMonthList;
public:
	//取得提醒月标示表
	void SetAlarmMonthFlagList();
protected:
	virtual void PaintOneDay(CDC &dc, INT16 nDay, UINT32 nIconId, INT16 startx, INT16 starty);
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//今月索引窗口
//---------------------------------------------------------------
//---------------------------------------------------------------

class CMonthAlarmList :public CAppWnd 
{
private:
	CDate NowDate;
	CAlarmCalendar m_Calendar;
	CIconButton PrevMonthButton;
	CIconButton NextMonthButton;
	CTodayAlarmList *Month_TodayAlarmWnd;
	//显示公历日期
	void OnPaintMonthText(CDC &dc);
protected:

	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
public:
	DECLARE_MESSAGE_MAP()
	BOOL Create(CDate Date);
	virtual void OnPaint(CDC &dc);
	MESSAGE_HANDLE void OnGetDay(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnPrevMonth(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnNextMonth(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
};

//---------------------------------------------------------------
//---------------------------------------------------------------
//提醒主界面
//---------------------------------------------------------------
//---------------------------------------------------------------
class CAlarmMain :public CAppWnd 
{
private:
	CButton OneAlarmButton;
	CButton DayAlarmButton;
	CButton WeekAlarmButton;
	CButton MonthAlarmButton;
	CButton YearAlarmButton;

    CButton TodayAlarmButton;
	CButton ThisMonthAlarmButton;

	CRepeatFindList *RepeatFindWnd;
	CTodayAlarmList *TodayAlarmWnd;
	CMonthAlarmList *MonthAlarmWnd;
protected:
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
public:
	DECLARE_MESSAGE_MAP()
	BOOL Create();
	virtual void OnPaint(CDC &dc);
	MESSAGE_HANDLE void OnAlarmMainButton(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	
};
#define IDM_ONEALARM_INDEX    0x10
#define IDM_DAYALARM_INDEX    0x11
#define IDM_WEEKALARM_INDEX   0x12
#define IDM_MONTHALARM_INDEX  0x13
#define IDM_YEARALARM_INDEX   0x14
#define IDM_TODAYALARM_INDEX  0x15
#define IDM_THISMONTHALARM_INDEX 0x16


//取得当前时刻等待的报警个数 
int Alarm_GetAlarmCount();
//设置报警
void Alarm_SetAlarm();
//取得当前时刻等待的报警队列  
CAlarm *Alarm_GetAlarmList();
//将提醒从延后提醒队列中删除
void Alarm_DeleteFromLaterAlarmList(UINT16 ID);
//将提醒插入延后提醒队列
void Alarm_InsertToLaterAlarmList(CAlarm AlarmRecord);
//取得该月提醒表
UINT8 *Alarm_GetMonthList(CDate NowDate);
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//月重复时选择天控件
//-------------------------------------------------------------------
//-------------------------------------------------------------------
#define SELECTDAY_COL_WIDTH  20
#define SELECTDAY_ROW_HEIGHT 16  
class CSelectDayCtrl : public CCtrl
{
private:
	INT16 nSelectDay;
	CRect m_SelectRect;
	UINT16 CSelectDayCtrl::CalcPenDay(CPoint &pt);
	void CSelectDayCtrl::SetSelectRect(UINT16 nDay);
public:
	BOOL Create(INT16 nDay,CPoint &ptLeftTop, CFrameWnd *pParent, UINT32 nCtrlId);
	void SetSelectDay(INT16 nDay);
	void GetSelectDay(INT16 &nDay);
	virtual void OnPaint(CDC &dc);
	virtual void WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam);
};
//
class CSelectDayDialog:public CDialog
{
private:
	INT16 m_nSelectDay;
	CSelectDayCtrl m_SelectDayCtrl;
	CIconButton m_obutOk;
public:
	virtual BOOL Create(INT16 nDay,char *StrName);
	virtual BOOL OnCreate();
	void  GetSelectDay(INT16 &nDay);
};
void SelectDayBox(INT16 &nDay,char *StrName);
#endif