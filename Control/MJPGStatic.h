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
	int			m_nIsCrollIndex;			//��item�е����
	int			m_nIsCrollCount;			//���Թ����ĸ���
	void		ScrollUnit(int upordown, int unitcount);

	CFont		m_Font;
	int			m_nIndexSelectUnit;
	MJPGList* m_currentMJPGList, *m_oldMJPGList;
	CString  m_oldLinkFile;
	static CString	ToFileDir(CString filename);

 	HDC  m_hMemDC;   //�Ƶ�ȫ��
 	HBITMAP m_hBmp;
 	HBITMAP m_hOldBmp;
 	LPVOID  m_plpBits;	
	BOOL m_bDirty;	// �Ƿ�������󣬸ñ������ڷ�ֹ��ε���InvalidateRect��

//	CBitmap *m_pOldbmp;
//	void CreateMemDC();
//	void ReleaseMemDC();
	void ReleaseMemDC_HDC();
	void CreateMemDC_HDC(CString sFilename);

	int  GetCStringMuliteLine(CString s, int height, CRect rt, MULTILINEINFO *pCount, int LineCount);
	void DrawMultiLine(CDC *pdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
	void DrawMultiLine(HDC hdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
	void DrawMultiLine_HDC(HDC hdc, CString s, int nPixel, int nFontSize, CRect rt, MULTILINEINFO *pVarry, int LineCount,int nAglin1);
	// ����key�����ݻ���һ��Unit�����ֵ�hdc�����»��档
	void DrawUnitText(HDC hdc, CRect & rc, CTextCacheKey & key);
	// ����ָ���Ķ���pUnit��hdcΪĿ��DC
	// isSelectedΪ�ö����Ƿ��ڱ�ѡ��״̬��rcPaintΪ���ڱ���Ч��������
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

	// �����Լ�������ͼ���ļ���
	void CacheSelf();
	// ***********************************************************************
	// Ŀǰʵ���У�����Set���������е�isInvalidate��isDraw�����������ԣ�
	// ���и��²����ȵ�WM_PAINT�¼���ͳһ����ֻ�ǳ��ڼ��ٴ����޸���
	// �Ŀ��Ƕ�����������������
	// �����Ҫ���ϸ��´��ڣ������UpdateWindow()�������й���Ŀ�����
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
	void InsertAdressCharacter(CString &ac);//��ӵ�ַ��������һ����ַ�����ܱ���ӡ

	void SetFocusUnit(int nUnit){m_nFocusUnit = nUnit;}
	int  GetFocusUnit(){return m_nFocusUnit;}

public:
	int m_SetUnitNo;

private:
	int  m_nFocusUnit;				//��ǰ���
	int  m_nRowFocusUnit;			//�й��
	int  m_nColFocusUnit;			//�й��

	BOOL InitFocusUnit();
	int  FindRowFocusUnit(FOCUSDIRECT direct);
	int  FindColFocusUnit(FOCUSDIRECT direct);
	BOOL SetFocusUnit(FOCUSDIRECT direct, int noUnit);
	void DrawRectLine(HDC hdc, CRect rt);
	BOOL SetClickUnit(int noUnit);
private:
	
	// ��ָ���Ķ�����ΪDirty
	void InvalidateItem(UINT nIndex);
	// ����IDΪUnitNO�Ķ��󣬷��ض����������е�Index��-1��ʾδ�ҵ�
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
