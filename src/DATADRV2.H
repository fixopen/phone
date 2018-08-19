/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      datadrv.h                                       PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年8月24日    */
/*                                                                       */
/*      数据驱动程序头文件                                               */
/*                                                                       */
/* 作者                                                                  */
/*                                                                       */
/*      白亚伟                                                           */
/*                                                                       */
/* 数据结构                                                              */
/*                                                                       */
/*      HISTORY                 通话记录                                 */
/*      MEMO                    记事                                     */
/*      CARD                    名片                                     */
/*      CARDCASE                名片夹                                   */
/*                                                                       */
/* 函数                                                                  */
/*                                                                       */
/*      录音驱动接口函数（11个）                                         */
/*      历史记录驱动接口函数（7个）                                      */
/*      记事驱动接口函数（6个）                                          */
/*      名片驱动接口函数（8个）                                          */
/*      名片夹驱动接口函数（7个）                                        */
/*                                                                       */
/* 关联                                                                  */
/*                                                                       */
/*                                                                       */
/* 历史记录                                                              */
/*                                                                       */
/*  byw, 2001/9/13                                                       */
/*		增加 MarkHistory() 函数。										 */
/*		增加 HISTORY_NEW。												 */
/*      修改 FindCardNumber() 函数。                                     */
/*  byw, 2001/9/19                                                       */
/*      修改 WAVE_TYPE 属性。                                            */
/*  byw, 2001/9/21                                                       */
/*      增加铃声驱动。                                                   */
/*  byw, 2001/10/11                                                      */
/*      重新分配Flash存储地址。                                          */
/*  hqf, 2001/11/13                                                      */
/*      名片字段增加。                                                   */
/*  ygr, 2001/11/15                                                      */
/*       增加录音content字段                                             */
/*  ygr 2001/11/16                                                       */
/*       添加:  DropWaveTail()函数                                       */
/*  ygr 2001/11/21                                                       */
/*              添加问候语数据                                           */
/*************************************************************************/

#ifndef __E201_DATADRV_H_
#define __E201_DATADRV_H_
/*
*  存储空间分配统计表
*
----------------------------------------------------------------------------
Addr scope						Sector(s)	Usage		Items/Item len/Space
----------------------------------------------------------------------------
0x00000000 - 0x000FFFFF			x			System		1M
0x00100000 - 0x001FEFFF			x			(reserved)	<1M (LCD buffer temp = 0x00180000, event task stack temp = 0x00100000)
0x001FF000 - 0x001FFFFF			x			Flash buf	4K
0x00200000 - 0x007FFFFF			x			Wave data	6M

0x01000000 - 0x0107FFFF			x			Code		512K
0x01080000 - 0x01083FFF			4			Hisroty		256/64/16K
0x01084000 - 0x01087FFF			4			Memo		128/128/16K
0x01088000 - 0x010A7FFF			32			Card		1024/128/128K
0x010A8000 - 0x010A8FFF			1			CardCase	128/32/4K
0x010A9000 - 0x010ACFFF			4			Alarm		128/128/16K
0x010AD000 - 0x010C0FFF			9			Picture		9/4096/36K	*note: actual size = 2880 per pic.
0x010B6000 - 0x010C0FFF			11			(reserved)	44K
0x010C1000 - 0x010C4FFF			4			Ring		64/256/16K
0x010C5000 - 0x010C5FFF			1			FastDial	64/64/4K
0x010C6000 - 0x010C6FFF			1			Settings	4K
0x010C7000 - 0x010CFFFF			9			(reserved)	36K
0x010D0000 - 0x010EFFFF			32			Icon		128K
0x010F0000 - 0x010FFFFF			16			Greeting	64K

0x01100000 - 0x011EFFFF			x			ROM DATA	<1M
0x011F0000 - 0x011FFFFF			x			ref, help	64K

*
*/
/*************************************************************************/

/* [ data base addr ]*/
#include "hwfcl.h"
#include "stdio.h"
//#include "options.h"

#define		HISTORY_BASE_ADDR		0x01080000
#define		MEMO_BASE_ADDR			0x01084000
#define		CARD_BASE_ADDR			0x01088000
#define		CARDCASE_BASE_ADDR		0x010A8000
#define		ALARM_BASE_ADDR			0x010A9000
#define		PICTURE_BASE_ADDR		0x010AD000
#define		RING_BASE_ADDR			0x010C1000
#define		FASTDIAL_BASE_ADDR		0x010C5000
#define		SETTINGS_BASE_ADDR		0x010C6000

#define		SMS_BASE_ADDR			0x01088000  /////SMS
#define		MODEL_BASE_ADDR			0x010c7000  /////Model         wangyu
#define     	CONTENT_BASE_ADDR       	0x010c8000  /////temp
#define		GROUP_BASE_ADDR			0x010B6000  /////GROUP         wangyu

#define 	ASC_FONT_ADDR			0x1100000					//长265792字节
#define		PINYIN_ADDRESS			(ASC_FONT_ADDR + 945552)	//拼音表装入地址
#define		HW_DIC_ADDRESS			(ASC_FONT_ADDR + 265792)
#define 	HZ_FONT_ADDR			(ASC_FONT_ADDR + 0x1000)
#define 	HELPADDRESS				0x011F0000			//帮助地址
#define 	PINYINABBR_ADDR			0x011EC764			// 拼音首字母表
#define 	G301_ICON_ADDR          (0x010D0000)		// for ROM

/*************************************************************************/

/* [ Bit access Macros ]*/

#define		BIT0					0x0001
#define		BIT1					0x0002
#define		BIT2					0x0004
#define		BIT3					0x0008
#define		BIT4					0x0010
#define		BIT5					0x0020
#define		BIT6					0x0040
#define		BIT7					0x0080
#define		BIT8					0x0100
#define		BIT9					0x0200
#define		BIT10					0x0400
#define		BIT11					0x0800
#define		BIT12					0x1000
#define		BIT13					0x2000
#define		BIT14					0x4000
#define		BIT15					0x8000

#define		PRIVATE					static

#define		FLASH_SECTOR_SIZE		0x1000			// Flash Sector 大小 4K, （以 Sector 为单位进行擦除）
#define		FLASH_BUF_ADDR			0x1FF000		// Flash 擦除保护缓存地址

/*************************************************************************/

/* [ 录音数据驱动 ] */

/* 录音存储空间 */
#define		WAVE_STORE_ADDR			0x200000		// 空间起始地址
#define		WAVE_STORE_LEN			0x600000		// 空间大小
#define		WAVE_MUTE				0				// 静音采样值
#define		WAVE_MAX_COUNT			125				// 最大录音条数，for safe //YGR

/* 录音存储头结构 */
#define		WAVE_NAME_LEN			14				//（不含结束符）
#define		WAVE_CONTENT_LEN		16				//（含结束符）
struct WAVEHEAD{
	UINT16		Flag;								// 声音属性
	short		Reserved;							// 保留，取值0
	long		Length;								// 声音数据长度
	DateTime	Time;								// 录音时间
	char		Name[WAVE_NAME_LEN + 2];			// 声音数据名称
	char		Content[WAVE_CONTENT_LEN];          // 录音祥细说明
};


// WAVEHEAD.Flag 定义:
//#define	保留					bit15-8			// 取值 0
#define		WAVE_NEW				BIT7			// 未听（新录音）
#define		WAVE_CODED				BIT6			// 已编码
#define		WAVE_CODEMETHOD			(BIT5|BIT4|BIT3)// 编码方法
#define		WAVE_CODENONE			BIT3			// 编码方法: 无编码
#define		WAVE_TYPE				(BIT2|BIT1|BIT0)// 录音类型
#define		WAVE_MIC				BIT0			// 录音类型: MIC
#define		WAVE_AUTO				BIT1			// 录音类型: 留言
#define		WAVE_LINE				BIT2			// 录音类型: 电话线
#define		WAVE_ALL				(BIT2|BIT1|BIT0)// 录音类型: 所有
//YGR 2001-11-21
#define		WAVE_GREETING			BIT8

#define		GREETING_DATA_LEN		0x10000	// 64KB
#define		GREETINGWAVE_RAMADD		(WAVE_STORE_ADDR + WAVE_STORE_LEN - GREETING_DATA_LEN)
#define		GREETINGWAVE_ROMADD		0x010F0000
/* Wave 驱动状态 */

enum WAVESTATE
{
	STATE_CLOSED,
	STATE_OPENREAD,
	STATE_OPENWRITE
};


/* 驱动接口 */

extern void InitWave(void);			// 格式化
extern bool OpenWaveForRead(long id);	// 只读方式打开录音
extern bool OpenWaveForWrite(int type);	// 只写方式打开录音
extern long GetWaveLen(long id);	// 返回录音数据长度
extern int GetWaveTotal(void);		// 返回录音总条数
extern bool WriteWave(UINT8 val);	// 写（录制），返回写成功标记
extern UINT8 ReadWave(void);		// 读（播放），返回数据
extern WAVESTATE CloseWave(void);	// 关闭打开的录音，返回关闭之前状态
extern bool DeleteWave(long id);	// 删除某条录音，返回删除成功标记
extern void DeleteAllWave(void);	// 删除所有录音
extern int FindWave(int type, long *idbuf, int buflen);	// 查找录音，返回查到条数
extern WAVEHEAD* GetWavePointer(long id);	// 获取录音头指针，返回 NULL 表示失败
extern BOOL DropWaveTail(int sizebyte);		// 栽去录音数据尾部部份数据

/*************************************************************************/

/* [ 通话记录数据驱动 ] */

/* 通话记录存储地址 */
//#define		HISTORY_BASE_ADDR		??

/* History Maximum records */
#define		HISTORY_MAX_COUNT		256				// HISTORY_MAX_COUNT × sizeof(HISTORY) 
													// 必须是 FLASH_SECTOR_SIZE 的整数倍

/* History structure */
#define		HISTORY_NAME_LEN		8				// （不含结束符）
#define		HISTORY_NUMBER_LEN		34				// （不含结束符）, 此值必须 > PHONE_NUMBER_INPUT_LEN

typedef struct
{
	UINT16			Flag;							// 标记
	short			NumberType;						// 号码类型
	short			HistoryType;					// 通话类型
	short			Reserved;						// 保留，取值0
	DateTime		Time;							// 时间
	char			Name[HISTORY_NAME_LEN+1];		// 姓名
	char			Number[HISTORY_NUMBER_LEN+1];	// 号码
	long			Length;							// 通话时间（秒）
} HISTORY;

/* Flag 定义 */
//#define	保留					bit15－4		// 保留，取值0
#define		HISTORY_NEWCREATE		BIT2			// 未看（只读）
#define		HISTORY_ERASED			BIT1			// 记录单元已擦除（只读）
#define		HISTORY_EXIST			BIT0			// 记录存在，未删除（只读）

/* HistoryType 定义 */
#define		HISTORY_DIALOUT			0				// 拨出
#define		HISTORY_CALLIN			1				// 打进已接
#define		HISTORY_MISSED			2				// 打进未接
#define		HISTORY_NEW				3				// 所有未看通话记录（包括所有类型）
#define		HISTORY_ALL				4				// 所有类别

extern void InitHistory(void);		// 格式化
extern short AppendHistory(HISTORY *pHistory);	// 返回 ID(ID>=0)
extern bool DeleteHistroy(short iHistoryID);	// 成功返回true, 失败返回false
extern bool ModifyHistory(short iHistoryID, HISTORY const *pHistory); // 成功返回true, 失败返回false
extern bool DeleteHistoryType(short iHistoryType); // 删除某类别
extern int FindHistory(short iHistoryType, short *pHistoryIDBuf, int iBufLen); // 返回查到个数
extern HISTORY* GetHistoryPointer(short iHistoryID); // 返回HISTORY结构指针(只读)，返回NULL表示失败
extern bool MarkHistory(short iHistoryID);  // 标记成已访问，成功返回 true, 失败返回 false

/*************************************************************************/

/* [ 记事数据驱动 ] */

/* 记事存储地址 */
//#define		MEMO_BASE_ADDR			??

/* 记事最大条数 */
#define		MEMO_MAX_COUNT			128				// MEMO_MAX_COUNT × sizeof(MEMO) 
													// 必须是 FLASH_SECTOR_SIZE 的整数倍

/* MEMO 结构 */
#define		MEMO_TITLE_LEN			16				// 标题长度（不含结束符）
#define		MEMO_PASSWORD_LEN		6				// 密码长度（不含结束符）
#define		MEMO_DATA_LEN			96				// 数据长度（不含结束符）

typedef struct 
{
	UINT16		Flag;								// 标记
	char		Title[MEMO_TITLE_LEN + 1];			// 标题
	char		Password[MEMO_PASSWORD_LEN + 1];	// 密码
	char		Data[MEMO_DATA_LEN + 2];			// 数据, +2 为了对齐
	long		Reserved;							// 保留，取值0
}MEMO;

/* MEMO.Flag 定义 */
//#define	保留					bit15－3		// 取值0
#define		MEMO_LOCKSET			BIT2			// 已设置密码
#define		MEMO_ERASED				BIT1			// 记录单元已擦除（只读）
#define		MEMO_EXIST				BIT0			// 记录存在，未删除（只读）

/* 驱动接口 */
extern void InitMemo(void);		// 格式化
extern short AppendMemo(MEMO *pMemo);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool DeleteMemo(short iMemoID);	// 成功返回true, 失败返回false
extern bool ModifyMemo(short iMemoID, MEMO const *pMemo); // 成功返回true, 失败返回false
extern int FindMemo(short *pMemoIDBuf, int iBufLen); // 返回查到个数
extern MEMO* GetMemoPointer(short iMemoID); // 返回MEMO结构指针(只读)，返回NULL表示失败

/*************************************************************************/

/* [ 名片数据驱动 ] */

//#define		CARD_BASE_ADDR			??
#define		CARD_MAX_COUNT			1024			// CARD_MAX_COUNT × sizeof(CARD) 
													// 必须是 FLASH_SECTOR_SIZE 的整数倍
#define		CARD_NAME_LEN			8				// 姓名长度（不含结束符）
#define		CARD_CONTENT_LEN		111				// 内容长度
#define     OTHER_SEG_LEN           40              //除电话、姓名外的字段的长度  hqf

#define		CARD_CONTENT_SEGMENT	15				// Content segment total
#define		CARD_NUMBERSTART		0				// Tel. No. start segment
#define		CARD_NUMBEREND			9				// Tel. No. end segment
#define		CARD_TITLE_NAME_LEN		20				// 
#define		PHONE_NUMBER_INPUT_LEN	16

/* 查找名片字符串范围 */
#define		CARD_FINDABBRNAME		0
#define		CARD_FINDHANZINAME		1
#define		CARD_FINDALL			2


/* 名片结构 */
typedef struct tagCARD
{
	UINT16			Flag;							// 名片标记
	short			CardCaseID;						// 所属名片夹 ID 号
	long			Reserved;						// 保留字段，取值0
	char			Name[CARD_NAME_LEN + 1];		// 姓名字段
	char			Content[CARD_CONTENT_LEN];		// 其他字段，"CARD_CONTENT_SEGMENT" 个字符串
} CARD;

/* 名片 Flag 定义 */
//#define	保留					bit15－3		// 取值0
#define		CARD_RINGALWAYS			BIT2			// 振铃始终打开
#define		CARD_ERASED				BIT1			// 记录单元已擦除（只读）
#define		CARD_EXIST				BIT0			// 记录存在，未删除（只读）

/* 名片字段定义 */

typedef enum
{
	CARD_MOBILE = 0,
	CARD_OFFI1,
	CARD_OFFI2,
	CARD_INSIDE,
	CARD_HOME,
	CARD_FAX,
	CARD_BP,
	CARD_OTHER1,
	CARD_OTHER2,
	CARD_OTHER3,
	CARD_ADDR,
	CARD_COMP,
	CARD_RANK ,
	CARD_EMAIL,
	CARD_NOTE,
	CARD_NONE
}CARDFIELD;

typedef struct 
{
	UINT32	icon;
	const char	*title;
} CARDSEGMENT;

extern void InitCard(void);		// 格式化
extern short AppendCard(CARD *pCard);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool DeleteCard(short iCardID);	// 成功返回true, 失败返回false
extern bool ModifyCard(short iCardID, CARD const *pCard); // 成功返回true, 失败返回false
extern int FindCard(short iCardCaseID, short *pCardIDBuf, int iBufLen); // 返回查到个数
extern int FindCardString(char const *pString, short *pCardIDBuf, int iBufLen, int iFindScope); // 返回查到个数
extern int FindCardNumber(char const *pNumber, short *pCardIDBuf, short *pNumberTypeBuf, short int iBufLen); // 返回查到个数
extern CARD* GetCardPointer(short iCardID); // 返回CARD结构指针(只读)，返回NULL表示失败

/*************************************************************************/

/* [ 名片夹数据驱动 ] */

//#define		CARDCASE_BASE_ADDR		??
#define		CARDCASE_MAX_COUNT		128				// CARDCASE_MAX_COUNT × sizeof(CARDCASE) 
													// 必须等于 FLASH_SECTOR_SIZE

#define		CARDCASE_NAME_LEN		8				// 名字长度（不含结束符）
#define		CARDCASE_PASSWORD_LEN	8				// 密码长度（不含结束符）

typedef struct tagCARDCASE
{
	UINT16	Flag;									// 标记
	short	RingID;									// 特色振铃ID
	long	Reserved;								// 保留，取值0
	char	Name[CARDCASE_NAME_LEN + 1];			// 名片夹名字
	char	Password[CARDCASE_PASSWORD_LEN +1];		// 密码
	short	Private;								// 名片夹排序（只读）
	char	Ext[4];									// 扩展，取值0
} CARDCASE;


/* 名片夹 Flag 定义 */
//#define		保留				bit15－6		// 取值 0
#define		CARDCASE_LOCKSET		BIT5			// 已加密
#define		CARDCASE_RINGSET		BIT4			// 已设特色振铃
#define		CARDCASE_CALLINSAFE		BIT3			// 打进保密
#define		CARDCASE_DIALOUTSAFE	BIT2			// 拨出保密
#define		CARDCASE_ERASED			BIT1			// 记录单元已擦除（只读）
#define		CARDCASE_EXIST			BIT0			// 记录存在，未删除（只读）

extern void InitCardCase(void);		// 格式化
extern short AppendCardCase(CARDCASE *pCardCase);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool DeleteCardCase(short iCardCaseID);	// 成功返回true, 失败返回false
extern bool ModifyCardCase(short iCardCaseID, CARDCASE const *pCardCase); // 成功返回true, 失败返回false
extern int FindCardCase(short *pCardCaseIDBuf, int iBufLen); // 返回查到个数
extern CARDCASE* GetCardCasePointer(short iCardCaseID); // 返回CARD结构指针(只读)，返回NULL表示失败
extern bool SortCardCase(short *pCardCaseIDBuf, int iCount); // 排序成功返回ture, 失败返回false

/*************************************************************************/

/* [ 提醒数据驱动 ] */

//#define		ALARM_BASE_ADDR			??
#define		ALARM_MAX_COUNT			128				// ALARM_MAX_COUNT × sizeof(ALARM) 
													// 必须是 FLASH_SECTOR_SIZE 的整数倍
#define		ALARM_TITLE_LEN			30				// 提醒标题长度（不含结束符）
#define		ALARM_TODO_LEN          76				// 提醒事情长度（不含结束符）

typedef struct
{
	UINT16	Flag;									// 标记
	short	HourBef;								// 提醒提前小时, 0 - 8
	short	MinBef;									// 提醒提前分钟，0 - 59
	short	Music;									// 提醒音乐
	DateTime When;									// 提醒日期时间
	char	Title[ALARM_TITLE_LEN + 1];				// 提醒标题
	char	Todo[ALARM_TODO_LEN + 1];				// 提醒事件
	long	Reserved;								// 保留，取值0
} ALARM;

// WAVEHEAD.Flag 定义:
//#define	保留					bit15-5			// 取值 0
#define		ALARM_REPEATTYPE		(BIT2|BIT3|BIT4)// 重复方式
#define		ALARM_ONE				(BIT2|BIT4)   // 重复方式
#define		ALARM_DAILY				BIT2			// 每天
#define		ALARM_WEEKLY			BIT3			// 每周
#define		ALARM_MONTHLY			(BIT2|BIT3)		// 每月
#define		ALARM_ANNUALLY			BIT4			// 每年
#define		ALARM_ERASED			BIT1			// 记录单元已擦除（只读）
#define		ALARM_EXIST				BIT0			// 记录存在，未删除（只读）

// 接口函数
extern void InitAlarmData(void);		// 格式化
extern short AppendAlarm(ALARM *pAlarm);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool DeleteAlarm(short iAlarmID);	// 成功返回true, 失败返回false
extern bool ModifyAlarm(short iAlarmID, ALARM const *pAlarm); // 成功返回true, 失败返回false
extern int FindAlarm(short *pAlarmIDBuf, int iBufLen); // 返回查到个数
extern ALARM* GetAlarmPointer(short iAlarmID); // 返回 ALARM 结构指针(只读)，返回NULL表示失败

/*************************************************************************/

/* [ 画板数据驱动 ] */

/* 画板存储地址 */
//#define		PICTURE_BASE_ADDR		??

/* 画板数量 */
#define		PICTURE_MAX_COUNT		9

/* 画板尺寸 */
#define		PICTURE_WIDTH			160		//  注意：宽度 × 高度 ÷ 8 = PICTURE_STORESIZE
#define		PICTURE_HEIGHT			140		//

/* 一张画板存储容量 */
#define		PICTURE_STORESIZE		(FLASH_SECTOR_SIZE)

/* 数据接口 */
extern bool LoadPicture(short iPicID, void *pBuf);	// 装入图片
extern bool SavePicture(short iPicID, void *pBuf);	// 保存图片


/*************************************************************************/

/* [ 铃声数据驱动 ] */

//#define		RING_BASE_ADDR			??
#define		RING_MAX_COUNT			64				// RING_MAX_COUNT × sizeof(RING) 
													// 必须是 FLASH_SECTOR_SIZE 的整数倍
#define		RING_NAME_LEN			32				// 铃声名字长度（不含结束符）
#define		RING_CONTENT_LEN		104				// 铃声内容长度

/* 铃声结构 */
typedef struct tagRING
{
	UINT16			Flag;							// 铃声标记
	short			Flat;							// 调号
	short			Rhythm;							// 节奏
	short			Tempo;							// 速度, 单位：拍/分钟
	long			Reserved;						// 保留字段，取值0
	char			Name[RING_NAME_LEN + 2];		// 名字字段
	UINT16			Content[RING_CONTENT_LEN + 1];	// 内容字段（音符数组）
} RING;

/*-------------------------------------------------------------------
*
*	铃声格式定义
*
*	Flat，调号，
*	
*	Rhythm，节奏，
*
*	Tempo，速度，
*
*	RING.Content[] 内存储的是铃声内容，每一个 UINT16 值表示一个音符，
*	整首曲子以 0xFFFF 作为结束标记。
*
*	每个音符（UINT16 类型）的高字节表示音高，低字节表示音长。音高定义
*	为频率表的索引号，频率表格式为：
*	int	NoteFrequencyTable[] = { 00, xx, xx, xx, xx, xx, xx, ..., xx};
*	其中，0 表示休止（频率为零），xx 表示每个音符的频率（单位 Hz）。
*
*	音长定义为：
*
*		0x00		全音符
*		0x01		二分音符
*		0x02		四分音符
*		0x03		八分音符
*		0x04		十六分音符
*		0x05		三十二分音符
*		0x80		附点音符，可以和上述所有音符相或(|)组合使用
*
*	每个音符具体播放时间长度根据音长和节拍速度共同确定。
*
*-------------------------------------------------------------------/

/* 铃声 Flag 定义 */
//#define	保留					bit15－3		// 取值0
#define		RING_SYSTEM				BIT2			// 系统振铃，不允许删除
#define		RING_ERASED				BIT1			// 记录单元已擦除（只读）
#define		RING_EXIST				BIT0			// 记录存在，未删除（只读）

extern void InitRing(void);		// 格式化
extern short AppendRing(RING *pRing);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool DeleteRing(short iRingID);	// 成功返回true, 失败返回false
extern bool ModifyRing(short iRingID, RING const *pRing); // 成功返回true, 失败返回false
extern int FindRing(short *pRingIDBuf, int iBufLen); // 返回查到个数
extern RING* GetRingPointer(short iRingID); // 返回RING结构指针(只读)，返回NULL表示失败


/*************************************************************************/

/* [ 速拨数据驱动 ] */

#define		FASTDIAL_MAX_COUNT		64				// 
#define		FASTDIAL_NUMBER_LEN		45

struct FASTDIAL
{
	UINT16	Flag;									// 标记
	short	Sort;									// 速拨排序（只读）
	UINT32 	nIcon;                             		// 图标
	long	Reserved;								// 保留，取值 0
	char   	strName[CARD_NAME_LEN + 1];				// 名字
	char   	strPhoneNumber[FASTDIAL_NUMBER_LEN + 1];// 电话号码
};

//拨号规则
#define DIALFLAG_DIALOUT    0xFFFF    //直接拨号
#define DIALFLAG_USERSEL    0xFFFE    //由用户选择拨号规则

#define DIALRULE_COUNT      5         //拨号规则条数
#define	OUTLINE_HEAD_LEN	4

/* 速拨 Flag 定义 */
//#define		保留				bit15－2		// 取值 0
#define		FASTDIAL_ERASED			BIT1			// 记录单元已擦除（只读）
#define		FASTDIAL_EXIST			BIT0			// 记录存在，未删除（只读）

extern void InitFastDial(void);		// 格式化
extern short AppendFastDial(FASTDIAL *pFastDial);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool DeleteFastDial(short iFastDialID);		// 成功返回true, 失败返回false
extern bool ModifyFastDial(short iFastDialID, FASTDIAL const *pFastDial); // 成功返回true, 失败返回false
extern int FindFastDial(short *pFastDialIDBuf, int iBufLen); // 返回查到个数
extern FASTDIAL* GetFastDialPointer(short iFastDialID); // 返回CARD结构指针(只读)，返回NULL表示失败
extern bool SortFastDial(short *pFastDialIDBuf, int iCount); // 排序成功返回ture, 失败返回false


//////////////////////////////////////////////////////////////
///  短消息数据驱动

/* [ 短消息数据驱动 ] */

//#define		SMS_BASE_ADDR		(DATA_BASE_ADDR + 0x010C5000)
#define		SMS_MAX_COUNT		320				// SMS_MAX_COUNT × sizeof(SMS) 


//////////信箱结构定义
typedef struct tag_SMSStatus{
		
		UINT8  iLockType;	/// 0 --- 未锁
						//// 1 --- 解锁
						/// 2 ---- 加锁
		
		UINT8	bRevNumber;/////信箱中收件箱的数目
		UINT8	bSendNumber;////信箱中发信箱中的信件数目
		UINT8	bSendedNumber;//信箱中已发信箱中的信件数目
		UINT8	bHavePasswordItem;/////已发信箱中加密信件的数目
		
		BOOL	bIsEnableSound;///接、发时是否需要声音
		BOOL	bIsEnableReturn;//是否需要服务器回执
		BOOL	bIsCanDelete;/////是否拒接
		
		short   iRingID;/////播放声音的ID
		char	szPassword[10];
		char	szName[7]; ////最多3个汉字

}SMSLETTERBOX,*LPSMSLETTERBOX;

													// 必须等于 FLASH_SECTOR_SIZE
//////短消息存储结构
typedef struct tagSMSStruct{
	
	UINT16	Flag;   //2
	UINT8	bMailBox;/////////////接收方的信箱 1
	UINT8	iChildMailBox;/////信箱中子信箱的标示 1
	UINT16	iLetterStatus; ///////信件的阅读状态 2
	
	short	dataID;	/////数据ID 2
	UINT8	bIsFlash;////////是否保存在Flash 1
	UINT8	iLockStatus;/////锁的状态 1
	
	char	szPhone[81];//////最多5个电话号码 81
	char	szTime[15];///////时间 15
	char	szMsg[141];//////短消息内容最多140个char 141
						/////或者70个汉字
	UINT8   Re[9];/////保留 9
}SMS,*LPSMS;


/* 短消息 Flag 定义 */
//#define		保留				bit15－2		// 取值 0
#define			SMS_ERASED			BIT1			// 记录单元已擦除（只读）
#define			SMS_EXIST			BIT0			// 记录存在，未删除（只读）

extern void InitSMS();		// 格式化
extern short AppendSMS(SMS *pSms);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool DeleteSMS(short iSmsID);	// 成功返回true, 失败返回false
extern bool ModifySMS(short iSmsID, SMS const *pSms); // 成功返回true, 失败返回false
extern int FindSMS(short *pSmsIDBuf, int iBufLen); // 返回查到个数
extern SMS* GetSMSPointer(short iSmsID); // 返回SMS结构指针(只读)，返回NULL表示失败

///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
///     集锦 
///     wangyunew
///  1  集锦项  128 条
///  2  集锦组   16 组
/////////////////////////////////////////////////////////////
#define   MAX_CONTENT_NUMBER         128   /// 集锦的总条数
////////// 集锦项的结构定义
typedef struct tagContent
{
  UINT16  Flag;   
  short   iDataID;        // 数据在FLASH中的ID号 
  UINT16  iModelID;     // 所属集锦组
  char    iRecordItem;    // 所属集锦组中的项数(第几项)
  char    szContent[141]; // 纪录的内容
  char	  szReserved[108];	// 0

}CONTENT,*LPCONTENT;

/////////////////集锦项
#define		    CONTENT_ERASED			BIT1			// 记录单元已擦除（只读）
#define			CONTENT_EXIST		  	BIT0			// 记录存在，未删除（只读）

extern void    InitContent();		// 格式化
extern short   AppendContent( CONTENT *pContent);	                      // 返回 ID(ID>=0)，返回 -1 表示失败
extern bool    DeleteContent( short iContentID);	                      // 成功返回true, 失败返回false
extern bool    ModifyContent( short iContentID, CONTENT const *pContent); // 成功返回true, 失败返回false
extern int     FindContent (short iModelID,short *pContentIDBuf, int iBufLen);           // 返回查到个数
extern int    FindAllContent (short *pContentIDBuf, int iBufLen);
extern CONTENT* GetContentPointer(short iContentID);                      // 返回MODEL结构指针(只读)，返回NULL表示失败

//////////  集锦组的结构定义  wangyu
#define   MAX_MODEL_NUMBER         16  /// 集锦的总组数 

typedef struct tagModel
{
  UINT16  Flag;
  short   iDataID;         // 数据在FLASH中的ID号 
  UINT16  iModelItem;      // 集锦组编号   
  char    szModelName[10]; // 集锦组名
  char	  szReserved[16];	// 0
}MODEL,*LPMODEL;
 
/////  集锦组 Flag定义
#define		    MODEL_ERASED			BIT1			// 记录单元已擦除（只读）
#define			MODEL_EXIST		    	BIT0			// 记录存在，未删除（只读）

extern void    InitModel();		// 格式化
extern short   AppendMODEL(MODEL *pMODEL);	// 返回 ID(ID>=0)，返回 -1 表示失败
extern bool    DeleteMODEL(short iMODELID);	// 成功返回true, 失败返回false
extern bool    ModifyModel(short iMODELID, MODEL const *pMODEL); // 成功返回true, 失败返回false
extern int     FindMODEL(short *pMODELIDBuf, int iBufLen); // 返回查到个数
extern MODEL* GetModelPointer(short iMODELID); // 返回MODEL结构指针(只读)，返回NULL表示失败
extern bool SortModel(short *pModelIDBuf, int iCount);

///////////////集锦结束  

///////////////////////////////////////////////////////////////
///     群发 
///     wangyu
///  1  群发内容   5 x 16 条  //每组5条
///  2  群组个数   16     组
/////////////////////////////////////////////////////////////
#define   MAX_GROUP_NUMBER         16  /// 群组 的总组数 
 
typedef struct tagPeople
{
	char szName[10];
	char szPhone[16];
} PEOPLE ;

typedef struct tagGroup
{   
    short    szGroupNumber;
	char	 szName[10];	
	PEOPLE	 Numbers[5]; 
	char     szReserved[114];
} GROUP ;
 
void    InitGroup();		// 格式化
void LoadGroupSend(GROUP *option);
void SaveGroupSend(GROUP *option);
/////////////////////////////////////////////////wangyu  end



/*************************************************************************/

/* [ 系统设置数据驱动 ] */

/*
  拨号规则字符串说明 由于0123456789#* 和','组成，
  ','表示拨号过程中停顿一段时间
*/
#define FORBID_COUNT                       6           //禁拨个数
#define ONLYACCEPT_COUNT                   7           //只接电话个数

#define	IP_DIALIN_LEN						9		// 接入号码
#define	IP_CARDNUM_LEN						21		// 帐号
#define	IP_PASSWORD_LEN						9		// 密码
#define	IP_DELAY_LEN						9		// 中间间隔、后缀
#define	IP_DIALOUT_LEN						21		// 真正拨出号码

struct IP_INFO
{
	char strHead[IP_DIALIN_LEN];            // 接入号码
	char strPostfix0[IP_DELAY_LEN];			// 提示语言和延时
	char strAccount[IP_CARDNUM_LEN];        // 帐号
	char strPostfix1[IP_DELAY_LEN];         // 后缀 '#'等
	char strPassword[IP_PASSWORD_LEN];      // 密码 
	char strPostfix2[IP_DELAY_LEN];         // 后缀 '#'等
	char strDialNum[IP_DIALOUT_LEN];        // 真正拨出号码
	char strPostfix3[IP_DELAY_LEN];         // 后缀 '#'等
};

struct SYSTEM_OPTION
{
	UINT32			 nSetupErasedFlag;							//系统设置已擦除标记
	BOOL             bPhoneWndPlayAni;                          //在显示或隐藏电话窗口时是否显示动画
	UINT16           nNoInterrept;                              //免打扰标志设为1为免打扰
	char             strOnlyAcceptTel[ONLYACCEPT_COUNT][21];    //只振铃电话号码
	INT16            nDefaultRingID;                            //默认振铃
	char             strOutLineHead[OUTLINE_HEAD_LEN + 1];                         //外线字头
    char             strLimitNumber[FORBID_COUNT][PHONE_NUMBER_INPUT_LEN + 1];            //禁拨号码
	INT16            nRecordRingCount;                          //来电自动接听振铃次数
	INT16            nDefaultIP;                                //默认IP帐号的索引(0 ~ 4)
	IP_INFO          iiDialNum[DIALRULE_COUNT];                 //IP拨号帐号等信息（最多5个） 
	int			KxNom;				// 触摸屏校准值，X 轴分子
	int			KxDenom;			// 触摸屏校准值，X 轴分母
	int			KyNom;				// 触摸屏校准值，Y 轴分子
	int			KyDenom;			// 触摸屏校准值，Y 轴分母
	int			Dx;					// 触摸屏校准值，X 轴偏移
	int			Dy;					// 触摸屏校准值，Y 轴偏移
	short		PowerDownTime;		// 自动关机时间（分钟），0 ＝ 永远开机，其他 ＝ 自动关机时间
	short       sTimeType;          // 系统时间标记，0 = 12小时制，1 ＝ 24小时制
	char        strScreenPassword[9]; //屏保密码
	char		Version[128];		// 版本信息(包括软件版本，硬件版本，安装日期，代理商代号)

	//////////////////////短消息设置
	SMSLETTERBOX		SMSStatus[4];    //信箱状态0,1,2,3
	char			    g_pCenterReceivePhone[32],  // 服务中心接入电话号码
						g_pCenterSendPhone[32];     // 服务中心发送号码
};

/* [ 系统设置驱动 ] */

void LoadSystemOption(SYSTEM_OPTION *option);
void SaveSystemOption(SYSTEM_OPTION *option);

/*
void GetSystemOption(SYSTEM_OPTION *sys_option);
BOOL SetSystemOption(SYSTEM_OPTION *sys_option);
void GetPhoneOption(PHONE_OPTION *pho_option); 
BOOL SetPhoneOption(PHONE_OPTION *pho_option); 
void GetFastDial(FASTDIAL *fd_option); 
BOOL SetFastDial(FASTDIAL *fd_option); 
*/

/*************************************************************************/

/* [ 获取用户数据使用空间 ] */

long GetWaveUsed(void);		// 录音已使用空间（字节）
int GetHisrotyUsed(void);	// 已有通话记录条数
int GetMemoUsed(void);		// 已有记事条数
int GetPictureUsed(void);	// 已有画板条数
int GetCardUsed(void);		// 已有名片条数
int GetCardCaseUsed(void);	// 已有名片夹条数
int GetAlarmUsed(void);		// 已有提醒条数
int GetRingUsed(void);		// 已有振铃条数

#endif

