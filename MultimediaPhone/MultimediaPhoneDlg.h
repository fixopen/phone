
// MultimediaPhoneDlg.h : ͷ�ļ�
//

#pragma once

#include <map>

#include "Control/InputDlg.h"
#include "Control/CeBtnST.h"
#include "Control/SoftKey.h"
#include "Control/MJPGStatic.h"
#include "View/MainDlg.h"
#include "View/ContactNewDlg.h"
#include "View/ContactGroupDlg.h"
#include "View/Player.h"
#include "View/PlayerDlg.h"	
#include "View/telephonedlg.h"
#include "View/MainVideoDlg.h"
#include "View/DeleteTipDlg.h"
#include "View/ContactInfoDlg.h"
#include "View/ContactDlg.h"
#include "View/SoundDlg.h"
#include "View/SettingDlg.h"
#include "View/LunarderDlg.h"
#include "View/NotebookDlg.h"
#include "View/WebDialog.h"
#include "View/AlarmShowDlg.h"
#include "View/HuangliDlg.h"
#include "View/NetStatusDlg.h"
#include "View/WarningNoFlashDlg.h"
#include "Logical/Telephone.h"
#include "Logical/Phone.h"
#include "Util/fsm.h"

using namespace View;
using namespace Control;
using namespace Logical;
using namespace Util;

//////SCL //////////////////
#define CTL_CODE(DeviceType, Function, Method, Access) (((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))
#define FILE_DEVICE_STREAMS 0x0000001e

//method
#define METHOD_BUFFERED 0
#define METHOD_IN_DIRECT 1
#define METHOD_OUT_DIRECT 2
#define METHOD_NEITHER 3

//access
#define FILE_ANY_ACCESS 0
#define FILE_READ_ACCESS (0x0001)
#define FILE_WRITE_ACCESS (0x0002)

#define SCL_IOCODE_BASE				2050

#define SCL_SYSTEM_RESET		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x1,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //reset system
#define SCL_ENABLE_WATCHDOG		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x2,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //enable watch dog
#define	SCL_ADJUST_BACKLIGHT		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x5,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	   //LCD �ƿ���	
#define SCL_GET_PRODUCT_INFO		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x7,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	   //��Ʒ��Ϣ
#define SCL_ADSL_POWERCTL		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xA,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //ADSL ��Դ����
#define SCL_ENCRYGPT_VERIFY		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xE,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	

#define SIM_IOCODE_BASE				    2050
#define SIM_IO_OFFSET				    100
#define	SIM_RESET				        CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x40,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	//cmdreset	
#define SIM_GETSIMCARDINFO				CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x45,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	//cmdgetinfo	                       
#define SIM_WRITE						CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x48,METHOD_IN_DIRECT,FILE_ANY_ACCESS)                       

typedef struct ManuInfo
{
    BYTE deviceID[32];
    BYTE manuInfo[32];
    BYTE flashID[128];
    BYTE serialNum[32];
    BYTE model[32];
    BYTE date[32];	
    BYTE osVer[32];
    BYTE bootVer[4];
    BYTE appVer[32];
    BYTE hardVer[32];
    BYTE MD5Key[64];
    BYTE bootCkSum[16];
    BYTE res[12];
} MANUINFO, *PMANUINFO;

typedef struct BacklightCommand {
    DWORD  bkCmd;			//��������
    DWORD  bkLevel;			//������Ƽ���			
} BACKLIGHTCMD;

typedef struct EncryptCommand {
    DWORD  bkCmd;			//��������
    DWORD  bkRandom;			//�����		
} ENCRYCMD;
//////end SCL///////////////


const CString csFlashRecordPath = _T("\\FlashDrv\\MY_RECORD\\");
const CString csStorageCardRecordPath = _T("\\StorageCard\\MY_RECORD\\");
const std::string ssFlashRecordPath = "\\FlashDrv\\MY_RECORD\\";
const std::string ssStorageCardRecordPath = "\\StorageCard\\MY_RECORD\\";
const CString csStarageCard = _T("\\StorageCard\\");
const std::string ssStarageCard = "\\StorageCard\\";
const CString csUsbDisk = _T("\\UsbDisk\\");

extern CString photoPath;
extern CString musicPath;
extern CString videoPath;
extern CString ringPath;
extern CString adPath;
extern CString recordPath;
extern CString notePath;
extern CString webPath;

// CMultimediaPhoneDlg �Ի���
class CMultimediaPhoneDlg : public CDialog
{
public:
    int n_StyleMain;
    unsigned int n_bklightcount;
    HANDLE m_hPort;
    int m_bklightvalue;
    int m_nBackLightStatus;			//0 �ر�     7 ��
    MANUINFO m_manuinfo;

    //��ʼ��SCL
    void InitSCLPort();
    //��ȡ�豸�ĳ�����Ϣ
    BOOL GetManuInfo(char *pBuffer, int length);
    //����watchdog
    BOOL SetWatchDog();
    //����ADSL��Դ����
    BOOL SetADSLPower(BOOL isOn);
    //����ϵͳ����
    BOOL SetReStart();
    //�رմ򿪱���   
    // 7    ��
    // 0    ��
    BOOL SetBackLight(int isOn);
    //��ȡ�����ñ����ֵ
    UINT32 GetBackLightValue();
    //��ȡ����Ĺ���ֵ
    UINT32 GetRawBackLightValue();
    //��֤����
    BOOL EnryRun();
    //��ȡ��������
    UINT32 GetEnry();

public:
    CMJPGStatic m_MJPGList;

    /*Logical::*/Phone* m_pPhone;
    Phone* phone_;
    SIMDevice* simdevice_;

    Player* playervideo_;
    Player* playeraudio_;
    Player* playerimage_;
    //CMainVideoDlg* mainVideoDlg_;
    //CPlayerDlg* playerDlg_;

    CMainDlg* m_pMainDlg;
    CContactDlg* m_pContactDlg;
    CContactNewDlg* m_pContactNewDlg;
    CContactGroupDlg* m_pContactGroupDlg;
    CTelephoneDlg* m_pTelephoneDlg;
    CDeleteTipDlg* m_pDeleteTipDlg;

    CContactInfoDlg* m_pContactInfoDlg;
    CSoundDlg* m_pSoundDlg;
    CSettingDlg* m_pSettingDlg;
    CNotebookDlg* m_pNotebookDlg;
    CAlarmShowDlg* m_AlarmShowDlg;
    CWarningNoFlashDlg* m_pWarningNoFlashDlg;
    CNetStatusDlg* m_pNetStatusDlg;

    //CWebDialog* m_pWebDialog;

    //CLunarderDlg* m_mainLunarderDlg1_;
    //CHuangliDlg* m_pHuangLiDlg_;

    BOOL m_bIsHaveUSB;
    BOOL m_bIsSD;
    BOOL m_bIsHungOn;
    BOOL GetPhoneHungOn() {
        return m_bIsHungOn;
    }

    void SetSkinStyle();
    LRESULT SendOutEvent(WPARAM w = 0, LPARAM l = 0);
    void SetScreenSaveTimer();
    void KillScreenSaveTimer();

    void SetControlBackLightTimer();
    BOOL SetNightControlBackLightTimer();
    BOOL ReStoreBackLight();
    BOOL CancelBalckLightSaveTimer();
    int CheckInternetStatus();

    afx_msg void OnTimer(UINT nIDEvent);

    //alarm setting
    int m_nAlarmID;
    void SetAlarmList();
    BOOL m_bNetOkStatus;
    int m_bNetType;
    void SetNetTelStatus();
// ����
public:
	CMultimediaPhoneDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMultimediaPhoneDlg();	// ��׼��������

// �Ի�������
	enum { IDD = IDD_MULTIMEDIAPHONE_DIALOG };

    virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
    std::map<int, CDialog*> panels_;
    CDialog* currentPanel_;
    UINT currentPanelID_;
    UINT oldcurrentPanelID_;
public:
    CDialog* GetPanel(int nID) {
        return panels_[nID];
    }
    void SwitchPanel_(int panelId);

    afx_msg LRESULT OnMainSoftKey(WPARAM w, LPARAM l);
protected:
    // Generated message map functions
    //{{AFX_MSG(CMultimediaPhoneDlg)
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    afx_msg void OnButtonTime();
    afx_msg void OnButtonIME();
    afx_msg void OnButtonLine();
    afx_msg void OnButtonMain();
    afx_msg void OnButtonContact();
    afx_msg void OnButtonContactinfo();
    afx_msg void OnButtonSound();
    afx_msg void OnButtonInformation();
    afx_msg void OnButtonSetting();
    afx_msg LRESULT OnEvent(WPARAM w, LPARAM l);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnDeviceChange(WPARAM w, LPARAM l);
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);

	DECLARE_MESSAGE_MAP()
private:
    std::map<int, CCEButtonST*> buttons_;
    CCEButtonST* m_pBtnCurrent;
    void SetButtonDefaultColor(CCEButtonST* button);
    void SetButtonSelectedColor(CCEButtonST* button);

public:
    void EnableLine(BOOL bEnable = TRUE);
    unsigned int m_uiKeyTimer;
    unsigned int m_uiRingTimer;
    Util::FSM* m_pFSM;

    enum TelephoneState
    {
        tsHangOff,
        tsHangOn,
        tsKey,
        tsConnect,
        tsRing,
        tsCallID,
    };

    enum TelephoneEvent
    {
        teHangOff,
        teHangOn,
        teKey,
        teConnect,
        teRing,
        teCallID,
    };


    void AddFSMRules(void);

    static void HangOff(void* param);
    static void HangOn(void* param);
    static void Key(void* param);
    static void SubKey(void* param);
    static void Connect(void* param);
    static void Ring(void* param);
    static void CallID(void* param);
};

void PickupFunc();
void HandsfreeFunc();
void HangupFunc();
void KeypadDialFunc(unsigned char const& keyCode);
void ConnectFunc();
void RingInFunc(Data::TelephoneNumber number);
void RemoteHangupFunc();
void TimeoutFunc();
void ErrorFunc();
