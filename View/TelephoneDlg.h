#if !defined(AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_)
#define AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// telephonedlg.h : header file
//
#include "../Control/CEStatic.h"
#include "../Control/CeBtnST.h"
#include "../Control/CEDialog.h"
#include "../Data/ContactInfo.h"
#include "../Data/SoundSegment.h"
#include "../inc/SABTD.h"
#include "../Util/OggCodec.h"

#include "../control/MJPGStatic.h"


/////////////////////////////////////////////////////////////////////////////
// CTelephoneDlg dialog

class CTelephoneDlg : public CCEDialog
{
// Construction
public:
	BOOL m_bFirstConnect;
	CMJPGStatic		m_MJPGList;
	CString m_strTelStatus;
	int     m_nTelStatus;
	BOOL    m_bTelUsing;
	void DialSpeedCode(int index);

	CTelephoneDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL m_bRecording;
	UINT m_uiIgnoreRingCount;
	UINT m_uTelSecondOff;

// Dialog Data
	//{{AFX_DATA(CTelephoneDlg)
	enum { IDD = IDD_DIALOG_TELEPHONE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

		afx_msg void OnButtonTelephoneRecord();

	void ShowWindow_(int cmdshow);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelephoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTelephoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMM_WIM_OPEN(UINT wParam,LONG lParam);
	afx_msg void OnMM_WIM_DATA(UINT wParam,LONG lParam);
	afx_msg void OnMM_WIM_CLOSE(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_OPEN(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_DONE(UINT wParam,LONG lParam);
	afx_msg void OnMM_WOM_CLOSE(UINT wParam,LONG lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnButtonTelephoneHide();
	afx_msg void OnButtonTelephoneNote();
	afx_msg void OnButtonTelephoneHandle();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnTelStatus(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()

private:

	std::string path;
	char m_chDialNumber[64];
	
	CCEFramePartStatic m_sticBackground;

	boost::shared_ptr<Data::ContactInfo> m_spContactInfo;
	boost::shared_ptr<Data::SoundSegment> m_spSoundSegment;
	boost::shared_ptr<Data::ContactInfo> m_spPstnContactInfo;
	std::vector<boost::shared_ptr<Data::SoundSegment> >m_vSoundSegment;
	CString m_strRecord;
	CString m_strStopRecord;
	CString m_strHangOff;
	CString m_strHangOn;
	UINT m_uiTelephoneTimer;
	UINT m_uiTelephoneSecond;
	UINT m_uiRecordTimer;
	UINT m_uiRecordSecond;
	UINT m_uiRingCount;
	UINT m_uiTipTimer;
	UINT m_uiInNoCount;
	UINT m_uiRecordCound;
	BOOL m_bHasCallID;
	BOOL m_bFirwall;
	UINT m_uiRemainRecordSecond;
	std::string m_sTelephoneNumber;
	std::string m_sTipFilename;
	std::string m_sRingFilename;
	std::string m_sDialNumber;
	std::string m_sOutLine;
	std::string m_sTel;
	std::string m_sCity;
	BOOL m_bRing;
	BOOL m_bRingPlayed;

private:
	UINT m_uiPstnRingCount;
	UINT m_uiIPstngnoreRingCount;
	UINT m_uiPstnTelephoneTimer ;
	UINT m_uPstnTelSecondOff;
	UINT m_uiPstnTelephoneSecond;

	BOOL m_bPstnFirwall ;
	BOOL m_bPstnHasCallID ;
	BOOL m_bPstnRingPlayed;
	BOOL m_bPstnRing;


	std::string m_sPstnRingFilename;
	std::string m_sPstnTel;
	std::string m_sPstnCity;


	
private:
	volatile BOOL m_bEnding;
	BOOL m_bPlaying;
 	DWORD m_dwDataLength;
	HWAVEIN m_hWaveIn;
	HWAVEOUT m_hWaveOut;
	PBYTE m_pOutBuffer;
	
	PBYTE m_pBuffer[WAVE_BUFFER_COUNT];
	PWAVEHDR m_pWaveHdr[WAVE_BUFFER_COUNT];
	
	CFile m_fWav;	
	WAVEFORMATEX m_waveform;
	std::string m_sRecordFileName;
	std::string m_sPlayFileName;
	bool RecStart(void);
	bool RecStop(void);
	void SaveWavHead(void);
	bool PlayStart(void);
	bool PlayStop(void);

	BOOL m_bPlayingLeaveWord;
	bool InitData(void);
	std::string GetSoundPath(void);

	OggCodec* m_pOggCodec;
	int  m_Volume ;

public:
	void HandleOn(void);
	void HandleOff(void);
	
	void HangOff_(void* param);
	void HangOn_(void* param);
	void Key_(void* param);
	void SubKey_(void* param);
	void Connect_(void* param);
	void Ring_(void* param);
	void CallID_(void* param);
	std::vector<boost::shared_ptr<Data::Contact> > FindCallContact(std::string number);
	void ShowContact(boost::shared_ptr<Data::Contact> contact, std::string number,int uintNo = 100);
	void DialContact(Data::TelephoneNumber telephoneNumber, int contactId = 0);

	void Mute(void);
	void Redial(void);
	void HangOnInRecord(void);
	void HangOnToRecord(void);

	UINT GetUnconnectCount(void);
	UINT GetRecordCount(void); 

	BOOL GetIsRecordStatus(){return m_bRecording;}

	//add by qi 2009_11_04
	void pstnRing_(void* param);
	void pstnCallID_(void* param);
	void PstnContect_(void* param);
	void PstnHangOff_(void* param);

	void G3HangOff();//单独挂断3G

	void FromTelDial(boost::shared_ptr<Data::ContactInfo> pContactInfo,std::string tel);//把电话拨打界面的信息传过来
	void SetVolume(unsigned int volume);
	void VolumeSwitch();
	void HandleAudio(bool bt);
	void Dialback(std::string telnum);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELEPHONEDLG_H__2FAA5271_4CC6_4EE3_B0A8_045FE16A15B1__INCLUDED_)
