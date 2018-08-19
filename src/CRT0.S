lcd_start      equ     $380000
 .xref _lamp

MON_STACKTOP	EQU	$DDA		;  best place for the stack
******************************************************************************
*       Register for MC68EZ328
****************************************************************************** 
M328BASE  equ $FFFFF000 
 
; SIM28 System Configuration Registers 
SCR             equ     (M328BASE+$000)

IDR             equ     (M328BASE+$004)
 
; Chip Select Registers 
GRPBASEA        equ     (M328BASE+$100) 
GRPBASEB        equ     (M328BASE+$102)
GRPBASEC        equ     (M328BASE+$104)
GRPBASED        equ     (M328BASE+$106)
CSA             equ     (M328BASE+$110)
CSB             equ     (M328BASE+$112)
CSC             equ     (M328BASE+$114)
CSD             equ     (M328BASE+$116)
CSCR            equ     (M328BASE+$10A)
DRAMCFG         equ     (M328BASE+$C00)
DRAMMC          equ     (M328BASE+$C00)
DRAMCTL         equ     (M328BASE+$C02)
DRAMC           equ     (M328BASE+$C02)
SDCTRL          equ     (M328BASE+$C04)
EMUCS           equ     (M328BASE+$118)
CSCTR           equ     (M328BASE+$150)

; PLL Registers 
PLLCR           equ     (M328BASE+$200) ; Control Reg 
PLLFSR          equ     (M328BASE+$202) ; Freq Select Reg 
PLLTSR          equ     (M328BASE+$204) ; Test Reg 
 
; Power Control Registers 
PCTLR           equ     (M328BASE+$206) ; Control Reg 
 
; Interrupt Registers 
IVR             equ     (M328BASE+$300) ; Interrupt Vector Reg 
ICR             equ     (M328BASE+$302) ; Interrupt Control Reg 
IMR             equ     (M328BASE+$304) ; Interrupt Mask Reg 
ISR             equ     (M328BASE+$30C) ; Interrupt Status Reg 
IPR             equ     (M328BASE+$310) ; Interrupt Pending Reg 
 
; PIO Registers 
					; Port A Registers 
PADIR           equ     (M328BASE+$400) ; Direction Reg 
PADATA          equ     (M328BASE+$401) ; Data Reg 
PAPUEN          equ     (M328BASE+$402) ; Pullup Enable Reg 
					; Port B Registers 
PBDIR           equ     (M328BASE+$408) ; Direction Reg 
PBDATA          equ     (M328BASE+$409) ; Data Reg 
PBPUEN          equ     (M328BASE+$40A) ; Pullup Enable Reg 
PBSEL           equ     (M328BASE+$40B) ; Select Reg 
					; Port C Registers 
PCDIR           equ     (M328BASE+$410) ; Direction Reg 
PCDATA          equ     (M328BASE+$411) ; Data Reg 
PCPDEN          equ     (M328BASE+$412) ; Pull-down Enable Reg 
PCSEL           equ     (M328BASE+$413) ; Select Reg 
					; Port D Registers 
PDDIR           equ     (M328BASE+$418) ; Direction Reg 
PDDATA          equ     (M328BASE+$419) ; Data Reg 
PDPUEN          equ     (M328BASE+$41A) ; Pullup Enable Reg 
PDSEL           equ     (M328BASE+$41B) ; port D select     
PDPOL           equ     (M328BASE+$41C) ; Polarity Reg 
PDIRQEN         equ     (M328BASE+$41D) ; IRQ Enable Reg 
PDKBEN          equ     (M328BASE+$41E) ; Keyboard Enable Reg
PDIRQEDGE       equ     (M328BASE+$41F) ; IRQ Edge Reg 
					; Port E Registers 
PEDIR           equ     (M328BASE+$420) ; Direction Reg 
PEDATA          equ     (M328BASE+$421) ; Data Reg 
PEPUEN          equ     (M328BASE+$422) ; Pullup Enable Reg 
PESEL           equ     (M328BASE+$423) ; Select Reg 
					; Port F Registers 
PFDIR           equ     (M328BASE+$428) ; Direction Reg 
PFDATA          equ     (M328BASE+$429) ; Data Reg 
PFPUEN          equ     (M328BASE+$42A) ; Pullup Enable Reg 
PFSEL           equ     (M328BASE+$42B) ; Select Reg 
					; Port G Registers 
PGDIR           equ     (M328BASE+$430) ; Direction Reg 
PGDATA          equ     (M328BASE+$431) ; Data Reg 
PGPUEN          equ     (M328BASE+$432) ; Pullup Enable Reg 
PGSEL           equ     (M328BASE+$433) ; Select Reg 

PJDIR           equ     (M328BASE+$438)
PJDATA          equ     (M328BASE+$439) ; Date reg
PJPUEN          equ     (M328BASE+$43A)
PJSEL           equ     (M328BASE+$43B)

PKDIR           equ     (M328BASE+$440)
PKDATA          equ     (M328BASE+$441)
PKPUEN          equ     (M328BASE+$442)
PKSEL           equ     (M328BASE+$443) ; Select Reg 

PMDIR           equ     (M328BASE+$448)
PMDATA          equ     (M328BASE+$449)
PMPUEN          equ     (M328BASE+$44A)
PMSEL           equ     (M328BASE+$44B) ; Select Reg 

; PWM Registers 
PWMC            equ     (M328BASE+$500) ; Control Reg 
PWMS            equ     (M328BASE+$502) ; Sample Reg 
PWMCNT          equ     (M328BASE+$504) ; Count Reg 
 
; Timer Registers 
					; Timer 1 Registers 
TCTL            equ     (M328BASE+$600) ; Control Reg 
TPRER           equ     (M328BASE+$602) ; Prescalar Reg 
TCMP            equ     (M328BASE+$604) ; Compare Reg 
TCR             equ     (M328BASE+$606) ; Capture Reg 
TCN             equ     (M328BASE+$608) ; Counter 
TSTAT           equ     (M328BASE+$60A) ; Status Reg 
 
; SPI Registers 
SPIMDATA        equ     (M328BASE+$800) ; Control/Status Reg 
SPIMCONT        equ     (M328BASE+$802) ; Data Reg 
 
; UART Registers 
USTCNT          equ     (M328BASE+$900) ; Status Control Reg 
UBAUD           equ     (M328BASE+$902) ; Baud Control Reg 
UARTRX          equ     (M328BASE+$904) ; Rx Reg 
UARTTX          equ     (M328BASE+$906) ; Tx Reg 
UARTMISC        equ     (M328BASE+$908) ; Misc Reg 
UARTNIPR        equ     (M328BASE+$90A) ; None-Integer Prscaler reg

; LCDC Registers 
LSSA            equ     (M328BASE+$A00) ; Screen Start Addr Reg
LVPW            equ     (M328BASE+$A05) ; Virtual Page Width Reg
LXMAX           equ     (M328BASE+$A08) ; Screen Width Reg
LYMAX           equ     (M328BASE+$A0A) ; Screen Height Reg
LCXP            equ     (M328BASE+$A18) ; Cursor X Position
LCYP            equ     (M328BASE+$A1A) ; Cursor Y Position
LCWCH           equ     (M328BASE+$A1C) ; Cursor Width & Height Reg
LBLKC           equ     (M328BASE+$A1F) ; Blink Control Reg
LPICF           equ     (M328BASE+$A20) ; Panel Interface Config Reg
LPOLCF          equ     (M328BASE+$A21) ; Polarity Config Reg
LACDRC          equ     (M328BASE+$A23) ; ACD (M) Rate Control Reg
LPXCD           equ     (M328BASE+$A25) ; Pixel Clock Divider Reg
LCKCON          equ     (M328BASE+$A27) ; Clocking Control Reg
LRRA            equ     (M328BASE+$A29) ; Refresh Rate Adjust reg
LPOSR           equ     (M328BASE+$A2D) ; Panning Offset Reg
LFRCM           equ     (M328BASE+$A31) ; Frame Rate Control Mod Reg
LGPMR           equ     (M328BASE+$A33) ; Gray Palette Mapping Reg
LPWM            equ     (M328BASE+$A36) ; contrast control Reg
 
; RTC Registers 
RTCHMSR         equ     (M328BASE+$B00) ; Hrs Mins Secs Reg 
RTCALM0R        equ     (M328BASE+$B04) ; Alarm Register   
RTCDAY          equ     (M328BASE+$B08) ; RTC date reg 
RTCWD           equ     (M328BASE+$B0A) ; RTC watch dog timer reg
RTCCTL          equ     (M328BASE+$B0C) ; Control Reg 
RTCISR          equ     (M328BASE+$B0E) ; Interrupt Status Reg 
RTCIENR         equ     (M328BASE+$B10) ; Interrupt Enable Reg 
RSTPWCH         equ     (M328BASE+$B12) ; Stopwatch Minutes
 
;ICEM registers
ICEMACR         equ     (M328BASE+$D00) 
ICEMAMR         equ     (M328BASE+$D04) 
ICEMCCR         equ     (M328BASE+$D08) 
ICEMCMR         equ     (M328BASE+$D0A) 
ICEMCR          equ     (M328BASE+$D0C) 
ICEMSR          equ     (M328BASE+$D0E) 



;
;       C program assembly startup for an embedded environment.
;       -------------------------------------------------------
;
;       Written by Tomas Evensen 1993-04-17.
;       Copyright 1993 Diab Data AB.
;
	PSECT
	XDEF    start
	ALIGN   4
	nop
start:
;
; insert/modify code here to initialize (if necessary):
; - sp (stack pointer)
; - a5 (data pointer)
; - the cache
; - exception vectors
; - fpu control registers
;
; initialize sp to value from link editor command file
	move.l  #___SP_INIT,a7

; initialize a5 to sdata (provided by linker)
;        move.l  #__SDA_BASE_,a5


RESET_HARD:     
    ;************************************************
    ;* System initialization                        *
    ;************************************************
    ;* Replace the system initialization code here  *
    ;************************************************
    move.b  #$18,SCR            ; Disable Double Map
    move.b  #$9,PGSEL           ; config PG0/DTACK to GPI/O,input
    move.w  #$2480,PLLCR        ; ??MHz Sysclk, enable clko

    move.l  #MON_STACKTOP,A7    ; Install stack pointer
    move.w  #$2700,sr           ; mask off all interrupts
    move.w  #$00,RTCWD          ; disable watch dog
    move.w  #$08,ICEMCR         ; disable ICEM vector hardmap
    move.w  #$07,ICEMSR         ; clear level 7 interrupt

    ;******************************
    ;* Port Initialization        *
    ;******************************
    
    move.b  #$03,PFSEL          ; select A23-A20, CLKO, CSA1
	ori.b   #$01,PBDIR
	ANDI.b  #$FE,PBDATA			; ¹Ø±³¹â
    move.b  #$01,PBSEL          ; Config port B for chip select A,B,C and D
    move.b  #$00,PESEL          ; select *DWE 
    move.b  #$F1,PKSEL
    move.b  #$00,PMSEL

    ori.b   #$03,PDSEL
    ori.b   #$03,PDDIR
    ori.b   #$03,PDPUEN
    move.b  #$01,PDDATA
    
    ;******************************
    ;* Chip Select initialization *
    ;******************************
    ;**********************
    ; Flash
    ;**********************
    move.w  #$0800,GRPBASEA     ; GROUPA BASE(FLASH), Start add.=0x1000000          800 before
    move.w  #$0199,CSA          ; each 4mBYTE

    ;***********************************
    ; SDRAM delay for power safe (ygr)
    ;***********************************
    clr.l   d0
delay
    addi.l  #1,d0
    cmp.l   #$FFFF,d0
    bne     delay

    ;****************************
    ; SDRAM
    ;****************************
    move.w  #$0000,GRPBASED
    move.w  #$0281,CSD
    move.w  #$0040,CSCR         ; Chip Sel Control Reg
    move.w  #$0000,DRAMC        ; Disable DRAM Controller
    move.w  #$C03F,SDCTRL
    move.w  #$4020,DRAMMC
    move.w  #$8000,DRAMC
    
    move.w  #$C83F,SDCTRL       ; issue precharge comm
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    move.w  #$D03F,SDCTRL       ; enable refresh      
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    move.w  #$D43F,SDCTRL       ; issue mode command  
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    ;************************************************
    ;* End of System initialization                 *
    ;************************************************

    clr.l       d0
    clr.l       d1
    clr.l       d2
    clr.l       d3
    clr.l       d4
    clr.l       d5
    clr.l       d6
    clr.l       d7
 
    ;**********************************************
    ;* LCD Initization
    ;**********************************************
	ORI.B	#$20,PKSEL
	ORI.B	#$20,PKDIR
	ORI.B	#$20,PKPUEN
	ORI.B	#$20,PKDATA
	
    move.b   #0,PCSEL
    move.b   #0,PCPDEN

    move.l   #$180000,LSSA
    move.w   #160,LXMAX
    move.w   #159,LYMAX
    move.b   #20,LVPW
    move.b   #$09,LPICF
    move.b   #$01,LPOLCF           ;1 before
    move.b   #$00,LACDRC
    move.b   #$0a,LPXCD
    move.b   #$14,LRRA
    move.b   #$00,LPOSR
    move.b   #$00,LCKCON           ; disable LCDC
    move.b   #$80,LCKCON           ; enable LCDC, 0ws, 16-bit
    nop
    nop
    nop
	ORI.B     #$80,$FFFFF443;pk sel
	ORI.B     #$80,$FFFFF440;pk dir
    ORI.B     #$00,$FFFFF441;pd dat
    nop
    nop
    nop 
    
    clr.l   d0
        


;**********************************************************************
;
; Initialize I/O to avoid contention
;
;**********************************************************************
 

 
 
;**********************************************************************
;
; Program Interrupt Controller
;
;**********************************************************************
 
	move.b #$40,IVR
	move.l #$00FFFFFF,IMR           ;enable NMI interrupt 
 
	jsr     ___init_main
	move.l  d0,-(a7)
	jsr     _exit           ; will never return
	jsr     _main           ; dummy to pull in main() as soon as possible

	section .init$00,2,C    ; Jeff Barth per Diab instructions 3/9/99
	XDEF    ___init
	ALIGN   4
___init:

	section .fini,2,C
; Return from __init
	rts
	XDEF    ___fini
___fini:

	section .eini,2,C
; Return from ___fini
	rts

