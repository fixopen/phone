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
		 //�ж�iYear�ǲ�������
		static BOOL IsLeapYear(UINT16 iYear)
		{return !(iYear%4)&&(iYear%100) || !(iYear%400);}

		static UINT8 WeekDay(UINT16 iYear, UINT16 iMonth, UINT16 iDay);

		static UINT16 MonthDays(UINT16 iYear, UINT16 iMonth);

		//��������iLunarYear���������
		// 1901��1��---2050��12��
		static UINT16 LunarYearDays(UINT16 iLunarYear);

		//��������iLunarYear��������·ݣ���û�з���0
		// 1901��1��---2050��12��
		static UINT16 GetLeapMonth(UINT16 iLunarYear);

		//��������iLunarYer������iLunarMonth�µ����������iLunarMonthΪ���£�
		//����Ϊ�ڶ���iLunarMonth�µ��������������Ϊ0 
		// 1901��1��---2050��12��
		static UINT32 LunarMonthDays(UINT16 iLunarYear, UINT16 iLunarMonth);

		//���㹫��iYear��iMonth��iDay�ն�Ӧ�Ľ��� 0-24��0���ǽ���
		static UINT16 IsLunarHolDay(UINT16 iYear, UINT16 iMonth, UINT16 iDay);

		//���㹫���������ڼ���������  1��1��1�� --- 65535��12��31��
		static UINT32 CalcDateDiff(UINT16 iEndYear, UINT16 iEndMonth, UINT16 iEndDay,
								 UINT16 iStartYear = START_YEAR, 
								 UINT16 iStartMonth =1, UINT16 iStartDay =1);


		//���㹫��iYear��iMonth��iDay�ն�Ӧ����������,���ض�Ӧ���������� 0-24
		//1901��1��1��---2050��12��31��
		static UINT16 GetLunarDate(UINT16 iYear, UINT16 iMonth, UINT16 iDay,
								 UINT16 &iLunarYear, UINT16 &iLunarMonth, UINT16 &iLunarDay);

		//�����1901��1��1�չ�iSpanDays������������
		//�����������Ӧ���������£��������µĵڶ������򷵻�1�� ���򷵻�0
		static UINT8   CalcLunarDate(UINT16 &iLunarYear, 
									  UINT16 &iLunarMonth,
									  UINT16 &iLuanrDay, UINT32 iSpanDays);

		//����������������1�ţ���Ӧ������������  ��������������꣬����0 ������������������
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