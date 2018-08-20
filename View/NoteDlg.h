#if !defined(AFX_NOTEDLG_H__3F570C84_070E_4F57_B13D_D6D2904F5769__INCLUDED_)
#define AFX_NOTEDLG_H__3F570C84_070E_4F57_B13D_D6D2904F5769__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// notedlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNoteDlg dialog

class CNoteDlg : public CCEDialog
{
// Construction
public:
	CNoteDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNoteDlg)
	enum { IDD = IDD_DIALOG_NOTE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNoteDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnButtonNoteOk();
	afx_msg void OnButtonNotePrev();
	afx_msg void OnButtonNoteNext();
	afx_msg void OnButtonNoteClear();
	DECLARE_MESSAGE_MAP()
private:
	char *pbuffer[3];
	TCHAR filename[3][128];
	int  m_nIndex;
	CCEStatic m_sticTitle;
	CCEStatic m_sticPageNumber;
	CCEStatic m_sticPage;
	CCEButtonST m_btnOk;
	CCEButtonST m_btnPrev;
	CCEButtonST m_btnNext;
	CCEButtonST m_btnClear;

	CCEWriteStatic	m_WriteStatic;

	CCEStatic m_sticBackground;

	void SetPageIndex();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTEDLG_H__3F570C84_070E_4F57_B13D_D6D2904F5769__INCLUDED_)
