#ifndef _ALARM_H
#define _ALARM_H
#include "datadrv.h"
#include "datetime.h"
#include "grid.h"
#include "menu.h"
#include "softKey.h"
#include "jishi.h"
#define     ALARM_INDEX_KIND_TODAY  (BIT3|BIT4)     //�������ͣ�������
#define ALARM_DAILY_NOWEEK   0x10//��ĩ������
class CAlarm
{
private:   
	
	//ȡ����ʱ�䣭�����ظ�
	void CAlarm::GetAlarmWithDayRepeat(CDate NowDate);
	//ȡ����ʱ�䣭�����ظ�
	void CAlarm::GetAlarmWithWeekRepeat(CDate NowDate);
	//ȡ����ʱ�䣭�����ظ�
	void CAlarm::GetAlarmWithMonthRepeat(CDate NowDate);
	//ȡ����ʱ�䣭�����ظ�
	void CAlarm::GetAlarmWithYearRepeat(CDate NowDate);
public :
	CAlarm();
	void Reset();
	~CAlarm(){};
public:
	UINT16 Repeat;
	short TimeBef;//��ǰʱ��
	short WeekDay;
	//short TimeLater;//�Ӻ�ʱ��
	short Music;     //��������
	DateTime  When; //ʱ��
	DateTime  AlarmTime;//����ʱ��
	DateTime  EventTime;
	char  Todo[ALARM_TODO_LEN+1];
	char  Phone[ALARM_TITLE_LEN + 1];
	long  Reserved;
	long  RecordID;
	UINT16 ID;//������
public:
	DateTime GetRightTimeByMinute(DateTime OldTime,int nTimeOffset);
	BOOL Load(UINT16 AlarmID);//װ��
	BOOL Save();         //����
	BOOL Modify();       //�޸�
	BOOL Delete();       //ɾ��
	void Init(CDate NowDate);
	
	BOOL bIsTodayAlarm(CDate NowTime);//�ж��Ƿ��������
	//�ж��¼�ʱ���Ƿ��ڽ���
	BOOL bEvenTimeIsToday(CDate NowDate);
	//�ж��¼�ʱ���Ƿ��ڽ���--��������ǰʱ��
	BOOL IsToday();
	BOOL bIsThisMonthAlarm(CDate NowTime);//�ж��Ƿ�������
	//ȡ�����������ַ���
	void GetEventDateStr(char *szUnitText,BOOL Flag=TRUE);
	//ȡ������ʱ�䴮
	void GetTimeStr(char* szUnitText);
	//ȡ����������
	void GetAlarmKindStr(char* AlarmKindStr);
	//ȡ����ǰʱ�䴮
	void GetTimeBefStr(char *szUnitText);
	DateTime GetAlarmTime();//ȡ����ʱ��
	DateTime GetEventTime();//ȡ�¼�ʱ��
	void GetEventDateTime(INT16 &nYear,INT16 &nMonth,INT16 &nDay,INT16 &nHour,INT16 &nMinute);
	CAlarm & operator = (const CAlarm &srcAlarm);
	bool operator==(CAlarm const &Alarm);
};
//------------------------------------------------------------------
//------------------------------------------------------------------
//�������Ѳ���
//------------------------------------------------------------------
//------------------------------------------------------------------
#define ALARM_ALARMDISPLAY_MAX   10
class CAlarmList
{
private:
	int AlarmCount;//���Ѹ���
	int LaterAlarmCount;//�Ӻ����ѵĸ���
	UINT8 MonthList[31];//�����±�

	DateTime AlarmTime;//����ʱ��????

	CAlarm AlarmList[ALARM_ALARMDISPLAY_MAX+1];//���Ѷ���--����ʱ����ֻͬ�������ѵ�ID
	CAlarm LaterAlarmList[ALARM_ALARMDISPLAY_MAX+1];//�Ӻ�����Ѷ���
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
	UINT8* GetMonthList(CDate NowDate);//������Ѷ���
	CAlarm *GetAlarmList();
	int FindAlarmList();//�������Ѷ���
	void FindMonthAlarmList(CDate NowDate);//ȡ�������±�
	void SetAlarm();   //��������ʱ��
	
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//������������
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
	BOOL ModifyFlag;//�޸ı�־
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
	int FindIndexList();//ȡ��ĳ������������
	//�жϸ������Ƿ��Ѿ�����
	BOOL IsAlarmExist(CAlarm AlarmRecord);
};
class CAlarmDisplay :public CFrameWnd
{
private:
	
	CAlarm AlarmDisplayList[ALARM_ALARMDISPLAY_MAX+1];//��ʾ���Ѷ���
	ScreenBuf         * m_pBackScrBuf;//������Ļ
	CCombox  LaterTimeBox;     //�Ӻ�ʱ����Ͽ�
	CStatic  AlarmTextLable;   //��ʾ��������
	CButton  OkButton;
	CButton  CancelButton;
	BOOL DisplayFlag;
	BOOL RingFlag;//�����־
	BOOL RecordFlag;//����¼����־
	int UnDispAlarmCount;             //��ʾ���ѵĸ���
	
	void DrawAlarmTime(DateTime EventTime,CDC &dc);
	//��ʾ������Ϣ
	BOOL DrawAlarmInfo(CDC dc);

	//��������ʾ������ɾ��һ������
	BOOL DeleteAlarm(CAlarm *pAlarmList,int *AlarmCount,UINT16 AlarmID);
	//��������ʾ�����в���һ������
	void InsertAlarm(CAlarm AlarmRecord);

	//��ָ��������ʾ�����в���һ������
	void InsertAlarmToList(CAlarm *pAlarmList,int *AlarmCount,CAlarm AlarmRecord);

	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	
public:
	CAlarmDisplay()
	{
		UnDispAlarmCount=0;
		DisplayFlag=FALSE;
		RingFlag=FALSE;
		RecordFlag=FALSE;//����¼����־
	};
	~CAlarmDisplay(){};
	virtual BOOL     Create();
	virtual void PaintTitle(CDC &dc);
	virtual void ShowWindow(UINT32 nCmdShow);
	//������ʾ���Ѷ���
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
	//����������Ϣ 
};


//---------------------------------------------------------------
//---------------------------------------------------------------
//�½����Ѵ��ڣ����޸�
//---------------------------------------------------------------
//---------------------------------------------------------------
class CCreateNewAlarm :public CAppWnd 
{
private:
	BOOL ModifyFlag;//���ӻ��޸ı�־ TRUE:modify FALSE :new
	BOOL SaveFlag;//true:save,false:not save;
	BOOL RingFlag;//true:�������壬false:��ǰû�в�������
	BOOL ExitFlag;//TRUE:save;  FALSE: cancel;
	CAlarm AlarmRecord;
	CIconButton SaveButton;//�����˳���ť
	CGrid NewAlarmGrid;
	CPopUpList  MusicBox;     //������Ͽ�
	CPopUpList  WeekBox;     //������Ͽ�
	void SetDisplayText();
	char WndTitle[60];//���ڱ���
	void SetWndTitle();
	char *GetWndTitle(){ return WndTitle;};
	//�洢һ������
	void SaveAlarm();
public:
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	void OnGridComboxClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam);

	void OnListSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	void OnWeekListSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
public:
	DECLARE_MESSAGE_MAP()
	BOOL Create(UINT16 nRepeatKind,UINT16 AlarmID,BOOL Flag);//����
	virtual void OnCancel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	MESSAGE_HANDLE void OnSave(UINT32  nMessage , UINT32 wParam , UINT32 lParam);
	virtual void WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam);
	virtual void OnDactive();
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//����������ڣ������
//---------------------------------------------------------------
//---------------------------------------------------------------
class CAlarmView :public CAppWnd 
{
private:
	CAlarm AlarmRecord;
	CButton ModifyButton;//�޸İ�ť
	CButton DeleteButton;//ɾ����ť
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
//������������
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
//������������
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
	//���ǵ�ǰ����
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
//���������ؼ�
//---------------------------------------------------------------
//---------------------------------------------------------------

class CAlarmCalendar :public CCalendar
{
private:
	UINT8 *pAlarmMonthList;
public:
	//ȡ�������±�ʾ��
	void SetAlarmMonthFlagList();
protected:
	virtual void PaintOneDay(CDC &dc, INT16 nDay, UINT32 nIconId, INT16 startx, INT16 starty);
};
//---------------------------------------------------------------
//---------------------------------------------------------------
//������������
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
	//��ʾ��������
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
//����������
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


//ȡ�õ�ǰʱ�̵ȴ��ı������� 
int Alarm_GetAlarmCount();
//���ñ���
void Alarm_SetAlarm();
//ȡ�õ�ǰʱ�̵ȴ��ı�������  
CAlarm *Alarm_GetAlarmList();
//�����Ѵ��Ӻ����Ѷ�����ɾ��
void Alarm_DeleteFromLaterAlarmList(UINT16 ID);
//�����Ѳ����Ӻ����Ѷ���
void Alarm_InsertToLaterAlarmList(CAlarm AlarmRecord);
//ȡ�ø������ѱ�
UINT8 *Alarm_GetMonthList(CDate NowDate);
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//���ظ�ʱѡ����ؼ�
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