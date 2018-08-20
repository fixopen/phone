#if !defined(AFX_CEPROCESSBAR_H__498696D3_AA64_49AC_8AD0_2AE036498BCF__INCLUDED_)
#define AFX_CEPROCESSBAR_H__498696D3_AA64_49AC_8AD0_2AE036498BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEProcessBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCEProcessBar window
#define WM_PROCESS_POS	(WM_USER+202)
class CCEProcessBar : public CStatic
{
public:
	int m_nMin;
	int m_nMax;
	int m_nStep;
	int m_nPos;
	int m_BitmapID;
	int m_BitmapIDTop;
	int m_BitmapIDBottom;
	int m_nStyle;		//0 »­ÇøÓò     1 bitmap
	BOOL m_bSelectedThick;
	COLORREF	m_backRGB;

	BOOL Create(int style, CRect r, CWnd *pParent, int ctrlID);
// Construction
public:
	CCEProcessBar();

	void SetParam(int nbmpID, int MIN, int MAX, int nStep, int nStyle = 0, int nBmpTop = 0, int nBmpBottom = 0);
	void SetPos(int nPos, BOOL redraw = TRUE);
	void SetBackRGB(COLORREF cl){m_backRGB = cl;}

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEProcessBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEProcessBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCEProcessBar)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEPROCESSBAR_H__498696D3_AA64_49AC_8AD0_2AE036498BCF__INCLUDED_)
