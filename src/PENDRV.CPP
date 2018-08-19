/*************************************************************************/
/*                                                                       */
/*                       版权所有(c) 2001汉王科技                        */
/*                                                                       */
/*************************************************************************/
  
/*************************************************************************/
/*                                                                       */
/* 文件名                                               版本             */
/*                                                                       */
/*      pendrv.cpp                                      PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* 描述                                                 2001年7月7日     */
/*                                                                       */
/*      触摸屏驱动程序                                                   */
/*                                                                       */
/* 作者                                                                  */
/*                                                                       */
/*      白亚伟                                                           */
/*                                                                       */
/* 数据结构                                                              */
/*                                                                       */
/*      PENMESSAGE                              笔消息                   */
/*                                                                       */
/* 函数                                                                  */
/*                                                                       */
/*      InitPenDrv - - - - - - - - - - - - - 初始化触摸屏驱动            */
/*      CalibrationPen - - - - - - - - - - - - - - 触摸屏校准            */
/*      EnablePenMessage - - - - - - - - - - - -允许发送触摸屏消息       */
/*      DisablePenMessage - - - - - - - - - - - 禁止发送触摸屏消息       */
/*      SetTouchPaneSampleRate  - - - - - - - - 设置触摸屏采样速率       */
/*                                                                       */
/* 关联                                                                  */
/*                                                                       */
/*      pendrv.h                                触摸屏驱动头文件         */
/*                                                                       */
/*                                                                       */
/* 历史记录                                                              */
/*                                                                       */
/*		byw, 2001/12/25													 */
/*			modify HISR_service() 函数									 */
/*                                                                       */
/*************************************************************************/
#include "dc.h"
#include "pendrv.h"
#include "keydrv.h"
#include "datadrv.h"

//------------------------------- Motorala VZ328 ------------------------------------
// Registers base addr
#define	M328BASE			0xFFFFF000

// PIO Registers 
#define	PCPDEN				(M328BASE+0x412) // Pull-down Enable Reg 
#define	PCSEL				(M328BASE+0x413) // Select Reg 

// SPI2 control registers
#define SPIDATA2			(M328BASE+0x800)
#define SPICONT2			(M328BASE+0x802)

// PortB control registers 
#define	PBDIR				(M328BASE+0x408) // 0 = input(default), 1 = output
#define	PBDATA				(M328BASE+0x409) // 0 (default) = drive output to low (DIRx is 1)
											 //	 = external is low (DIRx is 0)
											 // 1 = drive output to high (DIRx is 1)
											 //	 = external is high (DIRx is 0)
#define	PBPUEN				(M328BASE+0x40A) // 0 = pull up disable, 1 = pull up enable(default)
#define	PBSEL				(M328BASE+0x40B) // 0 = dedicated function, 1 = I/O port(default)

// PortE control registers 
#define PEDIR				(M328BASE+0x420) // 0 = input(default), 1 = output
#define	PEDATA				(M328BASE+0x421) // 0 (default) = drive output to low (DIRx is 1)
											 //	 = external is low (DIRx is 0)
											 // 1 = drive output to high (DIRx is 1)
											 //	 = external is high (DIRx is 0)
#define	PEPUEN				(M328BASE+0x422) // 0 = pull up disable, 1 = pull up enable(default)
#define	PESEL				(M328BASE+0x423) // 0 = dedicated function, 1 = I/O port(default)

// PortF control registers 
#define	PFDIR				(M328BASE+0x428) // 0 = input(default), 1 = output
#define	PFDATA				(M328BASE+0x429) // 0 (default) = drive output to low (DIRx is 1)
											 //	 = external is low (DIRx is 0)
											 // 1 = drive output to high (DIRx is 1)
											 //	 = external is high (DIRx is 0)
#define	PFPUEN				(M328BASE+0x42A) // 0 = pull up disable, 1 = pull up enable(default)
#define	PFSEL				(M328BASE+0x42B) // 0 = dedicated function, 1 = I/O port(default)

// Timer2 Registers 
#define	TCTL2				(M328BASE+0x610) // Control Reg 
#define	TPRER2				(M328BASE+0x612) // Prescalar Reg 
#define	TCMP2				(M328BASE+0x614) // Compare Reg 
#define	TCR2				(M328BASE+0x616) // Capture Reg 
#define	TCN2				(M328BASE+0x618) // Counter 
#define	TSTAT2				(M328BASE+0x61A) // Status Reg 

// Interrupt Registers 
#define	IVR					(M328BASE+0x300) // Interrupt Vector Reg 
#define	ICR					(M328BASE+0x302) // Interrupt Control Reg 
#define	IMR					(M328BASE+0x304) // Interrupt Mask Reg 
#define	ISR					(M328BASE+0x30C) // Interrupt Status Reg 
#define	IPR					(M328BASE+0x310) // Interrupt Pending Reg 

// port access macros 
#define	INBYTE(addr)			(*((unsigned char *)(addr)))
#define OUTBYTE(addr, data)		(*((unsigned char *)(addr)) = (unsigned char)(data))
#define	INWORD(addr)			(*((unsigned short *)(addr)))
#define OUTWORD(addr, data)		(*((unsigned short *)(addr)) = (unsigned short)(data))
#define	INLONG(addr)			(*((unsigned long *)(addr)))
#define	OUTLONG(addr, data)		(*((unsigned long *)(addr)) = (unsigned long)(data))

//------------------------------------ Constants --------------------------------
// hisr stack size
#define	PEN_HISR_STACK_SIZE		1024

// sample result state
#define RESULTOK			0		// sample result (x, y) ready for reading
#define RESULTNOTOK			1		// 

// state machine's states
#define	SMDISSEL			0		// disselect
#define	SMX0				1		// sampling x 1st time
#define	SMX1				2		// sampling x 2nd time
#define	SMY0				3		// sampling y 1st time
#define	SMY1				4		// sampling y 2nd time

// pen states
#define	SMPENDOWN			0		// pendown
#define	SMPENUP				1		//
#define	SMPENMOVE			2		//

// calibration error
#define	CALIBRATION_ERROR	8		// error max value(in pixels)

// move valid point error
#define	MOVE_ACCELERATION_MAX	4		// move maximum acceleration

//------------------------------------ Data type --------------------------------

// pen message
struct PENMESSAGE
{
	int		type;
	int		x;
	int		y;
};

//-------------------------------- Variables --------------------------------------

	int			SMState;				// state machine state
	int			SMPowerDownCount;		// state machine power down counter
	int			PhysicalX;				// physical x coordinate
	int			PhysicalY;				// physical y coordinate
	int			PenLogicalState;		// pen logical state (SMPENUP / SMPENDOWN)
	int			SampleResult;			// sample result state (RESULTOK / RESULTNOTOK)
	NU_HISR		CB_PenHISR;				// pen high level interrupt control block
	void		*HISRstack;				// high level interrupt stack
	PENMESSAGE	Penmsg;					// pen message

	// pen calibration modulus 
/*
	int			g_SystemOption.KxNom   = -119;
	int			g_SystemOption.KxDenom = -967; 
	int			g_SystemOption.KyNom   = -119;
	int			g_SystemOption.KyDenom = 942;
	int			g_SystemOption.Dx      = -70;
	int			g_SystemOption.Dy      = 174;
*/
	int			CalibrationOK = 1;

	// state machine sample rate
	int			SampleClocks = 10;		// state machine clocks per sample
										// minimum: 4 (the smaller, the faster sample rate)
										//
										// 触摸屏采点速率计算公式：
										//		采点速率 = 512 / SampleClocks （点/秒）

	static  int		g_iLastLogicalX, g_iLastLogicalY; 
	static	int		g_iLastVx, g_iLastVy;
	static	bool	g_bLastPointAbandon;

//--------------------------------------- Prototypes -----------------------------------

#define		PRIVATE			static
#define		PUBLIC

PRIVATE void HISR_service(void);
/*PRIVATE*/ void LISR_sample(int vector_number);
PRIVATE int SendADS7843Command(int command);
PRIVATE void ADS7843Enable(void);
PRIVATE void ADS7843Disable(void);
PRIVATE int	RunSampleSM(void);
PUBLIC void InitPenDrv(void);
PUBLIC void CalibrationPen(void);
PRIVATE void OutputState(int penmessage);
PUBLIC void SetTouchPaneSampleRate(int frequency);
PRIVATE bool ValidPoint();


///////////////////////////////////////////////////////////
//
//	Send pen message to queue
//	
///////////////////////////////////////////////////////////
PRIVATE void HISR_service(void)
{
    SYSTEM_MESSAGE msg;
	msg.handle = 0;
	msg.lparam = 0;

	int keycode = PointInTouchKey(Penmsg.x, Penmsg.y);
	// 向消息队列发送笔消息
	switch (Penmsg.type){
	case SMPENUP:
		if (keycode == -1){
			// 向消息队列发送“笔抬起消息”
			msg.message = WM_PENUP; 
		}
/*
		else{
			// 向消息队列发送“按键抬起”消息，键码 keycode
			msg.wparam = keycode;
			msg.message = WM_KEYUP;
		}
*/
		break;
	case SMPENDOWN:
		if (keycode == -1){
			// 向消息队列发送“笔按下消息”，笔坐标（Penmsg.x, Penmsg.y)
			msg.wparam = MAKELONG(Penmsg.y, Penmsg.x);   
			msg.message = WM_PENDOWN;
		}
		else{
/*
			// 向消息队列发送“按键按下”消息，键码 keycode
			msg.wparam = keycode;
			msg.message = WM_KEY;
*/
			// 向消息队列发送“按键抬起”消息，键码 keycode
			msg.wparam = keycode;
			msg.message = WM_KEYUP;
		}
		break;
	case SMPENMOVE:
		//if (keycode == -1){
			// 向消息队列发送“笔移动消息”，笔坐标（Penmsg.x, Penmsg.y)
			msg.wparam = MAKELONG(Penmsg.y, Penmsg.x);  
			msg.message = WM_PENMOVE;
/*
		}
		else{
			// 向消息队列发送“按键保持”消息，键码 keycode
//			msg.wparam = keycode;
//			msg.message = WM_KEYKEEP;
			return ;//ygr
		}
*/
		break;
	}
	PostMessageToQueue(&msg);
}

///////////////////////////////////////////////////////////
//
//	Sample timer service routine.
//	
///////////////////////////////////////////////////////////
/*PRIVATE*/ void LISR_sample(int vector_number)
{
	// run sample state machine
	RunSampleSM();

	// clear interrupt status flag
	OUTWORD(TSTAT2, INWORD(TSTAT2) & ~0x0001);
}

///////////////////////////////////////////////////////////
//
//	Send command to ADS7843 and receive reply.
//	
//	Parameters:
//	command			command sent to ADS7843
//
//	Return back:
//	data received from ADS7843
//	
///////////////////////////////////////////////////////////
PRIVATE int SendADS7843Command(int command)
{
	unsigned int	data;
	
	OUTBYTE(SPIDATA2 + 1, 0x00);					// load data
	OUTBYTE(SPICONT2, INBYTE(SPICONT2) | 0x01);		// trigger exchange
	while (!(INWORD(SPICONT2) & 0x0080));			// polling IRQ bit
	OUTBYTE(SPICONT2 + 1, INBYTE(SPICONT2 + 1) & ~0x80);	// clear IRQ bit
	data = INBYTE(SPIDATA2 + 1);					// read data
	data <<= 8;

	OUTBYTE(SPIDATA2 + 1, command);					// load data
	OUTBYTE(SPICONT2, INBYTE(SPICONT2) | 0x01);		// trigger exchange
	while (!(INWORD(SPICONT2) & 0x0080));			// polling IRQ bit
	OUTBYTE(SPICONT2 + 1, INBYTE(SPICONT2 + 1) & ~0x80);	// clear IRQ bit
	data |= INBYTE(SPIDATA2 + 1);					// read data

	return (data >> 4);								// return coordinate
}

///////////////////////////////////////////////////////////
//
//	ADS7843 enable and disable.
//	
///////////////////////////////////////////////////////////
PRIVATE void ADS7843Enable(void)
{
	OUTBYTE(PBDATA, INBYTE(PBDATA) & ~0x40);
}

PRIVATE void ADS7843Disable(void)
{
	OUTBYTE(PBDATA, INBYTE(PBDATA) | 0x40);
}

///////////////////////////////////////////////////////////
//
//	Initialize hardwares relative to touch pane.
//	
///////////////////////////////////////////////////////////
PUBLIC void InitPenDrv(bool bInitSystem)
{
	STATUS	status;
	void	(*old_lisr)(int);

	// initialize portF.1 (IRQ5)
	OUTBYTE(PFSEL, INBYTE(PFSEL) | 0x02);	// I/O function
	OUTBYTE(PFDIR, INBYTE(PFDIR) & 0xFD);	// intput direction
	OUTBYTE(PFPUEN, INBYTE(PFPUEN) & 0xFD);	// pull up disable

	// initialize portB.6 ( TchEN)
	OUTBYTE(PBSEL, INBYTE(PBSEL) | 0x40);	// I/O function
	OUTBYTE(PBDIR, INBYTE(PBDIR) | 0x40);	// output direction
	OUTBYTE(PBPUEN, INBYTE(PBPUEN) & 0xBF);	// pull up disable
	OUTBYTE(PBDATA, INBYTE(PBDATA) | 0x40);	// output high

	// initialize spi2
	OUTBYTE(PESEL, INBYTE(PESEL) & 0xF8);	// select PE port to SPI
	OUTWORD(SPICONT2, 0x4007);				// set SPI mode
	OUTWORD(SPICONT2, INWORD(SPICONT2) | 0x0200); // enable SPI2

	// Initialize state machine.
	SMState = SMDISSEL;
	PenLogicalState = SMPENUP;
	SMPowerDownCount = SampleClocks - 4;
	SampleResult = RESULTNOTOK;
    
	// initialize sample interrupt service routine
//	status = NU_Register_LISR(67, LISR_sample, &old_lisr);

	// create high level interrupt
	//status = NU_Allocate_Memory(&System_Memory, &HISRstack, 1024, NU_NO_SUSPEND);
	if(bInitSystem)		//ygr
	{
		HISRstack = SysMalloc(PEN_HISR_STACK_SIZE);
		// bbj
		status = NU_Create_HISR(&CB_PenHISR, "pen msg",	HISR_service, 2, HISRstack, PEN_HISR_STACK_SIZE);
	}

	// initialize timer2
	OUTWORD(TCTL2, 0x0019);		// compare interrupt, clk = 32768, enable timer2
	OUTWORD(TPRER2, 0);			// prescaler = 1
	OUTWORD(TCMP2, 64);			// compare = 64
	OUTWORD(TCR2, 0);			// clear capture
	OUTWORD(TCN2, 0);			// clear counter
	OUTWORD(TSTAT2, 0);			// clear status

	// ADS7843 hardware reset
	ADS7843Enable();
	SendADS7843Command(0xD0);
	SendADS7843Command(0x00);
	SendADS7843Command(0x00);
	SendADS7843Command(0x00);
	ADS7843Disable();

	// initialize interrupt
	OUTLONG(IMR, INLONG(IMR) & ~0x00000020);	// enable timer2 int

}



///////////////////////////////////////////////////////////
//
//	Run sapmle state machine.
//	Call this routine every one SM clock cycle.
//
//	return back:
//	State machine's state after this running.
//
///////////////////////////////////////////////////////////
PRIVATE int	RunSampleSM(void)
{
	int		penstate = INBYTE(PFDATA) & 0x02 ? SMPENUP : SMPENDOWN;

	switch (SMState){

	case SMDISSEL: // powerdown
		if (penstate == SMPENUP){
			if (PenLogicalState == SMPENDOWN){
				// Output "pen up"
				OutputState(SMPENUP);
				PenLogicalState = SMPENUP;
				SampleResult = RESULTNOTOK;
				//g_iLastLogicalX = -1;
				//g_iLastLogicalY = -1; 
			}
			SMPowerDownCount = SampleClocks - 4;
		}
		else {
			if (SampleResult == RESULTOK){
				if (PenLogicalState == SMPENUP){
					// Output "pen down"
					OutputState(SMPENDOWN);
					PenLogicalState = SMPENDOWN;
				}
				else{
					// Output "pen move"
					//if (ValidPoint())		// byw, 2002/1/15
						OutputState(SMPENMOVE);
				}
				SampleResult = RESULTNOTOK;
			}
			if (SMPowerDownCount)
				SMPowerDownCount --;
			else{
				ADS7843Enable();
				for (int i = 0; i < 10; i ++)	// 去抖, byw, 2002/1/15
					;
				SendADS7843Command(0x93);
				SMState = SMX0;
				SampleResult = RESULTNOTOK;
			}
		}
		return SMState;

	case SMX0: // sample x 1st time
		SendADS7843Command(0x93);
		SMState = SMX1;
		return SMState;
	
	case SMX1: // sample x 2nd time
		PhysicalY = SendADS7843Command(0xD3);
		SMState = SMY0;
		return SMState;
	
	case SMY0: // sample y 1st time
		SendADS7843Command(0xD3);
		SMState = SMY1;
		return SMState;

	case SMY1: // sample y 2nd time
		PhysicalX = SendADS7843Command(0xD0);
		ADS7843Disable();
		SMPowerDownCount = SampleClocks - 4;
		SampleResult = RESULTOK;
		SMState = SMDISSEL;
		return SMState;

	default:
		return SMState;
	} // end switch(SMState)
}


///////////////////////////////////////////////////////////
//
//	Touch pane calibration.
//
///////////////////////////////////////////////////////////
PUBLIC void CalibrationPen(void)
{
	int		crosswidth = 20;
	int		lcdx0, lcdy0, lcdx1, lcdy1, lcdx2, lcdy2, penx0, peny0, penx1, peny1, penx2, peny2;
	ScreenBuf	*save;
	CDC          dc;

	bool bHardRingEnabled=HardwareRingIsEnabled();
	EnableHardwareRing();//ygr 1-15

	CalibrationOK = 0; 
	// save current screen
	save = CDC::SaveScreen(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	///////////////////////////////////////////////////////////
	//获取光标属性
	BOOL bIfOpenCursor =  GetCursorStatus();
	CloseCursor();
extern void StopRing(bool bActiveHisr=false);
//    ::StopRing();  //关闭振铃，否则触摸屏可能不可用
	::CloseRtcInterrupt(RTCI_SECOND);
//	extern CFrameWnd *GetActiveWindow();
//	CFrameWnd *pCurWindow = GetActiveWindow();
//	if(pCurWindow)
//	{
//		pCurWindow->SendMessage(WM_DACTIVE , 0 , 0);

//	}

	bool	ok = false;
	while (!ok){
		lcdx0 = crosswidth / 2;
		lcdy0 = crosswidth / 2;
		lcdx1 = LCD_WIDTH - 1 - crosswidth / 2;
		lcdy1 = LCD_HEIGHT - 1 - crosswidth / 2;

		CDC::ClearDevice();
		dc.MoveTo(0, 0);
		dc.LineTo(crosswidth, crosswidth);
		dc.MoveTo(0, crosswidth);
		dc.LineTo(crosswidth, 0);
		dc.TextOut(32,60,"请准确点击左");
		dc.TextOut(32,78,"上角叉字中心"); 
		while(PenLogicalState != SMPENDOWN);
		penx0 = PhysicalX;
		peny0 = PhysicalY;
		while(PenLogicalState != SMPENUP);
		CDC::ClearDevice();
      
		dc.MoveTo(LCD_WIDTH - crosswidth - 1, LCD_HEIGHT - crosswidth - 1);
		dc.LineTo(LCD_WIDTH - 1, LCD_HEIGHT - 1);
		dc.MoveTo(LCD_WIDTH - crosswidth - 1, LCD_HEIGHT - 1);
		dc.LineTo(LCD_WIDTH - 1, LCD_HEIGHT - crosswidth - 1);
		dc.TextOut(32,60,"请准确点击右");
		dc.TextOut(32,78,"下角叉字中心"); 
		while(PenLogicalState != SMPENDOWN);
		penx1 = PhysicalX;
		peny1 = PhysicalY;
		while(PenLogicalState != SMPENUP);
		CDC::ClearDevice();
	extern SYSTEM_OPTION g_SystemOption;
		if(peny0 == peny1 ||penx0 == penx1) // hqf
			continue; 
	#ifdef	COORDINATE_ROTATION
		g_SystemOption.KxNom = lcdx0 - lcdx1;
		g_SystemOption.KxDenom = peny0 - peny1;
		g_SystemOption.KyNom = lcdy0 - lcdy1;
		g_SystemOption.KyDenom = penx0 - penx1;
		g_SystemOption.Dx = (peny0 * lcdx1 - lcdx0 * peny1) / (peny0 - peny1);
		g_SystemOption.Dy = (penx0 * lcdy1 - lcdy0 * penx1) / (penx0 - penx1);
	#else
		g_SystemOption.KxNom = lcdx0 - lcdx1;
		g_SystemOption.KxDenom = penx0 - penx1;
		g_SystemOption.KyNom = lcdy0 - lcdy1;
		g_SystemOption.KyDenom = peny0 - peny1;
		g_SystemOption.Dx = (penx0 * lcdx1 - lcdx0 * penx1) / (penx0 - penx1);
		g_SystemOption.Dy = (peny0 * lcdy1 - lcdy0 * peny1) / (peny0 - peny1);
	#endif

		CDC::ClearDevice();
		dc.MoveTo(0, LCD_HEIGHT - crosswidth - 1);
		dc.LineTo(crosswidth, LCD_HEIGHT - 1);
		dc.MoveTo(0, LCD_HEIGHT - 1);
		dc.LineTo(crosswidth, LCD_HEIGHT - crosswidth - 1);
		dc.TextOut(32,60,"请准确点击左");
		dc.TextOut(32,78,"下角叉字中心"); 
		while(PenLogicalState != SMPENDOWN);
		// Convert physical coordinates to logical
		if(g_SystemOption.KxDenom + g_SystemOption.Dx ==0 ||  // hqf 
			g_SystemOption.KyDenom + g_SystemOption.Dy==0)
			continue;
	#ifdef	COORDINATE_ROTATION
		lcdx2 = g_SystemOption.KxNom * PhysicalY / g_SystemOption.KxDenom + g_SystemOption.Dx;
		lcdy2 = g_SystemOption.KyNom * PhysicalX / g_SystemOption.KyDenom + g_SystemOption.Dy;
	#else
		lcdx2 = g_SystemOption.KxNom * PhysicalX / g_SystemOption.KxDenom + g_SystemOption.Dx;
		lcdy2 = g_SystemOption.KyNom * PhysicalY / g_SystemOption.KyDenom + g_SystemOption.Dy;
	#endif
		if (ABS(lcdx2 - crosswidth/2) >= CALIBRATION_ERROR ||
			ABS(lcdy2 - (LCD_HEIGHT - crosswidth/2)) >= CALIBRATION_ERROR)
			ok = false;
		else
			ok = true;
		while(PenLogicalState != SMPENUP);
	}    
	// restore screen
	CDC::PutScreen(save);
    if(bIfOpenCursor)
		OpenCursor();
	CalibrationOK = 1;
	::OpenRtcInterrupt(RTCI_SECOND);
//	if(pCurWindow)
//	{
		
//		pCurWindow->SendMessage(WM_ACTIVE , 0, 0);
//	}
	if(!bHardRingEnabled)
		DisableHardwareRing();//ygr 1-15
}


//----------------------- output interface --------------------


///////////////////////////////////////////////////////////
//
//	Test if valid point(jumping point)
//
//	Parameters:	none
//
//	Return:		TRUE = valid point
//				FALSE = invalid point
//
///////////////////////////////////////////////////////////
/*
PRIVATE bool ValidPoint()
{
	int		logicalx, logicaly;
	bool	ret;
	
	// Convert physical coordinates to logical
	if((g_SystemOption.KxDenom == 0) || (g_SystemOption.KyDenom == 0))
			return false;
	#ifdef	COORDINATE_ROTATION
	logicalx = g_SystemOption.KxNom * PhysicalY / g_SystemOption.KxDenom + g_SystemOption.Dx;
	logicaly = g_SystemOption.KyNom * PhysicalX / g_SystemOption.KyDenom + g_SystemOption.Dy;
	#else
	logicalx = g_SystemOption.KxNom * PhysicalX / g_SystemOption.KxDenom + g_SystemOption.Dx;
	logicaly = g_SystemOption.KyNom * PhysicalY / g_SystemOption.KyDenom + g_SystemOption.Dy;
	#endif
	
	if (g_iLastLogicalX == -1 || g_iLastLogicalY == -1){
		g_iLastLogicalX = logicalx;
		g_iLastLogicalY = logicaly;
		return true;
	}
	else if (ABS(logicalx - g_iLastLogicalX) <= MOVE_STEP_MAX && ABS(logicaly - g_iLastLogicalY) <= MOVE_STEP_MAX)
		ret = true;
	else
		ret = false;

	g_iLastLogicalX = logicalx;
	g_iLastLogicalY = logicaly;

	return ret;
}
*/

///////////////////////////////////////////////////////////
//
//	Output state machine's state.
//
//	Parameters:
//	penmessage			pen message, use follows:
//						SMPENUP			pen up
//						SMPENDOWN		pen down
//						SMPENMOVE		pen move
//
///////////////////////////////////////////////////////////
PRIVATE void OutputState(int penmessage)
{
	int		logicalx, logicaly;
	STATUS	status;

	if (!CalibrationOK)
		return;

	// Convert physical coordinates to logical
	if((g_SystemOption.KxDenom == 0) || (g_SystemOption.KyDenom == 0))
			return;
	#ifdef	COORDINATE_ROTATION
	logicalx = g_SystemOption.KxNom * PhysicalY / g_SystemOption.KxDenom + g_SystemOption.Dx;
	logicaly = g_SystemOption.KyNom * PhysicalX / g_SystemOption.KyDenom + g_SystemOption.Dy;
	#else
	logicalx = g_SystemOption.KxNom * PhysicalX / g_SystemOption.KxDenom + g_SystemOption.Dx;
	logicaly = g_SystemOption.KyNom * PhysicalY / g_SystemOption.KyDenom + g_SystemOption.Dy;
	#endif

	// 去坏点
	int		vx, vy, ax, ay;
	if (penmessage == SMPENDOWN || penmessage == SMPENUP){
		g_iLastLogicalX = Penmsg.x = logicalx;
		g_iLastLogicalY = Penmsg.y = logicaly;
		g_iLastVx = 0;
		g_iLastVy = 0;
		g_bLastPointAbandon = false;
	}
	else{
		vx = logicalx - g_iLastLogicalX;
		vy = logicaly - g_iLastLogicalY;
		ax = vx - g_iLastVx;
		ay = vy - g_iLastVy;
		if (ABS(ax) > MOVE_ACCELERATION_MAX || ABS(ay) > MOVE_ACCELERATION_MAX){
			if (g_bLastPointAbandon){
				g_bLastPointAbandon = false;
			}
			else{
				g_bLastPointAbandon = true;
				return;
			}
		}
		g_iLastLogicalX = Penmsg.x = logicalx;
		g_iLastLogicalY = Penmsg.y = logicaly;
		g_iLastVx = vx;
		g_iLastVy = vy;
	}

	switch (penmessage){
		case SMPENUP:
			// regist "pen up" message
			Penmsg.type = SMPENUP;
			break;
		case SMPENDOWN:
			// regist "pen down" message
			Penmsg.type = SMPENDOWN;
			break;
		case SMPENMOVE:
			// regist "pen move" message
			Penmsg.type = SMPENMOVE;
			break;
		default:
			return;
	}
	
	// activate high level interrupt that send message to message queue
	status = NU_Activate_HISR (&CB_PenHISR);
}

///////////////////////////////////////////////////////////
//
//	Set touch pane sample rate.
//
//	Parameters:
//	frequency			sample frequency (output dots / per second)
//						Min: 5
//						Max: 128
//
///////////////////////////////////////////////////////////
PUBLIC void SetTouchPaneSampleRate(int frequency)
{
	if (frequency > 128 || frequency < 5)
		return;
	SampleClocks = 512 / frequency;
}

///////////////////////////////////////////////////////////
//
//	Enable send pen message to message queue.
//
//	Parameters:			none
//	return:				none
//
///////////////////////////////////////////////////////////
void EnablePenMessage(void)
{
	CalibrationOK = 1;
}

///////////////////////////////////////////////////////////
//
//	Disable send pen message to message queue.
//
//	Parameters:			none
//	return:				none
//
///////////////////////////////////////////////////////////
void DisablePenMessage(void)
{
	CalibrationOK = 0;
}
