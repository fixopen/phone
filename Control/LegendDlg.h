#if !defined(AFX_LEGENDDLG_H__6CCE7BF0_0878_4D5F_8BAA_2A4769935CBA__INCLUDED_)
#define AFX_LEGENDDLG_H__6CCE7BF0_0878_4D5F_8BAA_2A4769935CBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LegendDlg.h : header file
#include "CEStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CLegendDlg window

class CLegendDlg : public CCEStatic
{
// Construction
public:
	CLegendDlg();

// Attributes
public:
	CListCtrl	  m_lstContent;
	CBitmapButton m_btnClose;
	CBitmapButton m_btnPageUp;
	CBitmapButton m_btnPageDown;
	CFont		  m_font;

	float m_tHeight;
	float m_tWidth;
// Operations
public:
	BOOL Create(DWORD dwStyle, CRect &rect, CWnd *pParentWnd, UINT nID, int fontHeight = 22);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLegendDlg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLegendDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLegendDlg)
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonClose();
	afx_msg void OnButtonPageUp();
	afx_msg void OnButtonPageDown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEGENDDLG_H__6CCE7BF0_0878_4D5F_8BAA_2A4769935CBA__INCLUDED_)
