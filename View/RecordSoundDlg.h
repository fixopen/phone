#if !defined(AFX_RECORDSOUNDDLG_H__898FF679_030E_487D_B244_2313FC1CF54B__INCLUDED_)
#define AFX_RECORDSOUNDDLG_H__898FF679_030E_487D_B244_2313FC1CF54B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordSoundDlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEProcessBar.h"
#include "../Control/CEDialog.h"
#include "../Data/SoundSegment.h"
#include "../Util/OggCodec.h"
/////////////////////////////////////////////////////////////////////////////
// CRecordSoundDlg dialog
//#define WM_CHANGELANGUAGE (WM_USER+300)
class CRecordSoundDlg : public CCEDialog
{
// Construction
public:
	CRecordSoundDlg(CWnd* pParent = NULL);   // standard constructor
    void OnBtRecordplay();
	void OnBtRecord();
// Dialog Data
	//{{AFX_DATA(CRecordSoundDlg)
	enum { IDD = IDD_DIALOG_RECORDSOUND };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordSoundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecordSoundDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnMM_WIM_OPEN(UINT wParam,LONG lParam);
	afx_msg void OnMM_WIM_DATA(UINT wParam,LONG lParam);
	afx_msg void OnMM_WIM_CLOSE(UINT wParam,LONG lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnChangeLanguage(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnButtonSoundClose();
	afx_msg void OnButtonSoundClear();
	afx_msg void OnButtonSoundPlay();
	afx_msg void OnButtonSoundRecord();
	afx_msg void OnPlaySeek(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
private:
	int  m_nSoundindex;
	BOOL m_bISRecording;
	volatile BOOL m_bEnding;
	BOOL m_bPlaying;
	HWAVEIN m_hWaveIn;
	HWAVEOUT m_hWaveOut;
// 	PBYTE m_pBuffer1;
// 	PBYTE m_pBuffer2;
// 	PBYTE m_pOutBuffer;
// 	PWAVEHDR m_pWaveHdr1;
// 	PWAVEHDR m_pWaveHdr2;
	CFile m_fWav;	
	WAVEFORMATEX m_waveform;
	UINT m_uiRemainRecordSecond;
	BOOL m_bClear;
	UINT m_uiCurrentSecond;
	UINT m_uiTotalSecond;
	OggCodec* m_pOggCodec;
	BOOL m_bRecordOver;
	boost::shared_ptr<Data::SoundSegment> m_spSoundSegment;
	//Data::SoundSegmentType m_sstType;

	PBYTE m_pBuffer[WAVE_BUFFER_COUNT];
	PWAVEHDR m_pWaveHdr[WAVE_BUFFER_COUNT];


private:
	CCEStatic m_sticNameNO;
	CCEStatic m_sticSoundName;
	CCEStatic m_sticSoundDuration;
	CCEStatic m_sticRecordCaption;
	CCEStatic m_sticRecord;
	CCEProcessBar m_procbarSound;
	CCEBmpButton m_btnClose;
	CCEBmpButton m_btnClear;
	CCEBmpButton m_btnPlay;
	CCEBmpButton m_btnRecord;
	CCEBmpButton m_btnRecordTip;
	CCEFramePartStatic m_sticBackground;

	std::string GetSoundPath(void);
	std::string m_sFilePath;
	bool RecStart(void);
	bool RecStop(void);
	bool PlayStart(void);
	bool PlayStop(void);
	void ShowSeconds(void);
	
public:
	void CloseSound(void);
	void StartRecord(void);
	void ChangeLanguage();
	CString GenerateRecordName(BOOL isHighQuality, BOOL isProtect);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDSOUNDDLG_H__898FF679_030E_487D_B244_2313FC1CF54B__INCLUDED_)
