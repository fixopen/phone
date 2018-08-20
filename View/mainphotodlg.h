#if !defined(AFX_MAINPHOTODLG_H__E1949077_7D8E_4D6B_AC38_AE24888FCFED__INCLUDED_)
#define AFX_MAINPHOTODLG_H__E1949077_7D8E_4D6B_AC38_AE24888FCFED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainPhotoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainPhotoDlg dialog

#include "../control/CeBtnST.h"
#include "../control/CEListCtrl.h"
#include "../control/CEStatic.h"
#include "../resource.h"
#include "PlayerDlg.h"

class CMainPhotoDlg : public CDialog
{
public:
	char		m_chDir[128];
	CImageList *m_pImageList;
	CCEStatic	m_stcTitle, m_stcCount;
	CCEButtonST	m_btnExit;
	CCEButtonST m_btnAllSelect;
	CCEButtonST m_btnAllClear;
	CCEButtonST m_btnOpenFile;
	CCEButtonST m_btnPlayAll;
	CCEButtonST m_btnPlayPhoto;
	CCEButtonST m_btnPrePhoto;
	CCEButtonST m_btnBackPhoto;
	CCEButtonST m_btnScreenSave;
	CCEListCtrl m_lstPlayList;
	CCEFrameStatic m_backGroudStatic;

	std::vector<CString> m_PhotoList;
	int m_nCountPhoto;
	
	int SetPlayList(char *dir);
	void SetPhoto();
	void SetCtrlEnable(BOOL flag = TRUE);
	void SetAllScreenPlayer(BOOL flag);

public:
	CPlayerDlg	*playerDlg_; 
	
// Construction
public:
	CMainPhotoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainPhotoDlg)
	enum { IDD = IDD_DIALOG_PHOTO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainPhotoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainPhotoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnExit();
	afx_msg void OnSelectAll();
	afx_msg void OnClearAll();
	afx_msg void OnPlayer();
	afx_msg void OnPre();
	afx_msg void OnBack();
	afx_msg void OnOpenFile();
	afx_msg void OnPlayerAll();
	afx_msg void OnScreenSave();
	afx_msg void OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINPHOTODLG_H__E1949077_7D8E_4D6B_AC38_AE24888FCFED__INCLUDED_)
