#if !defined(AFX_DATECTRL_H__3EA00372_0779_4BC6_A189_F29EDB00DE78__INCLUDED_)
#define AFX_DATECTRL_H__3EA00372_0779_4BC6_A189_F29EDB00DE78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DateCtrl.h : header file
//

#include "CeBtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CDateCtrl window

class CDateCtrl : public CEdit
{
// Construction
public:
	CDateCtrl();

// Attributes
public:
	enum em_mode{modeDate, modeTime};
private:
	enum em_selection{esel_year, esel_month, esel_day, esel_hour, esel_minute, esel_second};
public:
	static BOOL IsLeapYear(UINT16 iYear)
	{return !(iYear%4)&&(iYear%100) || !(iYear%400);}

	static UINT16 MonthDays(UINT16 iYear, UINT16 iMonth);
	static void FormatDateOrTime(CString &strData, const int nYear, const int nMonth, const int nDay, em_mode mode=modeDate); 
	static BOOL FormatSingleVal(CString &strData, const int nVal, em_selection sel);

	void SetTime(const int nYear=1970, const int nMonth=1, const int nDay=1, const int nHour=0, const int nMinute=0, const int nSecond=0);
	void SetTime(const SYSTEMTIME &ti);
	void SetTime(CTime *time);

	void GetTime(CTime &timeDest);
	void GetTime(SYSTEMTIME &sti);

private:
	static const int BUTTON_WIDTH;
	static const int ID_BUTTON_UP;
	static const int ID_BUTTON_DOWN;
	
	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nHour;
	int m_nMinute;
	int m_nSecond;

//	SYSTEMTIME m_sysTime;
	
	em_selection m_curSel;
	em_mode m_mode;
//	CTime m_time;

	CCEBmpButton m_btnUp;
	CCEBmpButton m_btnDown;
	
	CString m_strBuffer;
	CFont m_font;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateCtrl)
	public:
	virtual BOOL Create(/*LPCTSTR lpszClassName, LPCTSTR lpszWindowName, */DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT uFontHeight=28, em_mode mode=modeDate/*, CCreateContext* pContext = NULL*/);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDateCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDateCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	 afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	 afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	 afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	 afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	 afx_msg void OnSize(UINT nType, int cx, int cy);
	 afx_msg void OnButtonUp();
	 afx_msg void OnButtonDown();
	 afx_msg void OnSubBtnUp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATECTRL_H__3EA00372_0779_4BC6_A189_F29EDB00DE78__INCLUDED_)
