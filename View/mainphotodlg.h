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
#include "../Control/MJPGStatic.h"
#include "../resource.h"
#include "PlayerDlg.h"
#include "../data/OTAData.h"

enum PhotoType{local_type, net_type};

class CMainPhotoDlg : public CDialog
{
public:
	PhotoType	m_nPhotoType;
	TCHAR		m_chDir[128];
	CImageList *m_pImageList;

	CCEListCtrl m_lstPlayList;
	CMJPGStatic		m_MJPGList;


	std::vector<CString> m_PhotoList;
	std::vector<boost::shared_ptr<Data::MultimediaDownload> > m_pMediaFileresult;// = Data::MenuManager::GetFromDatabase(""); 

	int m_nCountPhoto;
	
	int SetPlayList(TCHAR *dir, int local = 0);
	void SetPhoto();
	void SetCtrlEnable(BOOL flag = TRUE);
	void SetAllScreenPlayer(BOOL flag);
	void OnExit_(){OnExit();}
	afx_msg void OnOpenFile();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	CPlayerDlg	*playerDlg_; 
	
// Construction
public:
	CMainPhotoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainPhotoDlg)
	enum { IDD = IDD_DIALOG_CACULATER };
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
	afx_msg void OnPlayer(int index = 0);
	afx_msg void OnPre();
	afx_msg void OnBack();
	afx_msg void OnPlayerAll();
	afx_msg void OnScreenSave();
	afx_msg void OnClickPlayList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); 
	afx_msg void OnDeviceChange(WPARAM w, LPARAM l);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnListCltrlClick(WPARAM w, LPARAM l);
	afx_msg void OnOutEvent(WPARAM w, LPARAM l);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINPHOTODLG_H__E1949077_7D8E_4D6B_AC38_AE24888FCFED__INCLUDED_)
