/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      uart.cpp                                        PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年10月15日   */
/*                                                                       */
/*      串口驱动文件     												 */
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
#include <stdlib.h>
#include "nucleus.h"
#include "uart.h"
#include "vz328.h"
#include "hwfcl.h"



/*
 *	Uart Relative variables
 */
char			UartBuf[UART_BUFFER_SIZE];
UINT32			pUartBufFPtr, pUartBufBPtr, UartTxBPtr, UartTxFPtr;
NU_HISR			UartRxHisrBlock, UartTxHisrBlock;
NU_EVENT_GROUP  UartEventsBlock;
NU_SEMAPHORE	UartTxSemaBlock;
char			*UartTxDataPtr;

/*
 *	Uart LISR and HISR prototypes
 */
void UART_Lisr(int Vector);
void UART_RxHisr(void);
void UART_TxHisr(void);
//////////////////////////////////////////////////
//
//  UART2 串口初始化
//
//	参数：	无
//
//  返回：	Nucleus STATUS
//
//////////////////////////////////////////////////
STATUS UART_Initialize(void)
{
    STATUS status;
    void *stack, (*oldlisr)(int);
    
	pUartBufFPtr = pUartBufBPtr = 0;

	/*
	*	Create uart send semaphore
	*/
	status = NU_Create_Semaphore(&UartTxSemaBlock,
		"UART SEMAPHORE", 1, NU_FIFO);
    if (status != NU_SUCCESS) return(status);
	
	/*
	*	Create event flag group
	*/
    status = NU_Create_Event_Group((NU_EVENT_GROUP *)&UartEventsBlock, "UART EVENT");
    if (status != NU_SUCCESS) return(status);

	/*
	*	Create the HISR
	*/
    stack = (VOID *)SysMalloc(UART_HISR_STACK_SIZE);

    if (stack == NU_NULL) return(NU_NO_MEMORY);
	status = NU_Create_HISR((NU_HISR *)&UartTxHisrBlock, "UART TX HISR",
                 UART_TxHisr, 2,
                 stack, UART_HISR_STACK_SIZE);
    if (status != NU_SUCCESS) return(status);

	/*
	*	Create the HISR
	*/
    stack = (VOID *)SysMalloc(UART_HISR_STACK_SIZE);
    if (stack == NU_NULL) return(NU_NO_MEMORY);
	status = NU_Create_HISR((NU_HISR *)&UartRxHisrBlock, "UART HISR",
                 UART_RxHisr, 2,
                 stack, UART_HISR_STACK_SIZE);
    if (status != NU_SUCCESS) return(status);
	
	/* 
	*	Register the LISR 
	*/
	status = NU_Register_LISR(64 + UART_INT_NUMBER, UART_Lisr, &oldlisr);
    if (status != NU_SUCCESS) return(status);

	/*
	*	Hardware initialize
	*/
	OUTBYTE(PJSEL, INBYTE(PJSEL) & ~(BIT4 | BIT5));	// dedicate function

#ifdef	_USE_UART_2
	OUTWORD(HMARK, 0x3C);					// Tx FIFO half = 12, Rx FIFO half = 48
#endif

//	OUTWORD(USTCNT, 0xE110);				// 收发允许，单字节中断，8数据位，1停止位，无校验
	OUTWORD(USTCNT, 0xE108);				// 收发允许，单字节中断，8数据位，1停止位，无校验
	//OUTWORD(USTCNT, 0xA100);				// 发允许，半满中断，8数据位，1停止位，无校验
	//OUTWORD(USTCNT, 0xC110);				// 收允许，半满中断，8数据位，1停止位，无校验
	OUTWORD(UBAUD, 0x0138);					// Divider = 6, prescaler = 0x26, baud = 1200
	INWORD(URX);							// dummy read , initialize FIFO and status bits

#ifdef	_USE_UART_2							// 只有 UART2 才可以调整中断级别
	OUTWORD(ILCR, (INWORD(ILCR) & 0xF0FF) | (UART_INT_NUMBER << 8));	// set UART2 int level
#endif

#ifdef	_USE_UART_2
	OUTLONG(IMR, INLONG(IMR) & ~BIT12);		// enable UART2 interrupt
#else
	OUTLONG(IMR, INLONG(IMR) & ~BIT2);		// enable UART1 interrupt
#endif
	
	
	return(status);
}
	

//////////////////////////////////////////////////
//
//  UART2 低级中断处理程序（包括 Rx, Tx）
//
//	参数：	Vector		中断向量号
//
//  返回：	无
//
//////////////////////////////////////////////////
void UART_Lisr(int Vector)
{
	/*
	*	Receive data from Rx FIFO, write to UartBuf[]
	*/
	if (INBYTE(URX) & BIT5){
		while(INBYTE(URX) & BIT5){
			UartBuf[pUartBufFPtr] = (char)INBYTE(URX + 1);
			if ((++pUartBufFPtr &= (UART_BUFFER_SIZE - 1)) == pUartBufBPtr){
				// TODO: alert uart buffer overflow
				--pUartBufFPtr &= (UART_BUFFER_SIZE - 1);
			}
		}
		NU_Activate_HISR(&UartRxHisrBlock);
	}
	
	/*
	*	Get data by transfer pointer, send to Tx FIFO
	*/
	if (INBYTE(UTX) & BIT5){
		while (INBYTE(UTX) & BIT5){
			if (UartTxBPtr == UartTxFPtr)
				break;
			OUTBYTE(UTX + 1, (char)(*(UartTxDataPtr + UartTxBPtr ++)));
		}
		if (UartTxBPtr == UartTxFPtr){
			OUTWORD(USTCNT, INWORD(USTCNT) & ~BIT2);		// disable tx interrupt
			NU_Activate_HISR(&UartTxHisrBlock);
		}
	}
}

//////////////////////////////////////////////////
//
//  UART2 接收高级中断处理程序
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void UART_RxHisr(void)
{
	STATUS		status;

	/*
	*  Set the event flag indicating data is available.
	*/
	status = NU_Set_Events(&UartEventsBlock, UART_DATA_READY, NU_OR);
}

//////////////////////////////////////////////////
//
//  UART2 发送高级中断处理程序
//
//	参数：	无
//
//  返回：	无
//
//////////////////////////////////////////////////
void UART_TxHisr(void)
{
	STATUS	status;
	
	/*
	*  Release Tx semaphore, so others can send data
	*/
	status = NU_Release_Semaphore(&UartTxSemaBlock);
}

//////////////////////////////////////////////////
//
//  读取 UART2 端口数据
//
//	参数：	pBuf		读取 buffer
//			BufLen		Buffer 长度
//
//  返回：	读到的字符数
//
//////////////////////////////////////////////////
int UART_Read(char *pBuf, int BufLen)
{
	int		count = 0, temp;

	/*
	*	Disable Uart interrupts, ensure data in UartBuf[] correct
	*/
	temp = NU_Control_Interrupts(64 + UART_INT_NUMBER);
	
	/*
	*	Read data from UartBuf[], write to output buffer
	*/
	while (count < BufLen){
		if (pUartBufFPtr == pUartBufBPtr)
			break;
		*pBuf++ = UartBuf[pUartBufBPtr++];
		pUartBufBPtr &= (UART_BUFFER_SIZE - 1);
		count ++;
	}
	
	/*
	*	Enable Uart interrupts again
	*/
	NU_Control_Interrupts(temp);
	
	/*
	*	Reture char numbers received
	*/
	return count;
}

//////////////////////////////////////////////////
//
//  向 UART2 端口写数据
//
//	参数：	pData		写 Data 指针
//			DataLen		写 Data 长度
//
//  返回：	Nucleus STATUS
//
//////////////////////////////////////////////////
STATUS UART_Write(char *pData, int DataLen)
{
	STATUS		status;
	
	/*
	*	Obtain Tx semaphore to avoid transfer conflict
	*/
	status = NU_Obtain_Semaphore(&UartTxSemaBlock, UART_TIME_OUT);
    if (status != NU_SUCCESS) return(status);

	/*
	*	Setup transfer pointer
	*/
	UartTxDataPtr = (char*)pData;
	UartTxBPtr = 0;
	UartTxFPtr = DataLen;
	
	/*
	*	Enable Tx interrupt, in which data sent
	*/
	OUTWORD(USTCNT, INWORD(USTCNT) | BIT2);
	
	return status;
}


// 设置波特率--cxd add
	 
void SetPortBaudRate(UINT16 nBaud)
{
	UINT16 nParam=0;
	switch(nBaud)
	{
	case 9600:
		nParam=0x0326;
		break;
	case 19200:
		nParam=0x0226;
		break;
	case 38400:
		nParam=0x0126;
		break;
	case 57600:
		nParam=0x0238;
		break;
	case 115200:
		nParam=0x0138;
		break;
	}
	OUTWORD(UBAUD, nParam);	
}


// 取得已接收数据个数

UINT32 GetInputDataLen()
{
	UINT32 DataLen=0;
	if(pUartBufBPtr > pUartBufFPtr)
		DataLen=UART_BUFFER_SIZE-pUartBufBPtr+pUartBufFPtr;
	if(pUartBufBPtr < pUartBufFPtr)
		DataLen=pUartBufFPtr-pUartBufBPtr;
	return DataLen;
}


// 判断数据是否发送完毕

UINT32 GetOutputDataLen()
{
	return UartTxFPtr -UartTxBPtr;
}


// 清空接收 buffer
void ClearRecieveBuf()	
{
	pUartBufBPtr = pUartBufFPtr=0;
}

//初始化串口，打开或关闭中断
void InitSerialPort(BOOL OpenFlag)
{
	static UINT32 IMRreg;
	
	if(OpenFlag)
	{
		IMRreg=INLONG(IMR);
		//OUTLONG(IMR, ~BIT2);		// enable UART1 interrupt
		OUTLONG(IMR, ~BIT12);		// enable UART1 interrupt
	}
	else
		OUTLONG(IMR, IMRreg);		// enable UART1 interrupt

}


int FastSendData(char *pData)
{
    unsigned long  len = strlen(pData);
	if(len == 0) return 0;
    UART_Write(pData, len);
	while(GetOutputDataLen());
	return len;
}

int SendBinData(char *pData, int len)
{
	UART_Write(pData, len);
	while(GetOutputDataLen());
	return len;
}
int SendData(char *pData)
{
    unsigned long len = strlen(pData)+1;
    UART_Write(pData, len);
	while(GetOutputDataLen());
	return len;
}

int RecieveData1(char *pBuff, int nBufLen)
{
	return UART_Read(pBuff, nBufLen);
}

int RecieveData(char *pBuff, int nBufLen)
{	
	unsigned long DataCount=GetInputDataLen();
	if(DataCount>nBufLen)
		DataCount=nBufLen;
	return UART_Read(pBuff, DataCount);
}
void delay(INT32 millisecond)
{
	//NU_Sleep(millisecond);
	for(int i=0; i<millisecond; i++)
		for(int j=0; j<1000; j++)
			;
}

BOOL TestAds(BOOL bCheckRet)
{
  char tmpbuf[100];
  for(int i=0; i<3; i++)
  {
      ::ClearRecieveBuf(); 
      ::SendData("\nabcd");
	  delay(100);
	  delay(100);
      ::RecieveData1(tmpbuf, 100);
	   if(bCheckRet)
	  {
        if(strstr(tmpbuf, "abcd"))
              return TRUE;
		 delay(10);
	  }
	  else 
		 return TRUE;
  }
  return FALSE;
}