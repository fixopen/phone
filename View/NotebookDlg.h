#if !defined(AFX_INFORMATIONDLG_H__BC1151CF_86F8_4B01_B80A_D28DB7948C38__INCLUDED_)
#define AFX_INFORMATIONDLG_H__BC1151CF_86F8_4B01_B80A_D28DB7948C38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InformationDlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
#include "../Control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CNotebookDlg dialog

class CNotebookDlg : public CDialog
{
// Construction
public:
	BOOL m_bIsOPenTel;
	CNotebookDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNotebookDlg)
	enum { IDD = IDD_DIALOG_NOTEBOOK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotebookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNotebookDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonNoteOk();
	afx_msg void OnButtonNotePrev();
	afx_msg void OnButtonNoteNext();
	afx_msg void OnButtonNoteClear();
	afx_msg void OnButtonNoteClose();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
private:
	char *pbuffer[3];
	TCHAR filename[20][128];
	int  m_nIndex;

	CCEWriteStatic	m_WriteStatic;
	CMJPGStatic		m_MJPGList;
public:
	void SetPageIndex();
	void ShowWindow_(int nCmdShow);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFORMATIONDLG_H__BC1151CF_86F9_4B01_B80A_D28DB7948C38__INCLUDED_)
