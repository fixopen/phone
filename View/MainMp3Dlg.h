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
#include "../Control/MJPGStatic.h"
#include "../resource.h"
#include "PlayerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMainMp3Dlg dialog

class CMainMp3Dlg : public CDialog
{
	//control
public:
	BOOL m_bIsPausebyEvent;
	int			m_nListIndex;

	TCHAR		m_chDir[128];
	CImageList *m_pImageList;
	
	CCEProcessBar m_prgPlayTime;
	CCEProcessBar m_prgSoundSelect;
	
	CCEListCtrl m_lstPlayList;
	CCEListCtrl m_lstPlayList1;

	CMJPGStatic		m_MJPGList;

	int m_IsPlay;
	BOOL m_IsSound;
	int  m_Volume;

	//local 0³õ¼¶½×¶Î
	void SetPlayList(TCHAR *dir, int local = 0);
	void SetMP3(char *filename);
	void SetCtrlEnable(BOOL flag = TRUE);
	void StartOpenNewFile();
	void OnExit_(BOOL isStopMusic);
	void SetVolume();

public:
	CPlayerDlg	*playerDlg_; 

	afx_msg void OnTimer(UINT nIDEvent);

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
	public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
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
	afx_msg void OnDeviceChange(WPARAM w, LPARAM l);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); 
	afx_msg void OnOutEvent(WPARAM w, LPARAM l);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINMP3DLG_H__47B29B59_B76C_4377_A489_1AF444DF996B__INCLUDED_)
