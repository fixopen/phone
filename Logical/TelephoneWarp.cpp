#include "stdafx.h"
#include "TelephoneWarp.h"
#include "../multimediaphone.h"
#include "../multimediaphoneDlg.h"
#include "../resource.h"

#define HUNGON_VALUE	0x82		//0x82	 摘机
#define HUNGOFF_VALUE	0x83		//0x83   挂机
#define RING_VALUE		0x84		//0x84   振铃信号
//#define WM_TEL_CALLIDEND                8023

#define DEFUALT_SOUND_VALUME   -1500

using namespace Telephone;

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
int gBatteryLevel[] = {830, 805, 780, 760, 740, 720};
int gBatteryOffset = 110;
int gBatteryBase = 720;
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
		main->phone_->m_bInputtingUserID = FALSE;
		char *ptr;
		static int batterySize = 0;
		if((ptr = strstr(main->phone_->m_chDetectCode, "AT=")) > 0)
		{
//			if ((ptr = strstr(main->phone_->m_chDetectCode, "AT=000")) > 0 && 
//				((ptr = strstr(main->phone_->m_chDetectCode, "CHG=0")) > 0))
			if ( strstr(main->phone_->m_chDetectCode, "AT=000") > 0 && 
			( strstr(main->phone_->m_chDetectCode, "CHG=0") > 0))
			{
				main->KillTimer(0x120);
				main->m_MJPGList.SetUnitBitmap(11, L"", L"",TRUE);
				main->m_isHaveBattery = FALSE;
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
				int n = Util::StringOp::ToInt(txt, 10);

				isBatteryStatus = 1;
				if(strstr(ptr, "CHG=2"))
				{
					if(batterySize == 2)
					{
					}
					else
					{
						batterySize = 2;
						main->phone_->m_BatteryStatus.isCharge = 0;
						isBatteryStatus = 2;
						int off = n - gBatteryBase;
						int index = 1;
						for(int i = 3; i >= 0; i--)
						{
							gBatteryLevel[i] = gBatteryBase+ (gBatteryOffset*25*index)/off;
							index++;
						}
						main->phone_->m_BatteryStatus.batteryProccess = 5;
					}
					return isBatteryStatus;
				}
				if(strstr(ptr, "CHG=1"))
				{
					batterySize = 1;
					for(int i = 0; i < 6; i++)
					{
						if(n >= gBatteryLevel[i])
						{
							i++;
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
					main->phone_->m_BatteryStatus.batteryType = BATTERY_DC;
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
	}
	return isBatteryStatus;  //123
}

bool DecodeCallIDPackage(BYTE *buf, CALLID_INFO *pcallid)
{
	bool	flag = false;

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
	extern void GMute(BOOL isOn);
	Sleep(10);
	int i;
	static BOOL isRingTelCode = FALSE;
	static CALLID_INFO	CallID;
	static UINT8   CallIDbuff[128];
	static int     CallIDLen = 0;

	for (i=0; i<length; i++)
		{		
			unsigned char c = data[i];
			if(c > 'z')
				Dprintf("com1:%x", c);
			if(c == '\n' || c == '\r')
				Dprintf("\r\n");
			
			CMultimediaPhoneDlg* main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
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
		
			switch(c)
			{
				case HUNGON_VALUE://切换到手柄到下面0xC8
		//			((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SWitchPhone3G(TRUE);
					Dprintf("手柄\r\n");
					main->phone_->Hand(Telephone::TelephoneWarp::HandOn);
					main->phone_->Free(Telephone::TelephoneWarp::FreeOff);						
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 0);
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
								if(main->m_nTELRigster >= TELRIGSTER_TD)
								Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
								
							}
							else//没按过免提直接挂断
							{	
								Dprintf("手柄挂机\r\n");
								if(main->m_nTELRigster >= TELRIGSTER_TD)
								Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
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
						
				case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6:
				case 0xB7: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC:
 					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
					break;
			
				case 0xC8:   //Handset摘机
					gIsHandSet = TRUE;
					main->phone_->Hand(Telephone::TelephoneWarp::HandOn);

					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
					break;

				case 0xC9:	//Handfree 摘机
					gIsHandSet = FALSE;
					main->phone_->Hand(Telephone::TelephoneWarp::HandOff);
					main->phone_->Free(Telephone::TelephoneWarp::FreeOn);	

					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )										
					Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
					break;

				case 0xCA:
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
					break;

				case 0xCB:
					if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
					{	
						main->phone_->Hand(Telephone::TelephoneWarp::HandOn);
						main->phone_->Free(Telephone::TelephoneWarp::FreeOff);						
						Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);

					}
					break;

				case 1: case 2: case 3: case 4: case 5: case 6:	
				case 7: case 8: case 9: 
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SPEEDDIAL, c, 0);
					break;
			
				case 0xBD:
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_REDIAL, 0, 0);
					break;
	
				default:
					break;
			}
		}

}

void ParseTelephoneData_(unsigned char const* const data, unsigned int const length)
{	
	extern void GMute(BOOL isOn);

	Sleep(10);
	int i;
	static BOOL isRingTelCode = FALSE;
	static CALLID_INFO	CallID;
	static UINT8   CallIDbuff[128];
	static int     CallIDLen = 0;
	
	for (i=0; i<length; i++)
	{		
		unsigned char c = data[i];
		Dprintf("com3:%x", c);
		if(c == '\n' || c == '\r')
			Dprintf("\r\n");
		
		CMultimediaPhoneDlg *main = (CMultimediaPhoneDlg*)theApp.m_pMainWnd;
	
		switch(c)
		{
		
		case HAND_ERROR:
			break;

		case HUNGON_VALUE://切换到手柄到下面0xC8
			//((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->phone_->SWitchPhone3G(false);
			//	main->phone_->Hand(Telephone::TelephoneWarp::HandOn);
			//	main->phone_->Free(Telephone::TelephoneWarp::FreeOff);
			//	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 3, 0);
			if ( main->m_phoneLine[2].pFSM->getCurrentState() == CMultimediaPhoneDlg::pstnsHangOff)
			{
				main->phone_->PSTNHangoff();
			}
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
					
				}
				else//没按过免提，也没按过手柄 = 开机
				{	
					gIsHandSet = FALSE;   //lxz20100108
					Dprintf("免提\r\n");
					main->phone_->Free(Telephone::TelephoneWarp::FreeOn);
					GMute(FALSE);//打开speeker
			//		Telephone::TelephoneWarp::GetTelephoneWarp()->PSTNHangoff();
					if(main->m_nTELRigster >= TELRIGSTER_TD)
					Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
					PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGON, 1, 0);					
				}

			}
			break;

		case RING_VALUE:
			{
				keybd_event(VK_F9, 0, 0, 0);
			}		
 			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, 3);
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
			gIsHandSet = TRUE;
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
				Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
			break;

		case 0xC9:	//Handfree 摘机
			gIsHandSet = FALSE;
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )					
				Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
			break;
			
		case 0xCA:
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
				Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(true);
			break;

		case 0xCB:
			if(((CMultimediaPhoneDlg*)(theApp.m_pMainWnd))->m_nTELRigster >= TELRIGSTER_TD )
				Telephone::TelephoneWarp::GetTelephoneWarp()->HandFree(false);
			break;
		
		case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6:
		case 0xB7: case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_KEYCODE, g_tel_code[c-0xB0], 0);
			break;

		case 4: case 5: case 6:	
		case 7: case 8: case 9:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_SPEEDDIAL, c, 0);
			break;

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
			PostMessage(theApp.m_pMainWnd->m_hWnd,WM_TEL_HUNGOFF,0, 0);
			break;

		case 0xBD:
			PostMessage(theApp.m_pMainWnd->m_hWnd, WM_REDIAL, 0, 0);
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
	m_BatteryStatus.batteryType = BATTERY_1;
	m_BatteryStatus.isCharge = 0;

	m_pRS232 = new Util::RS232();
	m_pRS232->OpenPort();
	m_pRS232->SetReadFunc(ParseTelephoneData);

	m_pRS232_ = new Util::RS232();
 	m_pRS232_->OpenPort(3);
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

//	AfxBeginThread(TelephoneThread, this, THREAD_PRIORITY_NORMAL); 
	

}

void TelephoneWarp::Update(std::string const& data)
{		   
	ParseATD(data);
	ParseCLCC(data);
	ParseCLIP(data);
	ParseDACTI(data);
	ParseCSQ(data);
	ParseCCWA(data);
}

void TelephoneWarp::ParseATD(std::string const& data)
{
	if (data.find("NO DIALTONE") != std::string::npos)
	{
		NoDialTone();
	}
	else if (data.find("BUSY") != std::string::npos)
	{
		Busy();
	}
	else if (data.find("NO ANSWER") != std::string::npos)
	{
		NoAnswer();	
	}
	else if (data.find("NO CARRIER") != std::string::npos)
	{
		NoCarrier();	
	}
	else if (data.find("CONNECT") != std::string::npos)
	{
		Connect();
	}
	else if (data.find("NW CONGESTION") != std::string::npos)
	{
		Congestion();
	}
	else if (data.find("ERROR ODB") != std::string::npos)
	{
		Odb();	
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

		size_t t = s.find("+CLCC");
		if (t != std::string::npos)
		{
			s = s.substr(t);
		}
		else
		{
			return ;
		}
		
		while (s.find("+CLCC") != std::string::npos)
		{	
			int line  ;
			CString c ;
			s = s.substr(7);
			
			c = Util::StringOp::ToCString(s);
			line = atoi(c);
			
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
				Ring(line) ;//incoming
			}
			else if( s.substr(0,3) == "1,5")
			{
				Waiting(line) ;//waiting
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

void TelephoneWarp::ParseDACTI(std::string const& data)
{
	unsigned int netType = 0xF;
	CString c ;
	char *ans= (char *)(data.c_str());
	if (strstr(ans, "^DACTI: ") != NULL)
	{
		if(strstr(ans, "OK\r\n") != NULL)   
		{
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
 		}

		if(netType != 0xF)
		{
			PhoneNettype(netType);
		}
	}


}
void TelephoneWarp::ParseCSQ(std::string const& data)
{	
	CString c ;
	if ( data.find("+CSQ: ") != std::string::npos)
	{
		UINT static signalOrg = 0;
		int result = 0xFF;
		char *ans= (char *)(data.c_str());
		
		if(strstr(ans, "OK\r\n") != NULL)   
		{
			char* p = strstr(ans, "+CSQ: ");//+CSQ: 129,99
			if (p != NULL)
			{
				p += 6;
				std::string st = p;
				c = Util::StringOp::ToCString(st);
				result = atoi(c);
				if (result == 99)
				{
					result = 0;
				}
				else
				{
					result = result - 100 - 116 + 113;
					result = (result + 1) / 2;
					if (result < 8)
					{
						result = 1;
					}
					else
					{
						result = (result - 8) / 6 + 2;
					}

					if (result > 5)
					{
						result = 5;
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
	m_pAT->PhoneDialTone(isOn, tone);
}
//////////////////////////////////////////////////////////////////////////
bool TelephoneWarp::Dial(char* number, BOOL isVideo)
{
	TEL_NUM num;
	memset(&num, 0, sizeof(TEL_NUM));
	strcpy(num.NUM, number);
	num.TYPE = isVideo;
	
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
	m_nFirstCall++;
	static CALLID_INFO CallID;
	memset(&CallID, 0, sizeof(CALLID_INFO));
	strcpy(CallID.number, num);
	if (1 == m_nFirstCall)
	{
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_CALLIDEND, (WPARAM)&CallID, line);
	}

}

void TelephoneWarp::Ring(int const line)//来电振铃
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_RING, 0, line);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMRING, line);
}

void TelephoneWarp::Busy(void)//对方忙
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMBUY, 0);
}

void TelephoneWarp::Connected(int const line) //通话联通状态
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONNECTED, line);
}

void TelephoneWarp::Connect(void)//对方接听
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONNECT, 0);
}

void TelephoneWarp::Held(int const line)//保持状态
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMHELD, line);
}
void TelephoneWarp::Dialing(int const line)//正在拨打状态
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMDIALING, line);
}
void TelephoneWarp::Alerting(int const line)//对方振铃状态
{
	PhoneDialTone(0, NULL);
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMALERTING, line);

}
void TelephoneWarp::Waiting(int const line)
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMWAITING, line);
}

void TelephoneWarp::NoDialTone(void)
{
	
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNODIALTONE, 0);

}
void TelephoneWarp::NoAnswer(void)//无应答
{
	PhoneDialTone(1, "hangup");
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNOANSWER, 0);
}
void TelephoneWarp::NoCarrier(void)//连接不能被建立
{
	PhoneDialTone(1, "hangup");
//	nState = stHangupPhone;
	Hangup_();
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMNOCARRIER, 0);
}
void TelephoneWarp::Congestion(void)//网络拥塞
{
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMCONGESTION, 0);
}
void TelephoneWarp::OppHangup(void)//对方挂机
{
	PhoneDialTone(1, "hangup");
//	nState = stHangupPhone;
	Hangup_();
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMOPPHUNGUP, 0);
}
void TelephoneWarp::Odb(void)//呼叫限制
{
	PhoneDialTone(1, "hangup");
	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_STATUS, TEL_FROMODB, 0);
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
//	PostMessage(theApp.m_pMainWnd->m_hWnd, WM_TEL_HUNGOFF, 0, 0);

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
	m_HandSet = (HandSet) hand;
}

int TelephoneWarp::Hand()
{
	return m_HandSet;
}

void TelephoneWarp::Free(int free )
{
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
	
	if(wcslen(filename) == 0)
	{
		memset(gRingFilename, 0, sizeof(TCHAR)*64);
		StopRing();
		return;
	}
	
	extern void GMute(BOOL isOn);
	GMute(FALSE);

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

			waveOutSetVolume(NULL, g_RingSound);
			
		//	ipBasAudio->put_Volume(g_RingSound); 
		}
	}
	m_bStartRing = TRUE;

}

void TelephoneWarp::StopRing(bool releaseSrc)
{
	/*
	sndPlaySound(NULL, 0);
	return;
	*/

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
	unsigned char data[] = {0xC2, 0xC1};
	m_pRS232_->WritePort(&data[is3G], 1);
	return true;
}

void TelephoneWarp::PSTNHangoff()
{	
	Dprintf("PSTNHangoff \r\n");
	unsigned char c = 0x8b;
	m_pRS232_->WritePort(&c, 1);
}

void TelephoneWarp::PSTNHangoff_()
{
	Dprintf("PSTNHangoff_ \r\n");
	unsigned char c = 0x83;
	m_pRS232_->WritePort(&c, 1);
	
}

void TelephoneWarp::PSTNHangOn()
{
	Dprintf("PSTNHangOn \r\n");
	unsigned char c = 0x82;
	m_pRS232_->WritePort(&c, 1);
}