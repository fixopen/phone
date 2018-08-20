#if !defined(AFX_MAINMP3DLG_H__47B29B59_B76C_4377_A489_1AF444DF996B__INCLUDED_)
#define AFX_MAINMP3DLG_H__47B29B59_B76C_4377_A489_1AF444DF996B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainMp3Dlg.h : header file
//

#include "../control/CeBtnST.h"
#include "../control/CEListCtrl.h"
#include "../control/CEStatic.h"
#include "../control/CEProcessBar.h"
#include "../Control/MJPGStatic.h"
#include "../resource.h"
#include "PlayerDlg.h"

#include "MidiPlayerControl.h"

/////////////////////////////////////////////////////////////////////////////
// CMainMp3Dlg dialog

class CMainMp3Dlg : public CDialog
{
public:
	BOOL				m_bIsPausebyEvent;
	TCHAR				m_chDir[128];
	
	CCEProcessBar		m_prgPlayTime;
	CMJPGStatic			m_MJPGList;
	CCEMoveTxtStatic	*m_MoveText;
	
	BOOL m_isMidiMusic;
	BOOL m_isMidiInit;
	BOOL m_isPlayerShow;
	int  m_ClickType;
	int  m_IsPlay;
	int  m_PreOrBack;  //0代表正常，1代表前一首，2代表下一首
	BOOL m_IsSound;
	int  m_Volume;
	BOOL m_isAllScreenPlay;
	unsigned int m_pageSize;
	unsigned int m_selectPageCount;//被选中文件总页数
	unsigned int m_selectCurrentPage;//当前显示的页数
	std::vector<CString> m_MP3List;
	std::vector<CString> m_ShowList;

	//local 0初级阶段
	bool SetMP3(CString filename);
	bool PlayAudio(CString filename,int nVolue = 20);
	void EliminateMidi();
	void OnExit_(BOOL isStopMusic);
	void ChangeVolume(int w);
	void OnFirst();
	void OnLast();
	void ShowArrayInList(std::vector<CString> fileName);
	void PageUp();
	void PageDown();
	void ClearAll();
	void CalculatePage(int dataCount);
	void OnClickShowList(int unitNO);
	void OnDBClickShowList(int unitNO);

	void HandleAudio(bool bt);


public:
	CPlayerDlg	*playerDlg_; 

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOpenFile();

// Construction
public:
	CMainMp3Dlg(CWnd* pParent = NULL);   // standard constructor
	
	MediaPlayer::MidiPlayer *pTheMidiPlayer;
	afx_msg void OnExit();  //wangzhenxing20100527
// Dialog Data
	//{{AFX_DATA(CMainMp3Dlg)
	enum { IDD = IDD_DIALOG_CACULATER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
private:
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainMp3Dlg)
	public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainMp3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPlayer();
	afx_msg void OnStop();
	afx_msg void OnPre();
	afx_msg void OnBack();
	afx_msg void OnMute();
	afx_msg void OnPregress(WPARAM w, LPARAM l);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized); 
	afx_msg void OnOutEvent(WPARAM w, LPARAM l);
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINMP3DLG_H__47B29B59_B76C_4377_A489_1AF444DF996B__INCLUDED_)
