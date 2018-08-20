#if !defined(AFX_CESTATIC_H__CFA72294_8208_4505_AFF8_8EB32D54A4D8__INCLUDED_)
#define AFX_CESTATIC_H__CFA72294_8208_4505_AFF8_8EB32D54A4D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEStatic.h : header file
//
#define WM_STATIC_CLICK			WM_USER+300

struct TextStruct
{
	UINT8 txtFontSize;
	COLORREF txtColor;
	char	 sTxt[64];
	CRect	txtRect;
	UINT32	sAlign;
};

/////////////////////////////////////////////////////////////////////////////
// CCEStatic window

class CCEStatic : public CStatic
{
public:
	BOOL		m_bIsClicked;
	BOOL        m_bIsUnderLine;
	BOOL		m_bIsToggle;
	COLORREF	m_txtRGB;
	COLORREF	m_bkRGB;
	CBrush		m_brush;
	int			m_nFontHeight;
	int			m_nLeftMargin;
	CFont		m_Font;
	UINT32		m_nBitmapID;
	int			m_nAlign;
	BOOL		m_bIsBorder;

	//�����Ƿ����»���
	void SetUnderLine(BOOL isUnderLine);
	//����������ɫ��������ɫ
	void SetColor(COLORREF txtClr, COLORREF bkClr);
	//�����Ƿ���Ӧ�������Ϣ
	void SetClicked(BOOL isClicked);
	void SetTogle(BOOL isToggle, BOOL flag = FALSE){m_bIsToggle = isToggle; if(flag)OnPaint();}
	//������߿�ʼ�ռ������ؿ�ʼ��ʾ����
	void SetLeftMargin(int nPixel){m_nLeftMargin = nPixel;}
	//��������Ĵ�С
	void SetFontSize(int size);
	//���ñ�����ͼ
	void SetBitmapID(int nBitmapID){m_nBitmapID = nBitmapID;}
	//�������ֶ��뷽ʽ
	void SetAlign(int nAglin){m_nAlign = nAglin;}
	//�����Ƿ�border
	void SetBorder(BOOL isBorder){m_bIsBorder = isBorder;}

	//���������ؼ�
	CString m_sDate1, m_sDate2;
	void SetDateTxt(CString sDate1, CString sDate2){m_sDate1 = sDate1; m_sDate2 = sDate2;}
	void GetDateTxt(CString &s){s = m_sDate1;}

// Construction
public:
	CCEStatic();

// Attributes
public:
  
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCEStatic)
	afx_msg void OnClicked();
	afx_msg void OnPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CCETimeStatic : public CButton
{
public:
	BOOL		m_bChineseVersion;
	int			m_nFontHeight;
	COLORREF    m_backRGB;
	void		SetBackRGB(COLORREF cl){m_backRGB = cl;}
	void		OnShow();

// Construction
public:
	CCETimeStatic();

// Attributes
public:
  
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCETimeStatic();
private:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

	// Generated message map functions
	//{{AFX_MSG(CCEStatic)
//	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CCEMoveTxtStatic : public CStatic
{
public:
	int m_nCount, m_nW;
	CFont	*m_font;
	CDC   memDC; 
	CBitmap   bmp;   
	BOOL Create(LPCTSTR sTxt, int style, CRect &rect, CWnd *pParentWnd, int nCtrlID= 0xFFFF);
	void SetTxt(LPCTSTR sTxt);
// Construction
public:
	CCEMoveTxtStatic();

// Attributes
public:
  
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEMoveTxtStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCEStatic)
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#include "DIBSectionCE.h"
class CCEWriteStatic : public CStatic
{
public:
	CDIBSectionCE aDibSectionCE;
	char	*m_data;
	CPoint m_OldPoint;
	CDC	    *m_MemDC;
	CBitmap m_Bitmap;
	BOOL    m_bChanged;

	void ResetPoint(){m_OldPoint = CPoint(0xFFFF, 0xFFFF);}
	void SetWrite(char *ptr);
	void GetWrite(char *ptr);

	void SetWrite(TCHAR *filename);
	void GetWrite(TCHAR *filename);
	void DrawMemDC();
	void ClearDC();

// Construction
public:
	CCEWriteStatic();

// Attributes
public:
  
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEWriteStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEWriteStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCEWriteStatic)
	afx_msg void OnPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CCEFrameStatic : public CStatic
{
public:
	int			m_nStyle;
	TextStruct	*m_pTxtStruct;
	int         m_ntxtCount;
	CFont		m_Font;
	CRect		m_InRect;

	void SetSkinStyle();
	// Construction
public:
	//nStyle  0 ����װ�� (������ MP3���ŵȴ���)  1 ��װ��(�½���Ƭ ����)
	BOOL Create(CRect &rect, CWnd *pParentWnd, int nStyle = 0);
	//��Ӵ�������Ҫ��ʾ������,�����ֵ��û����Ϣ��Ӧ
	void SetTextStruct(TextStruct *pTxtStruct, int ncount);
	void ChangeTextStruct(int nIndex, TextStruct txtStruct);
	void SetInRect(CRect rect){m_InRect = rect;}
	CCEFrameStatic();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEFrameStatic)
protected:
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CCEFrameStatic();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCEFrameStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

class CCEFramePartStatic : public CStatic
{
public:
	int			m_nStyle;
	void SetSkinStyle();
	TextStruct	*m_pTxtStruct;
	int         m_ntxtCount;
	CFont		m_Font;
	// Construction
public:
	//nStyle  0 ����װ�� (������ MP3���ŵȴ���)  1 ��װ��(�½���Ƭ ����)
	BOOL Create(CRect &rect, CWnd *pParentWnd, int nStyle = 0);
	//��Ӵ�������Ҫ��ʾ������,�����ֵ��û����Ϣ��Ӧ
	void SetTextStruct(TextStruct *pTxtStruct, int ncount);
	void ChangeTextStruct(int nIndex, TextStruct txtStruct);
	CCEFramePartStatic();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEFrameStatic)
protected:
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CCEFramePartStatic();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCEFramePartStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CESTATIC_H__CFA72294_8208_4505_AFF8_8EB32D54A4D8__INCLUDED_)
