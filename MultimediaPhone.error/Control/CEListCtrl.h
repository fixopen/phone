#if !defined(AFX_CELISTCTRL_H__EE01D1C7_DFBF_4D27_BBBE_99C875563597__INCLUDED_)
#define AFX_CELISTCTRL_H__EE01D1C7_DFBF_4D27_BBBE_99C875563597__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEListCtrl.h : header file
//

#include "CEScrollbar_.h"

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCEListCtrl window

    class CCEListCtrl : public CListCtrl
    {
    public:
        int			m_nListIndex;
        CCEScrollBar_ m_scollbar_;
        COLORREF	m_RGB1;
        COLORREF	m_RGB2;
        int			m_nLineHeight;
        int			m_nPage;
        CFont m_font;

        BOOL		m_bIsScroll;	//记录是否滚动
        BOOL        m_bIsSelfScroll;
        // Construction
    public:
        //BOOL isScroll 是否滚动
        BOOL Create(DWORD dwStyle, CRect &rect, CWnd *pParentWnd, UINT nID, BOOL isScroll = TRUE, int Scroll_style = 0, BOOL isSelfScroll = TRUE);
        CCEListCtrl();

        // Attributes
    public:
        //设置行差色颜色值
        void SetListColor(COLORREF RGB1, COLORREF RGB2);
        //设置滚动条的范围，根据ListCtrl 设置 滚动范围，翻页值
        void SetScrollRagle(BOOL redraw = TRUE);
        void ShowWindow_(int nCmdShow);
        void SetScrollPos(int pos, BOOL redraw);
        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCEListCtrl)
        virtual BOOL PreTranslateMessage(MSG* pMsg);
        //}}AFX_VIRTUAL

        // Implementation
    public:
        virtual ~CCEListCtrl();
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        // Generated message map functions
    protected:
        //{{AFX_MSG(CCEListCtrl)
        // NOTE - the ClassWizard will add and remove member functions here.
        afx_msg void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg void OnPaint();
        afx_msg BOOL OnClicked();
        afx_msg void OnTimer(UINT nIDEvent);
        afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
        //}}AFX_MSG
        virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_CELISTCTRL_H__EE01D1C7_DFBF_4D27_BBBE_99C875563597__INCLUDED_)
