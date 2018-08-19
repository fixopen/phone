/*************************************************************************/
/*                                                                       */
/*                       ��Ȩ����(c) 2001�����Ƽ�                        */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* �ļ���                                               �汾             */
/*                                                                       */
/*      datadrv.cpp                                     PRELIMINARY      */
/*                                                      VERSION 1.0      */
/* ����                                                 2001��8��24��    */
/*                                                                       */
/*      �������������ļ�												 */
/*                                                                       */
/* ����                                                                  */
/*                                                                       */
/*      ����ΰ                                                           */
/*                                                                       */
/* ���ݽṹ                                                              */
/*                                                                       */
/*      HISTORY                 ͨ����¼                                 */
/*      MEMO                    ����                                     */
/*      CARD                    ��Ƭ                                     */
/*      CARDCASE                ��Ƭ��                                   */
/*                                                                       */
/* ����                                                                  */
/*                                                                       */
/*      ¼�������ӿں�����11����                                         */
/*      ��ʷ��¼�����ӿں�����7����                                      */
/*      ���������ӿں�����6����                                          */
/*      ��Ƭ�����ӿں�����8����                                          */
/*      ��Ƭ�������ӿں�����7����                                        */
/*                                                                       */
/* ����                                                                  */
/*                                                                       */
/*                                                                       */
/* ��ʷ��¼                                                              */
/*                                                                       */
/*  byw, 2001/9/24                                                       */
/*      �޸ģ�SavePicture() bug��                                        */
/*                                                                       */
/*  byw, 2001/9/29                                                       */
/*      �޸ģ�DeleteRing()��                                             */
/*                                                                       */
/*  wf, 2001/10/23                                                       */
/*      �޸ģ�FindCardNumber                                             */
/*                                                                       */
/*  byw, 2001/10/23                                                      */
/*      �޸ģ�SaveFlashSector() �� LoadFlashSector() ��Bug.              */
/*                                                                       */
/*  wf, 2001/10/25                                                       */
/*      �޸ģ�ͨ����¼��HistoryCompare                                   */
/*                                                                       */
/*	ygr,2001/11/5						                                 */
/*		�޸ģ�IllegalType()��Bug                                         */
/*            OpenWaveForWrite,OpenWaveForRead                          */
/*                                                                       */
/*  ygr 2001/11/16                                                       */
/*       ���:  DropWaveTail()����                                       */
/*                                                                       */
/*  ygr 2001/11/21                                                       */
/*       �޸�:  OpenWaveForWrite,OpenWaveForRead��CloseWave              */
/*              ����ʺ�������                                           */
/*                                                                       */
/*  ygr 2001/11/28                                                       */
/*       ���:  DeleteWave ��  IllegalWave����				             */
/*                                                                       */
/*  ygr 2001/12/6                                                        */
/*       ���:  OpenWaveForWrite����				                     */
/*                                                                       */
/*  byw 2001/12/23                                                       */
/*       �޸�:  FindCardString() ����				                     */
/*************************************************************************/


#include "datadrv.h"
#include "flash.h"
#include "stdlib.h" 
#include "phoneapp.h"
#include "syssetup.h"
#include "edit.h"
/*************************************************************************/

/* [ Flash �ӿ� ] */

/* ��һ�� Sector �� Flash װ�뻺�� */
PRIVATE void LoadFlashSector(void *addr)
{
	//memcpy((void*)FLASH_BUF_ADDR, (void*)((long)addr & 0xFFFFF000), FLASH_SECTOR_SIZE);
	register int i;
	register UINT8 *psrc = (UINT8 *) ((long)addr & 0xFFFFF000);
	register UINT8 *pdest = (UINT8 *)FLASH_BUF_ADDR;

	for(i = 0; i < FLASH_SECTOR_SIZE ;i++, psrc++, pdest++)
		*pdest = *psrc;
	FlashEraseSector((UINT16*)addr);

}

/* ��һ�� Sector �� ���� д�� Flash */
PRIVATE void SaveFlashSector(void *addr)
{
	FlashWrite((UINT16*)((long)addr & 0xFFFFF000), (UINT16*)FLASH_BUF_ADDR, FLASH_SECTOR_SIZE / 2);
}

/*************************************************************************/
/*��λ�ϵ����*/
#define TEST_CHAR ("E201 E_PHONE DEVLEPOR: ����ΰ�������������������ᣬ���ɣ������У�����٣��ʻԣ�֣����������������Ӣ�������ڣ�����������췢�ȡ�copyright 1.0, 2002,2")
bool g_bFirstPowerOn=true;
extern SYSTEM_OPTION g_SystemOption;
bool FirstBootTest()
{
	if(strcmp((char *)FIRST_POWERON_BUFF,TEST_CHAR))
	{
		strcpy((char *)FIRST_POWERON_BUFF,TEST_CHAR);
		g_bFirstPowerOn=true;
		g_SystemOption.nResetTimes = 0;
	}		
	else
	{
		g_bFirstPowerOn=false;
		g_SystemOption.nResetTimes++;
	}
	::SaveSystemOption(&g_SystemOption);
	return g_bFirstPowerOn;
}

/*************************************************************************/

/* [ ¼���������� ] */

/* ȫ�ֱ��� */
WAVEHEAD	*pWaveHead, *(*pWaveTail);		// ����ָ��
WAVESTATE	WaveState;					// ¼��״̬
WAVEHEAD	*pReadWave;					// ��ǰָ��
long		WaveCount;					// ���ݼ�����
UINT8		*pAccess;					// ����ָ��
extern     SYSTEM_OPTION g_SystemOption;
/* �ж�¼������Ϊ�Ƿ� */
PRIVATE bool IllegalWave(long id)
{
	// ����
	if(id < WAVE_STORE_ADDR ) return true;
	WAVEHEAD * h = pWaveHead,* pd=(WAVEHEAD *)id;
	if(id==GREETINGWAVE_RAMADD||id==GREETINGWAVE_ROMADD)
		return false;
	while( ( h < (WAVEHEAD *) (WAVE_STORE_ADDR + WAVE_STORE_LEN) )
		 && (h < *pWaveTail) )
	{
		if(pd==h)
			return false;
		else
			h = (WAVEHEAD*)((long)h + sizeof(WAVEHEAD) + h->Length);
	}
	return true;
}

/* �ж�¼�����ͺϷ� */
PRIVATE bool IllegalType(int type)
{
	type &= WAVE_TYPE;
	if ((type == WAVE_MIC) || (type == WAVE_AUTO) || (type == WAVE_LINE) || (type == WAVE_ALL))
		return false;
	else	
		return true;
}

//////////////////////////////////////////////////
//
//  ����ʽ��¼����¼
//
//	������	id		¼����¼��ʶ
//
//  ���أ�	true	�򿪳ɹ�
//			false	��ʧ��
//
//////////////////////////////////////////////////
bool OpenWaveForRead(long id)
{
	WAVEHEAD	*pw = (WAVEHEAD*)id;
	if (WaveState != STATE_CLOSED)
		return false;
	if (IllegalWave(id))
		return false;
	WaveCount = pw->Length;
	pAccess = (UINT8*)((long)pw + sizeof(WAVEHEAD));
	pReadWave = pw;
	WaveState = STATE_OPENREAD;
	if (pReadWave->Flag & WAVE_NEW)
		pReadWave->Flag &= ~WAVE_NEW;
	return true;
}

//////////////////////////////////////////////////
//
//  д��ʽ��¼����¼
//
//	������	type	¼�����
//
//  ���أ�	true	�򿪳ɹ�
//			false	��ʧ��
//
//////////////////////////////////////////////////
extern   g_pPhoneWnd ;
int g_iType=0xff;
bool OpenWaveForWrite(int type)
{
	DateTime	time;
	WAVEHEAD	* pWaveGreeting=(WAVEHEAD *)GREETINGWAVE_RAMADD;
	if (WaveState != STATE_CLOSED)
		return false;
	GetSysTime(&time);
	if(type==WAVE_GREETING) 
	{
		if((long)(*pWaveTail)>=(long)(GREETINGWAVE_RAMADD) ) return false;
		g_iType=type; 
		pWaveGreeting->Flag=type; 
		pAccess=(UINT8*)(GREETINGWAVE_RAMADD + sizeof(WAVEHEAD));
		strcpy(pWaveGreeting->Name,"�ʺ���");
		WaveCount = 0;
		WaveState = STATE_OPENWRITE;
		return true;
	}
	if (IllegalType(type))
		return false;
	g_iType=type;
	pAccess = (UINT8*)((long)(*pWaveTail) + sizeof(WAVEHEAD));
	if ((long)pAccess > WAVE_STORE_ADDR + WAVE_STORE_LEN - sizeof(long))
		return false;
	memset((void*)(*pWaveTail), 0, sizeof(WAVEHEAD));
	(*pWaveTail)->Flag = WAVE_NEW | WAVE_CODENONE | type;
	extern BOOL GetPhoneStatus();
	if(GetPhoneStatus())	 						//��ȡ�绰��Ϣ
	{
		if(type==WAVE_AUTO)
		{
			extern short g_wLastHistroyID;
			HISTORY *pCur = GetHistoryPointer(g_wLastHistroyID);
			if(pCur && g_wLastHistroyID>=0)
			{
				char * sname=pCur->Name;
				if (strcmp(pCur->AreaCode, 
				          g_SystemOption.LocalAreaCode) != 0)
					strcpy((*pWaveTail)->Number,pCur->AreaCode);
				int len=strlen((*pWaveTail)->Number);
				strncpy((*pWaveTail)->Name,pCur->Name,WAVE_NAME_LEN);
				strncat((*pWaveTail)->Number,pCur->Number,WAVE_NUMBER_LEN-len);
			}
			if((*pWaveTail)->Name[0]==NULL) 
				strcpy((*pWaveTail)->Name,"����");
		}
		else
			strcpy((*pWaveTail)->Name,"�绰¼��");	//1-8 ygr
	}
	else
		strcpy((*pWaveTail)->Name, "����¼��");
	(*pWaveTail)->Time = time;
	WaveCount = 0;
	WaveState = STATE_OPENWRITE;
	return true;
}

//////////////////////////////////////////////////
//
//  ��ȡ¼�����ݳ���
//
//	������	id		¼��ID
//
//  ���أ�	���ݳ���
//
//////////////////////////////////////////////////
long GetWaveLen(long id)
{
	WAVEHEAD	*pw = (WAVEHEAD*)id;
	return pw->Length;
}

//////////////////////////////////////////////////
//
//  ��ȡ¼��������
//
//	������	��
//
//  ���أ�	¼��������
//
//////////////////////////////////////////////////
int GetWaveTotal(void)
{
	WAVEHEAD	*h = pWaveHead, *t = (*pWaveTail);
	int			total = 0;

	for (;(h != t) && (total < WAVE_MAX_COUNT); total ++)
		h = (WAVEHEAD*)((long)h + sizeof(WAVEHEAD) + h->Length);
	return total;
}
//////////////////////////////////////////////////
//
//  д���ݣ�¼�ƣ�
//
//	������	val		¼����������
//
//  ���أ�	true	д�ɹ�
//			false	дʧ�ܣ��ռ�����
//
//////////////////////////////////////////////////
bool WriteWave(UINT8 val)
{
	if ((WaveState == STATE_OPENWRITE) && ((long)pAccess < WAVE_STORE_ADDR + WAVE_STORE_LEN)){
		*pAccess = val;	//byw
		pAccess ++;
		return true;
		}
	else{
		CloseWave(); 
		return false;
	}
}

//////////////////////////////////////////////////
//
//	�����ݣ����ţ�
//
//	������	��
//
//  ���أ�	¼����������
//
//////////////////////////////////////////////////
UINT8 ReadWave(void)
{
	if ((WaveState == STATE_OPENREAD) && (WaveCount != 0)){
		WaveCount --;
		return *(pAccess++);
	}
	else{
		CloseWave();
		return WAVE_MUTE;
	}
}

//////////////////////////////////////////////////
//
//  �ر�¼��
//
//	������	��
//
//  ���أ�	�ر�֮ǰ��¼��״̬��ȡֵ
//				STATE_CLOSED		�ر�
//				STATE_OPENREAD		��
//				STATE_OPENWRITE		д
//
//////////////////////////////////////////////////
WAVESTATE CloseWave(void)
{
	WAVESTATE	old = WaveState;
	WAVEHEAD	*next;
	
	WaveState = STATE_CLOSED; 
	if (old == STATE_OPENWRITE){
		while ((long)pAccess % 4 != 0)
			pAccess ++;
		if(g_iType==WAVE_GREETING)
		{
			WAVEHEAD	* pWaveGreeting=(WAVEHEAD *)GREETINGWAVE_RAMADD;
			pWaveGreeting->Length= (long)pAccess - (long)pWaveGreeting - sizeof(WAVEHEAD); 
		}
		else
		{
			(*pWaveTail)->Length = (long)pAccess - (long)(*pWaveTail) - sizeof(WAVEHEAD);
			(*pWaveTail) = (WAVEHEAD*)((long)(*pWaveTail) + sizeof(WAVEHEAD) + (*pWaveTail)->Length);
			(*pWaveTail) ->Length =0;
		}
	}
	pAccess = (UINT8*)((long)(*pWaveTail) + sizeof(WAVEHEAD));
	return old;
}

////////////////////////////////////////////////////
//
//  ȥ����β���Ĳ���¼��
//
//	������	��ȥ�ֽ���
//
//  ���أ�	�ɹ�1 ��ʧ�� 0
////////////////////////////////////////////////////
BOOL DropWaveTail(int sizebyte)
{
	if (WaveState != STATE_CLOSED)
	{
		UINT8 * droptail=(UINT8 *)(pAccess-sizebyte);
		if((long)droptail<(long)((*pWaveTail)+sizeof(WAVEHEAD)))
			return false;
		else
			pAccess=droptail;
	}
	return true;
}
	
//////////////////////////////////////////////////
//
//  ɾ��¼��
//
//	������	id		��ɾ��¼����ID
//
//  ���أ�	true	�ɹ�
//			false	ʧ��
//
//////////////////////////////////////////////////
bool DeleteWave(long id)
{
	// temp
	CloseWave();
	//Todo
	if(IllegalWave(id))
		return false;
	WAVEHEAD *	pd = (WAVEHEAD*)id;
	int lenthofdrop=pd->Length ;
	UINT32 * ps=(UINT32 *)( (UINT32)pd+ sizeof(WAVEHEAD)+pd->Length );
	KillWatchDog();
	memmove((UINT32 *)pd,ps,((UINT32)(*pWaveTail)-(UINT32)pd));
	CLR_WATCHDOG();
	OpenWatchDog();
	(*pWaveTail) =(WAVEHEAD *)((UINT32)(*pWaveTail)-sizeof(WAVEHEAD)-lenthofdrop);
	pAccess=(UINT8 *)(*pWaveTail);
	return true;
}

//////////////////////////////////////////////////
//
//  ɾ������¼��
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void DeleteAllWave(void)
{
	CloseWave();
	pWaveHead = (*pWaveTail) = (WAVEHEAD*)WAVE_STORE_ADDR;
	pWaveHead->Length = 0;
}
		
//////////////////////////////////////////////////
//
//  ����¼��
//
//	������	type	¼�����ͣ�ȡֵ
//						WAVE_MIC		��˷�
//						WAVE_AUTO		����
//						WAVE_LINE		�绰��
//						WAVE_ALL		�������
//					ע�ͣ�	1)WAVE_MIC��WAVE_AUTO��WAVE_LINE 
//							  ������ '|' �������ʹ��, 
//							  ����������Ľ������ WAVE_ALL��
//							2)�������Կ��Ի���('|') WAVE_NEW 
//							  �����Ҹ����Ե���¼����
//
//			idbuf	��Ų��ҽ����buffer
//
//			buflen	idbuf�ĳ���
//
//  ���أ�	�鵽��¼������
//
//////////////////////////////////////////////////
int FindWave(int type, long *idbuf, int buflen)
{
	int			i, total = 0;
	WAVEHEAD	*h = pWaveHead;

	if (IllegalType(type))
		return 0;
	for (i = 0; (h != (*pWaveTail)) && ( i < buflen); i ++){
		if ((type & h->Flag & WAVE_TYPE) && (!(type & WAVE_NEW) || (h->Flag & WAVE_NEW))){
			*(idbuf + total) = (long)h;
			total ++;
		}
		h = (WAVEHEAD*)((long)h + sizeof(WAVEHEAD) + h->Length);
	}
	return total;
}

//////////////////////////////////////////////////
//
//  ��ȡ¼��ͷָ��
//
//	������	id		¼�� ID ��
//
//  ���أ�	ָ�� WAVEHEAD �ṹ��ָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
WAVEHEAD* GetWavePointer(long id)
{
	if (IllegalWave(id))
		return false;
	return (WAVEHEAD*)id;
}

//////////////////////////////////////////////////
//
//  ��ʼ��¼��������ɾ������¼��
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitWave(void)
{
	pWaveTail = (WAVEHEAD**)WAVE_TAIL_STORE;
	if(g_bFirstPowerOn)
	{
		pWaveHead = (*pWaveTail) = (WAVEHEAD*)WAVE_STORE_ADDR;
		pWaveHead->Length =0;
	}
	else
	{
		pWaveHead = (WAVEHEAD*)WAVE_STORE_ADDR;
		WAVEHEAD * pnext =pWaveHead;
		while( pnext <  (WAVEHEAD*)( WAVE_STORE_ADDR + WAVE_STORE_LEN) )
		{
			if( (pnext->Length < 0) || IllegalType(pnext->Flag) )
				break;		
			if( pnext== (*pWaveTail) ) return ;
			pnext=(WAVEHEAD*)( (long)pnext + pnext->Length + sizeof(WAVEHEAD) ); 
		}
		(*pWaveTail) = (WAVEHEAD*)WAVE_STORE_ADDR;
	}
	WaveState = STATE_CLOSED;
}

/*************************************************************************/

/* ͨ����¼���� */

/* ��λһ��ͨ����¼ */
PRIVATE HISTORY* LocateHistory(short iHistoryID)
{
	return (HISTORY*)(HISTORY_BASE_ADDR + sizeof(HISTORY) * iHistoryID);
}

/* ����һ��ͨ����¼ */
PRIVATE void EraseOneHistory(short iHistoryID)
{
	HISTORY		*his;

	his = LocateHistory(iHistoryID);
	LoadFlashSector(his);
	memset((void*)(FLASH_BUF_ADDR + (long)his % FLASH_SECTOR_SIZE), 0xFF, sizeof(HISTORY));
	SaveFlashSector(his);
}

/* д��һ��ͨ����¼ */
PRIVATE void WriteOneHistory(short iHistoryID, HISTORY const *pHistory)
{
	HISTORY		his;

	memcpy(&his, pHistory, sizeof(HISTORY));
	his.Flag = his.Flag & (~HISTORY_ERASED) | HISTORY_EXIST | HISTORY_NEWCREATE;
	FlashWrite((UINT16*)LocateHistory(iHistoryID), (UINT16*)(&his), sizeof(HISTORY) / 2);
}

/* �ж�ͨ����¼�Ϸ� */
PRIVATE HISTORY* LegalHistory(short iHistoryID)
{
	HISTORY*	phis;

	if ((iHistoryID >= HISTORY_MAX_COUNT) || (iHistoryID < 0))
		return NULL;
	phis = LocateHistory(iHistoryID);
	if (phis->Flag & HISTORY_ERASED)
		return NULL;
	if (!(phis->Flag & HISTORY_EXIST))
		return NULL;
	return phis;
}

/* �ж�ͨ����¼���ͺϷ� */
PRIVATE bool LegalHistoryType(short iHistoryType)
{
	if ((iHistoryType == HISTORY_DIALOUT) || (iHistoryType == HISTORY_CALLIN) || \
	   (iHistoryType == HISTORY_MISSED) || (iHistoryType == HISTORY_ALL))
	   return true;
	else
		return false;
}

/* ͨ����¼����Ƚ� */
PRIVATE int HistoryCompare(void const *key, void const *element)
{
	HISTORY		*pkey, *pelement;

	pkey = LocateHistory(*(short*)key);
	pelement = LocateHistory(*(short*)element);
	if (pkey->Time == pelement->Time)
		return 0;
	else
		return (pkey->Time > pelement->Time ? -1 : 1);
}


//////////////////////////////////////////////////
//
//  ��ʼ��ͨ����¼����
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitHistory(void)
{
	// added by hqf ��ʼ��ʱ����δ��ͨ����¼��
	short iNotVisitHis = 0 , iHisMissed;  
	short pHistoryId[HISTORY_MAX_COUNT];
	iHisMissed = ::FindHistory(HISTORY_MISSED, 
		pHistoryId , HISTORY_MAX_COUNT);
	for(short i = 0; i< iHisMissed; i++)
	{
		if(GetHistoryPointer(pHistoryId[i])->Flag & HISTORY_NEWCREATE)
			iNotVisitHis++;
	}
	if(iNotVisitHis==0)         
		//			if(LEDIsSet(LED_MISSEDCALL))
		ClearLED(LED_MISSEDCALL);
	else
		SetLED(LED_MISSEDCALL);
	//for (long addr = HISTORY_BASE_ADDR; addr < HISTORY_BASE_ADDR + HISTORY_MAX_COUNT * sizeof(HISTORY); addr += FLASH_SECTOR_SIZE)
	//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  ����ͨ����¼
//
//	������	pHistory	ָ������ͨ����¼
//
//  ���أ�	���� ID �ţ�ID >= 0, ���� -1 ��ʾʧ��
//
//////////////////////////////////////////////////
short AppendHistory(HISTORY *pHistory)
{
	HISTORY*	phis;
	DateTime	dt;
	short		i, j;

	for (i = 0; i < HISTORY_MAX_COUNT; i ++){
		phis = LocateHistory(i);
		if (phis->Flag & HISTORY_ERASED)
			break;
	}
	if (i == HISTORY_MAX_COUNT){
		for (i = 0; i < HISTORY_MAX_COUNT; i ++){
			phis = LocateHistory(i);
			if (!(phis->Flag & HISTORY_EXIST)){
				EraseOneHistory(i);
				break;
			}	
		}
	}
	if (i == HISTORY_MAX_COUNT){
		dt.year = 9999;
		for (i = 1; i < HISTORY_MAX_COUNT; i ++){
			phis = LocateHistory(i);
			if (phis->Time < dt){
				j = i;
				dt = phis->Time;
			}
		}
		i = j;
		EraseOneHistory(i);
	}
	WriteOneHistory(i, pHistory);
	return i;
}

//////////////////////////////////////////////////
//
//  ɾ��ͨ����¼
//
//	������	iHistoryID		��ɾ��ͨ����¼ID
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteHistroy(short iHistoryID)
{
	HISTORY*	phis;

	if (!(phis = LegalHistory(iHistoryID)))
		return false;
	return FlashWriteWord(&(phis->Flag), phis->Flag & ~HISTORY_EXIST);
}

//////////////////////////////////////////////////
//
//  �޸�ͨ����¼
//
//	������	iHistoryID		���޸�ͨ����¼ID
//			pHistory		�޸�����
//
//  ���أ�	�޸ĳɹ����
//
//////////////////////////////////////////////////
bool ModifyHistory(short iHistoryID, HISTORY const *pHistory)
{
	HISTORY*	phis;
	
	if (!(phis = LegalHistory(iHistoryID)))
		return false;
	EraseOneHistory(iHistoryID);
	WriteOneHistory(iHistoryID, pHistory);
	return true;
}

//////////////////////////////////////////////////
//
//  ɾ��ĳ��ͨ����¼
//
//	������	iHistoryType	���
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteHistoryType(short iHistoryType)
{
	short	buf[HISTORY_MAX_COUNT];
	int		len;
	
	if (!LegalHistoryType(iHistoryType))
		return false;
	len = FindHistory(iHistoryType, buf, HISTORY_MAX_COUNT);
	if (len == 0)
		return false;
	for (int i = 0; i < len; i ++)
		DeleteHistroy(buf[i]);
	return true;
}

//////////////////////////////////////////////////
//
//  ����ͨ����¼
//
//	������	iHistoryType	���
//			pHistoryIDBuf	��Ž����buffer
//			iBufLen			buffer ����
//
//  ���أ�	�鵽�ļ�¼����
//
//////////////////////////////////////////////////
int FindHistory(short iHistoryType, short *pHistoryIDBuf, int iBufLen)
{
	HISTORY*	phis;
	int			count = 0;
	
	for (short i = 0; (i < HISTORY_MAX_COUNT) && (count < iBufLen); i ++){
		if ((phis = LegalHistory(i)) != NULL){
			if ((phis->HistoryType == iHistoryType) || (iHistoryType == HISTORY_ALL) || ((iHistoryType == HISTORY_NEW) && (phis->Flag & HISTORY_NEWCREATE))){
				*(pHistoryIDBuf + count) = i;
				count ++;
			}
		}
	}
	qsort(pHistoryIDBuf, count, 2, HistoryCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡͨ����¼ָ�루ֻ����
//
//	������	iHistoryID		ͨ����¼ID
//
//  ���أ�	ָ��ͨ����¼��ָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
HISTORY* GetHistoryPointer(short iHistoryID)
{
	return LegalHistory(iHistoryID);
}	

//////////////////////////////////////////////////
//
//  ��ͨ����¼��ǳ��ѷ���
//
//	������	iHistoryID		ͨ����¼ID
//
//  ���أ�	�ɹ����
//
//////////////////////////////////////////////////
bool MarkHistory(short iHistoryID)
{
	HISTORY*	phis;
	
	if (!(phis = LegalHistory(iHistoryID)))
		return false;
	return FlashWriteWord(&(phis->Flag), phis->Flag & ~HISTORY_NEWCREATE);
}


/*************************************************************************/

/* �������� */

/* ��λ���� */
PRIVATE MEMO* LocateMemo(short iMemoID)
{
	return (MEMO*)(MEMO_BASE_ADDR + sizeof(MEMO) * iMemoID);
}

/* ����ĳ������ */
PRIVATE void EraseOneMemo(short iMemoID)
{
	MEMO		*memo;

	memo = LocateMemo(iMemoID);
	LoadFlashSector(memo);
	memset((void*)(FLASH_BUF_ADDR + (long)memo % FLASH_SECTOR_SIZE), 0xFF, sizeof(MEMO));
	SaveFlashSector(memo);
}

/* дĳ�����½� Flash */
PRIVATE void WriteOneMemo(short iMemoID, MEMO const *pMemo)
{
	MEMO		memo;

	memcpy(&memo, pMemo, sizeof(MEMO));
	memo.Flag = memo.Flag & (~MEMO_ERASED) | MEMO_EXIST;
	FlashWrite((UINT16*)LocateMemo(iMemoID), (UINT16*)(&memo), sizeof(MEMO) / 2);
}

/* �жϼ��ºϷ� */
PRIVATE MEMO* LegalMemo(short iMemoID)
{
	MEMO*	pmemo;

	if ((iMemoID >= MEMO_MAX_COUNT) || (iMemoID < 0))
		return NULL;
	pmemo = LocateMemo(iMemoID);
	if (pmemo->Flag & MEMO_ERASED)
		return NULL;
	if (!(pmemo->Flag & MEMO_EXIST))
		return NULL;
	return pmemo;
}

/* ���Ҽ��±ȽϺ��� */
PRIVATE int MemoCompare(void const *key, void const *element)
{
	MEMO	*pkey, *pelement;

	pkey = LocateMemo(*(short*)key);
	pelement = LocateMemo(*(short*)element);
	return strcmp(pkey->Title, pelement->Title);
}

//////////////////////////////////////////////////
//
//  ��ʼ������
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitMemo(void)
{
	//for (long addr = MEMO_BASE_ADDR; addr < MEMO_BASE_ADDR + MEMO_MAX_COUNT * sizeof(MEMO); addr += FLASH_SECTOR_SIZE)
	//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  ��������
//
//	������	pMemo		��������ָ��
//
//  ���أ�	���� ID��ID >= 0, ���� -1 ��ʾʧ��
//
//////////////////////////////////////////////////
short AppendMemo(MEMO *pMemo)
{
	MEMO*		pmemo;
	short		i, j;

	for (i = 0; i < MEMO_MAX_COUNT; i ++){
		pmemo = LocateMemo(i);
		if (pmemo->Flag & MEMO_ERASED)
			break;
	}
	if (i == MEMO_MAX_COUNT){
		for (i = 0; i < MEMO_MAX_COUNT; i ++){
			pmemo = LocateMemo(i);
			if (!(pmemo->Flag & MEMO_EXIST)){
				EraseOneMemo(i);
				break;
			}	
		}
	}
	if (i == MEMO_MAX_COUNT)
		return -1;
	WriteOneMemo(i, pMemo);
	return i;
}

//////////////////////////////////////////////////
//
//  ɾ������
//
//	������	iMemoID		ɾ�����µ� ID
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteMemo(short iMemoID)
{
	MEMO*	pmemo;

	if (!(pmemo = LegalMemo(iMemoID)))
		return false;
	return FlashWriteWord(&(pmemo->Flag), pmemo->Flag & ~MEMO_EXIST);
}
	
//////////////////////////////////////////////////
//
//  �޸ļ���
//
//	������	iMemoID		�޸ļ��µ�ID
//			pMemo		�޸�����
//
//  ���أ�	�޸ĳɹ����
//
//////////////////////////////////////////////////
bool ModifyMemo(short iMemoID, MEMO const *pMemo)
{
	MEMO*	pmemo;
	
	if (!(pmemo = LegalMemo(iMemoID)))
		return false;
	EraseOneMemo(iMemoID);
	WriteOneMemo(iMemoID, pMemo);
	return true;
}
	
//////////////////////////////////////////////////
//
//  ���Ҽ���
//
//	������	pMemoIDBuf	��Ų��ҽ���� buffer
//			iBufLen		buffer ����
//
//  ���أ�	�鵽�ĸ���
//
//////////////////////////////////////////////////
int FindMemo(short *pMemoIDBuf, int iBufLen)
{
	MEMO*		pmemo;
	int			count = 0;
	
	for (short i = 0; (i < MEMO_MAX_COUNT) && (count < iBufLen); i ++){
		CLR_WATCHDOG();
		if ((pmemo = LegalMemo(i)) != NULL){
			*(pMemoIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pMemoIDBuf, count, 2, MemoCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ����ָ��
//
//	������	iMemoID		���µ� ID
//
//  ���أ�	���µ�ָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
MEMO* GetMemoPointer(short iMemoID)
{
	return LegalMemo(iMemoID);
}	
 
/*************************************************************************/

/* �������� */

extern UINT8 g_iTwoBit2OneBitHeight[256];
extern UINT8 g_iTwoBit2OneBitLow[256];
extern UINT16 g_iOneBit2TwoBitMap[256];

/* ��ȡ�����ַ */
PRIVATE UINT8* GetPictureAddr(short iPicID)
{
	if ((iPicID < 0) || (iPicID >= PICTURE_MAX_COUNT))
		return NULL;
	else
		return (UINT8*)(PICTURE_BASE_ADDR + PICTURE_STORESIZE * iPicID);
}

//////////////////////////////////////////////////
//
//  װ�뻭��
//
//	������	iPicID		�������ţ�0 <= ����� < PICTURE_MAX_COUNT
//			pBuf		װ�� buffer����֤�� buffer size >= PICTURE_STORESIZE
//
//  ���أ�	װ��ɹ����
//
//////////////////////////////////////////////////
bool LoadPicture(short iPicID, void *pBuf)
{
	UINT8	*p = GetPictureAddr(iPicID);
	
	if (!p)
		return false;
	for (int i = 0; i < PICTURE_STORESIZE; i ++)
		*((UINT16*)pBuf + i) = g_iOneBit2TwoBitMap[*(p + i)];
	return true;
}

//////////////////////////////////////////////////
//
//  ���滭��
//
//	������	iPicID		�������ţ�0 <= ����� < PICTURE_MAX_COUNT
//			pBuf		������Ļ������ݣ�2bit��ʽ
//
//  ���أ�	����ɹ����
//
//////////////////////////////////////////////////
bool SavePicture(short iPicID, void *pBuf)
{
	UINT8	*p; 
	int		i;//, j = 0;
	
	p = GetPictureAddr(iPicID);
	if (!p)
		return false;
	//for (long addr = (long)p; addr < (long)p + PICTURE_STORESIZE; addr += FLASH_SECTOR_SIZE, j ++){
		for (i = 0; i < FLASH_SECTOR_SIZE; i ++){
			*((UINT8*)(FLASH_BUF_ADDR + i)) = g_iTwoBit2OneBitHeight[*((UINT8*)pBuf + (i * 2))] | \
											  g_iTwoBit2OneBitLow[*((UINT8*)pBuf + (i * 2 + 1))];
		}
		FlashEraseSector((UINT16*)p);
		FlashWrite((UINT16*)p, (UINT16*)FLASH_BUF_ADDR, FLASH_SECTOR_SIZE / 2);
		//FlashWrite((UINT16*)p, (UINT16*)FLASH_BUF_ADDR, 3000 / 2);
//	}
	return true;
}

/*************************************************************************/

/* ��Ƭ���� */

/* ��λ��Ƭ */
PRIVATE CARD* LocateCard(short iCardID)
{
	return (CARD*)(CARD_BASE_ADDR + sizeof(CARD) * iCardID);
}

/*           hqf                     */
void hwmemset(void *pointer , int c , unsigned int count)
{
	int i;
	UINT8 *pstart = (UINT8 *) pointer;
	for(i = 0; i < count;i++, pstart++)
		*pstart = (UINT8) c;
}
/*           hqf                     */

void *memcpy( void *dest, const void *src, unsigned int count )
{
	register int i;
	register UINT8 *psrc = (UINT8 *) src;
	register UINT8 *pdest = (UINT8 *)dest;

	for(i = 0; i < count;i++, psrc++, pdest++)
		*pdest = *psrc;
	return dest;
}

void  * memset (void * s, int  c, unsigned int n)
{
	register unsigned char  val = (unsigned char)  c;
	register unsigned char *ptr = (unsigned char*) s;
	register unsigned char *end = ptr + n;
    while ( ptr < end )  *ptr++ = val;
    return ( s );
}
/* ����һ����Ƭ */ 
PRIVATE void EraseOneCard(short iCardID)
{
	CARD	*card;

	card = LocateCard(iCardID);
	LoadFlashSector(card);
	hwmemset((void*)(FLASH_BUF_ADDR + (long)card % FLASH_SECTOR_SIZE), 0xFF, sizeof(CARD));
	SaveFlashSector(card);
}

/* дһ����Ƭ�� Flash */
PRIVATE void WriteOneCard(short iCardID, CARD const *pCard)
{
	CARD		card;

	memcpy(&card, pCard, sizeof(CARD));
	card.Flag = card.Flag & (~CARD_ERASED) | CARD_EXIST;
	FlashWrite((UINT16*)LocateCard(iCardID), (UINT16*)(&card), sizeof(CARD) / 2);
}

/* �ж���Ƭ�Ϸ� */
PRIVATE CARD* LegalCard(short iCardID)
{
	CARD*	pcard;

	if ((iCardID >= CARD_MAX_COUNT) || (iCardID < 0))
		return NULL;
	pcard = LocateCard(iCardID);
	if (pcard->Flag & CARD_ERASED)
		return NULL;
	if (!(pcard->Flag & CARD_EXIST))
		return NULL;
	return pcard;
}

/* ������Ƭ�ȽϺ��� */
PRIVATE int CardCompare(void const *key, void const *element)
{
	CARD	*pkey, *pelement;

	pkey = LocateCard(*(short*)key);
	pelement = LocateCard(*(short*)element);
	return strcmp(pkey->Name, pelement->Name);
}

bool IsLegalSubString(char *pString , char *pSubString)
{
	if(!pString || !pSubString || !pString[0] || !pSubString[0])
		return false;
	char *pCurrent = strstr(pString, pSubString);
	if(!pCurrent)
		return false;

	char *pPrev = CharPre(pString , pCurrent);
	if((pCurrent[0] & 0x80) && (pPrev[0] & 0x80)
		&& ((pCurrent - pPrev) & 0x01)) 
		return false;
	return true;
}

/* ����Ƭ�в����ַ��� */
PRIVATE bool FindOneCardString(char const *pString, short iCardID, int iFindScope)
{
	CARD*		pcard;
	bool		flag = false;
	char		*p;

	if (!(pcard = LegalCard(iCardID)))
		return false;
	if (iFindScope == CARD_FINDALL){
		flag = IsLegalSubString(pcard->Name, (char *)pString);
		if(!flag)
		{
			p = pcard->Content;
			for (int i = 0; i < CARD_CONTENT_SEGMENT; i ++){
				if (IsLegalSubString(p, (char *)pString))
					flag = true;
				p += strlen(p) + 1;
			}
		}
	}
	else if(iFindScope == CARD_FINDHANZINAME)
	{
		if (IsLegalSubString(pcard->Name, (char *)pString))
			flag = true;
	}
	else if(iFindScope == CARD_FINDABBRNAME)
	{
		char strAbbr[CARD_NAME_LEN+1];
		extern int GetAbbreviation(const char * hanzi, char * pinyin);
		GetAbbreviation(pcard->Name , strAbbr);
		if(strstr(strAbbr , pString))
			flag = true;
	}
	return flag;
}

//////////////////////////////////////////////////
//
//  ��ʼ����Ƭ
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitCard(void)
{
	extern CARD g_cloneCard;
	memset(&g_cloneCard , 0 , sizeof(CARD));//modified by hqf
	//for (long addr = CARD_BASE_ADDR; addr < CARD_BASE_ADDR + CARD_MAX_COUNT * sizeof(CARD); addr += FLASH_SECTOR_SIZE)
	//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  ������Ƭ
//
//	������	pCard		������Ƭ����
//
//  ���أ�	������Ƭ ID��ID >= 0, ���� -1 ��ʾʧ��
//
//////////////////////////////////////////////////
short AppendCard(CARD *pCard)
{
	CARD*		pcard;
	short		i, j = -1;
	for (i = 0; i < CARD_MAX_COUNT; i ++){
		pcard = LocateCard(i);
		if (pcard->Flag & CARD_ERASED)
			break;
		if (!(pcard->Flag & CARD_EXIST))
			j = i;
	}
	if ((i == CARD_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneCard(i);
	}
	if (i == CARD_MAX_COUNT)
		return -1;
	WriteOneCard(i, pCard);
	return i;
}

//////////////////////////////////////////////////
//
//  ɾ����Ƭ
//
//	������	iCardID		ɾ����Ƭ�� ID
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteCard(short iCardID)
{
	CARD*	pcard;

	if (!(pcard = LegalCard(iCardID)))
		return false;
	return FlashWriteWord(&(pcard->Flag), pcard->Flag & ~CARD_EXIST);
}

//////////////////////////////////////////////////
//
//  �޸���Ƭ
//
//	������	iCardID		�޸���Ƭ�� ID
//			pCard		�޸�����
//
//  ���أ�	�޸ĳɹ����
//
//////////////////////////////////////////////////
bool ModifyCard(short iCardID, CARD const *pCard)
{
	CARD*	pcard;
	
	if (!(pcard = LegalCard(iCardID)))
		return false;
	EraseOneCard(iCardID);
	WriteOneCard(iCardID, pCard);
	return true;
}
	
//////////////////////////////////////////////////
//
//  ������Ƭ
//
//	������	iCardCaseID		��Ƭ�� ID
//			pCardIDBuf		��Ų��ҽ���� buffer
//			iBufLen			buffer ����
//
//  ���أ�	�鵽�ĸ���
//
//////////////////////////////////////////////////
int FindCard(short iCardCaseID, short *pCardIDBuf, int iBufLen)
{
	CARD*		pcard;
	int			count = 0;
	
	for (short i = 0; (i < CARD_MAX_COUNT) && (count < iBufLen); i ++){
		if ((pcard = LegalCard(i)) != NULL){
			if (pcard->CardCaseID == iCardCaseID){
				*(pCardIDBuf + count) = i;
				count ++;
			}
		}
	}
	qsort(pCardIDBuf, count, 2, CardCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  �����ַ���
//
//	������	pString		�����ַ���
//			pCardIDBuf	��Ų��ҽ���� buffer
//			iBufLen		buffer ����
//			iFindScope	���ҷ�Χ��ȡֵ
//							CARD_FINDNAME		ֻ���������ֶ�
//							CARD_FINDALL		���������ֶ�
//  ���أ�	�鵽�ĸ���
//
//////////////////////////////////////////////////
int SearchCardString(char const *pString, short *pCardIDBuf, int iBufLen, int iFindScope)
{
	CARD*		pcard;
	int		count = 0;

	for (short i = 0; (i < CARD_MAX_COUNT) && (count < iBufLen); i ++){
		if ((pcard = LegalCard(i)) != NULL){
			if (FindOneCardString(pString, i, iFindScope)){
				*(pCardIDBuf + count) = i;
				count ++;
			}
		}
	}
	return count;
}

//////////////////////////////////////////////////
//
//  �����ַ���
//
//	������	pString		�����ַ���
//			pCardIDBuf	��Ŵ�����Ƭ ID �� buffer������ǰ��
//						��Ų��ҽ���� buffer�����ú�
//			iBufLen		buffer ����
//			iFindScope	���ҷ�Χ��ȡֵ
//							CARD_FINDNAME		ֻ���������ֶ�
//							CARD_FINDALL		���������ֶ�
//  ���أ�	�鵽�ĸ���
//
//////////////////////////////////////////////////
int FindCardString(char const *pString, short *pCardIDBuf, int iBufLen, int iFindScope)
{
	int		i, j;

	if (*pString == '\0')
		return 0;
	for (i = j = 0; i < iBufLen; i ++){
		if (FindOneCardString(pString, *(pCardIDBuf + i), iFindScope)){
			*(pCardIDBuf + j) = *(pCardIDBuf + i);
			j ++;
		}
	}
	return j;
}

//////////////////////////////////////////////////
//
//  ���ҵ绰����
//
//	������	pNumber			�������
//			pCardIDBuf		��Ų��ҽ�� ID �� buffer
//			pNumberTypeBuf	��ŵ绰���͵� buffer
//			iBufLen			�������� buffer �ĳ���(Ҫ�����)
//
//  ���أ�	�鵽�ĸ���
//
//////////////////////////////////////////////////
int FindCardNumber(char const *pNumber, short *pCardIDBuf, short *pNumberTypeBuf, short int iBufLen)
{
	CARD	*pcard;
	int		i, j, count = 0;
	char	*p;
	bool	flag; 
	
	for (i = 0; (i < CARD_MAX_COUNT) && (count < iBufLen); i ++){
		pcard = LocateCard(i);
		if (!(pcard->Flag & CARD_ERASED) && (pcard->Flag & CARD_EXIST)){
			flag = false;
			p = pcard->Content;
			for (j = 0; (j < CARD_CONTENT_SEGMENT) && (j <= CARD_NUMBEREND) && (!flag); j ++){
				if (j == CARD_INSIDE){
					if (pNumber && *pNumber && strstr(pNumber, p))	// byw
					{
						if (strcmp(pNumber, p) == 0)	// byw
						{
							flag = true; 
							break;
						}
					}
				}
				else if (j >= CARD_NUMBERSTART){
					//if (pNumber && *pNumber && strstr(p, pNumber)) // modified by hqf || ((*p !=0) && strstr(pNumber, p)))  //modify by wangfei
					if (pNumber && *pNumber && strstr(pNumber, p))	// byw
					{
						if (strcmp(strstr(pNumber, p), p) == 0){	// byw
							flag = true; 
							break;
						}
					}
				}
				p += strlen(p) + 1;
			}
			if (flag){
				*(pCardIDBuf + count) = i;
				*(pNumberTypeBuf + count) = j-CARD_NUMBERSTART;
				count ++;
			}
		}////// end if(!...)
	}
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ��Ƭָ��
//
//	������	iCardID		��Ƭ�� ID
//
//  ���أ�	��Ƭָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
CARD* GetCardPointer(short iCardID)
{
	return LegalCard(iCardID);
}


/*************************************************************************/

/* ��Ƭ�� */

/* ��λ��Ƭ�� */
PRIVATE CARDCASE* LocateCardCase(short iCardCaseID)
{
	return (CARDCASE*)(CARDCASE_BASE_ADDR + sizeof(CARDCASE) * iCardCaseID);
}

/* ������Ƭ�� */
PRIVATE void EraseOneCardCase(short iCardCaseID)
{
	CARDCASE		*cardcase;

	cardcase = LocateCardCase(iCardCaseID);
	LoadFlashSector(cardcase);
	memset((void*)(FLASH_BUF_ADDR + (long)cardcase % FLASH_SECTOR_SIZE), 0xFF, sizeof(CARDCASE));
	SaveFlashSector(cardcase);
}

/* д��Ƭ�� */
PRIVATE void WriteOneCardCase(short iCardCaseID, CARDCASE const *pCardCase, short iPrivate)
{
	CARDCASE		cardcase;

	memcpy(&cardcase, pCardCase, sizeof(CARDCASE));
	cardcase.Private = iPrivate;
	cardcase.Flag = cardcase.Flag & (~CARDCASE_ERASED) | CARDCASE_EXIST;
	FlashWrite((UINT16*)LocateCardCase(iCardCaseID), (UINT16*)(&cardcase), sizeof(CARDCASE) / 2);
}

/* �ж���Ƭ�кϷ��� */
PRIVATE CARDCASE* LegalCardCase(short iCardCaseID)
{
	CARDCASE*	pcardcase;

	if ((iCardCaseID >= CARDCASE_MAX_COUNT) || (iCardCaseID < 0))
		return NULL;
	pcardcase = LocateCardCase(iCardCaseID);
	if (pcardcase->Flag & CARDCASE_ERASED)
		return NULL;
	if (!(pcardcase->Flag & CARDCASE_EXIST))
		return NULL;
	return pcardcase;
}

/* ��Ƭ�в��ұȽϺ��� */
PRIVATE int CardCaseCompare(void const *key, void const *element)
{
	CARDCASE	*pkey, *pelement;

	pkey = LocateCardCase(*(short*)key);
	pelement = LocateCardCase(*(short*)element);
	return pkey->Private - pelement->Private;
}

/* ��ȡ�½���Ƭ�������� */
PRIVATE short CardCaseNewIndex(void)
{
	short		buf[CARDCASE_MAX_COUNT], id = -1;
	int			count, i;

	count = FindCardCase(buf, CARDCASE_MAX_COUNT);
	for (i = 0; i < count; i ++){
		if (LocateCardCase(buf[i])->Private > id)
			id = LocateCardCase(buf[i])->Private;
	}
	return id + 1;
}

//////////////////////////////////////////////////
//
//  ��ʼ����Ƭ��
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitCardCase(void)
{
	extern short g_nCases;
	g_nCases = 0;
	//for (long addr = CARDCASE_BASE_ADDR; addr < CARDCASE_BASE_ADDR + CARDCASE_MAX_COUNT * sizeof(CARDCASE); addr += FLASH_SECTOR_SIZE)
	//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  ������Ƭ��
//
//	������	pCardCase	������Ƭ������
//
//  ���أ�	���� ID��ID >= 0, ���� -1 ��ʾʧ��
//
//////////////////////////////////////////////////
short AppendCardCase(CARDCASE *pCardCase)
{
	CARDCASE	*pcardcase;
	short		i, j = -1;

	for (i = 0; i < CARDCASE_MAX_COUNT; i ++){
		pcardcase = LocateCardCase(i);
		if (pcardcase->Flag & CARDCASE_ERASED)
			break;
		if (!(pcardcase->Flag & CARDCASE_EXIST))
			j = i;
	}
	if ((i == CARDCASE_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneCardCase(i);
	}
	if (i == CARDCASE_MAX_COUNT)
		return -1;
	WriteOneCardCase(i, pCardCase, CardCaseNewIndex());
	return i;
}

//////////////////////////////////////////////////
//
//  ɾ����Ƭ��
//
//	������	iCardCaseID		��ɾ����Ƭ��ID
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteCardCase(short iCardCaseID)
{
	CARDCASE*	pcardcase;

	if (!(pcardcase = LegalCardCase(iCardCaseID)))
		return false;
	return FlashWriteWord(&(pcardcase->Flag), pcardcase->Flag & ~CARDCASE_EXIST);
}
	
//////////////////////////////////////////////////
//
//  �޸���Ƭ��
//
//	������	iCardCaseID		���޸���Ƭ��ID
//			pCardCase		�޸�����
//
//  ���أ�	�޸ĳɹ����
//
//////////////////////////////////////////////////
bool ModifyCardCase(short iCardCaseID, CARDCASE const *pCardCase)
{
	CARDCASE	*pcardcase;
	short		iprivate;
	
	if (!(pcardcase = LegalCardCase(iCardCaseID)))
		return false;
	iprivate = pcardcase->Private;
	EraseOneCardCase(iCardCaseID);
	WriteOneCardCase(iCardCaseID, pCardCase, iprivate);
	return true;
}
	
//////////////////////////////////////////////////
//
//  ������Ƭ��
//
//	������	pCardCaseIDBuf		���ҽ�� buffer
//			iBufLen				buffer ����
//
//  ���أ�	�鵽����
//
//////////////////////////////////////////////////
int FindCardCase(short *pCardCaseIDBuf, int iBufLen)
{
	CARDCASE*	pcardcase;
	int			count = 0;
	
	for (short i = 0; (i < CARDCASE_MAX_COUNT) && (count < iBufLen); i ++){
		CLR_WATCHDOG();
		if ((pcardcase = LegalCardCase(i)) != NULL){
			*(pCardCaseIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pCardCaseIDBuf, count, 2, CardCaseCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ��Ƭ��ָ��
//
//	������	iCardCaseID		��Ƭ�� ID
//
//  ���أ�	��Ƭ��ָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
CARDCASE* GetCardCasePointer(short iCardCaseID)
{
	return LegalCardCase(iCardCaseID);
}	

//////////////////////////////////////////////////
//
//  ��Ƭ������
//
//	������	pCardCaseIDBuf		��������Ƭ�� ID buffer
//			iCount				���� ID ����
//
//  ���أ�	����ɹ����
//
//////////////////////////////////////////////////
bool SortCardCase(short *pCardCaseIDBuf, int iCount)
{
	CARDCASE*	pcardcase;

	memcpy((void*)FLASH_BUF_ADDR, (void*)CARDCASE_BASE_ADDR, FLASH_SECTOR_SIZE);
	for (int i = 0; i < iCount; i ++){
		pcardcase = LegalCardCase(*(pCardCaseIDBuf + i));
		if (pcardcase)
			((CARDCASE*)((long)pcardcase - CARDCASE_BASE_ADDR + FLASH_BUF_ADDR))->Private = i;
		else
			return false;
	}
	FlashEraseSector((UINT16*)CARDCASE_BASE_ADDR);
	SaveFlashSector((void*)CARDCASE_BASE_ADDR);
	return true;
}


/*************************************************************************/

/* ���� */

/* ��λ���� */
PRIVATE ALARM* LocateAlarm(short iAlarmID)
{
	return (ALARM*)(ALARM_BASE_ADDR + sizeof(ALARM) * iAlarmID);
}

/* ����ĳ������ */
PRIVATE void EraseOneAlarm(short iAlarmID)
{
	ALARM		*alarm;

	alarm = LocateAlarm(iAlarmID);
	LoadFlashSector(alarm);
	memset((void*)(FLASH_BUF_ADDR + (long)alarm % FLASH_SECTOR_SIZE), 0xFF, sizeof(ALARM));
	SaveFlashSector(alarm);
}

/* дĳ�����ѽ� Flash */
PRIVATE void WriteOneAlarm(short iAlarmID, ALARM const *pAlarm)
{
	ALARM		alarm;

	memcpy(&alarm, pAlarm, sizeof(ALARM));
	alarm.Flag = alarm.Flag & (~ALARM_ERASED) | ALARM_EXIST;
	FlashWrite((UINT16*)LocateAlarm(iAlarmID), (UINT16*)(&alarm), sizeof(ALARM) / 2);
}

/* �ж����ѺϷ� */
PRIVATE ALARM* LegalAlarm(short iAlarmID)
{
	ALARM*	palarm;

	if ((iAlarmID >= ALARM_MAX_COUNT) || (iAlarmID < 0))
		return NULL;
	palarm = LocateAlarm(iAlarmID);
	if (palarm->Flag & ALARM_ERASED)
		return NULL;
	if (!(palarm->Flag & ALARM_EXIST))
		return NULL;
	return palarm;
}

/* �������ѱȽϺ��� */
PRIVATE int AlarmCompare(void const *key, void const *element)
{
	ALARM	*pkey, *pelement;

	pkey = LocateAlarm(*(short*)key);
	pelement = LocateAlarm(*(short*)element);
	return strcmp(pkey->Title, pelement->Title);
}

//////////////////////////////////////////////////
//
//  ��ʼ������
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitAlarmData(void)
{
//	for (long addr = ALARM_BASE_ADDR; addr < ALARM_BASE_ADDR + ALARM_MAX_COUNT * sizeof(ALARM); addr += FLASH_SECTOR_SIZE)
//		FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  ��������
//
//	������	pAlarm		��������ָ��
//
//  ���أ�	���� ID��ID >= 0, ���� -1 ��ʾʧ��
//
//////////////////////////////////////////////////
short AppendAlarm(ALARM *pAlarm)
{
	ALARM*		palarm;
	short		i, j = -1;

	for (i = 0; i < ALARM_MAX_COUNT; i ++){
		palarm = LocateAlarm(i);
		if (palarm->Flag & ALARM_ERASED)
			break;
		if (!(palarm->Flag & ALARM_EXIST))
			j = i;
	}
	if ((i == ALARM_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneAlarm(i);
	}
	if (i == ALARM_MAX_COUNT)
		return -1;
	WriteOneAlarm(i, pAlarm);
	return i;
}


//////////////////////////////////////////////////
//
//  ɾ������
//
//	������	iAlarmID		ɾ�����ѵ� ID
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteAlarm(short iAlarmID)
{
	ALARM*	palarm;

	if (!(palarm = LegalAlarm(iAlarmID)))
		return false;
	return FlashWriteWord(&(palarm->Flag), palarm->Flag & ~ALARM_EXIST);
}
	
//////////////////////////////////////////////////
//
//  �޸�����
//
//	������	iAlarmID	�޸����ѵ�ID
//			pAlarm		�޸�����
//
//  ���أ�	�޸ĳɹ����
//
//////////////////////////////////////////////////
bool ModifyAlarm(short iAlarmID, ALARM const *pAlarm)
{
	ALARM*	palarm;
	
	if (!(palarm = LegalAlarm(iAlarmID)))
		return false;
	EraseOneAlarm(iAlarmID);
	WriteOneAlarm(iAlarmID, pAlarm);
	return true;
}
	
//////////////////////////////////////////////////
//
//  ��������
//
//	������	pAlarmIDBuf	��Ų��ҽ���� buffer
//			iBufLen		buffer ����
//
//  ���أ�	�鵽�ĸ���
//
//////////////////////////////////////////////////
int FindAlarm(short *pAlarmIDBuf, int iBufLen)
{
	ALARM*		palarm;
	int			count = 0;
	
	for (short i = 0; (i < ALARM_MAX_COUNT) && (count < iBufLen); i ++){
		if ((palarm = LegalAlarm(i)) != NULL){
			*(pAlarmIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pAlarmIDBuf, count, 2, AlarmCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ����ָ��
//
//	������	iAlarmID		���ѵ� ID
//
//  ���أ�	���ѵ�ָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
ALARM* GetAlarmPointer(short iAlarmID)
{
	return LegalAlarm(iAlarmID);
}	

/*************************************************************************/

/* ���� */

/* ��λ���� */
PRIVATE RING* LocateRing(short iRingID)
{
	return (RING*)(RING_BASE_ADDR + sizeof(RING) * iRingID);
}

/* ����һ������ */
PRIVATE void EraseOneRing(short iRingID)
{
	RING		*ring;

	ring = LocateRing(iRingID);
	LoadFlashSector(ring);
	memset((void*)(FLASH_BUF_ADDR + (long)ring % FLASH_SECTOR_SIZE), 0xFF, sizeof(RING));
	SaveFlashSector(ring);
}

/* дһ�������� Flash */
PRIVATE void WriteOneRing(short iRingID, RING const *pRing)
{
	RING		ring;

	memcpy(&ring, pRing, sizeof(RING));
	ring.Flag = ring.Flag & (~RING_ERASED) | RING_EXIST;
	FlashWrite((UINT16*)LocateRing(iRingID), (UINT16*)(&ring), sizeof(RING) / 2);
}

/* �ж������Ϸ� */
PRIVATE RING* LegalRing(short iRingID)
{
	RING*	pring;

	if ((iRingID >= RING_MAX_COUNT) || (iRingID < 0))
		return NULL;
	pring = LocateRing(iRingID);
	if (pring->Flag & RING_ERASED)
		return NULL;
	if (!(pring->Flag & RING_EXIST))
		return NULL;
	return pring;
}

/* ���������ȽϺ��� */
PRIVATE int RingCompare(void const *key, void const *element)
{
	RING	*pkey, *pelement;

	pkey = LocateRing(*(short*)key);
	pelement = LocateRing(*(short*)element);
	return strcmp(pkey->Name, pelement->Name);
}

//////////////////////////////////////////////////
//
//  ��ʼ������
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitRing(void)
{
//	for (long addr = RING_BASE_ADDR; addr < RING_BASE_ADDR + RING_MAX_COUNT * sizeof(RING); addr += FLASH_SECTOR_SIZE)
//		FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  ��������
//
//	������	pRing		������������
//
//  ���أ�	�������� ID��ID >= 0, ���� -1 ��ʾʧ��
//
//////////////////////////////////////////////////
short AppendRing(RING *pRing)
{
	RING*		pring;
	short		i, j = -1;

	for (i = 0; i < RING_MAX_COUNT; i ++){
		pring = LocateRing(i);
		if (pring->Flag & RING_ERASED)
			break;
		if (!(pring->Flag & RING_EXIST))
			j = i;
	}
	if ((i == RING_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneRing(i);
	}
	if (i == RING_MAX_COUNT)
		return -1;
	WriteOneRing(i, pRing);
	return i;
}

//////////////////////////////////////////////////
//
//  ɾ������
//
//	������	iRingID		ɾ�������� ID
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteRing(short iRingID)
{
	RING*	pring;
	int		count;
	short	buf[CARDCASE_MAX_COUNT];

	if (!(pring = LegalRing(iRingID)))
		return false;
	if (pring->Flag & RING_SYSTEM)
		return false;
	count = FindCardCase(buf, CARDCASE_MAX_COUNT);
	for (int i = 0; i < count; i ++){
		if (GetCardCasePointer(buf[i])->RingID == iRingID)
			return false;
	}
	return FlashWriteWord(&(pring->Flag), pring->Flag & ~RING_EXIST);
}

//////////////////////////////////////////////////
//
//  �޸�����
//
//	������	iRingID		�޸������� ID
//			pRing		�޸�����
//
//  ���أ�	�޸ĳɹ����
//
//////////////////////////////////////////////////
bool ModifyRing(short iRingID, RING const *pRing)
{
	RING*	pring;
	
	if (!(pring = LegalRing(iRingID)))
		return false;
	EraseOneRing(iRingID);
	WriteOneRing(iRingID, pRing);
	return true;
}
	
//////////////////////////////////////////////////
//
//  ��������
//
//	������	pRingIDBuf		��Ų��ҽ���� buffer
//			iBufLen			buffer ����
//
//  ���أ�	�鵽�ĸ���
//
//////////////////////////////////////////////////
int FindRing(short *pRingIDBuf, int iBufLen)
{
	RING*		pring;
	int			count = 0;
	
	for (short i = 0; (i < RING_MAX_COUNT) && (count < iBufLen); i ++){
		if ((pring = LegalRing(i)) != NULL){
			*(pRingIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pRingIDBuf, count, 2, RingCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ����ָ��
//
//	������	iRingID		������ ID
//
//  ���أ�	����ָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
RING* GetRingPointer(short iRingID)
{
	return LegalRing(iRingID);
}

//////////////////////////////////////////////////
//
//  ��ѯ����ʹ����
//
//	������	iRingID		����ID
//			pUserType	����ʹ��������(DATATYPE����)
//			pUserID		����ʹ����ID
//
//  ���أ�	ture		����ʹ�ã���ϸ����� pUserType, pUserID ��
//			flase		û�б�ʹ��
//
//////////////////////////////////////////////////
bool RingInUse(short iRingID, short *pUserType, short *pUserID)
{
	CARD*		pcard;
	CARDCASE*	pcardcase;
	ALARM*		palarm;
	short		i, id, type;
	
	// ��ѯ��Ƭ����
	for (i = 0; i < CARD_MAX_COUNT; i ++){
		if ((pcard = LegalCard(i)) != NULL){
			if (pcard->RingID == iRingID){
				*pUserType = DATA_CARD;
				*pUserID = i;
				return true;
			}
		}
	}

	// ��ѯ��Ƭ������
	for (i = 0; i < CARDCASE_MAX_COUNT; i ++){
		if ((pcardcase = LegalCardCase(i)) != NULL){
			if (pcardcase->RingID == iRingID){
				*pUserType = DATA_CARDCASE;
				*pUserID = i;
				return true;
			}
		}
	}

	// ��ѯ��������
	for (i = 0; i < ALARM_MAX_COUNT; i ++){
		if ((palarm = LegalAlarm(i)) != NULL){
			if (palarm->Music == iRingID){
				*pUserType = DATA_ALARM;  // hqf 2-1
				*pUserID = i;
				return true;
			}
		}
	}

	return false;
}


/*************************************************************************/

/* �ٲ� */

/* ��λ�ٲ� */
PRIVATE FASTDIAL* LocateFastDial(short iFastDialID)
{
	return (FASTDIAL*)(FASTDIAL_BASE_ADDR + sizeof(FASTDIAL) * iFastDialID);
}

/* �����ٲ� */
PRIVATE void EraseOneFastDial(short iFastDialID)
{
	FASTDIAL		*fastdial;

	fastdial = LocateFastDial(iFastDialID);
	LoadFlashSector(fastdial);
	memset((void*)(FLASH_BUF_ADDR + (long)fastdial % FLASH_SECTOR_SIZE), 0xFF, sizeof(FASTDIAL));
	SaveFlashSector(fastdial);
}

/* д�ٲ� */
PRIVATE void WriteOneFastDial(short iFastDialID, FASTDIAL const *pFastDial, short iSort)
{
	FASTDIAL		fastdial;

	memcpy(&fastdial, pFastDial, sizeof(FASTDIAL));
	fastdial.Sort = iSort;
	fastdial.Flag = fastdial.Flag & (~FASTDIAL_ERASED) | FASTDIAL_EXIST;
	FlashWrite((UINT16*)LocateFastDial(iFastDialID), (UINT16*)(&fastdial), sizeof(FASTDIAL) / 2);
}

/* �ж��ٲ��Ϸ��� */
PRIVATE FASTDIAL* LegalFastDial(short iFastDialID)
{
	FASTDIAL*	pfastdial;

	if ((iFastDialID >= FASTDIAL_MAX_COUNT) || (iFastDialID < 0))
		return NULL;
	pfastdial = LocateFastDial(iFastDialID);
	if (pfastdial->Flag & FASTDIAL_ERASED)
		return NULL;
	if (!(pfastdial->Flag & FASTDIAL_EXIST))
		return NULL;
	return pfastdial;
}

/* �ٲ����ұȽϺ��� */
PRIVATE int FastDialCompare(void const *key, void const *element)
{
	FASTDIAL	*pkey, *pelement;

	pkey = LocateFastDial(*(short*)key);
	pelement = LocateFastDial(*(short*)element);
	return pkey->Sort - pelement->Sort;
}

/* ��ȡ�½��ٲ������� */
PRIVATE short FastDialNewIndex(void)
{
	short		buf[FASTDIAL_MAX_COUNT], id = -1;
	int			count, i;

	count = FindFastDial(buf, FASTDIAL_MAX_COUNT);
	for (i = 0; i < count; i ++){
		if (LocateFastDial(buf[i])->Sort > id)
			id = LocateFastDial(buf[i])->Sort;
	}
	return id + 1;
}

//////////////////////////////////////////////////
//
//  ��ʼ���ٲ�
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitFastDial(void)
{
//	for (long addr = FASTDIAL_BASE_ADDR; addr < FASTDIAL_BASE_ADDR + FASTDIAL_MAX_COUNT * sizeof(FASTDIAL); addr += FLASH_SECTOR_SIZE)
//		FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  �����ٲ�
//
//	������	pFastDial	�����ٲ�����
//
//  ���أ�	���� ID��ID >= 0, ���� -1 ��ʾʧ��
//
//////////////////////////////////////////////////
short AppendFastDial(FASTDIAL *pFastDial)
{
	FASTDIAL	*pfastdial;
	short		i, j = -1;

	for (i = 0; i < FASTDIAL_MAX_COUNT; i ++){
		pfastdial = LocateFastDial(i);
		if (pfastdial->Flag & FASTDIAL_ERASED)
			break;
		if (!(pfastdial->Flag & FASTDIAL_EXIST))
			j = i;
	}
	if ((i == FASTDIAL_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneFastDial(i);
	}
	if (i == FASTDIAL_MAX_COUNT)
		return -1;
	WriteOneFastDial(i, pFastDial, FastDialNewIndex());
	return i;
}

//////////////////////////////////////////////////
//
//  ɾ���ٲ�
//
//	������	iFastDialID		��ɾ���ٲ�ID
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteFastDial(short iFastDialID)
{
	FASTDIAL*	pfastdial;

	if (!(pfastdial = LegalFastDial(iFastDialID)))
		return false;
	return FlashWriteWord(&(pfastdial->Flag), pfastdial->Flag & ~FASTDIAL_EXIST);
}
	
//////////////////////////////////////////////////
//
//  �޸��ٲ�
//
//	������	iFastDialID		���޸��ٲ�ID
//			pFastDial		�޸�����
//
//  ���أ�	�޸ĳɹ����
//
//////////////////////////////////////////////////
bool ModifyFastDial(short iFastDialID, FASTDIAL const *pFastDial)
{
	FASTDIAL	*pfastdial;
	short		iprivate;
	
	if (!(pfastdial = LegalFastDial(iFastDialID)))
		return false;
	iprivate = pfastdial->Sort;
	EraseOneFastDial(iFastDialID);
	WriteOneFastDial(iFastDialID, pFastDial, iprivate);
	return true;
}
	
//////////////////////////////////////////////////
//
//  �����ٲ�
//
//	������	pFastDialIDBuf		���ҽ�� buffer
//			iBufLen				buffer ����
//
//  ���أ�	�鵽����
//
//////////////////////////////////////////////////
int FindFastDial(short *pFastDialIDBuf, int iBufLen)
{
	FASTDIAL*	pfastdial;
	int			count = 0;
	
	for (short i = 0; (i < FASTDIAL_MAX_COUNT) && (count < iBufLen); i ++){
		if ((pfastdial = LegalFastDial(i)) != NULL){
			*(pFastDialIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pFastDialIDBuf, count, 2, FastDialCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ�ٲ�ָ��
//
//	������	iFastDialID		�ٲ� ID
//
//  ���أ�	�ٲ�ָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
FASTDIAL* GetFastDialPointer(short iFastDialID)
{
	return LegalFastDial(iFastDialID);
}	

//////////////////////////////////////////////////
//
//  �ٲ�����
//
//	������	pFastDialIDBuf		�������ٲ� ID buffer
//			iCount				���� ID ����
//
//  ���أ�	����ɹ����
//
//////////////////////////////////////////////////
bool SortFastDial(short *pFastDialIDBuf, int iCount)
{
	FASTDIAL*	pfastdial;

	memcpy((void*)FLASH_BUF_ADDR, (void*)FASTDIAL_BASE_ADDR, FLASH_SECTOR_SIZE);
	for (int i = 0; i < iCount; i ++){
		pfastdial = LegalFastDial(*(pFastDialIDBuf + i));
		if (pfastdial)
			((FASTDIAL*)((long)pfastdial - FASTDIAL_BASE_ADDR + FLASH_BUF_ADDR))->Sort = i;
		else
			return false;
	}
	FlashEraseSector((UINT16*)FASTDIAL_BASE_ADDR);
	SaveFlashSector((void*)FASTDIAL_BASE_ADDR);
	return true;
}


/*************************************************************************/

/* ʹ�ÿռ� */

//////////////////////////////////////////////////
//
//  ��ȡ¼����ʹ�ÿռ�
//
//	������	��
//
//  ���أ�	¼����ʹ�ÿռ䣬��λ���ֽ�
//			¼���ռ����� WAVE_STORE_LEN
//
//////////////////////////////////////////////////
long GetWaveUsed(void)
{
	return ((long)(*pWaveTail) - (long)pWaveHead);
}

//////////////////////////////////////////////////
//
//  ��ȡ����ͨ����¼����
//
//	������	��
//
//  ���أ�	����ͨ����¼����
//
//////////////////////////////////////////////////
int GetHisrotyUsed(void)
{
	short	buf[HISTORY_MAX_COUNT];
	return FindHistory(HISTORY_ALL, buf, HISTORY_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  ��ȡ���м�������
//
//	������	��
//
//  ���أ�	���м�������
//
//////////////////////////////////////////////////
int GetMemoUsed(void)
{
	short	buf[MEMO_MAX_COUNT];
	return FindMemo(buf, MEMO_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  ��ȡ���л�������
//
//	������	��
//
//  ���أ�	���л�������
//
//////////////////////////////////////////////////
int GetPictureUsed(void)
{
	return PICTURE_MAX_COUNT;
}

//////////////////////////////////////////////////
//
//  ��ȡ������Ƭ����
//
//	������	��
//
//  ���أ�	������Ƭ����
//
//////////////////////////////////////////////////
int GetCardUsed(void)
{
	int			count = 0;
	
	for (short i = 0; i < CARD_MAX_COUNT; i ++){
		if (LegalCard(i) != NULL)
				count ++;
	}
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ������Ƭ������
//
//	������	��
//
//  ���أ�	������Ƭ������
//
//////////////////////////////////////////////////
int GetCardCaseUsed(void)
{
	short	buf[CARDCASE_MAX_COUNT];
	return FindCardCase(buf, CARDCASE_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  ��ȡ������������
//
//	������	��
//
//  ���أ�	������������
//
//////////////////////////////////////////////////
int GetAlarmUsed(void)
{
	short	buf[ALARM_MAX_COUNT];
	return FindAlarm(buf, ALARM_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  ��ȡ������������
//
//	������	��
//
//  ���أ�	������������
//
//////////////////////////////////////////////////
int GetRingUsed(void)
{
	short	buf[RING_MAX_COUNT];
	return FindRing(buf, RING_MAX_COUNT);
}

//////////////////////////////////////////////////
//
//  ��ȡ������������
//
//	������	��
//
//  ���أ�	������������
//
//////////////////////////////////////////////////
int GetSmsUsed(void)
{
	short	buf[SMS_MAX_COUNT];
	return FindSMS(buf, SMS_MAX_COUNT);
}


//ϵͳ���õ����ݽӿڶ��壨��ʱ��
void InitSystemOption()
{
	CSystemSetup::ResetSystemOption();
}

/*************************************************************************/

/* ϵͳ�������� */

// temp, debug
//extern void tempecho(int num);

void LoadSystemOption(SYSTEM_OPTION *option)
{
	memcpy(option, (void*)SETTINGS_BASE_ADDR, sizeof(SYSTEM_OPTION));
}

void SaveSystemOption(SYSTEM_OPTION *option)
{
	int		temp;

	temp = sizeof(SYSTEM_OPTION);
	LoadFlashSector((void*)SETTINGS_BASE_ADDR);
	memcpy((void*)FLASH_BUF_ADDR, option, sizeof(SYSTEM_OPTION));
	FlashEraseSector((UINT16*)SETTINGS_BASE_ADDR);
	FlashWrite((UINT16*)(SETTINGS_BASE_ADDR), (UINT16*)FLASH_BUF_ADDR, FLASH_SECTOR_SIZE / 2);
}

/*
void InitPhoneOption()
{
	extern PHONE_OPTION g_PhoneOption;
	memset(&g_PhoneOption , 0 , sizeof(PHONE_OPTION));
	g_PhoneOption.nRecordRingCount = 3; //Ĭ��
	for(INT16 i =0; i< DIALRULE_COUNT; i++)
	{
		strcpy(g_PhoneOption.iiDialNum[i].strPostfix0 , ",1,");
		strcpy(g_PhoneOption.iiDialNum[i].strPostfix1 , "#,");
		strcpy(g_PhoneOption.iiDialNum[i].strPostfix2 , "#,");
		strcpy(g_PhoneOption.iiDialNum[i].strPostfix3 , "#");
	}
	g_PhoneOption.nDefaultIP = 0;
}
*/
/*
void GetSystemOption(SYSTEM_OPTION *sys_option)
{
	memset(sys_option , 0 ,sizeof(SYSTEM_OPTION));
	strcpy(sys_option->Version , "hanwang .tec.");
}
BOOL SetSystemOption(SYSTEM_OPTION *sys_option)
{
	return FALSE;
}

void GetPhoneOption(PHONE_OPTION *pho_option)
{
	memset(pho_option , 0 ,sizeof(PHONE_OPTION));
}

BOOL SetPhoneOption(PHONE_OPTION *pho_option)
{
	return FALSE;
}

void GetFastDial(FASTDIAL *fd_option)
{
	memset(fd_option , 0 , sizeof(FASTDIAL));
}

BOOL SetFastDial(FASTDIAL *fd_option)
{
	return FALSE;
}
*/

/*************************************************************************/
/* ����Ϣ���� */

/* ��λ����Ϣ */
PRIVATE SMS* LocateSms(short iSmsID)
{
	return (SMS*)(SMS_BASE_ADDR + sizeof(SMS) * iSmsID);
}

/* ����һ������Ϣ */
PRIVATE void EraseOneSms(short iSmsID)
{
	SMS		*sms;

	sms = LocateSms(iSmsID);
	LoadFlashSector(sms);
	memset((void*)(FLASH_BUF_ADDR + (long)sms % FLASH_SECTOR_SIZE), 0xFF, sizeof(SMS));
	SaveFlashSector(sms);
}

/* дһ������Ϣ�� Flash */
PRIVATE void WriteOneSms(short iSmsID, SMS const *pSms)
{
	SMS		sms;

	memcpy(&sms, pSms, sizeof(SMS));
	sms.Flag = sms.Flag & (~SMS_ERASED) | SMS_EXIST;
	FlashWrite((UINT16*)LocateSms(iSmsID), (UINT16*)(&sms), sizeof(SMS) / 2);
}

/* �ж϶���Ϣ�Ϸ� */
PRIVATE SMS* LegalSms(short iSmsID)
{
	SMS*	psms;

	if ((iSmsID >= SMS_MAX_COUNT) || (iSmsID < 0))
		return NULL;
	psms = LocateSms(iSmsID);
	if (psms->Flag & SMS_ERASED)
		return NULL;
	if (!(psms->Flag & SMS_EXIST))
		return NULL;
	return psms;
}

/* ���Ҷ���Ϣ�ȽϺ��� */
PRIVATE int SmsCompare(void const *key, void const *element)
{
	SMS	*pkey, *pelement;

	pkey = LocateSms(*(short*)key);
	pelement = LocateSms(*(short*)element);
	// TODO: sort by date
	return strcmp(pkey->szTime, pelement->szTime);
}

//////////////////////////////////////////////////
//
//  ��ʼ������Ϣ
//
//	������	��
//
//  ���أ�	��
//
//////////////////////////////////////////////////
void InitSMS()
{
//	for (long addr = SMS_BASE_ADDR;
//			addr < SMS_BASE_ADDR + SMS_MAX_COUNT * sizeof(SMS); 
//						addr += FLASH_SECTOR_SIZE)
//		FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//
//  ��������Ϣ
//
//	������	pSms		��������Ϣ����
//
//  ���أ�	��������Ϣ ID��ID >= 0, ���� -1 ��ʾʧ��
//
//////////////////////////////////////////////////
short AppendSMS(SMS *pSms)
{
	SMS*		psms;
	short		i, j = -1;

	for (i = 0; i < SMS_MAX_COUNT; i ++){
		psms = LocateSms(i);
		if (psms->Flag & SMS_ERASED)
			break;
		if (!(psms->Flag & SMS_EXIST))
			j = i;
	}
	if ((i == SMS_MAX_COUNT) && (j != -1)){
		i = j;
		EraseOneSms(i);
	}
	if (i == SMS_MAX_COUNT)
		return -1;
	WriteOneSms(i, pSms);
	return i;
}

//////////////////////////////////////////////////
//
//  ɾ������Ϣ
//
//	������	iSmsID		ɾ������Ϣ�� ID
//
//  ���أ�	ɾ���ɹ����
//
//////////////////////////////////////////////////
bool DeleteSMS(short iSmsID)
{
	SMS*	psms;

	if (!(psms = LegalSms(iSmsID)))
		return false;
	return FlashWriteWord(&(psms->Flag), psms->Flag & ~SMS_EXIST);
}

//////////////////////////////////////////////////
//
//  �޸Ķ���Ϣ
//
//	������	iSmsID		�޸Ķ���Ϣ�� ID
//			pSms		�޸�����
//
//  ���أ�	�޸ĳɹ����
//
//////////////////////////////////////////////////
bool ModifySMS(short iSmsID, SMS const *pSms)
{
	SMS*	psms;
	
	if (!(psms = LegalSms(iSmsID)))
		return false;
	EraseOneSms(iSmsID);
	WriteOneSms(iSmsID, pSms);
	return true;
}
	
//////////////////////////////////////////////////
//
//  ���Ҷ���Ϣ
//
//	������	iSmsCaseID		����Ϣ�� ID
//			pSmsIDBuf		��Ų��ҽ���� buffer
//			iBufLen			buffer ����
//
//  ���أ�	�鵽�ĸ���
//
//////////////////////////////////////////////////
int FindSMS(short *pSmsIDBuf, int iBufLen)
{
	SMS*		psms;
	int			count = 0;
	
	for (short i = 0; (i < SMS_MAX_COUNT) && (count < iBufLen); i ++){
		if ((psms = LegalSms(i)) != NULL){
			*(pSmsIDBuf + count) = i;
			count ++;
		}
	}
	//qsort(pSmsIDBuf, count, 2, SmsCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ����Ϣָ��
//
//	������	iSmsID		����Ϣ�� ID
//
//  ���أ�	����Ϣָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
SMS* GetSMSPointer(short iSmsID)
{
	return LegalSms(iSmsID);
}

//************************  ����������  wangyu***********************

// ��λ������  
PRIVATE MODEL* LocateModel(short iModelID)
{
	return (MODEL*)(MODEL_BASE_ADDR + sizeof(MODEL) * iModelID);
} 

// ����������  
PRIVATE void EraseOneModel(short iModelID)
{ 
    MODEL         *model;

	model = LocateModel(iModelID);
	LoadFlashSector(model);
	memset((void*)(FLASH_BUF_ADDR + (long)model % FLASH_SECTOR_SIZE), 0xFF, sizeof(MODEL));
	SaveFlashSector(model);
}

//д������  
PRIVATE void WriteOneModel(short iModelID, MODEL const *pModel, short iModelItem)
{
    MODEL          model;
	memcpy(&model, pModel, sizeof(MODEL));

	model.iModelItem = iModelItem; //��������

	model.Flag = model.Flag & (~MODEL_ERASED) | MODEL_EXIST;

	FlashWrite((UINT16*)LocateModel(iModelID),  (UINT16*)(&model),  sizeof(MODEL) / 2);
}

//�жϼ�����Ϸ��� //
PRIVATE MODEL* LegalModel(short iModelID)
{
	MODEL*	pModel;

	if ((iModelID >= MAX_MODEL_NUMBER) || (iModelID < 0))
		return NULL;
	pModel = LocateModel(iModelID);
	if (pModel->Flag & MODEL_ERASED)
		return NULL;
	if (!(pModel->Flag & MODEL_EXIST))
		return NULL;
	return pModel;
}

//��������ұȽϺ��� ///
PRIVATE int ModelCompare(void const *key, void const *element)
{
	MODEL	*pkey, *pelement;

	pkey = LocateModel(*(short*)key);
	pelement = LocateModel(*(short*)element);
	return pkey->iModelItem - pelement->iModelItem;
}

//��ȡ�½�������������  
PRIVATE short ModelNewIndex(void)
{
	short		buf[MAX_MODEL_NUMBER], id = -1;
	int			count, i;
//////   NONONONONONO
	count = FindMODEL(buf, MAX_MODEL_NUMBER);
	for (i = 0; i < count; i ++){
		if (LocateModel(buf[i])->iModelItem > id)
			id = LocateModel(buf[i])->iModelItem;
	}
	return id + 1;
}

////////////////////////////////////////////////////
//  ��ʼ��������//    wangyu
//	������	��//
//  ���أ�	��//
////////////////////////////////////////////////// 
void   InitModel()
{
//	for (long addr =  MODEL_BASE_ADDR;
//			  addr <  MODEL_BASE_ADDR + MAX_MODEL_NUMBER * sizeof(MODEL); 
//			  addr += FLASH_SECTOR_SIZE )
//		FlashEraseSector((UINT16*)addr);
}

////////////////////////////////////////////////////
//  ����������//
//	������	pMODEL	��������������//
//  ���أ�	���� ID��ID >= 0, ���� -1 ��ʾʧ��//
////////////////////////////////////////////////// 
short   AppendMODEL(MODEL *pMODEL) 	// ���� ID(ID>=0)������ -1 ��ʾʧ��
{     
	 MODEL *pmodel;
	short		i, j = -1;
	for (i = 0; i < MAX_MODEL_NUMBER; i ++){
		pmodel = LocateModel(i);
		if (pmodel->Flag & MODEL_ERASED)
			break;
		if (!(pmodel->Flag & MODEL_EXIST))
			j = i;
	}
	if ((i == MAX_MODEL_NUMBER) && (j != -1)){
		i = j;
		EraseOneModel(i);
	}
	if (i == MAX_MODEL_NUMBER)
		return -1;
    WriteOneModel(i, pMODEL, ModelNewIndex());
	return i;
}
  

////////////////////////////////////////////////////
//  ɾ��������//
//	������	iModelID		��ɾ��������ID//
//  ���أ�	ɾ���ɹ����//
//////////////////////////////////////////////////
 bool    DeleteMODEL(short iModelID) // �ɹ�����true, ʧ�ܷ���false 
{
     MODEL*	pMODEL;
	if (!(pMODEL = LegalModel(iModelID)))
		return false;
	return FlashWriteWord(&(pMODEL->Flag), pMODEL->Flag & ~MODEL_EXIST);
}

////////////////////////////////////////////////////
//  �޸ļ�����//
//	������	iModelID		���޸ļ�����ID
//			pModel   		�޸�����//
//  ���أ�	�޸ĳɹ����//
////////////////////////////////////////////////// 
  bool    ModifyModel(short iModelID, MODEL const *pModel)
{
	 
	MODEL       *pmodel;
	short		iModelItem;
	
	if (!(pmodel = LegalModel(iModelID)))
		return false;
	iModelItem = pmodel->iModelItem;
	EraseOneModel(iModelID);
	WriteOneModel(iModelID, pModel, iModelItem);
	return true;
}
	
//////////////////////////////////////////////////
//  ���Ҽ�����//
//	������	pModelIDBuf		���ҽ�� buffer
//			iBufLen				buffer ���� 
//  ���أ�	�鵽����//
//////////////////////////////////////////////////       
  int  FindMODEL(short *pModelIDBuf, int iBufLen)// ���ز鵽����
{
 
	MODEL*      model;
	int			count = 0;
	
	for (short i = 0; (i < MAX_MODEL_NUMBER) && (count < iBufLen); i ++){
		if ((model = LegalModel(i)) != NULL){
			*(pModelIDBuf + count) = i;
			count ++;
		}
	}
	qsort(pModelIDBuf, count, 2, ModelCompare);
	return count;
}

//////////////////////////////////////////////////
//
//  ��ȡ������ָ��
//
//	������	iModelID		������ ID
//
//  ���أ�	 ���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
MODEL* GetModelPointer(short iModelID)
{
	return LegalModel(iModelID);
}	
 //////////////////////////////////////////////////
//
//  ����������
//
//	������	pModelIDBuf		�����򼯽��� ID buffer
//			iCount				���� ID ����
//
//  ���أ�	����ɹ����
//
//////////////////////////////////////////////////
bool SortModel(short *pModelIDBuf, int iCount)
{
	MODEL*	pmodel;

	memcpy((void*)FLASH_BUF_ADDR, (void*)MODEL_BASE_ADDR, FLASH_SECTOR_SIZE);
	for (int i = 0; i < iCount; i ++){
		pmodel = LegalModel(*(pModelIDBuf + i));
		if (pmodel)
			((MODEL*)((long)pmodel - MODEL_BASE_ADDR + FLASH_BUF_ADDR))->iModelItem = i;
		else
			return false;
	}
	FlashEraseSector((UINT16*)MODEL_BASE_ADDR);
	SaveFlashSector((void*)MODEL_BASE_ADDR);

	// temp, debug
	short	idbuf[20];
	int		count;
	count = FindMODEL(idbuf, 20);
	// debug end

	return true;
}
//***********************  ����������  wangyu********************************//

// ������ ��λ  
PRIVATE CONTENT* LocateContent(short iContentID)
{
	return (CONTENT*)(CONTENT_BASE_ADDR + sizeof(CONTENT) * iContentID);
}

//����һ�������� 
PRIVATE void EraseOneContent(short iContentID)
{
	CONTENT		*content;

	content = LocateContent(iContentID);
	LoadFlashSector(content);
	memset((void*)(FLASH_BUF_ADDR + (long)content % FLASH_SECTOR_SIZE), 0xFF, sizeof(CONTENT));
	SaveFlashSector(content);
}
// дһ��������� Flash  
PRIVATE void WriteOneContent(short iContentID, CONTENT const *pContent)
{	 
	CONTENT		content;

	memcpy(&content, pContent, sizeof(CONTENT));
	content.Flag = content.Flag & (~CONTENT_ERASED) | CONTENT_EXIST;
	FlashWrite((UINT16*)LocateContent(iContentID), (UINT16*)(&content), sizeof(CONTENT) / 2);
}
 
//�жϼ�����Ϸ�  
PRIVATE CONTENT* LegalContent(short iContentID)
{
	CONTENT		* pcontent;
	
	if ((iContentID >= MAX_CONTENT_NUMBER) || (iContentID < 0))
		return NULL;
	pcontent = LocateContent(iContentID);
	if (pcontent->Flag & CONTENT_ERASED)
		return NULL;
	if (!(pcontent->Flag & CONTENT_EXIST))
		return NULL;
	return pcontent;
}

 //���Ҽ�����ȽϺ��� 
PRIVATE int ContentCompare(void const *key, void const *element)
{
	CONTENT	*pkey, *pelement;

	pkey = LocateContent(*(short*)key);
	pelement = LocateContent(*(short*)element);
	return strcmp(pkey->szContent, pelement->szContent);//�Ƚ�����
}
 
////////////////////////////////////////////////////
//  ��ʼ��������//
//	������	��//
//  ���أ�	��//
//////////////////////////////////////////////////
void    InitContent()		// ��ʽ��
{
//   for (long addr = CONTENT_BASE_ADDR; 
//             addr < CONTENT_BASE_ADDR + MAX_CONTENT_NUMBER * sizeof(CONTENT); 
//			 addr += FLASH_SECTOR_SIZE )
//	FlashEraseSector((UINT16*)addr);
}

//////////////////////////////////////////////////
//  ����������
//	������	pContent		��������������
//  ���أ�	���������� ID��ID >= 0, ���� -1 ��ʾʧ��
//////////////////////////////////////////////////
short   AppendContent( CONTENT *pContent)	                       
{
 	CONTENT*    pcontent;
	short		i, j = -1;

	for (i = 0; i < MAX_CONTENT_NUMBER; i ++){
		pcontent = LocateContent(i);
		if (pcontent->Flag & CONTENT_ERASED)
			break;
		if (!(pcontent->Flag & CONTENT_EXIST))
			j = i;
	}
	if ((i == MAX_CONTENT_NUMBER) && (j != -1)){
		i = j;
		EraseOneContent(i);
	}
	if (i == MAX_CONTENT_NUMBER)
		return -1;
	WriteOneContent(i, pContent);
	return i;
}

////////////////////////////////////////////////////
//  ɾ�������� 
//	������	iContentID		ɾ��������� ID
//  ���أ�	ɾ���ɹ����
//////////////////////////////////////////////////
bool    DeleteContent( short iContentID)
{
    CONTENT*    pcontent;
	
	if (!(pcontent = LegalContent(iContentID)))
		return false;
	return FlashWriteWord(&(pcontent->Flag), pcontent->Flag & ~CONTENT_EXIST);
}

//////////////////////////////////////////////////
//  �޸ļ�����
//	������	iContentID		�޸ļ������ ID
//   		pContent		�޸�����
//  ���أ�	�޸ĳɹ����
//////////////////////////////////////////////////
bool    ModifyContent( short iContentID, CONTENT const *pContent)
{
	CONTENT*    pcontent;
	if (!(pcontent = LegalContent(iContentID)))
		return false;
	EraseOneContent(iContentID);
	WriteOneContent(iContentID, pContent);
	return true;
}
	
//////////////////////////////////////////////////
//  ����ĳ�����еļ�����
//	������	iModelID		������ ID
//  		pContentIDBuf		��Ų��ҽ���� buffer
//			iBufLen			buffer ����
//  ���أ�	�鵽�ĸ���
//////////////////////////////////////////////////
int     FindContent (short iModelID,short *pContentIDBuf, int iBufLen) 
{
 	CONTENT*    pcontent;
	int			count = 0;
	
	for (short i = 0; (i < MAX_CONTENT_NUMBER) && (count < iBufLen); i ++){
		if ((pcontent = LegalContent(i)) != NULL){
			if (pcontent->iModelID == iModelID){   ///������
				*(pContentIDBuf + count) = i;
				count ++;
			}
		}
	}
//	qsort(pCardIDBuf, count, 2, CardCompare);
	return count;
}
////////////////////
  ////  �������еĵļ�����
  ////////
int     FindAllContent (short *pContentIDBuf, int iBufLen)
{
	CONTENT*    pcontent;
	int			count = 0;
	
	for (short i = 0; (i < MAX_CONTENT_NUMBER) && (count < iBufLen); i ++){
		if ((pcontent = LegalContent(i)) != NULL){
				*(pContentIDBuf + count) = i;
				count ++;	 
		}
	} 
	return count;
}
 
//////////////////////////////////////////////////
//
//  ��ȡ������ָ��
//
//	������	iContentID		������� ID
//
//  ���أ�	��Ƭָ�룬���� NULL ��ʾʧ��
//
//////////////////////////////////////////////////
//CARD* GetCardPointer(short iCardID)
  CONTENT* GetContentPointer(short iContentID)
{
	return LegalContent(iContentID);
}


/*********Ⱥ������********************************************************/
void InitGroup()
{
//  for (long addr = GROUP_BASE_ADDR; 
//             addr < GROUP_BASE_ADDR + MAX_GROUP_NUMBER * sizeof(GROUP); 
//			 addr += FLASH_SECTOR_SIZE )
//	FlashEraseSector((UINT16*)addr);
}
  
void LoadGroupSend(GROUP *option)
{
	int		temp;

	//temp = sizeof(GROUP);
	temp = sizeof(GROUP) * MAX_GROUP_NUMBER;
	memcpy(option, (void*)GROUP_BASE_ADDR, temp);
}

void SaveGroupSend(GROUP *option)
{
	int		temp;

	//temp = sizeof(GROUP);
	temp = sizeof(GROUP) * MAX_GROUP_NUMBER;
	LoadFlashSector((void*)GROUP_BASE_ADDR);
	memcpy((void*)FLASH_BUF_ADDR, option, temp);
	FlashEraseSector((UINT16*)GROUP_BASE_ADDR);
	FlashWrite((UINT16*)(GROUP_BASE_ADDR), (UINT16*)FLASH_BUF_ADDR, FLASH_SECTOR_SIZE / 2);
}
/*****************************Ⱥ������****************************************/