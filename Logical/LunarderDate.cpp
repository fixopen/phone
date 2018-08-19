#include "stdafx.h"
#include "LunarderDate.h"
UINT32 lunarInfo[] = {
    0x4bd8, 0x4ae0, 0xa570, 0x54d5, 0xd260, 0xd950, 0x5554, 0x56af, 0x9ad0, 0x55d2, 0x4ae0, 0xa5b6, 0xa4d0, 0xd250, 0xd295, 0xb54f, 0xd6a0, 0xada2, 0x95b0, 0x4977, 0x497f, 0xa4b0, 0xb4b5, 0x6a50, 0x6d40, 0xab54, 0x2b6f, 0x9570, 0x52f2, 0x4970, 0x6566, 0xd4a0, 0xea50, 0x6a95, 0x5adf, 0x2b60, 0x86e3, 0x92ef, 0xc8d7, 0xc95f, 0xd4a0, 0xd8a6, 0xb55f, 0x56a0, 0xa5b4, 0x25df, 0x92d0, 0xd2b2, 0xa950, 0xb557, 0x6ca0, 0xb550, 0x5355, 0x4daf, 0xa5b0, 0x4573, 0x52bf, 0xa9a8, 0xe950, 0x6aa0, 0xaea6, 0xab50, 0x4b60, 0xaae4, 0xa570, 0x5260, 0xf263, 0xd950, 0x5b57, 0x56a0, 0x96d0, 0x4dd5, 0x4ad0, 0xa4d0, 0xd4d4, 0xd250, 0xd558, 0xb540, 0xb6a0, 0x95a6, 0x95bf, 0x49b0, 0xa974, 0xa4b0, 0xb27a, 0x6a50, 0x6d40, 0xaf46, 0xab60, 0x9570, 0x4af5, 0x4970, 0x64b0, 0x74a3, 0xea50, 0x6b58, 0x5ac0, 0xab60, 0x96d5, 0x92e0, 0xc960, 0xd954, 0xd4a0, 0xda50, 0x7552, 0x56a0, 0xabb7, 0x25d0, 0x92d0, 0xcab5, 0xa950, 0xb4a0, 0xbaa4, 0xad50, 0x55d9, 0x4ba0, 0xa5b0, 0x5176, 0x52bf, 0xa930, 0x7954, 0x6aa0, 0xad50, 0x5b52, 0x4b60, 0xa6e6, 0xa4e0, 0xd260, 0xea65, 0xd530, 0x5aa0, 0x76a3, 0x96d0, 0x4afb, 0x4ad0, 0xa4d0, 0xd0b6, 0xd25f, 0xd520, 0xdd45, 0xb5a0, 0x56d0, 0x55b2, 0x49b0, 0xa577, 0xa4b0, 0xaa50, 0xb255, 0x6d2f, 0xada0, 0x4b63, 0x937f, 0x49f8, 0x4970, 0x64b0, 0x68a6, 0xea5f, 0x6b20, 0xa6c4, 0xaaef, 0x92e0, 0xd2e3, 0xc960, 0xd557, 0xd4a0, 0xda50, 0x5d55, 0x56a0, 0xa6d0, 0x55d4, 0x52d0, 0xa9b8, 0xa950, 0xb4a0, 0xb6a6, 0xad50, 0x55a0, 0xaba4, 0xa5b0, 0x52b0, 0xb273, 0x6930, 0x7337, 0x6aa0, 0xad50, 0x4b55, 0x4b6f, 0xa570, 0x54e4, 0xd260, 0xe968, 0xd520, 0xdaa0, 0x6aa6, 0x56df, 0x4ae0, 0xa9d4, 0xa4d0, 0xd150, 0xf252, 0xd520
};

//====================================== 返回农历 y年闰哪个月 1-12 , 没闰返回 0
UINT leapMonth(UINT y) {
    UINT lm = lunarInfo[y - 1900] & 0xf;
    return(lm == 0xf ? 0 : lm);
}


//====================================== 返回农历 y年闰月的天数
UINT leapDays(UINT y) {
    if (leapMonth(y)) {
        return((lunarInfo[y - 1899] & 0xf) == 0xf ? 30 : 29);
    } else {
        return(0);
    }
}

UINT lYearDays(UINT y) {
    UINT i, sum = 348;
    for (i = 0x8000; i > 0x8; i >>= 1) {
        sum += (lunarInfo[y - 1900] & i) ? 1 : 0;
    }
    return(sum + leapDays(y));
}

//====================================== 返回农历 y年m月的总天数
UINT monthDays(UINT y, UINT m) {
    return((lunarInfo[y - 1900] & (0x10000 >> m)) ? 30 : 29);
}

UINT sTermInfo[] = {
    0, 21208, 42467, 63836, 85337, 107014, 128867, 150921, 173149, 195551, 218072, 240693, 263343, 285989, 308563, 331033, 353350, 375494, 397447, 419210, 440795, 462224, 483532, 504758
};
COleDateTime Get24JieQi(UINT year, int nIndex) {
    COleDateTime tm = COleDateTime(1900, 1, 6, 2, 5, 0);
    COleDateTimeSpan t = COleDateTimeSpan(0, 0, static_cast<int>(31556925974.7 * (year - 1900) / 60000 + sTermInfo[nIndex]), 0);
    tm += t;
    return tm;
}

namespace Logical
{
    //数组gLunarDay存入阴历1901年到2050年每年中的月天数信息，
    //阴历每月只能是29或30天，一年用12（或13）个二进制位表示，对应位为1表30天，否则为29天
    /*   数据出错 被废弃 1906 3 4, 1915 12，1916 1， 1920 9 10， 1989 6 7， 2013 4 5， 2025 3 4，
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
    */

    //数组gLanarMonth存放阴历1901年到2050年闰月的月份，如没有则为0，每字节存两年
    const UINT8 g_cLunarMonth[] = {
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

    const UINT8 g_cLunarHolDay[] = {
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
    const char g_szSky[] = "甲乙丙丁戊己庚辛壬癸";
    const char g_szTerra[] = "子丑寅卯辰巳午未申酉戌亥";
    const char g_szBirth[] = "鼠牛虎兔龙蛇马羊猴鸡狗猪";
    const char g_szChineseHolDay[][5] = {
        "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"
    };

    UINT16 LunarderDate::MonthDays(UINT16 iYear, UINT16 iMonth) {
        switch (iMonth) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
            break;
        case 2:
            //如果是闰年
            if (IsLeapYear(iYear)) {
                return 29;
            } else {
                return 28;
            }
            break;
        }
        return 0;
    }

    /*
    UINT8 LunarderDate::WeekDay() const
    {
    return WeekDay(m_iYear, m_iMonth, m_iDay);
    }
    */
    UINT8 LunarderDate::WeekDay(UINT16 iYear, UINT16 iMonth, UINT16 iDay) {
        //数组元素monthday[i]表示第i个月以前的总天数除以7的余数
        UINT16 monthday[] = {
            0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5
        };
        UINT16 iWeekDay = (iYear - 1) % 7 + (iYear - 1) / 4 - (iYear - 1) / 100 + (iYear - 1) / 400;
        iWeekDay += (monthday[iMonth - 1] + iDay) ;
        //如果iYear是闰年
        if (IsLeapYear(iYear) && iMonth > 2) {
            iWeekDay++;
        }
        //返回:0,1,2,3,4,5,6表日、一、二、三、四、五、六
        return iWeekDay % 7;
    }

    UINT32 LunarderDate::LunarMonthDays(UINT16 iLunarYear, UINT16 iLunarMonth) {
        /*
        if(iLunarYear < START_YEAR) 
        return 30L;
        UINT16 height =0 ,low =29;
        int iBit = 16 - iLunarMonth;
        if(iLunarMonth > GetLeapMonth(iLunarYear) && GetLeapMonth(iLunarYear))
        iBit --;
        if(g_iLunarMonthDay[iLunarYear - START_YEAR] & (1<<iBit))
        low ++;
        if(iLunarMonth == GetLeapMonth(iLunarYear))
        {
        if(g_iLunarMonthDay[iLunarYear - START_YEAR] & (1<< (iBit -1)))
        height =30;
        else 
        height =29;
        }
        */
        UINT16 height = 0 ,low = 29;
        if (iLunarMonth == GetLeapMonth(iLunarYear)) {
            height = leapDays(iLunarYear);
        }
        low = monthDays(iLunarYear, iLunarMonth); 
        return MAKELONG(low, height);
    }

    UINT16 LunarderDate::LunarYearDays(UINT16 iLunarYear) {
        UINT16 days = 0;
        for (UINT16  i = 1; i <= 12; i++) {
            UINT32 tmp = LunarMonthDays(iLunarYear, i); 
            days += HIWORD(tmp);
            days += LOWORD(tmp);
        }
        return days;
    }

    UINT16 LunarderDate::GetLeapMonth(UINT16 iLunarYear) {
        /*
        UINT8 flag = g_cLunarMonth[(iLunarYear - START_YEAR)/2];
        return  (iLunarYear - START_YEAR)%2 ? flag&0x0F : flag>>4;
        */
        return leapMonth(iLunarYear);
    }

    UINT16 LunarderDate::IsLunarHolDay(UINT16 iYear, UINT16 iMonth, UINT16 iDay) {
        /*
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
        */
        int nIndex = (iMonth - 1) * 2;
        COleDateTime tm = Get24JieQi(iYear, nIndex);
        if (tm.GetDay() == iDay) {
            return nIndex + 1;
        }
        nIndex = (iMonth - 1) * 2 + 1;
        tm = Get24JieQi(iYear, nIndex);
        if (tm.GetDay() == iDay) {
            return nIndex + 1;
        }
        return 0;
    }

    UINT32 LunarderDate::CalcDateDiff(UINT16 iEndYear, UINT16 iEndMonth, UINT16 iEndDay, UINT16  iStartYear, UINT16 iStartMonth, UINT16 iStartDay) {
        UINT16 monthday[] = {
            0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
        }; 

        //计算两个年份1月1日之间相差的天数
        UINT32 iDiffDays = (iEndYear - iStartYear) * 365;
        iDiffDays += (iEndYear - 1) / 4 - (iStartYear - 1) / 4;
        iDiffDays -= ((iEndYear - 1) / 100 - (iStartYear - 1) / 100);
        iDiffDays += (iEndYear - 1) / 400 - (iStartYear - 1) / 400;

        //加上iEndYear年1月1日到iEndMonth月iEndDay日之间的天数
        iDiffDays += monthday[iEndMonth - 1] + (IsLeapYear(iEndYear) && iEndMonth > 2 ? 1 : 0);
        iDiffDays += iEndDay;

        //减去iStartYear年1月1日到iStartMonth月iStartDay日之间的天数
        iDiffDays -= (monthday[iStartMonth - 1] + (IsLeapYear(iStartYear) && iStartMonth > 2 ? 1 : 0));
        iDiffDays -= iStartDay; 
        return iDiffDays;
    }

    UINT16 LunarderDate::GetLunarDate(UINT16 iYear, UINT16 iMonth, UINT16 iDay, UINT16& iLunarYear, UINT16& iLunarMonth, UINT16& iLunarDay) {
        //运行很慢 2007.10.15
        if (iYear < START_YEAR || iYear > END_YEAR) {
            iLunarYear = 1901;
            iLunarMonth = 1;
            iLunarDay = 1;
            return 0;
        }
        CalcLunarDate(iLunarYear, iLunarMonth, iLunarDay, CalcDateDiff(iYear, iMonth, iDay));

        return IsLunarHolDay(iYear, iMonth, iDay);
    }

    UINT8 LunarderDate::CalcLunarDate(UINT16& iYear, UINT16& iMonth, UINT16& iDay, UINT32 iSpanDays) {
        //阳历1901年2月19日为阴历1901年正月初一
        //阳历1901年1月1日到2月19日共有49天
        UINT8 rcode = 0;
        if (iSpanDays < 49) {
            iYear = START_YEAR - 1;
            if (iSpanDays < 19) {
                iMonth = 11;  
                iDay = 11 + UINT16(iSpanDays);
            } else {
                iMonth = 12;
                iDay = UINT16(iSpanDays) - 18;
            }
            return  rcode;
        }
        //下面从阴历1901年正月初一或2000年正月初一算起
        iSpanDays -= 49;

        if (iSpanDays < 36145) {
            iYear = START_YEAR;
        } else {
            iSpanDays -= 36145;
            iYear = START_YEAR + 99;
        }

        //    iYear  = START_YEAR;
        iMonth = 1;
        iDay = 1;
        //计算年
        UINT32 tmp = LunarYearDays(iYear); 
        while (iSpanDays >= tmp) {
            iSpanDays -= tmp;
            tmp = LunarYearDays(++iYear);
        }
        //计算月
        tmp = LOWORD(LunarMonthDays(iYear, iMonth));
        while (iSpanDays >= tmp) {
            iSpanDays -= tmp;
            if (iMonth == GetLeapMonth(iYear)) {
                tmp = HIWORD(LunarMonthDays(iYear, iMonth));
                if (iSpanDays < tmp) {
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
    //计算阴历闰年闰月1号，对应公历的年月日
    UINT16 LunarderDate::CalcLeapLunarYear(UINT16 iLunarYear, UINT16& iYear, UINT16& iMonth, UINT16& iDay) {
        int n = LunarderDate::GetLeapMonth(iLunarYear);
        int nFirst = 0;
        if (n > 0) {
            for (int i = 1; i <= 12; i++) {
                int m = MonthDays(iLunarYear - 1, i);
                for (int j = 1; j <= m; j++) {
                    UINT16 m1, y, d;
                    GetLunarDate(iLunarYear - 1, i, j, y, m1, d);
                    if (y == iLunarYear && m1 == n && d == 1) {
                        if (nFirst == 0) {
                            nFirst = 1;
                        } else if (nFirst == 1) {
                            iYear = iLunarYear - 1;
                            iMonth = i;
                            iDay = j;
                            return i;
                        }
                    }
                }
            }
            for (int i = 1; i <= 12; i++) {
                int m = MonthDays(iLunarYear, i);
                for (int j = 1; j <= m; j++) {
                    UINT16 m1, y, d;
                    GetLunarDate(iLunarYear, i, j, y, m1, d);
                    if (y == iLunarYear && m1 == n && d == 1) {
                        if (nFirst == 0) {
                            nFirst = 1;
                        } else if (nFirst == 1) {
                            iYear = iLunarYear;
                            iMonth = i;
                            iDay = j;
                            return i;
                        }
                    }
                }
            }
        }
        return 0;
    }
    //根据给出的年份计算对应的天干,存于szBuf中
    //返回生肖的索引，0表示鼠年，其他依次
    INT16 LunarderDate::FormatLunarYear(UINT16 iYear, char* szBuf) {
        extern const char g_szSky[];
        extern const char g_szTerra[];
        extern const char g_szBirth[];

        //天干
        memcpy(szBuf, g_szSky + ((iYear - 4) % 10) * 2, 2);
        memcpy(szBuf + 2, g_szTerra + ((iYear - 4) % 12) * 2, 2);

        //  szBuf[4]=' ';
        //生肖
        memcpy(szBuf + 4, g_szBirth + ((iYear - 4) % 12) * 2, 2);
        szBuf[6] = 0;

        //生肖
        //memcpy(szBuf,g_szBirth+((iYear-4)%12)*2,2);
        //szBuf[2]=0;

        return (iYear - 4) % 12;
        //strcpy(szBuf+6,"年");
    }
    void LunarderDate::FormatLunarMonth(UINT16 iMonth, char* szBuf) {
        char text[] = "正二三四五六七八九十";
        if (iMonth - 1 < 10) {
            //memcpy(szBuf,"  ",2);
            memcpy(szBuf, text + (iMonth - 1) * 2, 2);
            strcpy_s(szBuf + 2, 14, "月");
        } else {
            memcpy(szBuf, text + 18, 2);
            if (iMonth == 11) {
                memcpy(szBuf + 2, "一", 2);
            } else {
                memcpy(szBuf + 2, text + 2, 2);
            }
            strcpy_s(szBuf + 4, 12, "月");
        }
    }

    void LunarderDate::FormatLunarDay(UINT16 iDay, char* szBuf) {
        char text1[] = "初十廿三";
        char text2[] = "一二三四五六七八九十";
        if (iDay < 1 || iDay > 30) {
            *szBuf = 0;
            return;
        }
        if (iDay != 20 && iDay != 30) {
            memcpy(szBuf, text1 + (iDay - 1) / 10 * 2, 2);
            memcpy(szBuf + 2, text2 + ((iDay - 1) % 10) * 2, 2);
            szBuf[4] = '\0';
        } else {
            memcpy(szBuf, text1 + iDay / 10 * 2, 2);
            strcpy_s(szBuf + 2, 14, text2 + 18);
        }
    }

    void LunarderDate::FormatHolDay(UINT16 iHolday, char* szBuf) {
        extern const char g_szChineseHolDay[][5];
        strcpy_s(szBuf, 6, g_szChineseHolDay[iHolday - 1]);
    }

#define YANGLIJIERI_MAX 93
    //公历节日 *表示放假日
    UINT16 dFtv[YANGLIJIERI_MAX] = {
        101, 202, 207, 210, 214, 301, 303, 308, 312, 314, 315, 317, 321,
        //0321 世界儿歌日,
        322, 323, 324, 325, 330, 401, 407, 422, 423, 424, 501, 504, 505, 508, 512, 515, 517, 518, 520, 523, 531, 601, 605, 606, 617, 623, 625, 626, 701, 702, 707, 711, 730, 801, 808, 815, 908, 910, 914, 916, 918, 920, 927, 1001,
        //1001 国际音乐日,
        1002, 1004, 1008,
        //1008 世界视觉日,
        1009, 1010, 1013, 1014, 1015, 1016, 1017, 1022, 1024, 1031, 1107, 1108, 1109, 1110, 1111, 1112, 1114, 1117, 1121, 1129, 1201, 1203, 1205, 1208, 1209, 1210, 1212, 1213, 1221, 1224, 1225, 1229, 0
    };
    char* sFtv[YANGLIJIERI_MAX] = {
        "0101 新年元旦", "0202 世界湿地日", "0207 国际声援南非日", "0210 国际气象节", "0214 情人节", "0301 国际海豹日", "0303 全国爱耳日", "0308 国际妇女节", "0312 植树节 孙中山逝世纪念日", "0314 国际警察日", "0315 国际消费者权益日", "0317 中国国医节 国际航海日", "0321 世界森林日 消除种族歧视国际日 世界儿歌日",
        //"0321 世界儿歌日",
        "0322 世界水日", "0323 世界气象日", "0324 世界防治结核病日", "0325 全国中小学生安全教育日", "0330 巴勒斯坦国土日", "0401 愚人节 全国爱国卫生运动月(四月) 税收宣传月(四月)", "0407 世界卫生日", "0422 世界地球日", "0423 世界图书和版权日", "0424 亚非新闻工作者日", "0501 国际劳动节", "0504 中国五四青年节", "0505 碘缺乏病防治日", "0508 世界红十字日", "0512 国际护士节", "0515 国际家庭日", "0517 世界电信日", "0518 国际博物馆日", "0520 全国学生营养日", "0523 国际牛奶日", "0531 世界无烟日", "0601 国际儿童节", "0605 世界环境日", "0606 全国爱眼日", "0617 防治荒漠化和干旱日", "0623 国际奥林匹克日", "0625 全国土地日", "0626 国际反毒品日", "0701 中国共产党建党日 世界建筑日", "0702 国际体育记者日", "0707 中国人民抗日战争纪念日", "0711 世界人口日", "0730 非洲妇女日", "0801 中国建军节", "0808 中国男子节(爸爸节)", "0815 日本正式宣布无条件投降日", "0908 国际扫盲日 国际新闻工作者日", "0910 教师节", "0914 世界清洁地球日", "0916 国际臭氧层保护日", "0918 九·一八事变纪念日", "0920 国际爱牙日", "0927 世界旅游日", "1001*国庆节 世界音乐日 国际老人节",
        //"1001 国际音乐日",
        "1002 国际和平与民主自由斗争日", "1004 世界动物日", "1008 全国高血压日 世界视觉日",
        //"1008 世界视觉日",
        "1009 世界邮政日 万国邮联日", "1010 辛亥革命纪念日 世界精神卫生日", "1013 世界保健日 国际教师节", "1014 世界标准日", "1015 国际盲人节(白手杖节)", "1016 世界粮食日", "1017 世界消除贫困日", "1022 世界传统医药日", "1024 联合国日 世界发展信息日", "1031 世界勤俭日", "1107 十月社会主义革命纪念日", "1108 中国记者日", "1109 全国消防安全宣传教育日", "1110 世界青年节", "1111 国际科学与和平周(本日所属的一周)", "1112 孙中山诞辰纪念日", "1114 世界糖尿病日", "1117 国际大学生节 世界学生节", "1121 世界问候日 世界电视日", "1129 国际声援巴勒斯坦人民国际日", "1201 世界艾滋病日", "1203 世界残疾人日", "1205 国际经济和社会发展志愿人员日", "1208 国际儿童电视日", "1209 世界足球日", "1210 世界人权日", "1212 西安事变纪念日", "1213 南京大屠杀(1937年)纪念日！紧记血泪史！", "1221 国际篮球日", "1224 平安夜", "1225 圣诞节", "1229 国际生物多样性日", "",
    };

#define WEEK_JIERI  11  
    //某月的第几个星期几。 5,6,7,8 表示到数第 1,2,3,4 个星期几
    char* wFtv[WEEK_JIERI] = {
        "0110 黑人日",      //一月的第一个星期日
        "0150 世界麻风日", //一月的最后一个星期日（月倒数第一个星期日）
        "0520 国际母亲节",  //五月的第二个星期日
        "0530 全国助残日",  //五月的第三个星期日
        "0630 父亲节",      //六月的第三个星期日
        "0932 国际和平日",  //九月的第三个星期二
        "0940 国际聋人节 世界儿童日",  //九月的第四个星期日
        "0950 世界海事日",             //九月的最后一个星期日       
        "1011 国际住房日",             //十月的第一个星期一
        "1013 国际减轻自然灾害日(减灾日)",  //十月的第一个星期三
        "1144 感恩节",                      //十一月的第四个星期四

    };

#define  LONGLIJIERI_MAX    11
    //农历节日
    UINT16 dlFtv[LONGLIJIERI_MAX] = {
        101, 115, 202, 323, 505, 707, 815, 909, 1208, 1223, 100
    };
    char* lFtv[LONGLIJIERI_MAX] = {
        "0101 春节", "0115 元宵节", "0202 龙抬头节", "0323 妈祖生辰 (天上圣母诞辰)", "0505 端午节", "0707 七七中国情人节", "0815 中秋节", "0909 重阳节", "1208 腊八节", "1223 小年", "0100 除夕",
    };

    //查找阳历节日
    char* LunarderDate::FindYangLiJieri(int nM, int nD) {
        UINT16 n = nM*100 + nD;
        for (int i = 0 ; i < YANGLIJIERI_MAX; i++) {
            UINT t = dFtv[i];
            if (t == n) {
                return (char*)(sFtv[i] + 5);
            }
        }
        return NULL;
    }

    //查找农历节日
    char* LunarderDate::FindNongLiJieri(int nY, int nM, int nD, int y, int m, int d) {
        //      UINT16 iYear, iMonth, iDay;
        //      UINT ret = CalcLeapLunarYear(nY, iYear, iMonth, iDay);
        // 
        //      if(ret > 0)
        //      {
        //          if((GetLeapMonth(nY) == nM) && ((m > iMonth) || (m == iMonth && d >= iDay)))
        //              return NULL;
        //      }
        if (GetLeapMonth(nY) == nM) {
            if (nM == 4 || nM == 6 || nM == 10 || nM == 11) {
                return NULL;
            }

            UINT16 iYear, iMonth, iDay;
            UINT ret = CalcLeapLunarYear(nY, iYear, iMonth, iDay);

            if (ret > 0) {
                if ((m > iMonth) || (m == iMonth && d >= iDay)) {
                    return NULL;
                }
            }
        }

        if (nM == 12) {
            int nDay = LunarMonthDays(nY, nM);
            if (nDay == nD) {
                return (char*)(lFtv[LONGLIJIERI_MAX - 1] + 5);
            }
        }

        UINT16 n = nM*100 + nD;
        for (int i = 0 ; i < LONGLIJIERI_MAX; i++) {
            if (dlFtv[i] == n) {
                return (char*)(lFtv[i] + 5);
            }
        }
        return NULL;
    }
    /*
    "0110 黑人日",
    "0150 世界麻风日", //一月的最后一个星期日（月倒数第一个星期日）
    "0520 国际母亲节",
    "0530 全国助残日",
    "0630 父亲节",
    "0932 国际和平日",
    "0940 国际聋人节 世界儿童日",
    "0950 世界海事日",
    "1011 国际住房日",
    "1013 国际减轻自然灾害日(减灾日)",
    "1144 感恩节",
    */
    //查找星期节日
    int LunarderDate::FindWeekJieri(int nY, int nM, int* pDay, char* DayJieRi[]) {
        int nWeek = WeekDay(nY, nM, 1);
        int nDayCount = MonthDays(nY, nM);
        int nWeekend = WeekDay(nY, nM, nDayCount);

        int nBiao[4][7] = {
            // 0, 1, 2, 3, 4, 5, 6
            {1,0,6,5,4,3,2},     //星期一
            {2,1,0,6,5,4,3},     //星期二
            {3,2,1,0,6,5,4},     //星期三
            {4,3,2,1,0,6,5}      //星期四

        };

        if (nM == 1) {
            int nDay = (7 - nWeek) % 7 + 1;
            pDay[0] = nDay;
            DayJieRi[0] = (char*)(wFtv[0] + 5);


            nDay = nDayCount - nWeekend;
            pDay[1] = nDay;
            DayJieRi[1] = (char*)(wFtv[1] + 5);

            return 2;
        } else if (nM == 5) {
            int nDay = (7 - nWeek) % 7 + 1 + 1 * 7;
            pDay[0] = nDay;
            DayJieRi[0] = (char*)(wFtv[2] + 5);


            nDay = (7 - nWeek) % 7 + 1 + 2 * 7;
            pDay[1] = nDay;
            DayJieRi[1] = (char*)(wFtv[3] + 5);

            return 2;
        } else if (nM == 6) {
            int nDay = (7 - nWeek) % 7 + 1 + 2 * 7;
            pDay[0] = nDay;
            DayJieRi[0] = (char*)(wFtv[4] + 5);

            return 1;
        } else if (nM == 9) {
            /*  "0932 国际和平日",*/
            int nDay = nBiao[1][nWeek] + 1 + 2 * 7;
            pDay[0] = nDay;
            DayJieRi[0] = (char*)(wFtv[5] + 5);

            nDay = (7 - nWeek) % 7 + 1 + 3 * 7;
            pDay[1] = nDay;
            DayJieRi[1] = (char*)(wFtv[6] + 5);

            nDay = nDayCount - nWeekend;
            pDay[2] = nDay;
            DayJieRi[2] = (char*)(wFtv[7] + 5);

            return 3;
        } else if (nM == 10) {
            /*
            "1011 国际住房日",
            "1013 国际减轻自然灾害日(减灾日)",
            */
            int nDay = nBiao[0][nWeek] + 1;
            pDay[0] = nDay;
            DayJieRi[0] = (char*)(wFtv[8] + 5);

            nDay = nBiao[2][nWeek] + 1;
            pDay[1] = nDay;
            DayJieRi[1] = (char*)(wFtv[9] + 5);

            return 2;
        } else if (nM == 11) {
            /*
            "1144 感恩节",
            */
            int nDay = nBiao[3][nWeek] + 3 * 7 + 1;
            pDay[0] = nDay;
            DayJieRi[0] = (char*)(wFtv[10] + 5);

            return 1;
        }
        return 0;
    }

    const char* lunader_yi[] = {
        \
        "教牛马",\
        "余事勿取",\
        "嫁娶",\
        "出行",\
        "求医",\
        "治病",\
        "祭祀",\
        "祈福",\
        "上梁",\
        "纳畜",\
        "开市",\
        "立券",\
        "开光",\
        "解除",\
        "安机械",\
        "启攒",\
        "安葬",\
        "平治道涂",\
        "求嗣",\
        "斋醮",\
        "塑绘",\
        "订盟",\
        "纳采",\
        "出火",\
        "拆卸",\
        "修造",\
        "动土",\
        "造桥",\
        "栽种",\
        "牧养",\
        "入殓",\
        "除服",\
        "成服",\
        "移柩",\
        "破土",\
        "会亲友",\
        "移徙",\
        "入宅",\
        "造屋",\
        "安床",\
        "起基",\
        "定磉",\
        "安香",\
        "挂匾",\
        "置产",\
        "沐浴",\
        "捕捉",\
        "畋猎",\
        "理发",\
        "整手足甲",\
        "谢土",\
        "立碑",\
        "修坟",\
        "破屋",\
        "坏垣",\
        "开仓",\
        "出货财",\
        "交易",\
        "纳财",\
        "补垣",\
        "塞穴",\
        "断蚁",\
        "问名",\
        "冠笄",\
        "裁衣",\
        "进人口",\
        "作灶",\
        "竖柱",\
        "作梁",\
        "开柱眼",\
        "伐木",\
        "架马",\
        "安门",\
        "造畜椆栖",\
        "掘井",\
        "入学",\
        "扫舍",\
        "开生坟",\
        "合寿木",\
        "习艺",\
        "经络",\
        "结网",\
        "安碓磑",\
        "放水",\
        "造仓",\
        "开池",\
        "合帐",\
        "修饰垣墙",\
        "赴任",\
        "纳婿",\
        "取渔",\
        "造庙",\
        "酬神",\
        "针灸",\
        "造车器",\
        "造船",\
        "合脊",\
        "诸事不宜",\
        "雕刻",\
        "作厕",\
        "筑堤",\
        "开渠",\
        "普渡",\
        "分居",\
        "归岫",\
        "割蜜",\
        "雇庸",\
        "归宁",\
        "修门"
    };
    const char* lunader_ji[] = {
        \
        "入宅",\
        "动土",\
        "破土",\
        "余事勿取",\
        "开市",\
        "安葬",\
        "嫁娶",\
        "祈福",\
        "诸事不宜",\
        "出行",\
        "行丧",\
        "纳采",\
        "订盟",\
        "上梁",\
        "斋醮",\
        "造屋",\
        "安门",\
        "安床",\
        "造船",\
        "造庙",\
        "修造",\
        "修坟",\
        "开光",\
        "探病",\
        "祭祀",\
        "谢土",\
        "移徙",\
        "作灶",\
        "治病",\
        "伐木",\
        "作梁",\
        "入殓",\
        "开仓",\
        "掘井",\
        "出火",\
        "纳畜",\
        "置产",\
        "合脊",\
        "栽种",\
        "取渔",\
        "针灸",\
        "开渠",\
        "词讼",\
        "理发",\
        "合寿木",\
        "塑绘",\
        "造桥",\
        "除服",\
        "成服",\
        "纳财",\
        "交易",\
        "赴任",\
        "立券",\
        "出货财",\
        "开池",\
        "架马",\
        "会亲友",\
        "进人口",\
        "起基",\
        "移柩",\
        "立碑",\
        "捕捉",\
        "拆卸",\
        "开柱眼",\
        "合帐",\
        "经络",\
        "牧养",\
        "入学",\
        "冠笄",\
        "定磉",\
        "竖柱",\
        "开生坟",\
        "安香",\
        "分居",\
        "造畜椆栖",\
        "挂匾",\
        "启攒",\
        "塞穴",\
        "裁衣",\
        "乘船",\
        "补垣",\
        "问名",\
        "作厕",\
        "安碓磑",\
        "扫舍"
    };

    /*
    const char *lunderyi[] ={
    "教牛马.余事勿取xixi.",\
    "嫁娶.出行.求医.治病.祭祀.祈福.上梁.纳畜.",\
    "开市.立券.开光.解除.安机械.上梁.启攒.安葬.",\
    "平治道涂.余事勿取.",\
    "求嗣.斋醮.塑绘.订盟.纳采.出火.拆卸.修造.动土.造桥.安机械.栽种.纳畜.牧养.入殓.除服.成服.移柩.破土.安葬.",\
    "嫁娶.订盟.纳采.会亲友.祭祀.安机械.移徙.入宅.造屋.安床.起基.定磉.安香.出火.挂匾.拆卸.置产.",\
    "沐浴.捕捉.畋猎.理发.整手足甲.入殓.除服.成服.破土.安葬.谢土.立碑.修坟.启攒.",\
    "祭祀.破屋.坏垣.余事勿取.",\
    "沐浴.开仓.出货财.开市.交易.立券.纳财.栽种.纳畜.牧养.畋猎.入殓.破土.安葬.",\
    "祭祀.沐浴.补垣.塞穴.断蚁.解除.余事勿取.",\
    "嫁娶.纳采.订盟.问名.祭祀.冠笄.裁衣.会亲友.进人口.纳财.捕捉.作灶.",\
    "订盟.纳采.会亲友.祭祀.斋醮.沐浴.塑绘.出火.开光.竖柱.上梁.开市.交易.立券.作梁.开柱眼.伐木.架马.安门.安床.拆卸.牧养.造畜椆栖.掘井.",\
    "交易.立券.纳财.安床.裁衣.造畜椆栖.安葬.谢土.启攒.除服.成服.修坟.立碑.移柩.入殓.",\
    "祭祀.解除.教牛马.会亲友.余事勿取.",\
    "纳采.订盟.移徙.纳财.开市.交易.立券.入宅.会亲友.解除.求医.治病.入学.安床.安门.安香.出火.拆卸.扫舍.入宅.挂匾.开生坟.合寿木.破土.修坟.启攒.入殓.",\
    "嫁娶.订盟.纳采.祭祀.祈福.求嗣.会亲友.解除.出行.入学.纳财.开市.交易.立券.习艺.经络.安床.开仓.出货财.纳畜.安葬.启攒.修坟.入殓.",\
    "祭祀.冠笄.嫁娶.会亲友.进人口.裁衣.结网.平治道涂.",\
    "祭祀.安碓硙.结网.余事勿取.",\
    "嫁娶.祭祀.沐浴.裁衣.出行.理发.移徙.捕捉.畋猎.放水.入宅.除服.成服.启攒.安葬.移柩.入殓.",\
    "破屋.坏垣.余事勿取.",\
    "纳采.订盟.祭祀.求嗣.出火.塑绘.裁衣.会亲友.入学.拆卸.扫舍.造仓.挂匾.掘井.开池.结网.栽种.纳畜.破土.修坟.立碑.安葬.入殓.",\
    "入殓.除服.成服.移柩.启攒.安葬.修坟.立碑.",\
    "祭祀.作灶.入殓.除服.余事勿取.",\
    "塑绘.开光.沐浴.冠笄.会亲友.作灶.放水.造畜椆栖.",\
    "祭祀.沐浴.祈福.斋醮.订盟.纳采.裁衣.拆卸.起基.竖柱.上梁.安床.入殓.除服.成服.移柩.启攒.挂匾.求嗣.出行.合帐.造畜椆栖.",\
    "祭祀.解除.余事勿取.",\
    "沐浴.解除.订盟.纳采.裁衣.冠笄.拆卸.修造.动土.移徙.入宅.除服.成服.移柩.破土.启攒.安葬.扫舍.修坟.伐木.纳财.交易.立券.",\
    "出行.嫁娶.订盟.纳采.入殓.安床.启攒.安葬.祭祀.裁衣.会亲友.进人口.",\
    "修饰垣墙.平治道涂.入殓.移柩.余事勿取.",\
    "会亲友.纳采.进人口.修造.动土.竖柱.上梁.祭祀.开光.塑绘.祈福.斋醮.嫁娶.安床.移徙.入宅.安香.纳畜.",\
    "祭祀.会亲友.出行.订盟.纳采.沐浴.修造.动土.祈福.斋醮.嫁娶.拆卸.安床.入殓.移柩.安葬.谢土.赴任.裁衣.竖柱.上梁.伐木.捕捉.栽种.破土.安门.",\
    "解除.破屋.坏垣.余事勿取.",\
    "塑绘.开光.出行.订盟.纳采.除服.成服.嫁娶.纳婿.入殓.移柩.启攒.安葬.立碑.",\
    "入殓.除服.成服.移柩.启攒.安葬.立碑.余事勿取.",\
    "塞穴.结网.取渔.畋猎.",\
    "纳采.祭祀.祈福.出行.会亲友.修造.动土.移徙.入宅.",\
    "纳采.嫁娶.祭祀.祈福.出行.开市.会亲友.动土.破土.启攒.",\
    "祭祀.祈福.求嗣.斋醮.入殓.除服.成服.移柩.安葬.启攒.",\
    "纳采.会亲友.竖柱.上梁.立券.入殓.移柩.安葬.启攒.",\
    "祭祀.祈福.斋醮.出行.开市.立券.动土.移徙.入宅.破土.安葬.",\
    "会亲友.求嗣.理发.冠笄.结网.捕捉.开光.理发.",\
    "祭祀.平治道涂.余事勿取.",\
    "祈福.求嗣.斋醮.纳采.嫁娶.伐木.修造.动土.移徙.入宅.造庙.安机械.开市.入殓.除服.成服.移柩.安葬.破土.谢土.",\
    "入学.习艺.出行.纳采.订盟.嫁娶.会亲友.进人口.牧养.捕捉.入殓.移柩.安葬.启攒.",\
    "祭祀.沐浴.求医.治病.扫舍.破屋.坏垣.解除.余事勿取.",\
    "祭祀.冠笄.嫁娶.拆卸.修造.动土.起基.上梁.造屋.入宅.开市.开池.塞穴.入殓.除服.成服.移柩.安葬.破土.",\
    "",\
    "塑绘.开光.祈福.求嗣.订盟.纳采.裁衣.冠笄.拆卸.修造.动土.起基.安门.安床.移徙.造仓.结网.纳畜.",\
    "祭祀.沐浴.开光.塑绘.祈福.求嗣.订盟.纳采.冠笄.裁衣.嫁娶.动土.除服.成服.移柩.破土.启攒.出行.安碓硙.放水.开市.立券.交易.",\
    "祭祀.祈福.求嗣.酬神.裁衣.安床.立券.交易.入殓.除服.成服.移柩.谢土.启攒.",\
    "裁衣.合帐.入殓.除服.成服.会亲友.纳财.",\
    "祭祀.斋醮.裁衣.合帐.冠笄.订盟.纳采.嫁娶.入宅.安香.谢土.入殓.移柩.破土.立碑.安香.会亲友.出行.祈福.求嗣.立碑.上梁.放水.",\
    "安床.合帐.入宅.问名.纳采.求嗣.祭祀.开仓.",\
    "",\
    "塑绘.开光.酬神.斋醮.订盟.纳采.裁衣.合帐.拆卸.动土.上梁.安床.安香.造庙.挂匾.会亲友.进人口.出行.修造.纳财.伐木.放水.出火.纳畜.沐浴.安门.",\
    "祭祀.祈福.酬神.订盟.纳采.冠笄.裁衣.合帐.嫁娶.安床.移徙.入宅.安香.入殓.移柩.启攒.安葬.解除.取渔.捕捉.伐木.安门.出火.",\
    "求医.破屋.",\
    "祈福.求嗣.斋醮.塑绘.开光.订盟.纳采.嫁娶.动土.入宅.安香.移柩.安葬.谢土.出行.沐浴.修造.竖柱.上梁.纳财.破土.解除.安门.放水.",\
    "取渔.入殓.除服.成服.移柩.破土.安葬.立碑.",\
    "祭祀.求嗣.沐浴.酬神.订盟.纳采.裁衣.合帐.冠笄.安机械.安床.造仓.开池.经络.纳财.开市.立券.交易.结网.取渔.纳畜.捕捉.",\
    "祭祀.沐浴.祈福.求嗣.斋醮.订盟.纳采.裁衣.冠笄.开市.立券.交易.纳财.沐浴.除服.谢土.出行.移柩.",\
    "祭祀.祈福.求嗣.入殓.启攒.安葬.移柩.",\
    "安床.解除.裁衣.竖柱.上梁.交易.立券.纳财.纳畜.牧养.入殓.移柩.安葬.启攒.",\
    "嫁娶.安床.开光.出行.祭祀.动土.出火.解除.会亲友.开市.交易.立券.挂匾.入宅.移徙.拆卸.破土.启攒.安葬.",\
    "祭祀.沐浴.解除.扫舍.塞穴.牧养.",\
    "",\
    "嫁娶.冠笄.会亲友.安机械.纳财.交易.立券.置产.",\
    "嫁娶.造车器.纳采.订盟.祭祀.祈福.安机械.移徙.入宅.开市.立券.破土.安葬.",\
    "祈福.斋醮.出行.移徙.入宅.修造.动土.破土.安葬.",\
    "破屋.坏垣.余事勿取.",\
    "嫁娶.冠笄.祭祀.出行.会亲友.修造.动土.入殓.破土.",\
    "开光.求嗣.出行.纳采.冠笄.出火.拆卸.起基.修造.动土.上梁.移徙.造船.开市.交易.立券.纳财.",\
    "理发.冠笄.嫁娶.进人口.栽种.捕捉.针灸.",\
    "开光.祈福.求嗣.出行.解除.伐木.造屋.起基.修造.架马.安门.移徙.入宅.造庙.除服.成服.移柩.谢土.纳畜.牧养.",\
    "裁衣.经络.伐木.开柱眼.拆卸.修造.动土.上梁.合脊.合寿木.入殓.除服.成服.移柩.破土.安葬.启攒.修坟.立碑.",\
    "祭祀.会亲友.立券.交易.裁衣.合帐.嫁娶.冠笄.进人口.",\
    "扫舍.塞穴.余事勿取.",\
    "塑绘.开光.订盟.纳采.裁衣.合帐.冠笄.安机械.会亲友.纳财.开市.立券.交易.安床.竖柱.上梁.结网.栽种.解除.经络.",\
    "祭祀.嫁娶.纳婿.除服.成服.入殓.移柩.",\
    "祈福.求嗣.开光.塑绘.斋醮.订盟.纳采.嫁娶.拆卸.安床.入宅.安香.移柩.修坟.安葬.谢土.栽种.解除.冠笄.裁衣.移徙.修造.动土.竖柱.放水.启攒.立碑.",\
    "祭祀.解除.入殓.除服.成服.移柩.启攒.安葬.修坟.立碑.谢土.沐浴.扫舍.捕捉.取渔.结网.畋猎.理发.",\
    "破屋.坏垣.",\
    "祭祀.出行.订盟.纳采.裁衣.合帐.冠笄.进人口.动土.安床.作灶.入殓.移柩.安葬.破土.结网.取渔.畋猎.",\
    "祭祀.开光.塑绘.订盟.纳采.合帐.冠笄.拆卸.动土.起基.上梁.入宅.安香.开市.立券.纳财.沐浴.求嗣.出火.竖柱.安门.",\
    "祭祀.沐浴.捕捉.栽种.",\
    "祭祀.开光.塑绘.酬神.斋醮.订盟.纳采.嫁娶.裁衣.动土.起基.出火.拆卸.移徙.入宅.安香.修造.竖柱.上梁.纳畜.牧养.祈福.求嗣.解除.伐木.定磉.造屋.安门.",\
    "订盟.纳采.冠笄.拆卸.修造.动土.安床.入殓.除服.成服.移柩.安葬.破土.启攒.造仓.",\
    "祈福.开光.塑绘.酬神.订盟.纳采.裁衣.安床.开市.立券.入殓.除服.成服.移柩.启攒.安葬.立碑.赴任.会亲友.出行.交易.竖柱.",\
    "祭祀.扫舍.塞穴.",\
    "开光.塑绘.裁衣.冠笄.伐木.拆卸.竖柱.上梁.开仓.会亲友.安机械.造仓.造屋.交易.解除.开市.立券.纳财.",\
    "冠笄.入殓.除服.成服.移柩.平治道涂.修饰垣墙.",\
    "祭祀.嫁娶.祈福.纳采.裁衣.合帐.安床.入宅.安香.入殓.移柩.安葬.谢土.修造.安碓硙.求嗣.会亲友.挂匾.交易.立券.纳财.造仓.放水.",\
    "祭祀.祈福.斋醮.订盟.纳采.裁衣.合帐.拆卸.修造.动土.上梁.起基.移柩.安葬.谢土.沐浴.扫舍.开柱眼.伐木.出火.",\
    "",\
    "祭祀.沐浴.解除.求医.治病.破屋.坏垣.余事勿取.",\
    "沐浴.捕捉.畋猎.结网.取渔.",\
    "祭祀.祈福.求嗣.斋醮.纳采.订盟.开光.竖柱.上梁.开仓.出货财.造屋.起基.定磉.安门.诸事不宜.破土.入殓.启攒.谢土.",\
    "祭祀.捕捉.解除.余事勿取.",\
    "纳采.嫁娶.出行.开市.立券.纳畜.牧养.出火.移徙.入宅.",\
    "祭祀.祈福.求嗣.斋醮.冠笄.作灶.纳财.交易.",\
    "祭祀.解除.教牛马.出行.余事勿取.",\
    "沐浴.斋醮.解除.求医.治病.会亲友.造畜椆栖.栽种.理发.扫舍.",\
    "求嗣.出行.解除.订盟.纳采.嫁娶.会亲友.进人口.安床.开市.交易.纳畜.牧养.入殓.除服.成服.移柩.安葬.启攒.",\
    "祭祀.作灶.平治道涂.余事勿取.",\
    "造车器.祭祀.祈福.求嗣.斋醮.开市.交易.安机械.雕刻.开光.造屋.合脊.起基.定磉.安门.纳畜.安葬.开生坟.立碑.谢土.斋醮.",\
    "祭祀.祈福.开光.求嗣.斋醮.纳采.订盟.求医.治病.起基.定磉.造船.取渔.解除.安葬.启攒.谢土.入殓.",\
    "祭祀.沐浴.破屋.坏垣.求医.治病.解除.余事勿取.",\
    "诸事不宜.",\
    "祭祀.塑绘.开光.订盟.纳采.冠笄.裁衣.安机械.拆卸.修造.动土.安床.经络.开市.",\
    "祭祀.余事勿取.",\
    "订盟.纳采.嫁娶.进人口.会亲友.交易.立券.动土.除服.谢土.移柩.破土.启攒.赴任.出行.开市.纳财.栽种.",\
    "祭祀.祈福.裁衣.合帐.安床.入殓.除服.成服.移柩.破土.启攒.安葬.谢土.立碑.造畜椆栖.",\
    "祭祀.进人口.嫁娶.安床.解除.冠笄.出行.裁衣.扫舍.",\
    "纳采.开光.求医.治病.动土.上梁.移徙.入宅.",\
    "祭祀.会亲友.开市.安床.启攒.安葬.",\
    "祭祀.作灶.掘井.平治道涂.",\
    "祭祀.斋醮.开市.动土.入殓.破土.安葬.",\
    "嫁娶.纳采.祭祀.祈福.出行.移徙.求医.",\
    "祭祀.求医.治病.解除.余事勿取.",\
    "沐浴.结网.取渔.",\
    "",\
    "解除.坏垣.余事勿取.",\
    "嫁娶.开光.出行.出火.拆卸.进人口.开市.立券.交易.挂匾.入宅.移徙.安床.栽种.",\
    "嫁娶.出行.合帐.冠笄.安床.除服.成服.作灶.交易.立券.入殓.移柩.破土.安葬.",\
    "出行.修饰垣墙.造畜椆栖.教牛马.余事勿取.",\
    "祭祀.解除.断蚁.会亲友.余事勿取.",\
    "嫁娶.纳采.订盟.祭祀.祈福.治病.造车器.修造.动土.移徙.入宅.",\
    "嫁娶.纳采.订盟.会亲友.安机械.结网.冠笄.祭祀.求嗣.进人口.经络.",\
    "祭祀.沐浴.移徙.破土.安葬.扫舍.平治道涂.",\
    "祭祀.祈福.斋醮.求嗣.安机械.纳畜.移徙.入宅.安机械.塑绘.开光.起基.竖柱.上梁.作灶.安门.安香.出火.造屋.启攒.安葬.",\
    "嫁娶.纳采.订盟.斋醮.开光.祭祀.祈福.求医.治病.会亲友.动土.解除.捕捉.纳畜.牧养.入殓.破土.安葬.",\
    "祭祀.沐浴.解除.破屋.坏垣.余事勿取.",\
    "沐浴.扫舍.余事勿取.",\
    "开市.交易.立券.安机械.会亲友.开光.求医.治病.造屋.起基.修造.动土.定磉.竖柱.上梁.安门.作灶.放水.作厕.开池.栽种.牧养.造畜椆栖.破土.安葬.立碑.",\
    "栽种.捕捉.畋猎.余事勿取.",\
    "嫁娶.祭祀.祈福.求嗣.斋醮.订盟.纳采.解除.出行.动土.破土.习艺.针灸.理发.会亲友.起基.修造.动土.竖柱.定磉.安床.拆卸.纳畜.牧养.放水.破土.除服.成服.修坟.立碑.",\
    "余事勿取.",\
    "塞穴.断蚁.结网.余事勿取.",\
    "开光.出行.纳采.嫁娶.伐木.架马.出火.拆卸.移徙.入宅.造庙.造桥.造船.造畜椆栖.开市.入殓.除服.成服.移柩.安葬.",\
    "进人口.牧养.置产.塞穴.结网.余事勿取.",\
    "开光.出行.嫁娶.",\
    "嫁娶.纳采.出行.祭祀.祈福.开市.动土.移徙.入宅.破土.安葬.",\
    "嫁娶.纳采.求医.治病.修造.动土.移徙.入宅.破土.安葬.",\
    "祭祀.破屋.坏垣.余事勿取.",\
    "嫁娶.纳采.祭祀.祈福.出行.动土.上梁.移徙.入宅.破土.安葬.",\
    "纳采.祭祀.祈福.开市.求医.治病.动土.纳畜.",\
    "嫁娶.纳采.出行.移徙.入宅.",\
    "订盟.纳采.祭祀.动土.破土.交易.立券.",\
    "嫁娶.裁衣.祭祀.出行.安床.作灶.移徙.入宅.破土.安葬.",\
    "塞穴.结网.余事勿取.",\
    "",\
    "订盟.纳采.会亲友.安床.作灶.造畜椆栖.",\
    "沐浴.平治道涂.扫舍.入殓.移柩.破土.启攒.安葬.余事勿取.",\
    "嫁娶.祭祀.祈福.求嗣.开光.出行.出火.拆卸.动土.上梁.进人口.入宅.移徙.安床.安门.开市.交易.立券.挂匾.栽种.破土.安葬.",\
    "祭祀.开光.出行.解除.塑绘.裁衣.入殓.移柩.破土.启攒.安葬.除服.成服.",\
    "祭祀.解除.破屋.坏垣.余事勿取.",\
    "祭祀.沐浴.破屋.坏垣.余事勿取.",\
    "嫁娶.安机械.交易.出行.祭祀.祈福.求嗣.斋醮.塑绘.开光.合帐.裁衣.放水.开池.掘井.",\
    "纳采.冠笄.求医.治病.开市.立券.修造.动土.安机械.破土.安葬.",\
    "祭祀.作灶.余事勿取.",\
    "祭祀.祈福.求嗣.斋醮.安香.解除.移徙.入宅.会亲友.求医.治病.动土.破土.开生坟.合寿木.",\
    "嫁娶.冠笄.修造.动土.作灶.移徙.入宅.补垣.塞穴.纳畜.牧养.架马.修造.动土.起基.定磉.开池.造船.",\
    "祭祀.交易.纳财.",\
    "嫁娶.订盟.纳采.冠笄.会亲友.安机械.造车器.祭祀.出行.纳财.入宅.安香.出火.入学.塑绘.开光.拆卸.起基.修造.动土.牧养.栽种.安门.作厕.",\
    "开光.求嗣.出行.冠笄.嫁娶.伐木.架马.开柱眼.修造.移徙.入宅.开市.交易.立券.出行.安香.出火.挂匾.起基.修造.开生坟.合寿木.入殓.除服.成服.移柩.安葬.",\
    "祭祀.沐浴.理发.嫁娶.作灶.整手足甲.扫舍.修饰垣墙.平治道涂.",\
    "安机械.移徙.入宅.出行.祭祀.祈福.斋醮.纳采.订盟.安香.出火.解除.会亲友.修造.动土.拆卸.起基.定磉.移徙.入宅.造屋.安床.修造.破土.安葬.入殓.立碑.",\
    "祭祀.沐浴.捕捉.结网.畋猎.取渔.余事勿取.",\
    "破屋.坏垣.求医.治病.畋猎.余事勿取.",\
    "嫁娶.出行.安机械.祭祀.塑绘.开光.治病.经络.安床.结网.塞穴.破土.入殓.",\
    "订盟.纳采.会亲友.进人口.雕刻.拆卸.修造.动土.起基.开市.栽种.纳畜.牧养.入殓.除服.成服.移柩.破土.安葬.",\
    "祭祀.捕捉.取渔.修饰垣墙.余事勿取.",\
    "嫁娶.纳采.祭祀.祈福.求医.治病.出行.动土.移徙.入宅.",\
    "裁衣.作灶.移徙.入宅.纳畜.",\
    "祭祀.入殓.移柩.启攒.安葬.",\
    "订盟.纳采.出行.祈福.斋醮.安床.会亲友.",\
    "嫁娶.纳采.出行.求医.治病.开市.移徙.入宅.启攒.安葬.",\
    "嫁娶.祭祀.沐浴.扫舍.修饰垣墙.",\
    "嫁娶.订盟.纳采.出行.开市.祭祀.祈福.动土.移徙.入宅.破土.安葬.",\
    "订盟.纳采.出行.祭祀.祈福.修造.动土.移徙.入宅.",\
    "诸事不宜.",\
    "嫁娶.订盟.纳采.祭祀.祈福.入殓.破土.安葬.",\
    "开光.求医.治病.动土.上梁.入殓.破土.安葬.",\
    "祭祀.栽种.余事勿取.",\
    "嫁娶.开光.祭祀.祈福.求嗣.出行.解除.伐木.入宅.移徙.安床.出火.拆卸.修造.上梁.栽种.移柩.",\
    "求嗣.嫁娶.纳采.合帐.裁衣.冠笄.伐木.作梁.修造.动土.起基.竖柱.上梁.安门.作灶.筑堤.造畜椆栖.",\
    "祭祀.解除.余事勿取.",\
    "嫁娶.祭祀.祈福.出火.开光.求嗣.出行.拆卸.开市.交易.立券.挂匾.入宅.移徙.安床.栽种.动土.",\
    "祭祀.沐浴.塑绘.开光.入学.解除.扫舍.治病.开池.牧养.",\
    "纳财.开市.交易.立券.出行.祭祀.祈福.求嗣.开光.解除.扫舍.起基.竖柱.安床.移徙.开仓.出货财.补垣.塞穴.栽种.纳畜.牧养.",\
    "祭祀.修饰垣墙.平治道涂.",\
    "订盟.纳采.祭祀.祈福.开光.安香.出火.立券.安机械.移徙.入宅.竖柱.上梁.会亲友.安床.拆卸.挂匾.牧养.教牛马.",\
    "沐浴.理发.捕捉.入殓.移柩.破土.启攒.安葬.",\
    "求医.治病.破屋.坏垣.余事勿取.",\
    "纳采.订盟.嫁娶.移徙.入宅.出行.开市.交易.立券.纳财.会亲友.安香.出火.拆卸.造屋.起基.安床.作灶.挂匾.安葬.破土.启攒.立碑.入殓.移柩.",\
    "祭祀.祈福.斋醮.出行.纳采.订盟.安机械.出火.拆卸.修造.动土.起基.移徙.入宅.造庙.入殓.除服.成服.移柩.破土.安葬.谢土.",\
    "祭祀.进人口.纳财.纳畜.牧养.捕捉.余事勿取.",\
    "祭祀.塑绘.开光.求医.治病.嫁娶.会亲友.放水.掘井.牧养.纳畜.开渠.安碓硙.",\
    "祭祀.塞穴.结网.畋猎.余事勿取.",\
    "开市.纳财.祭祀.塑绘.安机械.冠笄.会亲友.裁衣.开仓.经络.纳畜.造畜椆栖.教牛马.牧养.",\
    "移徙.入宅.治病.会亲友.祭祀.祈福.斋醮.安香.移徙.嫁娶.造屋.起基.",\
    "塑绘.出行.冠笄.嫁娶.进人口.裁衣.纳婿.造畜椆栖.交易.立券.牧养.开生坟.入殓.除服.成服.移柩.安葬.启攒.",\
    "祭祀.冠笄.嫁娶.捕捉.结网.畋猎.取渔.余事勿取.",\
    "沐浴.扫舍.余事勿取.",\
    "纳采.祭祀.祈福.解除.动土.破土.安葬.",\
    "祭祀.破屋.坏垣.余事勿取.",\
    "嫁娶.纳采.开市.出行.动土.上梁.移徙.入宅.破土.安葬.",\
    "嫁娶.纳采.开市.出行.动土.上梁.移徙.入宅.破土.安葬.",\
    "祭祀.作灶.纳财.捕捉.",\
    "嫁娶.开市.立券.祭祀.祈福.动土.移徙.入宅.",\
    "补垣.塞穴.结网.入殓.除服.成服.移柩.安葬.启攒.余事勿取.",\
    "嫁娶.纳采.出行.祭祀.祈福.解除.移徙.入宅.",\
    "嫁娶.祭祀.祈福.斋醮.治病.破土.安葬.",\
    "嫁娶.出行.开市.安床.入殓.启攒.安葬.",\
    "嫁娶.祭祀.裁衣.结网.冠笄.沐浴.",\
    "入宅.移徙.安床.开光.祈福.求嗣.进人口.开市.交易.立券.出火.拆卸.修造.动土.",\
    "祭祀.解除.沐浴.整手足甲.入殓.移柩.破土.启攒.安葬.",\
    "破屋.坏垣.余事勿取.",\
    "嫁娶.开光.出行.理发.作梁.出火.拆卸.修造.开市.交易.立券.挂匾.动土.入宅.移徙.安床.栽种.",\
    "订盟.纳采.祭祀.祈福.安香.出火.开市.立券.入宅.挂匾.造桥.启攒.安葬.",\
    "嫁娶.祭祀.祈福.斋醮.普渡.移徙.入宅.动土.治病.开市.交易.立券.开光.修造.造车器.安香.安床.捕捉.畋猎.结网.",\
    "嫁娶.订盟.纳采.作灶.冠笄.裁衣.会亲友.纳畜.牧养.安机械.开市.立券.纳财.安床.",\
    "嫁娶.订盟.纳采.祭祀.斋醮.普渡.解除.出行.会亲友.开市.纳财.修造.动土.竖柱.上梁.开光.开仓.出货财.纳畜.牧养.开池.破土.启攒.",\
    "嫁娶.普渡.祭祀.祈福.补垣.塞穴.断蚁.筑堤.入殓.除服.成服.安葬.",\
    "嫁娶.冠笄.祭祀.沐浴.普渡.出行.纳财.扫舍.纳畜.赴任.",\
    "祭祀.沐浴.理发.整手足甲.冠笄.解除.入殓.移柩.破土.启攒.安葬.",\
    "塑绘.冠笄.嫁娶.会亲友.进人口.经络.裁衣.栽种.纳畜.牧养.补垣.塞穴.捕捉.",\
    "出行.沐浴.订盟.纳采.裁衣.竖柱.上梁.移徙.纳畜.牧养.",\
    "纳采.订盟.嫁娶.祭祀.祈福.普渡.开光.安香.出火.移徙.入宅.竖柱.修造.动土.竖柱.上梁.起基.造屋.安门.造庙.造桥.破土.启攒.安葬.",\
    "祭祀.捕捉.畋猎.纳畜.牧养.入殓.除服.成服.移柩.破土.安葬.启攒.",\
    "破屋.坏垣.治病.余事勿取.",\
    "祈福.斋醮.出行.冠笄.嫁娶.雕刻.开柱眼.入宅.造桥.开市.交易.立券.纳财.入殓.除服.成服.移柩.破土.安葬.启攒.",\
    "祈福.求嗣.解除.订盟.纳采.动土.起基.放水.造仓.开市.纳畜.牧养.开生坟.入殓.除服.成服.移柩.破土.安葬.",\
    "塑绘.开光.解除.订盟.纳采.嫁娶.出火.修造.动土.移徙.入宅.拆卸.起基.安门.分居.开市.交易.立券.纳财.纳畜.牧养.",\
    "祈福.出行.订盟.纳采.嫁娶.裁衣.动土.安床.放水.开市.掘井.交易.立券.栽种.开渠.除服.成服.移柩.破土.",\
    "嫁娶.祭祀.祈福.斋醮.作灶.移徙.入宅.",\
    "嫁娶.出行.纳畜.祭祀.入殓.启攒.安葬.",\
    "订盟.纳采.祭祀.祈福.修造.动土.上梁.破土.安葬.",\
    "订盟.纳采.出行.会亲友.修造.上梁.移徙.入宅.",\
    "沐浴.修饰垣墙.平治道涂.余事勿取.",\
    "嫁娶.祭祀.祈福.斋醮.动土.移徙.入宅.",\
    "捕捉.结网.入殓.破土.安葬.",\
    "沐浴.治病.破屋.坏垣.余事勿取.",\
    "嫁娶.订盟.纳采.出行.开市.祭祀.祈福.移徙.入宅.启攒.安葬.",\
    "嫁娶.订盟.纳采.祭祀.祈福.求医.治病.动土.移徙.入宅.破土.安葬.",\
    "订盟.纳采.祭祀.祈福.安机械.作灶.纳畜.",\
    "嫁娶.祭祀.祈福.求嗣.出行.动土.安床.掘井.破土.启攒.",\
    "嫁娶.祭祀.祈福.求嗣.出行.出火.拆卸.修造.移徙.动土.安床.入殓.破土.安葬.启攒.",\
    "祭祀.祈福.求嗣.开光.出行.解除.上梁.造屋.移徙.安门.纳财.牧养.纳畜.安葬.启攒.入殓.",\
    "祭祀.解除.沐浴.理发.整手足甲.入殓.移柩.破土.安葬.扫舍.",\
    "嫁娶.订盟.纳采.祭祀.祈福.出行.修造.动土.移徙.入宅.",\
    "出行.开市.交易.立券.安机械.出火.上梁.移徙.",\
    "祭祀.沐浴.修饰垣墙.平治道涂.余事勿取.",\
    "嫁娶.造车器.安机械.祭祀.祈福.开光.安香.出火.出行.开市.立券.修造.动土.移徙.入宅.破土.安葬.",\
    "沐浴.捕捉.入殓.除服.成服.破土.启攒.安葬.",\
    "余事勿取.",\
    "订盟.纳采.祭祀.祈福.安香.出火.修造.动土.上梁.安门.起基.竖柱.上梁.定磉.开池.移徙.入宅.立券.破土.",\
    "开光.求嗣.雕刻.嫁娶.订盟.纳采.出火.拆卸.修造.动土.起基.上梁.放水.移徙.入宅.造仓.造船.开市.开池.纳畜.牧养.挂匾.",\
    "祭祀.嫁娶.捕捉.",\
    "祭祀.普渡.解除.会亲友.捕捉.畋猎.启攒.除服.成服.移柩.",\
    "祭祀.出行.解除.冠笄.嫁娶.伐木.架马.开柱眼.修造.动土.移徙.入宅.开生坟.合寿木.入殓.移柩.破土.安葬.修坟.",\
    "祭祀.祈福.求嗣.出行.沐浴.交易.扫舍.教牛马.",\
    "出行.解除.纳采.冠笄.雕刻.修造.动土.起基.上梁.合脊.安床.移徙.入宅.开市.栽种.作厕.",\
    "祭祀.沐浴.解除.理发.冠笄.安机械.作灶.造仓.开市.开池.作厕.补垣.塞穴.断蚁.结网.",\
    "祭祀.沐浴.修饰垣墙.平治道涂.",\
    "祭祀.会亲友.纳采.嫁娶.开光.塑绘.斋醮.安香.开市.立券.除服.成服.入殓.移柩.安葬.赴任.进人口.出行.裁衣.修造.动土.上梁.经络.交易.",\
    "祭祀.冠笄.会亲友.拆卸.起基.除服.成服.移柩.启攒.安葬.沐浴.捕捉.开光.塑绘.",\
    "祭祀.沐浴.破屋.坏垣.余事勿取.",\
    "祭祀.塑绘.开光.出行.解除.订盟.嫁娶.拆卸.起基.安床.入宅.开市.入殓.除服.成服.移柩.破土.谢土.挂匾.开柱眼.交易.",\
    "祭祀.赴任.动土.上梁.开光.塑绘.冠笄.拆卸.起基.安床.开市.立券.赴任.经络.",\
    "祭祀.裁衣.冠笄.嫁娶.纳婿.会亲友.除服.成服.移柩.捕捉.进人口.入殓.",\
    "祭祀.诸事不宜.",\
    "祭祀.裁衣.冠笄.嫁娶.安机械.拆卸.动土.起基.移徙.入宅.入殓.启攒.安葬.造仓.经络.",\
    "祭祀.出行.成服.除服.沐浴.入殓.",\
    "祭祀.沐浴.赴任.出行.余事勿取.",\
    "诸事不宜.",\
    "沐浴.入殓.移柩.除服.成服.破土.平治道涂.",\
    "嫁娶.祭祀.祈福.求嗣.沐浴.出火.出行.拆卸.修造.动土.进人口.开市.交易.立券.入宅.移徙.安床.栽种.纳畜.入殓.安葬.启攒.除服.成服.",\
    "开光.解除.起基.动土.拆卸.上梁.立碑.修坟.安葬.破土.启攒.移柩.",\
    "破屋.坏垣.余事勿取.",\
    "嫁娶.祈福.求嗣.出行.出火.拆卸.修造.动土.上梁.开光.进人口.开市.交易.立券.挂匾.安床.入宅.移徙.栽种.伐木.入殓.破土.除服.成服.",\
    "订盟.纳采.会亲友.交易.立券.纳财.栽种.纳畜.牧养.",\
    "造车器.嫁娶.订盟.纳采.会亲友.祭祀.出行.开市.立券.移徙.入宅.破土.安葬.",\
    "祭祀.作灶.纳财.捕捉.畋猎.余事勿取.",\
    "嫁娶.订盟.纳采.祭祀.祈福.出行.求医.治病.出火.移徙.入宅.",\
    "冠笄.祭祀.沐浴.作灶.理发.整手足甲.扫舍.补垣.塞穴.入殓.破土.启攒.",\
    "纳采.订盟.嫁娶.祭祀.祈福.求嗣.置产.求医.治病.开市.交易.立券.会亲友.移徙.竖柱.上梁.造屋.合脊.安门.放水.捕捉.纳畜.",\
    "出行.造车器.造畜椆栖.解除.冠笄.裁衣.作梁.雕刻.会亲友.移徙.入宅.安机械.造畜椆栖.开市.扫舍.",\
    "沐浴.理发.冠笄.安床.开市.立券.会亲友.交易.纳财.结网.教牛马.",\
    "祭祀.造畜椆栖.修饰垣墙.平治道涂.余事勿取.",\
    "捕捉.结网.入殓.除服.成服.移柩.破土.安葬.启攒.立碑.",\
    "祭祀.祈福.求嗣.斋醮.造庙.出火.安机械.会亲友.开市.交易.立券.纳财.习艺.经络.求医.治病.开池.作厕.畋猎.结网.栽种.牧养.安葬.破土.启攒.",\
    "破屋.坏垣.余事勿取.",\
    "会亲友.嫁娶.订盟.纳采.纳婿.拆卸.修造.动土.起基.竖柱.上梁.安床.会亲友.纳财.",\
    "祭祀.塑绘.开光.祈福.斋醮.出行.订盟.纳采.裁衣.嫁娶.拆卸.修造.安床.入宅.安香.入殓.启攒.安葬.谢土.赴任.会亲友.进人口.出行.移徙.上梁.经络.开市.交易.立券.纳财.",\
    "祭祀.作灶.入殓.除服.成服.畋猎.",\
    "祭祀.祈福.斋醮.沐浴.竖柱.订盟.纳采.嫁娶.拆卸.入宅.移柩.启攒.谢土.赴任.出火.纳畜.",\
    "嫁娶.祭祀.安机械.入殓.破土.安葬.",\
    "作灶.造畜椆栖.",\
    "沐浴.理发.入学.习艺.进人口.",\
    "开光.针灸.会亲友.启攒.安葬.",\
    "祭祀.结网.造畜椆栖.余事勿取.",\
    "入殓.除服.成服.移柩.破土.启攒.安葬.",\
    "嫁娶.订盟.纳采.出行.祭祀.祈福.动土.移徙.入宅.破土.安葬.",\
    "祭祀.解除.破屋.坏垣.余事勿取.",\
    "订盟.纳采.会亲友.安机械.纳财.牧养.",\
    "嫁娶.订盟.纳采.出行.开市.祭祀.祈福.动土.移徙.入宅.破土.安葬.",\
    "祭祀.塞穴.余事勿取.",\
    "祭祀.祈福.求嗣.开光.开市.出行.解除.动土.起基.置产.栽种.",\
    "祭祀.解除.裁衣.理发.安床.作灶.造畜椆栖.放水.筑堤.补垣.塞穴.整手足甲.扫舍.",\
    "祭祀.出行.裁衣.冠笄.会亲友.造畜椆栖.嫁娶.竖柱.上梁.移徙.纳财.纳畜.",\
    "祭祀.沐浴.出行.余事勿取.",\
    "嫁娶.造车器.出行.会亲友.移徙.入宅.修造.动土.雕刻.开光.安香.出火.理发.会亲友.造屋.合脊.起基.归岫.安门.拆卸.扫舍.栽种.造畜椆栖.",\
    "塑绘.会亲友.安机械.塞穴.结网.裁衣.经络.",\
    "纳采.移徙.纳财.开市.交易.立券.纳财.入宅.修造.动土.竖柱.起基.定磉.造庙.安香.出火.修饰垣墙.平治道涂.会亲友.出行.开池.作厕.",\
    "订盟.纳采.纳财.开市.立券.祭祀.祈福.移徙.入宅.出行.造屋.起基.修造.动土.竖柱.上梁.安门.安香.出火.教牛马.会亲友.破土.",\
    "嫁娶.订盟.纳采.祭祀.祈福.塑绘.开光.移徙.安床.伐木.作梁.捕捉.畋猎.结网.求医.治病.解除.安葬.除服.成服.移柩.入殓.立碑.谢土.",\
    "破屋.坏垣.祭祀.余事勿取.",\
    "嫁娶.纳采.订盟.祭祀.冠笄.裁衣.伐木.作梁.架马.定磉.开柱眼.作灶.移徙.安床.畋猎.结网.开池.作厕.除服.成服.启攒.入殓.移柩.安葬.",\
    "纳采.订盟.祭祀.祈福.求嗣.斋醮.开光.会亲友.解除.入学.纳财.交易.立券.经络.起基.动土.定磉.开池.栽种.纳畜.牧养.破土.入殓.立碑.安葬.",\
    "捕捉.畋猎.会亲友.解除.入殓.除服.成服.移柩.余事勿取.",\
    "祭祀.祈福.求嗣.斋醮.沐浴.冠笄.出行.理发.拆卸.解除.起基.动土.定磉.安碓硙.开池.掘井.扫舍.除服.成服.移柩.启攒.立碑.谢土.",\
    "嫁娶.冠笄.安床.纳采.会亲友.塞穴.捕捉.置产.造畜椆栖.",\
    "祭祀.沐浴.余事勿取.",\
    "祭祀.会亲友.嫁娶.沐浴.修造.动土.祈福.开光.塑绘.出行.订盟.纳采.裁衣.入殓.除服.成服.移柩.启攒.赴任.竖柱.上梁.纳财.扫舍.栽种.纳畜.伐木.",\
    "理发.会亲友.补垣.塞穴.结网.",\
    "祭祀.祈福.订盟.纳采.裁衣.拆卸.修造.动土.起基.安床.移徙.入宅.安香.除服.成服.入殓.移柩.安葬.谢土.赴任.会亲友.进人口.出行.竖柱.上梁.经络.开市.交易.立券.纳财.开仓.",\
    "祭祀.祈福.订盟.纳采.裁衣.拆卸.修造.动土.起基.安床.移徙.入宅.安香.入殓.移柩.安葬.谢土.赴任.进人口.会亲友.",\
    "祭祀.塑绘.开光.订盟.纳采.嫁娶.安床.进人口.入殓.除服.成服.移柩.启攒.安葬.立碑.",\
    "祭祀.解除.破屋.坏垣.余事勿取.",\
    "祭祀.解除.祈福.开光.塑绘.斋醮.订盟.纳采.裁衣.冠笄.拆卸.修造.动土.入殓.除服.成服.移柩.启攒.安床.赴任.出行.移徙.竖柱.上梁.伐木.栽种.破土.安葬.纳畜.",\
    "祭祀.祈福.订盟.纳采.裁衣.合帐.冠笄.安机械.安床.造畜椆栖.入殓.移柩.启攒.安葬.谢土.除服.成服.会亲友.竖柱.上梁.经络.开市.交易.立券.纳财.纳畜.筑堤.",\
    "沐浴.扫舍.余事勿取.",\
    "诸事不宜.",\
    "祈福.斋醮.出行.订盟.纳采.入殓.移柩.破土.安葬.立碑.结网.",\
    "祭祀.沐浴.出行.冠笄.进人口.余事勿取.",\
    "祭祀.祈福.斋醮.塑绘.开光.订盟.纳采.裁衣.冠笄.嫁娶.拆卸.入宅.安香.入殓.移柩.理发.安葬.修坟.谢土.赴任.移徙.沐浴.治病.破土.启攒.整手足甲.入学.作梁.",\
    "诸事不宜.",\
    "开市.交易.立券.挂匾.纳财.开光.出行.入宅.移徙.安床.纳畜.入殓.移柩.安葬.",\
    "嫁娶.祭祀.祈福.求嗣.开光.出行.解除.出火.出行.拆卸.进人口.入宅.移徙.安床.栽种.动土.修造.纳畜.入殓.安葬.立碑.除服.成服.",\
    "开光.解除.拆卸.修造.动土.安床.纳畜.安葬.启攒.入殓.",\
    "嫁娶.订盟.纳采.祭祀.祈福.求嗣.斋醮.安香.出火.修造.起基.造屋.合脊.安门.安碓硙.动土.上梁.移徙.入宅.",\
    "祭祀.沐浴.破屋.坏垣.余事勿取.",\
    "冠笄.纳财.掘井.开池.出火.安床.交易.立券.畋猎.结网.理发.放水.",\
    "纳采.订盟.移徙.入宅.出行.安机械.会亲友.祭祀.祈福.斋醮.开光.安香.出火.解除.求医.针灸.治病.造屋.起基.修造.安门.造船.纳畜.牧养.移柩.入殓.启攒.谢土.修坟.立碑.",\
    "祭祀.沐浴.作灶.纳财.捕捉.畋猎.安床.扫舍.",\
    "祈福.斋醮.纳采.订盟.解除.架马.开柱眼.修造.动土.起基.上梁.归岫.造屋.合脊.掘井.除服.成服.破土.栽种.",\
    "纳采.订盟.祭祀.沐浴.冠笄.合帐.裁衣.修造.动土.拆卸.移徙.入宅.安门.开仓.筑堤.作厕.栽种.纳畜.补垣.塞穴.",\
    "合帐.裁衣.教牛马.余事勿取.",\
    "纳采.订盟.嫁娶.祭祀.祈福.安香.出火.出行.会亲友.经络.求医.治病.解除.拆卸.起基.修造.动土.定磉.扫舍.栽种.牧养.造畜椆栖.",\
    "纳财.开市.交易.立券.会亲友.进人口.经络.祭祀.祈福.安香.出火.求医.治病.修造.动土.拆卸.扫舍.安床.栽种.牧养.开生坟.合寿木.入殓.安葬.启攒.",\
    "祭祀.入殓.移柩.余事勿取.",\
    "塑绘.开光.订盟.纳采.裁衣.冠笄.拆卸.修造.安床.入宅.出火.安葬.谢土.赴任.",\
    "祭祀.塑绘.开光.裁衣.冠笄.嫁娶.纳采.拆卸.修造.动土.竖柱.上梁.安床.移徙.入宅.安香.结网.捕捉.畋猎.伐木.进人口.放水.",\
    "祭祀.求医.破屋.坏垣.余事勿取.",\
    "祭祀.祈福.斋醮.出行.冠笄.安机械.移徙.入宅.安香.安床.除服.成服.移柩.启攒.",\
    "塑绘.斋醮.出行.拆卸.解除.修造.移徙.造船.入殓.除服.成服.移柩.启攒.修坟.立碑.谢土.",\
    "祭祀.沐浴.安床.纳财.畋猎.捕捉.",\
    "订盟.纳采.祭祀.祈福.修造.动土.上梁.破土.",\
    "出行.沐浴.理发.补垣.塞穴.",\
    "教牛马.余事勿取.",\
    "嫁娶.出行.求医.治病.祭祀.祈福.上梁.纳畜.",\
    "开市.立券.开光.解除.安机械.上梁.启攒.安葬.",\
    "平治道涂.余事勿取.",\
    "求嗣.斋醮.塑绘.订盟.纳采.出火.拆卸.修造.动土.造桥.安机械.栽种.纳畜.牧养.入殓.除服.成服.移柩.破土.安葬.",\
    "嫁娶.订盟.纳采.祭祀.祈福.修造.动土.移徙.入宅.",\
    };
    const char *lunderji[] = {
    "入宅.动土.破土.余事勿取.",\
    "开市.安葬.",\
    "嫁娶.祈福.",\
    "诸事不宜.",\
    "开市.嫁娶.",\
    "开市.出行.安葬.行丧.",\
    "纳采.订盟.嫁娶.上梁.开市.斋醮.造屋.安门.",\
    "斋醮.嫁娶.开市.",\
    "祈福.嫁娶.安床.入宅.造船.",\
    "造庙.入宅.修造.安葬.行丧.嫁娶.",\
    "开市.安床.安葬.修坟.",\
    "造庙.嫁娶.出行.动土.安葬.行丧.",\
    "开光.嫁娶.开市.动土.破土.",\
    "破土.动土.安葬.",\
    "探病.祭祀.出行.上梁.造屋.谢土.安葬.",\
    "入宅.开光.开市.动土.",\
    "移徙.入宅.造庙.作灶.治病.安葬.",\
    "嫁娶.安葬.",\
    "造屋.开市.动土.破土.",\
    "嫁娶.开市.安葬.",\
    "祈福.嫁娶.造庙.安床.谢土.",\
    "开市.伐木.嫁娶.作梁.",\
    "开市.安床.",\
    "嫁娶.入殓.安葬.出行.",\
    "开仓.嫁娶.移徙.入宅.",\
    "诸事不宜.",\
    "作灶.祭祀.上梁.出行.",\
    "作灶.掘井.谢土.入宅.",\
    "嫁娶.移徙.入宅.开光.",\
    "出行.治病.安葬.开市.",\
    "造屋.开市.作灶.入宅.",\
    "诸事不宜.",\
    "入宅.安床.",\
    "破土.伐木.",\
    "嫁娶.安门.移徙.入宅.安葬.",\
    "嫁娶.开市.安葬.破土.",\
    "移徙.入宅.出火.安门.安葬.",\
    "嫁娶.动土.开光.造屋.破土.",\
    "祭祀.移徙.入宅.动土.破土.",\
    "开光.嫁娶.作灶.掘井.纳畜.",\
    "开市.动土.安葬.破土.",\
    "嫁娶.祈福.掘井.安葬.",\
    "置产.造屋.合脊.开光.探病.安门.作灶.",\
    "开光.开市.入宅.动土.造屋.",\
    "入宅.开市.安葬.",\
    "安床.栽种.治病.作灶.",\
    "",\
    "伐木.作灶.安葬.取渔.入宅.",\
    "安葬.上梁.入宅.作灶.",\
    "出行.嫁娶.入宅.动土.",\
    "祭祀.祈福.移徙.嫁娶.入宅.",\
    "掘井.",\
    "斋醮.作灶.安床.安葬.",\
    "",\
    "造屋.栽种.安葬.作灶.",\
    "栽种.动土.开市.作灶.",\
    "诸事不宜.",\
    "作灶.安床.",\
    "嫁娶.上梁.入宅.作灶.",\
    "安葬.作灶.伐木.作梁.",\
    "入殓.安葬.作灶.入宅.",\
    "开光.掘井.针灸.出行.嫁娶.入宅.移徙.作灶.动土.",\
    "嫁娶.出行.动土.开渠.入宅.祭祀.掘井.",\
    "掘井.词讼.",\
    "嫁娶.安葬.行丧.安门.",\
    "",\
    "开市.造屋.治病.作灶.",\
    "纳畜.理发.合寿木.",\
    "纳采.开光.安床.嫁娶.开市.",\
    "嫁娶.移徙.开市.入宅.",\
    "塑绘.开光.造桥.除服.成服.",\
    "祈福.嫁娶.安葬.破土.",\
    "纳财.开市.安葬.破土.",\
    "纳采.动土.开市.交易.安门.",\
    "祭祀.嫁娶.出行.上梁.掘井.",\
    "栽种.动土.安葬.掘井.修坟.探病.",\
    "诸事不宜.",\
    "作灶.出行.入宅.安葬.",\
    "动土.作灶.入宅.开光.安床.",\
    "赴任.",\
    "安床.嫁娶.作灶.入宅.",\
    "诸事不宜.",\
    "作梁.造庙.",\
    "造庙.嫁娶.伐木.安葬.",\
    "嫁娶.入宅.移徙.作灶.安葬.",\
    "栽种.安葬.",\
    "作灶.开光.嫁娶.开市.入宅.",\
    "作灶.掘井.动土.栽种.",\
    "栽种.作灶.安葬.嫁娶.",\
    "出行.嫁娶.入宅.安葬.",\
    "造屋.作灶.治病.探病.",\
    "栽种.伐木.",\
    "安床.开市.立券.作灶.",\
    "",\
    "祈福.斋醮.开市.安葬.",\
    "祭祀.嫁娶.入宅.作灶.安葬.",\
    "出火.嫁娶.开市.",\
    "嫁娶.安葬.",\
    "祈福.动土.破土.安葬.入殓.",\
    "开光.嫁娶.掘井.安葬.安门.探病.",\
    "动土.破土.行丧.开光.作梁.安葬.探病.",\
    "开市.嫁娶.移徙.入宅.掘井.安葬.",\
    "祈福.开市.修造.动土.破土.谢土.",\
    "嫁娶.安葬.动土.安床.治病.",\
    "入宅.动土.开仓.出货财.",\
    "开市.动土.掘井.开池.",\
    "嫁娶.开市.交易.入宅.安葬.",\
    "诸事不宜.",\
    "出火.入宅.安葬.伐木.",\
    "造庙.嫁娶.安床.余事勿取.",\
    "入殓.安葬.入宅.安床.",\
    "掘井.安门.嫁娶.纳采.",\
    "掘井.动土.破土.安葬.开光.",\
    "嫁娶.开市.安葬.",\
    "嫁娶.动土.破土.",\
    "嫁娶.安葬.",\
    "嫁娶.移徙.入宅.",\
    "开市.动土.破土.",\
    "诸事不宜.",\
    "嫁娶.入宅.安葬.",\
    "诸事不宜.",\
    "诸事不宜.",\
    "祈福.入殓.祭祀.作灶.安葬.探病.",\
    "词讼.开光.开市.",\
    "诸事不宜.",\
    "嫁娶.安葬.",\
    "开市.出行.安床.作灶.安葬.",\
    "开市.作灶.动土.行丧.安葬.",\
    "祈福.嫁娶.入宅.安床.作灶.",\
    "动土.破土.嫁娶.嫁娶.",\
    "移徙.入宅.造屋.架马.",\
    "行丧.安葬.",\
    "斋醮.开市.嫁娶.作灶.",\
    "嫁娶.出火.移徙.入宅.",\
    "开市.动土.祭祀.斋醮.安葬.探病.",\
    "开市.入宅.探病.出火.造屋.",\
    "余事勿取.",\
    "破土.安葬.",\
    "",\
    "诸事不宜.",\
    "会亲友.进人口.修造.动土.起基.移徙.开市.纳畜.入殓.除服.成服.移柩.破土.安葬.修坟.立碑.会亲友.",\
    "安门.",\
    "开市.开光.",\
    "诸事不宜.",\
    "祈福.斋醮.",\
    "嫁娶.安葬.",\
    "动土.破土.安葬.",\
    "嫁娶.安葬.",\
    "赴任.捕捉.",\
    "诸事不宜.",\
    "",\
    "开市.安葬.",\
    "诸事不宜.",\
    "",\
    "嫁娶.上梁.修造.拆卸.架马.入宅.伐木.动土.出火.开柱眼.",\
    "诸事不宜.",\
    "入宅.嫁娶.移徙.",\
    "作灶.理发.造桥.行丧.安葬.",\
    "斋醮.祭祀.移徙.入宅.上梁.嫁娶.",\
    "开市.安葬.破土.修坟.掘井.",\
    "合帐.上梁.经络.安葬.入殓.",\
    "祈福.开光.掘井.开市.安葬.",\
    "斋醮.开渠.上梁.动土.破土.",\
    "行丧.伐木.作梁.作灶.",\
    "安床.出货财.作灶.动土.破土.",\
    "斋醮.出行.治病.合寿木.",\
    "开市.伐木.作梁.作灶.",\
    "开市.交易.嫁娶.安葬.行丧.",\
    "嫁娶.入宅.",\
    "开市.安门.掘井.作灶.",\
    "",\
    "诸事不宜.",\
    "开市.安门.",\
    "嫁娶.安葬.",\
    "上梁.动土.破土.",\
    "移徙.入宅.安葬.",\
    "动土.破土.",\
    "行丧.安葬.",\
    "作灶.",\
    "开市.安葬.",\
    "诸事不宜.",\
    "开光.开市.",\
    "嫁娶.开光.",\
    "诸事不宜.",\
    "安葬.开市.交易.立券.",\
    "安葬.出行.祈福.栽种.",\
    "诸事不宜.",\
    "安葬.行丧.伐木.作梁.",\
    "嫁娶.出行.纳采.入宅.作灶.",\
    "斋醮.入宅.安门.安葬.破土.行丧.",\
    "开市.动土.破土.嫁娶.修造.安葬.",\
    "嫁娶.安葬.行丧.破土.修坟.",\
    "出火.嫁娶.入宅.作灶.破土.上梁.动土.",\
    "嫁娶.出行.",\
    "祈福.上梁.开仓.掘井.牧养.",\
    "嫁娶.开市.栽种.合寿木.",\
    "开市.入宅.安床.动土.安葬.",\
    "造屋.入宅.作灶.入学.安葬.行丧.",\
    "移徙.开市.入宅.嫁娶.开光.安门.",\
    "动土.破土.安葬.治病.",\
    "开市.斋醮.安床.出行.经络.",\
    "",\
    "余事勿取.",\
    "诸事不宜.",\
    "嫁娶.移徙.入宅.",\
    "诸事不宜.",\
    "祭祀.祈福.",\
    "赴任.",\
    "开市.破土.",\
    "造庙.安葬.",\
    "诸事不宜.",\
    "动土.安葬.",\
    "开市.入宅.",\
    "祈福.动土.破土.",\
    "开仓.出货财.置产.安葬.动土.破土.掘井.栽种.",\
    "嫁娶.破土.置产.栽种.安葬.修坟.行丧.",\
    "嫁娶.入宅.移徙.作灶.开市.交易.安门.栽种.",\
    "诸事不宜.",\
    "伐木.祭祀.纳畜.祭祀.",\
    "动土.破土.嫁娶.掘井.安床.",\
    "纳采.订盟.经络.行丧.安葬.探病.",\
    "掘井.出行.破土.行丧.安葬.",\
    "出火.入宅.造屋.安门.安葬.",\
    "动土.破土.掘井.开光.上梁.词讼.",\
    "开市.动土.破土.安床.开仓.上梁.",\
    "嫁娶.出行.入宅.开市.安门.",\
    "祈福.开市.动土.行丧.安葬.",\
    "嫁娶.安门.动土.安葬.",\
    "开市.立券.纳财.作灶.",\
    "嫁娶.纳采.订盟.开市.入宅.",\
    "行丧.安葬.",\
    "动土.破土.订盟.安床.开池.",\
    "",\
    "",\
    "",\
    "动土.破土.",\
    "作灶.动土.破土.",\
    "嫁娶.开市.",\
    "开市.安葬.",\
    "嫁娶.祈福.余事勿取.",\
    "开市.安葬.",\
    "嫁娶.入宅.",\
    "诸事不宜.",\
    "动土.破土.",\
    "开光.针灸.",\
    "动土.安葬.",\
    "入宅.作梁.安门.伐木.修造.上梁.入殓.造屋.",\
    "造屋.开光.理发.造船.掘井.作灶.",\
    "破土.置产.掘井.动土.安床.",\
    "嫁娶.会亲友.进人口.出行.入宅.移徙.赴任.作灶.",\
    "针灸.伐木.作梁.造庙.行丧.安葬.",\
    "嫁娶.安葬.动土.造桥.",\
    "斋醮.嫁娶.移徙.出行.上梁.入宅.",\
    "纳采.订盟.架马.词讼.开渠.",\
    "祭祀.嫁娶.安床.开市.入宅.探病.上梁.",\
    "探病.余事勿取.",\
    "嫁娶.造庙.造桥.造船.作灶.安葬.",\
    "行丧.安葬.合寿木.",\
    "开光.动土.破土.开市.修造.入宅.安门.",\
    "嫁娶.开市.动土.掘井.开池.安葬.",\
    "开光.安床.",\
    "动土.作灶.行丧.安葬.修坟.",\
    "造庙.安门.行丧.安葬.",\
    "嫁娶.安葬.",\
    "嫁娶.入宅.",\
    "入宅.伐木.",\
    "作灶.祭祀.入宅.嫁娶.",\
    "移徙.入宅.出行.栽种.",\
    "造桥.冠笄.造屋.掘井.",\
    "定磉.安葬.",\
    "移徙.入宅.作灶.安葬.",\
    "入殓.安葬.开市.交易.",\
    "安床.开光.开市.交易.",\
    "动土.冠笄.移徙.入宅.开市.竖柱.上梁.",\
    "诸事不宜.",\
    "诸事不宜.",\
    "嫁娶.移徙.入宅.开市.",\
    "",\
    "嫁娶.出行.安床.作灶.祭祀.入宅.移徙.出火.进人口.置产.",\
    "诸事不宜.",\
    "",\
    "嫁娶.开市.入宅.祈福.安葬.",\
    "上梁.开光.造屋.架马.合寿木.",\
    "动土.破土.开市.安葬.",\
    "开市.开仓.出货财.安床.安门.安葬.",\
    "开光.嫁娶.会亲友.栽种.针灸.安葬.",\
    "造庙.造船.动土.破土.安葬.",\
    "嫁娶.动土.破土.修坟.",\
    "移徙.入宅.出行.祈福.嫁娶.",\
    "嫁娶.开市.安床.掘井.",\
    "嫁娶.祭祀.入宅.造屋.移徙.",\
    "开光.嫁娶.掘井.伐木.作梁.",\
    "诸事不宜.",\
    "出行.祈福.安葬.作灶.",\
    "开仓.冠笄.伐木.作梁.",\
    "栽种.动土.安葬.开市.",\
    "作灶.入殓.安葬.安床.",\
    "动土.上梁.",\
    "行丧.安葬.",\
    "嫁娶.入宅.",\
    "开市.动土.破土.",\
    "余事勿取.",\
    "移徙.入宅.",\
    "开市.赴任.",\
    "余事勿取.",\
    "祈福.安葬.",\
    "斋醮.安门.",\
    "诸事不宜.",\
    "嫁娶.作灶.修坟.安门.入宅.立碑.安葬.安床.",\
    "嫁娶.开光.会亲友.掘井.安门.栽种.",\
    "动土.伐木.作梁.行丧.安葬.开生坟.",\
    "开市.动土.破土.行丧.安葬.",\
    "开市.纳采.造庙.安床.开渠.安葬.",\
    "嫁娶.开市.祈福.斋醮.安葬.",\
    "开仓.造屋.造桥.祭祀.",\
    "嫁娶.安葬.掘井.置产.造船.",\
    "开市.造庙.动土.破土.",\
    "嫁娶.安葬.",\
    "造屋.造船.动土.破土.",\
    "嫁娶.开市.入宅.出火.移徙.",\
    "安床.安门.破土.修坟.安葬.",\
    "移徙.入宅.安门.作梁.安葬.",\
    "开光.掘井.安葬.谢土.修坟.",\
    "诸事不宜.",\
    "入宅.作灶.安床.开仓.",\
    "嫁娶.入宅.安门.移徙.",\
    "作灶.治病.伐木.作梁.",\
    "作灶.治病.",\
    "开市.交易.破土.作灶.",\
    "诸事不宜.",\
    "造屋.治病.",\
    "嫁娶.入宅.治病.赴任.",\
    "诸事不宜.",\
    "诸事不宜.",\
    "入宅.作灶.",\
    "嫁娶.动土.安葬.作灶.",\
    "开市.",\
    "诸事不宜.",\
    "栽种.破土.置产.祭祀.嫁娶.动土.作灶.祈福.",\
    "",\
    "嫁娶.开市.出火.栽种.破土.动土.入宅.移徙.安香.分居.掘井.作灶.",\
    "出行.掘井.破土.行丧.安葬.",\
    "嫁娶.移徙.入宅.探病.出行.造屋.",\
    "安门.动土.破土.行丧.安葬.成服.",\
    "嫁娶.动土.安床.造桥.掘井.",\
    "开市.斋醮.破土.安葬.",\
    "移徙.开市.入宅.安葬.",\
    "嫁娶.祈福.开光.掘井.安葬.行丧.",\
    "入宅.动土.破土.嫁娶.作灶.造船.",\
    "斋醮.作梁.掘井.行丧.安葬.",\
    "嫁娶.祈福.出火.移徙.入宅.",\
    "入宅.修造.动土.破土.安门.上梁.",\
    "掘井.伐木.斋醮.作灶.",\
    "出行.安葬.修坟.开市.",\
    "诸事不宜.",\
    "开光.栽种.治病.安门.作灶.",\
    "",\
    "开市.破土.",\
    "嫁娶.作灶.",\
    "入宅.安葬.",\
    "入宅.动土.破土.余事勿取.",\
    "开市.安葬.",\
    "嫁娶.祈福.",\
    "诸事不宜.",\
    "开市.嫁娶.",\
    "开市.安葬.",\
    };
    */
    BOOL LunarderDate::FindHuangDaoData(CTime time, char* data) {
        if (time.GetYear() < 2008 || time.GetYear() > 2015) {
            return FALSE;
        }

        //      int monday[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        //      int index = monday[(time.GetMonth() -1)] + time.GetDay() - 1;
        //      
        //      if(index < 366)
        //      {
        //          strcpy(data, "宜: ");
        //          strcat(data, lunderyi[index]);
        //          strcat(data, "\r\n");
        //          strcat(data, "忌: ");
        //          strcat(data, lunderji[index]);
        //          return TRUE;
        //      }
        //      return FALSE;
        //  

        int n = CalcDateDiff(time.GetYear(), time.GetMonth(), time.GetDay(), 2008, 1, 1);
        if (n >= 0) {
            FILE* file = 0;

            fopen_s(&file, res_hl_file, "r+b");
            if (file) {
                BOOL isFirst = TRUE;
                strcpy_s(data, 512, "宜：");
                fseek(file, sizeof(LUNERADER_HUANGDAO) * n, SEEK_SET);
                LUNERADER_HUANGDAO l_data;
                fread(&l_data, sizeof(LUNERADER_HUANGDAO), 1, file);

                for (int i = 0; i < 16; i++) {
                    for (int j = 0 ; j < 8; j++) {
                        if ((l_data.yi[i] >> j) & 0x1) {
                            if (!isFirst) {
                                strcat_s(data, 512, "、");
                            } else {
                                isFirst = FALSE;
                            }
                            strcat_s(data, 512, lunader_yi[i * 8 + j]);
                        }
                    }
                }
                strcat_s(data, 512, "\r\n忌：");
                isFirst = TRUE;
                for (int i = 0; i < 16; i++) {
                    for (int j = 0; j < 8; j++) {
                        if ((l_data.ji[i] >> j) & 0x1) {
                            if (!isFirst) {
                                strcat_s(data, 512, "、");
                            } else {
                                isFirst = FALSE;
                            }
                            strcat_s(data, 512, lunader_ji[i * 8 + j]);
                        }
                    }
                }
                fclose(file);
                return TRUE;
            }
        }
        return FALSE;
    }
}

// 
// UINT Get24JieQi_(int iYear, int nIndex)
// {
//  UINT iMonth = nIndex/2+1;
//  int iDay = nIndex%2;
//  UINT8  flag = Logical::g_cLunarHolDay[(iYear - START_YEAR)*12+iMonth -1];
//  UINT16 day;
//  if(iDay < 1)
//      day= 15 - ((flag>>4)&0x0f);
//  else
//      day = ((flag)&0x0f)+15;
//  return day;
// }
// 
// void Test24JieQi()
// {
//  for(int i = 1901; i <= 2050; i++)
//  {
//      printf("%d: ", i);
//      
//      for(int j = 1; j <= 12; j++)
//      {
//          COleDateTime tm = Get24JieQi(i, (j-1)*2);
//          UINT day = Get24JieQi_(i, (j-1)*2);
//          int y = tm.GetYear();
//          int m = tm.GetMonth();
//          int d = tm.GetDay();
//          if(d != day)
//          {
//              printf("(%d %d %d)", (j-1)*2, tm.GetDay(), day);
//          }
// 
//          tm = Get24JieQi(i, (j-1)*2+1);
//          day = Get24JieQi_(i, (j-1)*2+1);
//          
//           y = tm.GetYear();
//           m = tm.GetMonth();
//           d = tm.GetDay();
//          if(tm.GetDay() != day)
//          {
//              printf("(%d %d %d)", (j-1)*2+1, tm.GetDay(), day);
//          }
//      }
//      printf("\r\n");
//  }
// }
/*
void TestLunder()
{
for(int i = 1901; i <= 2050; i++)
{
printf("%d: ", i);
UINT isleep = leapMonth(i);
for(int j = 1; j <=12; j++)
{
UINT32 ret = Logical::LunarderDate::LunarMonthDays(i, j);
UINT high = (ret>>16);
UINT low = ret&0xFFFF;
UINT isleep1 = 0;
if(high > 0)
{
isleep1 = j;
}
if(isleep != isleep1 && (isleep1 != 0))
{
printf("leap month %d %d error ", isleep, isleep1);
}
if(isleep == j)
{
if(high != leapDays(i))
{
printf("%d leap day %d %d error ", j, leapDays(i), high);
}
}
if(monthDays(i, j) != low)
{
printf("%d day %d %d error ", j, monthDays(i, j), low);
}
//printf(" %02d_%02d", (ret>>16), ret&0xFFFF);
}
printf("\r\n");
}
}
*/
