#if !defined(AFX_CELINEEDIT_H__A8240726_544A_4649_B183_B78BA37F3106__INCLUDED_)
#define AFX_CELINEEDIT_H__A8240726_544A_4649_B183_B78BA37F3106__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CELineEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCELineEdit window

class CCELineEdit : public CEdit
{
public:
	CFont m_font;
	CBrush m_brBk;
	int m_nFontHeight;
	BOOL m_isAutoInPut;
	COLORREF m_backRGB;
	void SetIsAutoInput(BOOL isAuto = FALSE){m_isAutoInPut = isAuto;}
// Construction
public:
	CCELineEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCELineEdit)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCELineEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCELineEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHideInput(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CCERectEdit window

class CCERectEdit : public CEdit
{
	// Construction
public:
	BOOL m_isAutoInPut;
	void SetIsAutoInput(BOOL isAuto = FALSE){m_isAutoInPut = isAuto;}
	CBrush m_brBk;
	CFont	m_font;
	BOOL    m_bDigital;
	CCERectEdit();
	
	// Attributes
public:
	
	// Operations
public:
	BOOL Create(DWORD dwStyle, CRect &rect, CWnd *pParentWnd, int nCtrlID, int nFontHeight = 28);
	void SetLimitDiagital(BOOL flag = TRUE){m_bDigital = flag;}
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCERectEdit)
protected:
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CCERectEdit();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCERectEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHideInput(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CELINEEDIT_H__A8240726_544A_4649_B183_B78BA37F3106__INCLUDED_)
