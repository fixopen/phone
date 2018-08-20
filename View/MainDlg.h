#if !defined(AFX_MAINDLG_H__23C98484_7A60_4423_A4C0_948678D07F7C__INCLUDED_)
#define AFX_MAINDLG_H__23C98484_7A60_4423_A4C0_948678D07F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog
#include "../Control/CeBtnST.h"
#include "../Control/CEStatic.h"
// #include "MainMenuDlg.h"
// #include "MainVideoDlg.h"
// #include "MainPhotoDlg.h"
// #include "MainMp3Dlg.h"
#include "LunarderDlg.h"
#include "ScreenSaveDlg.h"
#include "CalculaterDlg.h"
#include "firewalldlg.h"
#include "../Control/CEProcessBar.h"
#include "../Control/SoftKey.h"
#define MAIN_MARGIN_OFFSET	0	
//#define WM_CHANGELANGUAGE (WM_USER+300)
class CMainDlg : public CDialog
{
public:
	ICONKEYSTRUCT softkeyval_[3];
	int             m_nRdTime;
	CLunarderDlg	*m_mainLunarderDlg_;
	CScreenSaveDlg	*m_mainScreenSaveDlg_;
	CCalculaterDlg	*m_mainCalucaterDlg_;
	CFireWallDlg	*m_firewalDlg_;
	CWnd *m_currentWnd;
	UINT m_nUnconnectCount;
	UINT m_nRecordCount;
	
public:
	void SetCtrlEnable(BOOL flag);
	//获取未接电话个数
	int		GetMissedTelInfo();
	//获取留言电话个数
	int		GetRecordedTelInfo();
	//获取是否设置启动防火墙
	BOOL	GetCallWallInfo();
	void 	OnShowNoteStatic();
	void	OnShowCallWallStatic();
	void	OnShowTelStatusStatic();
	void	ShowWindow_(int nShowCmd);
	void    OnAppSoftKey(WPARAM wParam, LPARAM lParam);
	void OnScreenSaveBtn();
	void OnCalculBtn();
	void OnLunarderBtn();
	void UpdateDiskInfo(void);
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
public:
	void ChangeLanguage();
	virtual ~CMainDlg();
// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_DIALOG_MAIN };
//	CCEStatic	m_LineStatic;
//	CCEStatic	m_TodayStatic;
	CCEStatic	m_TelStatusStatic;
	CCEStatic	m_CallWallStatic;
	CCEStatic	m_TodayNoteStatic;
	CCETimeStatic	m_TimeStatic;

	CCEFrameStatic  m_FrameStatic;

	CCEIconSoftKey  m_AppSoftKey;
	
	CCEStatic m_sticRecordMode;
	CCEStatic m_sticRecordQuality;
	CCEStatic m_sticRecordRemain;
	CCEStatic m_sticDiskVolume;
	CCEStatic m_sticUnUsedDiskVolume;
	CCEProcessBar m_procbarDiskUse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnChangeWindow(WPARAM w, LPARAM l);
	afx_msg void OnStaticClick(WPARAM w, LPARAM l);
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
//	afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );
	//}}AFX_MSG
private:
	//CCEButtonST m_btnMovie;
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__23C98484_7A60_4423_A4C0_948678D07F7C__INCLUDED_)
