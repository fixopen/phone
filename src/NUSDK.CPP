/******************************************************************************/
/*                                                                            */
/*  版权所有(c)  2001汉王科技                                                 */
/*                                                                            */
/******************************************************************************/
/*  文 件 名                                                                  */
/*             nusdk.cpp                                                      */
/*                                                                            */
/*  版 本 号                                                                  */
/*             1.0                                                            */
/*                                                                            */
/*  描    述                                                                  */
/*             与Nucleus操作系统相关的一些接口函数                            */
/*                                                                            */
/*  组    成                                                                  */
/*             系统内存池的创建                                               */
/*             系统的内存分配及释放                                           */
/*             系统消息队列的创建                                             */
/*             向消息队列发送消息                                             */
/*             从消息队列中取出息                                             */
/*                                                                            */
/*  设计说明                                                                  */
/*             本文件包含一些与Nucleus相关的接口                              */
/*                                                                            */
/*  作    者                                                                  */
/*             王飞                                                           */
/*                                                                            */
/*  日    期                                                                  */
/*             2001-06-15                                                     */
/*                                                                            */
/*  历史记录                                                                  */
/*          ygr   2001-11-27   添加PostSystemMessage()函数                    */
/*                                                                            */
/*                                                                            */
/******************************************************************************/

#include "hwfcl.h"

//#define DEBUG
// temp, bbj
bool		Sending = false;
int			msgcount = 0;

#define		SYS_QUEUE_SIZE		128

/*============================================================================*/
//  有关的宏定义                                                              //
/*============================================================================*/

////////////////////////////////////////////////////////////////////////////////
//系统中所有可分配的内存大小
#define SYSTEM_ALLOC_MEMORY_SIZE     0x80000

////////////////////////////////////////////////////////////////////////////////
//系统内存池
NU_MEMORY_POOL  g_nuSystem_Memory_Pool;

////////////////////////////////////////////////////////////////////////////////
//系统消息队列
static  NU_QUEUE     g_nuQueue;

void DebugInfo(char *Info)
{
	CDC		dc;
	dc.TextOut(0, 0, Info);
	while(1);
}

////////////////////////////////////////////////////////////////////////////////
//
void InitSystemMemory(void *first_available_memory)
{
 NU_Create_Memory_Pool(&g_nuSystem_Memory_Pool, 
	                   "SYSMEM", 
		               first_available_memory, 
					   SYSTEM_ALLOC_MEMORY_SIZE, 
					   16, 
					   NU_FIFO);
}

////////////////////////////////////////////////////////////////////////////////
//
void * SysMalloc(UINT32 nSize)
{
  void *pointer;

  if(NU_Allocate_Memory(&g_nuSystem_Memory_Pool, 
						&pointer, 
	                    nSize, 
						NU_NO_SUSPEND) != NU_SUCCESS)
  {
#ifdef DEBUG
	  char strTemp[64];
	  sprintf(strTemp , "%d %d " , g_nuSystem_Memory_Pool.words[11], nSize);
	  strcat(strTemp , "内存分配失败！");
	  DebugInfo(strTemp);
#else
    return NULL;
#endif
  }
  return pointer;
}

////////////////////////////////////////////////////////////////////////////////
//
void * SysRealloc(void *pPointer , UINT32 nBeforeSize , UINT32 nAfterSize)
{
	void	*p;
	
	if (nAfterSize <= nBeforeSize)
		return pPointer;
	if ((p = SysMalloc(nAfterSize)) != NULL){
		memcpy(p, pPointer, nBeforeSize);
		SysFree(pPointer);
	}
	return p;
}


////////////////////////////////////////////////////////////////////////////////
//
void SysFree(void * pPointer)
{
	if (!pPointer)		// byw
		return;
	NU_Deallocate_Memory(pPointer);
	pPointer = NULL;
}
static	void *MsgQueuepointer;
//////////////////////////////////////////////////////////////////////////
//ygr
//
#define MSG_HISR_STACK_SIZE	1024
NU_HISR SysMsgHisr;
static SYSTEM_MESSAGE SYS_MSG ;

void SysMsgHisrEntry(void)
{
	PostMessageToQueue(&SYS_MSG);
	memset(&SYS_MSG,0,sizeof(SYSTEM_MESSAGE));
}

void PostSystemMessage(UINT32 msg,UINT32 handle,UINT32 wparam,UINT32 lparam,bool bActivHisr)
{
	SYS_MSG.message =msg;
	SYS_MSG.handle	=handle;
	SYS_MSG.lparam	=lparam;
	SYS_MSG.wparam	=wparam;
	if(bActivHisr)
		NU_Activate_HISR(&SysMsgHisr);
	else
	{
		PostMessageToQueue(&SYS_MSG);
		memset(&SYS_MSG,0,sizeof(SYSTEM_MESSAGE));
	}
}
/**************************************************************************
*ygr*/

////////////////////////////////////////////////////////////////////////////////
//
NU_SEMAPHORE g_QueueSemPhore;
BOOL CreateSystemMessageQueue()
{
/*	STATUS  NU_Create_Semaphore(NU_SEMAPHORE *semaphore,
CHAR *name,
UNSIGNED initial_count,
OPTIONsuspend_type);  */
    NU_Create_Semaphore(&g_QueueSemPhore,"Queue" , 1, NU_FIFO);
	MsgQueuepointer = SysMalloc(SYS_QUEUE_SIZE*4); 
	if(NU_Create_Queue(&g_nuQueue, "", MsgQueuepointer, SYS_QUEUE_SIZE, NU_FIXED_SIZE, 4, NU_FIFO) != NU_SUCCESS)
         return FALSE;
//ygr-------------------------------
	void *pointer;
	pointer = SysMalloc(MSG_HISR_STACK_SIZE);
	if(NU_Create_HISR(&SysMsgHisr, "SysMessageSendHisr", SysMsgHisrEntry, 2, pointer,  MSG_HISR_STACK_SIZE) !=NU_SUCCESS)
		return false;
//ygr-------------------------------
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
void PostMessageToQueue(SYSTEM_MESSAGE *pMessage)
{

#ifdef	DEBUG
	STATUS	status;
	char    strMsg[64];

	//NU_Obtain_Semaphore(&g_QueueSemPhore, NU_SUSPEND);
	
	status = NU_Send_To_Queue(&g_nuQueue, pMessage, 4, NU_NO_SUSPEND);
	//NU_Release_Semaphore(&g_QueueSemPhore);

	if (status != NU_SUCCESS)
	{
		sprintf(strMsg , "M:%d,C:%d", pMessage->message, status);
		strcat(strMsg, "队列出错!");
//		DebugInfo(strMsg);
	}
#else
//	NU_Obtain_Semaphore(&g_QueueSemPhore, NU_NO_SUSPEND);
	NU_Send_To_Queue(&g_nuQueue, pMessage, 4, NU_NO_SUSPEND);
//	NU_Release_Semaphore(&g_QueueSemPhore);
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
void GetMessageFromeQueue(SYSTEM_MESSAGE *pMessageBuf)
{
	UNSIGNED actual_size;
	//NU_Obtain_Semaphore(&g_QueueSemPhore, NU_SUSPEND);
	NU_Receive_From_Queue(&g_nuQueue, pMessageBuf, 4, &actual_size, NU_SUSPEND);
	//NU_Release_Semaphore(&g_QueueSemPhore);
}

////////////////////////////////////////////////////////////////////////////////
//
void * operator new(size_t nSize)
{
	return SysMalloc(nSize);
}

////////////////////////////////////////////////////////////////////////////////
//
void * operator new[](size_t nSize)
{
	return operator new(nSize);
}

////////////////////////////////////////////////////////////////////////////////
//
void operator delete(void * obj)
{
	SysFree(obj);
}

////////////////////////////////////////////////////////////////////////////////
//
void operator delete[](void *obj)
{
	operator delete(obj);
}

char *hwstrncpy(char *strDest, const char *strSource, size_t count )
{
	strncpy(strDest,  strSource,  count );
//	strDest[count-1] = '\0';
	return strDest;
}


////////////////////////////////////////////////////////////////////////////////
//end nusdk.cpp