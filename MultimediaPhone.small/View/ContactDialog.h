#ifndef __VIEW_CONTACTDIALOG_H__
#define __VIEW_CONTACTDIALOG_H__

#include "../Resource.h"

namespace View {
    class CContactDialog : public CDialog
    {
        // Construction
    public:
        CContactDialog(CWnd* pParent = NULL);   // standard constructor

        // Dialog Data
        //{{AFX_DATA(CContactDialog)
        enum { IDD = IDD_DIALOG_CONTACT };
        // NOTE: the ClassWizard will add data members here
        //}}AFX_DATA


        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CContactDialog)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:

        // Generated message map functions
        //{{AFX_MSG(CContactDialog)
        // NOTE: the ClassWizard will add member functions here
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
    };

    //{{AFX_INSERT_LOCATION}}
    // Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif //__VIEW_CONTACTDIALOG_H__
