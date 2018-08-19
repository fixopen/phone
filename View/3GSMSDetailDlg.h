#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44_40_40__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GSMSDetailDlg dialog
#include "CardSelectDlg.h"
#include "../control/MJPGStatic.h"
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"
#include "../Data/Message.h"
#include "../Data/MMSData.h"
#include "../control/MmsShow.h"
#include "../control/CEComboBox_.h"

enum SMSDETAILTYPE {
    SMS_READ = 0,
    SMS_NEW,
    MMS_READ,
    MMS_NEW
};
class C3GSMSDetailDlg : public CDialog {
    // Construction
public:
    CCEComboBox_ m_cmbType;

    SMSDETAILTYPE m_nSMSType;           //0 短信阅读   1 写短信  2  彩信阅读  3  写彩信
    int m_nSMSId;           //记录在数据库中的位置
    CCELineEdit m_senderEdit;
    CCELineEdit m_contentEdit;
    MmsShow m_MMSShow;
    CString m_sReverse;

    void initDataBase(SMSDETAILTYPE type, int smsid = -1, BOOL reDraw = FALSE);
    void SetSMSDetail(CString Sender, CString Content);

    boost::shared_ptr<Data::Message> m_pMessageData;
    boost::shared_ptr<Data::MMSData> m_pMMSData;

    CMJPGStatic m_MJPGList;
    C3GSMSDetailDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(C3GSMSDetailDlg)
    enum { IDD = IDD_DIALOG_CONTACT };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(C3GSMSDetailDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

private :
    CCardSelectDlg m_dlgCardSelect; 

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(C3GSMSDetailDlg)
    virtual BOOL OnInitDialog();
    //  afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    afx_msg LRESULT OnDeleteItem(WPARAM w, LPARAM l);
    afx_msg LRESULT OnComboSelect(WPARAM w, LPARAM l);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
