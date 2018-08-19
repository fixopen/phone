#if !defined(AFX_PLAYERDLG_H__8E2A082C_5F67_487C_9E41_4B41EEC535A4__INCLUDED_)
#define AFX_PLAYERDLG_H__8E2A082C_5F67_487C_9E41_4B41EEC535A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerDlg.h : header file
//


#include "player.h"

#include "../resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog
#include <vector>

class CPlayerDlg : public CDialog
{
public:
	Player   *player_;
	void SetPlayer(Player *player){player_ = player;}

	int isActiveMode_; //记录是否屏保  0 屏保 1 手动
	char m_chVideoFilename[128];
	void ReSetWindowsRect(CRect rt);
	void SetParam(char *videoFile, MediaType isVideo, BOOL isSaveScreen = FALSE);
	void SetImageList(char *DIR);
	void AddImageList(char *DIR[], int n);
	BOOL SetImageListForScreenSave(char *FileName);
	std::vector<CString> SetImageList_(char *DIR);
	void Show();
	void Hide();
	void SetResumeTimer();
	void StopTimer();

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
// Construction
public:
	CPlayerDlg( Player *player, CWnd* pParent = NULL);   // standard constructor
	void OnOk(void);
// Dialog Data
	//{{AFX_DATA(CPlayerDlg)
	enum { IDD = IDD_DIALOG_PLAYER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayerDlg)
	afx_msg LRESULT OnReceiveMsg(WPARAM w, LPARAM l);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERDLG_H__8E2A082C_5F67_487C_9E41_4B41EEC535A4__INCLUDED_)
