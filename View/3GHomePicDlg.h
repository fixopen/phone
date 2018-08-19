#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B440011__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B440011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GHomePicDlg dialog
#include "../control/MJPGStatic.h"
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"

class C3GHomePicDlg : public CDialog {
    // Construction
public:
    int m_callType; //调用类型
    CMJPGStatic m_MJPGList;

    C3GHomePicDlg(CWnd* pParent = NULL);   // standard constructor

    void SetUnitFont();
    void OnClearAll();
    void ShowItemsInList(int type);
    void ShowArrayInList(std::vector<CString> fileName);
    void SetShowStatus(int unitNO, BOOL status);
    int SetPlayList(TCHAR* dir);
    void OnSelectAll();
    void OnButtonCheck(int uintNO);
    void PageUp();
    void PageDown();
    void OnShiftFile();
    void OnClickDelete();
    void DeleteSelectedItem();
    void OnExit_();
    void OnOK_();
    void OnPlayer(int index = 0);
    void FindSelectPage();
    void ShowWindow_(int nCmdShow);
    void AddFilePicture();
    void SetInitShow();

    // Dialog Data
    //{{AFX_DATA(C3GHomePicDlg)
    enum { IDD = IDD_DIALOG_CACULATER };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
private:
    std::vector<CString> m_filePicture;
    std::vector<CString> m_fileDir;
    std::vector<int> m_isFileSelect;

    unsigned int m_cardType1;
    unsigned int m_cardType2;
    unsigned int m_uiType;
    unsigned int m_fileCount;
    unsigned int m_currentPage;
    unsigned int m_firstSelPage;
    unsigned int m_lastSelPage;
    unsigned int m_pageSize;
    unsigned int m_pageCount;
    unsigned int m_offSet;

    TCHAR m_chDir[128];
    BOOL m_isSelectAll;
    BOOL m_UnitStatus[5];
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(C3GHomePicDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(C3GHomePicDlg)
    virtual BOOL OnInitDialog();
    //  afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT OnDeviceChange(WPARAM w, LPARAM l);
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
