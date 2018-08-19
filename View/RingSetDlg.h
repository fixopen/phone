#if !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B02__INCLUDED_)
#define AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fastdialsdlg.h : header file
//
#include "../Control/CELineEdit.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
#include "../Control/CECombobox_.h"

#include "../Data/Setting.h"

#include "../control/MJPGStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CRingSetDlg dialog

class CRingSetDlg : public CCEDialog
{
    // Construction
public:
    boost::shared_ptr<Data::Setting>  m_Data;
    void SetRingSetParam(	boost::shared_ptr<Data::Setting> data);
    void SetRingLst(TCHAR *dir);
    CRingSetDlg(CWnd* pParent = NULL);   // standard constructor

    CCEButtonST m_chbEnableRing;
    CCEComboBox_ m_cmbRing;
    CCEComboBox_ m_cmbRingVolume;

    CCEButtonST m_chbEnableSMSRing;
    CCEComboBox_ m_cmbSMSRing;
    CCEComboBox_ m_cmbSMSRingVolume;

    CCEButtonST m_chbEnableSpecRing;
    CCEComboBox_ m_cmbSpecRing;
    CCEComboBox_ m_cmbSpecRingVolume;

    // Dialog Data
    //{{AFX_DATA(CRingSetDlg)
    enum { IDD = IDD_DIALOG_FASTDIALS };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRingSetDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CRingSetDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    afx_msg void OnButtonFastDialsOk();
    afx_msg void OnButtonFastDialsCancel();
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    afx_msg LRESULT OnComboSelect(WPARAM w, LPARAM l);
    DECLARE_MESSAGE_MAP()

private:
    CMJPGStatic		m_MJPGList;
    CCERectEdit m_edtNumber[3];

    //	CCEBmpButton m_btnOk;
    //	CCEBmpButton m_btnCancel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTDIALSDLG_H__6F1EB099_1F0C_487C_AAED_221F5FB6088B__INCLUDED_)
