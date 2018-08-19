#ifndef _E201_REGISTER_H
#define _E201_REGISTER_H


#define SBIT0	(0X01)
#define SBIT1	(0X02)
#define SBIT2	(0X04)
#define SBIT3	(0X08)
#define SBIT4	(0X10)
#define SBIT5	(0X20)
#define SBIT6	(0X40)
#define SBIT7	(0X80)
#define MBIT0	(0XFE)
#define MBIT1	(0XFD)
#define MBIT2	(0XFB)
#define MBIT3	(0XF7)
#define MBIT4	(0XEF)
#define MBIT5	(0XDF)
#define MBIT6	(0XBF)
#define MBIT7	(0X7F)

#define PBDIR			(*(UINT8  *)0xFFFFF408)
#define PBDATA			(*(UINT8  *)0xFFFFF409)
#define PBPUL			(*(UINT8  *)0xFFFFF40A)
#define PBSEL			(*(UINT8  *)0xFFFFF40B)

#define PDDIR			(* ((UINT8  *) 0xFFFFF418))
#define PDDATA			(* ((UINT8  *) 0xFFFFF419))
#define PDPUL			(* ((UINT8  *) 0xFFFFF41A))
#define PDSEL			(* ((UINT8  *) 0xFFFFF41B))
#define PDPOL			(* ((UINT8  *) 0xFFFFF41C))
#define PDIRQEN			(* ((UINT8  *) 0xFFFFF41D))
#define PDKBEN			(* ((UINT8  *) 0xFFFFF41E))
#define PDIRQEG			(* ((UINT8  *) 0xFFFFF41F))

#define	PEDIR			(*(UINT8  *)0XFFFFF420)
#define	PEDATA			(*(UINT8  *)0XFFFFF421)
#define	PEPUL			(*(UINT8  *)0XFFFFF422)
#define	PESEL			(*(UINT8  *)0XFFFFF423)

#define	PFDIR			(*(UINT8  *)0XFFFFF428)
#define	PFDATA			(*(UINT8  *)0XFFFFF429)
#define	PFPUL			(*(UINT8  *)0XFFFFF42A)
#define	PFSEL			(*(UINT8  *)0XFFFFF42B)

#define	PGDIR			(*(UINT8  *)0XFFFFF430)
#define	PGDATA			(*(UINT8  *)0XFFFFF431)
#define	PGPUL			(*(UINT8  *)0XFFFFF432)
#define	PGSEL			(*(UINT8  *)0XFFFFF433)

#define	PJDIR			(*(UINT8  *)0XFFFFF438)
#define	PJDATA			(*(UINT8  *)0XFFFFF439)
#define	PJPUL			(*(UINT8  *)0XFFFFF43A)
#define	PJSEL			(*(UINT8  *)0XFFFFF43B)

#define	PKDIR			(*(UINT8  *)0XFFFFF440)
#define	PKDATA			(*(UINT8  *)0XFFFFF441)
#define	PKPUL			(*(UINT8  *)0XFFFFF442)
#define	PKSEL			(*(UINT8  *)0XFFFFF443)

#define	PMDIR			(*(UINT8  *)0XFFFFF448)
#define	PMDATA			(*(UINT8  *)0XFFFFF449)
#define	PMPUL			(*(UINT8  *)0XFFFFF44A)
#define	PMSEL			(*(UINT8  *)0XFFFFF44B)

#define SPIRXD			(*(UINT8  *)0xFFFFF801)
#define SPIRXD16		(*(UINT16 *)0XFFFFF800)
#define SPITXD			(*(UINT8  *)0XFFFFF801)
#define SPITXD16		(*(UINT16 *)0XFFFFF800)
#define SPICONT			(*(UINT16 *)0XFFFFF802)

#define PWMC2					(*(UINT16 *) 0XFFFFF510) 
#define PWMP2					(*(UINT16 *) 0XFFFFF512)
#define PWMW2					(*(UINT16 *) 0XFFFFF514)
#define PWMCNT2					(*(UINT8  *) 0xFFFFF516)
		
#define PWMC1					(*(UINT16 *) 0XFFFFF500) 
#define PWMS1_16				(*(UINT16 *) 0XFFFFF502)
#define PWMS1_8					(*(UINT8  *) 0XFFFFF503)
#define PWMP1					(*(UINT8  *) 0XFFFFF504)
#define PWMCNT1					(*(UINT8  *) 0xFFFFF505)
	

// Timer2 Registers 
#define	TCTL2			(*(UINT16 *)0xFFFFF610) // Control Reg 
#define	TPRER2			(*(UINT16 *)0XFFFFF612) // Prescalar Reg 
#define	TCMP2			(*(UINT16 *)0XFFFFF614) // Compare Reg 
#define	TCR2			(*(UINT16 *)0XFFFFF616) // Capture Reg 
#define	TCN2			(*(UINT16 *)0XFFFFF618) // Counter 
#define	TSTAT2			(*(UINT16 *)0XFFFFF61A) // Status Reg 

#define ICR				(*(UINT16 *)0XFFFFF302)
#define IMR				(*(UINT32 *)0XFFFFF304)
#define ISR				(*(UINT32 *)0XFFFFF30C)
#define ILCR			(*(UINT16 *)0XFFFFF314)

//RTC Register
#define RTCTIME 		(*(UINT32 *)0xFFFFFB00)
#define ALMTIME 		(*(UINT32 *)0xFFFFFB04)
#define RTCCTL			(*(UINT16 *)0xFFFFFB0C)
#define RTCISR  		(*(UINT16 *)0xFFFFFB0E)
#define RTCIENR 		(*(UINT16 *)0xFFFFFB10)
#define STPWCH			(*(UINT16 *)0XFFFFFB12)
#define DAYR    		(*(UINT16 *)0xFFFFFB1A)
#define ALMDAY  		(*(UINT16 *)0xFFFFFB1C)


//SYS_COLCK Register
#define PCTLR	(*(UINT8  *)	0XFFFFF207)
#define PLLCR	(*(UINT16 *)	0xFFFFF200)

//SDRAM Register
#define DRAMC	(*(UINT16 *)	0XFFFFFC02)
#define SDCTRL  (*(UINT16 *)	0XFFFFFC04)
#define SDPWDN	(*(UINT16 *)	0XFFFFFC06)  

//lcd controler register 
#define LCKCON	(*(UINT8 *)		0XFFFFFA27)			

#define WATCHDOG (*(UINT16 *)	0XFFFFFB0A)


#endif		
		
		
		