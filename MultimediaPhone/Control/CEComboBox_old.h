#if !defined(AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A3__INCLUDED_)
#define AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEComboBox_.h : header file
//
#include "CEStatic.h"
#include "CeBtnST.h"

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCEComboBox_ window
#define IDC_COMBO_BTN		0xFFFD
#define IDC_COMBO_COMBO		0xFFFC
    class CCEComboBox_ : public CComboBox
    {
    public:
        COLORREF    m_backRGB;
        CCEStatic	m_Static;
        CCEBmpButton m_Btn;
        CComboBox	m_Combo;
        int			m_nCtrlID;
        CFont		m_font;
        int			m_nFontHeight;

        BOOL Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID= 0xFFFF);

        int GetCount() {return m_Combo.GetCount();}
        int GetCurSel() {return m_Combo.GetCurSel();}
        int SetCurSel(int nSelect){
            int ret= m_Combo.SetCurSel(nSelect);
            CString s;
            m_Combo.GetWindowText(s);
            m_Static.SetWindowText(s);
            return ret;
        }

        // manipulating listbox items
        int FindString(int nStartAfter, LPCTSTR lpszString) {return m_Combo.FindString(nStartAfter, lpszString);}
        int SelectString(int nStartAfter, LPCTSTR lpszString){
            int ret = m_Combo.SelectString(nStartAfter, lpszString);
            CString s;
            m_Combo.GetWindowText(s);
            m_Static.SetWindowText(s);
            return ret;
        }
        int GetLBText(int nIndex, LPTSTR lpszText){return m_Combo.GetLBText(nIndex, lpszText); }
        void GetLBText(int nIndex, CString& rString) {m_Combo.GetLBText(nIndex, rString); };
        int AddString(LPCTSTR lpszString){return m_Combo.AddString(lpszString);}
        int DeleteString(UINT nIndex){return m_Combo.DeleteString(nIndex);}
        int InsertString(int nIndex, LPCTSTR lpszString){return m_Combo.InsertString(nIndex, lpszString);}
        DWORD GetItemData(int nIndex) {return m_Combo.GetItemData(nIndex);}
        int SetItemData(int nIndex, DWORD dwItemData){return m_Combo.SetItemData(nIndex, dwItemData);}
        void SetWindowText_(CString s);
        void ResetContent(){m_Combo.ResetContent();}
        void GetWindowText(CString &string);
        int GetLBTextLen(int nIndex){return m_Combo.GetLBTextLen(nIndex);}
        void SetDroppedWidth(UINT nWidth){m_Combo.SetDroppedWidth(nWidth);}
        void ShowDropDown(BOOL bShowIt = TRUE){m_Combo.ShowDropDown(bShowIt);}
        // Construction
    public:
        CCEComboBox_();

        // Attributes
    public:

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCEComboBox_)
        //}}AFX_VIRTUAL

        // Implementation
    public:
        virtual ~CCEComboBox_();

        // Generated message map functions
    protected:
        //{{AFX_MSG(CCEComboBox_)
        afx_msg void OnPaint();
        afx_msg void OnButton1();
        afx_msg void OnSelchangeCombo1();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A3__INCLUDED_)
