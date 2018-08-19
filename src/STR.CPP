///////////////////////////////////////////////
//    字符串资源文件，利用 string.exe 生成   //
//     COPYRIGHT Hanwang Tech Corp.(2001)    //
///////////////////////////////////////////////
#include       "str.h"

extern         const char* ENG_STR[];
extern         const char* CHS_STR[];

char**         g_pLanguage = (char**)CHS_STR;

void SetLanguage(char language)
{
     g_pLanguage = (char**)(language ? ENG_STR : CHS_STR);
}

unsigned short GetLanguage()
{
	if (g_pLanguage == CHS_STR)
		return LAN_CHINESE;
	else
		return LAN_ENGLISH;
}



const char* ENG_STR[] = {\
	"LENGTH",\
	"AREA",\
	"WEIGHT",\
	"CAP.",\
	"TEMPER.",\
	"POWER",\
	"ENERGY",\
	"PRESS.",\
	"METER",\
	"FEET",\
	"SHESHI",\
	"DM.",\
	"CM.",\
	"MM.",\
	"YARD",\
	"INCH",\
	"SM.",\
	"KM.",\
	"MILE",\
	"SQ.M.",\
	"SQ.FEET",\
	"SQ.KM.",\
	"LG.",\
	"ARE",\
	"ACRE",\
	"MU",\
	"SQ.YARD",\
	"HEKTARE",\
	"SQ.MILE",\
	"KG",\
	"JIN",\
	"LIANG",\
	"POUND",\
	"GRAM",\
	"CARAT",\
	"OUNCE",\
	"TON",\
	"LITRE",\
	"GAL.AME",\
	"GAL.ENG",\
	"CUB.CUN",\
	"OUNCE",\
	"PINT",\
	"QUART",\
	"BUSHEL",\
	"CUB.YARD",\
	"CUB.M",\
	"SHESHI",\
	"KAISHI",\
	"HUASHI",\
	"W.",\
	"HP.AME",\
	"HP.ENG",\
	"QIANK.",\
	"JIAO.",\
	"KWH.",\
	"MH.",\
	"P.",\
	"S.GP.",\
	"CLEAR",\
	"CONVT",\
};

const char* CHS_STR[] = {\
	"长度",\
	"面积",\
	"重量",\
	"容量",\
	"温度",\
	"功率",\
	"能量",\
	"压强",\
	"米",\
	"英尺",\
	"市尺",\
	"分米",\
	"厘米",\
	"毫米",\
	"码",\
	"英寸",\
	"海里",\
	"公里",\
	"英里",\
	"平方米",\
	"平方英尺",\
	"平方公里",\
	"坪",\
	"公亩",\
	"英亩",\
	"亩",\
	"平方码",\
	"公顷",\
	"平方英里",\
	"公斤",\
	"市斤",\
	"市两",\
	"磅",\
	"克",\
	"克拉",\
	"盎司",\
	"吨",\
	"升",\
	"美加仑",\
	"英加仑",\
	"立方寸",\
	"美盎司",\
	"美品脱",\
	"美夸脱",\
	"蒲式耳",\
	"立方码",\
	"立方米",\
	"摄氏",\
	"开氏",\
	"华氏",\
	"瓦",\
	"公马力",\
	"英马力",\
	"千卡",\
	"焦耳",\
	"千瓦时",\
	"毫汞柱",\
	"帕",\
	"大气压",\
	"清除",\
	"换算",\
};
