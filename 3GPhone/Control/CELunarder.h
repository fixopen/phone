#ifndef __CONTROL_CELUNARDER_H__
#define __CONTROL_CELUNARDER_H__

#include "CEStatic.h"
#include "CeBtnST.h"

namespace Control {
    class CCELunarder : public CStatic
    {
    public:
        CCEStatic	*m_pStcYear, *m_pStcMonth;
        CCEStatic   m_StcWeek;
        CCEStatic	m_StcDay[49];
        CCEStatic  m_tipStatic;

        //	CCEButtonST m_BtnYleft, m_BtnYRight, m_BtnMlelt, m_BtnMright;

        UINT16 m_nY;
        UINT8  m_nM, m_nD;
        BOOL   m_bIsTipStatic;
        void SetIsTipStatic(BOOL flag) {m_bIsTipStatic = flag;}

        BOOL Create(CCEStatic *pStatic1, CCEStatic *pStatic2, int style, CRect &rect, CWnd *pParentWnd, int nCtrlID);
        void SetData(UINT16 year, UINT8 month, UINT8 day, BOOL reDraw = FALSE);
        void GetData(UINT16 &year, UINT8 &month, UINT8 &day);
        void SetStcDay();
        void SetStatic(CCEStatic *pStatic1, CCEStatic *pStatic2);
        void OnYleft();
        void OnYright();
        void OnMleft();
        void OnMright();
        BOOL IsHaveNote(int y, int m ,int d, CString &s);
        // Construction

    public:
        CCELunarder();

        // Attributes
    public:

        // Operations
    public:

        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CCELunarder)
        //}}AFX_VIRTUAL

        // Implementation
    public:
        virtual ~CCELunarder();

        // Generated message map functions
    protected:
        //{{AFX_MSG(CCELunarder)
        afx_msg void OnPaint();
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg LRESULT OnDaySelect(WPARAM w, LPARAM l);
        afx_msg void OnTimer(UINT nID);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
    };

    /////////////////////////////////////////////////////////////////////////////

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif //__CONTROL_CELUNARDER_H__
