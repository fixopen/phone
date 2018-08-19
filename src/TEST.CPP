#include "hwfcl.h"     
#include "keydrv.h"
#include "pendrv.h"
#include "phonedrv.h"
#include "flash.h"
#include "uart.h"
#include "datadrv.h"
///////////////////////////////////////////////////////////////////////////////
NU_TASK         Task0;
extern BOOL TranslateMessage(const SYSTEM_MESSAGE *lpMsg);
extern void InitWndSystem();
extern void InitHwDictionary();
extern SYSTEM_OPTION  g_SystemOption;

void tempecho(int num)
{
	CDC		dc;
	char	buf[8];

	sprintf(buf, "%02d", num);
	dc.TextOut(30, 30, buf);
}

void EventLoop(UINT32 * pExitFlag)
{
//	KillWatchDog();
	OpenWatchDog();

	SYSTEM_MESSAGE  CurMessage;
	while(1)
	{
		CLR_WATCHDOG();
		GetMessageFromeQueue(&CurMessage);
        if(!TranslateMessage(&CurMessage))
           DispatchMessage(&CurMessage);

		if(pExitFlag && *pExitFlag)
			break;
     }
}
/*
void TestPenDrvLoop()
{ 
   SYSTEM_MESSAGE  CurMessage;
//   CRect r(0,0,LCD_WIDTH-1, LCD_HEIGHT-1);
   static CDC dc;
   CDC::ClearDevice();
   ::SetTouchPaneSampleRate(80);
	while(1)
	{
		GetMessageFromeQueue(&CurMessage);
		CPoint pt(HIWORD(CurMessage.wparam), LOWORD(CurMessage.wparam));  

	     switch(CurMessage.message)
         {
         case WM_PENDOWN: case WM_KEY:
  //           if(pt.InRect(r))
              dc.MoveTo(pt.x, pt.y);
             break;
         case WM_PENMOVE: case WM_KEYKEEP:
   //          if(pt.InRect(r))
               dc.LineTo(pt.x, pt.y);
             break;
         case WM_PENUP: 
             break;
         }
	 //   NU_Sleep(5);
     }
}
*/

#include "ring.h"
void    event_task(UNSIGNED argc, void *argv)
{
   //Init touch panel
   //CalibrationPen();
	
	//关底板继电器，避免短消息死机 Watchdog 启动后继电器继续吸合, byw, 2002/3/5
	DisconnectServer();
	
	//初始化留言设备
	extern void initRecorder(void);
	initRecorder();

/*	int size = sizeof(WAVEHEAD);
	size = sizeof(HISTORY);
	size = sizeof(MEMO);
	size = sizeof(CARD);
	size = sizeof(CARDCASE);
	size = sizeof(ALARM);
	size = sizeof(FASTDIAL);
	size = sizeof(SYSTEM_OPTION);
	size = sizeof(SMSLETTERBOX);

	size = sizeof(SMS);
	size = sizeof(CONTENT);
	size = sizeof(MODEL);
	size = sizeof(PEOPLE);
	size = sizeof(GROUP);

	size = sizeof(SYSTEM_OPTION);
*/

    EventLoop(NULL);
//TestPenDrvLoop();
}
#include "datadrv.h"

void    Application_Initialize(void *first_available_memory)
{
	//创建系统内存池
    InitSystemMemory(first_available_memory);
	
	//创建系统消息队列
	CreateSystemMessageQueue();

	//初始化图形系统
	HwInitGraphicsSystem(); 

	//延时，等待系统电源供电稳定
/*	CDC		dc;
	dc.TextOut(30, 70, "请稍后...");
	for (int i = 0; i < 320000; i ++)
	{
		if (i % 20000 == 0)
		{
			dc.InvertRect(100, 70, 120, 85);
		}
	}
*/
	//初始化I/O端口
	InitControlDev();

	//HwClearDevice(WHITE);
		
	//初始化触摸屏驱动
	InitPenDrv();
	
	//初始化电话驱动
    InitPhoneDrv();
	
	//初始化FLASH驱动
	InitFlashDrv();

	//初始化串口驱动
	UART_Initialize();

	//初始化键盘驱动
	InitKeyDrv();

	//初始化名片夹和名片数据
//	extern void InitCardCase(void);
//	extern void InitCard(void);
	InitCardCase();
	InitCard();

    //初始化提醒数据
	extern void InitAlarmData();
	InitAlarmData();
	
	//初始化通话记录
	InitHistory();
	
	//初始化系统设置
	extern void InitSystemOption();
	InitSystemOption();
	
	//初始化RTC及系统时钟         // hqf 2－28 改变次序
    InitRtcDriver();
	
	//初始化速拨设置
	extern void InitFastDial();
	InitFastDial();

	//初始化窗口系统
	InitWndSystem();
	
	//开机画面
	extern void ShowPowerOnTip();
	ShowPowerOnTip();

	//初始化手写识别 
	InitHwDictionary(); 

	//初始化振铃设备
	extern int initRinger();
	initRinger();
	if (g_SystemOption.nDefaultRingID == -1){
		EnableHardwareRing();
	}
	else{
		DisableHardwareRing();
	}
	
	//初始化提醒
	extern void initAlarm();
	initAlarm();

	////////初始化短消息
	extern void InitSmsPhy();
	InitSmsPhy(); /////////物理层
	
	extern void RunSMSL2 ( );
	RunSMSL2 ( ); ///////// 链路层

	extern void RunSMSMessage ( ); 
	RunSMSMessage ();///// 消息层	

	//创建任务
	// debug, temp, bbj
	//VOID  *pointer = SysMalloc(EVENT_TASK_STACK_SIZE);
	
	void *pointer;
	pointer = (void*)EVENT_TASK_STACK;
	memset(pointer, 0xFF, EVENT_TASK_STACK_SIZE);
	memset(&Task0 , 0 , sizeof(NU_TASK)); // hqf 2002-2-6
	NU_Create_Task(&Task0, "TASK 0", event_task, 0, NU_NULL, pointer,
                              EVENT_TASK_STACK_SIZE, 2, 50, NU_PREEMPT, NU_START);

}



/////////////////////////////////////////////////////////////////////////////////
// 
/**********************	internal debug interface(1) **********************/

void _save_callid_data(BYTE *pdata, BYTE type)
{
	DateTime	dt;
	UINT32		addr, len;

	/* Locate free flash space */
	
	for (addr = 0x01300000; addr < 0x01400000; addr += 1024){
		if (*(BYTE*)addr == 0xFF)
			break;
	}
	if (addr == 0x01400000)
		return;
	
	
	/* Count write lenght */
	
	for (len = 0; len < 1023; len ++){
		if (*(pdata + len) == type)
			break;
	}
	len ++;


	/* Write to flash */

	GetSysTime(&dt);
	FlashWrite((UINT16*)addr, (UINT16*)&dt, sizeof(DateTime)/2);
	FlashWrite((UINT16*)(addr + sizeof(DateTime)), (UINT16*)pdata, (len + 1) / 2);
	//FlashWrite(UINT16 *lpwDst, UINT16 *lpwSrc, UINT32 dwSrcWords);
}


/**********************	internal debug interface(2) **********************/

void _password_filter(char *password)
{
	CDC		dc;
	char	temp[32];

	/* Erase debug data */

	if (strcmp(password, "9fg5i") == 0){
		for (UINT32	addr = 0x01300000; addr < 0x01400000; addr += 0x1000){
			FlashEraseSector((UINT16*)addr);
		}
		dc.TextOut(0, 0, "_erase OK!");
	}


	/* Display debug item total */

	else if (strcmp(password, "zr4z4") == 0){
		UINT32	addr;
		for (addr = 0x01300000; addr < 0x01400000; addr += 1024){
			if (*(BYTE*)addr == 0xFF)
				break;
		}
		sprintf(temp, "_total %04d", (addr - 0x01300000) / 1024);
		dc.TextOut(0, 0, temp);
	}
}
