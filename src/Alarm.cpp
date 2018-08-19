#include "window.h"
#include "datadrv.h"
#include "Alarm.h"
#include "edit.h"
#include "Messagebox.h"
#include "dc.h"
#include "ring.h"
#include "keydrv.h"
#include "phonedrv.h"
#include "record.h"
#include "phoneapp.h"
static CAlarmMain *g_AlarmApp=NULL;
static CAlarmIndexList *g_AlarmIndexList=NULL;
static CAlarmList *g_pAlarmList=NULL;
CAlarmDisplay *g_pAlarmDisplay=NULL;
////////////// history////////////////////////////
//
// 2001/12/4, chengxiaodong, modify CharToShort()
// 2001/12/6  程晓东修改增加窗口中日期项的设置，修改主窗口中的显示
// 2001/1210  程晓东增加月重复天选择模块，修改记录满死机问题,处理日历控件中反黑问题
// 2001/12/17 程晓东修改主界面，提醒增加内容为空处理，提醒浏览不显示无效项
//            今日提醒增加表头线，修改振铃试听，改正不能修改提醒音乐,处理延后提醒
// 2001/12/18 程晓东 修改浏览时如果记录满不能修改的问题,修改播放录音时弹提醒的问题 
//2001/12/19  程晓东 修改周重复浏览不能正确显示星期的问题，修改新建提醒存盘退出
//2001/12/27  程晓东 修改每日提醒增加工作日提醒，修改增加提醒，去除电话
//2001/12/29  程晓东 修改在特殊情况下提醒的弹出，修改振铃试听
//2001/12/31  程晓东修改提醒零点不提醒问题
//------------------------------------------------------------------- 
//-------------------------------------------------------------------
//提醒类实现 
//-------------------------------------------------------------------
//-------------------------------------------------------------------	

const char Alarm_WeekDayStr[][3]={"日","一","二","三","四","五","六"};
const char szAlarmMainText[][9]=
	{"提醒","提醒分类","提醒总览","一次提醒","每日提醒","每周提醒"
	 ,"每月提醒","每年提醒","今日提醒","本月提醒"};

//判断是否可以增加新提醒
BOOL IsCanAddNewAlarm()
{
	int RecordCount;//存储的提醒个数
	short AlarmIDBuf[ALARM_MAX_COUNT+2];
	RecordCount=FindAlarm(AlarmIDBuf, ALARM_MAX_COUNT); // 返回查到个数
	if(RecordCount>=ALARM_MAX_COUNT)
	{
		::MessageBox("提醒空间已满！","新建", MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;
}

CAlarm::CAlarm()
{
	Reset();
}

void CAlarm::Reset()
{
	GetSysTime(&When);
	When.second=0;
	WeekDay=CDate::WeekDay(When.year,When.month,When.day);
	ID=0;
	EventTime=When;
	Music=1;
	Repeat=ALARM_ONE;
	TimeBef=0;
	RecordID=0;
	Reserved=0;//用于判断是否为延后提醒
	memset((char*)Phone,0x00,ALARM_TITLE_LEN);
	memset((char*)Todo,0x00,ALARM_TODO_LEN);
}
CAlarm &CAlarm::operator = (const CAlarm &srcAlarm)
{
	memcpy(this, &srcAlarm, sizeof(CAlarm));
	return *this;
}
bool CAlarm::operator == (CAlarm const &Alarm)
{
	if(!(When==Alarm.When))
		return FALSE;
	if(WeekDay!=Alarm.WeekDay)
		return FALSE;
	if(ID!=Alarm.ID)
		return FALSE;
	if(!(EventTime==Alarm.EventTime))
		return FALSE;
	if(Music!=Alarm.Music)
		return FALSE;
	if(Repeat!=Alarm.Repeat)
		return FALSE;
	if(TimeBef!=Alarm.TimeBef)
		return FALSE;
	if(RecordID!=Alarm.RecordID)
		return FALSE;
	if(Reserved!=Alarm.Reserved)
		return FALSE;
	if(strcmp((char*)Phone,Alarm.Phone)!=0)
		return FALSE;
	if(strcmp((char*)Todo,Alarm.Todo)!=0)
		return FALSE;
	return TRUE;
}
DateTime CAlarm::GetRightTimeByMinute(DateTime OldTime,int nTimeOffset)
{
	int InMinute=0,Hour=0,Day=0,InHour;
	CDate NewDate;
	
	if(nTimeOffset==0)
		return OldTime;

	nTimeOffset=-1*nTimeOffset;//所有时间为提前时间

	NewDate.SetDate(OldTime.year,OldTime.month,OldTime.day);
	InMinute=OldTime.minute;
	InHour=OldTime.hour;

	InMinute+=nTimeOffset;
	Hour=InMinute/60;
	InMinute=InMinute%60;
	if(InMinute<0)
	{
		InMinute+=60;
		Hour-=1;
	}
	InHour+=Hour;
	InHour=InHour%24;
	Day=InHour/24;
	if(InHour<0)
	{
		InHour+=24;
		Day-=1;
	}
	if(Day!=0)
		NewDate+=Day;
	OldTime.year=NewDate.GetYear();
	OldTime.month=NewDate.GetMonth();
	OldTime.day=NewDate.GetDay();
	OldTime.hour=InHour;
	OldTime.minute=InMinute;
	return OldTime;
}


//装载
BOOL CAlarm::Load(UINT16 AlarmID)
{
	ALARM *NewAlarm; 
	ID=AlarmID;
	if(ID<0)
	{
		Reset();
		//::MessageBox("Alarm ID < 0","Alarm Load Err",MB_OK);
		return FALSE;
	}
	NewAlarm=GetAlarmPointer(ID);
	if(NewAlarm==NULL)
	{
		Reset();
		//::MessageBox("Alarm","Alarm Load Err",MB_OK);
		return FALSE;
	}
	Repeat=NewAlarm->MinBef;
    When=NewAlarm->When;
	Music=NewAlarm->Music;
	TimeBef=NewAlarm->HourBef;
	RecordID=NewAlarm->Reserved;
	Reserved=0;//用于判断是否为延后提醒
	strcpy((char*)Todo,(char*)NewAlarm->Todo);
	strcpy((char*)Phone,(char*)NewAlarm->Title);
	//cxd add主要针对日重复时周末不提醒
	WeekDay=NewAlarm->When.second;
	When.second=0;
	return TRUE;
}
//保存
BOOL CAlarm::Save() 
{
	ALARM NewAlarm; 
	NewAlarm.HourBef=TimeBef;
	NewAlarm.MinBef=Repeat;
	NewAlarm.Music=Music;
	NewAlarm.When=When;
	NewAlarm.When.second=WeekDay;
	NewAlarm.Reserved=RecordID;
	
	//NewAlarm.Reserved=Repeat;
	strcpy((char*)(NewAlarm.Todo),(char*)Todo);
	strcpy((char*)(NewAlarm.Title),(char*)Phone);
	ID=AppendAlarm((ALARM *)&NewAlarm);	
	if(ID<0)
	{
		ID=0;
		::MessageBox("提醒空间已满！","新建", MB_OK|MB_ICONINFORMATION);	// byw
		return FALSE;
	}
	return TRUE;
}
//修改
BOOL CAlarm::Modify()
{
	BOOL ReturnFlag=TRUE;
	ALARM NewAlarm; 
	NewAlarm.HourBef=TimeBef;
	NewAlarm.MinBef=Repeat;
	NewAlarm.Music=Music;
	NewAlarm.When=When;
	NewAlarm.When.second=WeekDay;
	NewAlarm.Reserved=RecordID;
	//NewAlarm.Reserved=Repeat;
	strcpy((char*)(NewAlarm.Todo),(char*)Todo);
	strcpy((char*)(NewAlarm.Title),(char*)Phone);
	ReturnFlag=ModifyAlarm(ID, &NewAlarm); // 成功返回true, 失败返回false
	if(ReturnFlag==FALSE)
		::MessageBox("修改提醒出错！","修改",MB_OK|MB_ICONSTOP);
	return ReturnFlag;
	
}
//取得提醒日期字符串
//12-27 cxd modify,add BOOL Flag 
//Flag :TRUE 完整字符  :FALSE 短字符串
void CAlarm::GetEventDateStr(char *szUnitText,BOOL Flag)
{
	switch(Repeat)
	{
	case ALARM_ONE:
		if(Flag==TRUE)
			sprintf((char*)szUnitText,"%d年%02d月%02d日",When.year,When.month,When.day);		
		else
			sprintf((char*)szUnitText,"%d/%02d/%02d",When.year,When.month,When.day);		
		break;
	case ALARM_DAILY:
		if(WeekDay==ALARM_DAILY_NOWEEK)
			sprintf((char*)szUnitText,"%s","工作日");		
		else
			sprintf((char*)szUnitText,"%s","每日");		
		break;
	case ALARM_WEEKLY:
		//WeekDay=CDate::WeekDay(When.year,When.month,When.day);
		sprintf((char*)szUnitText,"%s%s","每周",(char*)(Alarm_WeekDayStr[WeekDay]));		
		break;
	case ALARM_MONTHLY:
		sprintf((char*)szUnitText,"%s%02d%s","每月",When.day,"日");		
		break;
	case ALARM_ANNUALLY:
		sprintf((char*)szUnitText,"%s%d%s%02d%s","每年",When.month,"月",When.day,"日");		
		break;	
	}

}
//取的提醒类型
void CAlarm::GetAlarmKindStr(char* AlarmKindStr)
{
	switch(Repeat)
	{
	case ALARM_ONE:
		sprintf((char*)AlarmKindStr,"%s","一次提醒");		
		break;
	case ALARM_DAILY:
		sprintf((char*)AlarmKindStr,"%s","每日提醒");		
		break;
	case ALARM_WEEKLY:
		sprintf((char*)AlarmKindStr,"%s","每周提醒");		
		break;
	case ALARM_MONTHLY:
		sprintf((char*)AlarmKindStr,"%s","每月提醒");		
		break;
	case ALARM_ANNUALLY:
		sprintf((char*)AlarmKindStr,"%s","每年提醒");		
		break;	
	}

}
//取得提醒时间串
void CAlarm::GetTimeStr(char* szUnitText)
{
	sprintf((char*)szUnitText,"%02d:%02d",When.hour,When.minute);
}
//取得提前时间串
void CAlarm::GetTimeBefStr(char *szUnitText)
{
	sprintf((char*)szUnitText,"%d分钟",TimeBef);
}
//删除
BOOL CAlarm::Delete()
{
	BOOL ReturnFlag;
	ReturnFlag=DeleteAlarm(ID);	// 成功返回true, 失败返回false
	if(ReturnFlag==FALSE)
		::MessageBox("删除提醒失败！","删除", MB_OK|MB_ICONSTOP);
	return ReturnFlag;
}


CDate GetNextMonth(CDate NowDate)
{
	int MonthDayCount;
	UINT16 Year, Month, Day;
	NowDate.GetDate(Year,Month,Day);
	Month++;
	if(Month>12)
	{
		Year++;
		Month-=12;
	}
	MonthDayCount=CDate::MonthDays(Year,Month);
	if(Day>MonthDayCount)//如果该日大于本月最大天数，一如何处理？？
	{
		Month++;
		if(Month>12)
		{
			Year++;
			Month-=12;
		}

	}
	CDate ReturnDate;
	ReturnDate.SetDate(Year,Month,Day);
	return ReturnDate;
}
//取提醒时间－－天重复
//12-27 cxd modify主要针对周末不提醒
void CAlarm::GetAlarmWithDayRepeat(CDate NowDate)
{
	EventTime=When;
	EventTime.year=NowDate.GetYear();
	EventTime.month=NowDate.GetMonth();
	EventTime.day=NowDate.GetDay();
	AlarmTime=GetRightTimeByMinute(EventTime,TimeBef);
	if(AlarmTime.day!=NowDate.GetDay())//提醒时间在前一天
	{
		AlarmTime.day=NowDate.GetDay();
		NowDate+=1;
		EventTime.day=NowDate.GetDay();
		EventTime.month=NowDate.GetMonth();
		EventTime.year=NowDate.GetYear();
	}
	if(WeekDay==ALARM_DAILY_NOWEEK)//处理周末不提醒
	{
		UINT8 AlarmWeek,LaterDays=0;//LaterDays:如果是周末，延后的天数
		AlarmWeek=CDate::WeekDay(AlarmTime.year, AlarmTime.month, AlarmTime.day);
		switch(AlarmWeek)
		{
		case 0://周日
			LaterDays=1;
			break;
		case 6://周六
			LaterDays=2;
			break;
		}
		NowDate.SetDate(EventTime.year,EventTime.month,EventTime.day);
		NowDate+=LaterDays;
		EventTime.day=NowDate.GetDay();
		EventTime.month=NowDate.GetMonth();
		EventTime.year=NowDate.GetYear();
		AlarmTime=GetRightTimeByMinute(EventTime,TimeBef);
	}

}
//取提醒时间－－周重复
void CAlarm::GetAlarmWithWeekRepeat(CDate NowDate)
{
	EventTime=When;
	int Week;
	Week=NowDate.WeekDay();
	Week=(WeekDay-Week);
	if(Week<0)
	{
		Week=Week+7;
	}
	NowDate+=Week;
	EventTime.year=NowDate.GetYear();
	EventTime.month=NowDate.GetMonth();
	EventTime.day=NowDate.GetDay();
	AlarmTime=GetRightTimeByMinute(EventTime,TimeBef);
		
}
//取提醒时间－－月重复
void CAlarm::GetAlarmWithMonthRepeat(CDate NowDate)
{
	int MonthDayCount;
	EventTime=When;
	MonthDayCount=CDate::MonthDays(NowDate.GetYear(),NowDate.GetMonth());
	if(When.day>MonthDayCount)
		NowDate=GetNextMonth(NowDate);
	EventTime.year=NowDate.GetYear();
	EventTime.month=NowDate.GetMonth();
	EventTime.day=When.day;
	AlarmTime=GetRightTimeByMinute(EventTime,TimeBef);
	if(AlarmTime.month!=NowDate.GetMonth())//提前到上个月
	{
		NowDate=GetNextMonth(NowDate);
		EventTime.year=NowDate.GetYear();
		EventTime.month=NowDate.GetMonth();
		EventTime.day=When.day;
		AlarmTime=GetRightTimeByMinute(EventTime,TimeBef);
	}
}
//取提醒时间－－年重复
void CAlarm::GetAlarmWithYearRepeat(CDate NowDate)
{
	int MonthDayCount;
	EventTime=When;
	EventTime.year=NowDate.GetYear();
		
	MonthDayCount=CDate::MonthDays(EventTime.year,EventTime.month);
	while(EventTime.day>MonthDayCount)//处理2月
	{
		EventTime.year++;
		MonthDayCount=CDate::MonthDays(EventTime.year,EventTime.month);
	}
	AlarmTime=GetRightTimeByMinute(EventTime,TimeBef);
	if(AlarmTime.year!=EventTime.year)//处理提前
	{
		AlarmTime.year=EventTime.year;
		EventTime.year++;
	}
}
void CAlarm::Init(CDate NowDate)
{
	int MonthDayCount;
	EventTime=When;
	switch(Repeat)
	{
	case ALARM_DAILY://天
		GetAlarmWithDayRepeat(NowDate);
		break;
	case ALARM_WEEKLY://周
		GetAlarmWithWeekRepeat(NowDate);
		break;
	case ALARM_MONTHLY://月
		GetAlarmWithMonthRepeat(NowDate);
		break;
	case ALARM_ANNUALLY://年
		CAlarm::GetAlarmWithYearRepeat(NowDate);
		break;
	case ALARM_ONE://一次
		AlarmTime=GetRightTimeByMinute(EventTime,TimeBef);
		break;
	}
}


//判断提醒时间是否今天提醒
BOOL CAlarm::bIsTodayAlarm(CDate NowTime)
{
	if(AlarmTime.year==NowTime.GetYear()&&AlarmTime.month==NowTime.GetMonth()&&AlarmTime.day==NowTime.GetDay())
		return TRUE;
	return FALSE;
}
//判断事件时间是否在今天
BOOL CAlarm::bEvenTimeIsToday(CDate NowDate)
{
	if(EventTime.year==NowDate.GetYear()&&EventTime.month==NowDate.GetMonth()&&EventTime.day==NowDate.GetDay())
		return TRUE;
	return FALSE;
}
//判断事件时间是否在今天--不包含提前时间
BOOL CAlarm::IsToday()
{
	CDate NowDate;
	NowDate=GetSysDate();
	switch(Repeat)
	{
	case ALARM_DAILY://天
		return TRUE;
	case ALARM_WEEKLY://周
		if(WeekDay==NowDate.WeekDay())
			return TRUE;
		break;
	case ALARM_MONTHLY://月
		if(When.day==NowDate.GetDay())
			return TRUE;
		break;
	case ALARM_ANNUALLY://年
		if(When.month==NowDate.GetMonth()&&When.day==NowDate.GetDay())
			return TRUE;
		break;
	case ALARM_ONE://一次
		if(When.year==NowDate.GetYear()&&When.month==NowDate.GetMonth()&&When.day==NowDate.GetDay())
			return TRUE;
		break;
	}
	return FALSE;
}

//判断是否本月提醒
BOOL CAlarm::bIsThisMonthAlarm(CDate NowTime)
{
	if(EventTime.year==NowTime.GetYear()&&EventTime.month==NowTime.GetMonth())
		return TRUE;
	else
		return FALSE;
}


//取提醒时间
DateTime CAlarm::GetAlarmTime()
{
	return AlarmTime;
}
//取事件时间
DateTime CAlarm::GetEventTime()
{
	return EventTime;
}
void CAlarm::GetEventDateTime(INT16 &nYear,INT16 &nMonth,INT16 &nDay,INT16 &nHour,INT16 &nMinute)
{
	nYear=EventTime.year;
	nMonth=EventTime.month;
	nDay=EventTime.day;
	nHour=EventTime.hour;
	nMinute=EventTime.minute;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//提醒数组类实现
//-------------------------------------------------------------------
//-------------------------------------------------------------------	
BOOL CAlarmList::InsertToAlarmList(CAlarm Alarm)
{
	DateTime NewAlarmTime;
	NewAlarmTime=Alarm.GetAlarmTime();
	if(NewAlarmTime.hour==0&&NewAlarmTime.minute==0)
	{
		NewAlarmTime.second=10;//cxd add 处理零点提醒
		Alarm.AlarmTime.second=10;
	}
	if(NewAlarmTime>AlarmTime)
	{
		if(AlarmCount==0||NewAlarmTime<AlarmList[0].AlarmTime)
		{
			AlarmList[0]=Alarm;
			AlarmCount=1;
			return TRUE;
		}
		if(NewAlarmTime==AlarmList[0].AlarmTime)
		{
			if(AlarmCount<ALARM_ALARMDISPLAY_MAX)
			{	
				AlarmList[AlarmCount]=Alarm;
				AlarmCount++;
				return TRUE;
			}
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CAlarmList::InsertToLaterAlarmList(CAlarm Alarm)
{
	if(LaterAlarmCount<ALARM_ALARMDISPLAY_MAX)
	{
		LaterAlarmList[LaterAlarmCount]=Alarm;
		LaterAlarmCount++;
	}
	else
	{
		for(int i=0;i<LaterAlarmCount-1;i++)
		{
			LaterAlarmList[i]=LaterAlarmList[i+1];
		}
		LaterAlarmList[LaterAlarmCount-1]=Alarm;
	}
	return TRUE;
}
BOOL CAlarmList::DeleteFormLaterAlarmList(UINT16 ID)
{
	for(int i=0;i<LaterAlarmCount;i++)
	{
		if(LaterAlarmList[i].ID==ID)
		{
			for(int j=i;j<LaterAlarmCount;j++)
				LaterAlarmList[j]=LaterAlarmList[j+1];
			LaterAlarmCount--;
			return TRUE;
		}
	}
	return FALSE;
}

//获得提醒队列
int CAlarmList::FindAlarmList()
{
	int RecordCount;//存储的提醒个数
	short AlarmIDBuf[ALARM_MAX_COUNT+2];
	CAlarm AlarmRecord;
	CDate NowDate;
	GetSysTime(&AlarmTime);
	NowDate.SetDate(AlarmTime.year,AlarmTime.month,AlarmTime.day);
	RecordCount=FindAlarm(AlarmIDBuf, ALARM_MAX_COUNT); // 返回查到个数
	
	AlarmCount=0;//当前提醒队列清空
	for(int i=0;i<RecordCount;i++)
	{
		AlarmRecord.Load(AlarmIDBuf[i]);
		AlarmRecord.Init(NowDate);
		AlarmRecord.Reserved=0;//用于判断是否为延后提醒
		if(AlarmRecord.bIsTodayAlarm(NowDate)==TRUE)
		{
			InsertToAlarmList(AlarmRecord);
		}
	}
	for(int i=0;i<LaterAlarmCount;i++)
	{
		if(LaterAlarmList[i].bIsTodayAlarm(NowDate)==TRUE)
		{
			LaterAlarmList[i].Reserved=1;
			InsertToAlarmList(LaterAlarmList[i]);
			//if(InsertToAlarmList(LaterAlarmList[i])==TRUE)//清除已提醒或已设置的提醒
			//	DeleteFormLaterAlarmList(LaterAlarmList[i].ID);
		}
	}
	return 0;
}
//取得提醒月表
void CAlarmList::FindMonthAlarmList(CDate NowDate)
{
	int RecordCount;//存储的提醒个数
	short AlarmIDBuf[ALARM_MAX_COUNT+2];
	CAlarm AlarmRecord;
	memset((UINT8*)MonthList,0x00,31);
	RecordCount=FindAlarm(AlarmIDBuf, ALARM_MAX_COUNT); // 返回查到个数
	for(int i=0;i<RecordCount;i++)
	{
		AlarmRecord.Load(AlarmIDBuf[i]);
		if(AlarmRecord.Repeat==ALARM_DAILY)
			continue;
		AlarmRecord.Init(NowDate);
		if(AlarmRecord.bIsThisMonthAlarm(NowDate)==TRUE)
		{
			MonthList[AlarmRecord.EventTime.day-1]=1;
			if(AlarmRecord.Repeat==ALARM_WEEKLY)
			{
				for(int j=AlarmRecord.EventTime.day-1;j<31;j=j+7)
					MonthList[j]=1;
			}
		}
	}
	
}
int CAlarmList::GetAlarmCount()
{
	return AlarmCount;
}
int CAlarmList::GetLaterAlarmCount()
{
	return LaterAlarmCount;
}
UINT8* CAlarmList::GetMonthList(CDate NowDate)
{
	FindMonthAlarmList(NowDate);
	return MonthList;
}
CAlarm *CAlarmList::GetAlarmList()
{
	return AlarmList;
}
//设置提醒时应增加	天，秒
//设置提醒时间
void CAlarmList::SetAlarm()
{
	FindAlarmList();
	if(AlarmCount>0)
		SetAlarmTime(AlarmList[0].AlarmTime.hour,AlarmList[0].AlarmTime.minute,AlarmList[0].AlarmTime.second);
}
//---------------------------------------------------------------
//---------------------------------------------------------------
//提醒索引部分
//---------------------------------------------------------------
//---------------------------------------------------------------
//修改索引
void CAlarmIndexList::Modify(CAlarm Alarm)
{
	Delete(Alarm.ID);
	Insert(Alarm);
}
//删除索引
void CAlarmIndexList::Delete(UINT16 AlarmID)
{
	int i;
	for(i=0;i<AlarmIndexCount;i++)//找到删除记录位置
	{
		if(AlarmID==DayAlarmIndex[i].AlarmID)
			break;
	}
	if(i==AlarmIndexCount)
		return;//没找到该记录
	for(int j=i;j<AlarmIndexCount-1;j++)//记录前移
	{
		DayAlarmIndex[j].EventTime=DayAlarmIndex[j+1].EventTime;
		DayAlarmIndex[j].AlarmID=DayAlarmIndex[j+1].AlarmID;
	
	}
	if(AlarmIndexCount)
		AlarmIndexCount--;
	ModifyFlag=TRUE;//修改标志=
}
//将索引记录项插入索引表中
void CAlarmIndexList::Insert(CAlarm Alarm)
{
	int i;
	//------------------------------判断是否应该插入
	switch(IndexKind)
	{
	case ALARM_INDEX_KIND_TODAY:
		Alarm.Init(IndexDate);
		if(Alarm.bEvenTimeIsToday(IndexDate)==FALSE)
			return;
		break;
	default:
		if(Alarm.Repeat!=IndexKind)
			return;
		Alarm.Init(IndexDate);//注意改进？？？？？？？
		break;
	}
	//------------------------------
	for(i=0;i<AlarmIndexCount;i++)//找到插入位置
	{
		if(Alarm.EventTime<=DayAlarmIndex[i].EventTime)
			break;
	}
	for(int j=AlarmIndexCount-1;j>=i;j--)//记录后移
	{
		DayAlarmIndex[j+1].EventTime=DayAlarmIndex[j].EventTime;
		DayAlarmIndex[j+1].AlarmID=DayAlarmIndex[j].AlarmID;
	}
	//插入记录
	DayAlarmIndex[i].EventTime=Alarm.EventTime;
	DayAlarmIndex[i].AlarmID=Alarm.ID;
	if(AlarmIndexCount<ALARM_MAX_COUNT)
		AlarmIndexCount++;
	ModifyFlag=TRUE;
}
//取得提醒索引表
int CAlarmIndexList::FindIndexList()
{
	int RecordCount;//存储的提醒个数
	short AlarmIDBuf[ALARM_MAX_COUNT+2];
	CAlarm AlarmRecord;
	AlarmIndexCount=0;
	RecordCount=FindAlarm(AlarmIDBuf, ALARM_MAX_COUNT); // 返回查到个数
	for(int i=0;i<RecordCount;i++)
	{
		AlarmRecord.Load(AlarmIDBuf[i]);
		Insert(AlarmRecord);
	}
	return AlarmIndexCount;
}
AlarmIndex* CAlarmIndexList::GetAlarmIndex()
{
	return DayAlarmIndex;
}
int CAlarmIndexList::GetAlarmIndexCount()
{
	return AlarmIndexCount;
}
UINT8 CAlarmIndexList::GetAlarmIndexKind()
{
	return IndexKind;
}
CDate CAlarmIndexList::GetAlarmIndexDate()
{
	return IndexDate;
}
//判断该提醒是否已经存在
BOOL CAlarmIndexList::IsAlarmExist(CAlarm AlarmRecord)
{
	int RecordCount;//存储的提醒个数
	ALARM *NewAlarm; 
	
	for(int i=0;i<AlarmIndexCount;i++)
	{
		NewAlarm=GetAlarmPointer(DayAlarmIndex[i].AlarmID);
		if(	NewAlarm->HourBef==AlarmRecord.TimeBef
			&&NewAlarm->Music==AlarmRecord.Music
			&&NewAlarm->When.second==AlarmRecord.WeekDay
			&&strcmp((char*)(NewAlarm->Todo),(char*)AlarmRecord.Todo)==0
			&&strcmp((char*)(NewAlarm->Title),(char*)AlarmRecord.Phone)==0)
		{
			AlarmRecord.When.second=NewAlarm->When.second;
			if(AlarmRecord.When==NewAlarm->When)
				return TRUE;
		}
	}
	return FALSE;
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//显示提醒部分
//---------------------------------------------------------------
//---------------------------------------------------------------
#define ALARMDISPLAY_TEXT       0x10
#define IDB_ALARMDISPLAY_OK     0X11
#define IDB_ALARMDISPLAY_CANCEL 0X12
#define IDC_ALARMDISPLAY_COMBOX 0X13
BEGIN_MESSAGE_MAP(CAlarmDisplay, CDialog)
ON_MESSAGE(IDB_ALARMDISPLAY_OK, CN_COMMAND , OnOk)
ON_MESSAGE(IDB_ALARMDISPLAY_CANCEL, CN_COMMAND , OnCancel)
ON_MESSAGE(0, WM_ALARM , OnAlarm)
//ON_MESSAGE(0, WM_RTC , OnRTCDay)
END_MESSAGE_MAP


void CAlarmDisplay::PaintTitle(CDC &dc)
{
   CRect r(m_sRect);
   r.bottom = r.top + WND_TITLE_HEIGHT-1;
   dc.SetBackColor(GRAY);
   dc.EraseRect(r); 
   dc.SetPenColor(WHITE); 
   dc.DrawText(r.left +2, r.top + 1, m_strName);
   if(m_dwStyle & WS_CONTROLBOX)
   {
	   r.left = r.right - WND_TITLE_HEIGHT-1;
	   r.DeflateRect(1,1); 
	   dc.Draw3dButton(r, NULL); 
   }
   dc.Reset();
}
//设置显示提醒队列
BOOL CAlarmDisplay::SetAlarmDispList()
{
	int NewAlarmCount;
	CAlarm *pAlarmRecord;
	
	NewAlarmCount=Alarm_GetAlarmCount();
	if(NewAlarmCount<=0)
		return FALSE;
	pAlarmRecord=Alarm_GetAlarmList();
	for(int i=NewAlarmCount-1;i>=0;i--)
	{	
		InsertAlarm(pAlarmRecord[i]);
		//从延后提醒队列中删除一个提醒
		if(pAlarmRecord[i].Reserved==1)
			Alarm_DeleteFromLaterAlarmList(pAlarmRecord[i].ID);
	}
	//-------------------------
	//重新设置提醒
	Alarm_SetAlarm();
	//-------------------------
	::SetLED(LED_MISSEDALARM	);
	 return TRUE;
}

BOOL CAlarmDisplay::Create()
{
	return CFrameWnd::Create("提醒" , WS_CAPTION|WS_BORDER, CRect(3,18,156,146),HELP_ALARM_OK);
}

extern CFrameWnd *g_pCurWindow;
void CAlarmDisplay::ShowWindow(UINT32 nCmdShow)
{
	if(nCmdShow == SW_SHOW)
	{
		if(m_dwStyle & WS_VISIBLE && g_pCurWindow == (CFrameWnd *)this)
			Paint();
		else
		{	
			if(g_pCurWindow)             // hqf dactive 当前窗口
				g_pCurWindow->SendMessage(WM_DACTIVE, 0, 0); 
			if( g_pCurWindow == (CFrameWnd *)this ) // hqf 12/31
				Paint();
			else
			{
				m_pBackScrBuf = CDC::SaveScreen(m_sRect); 
				CFrameWnd::ShowWindow(); 
			}
		}
		DisplayFlag=TRUE;
	}
    else
	{	
		if(m_dwStyle & WS_VISIBLE)
		{	
			if(m_pBackScrBuf != NULL)
				CDC::PutScreen(m_pBackScrBuf);
			extern void HideWindowNotRedrawBack(CFrameWnd *);
			::HideWindowNotRedrawBack(this);
		}
		DisplayFlag=FALSE;
	}
}

BOOL CAlarmDisplay::OnCreate()
{
	OkButton.Create("确定",CRect(21,126,53,143),this,IDB_ALARMDISPLAY_OK);
	CancelButton.Create("取消",CRect(101,126,132,143),this,IDB_ALARMDISPLAY_CANCEL);

	LaterTimeBox.Create(NULL,CRect(20,104,60,120),3*19,this,IDC_ALARMDISPLAY_COMBOX);
	char *szText[]={"5","10","15","20","25","30"};
	for(UINT16 i=0; i<6; i++)
		LaterTimeBox.AppendItem(szText[i], 0); 
	LaterTimeBox.SetCurSel(1); 
	memset(AlarmDisplayList[0].Todo,0x00,ALARM_TODO_LEN);
	AlarmTextLable.Create(AlarmDisplayList[0].Todo,CRect(12,53,150,101),this,WS_VISIBLE,ALARMDISPLAY_TEXT,ALARM_TITLE_LEN+ALARM_TODO_LEN+15);
	return CFrameWnd::OnCreate();
}
//显示提醒时间
void CAlarmDisplay::DrawAlarmTime(DateTime EventTime,CDC &dc)
{
	char DateStr[30];
	sprintf(DateStr,"%d/%02d/%02d %02d:%02d",EventTime.year,EventTime.month,EventTime.day,EventTime.hour,EventTime.minute);
	UINT8 BackColor,BackPen;
	BackColor=dc.SetBackColor();
	BackPen=dc.SetPenColor();
	dc.TextOut(15,36,DateStr);
	dc.SetBackColor(BackColor);
	dc.SetPenColor(BackPen);
}
//显示提醒信息
BOOL CAlarmDisplay::DrawAlarmInfo(CDC dc)
{
	char AlarmKindStr[20];
	if(UnDispAlarmCount)
	{
		/*
		DrawAlarmTime(AlarmDisplayList[UnDispAlarmCount-1].EventTime,dc);
		AlarmTextLable.SetWindowText(AlarmDisplayList[UnDispAlarmCount-1].Todo);
		
		AlarmTextLable.OnPaint(dc);
		CloseCursor();
		AlarmDisplayList[UnDispAlarmCount-1].GetAlarmKindStr(AlarmKindStr);
		SetWindowText(AlarmKindStr);
*/
		OnPaint(dc);
		return TRUE;
	}
	return FALSE;
}
//从提醒显示队列中删除一个提醒
BOOL CAlarmDisplay::DeleteAlarm(CAlarm *pAlarmList,int *AlarmCount,UINT16 AlarmID)
{
	for(int i=0;i<(*AlarmCount);i++)
	{
		if(pAlarmList[i].ID==AlarmID)
		{
			for(int j=i+1;j<(*AlarmCount);j++)
			{
				pAlarmList[j-1]=pAlarmList[j];
			}
			
			(*AlarmCount)--;
			return TRUE;
		}
	}
	return FALSE;
}
 
//向指定提醒显示队列中插入一个提醒
void CAlarmDisplay::InsertAlarmToList(CAlarm *pAlarmList,int *AlarmCount,CAlarm AlarmRecord)
{
	DeleteAlarm(pAlarmList,AlarmCount,AlarmRecord.ID); 
	if((*AlarmCount)==ALARM_ALARMDISPLAY_MAX)
	{
		for(int i=0;i<(*AlarmCount);i++)
		{
			pAlarmList[i]=pAlarmList[i+1];
		}
		
		(*AlarmCount)--;
	}
	pAlarmList[*AlarmCount]=AlarmRecord;
	(*AlarmCount)++;
}

//向提醒显示队列中插入一个提醒
void CAlarmDisplay::InsertAlarm(CAlarm AlarmRecord)
{
	InsertAlarmToList(AlarmDisplayList,&UnDispAlarmCount,AlarmRecord);
}

//处理提醒消息 
void CAlarmDisplay::OnAlarm(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	//处理零点报警
	int NewAlarmCount;
	NewAlarmCount=Alarm_GetAlarmCount();
	if(NewAlarmCount<=0)
	{
		Alarm_SetAlarm();
		return;
	}	
	//CAlarm *AlarmRecord=Alarm_GetAlarmList();
	SetAlarmDispList();

	if(!GetRecordStatus())
	{
		if(!GetRingStatus())
		{	
			RingFlag=TRUE;
			::sndPlayMusic(AlarmDisplayList[UnDispAlarmCount-1].Music,FALSE,1);
		}
		this->OnRecord(0,0,0);
	}
	
}

//处理留言结束消息
void CAlarmDisplay::OnRecord(UINT32 nMessage, UINT32 nSrcId, UINT32 lParam)
{
	extern int g_iScreenStatus;
	if(UnDispAlarmCount&&g_iScreenStatus==SS_NORMAL)
	{
		this->ShowWindow(SW_SHOW);//显示窗口
		return;
	}
}

void CAlarmDisplay::OnPaint(CDC &dc) 
{
	char AlarmKindStr[20];
	AlarmTextLable.SetWindowText(AlarmDisplayList[UnDispAlarmCount-1].Todo);
	AlarmDisplayList[UnDispAlarmCount-1].GetAlarmKindStr(AlarmKindStr);
	SetWindowText(AlarmKindStr);

	CFrameWnd::OnPaint(dc);
	CloseCursor();
	DrawAlarmTime(AlarmDisplayList[UnDispAlarmCount-1].EventTime,dc);
	
	dc.Reset();
	dc.TextOut(65,105,"分钟后提醒");
	
}
void CAlarmDisplay::OnOk()
{
	//处理延后
	int MinuteLater;
	MinuteLater=(LaterTimeBox.GetCurSel()+1)*(-5);
	::GetSysTime(&(AlarmDisplayList[UnDispAlarmCount-1].When));
	AlarmDisplayList[UnDispAlarmCount-1].When=AlarmDisplayList[UnDispAlarmCount-1].GetRightTimeByMinute(AlarmDisplayList[UnDispAlarmCount-1].When,MinuteLater);
	AlarmDisplayList[UnDispAlarmCount-1].When.second=0;
	AlarmDisplayList[UnDispAlarmCount-1].TimeBef=0;
	AlarmDisplayList[UnDispAlarmCount-1].Repeat=ALARM_ONE;
	AlarmDisplayList[UnDispAlarmCount-1].Reserved=1;//判断是否为延后提醒
	AlarmDisplayList[UnDispAlarmCount-1].Init(::GetSysDate());
	Alarm_InsertToLaterAlarmList(AlarmDisplayList[UnDispAlarmCount-1]);
	Alarm_SetAlarm();
	//从提醒显示队列中删除一个提醒
	DeleteAlarm(AlarmDisplayList,&UnDispAlarmCount,AlarmDisplayList[UnDispAlarmCount-1].ID);
	if(DrawAlarmInfo(CDC())==FALSE)
	{
		//退出
		if((UnDispAlarmCount)==0)
			::ClearLED(LED_MISSEDALARM	);
		ShowWindow(SW_HIDE);
	}
}
void CAlarmDisplay::OnCancel()
{
	//从提醒显示队列中删除一个提醒
	DeleteAlarm(AlarmDisplayList,&UnDispAlarmCount,AlarmDisplayList[UnDispAlarmCount-1].ID);
	if(DrawAlarmInfo(CDC())==FALSE)
	{
		//退出
		if((UnDispAlarmCount)==0)
			::ClearLED(LED_MISSEDALARM	);
		ShowWindow(SW_HIDE);	
	}
}
void CAlarmDisplay::WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	switch(nMessage)
	{
	case WM_PENUP:
		if (RingFlag&& GetRingStatus())
		{
			StopRing();
			//RingFlag=FALSE;
		}
		break;
	}
	CFrameWnd::WindowProcess(nMessage, wParam, lParam);
	switch(nMessage)
	{
	case WM_RTC :
		if(wParam & RTCI_DAY)
			Alarm_SetAlarm();
		break;
	case WM_SOUND_CLOSE:
		if(RingFlag==TRUE)
			RingFlag=FALSE;
		break;
	case WM_EXIT_SCRPROT:
		OnRecord(0,0,0);
		break;
	case WM_RECORD_END:
		OnRecord(0,0,0);
		break;
	}
}
void CAlarmDisplay::Destroy()
{
	if (RingFlag&&GetRingStatus())
	{
		StopRing();
		
	}
	RingFlag=FALSE;
	ShowWindow(SW_HIDE);
}
void CAlarmDisplay::OnDactive()
{
	if (RingFlag&&GetRingStatus())
	{
		StopRing();
	}
	RingFlag=FALSE;
	CFrameWnd::OnDactive();
	ShowWindow(SW_HIDE);
	
}
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------






const char szRepeatFindText[][9]=
	{"新建","一次提醒","每日提醒","每周提醒"
	 ,"每月提醒","每年提醒"};
//---------------------------------------------------------------
//---------------------------------------------------------------
//新建提醒窗口－－修改
//---------------------------------------------------------------
//---------------------------------------------------------------
#define IDB_NEWALARM_SAVE 0x11//保存退出
#define IDG_NEWALARM_GRID 0x12//网格控件
#define IDC_ALARM_MUSICLIST 0x13
#define IDC_ALARM_WEEKLIST  0x14
#define IDC_ALARM_DAYLIST   0x15 
const char szNewAlarmText[][6]={"内容:","日期:","时间:","提前:","音乐:","电话:","分钟","试听:"};
const char szAlarmMusicPlayText[][14]={"  开始试听","  停止试听"};
BEGIN_MESSAGE_MAP(CCreateNewAlarm, CAppWnd)
ON_MESSAGE(IDG_NEWALARM_GRID, CN_GRIDSEL , OnGridSel)
ON_MESSAGE(IDB_NEWALARM_SAVE, CN_COMMAND , OnSave)
ON_MESSAGE(IDG_NEWALARM_GRID , CN_GRIDDROPDOWN , OnGridComboxClick)
ON_MESSAGE(IDC_ALARM_MUSICLIST , CN_LISTSEL , OnListSel)
ON_MESSAGE(IDC_ALARM_WEEKLIST , CN_LISTSEL , OnWeekListSel)
END_MESSAGE_MAP
void CCreateNewAlarm::SetWndTitle()
{
	int i;
	switch(AlarmRecord.Repeat)
	{
	case ALARM_ONE:
		i=1;
		break;
    case ALARM_DAILY:
		i=2;
		break;
	case ALARM_WEEKLY:
		i=3;
		break;
	case ALARM_MONTHLY:
		i=4;
		break;
	case ALARM_ANNUALLY:
		i=5;
		break;
	default:
		i=1;
		break;
	}
	if(ModifyFlag==TRUE)//修改
		sprintf((char*)WndTitle,"%s%s",(char*)"修改",(char*)(szRepeatFindText[i]));
	else
		sprintf((char*)WndTitle,"%s%s",(char*)(szRepeatFindText[0]),(char*)(szRepeatFindText[i]));
	
}
//创建——增加
BOOL CCreateNewAlarm::Create(UINT16 nRepeatKind,UINT16 AlarmID,BOOL Flag)
{
	memset((char*)AlarmRecord.Todo,0x00,ALARM_TODO_LEN);
	memset((char*)AlarmRecord.Phone,0x00,ALARM_TITLE_LEN);
	ModifyFlag=Flag;
	SaveFlag=FALSE;//初始位不存盘
	RingFlag=FALSE;  //播放响铃标志
	ExitFlag=FALSE;//TRUE:save;  FALSE: cancel;
	if(ModifyFlag==FALSE)//增加
	{
		GetSysTime(&(AlarmRecord.When));
		AlarmRecord.When.second=0;
		AlarmRecord.Repeat=nRepeatKind;
		AlarmRecord.EventTime=AlarmRecord.When;
		AlarmRecord.TimeBef=0;
		AlarmRecord.Music=1;
		AlarmRecord.RecordID=0;
		AlarmRecord.WeekDay=0;
		//cxd add 重新设置weekday主要针对日重复时周末不提醒
		switch(nRepeatKind)
		{
		case ALARM_DAILY:
			AlarmRecord.WeekDay=0;
			break;
		case ALARM_WEEKLY:
			AlarmRecord.WeekDay=CDate::WeekDay(AlarmRecord.EventTime.year,AlarmRecord.EventTime.month,AlarmRecord.EventTime.day);
			break;
		default:
			AlarmRecord.WeekDay=0;
			break;
		}
	}
	else//修改
	{
		AlarmRecord.Load(AlarmID);
		AlarmRecord.Init(g_AlarmIndexList->GetAlarmIndexDate());
		AlarmRecord.When=AlarmRecord.EventTime;
	}
	
	SetWndTitle();
	if(!CAppWnd::Create((char*)GetWndTitle(), WS_CAPTION |WS_VISIBLE, 
			DESK_TOP_RECT, HELP_ALARM_NEWOREDIT))
		return FALSE;
	return TRUE;

}

void CCreateNewAlarm::SetDisplayText()
{
	char *pText[2];
	char pStyle[]= {0 , 0};
	UINT32 pIconID[2] = {0 , 0};
	char szUnitText[ALARM_TODO_LEN+1];
	memset((char*)szUnitText,0x00,ALARM_TODO_LEN+1);
	
	//设置内容
	pText[0]=(char*)(szNewAlarmText[0]);
	pText[1]=AlarmRecord.Todo;
	NewAlarmGrid.AddRow(pText,pStyle,pIconID);
	
	//设置日期
	AlarmRecord.GetEventDateStr(szUnitText,FALSE);
	pText[0]=(char*)(szNewAlarmText[1]);
	pText[1]=szUnitText;
	if(AlarmRecord.Repeat==ALARM_WEEKLY||AlarmRecord.Repeat==ALARM_DAILY)
		pStyle[1]=UNS_COMBOX;
	NewAlarmGrid.AddRow(pText,pStyle,pIconID);
	pStyle[1]=0;

	//设置时间
	AlarmRecord.GetTimeStr(szUnitText);
	pText[0]=(char*)(szNewAlarmText[2]);
	pText[1]=szUnitText;
	NewAlarmGrid.AddRow(pText,pStyle,pIconID);

	//设置提前
	AlarmRecord.GetTimeBefStr(szUnitText);
	pText[0]=(char*)(szNewAlarmText[3]);
	pText[1]=szUnitText;
	NewAlarmGrid.AddRow(pText,pStyle,pIconID);

	//设置音乐
	pText[0]=(char*)(szNewAlarmText[4]);
	pText[1]=(char*)(::GetRingPointer(AlarmRecord.Music)->Name);
	pStyle[1]=UNS_COMBOX;
	NewAlarmGrid.AddRow(pText,pStyle,pIconID);

	//设置试听
	pStyle[1]=0;
	pIconID[1]=BMP_ALARM_MUSIC_START;
	pText[0]=NULL;
	pText[1]=NULL;
	NewAlarmGrid.AddRow(pText,pStyle,pIconID);
}
const char WeeklyStr[][7]={"每周日","每周一","每周二","每周三","每周四","每周五","每周六"};
BOOL CCreateNewAlarm::OnCreate()
{
	UINT16 pWidth[2]={44,0};
	if(!CAppWnd::OnCreate())
		return FALSE;
	SaveButton.Create("",ICON_OK,
					  CRect(  m_sRect.left+124, 
							  m_sRect.top, 
							  m_sRect.left+141, 
							  m_sRect.top+WND_TITLE_HEIGHT-1
							),this,IDB_NEWALARM_SAVE);
	NewAlarmGrid.Create(this,
					    CRect(m_sRect.left, 
					          m_sRect.top+WND_TITLE_HEIGHT+1, 
					          m_sRect.right-18, m_sRect.bottom
					         ), 
					    WS_CHILD|TBS_SPACELINE,0,2,IDG_NEWALARM_GRID);
	NewAlarmGrid.SetColsWidth(pWidth);
	SetDisplayText();
	
	CRect r;
	switch(AlarmRecord.Repeat)
	{
	case ALARM_DAILY:
		r.SetRect(m_sRect.left +41, m_sRect.top +1 , m_sRect.left + NewAlarmGrid.GetColsWidth(2) +41 , m_sRect.top + (TABLE_ROW_HEIGHT+1) *2 );
		WeekBox.Create(r , this, NULL, IDC_ALARM_WEEKLIST);
		WeekBox.AppendItem((char*)"每日", 0 , 0);
		WeekBox.AppendItem((char*)"工作日", 0 , 0x10);
		if(AlarmRecord.WeekDay==ALARM_DAILY_NOWEEK)
			WeekBox.SetCurSel(1); 
		else
			WeekBox.SetCurSel(0); 
		break;
	case ALARM_WEEKLY:
		r.SetRect(m_sRect.left +41, m_sRect.top +1 , m_sRect.left + NewAlarmGrid.GetColsWidth(2) +41 , m_sRect.top + (TABLE_ROW_HEIGHT+1) *5 );
		WeekBox.Create(r , this, NULL, IDC_ALARM_WEEKLIST);
		for(int i=0; i<7; i++)
		{
			WeekBox.AppendItem((char*)WeeklyStr[i], 0 , i);
		}
		WeekBox.SetCurSel(AlarmRecord.WeekDay); 
		break;
	}
	
	r.SetRect(m_sRect.left +41, m_sRect.top +1 , m_sRect.left + NewAlarmGrid.GetColsWidth(2) +41 , m_sRect.top + (TABLE_ROW_HEIGHT+1) *5 );
	MusicBox.Create(r , this, NULL, IDC_ALARM_MUSICLIST);
	short ridbuf[RING_MAX_COUNT];
	int count = ::FindRing(ridbuf, RING_MAX_COUNT);
	INT16 cursel=0;
	for(int i=0; i<count; i++)
	{
		MusicBox.AppendItem(::GetRingPointer(ridbuf[i])->Name, 0 , ridbuf[i]);
		if(AlarmRecord.Music == ridbuf[i])
           cursel = i;
	}
	MusicBox.SetCurSel(cursel); 
	return TRUE;

}
short CharToShort(char* Str)
{
	int Len;
	short nReturn=0;
	Len=strlen((char *)Str);
	for(int i=0;i<Len;i++)
	{
		if(Str[i]<0x30||Str[i]>0x39)
			continue;//处理非法字符
		nReturn*=10;
		nReturn+=Str[i]-0x30;
	}
	return nReturn;
}
void CCreateNewAlarm::OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	short nRow , nCol ;
	INT16 nYear,nMonth,nDay,nHour,nMinute;
	char  AlarmText[ALARM_TODO_LEN+10];
	memset(AlarmText,0x00,ALARM_TODO_LEN+10);
	nRow = LOWORD(lParam);
	nCol = HIWORD(lParam);
	if(nCol==1)
		return;
	//为输入赋初值
	AlarmRecord.GetEventDateTime(nYear,nMonth,nDay,nHour,nMinute);
	switch(nRow)
	{
	case 1://内容
		strcpy(AlarmText,(char*)AlarmRecord.Todo);
		if(InputBox((char *)"提醒内容", (char *)AlarmText, ALARM_TODO_LEN, INIT_WRITE,FALSE)==IDOK)
		{
			if(strcmp((char*)AlarmRecord.Todo,AlarmText))
			{
				strcpy((char*)AlarmRecord.Todo,AlarmText);
				NewAlarmGrid.SetUnitText(1,2,AlarmText,0,TRUE);
				SaveFlag=TRUE;//初始位存盘
			
			}
		}
		return;
	case 2://日期
		switch(AlarmRecord.Repeat)
		{
		case ALARM_MONTHLY:
			SelectDayBox( nDay,(char *)"提醒日期");
			if(AlarmRecord.When.day!=nDay)
			{
				AlarmRecord.When.day=nDay;
				AlarmRecord.EventTime=AlarmRecord.When;
				AlarmRecord.GetEventDateStr(AlarmText,FALSE);
				NewAlarmGrid.SetUnitText(2,2,AlarmText,0,TRUE);
				SaveFlag=TRUE;//初始位存盘
			
			}
			return;
		case ALARM_ANNUALLY://年
		case ALARM_ONE://一次
			if(DateInputBox((char *)"提醒日期", nYear, nMonth, nDay)==IDOK)
			{
				AlarmRecord.When.year=nYear;
				AlarmRecord.When.month=nMonth;
				AlarmRecord.When.day=nDay;
				AlarmRecord.EventTime=AlarmRecord.When;
				AlarmRecord.GetEventDateStr(AlarmText,FALSE);
				NewAlarmGrid.SetUnitText(2,2,AlarmText,0,TRUE);
				SaveFlag=TRUE;//初始位存盘
			
			}
			return;
		}
		return;
	case 3://时间
		INT16 nType;
		if(TimeInputBox((char *)"提醒时间", nHour, nMinute,nType)==IDOK)
		{
			AlarmRecord.When.hour=nHour;
			AlarmRecord.When.minute=nMinute;
			AlarmRecord.EventTime=AlarmRecord.When;
			AlarmRecord.GetTimeStr(AlarmText);
			NewAlarmGrid.SetUnitText(3,2,AlarmText,0,TRUE);
			SaveFlag=TRUE;//初始位存盘
			
		}
		return;
	case 4://提前
		//memset(AlarmText,0x00,
		//sprintf((char*)AlarmText,"%d",AlarmRecord.TimeBef);
		if(NumInputBox((char *)"提醒提前", AlarmText, 4)==IDOK)
		{
			int TimeBef=0;
			if(strlen(AlarmText))
				TimeBef=CharToShort(AlarmText);
			if(TimeBef>240)
			{
				TimeBef=240;
				::MessageBox("提前时间最多为240分钟！",GetWndTitle(),MB_OK);
			}
			if(TimeBef!=AlarmRecord.TimeBef)
			{
				AlarmRecord.TimeBef=TimeBef;
				AlarmRecord.GetTimeBefStr(AlarmText);
				NewAlarmGrid.SetUnitText(4,2,AlarmText,0,TRUE);
				SaveFlag=TRUE;//初始位存盘
				//NewAlarmGrid.OnPaint(CDC());
			}
		}
		return ;
	//case 5://音乐
	//	break;
	case 6://试听
		if (RingFlag&&GetRingStatus())//停止
		{
			StopRing();
			return;
			
		}
		if(RingFlag==FALSE)
		{
			RingFlag=TRUE;
			::sndPlayMusic(AlarmRecord.Music,FALSE,1);
			NewAlarmGrid.SetUnitIcon(6,2 , BMP_ALARM_MUSIC_STOP , TRUE);
		}
		return;
	}
}
void CCreateNewAlarm::OnGridComboxClick(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	CRect r;
	short i;
	char item[8];
	UINT16 nRow ,nCol;
	nRow = LOWORD(lParam) , nCol = HIWORD(lParam);
	if (RingFlag&&GetRingStatus())//停止
	{
		StopRing();
		RingFlag=FALSE;
		NewAlarmGrid.SetUnitIcon(6,2 , BMP_ALARM_MUSIC_START , TRUE);
	}
	switch(nRow)
	{
	case 2:
		switch(AlarmRecord.Repeat)
		{
		case ALARM_DAILY:
			NewAlarmGrid.GetRectFromClick(2 , 2, r);
			r.top = r.top+ TABLE_ROW_HEIGHT;
			WeekBox.PopUpList(r.left , r.top);
			break;
		case ALARM_WEEKLY:
			WeekBox.PopUpList(m_sRect.left + 42 , m_sRect.top+ 52);
			break;
		}
		break;
	case 5:
		MusicBox.PopUpList(m_sRect.left + 42 , m_sRect.top+ 52);
		break;
	}
}
void CCreateNewAlarm::WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	CAppWnd::WindowProcess(nMessage, wParam, lParam);
	switch(nMessage)
	{
	case WM_SOUND_CLOSE:
		if(RingFlag==TRUE)
		{	
			RingFlag=FALSE;
			NewAlarmGrid.SetUnitIcon(6,2 , BMP_ALARM_MUSIC_START , TRUE);
		}
		break;
	}

}
void CCreateNewAlarm::OnDactive()
{
	if (RingFlag&&GetRingStatus())
	{
		StopRing();
		RingFlag=FALSE;
		NewAlarmGrid.SetUnitIcon(6,2 , BMP_ALARM_MUSIC_START , TRUE);
	}
	CAppWnd::OnDactive();
}
void CCreateNewAlarm::OnListSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	CPopListItem * pItem = MusicBox.GetItem(lParam);
	if(pItem)
	{
		if(AlarmRecord.Music != INT16(pItem->exData))
		{
			AlarmRecord.Music = INT16(pItem->exData);
			NewAlarmGrid.SetUnitText(5, 2, pItem->strText, 0, TRUE);
			SaveFlag=TRUE;//初始位存盘
		}
	}
	
}
void CCreateNewAlarm::OnWeekListSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	CPopListItem * pItem = WeekBox.GetItem(lParam);
	if(pItem)
	{
		if(AlarmRecord.WeekDay != INT16(pItem->exData))
		{
			AlarmRecord.WeekDay = INT16(pItem->exData);
			NewAlarmGrid.SetUnitText(2, 2, pItem->strText, 0, TRUE);
			SaveFlag=TRUE;//初始位存盘
		}
	}
}

//存储一条提醒
void CCreateNewAlarm::SaveAlarm()
{
	switch(ExitFlag)
	{
	case TRUE://save
		if(SaveFlag==FALSE)
		{
			if(::MessageBox("尚未修改，要保存吗？" , GetWndTitle(), MB_YESNO|MB_ICONQUESTION)!= IDYES)
				return;	
		}
		else
		{
			
			if( strlen((char*)(AlarmRecord.Todo))==0 )
			{
				if(::MessageBox("内容为空，要保存吗？" , GetWndTitle(), MB_YESNO|MB_ICONQUESTION)!= IDYES)
				{
					//SaveFlag=FALSE;
					return;
				}
			}
		}
		break;
	case FALSE://cancel
		if(SaveFlag==TRUE)//初始位存盘
		{
		
			if( strlen((char*)(AlarmRecord.Todo))==0 )
			{
				if(::MessageBox("内容为空，要保存吗？" , "退出", MB_YESNO|MB_ICONQUESTION)!= IDYES)
				{	
					//SaveFlag=FALSE;
					return;
				}
			}
			else
			{
				if(::MessageBox("要保存吗?" , "退出", MB_YESNO|MB_ICONQUESTION)!= IDYES)
				{
					//SaveFlag=FALSE;
					return;
				}
			}
			break;
		}
		else
			return;
		break;
	}
	//判断该提醒是否已经存在
	if(g_AlarmIndexList->IsAlarmExist(AlarmRecord)==TRUE)
	{	
		::MessageBox("该提醒已经存在！",GetWndTitle(),MB_OK);
		return;
	}
	switch(ModifyFlag)
	{
	case TRUE:
		AlarmRecord.Modify();
		AlarmRecord.Init(g_AlarmIndexList->GetAlarmIndexDate());
		g_AlarmIndexList->Modify(AlarmRecord);
		break;
	case FALSE:
		AlarmRecord.Save();
		AlarmRecord.Init(g_AlarmIndexList->GetAlarmIndexDate());
		g_AlarmIndexList->Insert(AlarmRecord);
		break;
	}
	SaveFlag=FALSE;
	Alarm_SetAlarm();
}
void CCreateNewAlarm::OnSave(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	ExitFlag=TRUE;//TRUE:save;  FALSE: cancel;
	this->Destroy();
}
void CCreateNewAlarm::OnCancel(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	ExitFlag=FALSE;//TRUE:save;  FALSE: cancel;
	this->Destroy();
}
BOOL CCreateNewAlarm::OnDestroy()
{
	if (RingFlag&&GetRingStatus())
	{
		StopRing();
	}
	RingFlag=FALSE;
	SaveAlarm();
	return CAppWnd::OnDestroy();
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//提醒浏览窗口
//---------------------------------------------------------------
//---------------------------------------------------------------
#define IDG_ALARMVIEW_GIRD    0x11
#define IDB_ALARMVIEW_MODIFY  0x12
#define IDB_ALARMVIEW_DELETE  0x13
BEGIN_MESSAGE_MAP(CAlarmView, CAppWnd)
ON_MESSAGE(IDB_ALARMVIEW_MODIFY, CN_COMMAND , OnModify)
ON_MESSAGE(IDB_ALARMVIEW_DELETE, CN_COMMAND , OnDelete)
ON_MESSAGE(IDG_ALARMVIEW_GIRD, CN_GRIDSEL , OnGridSel)
END_MESSAGE_MAP

//创建
BOOL CAlarmView::Create(UINT16 AlarmID)
{
	int i;
	ModifyAlarmWnd=NULL;
	RingFlag=FALSE;
	AlarmRecord.Load(AlarmID);
	AlarmRecord.Init(g_AlarmIndexList->GetAlarmIndexDate());
	AlarmRecord.When=AlarmRecord.EventTime;
	switch(AlarmRecord.Repeat)
	{
	case ALARM_ONE:
		i=1;
		break;
    case ALARM_DAILY:
		i=2;
		break;
	case ALARM_WEEKLY:
		i=3;
		break;
	case ALARM_MONTHLY:
		i=4;
		break;
	case ALARM_ANNUALLY:
		i=5;
		break;
	default:
		i=1;
		break;
	}
	if(!CAppWnd::Create((char*)(szRepeatFindText[i]), WS_CAPTION |WS_VISIBLE, 
			DESK_TOP_RECT, HELP_ALARM_BROWSER))
		return FALSE;
	return TRUE;
}

void CAlarmView::SetDisplayText()
{
	char *pText[2]={NULL,NULL}; 
	char pStyle[2]={0,0};
	UINT32 pIconID[2] = {0,0};

	char szUnitText[50];
	memset((char*)szUnitText,0x00,50);
	//设置内容
	if( strlen((char*)AlarmRecord.Todo) )
	{
		pText[0]=(char*)(szNewAlarmText[0]);
		pText[1]=AlarmRecord.Todo;
		AlarmViewGrid.AddRow(pText,pStyle,pIconID);
	}

	//设置日期
	AlarmRecord.GetEventDateStr(szUnitText,FALSE);
	pText[0]=(char*)(szNewAlarmText[1]);
	pText[1]=szUnitText;
	AlarmViewGrid.AddRow(pText,pStyle,pIconID);

	//设置时间
	AlarmRecord.GetTimeStr(szUnitText);
	pText[0]=(char*)(szNewAlarmText[2]);
	pText[1]=szUnitText;
	AlarmViewGrid.AddRow(pText,pStyle,pIconID);
	
	//设置提前
	if(AlarmRecord.TimeBef)
	{
		AlarmRecord.GetTimeBefStr(szUnitText);
		pText[0]=(char*)(szNewAlarmText[3]);
		pText[1]=szUnitText;
		AlarmViewGrid.AddRow(pText,pStyle,pIconID);
	}
	
	//设置音乐
	pText[0]=(char*)(szNewAlarmText[4]);
	pText[1]=(char*)(::GetRingPointer(AlarmRecord.Music)->Name);
	AlarmViewGrid.AddRow(pText,pStyle,pIconID);

	//设置电话
	if(strlen(AlarmRecord.Phone))
	{
		AlarmViewGrid.SetUnitText(6,2,AlarmRecord.Phone,0);
		pText[0]=(char*)(szNewAlarmText[5]);
		pText[1]=(char*)AlarmRecord.Phone;
		AlarmViewGrid.AddRow(pText,pStyle,pIconID);
	}
}

BOOL CAlarmView::OnCreate()
{
	UINT16 pWidth[2]={44,0};
	if(!CAppWnd::OnCreate())
		return FALSE;
	ModifyButton.Create("修改",
						CRect(  m_sRect.left+77, 
							    m_sRect.top, 
							    m_sRect.left+109, 
							    m_sRect.top+WND_TITLE_HEIGHT-1
							),
						this,IDB_ALARMVIEW_MODIFY);
	DeleteButton.Create("删除",
						CRect(  m_sRect.left+110, 
							    m_sRect.top, 
							    m_sRect.left+142, 
							    m_sRect.top+WND_TITLE_HEIGHT-1
							),
						this,IDB_ALARMVIEW_DELETE);
	AlarmViewGrid.Create(this,
					    CRect(m_sRect.left, 
					          m_sRect.top+WND_TITLE_HEIGHT+1, 
					          m_sRect.right-18, m_sRect.bottom
					         ), 
					    WS_CHILD|TBS_SPACELINE|TBS_ROWSELECT,0,2,IDG_ALARMVIEW_GIRD);
	AlarmViewGrid.SetColsWidth(pWidth);
	SetDisplayText();
	return TRUE;

}

void CAlarmView::OnActive()
{
	CAlarm Alarm;
	Alarm.Load(AlarmRecord.ID);
	Alarm.Init(g_AlarmIndexList->GetAlarmIndexDate());
	Alarm.When=Alarm.EventTime;
	
	if(Alarm==AlarmRecord)
	{	
		CAppWnd::OnActive();
		return ;
	}
	else
		AlarmRecord=Alarm;
	AlarmViewGrid.RemoveAll();
	SetDisplayText();
	AlarmViewGrid.OnPaint(CDC());
	CAppWnd::OnActive();
}

void CAlarmView::OnModify(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	if(ModifyAlarmWnd==NULL)
		ModifyAlarmWnd=new CCreateNewAlarm;
	ModifyAlarmWnd->Create(AlarmRecord.Repeat,AlarmRecord.ID,TRUE);
	ModifyAlarmWnd->ShowWindow();
}

void CAlarmView::OnDelete(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	if(::MessageBox("确定删除吗？", "删除", MB_YESNO|MB_ICONQUESTION)== IDYES)
	{
		g_AlarmIndexList->Delete(AlarmRecord.ID);
		AlarmRecord.Delete();
		Alarm_SetAlarm();
		this->Destroy();
	}
}

BOOL CAlarmView::OnDestroy()
{
	BOOL Flag;
	Flag=CAppWnd::OnDestroy();
	if(Flag==FALSE)
		return FALSE;
	if (RingFlag&&GetRingStatus())
	{
		StopRing();
		
	}
	RingFlag=FALSE;
	if(ModifyAlarmWnd)
		delete ModifyAlarmWnd;
	ModifyAlarmWnd=NULL;
	return TRUE;
}
void CAlarmView::OnDactive()
{
	if (RingFlag&&GetRingStatus())
	{
		StopRing();
		
	}
	RingFlag=FALSE;
	CAppWnd::OnDactive();
}
void CAlarmView::WindowProcess(UINT32 nMessage , UINT32 wParam , UINT32 lParam)
{
	
	switch(nMessage)
	{
	case WM_PENUP:
		if (RingFlag&& GetRingStatus())
		{
			StopRing();
			//RingFlag=FALSE;
		}
		break;
	case WM_SOUND_CLOSE:
		if(RingFlag==TRUE)
			RingFlag=FALSE;
		break;
	}
	CAppWnd::WindowProcess(nMessage, wParam, lParam);
}
void CAlarmView::OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	short nRow , nCol ,MusicLine=5;
	nRow = LOWORD(lParam);
	nCol = HIWORD(lParam);
	if(AlarmRecord.TimeBef==0)
		MusicLine--;
	if( strlen((char*)AlarmRecord.Todo)==0 )
		MusicLine--;
	if(nRow==MusicLine)//振铃
	{
		if (RingFlag&&GetRingStatus())
		{
			StopRing();
			return;
			//RingFlag=FALSE;
		}
		if(RingFlag==FALSE)
		{
			RingFlag=TRUE;
			::sndPlayMusic(AlarmRecord.Music,FALSE,1);
		}
	}
	return;
}	

//---------------------------------------------------------------
//---------------------------------------------------------------
//分类索引窗口
//---------------------------------------------------------------
//---------------------------------------------------------------
#define ALARM_NEW           0x11
#define ALARM_DELETE        0x13 
#define ID_REPEATLIST_GRID  0x12
BEGIN_MESSAGE_MAP(CRepeatFindList, CAppWnd)
ON_MESSAGE(ID_REPEATLIST_GRID, CN_GRIDSEL , OnGridSel)
ON_MESSAGE(ALARM_NEW, CN_COMMAND , OnCreatNewAlarm)
ON_MESSAGE(ALARM_DELETE, CN_COMMAND , OnDeleteAlarm)

END_MESSAGE_MAP
void CRepeatFindList::GetWindowTitleAndHelpID(UINT16 nKind,char *WndTitle,UINT32 *pHelpID)
{
	int i;
	switch(nKind)
	{
	case ALARM_ONE:
		i=1;
		*pHelpID=HELP_ALARM_ONCE;
		break;
    case ALARM_DAILY:
		i=2;
		*pHelpID=HELP_ALARM_EVERYDAY;
		break;
	case ALARM_WEEKLY:
		i=3;
		*pHelpID=HELP_ALARM_EVERYWEEK;
		break;
	case ALARM_MONTHLY:
		*pHelpID=HELP_ALARM_EVERYMONTH;
		i=4;
		break;
	case ALARM_ANNUALLY:
		i=5;
		*pHelpID=HELP_ALARM_EVERYYEAR;
		break;
	default:
		i=1;
		*pHelpID=0;
		break;
	}
	sprintf(WndTitle,"%s",(char*)(szRepeatFindText[i]));
}
BOOL CRepeatFindList::Create(UINT16 nKind)
{
	int i;
	char WndTitle[50];
	CreateAlarmWnd=NULL;
	AlarmViewWnd=NULL;
	nFindKind=nKind;
	UINT32 nHelpID=0;
	GetWindowTitleAndHelpID(nKind,WndTitle,&nHelpID);
	if(!CAppWnd::Create((char*)WndTitle, WS_CAPTION |WS_VISIBLE, 
			DESK_TOP_RECT, nHelpID))
		return FALSE;
	return TRUE;
}
void CRepeatFindList::SetDisplayText()
{		
	char szAlarmStr[60];
	char *TextEnd;
	CAlarm AlarmRecord;
	char *pText[2]={NULL,NULL}; 
	char pStyle[2]={0,0};
	UINT32 nScrollPos=0;
	UINT32 pIconID[2] = {0,0};
	AlarmIndex *AlarmIndexRecord;
	AlarmIndexRecord=g_AlarmIndexList->GetAlarmIndex();
	for(int i=0;i<g_AlarmIndexList->GetAlarmIndexCount();i++)
	{
		AlarmRecord.Load(AlarmIndexRecord[i].AlarmID);
		if(nScrollPos==0)
		{
			if(AlarmRecord.IsToday()==TRUE)
				nScrollPos=i+1;
		}
		memset((char*)szAlarmStr,0x0,60);
		TextEnd=CharPre(AlarmRecord.Todo,&(AlarmRecord.Todo[10]));
		*TextEnd=0x00;
		if(AlarmRecord.Repeat!=ALARM_DAILY)
		{
			AlarmRecord.GetEventDateStr(szAlarmStr);
			memset(&(szAlarmStr[strlen(szAlarmStr)]),0x20,17-strlen(szAlarmStr));
			
		}
		else
			szAlarmStr[0]=0x0;
		AlarmRecord.GetTimeStr(&(szAlarmStr[strlen(szAlarmStr)]));
		sprintf((char*)&(szAlarmStr[strlen(szAlarmStr)])," %s",(char*)(AlarmRecord.Todo));
		pText[0]=szAlarmStr;
		if(AlarmRecord.Repeat==ALARM_DAILY&&AlarmRecord.WeekDay==ALARM_DAILY_NOWEEK)
			pIconID[0]=BMP_ALARM_NOWORK;
		ListGrid.AddRow(pText,pStyle,pIconID);
		pIconID[0]=0;
	}
	if(nScrollPos)
		ListGrid.SetStartRow(nScrollPos,FALSE);
	g_AlarmIndexList->ModifyFlag=FALSE;//修改索引表修改标志
}
BOOL CRepeatFindList::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	NewButton.Create((char*)szRepeatFindText[0],
						CRect(  m_sRect.left+77, 
								m_sRect.top, 
								m_sRect.left+109, 
								m_sRect.top+WND_TITLE_HEIGHT-1
							 ),this,ALARM_NEW);
	DeleteButton.Create((char*)"全删",
						CRect(  m_sRect.left+110, 
								m_sRect.top, 
								m_sRect.left+142, 
								m_sRect.top+WND_TITLE_HEIGHT-1
							 ),this,ALARM_DELETE);
	
	ListGrid.Create(this,
					CRect(m_sRect.left, 
					      m_sRect.top+WND_TITLE_HEIGHT+1, 
					      m_sRect.right-18, m_sRect.bottom
					     ), 
					 WS_CHILD|TBS_SPACELINE|TBS_ROWSELECT , 0 , 1 , ID_REPEATLIST_GRID
					 );
	g_AlarmIndexList->Init(GetSysDate(),nFindKind);
	g_AlarmIndexList->FindIndexList();//获得索引表
	SetDisplayText();//设置显示文本
	return TRUE;
}
void CRepeatFindList::OnActive()
{
	if(g_AlarmIndexList->ModifyFlag)
	{
		INT16 CourrentRow,TotalRows;
		CourrentRow = LOWORD(ListGrid.GetSelectedRowCol());
		ListGrid.RemoveAll();
		SetDisplayText();//设置显示文本
		TotalRows=ListGrid.GetTotalRows();
		if(CourrentRow>TotalRows)
			CourrentRow=TotalRows;
		ListGrid.SetSelRow(CourrentRow);
	}
	CAppWnd::OnActive();
}
BOOL CRepeatFindList::OnDestroy()
{
	BOOL Flag;
	Flag=CAppWnd::OnDestroy();
	if(Flag==FALSE)
		return FALSE;
	if(CreateAlarmWnd)
		delete CreateAlarmWnd;
	if(AlarmViewWnd)
		delete AlarmViewWnd;
	CreateAlarmWnd=NULL;
	AlarmViewWnd=NULL;
	return TRUE;
}
void CRepeatFindList::OnPaint(CDC &dc)
{
	
	CAppWnd::OnPaint(dc);
}
void CRepeatFindList::OnCreatNewAlarm(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	if(IsCanAddNewAlarm()==FALSE)//记录已满
		return;
	if(CreateAlarmWnd==NULL)
		CreateAlarmWnd=new CCreateNewAlarm;
	CreateAlarmWnd->Create(nFindKind,0,FALSE);
	CreateAlarmWnd->ShowWindow();
} 
//处理全删
void CRepeatFindList::OnDeleteAlarm(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	CAlarm AlarmRecord;
	AlarmIndex *AlarmIndexRecord;
	char WndTitle[50];
	BOOL ReturnFlag;
	UINT32 nHelpID;
	GetWindowTitleAndHelpID(nFindKind,WndTitle,&nHelpID);
	AlarmIndexRecord=g_AlarmIndexList->GetAlarmIndex();
	if(g_AlarmIndexList->GetAlarmIndexCount())
	{
		if(::MessageBox("是否要删除全部提醒？" , WndTitle, MB_YESNO|MB_ICONQUESTION)!= IDYES)
			return;
	}
	int AlarmCount=g_AlarmIndexList->GetAlarmIndexCount();
	for(int i=0;i<AlarmCount;i++)
	{
		
		ReturnFlag=DeleteAlarm(AlarmIndexRecord[i].AlarmID);	// 成功返回true, 失败返回false
		if(ReturnFlag==FALSE)
		{	
			::MessageBox("删除提醒失败！",WndTitle, MB_OK|MB_ICONSTOP);
			return;
		}
		//g_AlarmIndexList->Delete(AlarmIndexRecord[i].AlarmID);
	}
	g_AlarmIndexList->DeleteAll();
	Alarm_SetAlarm();
	ListGrid.SetSelRow(0);
	ListGrid.RemoveAll();
}
void CRepeatFindList::OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	short nRow , nCol ;
	AlarmIndex *AlarmIndexRecord;
	AlarmIndexRecord=g_AlarmIndexList->GetAlarmIndex();
	nRow = LOWORD(lParam);
	nCol = HIWORD(lParam);
	if(AlarmViewWnd==NULL)
		AlarmViewWnd=new CAlarmView;
	AlarmViewWnd->Create(AlarmIndexRecord[nRow-1].AlarmID);
	AlarmViewWnd->ShowWindow();
	
	
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//今日索引窗口
//---------------------------------------------------------------
//---------------------------------------------------------------

#define IDB_TODAYLIST_PREVBUTTON           0x11
#define IDB_TODAYLIST_NEXTBUTTON           0x12
#define IDG_TODAYLIST_GRID                 0x13
#define IDMB_TODAYLIST_MENUBUTTON           0x14
#define IDM_TODAYLIST_TODAY                  0x15
#define IDM_TODAYLIST_MONTH                0x16
BEGIN_MESSAGE_MAP(CTodayAlarmList, CAppWnd)
ON_MESSAGE(IDG_TODAYLIST_GRID, CN_GRIDSEL , OnGridSel)
ON_MESSAGE(IDB_TODAYLIST_PREVBUTTON, CN_COMMAND , OnPrevDay)
ON_MESSAGE(IDB_TODAYLIST_NEXTBUTTON, CN_COMMAND , OnNextDay)
END_MESSAGE_MAP

BOOL CTodayAlarmList::Create(CDate Date)
{
	
	AlarmViewWnd=NULL;
	NowDate=Date;
	if(!CAppWnd::Create((char*)(szAlarmMainText[8]), WS_CAPTION |WS_VISIBLE, 
			DESK_TOP_RECT, HELP_ALARM_TODAY))
		return FALSE;
	return TRUE;
}

void CTodayAlarmList::SetDisplayText()
{		
	char szAlarmStr[60];
	char *TextEnd;
	CAlarm AlarmRecord;
	char *pText[2]={NULL,NULL};
	char pStyle[2]={0,0};
	UINT32 pIconID[2] = {0,0};
	AlarmIndex *AlarmIndexRecord;
	AlarmIndexRecord=g_AlarmIndexList->GetAlarmIndex();
	for(int i=0;i<g_AlarmIndexList->GetAlarmIndexCount();i++)
	{
		AlarmRecord.Load(AlarmIndexRecord[i].AlarmID);
		memset((char*)szAlarmStr,0x00,60);
		TextEnd=CharPre(AlarmRecord.Todo,&(AlarmRecord.Todo[10]));
		*TextEnd=0x00;
		AlarmRecord.GetTimeStr(szAlarmStr);
		sprintf((char*)&(szAlarmStr[5])," %s",(char*)(AlarmRecord.Todo));
		pText[0]=szAlarmStr;
		ListGrid.AddRow(pText,pStyle,pIconID);
	}
	g_AlarmIndexList->ModifyFlag=FALSE;//修改索引表修改标志
}
BOOL CTodayAlarmList::OnCreate()
{ 
	if(!CAppWnd::OnCreate())
		return FALSE;
	PrevDayButton.Create("",ICON_LEFTARROW,
						CRect(  m_sRect.left+112, 
								m_sRect.top+WND_TOOLBAR_HEIGHT, 
								m_sRect.left+128, 
								m_sRect.top+WND_TOOLBAR_HEIGHT+16
							 ),this,IDB_TODAYLIST_PREVBUTTON);

	NextDayButton.Create("",ICON_RIGHTARROW,
						CRect(  m_sRect.left+129, 
								m_sRect.top+WND_TOOLBAR_HEIGHT, 
								m_sRect.left+145, 
								m_sRect.top+WND_TOOLBAR_HEIGHT+16
							 ),this,IDB_TODAYLIST_NEXTBUTTON);

	ListGrid.Create(this,
					CRect(m_sRect.left, 
					      m_sRect.top+WND_TITLE_HEIGHT+20, 
					      m_sRect.right-18, m_sRect.bottom
					     ), 
					 WS_CHILD|TBS_SPACELINE|TBS_ROWSELECT , 0 , 1 , IDG_TODAYLIST_GRID
					 );

	g_AlarmIndexList->Init(NowDate,ALARM_INDEX_KIND_TODAY);
	g_AlarmIndexList->FindIndexList();//获得索引表
	SetDisplayText();//设置显示文本
	return TRUE;
}
void CTodayAlarmList::OnActive()
{
	if(g_AlarmIndexList->ModifyFlag) 
	{
		INT16 CourrentRow,TotalRows;
		CourrentRow = LOWORD(ListGrid.GetSelectedRowCol());
		ListGrid.RemoveAll();
		TotalRows=ListGrid.GetTotalRows();
		if(CourrentRow>TotalRows)
			CourrentRow=TotalRows;
		
		SetDisplayText();//设置显示文本
		ListGrid.SetSelRow(CourrentRow);
	}
	CAppWnd::OnActive();
}
BOOL CTodayAlarmList::OnDestroy()
{
	BOOL Flag;
	Flag=CAppWnd::OnDestroy();
	if(Flag==FALSE)
		return FALSE;
	if(AlarmViewWnd)
		delete AlarmViewWnd;
	AlarmViewWnd=NULL;
	return TRUE;
}
//先是当前日期
void CTodayAlarmList::DisplayDate(CDC &dc)
{
	char szDisplayDateStr[30];
	int LineStyle;
	memset((char*)szDisplayDateStr,0x00,30);
	sprintf((char*)szDisplayDateStr,"%d年%02d月%02d日",NowDate.GetYear(),NowDate.GetMonth(),NowDate.GetDay());	
	
	UINT8 BackColor;
	BackColor=dc.SetBackColor();
	dc.DrawText(0,19,(char*)szDisplayDateStr);
	
	LineStyle=dc.SetLineStyle(0);
	dc.MoveTo(2,m_sRect.top+WND_TITLE_HEIGHT+20);
	dc.LineTo(158,m_sRect.top+WND_TITLE_HEIGHT+20);
	dc.SetLineStyle(LineStyle);
	dc.SetBackColor(BackColor);
}
void CTodayAlarmList::OnPaint(CDC &dc)
{
	
	CAppWnd::OnPaint(dc);
	DisplayDate(dc);
	 
}
void CTodayAlarmList::OnPrevDay(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	NowDate-=1;
	if(NowDate.GetYear()<1901)
		NowDate+=1;
	g_AlarmIndexList->Init(NowDate,ALARM_INDEX_KIND_TODAY);
	g_AlarmIndexList->FindIndexList();//获得索引表
	ListGrid.RemoveAll();
	SetDisplayText();//设置显示文本
	ListGrid.OnPaint(CDC());
	DisplayDate(CDC());
} 
void CTodayAlarmList::OnNextDay(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	NowDate+=1;
	if(NowDate.GetYear()>2050)
		NowDate-=1;
	g_AlarmIndexList->Init(NowDate,ALARM_INDEX_KIND_TODAY);
	g_AlarmIndexList->FindIndexList();//获得索引表
	ListGrid.RemoveAll();
	SetDisplayText();//设置显示文本
	ListGrid.OnPaint(CDC());
	DisplayDate(CDC());
	
}
void CTodayAlarmList::OnGridSel(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	short nRow , nCol ;
	AlarmIndex *AlarmIndexRecord;
	AlarmIndexRecord=g_AlarmIndexList->GetAlarmIndex();
	nRow = LOWORD(lParam);
	nCol = HIWORD(lParam);
	if(AlarmViewWnd==NULL)
		AlarmViewWnd=new CAlarmView;
	AlarmViewWnd->Create(AlarmIndexRecord[nRow-1].AlarmID);
	AlarmViewWnd->ShowWindow();
	
	
}
CDate CTodayAlarmList::GetNowDate()
{
	return NowDate;
}
void CTodayAlarmList::SetNowDate(CDate Date)
{
	NowDate=Date;
	g_AlarmIndexList->Init(NowDate,ALARM_INDEX_KIND_TODAY);
	g_AlarmIndexList->FindIndexList();//获得索引表
	ListGrid.RemoveAll();
	SetDisplayText();//设置显示文本
	ListGrid.OnPaint(CDC());

	DisplayDate(CDC());
}
//---------------------------------------------------------------
//---------------------------------------------------------------
//提醒日历控件
//---------------------------------------------------------------
//---------------------------------------------------------------
#define ALARMCALENDAR_ROW_HEIGHT   17
#define ALARMCALENDAR_COL_WIDTH    22
void CAlarmCalendar::PaintOneDay(CDC &dc, INT16 nDay, UINT32 nIconId, INT16 startx, INT16 starty)
{
	
	if(pAlarmMonthList[nDay-1])
	{
		dc.SetBackColor(LGRAY); 
		dc.EraseRect(startx+1, starty, startx+ALARMCALENDAR_COL_WIDTH-1, starty+ALARMCALENDAR_ROW_HEIGHT-1);
		dc.SetROP2(DRAWMODE_AND); 
	}
	dc.PutIcon(startx, starty, (nDay-1) + DAY_ICON01); 
    dc.PutIcon(startx+14, starty, nIconId);
	dc.Reset(); 
}
//取得提醒月标示表
void CAlarmCalendar::SetAlarmMonthFlagList()
{
	CDate NowDate;
	NowDate.SetDate(CCalendar::GetYear(),CCalendar::GetMonth(),1);
	pAlarmMonthList=Alarm_GetMonthList(NowDate);

}

//---------------------------------------------------------------
//---------------------------------------------------------------
//今月索引窗口
//---------------------------------------------------------------
//---------------------------------------------------------------
#define IDB_MONTHLIST_PREVBUTTON           0x11
#define IDB_MONTHLIST_NEXTBUTTON           0x12
#define IDC_MONTHLIST_CALENDAR                 0x13
#define IDMB_MONTHLIST_MENUBUTTON           0x14
#define IDM_MONTHLIST_TODAY                  0x15
#define IDM_MONTHLIST_MONTH                0x16
BEGIN_MESSAGE_MAP(CMonthAlarmList, CAppWnd)
ON_MESSAGE(IDC_MONTHLIST_CALENDAR, CN_COMMAND , OnGetDay)
ON_MESSAGE(IDB_MONTHLIST_PREVBUTTON, CN_COMMAND , OnPrevMonth)
ON_MESSAGE(IDB_MONTHLIST_NEXTBUTTON, CN_COMMAND , OnNextMonth)

END_MESSAGE_MAP

BOOL CMonthAlarmList::Create(CDate Date)
{
	
	
	Month_TodayAlarmWnd=NULL;
	NowDate=Date;
	if(!CAppWnd::Create((char*)(szAlarmMainText[9]), WS_CAPTION |WS_VISIBLE, 
			DESK_TOP_RECT, HELP_ALARM_MONTH))
		return FALSE;
	
	return TRUE;
}
BOOL CMonthAlarmList::OnCreate()
{ 
	if(!CAppWnd::OnCreate())
		return FALSE;
	PrevMonthButton.Create("",ICON_LEFTARROW,
						CRect(  m_sRect.left+102, 
								m_sRect.top+WND_TOOLBAR_HEIGHT+2, 
								m_sRect.left+118 ,
								m_sRect.top+WND_TOOLBAR_HEIGHT+18
							 ),this,IDB_MONTHLIST_PREVBUTTON);

	NextMonthButton.Create("",ICON_RIGHTARROW,
						CRect(  m_sRect.left+119, 
								m_sRect.top+WND_TOOLBAR_HEIGHT+2, 
								m_sRect.left+135, 
								m_sRect.top+WND_TOOLBAR_HEIGHT+18
							 ),this,IDB_MONTHLIST_NEXTBUTTON);
	
	m_Calendar.SetDate(NowDate.GetYear(),NowDate.GetMonth(),NowDate.GetDay());
	m_Calendar.Create(CPoint(2,40),this,IDC_MONTHLIST_CALENDAR,TRUE);
	return TRUE;
}
CDate GetDifMonth(CDate Date,int Months)
{
	int Year,Month,Day;
	int YearOffset,MonthOffset;
	Year=Date.GetYear();
	Month=Date.GetMonth();
	Day=Date.GetDay();

	YearOffset=Months/12;
	MonthOffset=Months%12;
	Month+=MonthOffset;
	if(Month>12)
	{
		Month-=12;
		YearOffset++;
	}
	else
	{
		if(Month<1)
		{
			Month+=12;
			YearOffset--;
		}
	}
	Year+=YearOffset;
	if(Year<1901)
	{	
		Year=2050;
		Month=12;
	}
	else
	{
		if(Year>2050)
		{
			Year=1901;
			Month=1;
		}
	}
	int MonthDays;
	MonthDays=CDate::MonthDays(Year,Month);
	if(Day>MonthDays)
		Day=MonthDays;
	Date.SetDate(Year,Month,Day);
	return Date;
}
void CMonthAlarmList::OnPrevMonth(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	NowDate=GetDifMonth(NowDate,-1);
	m_Calendar.SetDate(NowDate.GetYear(),NowDate.GetMonth(),NowDate.GetDay());
	m_Calendar.SetAlarmMonthFlagList();
	m_Calendar.OnPaint(CDC());
	OnPaintMonthText(CDC());
	
} 
void CMonthAlarmList::OnNextMonth(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	NowDate=GetDifMonth(NowDate,1);
	m_Calendar.SetDate(NowDate.GetYear(),NowDate.GetMonth(),NowDate.GetDay());
	m_Calendar.SetAlarmMonthFlagList();
	m_Calendar.OnPaint(CDC());
	OnPaintMonthText(CDC());
}
void CMonthAlarmList::OnGetDay(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	INT16 nYear,nMonth,nDay;
	m_Calendar.GetDate(nYear,nMonth,nDay);
	NowDate.SetDate(nYear,nMonth,nDay);
	if(Month_TodayAlarmWnd==NULL)
		Month_TodayAlarmWnd=new CTodayAlarmList;
	Month_TodayAlarmWnd->Create(NowDate);
	Month_TodayAlarmWnd->ShowWindow();
}
BOOL CMonthAlarmList::OnDestroy()
{
	BOOL Flag;
	Flag=CAppWnd::OnDestroy();
	if(Flag==FALSE)
		return FALSE;
	if(Month_TodayAlarmWnd)
		delete Month_TodayAlarmWnd;
	Month_TodayAlarmWnd=NULL;
	return TRUE;
}
//显示公历日期
void CMonthAlarmList::OnPaintMonthText(CDC &dc)
{
	char szDisplayDateStr[20];
	memset((char*)szDisplayDateStr,0x00,20);
	sprintf((char*)szDisplayDateStr,"%d年%02d月",NowDate.GetYear(),NowDate.GetMonth());	
	UINT8 BackColor;
	BackColor=dc.SetBackColor();
	dc.DrawText(20,21,(char*)szDisplayDateStr);
	dc.MoveTo(0,38);
	dc.LineTo(160,38);
	dc.SetBackColor(BackColor);
}
void CMonthAlarmList::OnPaint(CDC &dc)
{
	m_Calendar.SetAlarmMonthFlagList();
	CAppWnd::OnPaint(dc);
	OnPaintMonthText(dc);
}
//---------------------------------------------------------------
//---------------------------------------------------------------
//提醒主界面
//---------------------------------------------------------------
//---------------------------------------------------------------


BEGIN_MESSAGE_MAP(CAlarmMain, CAppWnd)

ON_MESSAGE(IDM_ONEALARM_INDEX , CN_COMMAND , OnAlarmMainButton)
ON_MESSAGE(IDM_DAYALARM_INDEX , CN_COMMAND , OnAlarmMainButton)
ON_MESSAGE(IDM_WEEKALARM_INDEX , CN_COMMAND , OnAlarmMainButton)
ON_MESSAGE(IDM_MONTHALARM_INDEX , CN_COMMAND , OnAlarmMainButton)
ON_MESSAGE(IDM_YEARALARM_INDEX , CN_COMMAND , OnAlarmMainButton)
ON_MESSAGE(IDM_TODAYALARM_INDEX , CN_COMMAND , OnAlarmMainButton)
ON_MESSAGE(IDM_THISMONTHALARM_INDEX , CN_COMMAND , OnAlarmMainButton)

END_MESSAGE_MAP
BOOL CAlarmMain::Create()
{
	RepeatFindWnd=NULL;
	TodayAlarmWnd=NULL;
	MonthAlarmWnd=NULL;
	g_AlarmIndexList=NULL;
	g_AlarmIndexList=new CAlarmIndexList;
	if(!CAppWnd::Create((char*)(szAlarmMainText[0]), WS_CAPTION |WS_VISIBLE, 
			DESK_TOP_RECT, HELP_ALARM_MAIN))
		return FALSE;
	return TRUE;
}
BOOL CAlarmMain::OnCreate()
{
	if(!CAppWnd::OnCreate())
		return FALSE;
	OneAlarmButton.Create((char*)(szAlarmMainText[3]),CRect(3,41,77,58),this,IDM_ONEALARM_INDEX);
	DayAlarmButton.Create((char*)(szAlarmMainText[4]),CRect(81,41,155,58),this,IDM_DAYALARM_INDEX);
	WeekAlarmButton.Create((char*)(szAlarmMainText[5]),CRect(3,62,77,79),this,IDM_WEEKALARM_INDEX);
	MonthAlarmButton.Create((char*)(szAlarmMainText[6]),CRect(81,62,155,79),this,IDM_MONTHALARM_INDEX);
	YearAlarmButton.Create((char*)(szAlarmMainText[7]),CRect(3,83,77,100),this,IDM_YEARALARM_INDEX);
	TodayAlarmButton.Create((char*)(szAlarmMainText[8]),CRect(3,138,77,155),this,IDM_TODAYALARM_INDEX);
	ThisMonthAlarmButton.Create((char*)(szAlarmMainText[9]),CRect(81,138,155,155),this,IDM_THISMONTHALARM_INDEX);
	return TRUE;
}
BOOL CAlarmMain::OnDestroy()
{
	BOOL Flag;
	Flag=CAppWnd::OnDestroy();
	if(Flag==FALSE)
		return FALSE;
	if(RepeatFindWnd)
		delete RepeatFindWnd;
	if(TodayAlarmWnd)
		delete TodayAlarmWnd;
	if(MonthAlarmWnd)
		delete MonthAlarmWnd;
	if(g_AlarmIndexList)
		delete g_AlarmIndexList;
	RepeatFindWnd=NULL;
	TodayAlarmWnd=NULL;
	MonthAlarmWnd=NULL;
	g_AlarmIndexList=NULL;
	::SetCurrentApp();
	return TRUE;
}
void CAlarmMain::OnPaint(CDC &dc)
{
	CAppWnd::OnPaint(dc);
	UINT8 BackColor,LineStyle;
	BackColor=dc.SetBackColor();
	dc.DrawText(48,19,(char*)(szAlarmMainText[1]));
	dc.DrawText(48,116,(char*)(szAlarmMainText[2]));
	LineStyle=dc.SetLineStyle(0);
	dc.MoveTo(2,37);
	dc.LineTo(158,37);
	dc.MoveTo(2,134);
	dc.LineTo(158,134);
	dc.SetLineStyle(LineStyle);
	dc.SetBackColor(BackColor);
	
}
void CAlarmMain::OnAlarmMainButton(UINT32  nMessage , UINT32 wParam , UINT32 lParam)
{
	UINT16 Kind=0;
	switch(wParam)
	{
	case IDM_ONEALARM_INDEX:
		Kind=ALARM_ONE;
		break;
    case IDM_DAYALARM_INDEX:
		Kind=ALARM_DAILY;
		break;
	case IDM_WEEKALARM_INDEX:
		Kind=ALARM_WEEKLY;
		break;
	case IDM_MONTHALARM_INDEX: 
		Kind=ALARM_MONTHLY;
		break;
	case IDM_YEARALARM_INDEX:
		Kind=ALARM_ANNUALLY;
		
		break;
	}
	if(Kind>0)
	{
		if(RepeatFindWnd==NULL)
			RepeatFindWnd=new CRepeatFindList;
		RepeatFindWnd->Create(Kind);
		RepeatFindWnd->ShowWindow();
		return ;
	}
	
	switch(wParam)
	{
	case IDM_TODAYALARM_INDEX:
		if(TodayAlarmWnd==NULL)
			TodayAlarmWnd=new CTodayAlarmList;
		TodayAlarmWnd->Create(GetSysDate());
		TodayAlarmWnd->ShowWindow();
		break;
	case IDM_THISMONTHALARM_INDEX:
		if(MonthAlarmWnd==NULL)
			MonthAlarmWnd=new CMonthAlarmList;
		MonthAlarmWnd->Create(GetSysDate());
		MonthAlarmWnd->ShowWindow();
		
		break;
	}
}

void AlarmAppRun()
{
	if(!g_AlarmApp)
		g_AlarmApp = new CAlarmMain;
	::SetCurrentApp(APPID_ALARM);
	g_AlarmApp->Create();
	g_AlarmApp->ShowWindow();

	if(g_pAlarmDisplay->GetNotDispAlarmCount())//显示未看提醒
	{	
		g_pAlarmDisplay->ShowWindow(SW_SHOW);
	}
}
//取得当前时刻等待的报警个数 
int Alarm_GetAlarmCount()
{
	return g_pAlarmList->GetAlarmCount();
}
//设置报警
void Alarm_SetAlarm()
{
	g_pAlarmList->SetAlarm();
}
//取得当前时刻等待的报警队列  
CAlarm *Alarm_GetAlarmList()
{
	return g_pAlarmList->GetAlarmList();
}
//将提醒从延后提醒队列中删除
void Alarm_DeleteFromLaterAlarmList(UINT16 ID)
{
	g_pAlarmList->DeleteFormLaterAlarmList(ID);
}
//将提醒插入延后提醒队列
void Alarm_InsertToLaterAlarmList(CAlarm AlarmRecord)
{
	g_pAlarmList->DeleteFormLaterAlarmList(AlarmRecord.ID);
	g_pAlarmList->InsertToLaterAlarmList(AlarmRecord);
}
//取得该月提醒表
UINT8 *Alarm_GetMonthList(CDate NowDate)
{
	return g_pAlarmList->GetMonthList(NowDate);
}
 
void initAlarm()
{
	if(g_pAlarmList==NULL)
		g_pAlarmList=new CAlarmList;
	
	if(g_pAlarmDisplay==NULL)
	{
		g_pAlarmDisplay=new CAlarmDisplay;
		g_pAlarmDisplay->Create();
	}
	Alarm_SetAlarm();

}
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//月重复时选择天控件
//-------------------------------------------------------------------
//-------------------------------------------------------------------

BOOL CSelectDayCtrl::Create(INT16 nDay,CPoint &ptLeftTop, CFrameWnd *pParent, UINT32 nCtrlId)
{	
	m_SelectRect=0;
	SetSelectDay(nDay);
	CRect ctrRect(ptLeftTop.x, ptLeftTop.y, ptLeftTop.x + SELECTDAY_COL_WIDTH * 7,  ptLeftTop.y + SELECTDAY_ROW_HEIGHT*5-1);
	return CCtrl::Create(NULL, ctrRect, pParent, WS_CHILD|WS_VISIBLE, nCtrlId); 
}
void CSelectDayCtrl::SetSelectDay(INT16 nDay)
{
	nSelectDay=nDay;
	if(nSelectDay<1||nSelectDay>31)
		nSelectDay=1;
}
void CSelectDayCtrl::GetSelectDay(INT16 &nDay)
{
	nDay=nSelectDay;
}
void CSelectDayCtrl::OnPaint(CDC &dc)
{	
	INT16 iDay =1,starty =m_sRect.top;
	int startx;
	for(int i=0; i<5; i++, starty += SELECTDAY_ROW_HEIGHT)
	{
		startx = m_sRect.left +1;
		dc.EraseRect(m_sRect.left, starty, m_sRect.right+1, starty + SELECTDAY_ROW_HEIGHT-1); 
		for(int j=0; j<7; j++,startx += SELECTDAY_COL_WIDTH)
		{
			if(iDay > 31)
				break;
			dc.PutIcon(startx+3, starty, (iDay-1) + DAY_ICON01); 
			if(iDay==nSelectDay)
			{
				m_SelectRect=CRect(startx,starty,startx+SELECTDAY_COL_WIDTH,starty+SELECTDAY_ROW_HEIGHT-1);
				CDC::InvertRect(m_SelectRect);
			}
			iDay++;
		}//end of j 
	} //end of i
}
UINT16 CSelectDayCtrl::CalcPenDay(CPoint &pt)
{
    UINT16 iRow = (pt.y - m_sRect.top )/SELECTDAY_ROW_HEIGHT;
    UINT16 iCol = (pt.x - m_sRect.left )/SELECTDAY_COL_WIDTH;
    return iRow *7 + iCol + 1 ;
}
void CSelectDayCtrl::SetSelectRect(UINT16 nDay)
{
    if(nDay)
	{
	    UINT16 iRow = (nDay-1)/7;
	    UINT16 iCol = (nDay-1)%7;
	    m_SelectRect.left   = m_sRect.left + iCol * SELECTDAY_COL_WIDTH+2;
	    m_SelectRect.right  = m_SelectRect.left + SELECTDAY_COL_WIDTH-1;
	    m_SelectRect.top    = m_sRect.top + iRow * SELECTDAY_ROW_HEIGHT;
	    m_SelectRect.bottom = m_SelectRect.top + SELECTDAY_ROW_HEIGHT-1;
	}
}
void CSelectDayCtrl::WindowProcess(UINT32 nMessage, UINT32 wParam, UINT32 lParam)
{
	INT16 nDay;
	if(m_dwStyle & WS_DISABLE)
		return;
    switch(nMessage)
    {
    case WM_PENDOWN:
    case WM_PENMOVE:
		nDay=CalcPenDay(CPoint(HIWORD(wParam), LOWORD(wParam)));
		if(nDay==nSelectDay||nDay>31)
			return ;
		
		nSelectDay=nDay;
		if(m_SelectRect!=0)
		{
			CDC::InvertRect(m_SelectRect);
		}
		SetSelectRect(nSelectDay);
		CDC::InvertRect(m_SelectRect);
        break;
    }
}
#define ID_SELECTDAY  0x100
BOOL CSelectDayDialog::Create(INT16 nDay,char *StrName)
{
	m_nSelectDay=nDay;
	return CDialog::Create(StrName,WS_CAPTION|WS_BORDER,CRect(8,25,152,130));
}
BOOL CSelectDayDialog::OnCreate()
{
	m_SelectDayCtrl.Create(m_nSelectDay,CPoint(m_sRect.left+1,m_sRect.top+4+WND_TITLE_HEIGHT), this, ID_SELECTDAY);
	m_obutOk.Create(NULL, ICON_OK, CRect(m_sRect.right-36,m_sRect.top,m_sRect.right-19,m_sRect.top+WND_TOOLBAR_HEIGHT-1), this, IDOK);
	return CDialog::OnCreate();
}
void CSelectDayDialog::GetSelectDay(INT16 &nDay)
{
	m_SelectDayCtrl.GetSelectDay(nDay);
}

void SelectDayBox(INT16 &nDay,char *StrName)
{
	CSelectDayDialog SelectDlg;
	SelectDlg.Create(nDay,StrName);
	if(SelectDlg.DoModal()==IDOK)
		SelectDlg.GetSelectDay(nDay);
}
