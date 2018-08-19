#if !defined(AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_)
#define AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MJPGStatic.h : header file
//

#include "../xml/config.h"
#include "../xml/xmlParser.h"

/////////////////////////////////////////////////////////////////////////////
// CMJPGStatic window
struct MULTILINEINFO
{
	int nStart;
	int nCount;
};

enum FOCUSDIRECT{TOUP, TODOWN, TOLEFT, TORIGHT};

class CMJPGStatic : public CStatic
{
public:
	int			m_nIsCrollIndex;			//在item中的序号
	int			m_nIsCrollCount;			//可以滚动的个数
	void		ScrollUnit(int upordown, int unitcount);

	CFont		m_Font;
	int			m_nIndexSelectUnit;
	MJPGList* m_currentMJPGList, *m_oldMJPGList;
	CString  m_oldLinkFile;
	CString	ToFileDir(CString filename);

 	HDC  m_hMemDC;   //移到全局
 	HBITMAP m_hBmp;
 	HBITMAP m_hOldBmp;
 	LPVOID  m_plpBits;	

//	CBitmap *m_pOldbmp;
//	void CreateMemDC();
//	void ReleaseMemDC();
	void ReleaseMemDC_HDC();
	void CreateMemDC_HDC(CString sFilename);

	int GetCStringMuliteLine(CString s, int height, CRect rt, MULTILINEINFO *pCount, int LineCount);
	void DrawMultiLine(CDC *pdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
	void DrawMultiLine_HDC(HDC hdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
	void DrawUnitStatus_HDC(UNIT *pUnit, int nUintStatus);
//	void DrawUnit(UNIT *pUnit, CDC *pdc);
//	void DrawMJPGPage(CString sFile);
	void DrawUnit_HDC(UNIT *pUnit, HDC pdc);
	void DrawMJPGPage_HDC(CString sFile);
	int FindUnitIndex(CPoint pt);
	void InvertRect(CRect rt);
	void InvertRect_HDC(HDC hdc, CRect rt);

	void DrawGroup_HDC(int *nNO, int nCount, BOOL isShow);
//	void DrawUnitStatus(UNIT *pUnit, int nUintStatus);
		
	CRect		m_rtMJPG;
	CRect		m_ClientRect;
	void SetMJPGRect(CRect rt){m_rtMJPG = rt; m_ClientRect = CRect(0, 0, rt.Width(), rt.Height());}
	CString 	m_sCurrentLinkFile;
	void SetCurrentLinkFile(CString s);
	void GetCurrentLinkFile(CString &s){s = m_sCurrentLinkFile;}

	BOOL SetUnitIsDownStatus(int UnitNO, BOOL isDown);
	int SetUnitIsShow(int UnitNO, BOOL isShow, BOOL bRefresh = TRUE, BOOL isInvalidate = FALSE);

	BOOL SetUnitBitmap(int UnitNO, CString filename_up, CString filename_down, BOOL isDraw, BOOL isInvalidate = FALSE);
	BOOL SetUnitText(int UnitNO, CString text, BOOL isDraw, BOOL isInvalidate = FALSE);
	CString GetUnitText(int UnitNO);
	BOOL SetUnitColor(int UnitNO, TEXTCOLOR cl, BOOL isDraw, BOOL isInvalidate = FALSE);
	BOOL SetUnitIsTranslate(int UnitNO, BOOL isTrans);
	BOOL SetUnitIsDisable(int UnitNO, BOOL isDisable);
	BOOL SetUnitFont(int unitNO, TEXTSIZE nFont);
	BOOL GetUnitIsShow(int UnitNO);
	BOOL GetUnitIsDownStatus(int UnitNO);
	CRect GetUnitRect(int UnitNO);
	BOOL GetUnitIsDisable(int UnitNO);

private:
	int  m_nFocusUnit;				//当前光标
	int  m_nRowFocusUnit;			//行光标
	int  m_nColFocusUnit;			//列光标

	BOOL InitFocusUnit();
	int  FindRowFocusUnit(FOCUSDIRECT direct);
	int  FindColFocusUnit(FOCUSDIRECT direct);
	BOOL SetFocusUnit(FOCUSDIRECT direct, int noUnit);

// Construction
public:
	CMJPGStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMJPGStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMJPGStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMJPGStatic)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_)
