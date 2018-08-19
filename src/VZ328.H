//////////////////////////////////////////
//										//
//	VZ328 寄存器定义文件				//
//										//
//		byw								//
//		2001/9/13						//
//										//
//	vz328.pdf, page 1_11				//
//										//
//////////////////////////////////////////

//////////////////////////////////////////
//	[history]
//	byw, 2001/11/28, modify 'RTCISR' name
//	byw, 2001/11/28, modify PWMCNT2's value

#ifndef		__VZ328_HEAD_FILE__
#define		__VZ328_HEAD_FILE__

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

#define		INBYTE(addr)			(*((unsigned char *)(addr)))
#define		OUTBYTE(addr, data)		(*((unsigned char *)(addr)) = (unsigned char)(data))
#define		INWORD(addr)			(*((unsigned short *)(addr)))
#define		OUTWORD(addr, data)		(*((unsigned short *)(addr)) = (unsigned short)(data))
#define		INLONG(addr)			(*((unsigned long *)(addr)))
#define		OUTLONG(addr, data)		(*((unsigned long *)(addr)) = (unsigned long)(data))

#define		SCR 		0xFFFFF000    // 8 System Control Register 0x1C 3-2
#define		PCR 		0xFFFFF003    // 8 Peripheral Control Register 0x00 3-3
#define		IDR 		0xFFFFF004    // 32 Silicon ID Register 0x56010000 3-4
#define		IODCR 		0xFFFFF008    // 16 I/O Drive Control Register 0xFFFF 3-5
#define		CSGBA 		0xFFFFF100    // 16 Chip Select Group A Base Register 0x0000 4-5
#define		CSGBB 		0xFFFFF102    // 16 Chip Select Group B Base Register 0x0000 4-5
#define		CSGBC 		0xFFFFF104    // 16 Chip Select Group C Base Register 0x0000 4-5
#define		CSGBD 		0xFFFFF106    // 16 Chip Select Group D Base Register 0x0000 4-5
#define		CSUGBA 		0xFFFFF108    // 16 Chip Select Upper Group Address Register 0x0000 4-8
#define		CSCR 		0xFFFFF10A    // 16 Chip Select Control Register 0x0000 4-18
#define		CSA 		0xFFFFF110    // 16 Group A Chip-Select Register 0x00E0 4-9
#define		CSB 		0xFFFFF112    // 16 Group B Chip-Select Register 0x0000 4-9
#define		CSC 		0xFFFFF114    // 16 Group C Chip-Select Register 0x0000 4-9
#define		CSD 		0xFFFFF116    // 16 Group D Chip-Select Register 0x0200 4-9
#define		EMUCS 		0xFFFFF118    // 16 Emulation Chip-Select Register 0x0060 4-17
#define		CGMCR 		0xFFFFF200    // 16 PLL Control Register 0x2430 5-7
#define		CGMFSR 		0xFFFFF202    // 16 CGM Frequency Select Register 0x0123 5-9
//#define	RES 		0xFFFFF204    // — Reserved — —
#define		PCTLR 		0xFFFFF207    // 8 Power Control Register 0x1F 5-14
#define		IVR 		0xFFFFF300    // 8 Interrupt Vector Register 0x00 6-7
#define		ICR 		0xFFFFF302    // 16 Interrupt Control Register 0x0000 6-8
#define		IMR 		0xFFFFF304    // 32 Interrupt Mask Register 0x00FFFFFF 6-11
//#define	RES 		0xFFFFF308    // 32 Reserved — —
#define		ISR 		0xFFFFF30C    // 32 Interrupt Status Register 0x00000000 6-14
#define		IPR 		0xFFFFF310    // 32 Interrupt Pending Register 0x00000000 6-19
#define		ILCR 		0xFFFFF314    // 16 Interrupt Level Control register 0x6533 6-24
#define		PADIR 		0xFFFFF400    // 8 Port A Direction Register 0x00 7-4
#define		PADATA 		0xFFFFF401    // 8 Port A Data Register 0x00 7-4
#define		PAPUEN 		0xFFFFF402    // 8 Port A Pull-Up Enable Register 0xFF 7-4
//#define	RES 		0xFFFFF403    // 8 Reserved — —
#define		PBDIR 		0xFFFFF408    // 8 Port B Direction Register 0x00 7-7
#define		PBDATA 		0xFFFFF409    // 8 Port B Data Register 0x00 7-7
#define		PBPUEN 		0xFFFFF40A    // 8 Port B Pull-Up Enable Register 0xFF 7-7
#define		PBSEL 		0xFFFFF40B    // 8 Port B Select Register 0xFF 7-7
#define		PCDIR 		0xFFFFF410    // 8 Port C Direction Register 0x00 7-10
#define		PCDATA 		0xFFFFF411    // 8 Port C Data Register 0x00 7-10
#define		PCPDEN 		0xFFFFF412    // 8 Port C Pull-Down Enable Register 0xFF 7-10
#define		PCSEL 		0xFFFFF413    // 8 Port C Select Register 0xFF 7-10
#define		PDDIR 		0xFFFFF418    // 8 Port D Direction Register 0x00 7-14
#define		PDDATA 		0xFFFFF419    // 8 Port D Data Register 0x00 7-14
#define		PDPUEN 		0xFFFFF41A    // 8 Port D Pull-Up Enable Register 0xFF 7-14
#define		PDSEL 		0xFFFFF41B    // 8 Port D Select Register 0xF0 7-14
#define		PDPOL 		0xFFFFF41C    // 8 Port D Polarity Register 0x00 7-14
#define		PDIRQEN 	0xFFFFF41D    // 8 Port D Interrupt Request Enable Register 0x00 7-14
#define		PDKBEN 		0xFFFFF41E    // 8 Port D Keyboard Enable Register 0x00 7-14
#define		PDIRQEG 	0xFFFFF41F    // 8 Port D Interrupt Request Edge Register 0x00 7-14
#define		PEDIR 		0xFFFFF420    // 8 Port E Direction Register 0x00 7-22
#define		PEDATA 		0xFFFFF421    // 8 Port E Data Register 0x00 7-22
#define		PEPUEN 		0xFFFFF422    // 8 Port E Pull-Up Enable Register 0xFF 7-22
#define		PESEL 		0xFFFFF423    // 8 Port E Select Register 0xFF 7-22
#define		PFDIR 		0xFFFFF428    // 8 Port F Direction Register 0x00 7-26
#define		PFDATA 		0xFFFFF429    // 8 Port F Data Register 0x00 7-26
#define		PFPUEN 		0xFFFFF42A    // 8 Port F Pull-Up Enable Register 0xFF 7-28
#define		PFSEL 		0xFFFFF42B    // 8 Port F Select Register 0x00 7-29
#define		PGDIR 		0xFFFFF430    // 8 Port G Direction Register 0x00 7-30
#define		PGDATA 		0xFFFFF431    // 8 Port G Data Register 0x00 7-31
#define		PGPUEN 		0xFFFFF432    // 8 Port G Pull-Up Enable Register 0x3D 7-33
#define		PGSEL 		0xFFFFF433    // 8 Port G Select Register 0x08 7-34
#define		PJDIR 		0xFFFFF438    // 8 Port J Direction Register 0x00 7-35
#define		PJDATA 		0xFFFFF439    // 8 Port J Data Register 0x00 7-35
#define		PJPUEN 		0xFFFFF43A    // 8 Port J Pull-Up Enable Register 0xFF 7-38
#define		PJSEL 		0xFFFFF43B    // 8 Port J Select Register 0x00 7-38
#define		PKDIR 		0xFFFFF440    // 8 Port K Direction Register 0x00 7-39
#define		PKDATA 		0xFFFFF441    // 8 Port K Data Register 0x00 7-41
#define		PKPUEN 		0xFFFFF442    // 8 Port K Pull-Up Enable Register 0xFF 7-42
#define		PKSEL 		0xFFFFF443    // 8 Port K Select Register 0x00 7-42
#define		PMDIR 		0xFFFFF448    // 8 Port M Direction Register 0x00 7-43
#define		PMDATA 		0xFFFFF449    // 8 Port M Data Register 0x00 7-45
#define		PMPUEN 		0xFFFFF44A    // 8 Port M Pull-Up Enable Register 0x3F 7-46
#define		PMSEL 		0xFFFFF44B    // 8 Port M Select Register 0x00 7-46
#define		PWMC1 		0xFFFFF500    // 16 PWM Unit 1 Control Register 0x0020 8-4
#define		PWMS1 		0xFFFFF502    // 16 PWM Unit 1 Sample Register 0xxxxx 8-6
#define		PWMP1 		0xFFFFF504    // 8 PWM Unit 1 Period Register 0xFE 8-7
#define		PWMCNT1 	0xFFFFF505    // 8 PWM Unit 1 Counter Register 0x00 8-7
//#define	RES 		0xFFFFF506    // 16 Reserved — —
#define		PWMC2 		0xFFFFF510    // 16 PWM Unit 2 Control Register 0x0000 8-8
#define		PWMP2 		0xFFFFF512    // 16 PWM Unit 2 Period Register 0x0000 8-10
#define		PWMW2 		0xFFFFF514    // 16 PWM Unit 2 Width Register 0x0000 8-10
#define		PWMCNT2 	0xFFFFF516    // 16 PWM Unit 2 Counter Register 0x0000 8-11
#define		TCTL1 		0xFFFFF600    // 16 Timer Unit 1 Control Register 0x0000 9-5
#define		TPRER1 		0xFFFFF602    // 16 Timer Unit 1 Prescaler Register 0x0000 9-7
#define		TCMP1 		0xFFFFF604    // 16 Timer Unit 1 Compare Register 0xFFFF 9-8
#define		TCR1 		0xFFFFF606    // 16 Timer Unit 1 Capture Register 0x0000 9-9
#define		TCN1 		0xFFFFF608    // 16 Timer Unit 1 Counter Register 0x0000 9-10
#define		TSTAT1 		0xFFFFF60A    // 16 Timer Unit 1 Status Register 0x0000 9-11
#define		TCTL2 		0xFFFFF610    // 16 Timer Unit 2 Control Register 0x0000 9-6
#define		TPRER2 		0xFFFFF612    // 16 Timer Unit 2 Prescaler Register 0x0000 9-8
#define		TCMP2 		0xFFFFF614    // 16 Timer Unit 2 Compare Register 0xFFFF 9-9
#define		TCR2 		0xFFFFF616    // 16 Timer Unit 2 Capture Register 0x0000 9-10
#define		TCN2 		0xFFFFF618    // 16 Timer Unit 2 Counter Register 0x0000 9-10
#define		TSTAT2 		0xFFFFF61A    // 16 Timer Unit 2 Status Register 0x0000 9-12
#define		SPIRXD 		0xFFFFF700    // 16 SPI Unit 1Receive Data Register 0x0000 10-3
#define		SPITXD 		0xFFFFF702    // 16 SPI Unit 1Transmit Data Register 0x0000 10-3
#define		SPICONT1 	0xFFFFF704    // SPI Unit 1Control/Status Register 0x0000 10-4
#define		SPIDATA2 	0xFFFFF800    // 16 SPI Unit 2 Data Register 0x0000 11-4
#define		SPICONT2 	0xFFFFF802    // 16 SPI Unit 2 Control/Status Register 0x0000 11-5
#define		USTCNT1 	0xFFFFF900    // 16 UART Unit 1 Status/Control Register 0x0000 12-11
#define		UBAUD1 		0xFFFFF902    // 16 UART Unit 1 Baud Control Register 0x003F 12-14
#define		URX1 		0xFFFFF904    // 16 UART Unit 1 Receiver Register 0x0000 12-15
#define		UTX1 		0xFFFFF906    // 16 UART Unit 1 Transmitter Register 0x0000 12-17
#define		UMISC1 		0xFFFFF908    // 16 UART Unit 1 Miscellaneous Register 0x0000 12-19
#define		NIPR1 		0xFFFFF90A    // 16 UART Unit 1 Non-Integer Prescaler Register 0x0000 12-21
#define		USTCNT2 	0xFFFFF910    // 16 UART Unit 2 Status/Control Register 0x0000 13-11
#define		UBAUD2 		0xFFFFF912    // 16 UART Unit 2 Baud Control Register 0x003F 13-14
#define		URX2 		0xFFFFF914    // 16 UART Unit 2 Receiver Register 0x0000 13-15
#define		UTX2 		0xFFFFF916    // 16 UART Unit 2 Transmitter Register 0x0000 13-17
#define		UMISC2 		0xFFFFF918    // 16 UART Unit 2 Miscellaneous Register 0x0000 13-19
#define		NIPR2 		0xFFFFF91A    // 16 UART Unit 2 Non-Integer Prescaler Register 0x0000 13-21
#define		HMARK 		0xFFFFF91C    // 16 UART Unit 2 FIFO Half Mark Register 0x0000 13-22
#define		LSSA 		0xFFFFFA00    // 32 LCD Screen Starting Address Register 0x00000000 14-12
#define		LVPW 		0xFFFFFA05    // 8 LCD Virtual Page Width Register 0xFF 14-13
#define		LXMAX 		0xFFFFFA08    // 16 LCD Screen Width Register 0x03FF 14-13
#define		LYMAX 		0xFFFFFA0A    // 16 LCD Screen Height Register 0x01FF 14-14
#define		LCXP 		0xFFFFFA18    // 16 LCD Cursor X Position Register 0x0000 14-14
#define		LCYP 		0xFFFFFA1A    // 16 LCD Cursor Y Position Register 0x0000 14-14
#define		LCWCH 		0xFFFFFA1C    // 16 LCD Cursor Width and Height Register 0x0101 14-15
#define		LBLKC 		0xFFFFFA1F    // 8 LCD Blink Control Register 0x7F 14-15
#define		LPICF 		0xFFFFFA20    // 8 LCD Panel Interface Configuration Register 0x00 14-16
#define		LPOLCF 		0xFFFFFA21    // 8 LCD Polarity Configuration Register 0x00 14-17
#define		LACDRC 		0xFFFFFA23    // 8 LACD Rate Control Register 0x00 14-18
#define		LPXCD 		0xFFFFFA25    // 8 LCD Pixel Clock Divider Register 0x00 14-18
#define		LCKCON 		0xFFFFFA27    // 8 LCD Clocking Control Register 0x40 14-19
#define		LRRA 		0xFFFFFA29    // 8 LCD Refresh Rate Adjustment Register 0xFF 14-19
//#define	RES 		0xFFFFFA2B    // 8 Reserved — —
#define		LPOSR 		0xFFFFFA2D    // 8 LCD Panning Offset Register 00 14-20
#define		LFRCM 		0xFFFFFA31    // 8 LCD Frame Rate Control Modulation Register 0xB9 14-21
#define		LGPMR 		0xFFFFFA33    // 8 LCD Gray Palette Mapping Register 0x84 14-21
#define		PWMR 		0xFFFFFA36    // 16 PWM Contrast Control Register 0x0000 14-21
#define		RMCR 		0xFFFFFA39    // 8 Refresh Mode Control Register 0x00 14-22
#define		DMACR 		0xFFFFFA36    // 8 DMA Control Register 0x62 14-23
#define		RTCTIME 	0xFFFFFB00    // 32 RTC Time of Day Register 0x00000000 15-4
#define		RTCALRM 	0xFFFFFB04    // 32 RTC Alarm Register 0x00000000 15-4
#define		WATCHDOG 	0xFFFFFB0A    // 16 Watchdog Timer Register 0x0001 15-5
#define		RTCCTL 		0xFFFFFB0C    // 8 RTC Control Register 0x00 15-11
#define		RTCISR  	0xFFFFFB0E    // 16 RTC Interrupt Status Register 0x0000 15-12
#define		RTCIENR 	0xFFFFFB10    // 16 RTC Interrupt Enable Register 0x0000 15-14
#define		STPWCH 		0xFFFFFB12    // 8 Stopwatch Minutes Register 0x00 15-16
#define		DAYR 		0xFFFFFB1A    // 16 RTC Day Count Register 0x0xxx 15-7
#define		DAYALARM 	0xFFFFFB1C    // 16 RTC Day Alarm Register 0x0000 15-9
#define		DRAMMC 		0xFFFFFC00    // 16 DRAM Memory Configuration Register 0x00000000 16-10
#define		DRAMC 		0xFFFFFC02    // 16 DRAM Control Register 0x00000000 16-12
#define		SDCTRL 		0xFFFFFC04    // 16 SDRAM Control Register 0x00000000 16-14
#define		SDPWDN 		0xFFFFFC06    // 16 SDRAM Power Down Register 0x00000000 16-16
//#define	RES 		0xFFFFFC80    // — Reserved — —
#define		ICEMACR 	0xFFFFFD00    // 32 ICEM Address Compare Register 0x00000000 17-5
#define		ICEMAMR 	0xFFFFFD04    // 32 ICEM Address Mask Register 0x00000000 17-5
#define		ICEMCCR 	0xFFFFFD08    // 16 ICEM Control Compare Register 0x0000 17-6
#define		ICEMCMR 	0xFFFFFD0A    // 16 ICEM Control Mask Register 0x0000 17-6
#define		ICEMCR 		0xFFFFFD0C    // 16 ICEM Control Register 0x0000 17-7
#define		ICEMSR 		0xFFFFFD0E    // 16 ICEM Status Register 0x0000 17-9
//0xFFFFFExx Bootloader — Bootload// er Microcode Space — –

#endif
