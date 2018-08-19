/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             flash.cpp                                                      */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             汉王科技E-PhoneII驱动程序源文件                                */
/*                                                                            */
/*  组    成                                                                  */
/*             FLASH驱动程序源文件                                            */
/*                                                                            */
/*  作    者                                                                  */
/*             张向忠   王飞                                                  */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-07-11                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/******************************************************************************/
#include "flash.h"
#include "keydrv.h"
  
/////////////////////////////////////////////////////////////////////
// 描述：SST  Flash接口。
//
// 时间：2001年7月11日
// 
// 注释：FLASH   扇区大小   块 大 小   每片大小(对应片选)
//       =====   ========   ========   ==================
//         SST   4096字节   64K字节    2M字节(一个物理片)

#define   sst_get_address(lpwDst)   ((UINT16 *)((UINT32)lpwDst&0xFFE00000|0xAAAA))
#define   sst_get_offset(lpwDst)    ((UINT16 *)((UINT32)lpwDst&0xFFE00000|0x5554))

static void           (* pwait_entry)(UINT16 *) = NULL;
static BOOL           (* perase_entry)(UINT16 *) = NULL;
static BOOL           (* pwrite_word_entry)(UINT16 *, UINT16) = NULL;
static UINT32         (* pwrite_entry)(UINT16 *, UINT16 *, UINT32) = NULL;



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// sst flash IO
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/////////////////////////////////////////////////////////////////////
// 描述：Flash写操作需要一段时间，调用wait_till_ready将等待直
//       至Flash写操作结束。
//
// 参数：lpwDst   Flash的写地址。
//
// 注释：写操作过程中目标地址中的每个字节第6bit应在不停的变
//       化，如果不发生变化，表示写操作结束。
static void wait_till_ready (UINT16 *lpwDst)
{
	UINT16 predata, curdata;
	UINT32 timeout;
    UINT8  loop;

	CLR_WATCHDOG(); // hqf

	predata = *lpwDst;
	predata = predata & 0x4040;
	timeout = 0;
	loop = TRUE;
	while ((timeout < 0x07FFFFFF) && (loop))
	{
		curdata = *lpwDst;
		curdata = curdata & 0x4040;
		if(predata == curdata)
			loop = FALSE;

		predata = curdata;
		timeout++;
	}
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL sst_erase_sector(UINT16 *lpwDst)
{
	if(!pwait_entry)
		return FALSE;

	UINT16 *addres  = sst_get_address(lpwDst);
	UINT16 *offset  = sst_get_offset(lpwDst);

	*addres = 0xAAAA;
	*offset = 0x5555;
	*addres = 0x8080;
	*addres = 0xAAAA;
	*offset = 0x5555;
	*lpwDst = 0x3030;

	//wait_till_ready(lpwDst);
	//pwait_entry(lpwDst);
	CLR_WATCHDOG(); // hqf
	for (int i = 0; i < 0x8000; i ++);

	return TRUE; 
}
////////////////////////////////////////////////////////////////////////////////
//
BOOL sst_write_word(UINT16 *lpwDst, UINT16 wSrc)
{
	if(!pwait_entry)
		return FALSE;

	UINT16 *addres  = sst_get_address(lpwDst);
	UINT16 *offset  = sst_get_offset(lpwDst);

	*addres = 0xAAAA;
	*offset = 0x5555;
	*addres = 0xA0A0;
	*lpwDst = wSrc;

	//wait_till_ready(lpwDst);
	pwait_entry(lpwDst);
	
	return (*lpwDst == wSrc);
}
////////////////////////////////////////////////////////////////////////////////
//
UINT32 sst_erase(UINT16 *lpwStart,UINT16 *lpwStop)
{       
	while( (UINT16 *)((UINT8 *)lpwStart+0x1000)<=lpwStop )
	{	
		sst_erase_sector(lpwStart);	
		lpwStart=(UINT16 *)((UINT8 *)lpwStart+0x1000);
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////
//
UINT32 sst_write(UINT16 *lpwDst, UINT16 *lpwSrc, UINT32 dwSrcWords)
{ 
	if(!pwait_entry)
		return 0;

	UINT32  index;
	UINT16 *address = sst_get_address(lpwDst);
	UINT16 *offset  = sst_get_offset(lpwDst);

	UINT16 *src = lpwSrc, *dst = lpwDst;
	for(index=0; index<dwSrcWords; index++)
	{
		*address = 0xAAAA;
		*offset  = 0x5555;
		*address = 0xA0A0;

		*lpwDst++ = *lpwSrc++;
		pwait_entry(lpwDst-1);
		//wait_till_ready(lpwDst-1);
	}
	
	// 逐字校验
	for(index=0; index<dwSrcWords; index++)
		if(*src++ != *dst++)
		break;

    // 返回写正确的字数
	return index;
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL InitFlashDrv()
{
#define PROGRAM_LEN   512
  pwait_entry = (void (*)(UINT16 *))SysMalloc(PROGRAM_LEN);
  if(pwait_entry == NULL)
	  return FALSE;
  memcpy(pwait_entry, wait_till_ready, PROGRAM_LEN);

  perase_entry = (BOOL (*)(UINT16 *))SysMalloc(PROGRAM_LEN);
  if(perase_entry == NULL)
	  return FALSE;
  memcpy(perase_entry, sst_erase_sector, PROGRAM_LEN);

  pwrite_word_entry = (BOOL (*)(UINT16 *, UINT16))SysMalloc(PROGRAM_LEN);
  if(pwrite_word_entry == NULL)
	  return FALSE;
  memcpy(pwrite_word_entry, sst_write_word, PROGRAM_LEN);

  pwrite_entry = (UINT32  (*)(UINT16 *, UINT16 *, UINT32))SysMalloc(PROGRAM_LEN);
  if(pwrite_entry == NULL)
	  return FALSE;
  memcpy(pwrite_entry, sst_write, PROGRAM_LEN);
  return TRUE;
#undef  PROGRAM_LEN 
}
#define IMR     (*((UINT32*)0xFFFFF304)) 
////////////////////////////////////////////////////////////////////////////////
//
BOOL FlashEraseSector(UINT16 *lpwDst)
{
	BOOL	flag;
	if(perase_entry == NULL)
		return FALSE;
    //屏蔽中断
	UINT32 old = IMR;
    IMR = 0x00FFFFFF;
	flag = perase_entry(lpwDst);
	IMR = old;
	return flag;
	//打开中断
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL FlashWriteWord(UINT16  *lpwDst, UINT16 wSrc)
{
	BOOL	flag;
	if(pwrite_word_entry == NULL)
		return FALSE;
    //屏蔽中断
	UINT32 old = IMR;
    IMR = 0x00FFFFFF;
	flag = pwrite_word_entry(lpwDst, wSrc);
	IMR = old;
	return flag;
	//打开中断
}

////////////////////////////////////////////////////////////////////////////////
//
BOOL FlashWrite(UINT16 *lpwDst, UINT16 *lpwSrc, UINT32 dwSrcWords)
{
	BOOL	flag;
	if(pwrite_entry == NULL)
        return FALSE;

	//屏蔽中断
    UINT32 old = IMR;
    IMR = 0x00FFFFFF;
	flag = pwrite_entry(lpwDst, lpwSrc, dwSrcWords);
	IMR = old;
	return flag;
	//打开中断
}

////////////////////////////////////////////////////////////////////////////////
// end flash.cpp
