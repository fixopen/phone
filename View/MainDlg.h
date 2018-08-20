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
#include "MainMenuDlg.h"
#include "MainVideoDlg.h"
#include "MainPhotoDlg.h"
#include "MainMp3Dlg.h"
#include "LunarderDlg.h"
#include "ScreenSaveDlg.h"
#include "CalculaterDlg.h"
#include "firewalldlg.h"
#include "WebDialog.h"
#include "./Control/MJPGStatic.h"
#include "View/HuangliDlg.h"
#include "3GTelDlg.h"
#include "3GSMSDlg.h"
#include "3GDetailDlg.h"
#include "3GSysToolTelDlg.h"
#include "3GHomeJoyDlg.h"
#include "3GHomePicDlg.h"

#define MAIN_MARGIN_OFFSET	0	

class CMainDlg : public CDialog
{
public:
	//weather
	int  m_nSMSCount;
	int  m_nMMSCount;
	int  m_nSMSLeaveCount;
	int  m_nRssCount;
	void SetRightInfo(BOOL isDraw = FALSE);
	void SetWeather();
	std::vector<boost::shared_ptr<Data::MultimediaDownload> > rssFileresult;
	std::vector<boost::shared_ptr<Data::Message> > smsFileresult;
	std::vector<boost::shared_ptr<Data::MMSData> > mmsFileresult;
	std::vector<boost::shared_ptr<Data::Message> > smsLeaveresult;

	void doReadRss();
	void doReadSMS();
	void doReadLeaveHome();
	void doReadWeather();

	C3GTelDlg	*m_p3GTelDlg;
	C3GSysToolDlg	*m_p3GSysToolDlg;
	
	C3GHomePicDlg	*m_p3GHomePicDlg;
	C3GHomeJoyDlg	*m_p3GHomeJoyDlg;

	C3GSMSDlg	*m_p3GSMSDlg;
	C3GDetailDlg	*m_p3GDetailDlg;

	CWebDialog* m_pWebDialog;
	void StartWeb();
	CMainMenuDlg	*m_mainmenuDlg_;
	CMainVideoDlg	*m_mainVideoDlg_;
	CMainPhotoDlg	*m_mainPhotoDlg_;
	CMainMp3Dlg		*m_mainMp3Dlg_;
	CLunarderDlg	*m_mainLunarderDlg_;
	CScreenSaveDlg	*m_mainScreenSaveDlg_;
	CCalculaterDlg	*m_mainCalucaterDlg_;
	CFireWallDlg	*m_firewalDlg_;
	CHuangliDlg		*m_pHuangLiDlg_;
	CWnd *m_currentWnd;

	std::vector<CString> m_PhotoList;
	int SetPhotoList();
	void SetTimer_(BOOL flag);
	
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
	void	OnShowTelStatusStatic(WPARAM wParam, LPARAM lParam);
	void	ShowWindow_(int nShowCmd);
	void	SetMainMenu();

	void ShowRightCtrl(int nCmdShow);
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
public:
	virtual ~CMainDlg();
	
	int     m_nUnSMS;
	int		m_nUnTel;					//未接电话数
	int     m_nLeaveSound;				//录音留言数
	BOOL    m_bIsFireWall;				//防火墙是否开启
	BOOL	m_bIsAlarm;					//是否有错过的闹铃
	BOOL	FindTodayAlarm();			//查找今天已经过期的闹铃
	BOOL    ShowTodayAlarm();			//显示今天已经过期的闹铃
	void    SetStatusAll(BOOL flag = TRUE);

	void SetDateTime(BOOL isDraw);
// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_DIALOG_MAIN };
//	CCEStatic	m_LineStatic;
//	CCEStatic	m_TodayStatic;

	CMJPGStatic		m_MJPGList;
	afx_msg void OnTimer(UINT nIDEvent);

	/*
	CCEStatic	m_TelStatusStatic;
	CCEStatic	m_CallWallStatic;
	CCEStatic	m_TodayNoteStatic;
	CCETimeStatic	m_TimeStatic;
	*/

//	CCEFrameStatic  m_FrameStatic;
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
	afx_msg void OnChangeWindow(WPARAM w, LPARAM l);
	afx_msg void OnPlayVideo(WPARAM w, LPARAM l);
	afx_msg void OnPlayPhoto(WPARAM w, LPARAM l);
	afx_msg void OnStaticClick(WPARAM w, LPARAM l);
	afx_msg void OnMJPGShowHalf(WPARAM w, LPARAM l);
	afx_msg void OnClickMJPGToApp(WPARAM w, LPARAM l);
	afx_msg void OnSetAdmin(WPARAM w, LPARAM l);
	afx_msg void OnUnSetAdmin(WPARAM w, LPARAM l);
	afx_msg void OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized );
	//}}AFX_MSG
private:
	//CCEButtonST m_btnMovie;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__23C98484_7A60_4423_A4C0_948678D07F7C__INCLUDED_)
