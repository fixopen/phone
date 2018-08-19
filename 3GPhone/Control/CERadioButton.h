#ifndef __CONTROL_CERADIOBUTTON_H__
#define __CONTROL_CERADIOBUTTON_H__

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCERadioButton window
    int const GROUP_MAX = 12;
    class CCERadioButton : public CButton
    {
        // Construction
    public:
        BOOL     m_nCheck;
        CFont    m_font;
        COLORREF m_txtRGB, m_bkRGB;
        CCERadioButton	*m_GroupButton[GROUP_MAX];
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

        BOOL GetCheck_();
        void SetCheck_(BOOL nCheck);
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
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
        // Generated message map functions
    protected:
        virtual void PreSubclassWindow();
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
#endif //__CONTROL_CERADIOBUTTON_H__
