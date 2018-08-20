#if !defined(AFX_SIMIMPORTDLG_H__D34B822E_F946_480E_98CA_6451685DC58F__INCLUDED_)
#define AFX_SIMIMPORTDLG_H__D34B822E_F946_480E_98CA_6451685DC58F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SIMImportDlg.h : header file
//
#include "../Resource.h"
#include "../Control/CEDialog.h"
#include "../Control/MJPGStatic.h"
#include "../Control/CEComboBox_.h"
#include "../Control/CEProcessBar.h"

#include "../Data/Contact.h"
/////////////////////////////////////////////////////////////////////////////
// CSIMImportDlg dialog

namespace View {
    using namespace Control;
    class CSIMImportDlg : public CCEDialog
    {
    private:
        CEdit			m_Edit1;
        CStatic			m_Static1;
        CMJPGStatic		m_MJPGList;
        CCEProcessBar m_procbar;
        BOOL		  m_bIsSIM;

        int				m_nSIMID;
        void WriteSIMTelPhoneBOOK(CString name, CString telcode);

        Util::shared_ptr<Data::Contact> m_pContact;
        // Construction
    public:
        CCEComboBox_ m_cmbGroup;

        CSIMImportDlg(CWnd* pParent = NULL);   // standard constructor

        //
        CString gEditList;
        int gIndex;
        int gCmdIndex;
        int gRecordCount;
        int gValidRecordCount;
        int	m_nRecordeSize;

        void InitSIM();
        void SendCMDSIM(int nIndex);


        // Dialog Data
        //{{AFX_DATA(CSIMImportDlg)
        enum { IDD = IDD_DIALOG_SIMIMPORT };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CSIMImportDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CSIMImportDlg)
        afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
        virtual BOOL OnInitDialog();
        afx_msg void OnTimer(UINT nIDEvent);
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_SIMIMPORTDLG_H__D34B822E_F946_480E_98CA_6451685DC58F__INCLUDED_)
