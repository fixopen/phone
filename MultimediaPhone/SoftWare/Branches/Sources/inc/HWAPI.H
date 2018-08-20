/* --------------------------------------------------------
  Copyright (c) 2006 汉王科技股份有限公司
  
  File：       HWAPI.h

  Author：秦建辉 jhqin@hanwang.com.cn

  Date：2006-8-9

  关键字：
		简体中文 GBK编码 LittleEndian
------------------------------------------------------------ */

#ifndef __HWAPI_H
#define __HWAPI_H

//如果不是以下定义的版本，请屏蔽相应项
#define HANWANG_GBKCODE			//采用GBK编码
//#define HANWANG_BIGENDIAN		//采用BigEndian
//#define HANWANG_DICINTEGRATION	//字典与代码集成
/* ------------------识别范围设置---------------------- */
#define ALC_CHINESE_COMMON		0x00000001  //常用汉字
#define ALC_CHINESE_RARE		0x00000002  //次常用汉字	
#define ALC_CHINESE_VARIANT		0x00000004  //写繁出简
#define ALC_CHINESE_RADICAL		0x00000008	//部首	
#define ALC_NUMERIC				0x00000010  //数字	
#define ALC_UCALPHA				0x00000020  //大写字母	
#define ALC_LCALPHA				0x00000040  //小写字母	
#define	ALC_PUNCTUATION			0x00000080  //标点	
#define	ALC_SYMBOLS				0x00000100  //符号
#define	ALC_GESTURE				0x00000200  //手势
#define ALC_CURSIVE				0x00000400	//草书
#define ALC_VALID				0x000007FF	//识别字符集有效组合

/* -----------------输出自动转换------------------------ */
#define OUT_LOWERCASE			0x20000000	//自动将大写字母转成小写字母
#define OUT_UPPERCASE			0x40000000	//自动将小写字母转成大写字母
#define OUT_SBCCASE				0x80000000	//输出全角字符
#define OUT_ASSISTANT			(OUT_LOWERCASE | OUT_UPPERCASE | OUT_SBCCASE)

/* -----------------常用组合----------------------------- */
#define CHARSET_CNDEFAULT		(ALC_CHINESE_COMMON | ALC_CHINESE_RARE)//缺省中文识别
#define CHARSET_SYMPUNC			(ALC_PUNCTUATION | ALC_SYMBOLS)//符号标点
#define CHARSET_ENGLISH			(ALC_UCALPHA | ALC_LCALPHA)//英文识别

/* ----------------定义手势编码值-------------------------- */
#define CODE_BACKSPACE			0x0008	//回删
#define CODE_TAB				0x0009	//Tab
#define CODE_RETURN				0x000D	//回车
#define CODE_SPACE				0x0020	//空格

/* ---------------常量定义---------------------------- */
#define MAXCANDNUM				10			// 可输出的最大侯选字个数
#define HWRERAMSIZE				(14*1024)	// 引擎需要的临时运算空间大小

/* ------------笔迹结构定义------------------- */
#define STROKEENDMARK			(-1)		// 定义笔画结束标记值

typedef struct tagTHWPoint
{
	short x; // 笔迹 X 坐标
	short y; // 笔迹 Y 坐标
} THWPoint;

/* ------------识别属性定义------------------ */
typedef struct tagTHWAttribute
{
	int				iCandidateNum;	// 用户需要的候选字个数
	unsigned long	dwRange;		// 识别范围设置		
	unsigned char*	pRam;			// 运算空间，大小为 HWRERAMSIZE，要求起始地址4字节对齐
	unsigned char*	pRom;			// 识别字典起始地址，要求起始地址4字节对齐
} THWAttribute;

#ifdef __cplusplus
extern "C" {
#endif

/*
功能：笔迹识别
参数说明：
	pTrace：笔迹数据，其格式为(x1,y1),(x2,y2),...,(STROKEENDMARK,0),...,
	        (STROKEENDMARK,0),(STROKEENDMARK,STROKEENDMARK)，
			其中(STROKEENDMARK,0)为笔画结束标志，
			(STROKEENDMARK,STROKEENDMARK)为字结束标志。
	pAttr：识别属性。
	pResult：识别结果存放区。大小为 2*MAXCANDNUM。	
返回值：
	 -1:参数错误
	= 0:拒识
	> 0:候选字个数
*/
int HWRecognize( const THWPoint* pTrace, const THWAttribute* pAttr, unsigned short* pResult );

#ifdef __cplusplus
}
#endif

#endif
