///////////////////////////////////////////////
//  字符串资源头文件，利用 string.exe 生成   //
//     COPYRIGHT Hanwang Tech Corp.(2001)    //
///////////////////////////////////////////////

#ifndef        _G301_STRING_RESOURCE_H
#define        _G301_STRING_RESOURCE_H

#define        LAN_CHINESE           0
#define        LAN_ENGLISH           1

extern         char**                g_pLanguage;

// language: 0 = 中文, 1 = 英文
extern         void                  SetLanguage(char language);
extern         unsigned short        GetLanguage();

#define        LOADSTRING(ID)        ((char*)(*(g_pLanguage + ID)))

#define	                            STR_IDLENGTH	0
#define	                              STR_IDAREA	1
#define	                            STR_IDWEIGHT	2
#define	                               STR_IDCAP	3
#define	                              STR_IDTEMP	4
#define	                             STR_IDPOWER	5
#define	                            STR_IDENERGY	6
#define	                             STR_IDPRESS	7
#define	                             STR_IDMETER	8
#define	                              STR_IDFEET	9
#define	                            STR_IDSHICHI	10
#define	                                STR_IDDM	11
#define	                                STR_IDCM	12
#define	                                STR_IDMM	13
#define	                              STR_IDYARD	14
#define	                              STR_IDINCH	15
#define	                                STR_IDSM	16
#define	                                STR_IDKM	17
#define	                              STR_IDMILE	18
#define	                            STR_IDSQRTMT	19
#define	                             STR_IDSQCHI	20
#define	                              STR_IDSQKM	21
#define	                                STR_IDLG	22
#define	                               STR_IDARE	23
#define	                              STR_IDACRE	24
#define	                                STR_IDMU	25
#define	                            STR_IDSQYARD	26
#define	                           STR_IDHEKTARE	27
#define	                            STR_IDSQMILE	28
#define	                                STR_IDKG	29
#define	                               STR_IDJIN	30
#define	                             STR_IDLIANG	31
#define	                             STR_IDPOUND	32
#define	                              STR_IDGRAM	33
#define	                             STR_IDCARAT	34
#define	                             STR_IDOUNCE	35
#define	                               STR_IDTON	36
#define	                             STR_IDLITRE	37
#define	                            STR_IDGALAME	38
#define	                            STR_IDGALENG	39
#define	                            STR_IDCUBCUN	40
#define	                          STR_IDCAPOUNCE	41
#define	                              STR_IDPINT	42
#define	                             STR_IDQUART	43
#define	                            STR_IDBUSHEL	44
#define	                           STR_IDCUBYARD	45
#define	                              STR_IDCUBM	46
#define	                            STR_IDSHESHI	47
#define	                            STR_IDKAISHI	48
#define	                            STR_IDHUASHI	49
#define	                                 STR_IDW	50
#define	                             STR_IDHPAME	51
#define	                             STR_IDHPENG	52
#define	                             STR_IDQIANK	53
#define	                              STR_IDJIAO	54
#define	                               STR_IDKWH	55
#define	                                STR_IDMH	56
#define	                                 STR_IDP	57
#define	                               STR_IDSGP	58
#define	                               STR_CLEAR	59
#define	                                STR_CONV	60

#endif
