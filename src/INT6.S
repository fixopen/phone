;	.xdef  _6Level_INT
;
;_6Level_INT:
;	BTST.B #1,0xFFFFF30F
;	BEQ _E201_otherlisr
;	JMP _INT_Timer_Interrupt
;_E201_otherlisr:
;	LINK A6,#0
;	MOVEM.L D0-D7/A0-A6,-(A7)           ; Save all registers
;   MOVEM.L (A7)+,D0-D7/A0-A6           ; Restore all registers
;	UNLK A6
;	RTE

;*******************************************ygr>*******


;/*************************************************************************/
;/*
;/*文件名：	LISR6.s
;/*
;/*作者；余国荣
;/*
;/*
;/*描述：
;/*		截获6级中断(for PWM1)与2级中断(for timer2)的中断服务程序
;/*函数：
;/*		1. void LEVEL6_LISR(void)
;/*		2. void LEVEL2_LISR(void)
;/*关联：
;/*	calls:
;/*		record.cpp 的 void PWM1LISR(int vector)
;/*		record.cpp 的 void LISR_ADSample(int vector)
;/*历史：
;/*
;/*
;/*************************************************************************/

	.xdef _LEVEL6_LISR
	.xdef _LEVEL2_LISR
	.xref _PWM1LISR
 
_LEVEL6_LISR:
	LINK A6,#0
    MOVEM.L D0-D7/A0-A6,-(A7)           ; Save all registers
	TST.B	0xFFFFF30F.W
	BMI.B	level6_lisr_else:
    MOVEM.L (A7)+,D0-D7/A0-A6           ; Restore all registers
	UNLK A6
	JMP _INT_Timer_Interrupt:
level6_lisr_else:
	MOVE.L #0,-(A7)
	jSR   _PWM1LISR
	ADDQ.L #4,A7
;	CLR.W	0xFFFFF61A.W
_record_LISR_END:
    MOVEM.L (A7)+,D0-D7/A0-A6           ; Restore all registers
	UNLK A6
	RTE

	.xref _LISR_ADSample
_LEVEL2_LISR:
	LINK A6,#0
    MOVEM.L D0-D7/A0-A6,-(A7)           ; Save all registers
;	MOVE.L #66,-(A7)
	JSR   _LISR_ADSample
;	ADDQ.L #4,A7
    MOVEM.L (A7)+,D0-D7/A0-A6           ; Restore all registers
	UNLK A6
	RTE

	.xdef _NoMemUseDelay
_NoMemUseDelay:
	move.l	#0,d0
DelayReg:
    addi.l  #1,d0
    cmp.l   #$f000,d0
    bne     DelayReg
	rts
