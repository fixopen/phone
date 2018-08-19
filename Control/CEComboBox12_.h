#if !defined(AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A312__INCLUDED_)
#define AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A312__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEComboBox_.h : header file
//
#include "CEStatic.h"
#include "CeBtnST.h"
/////////////////////////////////////////////////////////////////////////////
// CCEComboBox12_ window
#define IDC_COMBO_BTN		0xFFFD
#define IDC_COMBO_COMBO		0xFFFC
class CCEComboBox12_ : public CStatic
{
public:
	COLORREF    m_backRGB;
	CCEStatic	m_Static;
	CCEBmpButton m_Btn;
	CComboBox	*m_pCombo;
	int			m_nCtrlID;
	CFont		m_font;
	int			m_nFontHeight;
	CRect       m_rt;
	BOOL CreateEx(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID, int fontSize, int btnWidth, int btnHeight, int bitmapType = 0);
	BOOL Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID= 0xFFFF);

	int GetCount() {return m_pCombo->GetCount();}
	int GetCurSel() {return m_pCombo->GetCurSel();}
	int SetCurSel(int nSelect){
		int ret= m_pCombo->SetCurSel(nSelect);
		CString s;
		m_pCombo->GetWindowText(s);
		m_Static.SetWindowText(s);
		return ret;
	}

	// manipulating listbox items
	int FindString(int nStartAfter, LPCTSTR lpszString) {return m_pCombo->FindString(nStartAfter, lpszString);}
	int SelectString(int nStartAfter, LPCTSTR lpszString){
		int ret = m_pCombo->SelectString(nStartAfter, lpszString);
		CString s;
		m_pCombo->GetWindowText(s);
		m_Static.SetWindowText(s);
		return ret;
	}
	int GetLBText(int nIndex, LPTSTR lpszText){return m_pCombo->GetLBText(nIndex, lpszText); }
	void GetLBText(int nIndex, CString& rString) {m_pCombo->GetLBText(nIndex, rString); };
	int AddString(LPCTSTR lpszString){return m_pCombo->AddString(lpszString);}
	int DeleteString(UINT nIndex){return m_pCombo->DeleteString(nIndex);}
	int InsertString(int nIndex, LPCTSTR lpszString){return m_pCombo->InsertString(nIndex, lpszString);}
	DWORD GetItemData(int nIndex) {return m_pCombo->GetItemData(nIndex);}
	int SetItemData(int nIndex, DWORD dwItemData){return m_pCombo->SetItemData(nIndex, dwItemData);}
	void SetWindowText_(CString s);
	void ResetContent(){m_pCombo->ResetContent();}
	void GetWindowText(CString &string);
	int GetLBTextLen(int nIndex){return m_pCombo->GetLBTextLen(nIndex);}
	void SetDroppedWidth(UINT nWidth){m_pCombo->SetDroppedWidth(nWidth);}
	void ShowDropDown(BOOL bShowIt = TRUE){m_pCombo->ShowDropDown(bShowIt);}
	void ReCreateComm();
// Construction
public:
	CCEComboBox12_();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEComboBox12_)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEComboBox12_();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCEComboBox12_)
	afx_msg void OnPaint();
	afx_msg void OnButton1();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelendcancelCombo1();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A3__INCLUDED_)
