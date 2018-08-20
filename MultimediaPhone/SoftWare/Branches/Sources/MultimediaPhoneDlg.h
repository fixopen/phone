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
#include "View/ContactNewDlg.h"
#include "View/ContactGroupDlg.h"
#include "View/Player.h"
#include "./View/PlayerDlg.h"	
#include "View/telephonedlg.h"
#include "./View/MainVideoDlg.h"
#include "View/DeleteTipDlg.h"
#include "View/ContactInfoDlg.h"
#include "View/SoundDlg.h"
#include "./Logical/Telephone.h"
#include "Util/fsm.h"
#include "View/SettingDlg.h"
#include "Logical/Phone.h"
#include "View/LunarderDlg.h"
#include "View/NotebookDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMultimediaPhoneDlg dialog

class CMultimediaPhoneDlg : public CDialog
{
public:
	Logical::Phone* m_pPhone;
	Phone* phone_;
	Player*	   player_;
	//CMainVideoDlg	*mainVideoDlg_;
	//CPlayerDlg	*playerDlg_; 

	CMainDlg* m_pMainDlg;
	CContactNewDlg* m_pContactNewDlg;
	CContactGroupDlg* m_pContactGroupDlg;
	CTelephoneDlg* m_pTelephoneDlg;
	CDeleteTipDlg* m_pDeleteTipDlg;

	CContactInfoDlg* m_pContactInfoDlg;
	CSoundDlg* m_pSoundDlg;
	CSettingDlg* m_pSettingDlg;
	CNotebookDlg *m_pNotebookDlg;

	CLunarderDlg	*m_mainLunarderDlg1_;
	BOOL			m_bIsHaveUSB;
	BOOL			m_bIsSD;

	void SetSkinStyle();
	void SendOutEvnet(WPARAM w = 0, LPARAM l = 0);
	void SetScreenSaveTimer();
	void KillScreenSaveTimer();
	
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
protected:
	// Generated message map functions
	//{{AFX_MSG(CMultimediaPhoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
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
	afx_msg void OnDeviceChange(WPARAM w, LPARAM l);
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
