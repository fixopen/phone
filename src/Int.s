;/*************************************************************************/
;/*                                                                       */
;/*        Copyright (c) 1993-1999 Accelerated Technology, Inc.           */
;/*                                                                       */
;/* PROPRIETARY RIGHTS of Accelerated Technology are involved in the      */
;/* subject matter of this material.  All manufacturing, reproduction,    */
;/* use, and sales rights pertaining to this subject matter are governed  */
;/* by the license agreement.  The recipient of this software implicitly  */
;/* accepts the terms of the license.                                     */
;/*                                                                       */
;/*************************************************************************/
;
;/*************************************************************************/
;/*                                                                       */
;/* FILE NAME                                            VERSION          */
;/*                                                                       */
;/*      int.s                                      PLUS/68328/1.11.3     */
;/*                                                                       */
;/* COMPONENT                                                             */
;/*                                                                       */
;/*      IN - Initialization                                              */
;/*                                                                       */
;/* DESCRIPTION                                                           */
;/*                                                                       */
;/*      This file contains the target processor dependent initialization */
;/*      routines and data.                                               */
;/*                                                                       */
;/* AUTHOR                                                                */
;/*                                                                       */
;/*      G. D. Fender, Accelerated Technology, Inc.                       */
;/*                                                                       */
;/* DATA STRUCTURES                                                       */
;/*                                                                       */
;/*      INT_Vectors                         Interrupt vector table       */
;/*                                                                       */
;/* FUNCTIONS                                                             */
;/*                                                                       */
;/*      INT_Initialize                      Target initialization        */
;/*      INT_Vectors_Loaded                  Returns a NU_TRUE if all the */
;/*                                            default vectors are loaded */
;/*      INT_Setup_Vector                    Sets up an actual vector     */
;/*                                                                       */
;/* DEPENDENCIES                                                          */
;/*                                                                       */
;/*      nucleus.h                           System constants             */
;/*                                                                       */
;/* HISTORY                                                               */
;/*                                                                       */
;/*         NAME            DATE                    REMARKS               */
;/*                                                                       */
;/*    G. Fender        09-21-1998      Created initial version 1.0       */
;/*                                       for 68328ADS port.              */
;/*    K. Pontzloff     03-12-1999      Renamed sections, changed the     */
;/*                                       interrupt register mask,        */
;/*                                       changed some BRA's to JMP's     */
;/*    D. Sharer        11-29-1999      Updated to support latest tools.  */
;/*                                                                       */
;/*************************************************************************/
	NAME	"int.s"
;#define         NU_SOURCE_FILE
;
;#include        "nucleus.h"                 /* System constants          */
;
;
;/* Define the global system stack variable.  This is setup by the 
;   initialization routine.  */
;
;extern VOID            *TCD_System_Stack;
;
        .xref    _TCD_System_Stack
;
;
;/* Define the global data structures that need to be initialized by this
;   routine.  These structures are used to define the system timer management
;   HISR.  */
;
;extern VOID     *TMD_HISR_Stack_Ptr;
;extern UNSIGNED  TMD_HISR_Stack_Size;
;extern INT       TMD_HISR_Priority;
;
        .xref    _TMD_HISR_Stack_Ptr
        .xref    _TMD_HISR_Stack_Size
        .xref    _TMD_HISR_Priority
;
;
;/* Define extern function references.  */
;
;VOID   INC_Initialize(VOID *first_available_memory);
;VOID   TCT_Interrupt_Context_Save(VOID);
;VOID   TCT_Interrupt_Context_Restore(VOID);
;VOID   TCC_Dispatch_LISR(INT vector_number);
;VOID   TMT_Timer_Interrupt(VOID);
;
        .xref    _INC_Initialize
        .xref    _TCT_Interrupt_Context_Save
        .xref    _TCT_Interrupt_Context_Restore
        .xref    _TCC_Dispatch_LISR
        .xref    _TMT_Timer_Interrupt
;
;
;/* Define the initialization flag that indicates whether or not all of the
;   default vectors have been loaded during initialization.  */
;
;INT    INT_Loaded_Flag;
;
        .data 
        .xdef    _INT_Loaded_Flag
_INT_Loaded_Flag        .align   2
        .skip    4
;
;/* Define the sizes for the system stack and the timer management stack.  */
;
SYS_STACK       .equ     16*1024                ; Size of system stack  2*1024

HISR_STACK      .equ     1024                 ; Size of Timer HISR stack 512
;
; Address locations assigned for the M68328 ADS hardware platform.

*****************************************************************************/


;/* Define the sections that allocate space for the system stack and possibly
;   the heap.  Also, define a section that is used to identify where the 
;   available system memory starts.  */
;
;        .section stack,,d 
;        .xdef    _stack_area
;_stack_area             .align   4
;        .skip    SYS_STACK
;
;        .section stack_top,,d 
;        .xdef    _stack_top
;_stack_top              .align   4
;        .skip    4

        .section sys_memory,,d 
        .xdef    _sys_memory
	
_sys_memory             .align   4
        .skip    4
         

;
;
;/* Define the interrupt vector table for the 68xxx family.  Note that this
;   vector table (or a copy of it) resides at address 0. In situations
;   where co-existence with a target monitor is required, this vector table
;   is typically not completely loaded.  The template interrupt vectors are
;   defined at the bottom of this file.  */
;
	.xref    start
        .section .vectors,,c
        .xdef    _INT_Vectors
_INT_Vectors:
        .long    STKTOP-4                ; Initial stack value       0
        .long    start                           ; unused                    1
        .long    _INT_Bus_Error              ; Bus Error                 2
        .long    _INT_Address_Error          ; Address Error             3
        .long    _INT_Illegal_Instruction    ; Illegal Instruction       4
        .long    _INT_Zero_Divide            ; Divide by zero            5
        .long    _INT_Chk_Chk2               ; CHK and CHK2 instructions 6
        .long    _INT_TrapV                  ; TRAPV instruction         7
        .long    _INT_Privilege              ; Privilege violation       8
        .long    _INT_Trace                  ; Trace                     9
        .long    _INT_Emul_A                 ; Emulate A                 10
        .long    _INT_Emul_B                 ; Emulate B                 11
        .long    _INT_Hdw_Breakpoint         ; Hardware breakpoint       12
        .long    _INT_Coprocessor_Violate    ; Coprocessor violation     13
        .long    _INT_Format_Error           ; Format error              14
        .long    _INT_Uninitialized_Int      ; Uninitialized interrupt   15
        .long    _INT_Reserved_0             ; Reserved 0                16
        .long    _INT_Reserved_1             ; Reserved 1                17
        .long    _INT_Reserved_2             ; Reserved 2                18
        .long    _INT_Reserved_3             ; Reserved 3                19
        .long    _INT_Reserved_4             ; Reserved 4                20
        .long    _INT_Reserved_5             ; Reserved 5                21
        .long    _INT_Reserved_6             ; Reserved 6                22
        .long    _INT_Reserved_7             ; Reserved 7                23
        .long    _INT_Spurious               ; Spurious interrupt        24
        .long    _INT_Level_1_Auto           ; Level 1 Autovector        25
        .long    _INT_Level_2_Auto           ; Level 2 Autovector        26
        .long    _INT_Level_3_Auto           ; Level 3 Autovector        27
        .long    _INT_Level_4_Auto           ; Level 4 Autovector        28
        .long    _INT_Level_5_Auto           ; Level 5 Autovector        29
        .long    _INT_Level_6_Auto           ; Level 6 Autovector        30
        .long    _INT_Level_7_Auto           ; Level 7 Autovector        31
        .long    _INT_Trap_0                 ; Trap 0                    32
        .long    _INT_Trap_1                 ; Trap 1                    33
        .long    _INT_Trap_2                 ; Trap 2                    34
        .long    _INT_Trap_3                 ; Trap 3                    35
        .long    _INT_Trap_4                 ; Trap 4                    36
        .long    _INT_Trap_5                 ; Trap 5                    37
        .long    _INT_Trap_6                 ; Trap 6                    38
        .long    _INT_Trap_7                 ; Trap 7                    39
        .long    _INT_Trap_8                 ; Trap 8                    40
        .long    _INT_Trap_9                 ; Trap 9                    41
        .long    _INT_Trap_10                ; Trap 10                   42
        .long    _INT_Trap_11                ; Trap 11                   43
        .long    _INT_Trap_12                ; Trap 12                   44
        .long    _INT_Trap_13                ; Trap 13                   45
        .long    _INT_Trap_14                ; Trap 14                   46
        .long    _INT_Trap_15                ; Trap 15                   47
        .long    _INT_Cop_Reserved_0         ; Coprocessor Reserved 0    48
        .long    _INT_Cop_Reserved_1         ; Coprocessor Reserved 1    49
        .long    _INT_Cop_Reserved_2         ; Coprocessor Reserved 2    50
        .long    _INT_Cop_Reserved_3         ; Coprocessor Reserved 3    51
        .long    _INT_Cop_Reserved_4         ; Coprocessor Reserved 4    52
        .long    _INT_Cop_Reserved_5         ; Coprocessor Reserved 5    53
        .long    _INT_Cop_Reserved_6         ; Coprocessor Reserved 6    54
        .long    _INT_Cop_Reserved_7         ; Coprocessor Reserved 7    55
        .long    _INT_Cop_Reserved_8         ; Coprocessor Reserved 8    56
        .long    _INT_Cop_Reserved_9         ; Coprocessor Reserved 9    57
        .long    _INT_Cop_Reserved_10        ; Coprocessor Reserved 10   58
        .long    _INT_Reserved_8             ; Reserved 8                59
        .long    _INT_Reserved_9             ; Reserved 9                60
        .long    _INT_Reserved_10            ; Reserved 10               61
        .long    _INT_Reserved_11            ; Reserved 11               62
        .long    _INT_Reserved_12            ; Reserved 12               63
        .long    _INT_User_0                 ; User Interrupt 0          64
        .long    _INT_User_1                 ; User Interrupt 1          65
        .long    _INT_User_2                 ; User Interrupt 2          66
        .long    _INT_User_3                 ; User Interrupt 3          67
        .long    _INT_User_4                 ; User Interrupt 4          68
        .long    _INT_User_5                 ; User Interrupt 5          69
        .long    _INT_User_6                 ; User Interrupt 6          70
        .long    _INT_User_7                 ; User Interrupt 7          71
        .long    _INT_User_8                 ; User Interrupt 8          72
        .long    _INT_User_9                 ; User Interrupt 9          73
        .long    _INT_User_10                ; User Interrupt 10         74
        .long    _INT_User_11                ; User Interrupt 11         75
        .long    _INT_User_12                ; User Interrupt 12         76
        .long    _INT_User_13                ; User Interrupt 13         77
        .long    _INT_User_14                ; User Interrupt 14         78
        .long    _INT_User_15                ; User Interrupt 15         79
        .long    _INT_User_16                ; User Interrupt 16         80
        .long    _INT_User_17                ; User Interrupt 17         81
        .long    _INT_User_18                ; User Interrupt 18         82
        .long    _INT_User_19                ; User Interrupt 19         83
        .long    _INT_User_20                ; User Interrupt 20         84
        .long    _INT_User_21                ; User Interrupt 21         85
        .long    _INT_User_22                ; User Interrupt 22         86
        .long    _INT_User_23                ; User Interrupt 23         87
        .long    _INT_User_24                ; User Interrupt 24         88
        .long    _INT_User_25                ; User Interrupt 25         89
        .long    _INT_User_26                ; User Interrupt 26         90
        .long    _INT_User_27                ; User Interrupt 27         91
        .long    _INT_User_28                ; User Interrupt 28         92
        .long    _INT_User_29                ; User Interrupt 29         93
        .long    _INT_User_30                ; User Interrupt 30         94
        .long    _INT_User_31                ; User Interrupt 31         95
        .long    _INT_User_32                ; User Interrupt 32         96
        .long    _INT_User_33                ; User Interrupt 33         97
        .long    _INT_User_34                ; User Interrupt 34         98
        .long    _INT_User_35                ; User Interrupt 35         99
        .long    _INT_User_36                ; User Interrupt 36         100
        .long    _INT_User_37                ; User Interrupt 37         101
        .long    _INT_User_38                ; User Interrupt 38         102
        .long    _INT_User_39                ; User Interrupt 39         103
        .long    _INT_User_40                ; User Interrupt 40         104
        .long    _INT_User_41                ; User Interrupt 41         105
        .long    _INT_User_42                ; User Interrupt 42         106
        .long    _INT_User_43                ; User Interrupt 43         107
        .long    _INT_User_44                ; User Interrupt 44         108
        .long    _INT_User_45                ; User Interrupt 45         109
        .long    _INT_User_46                ; User Interrupt 46         110
        .long    _INT_User_47                ; User Interrupt 47         111
        .long    _INT_User_48                ; User Interrupt 48         112
        .long    _INT_User_49                ; User Interrupt 49         113
        .long    _INT_User_50                ; User Interrupt 50         114
        .long    _INT_User_51                ; User Interrupt 51         115
        .long    _INT_User_52                ; User Interrupt 52         116
        .long    _INT_User_53                ; User Interrupt 53         117
        .long    _INT_User_54                ; User Interrupt 54         118
        .long    _INT_User_55                ; User Interrupt 55         119
        .long    _INT_User_56                ; User Interrupt 56         120
        .long    _INT_User_57                ; User Interrupt 57         121
        .long    _INT_User_58                ; User Interrupt 58         122
        .long    _INT_User_59                ; User Interrupt 59         123
        .long    _INT_User_60                ; User Interrupt 60         124
        .long    _INT_User_61                ; User Interrupt 61         125
        .long    _INT_User_62                ; User Interrupt 62         126
        .long    _INT_User_63                ; User Interrupt 63         127
        .long    _INT_User_64                ; User Interrupt 64         128
        .long    _INT_User_65                ; User Interrupt 65         129
        .long    _INT_User_66                ; User Interrupt 66         130
        .long    _INT_User_67                ; User Interrupt 67         131
        .long    _INT_User_68                ; User Interrupt 68         132
        .long    _INT_User_69                ; User Interrupt 69         133
        .long    _INT_User_70                ; User Interrupt 70         134
        .long    _INT_User_71                ; User Interrupt 71         135
        .long    _INT_User_72                ; User Interrupt 72         136
        .long    _INT_User_73                ; User Interrupt 73         137
        .long    _INT_User_74                ; User Interrupt 74         138
        .long    _INT_User_75                ; User Interrupt 75         139
        .long    _INT_User_76                ; User Interrupt 76         140
        .long    _INT_User_77                ; User Interrupt 77         141
        .long    _INT_User_78                ; User Interrupt 78         142
        .long    _INT_User_79                ; User Interrupt 79         143
        .long    _INT_User_80                ; User Interrupt 80         144
        .long    _INT_User_81                ; User Interrupt 81         145
        .long    _INT_User_82                ; User Interrupt 82         146
        .long    _INT_User_83                ; User Interrupt 83         147
        .long    _INT_User_84                ; User Interrupt 84         148
        .long    _INT_User_85                ; User Interrupt 85         149
        .long    _INT_User_86                ; User Interrupt 86         150
        .long    _INT_User_87                ; User Interrupt 87         151
        .long    _INT_User_88                ; User Interrupt 88         152
        .long    _INT_User_89                ; User Interrupt 89         153
        .long    _INT_User_90                ; User Interrupt 90         154
        .long    _INT_User_91                ; User Interrupt 91         155
        .long    _INT_User_92                ; User Interrupt 92         156
        .long    _INT_User_93                ; User Interrupt 93         157
        .long    _INT_User_94                ; User Interrupt 94         158
        .long    _INT_User_95                ; User Interrupt 95         159
        .long    _INT_User_96                ; User Interrupt 96         160
        .long    _INT_User_97                ; User Interrupt 97         161
        .long    _INT_User_98                ; User Interrupt 98         162
        .long    _INT_User_99                ; User Interrupt 99         163
        .long    _INT_User_100               ; User Interrupt 100        164
        .long    _INT_User_101               ; User Interrupt 101        165
        .long    _INT_User_102               ; User Interrupt 102        166
        .long    _INT_User_103               ; User Interrupt 103        167
        .long    _INT_User_104               ; User Interrupt 104        168
        .long    _INT_User_105               ; User Interrupt 105        169
        .long    _INT_User_106               ; User Interrupt 106        170
        .long    _INT_User_107               ; User Interrupt 107        171
        .long    _INT_User_108               ; User Interrupt 108        172
        .long    _INT_User_109               ; User Interrupt 109        173
        .long    _INT_User_110               ; User Interrupt 110        174
        .long    _INT_User_111               ; User Interrupt 111        175
        .long    _INT_User_112               ; User Interrupt 112        176
        .long    _INT_User_113               ; User Interrupt 113        177
        .long    _INT_User_114               ; User Interrupt 114        178
        .long    _INT_User_115               ; User Interrupt 115        179
        .long    _INT_User_116               ; User Interrupt 116        180
        .long    _INT_User_117               ; User Interrupt 117        181
        .long    _INT_User_118               ; User Interrupt 118        182
        .long    _INT_User_119               ; User Interrupt 119        183
        .long    _INT_User_120               ; User Interrupt 120        184
        .long    _INT_User_121               ; User Interrupt 121        185
        .long    _INT_User_122               ; User Interrupt 122        186
        .long    _INT_User_123               ; User Interrupt 123        187
        .long    _INT_User_124               ; User Interrupt 124        188
        .long    _INT_User_125               ; User Interrupt 125        189
        .long    _INT_User_126               ; User Interrupt 126        190
        .long    _INT_User_127               ; User Interrupt 127        191
        .long    _INT_User_128               ; User Interrupt 128        192
        .long    _INT_User_129               ; User Interrupt 129        193
        .long    _INT_User_130               ; User Interrupt 130        194
        .long    _INT_User_131               ; User Interrupt 131        195
        .long    _INT_User_132               ; User Interrupt 132        196
        .long    _INT_User_133               ; User Interrupt 133        197
        .long    _INT_User_134               ; User Interrupt 134        198
        .long    _INT_User_135               ; User Interrupt 135        199
        .long    _INT_User_136               ; User Interrupt 136        200
        .long    _INT_User_137               ; User Interrupt 137        201
        .long    _INT_User_138               ; User Interrupt 138        202
        .long    _INT_User_139               ; User Interrupt 139        203
        .long    _INT_User_140               ; User Interrupt 140        204
        .long    _INT_User_141               ; User Interrupt 141        205
        .long    _INT_User_142               ; User Interrupt 142        206
        .long    _INT_User_143               ; User Interrupt 143        207
        .long    _INT_User_144               ; User Interrupt 144        208
        .long    _INT_User_145               ; User Interrupt 145        209
        .long    _INT_User_146               ; User Interrupt 146        210
        .long    _INT_User_147               ; User Interrupt 147        211
        .long    _INT_User_148               ; User Interrupt 148        212
        .long    _INT_User_149               ; User Interrupt 149        213
        .long    _INT_User_150               ; User Interrupt 150        214
        .long    _INT_User_151               ; User Interrupt 151        215
        .long    _INT_User_152               ; User Interrupt 152        216
        .long    _INT_User_153               ; User Interrupt 153        217
        .long    _INT_User_154               ; User Interrupt 154        218
        .long    _INT_User_155               ; User Interrupt 155        219
        .long    _INT_User_156               ; User Interrupt 156        220
        .long    _INT_User_157               ; User Interrupt 157        221
        .long    _INT_User_158               ; User Interrupt 158        222
        .long    _INT_User_159               ; User Interrupt 159        223
        .long    _INT_User_160               ; User Interrupt 160        224
        .long    _INT_User_161               ; User Interrupt 161        225
        .long    _INT_User_162               ; User Interrupt 162        226
        .long    _INT_User_163               ; User Interrupt 163        227
        .long    _INT_User_164               ; User Interrupt 164        228
        .long    _INT_User_165               ; User Interrupt 165        229
        .long    _INT_User_166               ; User Interrupt 166        230
        .long    _INT_User_167               ; User Interrupt 167        231
        .long    _INT_User_168               ; User Interrupt 168        232
        .long    _INT_User_169               ; User Interrupt 169        233
        .long    _INT_User_170               ; User Interrupt 170        234
        .long    _INT_User_171               ; User Interrupt 171        235
        .long    _INT_User_172               ; User Interrupt 172        236
        .long    _INT_User_173               ; User Interrupt 173        237
        .long    _INT_User_174               ; User Interrupt 174        238
        .long    _INT_User_175               ; User Interrupt 175        239
        .long    _INT_User_176               ; User Interrupt 176        240
        .long    _INT_User_177               ; User Interrupt 177        241
        .long    _INT_User_178               ; User Interrupt 178        242
        .long    _INT_User_179               ; User Interrupt 179        243
        .long    _INT_User_180               ; User Interrupt 180        244
        .long    _INT_User_181               ; User Interrupt 181        245
        .long    _INT_User_182               ; User Interrupt 182        246
        .long    _INT_User_183               ; User Interrupt 183        247
        .long    _INT_User_184               ; User Interrupt 184        248
        .long    _INT_User_185               ; User Interrupt 185        249
        .long    _INT_User_186               ; User Interrupt 186        250
        .long    _INT_User_187               ; User Interrupt 187        251
        .long    _INT_User_188               ; User Interrupt 188        252
        .long    _INT_User_189               ; User Interrupt 189        253
        .long    _INT_User_190               ; User Interrupt 190        254
        .long    _INT_User_191               ; User Interrupt 191        255
;
;
;
        .section .text,,c
;/*************************************************************************/
;/*                                                                       */
;/* FUNCTION                                                              */
;/*                                                                       */
;/*      INT_Initialize                                                   */
;/*                                                                       */
;/* DESCRIPTION                                                           */
;/*                                                                       */
;/*      This function handles all low-level, target dependent            */
;/*      initialization.  Once this function is complete, control is      */
;/*      transfered to the target independent initialization function,    */
;/*      INC_Initialize.  Responsibilities of this function include the   */
;/*      following:
;/*                                                                       */
;/*             - Setup necessary processor/system control registers      */
;/*             - Initialize the vector table                             */
;/*             - Setup the system stack pointer                          */
;/*             - Setup the timer interrupt                               */
;/*             - Calculate the timer HISR stack and priority             */
;/*             - Calculate the first available memory address            */
;/*             - Transfer control to INC_Initialize to initialize all of */
;/*               the system components.                                  */
;/*                                                                       */
;/* AUTHOR                                                                */
;/*                                                                       */
;/*      G. D. Fender, Accelerated Technology, Inc.                          */
;/*                                                                       */
;/* CALLED BY                                                             */
;/*                                                                       */
;/*      Diab Data startup function                                       */
;/*                                                                       */
;/* CALLS                                                                 */
;/*                                                                       */
;/*      INC_Initialize                      Common initialization        */
;/*                                                                       */
;/* INPUTS                                                                */
;/*                                                                       */
;/*      None                                                             */
;/*                                                                       */
;/* OUTPUTS                                                               */
;/*                                                                       */
;/*      None                                                             */
;/*                                                                       */
;/* HISTORY                                                               */
;/*                                                                       */
;/*         NAME            DATE                    REMARKS               */
;/*                                                                       */
;/*      G. D. Fender    09-23-1998      Added clear BSS, 68328 timer code*/
;/*                                                                       */
;/*************************************************************************/
        .xdef    _INT_Initialize
        .xdef    _main
_INT_Initialize:
_main:
;VOID    INT_Initialize(void)
;{
;
;    /* Lockout interrupts during initialization.  */

      MOVE.W    #0x2700,SR           ; Lockout interrupts
      MOVE.W    #0x0,0xFFFFF618        ; kill watch dog timer
;
;  /*	Assign timer interrupt vector
	MOVE.B    #0x40,0xFFFFF300       ; Int Vect Reg - User Area = 0x100
	MOVE.L    #_INT_Timer_Interrupt,0x118   ; steal timer1 interrupt

;    /* BEGIN 68328 specific Code */

;   /* Copy the ROM vector table to RAM (address 0x0)  */

;      MOVE.L   #_INT_Vectors, A1
;      MOVE.L   #0,A2
;      MOVE.L   #256,D1
;_INT_Vect_Copy:
;      MOVE.L   (A1)+,D0
;      MOVE.L   D0,(A2)+
;      SUBQ.L   #1,D1
;      BNE      _INT_Vect_Copy

;    /* END 68328 specific Code */

;   /*  Clear the BSS area */
;
      MOVEA.L   #__BSS_START,A0      ; Pickup address of start of BSS area
      MOVEQ     #0,D0                ; Get a zero
BSS_Loop:
      CMPA.L    #__BSS_END,A0        ; Are we done?
      BEQ       BSS_Loop_Done        ; Yes, jump
      MOVE.L    D0,(A0)              ; Store a zip
      ADDQ.L    #4,A0                ; Bump Addr
      BRA       BSS_Loop             ; repeat
BSS_Loop_Done:
;
;    /* Setup the vectors loaded flag to indicate to other routines in the
;       system whether or not all of the default vectors have been loaded.
;       If INT_Loaded_Flag is 1, all of the default vectors have been loaded.
;       Otherwise, if INT_Loaded_Flag is 0, registering an LISR cause the
;       default vector to be loaded.  */
;    INT_Loaded_Flag =  0;
;
      MOVE.L    #0,_INT_Loaded_Flag  ; Indicate whether vector table is
                                       ; loaded (1) or not loaded (0)
;
;    /* Save the current value of the system stack.  */
;
;      CLR.L     -(A7)                ; Place a NULL on the system stack
;      MOVE.L    A7,_TCD_System_Stack ; Save system stack pointer

; Start insert new *********************************

        MOVE.L #_sys_memory ,A7               ; Get last available memory
        ADD.L   #SYS_STACK,A7               ; Add system stack size
        CLR.L   -(A7)                       ; Place a NULL on the system stack 
        MOVE.L  A7,_TCD_System_Stack        ; Save system stack pointer

;
;    /* Define the global data structures that need to be initialized by this
;       routine.  These structures are used to define the system timer
;       management HISR.  */
;    TMD_HISR_Stack_Ptr =        _sys_memory;
;    TMD_HISR_Stack_Size =       HISR_STACK;
;    TMD_HISR_Priority =         0;
;
      MOVE.L    A7 , D0      ; Put the available memory
                                       ; address in D0
      MOVE.L    D0,_TMD_HISR_Stack_Ptr ; Save the HISR stack pointer
      MOVE.L    #HISR_STACK,_TMD_HISR_Stack_Size ;   and stack size
      MOVE.L    #0,_TMD_HISR_Priority; HISR priority 0-2
      ADD.L     #HISR_STACK,D0       ; Position past the HISR stack
;
;
;     /* Call INC_Initialize with a pointer to the first available memory
;        address after the compiler's global data.  This memory may be used
;        by the application.  */
;     INC_Initialize(first_available_address);
;
;
;    /* 68328 Periodic  Interrupt setup.  Setup registers to generate
;       a periodic interrupt every 10ms. This takes 208 counts of the
;       timer running at the sysclock (33.33Mhz/2) divided by 16*50
;
;    /* BEGIN 68328 specific Code */
	MOVE.W	#0x32,0xFFFFF602       ; timer1_prescale = 50
	MOVE.W	#0xD0,0xFFFFF604      ; timer1_compare = 208d = 10 ms
	MOVE.W	#0x35,0xFFFFF600       ; timer1_control = restart mode,
                                       ; toggle output, sysclock/16,
                                       ; enable int and timer.
        MOVE.L  #0x003FFFFD,0xFFFFF304   ; Int Mask Reg Allow IRQ7,Timer1
;	MOVE.W    #0x2000,SR           ; Enable all interrupts
;    /* End of 68328 Periodic Timer Interrupt setup.  */
;
;    /* END 68328 specific Code */
      MOVE.L    D0,-(A7)

      JSR       _INC_Initialize
;}
;
;
;/*************************************************************************/
;/*                                                                       */
;/* FUNCTION                                                              */
;/*                                                                       */
;/*      INT_Vectors_Loaded                                               */
;/*                                                                       */
;/* DESCRIPTION                                                           */
;/*                                                                       */
;/*      This function returns the flag that indicates whether or not     */
;/*      all the default vectors have been loaded.  If it is false,       */
;/*      each LISR register also loads the ISR shell into the actual      */
;/*      vector table.                                                    */
;/*                                                                       */
;/* AUTHOR                                                                */
;/*                                                                       */
;/*                                                                       */
;/*      G. D. Fender, Accelerated Technology, Inc.                          */
;/*                                                                       */
;/* CALLED BY                                                             */
;/*                                                                       */
;/*      TCC_Register_LISR                   Register LISR for vector     */
;/*                                                                       */
;/* CALLS                                                                 */
;/*                                                                       */
;/*      None                                                             */
;/*                                                                       */
;/* INPUTS                                                                */
;/*                                                                       */
;/*      None                                                             */
;/*                                                                       */
;/* OUTPUTS                                                               */
;/*                                                                       */
;/*      None                                                             */
;/*                                                                       */
;/* HISTORY                                                               */
;/*                                                                       */
;/*         NAME            DATE                    REMARKS               */
;/*                                                                       */
;/*    G. Fender        09-21-1998      Created initial version 1.0       */
;/*                                       for 68328ADS port.              */
;/*************************************************************************/
        .xdef    _INT_Vectors_Loaded
_INT_Vectors_Loaded:
;INT    INT_Vectors_Loaded(void)
;{
;
;    /* Just return the loaded vectors flag.  */
;    return(INT_Loaded_Flag);
        MOVE.L  _INT_Loaded_Flag,D0         ; Put the flag into D0
        RTS                                 ; Return to caller
;}
;
;
;/*************************************************************************/
;/*                                                                       */
;/* FUNCTION                                                              */
;/*                                                                       */
;/*      INT_Setup_Vector                                                 */
;/*                                                                       */
;/* DESCRIPTION                                                           */
;/*                                                                       */
;/*      This function sets up the specified vector with the new vector   */
;/*      value.  The previous vector value is returned to the caller.     */
;/*                                                                       */
;/* AUTHOR                                                                */
;/*                                                                       */
;/*                                                                       */
;/*      G. D. Fender, Accelerated Technology, Inc.                          */
;/*                                                                       */
;/* CALLED BY                                                             */
;/*                                                                       */
;/*      Application                                                      */
;/*      TCC_Register_LISR                   Register LISR for vector     */
;/*                                                                       */
;/* CALLS                                                                 */
;/*                                                                       */
;/*      None                                                             */
;/*                                                                       */
;/* INPUTS                                                                */
;/*                                                                       */
;/*      vector                              Vector number to setup       */
;/*      new                                 Pointer to new assembly      */
;/*                                            language ISR               */
;/*                                                                       */
;/* OUTPUTS                                                               */
;/*                                                                       */
;/*      old vector contents                                              */
;/*                                                                       */
;/* HISTORY                                                               */
;/*                                                                       */
;/*         NAME            DATE                    REMARKS               */
;/*                                                                       */
;/*    G. Fender        09-21-1998      Created initial version 1.0       */
;/*                                       for 68328ADS port.              */
;/*                                                                       */
;/*************************************************************************/
        .xdef    _INT_Setup_Vector
_INT_Setup_Vector:
;VOID  *INT_Setup_Vector(INT vector, VOID *new)
;{
;
;VOID    *old_vector;                        /* Old interrupt vector      */
;VOID   **vector_table;                      /* Pointer to vector table   */
;
;    /* Calculate the starting address of the actual vector table.  */
;    vector_table =  (VOID **) 0x0;
;
        MOVEA.L #0,A0                      ; Pointer to the vector table
;
;    /* Pickup the old interrupt vector.  */
;    old_vector =  vector_table[vector];
;
        MOVE.L  4(A7),D0                    ; Pickup the vector number
        LSL.L   #2,D0                       ; Adjust for the vector size
        ADDA.L  D0,A0                       ; Add in offset to vector base
        MOVE.L  (A0),D0                     ; Pickup previous vector
;    
;    /* Setup the new interrupt vector.  */
;    vector_table[vector] =  new;
;
        MOVE.L  8(A7),D1                    ; Pickup the new vector pointer
        MOVE.L  D1,(A0)                     ; Store it in the vector table
;    
;    /* Return the old interrupt vector.  */
;    return(old_vector);
;
        RTS                                 ; Return to caller
;}
;
;
;/*************************************************************************/
;/*                                                                       */
;/* FUNCTION                                                              */
;/*                                                                       */
;/*      INT_Retrieve_Shell                                               */
;/*                                                                       */
;/* DESCRIPTION                                                           */
;/*                                                                       */
;/*      This function retrieves the pointer to the shell interrupt       */
;/*      service routine.  The shell interrupt service routine calls      */
;/*      the LISR dispatch routine.                                       */
;/*                                                                       */
;/* AUTHOR                                                                */
;/*                                                                       */
;/*                                                                       */
;/*      G. D. Fender, Accelerated Technology, Inc.                          */
;/*                                                                       */
;/* CALLED BY                                                             */
;/*                                                                       */
;/*      TCC_Register_LISR                   Register LISR for vector     */
;/*                                                                       */
;/* CALLS                                                                 */
;/*                                                                       */
;/*      None                                                             */
;/*                                                                       */
;/* INPUTS                                                                */
;/*                                                                       */
;/*      vector                              Vector number to setup       */
;/*                                                                       */
;/* OUTPUTS                                                               */
;/*                                                                       */
;/*      shell pointer                                                    */
;/*                                                                       */
;/* HISTORY                                                               */
;/*                                                                       */
;/*         NAME            DATE                    REMARKS               */
;/*                                                                       */
;/*    G. Fender        09-21-1998      Created initial version 1.0       */
;/*                                       for 68328ADS port.              */
;/*                                                                       */
;/*************************************************************************/
        .xdef    _INT_Retrieve_Shell
_INT_Retrieve_Shell:
;VOID  *INT_Retrieve_Shell(INT vector)
;{
;
;    /* Return the LISR Shell interrupt routine.  */
;    return(INT_Vectors[vector]);
;
        LEA     _INT_Vectors,A0             ; Build address to default ISRs
        MOVE.L  4(A7),D1                    ; Pickup the vector number
        ASL.L   #2,D1                       ; Multiply by 4
        MOVE.L  (A0,D1.W),D0                ; Pickup the default ISR
        RTS                                 ; Return to caller
;}
;
;
;/* The following area contains the default interrupt vector processing for
;   each interrupt.  Note:  Nothing else can be placed between the 
;   INT_Start_ISRs label and the INT_End_ISRs label.  */
;
        .xdef    _INT_Start_ISRs
_INT_Start_ISRs:
;
;
_INT_Bus_Error:                             ; Bus Error                 2
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Address_Error:                         ; Address Error             3
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Illegal_Instruction:                   ; Illegal Instruction       4
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Zero_Divide:                           ; Divide by zero            5
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Chk_Chk2:                              ; CHK and CHK2 instructions 6
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_TrapV:                                 ; TRAPV instruction         7
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Privilege:                             ; Privilege violation       8
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trace:                                 ; Trace                     9
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Emul_A:                                ; Emulate A                 10
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Emul_B:                                ; Emulate B                 11
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Hdw_Breakpoint:                        ; Hardware breakpoint       12
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Coprocessor_Violate:                   ; Coprocessor violation     13
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Format_Error:                          ; Format error              14
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Uninitialized_Int:                     ; Uninitialized interrupt   15
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_0:                            ; Reserved 0                16
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_1:                            ; Reserved 1                17
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_2:                            ; Reserved 2                18
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_3:                            ; Reserved 3                19
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_4:                            ; Reserved 4                20
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_5:                            ; Reserved 5                21
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_6:                            ; Reserved 6                22
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_7:                            ; Reserved 7                23
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Spurious:                              ; Spurious interrupt        24
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Level_1_Auto:                          ; Level 1 Autovector        25
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Level_2_Auto:                          ; Level 2 Autovector        26
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Level_3_Auto:                          ; Level 3 Autovector        27
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Level_4_Auto:                          ; Level 4 Autovector        28
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Level_5_Auto:                          ; Level 5 Autovector        29
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Level_6_Auto:                          ; Level 6 Autovector        30
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Level_7_Auto:                          ; Level 7 Autovector        31
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_0:                                ; Trap 0                    32
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_1:                                ; Trap 1                    33
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_2:                                ; Trap 2                    34
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_3:                                ; Trap 3                    35
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_4:                                ; Trap 4                    36
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_5:                                ; Trap 5                    37
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_6:                                ; Trap 6                    38
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_7:                                ; Trap 7                    39
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_8:                                ; Trap 8                    40
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_9:                                ; Trap 9                    41
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_10:                               ; Trap 10                   42
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_11:                               ; Trap 11                   43
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_12:                               ; Trap 12                   44
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_13:                               ; Trap 13                   45
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_14:                               ; Trap 14                   46
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Trap_15:                               ; Trap 15                   47
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_0:                        ; Coprocessor Reserved 0    48
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_1:                        ; Coprocessor Reserved 1    49
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_2:                        ; Coprocessor Reserved 2    50
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_3:                        ; Coprocessor Reserved 3    51
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_4:                        ; Coprocessor Reserved 4    52
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_5:                        ; Coprocessor Reserved 5    53
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_6:                        ; Coprocessor Reserved 6    54
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_7:                        ; Coprocessor Reserved 7    55
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_8:                        ; Coprocessor Reserved 8    56
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_9:                        ; Coprocessor Reserved 9    57
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Cop_Reserved_10:                       ; Coprocessor Reserved 10   58
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_8:                            ; Reserved 8                59
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_9:                            ; Reserved 9                60
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_10:                           ; Reserved 10               61
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_11:                           ; Reserved 11:              62
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_Reserved_12:                           ; Reserved 12               63
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_0:                                ; User Interrupt 0          64
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_1:                                ; User Interrupt 1          65
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_2:                                ; User Interrupt 2          66
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_3:                                ; User Interrupt 3          67
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_4:                                ; User Interrupt 4          68
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_5:                                ; User Interrupt 5          69
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_6:                                ; User Interrupt 6          70
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_7:                                ; User Interrupt 7          71
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_8:                                ; User Interrupt 8          72
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_9:                                ; User Interrupt 9          73
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_10:                               ; User Interrupt 10         74
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_11:                               ; User Interrupt 11         75
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_12:                               ; User Interrupt 12         76
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_13:                               ; User Interrupt 13         77
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_14:                               ; User Interrupt 14         78
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_15:                               ; User Interrupt 15         79
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_16:                               ; User Interrupt 16         80
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_17:                               ; User Interrupt 17         81
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_18:                               ; User Interrupt 18         82
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_19:                               ; User Interrupt 19         83
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_20:                               ; User Interrupt 20         84
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_21:                               ; User Interrupt 21         85
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_22:                               ; User Interrupt 22         86
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_23:                               ; User Interrupt 23         87
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_24:                               ; User Interrupt 24         88
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_25:                               ; User Interrupt 25         89
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_26:                               ; User Interrupt 26         90
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_27:                               ; User Interrupt 27         91
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_28:                               ; User Interrupt 28         92
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_29:                               ; User Interrupt 29         93
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_30:                               ; User Interrupt 30         94
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_31:                               ; User Interrupt 31         95
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_32:                               ; User Interrupt 32         96
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_33:                               ; User Interrupt 33         97
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_34:                               ; User Interrupt 34         98
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_35:                               ; User Interrupt 35         99
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_36:                               ; User Interrupt 36         100
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_37:                               ; User Interrupt 37         101
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_38:                               ; User Interrupt 38         102
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_39:                               ; User Interrupt 39         103
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_40:                               ; User Interrupt 40         104
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_41:                               ; User Interrupt 41         105
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_42:                               ; User Interrupt 42         106
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_43:                               ; User Interrupt 43         107
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_44:                               ; User Interrupt 44         108
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_45:                               ; User Interrupt 45         109
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_46:                               ; User Interrupt 46         110
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_47:                               ; User Interrupt 47         111
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_48:                               ; User Interrupt 48         112
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_49:                               ; User Interrupt 49         113
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_50:                               ; User Interrupt 50         114
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_51:                               ; User Interrupt 51         115
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_52:                               ; User Interrupt 52         116
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_53:                               ; User Interrupt 53         117
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_54:                               ; User Interrupt 54         118
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_55:                               ; User Interrupt 55         119
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_56:                               ; User Interrupt 56         120
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_57:                               ; User Interrupt 57         121
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_58:                               ; User Interrupt 58         122
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_59:                               ; User Interrupt 59         123
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_60:                               ; User Interrupt 60         124
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_61:                               ; User Interrupt 61         125
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_62:                               ; User Interrupt 62         126
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_63:                               ; User Interrupt 63         127
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_64:                               ; User Interrupt 64         128
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_65:                               ; User Interrupt 65         129
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_66:                               ; User Interrupt 66         130
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_67:                               ; User Interrupt 67         131
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_68:                               ; User Interrupt 68         132
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_69:                               ; User Interrupt 69         133
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_70:                               ; User Interrupt 70         134
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_71:                               ; User Interrupt 71         135
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_72:                               ; User Interrupt 72         136
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_73:                               ; User Interrupt 73         137
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_74:                               ; User Interrupt 74         138
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_75:                               ; User Interrupt 75         139
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_76:                               ; User Interrupt 76         140
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_77:                               ; User Interrupt 77         141
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_78:                               ; User Interrupt 78         142
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_79:                               ; User Interrupt 79         143
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_80:                               ; User Interrupt 80         144
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_81:                               ; User Interrupt 81         145
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_82:                               ; User Interrupt 82         146
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_83:                               ; User Interrupt 83         147
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_84:                               ; User Interrupt 84         148
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_85:                               ; User Interrupt 85         149
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_86:                               ; User Interrupt 86         150
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_87:                               ; User Interrupt 87         151
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_88:                               ; User Interrupt 88         152
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_89:                               ; User Interrupt 89         153
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_90:                               ; User Interrupt 90         154
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_91:                               ; User Interrupt 91         155
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_92:                               ; User Interrupt 92         156
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_93:                               ; User Interrupt 93         157
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_94:                               ; User Interrupt 94         158
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_95:                               ; User Interrupt 95         159
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_96:                               ; User Interrupt 96         160
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_97:                               ; User Interrupt 97         161
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_98:                               ; User Interrupt 98         162
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_99:                               ; User Interrupt 99         163
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_100:                              ; User Interrupt 100        164
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_101                               ; User Interrupt 101        165
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_102                               ; User Interrupt 102        166
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_103:                              ; User Interrupt 103        167
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_104:                              ; User Interrupt 104        168
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_105:                              ; User Interrupt 105        169
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_106:                              ; User Interrupt 106        170
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_107:                              ; User Interrupt 107        171
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_108:                              ; User Interrupt 108        172
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_109:                              ; User Interrupt 109        173
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_110:                              ; User Interrupt 110        174
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_111:                              ; User Interrupt 111        175
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_112:                              ; User Interrupt 112        176
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_113:                              ; User Interrupt 113        177
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_114:                              ; User Interrupt 114        178
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_115:                              ; User Interrupt 115        179
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_116:                              ; User Interrupt 116        180
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_117:                              ; User Interrupt 117        181
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_118:                              ; User Interrupt 118        182
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_119:                              ; User Interrupt 119        183
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_120:                              ; User Interrupt 120        184
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_121:                              ; User Interrupt 121        185
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_122:                              ; User Interrupt 122        186
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_123:                              ; User Interrupt 123        187
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_124:                              ; User Interrupt 124        188
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_125:                              ; User Interrupt 125        189
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_126:                              ; User Interrupt 126        190
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_127:                              ; User Interrupt 127        191
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_128:                              ; User Interrupt 128        192
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_129:                              ; User Interrupt 129        193
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_130:                              ; User Interrupt 130        194
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_131:                              ; User Interrupt 131        195
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_132:                              ; User Interrupt 132        196
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_133:                              ; User Interrupt 133        197
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_134:                              ; User Interrupt 134        198
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_135:                              ; User Interrupt 135        199
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_136:                              ; User Interrupt 136        200
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_137:                              ; User Interrupt 137        201
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_138:                              ; User Interrupt 138        202
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_139:                              ; User Interrupt 139        203
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_140:                              ; User Interrupt 140        204
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_141:                              ; User Interrupt 141        205
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_142:                              ; User Interrupt 142        206
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_143:                              ; User Interrupt 143        207
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_144:                              ; User Interrupt 144        208
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_145:                              ; User Interrupt 145        209
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_146:                              ; User Interrupt 146        210
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_147:                              ; User Interrupt 147        211
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_148:                              ; User Interrupt 148        212
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_149:                              ; User Interrupt 149        213
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_150:                              ; User Interrupt 150        214
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_151:                              ; User Interrupt 151        215
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_152:                              ; User Interrupt 152        216
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_153:                              ; User Interrupt 153        217
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_154:                              ; User Interrupt 154        218
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_155:                              ; User Interrupt 155        219
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_156:                              ; User Interrupt 156        220
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_157:                              ; User Interrupt 157        221
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_158:                              ; User Interrupt 158        222
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_159:                              ; User Interrupt 159        223
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_160:                              ; User Interrupt 160        224
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_161:                              ; User Interrupt 161        225
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_162:                              ; User Interrupt 162        226
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_163:                              ; User Interrupt 163        227
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_164:                              ; User Interrupt 164        228
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_165:                              ; User Interrupt 165        229
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_166:                              ; User Interrupt 166        230
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_167:                              ; User Interrupt 167        231
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_168:                              ; User Interrupt 168        232
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_169:                              ; User Interrupt 169        233
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_170:                              ; User Interrupt 170        234
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_171:                              ; User Interrupt 171        235
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_172:                              ; User Interrupt 172        236
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_173:                              ; User Interrupt 173        237
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_174:                              ; User Interrupt 174        238
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_175:                              ; User Interrupt 175        239
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_176:                              ; User Interrupt 176        240
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_177:                              ; User Interrupt 177        241
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_178:                              ; User Interrupt 178        242
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_179:                              ; User Interrupt 179        243
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_180:                              ; User Interrupt 180        244
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_181:                              ; User Interrupt 181        245
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_182:                              ; User Interrupt 182        246
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_183:                              ; User Interrupt 183        247
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_184:                              ; User Interrupt 184        248
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_185:                              ; User Interrupt 185        249
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_186:                              ; User Interrupt 186        250
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_187:                              ; User Interrupt 187        251
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_188:                              ; User Interrupt 188        252
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_189:                              ; User Interrupt 189        253
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_190:                              ; User Interrupt 190        254
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
_INT_User_191:                              ; User Interrupt 191        255
        BSR.W   _INT_Interrupt_Shell        ; Branch to interrupt shell
;
;
        .xdef    _INT_End_ISRs
_INT_End_ISRs:
;
;
;/* Define the standard interrupt processing shell.  */
        .xdef    _INT_Interrupt_Shell
_INT_Interrupt_Shell:
        MOVEM.L D0-D7/A0-A6,-(A7)           ; Save all registers
        MOVE.L  60(A7),D7                   ; Pickup next address after ISR
        SUB.L   #_INT_Start_ISRs,D7         ; Calculate relative offset from
                                            ;   the start of the ISRs
        ASR.W   #2,D7                       ; Divide by 4 to get vector number
                                            ;  minus 1
        ADDQ.L  #1,D7                       ; Vector number is now in D7
        JSR     _TCT_Interrupt_Context_Save ; Call context save function with
                                            ;   D7 containing the vector number
        MOVE.L  D7,-(A7)                    ; Put vector number on the stack
        JSR     _TCC_Dispatch_LISR          ; Call the LISR dispatch function
        ADDQ.L  #4,A7                       ; Adjust stack

        JMP     _TCT_Interrupt_Context_Restore ; Restore context
;
;/* The following is a default timer interrupt ISR.  This is used instead of
;   going directly to the TMT_Timer_Interrupt function because there is often
;   some special timer reset logic that is necessary for different timer
;   chips.  Hence, the target dependencies are isolated to this file.  */
        .xdef    _INT_Timer_Interrupt
_INT_Timer_Interrupt:
;
;/* Reset logic may be placed here.  Be sure to save and restore any registers
;   used before transferring control to the actual timer interrupt routine. */
;
;   interrupt handling routine
; Reset the interrupt by reading and writing to the status reg
;
;    /* Save minimal context on the stack.  */
      MOVE.L      A0,-(A7)           ; Save A0 on the stack
	MOVE.L	#0xFFFFF60A,A0       ; read timer 1 status address to A0 
	MOVE.W	(A0),A0            ; to clear timer1_status first read it
	MOVE.L	#0xFFFFF60A,A0       ; read timer 1 status address to A0 
	CLR.W	      (A0)               ; then clear it
      MOVE.L	#0xFFFFF608,A0       ; timer 1 current value addr to A0
      MOVE.W      (A0),A0            ; for testing
      MOVE.L      (A7)+,A0           ; Recover A0
      JMP     _TMT_Timer_Interrupt   ; Transfer control to actual timer

______________________________________________________________
;    /* Save minimal context on the stack.  */
;      MOVE.L      A0,-(A7)                ; Save A0 on the stack
;	MOVE.W	0xFFFFF60A,A0             ; read timer 1 status 
;	MOVE.W	#0x0,0xFFFFF60A           ; clear timer1_status
;      MOVE.L      (A7)+,A0                ; Recover A0
;      BRA     _TMT_Timer_Interrupt        ; Transfer control to actual timer
;/* Resolve call to _exit in DIABDATA's crt0.s file */
	.xdef    _exit
_exit:
	BRA     _exit

