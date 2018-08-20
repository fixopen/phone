#if !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
#define AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContactDlg.h : header file
#include "../Resource.h"
#include "ContactSearchDlg.h"
#include "SIMImportDlg.h"
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/SoftKey.h"
#include "../Control/MJPGStatic.h"
#include "../Data/Contact.h"

/////////////////////////////////////////////////////////////////////////////
// CContactDlg dialog
namespace View {
    using namespace Control;
    using namespace Data;
    class CContactDlg : public CDialog
    {
        // Construction
    public:
        CContactDlg(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CContactDlg)
        enum { IDD = IDD_DIALOG_CONTACT };
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
        afx_msg LRESULT OnSoftKeyMessage(WPARAM wParam, LPARAM lParam);   
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        afx_msg LRESULT OnListCtrlClick(WPARAM w, LPARAM l);
        afx_msg void OnTimer(UINT nIDEvent);

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
        CSIMImportDlg     *m_pSimImportDlg;
        int GetContactTotal() { return ContactTotal; };

    public:
        const int ContactTotal;
        const int ContactGroupTotal;
        const int PageSize;
    private:
        std::string m_sListFilter;
        std::string m_sListSearchFilter;
        std::vector<Util::shared_ptr<Data::Contact> > m_vCurrentResult;
        void ShowArrayInList(std::vector<Util::shared_ptr<Data::Contact> > array);
        std::string GetPYIndex(void);

        CContactSearchDlg* m_pContactSearchDlg;
        void ScrollItemsInList(int step, int nPos);

    public:
        void ShowItemsInList(void);
        void ShowTypeItems(void);
        int GetTypeListSelected(void);
        void SetTypeListSelected(int index);
        void Search(std::string filter);
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_CONTACTDLG_H__8E8BDFEC_AA75_4628_8820_CDAA92906516__INCLUDED_)
