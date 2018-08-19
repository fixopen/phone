/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      uart.h                                          PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年10月15日   */
/*                                                                       */
/*      串口驱动头文件   												 */
/*                                                                       */
/* 作者                                                                  */
/*                                                                       */
/*      白亚伟                                                           */
/*                                                                       */
/* 数据结构                                                              */
/*                                                                       */
/*                                                                       */
/* 函数                                                                  */
/*                                                                       */
/*		UART_Initialize				串口初始化                           */
/*      UART_Read					读串口                               */
/*      UART_Write                  写串口                               */
/*                                                                       */
/* 关联                                                                  */
/*                                                                       */
/*                                                                       */
/* 历史记录                                                              */
/*                                                                       */
/*                                                                       */
/*************************************************************************/
#ifndef _UART_H
#define _UART_H

/*
*	UART macros select
*/
//#define _USE_UART_2
#ifdef	_USE_UART_2
#define	USTCNT		USTCNT2
#define	UBAUD		UBAUD2
#define	URX			URX2
#define	UTX			UTX2
#else
#define	USTCNT		USTCNT1
#define	UBAUD		UBAUD1
#define	URX			URX1
#define	UTX			UTX1
#endif

/*
*	UART port select
*/
//#define	_USE_UART_2					/* if defined, use UART2, otherwise UART1

/*
 *  UART Buffer Size
 */
#define	UART_BUFFER_SIZE			0x1000	/* 4K */

/*
 *  UART Event Flags
 */
#define	UART_DATA_READY				1			

/*
 *  UART Interrupt Level
 */
#ifdef	_USE_UART_2
#define	UART_INT_NUMBER				4	/* can vary from 1 to 6 */
#else
#define	UART_INT_NUMBER				4	/* fixed */
#endif


/*
 *  UART HISR Stack Size
 */
#define	UART_HISR_STACK_SIZE		0x1000	/* 4K */

/*
 *	UART Transfer Timeout Value
 */
#define	UART_TIME_OUT				500		/* ticks */

/*
 *	Function Prototypes
 */
extern STATUS UART_Initialize(void);				/* Initialize */
extern int UART_Read(char *pBuf, int BufLen);		/* Read from uart */
extern STATUS UART_Write(char *pData, int DataLen);	/* Write to uart */
//设置波特率--cxd add
extern void SetPortBaudRate(UINT16 nBaud);
//取得已接收数据个数
extern UINT32 GetInputDataLen();
//判断数据是否发送完毕
extern UINT32 GetOutputDataLen();
extern void ClearRecieveBuf();
#endif
