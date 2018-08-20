#include "stdafx.h"
#include "TelephoneWarp.h"
#include "../multimediaphone.h"
#include "../multimediaphoneDlg.h"
#include "../resource.h"
#include "waveHeader.h"

#define HUNGON_VALUE	0x82		//0x82	 摘机
#define HUNGOFF_VALUE	0x83		//0x83   挂机
#define RING_VALUE		0x84		//0x84   振铃信号
//#define WM_TEL_CALLIDEND            8023

#define DEFUALT_SOUND_VALUME   -1500

using namespace Telephone;

extern VOID WriteMyLog_(char *ptr, int size);

typedef enum
{   
	stBeginRest,                // 开始休息/延时    
	stContinueRest,             // 继续休息/延时    
	stSendPhoneRequest,         // 拨打电话   
	stSendPhoneResponse,        // 读取拨打应答到缓冲区    
	//stSendPhoneWaitIdle,      // 拨打不成功，等待GSM就绪    
	stReadPhoneRequest,         // 发送读取来电列表的命令    
	stReadPhoneResponse,        // 读取来电列表到缓冲区    
	//stDeletePhoneRequest,     // 删除记录    
	//stDeletePhoneResponse,    // 删除记录应答    
	//stDeletePhoneWaitIdle,    // 删除不成功，等待GSM就绪  
	stRedial,
	stAnswer,
	stHangupPhone,			//挂机
	stExitThread                // 退出    
}TelephoneState;               // 处理过程的状态 

static unsigned char g_tel_code[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C', 'D'};
const char *const DTMFNUMBER = "D1234567890*#ABC";

char *gDetectString  = "1*0#*0#*0#*0#1";
char *gDetectString1 = "*#1579#";
char *gSetUserIDString = "*#357#"; //"3*0#*0#*0#*0#3";
static char gTelVersion1_[128];
//int gBatteryLevel[] = {830, 805, 780, 760, 740, 720};
int gBatteryLevel[] = {800, 785, 770, 755, 740};
int gBatteryLevel_org[] = {800, 785, 770, 755, 740};
int gBatteryOffset = 115;
int gBatteryBase = 740;
int g_BatteryLevel = 840;
int TelephoneWarp::DetectTestStatus(unsigned char c)
{
	int isBatteryStatus = 0;
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	if(c == 'A')
	{
		main->phone_->m_bInputtingUserID = TRUE;
		memset(main->phone_->m_chDetectCode, 0, 64);
		main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)] = c;
	}
	else if(main->phone_->m_bInputtingUserID)
	{
		if(strlen(main->phone_->m_chDetectCode) < 63)
			main->phone_->m_chDetectCode[strlen(main->phone_->m_chDetectCode)] = c;
	}
	if(c == '\x0d')
	{	
		int n ;
		main->phone_->m_bInputtingUserID = FALSE;
		char *ptr;
		static int batterySize = 0;
	//	Dprintf("battrySize : %s\r\n",main->phone_->m_chDetectCode);
		char log[100];
		memset(log,0,100);
		sprintf(log,"batterySize:%s",main->phone_->m_chDetectCode);
		extern VOID WriteMyLog_(char *ptr, int size);
		WriteMyLog_(log,strlen(log));

		if ( strstr(main->phone_->m_chDetectCode, "SRC=1") > 0)  //有电源
		{
			main->phone_->m_BatteryStatus.isBattery_DC = TRUE;
		}
		else if ( strstr(main->phone_->m_chDetectCode, "SRC=0") > 0)
		{
			main->phone_->m_BatteryStatus.isBattery_DC = FALSE;
		}

		if((ptr = strstr(main->phone_->m_chDetectCode, "AT=")) > 0)
		{
			if ( strstr(main->phone_->m_chDetectCode, "SRC=1") > 0 && 
			( strstr(main->phone_->m_chDetectCode, "CHG=0") > 0))
			{	
// 				bool bInvalidate = false; //add by qi 20100408
// 				if (main->m_binitOver)
// 				{
// 					bInvalidate = true ;	
// 				}

				main->KillTimer(0x120);
				main->m_MJPGList.SetUnitBitmap(11, L".\\adv\\mjpg\\k5\\common\\png\\无电池.bmp", L"",false);
				main->m_isHaveBattery = FALSE;
			}
			else
			{
				main->m_isHaveBattery = TRUE;
			}
			char *p = strstr(ptr+3, ";");
			if(!p)
			{
				p = strstr(ptr+3, "'");
			}
			if(p)
			{
				char txt[4] = {0};
				memcpy(txt, ptr+3, p-ptr-3);
				CString s = txt;
				n = Util::StringOp::ToInt(txt, 10);
				g_BatteryLevel = n;
				if(g_BatteryLevel < gBatteryLevel[4])
				{
					main->PostMessage(WM_BATLOW, 0, 0);
				}
				extern VOID WriteLog(char *ptr);
				WriteLog(txt);
				isBatteryStatus = 1;
				if(strstr(ptr, "CHG=2"))
				{
					if(batterySize == 2)
					{
						for(int i = 0; i < 5; i++)
						{
							gBatteryLevel[i] = (int)((float)gBatteryLevel_org[i] / (840.0 / (float)n));
						}
						main->phone_->m_BatteryStatus.batteryProccess = 5;
					}
					else
					{
						batterySize = 2;
						main->phone_->m_BatteryStatus.isCharge = 0;
						isBatteryStatus = 2;

						FILE *file = fopen("\\flashdrv\\res_dat\\config.txt", "w+b");
						for(int i = 0; i < 5; i++)
						{
							gBatteryLevel[i] = (int)((float)gBatteryLevel_org[i] / (840.0 / (float)n));
							if(file != NULL)
							{
								char str[32] = {0};
								sprintf(str, "BatteryLevel%d = %d\n", i+1, gBatteryLevel[i]);
								fwrite(str, sizeof(char), strlen(str), file);
							}
						}

						fclose(file);
						main->phone_->m_BatteryStatus.batteryProccess = 5;
					}

					if ( strstr(main->phone_->m_chDetectCode, "SRC=1") > 0)  //有电源
					{
						main->phone_->m_BatteryStatus.batteryType = BATTERY_DC;
					}
					else if ( strstr(main->phone_->m_chDetectCode, "SRC=0") > 0)
					{
						main->phone_->m_BatteryStatus.batteryType = BATTERY_1;
					}
					return isBatteryStatus;
				}
				else    //if(strstr(ptr, "CHG=1"))
				{
					batterySize = 1;
					for(int i = 0; i < 5; i++)
					{
						if(n >= gBatteryLevel[i])
						{
							break;
						}
					}
					main->phone_->m_BatteryStatus.batteryProccess = (5-i);
				}
			}
		}

		if((ptr = strstr(main->phone_->m_chDetectCode, "CHG=")) > 0)
		{
			char *p = strstr(ptr+4, ";");
			if(!p)
			{
				p = strstr(ptr+4, "'");
			}
			if(p)
			{
				isBatteryStatus = 1;
				char txt[4] = {0};
				memcpy(txt, ptr+4, p-ptr-4);
				if(strstr(txt,"1"))
				{
					main->phone_->m_BatteryStatus.isCharge = 1;
					//main->phone_->m_BatteryStatus.batteryType = BATTERY_DC;
				}
				else if(strstr(txt,"0"))
				{
					main->phone_->m_BatteryStatus.isCharge = 0;
				}
			}
		}

		if((ptr = strstr(main->phone_->m_chDetectCode, "SRC=")) > 0)
		{
			char *p = strstr(ptr+4, ";");
			if(!p)
			{
				p = strstr(ptr+4, "'");
			}
			if(p)
			{
				char txt[4] = {0};
				memcpy(txt, ptr+4, p-ptr-4);
				if(strstr(txt,"1") && (main->phone_->m_BatteryStatus.batteryType != BATTERY_DC))
				{
					main->phone_->m_BatteryStatus.typeChange = -1;
					main->phone_->m_BatteryStatus.batteryType = BATTERY_DC;
					main->phone_->m_BatteryStatus.typeChange = 0;
					batterySize = 1;
					isBatteryStatus = 3;     
				}
				else if(strstr(txt,"0") && (main->phone_->m_BatteryStatus.batteryType != BATTERY_1))
				{
					main->phone_->m_BatteryStatus.typeChange = -1;
					main->phone_->m_BatteryStatus.batteryType = BATTERY_1;
					main->phone_->m_BatteryStatus.typeChange = 1;
					batterySize = 0;
					isBatteryStatus = 3; 
				}
			}
		}
		if((ptr = strstr(main->phone_->m_chDetectCode, "PWR=0")) > 0)//通知关机
		{
			isBatteryStatus = 4;
		}
	}
	return isBatteryStatus;  //123
}

bool DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
{
	bool flag = false;

	if (*buf >= DTMF_CHANNEL_STRAT && *buf <= DTMF_CHANNEL_END)
	{
		flag = DecodeDTMFCallIDPackage(buf, pcallid);
	}

	else if (*buf >= FSK_CHANNEL_STRAT && *buf <= FSK_CHANNEL_END)
	{
		flag = DecodeFSKCallIDPackage(buf, pcallid);
	}

	if (flag)
	{
		// 过滤无效号码字符
		int		i, j, k;
		i = strlen((char*)(pcallid->number));

		for (j = 0, k = 0; k < i; k ++)
		{
			if ((pcallid->number[k] >= '0') && (pcallid->number[k] <= '9'))
			{
				pcallid->number[j] = pcallid->number[k];
				j ++;
			}
		}
		pcallid->number[j] = '\0';

		if (j == 0)
		{
	          	//pcallid->numberstatus = CALLID_NONE;
			flag = false;
		}
	}

	else
	{
		pcallid->number[0] = '\0';
                pcallid->numberstatus = CALLID_ERROR;   //fjm
	}
        //Dprintf("Before return ,pcallid->numberstatus is %d\n", pcallid->numberstatus);
	return flag;

}

//////////////////////////////////////////
//
//	解 DTMF 来电包
//
bool DecodeDTMFCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
{
	int		i;

	for (i = 0; (*(buf + i) != TEL_DTMFEND) && (i < CALLID_NUM_LEN - 1); i ++)
	{
		pcallid->number[i] = DTMFNUMBER[*(buf + i) - DTMF_CHANNEL_STRAT];
	}
	pcallid->number[i] = '\0';
	pcallid->numberstatus = CALLID_OK;
	pcallid->type = CALLID_TYPE_DTMF;
	return true;
}


////////////////////////////////
//
//	解 FSK 来电显示包
//
bool DecodeFSKCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
{
	int		pklen, datalen, i;
	BYTE	*pparm;		// pparm: pointer_parameter
	BYTE	*pd;		// pd:    pointer_data
	/*
	*	Assemble package
	*/
	for (pklen = 0; *(buf + pklen) != TEL_FSKEND; pklen ++){
		if (pklen % 2 == 0)
			*(buf + pklen / 2) = (*(buf + pklen) << 4) & 0xF0;
		else
			*(buf + pklen / 2) |= (*(buf + pklen) & 0x0F);
	}

	/*
	*	Package frame error
	*/
	//if (pklen % 2 != 0)
	//	return false;

	/*
	*	Locate pakage start flag error
	*/
	pklen /= 2;
	for (i = 0; i < pklen; i ++)
	{
		if ((*buf == CALLID_TYPE_FSK_SIMPLE)
		   ||(*buf == CALLID_TYPE_FSK_COMPLEX))
			break;
		else
			buf ++;
	}
	if (i == pklen)
		return false;
	else
		pklen -= i;

	/*
	*	Package check sum error
	*/
	BYTE	sum = 0;
	for (i = 0; i < pklen - 1; i ++)
		sum += *(buf + i);
	//if ((BYTE)(sum + *(buf + pklen - 1)) != 0)
	//	return false;

	/*
	*	Package length error
	*/
	datalen = *(buf + 1);
	//if (datalen + 3 != pklen)
	//	return false;

	/*
	*	Decode simple format
	*/
	if (*buf == CALLID_TYPE_FSK_SIMPLE){
		pd = buf + 2;
		DecodeFSKSimpleParam(pd, datalen, pcallid);
		pcallid->type = CALLID_TYPE_FSK_SIMPLE;
	}

	/*
	*	Decode complex format
	*/
	else if (*buf == CALLID_TYPE_FSK_COMPLEX){
		pparm = buf + 2;
		while (pparm < buf + 2 + datalen){
			DecodeFSKComplexParam(pparm, pcallid);		// Decode every parameter
			pparm += *(pparm + 1) + 2;
		}
		pcallid->type = CALLID_TYPE_FSK_COMPLEX;
	}
        //Dprintf("Before return to main ,pcallid->numberstatus is %d\n", pcallid->numberstatus);
	return true;
}

////////////////////////////////
//
//	解 FSK 来电简单格式参数包
//
void DecodeFSKSimpleParam(BYTE *pd, int datalen, CALLID_INFO *pcallid)
{
	/*
	*	Data length error
	*/
	//if (datalen < 8 + 1)
	//	return;

	/*
	*	Decode time
	*/
	pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
	pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
	pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
	pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
	pcallid->timestatus = CALLID_OK;

	/*
	*	Decode number
	*/
	BYTE	*pnum = pd + 8;
	int		numlen = datalen - 8, i;

	// byw, 2002/8/27, 去掉奇偶校验信息
	for (i = 0; i < numlen; i ++)
	{
		*(pnum + i) &= 0x7F;
	}

	if (*pnum == 'O')
		pcallid->numberstatus = CALLID_NOTGET;
	else if (*pnum == 'P')
		pcallid->numberstatus = CALLID_FORBID;
	else{
////////////////////////////////////////////////shiyu   2005-07-15
                for (i = 0; i < numlen; i ++)
                {
                        *(pnum + i) &= 0x0F;
                }
                for (i = 0; i < numlen; i ++)
                {
                        *(pnum + i) |= 0x30;
                }
///////////////////////////////////////////////////

		for (i = 0; (i < numlen) && (i < CALLID_NUM_LEN -1); i ++)
			pcallid->number[i] = *(pnum + i);
		pcallid->number[i] = '\0';
		pcallid->numberstatus = CALLID_OK;
	}
}

////////////////////////////////
//
//	解 FSK 来电复杂格式参数包
//
void DecodeFSKComplexParam(BYTE *pparm, CALLID_INFO *pcallid)
{
	// pparm: pointer_parameter, pd: pointer_data
	int		datalen, i;
	BYTE	*pd;

	datalen = *(pparm + 1);
	pd = pparm + 2;

	/*
	*	odd-even check error
	*	just cast off check bit temporarily
	*/
	for (i = 0; i < datalen; i ++)
		*(pd + i) &= 0x7F;

	switch (*pparm){

	case CALLID_PARAM_TIME:

		/*
		*	Data time length error
		*/
		//if (datalen != 8)
		//	return;

		/*
		*	Decode date time
		*/
		pcallid->month = (*(pd + 0) - '0') * 10 + (*(pd + 1) - '0');
		pcallid->day = (*(pd + 2) - '0') * 10 + (*(pd + 3) - '0');
		pcallid->hour = (*(pd + 4) - '0') * 10 + (*(pd + 5) - '0');
		pcallid->minute = (*(pd + 6) - '0') * 10 + (*(pd + 7) - '0');
		pcallid->timestatus = CALLID_OK;
		break;

	case CALLID_PARAM_NUMBER:
		/*
		*	Decode number
		*/
/////////////////////////////////////////////////////////shiyu       2005-07-15
                for (i = 0; i < datalen; i ++)
                {
                        *(pd + i) &= 0x0F;
                }
                for (i = 0; i < datalen; i ++)
                {
                        *(pd + i) |= 0x30;
                }
///////////////////////////////////////////////////////

		for (i = 0; (i < datalen) && (i < CALLID_NUM_LEN -1); i ++)
			pcallid->number[i] = *(pd + i);
		pcallid->number[i] = '\0';
		pcallid->numberstatus = CALLID_OK;
		break;

	case CALLID_PARAM_NONUMBER:
		/*
		*	Set 'no number'
		*/
		if (*pd == 'O')
			pcallid->numberstatus = CALLID_NOTGET;
		else if (*pd == 'P')
                {
					pcallid->numberstatus = CALLID_FORBID;
                }
		else
			pcallid->numberstatus = CALLID_ERROR; //fjm
		break;

	case CALLID_PARAM_NAME:
		/*
		*	Decode name
		*/
		for (i = 0; (i < datalen) && (i < CALLID_NAME_LEN -1); i ++)
			pcallid->name[i] = *(pd + i);
		pcallid->name[i] = '\0';
		pcallid->namestatus = CALLID_OK;
		break;

	case CALLID_PARAM_NONAME:
		/*
		*	Set 'no name'
		*/
		if (*pd == 'O')
			pcallid->namestatus = CALLID_NOTGET;
		else if (*pd == 'P')
			pcallid->namestatus = CALLID_FORBID;
		else
			pcallid->namestatus = CALLID_FORBID;

		break;
	}
}


BOOL gIsHandSet = FALSE;
void ParseTelephoneData(unsigned char const* const data, unsigned int const length)
{	
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	extern void GMute(BOOL isOn);
	Sleep(10);
	int i;
	static BOOL isRingTelCode = FALSE;
	static CALLID_INFO	CallID;
	static UINT8   CallIDbuff[256];
	static int     CallIDLen = 0;

	for (i=0; i<length; i++)
		{		
			unsigned char c = data[i];
			if(c > 'z')
				Dprintf("com1:%x\r\n", c);
			if(c == '\n' || c == '\r')
				Dprintf("\r\n");
			
			int ret = main->phone_->DetectTestStatus(c);
			if(ret == 1)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CHANGE_BATTERY, 0, 0);
			}
			else if(ret == 2)
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CHANGE_BATTERYOK, 0, 0);
			}
			else if(ret == 3)  //DC --Battery
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_CHANGE_DCBATTERY, 0, 0);
			}
			else if(ret == 4)  //DC --Battery
			{
				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_POWER_OFF, 0, 0);
			}
		
			switch(c)
			{
				case HUNGON_VALUE://切换到手柄到下面0xC8
		//			if (!main->phone_->Hand())//手柄摘过机就不再进来
					{

		//				change by qi 20100831
		//				Dprintf("手柄摘机\r\n");
		//				main->phone_->Hand(Telephone::TelephoneWarp::HandOn);
		//				main->phone_->Free(Telephone::TelephoneWarp::FreeOff);
		//				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 2);//2代表硬件摘机
					}
					break;	
					
				case HUNGOFF_VALUE:
//					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD)
					{	
						if (main->phone_->Hand())//手柄摘机了，才处理
						{
							if (main->phone_->Free())//如果按过免提，切换到免提
							{	
								Dprintf("免提\r\n");
								
								GMute(FALSE);
								main->phone_->Hand(Telephone::TelephoneWarp::HandOff);
								//change by qi 0705
								//if(main->m_nTELRigster >= TELRIGSTER_TD)
								Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
								
								// add 20100702
								extern void GNotifyDial(BOOL isDial);
								GNotifyDial(TRUE);

								extern void GIsOpenMix(BOOL isOn);
								GIsOpenMix(1);
								
							}
							else//没按过免提直接挂断
							{	
								Dprintf("手柄挂机\r\n");
//								if(main->m_nTELRigster >= TELRIGSTER_TD)
//								Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
								main->phone_->Hand(Telephone::TelephoneWarp::HandOff);
								main->phone_->Free(Telephone::TelephoneWarp::FreeOff);
								PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);
							
							}

							if (main->m_phoneLine[2].pFSM->getCurrentState() 
								== CMultimediaPhoneDlg::p3gsHangOff )
							{
								main->phone_->PSTNHangoff();	
							}

						}

					}	
					break;			
						
// 				case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6:
// 				case 0xB7: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC:
//  				PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
// 					break;
			
				case 0xC8:   //Handset摘机
					
					//add bu qi 20100831
					Dprintf("手柄摘机\r\n");
					main->phone_->Hand(Telephone::TelephoneWarp::HandOn);
					main->phone_->Free(Telephone::TelephoneWarp::FreeOff);
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 2);//2代表硬件摘机
					//

					gIsHandSet = TRUE;					
					//main->phone_->Hand(Telephone::TelephoneWarp::HandOn);
					extern void GIsOpenMix(BOOL isOn);
					GIsOpenMix(1);
					
					//change by qi 0705
					//if(main->m_nTELRigster >= TELRIGSTER_TD )
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
					break;

				case 0xC9:	//Handfree 摘机
					gIsHandSet = FALSE;
					Dprintf("免提摘机\r\n");
					main->phone_->Hand(Telephone::TelephoneWarp::HandOff);
					main->phone_->Free(Telephone::TelephoneWarp::FreeOn);	
					
					//change by qi 0705
					//if(main->m_nTELRigster >= TELRIGSTER_TD )										
					Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
					break;

				case 0xCA://handSet->HandFree					
					main->phone_->Hand(Telephone::TelephoneWarp::HandOff);
					if (main->phone_->Free())//按过免提切换到免提
					{	
						//change by qi 0705
						//if(main->m_nTELRigster >= TELRIGSTER_TD)
							Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
						GMute(FALSE);						
						extern void GIsOpenMix(BOOL isOn);
						GIsOpenMix(1);	
					}
					else //挂断 
					{
						GMute(TRUE);
						PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);	
					}
					break;

				case 0xCB://handFree->handSet
					main->phone_->Hand(Telephone::TelephoneWarp::HandOn);
					main->phone_->Free(Telephone::TelephoneWarp::FreeOff);
					//change by qi 0705
					//if(main->m_nTELRigster >= TELRIGSTER_TD )
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);					
					GMute(TRUE);						
					extern void GIsOpenMix(BOOL isOn);
						GIsOpenMix(1);	
					break;

				case 1: case 2: case 3: case 4: case 5: case 6:	
				case 7: case 8: case 9: 
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SPEEDDIAL, c, 0);
					break;
			
				case 0xBD:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_REDIAL, 0, 0);
					break;
				case 0x88:
					{
						CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
						if (!main->m_bSearchNetOver)
						{
							return;
						}
						main->ReStoreBackLight();
						HWND hwnd = main->GetIconCurrentWnd()->m_hWnd;
						if(hwnd)
						{
							HWND hMjpg = ::GetDlgItem(hwnd, 10086);
							if(hMjpg)
							{
								::SendMessage(hMjpg, WM_KEYDOWN, c, 0);	
							}
						}
					}
					break;
				default:
					break;
			}
		}

}

void ParseTelephoneData_(unsigned char const* const data, unsigned int const length)
{	

	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
// 	if (!(main->OpenAllPort()))
// 	{
// 		return ;
// 	}

	extern void GMute(BOOL isOn);

	Sleep(10);
	int i;
	static BOOL isRingTelCode = FALSE;
	static CALLID_INFO	CallID;
	static UINT8   CallIDbuff[256];
	static int     CallIDLen = 0;
	
	for (i=0; i<length; i++)
	{		
		unsigned char c = data[i];
		Dprintf("com3:%x\r\n", c);
		if(c == '\n' || c == '\r')
			Dprintf("\r\n");
		
		char log[100];
		sprintf(log,"com:%x",c);
		WriteMyLog_(log,strlen(log));

		switch(c)
		{
		
		case HAND_ERROR:
			break;

		case HUNGON_VALUE://切换到手柄到下面0xC8
			//	((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SWitchPhone3G(false);
			//	main->phone_->Hand(Telephone::TelephoneWarp::HandOn);
			//	main->phone_->Free(Telephone::TelephoneWarp::FreeOff);
			//	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 3, 0);
			if ( main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHangOff)
			{
				main->phone_->PSTNHangoff();
			}
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SEND_0X82_WIN, 0, 0);
			break;

		case HUNGOFF_VALUE:
		//	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 3, 0);
			break;

		case DT_HUNGON_VALUE: //免提		
		//((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SWitchPhone3G(TRUE);
		//PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);

			if (main->phone_->Hand())//手柄已经摘机,此时按免提全是ON
			{
				main->phone_->Free(Telephone::TelephoneWarp::FreeOn);
			}
			else
			{
				if (main->phone_->Free())//按过免提，再按挂断
				{	
					Dprintf("挂机\r\n");
					main->phone_->Hand(Telephone::TelephoneWarp::HandOff);
					main->phone_->Free(Telephone::TelephoneWarp::FreeOff);
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);

					WriteMyLog_("Post_tel_hungoff",strlen("Post_tel_hungoff"));
				}
				else//没按过免提，也没按过手柄 = 开机
				{	
					gIsHandSet = FALSE;   //lxz20100108
					Dprintf("免提摘机\r\n");
					main->SendOutEvnet(WM_TEL_HUNGON, 0);
										
					main->phone_->Free(Telephone::TelephoneWarp::FreeOn);
					GMute(FALSE);//打开speeker
					
					//change by qi 0705
					//if(main->m_nTELRigster >= TELRIGSTER_TD)
					Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 0);

					// add 20100114
					extern void GNotifyDial(BOOL isDial);
					GNotifyDial(TRUE);
					
					WriteMyLog_("Post_tel_HUNGON",strlen("Post_tel_HUNGON"));
				
				}

			}
			break;

		case RING_VALUE:
			{
				keybd_event(VK_F9, 0, 0, 0);
			}		
 			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, 3);
			break;

		case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37:
		case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C: case 0x3D: case 0x3E: case 0x3F:
			isRingTelCode = TRUE;
			CallIDbuff[CallIDLen++] = c;
			break;

		case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
		case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
			isRingTelCode = TRUE;
			CallIDbuff[CallIDLen++] = c;
			break;

		case FSKEND_VALUE:
			isRingTelCode = FALSE;
			CallIDbuff[CallIDLen++] = c;
			memset(&CallID, 0, sizeof(CALLID_INFO));
			DecodeCallIDPackage(CallIDbuff, &CallID);
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_CALLIDEND, (WPARAM)&CallID, 3);
			CallIDLen = 0;
			break;
			
		case DTMFEND_VALUE:
			isRingTelCode = FALSE;
			CallIDbuff[CallIDLen++] = c;
			memset(&CallID, 0, sizeof(CALLID_INFO));
			DecodeCallIDPackage(CallIDbuff, &CallID);
			if(strlen(CallID.number) > 2 )
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_CALLIDEND, (WPARAM)&CallID, 3);
			CallIDLen = 0;
			break;

		case 0xC8:   //Handset摘机
			//gIsHandSet = TRUE;
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
			//	Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
			break;

		case 0xC9:	//Handfree 摘机
			//gIsHandSet = FALSE;
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )					
			//	Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
			break;
			
		case 0xCA:
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
			//	Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
			break;

		case 0xCB:
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
			//	Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
			break;
		
		case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6:
		case 0xB7: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
			{
				keybd_event(g_tel_code[c-0xB0],   0,   0,   0);  
				keybd_event(g_tel_code[c-0xB0],   0,   KEYEVENTF_KEYUP,   0);
			}
			break;

		case 4: case 5: case 6:	
		case 7: case 8: case 9:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SPEEDDIAL, c, 0);
			break;
/*
		case DT_UP_VALUE:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'U', 0);
			break;
			
		case DT_DOWN_VALUE:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'D', 0);
			break;				

		case DT_LEFT_VALUE:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'L', 0);
			break;				

		case DT_RIGHT_VALUE:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, 'R', 0);
			break;				

		case DT_OK_VALUE:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN,'O', 0);
			break;
*/							
		case DT_VK_F1:case DT_VK_F2:case DT_VK_F3:case DT_VK_F4:
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_VK_F,c-0x09, 0);
			break;
		
		case DT_MUTE_VALUE:
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_MUTE,0, 0);
			break;

		case SPEED_DIAL1:
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SPEEDDIAL,1, 0);
			break;

		case SPEED_DIAL2:
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SPEEDDIAL,2, 0);
			break;

		case REIVER_VALUE:
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_RECEIVE_BOX,0, 0);
			break;

		case FLASH_OFF:
		//	PostMessage(theApp.m_pMainWnd->m_hWnd,WM_TEL_HUNGOFF,0, 0);
			break;

		case 0xBD:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_REDIAL, 0, 0);
			break;

		case CANCEL_KEY:
		case OK_KEY:
		case LEFT_KEY:
		case RIGHT_KEY:
		case UP_KEY:
		case DOWN_KEY:
			{
				CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
				main->ReStoreBackLight();
				HWND hwnd = main->GetIconCurrentWnd()->m_hWnd;
				if(hwnd)
				{
					HWND hMjpg = ::GetDlgItem(hwnd, 10086);
					if(hMjpg)
					{
						::SendMessage(hMjpg, WM_KEYDOWN, c, 0);	
					}
				}
			}
			break;

		default:
			break;

		}
	}
}

static TelephoneState nState = stBeginRest;
TelephoneWarp::TelephoneWarp():Util::Observer()
{
	m_nSendIn = 0;   
	m_nSendOut = 0;   
	m_nRecvIn = 0;   
	m_nRecvOut = 0;
	m_nFirstCall = 0 ;

	m_bOpen1Port = false;
	m_bOpen3Port = false ;

	m_hKillThreadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);   
	m_hThreadKilledEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 
	InitializeCriticalSection(&m_csSend);   
	InitializeCriticalSection(&m_csRecv);

	//ring tone
	pGraph = NULL;   
	pMediaControl = NULL; 
	pMediaPosition = NULL;
	ipBasAudio       =   NULL;
	pEvent = NULL;
	g_RingSound = -1500; //-(MAX_SOUND-MIN_SOUND)/2; 
	gRingFilename[0] = '\0';
	
	m_chTelVersion[0] = '\0';
	m_chDetectCode[0] = '\0';
	m_bInputtingUserID = FALSE;
	m_bStartRing = FALSE;
	
	m_pMsgWnd = NULL;

	m_BatteryStatus.batteryProccess = 100;
	m_BatteryStatus.batteryType = BATTERY_INIT;
	m_BatteryStatus.isCharge = 0;
	m_BatteryStatus.isBattery_DC = FALSE;

	m_pRS232 = new Util::RS232();
	m_bOpen1Port = m_pRS232->OpenPort();
	m_pRS232->SetReadFunc(ParseTelephoneData);

	m_pRS232_ = new Util::RS232();
 	m_bOpen3Port = m_pRS232_->OpenPort(3);
 	m_pRS232_->SetReadFunc(ParseTelephoneData_);

	Hand(0);
	Free(0);
	

	//end ring tone
	::InitializeCriticalSection(&m_ringSetion_);
	InitRing();


}
TelephoneWarp::~TelephoneWarp()
{
	SetEvent(m_hKillThreadEvent);           // 发出关闭子线程的信号    
	WaitForSingleObject(m_hThreadKilledEvent, INFINITE);    // 等待子线程关闭    

	CloseHandle(m_hKillThreadEvent);   
	CloseHandle(m_hThreadKilledEvent);  
	
}

TelephoneWarp::TelephoneWarp(const TelephoneWarp& tel)
{

}

TelephoneWarp* TelephoneWarp::GetTelephoneWarp()
{
	static TelephoneWarp* tw = 0;
	if (tw == 0)
	{
		tw = new TelephoneWarp();
	}
	return tw;
}
void TelephoneWarp::Bind_(Util::ATCommandWarp* at)
{
	m_pAT = at;
}

void TelephoneWarp::Bind(Util::ATCommandWarp* at)
{
	// 启动子线程  
	Util::ATCommand::Instance()->RegisterObserver(this);

}

void TelephoneWarp::Update(std::string const& data)
{		   
//	ParseATD(data);
	ParseCLIP(data);
	ParseDACTI(data);
	ParseCSQ(data);
	ParseCCWA(data);
	ParseDSCI(data);
	ParseCLCC(data);//放在DSCI下面
	ParseCIEV(data);

}

void TelephoneWarp::ParseATD(std::string const& data)
{
	if (data.find("NO DIALTONE") != std::string::npos)
	{
	//	NoDialTone();
	}
	else if (data.find("BUSY") != std::string::npos)
	{
	//	Busy();
	}
	else if (data.find("NO ANSWER") != std::string::npos)
	{
//		NoAnswer();	
	}
	else if (data.find("NO CARRIER") != std::string::npos)
	{
//		NoCarrier();	
	}
	else if (data.find("CONNECT") != std::string::npos)
	{
//		Connect();
	}
	else if (data.find("NW CONGESTION") != std::string::npos)
	{
//		Congestion();
	}
	else if (data.find("ERROR ODB") != std::string::npos)
	{
//		Odb();	
	}

}

void TelephoneWarp::ParseCLCC(std::string const& data)
{	
	/*
		+CLCC: 1,1,0,0,0,"10000",161\r\n
		+CLCC: 2,1,5,0,0,"88888",161\r\n0\r
	*/
	
	std::string s = data ;
	if ( s.find("+CLCC") != std::string::npos)
	{				
		int allLine = 0;
		size_t t1 ;
		while ((t1 = s.find("+CLCC")) != std::string::npos)
		{	
			s = s.substr(t1+7);
			allLine++;
		}
		
		if (allLine > 2)
		{	
			allLine = 2;
		}
		
		::PostMessage(theApp.m_pMainWnd->m_hWnd,WM_ALL_PHONE_NUM,allLine,0);

		s = data ;
		size_t t = s.find("+CLCC");
		s = s.substr(t);

		while (s.find("+CLCC") != std::string::npos)
		{	
			int line  ;
			CString c ;
			size_t t = s.find("+CLCC");
			s = s.substr(t+7);
			
			c = Util::StringOp::ToCString(s);
			line = atoi(c);
			
			if ( 3 == line)//第三路不处理
			{
				break;
			}
			
			s = s.substr(2);
			if ( s.substr(0,3) == "0,0" || s.substr(0,3) == "1,0" )
			{				
				Connected(line);//active
			}
			else if( s.substr(0,3) == "0,1" ||  s.substr(0,3) == "1,1" )
			{
				Held(line) ;//held
			}
			else if( s.substr(0,3) == "0,2")
			{
				Dialing(line) ;//dialing
			}
			else if( s.substr(0,3) == "0,3")
			{
				Alerting(line) ;//altering
			}
			else if( s.substr(0,3) == "1,4")
			{
				Ring(line) ;//ring
			}
			else if( s.substr(0,3) == "1,5")
			{
				Waiting(line) ;//waiting
			}

		}

	}

}

void TelephoneWarp::ParseDSCI(std::string const& data)
{
	//先来判断哪路被挂断
	if (data.find("^DSCI: ") != std::string::npos)
	{	
		/*
			^DSCI: 1,1,6,0,0,"01082897270",161,,16,0,"",0
		*/
		
		extern VOID WriteLog_(char *ptr, int size);
		WriteLog_("Dsci:",strlen("Dsci:"));
		WriteLog_((char*)data.c_str(),data.size());
		
		std::string s = data ;

		while (s.find("^DSCI: ") != std::string::npos)
		{   
			//add by qi 0827
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
			if (main->m_pATCommandWarp1->GetOkStatus() != okSmsSend
				&& main->m_pATCommandWarp1->GetOkStatus() != okChld)
			{
				main->m_pATCommandWarp1->SetOkStatus(okIdle);
			}

			CString c ;
			int line;
			int cause;
			int callstatus;

			size_t t = s.find("^DSCI: ");
			s = s.substr(t+7);
			c = Util::StringOp::ToCString(s);
			line = atoi(c);
			
			//发现第三路打进来，不处理
			if ( 3 == line)
			{
				break;
			}

			t = s.find(",");
			if (t != std::string::npos)
			{
				s = s.substr(t+1);
				t = s.find(",");
				if ( t != std::string::npos)
				{
					s = s.substr(t+1);
					c = Util::StringOp::ToCString(s);
					callstatus = atoi(c);//状态
					switch (callstatus)
					{	
						case 0://active
							break;
						case 1://hold
							break;
						case 2://Dialing
							Dialing(line);
							break;
						case 3://altering
							Alerting(line);
							break;
						case 4://Incoming
							break;
						case 5:
						 break;
					}
					
					int num = 0;
					while ( num != 6 && ( (t = s.find(",")) != std::string::npos))
					{
						s = s.substr(t+1);
						num++;
						
						std::string tel = s; //呼叫等待状态
						if ( 3 == num )
						{
							tel = s.substr(1,s.find(",")-1-1);
							if ( /*2 == line && */ 5 == callstatus || 4 == callstatus )
							{
								Incoming((char *)tel.c_str(),line);
							}
						}
					}
					
					c = Util::StringOp::ToCString(s);
					cause = atoi(c);
					Dprintf("Cause: %d\r\n",cause);
					switch (cause)
					{
					case 8 ://congestion
						Odb(line);
						break;
						
					case 1: //Unassigned(unallocated) number
					case 31://Normal,unspecified
					case 34://
					case 44://Requested circuit/channel not available
					case 16://no carrier
					case 102://Recovery on timer expiry
					case 111:
						{
							NoCarrier(line);

							//add by qi 0827
							//CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg *)theApp.m_pMainWnd;
							//if (main->m_pATCommandWarp1->GetOkStatus() != okSmsSend)
							//{
							//	main->m_pATCommandWarp1->SetOkStatus(okIdle);
							//}
						}
						break;
						
					case 17://user busy
						Busy(line);
						break;
						
					case 19://User alerting, no answer
						NoAnswer(line);
						break;
						
					case 22:
						Congestion(line);//call rejected
						break;
						
					}
					
				}
			}
		}

	}

}

void TelephoneWarp::ParseCLIP(std::string const& data)
{
	/*	
		+CRING: VOICE\r\n
		+CLIP:"10000",161,"",,"",0\r\n
	*/
	
	//CALLID line1
	if (data.find("+CLIP:") != std::string::npos)
	{
		std::string s = data ;
		size_t t = s.find("+CLIP:");
		
		if ( t != std::string::npos)
		{
			s = s.substr(t);
			t = s.find('\"');
			if (t != std::string::npos)
			{
				s = s.substr(t+1);
				t = s.find('\"');
				if (t != std::string::npos)//电话号码
				{
					s = s.substr(0,t);
					Incoming(( char* )s.c_str());
				}
			}
		}
	}

		
}

void TelephoneWarp::ParseCCWA(std::string const& data)
{	
	//第二路呼入
	/*
	+CCWA: "88888",161,1,,0,"",0\r\n
	*/
	//CALLID line2
	if (data.find("+CCWA") != std::string::npos)
	{
		std::string s = data ;
		size_t t = s.find("+CCWA"); 
		if ( t != std::string::npos)
		{
			s = s.substr(t);
			t = s.find('\"');
			if ( t != std::string::npos)
			{
				s = s.substr(t+1);
				t = s.find('\"');
				if ( t != std::string::npos)
				{
					s = s.substr(0,t);
					Incoming(( char* )s.c_str(),2);
				}
			}

		}
	}

}

int static signalOrg = -1;

void TelephoneWarp::ParseDACTI(std::string const& data)
{
	unsigned int  netType = 0xF;
	unsigned int static oldType = 3 ;

	CString c ;
	char *ans= (char *)(data.c_str());
	if (strstr(ans, "^DACTI: ") != NULL)
	{
// 		if(strstr(ans, "OK\r\n") != NULL)   
// 		{
			char* p = strstr(ans, "^DACTI: ");//
			if (p != NULL)
			{
				p += 8;
				
				char *p1 = strstr(ans, ",");
				char i[4] = {0};
				if(p1 != NULL)
				{
					memcpy(i, p1+1, 1);
					c = Util::StringOp::ToCString(i);
					netType = atoi(c);
				}
				else
				{
					c = Util::StringOp::ToCString(p);
					netType = atoi(c);
				}
			}
 	//	}

		if(netType != 0xF && oldType != netType )
		{	
			PhoneNettype(netType);
			oldType = netType;
			signalOrg = 0;
		}
	}


}
void TelephoneWarp::ParseCSQ(std::string const& data)
{	
	CString c ;
	if ( data.find("+CSQ: ") != std::string::npos)
	{
		//显示信号值
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;


		int result = 0xFF;
		char *ans= (char *)(data.c_str());
	
	//  change by qi 20100805
	//	if(strstr(ans, "OK\r\n") != NULL)
		if(strstr(ans, "\r\n") != NULL)   
		{
			char* p = strstr(ans, "+CSQ: ");//+CSQ: 129,99
			if (p != NULL)
			{
				p += 6;
				std::string st = p;
				c = Util::StringOp::ToCString(st);
				result = atoi(c);
				if (result == 99)//G网无网络
				{
					result = 0;
				}
				else if( result >= 100 && result < 199)
				{
				//	result = result - 100 - 116 + 113;
					result = result - 100 ;
				//	result = (result + 1) / 2;
					if (result < 8)
					{
						result = 1;
					}
					else
					{
						//	result = (result - 8) / 6 + 2;
						if ( 27 <= result && result <= 31 )
						{
							result = 5;
						}
						else if ( result < 27 && result >= 23)
						{
							result = 4;
						}
						else
						{
							result = result / 6;

						}
					}

					if (result > 5)
					{
						result = 5;
					}
				}
				else if ( 199 == result)
				{
					result = 0 ;
				}
				else if (result < 99)
				{
					if ( 27 <= result && result <= 31 )
					{
						result = 5;
					}
					else if ( result < 27 && result >= 23)
					{
						result = 4;
					}
					else 
					{
						result = result/6;
					}
					
				}
				
			}
		}
		
		if (result != 0xFF && result != signalOrg)
		{
			signalOrg = result;
			SignalQuality(result);
		}
	}

}

void TelephoneWarp::ParseCIEV(std::string const& data)
{	
	if ( data.find("+CIEV: 2") != std::string::npos)
	{	
		CString c ;
		std::string s = data ;
		size_t t = s.find("+CIEV: 2");
		if ( t != std::string::npos)
		{
			s = s.substr(t+1);
			c = Util::StringOp::ToCString(s);
		}	
	//	SignalQuality(atoi(c));
	}
	else if (data.find("+CIEV: 8,1") != std::string::npos)
	{
		PostMessage(theApp.m_pMainWnd->m_hWnd,WM_SMS_FULL,0,0);	
	}
}

// 将一条短消息放入发送队列    
void TelephoneWarp::PutSendMessage(TEL_NUM* pparam)   
{   
	EnterCriticalSection(&m_csSend);   

	memcpy(&m_PhoneSend[m_nSendIn], pparam, sizeof(TEL_NUM));   

	m_nSendIn++;   
	if (m_nSendIn >= MAX_PHONE_SEND)  m_nSendIn = 0;   

	LeaveCriticalSection(&m_csSend);   
}   

// 从发送队列中取一条短消息    
BOOL TelephoneWarp::GetSendMessage(TEL_NUM* pparam)   
{   
	BOOL fSuccess = FALSE;   

	EnterCriticalSection(&m_csSend);   

	if (m_nSendOut != m_nSendIn)   
	{   
		memcpy(pparam, &m_PhoneSend[m_nSendOut], sizeof(TEL_NUM));   

		m_nSendOut++;   
		if (m_nSendOut >= MAX_PHONE_SEND)  m_nSendOut = 0;   

		fSuccess = TRUE;   
	}   

	LeaveCriticalSection(&m_csSend);   

	return fSuccess;   
}   

// 将短消息放入接收队列    
void TelephoneWarp::PutRecvMessage(TEL_NUM* pparam, int nCount)   
{   
	EnterCriticalSection(&m_csRecv);   

	for (int i = 0; i < nCount; i++)   
	{   
		memcpy(&m_PhoneRecv[m_nRecvIn], pparam, sizeof(TEL_NUM));   

		m_nRecvIn++;   
		if (m_nRecvIn >= MAX_PHONE_RECV)  m_nRecvIn = 0;   

		pparam++;   
	}   

	LeaveCriticalSection(&m_csRecv);   
}   

// 从接收队列中取一条短消息    
BOOL TelephoneWarp::GetRecvMessage(TEL_NUM* pparam)   
{   
	BOOL fSuccess = FALSE;   

	EnterCriticalSection(&m_csRecv);   

	if (m_nRecvOut != m_nRecvIn)   
	{   
		memcpy(pparam, &m_PhoneRecv[m_nRecvOut], sizeof(TEL_NUM));   

		m_nRecvOut++;   
		if (m_nRecvOut >= MAX_PHONE_RECV)  m_nRecvOut = 0;   

		fSuccess = TRUE;   
	}   

	LeaveCriticalSection(&m_csRecv);   

	return fSuccess;   
}   

UINT TelephoneWarp::TelephoneThread(LPVOID lParam)
{

// 	TelephoneWarp* p=(TelephoneWarp *)lParam;   // this    
// 	char buff[1024];           // 接收电话列表的缓冲区   来电号码 
// 	TEL_NUM param[256];    // 发送/接收电话缓冲区    
// 	char* state = NULL;
// 	unsigned int signalOrg = 0;
// 	unsigned int signalNow = 0;

	// 发送和接收处理的状态循环    
	while (nState != stExitThread) 
	{   				
		switch(nState)   
		{   
// 		case stBeginRest:   
// 			TRACE(L"State=stBeginRest\n");    
// 			Sleep(1000);
		//	nState = stContinueRest;   
		//	EnterCriticalSection(&csCom);
// 			nState = stReadPhoneRequest;   
// 			TRACE(L"Tel Enter\n");
// 			break;

// 		case stContinueRest:   
// 			TRACE(L"State=stContinueRest\n");    
// 			if (p->GetSendMessage(param))   
// 			{   
// 					nState = stSendPhoneRequest;  //有待拨打电话，就不休息了
// 				
// 			}   
// 			else  
// 			{   
// 					nState = stReadPhoneRequest;  // 转到读来电状态
// 
// 			}   
// 			break;   
// 
// 		case stSendPhoneRequest:   
// 			{
// 				CMultimediaPhoneDlg*main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
// 				if(main->m_pTelephoneDlg->IsWindowVisible())
// 				{
// 					TRACE(L"State=stSendPhoneRequest\n");    
// 					p->Dial_(param[0]);   
// 					memset(buff, 0, sizeof(buff));  
// 						nState = stSendPhoneResponse;   
// 				}
// 				else
// 				{
// 						nState = stReadPhoneRequest;  //转到读来电状态    
// 				}
// 			}
// 			
// 			break;   

// 		case stSendPhoneResponse:   
// 			TRACE(L"State=stSendPhoneResponse\n");    
// 			Sleep(1000);   
// 			p->State_();
// 			break;

		case stReadPhoneRequest:   
			TRACE(L"State=stReadPhoneRequest\n");    
		//	memset(&buff, 0, sizeof(buff));   
		//	memset(buff, 0, sizeof(buff));   
			
// 			Sleep(1000);
// 			p->State_();//CLCC			
// 			p->PhoneNettype_();//DACTI
// 			p->SignalQuality_();//CSQ

		//	LeaveCriticalSection(&csCom);
			TRACE(L"Tel Leave\n");		   		
			nState = stBeginRest;
						
			//test lxz
			break;   

// 		case stReadPhoneResponse:   
// 			TRACE(L"State=stReadPhoneResponse\n");    
// 			Sleep(1000);   
// 			p->State_();
// 			break;   

// 		case stRedial:
// 			TRACE(L"State=stRedial\n");    
// 			p->Redial_();
// 			break;

	//	case stAnswer:
	//		TRACE(L"State=stAnswer\n");    
	//		p->Answer_();
	//			nState = stReadPhoneResponse;  
	//		break;

// 		case stHangupPhone:
// 			TRACE(L"State=stHangupPhone\n");    
// 			p->Hangup_();
// 			if (nState != stAnswer && nState != stRedial)
// 				nState = stBeginRest;
// 		//	LeaveCriticalSection(&csCom);
// 			TRACE(L"Tel Leave\n");
// 			break;

		} 
		// 检测是否有关闭本线程的信号  
		
//		DWORD dwEvent = WaitForSingleObject(p->m_hKillThreadEvent, 20);   
//		if (dwEvent == WAIT_OBJECT_0)  nState = stExitThread;
		
	}   

	// 置该线程结束标志    
//	SetEvent(p->m_hThreadKilledEvent); 
	return 0;

}

void TelephoneWarp::Dial_(TEL_NUM num)
{
	m_pAT->PhoneDial(num.NUM, (BOOL)num.TYPE);
}

void TelephoneWarp::State_(void)
{
	 m_pAT->PhoneState();
}

void TelephoneWarp::Hangup_(void)
{
	m_pAT->PhoneHangup();
}

void TelephoneWarp::Answer_(void)
{
	m_pAT->PhoneAnswer();
}

void TelephoneWarp::Redial_()
{
	m_pAT->PhoneRedial();
}

void TelephoneWarp::Incoming_(TEL_NUM* num)
{	
	m_pAT->PhoneRing(num->NUM, &num->TYPE);
}

void TelephoneWarp::SignalQuality_(void)
{
	m_pAT->PhoneSignalQuality();
}

void TelephoneWarp::PhoneNettype_(void)
{
	m_pAT->PhoneNettype();
}


void TelephoneWarp::PhoneDialTone(BOOL isOn, char *tone)
{
	extern void GMute(BOOL isTrue);
	if (Hand())//lxz20100609
	{
		GMute(TRUE);
	}
	else
	{
		GMute(FALSE);//
	}

	//add by qi 20100708
	if (tone != NULL)
	{
		if (strlen(tone) > 100)
		{	
			CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
			main->m_pWarningNoFlashDlg->SetTitle(L"号码太长");
			main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
			return ;
		}
	}	

	m_pAT->PhoneDialTone(isOn, tone);
}
//////////////////////////////////////////////////////////////////////////
bool TelephoneWarp::Dial(char* number, BOOL isVideo)
{
//	TEL_NUM num;
//	memset(&num, 0, sizeof(TEL_NUM));
//	strcpy(num.NUM, number);
//	num.TYPE = isVideo;
	
	//chnage by qi 
	if (strlen(number) > 1000)
	{	
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
		main->m_pWarningNoFlashDlg->SetTitle(L"号码输入过长");
		main->m_pWarningNoFlashDlg->ShowWindow_(SW_SHOW);
		return false;
	}

	m_pAT->PhoneDial(number,isVideo);
//	PutSendMessage(&num);
	return true;

}
bool TelephoneWarp::Hangup(void)
{
//	nState = stHangupPhone;
	Hangup_();
	return true;
}

void TelephoneWarp::Answer(void)
{
// 	nState = stAnswer;
// 	Dprintf("Set Answer--------------\r\n");
// 	return true;
	Answer_();
}

bool TelephoneWarp::Redial(void)
{ 
	nState = stRedial;
	return true;
}

void TelephoneWarp::SubDial(char number)
{
	m_pAT->PhoneSubDial(number);
}

void TelephoneWarp::Volume(unsigned int level)
{
	 m_pAT->PhoneVolume(level);
}

void TelephoneWarp::SendDialTone(char *code, int isON)
{
	m_pAT->SendDialTone(code, isON);
}

void TelephoneWarp::Mute(bool isMute)
{
	 m_pAT->PhoneMute(isMute);
}

void TelephoneWarp::AutoAnswer(unsigned int second)
{
	 m_pAT->PhoneAutoAnswer(second);
}

void TelephoneWarp::HandFree(bool isHandFree)
{
	 m_pAT->PhoneHandFree(isHandFree);
}

std::string TelephoneWarp::GetNumber(void)
{
	return m_pAT->PhoneNumber();
}
//////////////////////////////////////////////////////////////////////////
void TelephoneWarp::Incoming(char* num,int const line )
{
//	m_nFirstCall++;
	static CALLID_INFO CallID;
	memset(&CallID, 0, sizeof(CALLID_INFO));
	strcpy(CallID.number, num);
	CallID.Line = line;
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_CALLIDEND, (WPARAM)&CallID, line);
	
}

void TelephoneWarp::Ring(int const line)//来电振铃
{	
	Dprintf("TelephoneWarp Ring : %d\r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMRING, line);
}

void TelephoneWarp::Busy(int const line)//对方忙
{	
	Dprintf("TelephoneWarp Busy : %d \r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMBUY, line);
}

void TelephoneWarp::Connected(int const line) //通话联通状态
{
//	Dprintf("TelephoneWarp Connected : %d \r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONNECTED, line);
}

void TelephoneWarp::Connect(int const line)//对方接听
{
//	Dprintf("TelephoneWarp Connect : %d \r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONNECT, line);
}

void TelephoneWarp::Held(int const line)//保持状态
{	
	Dprintf("TelephoneWarp Held : %d \r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMHELD, line);
}
void TelephoneWarp::Dialing(int const line)//正在拨打状态
{
	Dprintf("Dialing:%d\r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMDIALING, line);
}
void TelephoneWarp::Alerting(int const line)//对方振铃状态
{
	Dprintf("Altering: %d\r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMALERTING, line);
}
void TelephoneWarp::Waiting(int const line)
{
	Dprintf("Watting : %d\r\n",line);
	
	//add by qi 20100317 
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMWAITING, line);
}

void TelephoneWarp::NoDialTone(int const line)
{	
	Dprintf("NoDialTone :%d\r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNODIALTONE, line);
}
void TelephoneWarp::NoAnswer(int const line)//无应答
{
	Dprintf("NoAnswer :%d\r\n",line);

//	PhoneDialTone(1, "hangup");
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNOANSWER, line);   //
}
void TelephoneWarp::NoCarrier(int const line)//连接不能被建立
{	
	Dprintf("NoCarrier :%d\r\n",line);
//  change by qi 20100315
//	PhoneDialTone(1, "hangup");
//	Hangup_();

	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNOCARRIER, line);
}
void TelephoneWarp::Congestion(int const line)//网络拥塞
{	
	Dprintf("Congestion :%d\r\n",line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONGESTION, line);
}
void TelephoneWarp::OppHangup(int const line)//对方挂机
{
//	PhoneDialTone(1, "hangup");
//	nState = stHangupPhone;
//	Hangup_();
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMOPPHUNGUP, line);
}

void TelephoneWarp::Odb(int const line)//呼叫限制
{
//	PhoneDialTone(1, "hangup");
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMODB, line);
}
void TelephoneWarp::SignalQuality(int level)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_SIGNALQUALITY, level);
}
void TelephoneWarp::PhoneNettype(int type)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_NETTYPE, type);
}

//拨打电话
void TelephoneWarp::DialNumber(char* telcode, int dial_tyle)   //int dial_type 0 免提， 1 摘机
{
	Dial(telcode, dial_tyle);
}

//挂机
void TelephoneWarp::HungOff()
{	
	Hangup();
	m_nFirstCall = 0 ;

}
//摘机
void TelephoneWarp::HungOn(BOOL isRing)
{
//	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 0, 0);   //lxz 20091111
	if(isRing)
		Answer();   
}

void TelephoneWarp::Hand(int hand )
{	
	if ( HandOn == hand)
	{
		Dprintf("HandOn\r\n");
	}
	else
	{
		Dprintf("HandOff\r\n");
	}
	m_HandSet = (HandSet) hand;
}

int TelephoneWarp::Hand()
{
	return m_HandSet;
}

void TelephoneWarp::Free(int free )
{
	if ( FreeOn == free)
	{
		Dprintf("FreeOn\r\n");
	}
	else
	{
		Dprintf("FreeOff\r\n");
	}
	m_FreeSet =(FreeSet)free;
}

int TelephoneWarp::Free()
{
	return m_FreeSet;
}

//打开/关闭振铃
void TelephoneWarp::OpenTelRing(BOOL Flag )
{
	
}
//静音
void TelephoneWarp::Mute()
{

}
//录音
void TelephoneWarp::HungOnToRecord()
{

}

//ring tone    
#pragma   comment   (lib,"Ole32.lib") 
#pragma   comment   (lib,"Strmiids.lib") 

void TelephoneWarp::PlayRingPrc(void)
{
	CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;

	BOOL bRun = TRUE;
	while(bRun)
	{
		Sleep(10);
		long evCode;
		if(main->phone_->pMediaControl &&  main->phone_->pGraph && main->phone_->ipBasAudio  && main->phone_->pEvent && main->phone_->gRingFilename[0] != '\0' && main->phone_->nRingCount ) 	//循环播放
		{
			main->phone_->pEvent->WaitForCompletion(5, &evCode);
		//	Dprintf("evCode = %x\n", evCode);
			if(evCode != 0)	
			{
				main->phone_->nRingCount--;
				if(main->phone_->nRingCount)
				{
					main->phone_->pMediaPosition->put_CurrentPosition(0);
					main->phone_->pMediaControl->Run();
				}
				else
				{
// 					if(main->phone_->m_pMsgWnd)
// 						main->phone_->m_pMsgWnd->SendMessage(WM_STOPTRYRING);
//					main->phone_->StartRing(L"");
					main->phone_->StopRing();
				}
			//	main->phone_->StartRing(main->phone_->gRingFilename);
			}
		}		
	}
}

void TelephoneWarp::InitRing()
{
	CoInitialize(NULL); 
	hPlayRingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PlayRingPrc, NULL, 0, NULL);
}

void TelephoneWarp::ReleaseRing()
{
	CloseHandle(hPlayRingThread);
}

void TelephoneWarp::InitRingSrc()
{
	//
	//  以下程序每次播放MP3(或其他格式)文件时调用        
	//  创建一个Filter   Graph   manager      
	CoCreateInstance(CLSID_FilterGraph,   NULL,   CLSCTX_INPROC,   IID_IGraphBuilder,   (void   **)&pGraph); 
	pGraph->QueryInterface(IID_IMediaControl,   (void   **)&pMediaControl); 
	pGraph->QueryInterface(IID_IMediaPosition,(LPVOID*)&pMediaPosition);

	//得到音频相关接口 
    pGraph->QueryInterface(IID_IBasicAudio,   (void   **)&ipBasAudio); 
	pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

}

void TelephoneWarp::RelaseRingSrc()
{
	//  释放对象 
	if(pMediaControl)
	{
		pMediaControl->Stop();
	}
	if(pEvent)
	{
		pEvent->Release();
		pEvent = NULL;
	}
	if(ipBasAudio)
	{
		ipBasAudio->Release();
		ipBasAudio = NULL;
	}
	if(pMediaPosition)
	{
		pMediaPosition->Release();
		pMediaPosition = NULL;
	}
	if(pMediaControl)
	{
		pMediaControl->Release(); 
		pMediaControl = NULL;
	}
	if(pGraph)
	{
		pGraph->Release();  
		pGraph = NULL;
	}
}

void TelephoneWarp::StartRing(TCHAR *filename, int ncount)
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if(wcslen(filename) == 0)
	{
		memset(gRingFilename, 0, sizeof(TCHAR)*64);
		StopRing();
		return;
	}
	
	Dprintf("StartRing\r\n");
	extern void GMute(BOOL isOn);
	if (main->phone_->Hand())//
	{
		GMute(TRUE);
	}
	else
	{
		GMute(FALSE);//
	}
	

	::EnterCriticalSection(&m_ringSetion_);
	nRingCount = ncount;
//	memset(gRingFilename, 0, 64*2);
	memcpy(gRingFilename, filename, wcslen(filename)*2);	
	gRingFilename[wcslen(filename)] = '\0';
	RelaseRingSrc();
	InitRingSrc();
	::LeaveCriticalSection(&m_ringSetion_);
	if(pGraph && pMediaControl)
	{
		//  连接播放指定文件所需的filter    
		pGraph->RenderFile(filename,   NULL); 
		//  开始播放指定的文件    
		pMediaControl->Run(); 
		if(ipBasAudio)
		{
			Dprintf("g_RingSound = %x\n", g_RingSound);
			
			waveOutGetVolume(NULL,&m_DOldVoulme);
			waveOutSetVolume(NULL, g_RingSound);
			
		//	ipBasAudio->put_Volume(g_RingSound); 
		}
	}
	m_bStartRing = TRUE;

}

void TelephoneWarp::StopRing(bool releaseSrc,bool breset)
{
	/*
	sndPlaySound(NULL, 0);
	return;
	*/
	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	::EnterCriticalSection(&m_ringSetion_);
	if(m_bStartRing)
	{
		gRingFilename[0] = '\0';
		if(releaseSrc)
		{
			RelaseRingSrc();
		}
		
		if(m_pMsgWnd)
		{
			m_pMsgWnd->SendMessage(WM_STOPTRYRING);
		}
		m_bStartRing = FALSE;
	}
	m_pMsgWnd = NULL;
	Dprintf("Stop Ring\r\n");
	
	//add by qi
	if (breset)
	{
		//waveOutSetVolume(NULL,m_DOldVoulme);
		waveOutSetVolume(NULL,0xFFFFFFFF);
	}

	extern void GMute(BOOL isOn);	
	GMute(TRUE);
		
	::LeaveCriticalSection(&m_ringSetion_);

}

void TelephoneWarp::PauseRing()
{
	if(pMediaControl)
		pMediaControl->Pause();
}

void TelephoneWarp::ResumeRing()
{
	if(pMediaControl)
		pMediaControl->Run();
}

//-8000  ~ 0
void TelephoneWarp::SetSoundRingVal(int val)
{
	if(val <= -8000)
		val = -8000;
	else if(val > 0)
		val = 0;
	g_RingSound = val;
	if(ipBasAudio)
	{
		ipBasAudio->put_Volume(g_RingSound);
	}
}

void TelephoneWarp::ReduceSoundRing(int offset)
{
	if(ipBasAudio)
	{
		g_RingSound -= offset;
		if(g_RingSound < MIN_SOUND)
			g_RingSound = MIN_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void TelephoneWarp::IncreaseSoundRing(int offset)
{
	if(ipBasAudio)
	{
		g_RingSound += offset;
		if(g_RingSound > MAX_SOUND)
			g_RingSound = MAX_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void TelephoneWarp::MinSoundRing()
{
	if(ipBasAudio)
	{
		g_RingSound = MIN_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void TelephoneWarp::MaxSoundRing()
{
	if(ipBasAudio)
	{
		g_RingSound = MAX_SOUND;
		ipBasAudio->put_Volume(g_RingSound); 
	}
}

void TelephoneWarp::ResumeSoundValueRing()
{
	g_RingSound = DEFUALT_SOUND_VALUME;
	if(ipBasAudio)
	{
		ipBasAudio->put_Volume(g_RingSound); 
	}
}


bool TelephoneWarp::SWitchPhone3G(BOOL is3G)
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!(main->OpenAllPort()))
	{
		return false;
	}
	
	if (is3G)
	{
		Dprintf("switch 3G\r\n");
	}
	else 
	{
		Dprintf("switch PSTN\r\n");
	}

	unsigned char data[] = {0xC2, 0xC1};//手柄给PSTN用 还是给3G用
	m_pRS232_->WritePort(&data[is3G], 1);
	return true;
}

void TelephoneWarp::PSTNHangoff()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!(main->OpenAllPort()))
	{
		return;
	}
	
	Dprintf("PSTNHangoff \r\n");
	unsigned char c = 0x8b;
	m_pRS232_->WritePort(&c, 1);

}

void TelephoneWarp::PSTNHangoff_()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!(main->OpenAllPort()))
	{
		return;
	}
	
	Dprintf("PSTNHangoff_ \r\n");
	unsigned char c = 0x83;
	m_pRS232_->WritePort(&c, 1);
	
}

void TelephoneWarp::PSTNHangOn()
{	
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!(main->OpenAllPort()))
	{
		return;
	}
	
	Dprintf("PSTNHangOn \r\n");
	unsigned char c = 0x82;
	m_pRS232_->WritePort(&c, 1);

	//启动定时器，检查指令是否发送成功
	::KillTimer(main->m_pTelephoneDlg->m_hWnd,CHECK_0X82_TIMER);
	::SetTimer(main->m_pTelephoneDlg->m_hWnd,CHECK_0X82_TIMER,150,NULL);

}

void TelephoneWarp::PSTNRingMute(bool bmute )
{
	CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	if (!(main->OpenAllPort()))
	{
		return;
	}

	Dprintf("PSTNRingMute \r\n");
	unsigned char c ;
	if (bmute)
	{
		c = 0xD0;	
	}
	else
	{
		c = 0xD1;
	}
	m_pRS232_->WritePort(&c, 1);
	
}