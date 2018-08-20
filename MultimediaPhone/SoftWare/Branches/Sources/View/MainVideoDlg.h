#if !defined(AFX_MAINVIDEODLG_H__B24F63BA_C6CD_4AB1_842E_711E35A94A6F__INCLUDED_)
#define AFX_MAINVIDEODLG_H__B24F63BA_C6CD_4AB1_842E_711E35A94A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainVideoDlg.h : header file
//
#include "../control/CeBtnST.h"
#include "../control/CEListCtrl.h"
#include "../control/CEStatic.h"
#include "../control/CEProcessBar.h"
#include "../resource.h"
#include "PlayerDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CMainVideoDlg dialog

class CMainVideoDlg : public CDialog
{
//control
public:
	TCHAR		m_chDir[128];
	CImageList *m_pImageList;
	CCEStatic	m_stcTitle, m_stcTime;
	CCEMoveTxtStatic m_stcFilename;
	CCEBmpButton	m_btnExit;

	CCEBmpButton m_btnAllSelect;
	CCEBmpButton m_btnAllClear;
	CCEBmpButton m_btnOpenFile;
	CCEBmpButton m_btnPlayAll;
	CCEBmpButton m_btnPlayVideo;
	CCEBmpButton m_btnPreVideo;
	CCEBmpButton m_btnBackVideo;
	CCEBmpButton m_btnMuteSound;

	CCEListCtrl m_lstPlayList;

	CCEProcessBar m_prgPlayTime;
	CCEProcessBar m_prgSoundSelect;
	CCEFrameStatic m_backGroudStatic;

	int m_IsPlay;
	BOOL m_IsSound;
	int  m_Volume;
	void SetPlayList(TCHAR *dir, int local = 0);
	void SetVideo();
	void SetCtrlEnable(BOOL flag = TRUE);
	void StartOpenNewFile();
	void OnExit_(){OnExit();}

public:
	CPlayerDlg	*playerDlg_; 
// Construction
public:
	CMainVideoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainVideoDlg)
	enum { IDD = IDD_DIALOG_VIDEO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainVideoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainVideoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSelectAll();
	afx_msg void OnClearAll();
	afx_msg void OnPlayer();
	afx_msg void OnPre();
	afx_msg void OnBack();
	afx_msg void OnOpenFile();
	afx_msg void OnPlayerAll();
	afx_msg void OnMute();
	afx_msg void OnPregress(WPARAM w, LPARAM l);
	afx_msg void OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); 
	afx_msg void OnDeviceChange(WPARAM w, LPARAM l);
	afx_msg void OnOutEvent(WPARAM w, LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIDEODLG_H__B24F63BA_C6CD_4AB1_842E_711E35A94A6F__INCLUDED_)
