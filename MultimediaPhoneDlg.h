// MultimediaPhoneDlg.h : header file
//

#if !defined(AFX_MULTIMEDIAPHONEDLG_H__3E2355EC_A794_4663_A7BF_14ACD17164FB__INCLUDED_)
#define AFX_MULTIMEDIAPHONEDLG_H__3E2355EC_A794_4663_A7BF_14ACD17164FB__INCLUDED_


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#pragma warning(disable: 4786)
#include <map>

#include   <imaging.h> //add 20100920

#include "View/MainDlg.h"
#include "./Control/InputDlg.h"
#include "./Control/CeBtnST.h"
#include "./Control/SoftKey.h"
#include "View/ContactNewDlg.h"
#include "View/ContactGroupDlg.h"
#include "View/Player.h"
#include "./View/PlayerDlg.h"	
#include "View/telephonedlg.h"
#include "./View/MainVideoDlg.h"
#include "View/DeleteTipDlg.h"
#include "View/ContactInfoDlg.h"
#include "View/ContactDlg.h"
#include "View/SoundDlg.h"
#include "./Logical/Telephone.h"
#include "Util/fsm.h"
#include "View/SettingDlg.h"
#include "Logical/Phone.h"
#include "View/LunarderDlg.h"
#include "View/NotebookDlg.h"
#include "View/WebDialog.h"
#include "View/AlarmShowDlg.h"
#include "control/MJPGStatic.h"
#include "View/HuangliDlg.h"
#include "View/NetStatusDlg.h"
#include "View/WarningNoFlashDlg.h"
#include "View/PasswordDlg.h"
#include "./util/SMSWarp.h"

#include "View/MMSDlg.h"
#include "View/3GSMSListDlg.h"
#include "View/SMSDraftDlg.h"
#include "View/ShiftFile.h"
#include "View/CSMSSettingDlg.h"
#include "View/CMMSSettingDlg.h"
#include "View/TelephoneDialDlg.h"
#include "View/TelephoneRingDlg.h"
#include "Data/MessageSet.h"
#include "Data/MmsMessage.h"

#include "View/TipDlg.h"//��ʾ��

#include "View/STKDlg.h"

#include "Control/Cache.h"
/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg dialog

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
#define	SCL_ADJUST_BACKLIGHT	CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x5,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	   //LCD �ƿ���	
#define SCL_GET_PRODUCT_INFO	CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x7,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	   //��Ʒ��Ϣ
#define SCL_ADSL_POWERCTL		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xA,METHOD_IN_DIRECT,FILE_ANY_ACCESS) //ADSL ��Դ����
#define SCL_ENCRYGPT_VERIFY		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xE,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	

#define SCL_NET_POWERCTL		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0xA,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define SCL_GET_CPU_FREQ		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x8,METHOD_IN_DIRECT,FILE_ANY_ACCESS)
#define SCL_SET_CPU_FREQ		CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x9,METHOD_IN_DIRECT,FILE_ANY_ACCESS)

#define SCL_GET_SleepFlag       CTL_CODE(FILE_DEVICE_STREAMS,SCL_IOCODE_BASE+0x24,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	//get sleep flag

#define SIM_IOCODE_BASE				    2050
#define SIM_IO_OFFSET				    100
#define	SIM_RESET				        CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x40,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	//cmdreset	
#define SIM_GETSIMCARDINFO				CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x45,METHOD_IN_DIRECT,FILE_ANY_ACCESS)	//cmdgetinfo	                       
#define SIM_WRITE						CTL_CODE(FILE_DEVICE_STREAMS,SIM_IOCODE_BASE+SIM_IO_OFFSET+0x48,METHOD_IN_DIRECT,FILE_ANY_ACCESS)                       

extern CRITICAL_SECTION csCom;

typedef struct
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
}MANUINFO,*PMANUINFO;
typedef struct{
	DWORD  bkCmd;			//��������
	DWORD  bkLevel;			//������Ƽ���			
}BACKLIGHTCMD;

typedef struct{
	DWORD  bkCmd;			//��������
	DWORD  bkRandom;			//�����		
}ENCRYCMD;
//////end SCL///////////////


const CString csFlashRecordPath = _T("\\FlashDrv\\MY_RECORD\\");
const CString csStorageCardRecordPath = _T("\\StorageCard\\MY_RECORD\\");
const std::string ssFlashRecordPath = "\\FlashDrv\\MY_RECORD\\";
const std::string ssStorageCardRecordPath = "\\StorageCard\\MY_RECORD\\";
const CString csStarageCard = _T("\\StorageCard\\");
const std::string ssStarageCard = "\\StorageCard\\";
const CString csUsbDisk = _T("\\UsbDisk\\");
enum TelTDStatus{TELRIGSTER_UN,TELRIGSTER_FAILED, TELRIGSTER_TD, TELRIGSTER_DIALING, TELRIGSTER_DIALED, TELRIGSTER_REG, TEL_UNNET};    //�绰δ����   //�绰������   //�绰��ע��   //������

enum NetStatus{
	netUnRegister,
	netSearching,
	netRegistered,
	netReject,
	netUnknown,
	netRamble,
};

struct DIAL_APN
{
	int apn_type;
	char dialnumber[24];
	char proxy[32];
	char http[64];
}; 

#define  SEARCH_NET_TIMER 10


class CMultimediaPhoneDlg : public CDialog
{
public:

	//
	//
	void UnSetPreADSLDial();
	void SetPreADSLDial();

	BOOL m_bIsCloseLCD;
	BOOL IsCloseLCD(BOOL flag);
	BOOL IsCloseNet(BOOL flag);
	BOOL IsReduceRreq(BOOL flag);
	BOOL IsNightBackTime();
	
	BOOL IsMediaPlay();
	void IsSendMessage(BOOL flag);
	void BatteryLow();
	void SelMMSDataBase();

	Data::MmsMessage *m_MmsMsg;
	boost::shared_ptr<Data::MessageSet> m_pMessageSet;
	boost::shared_ptr<Data::MessageSet> m_pTempMsgSet;
	std::string m_sMobileNumber;
	void ParseSmil(CString FilePath, MMS::MMSWarp::MMS_SubmitRequest &r);
	int        m_isDialTimeout;
	DIAL_APN    m_nAPN;
	BOOL        m_bIsDial;   //�Ƿ���Ҫ����
	BOOL        GetIs3GDialed();
	void        SetAPN(int apn);
	TelTDStatus m_nTELRigster;
	void		BreakNetwork();
	BOOL        IsConnectNet();
	BOOL        IsUnConnectNet();
	//�������
	//int type 0	����
	//		   1	����ͨ
	//		   2	����
	void SMSSpecRing(int type, CString s);
	void doWithSMS(WPARAM wParam, LPARAM lParam);
	void SearchNetInit();
	BOOL doSerachrTDNet(BOOL isSleepStart);    //�ֻ�����
	void doRegisterTel();	  //����ע������
	void doWithDownLoad();    //����һ��������ȥ����
	void Net3GHungOff();
	void MMSReciveTip(Data::MmsMessage *mms);
	
	Util::ComWarp *m_pComWarp7;//com7

	Util::ComWarp* m_pComWarp1;
	Util::ATCommandWarp* m_pATCommandWarp1;
	Telephone::TelephoneWarp* phone_;
	Util::ComWarp* m_pComWarp2;
	Util::ATCommandWarp* m_pATCommandWarp2;
	SMS::SMSWarp* m_pSMSWarp;
	BOOL m_bATComm;

	//����VP����ͨ��
	Util::ComWarp* m_pVideoComWarp;
	
	int          n_StyleMain;
	unsigned int n_bklightcount; 
	HANDLE  m_hPort;
	int     m_nBackLightStatus;			//0 �ر�     7 ��
	int		m_nBackLightValue;
	MANUINFO	m_manuinfo;

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
	BOOL SetBackLight_(int level);
	//��ȡ�����ñ����ֵ
	UINT32 GetBackLightValue();
	//��ȡ����Ĺ���ֵ
	UINT32 GetRawBackLightValue();
	//��֤����
	BOOL EnryRun();
	//��ȡ��������
	UINT32 GetEnry();

//add by qi 2009_07_22
public:
	void GetDataFromContact(std::vector<Util::ATCommandWarp::SIM_FORMAT> &vsim);
	void SaveDataToContact(const std::vector<Util::ATCommandWarp::SIM_FORMAT> vsim);
	void InsertSimToContact(const std::vector<Util::ATCommandWarp::SIM_FORMAT> vsim);//��sim����绰���뵽Contact
	void RemoveSIMGroup();
	std::string GetAlias(std::string name);

	
//	void AddIcon(CString icon,bool bjudge = true);//���ͼ��
	void AddIcon(CString icon,CWnd *pcwnd,bool bjudge = true);//���ͼ��
	void PopbackIcon(CWnd *p = NULL);//�Ƴ�ͼ��
	void PopIcon(CWnd *c);//���ش���,�ͷ�ͼ��

	void Desktop();//����
	void AddDesktopBtn();//
	bool GetSimStatus();//
	void VK_F(WPARAM w);
	void AddAudio(bool badd);
	std::string GetName(std::string number);//�������

//	std::vector<CString>	m_vIcon ; //����ÿ�������ͼ��
	std::vector<std::pair<CWnd*,CString> >	m_vIcon ; //����ÿ�������ͼ��
	CWnd *GetIconCurrentWnd();

private :
	int m_nSIMID ;
	std::vector<CDialog* >		m_vAllCwnd;//���д���
public:
	CMJPGStatic			m_MJPGList;
	bool m_bInsertSim;//�Ƿ����SIM��


	//Logical::Phone* m_pPhone;

//	Phone* phone_;

//	SIMDevice *simdevice_;

	Player*	   playervideo_;
	Player*	   playeraudio_;
	Player*	   playerimage_;
	//CMainVideoDlg	*mainVideoDlg_;
	//CPlayerDlg	*playerDlg_; 

	CPasswordDlg		*m_pPasswordDlg;
	CMainDlg			*m_pMainDlg;
	CContactDlg			*m_pContactDlg;
	CContactNewDlg		*m_pContactNewDlg;
	CContactGroupDlg	*m_pContactGroupDlg;
	CTelephoneDlg		*m_pTelephoneDlg;
	CDeleteTipDlg		*m_pDeleteTipDlg;
	CNetStatusDlg		*m_pNetStatusDlg;

	CContactInfoDlg		*m_pContactInfoDlg;
	CSoundDlg			*m_pSoundDlg;
	CSettingDlg			*m_pSettingDlg;
	CNotebookDlg		*m_pNotebookDlg;
	CAlarmShowDlg		*m_AlarmShowDlg;
	CWarningNoFlashDlg	*m_pWarningNoFlashDlg;

	CMMSDlg				*m_pMMSDlg ;//д���Ž���
	C3GSMSListDlg		*m_pSMSListDlg;//�ռ���
	CDraftDlg			*m_pDraftDlg;//�ݸ���
	CSMSSettingDlg		*m_pSmsSettingDlg;//����Ϣ����
	CMMSSettingDlg		*m_pMmsSettingDlg;//����Ϣ����
	CTelephoneDialDlg	*m_pTelphoneDialDlg;//�绰����
	CTelephoneRingDlg	*m_pTelphoneRingDlg;//�绰�������
	
	CSTKDlg				*m_pCstkDlg;//stk��ʾ����
	CTipDlg				*m_pTipDlg;//��ʾ��


//	CWebDialog* m_pWebDialog;
//	CLunarderDlg	*m_mainLunarderDlg1_;

//  add by qi 2009_10_19
	CShiftFile	*m_pShiftFileDlg ;

	BOOL			m_bIsHaveUSB;
	BOOL			m_bIsSD;
	BOOL			m_bIsHungOn;
	bool			m_isBattery;
	BOOL			m_isHaveBattery;
	bool			m_bNoDc;//�Ƿ�����Դ

	BOOL  GetPhoneHungOn(){return m_bIsHungOn;}

	void SendOutEvnet(WPARAM w = 0, LPARAM l = 0);
	void SetScreenSaveTimer();
	void KillScreenSaveTimer();

	void SetControlBackLightTimer();
	BOOL SetNightControlBackLightTimer();
	BOOL ReStoreBackLight();
	BOOL CancelBalckLightSaveTimer();
	int  CheckInternetStatus();

	afx_msg void OnTimer(UINT nIDEvent);

	//alarm setting
	int	 m_nAlarmID;
	void SetAlarmList();
	BOOL m_bNetOkStatus;
	int  m_bNetType;
	
// Construction
public:
	CMultimediaPhoneDlg(CWnd* pParent = NULL);	// standard constructor
	~CMultimediaPhoneDlg();	// standard destructor

// Dialog Data
	//{{AFX_DATA(CMultimediaPhoneDlg)
	enum { IDD = IDD_MULTIMEDIAPHONE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultimediaPhoneDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	std::map<int, CDialog*> panels_;
	CDialog* currentPanel_;
	UINT	 currentPanelID_;
	UINT	 oldcurrentPanelID_;

public:
	CDialog *GetPanel(int nID){return panels_[nID];}

public:
	void SwitchPanel_(int panelId);
	afx_msg void OnButtonSetting();
	afx_msg void OnMainSoftKey(WPARAM w, LPARAM l);

protected:
	// Generated message map functions
	//{{AFX_MSG(CMultimediaPhoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnButtonIME();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonSound();
	afx_msg void OnButtonInformation();
	afx_msg void OnEvent(WPARAM w, LPARAM l);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDeviceChange(WPARAM w, LPARAM l);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnRasErrorCode(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()

private:

	std::map<int, CCEButtonST*> buttons_;
	CCEButtonST* m_pBtnCurrent;
	void SetButtonSelectedColor(CCEButtonST* button);
	
public:
	static void SetButtonDefaultColor(CCEButtonST* button);
	void EnableLine(BOOL bEnable = TRUE);
	unsigned int	m_uiKeyTimer;
	unsigned int	m_uiRingTimer;
	unsigned int	m_uiPSTNRingTimer;
	Util::FSM*		m_pFSM;
	int				m_nline ;
	int				m_nContactNum;
	bool			m_bSearchNetOver;
	bool			m_bSearchNetWin;
	bool			m_binitOver;//UI��ʼ���Ƿ����
	bool			m_bUnconditonSet;
	bool			m_bAutoSearchNet;//�Զ�����
	
	bool			OpenAllPort();//�Ƿ����еĿ�ȫ��
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
	

	enum phone3gState
	{
		p3gsHangOff,
		p3gsHangOn,
		p3gsKey,
		p3gsDialing,
		p3gsAltering,
		p3gsRing,
		p3gsConnected,		
		p3gsHold,
		p3gsOppOff,	
		p3gsPstn,
		
	};

	enum phone3gEvent
	{
		p3geHangOff,
		p3geHangOn,
		p3geKey,
		p3geDialing,
		p3geAltering,
		p3geCallID,
		p3geRing,
		p3geConnected,

		p3geActive,
		p3geHold,

		p3geNoDialtone,
		p3geODB,
		p3geBusy,
		p3geNoanswer,
		p3geNoNet,
		p3geCarrier,
		p3geOppOff,
			
	};

	enum pstnstate
	{
		pstnsHangOff,
		pstns3gRing,
		pstnsRing,
		pstnsConnected,		
		pstnsHold,
		
	};
	
	enum pstnevent
	{
		pstneHangOff,
		pstneHangOn,
		pstne3gRing,
		pstneRing,
		pstneCallID,
		pstneConnected,
		pstneHold,
		pstneActive,
		
	};

	typedef struct {
		
		enum phonetype{
			phone3g1,
			phone3g2,
			phonepstn,
		};
		
		std::string			contact;//��ϵ��
		std::string			telnum;
		std::string         city;
		std::string         lineStatus;
		std::string         RingFilename ;//�����ļ�

		int					TelStatus;
		UINT				timer;//ʱ��
		UINT				LineStatusTimer;//��·״̬��ʱ��
		UINT				HangoffTimer;//�һ���ʱ��
		UINT				SecondOff;
		UINT				Second;//
		UINT				RingCount;//������
		UINT				iIgnoreRingCount;//����������
		bool				bHasCallID;//�Ƿ�����CALL_ID
		bool				bFirwall;//��·����ǽ
		bool				bRingPlayed;//����·�Ƿ񲥷Ź�����
		bool				b3gReject;//�ܾ�����
		bool				b3gFisrtRing;//�Ƿ��һ������
		phonetype			type;
		Util::FSM*			pFSM;
		Util::Timestamp     starttime;
		
	}phoneline; 
	phoneline m_phoneLine[3];
	
	static void g3HangOff(void* param);
	static void g3HangOn(void* param);
	static void g3Key(void* param);
	static void g3Dialing(void* param);
	static void g3Ring(void* param);
	static void g3CallID(void* param);
	static void g3Contect(void* param);//��ͨ
	static void g3Contected(void* param);//�Ѿ�����
	static void g3SubDial(void* param);
	static void g3Active(void* param);
	static void g3Hold(void* param);

	static void pstn3gRing(void* param);//3g����
	static void pstnRing(void* param);
	static void pstnHangOff(void* param);
	static void pstnHangOn(void* param);
	static void pstnContect(void* param);
	static void pstnHold(void* param);
	static void pstnCallID(void* param);

	void HandleHungOn(WPARAM wParam,LPARAM lParam);//ժ��
	void HandleCallIDEnd(WPARAM wParam ,LPARAM lParam);
	void HandleRing(WPARAM wParam ,LPARAM lParam);
	void HandleHungOff(WPARAM wParam ,LPARAM lParam);

	void  AddFSMRules(void);
	bool  FindIdleLine(void);
	bool  FindActiveLine(void);
	void  Find3gLineNum(int &g3Num);//3g ��Ŀ
	void  ReleasesOneLine(int const line);//�ͷŵ�һ·
	void  Swtich2AnotherLine(void);//�л�
	bool  PstnActive();//pstn��·�Ƿ��ǻ��

	//
	void  ReadSimSMS();//��SIM�ж���Ϣ
	void  RemoveSimSms();//�ѱ��صĸ�SIM����ص�SMS�Ƴ�
	bool  IsDiskFull();//�Ƿ�Flashdrv���̿ռ�����

	//���˯�߱�־
	bool  GetSleepFlag(char &flag);

	static void HangOff(void* param);
	static void HangOn(void* param);
	static void Key(void* param);
	static void SubKey(void* param);
	static void Connect(void* param);
	static void Ring(void* param);
	static void CallID(void* param);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.
void PickupFunc();
void HandsfreeFunc();
void HangupFunc();
void KeypadDialFunc(unsigned char const& keyCode);
void ConnectFunc();
void RingInFunc(Data::TelephoneNumber number);
void RemoteHangupFunc();
void TimeoutFunc();
void ErrorFunc();

#endif // !defined(AFX_MULTIMEDIAPHONEDLG_H__3E2355EC_A794_4663_A7BF_14ACD17164FB__INCLUDED_)
