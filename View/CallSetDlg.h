#if !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B01__INCLUDED_)
#define AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fastdialsdlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CELineEdit.h"
#include "../Control/CECombobox_.h"
#include "../Control/CERadioButton.h"
#include "../Control/CEDialog.h"

#include "../Data/Setting.h"

#include "../control/MJPGStatic.h"
#include "../Util/ATCommandWarp.h"
/////////////////////////////////////////////////////////////////////////////
// CCallSetDlg dialog

class CCallSetDlg : public CCEDialog {
    // Construction
public:
    PhoneCallTRans m_calltrans;
    PhoneCallWaiting m_callwaiting;
    PhoneCallLimit m_calllimit;

    CCERectEdit m_edtTransferNumber;
    CCERectEdit m_edtPinNumber1;
    CCERectEdit m_edtPinNumber2;

    int m_uiType;
    void SetCallSetParam();
    void ShowItemsControl();

    CMJPGStatic m_MJPGList;
    CCallSetDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CCallSetDlg)
    enum { IDD = IDD_DIALOG_FASTDIALS };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCallSetDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CCallSetDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    afx_msg void OnButtonFastDialsOk();
    afx_msg void OnButtonFastDialsCancel();
    afx_msg LRESULT  OnClickMJPG(WPARAM w, LPARAM l);
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_)
