#if !defined(AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A3__INCLUDED_)
#define AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEComboBox_.h : header file
//
#include "CEStatic.h"
#include "CeBtnST.h"
#include "URLEdit.h"

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCEComboBox_ window
#define IDC_COMBO_BTN		0xFFFD
#define IDC_COMBO_COMBO		0xFFFC
    class CCEComboBox_ : public CComboBox
    {
    public:
        URLEdit m_edit;
        //CListBox m_listbox;
        int m_nCtrlID;
        CFont m_font;
        int m_nFontHeight;
        BOOL Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID = 0xFFFF);
        //void SetWindowText_(CString s);
    public:
        CCEComboBox_();
    public:
        virtual ~CCEComboBox_();

        // Attributes
    public:

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCEComboBox_)
        //}}AFX_VIRTUAL

        // Implementation

        // Generated message map functions
    protected:
        //{{AFX_MSG(CCEComboBox_)
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg void OnDestroy();
        //afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
        //afx_msg void OnButton1();
        //afx_msg void OnSelchangeCombo1();
        //afx_msg void OnEditChange();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_CECOMBOBOX__H__2F93731D_80A7_4CC3_B48A_2FA5B2F6F5A3__INCLUDED_)
