#if !defined(AFX_SOUNDDLG_H__641E06F2_DBB9_4EFF_96A3_2B8626006167__INCLUDED_)
#define AFX_SOUNDDLG_H__641E06F2_DBB9_4EFF_96A3_2B8626006167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoundDlg.h : header file
//
#include "../Control/CEListCtrl.h"
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Data/SoundSegment.h"
#include "PlaySoundDlg.h"
#include "SoundSearchDlg.h"
#include "passwordDlg.h"
#include "RecordSoundDlg.h"

#include "../control/MJPGStatic.h"
#include "RenameDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSoundDlg dialog

class CSoundDlg : public CDialog
{
    // Construction
public:
    CSoundDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CSoundDlg)
    enum { IDD = IDD_DIALOG_SOUND };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSoundDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSoundDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    afx_msg void OnClickListType(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickListList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonSound();
    afx_msg void OnButtonRecord();
    afx_msg void OnButtonSearch();
    afx_msg void OnButtonSetTip();
    afx_msg void OnButtonDelete();
    afx_msg void OnButtonClose();
    afx_msg LRESULT OnClickMJPG(WPARAM w, LPARAM l);
    afx_msg LRESULT OnListCltrlClick(WPARAM w, LPARAM l);
    afx_msg LRESULT OnRename(WPARAM w, LPARAM l);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    DECLARE_MESSAGE_MAP()

private:
    CRenameDlg		*m_pRenameDlg;
    CMJPGStatic		m_MJPGList;
    /*
    CCEStatic	m_sticTypeTitle;
    CCEStatic	m_sticSound;
    CCEStatic	m_sticNameNO;
    CCEStatic	m_sticTime;
    CCEStatic	m_sticDuration;
    CCEStatic	m_sticOperation;
    CCEStatic	m_sticSepLine1;
    CCEStatic	m_sticSepLine2;
    CCEStatic   m_sticPanel;
    CCEListCtrl	m_lsType;
    */
    CCEListCtrl	m_lsList;

    CImageList* m_pImageList;
    /*
    CCEBmpButton m_btnSound;
    CCEBmpButton m_btnRecord;
    CCEBmpButton m_btnSearch;
    CCEBmpButton m_btnSetTip;
    CCEBmpButton m_btnDelete;
    CCEBmpButton m_btnClose;

    CCEFrameStatic m_sticBackground;
    */
    CSoundSearchDlg* m_pSoundSearchDlg;
    CPasswordDlg* m_pPasswordDlg;
    bool m_bTelephoneInUse;

    void SetButtonDefaultColor(CCEButtonST* button);
    void SetButtonSelectedColor(CCEButtonST* button);
    void ShowArrayInList(std::vector<boost::shared_ptr<Data::SoundSegment> > array, int type = -1);
    void DeleteSelectedItem(void);

    const int SoundTotal;
    const int PageSize;
    unsigned int m_uiSelectIndex;
    unsigned int m_uiType;

    std::string m_sListFilter;
    std::string m_sListSearchFilter;
    std::vector<boost::shared_ptr<Data::SoundSegment> > m_vCurrentResult;
    std::vector<boost::shared_ptr<Data::SoundSegment> > m_vSearchResult;
    void ScrollItemsInList(int step, int nPos);
public:
    CRecordSoundDlg* m_pRecordSoundDlg;
    CPlaySoundDlg* m_pPlaySoundDlg;

    void SaveSoundSegment(boost::shared_ptr<Data::SoundSegment> ss);
    void Search(std::string filter);
    void ShowItemsInList(int type);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDDLG_H__641E06F2_DBB9_4EFF_96A3_2B8626006167__INCLUDED_)
