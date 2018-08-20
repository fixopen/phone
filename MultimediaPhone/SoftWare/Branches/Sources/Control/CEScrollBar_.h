#if !defined(AFX_CESCROLLBAR__H__6193D012_453D_4F30_887C_9CC2827782E9__INCLUDED_)
#define AFX_CESCROLLBAR__H__6193D012_453D_4F30_887C_9CC2827782E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEScrollBar_.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCEScrollBar_ window
#define SCROLL_ARROW_WIDTH		20			// 17
#define SCROLL_ARROW_HEIGHT     17
#define SCROLL_THICK_WIDTH		18
#define SCROLL_THICK_HEIGHT		29

//#define SCROLLBAR_DISABLERGB2  RGB(192, 192, 192)
//#define SCROLLBAR_FRAMERGB	   RGB(76,165,229)

//#define SCROLLBAR_RGB  RGB(0xFF, 0xFF, 0)

class CCEScrollBar_ : public CStatic
{
public:
	int m_nStyle;     //0 yellow 滚动条     1  gray 滚动条
	BOOL m_bIsActive; //0 Disable			1  Enable
	int m_nMin;
	int m_nMax;
	int m_nStep;
	int m_nPos;
	int m_nPage;
	int m_BitmapUpID;
	int m_BitmapDownID;
	int m_BitmapThickID;
	CRect m_RectUpArrow;
	CRect m_RectDownArrow;
	CRect m_RectThick;
	CRect m_RectUp;
	CRect m_RectDown;
	
	int	m_nPressed;			//0 无 1 up 2 down 3 up page 4 down page;
	CWnd	*m_Owner;

	void SetParam(int style, int nMin, int nMax, int nPage, CWnd *pListCtrl);
	void SetRangle(int nMin, int nMax, int nPage);
	void SetPos(int nPos, BOOL redraw = TRUE);

// Construction
public:
	CCEScrollBar_();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEScrollBar_)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEScrollBar_();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCEScrollBar_)
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CESCROLLBAR__H__6193D012_453D_4F30_887C_9CC2827782E9__INCLUDED_)
