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

#define MAIN_MARGIN_OFFSET	0	

//#define DIVIDEDLINE_RGB		RGB(154, 212, 255)
#define STATIC_RGB1			RGB(255, 0, 0)
#define STATIC_RGB2			RGB(0, 255, 0)
#define STATIC_RGB3			RGB(255, 0, 255)


class CMainDlg : public CDialog
{
public:
	CMainMenuDlg	*m_mainmenuDlg_;
	CMainVideoDlg	*m_mainVideoDlg_;
	CMainPhotoDlg	*m_mainPhotoDlg_;
	CMainMp3Dlg		*m_mainMp3Dlg_;
	CLunarderDlg	*m_mainLunarderDlg_;
	CScreenSaveDlg	*m_mainScreenSaveDlg_;
	CCalculaterDlg	*m_mainCalucaterDlg_;

public:
	void SetCtrlEnable(BOOL flag);
	//获取未接电话个数
	int		GetMissedTelInfo();
	//获取留言电话个数
	int		GetRecordedTelInfo();
	//获取是否设置启动防火墙
	BOOL	GetCallWallInfo();
// Construction
public:
	CMainDlg(CWnd* pParent = NULL);   // standard constructor
public:
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
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeWindow(WPARAM w, LPARAM l);
	afx_msg void OnPlayVideo(WPARAM w, LPARAM l);
	afx_msg void OnPlayPhoto(WPARAM w, LPARAM l);
	//}}AFX_MSG
private:
	//CCEButtonST m_btnMovie;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__23C98484_7A60_4423_A4C0_948678D07F7C__INCLUDED_)
