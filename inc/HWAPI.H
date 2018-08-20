/* --------------------------------------------------------
  Copyright (c) 2006 �����Ƽ��ɷ����޹�˾
  
  File��       HWAPI.h

  Author���ؽ��� jhqin@hanwang.com.cn

  Date��2006-8-9

  �ؼ��֣�
		�������� GBK���� LittleEndian
------------------------------------------------------------ */

#ifndef __HWAPI_H
#define __HWAPI_H

//����������¶���İ汾����������Ӧ��
#define HANWANG_GBKCODE			//����GBK����
//#define HANWANG_BIGENDIAN		//����BigEndian
//#define HANWANG_DICINTEGRATION	//�ֵ�����뼯��
/* ------------------ʶ��Χ����---------------------- */
#define ALC_CHINESE_COMMON		0x00000001  //���ú���
#define ALC_CHINESE_RARE		0x00000002  //�γ��ú���	
#define ALC_CHINESE_VARIANT		0x00000004  //д������
#define ALC_CHINESE_RADICAL		0x00000008	//����	
#define ALC_NUMERIC				0x00000010  //����	
#define ALC_UCALPHA				0x00000020  //��д��ĸ	
#define ALC_LCALPHA				0x00000040  //Сд��ĸ	
#define	ALC_PUNCTUATION			0x00000080  //���	
#define	ALC_SYMBOLS				0x00000100  //����
#define	ALC_GESTURE				0x00000200  //����
#define ALC_CURSIVE				0x00000400	//����
#define ALC_VALID				0x000007FF	//ʶ���ַ�����Ч���

/* -----------------����Զ�ת��------------------------ */
#define OUT_LOWERCASE			0x20000000	//�Զ�����д��ĸת��Сд��ĸ
#define OUT_UPPERCASE			0x40000000	//�Զ���Сд��ĸת�ɴ�д��ĸ
#define OUT_SBCCASE				0x80000000	//���ȫ���ַ�
#define OUT_ASSISTANT			(OUT_LOWERCASE | OUT_UPPERCASE | OUT_SBCCASE)

/* -----------------�������----------------------------- */
#define CHARSET_CNDEFAULT		(ALC_CHINESE_COMMON | ALC_CHINESE_RARE)//ȱʡ����ʶ��
#define CHARSET_SYMPUNC			(ALC_PUNCTUATION | ALC_SYMBOLS)//���ű��
#define CHARSET_ENGLISH			(ALC_UCALPHA | ALC_LCALPHA)//Ӣ��ʶ��

/* ----------------�������Ʊ���ֵ-------------------------- */
#define CODE_BACKSPACE			0x0008	//��ɾ
#define CODE_TAB				0x0009	//Tab
#define CODE_RETURN				0x000D	//�س�
#define CODE_SPACE				0x0020	//�ո�

/* ---------------��������---------------------------- */
#define MAXCANDNUM				10			// �����������ѡ�ָ���
#define HWRERAMSIZE				(14*1024)	// ������Ҫ����ʱ����ռ��С

/* ------------�ʼ��ṹ����------------------- */
#define STROKEENDMARK			(-1)		// ����ʻ��������ֵ

typedef struct tagTHWPoint
{
	short x; // �ʼ� X ����
	short y; // �ʼ� Y ����
} THWPoint;

/* ------------ʶ�����Զ���------------------ */
typedef struct tagTHWAttribute
{
	int				iCandidateNum;	// �û���Ҫ�ĺ�ѡ�ָ���
	unsigned long	dwRange;		// ʶ��Χ����		
	unsigned char*	pRam;			// ����ռ䣬��СΪ HWRERAMSIZE��Ҫ����ʼ��ַ4�ֽڶ���
	unsigned char*	pRom;			// ʶ���ֵ���ʼ��ַ��Ҫ����ʼ��ַ4�ֽڶ���
} THWAttribute;

#ifdef __cplusplus
extern "C" {
#endif

/*
���ܣ��ʼ�ʶ��
����˵����
	pTrace���ʼ����ݣ����ʽΪ(x1,y1),(x2,y2),...,(STROKEENDMARK,0),...,
	        (STROKEENDMARK,0),(STROKEENDMARK,STROKEENDMARK)��
			����(STROKEENDMARK,0)Ϊ�ʻ�������־��
			(STROKEENDMARK,STROKEENDMARK)Ϊ�ֽ�����־��
	pAttr��ʶ�����ԡ�
	pResult��ʶ�������������СΪ 2*MAXCANDNUM��	
����ֵ��
	 -1:��������
	= 0:��ʶ
	> 0:��ѡ�ָ���
*/
int HWRecognize( const THWPoint* pTrace, const THWAttribute* pAttr, unsigned short* pResult );

#ifdef __cplusplus
}
#endif

#endif