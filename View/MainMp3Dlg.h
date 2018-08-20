#if !defined(AFX_MAINMP3DLG_H__47B29B59_B76C_4377_A489_1AF444DF996B__INCLUDED_)
#define AFX_MAINMP3DLG_H__47B29B59_B76C_4377_A489_1AF444DF996B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainMp3Dlg.h : header file
//

#include "../control/CeBtnST.h"
#include "../control/CEListCtrl.h"
#include "../control/CEStatic.h"
#include "../control/CEProcessBar.h"
#include "../resource.h"
#include "PlayerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMainMp3Dlg dialog

class CMainMp3Dlg : public CDialog
{
	//control
public:
	char		m_chDir[128];
	CImageList *m_pImageList;
	CCEStatic	m_stcTitle, m_stcTime;
	CCEMoveTxtStatic m_stcFilename;
	CCEButtonST	m_btnExit;

	CCEBmpButton m_btnAllSelect;
	CCEBmpButton m_btnAllClear;
	CCEBmpButton m_btnOpenFile;
	CCEBmpButton m_btnPlayMp3;
	CCEBmpButton m_btnPreMp3;
	CCEBmpButton m_btnBackMp3;
	CCEBmpButton m_btnMuteSound;

	CCEProcessBar m_prgPlayTime;
	CCEProcessBar m_prgSoundSelect;
	
	CCEListCtrl m_lstPlayList;
	CCEListCtrl m_lstPlayList1;
	CCEFrameStatic m_backGroudStatic;

	int m_IsPlay;
	BOOL m_IsSound;
	int  m_Volume;

	void SetPlayList(char *dir);
	void SetMP3();
	void SetCtrlEnable(BOOL flag = TRUE);
	void StartOpenNewFile();

public:
	CPlayerDlg	*playerDlg_; 

// Construction
public:
	CMainMp3Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainMp3Dlg)
	enum { IDD = IDD_DIALOG_MP3 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainMp3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainMp3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSelectAll();
	afx_msg void OnClearAll();
	afx_msg void OnPlayer();
	afx_msg void OnPre();
	afx_msg void OnBack();
	afx_msg void OnOpenFile();
	afx_msg void OnMute();
	afx_msg void OnPregress(WPARAM w, LPARAM l);
	afx_msg void OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINMP3DLG_H__47B29B59_B76C_4377_A489_1AF444DF996B__INCLUDED_)
