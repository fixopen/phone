#if !defined(AFX_PLAYSOUNDDLG_H__7CBB5B9B_6102_4C16_8872_30F6FBB5A542__INCLUDED_)
#define AFX_PLAYSOUNDDLG_H__7CBB5B9B_6102_4C16_8872_30F6FBB5A542__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// playsounddlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEProcessBar.h"
#include "../Control/CEDialog.h"
#include "../Data/SoundSegment.h"

/////////////////////////////////////////////////////////////////////////////
// CPlaySoundDlg dialog

class CPlaySoundDlg : public CCEDialog
{
// Construction
public:
	CPlaySoundDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlaySoundDlg)
	enum { IDD = IDD_DIALOG_PLAYSOUND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlaySoundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlaySoundDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg void OnButtonClose();
	afx_msg void OnButtonNext();
	afx_msg void OnButtonPrev();
	afx_msg void OnButtonPlay();
	afx_msg void OnCheckPlaycontinue();
	DECLARE_MESSAGE_MAP()
private:
	CCEStatic m_sticTitle;
	CCEStatic m_sticNameNO;
	CCEStatic m_sticSoundName;
	CCEStatic m_sticSoundDuration;
	CCEProcessBar m_procbarSound;
	CCEBmpButton m_btnClose;
	CCEBmpButton m_btnPrev;
	CCEBmpButton m_btnNext;
	CCEBmpButton m_btnPlay;
	CCEButtonST m_chkPlayContinue;
	CCEFramePartStatic m_sticBackground;

	std::vector<boost::shared_ptr<Data::SoundSegment> > m_vSoundResult;
	int m_iCurrentSound;
	std::string m_sFilePath;
	int m_iType;
	std::string m_sSearchFilter;
	bool m_bHasNext;
private:
	int m_iModel;
	int m_iCurrentID;
	int m_iTotalSeconds;
	int m_iCurrentSecond;
	void ShowSeconds(void);

public:
	void SetSound(int type, int id, int dir = 0, std::string searchFilter = "");
	void SetModel(int model);
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYSOUNDDLG_H__7CBB5B9B_6102_4C16_8872_30F6FBB5A542__INCLUDED_)
