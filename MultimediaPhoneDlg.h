// MultimediaPhoneDlg.h : header file
//

#if !defined(AFX_MULTIMEDIAPHONEDLG_H__3E2355EC_A794_4663_A7BF_14ACD17164FB__INCLUDED_)
#define AFX_MULTIMEDIAPHONEDLG_H__3E2355EC_A794_4663_A7BF_14ACD17164FB__INCLUDED_


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#pragma warning(disable: 4786)
#include <map>

#include "View/MainDlg.h"
#include "./Control/InputDlg.h"
#include "./Control/CeBtnST.h"
#include "./Control/SoftKey.h"
#include "View/ContactDlg.h"
#include "View/ContactNewDlg.h"
#include "View/ContactGroupDlg.h"
#include "View/Player.h"
#include "./View/PlayerDlg.h"	
#include "View/telephonedlg.h"
#include "./View/MainVideoDlg.h"
#include "View/ContactInfoDlg.h"
#include "View/SoundDlg.h"
#include "./Logical/Telephone.h"
#include "Util/fsm.h"
#include "View/SettingDlg.h"
#include "Logical/Phone.h"
#include "View/LunarderDlg.h"
#include "View/InformationDlg.h"
#include "View/deletetipdlg.h"
#include "View/TestDlg.h"
#include "View/CWarningDlg.h"

//#define WM_CHANGELANGUAGE (WM_USER+300)
const CString csFlashPath = _T("\\FlashDrv\\mmp.db");
const CString BackupPath=_T("\\FlashDrv\\backup.db");
const CString cshivePath=_T("\\Hive\\mmp.db");
const CString csFlashRingPath = _T("\\FlashDrv\\MY_RING\\");
const CString csHiveTipPath = _T("\\Hive\\MY_TIP\\");
const CString csFlashRecordPath = _T("\\FlashDrv\\MY_RECORD\\");
const CString csStorageCardRecordPath = _T("\\StorageCard\\MY_RECORD\\");
const std::string ssFlashRecordPath = "\\FlashDrv\\MY_RECORD\\";
const std::string ssStorageCardRecordPath = "\\StorageCard\\MY_RECORD\\";
const CString ssFlashPath=_T("\\FlashDrv\\");
const CString ssHivePath=_T("\\hive\\");

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

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg dialog
class CMultimediaPhoneDlg : public CDialog
{
public:
	//Logical::Phone* m_pPhone;
	Phone* phone_;
	SIMDevice* simdevice_;
	//Player*	   player_;
	//CMainVideoDlg	*mainVideoDlg_;
	//CPlayerDlg	*playerDlg_; 

	CMainDlg* m_pMainDlg;
	CContactDlg* m_pContactDlg;
	//增加名片对话框
	CContactNewDlg* m_pContactNewDlg;
	CContactGroupDlg* m_pContactGroupDlg;
	CTelephoneDlg* m_pTelephoneDlg;
	CDeleteTipDlg* m_pDeleteTipDlg;

	CContactInfoDlg* m_pContactInfoDlg;
	CSoundDlg* m_pSoundDlg;
	CSettingDlg* m_pSettingDlg;
    CInformationDlg* m_pInformationdlg;
	
	//测试程序
	CTestDlg	  *m_pTestDlg ;
	CCWarningDlg  *m_pCCWarningDlg;  //add by yy 20111028

// 	CLunarderDlg	*m_mainLunarderDlg1_;
	BOOL			m_bIsHaveUSB;
	BOOL			m_bIsSD;
	BOOL            m_bIsDB;
	bool            m_isTaking;
	BOOL			m_bUpgrade;
	CString			m_keyCode;
	///////USB同步时，阻止其它对话框弹出的标志
	bool         m_hangon;
	MANUINFO    m_manuinfo;
	HANDLE hEvent[2];
	HANDLE hthreadID;
	void SetSkinStyle();
	void SendOutEvnet(WPARAM w = 0, LPARAM l = 0);
	void SetScreenSaveTimer();
	void KillScreenSaveTimer();
	void HideComboBoxDropDown(void);
	void CloseBacklight();
	bool IsFindFile(CString path);
    void WriteTOFlag(char * s);
	void UpdateShowList();
	
	//在屏保状态下屏蔽按键
	bool IsKeyShield(UINT message);

	
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

	ICONKEYSTRUCT softkeyval_[9];
public:
	CDialog *GetPanel(int nID){return panels_[nID];}
public:
	void SwitchPanel_(int panelId);
protected:
	// Generated message map functions
	//{{AFX_MSG(CMultimediaPhoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg LRESULT OnChangeLanguage();
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
	afx_msg void OnMainSoftKey(WPARAM w, LPARAM l);
	afx_msg void OnEvent(WPARAM w, LPARAM l);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CCEButtonST m_btnTime;
	CCEButtonST m_btnIME;
	CCEButtonST m_btnLine;
	CCEButtonST m_btnMain;
	CCEButtonST	m_btnContact;
	CCEButtonST	m_btnContactInfo;
	CCEButtonST	m_btnSound;
	CCEButtonST	m_btnInformation;
	CCEButtonST	m_btnSetting;
	CCEIconSoftKey	m_StatusKey;
    std::string m_sListSearchFilter;
	std::map<int, CCEButtonST*> buttons_;
	CCEButtonST* m_pBtnCurrent;
	void SetButtonDefaultColor(CCEButtonST* button);
	void SetButtonSelectedColor(CCEButtonST* button);
	
public:
	void ChangeLanguage();
	void EnableLine(BOOL bEnable = TRUE);
	unsigned int m_uiKeyTimer;
	unsigned int m_uiRingTimer;
	unsigned int m_uiCallIdTimer;
	BOOL m_bComeRing;
	Util::FSM* m_pFSM;
	void panelToLeft();
	void panelToRight();
    void ScreenPS();
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

	enum PopTip
	{  
		UsbINSERTTIP,//电话功能			
		UsbINSERTTIP_,	// 拔掉重启		
	    START_UP_TIP,//拷贝数据库重启
		UsbRemoveTip//系统重新启动
	};

	void AddFSMRules(void);
	void PopUsbTIP(PopTip popType);
    void setIsTaking(bool flag);
	bool getIsTaking();

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

#endif // !defined(AFX_MULTIMEDIAPHONEDLG_H__3E2355EC_A794_4663_A7BF_14ACD17164FB__INCLUDED_)
