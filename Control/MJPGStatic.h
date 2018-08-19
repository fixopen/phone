#if !defined(AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_)
#define AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MJPGStatic.h : header file
//

#include "../xml/config.h"
#include "../xml/xmlParser.h"

struct CTextCacheKey;

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
	static CString	ToFileDir(CString filename);

 	HDC  m_hMemDC;   //移到全局
 	HBITMAP m_hBmp;
 	HBITMAP m_hOldBmp;
 	LPVOID  m_plpBits;	
	BOOL m_bDirty;	// 是否有脏对象，该变量用于防止多次调用InvalidateRect。

//	CBitmap *m_pOldbmp;
//	void CreateMemDC();
//	void ReleaseMemDC();
	void ReleaseMemDC_HDC();
	void CreateMemDC_HDC(CString sFilename);

	int  GetCStringMuliteLine(CString s, int height, CRect rt, MULTILINEINFO *pCount, int LineCount);
	void DrawMultiLine(CDC *pdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
	void DrawMultiLine(HDC hdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
	void DrawMultiLine_HDC(HDC hdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
	// 根据key的内容绘制一个Unit的文字到hdc，更新缓存。
	void DrawUnitText(HDC hdc, CRect & rc, CTextCacheKey & key);
	// 绘制指定的对象pUnit。hdc为目标DC
	// isSelected为该对象是否处于被选择状态，rcPaint为窗口被无效化的区域。
	void DrawUnit(HDC hdc, UNIT * pUnit, BOOL isSelected, CRect & rcPaint);
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

	// 缓冲自己包含的图形文件。
	void CacheSelf();
	// ***********************************************************************
	// 目前实现中，下面Set函数的所有的isInvalidate和isDraw参数都被忽略，
	// 所有更新操作等到WM_PAINT事件中统一处理。只是出于减少代码修改量
	// 的考虑而保留这两个参数。
	// 如果需要马上更新窗口，请调用UpdateWindow()，不会有过多的开销。
	// ***********************************************************************

	BOOL SetUnitIsDownStatus(int UnitNO, BOOL isDown);
	BOOL SetUnitIsShow(int UnitNO, BOOL isShow, BOOL isInvalidate = FALSE);
	BOOL SetUnitIsShowNOResh(int UnitNO, BOOL isShow);

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
	BOOL SetUnitRect(int UnitNO, CRect rt);
	UNIT *GetUnit(int UnitNO);
	
	//add by qi 20100611
	void InsertAdressCharacter(CString &ac);//添加地址符，现在一个地址符不能被打印

	void SetFocusUnit(int nUnit){m_nFocusUnit = nUnit;}
	int  GetFocusUnit(){return m_nFocusUnit;}

public:
	int m_SetUnitNo;

private:
	int  m_nFocusUnit;				//当前光标
	int  m_nRowFocusUnit;			//行光标
	int  m_nColFocusUnit;			//列光标

	BOOL InitFocusUnit();
	int  FindRowFocusUnit(FOCUSDIRECT direct);
	int  FindColFocusUnit(FOCUSDIRECT direct);
	BOOL SetFocusUnit(FOCUSDIRECT direct, int noUnit);
	void DrawRectLine(HDC hdc, CRect rt);
	BOOL SetClickUnit(int noUnit);
private:
	
	// 将指定的对象标记为Dirty
	void InvalidateItem(UINT nIndex);
	// 查找ID为UnitNO的对象，返回对象在数组中的Index，-1表示未找到
	int GetIndexByUnitNo(int UnitNO);


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
	afx_msg void OnTimer(UINT nID);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MJPGSTATIC_H__4585B87E_B19A_4FD8_BFFC_F6A01456C224__INCLUDED_)
