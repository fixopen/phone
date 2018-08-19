/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      datadrv.cpp                                     PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年8月24日    */
/*                                                                       */
/*      数据驱动程序文件												 */
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
/*  byw, 2001/9/24                                                       */
/*      修改：SavePicture() bug。                                        */
/*                                                                       */
/*  byw, 2001/9/29                                                       */
/*      修改：DeleteRing()。                                             */
/*                                                                       */
/*  wf, 2001/10/23                                                       */
/*      修改：FindCardNumber                                             */
/*                                                                       */
/*  byw, 2001/10/23                                                      */
/*      修改：SaveFlashSector() 和 LoadFlashSector() 的Bug.              */
/*                                                                       */
/*  wf, 2001/10/25                                                       */
/*      修改：通话记录的HistoryCompare                                   */
/*                                                                       */
/*	ygr,2001/11/5						                                 */
/*		修改：IllegalType()的Bug                                         */
/*            OpenWaveForWrite,OpenWaveForRead                          */
/*                                                                       */
/*  ygr 2001/11/16                                                       */
/*       添加:  DropWaveTail()函数                                       */
/*                                                                       */
/*  ygr 2001/11/21                                                       */
/*       修改:  OpenWaveForWrite,OpenWaveForRead，CloseWave              */
/*              添加问候语数据                                           */
/*                                                                       */
/*  ygr 2001/11/28                                                       */
/*       完成:  DeleteWave 与  IllegalWave函数				             */
/*                                                                       */
/*  ygr 2001/12/6                                                        */
/*       完成:  OpenWaveForWrite函数				                     */
/*                                                                       */
/*  byw 2001/12/23                                                       */
/*       修改:  FindCardString() 函数				                     */
/*************************************************************************/


#include "datadrv.h"
#include "flash.h"
#include "stdlib.h" 
#include "phoneapp.h"
#include "syssetup.h"
#include "edit.h"
/*************************************************************************/

/* [ Flash 接口 ] */

/* 把一个 Sector 的 Flash 装入缓存 */
PRIVATE void LoadFlashSector(void *addr)
{
	//memcpy((void*)FLASH_BUF_ADDR, (void*)((long)addr & 0xFFFFF000), FLASH_SECTOR_SIZE);
	register int i;
	register UINT8 *psrc = (UINT8 *) ((long)addr & 0xFFFFF000);
	register UINT8 *pdest = (UINT8 *)FLASH_BUF_ADDR;

	for(i = 0; i < FLASH_SECTOR_SIZE ;i++, psrc++, pdest++)
		*pdest = *psrc;
	FlashEraseSector((UINT16*)addr);

}

/* 把一个 Sector 的 缓存 写入 Flash */
PRIVATE void SaveFlashSector(void *addr)
{
	FlashWrite((UINT16*)((long)addr & 0xFFFFF000), (UINT16*)FLASH_BUF_ADDR, FLASH_SECTOR_SIZE / 2);
}

/*************************************************************************/
/*复位上电测试*/
#define TEST_CHAR ("E201 E_PHONE DEVLEPOR: 白亚伟，向国威，董博，郝庆丰，王飞，张相中，余国荣，甘辉，郑春花，程晓东，宋英剑，王钰，王洪滨，孟庆发等。copyright 1.0, 2002,2")
bool g_bFirstPowerOn=true;
extern SYSTEM_OPTION g_SystemOption;
bool FirstBootTest()
{
	if(strcmp((char *)FIRST_POWERON_BUFF,TEST_CHAR))
	{
		strcpy((char *)FIRST_POWERON_BUFF,TEST_CHAR);
		g_bFirstPowerOn=true;
		g_SystemOption.nResetTimes = 0;
	}		
	else
	{
		g_bFirstPowerOn=false;
		g_SystemOption.nResetTimes++;
	}
	::SaveSystemOption(&g_SystemOption);
	return g_bFirstPowerOn;
}

/*************************************************************************/

/* [ 录音数据驱动 ] */

/* 全局变量 */
WAVEHEAD	*pWaveHead, *(*pWaveTail);		// 队列指针
WAVESTATE	WaveState;					// 录音状态
WAVEHEAD	*pReadWave;					// 当前指针
long		WaveCount;					// 数据计数器
UINT8		*pAccess;					// 数据指针
extern     SYSTEM_OPTION g_SystemOption;
/* 判断录音数据为非法 */
PRIVATE bool IllegalWave(long id)
{
	// 保留
	if(id < WAVE_STORE_ADDR ) return true;
	WAVEHEAD * h = pWaveHead,* pd=(WAVEHEAD *)id;
	if(id==GREETINGWAVE_RAMADD||id==GREETINGWAVE_ROMADD)
		return false;
	while( ( h < (WAVEHEAD *) (WAVE_STORE_ADDR + WAVE_STORE_LEN) )
		 && (h < *pWaveTail) )
	{
		if(pd==h)
			return false;
		else
			h = (WAVEHEAD*)((long)h + sizeof(WAVEHEAD) + h->Length);
	}
	return true;
}

/* 判断录音类型合法 */
PRIVATE bool IllegalType(int type)
{
	type &= WAVE_TYPE;
	if ((type == WAVE_MIC) || (type == WAVE_AUTO) || (type == WAVE_LINE) || (type == WAVE_ALL))
		return false;
	else	
		return true;
}

//////////////////////////////////////////////////
//
//  读方式打开录音记录
//
//	参数：	id		录音记录标识
//
//  返回：	true	打开成功
//			false	打开失败
//
//////////////////////////////////////////////////
bool OpenWaveForRead(long id)
{
	WAVEHEAD	*pw = (WAVEHEAD*)id;
	if (WaveState != STATE_CLOSED)
		return false;
	if (IllegalWave(id))
		return false;
	WaveCount = pw->Length;
	pAccess = (UINT8*)((long)pw + sizeof(WAVEHEAD));
	pReadWave = pw;
	WaveState = STATE_OPENREAD;
	if (pReadWave->Flag & WAVE_NEW)
		pReadWave->Flag &= ~WAVE_NEW;
	return true;
}

//////////////////////////////////////////////////
//
//  写方式打开录音记录
//
//	参数：	type	录音类别
//
//  返回：	true	打开成功
//			false	打开失败
//
//////////////////////////////////////////////////
extern   g_pPhoneWnd ;
int g_iType=0xff;
bool OpenWaveForWrite(int type)
{
	DateTime	time;
	WAVEHEAD	* pWaveGreeting=(WAVEHEAD *)GREETINGWAVE_RAMADD;
	if (WaveState != STATE_CLOSED)
		return false;
	GetSysTime(&time);
	if(type==WAVE_GREETING) 
	{
		if((long)(*pWaveTail)>=(long)(GREETINGWAVE_RAMADD) ) return false;
		g_iType=type; 
		pWaveGreeting->Flag=type; 
		pAccess=(UINT8*)(GREETINGWAVE_RAMADD + sizeof(WAVEHEAD));
		strcpy(pWaveGreeting->Name,"问候语");
		WaveCount = 0;
		WaveState = STATE_OPENWRITE;
		return true;
	}
	if (IllegalType(type))
		return false;
	g_iType=type;
	pAccess = (UINT8*)((long)(*pWaveTail) + sizeof(WAVEHEAD));
	if ((long)pAccess > WAVE_STORE_ADDR + WAVE_STORE_LEN - sizeof(long))
		return false;
	memset((void*)(*pWaveTail), 0, sizeof(WAVEHEAD));
	(*pWaveTail)->Flag = WAVE_NEW | WAVE_CODENONE | type;
	extern BOOL GetPhoneStatus();
	if(GetPhoneStatus())	 						//读取电话信息
	{
		if(type==WAVE_AUTO)
		{
			extern short g_wLastHistroyID;
			HISTORY *pCur = GetHistoryPointer(g_wLastHistroyID);
			if(pCur && g_wLastHistroyID>=0)
			{
				char * sname=pCur->Name;
				if (strcmp(pCur->AreaCode, 
				          g_SystemOption.LocalAreaCode) != 0)
					strcpy((*pWaveTail)->Number,pCur->AreaCode);
				int len=strlen((*pWaveTail)->Number);
				strncpy((*pWaveTail)->Name,pCur->Name,WAVE_NAME_LEN);
				strncat((*pWaveTail)->Number,pCur->Number,WAVE_NUMBER_LEN-len);
			}
			if((*pWaveTail)->Name[0]==NULL) 
				strcpy((*pWaveTail)->Name,"留言");
		}
		else
			strcpy((*pWaveTail)->Name,"电话录音");	//1-8 ygr
	}
	else
		strcpy((*pWaveTail)->Name, "本机录音");
	(*pWaveTail)->Time = time;
	WaveCount = 0;
	WaveState = STATE_OPENWRITE;
	return true;
}

//////////////////////////////////////////////////
//
//  获取录音数据长度
//
//	参数：	id		录音ID
//
//  返回：	数据长度
//
//////////////////////////////////////////////////
long GetWaveLen(long id)
{
	WAVEHEAD	*pw = (WAVEHEAD*)id;
	return pw->Length;
}

//////////////////////////////////////////////////
//
//  获取录音总条数
//
//	参数：	无
//
//  返回：	录音总条数
//
//////////////////////////////////////////////////
int GetWaveTotal(void)
{
	WAVEHEAD	*h = pWaveHead, *t = (*pWaveTail);
	int			total = 0;

	for (;(h != t) && (total < WAVE_MAX_COUNT); total ++)
		h = (WAVEHEAD*)((long)h + sizeof(WAVEHEAD) + h->Length);
	return total;
}
//////////////////////////////////////////////////
//
//  写数据（录制）
//
//	参数：	val		录音采样数据
//
//  返回：	true	写成功
//			false	写失败（空间满）
//
//////////////////////////////////////////////////
bool WriteWave(UINT8 val)
{
	if ((WaveState == STATE_OPENWRITE) && ((long)pAccess < WAVE_STORE_ADDR + WAVE_STORE_LEN)){
		*pAccess = val;	//byw
		pAccess ++;
		return true;
		}
	else{
		CloseWave(); 
		return false;
	}
}

//////////////////////////////////////////////////
//
//	读数据（播放）
//
//	参数：	无
//
//  返回：	录音采样数据
//
//////////////////////////////////////////////////
UINT8 ReadWave(void)
{
	if ((WaveState == STATE_OPENREAD) && (WaveCount != 0)){
		WaveCount --;
		return *(pAccess++);
	}
	else{
		CloseWave();
		return WAVE_MUTE;
	}
}

//////////////////////////////////////////////////
//
//  关闭录音
//
//	参数：	无
//
//  返回：	关闭之前的录音状态，取值
//				STATE_CLOSED		关闭
//				STATE_OPENREAD		读
//				STATE_OPENWRITE		写
//
//////////////////////////////////////////////////
WAVESTATE CloseWave(void)
{
	WAVESTATE	old = WaveState;
	WAVEHEAD	*next;
	
	WaveState = STATE_CLOSED; 
	if (old == STATE_OPENWRITE){
		while ((long)pAccess % 4 != 0)
			pAccess ++;
		if(g_iType==WAVE_GREETING)
		{
			WAVEHEAD	* pWaveGreeting=(WAVEHEAD *)GREETINGWAVE_RAMADD;
			pWaveGreeting->Length= (long)pAccess - (long)pWaveGreeting - sizeof(WAVEHEAD); 
		}
		else
		{
			(*pWaveTail)->Length = (long)pAccess - (long)(*pWaveTail) - sizeof(WAVEHEAD);
			(*pWaveTail) = (WAVEHEAD*)((long)(*pWaveTail) + sizeof(WAVEHEAD) + (*pWaveTail)->Length);
			(*pWaveTail) ->Length =0;
		}
	}
	pAccess = (UINT8*)((long)(*pWaveTail) + sizeof(WAVEHEAD));
	return old;
}

////////////////////////////////////////////////////
//
//  去掉结尾处的部份录音
//
//	参数：	栽去字节数
//
//  返回：	成功1 ，失败 0
////////////////////////////////////////////////////
BOOL DropWaveTail(int sizebyte)
{
	if (WaveState != STATE_CLOSED)
	{
		UINT8 * droptail=(UINT8 *)(pAccess-sizebyte);
		if((long)droptail<(long)((*pWaveTail)+sizeof(WAVEHEAD)))
			return false;
		else
			pAccess=droptail;
	}
	return true;
}
	
//////////////////////////////////////////////////
//
//  删除录音
//
//	参数：	id		待删除录音的ID
//
//  返回：	true	成功
//			false	失败
//
//////////////////////////////////////////////////
bool DeleteWave(long id)
{
	// temp
	CloseWave();
	//Todo
	if(IllegalWave(id))
		return false;
	WAVEHEAD *	pd = (WAVEHEAD*)id;
	int lenthofdrop=pd->Length ;
	UINT32 * ps=(UINT32 *)( (UINT32)pd+ sizeof(WAVEHEAD)+pd->Length );
	KillWatchDog();
	memmove((UINT32 *)pd,ps,((UINT32)(*pWaveTail)-(UINT32)pd));
	CLR_WATCHDOG();
	OpenWatchDog();
	(*pWaveTail) =(WAVEHEAD *)((UINT32)(*pWaveTail)-sizeof(WAVEHEAD)-lenthofdrop);
	pAccess=(UINT8 *)(*pWaveTail);
	return true;
}

//////////////////////////////////////////////////
//
//  删除所有录音
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void DeleteAllWave(void)
{
	CloseWave();
	pWaveHead = (*pWaveTail) = (WAVEHEAD*)WAVE_STORE_ADDR;
	pWaveHead->Length = 0;
}
		
//////////////////////////////////////////////////
//
//  查找录音
//
//	参数：	type	录音类型，取值
//						WAVE_MIC		麦克风
//						WAVE_AUTO		留言
//						WAVE_LINE		电话线
//						WAVE_ALL		所有类别
//					注释：	1)WAVE_MIC、WAVE_AUTO、WAVE_LINE 
//							  可以用 '|' 组合起来使用, 
//							  都组合起来的结果等于 WAVE_ALL。
//							2)上述属性可以或上('|') WAVE_NEW 
//							  来查找该属性的新录音。
//
//			idbuf	存放查找结果的buffer
//
//			buflen	idbuf的长度
//
//  返回：	查到的录音个数
//
//////////////////////////////////////////////////
int FindWave(int type, long *idbuf, int buflen)
{
	int			i, total = 0;
	WAVEHEAD	*h = pWaveHead;

	if (IllegalType(type))
		return 0;
	for (i = 0; (h != (*pWaveTail)) && ( i < buflen); i ++){
		if ((type & h->Flag & WAVE_TYPE) && (!(type & WAVE_NEW) || (h->Flag & WAVE_NEW))){
			*(idbuf + total) = (long)h;
			total ++;
		}
		h = (WAVEHEAD*)((long)h + sizeof(WAVEHEAD) + h->Length);
	}
	return total;
}

//////////////////////////////////////////////////
//
//  获取录音头指针
//
//	参数：	id		录音 ID 号
//
//  返回：	指向 WAVEHEAD 结构的指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
WAVEHEAD* GetWavePointer(long id)
{
	if (IllegalWave(id))
		return false;
	return (WAVEHEAD*)id;
}

//////////////////////////////////////////////////
//
//  初始化录音驱动，删除所有录音
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitWave(void)
{
	pWaveTail = (WAVEHEAD**)WAVE_TAIL_STORE;
	if(g_bFirstPowerOn)
	{
		pWaveHead = (*pWaveTail) = (WAVEHEAD*)WAVE_STORE_ADDR;
		pWaveHead->Length =0;
	}
	else
	{
		pWaveHead = (WAVEHEAD*)WAVE_STORE_ADDR;
		WAVEHEAD * pnext =pWaveHead;
		while( pnext <  (WAVEHEAD*)( WAVE_STORE_ADDR + WAVE_STORE_LEN) )
		{
			if( (pnext->Length < 0) || IllegalType(pnext->Flag) )
				break;		
			if( pnext== (*pWaveTail) ) return ;
			pnext=(WAVEHEAD*)( (long)pnext + pnext->Length + sizeof(WAVEHEAD) ); 
		}
		(*pWaveTail) = (WAVEHEAD*)WAVE_STORE_ADDR;
	}
	WaveState = STATE_CLOSED;
}

/*************************************************************************/

/* 通话记录驱动 */

/* 定位一条通话记录 */
PRIVATE HISTORY* LocateHistory(short iHistoryID)
{
	return (HISTORY*)(HISTORY_BASE_ADDR + sizeof(HISTORY) * iHistoryID);
}

/* 擦除一条通话记录 */
PRIVATE void EraseOneHistory(short iHistoryID)
{
	HISTORY		*his;

	his = LocateHistory(iHistoryID);
	LoadFlashSector(his);
	memset((void*)(FLASH_BUF_ADDR + (long)his % FLASH_SECTOR_SIZE), 0xFF, sizeof(HISTORY));
	SaveFlashSector(his);
}

/* 写入一条通话记录 */
PRIVATE void WriteOneHistory(short iHistoryID, HISTORY const *pHistory)
{
	HISTORY		his;

	memcpy(&his, pHistory, sizeof(HISTORY));
	his.Flag = his.Flag & (~HISTORY_ERASED) | HISTORY_EXIST | HISTORY_NEWCREATE;
	FlashWrite((UINT16*)LocateHistory(iHistoryID), (UINT16*)(&his), sizeof(HISTORY) / 2);
}

/* 判断通话记录合法 */
PRIVATE HISTORY* LegalHistory(short iHistoryID)
{
	HISTORY*	phis;

	if ((iHistoryID >= HISTORY_MAX_COUNT) || (iHistoryID < 0))
		return NULL;
	phis = LocateHistory(iHistoryID);
	if (phis->Flag & HISTORY_ERASED)
		return NULL;
	if (!(phis->Flag & HISTORY_EXIST))
		return NULL;
	return phis;
}

/* 判断通话记录类型合法 */
PRIVATE bool LegalHistoryType(short iHistoryType)
{
	if ((iHistoryType == HISTORY_DIALOUT) || (iHistoryType == HISTORY_CALLIN) || \
	   (iHistoryType == HISTORY_MISSED) || (iHistoryType == HISTORY_ALL))
	   return true;
	else
		return false;
}

/* 通话记录排序比较 */
PRIVATE int HistoryCompare(void const *key, void const *element)
{
	HISTORY		*pkey, *pelement;

	pkey = LocateHistory(*(short*)key);
	pelement = LocateHistory(*(short*)element);
	if (pkey->Time == pelement->Time)
		return 0;
	else
		return (pkey->Time > pelement->Time ? -1 : 1);
}


//////////////////////////////////////////////////
//
//  初始化通话记录驱动
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitHistory(void)
{
	// added by hqf 初始化时点亮未看通话记录灯
	short iNotVisitHis = 0 , iHisMissed;  
	short pHistoryId[HISTORY_MAX_COUNT];
	iHisMissed = ::FindHistory(HISTORY_MISSED, 
		pHistoryId , HISTORY_MAX_COUNT);
	for(short i = 0; i< iHisMissed; i++)
	{
		if(GetHistoryPointer(pHistoryId[i])->Flag & HISTORY_NEWCREATE)
			iNotVisitHis++;
	}
	if(iNotVisitHis==0)         
		//			if(LEDIsSet(LED_MISSEDCALL))
		ClearLED(LED_MISSEDCALL);
	else
		SetLED(LED_MISSEDCALL);
	//for (long addr = HISTORY_BASE_ADDR; addr < HISTORY_BASE_ADDR + HISTORY_MAX_COUNT * sizeof(HISTORY); addr += FLASH_SECTOR_SIZE)
	//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  新增通话记录
//
//	参数：	pHistory	指向新增通话记录
//
//  返回：	新增 ID 号，ID >= 0, 返回 -1 表示失败
//
//////////////////////////////////////////////////
short AppendHistory(HISTORY *pHistory)
{
	HISTORY*	phis;
	DateTime	dt;
	short		i, j;

	for (i = 0; i < HISTORY_MAX_COUNT; i ++){
		phis = LocateHistory(i);
		if (phis->Flag & HISTORY_ERASED)
			break;
	}
	if (i == HISTORY_MAX_COUNT){
		for (i = 0; i < HISTORY_MAX_COUNT; i ++){
			phis = LocateHistory(i);
			if (!(phis->Flag & HISTORY_EXIST)){
				EraseOneHistory(i);
				break;
			}	
		}
	}
	if (i == HISTORY_MAX_COUNT){
		dt.year = 9999;
		for (i = 1; i < HISTORY_MAX_COUNT; i ++){
			phis = LocateHistory(i);
			if (phis->Time < dt){
				j = i;
				dt = phis->Time;
			}
		}
		i = j;
		EraseOneHistory(i);
	}
	WriteOneHistory(i, pHistory);
	return i;
}

//////////////////////////////////////////////////
//
//  删除通话记录
//
//	参数：	iHistoryID		待删除通话记录ID
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteHistroy(short iHistoryID)
{
	HISTORY*	phis;

	if (!(phis = LegalHistory(iHistoryID)))
		return false;
	return FlashWriteWord(&(phis->Flag), phis->Flag & ~HISTORY_EXIST);
}

//////////////////////////////////////////////////
//
//  修改通话记录
//
//	参数：	iHistoryID		待修改通话记录ID
//			pHistory		修改内容
//
//  返回：	修改成功标记
//
//////////////////////////////////////////////////
bool ModifyHistory(short iHistoryID, HISTORY const *pHistory)
{
	HISTORY*	phis;
	
	if (!(phis = LegalHistory(iHistoryID)))
		return false;
	EraseOneHistory(iHistoryID);
	WriteOneHistory(iHistoryID, pHistory);
	return true;
}

//////////////////////////////////////////////////
//
//  删除某类通话记录
//
//	参数：	iHistoryType	类别
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteHistoryType(short iHistoryType)
{
	short	buf[HISTORY_MAX_COUNT];
	int		len;
	
	if (!LegalHistoryType(iHistoryType))
		return false;
	len = FindHistory(iHistoryType, buf, HISTORY_MAX_COUNT);
	if (len == 0)
		return false;
	for (int i = 0; i < len; i ++)
		DeleteHistroy(buf[i]);
	return true;
}

//////////////////////////////////////////////////
//
//  查找通话记录
//
//	参数：	iHistoryType	类别
//			pHistoryIDBuf	存放结果的buffer
//			iBufLen			buffer 长度
//
//  返回：	查到的记录个数
//
//////////////////////////////////////////////////
int FindHistory(short iHistoryType, short *pHistoryIDBuf, int iBufLen)
{
	HISTORY*	phis;
	int			count = 0;
	
	for (short i = 0; (i < HISTORY_MAX_COUNT) && (count < iBufLen); i ++){
		if ((phis = LegalHistory(i)) != NULL){
			if ((phis->HistoryType == iHistoryType) || (iHistoryType == HISTORY_ALL) || ((iHistoryType == HISTORY_NEW) && (phis->Flag & HISTORY_NEWCREATE))){
				*(pHistoryIDBuf + count) = i;
				count ++;
			}
		}
	}
	qsort(pHistoryIDBuf, count, 2, HistoryCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  获取通话记录指针（只读）
//
//	参数：	iHistoryID		通话记录ID
//
//  返回：	指向通话记录的指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
HISTORY* GetHistoryPointer(short iHistoryID)
{
	return LegalHistory(iHistoryID);
}	

//////////////////////////////////////////////////
//
//  把通话记录标记成已访问
//
//	参数：	iHistoryID		通话记录ID
//
//  返回：	成功标记
//
//////////////////////////////////////////////////
bool MarkHistory(short iHistoryID)
{
	HISTORY*	phis;
	
	if (!(phis = LegalHistory(iHistoryID)))
		return false;
	return FlashWriteWord(&(phis->Flag), phis->Flag & ~HISTORY_NEWCREATE);
}


/*************************************************************************/

/* 记事驱动 */

/* 定位记事 */
PRIVATE MEMO* LocateMemo(short iMemoID)
{
	return (MEMO*)(MEMO_BASE_ADDR + sizeof(MEMO) * iMemoID);
}

/* 擦除某条记事 */
PRIVATE void EraseOneMemo(short iMemoID)
{
	MEMO		*memo;

	memo = LocateMemo(iMemoID);
	LoadFlashSector(memo);
	memset((void*)(FLASH_BUF_ADDR + (long)memo % FLASH_SECTOR_SIZE), 0xFF, sizeof(MEMO));
	SaveFlashSector(memo);
}

/* 写某条记事进 Flash */
PRIVATE void WriteOneMemo(short iMemoID, MEMO const *pMemo)
{
	MEMO		memo;

	memcpy(&memo, pMemo, sizeof(MEMO));
	memo.Flag = memo.Flag & (~MEMO_ERASED) | MEMO_EXIST;
	FlashWrite((UINT16*)LocateMemo(iMemoID), (UINT16*)(&memo), sizeof(MEMO) / 2);
}

/* 判断记事合法 */
PRIVATE MEMO* LegalMemo(short iMemoID)
{
	MEMO*	pmemo;

	if ((iMemoID >= MEMO_MAX_COUNT) || (iMemoID < 0))
		return NULL;
	pmemo = LocateMemo(iMemoID);
	if (pmemo->Flag & MEMO_ERASED)
		return NULL;
	if (!(pmemo->Flag & MEMO_EXIST))
		return NULL;
	return pmemo;
}

/* 查找记事比较函数 */
PRIVATE int MemoCompare(void const *key, void const *element)
{
	MEMO	*pkey, *pelement;

	pkey = LocateMemo(*(short*)key);
	pelement = LocateMemo(*(short*)element);
	return strcmp(pkey->Title, pelement->Title);
}

//////////////////////////////////////////////////
//
//  初始化记事
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitMemo(void)
{
	//for (long addr = MEMO_BASE_ADDR; addr < MEMO_BASE_ADDR + MEMO_MAX_COUNT * sizeof(MEMO); addr += FLASH_SECTOR_SIZE)
	//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  新增记事
//
//	参数：	pMemo		新增记事指针
//
//  返回：	新增 ID，ID >= 0, 返回 -1 表示失败
//
//////////////////////////////////////////////////
short AppendMemo(MEMO *pMemo)
{
	MEMO*		pmemo;
	short		i, j;

	for (i = 0; i < MEMO_MAX_COUNT; i ++){
		pmemo = LocateMemo(i);
		if (pmemo->Flag & MEMO_ERASED)
			break;
	}
	if (i == MEMO_MAX_COUNT){
		for (i = 0; i < MEMO_MAX_COUNT; i ++){
			pmemo = LocateMemo(i);
			if (!(pmemo->Flag & MEMO_EXIST)){
				EraseOneMemo(i);
				break;
			}	
		}
	}
	if (i == MEMO_MAX_COUNT)
		return -1;
	WriteOneMemo(i, pMemo);
	return i;
}

//////////////////////////////////////////////////
//
//  删除记事
//
//	参数：	iMemoID		删除记事的 ID
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteMemo(short iMemoID)
{
	MEMO*	pmemo;

	if (!(pmemo = LegalMemo(iMemoID)))
		return false;
	return FlashWriteWord(&(pmemo->Flag), pmemo->Flag & ~MEMO_EXIST);
}
	
//////////////////////////////////////////////////
//
//  修改记事
//
//	参数：	iMemoID		修改记事的ID
//			pMemo		修改内容
//
//  返回：	修改成功标记
//
//////////////////////////////////////////////////
bool ModifyMemo(short iMemoID, MEMO const *pMemo)
{
	MEMO*	pmemo;
	
	if (!(pmemo = LegalMemo(iMemoID)))
		return false;
	EraseOneMemo(iMemoID);
	WriteOneMemo(iMemoID, pMemo);
	return true;
}
	
//////////////////////////////////////////////////
//
//  查找记事
//
//	参数：	pMemoIDBuf	存放查找结果的 buffer
//			iBufLen		buffer 长度
//
//  返回：	查到的个数
//
//////////////////////////////////////////////////
int FindMemo(short *pMemoIDBuf, int iBufLen)
{
	MEMO*		pmemo;
	int			count = 0;
	
	for (short i = 0; (i < MEMO_MAX_COUNT) && (count < iBufLen); i ++){
		CLR_WATCHDOG();
		if ((pmemo = LegalMemo(i)) != NULL){
			*(pMemoIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pMemoIDBuf, count, 2, MemoCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  获取记事指针
//
//	参数：	iMemoID		记事的 ID
//
//  返回：	记事的指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
MEMO* GetMemoPointer(short iMemoID)
{
	return LegalMemo(iMemoID);
}	
 
/*************************************************************************/

/* 画板驱动 */

extern UINT8 g_iTwoBit2OneBitHeight[256];
extern UINT8 g_iTwoBit2OneBitLow[256];
extern UINT16 g_iOneBit2TwoBitMap[256];

/* 获取画板地址 */
PRIVATE UINT8* GetPictureAddr(short iPicID)
{
	if ((iPicID < 0) || (iPicID >= PICTURE_MAX_COUNT))
		return NULL;
	else
		return (UINT8*)(PICTURE_BASE_ADDR + PICTURE_STORESIZE * iPicID);
}

//////////////////////////////////////////////////
//
//  装入画板
//
//	参数：	iPicID		画板的序号，0 <= 此序号 < PICTURE_MAX_COUNT
//			pBuf		装入 buffer，保证此 buffer size >= PICTURE_STORESIZE
//
//  返回：	装入成功标记
//
//////////////////////////////////////////////////
bool LoadPicture(short iPicID, void *pBuf)
{
	UINT8	*p = GetPictureAddr(iPicID);
	
	if (!p)
		return false;
	for (int i = 0; i < PICTURE_STORESIZE; i ++)
		*((UINT16*)pBuf + i) = g_iOneBit2TwoBitMap[*(p + i)];
	return true;
}

//////////////////////////////////////////////////
//
//  保存画板
//
//	参数：	iPicID		画板的序号，0 <= 此序号 < PICTURE_MAX_COUNT
//			pBuf		待保存的画板数据，2bit格式
//
//  返回：	保存成功标记
//
//////////////////////////////////////////////////
bool SavePicture(short iPicID, void *pBuf)
{
	UINT8	*p; 
	int		i;//, j = 0;
	
	p = GetPictureAddr(iPicID);
	if (!p)
		return false;
	//for (long addr = (long)p; addr < (long)p + PICTURE_STORESIZE; addr += FLASH_SECTOR_SIZE, j ++){
		for (i = 0; i < FLASH_SECTOR_SIZE; i ++){
			*((UINT8*)(FLASH_BUF_ADDR + i)) = g_iTwoBit2OneBitHeight[*((UINT8*)pBuf + (i * 2))] | \
											  g_iTwoBit2OneBitLow[*((UINT8*)pBuf + (i * 2 + 1))];
		}
		FlashEraseSector((UINT16*)p);
		FlashWrite((UINT16*)p, (UINT16*)FLASH_BUF_ADDR, FLASH_SECTOR_SIZE / 2);
		//FlashWrite((UINT16*)p, (UINT16*)FLASH_BUF_ADDR, 3000 / 2);
//	}
	return true;
}

/*************************************************************************/

/* 名片驱动 */

/* 定位名片 */
PRIVATE CARD* LocateCard(short iCardID)
{
	return (CARD*)(CARD_BASE_ADDR + sizeof(CARD) * iCardID);
}

/*           hqf                     */
void hwmemset(void *pointer , int c , unsigned int count)
{
	int i;
	UINT8 *pstart = (UINT8 *) pointer;
	for(i = 0; i < count;i++, pstart++)
		*pstart = (UINT8) c;
}
/*           hqf                     */

void *memcpy( void *dest, const void *src, unsigned int count )
{
	register int i;
	register UINT8 *psrc = (UINT8 *) src;
	register UINT8 *pdest = (UINT8 *)dest;

	for(i = 0; i < count;i++, psrc++, pdest++)
		*pdest = *psrc;
	return dest;
}

void  * memset (void * s, int  c, unsigned int n)
{
	register unsigned char  val = (unsigned char)  c;
	register unsigned char *ptr = (unsigned char*) s;
	register unsigned char *end = ptr + n;
    while ( ptr < end )  *ptr++ = val;
    return ( s );
}
/* 擦除一条名片 */ 
PRIVATE void EraseOneCard(short iCardID)
{
	CARD	*card;

	card = LocateCard(iCardID);
	LoadFlashSector(card);
	hwmemset((void*)(FLASH_BUF_ADDR + (long)card % FLASH_SECTOR_SIZE), 0xFF, sizeof(CARD));
	SaveFlashSector(card);
}

/* 写一条名片进 Flash */
PRIVATE void WriteOneCard(short iCardID, CARD const *pCard)
{
	CARD		card;

	memcpy(&card, pCard, sizeof(CARD));
	card.Flag = card.Flag & (~CARD_ERASED) | CARD_EXIST;
	FlashWrite((UINT16*)LocateCard(iCardID), (UINT16*)(&card), sizeof(CARD) / 2);
}

/* 判断名片合法 */
PRIVATE CARD* LegalCard(short iCardID)
{
	CARD*	pcard;

	if ((iCardID >= CARD_MAX_COUNT) || (iCardID < 0))
		return NULL;
	pcard = LocateCard(iCardID);
	if (pcard->Flag & CARD_ERASED)
		return NULL;
	if (!(pcard->Flag & CARD_EXIST))
		return NULL;
	return pcard;
}

/* 查找名片比较函数 */
PRIVATE int CardCompare(void const *key, void const *element)
{
	CARD	*pkey, *pelement;

	pkey = LocateCard(*(short*)key);
	pelement = LocateCard(*(short*)element);
	return strcmp(pkey->Name, pelement->Name);
}

bool IsLegalSubString(char *pString , char *pSubString)
{
	if(!pString || !pSubString || !pString[0] || !pSubString[0])
		return false;
	char *pCurrent = strstr(pString, pSubString);
	if(!pCurrent)
		return false;

	char *pPrev = CharPre(pString , pCurrent);
	if((pCurrent[0] & 0x80) && (pPrev[0] & 0x80)
		&& ((pCurrent - pPrev) & 0x01)) 
		return false;
	return true;
}

/* 在名片中查找字符串 */
PRIVATE bool FindOneCardString(char const *pString, short iCardID, int iFindScope)
{
	CARD*		pcard;
	bool		flag = false;
	char		*p;

	if (!(pcard = LegalCard(iCardID)))
		return false;
	if (iFindScope == CARD_FINDALL){
		flag = IsLegalSubString(pcard->Name, (char *)pString);
		if(!flag)
		{
			p = pcard->Content;
			for (int i = 0; i < CARD_CONTENT_SEGMENT; i ++){
				if (IsLegalSubString(p, (char *)pString))
					flag = true;
				p += strlen(p) + 1;
			}
		}
	}
	else if(iFindScope == CARD_FINDHANZINAME)
	{
		if (IsLegalSubString(pcard->Name, (char *)pString))
			flag = true;
	}
	else if(iFindScope == CARD_FINDABBRNAME)
	{
		char strAbbr[CARD_NAME_LEN+1];
		extern int GetAbbreviation(const char * hanzi, char * pinyin);
		GetAbbreviation(pcard->Name , strAbbr);
		if(strstr(strAbbr , pString))
			flag = true;
	}
	return flag;
}

//////////////////////////////////////////////////
//
//  初始化名片
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitCard(void)
{
	extern CARD g_cloneCard;
	memset(&g_cloneCard , 0 , sizeof(CARD));//modified by hqf
	//for (long addr = CARD_BASE_ADDR; addr < CARD_BASE_ADDR + CARD_MAX_COUNT * sizeof(CARD); addr += FLASH_SECTOR_SIZE)
	//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  新增名片
//
//	参数：	pCard		新增名片内容
//
//  返回：	新增名片 ID，ID >= 0, 返回 -1 表示失败
//
//////////////////////////////////////////////////
short AppendCard(CARD *pCard)
{
	CARD*		pcard;
	short		i, j = -1;
	for (i = 0; i < CARD_MAX_COUNT; i ++){
		pcard = LocateCard(i);
		if (pcard->Flag & CARD_ERASED)
			break;
		if (!(pcard->Flag & CARD_EXIST))
			j = i;
	}
	if ((i == CARD_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneCard(i);
	}
	if (i == CARD_MAX_COUNT)
		return -1;
	WriteOneCard(i, pCard);
	return i;
}

//////////////////////////////////////////////////
//
//  删除名片
//
//	参数：	iCardID		删除名片的 ID
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteCard(short iCardID)
{
	CARD*	pcard;

	if (!(pcard = LegalCard(iCardID)))
		return false;
	return FlashWriteWord(&(pcard->Flag), pcard->Flag & ~CARD_EXIST);
}

//////////////////////////////////////////////////
//
//  修改名片
//
//	参数：	iCardID		修改名片的 ID
//			pCard		修改内容
//
//  返回：	修改成功标记
//
//////////////////////////////////////////////////
bool ModifyCard(short iCardID, CARD const *pCard)
{
	CARD*	pcard;
	
	if (!(pcard = LegalCard(iCardID)))
		return false;
	EraseOneCard(iCardID);
	WriteOneCard(iCardID, pCard);
	return true;
}
	
//////////////////////////////////////////////////
//
//  查找名片
//
//	参数：	iCardCaseID		名片夹 ID
//			pCardIDBuf		存放查找结果的 buffer
//			iBufLen			buffer 长度
//
//  返回：	查到的个数
//
//////////////////////////////////////////////////
int FindCard(short iCardCaseID, short *pCardIDBuf, int iBufLen)
{
	CARD*		pcard;
	int			count = 0;
	
	for (short i = 0; (i < CARD_MAX_COUNT) && (count < iBufLen); i ++){
		if ((pcard = LegalCard(i)) != NULL){
			if (pcard->CardCaseID == iCardCaseID){
				*(pCardIDBuf + count) = i;
				count ++;
			}
		}
	}
	qsort(pCardIDBuf, count, 2, CardCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  查找字符串
//
//	参数：	pString		待查字符串
//			pCardIDBuf	存放查找结果的 buffer
//			iBufLen		buffer 长度
//			iFindScope	查找范围，取值
//							CARD_FINDNAME		只查找名字字段
//							CARD_FINDALL		查找所有字段
//  返回：	查到的个数
//
//////////////////////////////////////////////////
int SearchCardString(char const *pString, short *pCardIDBuf, int iBufLen, int iFindScope)
{
	CARD*		pcard;
	int		count = 0;

	for (short i = 0; (i < CARD_MAX_COUNT) && (count < iBufLen); i ++){
		if ((pcard = LegalCard(i)) != NULL){
			if (FindOneCardString(pString, i, iFindScope)){
				*(pCardIDBuf + count) = i;
				count ++;
			}
		}
	}
	return count;
}

//////////////////////////////////////////////////
//
//  查找字符串
//
//	参数：	pString		待查字符串
//			pCardIDBuf	存放待查名片 ID 的 buffer（调用前）
//						存放查找结果的 buffer（调用后）
//			iBufLen		buffer 长度
//			iFindScope	查找范围，取值
//							CARD_FINDNAME		只查找名字字段
//							CARD_FINDALL		查找所有字段
//  返回：	查到的个数
//
//////////////////////////////////////////////////
int FindCardString(char const *pString, short *pCardIDBuf, int iBufLen, int iFindScope)
{
	int		i, j;

	if (*pString == '\0')
		return 0;
	for (i = j = 0; i < iBufLen; i ++){
		if (FindOneCardString(pString, *(pCardIDBuf + i), iFindScope)){
			*(pCardIDBuf + j) = *(pCardIDBuf + i);
			j ++;
		}
	}
	return j;
}

//////////////////////////////////////////////////
//
//  查找电话号码
//
//	参数：	pNumber			待查号码
//			pCardIDBuf		存放查找结果 ID 的 buffer
//			pNumberTypeBuf	存放电话类型的 buffer
//			iBufLen			上述两个 buffer 的长度(要求相等)
//
//  返回：	查到的个数
//
//////////////////////////////////////////////////
int FindCardNumber(char const *pNumber, short *pCardIDBuf, short *pNumberTypeBuf, short int iBufLen)
{
	CARD	*pcard;
	int		i, j, count = 0;
	char	*p;
	bool	flag; 
	
	for (i = 0; (i < CARD_MAX_COUNT) && (count < iBufLen); i ++){
		pcard = LocateCard(i);
		if (!(pcard->Flag & CARD_ERASED) && (pcard->Flag & CARD_EXIST)){
			flag = false;
			p = pcard->Content;
			for (j = 0; (j < CARD_CONTENT_SEGMENT) && (j <= CARD_NUMBEREND) && (!flag); j ++){
				if (j == CARD_INSIDE){
					if (pNumber && *pNumber && strstr(pNumber, p))	// byw
					{
						if (strcmp(pNumber, p) == 0)	// byw
						{
							flag = true; 
							break;
						}
					}
				}
				else if (j >= CARD_NUMBERSTART){
					//if (pNumber && *pNumber && strstr(p, pNumber)) // modified by hqf || ((*p !=0) && strstr(pNumber, p)))  //modify by wangfei
					if (pNumber && *pNumber && strstr(pNumber, p))	// byw
					{
						if (strcmp(strstr(pNumber, p), p) == 0){	// byw
							flag = true; 
							break;
						}
					}
				}
				p += strlen(p) + 1;
			}
			if (flag){
				*(pCardIDBuf + count) = i;
				*(pNumberTypeBuf + count) = j-CARD_NUMBERSTART;
				count ++;
			}
		}////// end if(!...)
	}
	return count;
}

//////////////////////////////////////////////////
//
//  获取名片指针
//
//	参数：	iCardID		名片的 ID
//
//  返回：	名片指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
CARD* GetCardPointer(short iCardID)
{
	return LegalCard(iCardID);
}


/*************************************************************************/

/* 名片夹 */

/* 定位名片夹 */
PRIVATE CARDCASE* LocateCardCase(short iCardCaseID)
{
	return (CARDCASE*)(CARDCASE_BASE_ADDR + sizeof(CARDCASE) * iCardCaseID);
}

/* 擦除名片夹 */
PRIVATE void EraseOneCardCase(short iCardCaseID)
{
	CARDCASE		*cardcase;

	cardcase = LocateCardCase(iCardCaseID);
	LoadFlashSector(cardcase);
	memset((void*)(FLASH_BUF_ADDR + (long)cardcase % FLASH_SECTOR_SIZE), 0xFF, sizeof(CARDCASE));
	SaveFlashSector(cardcase);
}

/* 写名片夹 */
PRIVATE void WriteOneCardCase(short iCardCaseID, CARDCASE const *pCardCase, short iPrivate)
{
	CARDCASE		cardcase;

	memcpy(&cardcase, pCardCase, sizeof(CARDCASE));
	cardcase.Private = iPrivate;
	cardcase.Flag = cardcase.Flag & (~CARDCASE_ERASED) | CARDCASE_EXIST;
	FlashWrite((UINT16*)LocateCardCase(iCardCaseID), (UINT16*)(&cardcase), sizeof(CARDCASE) / 2);
}

/* 判断名片夹合法性 */
PRIVATE CARDCASE* LegalCardCase(short iCardCaseID)
{
	CARDCASE*	pcardcase;

	if ((iCardCaseID >= CARDCASE_MAX_COUNT) || (iCardCaseID < 0))
		return NULL;
	pcardcase = LocateCardCase(iCardCaseID);
	if (pcardcase->Flag & CARDCASE_ERASED)
		return NULL;
	if (!(pcardcase->Flag & CARDCASE_EXIST))
		return NULL;
	return pcardcase;
}

/* 名片夹查找比较函数 */
PRIVATE int CardCaseCompare(void const *key, void const *element)
{
	CARDCASE	*pkey, *pelement;

	pkey = LocateCardCase(*(short*)key);
	pelement = LocateCardCase(*(short*)element);
	return pkey->Private - pelement->Private;
}

/* 获取新建名片夹索引号 */
PRIVATE short CardCaseNewIndex(void)
{
	short		buf[CARDCASE_MAX_COUNT], id = -1;
	int			count, i;

	count = FindCardCase(buf, CARDCASE_MAX_COUNT);
	for (i = 0; i < count; i ++){
		if (LocateCardCase(buf[i])->Private > id)
			id = LocateCardCase(buf[i])->Private;
	}
	return id + 1;
}

//////////////////////////////////////////////////
//
//  初始化名片夹
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitCardCase(void)
{
	extern short g_nCases;
	g_nCases = 0;
	//for (long addr = CARDCASE_BASE_ADDR; addr < CARDCASE_BASE_ADDR + CARDCASE_MAX_COUNT * sizeof(CARDCASE); addr += FLASH_SECTOR_SIZE)
	//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  新增名片夹
//
//	参数：	pCardCase	新增名片夹内容
//
//  返回：	新增 ID，ID >= 0, 返回 -1 表示失败
//
//////////////////////////////////////////////////
short AppendCardCase(CARDCASE *pCardCase)
{
	CARDCASE	*pcardcase;
	short		i, j = -1;

	for (i = 0; i < CARDCASE_MAX_COUNT; i ++){
		pcardcase = LocateCardCase(i);
		if (pcardcase->Flag & CARDCASE_ERASED)
			break;
		if (!(pcardcase->Flag & CARDCASE_EXIST))
			j = i;
	}
	if ((i == CARDCASE_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneCardCase(i);
	}
	if (i == CARDCASE_MAX_COUNT)
		return -1;
	WriteOneCardCase(i, pCardCase, CardCaseNewIndex());
	return i;
}

//////////////////////////////////////////////////
//
//  删除名片夹
//
//	参数：	iCardCaseID		待删除名片夹ID
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteCardCase(short iCardCaseID)
{
	CARDCASE*	pcardcase;

	if (!(pcardcase = LegalCardCase(iCardCaseID)))
		return false;
	return FlashWriteWord(&(pcardcase->Flag), pcardcase->Flag & ~CARDCASE_EXIST);
}
	
//////////////////////////////////////////////////
//
//  修改名片夹
//
//	参数：	iCardCaseID		待修改名片夹ID
//			pCardCase		修改内容
//
//  返回：	修改成功标记
//
//////////////////////////////////////////////////
bool ModifyCardCase(short iCardCaseID, CARDCASE const *pCardCase)
{
	CARDCASE	*pcardcase;
	short		iprivate;
	
	if (!(pcardcase = LegalCardCase(iCardCaseID)))
		return false;
	iprivate = pcardcase->Private;
	EraseOneCardCase(iCardCaseID);
	WriteOneCardCase(iCardCaseID, pCardCase, iprivate);
	return true;
}
	
//////////////////////////////////////////////////
//
//  查找名片夹
//
//	参数：	pCardCaseIDBuf		查找结果 buffer
//			iBufLen				buffer 长度
//
//  返回：	查到个数
//
//////////////////////////////////////////////////
int FindCardCase(short *pCardCaseIDBuf, int iBufLen)
{
	CARDCASE*	pcardcase;
	int			count = 0;
	
	for (short i = 0; (i < CARDCASE_MAX_COUNT) && (count < iBufLen); i ++){
		CLR_WATCHDOG();
		if ((pcardcase = LegalCardCase(i)) != NULL){
			*(pCardCaseIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pCardCaseIDBuf, count, 2, CardCaseCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  获取名片夹指针
//
//	参数：	iCardCaseID		名片夹 ID
//
//  返回：	名片夹指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
CARDCASE* GetCardCasePointer(short iCardCaseID)
{
	return LegalCardCase(iCardCaseID);
}	

//////////////////////////////////////////////////
//
//  名片夹排序
//
//	参数：	pCardCaseIDBuf		待排序名片夹 ID buffer
//			iCount				排序 ID 个数
//
//  返回：	排序成功标记
//
//////////////////////////////////////////////////
bool SortCardCase(short *pCardCaseIDBuf, int iCount)
{
	CARDCASE*	pcardcase;

	memcpy((void*)FLASH_BUF_ADDR, (void*)CARDCASE_BASE_ADDR, FLASH_SECTOR_SIZE);
	for (int i = 0; i < iCount; i ++){
		pcardcase = LegalCardCase(*(pCardCaseIDBuf + i));
		if (pcardcase)
			((CARDCASE*)((long)pcardcase - CARDCASE_BASE_ADDR + FLASH_BUF_ADDR))->Private = i;
		else
			return false;
	}
	FlashEraseSector((UINT16*)CARDCASE_BASE_ADDR);
	SaveFlashSector((void*)CARDCASE_BASE_ADDR);
	return true;
}


/*************************************************************************/

/* 提醒 */

/* 定位提醒 */
PRIVATE ALARM* LocateAlarm(short iAlarmID)
{
	return (ALARM*)(ALARM_BASE_ADDR + sizeof(ALARM) * iAlarmID);
}

/* 擦除某条提醒 */
PRIVATE void EraseOneAlarm(short iAlarmID)
{
	ALARM		*alarm;

	alarm = LocateAlarm(iAlarmID);
	LoadFlashSector(alarm);
	memset((void*)(FLASH_BUF_ADDR + (long)alarm % FLASH_SECTOR_SIZE), 0xFF, sizeof(ALARM));
	SaveFlashSector(alarm);
}

/* 写某条提醒进 Flash */
PRIVATE void WriteOneAlarm(short iAlarmID, ALARM const *pAlarm)
{
	ALARM		alarm;

	memcpy(&alarm, pAlarm, sizeof(ALARM));
	alarm.Flag = alarm.Flag & (~ALARM_ERASED) | ALARM_EXIST;
	FlashWrite((UINT16*)LocateAlarm(iAlarmID), (UINT16*)(&alarm), sizeof(ALARM) / 2);
}

/* 判断提醒合法 */
PRIVATE ALARM* LegalAlarm(short iAlarmID)
{
	ALARM*	palarm;

	if ((iAlarmID >= ALARM_MAX_COUNT) || (iAlarmID < 0))
		return NULL;
	palarm = LocateAlarm(iAlarmID);
	if (palarm->Flag & ALARM_ERASED)
		return NULL;
	if (!(palarm->Flag & ALARM_EXIST))
		return NULL;
	return palarm;
}

/* 查找提醒比较函数 */
PRIVATE int AlarmCompare(void const *key, void const *element)
{
	ALARM	*pkey, *pelement;

	pkey = LocateAlarm(*(short*)key);
	pelement = LocateAlarm(*(short*)element);
	return strcmp(pkey->Title, pelement->Title);
}

//////////////////////////////////////////////////
//
//  初始化提醒
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitAlarmData(void)
{
//	for (long addr = ALARM_BASE_ADDR; addr < ALARM_BASE_ADDR + ALARM_MAX_COUNT * sizeof(ALARM); addr += FLASH_SECTOR_SIZE)
//		FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  新增提醒
//
//	参数：	pAlarm		新增提醒指针
//
//  返回：	新增 ID，ID >= 0, 返回 -1 表示失败
//
//////////////////////////////////////////////////
short AppendAlarm(ALARM *pAlarm)
{
	ALARM*		palarm;
	short		i, j = -1;

	for (i = 0; i < ALARM_MAX_COUNT; i ++){
		palarm = LocateAlarm(i);
		if (palarm->Flag & ALARM_ERASED)
			break;
		if (!(palarm->Flag & ALARM_EXIST))
			j = i;
	}
	if ((i == ALARM_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneAlarm(i);
	}
	if (i == ALARM_MAX_COUNT)
		return -1;
	WriteOneAlarm(i, pAlarm);
	return i;
}


//////////////////////////////////////////////////
//
//  删除提醒
//
//	参数：	iAlarmID		删除提醒的 ID
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteAlarm(short iAlarmID)
{
	ALARM*	palarm;

	if (!(palarm = LegalAlarm(iAlarmID)))
		return false;
	return FlashWriteWord(&(palarm->Flag), palarm->Flag & ~ALARM_EXIST);
}
	
//////////////////////////////////////////////////
//
//  修改提醒
//
//	参数：	iAlarmID	修改提醒的ID
//			pAlarm		修改内容
//
//  返回：	修改成功标记
//
//////////////////////////////////////////////////
bool ModifyAlarm(short iAlarmID, ALARM const *pAlarm)
{
	ALARM*	palarm;
	
	if (!(palarm = LegalAlarm(iAlarmID)))
		return false;
	EraseOneAlarm(iAlarmID);
	WriteOneAlarm(iAlarmID, pAlarm);
	return true;
}
	
//////////////////////////////////////////////////
//
//  查找提醒
//
//	参数：	pAlarmIDBuf	存放查找结果的 buffer
//			iBufLen		buffer 长度
//
//  返回：	查到的个数
//
//////////////////////////////////////////////////
int FindAlarm(short *pAlarmIDBuf, int iBufLen)
{
	ALARM*		palarm;
	int			count = 0;
	
	for (short i = 0; (i < ALARM_MAX_COUNT) && (count < iBufLen); i ++){
		if ((palarm = LegalAlarm(i)) != NULL){
			*(pAlarmIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pAlarmIDBuf, count, 2, AlarmCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  获取提醒指针
//
//	参数：	iAlarmID		提醒的 ID
//
//  返回：	提醒的指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
ALARM* GetAlarmPointer(short iAlarmID)
{
	return LegalAlarm(iAlarmID);
}	

/*************************************************************************/

/* 铃声 */

/* 定位铃声 */
PRIVATE RING* LocateRing(short iRingID)
{
	return (RING*)(RING_BASE_ADDR + sizeof(RING) * iRingID);
}

/* 擦除一条铃声 */
PRIVATE void EraseOneRing(short iRingID)
{
	RING		*ring;

	ring = LocateRing(iRingID);
	LoadFlashSector(ring);
	memset((void*)(FLASH_BUF_ADDR + (long)ring % FLASH_SECTOR_SIZE), 0xFF, sizeof(RING));
	SaveFlashSector(ring);
}

/* 写一条铃声进 Flash */
PRIVATE void WriteOneRing(short iRingID, RING const *pRing)
{
	RING		ring;

	memcpy(&ring, pRing, sizeof(RING));
	ring.Flag = ring.Flag & (~RING_ERASED) | RING_EXIST;
	FlashWrite((UINT16*)LocateRing(iRingID), (UINT16*)(&ring), sizeof(RING) / 2);
}

/* 判断铃声合法 */
PRIVATE RING* LegalRing(short iRingID)
{
	RING*	pring;

	if ((iRingID >= RING_MAX_COUNT) || (iRingID < 0))
		return NULL;
	pring = LocateRing(iRingID);
	if (pring->Flag & RING_ERASED)
		return NULL;
	if (!(pring->Flag & RING_EXIST))
		return NULL;
	return pring;
}

/* 查找铃声比较函数 */
PRIVATE int RingCompare(void const *key, void const *element)
{
	RING	*pkey, *pelement;

	pkey = LocateRing(*(short*)key);
	pelement = LocateRing(*(short*)element);
	return strcmp(pkey->Name, pelement->Name);
}

//////////////////////////////////////////////////
//
//  初始化铃声
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitRing(void)
{
//	for (long addr = RING_BASE_ADDR; addr < RING_BASE_ADDR + RING_MAX_COUNT * sizeof(RING); addr += FLASH_SECTOR_SIZE)
//		FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  新增铃声
//
//	参数：	pRing		新增铃声内容
//
//  返回：	新增铃声 ID，ID >= 0, 返回 -1 表示失败
//
//////////////////////////////////////////////////
short AppendRing(RING *pRing)
{
	RING*		pring;
	short		i, j = -1;

	for (i = 0; i < RING_MAX_COUNT; i ++){
		pring = LocateRing(i);
		if (pring->Flag & RING_ERASED)
			break;
		if (!(pring->Flag & RING_EXIST))
			j = i;
	}
	if ((i == RING_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneRing(i);
	}
	if (i == RING_MAX_COUNT)
		return -1;
	WriteOneRing(i, pRing);
	return i;
}

//////////////////////////////////////////////////
//
//  删除铃声
//
//	参数：	iRingID		删除铃声的 ID
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteRing(short iRingID)
{
	RING*	pring;
	int		count;
	short	buf[CARDCASE_MAX_COUNT];

	if (!(pring = LegalRing(iRingID)))
		return false;
	if (pring->Flag & RING_SYSTEM)
		return false;
	count = FindCardCase(buf, CARDCASE_MAX_COUNT);
	for (int i = 0; i < count; i ++){
		if (GetCardCasePointer(buf[i])->RingID == iRingID)
			return false;
	}
	return FlashWriteWord(&(pring->Flag), pring->Flag & ~RING_EXIST);
}

//////////////////////////////////////////////////
//
//  修改铃声
//
//	参数：	iRingID		修改铃声的 ID
//			pRing		修改内容
//
//  返回：	修改成功标记
//
//////////////////////////////////////////////////
bool ModifyRing(short iRingID, RING const *pRing)
{
	RING*	pring;
	
	if (!(pring = LegalRing(iRingID)))
		return false;
	EraseOneRing(iRingID);
	WriteOneRing(iRingID, pRing);
	return true;
}
	
//////////////////////////////////////////////////
//
//  查找铃声
//
//	参数：	pRingIDBuf		存放查找结果的 buffer
//			iBufLen			buffer 长度
//
//  返回：	查到的个数
//
//////////////////////////////////////////////////
int FindRing(short *pRingIDBuf, int iBufLen)
{
	RING*		pring;
	int			count = 0;
	
	for (short i = 0; (i < RING_MAX_COUNT) && (count < iBufLen); i ++){
		if ((pring = LegalRing(i)) != NULL){
			*(pRingIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pRingIDBuf, count, 2, RingCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  获取铃声指针
//
//	参数：	iRingID		铃声的 ID
//
//  返回：	铃声指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
RING* GetRingPointer(short iRingID)
{
	return LegalRing(iRingID);
}

//////////////////////////////////////////////////
//
//  查询铃声使用者
//
//	参数：	iRingID		振铃ID
//			pUserType	接收使用者类型(DATATYPE类型)
//			pUserID		接收使用者ID
//
//  返回：	ture		正在使用，详细情况在 pUserType, pUserID 中
//			flase		没有被使用
//
//////////////////////////////////////////////////
bool RingInUse(short iRingID, short *pUserType, short *pUserID)
{
	CARD*		pcard;
	CARDCASE*	pcardcase;
	ALARM*		palarm;
	short		i, id, type;
	
	// 查询名片振铃
	for (i = 0; i < CARD_MAX_COUNT; i ++){
		if ((pcard = LegalCard(i)) != NULL){
			if (pcard->RingID == iRingID){
				*pUserType = DATA_CARD;
				*pUserID = i;
				return true;
			}
		}
	}

	// 查询名片夹振铃
	for (i = 0; i < CARDCASE_MAX_COUNT; i ++){
		if ((pcardcase = LegalCardCase(i)) != NULL){
			if (pcardcase->RingID == iRingID){
				*pUserType = DATA_CARDCASE;
				*pUserID = i;
				return true;
			}
		}
	}

	// 查询提醒振铃
	for (i = 0; i < ALARM_MAX_COUNT; i ++){
		if ((palarm = LegalAlarm(i)) != NULL){
			if (palarm->Music == iRingID){
				*pUserType = DATA_ALARM;  // hqf 2-1
				*pUserID = i;
				return true;
			}
		}
	}

	return false;
}


/*************************************************************************/

/* 速拨 */

/* 定位速拨 */
PRIVATE FASTDIAL* LocateFastDial(short iFastDialID)
{
	return (FASTDIAL*)(FASTDIAL_BASE_ADDR + sizeof(FASTDIAL) * iFastDialID);
}

/* 擦除速拨 */
PRIVATE void EraseOneFastDial(short iFastDialID)
{
	FASTDIAL		*fastdial;

	fastdial = LocateFastDial(iFastDialID);
	LoadFlashSector(fastdial);
	memset((void*)(FLASH_BUF_ADDR + (long)fastdial % FLASH_SECTOR_SIZE), 0xFF, sizeof(FASTDIAL));
	SaveFlashSector(fastdial);
}

/* 写速拨 */
PRIVATE void WriteOneFastDial(short iFastDialID, FASTDIAL const *pFastDial, short iSort)
{
	FASTDIAL		fastdial;

	memcpy(&fastdial, pFastDial, sizeof(FASTDIAL));
	fastdial.Sort = iSort;
	fastdial.Flag = fastdial.Flag & (~FASTDIAL_ERASED) | FASTDIAL_EXIST;
	FlashWrite((UINT16*)LocateFastDial(iFastDialID), (UINT16*)(&fastdial), sizeof(FASTDIAL) / 2);
}

/* 判断速拨合法性 */
PRIVATE FASTDIAL* LegalFastDial(short iFastDialID)
{
	FASTDIAL*	pfastdial;

	if ((iFastDialID >= FASTDIAL_MAX_COUNT) || (iFastDialID < 0))
		return NULL;
	pfastdial = LocateFastDial(iFastDialID);
	if (pfastdial->Flag & FASTDIAL_ERASED)
		return NULL;
	if (!(pfastdial->Flag & FASTDIAL_EXIST))
		return NULL;
	return pfastdial;
}

/* 速拨查找比较函数 */
PRIVATE int FastDialCompare(void const *key, void const *element)
{
	FASTDIAL	*pkey, *pelement;

	pkey = LocateFastDial(*(short*)key);
	pelement = LocateFastDial(*(short*)element);
	return pkey->Sort - pelement->Sort;
}

/* 获取新建速拨索引号 */
PRIVATE short FastDialNewIndex(void)
{
	short		buf[FASTDIAL_MAX_COUNT], id = -1;
	int			count, i;

	count = FindFastDial(buf, FASTDIAL_MAX_COUNT);
	for (i = 0; i < count; i ++){
		if (LocateFastDial(buf[i])->Sort > id)
			id = LocateFastDial(buf[i])->Sort;
	}
	return id + 1;
}

//////////////////////////////////////////////////
//
//  初始化速拨
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitFastDial(void)
{
//	for (long addr = FASTDIAL_BASE_ADDR; addr < FASTDIAL_BASE_ADDR + FASTDIAL_MAX_COUNT * sizeof(FASTDIAL); addr += FLASH_SECTOR_SIZE)
//		FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  新增速拨
//
//	参数：	pFastDial	新增速拨内容
//
//  返回：	新增 ID，ID >= 0, 返回 -1 表示失败
//
//////////////////////////////////////////////////
short AppendFastDial(FASTDIAL *pFastDial)
{
	FASTDIAL	*pfastdial;
	short		i, j = -1;

	for (i = 0; i < FASTDIAL_MAX_COUNT; i ++){
		pfastdial = LocateFastDial(i);
		if (pfastdial->Flag & FASTDIAL_ERASED)
			break;
		if (!(pfastdial->Flag & FASTDIAL_EXIST))
			j = i;
	}
	if ((i == FASTDIAL_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneFastDial(i);
	}
	if (i == FASTDIAL_MAX_COUNT)
		return -1;
	WriteOneFastDial(i, pFastDial, FastDialNewIndex());
	return i;
}

//////////////////////////////////////////////////
//
//  删除速拨
//
//	参数：	iFastDialID		待删除速拨ID
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteFastDial(short iFastDialID)
{
	FASTDIAL*	pfastdial;

	if (!(pfastdial = LegalFastDial(iFastDialID)))
		return false;
	return FlashWriteWord(&(pfastdial->Flag), pfastdial->Flag & ~FASTDIAL_EXIST);
}
	
//////////////////////////////////////////////////
//
//  修改速拨
//
//	参数：	iFastDialID		待修改速拨ID
//			pFastDial		修改内容
//
//  返回：	修改成功标记
//
//////////////////////////////////////////////////
bool ModifyFastDial(short iFastDialID, FASTDIAL const *pFastDial)
{
	FASTDIAL	*pfastdial;
	short		iprivate;
	
	if (!(pfastdial = LegalFastDial(iFastDialID)))
		return false;
	iprivate = pfastdial->Sort;
	EraseOneFastDial(iFastDialID);
	WriteOneFastDial(iFastDialID, pFastDial, iprivate);
	return true;
}
	
//////////////////////////////////////////////////
//
//  查找速拨
//
//	参数：	pFastDialIDBuf		查找结果 buffer
//			iBufLen				buffer 长度
//
//  返回：	查到个数
//
//////////////////////////////////////////////////
int FindFastDial(short *pFastDialIDBuf, int iBufLen)
{
	FASTDIAL*	pfastdial;
	int			count = 0;
	
	for (short i = 0; (i < FASTDIAL_MAX_COUNT) && (count < iBufLen); i ++){
		if ((pfastdial = LegalFastDial(i)) != NULL){
			*(pFastDialIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pFastDialIDBuf, count, 2, FastDialCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  获取速拨指针
//
//	参数：	iFastDialID		速拨 ID
//
//  返回：	速拨指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
FASTDIAL* GetFastDialPointer(short iFastDialID)
{
	return LegalFastDial(iFastDialID);
}	

//////////////////////////////////////////////////
//
//  速拨排序
//
//	参数：	pFastDialIDBuf		待排序速拨 ID buffer
//			iCount				排序 ID 个数
//
//  返回：	排序成功标记
//
//////////////////////////////////////////////////
bool SortFastDial(short *pFastDialIDBuf, int iCount)
{
	FASTDIAL*	pfastdial;

	memcpy((void*)FLASH_BUF_ADDR, (void*)FASTDIAL_BASE_ADDR, FLASH_SECTOR_SIZE);
	for (int i = 0; i < iCount; i ++){
		pfastdial = LegalFastDial(*(pFastDialIDBuf + i));
		if (pfastdial)
			((FASTDIAL*)((long)pfastdial - FASTDIAL_BASE_ADDR + FLASH_BUF_ADDR))->Sort = i;
		else
			return false;
	}
	FlashEraseSector((UINT16*)FASTDIAL_BASE_ADDR);
	SaveFlashSector((void*)FASTDIAL_BASE_ADDR);
	return true;
}


/*************************************************************************/

/* 使用空间 */

//////////////////////////////////////////////////
//
//  获取录音已使用空间
//
//	参数：	无
//
//  返回：	录音已使用空间，单位：字节
//			录音空间总量 WAVE_STORE_LEN
//
//////////////////////////////////////////////////
long GetWaveUsed(void)
{
	return ((long)(*pWaveTail) - (long)pWaveHead);
}

//////////////////////////////////////////////////
//
//  获取已有通话记录条数
//
//	参数：	无
//
//  返回：	已有通话记录条数
//
//////////////////////////////////////////////////
int GetHisrotyUsed(void)
{
	short	buf[HISTORY_MAX_COUNT];
	return FindHistory(HISTORY_ALL, buf, HISTORY_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  获取已有记事条数
//
//	参数：	无
//
//  返回：	已有记事条数
//
//////////////////////////////////////////////////
int GetMemoUsed(void)
{
	short	buf[MEMO_MAX_COUNT];
	return FindMemo(buf, MEMO_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  获取已有画板条数
//
//	参数：	无
//
//  返回：	已有画板条数
//
//////////////////////////////////////////////////
int GetPictureUsed(void)
{
	return PICTURE_MAX_COUNT;
}

//////////////////////////////////////////////////
//
//  获取已有名片条数
//
//	参数：	无
//
//  返回：	已有名片条数
//
//////////////////////////////////////////////////
int GetCardUsed(void)
{
	int			count = 0;
	
	for (short i = 0; i < CARD_MAX_COUNT; i ++){
		if (LegalCard(i) != NULL)
				count ++;
	}
	return count;
}

//////////////////////////////////////////////////
//
//  获取已有名片夹条数
//
//	参数：	无
//
//  返回：	已有名片夹条数
//
//////////////////////////////////////////////////
int GetCardCaseUsed(void)
{
	short	buf[CARDCASE_MAX_COUNT];
	return FindCardCase(buf, CARDCASE_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  获取已有提醒条数
//
//	参数：	无
//
//  返回：	已有提醒条数
//
//////////////////////////////////////////////////
int GetAlarmUsed(void)
{
	short	buf[ALARM_MAX_COUNT];
	return FindAlarm(buf, ALARM_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  获取已有振铃条数
//
//	参数：	无
//
//  返回：	已有振铃条数
//
//////////////////////////////////////////////////
int GetRingUsed(void)
{
	short	buf[RING_MAX_COUNT];
	return FindRing(buf, RING_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  获取已有振铃条数
//
//	参数：	无
//
//  返回：	已有振铃条数
//
//////////////////////////////////////////////////
int GetSmsUsed(void)
{
	short	buf[SMS_MAX_COUNT];
	return FindSMS(buf, SMS_MAX_COUNT);
}


//系统设置的数据接口定义（临时）
void InitSystemOption()
{
	CSystemSetup::ResetSystemOption();
}

/*************************************************************************/

/* 系统设置驱动 */

// temp, debug
//extern void tempecho(int num);

void LoadSystemOption(SYSTEM_OPTION *option)
{
	memcpy(option, (void*)SETTINGS_BASE_ADDR, sizeof(SYSTEM_OPTION));
}

void SaveSystemOption(SYSTEM_OPTION *option)
{
	int		temp;

	temp = sizeof(SYSTEM_OPTION);
	LoadFlashSector((void*)SETTINGS_BASE_ADDR);
	memcpy((void*)FLASH_BUF_ADDR, option, sizeof(SYSTEM_OPTION));
	FlashEraseSector((UINT16*)SETTINGS_BASE_ADDR);
	FlashWrite((UINT16*)(SETTINGS_BASE_ADDR), (UINT16*)FLASH_BUF_ADDR, FLASH_SECTOR_SIZE / 2);
}

/*
void InitPhoneOption()
{
	extern PHONE_OPTION g_PhoneOption;
	memset(&g_PhoneOption , 0 , sizeof(PHONE_OPTION));
	g_PhoneOption.nRecordRingCount = 3; //默认
	for(INT16 i =0; i< DIALRULE_COUNT; i++)
	{
		strcpy(g_PhoneOption.iiDialNum[i].strPostfix0 , ",1,");
		strcpy(g_PhoneOption.iiDialNum[i].strPostfix1 , "#,");
		strcpy(g_PhoneOption.iiDialNum[i].strPostfix2 , "#,");
		strcpy(g_PhoneOption.iiDialNum[i].strPostfix3 , "#");
	}
	g_PhoneOption.nDefaultIP = 0;
}
*/
/*
void GetSystemOption(SYSTEM_OPTION *sys_option)
{
	memset(sys_option , 0 ,sizeof(SYSTEM_OPTION));
	strcpy(sys_option->Version , "hanwang .tec.");
}
BOOL SetSystemOption(SYSTEM_OPTION *sys_option)
{
	return FALSE;
}

void GetPhoneOption(PHONE_OPTION *pho_option)
{
	memset(pho_option , 0 ,sizeof(PHONE_OPTION));
}

BOOL SetPhoneOption(PHONE_OPTION *pho_option)
{
	return FALSE;
}

void GetFastDial(FASTDIAL *fd_option)
{
	memset(fd_option , 0 , sizeof(FASTDIAL));
}

BOOL SetFastDial(FASTDIAL *fd_option)
{
	return FALSE;
}
*/

/*************************************************************************/
/* 短消息驱动 */

/* 定位短消息 */
PRIVATE SMS* LocateSms(short iSmsID)
{
	return (SMS*)(SMS_BASE_ADDR + sizeof(SMS) * iSmsID);
}

/* 擦除一条短消息 */
PRIVATE void EraseOneSms(short iSmsID)
{
	SMS		*sms;

	sms = LocateSms(iSmsID);
	LoadFlashSector(sms);
	memset((void*)(FLASH_BUF_ADDR + (long)sms % FLASH_SECTOR_SIZE), 0xFF, sizeof(SMS));
	SaveFlashSector(sms);
}

/* 写一条短消息进 Flash */
PRIVATE void WriteOneSms(short iSmsID, SMS const *pSms)
{
	SMS		sms;

	memcpy(&sms, pSms, sizeof(SMS));
	sms.Flag = sms.Flag & (~SMS_ERASED) | SMS_EXIST;
	FlashWrite((UINT16*)LocateSms(iSmsID), (UINT16*)(&sms), sizeof(SMS) / 2);
}

/* 判断短消息合法 */
PRIVATE SMS* LegalSms(short iSmsID)
{
	SMS*	psms;

	if ((iSmsID >= SMS_MAX_COUNT) || (iSmsID < 0))
		return NULL;
	psms = LocateSms(iSmsID);
	if (psms->Flag & SMS_ERASED)
		return NULL;
	if (!(psms->Flag & SMS_EXIST))
		return NULL;
	return psms;
}

/* 查找短消息比较函数 */
PRIVATE int SmsCompare(void const *key, void const *element)
{
	SMS	*pkey, *pelement;

	pkey = LocateSms(*(short*)key);
	pelement = LocateSms(*(short*)element);
	// TODO: sort by date
	return strcmp(pkey->szTime, pelement->szTime);
}

//////////////////////////////////////////////////
//
//  初始化短消息
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void InitSMS()
{
//	for (long addr = SMS_BASE_ADDR;
//			addr < SMS_BASE_ADDR + SMS_MAX_COUNT * sizeof(SMS); 
//						addr += FLASH_SECTOR_SIZE)
//		FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  新增短消息
//
//	参数：	pSms		新增短消息内容
//
//  返回：	新增短消息 ID，ID >= 0, 返回 -1 表示失败
//
//////////////////////////////////////////////////
short AppendSMS(SMS *pSms)
{
	SMS*		psms;
	short		i, j = -1;

	for (i = 0; i < SMS_MAX_COUNT; i ++){
		psms = LocateSms(i);
		if (psms->Flag & SMS_ERASED)
			break;
		if (!(psms->Flag & SMS_EXIST))
			j = i;
	}
	if ((i == SMS_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneSms(i);
	}
	if (i == SMS_MAX_COUNT)
		return -1;
	WriteOneSms(i, pSms);
	return i;
}

//////////////////////////////////////////////////
//
//  删除短消息
//
//	参数：	iSmsID		删除短消息的 ID
//
//  返回：	删除成功标记
//
//////////////////////////////////////////////////
bool DeleteSMS(short iSmsID)
{
	SMS*	psms;

	if (!(psms = LegalSms(iSmsID)))
		return false;
	return FlashWriteWord(&(psms->Flag), psms->Flag & ~SMS_EXIST);
}

//////////////////////////////////////////////////
//
//  修改短消息
//
//	参数：	iSmsID		修改短消息的 ID
//			pSms		修改内容
//
//  返回：	修改成功标记
//
//////////////////////////////////////////////////
bool ModifySMS(short iSmsID, SMS const *pSms)
{
	SMS*	psms;
	
	if (!(psms = LegalSms(iSmsID)))
		return false;
	EraseOneSms(iSmsID);
	WriteOneSms(iSmsID, pSms);
	return true;
}
	
//////////////////////////////////////////////////
//
//  查找短消息
//
//	参数：	iSmsCaseID		短消息夹 ID
//			pSmsIDBuf		存放查找结果的 buffer
//			iBufLen			buffer 长度
//
//  返回：	查到的个数
//
//////////////////////////////////////////////////
int FindSMS(short *pSmsIDBuf, int iBufLen)
{
	SMS*		psms;
	int			count = 0;
	
	for (short i = 0; (i < SMS_MAX_COUNT) && (count < iBufLen); i ++){
		if ((psms = LegalSms(i)) != NULL){
			*(pSmsIDBuf + count) = i;
			count ++;
		}
	}
	//qsort(pSmsIDBuf, count, 2, SmsCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  获取短消息指针
//
//	参数：	iSmsID		短消息的 ID
//
//  返回：	短消息指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
SMS* GetSMSPointer(short iSmsID)
{
	return LegalSms(iSmsID);
}

//************************  集锦组驱动  wangyu***********************

// 定位集锦组  
PRIVATE MODEL* LocateModel(short iModelID)
{
	return (MODEL*)(MODEL_BASE_ADDR + sizeof(MODEL) * iModelID);
} 

// 擦除集锦组  
PRIVATE void EraseOneModel(short iModelID)
{ 
    MODEL         *model;

	model = LocateModel(iModelID);
	LoadFlashSector(model);
	memset((void*)(FLASH_BUF_ADDR + (long)model % FLASH_SECTOR_SIZE), 0xFF, sizeof(MODEL));
	SaveFlashSector(model);
}

//写集锦组  
PRIVATE void WriteOneModel(short iModelID, MODEL const *pModel, short iModelItem)
{
    MODEL          model;
	memcpy(&model, pModel, sizeof(MODEL));

	model.iModelItem = iModelItem; //集锦排序

	model.Flag = model.Flag & (~MODEL_ERASED) | MODEL_EXIST;

	FlashWrite((UINT16*)LocateModel(iModelID),  (UINT16*)(&model),  sizeof(MODEL) / 2);
}

//判断集锦组合法性 //
PRIVATE MODEL* LegalModel(short iModelID)
{
	MODEL*	pModel;

	if ((iModelID >= MAX_MODEL_NUMBER) || (iModelID < 0))
		return NULL;
	pModel = LocateModel(iModelID);
	if (pModel->Flag & MODEL_ERASED)
		return NULL;
	if (!(pModel->Flag & MODEL_EXIST))
		return NULL;
	return pModel;
}

//集锦组查找比较函数 ///
PRIVATE int ModelCompare(void const *key, void const *element)
{
	MODEL	*pkey, *pelement;

	pkey = LocateModel(*(short*)key);
	pelement = LocateModel(*(short*)element);
	return pkey->iModelItem - pelement->iModelItem;
}

//获取新建集锦组索引号  
PRIVATE short ModelNewIndex(void)
{
	short		buf[MAX_MODEL_NUMBER], id = -1;
	int			count, i;
//////   NONONONONONO
	count = FindMODEL(buf, MAX_MODEL_NUMBER);
	for (i = 0; i < count; i ++){
		if (LocateModel(buf[i])->iModelItem > id)
			id = LocateModel(buf[i])->iModelItem;
	}
	return id + 1;
}

////////////////////////////////////////////////////
//  初始化集锦组//    wangyu
//	参数：	无//
//  返回：	无//
////////////////////////////////////////////////// 
void   InitModel()
{
//	for (long addr =  MODEL_BASE_ADDR;
//			  addr <  MODEL_BASE_ADDR + MAX_MODEL_NUMBER * sizeof(MODEL); 
//			  addr += FLASH_SECTOR_SIZE )
//		FlashEraseSector((UINT16*)addr);
}

////////////////////////////////////////////////////
//  新增集锦组//
//	参数：	pMODEL	新增集锦组内容//
//  返回：	新增 ID，ID >= 0, 返回 -1 表示失败//
////////////////////////////////////////////////// 
short   AppendMODEL(MODEL *pMODEL) 	// 返回 ID(ID>=0)，返回 -1 表示失败
{     
	 MODEL *pmodel;
	short		i, j = -1;
	for (i = 0; i < MAX_MODEL_NUMBER; i ++){
		pmodel = LocateModel(i);
		if (pmodel->Flag & MODEL_ERASED)
			break;
		if (!(pmodel->Flag & MODEL_EXIST))
			j = i;
	}
	if ((i == MAX_MODEL_NUMBER) && (j != -1)){
		i = j;
		EraseOneModel(i);
	}
	if (i == MAX_MODEL_NUMBER)
		return -1;
    WriteOneModel(i, pMODEL, ModelNewIndex());
	return i;
}
  

////////////////////////////////////////////////////
//  删除集锦组//
//	参数：	iModelID		待删除集锦组ID//
//  返回：	删除成功标记//
//////////////////////////////////////////////////
 bool    DeleteMODEL(short iModelID) // 成功返回true, 失败返回false 
{
     MODEL*	pMODEL;
	if (!(pMODEL = LegalModel(iModelID)))
		return false;
	return FlashWriteWord(&(pMODEL->Flag), pMODEL->Flag & ~MODEL_EXIST);
}

////////////////////////////////////////////////////
//  修改集锦组//
//	参数：	iModelID		待修改集锦组ID
//			pModel   		修改内容//
//  返回：	修改成功标记//
////////////////////////////////////////////////// 
  bool    ModifyModel(short iModelID, MODEL const *pModel)
{
	 
	MODEL       *pmodel;
	short		iModelItem;
	
	if (!(pmodel = LegalModel(iModelID)))
		return false;
	iModelItem = pmodel->iModelItem;
	EraseOneModel(iModelID);
	WriteOneModel(iModelID, pModel, iModelItem);
	return true;
}
	
//////////////////////////////////////////////////
//  查找集锦组//
//	参数：	pModelIDBuf		查找结果 buffer
//			iBufLen				buffer 长度 
//  返回：	查到个数//
//////////////////////////////////////////////////       
  int  FindMODEL(short *pModelIDBuf, int iBufLen)// 返回查到个数
{
 
	MODEL*      model;
	int			count = 0;
	
	for (short i = 0; (i < MAX_MODEL_NUMBER) && (count < iBufLen); i ++){
		if ((model = LegalModel(i)) != NULL){
			*(pModelIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pModelIDBuf, count, 2, ModelCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  获取集锦组指针
//
//	参数：	iModelID		集锦组 ID
//
//  返回：	 返回 NULL 表示失败
//
//////////////////////////////////////////////////
MODEL* GetModelPointer(short iModelID)
{
	return LegalModel(iModelID);
}	
 //////////////////////////////////////////////////
//
//  集锦组排序
//
//	参数：	pModelIDBuf		待排序集锦组 ID buffer
//			iCount				排序 ID 个数
//
//  返回：	排序成功标记
//
//////////////////////////////////////////////////
bool SortModel(short *pModelIDBuf, int iCount)
{
	MODEL*	pmodel;

	memcpy((void*)FLASH_BUF_ADDR, (void*)MODEL_BASE_ADDR, FLASH_SECTOR_SIZE);
	for (int i = 0; i < iCount; i ++){
		pmodel = LegalModel(*(pModelIDBuf + i));
		if (pmodel)
			((MODEL*)((long)pmodel - MODEL_BASE_ADDR + FLASH_BUF_ADDR))->iModelItem = i;
		else
			return false;
	}
	FlashEraseSector((UINT16*)MODEL_BASE_ADDR);
	SaveFlashSector((void*)MODEL_BASE_ADDR);

	// temp, debug
	short	idbuf[20];
	int		count;
	count = FindMODEL(idbuf, 20);
	// debug end

	return true;
}
//***********************  集锦项驱动  wangyu********************************//

// 集锦项 定位  
PRIVATE CONTENT* LocateContent(short iContentID)
{
	return (CONTENT*)(CONTENT_BASE_ADDR + sizeof(CONTENT) * iContentID);
}

//擦除一条集锦项 
PRIVATE void EraseOneContent(short iContentID)
{
	CONTENT		*content;

	content = LocateContent(iContentID);
	LoadFlashSector(content);
	memset((void*)(FLASH_BUF_ADDR + (long)content % FLASH_SECTOR_SIZE), 0xFF, sizeof(CONTENT));
	SaveFlashSector(content);
}
// 写一条集锦项进 Flash  
PRIVATE void WriteOneContent(short iContentID, CONTENT const *pContent)
{	 
	CONTENT		content;

	memcpy(&content, pContent, sizeof(CONTENT));
	content.Flag = content.Flag & (~CONTENT_ERASED) | CONTENT_EXIST;
	FlashWrite((UINT16*)LocateContent(iContentID), (UINT16*)(&content), sizeof(CONTENT) / 2);
}
 
//判断集锦项合法  
PRIVATE CONTENT* LegalContent(short iContentID)
{
	CONTENT		* pcontent;
	
	if ((iContentID >= MAX_CONTENT_NUMBER) || (iContentID < 0))
		return NULL;
	pcontent = LocateContent(iContentID);
	if (pcontent->Flag & CONTENT_ERASED)
		return NULL;
	if (!(pcontent->Flag & CONTENT_EXIST))
		return NULL;
	return pcontent;
}

 //查找集锦项比较函数 
PRIVATE int ContentCompare(void const *key, void const *element)
{
	CONTENT	*pkey, *pelement;

	pkey = LocateContent(*(short*)key);
	pelement = LocateContent(*(short*)element);
	return strcmp(pkey->szContent, pelement->szContent);//比较内容
}
 
////////////////////////////////////////////////////
//  初始化集锦项//
//	参数：	无//
//  返回：	无//
//////////////////////////////////////////////////
void    InitContent()		// 格式化
{
//   for (long addr = CONTENT_BASE_ADDR; 
//             addr < CONTENT_BASE_ADDR + MAX_CONTENT_NUMBER * sizeof(CONTENT); 
//			 addr += FLASH_SECTOR_SIZE )
//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//  新增集锦项
//	参数：	pContent		新增集锦项内容
//  返回：	新增集锦项 ID，ID >= 0, 返回 -1 表示失败
//////////////////////////////////////////////////
short   AppendContent( CONTENT *pContent)	                       
{
 	CONTENT*    pcontent;
	short		i, j = -1;

	for (i = 0; i < MAX_CONTENT_NUMBER; i ++){
		pcontent = LocateContent(i);
		if (pcontent->Flag & CONTENT_ERASED)
			break;
		if (!(pcontent->Flag & CONTENT_EXIST))
			j = i;
	}
	if ((i == MAX_CONTENT_NUMBER) && (j != -1)){
		i = j;
		EraseOneContent(i);
	}
	if (i == MAX_CONTENT_NUMBER)
		return -1;
	WriteOneContent(i, pContent);
	return i;
}

////////////////////////////////////////////////////
//  删除集锦项 
//	参数：	iContentID		删除集锦项的 ID
//  返回：	删除成功标记
//////////////////////////////////////////////////
bool    DeleteContent( short iContentID)
{
    CONTENT*    pcontent;
	
	if (!(pcontent = LegalContent(iContentID)))
		return false;
	return FlashWriteWord(&(pcontent->Flag), pcontent->Flag & ~CONTENT_EXIST);
}

//////////////////////////////////////////////////
//  修改集锦项
//	参数：	iContentID		修改集锦项的 ID
//   		pContent		修改内容
//  返回：	修改成功标记
//////////////////////////////////////////////////
bool    ModifyContent( short iContentID, CONTENT const *pContent)
{
	CONTENT*    pcontent;
	if (!(pcontent = LegalContent(iContentID)))
		return false;
	EraseOneContent(iContentID);
	WriteOneContent(iContentID, pContent);
	return true;
}
	
//////////////////////////////////////////////////
//  查找某集锦中的集锦项
//	参数：	iModelID		集锦组 ID
//  		pContentIDBuf		存放查找结果的 buffer
//			iBufLen			buffer 长度
//  返回：	查到的个数
//////////////////////////////////////////////////
int     FindContent (short iModelID,short *pContentIDBuf, int iBufLen) 
{
 	CONTENT*    pcontent;
	int			count = 0;
	
	for (short i = 0; (i < MAX_CONTENT_NUMBER) && (count < iBufLen); i ++){
		if ((pcontent = LegalContent(i)) != NULL){
			if (pcontent->iModelID == iModelID){   ///所属组
				*(pContentIDBuf + count) = i;
				count ++;
			}
		}
	}
//	qsort(pCardIDBuf, count, 2, CardCompare);
	return count;
}
////////////////////
  ////  查找所有的的集锦项
  ////////
int     FindAllContent (short *pContentIDBuf, int iBufLen)
{
	CONTENT*    pcontent;
	int			count = 0;
	
	for (short i = 0; (i < MAX_CONTENT_NUMBER) && (count < iBufLen); i ++){
		if ((pcontent = LegalContent(i)) != NULL){
				*(pContentIDBuf + count) = i;
				count ++;	 
		}
	} 
	return count;
}
 
//////////////////////////////////////////////////
//
//  获取集锦项指针
//
//	参数：	iContentID		集锦项的 ID
//
//  返回：	名片指针，返回 NULL 表示失败
//
//////////////////////////////////////////////////
//CARD* GetCardPointer(short iCardID)
  CONTENT* GetContentPointer(short iContentID)
{
	return LegalContent(iContentID);
}


/*********群发驱动********************************************************/
void InitGroup()
{
//  for (long addr = GROUP_BASE_ADDR; 
//             addr < GROUP_BASE_ADDR + MAX_GROUP_NUMBER * sizeof(GROUP); 
//			 addr += FLASH_SECTOR_SIZE )
//	FlashEraseSector((UINT16*)addr);
}
  
void LoadGroupSend(GROUP *option)
{
	int		temp;

	//temp = sizeof(GROUP);
	temp = sizeof(GROUP) * MAX_GROUP_NUMBER;
	memcpy(option, (void*)GROUP_BASE_ADDR, temp);
}

void SaveGroupSend(GROUP *option)
{
	int		temp;

	//temp = sizeof(GROUP);
	temp = sizeof(GROUP) * MAX_GROUP_NUMBER;
	LoadFlashSector((void*)GROUP_BASE_ADDR);
	memcpy((void*)FLASH_BUF_ADDR, option, temp);
	FlashEraseSector((UINT16*)GROUP_BASE_ADDR);
	FlashWrite((UINT16*)(GROUP_BASE_ADDR), (UINT16*)FLASH_BUF_ADDR, FLASH_SECTOR_SIZE / 2);
}
/*****************************群发结束****************************************/