#if !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B440012__INCLUDED_)
#define AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B440012__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// YHTelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GHomeJoyDlg dialog
#include "../control/MJPGStatic.h"
#include "../Logical/LunarderDate.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"
#include "../Data/SoundSegment.h"
#include "../Data/Data.h"
#include "PlaySoundDlg.h"
#include "SoundSearchDlg.h"
#include "passwordDlg.h"
#include "RecordSoundDlg.h"
#include "RenameDlg.h"

class C3GHomeJoyDlg : public CDialog {
    // Construction
public:
    CMJPGStatic m_MJPGList;
    C3GHomeJoyDlg(CWnd* pParent = NULL);   // standard constructor
    CRecordSoundDlg* m_pRecordSoundDlg;
    CSoundSearchDlg* m_pSoundSearchDlg;
    CPasswordDlg* m_pPasswordDlg;
    CPlaySoundDlg* m_pPlaySoundDlg;
    CRenameDlg* m_pRenameDlg;

    void SetUnitFont();
    void OnClearAll();
    void ShowItemsInList(int type);
    void ShowArrayInList();
    void SetShowStatus(int unitNO, BOOL status);
    int SetPlayList(TCHAR* dir, int local);
    void SetUnitIsAvailable(int type);
    void OnSelectAll();
    void OnButtonCheck(int uintNO);
    void FindSelectPage();
    void PageUp();
    void PageDown();
    void OnShiftFile();
    void OnClickDelete();
    void DeleteSelectedItem();
    void OnExit_();
    void OnPlayer(int index = 0);
    void PlaySound();
    void ShowWindow_(int nCmdShow);
    void ShowRecord(std::string filter);
    void OnButtonRecord();
    void OnButtonSetTip();
    void OnButtonSearch();
    void Search(std::string filter);
    void AddFileMp3();
    void SetInitShow();

    // Dialog Data
    //{{AFX_DATA(C3GHomeJoyDlg)
    enum { IDD = IDD_DIALOG_CACULATER };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA
private:
    std::vector<CString> m_fileMp3;
    std::vector<CString> m_fileDir;
    std::vector<int> m_isFileSelect;
    std::vector<boost::shared_ptr<Data::SoundSegment> > m_vCurrentResult;

    unsigned int m_uiType;
    unsigned int m_fileCount;
    unsigned int m_currentPage;
    unsigned int m_pageSize;
    unsigned int m_pageCount;
    unsigned int m_offSet;
    unsigned int m_firstSelPage;
    unsigned int m_lastSelPage;
    unsigned int m_cardType2;
    unsigned int m_cardType3;
    int m_soundType;

    TCHAR m_chDir[128];
    BOOL m_isSelectAll;
    BOOL m_UnitStatus[5];
    bool m_bTelephoneInUse;

    std::string m_sListSearchFilter;
    std::string m_sListFilter;
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(C3GHomeJoyDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(C3GHomeJoyDlg)
    virtual BOOL OnInitDialog();
    //  afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT OnDeviceChange(WPARAM w, LPARAM l);
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    afx_msg LRESULT OnRename(WPARAM w, LPARAM l);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YHTELDLG_H__C94EBB4B_B3AB_4063_9DEB_23E73E760B44__INCLUDED_)
