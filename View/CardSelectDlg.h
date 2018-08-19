#if !defined CARDCONTACT
#define CARDCONTACT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactDlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/SoftKey.h"
#include "../Data/Contact.h"
#include "ContactSearchDlg.h"

#include "../Util/ATCommandWarp.h"
//#include "SIMImportDlg.h"

#include "../control/MJPGStatic.h"
#include "../control/CEDialog.h"

#define  IDC_BTN_SELECTALL 1000
#define  IDC_BTN_CANCELALL 1001
//#define  IDC_BTN_OK		   1002
//#define  IDC_BTN_CANCEL	   1003

#define  MAX_SEND_CONTACT  50

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog

class CCardSelectDlg : public CDialog
{
    // Construction
public:
    CCardSelectDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CContactDlg)
    enum { IDD = IDD_DIALOG_TELEPHONE };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CContactDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CContactDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickListList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonContactNew();
    afx_msg void OnButtonContactNewtype();
    afx_msg void OnButtonContactEdittype();
    afx_msg void OnButtonContactSearch();
    afx_msg void OnButtonContactClose();
    afx_msg LRESULT  OnListCltrlClick(WPARAM w, LPARAM l);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnBtnSelectAll();
    afx_msg void OnBtnCancelAll();
    afx_msg void OnBtnOk();
    afx_msg void OnBtnCancel();

    DECLARE_MESSAGE_MAP()
private:
    CMJPGStatic		m_MJPGList;
    /*
    CCEStatic	m_sticTypeTitle;
    CCEStatic	m_sticName;
    CCEStatic	m_sticCompany;
    CCEStatic	m_sticDepartment;
    CCEStatic	m_sticDuty;
    CCEStatic	m_sticOperation;
    CCEStatic	m_sticSepLine1;
    CCEStatic	m_sticSepLine2;
    */	CCEStatic   m_sticPinYin;
    /*	CCEStatic   m_sticPanel;
    CCEFrameStatic m_sticBackground;
    */
    CCEListCtrl	m_lsType;
    CCEListCtrl	m_lsList;
    CSoftKey    m_skSoftKey;
    CImageList* m_pImageList;

    std::vector<Util::ATCommandWarp::SIM_FORMAT> m_vContact;//联系人容器
    BOOL m_bSelectAll ;//选中所有的
    CCEButtonST m_btnSelectAll ;//全选
    CCEButtonST	m_btnCancelAll ;//取消全选
    CCEButtonST m_btnOK ;//确定
    CCEButtonST m_btnCancel ; //取消

    /*
    CCEBmpButton m_btnNewContact;
    CCEBmpButton m_btnNewType;
    CCEBmpButton m_btnEditType;
    CCEBmpButton m_btnSearch;
    CCEBmpButton m_btnClose;
    */	

    void SetButtonDefaultColor(CCEButtonST* button);
    void SetButtonSelectedColor(CCEButtonST* button);
public:
    //	CSIMImportDlg     *m_pSimImportDlg;
    int GetContactTotal() { return ContactTotal; };
public:
    const size_t ContactTotal;
    const size_t ContactGroupTotal;
    const int PageSize;
private:
    std::string m_sListFilter;
    std::string m_sListSearchFilter;
    std::vector<boost::shared_ptr<Data::Contact> > m_vCurrentResult;
    void ShowArrayInList(std::vector<boost::shared_ptr<Data::Contact> > array);
    std::string GetPYIndex(void);

    CContactSearchDlg* m_pContactSearchDlg;
    void ScrollItemsInList(int step, int nPos);

public:
    void ShowItemsInList(void);
    void ShowTypeItems(void);
    int  GetTypeListSelected(void);
    void SetTypeListSelected(int index);
    void Search(std::string filter);

    // add by qi 2009_08_05
    void GetSendContact(std::vector<Util::ATCommandWarp::SIM_FORMAT> &vetor);
    void ShowCard();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
