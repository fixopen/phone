 
        /*******************************************************************/
        /*                                                                 */
        /*   版权所有(c) 2001汉王科技                                      */
        /*                                                                 */
        /*******************************************************************/
        /*   文件名：   datetime.h        版本号：1.0                      */
        /*                                                                 */
        /*   组  成：   与时间日期相关的各部分                             */
        /*                                                                 */
        /*   描  述：   与时间日期相关CDate类的源文件                      */
        /*                                                                 */
        /*   作  者：   王飞                                               */
        /*                                                                 */
        /*   日  期：   2000年12月15日                                     */
        /*                                                                 */
        /*              2000年12月19日 修改BUG                             */
        /*                                                                 */
        /*              2000年12月21日 修改，增加宏TESTDAY                 */
        /*                                                                 */
        /*              2001年1月7日 修改                                  */
        /*                  增加格式化中文字日期的函数                     */
        /*                                                                 */
        /*              2001年2月15日修改                                  */
        /*                  增加 RTC 驱动部分                              */
        /*                                                                 */
        /*              2001年3月2日修改                                   */
        /*                  修改RTC驱动部分GetSysTime的一个BUG             */
        /*                  += 运算符有问题,m_iDay是无符号型,不可能小于0   */
        /*                  += 运算符的BUG已修正                           */
        /*                  应修改部分:                                    */
        /*                  另 = -= += 应返回一个引用                      */
        /*                  ++, -- 只重载为友元函数，并返回一个引用        */
        /*                  这里只有前置 ++ --没有后置++,--                */
        /*                                                                 */
        /*              2001年3月3日修改                                   */
        /*                  增加 SetAlarmTime() 函数                       */
        /*              2001年3月3日修改                                   */
        /*                  增加 SetAlarmTime() 函数                       */
		/*              2001年11月16日程晓东修改                           */
		/*              添加取系统日期CDate GetSysDate()                   */
		/*              2001年11月27日郝庆丰修改                           */
		/*              添加取系统时分秒 GetSysExactTime()                 */                
        /*                                                                 */
		/*              2001年11月28日白亚伟                               */
		/*              修改 RTC_HISR_STACK_SIZE 大小，太小会死机          */                
        /*              //程晓东修改SetAlarmTime()增加秒                   */
		/*                                                                 */
		/*******************************************************************/


#include "datetime.h"
#include "datadrv.h"
#include "keydrv.h"
/*
*	RTC 高级中断堆栈大小，不应太小，否则死机
*/
#define		RTC_HISR_STACK_SIZE			2048


///////////////////////////////////////////////////////////////////////
//                                                                   //
//                     DateTime structure                            //
//                                                                   //
///////////////////////////////////////////////////////////////////////

DateTime::DateTime()
{
}

DateTime::DateTime(short y, char m, char d, char h, char min, char s, char r)
{
	year = y;
	month = m;
	day = d;
	hour = h;
	minute = min;
	second = s;
	reserved = r;
}

DateTime& DateTime::operator=(DateTime const &dt)
{
	memcpy(this, &dt, sizeof(DateTime));
	return *this;
}

bool DateTime::operator==(DateTime const &dt)
{
	return ((year == dt.year) && (month == dt.month) && (day == dt.day) && \
		    (hour == dt.hour) && (minute == dt.minute) && (second == dt.second));
}

bool DateTime::operator>(DateTime const &dt)
{
	if (year > dt.year)		return true;
	if (year < dt.year)		return false;
	if (month > dt.month)	return true;
	if (month < dt.month)	return false;
	if (day > dt.day)		return true;
	if (day < dt.day)		return false;
	if (hour > dt.hour)		return true;
	if (hour < dt.hour)		return false;
	if (minute > dt.minute)	return true;
	if (minute < dt.minute)	return false;
	if (second > dt.second)	return true;
	return false;
}

bool DateTime::operator<(DateTime const &dt)
{
	if (year < dt.year)		return true;
	if (year > dt.year)		return false;
	if (month < dt.month)	return true;
	if (month > dt.month)	return false;
	if (day < dt.day)		return true;
	if (day > dt.day)		return false;
	if (hour < dt.hour)		return true;
	if (hour > dt.hour)		return false;
	if (minute < dt.minute)	return true;
	if (minute > dt.minute)	return false;
	if (second < dt.second)	return true;
	return false;
}

bool DateTime::operator>=(DateTime const &dt)
{
	return ((*this > dt) || (*this == dt));
}

bool DateTime::operator<=(DateTime const &dt)
{
	return ((*this < dt) || (*this == dt));
}


///////////////////////////////////////////////////////////////////////
//                                                                   //
//                        RTC 驱动部分                               //
//                                                                   //
///////////////////////////////////////////////////////////////////////

#define RTCTIME (*((UINT32*)0xFFFFFB00))
#define DAYR    (*((UINT16*)0xFFFFFB1A))
#define RTCIENR (*((UINT16*)0xFFFFFB10))
#define RTCISR  (*((UINT16*)0xFFFFFB0E))
#define IMR     (*((UINT32*)0xFFFFF304)) 
#define ALMTIME (*((UINT32*)0xFFFFFB04))
#define ALMDAY  (*((UINT16*)0xFFFFFB1C))

// ?? bbj
#define CLS_RTC_ISR  (RTCISR &= 0xFF)   //0x3F

static UINT16          g_iSysYear = 2002;
static VOID            (*old_rtc_isr)(INT) =NULL;  
static NU_HISR         g_hRtcHisr;
static UINT16          g_iRtcIntStatus;
static UINT8           g_bDayRtcUse=0;

INT32 GetSysSecond()
{
	return ((RTCTIME>>24)&0x1F) * 3600 + ((RTCTIME>>16)&0x3F) * 60 + (RTCTIME & 0x3F);
}

void GetSysExactTime(INT16 &iHour, INT16 &iMinute, INT16 &iSecond)
{
	DateTime dtBuf;
	GetSysTime(&dtBuf);
	iHour = dtBuf.hour;
	iMinute = dtBuf.minute;
	iSecond = dtBuf.second;
}

void GetSysTime(DateTime * dtBuf)
{
    UINT16 days = DAYR & 0x1FF;
    if (days > 366)
        return;
    UINT16 month =1;
    UINT16 monthdays = CDate::MonthDays(g_iSysYear, 1);
    while(days>monthdays)
    {
        days -= monthdays;
        monthdays = CDate::MonthDays(g_iSysYear, ++month);
    }
    dtBuf->year = g_iSysYear;
    dtBuf->month = month;
    dtBuf->day = days;
    dtBuf->hour =  (RTCTIME>>24)&0x1F;
    dtBuf->minute =(RTCTIME>>16)&0x3F;
    dtBuf->second = RTCTIME & 0x3F;
	/*
	*	判断系统时钟溢出
	*/
	if (dtBuf->year > END_YEAR){
		dtBuf->year = START_YEAR;
		dtBuf->month = 1;
		dtBuf->day = 1;
		dtBuf->hour = 0;
		dtBuf->minute = 0;
		dtBuf->second = 0;
		SetSysTime(dtBuf);
	}
}
//程晓东添加
//取系统日期
CDate GetSysDate()
{
	CDate NowDate;
	UINT16 days = DAYR & 0x1FF;
    if (days > 366)
        days =366;
    UINT16 month =1;
    UINT16 monthdays = CDate::MonthDays(g_iSysYear, 1);
    while(days>monthdays)
    {
        days -= monthdays;
        monthdays = CDate::MonthDays(g_iSysYear, ++month);
    }
    NowDate.SetDate(g_iSysYear,month,days);
	return NowDate;

}
void SetSysTime(DateTime * dt)
{
    UINT16 monthday[]={0, 31, 59 ,90, 120, 151, 181, 212, 243, 273, 304, 334}; 
    UINT16 days = monthday[dt->month-1]+dt->day;
    if(CDate::IsLeapYear(dt->year) && dt->month >2)
        days++;

    g_iSysYear = dt->year; 
	*(UINT16*)SYS_DATE_SOTRE = dt->year;	// byw
    DAYR = days&0x01FF;
    RTCTIME = RTCTIME & 0xE0FFFFFF |(UINT32(dt->hour) <<24);
    RTCTIME = RTCTIME & 0xFFC0FFFF |(UINT32(dt->minute)<<16);
    RTCTIME = RTCTIME & 0xFFFFFFC0 |UINT32(dt->second); 
    // TODO: SetAlarm();
}

//每天检查一次
static void YearDetect()
{
    UINT16 days = DAYR & 0x1FF;
    if(days >365 && !CDate::IsLeapYear(g_iSysYear) || days>366)
    {
       g_iSysYear++;
		(*(UINT16*)SYS_DATE_SOTRE) ++;	// byw
       DAYR=1;
    }
    // TODO: SetAlarm();
}

VOID g301_rtc_isr(INT vector)
{
  g_iRtcIntStatus = RTCISR;
  CLS_RTC_ISR;

  if(!(RTCISR & RTCI_STOPWATCH)){
		NU_Activate_HISR(&g_hRtcHisr);
  if(++Watch_TimeOut<WATCH_TIMEOUT)
	(*(UINT8 *)0XFFFFFB0A=0);		//reset watchdog 
	//CB_PenHISR
#ifdef DEBUG
  if(LEDIsSet(3))
	  ClearLED(3);
  else
	SetLED(3);
#endif
  //else
      //The stopwatch timed out.
  }

//  if(old_rtc_isr)
//      old_rtc_isr(vector);

}

static void StoreRtcMessage(UINT16 iRtcType)
{
    SYSTEM_MESSAGE msg;

	if(iRtcType!= RTCI_ALARM)
	{
		msg.handle = 0;
		msg.wparam = iRtcType;
		msg.lparam = 0;
		msg.message = WM_RTC;
	}
	else
	{
		msg.handle = 0;
		msg.wparam = 0;
		msg.lparam = 0;
		msg.message = WM_ALARM;
	}
	
	// bbj, temp
	PostMessageToQueue(&msg);
}

static void RtcHisrEntry()
{
  if(g_iRtcIntStatus & RTCI_MINUTE)
  {
     //A 1-minute interrupt has occurred.
     StoreRtcMessage(RTCI_MINUTE);
  }
  if(g_iRtcIntStatus & RTCI_ALARM) 
  {
    // An alarm interrupt has occurred.
     StoreRtcMessage(RTCI_ALARM);
  }

  if(g_iRtcIntStatus & RTCI_DAY) 
  {
      //A 24-hour rollover interrupt has occurred.
      YearDetect();
      if(g_bDayRtcUse)
        StoreRtcMessage(RTCI_DAY);
  }
  if(g_iRtcIntStatus & RTCI_SECOND) 
  {
     //A 1Hz interrupt has occurred.
      StoreRtcMessage(RTCI_SECOND);
  }
  if(g_iRtcIntStatus & RTCI_HOUR) 
  {
      //A 1-hour interrupt has occurred.
      StoreRtcMessage(RTCI_HOUR);
  }
}

//初始化RTC驱动,设置中断服务程序
void InitRtcDriver()
{
     void *pointer;
	 pointer = SysMalloc(RTC_HISR_STACK_SIZE);
     
	 NU_Create_HISR(&g_hRtcHisr, "RtcDrvHisr", RtcHisrEntry, 2, pointer,  RTC_HISR_STACK_SIZE);

     //NU_Register_LISR(68, g301_rtc_isr, &old_rtc_isr);
     RTCIENR = RTCI_DAY;
     FirstBootTest();
     if(g_bFirstPowerOn)
     {     
     	DateTime   dtime(2002, 5, 1, 0, 0, 1, 0);
     	SetSysTime(&dtime);
     }
	 else
		 g_iSysYear = *(UINT16*)SYS_DATE_SOTRE; //ygr
     CLS_RTC_ISR;
     IMR &= 0xFFFFFFEF;
     OpenRtcInterrupt(RTCI_SECOND);
	 OpenRtcInterrupt(RTCI_DAY);
}

//打开某一个RTC中断
void OpenRtcInterrupt(UINT16 iRtcType)
{
    if(iRtcType & RTCI_DAY)
        g_bDayRtcUse = TRUE;
    if(iRtcType & RTCI_SECOND)
	OpenWatchDog();
     RTCIENR |= iRtcType;
     CLS_RTC_ISR;
}

BOOL TestRtc(UINT16 iRtcType)
{
	return (RTCIENR & iRtcType);
}

//关闭某一个RTC中断
void CloseRtcInterrupt(UINT16 iRtcType)
{
    if(iRtcType & RTCI_DAY)
    {
      g_bDayRtcUse = FALSE;
      iRtcType |= RTCI_DAY;
    }
    if(iRtcType & RTCI_SECOND)
		KillWatchDog();    
    RTCIENR &= ~iRtcType;
}

//设置提醒
//程晓东修改增加秒
void SetAlarmTime(UINT16 hour, UINT16 minute,UINT16 second)
{
    ALMDAY = DAYR;
	//cxd modify
	ALMTIME = ((hour << 24) | (minute << 16) | second);
	//ALMTIME = ((hour << 24) | (minute << 16));
}

/*/////////////////////////////////////////////////////////////////////
//                                                                   //
//                          日期时间类                               //
//                                                                   //
/////////////////////////////////////////////////////////////////////*/

extern const UINT16 g_iLunarMonthDay[];
extern const UINT8  g_cLunarMonth[];
extern const UINT8  g_cLunarHolDay[];

CDate::CDate(const CDate & srcDate)
{
        m_iYear  = srcDate.GetYear();
        m_iMonth = srcDate.GetMonth();
        m_iDay   = srcDate.GetDay();
}

CDate::CDate(const UINT16 iYear, const UINT16 iMonth, const UINT16 iDay)
{
    if(TESTDAY(iYear, iMonth, iDay))
    {
        m_iYear  = iYear;
        m_iMonth = iMonth;
        m_iDay   = iDay; 
    }
    else
    {
        m_iYear  = START_YEAR;
        m_iMonth = 1;
        m_iDay   = 1;
    }
}

void CDate::operator = (const CDate &srcDate)
{
    m_iYear  = srcDate.m_iYear;
    m_iMonth = srcDate.m_iMonth;
    m_iDay   = srcDate.m_iDay; 
    
}

void CDate::operator += (const INT16 iDiffDay)
{
    INT16   day = m_iDay;

    day += iDiffDay;
    INT16 monthdays = CDate::MonthDays(m_iYear, m_iMonth);
    while(day > monthdays)
    {
        if(++m_iMonth > 12)
        {
            m_iMonth =1;
            m_iYear ++;
        }
        day -= monthdays;
        monthdays = CDate::MonthDays(m_iYear, m_iMonth);
    }

    while(day<=0)
    {
        if(!(--m_iMonth))
        {
            m_iMonth = 12;
            m_iYear --;
        }
        day += CDate::MonthDays(m_iYear, m_iMonth); 
    }
    m_iDay = day;
}

void CDate::operator ++()
{
    *this += 1;
}

void CDate::operator -= (const INT16 iDiffDay)
{
   *this += (- iDiffDay);   
}

void CDate::operator --()
{
    *this -= 1; 
}

//计算CDate 和srcDate相差的天数
INT32 CDate::operator-(const CDate & stDate) const
{
    UINT16 iStartYear, iStartMonth, iStartDay;
    UINT16 iEndYear, iEndMonth, iEndDay;
    INT8  sign=1;
    if((*this)>stDate)
    {
        stDate.GetDate(iStartYear, iStartMonth, iStartDay); 
        GetDate(iEndYear, iEndMonth, iEndDay);
    }
    else
    {
        stDate.GetDate(iEndYear, iEndMonth, iEndDay);
        GetDate(iStartYear, iStartMonth, iStartDay);
        sign = -1;
    }
    return CalcDateDiff(iEndYear, iEndMonth, iEndDay, 
                        iStartYear, iStartMonth, iStartDay)*sign;
}

BOOL  CDate::operator>(const CDate & srcDate) const
{
   if(m_iYear > srcDate.m_iYear)
       return TRUE;
   if(m_iYear < srcDate.m_iYear)
       return FALSE;
   if(m_iMonth > srcDate.m_iMonth)
       return TRUE;
   if(m_iMonth < srcDate.m_iMonth)
       return FALSE;

   return  (m_iDay > srcDate.GetDay());
}

UINT16 CDate::MonthDays(UINT16 iYear, UINT16 iMonth)
{
    switch(iMonth)
    {
    case 1:case 3:case 5:case 7:case 8:case 10:case 12:
        return 31;
        break;
    case 4:case 6:case 9:case 11:
        return 30;
        break;
    case 2:     //如果是闰年
        if(IsLeapYear(iYear))
            return 29;
        else
            return 28;
        break;
    }
    return 0;
}

BOOL CDate::SetDate(UINT16 iYear, UINT16 iMonth, UINT16 iDay)
{
    if(TESTDAY(iYear, iMonth, iDay))
    {
      m_iYear  = iYear;
      m_iMonth = iMonth;
      m_iDay   = iDay;
      return TRUE;
    }
    return FALSE;
}

UINT8 CDate::WeekDay() const
{
    return WeekDay(m_iYear, m_iMonth, m_iDay);
}

UINT8 CDate::WeekDay(UINT16 iYear, UINT16 iMonth, UINT16 iDay)
{
   //数组元素monthday[i]表示第i个月以前的总天数除以7的余数
    UINT16 monthday[]={0,3,3,6,1,4,6,2,5,0,3,5};
    UINT16 iWeekDay = (iYear-1)%7 + (iYear-1)/4 - (iYear-1)/100 +(iYear-1)/400;
    iWeekDay += (monthday[iMonth-1] + iDay) ;
    //如果iYear是闰年
    if(IsLeapYear(iYear) && iMonth>2)
        iWeekDay++;
    //返回:0,1,2,3,4,5,6表日、一、二、三、四、五、六
    return iWeekDay%7;
}

UINT32 CDate::LunarMonthDays(UINT16 iLunarYear, UINT16 iLunarMonth)
{
   if(iLunarYear < START_YEAR) 
        return 30L;

    UINT16 height =0 ,low =29;
    int iBit = 16 - iLunarMonth;

    if(iLunarMonth > GetLeapMonth(iLunarYear) && GetLeapMonth(iLunarYear))
           iBit --;

    if(g_iLunarMonthDay[iLunarYear - START_YEAR] & (1<<iBit))
            low ++;
        
    if(iLunarMonth == GetLeapMonth(iLunarYear))
        if(g_iLunarMonthDay[iLunarYear - START_YEAR] & (1<< (iBit -1)))
             height =30;
        else 
             height =29;

    return MAKELONG(low, height);
}

UINT16 CDate::LunarYearDays(UINT16 iLunarYear)
{
    UINT16 days =0;
    for(UINT16  i=1; i<=12; i++)
    { 
        UINT32  tmp = LunarMonthDays(iLunarYear ,i); 
        days += HIWORD(tmp);
        days += LOWORD(tmp);
    }
    return days;
}

UINT16 CDate::GetLeapMonth(UINT16 iLunarYear)
{
    UINT8 flag = g_cLunarMonth[(iLunarYear - START_YEAR)/2];
    return  (iLunarYear - START_YEAR)%2 ? flag&0x0F : flag>>4;
}

UINT16 CDate::IsLunarHolDay(UINT16 iYear, UINT16 iMonth, UINT16 iDay)
{
    UINT8  flag = g_cLunarHolDay[(iYear - START_YEAR)*12+iMonth -1];
    UINT16 day;
    if(iDay <15)
         day= 15 - ((flag>>4)&0x0f);
    else
        day = ((flag)&0x0f)+15;

    if(iDay == day)
       return (iMonth-1) *2 + (iDay>15? 1: 0) +1; 
    else
       return 0;
}

UINT32 CDate::CalcDateDiff(UINT16 iEndYear, UINT16 iEndMonth, UINT16 iEndDay,
                            UINT16  iStartYear, UINT16 iStartMonth, UINT16 iStartDay)
{
    UINT16 monthday[]={0, 31, 59 ,90, 120, 151, 181, 212, 243, 273, 304, 334}; 

    //计算两个年份1月1日之间相差的天数
    UINT32 iDiffDays =(iEndYear - iStartYear)*365;
    iDiffDays += (iEndYear-1)/4 - (iStartYear-1)/4;
    iDiffDays -= ((iEndYear-1)/100 - (iStartYear-1)/100);
    iDiffDays += (iEndYear-1)/400 - (iStartYear-1)/400;

    //加上iEndYear年1月1日到iEndMonth月iEndDay日之间的天数
    iDiffDays += monthday[iEndMonth-1] +
                                   (IsLeapYear(iEndYear)&&iEndMonth>2? 1: 0);
    iDiffDays += iEndDay;

    //减去iStartYear年1月1日到iStartMonth月iStartDay日之间的天数
    iDiffDays -= (monthday[iStartMonth-1] + 
                          (IsLeapYear(iStartYear)&&iStartMonth>2 ? 1: 0));
    iDiffDays -= iStartDay; 
    return iDiffDays;
}

UINT16 CDate::GetLunarDate(UINT16 iYear, UINT16 iMonth, UINT16 iDay,
                             UINT16 &iLunarYear, UINT16 &iLunarMonth, UINT16 &iLunarDay)
{
       CalcLunarDate(iLunarYear, iLunarMonth, iLunarDay, 
                                  CalcDateDiff(iYear, iMonth, iDay));

       return IsLunarHolDay(iYear, iMonth, iDay);
}

UINT8 CDate::CalcLunarDate(UINT16 &iYear, UINT16 &iMonth,UINT16 &iDay, UINT32 iSpanDays)
{
    //阳历1901年2月19日为阴历1901年正月初一
    //阳历1901年1月1日到2月19日共有49天
    UINT8 rcode =0;
    if(iSpanDays <49)
    {
        iYear  = START_YEAR-1;
        if(iSpanDays <19)
        { 
          iMonth = 11;  
          iDay   = 11+UINT16(iSpanDays);
        }
        else
        {
            iMonth = 12;
            iDay   =  UINT16(iSpanDays) -18;
        }
        return  rcode;
    }
    //下面从阴历1901年正月初一或2000年正月初一算起
    iSpanDays -=49;

    if(iSpanDays < 36145)
        iYear  = START_YEAR;
    else
    {
        iSpanDays -= 36145;
        iYear  = START_YEAR+99;
    }

//    iYear  = START_YEAR;
    iMonth = 1;
    iDay   = 1;
    //计算年
    UINT32 tmp = LunarYearDays(iYear); 
    while(iSpanDays >= tmp)
    {
        iSpanDays -= tmp;
        tmp = LunarYearDays(++iYear);
    }
    //计算月
    tmp = LOWORD(LunarMonthDays(iYear, iMonth));
    while(iSpanDays >= tmp)
    {
        iSpanDays -= tmp;
        if(iMonth == GetLeapMonth(iYear))
        {
            tmp  = HIWORD(LunarMonthDays(iYear, iMonth));
            if(iSpanDays < tmp) 
            {
               rcode = 1;
               break;
            }
            iSpanDays -= tmp;
        }
        tmp = LOWORD(LunarMonthDays(iYear, ++iMonth));
    }
    //计算日
    iDay += UINT16(iSpanDays);
    return rcode;
}

//根据给出的年份计算对应的天干,存于szBuf中
//返回生肖的索引，0表示鼠年，其他依次
INT16 CDate::FormatLunarYear(UINT16 iYear, char *szBuf)
{
    extern const char g_szSky[];
    extern const char g_szTerra[];
    extern const char g_szBirth[];
    memcpy(szBuf,  g_szSky+((iYear-4)%10)*2,2);
    memcpy(szBuf+2,g_szTerra+((iYear-4)%12)*2,2);
    //szBuf[4] = szBuf[5] = ' ';
    memcpy(szBuf+4,g_szBirth+((iYear-4)%12)*2,2);
	szBuf[6]=0;
	return (iYear-4)%12;
    //strcpy(szBuf+6,"年");
    
}

void CDate::FormatLunarMonth(UINT16 iMonth, char *szBuf)
{
    char text[]="正二三四五六七八九十";
    if(iMonth-1<10)
    {
    //  memcpy(szBuf,"  ",2);
        memcpy(szBuf, text+(iMonth-1)*2 ,2);
        strcpy(szBuf+2 , "月");
    }
    else
    {
        memcpy(szBuf, text +18, 2);
        if(iMonth == 11)
            memcpy(szBuf+2, "一", 2);
        else
           memcpy(szBuf+2, text + 2, 2);
        strcpy(szBuf+4 , "月");
    }
}

void  CDate::FormatLunarDay(UINT16 iDay, char *szBuf)
{
    char text1[]="初十廿三";
    char text2[]="一二三四五六七八九十";
    if(iDay<1 || iDay >30)
    {
        *szBuf =0;
        return ;
    }
    if(iDay != 20 && iDay !=30)
    {
        memcpy(szBuf, text1 + (iDay-1)/10*2 ,2);
        memcpy(szBuf+2, text2 + ((iDay-1)%10)*2 ,2);
        szBuf[4]='\0';
    }
    else
    {
        memcpy(szBuf, text1 + iDay/10*2, 2);
        strcpy(szBuf+2, text2 +18);
    }
}
void  CDate::FormatHolDay(UINT16 iHolday, char *szBuf)
{
    extern const char g_szChineseHolDay[][5];
    strcpy(szBuf, g_szChineseHolDay[iHolday-1]);
}

const char g_szSky[]   = "甲乙丙丁戊己庚辛壬癸";
const char g_szTerra[] = "子丑寅卯辰巳午未申酉戌亥";
const char g_szBirth[] = "鼠牛虎兔龙蛇马羊猴鸡狗猪";
const char g_szChineseHolDay[][5]=
{
    "小寒","大寒","立春","雨水","惊蛰","春分",
    "清明","谷雨","立夏","小满","芒种","夏至",
    "小暑","大暑","立秋","处暑","白露","秋分",
    "寒露","霜降","立冬","小雪","大雪","冬至"
};
//数组gLunarDay存入阴历1901年到2050年每年中的月天数信息，
//阴历每月只能是29或30天，一年用12（或13）个二进制位表示，对应位为1表30天，否则为29天
const UINT16 g_iLunarMonthDay[]=
{
    //测试数据只有1901.1.1 --2050.12.31
  0X4ae0, 0Xa570, 0X5268, 0Xd260, 0Xd950, 0X6aa8, 0X56a0, 0X9ad0, 0X4ae8, 0X4ae0,   //1910
  0Xa4d8, 0Xa4d0, 0Xd250, 0Xd548, 0Xb550, 0X56a0, 0X96d0, 0X95b0, 0X49b8, 0X49b0,   //1920
  0Xa4b0, 0Xb258, 0X6a50, 0X6d40, 0Xada8, 0X2b60, 0X9570, 0X4978, 0X4970, 0X64b0,   //1930
  0Xd4a0, 0Xea50, 0X6d48, 0X5ad0, 0X2b60, 0X9370, 0X92e0, 0Xc968, 0Xc950, 0Xd4a0,   //1940
  0Xda50, 0Xb550, 0X56a0, 0Xaad8, 0X25d0, 0X92d0, 0Xc958, 0Xa950, 0Xb4a8, 0X6ca0,   //1950
  0Xb550, 0X55a8, 0X4da0, 0Xa5b0, 0X52b8, 0X52b0, 0Xa950, 0Xe950, 0X6aa0, 0Xad50,   //1960
  0Xab50, 0X4b60, 0Xa570, 0Xa570, 0X5260, 0Xe930, 0Xd950, 0X5aa8, 0X56a0, 0X96d0,   //1970
  0X4ae8, 0X4ad0, 0Xa4d0, 0Xd268, 0Xd250, 0Xd528, 0Xb540, 0Xb6a0, 0X96d0, 0X95b0,   //1980
  0X49b0, 0Xa4b8, 0Xa4b0, 0Xb258, 0X6a50, 0X6d40, 0Xada0, 0Xab60, 0X9370, 0X4978,   //1990
  0X4970, 0X64b0, 0X6a50, 0Xea50, 0X6b28, 0X5ac0, 0Xab60, 0X9368, 0X92e0, 0Xc960,   //2000
  0Xd4a8, 0Xd4a0, 0Xda50, 0X5aa8, 0X56a0, 0Xaad8, 0X25d0, 0X92d0, 0Xc958, 0Xa950,   //2010
  0Xb4a0, 0Xb550, 0Xb550, 0X55a8, 0X4ba0, 0Xa5b0, 0X52b8, 0X52b0, 0Xa930, 0X74a8,   //2020
  0X6aa0, 0Xad50, 0X4da8, 0X4b60, 0X9570, 0Xa4e0, 0Xd260, 0Xe930, 0Xd530, 0X5aa0,   //2030
  0X6b50, 0X96d0, 0X4ae8, 0X4ad0, 0Xa4d0, 0Xd258, 0Xd250, 0Xd520, 0Xdaa0, 0Xb5a0,   //2040
  0X56d0, 0X4ad8, 0X49b0, 0Xa4b8, 0Xa4b0, 0Xaa50, 0Xb528, 0X6d20, 0Xada0, 0X55b0,   //2050
  
};

//数组gLanarMonth存放阴历1901年到2050年闰月的月份，如没有则为0，每字节存两年
const UINT8  g_cLunarMonth[]=
{
    0X00, 0X50, 0X04, 0X00, 0X20,   //1910
    0X60, 0X05, 0X00, 0X20, 0X70,   //1920
    0X05, 0X00, 0X40, 0X02, 0X06,   //1930
    0X00, 0X50, 0X03, 0X07, 0X00,   //1940
    0X60, 0X04, 0X00, 0X20, 0X70,   //1950
    0X05, 0X00, 0X30, 0X80, 0X06,   //1960
    0X00, 0X40, 0X03, 0X07, 0X00,   //1970
    0X50, 0X04, 0X08, 0X00, 0X60,   //1980
    0X04, 0X0a, 0X00, 0X60, 0X05,   //1990
    0X00, 0X30, 0X80, 0X05, 0X00,   //2000
    0X40, 0X02, 0X07, 0X00, 0X50,   //2010
    0X04, 0X09, 0X00, 0X60, 0X04,   //2020
    0X00, 0X20, 0X60, 0X05, 0X00,   //2030
    0X30, 0Xb0, 0X06, 0X00, 0X50,   //2040
    0X02, 0X07, 0X00, 0X50, 0X03    //2050
};

//数组gLanarHoliDay存放每年的二十四节气对应的阳历日期
//每年的二十四节气对应的阳历日期几乎固定，平均分布于十二个月中
//   1月          2月         3月         4月         5月         6月   
//小寒 大寒   立春  雨水   惊蛰 春分   清明 谷雨   立夏 小满   芒种 夏至
//   7月          8月         9月         10月       11月        12月  
//小暑 大暑   立秋  处暑   白露 秋分   寒露 霜降   立冬 小雪   大雪 冬至

//数据格式说明:
//如1901年的节气为
//  1月     2月     3月   4月    5月   6月   7月    8月   9月    10月  11月     12月
// 6, 21, 4, 19,  6, 21, 5, 21, 6,22, 6,22, 8, 23, 8, 24, 8, 24, 8, 24, 8, 23, 8, 22
// 9, 6,  11,4,   9, 6,  10,6,  9,7,  9,7,  7, 8,  7, 9,  7,  9, 7,  9, 7,  8, 7, 15
//上面第一行数据为每月节气对应日期,15减去每月第一个节气,每月第二个节气减去15得第二行
// 这样每月两个节气对应数据都小于16,每月用一个字节存放,高位存放第一个节气数据,低位存放
//第二个节气的数据,可得下表

const UINT8 g_cLunarHolDay[]=
{
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1901
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1902
    0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1903
    0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //1904
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1905
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1906
    0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1907
    0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1908
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1909
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1910
    0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   //1911
    0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1912
    0X95, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1913
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1914
    0X96, 0XA5, 0X97, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1915
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1916
    0X95, 0XB4, 0X96, 0XA6, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1917
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X77,   //1918
    0X96, 0XA5, 0X97, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1919
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1920
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1921
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X77,   //1922
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   //1923
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1924
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   //1925
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1926
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1927
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1928
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1929
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1930
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1931
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1932
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1933
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1934
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1935
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1936
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1937
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1938
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1939
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1940
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1941
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1942
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1943
    0X96, 0XA5, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1944
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1945
    0X95, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1946
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1947
    0X96, 0XA5, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1948
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X79, 0X78, 0X79, 0X77, 0X87,   //1949
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1950
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   //1951
    0X96, 0XA5, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1952
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1953
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X68, 0X78, 0X87,   //1954
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1955
    0X96, 0XA5, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1956
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1957
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1958
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1959
    0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1960
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1961
    0X96, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1962
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1963
    0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1964
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1965
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1966
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1967
    0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1968
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1969
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1970
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   //1971
    0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1972
    0XA5, 0XB5, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1973
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1974
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1975
    0X96, 0XA4, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X89, 0X88, 0X78, 0X87, 0X87,   //1976
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1977
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //1978
    0X96, 0XB4, 0X96, 0XA6, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1979
    0X96, 0XA4, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1980
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X77, 0X87,   //1981
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1982
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   //1983
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //1984
    0XA5, 0XB4, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //1985
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1986
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X79, 0X78, 0X69, 0X78, 0X87,   //1987
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1988
    0XA5, 0XB4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1989
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //1990
    0X95, 0XB4, 0X96, 0XA5, 0X86, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1991
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1992
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1993
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1994
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X76, 0X78, 0X69, 0X78, 0X87,   //1995
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //1996
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //1997
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //1998
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //1999
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2000
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2001
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2002
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //2003
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2004
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2005
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2006
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   //2007
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2008
    0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2009
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2010
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   //2011
    0X96, 0XB4, 0XA5, 0XB5, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2012
    0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2013
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2014
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2015
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2016
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2017
    0XA5, 0XB4, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2018
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2019
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X86,   //2020
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2021
    0XA5, 0XB4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2022
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   //2023
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2024
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2025
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2026
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2027
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2028
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2029
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2030
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   //2031
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2032
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X86,   //2033
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X78, 0X88, 0X78, 0X87, 0X87,   //2034
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2035
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2036
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   //2037
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2038
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2039
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   //2040
    0XA5, 0XC3, 0XA5, 0XB5, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2041
    0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   //2042
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2043
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X88, 0X87, 0X96,   //2044
    0XA5, 0XC3, 0XA5, 0XB4, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   //2045
    0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   //2046
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   //2047
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA5, 0X97, 0X87, 0X87, 0X88, 0X86, 0X96,   //2048
    0XA4, 0XC3, 0XA5, 0XA5, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X86,   //2049
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X78, 0X78, 0X87, 0X87    //2050
};

// datetime.cpp end
