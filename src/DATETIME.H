 
        /*******************************************************************/
        /*                                                                 */
        /*   版权所有(c) 2001汉王科技                                      */
        /*                                                                 */
        /*******************************************************************/
        /*   文件名：   datetime.h        版本号：1.0                      */
        /*                                                                 */
        /*   组  成：   与时间日期相关的各部分                             */
        /*                                                                 */
        /*   描  述：   与时间日期相关CDate类的头文件                      */
        /*                                                                 */
        /*   作  者：   王飞                                               */
        /*                                                                 */
        /*   日  期：   2000年12月15日                                     */
        /*                                                                 */
        /*              2000年12月21日修改                                 */
        /*                  增加宏TESTDAY                                  */ 
        /*                                                                 */
        /*              2000年12月25日修改                                 */
        /*                  增加                                           */
        /*                                                                 */
        /*              2001年2月15日修改                                  */
        /*                  增加 RTC 驱动部分                              */
        /*                                                                 */
        /*              2001年3月3日修改                                   */
        /*                  增加 SetAlarmTime() 函数                       */
		/*              2001年11月16日程晓东修改                           */
		/*              添加取系统日期CDate GetSysDate()                   */
		/*              //程晓东修改SetAlarmTime()增加秒                   */        
		/*******************************************************************/


#ifndef _G301_DATETIME_H
#define _G301_DATETIME_H

#include "hwfcl.h"

#define SUN    0
#define MON    1
#define TUE    2
#define WED    3
#define THU    4
#define FRI    5
#define SAT    6

#define TESTDAY(iYear, iMonth, iDay)   (((iYear)>=START_YEAR&&(iYear)<=END_YEAR)&& \
                                       ((iMonth)<13&&(iMonth)>0)&&((iDay)>0&&(iDay)<=CDate::MonthDays(iYear, iMonth))) 

#define START_YEAR   1901
#define END_YEAR     2050




///////////////////////////////////////////////////////////////////////
//                                                                   //
//                        RTC 驱动部分                               //
//                                                                   //
///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
//RTC中断类型
#define RTCI_NONE       0           //没有中断 
#define RTCI_STOPWATCH  (0x0001)    //stopwatch
#define RTCI_SECOND     (0x0010)    //秒
#define RTCI_MINUTE     (0x0002)    //分
#define RTCI_HOUR       (0x0020)    //小时 
#define RTCI_DAY        (0x0008)    //天 
#define RTCI_ALARM      (0x0004)    //定时器

///////////////////////////////////////////////////////////////////////
// 日期时间结构定义  
/*
struct DateTime
{
    UINT16 year, month, day;
    UINT16 hour, minute, second;
    UINT16 timezone;
};
*/

/* DateTime structure */
struct DateTime
{
	short	year;
	char	month;
	char	day;
	char	hour;
	char	minute;
	char	second;
	char	reserved;

	DateTime();
	DateTime(short y, char m, char d, char h, char min, char s, char r);
	DateTime& operator=(DateTime const &dt);
	bool operator>(DateTime const &dt);
	bool operator<(DateTime const &dt);
	bool operator>=(DateTime const &dt);
	bool operator<=(DateTime const &dt);
	bool operator==(DateTime const &dt);
};

/////////////////////////////////////////////////////////////////////
// 描述: 获取系统时间,即VZ328的实时钟
// 
// 参数: 无
//
// 返回: 返回一个整数表示今天的秒数 如当前时间是：0:1:0 返回60 
//                                  如当前时间是：1:1:0 返回1*60*60+1*60 
//
extern INT32 GetSysSecond();

/////////////////////////////////////////////////////////////////////
// 描述: 获取系统时间,即VZ328的实时钟
// 
// 参数: DateTime *dtBuf 用于存放系统时间的缓存
//
// 返回:  无
//
extern void GetSysTime(DateTime * dtBuf);
/////////////////////////////////////////////////////////////////////
// 描述: 获取系统时分秒时间,即VZ328的实时钟
// 
// 参数: iHour时，iMinute 分, iSecond秒
//
// 返回:  无
//

extern void GetSysExactTime(INT16 &iHour, INT16 &iMinute, INT16 &iSecond);
//程晓东添加
//取系统日期
extern CDate GetSysDate();
/////////////////////////////////////////////////////////////////////
// 描述: 设置系统时间,即VZ328的实时钟
// 
// 参数: DateTime *dt 要设的时间值
//
// 返回:  无
//
// 说明: 在调用此函数前,用户需保证所给出的时间是合法值
//
extern void SetSysTime(DateTime * dt);

/////////////////////////////////////////////////////////////////////
// 描述: 初始化VZ328的实时钟驱动
// 
// 参数: 无
//
// 返回: 无
//
// 说明: 此函数完成以下过程:
//       1)注册实时钟中断的低级服务程序
//       2)创建实时钟高级中断
//       3)初始化相关的寄存器
//       3)设置实时钟中断每天一来一次，用于检察记年
//
extern void InitRtcDriver();

/////////////////////////////////////////////////////////////////////
// 描述: 打开某一类型的实时钟中断
// 
// 参数: UINT16 iRtcType  要打开的中断类型，类型参见上面的定义
//
// 返回: 无
//
// 说明: 如果打开某一类型的RTC中断后,中断服务程序会向当前窗口发送一个WM_RTC消息
//       消息参数里会有RTC中断的类型(参见classwnd.h中WM_RTC定义处的说明),用户只
//       需增加WM_RTC消息处理函数即可完成对RTC中断消息的处理
//
extern void OpenRtcInterrupt(UINT16 iRtcType);

/////////////////////////////////////////////////////////////////////
// 描述: 关闭某一个RTC中断
//
// 参数: UINT16 iRtcType 要关闭的RTC中断的类型
//
// 返回: 无
//
// 说明: 如果指定的类型本就没有打开,则不做任何操作,
//       如果指定的类型是RTCI_DAY,RTC的中断服务程序事实上只是不再向当前
//       窗口发送WM_RTC消息,仍然会每天来一个中断来完成YearDetect函数,实
//       现正确的记年
//
extern void CloseRtcInterrupt(UINT16 iRtcType);

/////////////////////////////////////////////////////////////////////
// 描述: 设置提醒
//
// 参数: hour, minute 分别是提醒的时、分
//
// 返回: 无
//
// 说明: 在 SetAlarm() 函数中调用，用于设置提醒时间。提醒的天默认等于
//       RTC 的天，提醒的秒默认等于 0。
//       (注意: 此函数只完成设置 ALM，并不打开 ALM 中断允许)
// cxd add second
extern void SetAlarmTime(UINT16 hour, UINT16 minute,UINT16 second);

/////////////////////////////////////////////////////////////////////
// 描述: 测试某种RTC中断是否已打开
//
// 参数: 无
//
// 返回: TRUE  已打开
//       FALSE 未打开
//
extern BOOL TestRtc(UINT16 iRtcType);

/*/////////////////////////////////////////////////////////////////////
//                                                                   //
//                          日期时间类                               //
//                                                                   //
/////////////////////////////////////////////////////////////////////*/
class CDate
{
private:
    UINT16 m_iYear, m_iMonth, m_iDay;
public:
    //构造函数
    CDate()
    {
        DateTime dtBuf;
        GetSysTime(&dtBuf);
        m_iYear  = dtBuf.year;
        m_iMonth = dtBuf.month;
        m_iDay   = dtBuf.day; 
    }

    CDate(const CDate & srcDate);
    CDate(const UINT16 iYear, const UINT16 iMonth, const UINT16 iDay);

public:    
    UINT16 GetYear() const
    {return m_iYear;} 

    UINT16 GetMonth() const
    {return m_iMonth;}

    UINT16 GetDay() const
    {return m_iDay;}

    void   GetDate(UINT16 &iYear, UINT16 &iMonth, UINT16 &iDay) const
    {
         iYear  = m_iYear;
         iMonth = m_iMonth;
         iDay   = m_iDay;
    }

    BOOL SetDate(UINT16 iYear, UINT16 iMonth, UINT16 iDay);

    UINT8 WeekDay() const;

public:

    void operator = (const CDate &srcDate);

    void operator -= (const INT16 iDiffDay);
    void operator --();

    void operator +=(const INT16 iDiffDay);
    void operator ++();
    //计算CDate 和srcDate相差的天数
    INT32 operator-(const CDate & srcDate) const;
    //比较日期大小
    BOOL  operator>(const CDate & srcDate) const;

public:
    //判断iYear是不是闰年
    static BOOL IsLeapYear(UINT16 iYear)
    {return !(iYear%4)&&(iYear%100) || !(iYear%400);}

    static UINT8 WeekDay(UINT16 iYear, UINT16 iMonth, UINT16 iDay);

    static UINT16 MonthDays(UINT16 iYear, UINT16 iMonth);

    //返回阴历iLunarYear年的总天数
    // 1901年1月---2050年12月
    static UINT16 LunarYearDays(UINT16 iLunarYear);

    //返回阴历iLunarYear年的闰月月份，如没有返回0
    // 1901年1月---2050年12月
    static UINT16 GetLeapMonth(UINT16 iLunarYear);

    //返回阴历iLunarYer年阴历iLunarMonth月的天数，如果iLunarMonth为闰月，
    //高字为第二个iLunarMonth月的天数，否则高字为0 
    // 1901年1月---2050年12月
    static UINT32 LunarMonthDays(UINT16 iLunarYear, UINT16 iLunarMonth);

    //计算公历iYear年iMonth月iDay日对应的节气 0-24，0表不是节气
    static UINT16 IsLunarHolDay(UINT16 iYear, UINT16 iMonth, UINT16 iDay);

    //计算公历两个日期间相差的天数  1年1月1日 --- 65535年12月31日
    static UINT32 CalcDateDiff(UINT16 iEndYear, UINT16 iEndMonth, UINT16 iEndDay,
                             UINT16 iStartYear = START_YEAR, 
                             UINT16 iStartMonth =1, UINT16 iStartDay =1);


    //计算公历iYear年iMonth月iDay日对应的阴历日期,返回对应的阴历节气 0-24
    //1901年1月1日---2050年12月31日
    static UINT16 GetLunarDate(UINT16 iYear, UINT16 iMonth, UINT16 iDay,
                             UINT16 &iLunarYear, UINT16 &iLunarMonth, UINT16 &iLunarDay);

    //计算从1901年1月1日过iSpanDays天后的阴历日期
    //如果计算结果对应的月是闰月，且是闰月的第二个月则返回1， 否则返回0
    static UINT8   CalcLunarDate(UINT16 &iLunarYear, 
                                  UINT16 &iLunarMonth,
                                  UINT16 &iLuanrDay, UINT32 iSpanDays);

    static INT16 FormatLunarYear(UINT16 iLuanrYear, char *szBuf);
    static void FormatLunarMonth(UINT16 iMonth, char *szBuf);
    static void FormatLunarDay(UINT16 iDay, char *szBuf);
    static void FormatHolDay(UINT16 iHolDay, char *szBuf);

};

#endif  //datetime.h end