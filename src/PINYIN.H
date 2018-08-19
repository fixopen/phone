/////////////////////////////////////////////////
//                                             //
//         PinYin Driver Header File           //
//        COPYRIGHT: Hanwang Tech (2001)       //
//                                             //
/////////////////////////////////////////////////

#ifndef	_E201_PINYIN_INPUT_H
#define	_E201_PINYIN_INPUT_H

#include "datadrv.h"
//#define	PINYIN_ADDRESS		(ASC_FONT_ADDR + 945552)	//拼音表装入地址
#define	PINYIN_DATA_OFFSET	1664		//拼音数据在拼音表中的偏移地址
#define	PINYIN_LEN_MAX		8			//拼音串最大长度
#define	HANZI_LEN_MAX		316			//汉字串最大长度
#define	PINYIN_TOTAL		416			//拼音总个数

///////////////////////////////////////////////////////////////
//
//	功能	输入拼音，查找汉字
//
//	参数	pinyin	输入拼音字符串，不区分大小写
//			hanzi	输出汉字字符串 buffer
//			size	输出汉字字符串 buffer 的大小，包含结束符
//
//	返回	0		没有查到，例如输入 z 
//			1 -> N	查到 N 个
//			-1		输入拼音字符串为不可能字符串，例如输入 zb 
//
//////////////////////////////////////////////////////////////
int FindPinYin(const char * pinyin, char * hanzi, int size);

///////////////////////////////////////////////////////////////
//
//	功能	把汉字串转换成拼音首字母串
//
//	参数	hanzi	输入汉字字符串
//			pinyin	输出拼音首字母串 buffer，每个汉字一个字母，小写，
//					调用者保证空间足够
//
//	返回	输出的字母个数
//
//////////////////////////////////////////////////////////////
int GetAbbreviation(const char * hanzi, char * pinyin);

#endif
