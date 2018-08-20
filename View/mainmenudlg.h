#if !defined(AFX_MAINMENUDLG_H__E608C677_E0D6_4BBF_9ED5_52B5DEC2EB2E__INCLUDED_)
#define AFX_MAINMENUDLG_H__E608C677_E0D6_4BBF_9ED5_52B5DEC2EB2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainMenuDlg.h : header file
//
#include "../Control/CeBtnST.h"
#include "../Control/CEStatic.h"
#include "PlayerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMainMenuDlg dialog

class CMainMenuDlg : public CDialog
{
public:
	CPlayerDlg	*playerDlg_; 
	void SetVideo();
// Construction
public:
	CMainMenuDlg(CWnd* pParent = NULL);   // standard constructor
public:
	virtual ~CMainMenuDlg();
// Dialog Data
	//{{AFX_DATA(CMainMenuDlg)
	enum { IDD = IDD_DIALOG_MAINMENU };
	
	CCEButtonST	m_ScreenSaveBtn;
	CCEButtonST m_CalcuBtn;
	CCEButtonST	m_CalenBtn;
	CCEButtonST	m_MusicBtn;
	CCEButtonST	m_PhotoBtn;
	CCEButtonST	m_VideoBtn;
	CCEFrameStatic m_backGroudStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainMenuDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainMenuDlg)
	afx_msg void OnVideoBtn();
	afx_msg void OnPhotoBtn();
	afx_msg void OnMp3Btn();
	afx_msg void OnScreenSaveBtn();
	afx_msg void OnCalculBtn();
	afx_msg void OnLunarderBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINMENUDLG_H__E608C677_E0D6_4BBF_9ED5_52B5DEC2EB2E__INCLUDED_)
