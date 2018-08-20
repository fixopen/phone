#ifndef __VIEW_MAINDIALOG_H__
#define __VIEW_MAINDIALOG_H__

#include "../Resource.h"

namespace View {
    class CMainDialog : public CDialog
    {
        // Construction
    public:
        CMainDialog(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CMainDialog)
        enum { IDD = IDD_DIALOG_MAIN };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMainDialog)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CMainDialog)
        // NOTE: the ClassWizard will add member functions here
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif //__VIEW_MAINDIALOG_H__
