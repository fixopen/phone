#if !defined(CSHIFT_)
#define CSHIFT_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShiftFile.h : header file
//

#include "../Control/MJPGStatic.h"
#include "../Control/CEDialog.h"
#include "../Control/CEProcessBar.h"

/////////////////////////////////////////////////////////////////////////////
// CShiftFile dialog

class CShiftFile : public CCEDialog {
    // Construction
public:
    CShiftFile(CWnd* pParent = NULL);   // standard constructor
    CMJPGStatic m_MJPGList;
    CCEProcessBar* m_shiftProc;

    unsigned int m_fileCount;
    unsigned int m_currentFile;
    std::vector<CString> m_fileList;

    ULARGE_INTEGER m_freeBytes;
    ULARGE_INTEGER m_totalBytes;
    ULARGE_INTEGER m_totalFreeBytes;

    void SetHWnd(HWND handle);
    void OnClickCancel();

    HWND m_handle;
    // Dialog Data
    //{{AFX_DATA(CShiftFile)
    enum { IDD = IDD_DIALOG_SIMIMPORT};
    //}}AFX_DATA
private:
    BOOL m_bIsTipModel;
    BOOL m_bCheckStatus;

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CShiftFile)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    // Implementation

protected:

    // Generated message map functions
    //{{AFX_MSG(CShiftFile)
    virtual BOOL OnInitDialog();
    afx_msg void OnTimer(UINT nIDEvent);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    afx_msg void OnClickOk();
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHIFTFILE_H__EE4E4F43_0B66_43A1_BEE3_67561E3E4197__INCLUDED_)
