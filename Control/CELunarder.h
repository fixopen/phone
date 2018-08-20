#if !defined(AFX_CELUNARDER_H__D3FCF872_8174_4E5D_8180_670F057C0EE7__INCLUDED_)
#define AFX_CELUNARDER_H__D3FCF872_8174_4E5D_8180_670F057C0EE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CELunarder.h : header file
//

#include "CEStatic.h"
#include "CeBtnST.h"
/////////////////////////////////////////////////////////////////////////////
// CCELunarder window

class CCELunarder : public CStatic
{
public:
	CCEStatic	m_StcYear, m_StcMonth, m_StcWeek;
	CCEStatic	m_StcDay[49];
	CCEButtonST m_BtnYleft, m_BtnYRight, m_BtnMlelt, m_BtnMright;

	UINT16 m_nY;
	UINT8  m_nM, m_nD;
	
	BOOL Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID);
	void SetData(UINT16 year, UINT8 month, UINT8 day, BOOL reDraw = FALSE);
	void GetData(UINT16 &year, UINT8 &month, UINT8 &day);
	void SetStcDay();
// Construction

public:
	CCELunarder();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCELunarder)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCELunarder();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCELunarder)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDaySelect(WPARAM w, LPARAM l);
	afx_msg void OnYleft();
	afx_msg void OnYright();
	afx_msg void OnMleft();
	afx_msg void OnMright();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELUNARDER_H__D3FCF872_8174_4E5D_8180_670F057C0EE7__INCLUDED_)
