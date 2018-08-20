#if !defined(AFX_CONTACTINFOSEARCHDLG_H__0157F3C1_C2B2_4FA5_860E_1DA22489E14E__INCLUDED_)
#define AFX_CONTACTINFOSEARCHDLG_H__0157F3C1_C2B2_4FA5_860E_1DA22489E14E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactInfoSearchDlg.h : header file
#include "../Resource.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CEDialog.h"

#include "../Control/MJPGStatic.h"
#include "../Control/DateCtrl.h"

namespace View {
    using namespace Control;
    /////////////////////////////////////////////////////////////////////////////
    // CContactInfoSearchDlg dialog

    class CContactInfoSearchDlg : public Control::CCEDialog
    {
        // Construction
    public:
        CContactInfoSearchDlg(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CContactInfoSearchDlg)
        enum { IDD = IDD_DIALOG_CONTACTINFOSEARCH };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CContactInfoSearchDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CContactInfoSearchDlg)
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        afx_msg void OnButtonSearchcinfoCancel();
        afx_msg void OnButtonSearchcinfoSearch();
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        DECLARE_MESSAGE_MAP()
    private:
        CMJPGStatic		m_MJPGList;
        CFont m_Font;

        CCEStatic m_sticTitle;

        CCEStatic m_sticTip;
        CCEStatic m_sticTimeRange;
        CCEStatic m_sticFrom;
        CCEStatic m_sticTo;
        CCEStatic m_sticName;
        CCEStatic m_sticNumber;
        CCERectEdit m_edtName;
        CCERectEdit m_edtNumber;
        CDateCtrl m_dtFrom;
        CDateCtrl m_dtTo;

        CCEBmpButton m_btnSearch;
        CCEBmpButton m_btnCancel;

        CCEFramePartStatic m_sticBackground;
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_CONTACTINFOSEARCHDLG_H__0157F3C1_C2B2_4FA5_860E_1DA22489E14E__INCLUDED_)
