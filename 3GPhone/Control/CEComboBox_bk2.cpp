// CEComboBox_.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "CEComboBox_.h"
#include "../Data/SkinStyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace Control {
    /////////////////////////////////////////////////////////////////////////////
    // CCEComboBox_

    CCEComboBox_::CCEComboBox_()
        : CComboBox()
    {
       m_nFontHeight = 42;
    }

    CCEComboBox_::~CCEComboBox_()
    {
    }

    BOOL CCEComboBox_::Create(int style, CRect &rect, CWnd *pParentWnd, int nCtrlID)
    {
        //rect.bottom = rect.top + 30;
        //rect.right = rect.left + 200;
        CComboBox::Create(style | CBS_DROPDOWN | WS_VSCROLL, rect, pParentWnd, IDC_COMBO_COMBO);
       
        m_nCtrlID = nCtrlID;
        VERIFY(m_font.CreateFont(
            m_nFontHeight,                        // nHeight
            0,                         // nWidth
            0,                         // nEscapement
            0,                         // nOrientation
            FW_NORMAL,                 // nWeight
            FALSE,                     // bItalic
            FALSE,                     // bUnderline
            0,                         // cStrikeOut
            ANSI_CHARSET,              // nCharSet
            OUT_DEFAULT_PRECIS,        // nOutPrecision
            CLIP_DEFAULT_PRECIS,       // nClipPrecision
            DEFAULT_QUALITY,           // nQuality
            DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
            _T("ËÎÌו")));                 // lpszFacename
        SetFont(&m_font);

        return TRUE;
    }

    BEGIN_MESSAGE_MAP(CCEComboBox_, CComboBox)
        //{{AFX_MSG_MAP(CCEComboBox_)
        //ON_WM_CHAR()
        //ON_CBN_SELCHANGE(IDC_COMBO_COMBO, OnSelchangeCombo1)
        //ON_CBN_EDITCHANGE(IDC_COMBO_COMBO, OnEditChange)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    HBRUSH CCEComboBox_::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
    {
        if (nCtlColor == CTLCOLOR_EDIT)
        {
            //Edit control
            if (m_edit.GetSafeHwnd() == NULL)
                m_edit.SubclassWindow(pWnd->GetSafeHwnd());
        }
        //else if (nCtlColor == CTLCOLOR_LISTBOX)
        //{
        //    //ListBox control
        //    if (m_listbox.GetSafeHwnd() == NULL)
        //        m_listbox.SubclassWindow(pWnd->GetSafeHwnd());
        //}
        HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
        return hbr;
    }

    void CCEComboBox_::OnDestroy()
    {
        if (m_edit.GetSafeHwnd() != NULL)
            m_edit.UnsubclassWindow();
        //if (m_listbox.GetSafeHwnd() != NULL)
        //    m_listbox.UnsubclassWindow();
        CComboBox::OnDestroy();
    }

    //void CCEComboBox_::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
    //    if (nChar == VK_RETURN) {
    //        if (GetParent())
    //        {
    //            GetParent()->SendMessage(CBN_SELCHANGE, m_nCtrlID, m_nCtrlID); //WM_COMMBOX_CLICKED
    //        }
    //    } else {
    //        int i = 0;
    //        //m_edit.OnChar(nChar, nRepCnt, nFlags);
    //    }
    //}

    /////////////////////////////////////////////////////////////////////////////
    // CCEComboBox_ message handlers

    //void CCEComboBox_::OnSelchangeCombo1()
    //{
    //    if (GetParent())
    //    {
    //        GetParent()->SendMessage(CBN_SELCHANGE, m_nCtrlID, m_nCtrlID); //WM_COMMBOX_CLICKED
    //    }
    //}

    //void CCEComboBox_::OnEditChange()
    //{
    //    if (GetParent())
    //    {
    //        GetParent()->SendMessage(WM_COMMBOX_CLICKED, m_nCtrlID, m_nCtrlID);
    //    }
    //}

    //void CCEComboBox_::SetWindowText_(CString s)
    //{
    //    int nn = -1;
    //    int ncount = GetCount();
    //    for (int i = 0; i < ncount; ++i)
    //    {
    //        CString rString;
    //        GetLBText(i, rString);
    //        if (rString == s)
    //            SetCurSel(i);
    //    }
    //}
}
