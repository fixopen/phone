#if !defined(AFX_CEDATETIMECTRL_H__C8A8A088_9983_4FD8_A0BB_E04055891050__INCLUDED_)
#define AFX_CEDATETIMECTRL_H__C8A8A088_9983_4FD8_A0BB_E04055891050__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEDateTimeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCEDateTimeCtrl window
#define DATETIME_DATESTLE	DTS_SHORTDATEFORMAT
#define DATETIME_TIMESTLE	DTS_TIMEFORMAT

class CCEDateTimeCtrl : public CStatic
{
// Construction
public:
	CFont m_font;
	CTime m_time;
	CTime m_time1, m_time2;

	CRect  m_rect1, m_rect2, m_rect3, m_rect4, m_rect5;
	CButton  m_button1, m_button2;
	int      m_nClick;

	int        m_style;		 
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	BOOL SetRange(const CTime* pMinRange, const CTime* pMaxRange);

	// Operations
	BOOL SetTime(const CTime* pTimeNew);
	DWORD GetTime(CTime& pTimeDest) const;

public:
	CCEDateTimeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEDateTimeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEDateTimeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCEDateTimeCtrl)
	afx_msg void OnPaint();
	afx_msg void OnButton1();
	afx_msg void OnButton2(); 
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEDATETIMECTRL_H__C8A8A088_9983_4FD8_A0BB_E04055891050__INCLUDED_)
