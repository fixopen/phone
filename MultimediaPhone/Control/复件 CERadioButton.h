#if !defined(AFX_CERADIOBUTTON_H__982EDB6F_7713_458A_A6DC_E8F39BB6247B__INCLUDED_)
#define AFX_CERADIOBUTTON_H__982EDB6F_7713_458A_A6DC_E8F39BB6247B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CERadioButton.h : header file
//

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCERadioButton window
#define GROUP_MAX		12
    class CCERadioButton : public CButton
    {
        // Construction
    public:
        //	int nCheck;
        CFont    m_font;
        COLORREF m_txtRGB, m_bkRGB;
        CButton	*m_GroupButton[GROUP_MAX];
        int m_nGroup;
        void SetColor(COLORREF txtRGB, COLORREF bkRGB){m_txtRGB = txtRGB; m_bkRGB = bkRGB;}
        void SetGroupButton(CButton *pButton[], int n);
        /*
        int GetCheck() const{return nCheck;}
        void SetCheck(int n){nCheck = n;}
        */	

        CCERadioButton();
        BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle,
            const RECT& rect, CWnd* pParentWnd, UINT nID = 0xFFFF);
        // Attributes
    public:

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCERadioButton)
        //}}AFX_VIRTUAL

        // Implementation
    public:
        virtual ~CCERadioButton();

    private:
        //virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
        // Generated message map functions
    protected:
        //virtual void PreSubclassWindow();
        //{{AFX_MSG(CCERadioButton)
        afx_msg void OnPaint();
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_CERADIOBUTTON_H__982EDB6F_7713_458A_A6DC_E8F39BB6247B__INCLUDED_)
