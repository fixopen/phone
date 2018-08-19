#ifndef __LOGICAL_LUNARDERDATE_H__
#define __LOGICAL_LUNARDERDATE_H__

#define START_YEAR   1901
#define END_YEAR     2050

struct LUNERADER_HUANGDAO
{
	unsigned char yi[16];
	unsigned char ji[16];
	unsigned char sha[1];
	unsigned char cheng[2];
	unsigned char dao[8];
	unsigned char cho[8];
	unsigned char cho1[2];
	unsigned char huangdao[2];
	unsigned char reverse;
};

namespace Logical
{
	class LunarderDate
	{
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

		//计算阴历闰年闰月1号，对应公历的年月日  该年如果不是闰年，返回0 否则闰年的闰年的闰月
		static UINT16 CalcLeapLunarYear(UINT16 iLunarYear, UINT16 &iYear, UINT16 &iMonth, UINT16 &iDay);

		static INT16 FormatLunarYear(UINT16 iLuanrYear, char *szBuf);
		static void FormatLunarMonth(UINT16 iMonth, char *szBuf);
		static void FormatLunarDay(UINT16 iDay, char *szBuf);
		static void FormatHolDay(UINT16 iHolDay, char *szBuf);

		static BOOL FindHuangDaoData(CTime time, char* data);

		static char *LunarderDate::FindYangLiJieri(int nM, int nD);
		static char *LunarderDate::FindNongLiJieri(int nY, int nM, int nD, int y, int m, int d);
		static int LunarderDate::FindWeekJieri(int nY, int nM, int *pDay, char *DayJieRi[]);
	};
}

#endif //__LOGICAL_TELEPHONE_H__