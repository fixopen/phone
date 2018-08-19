#if !defined(AFX_3GVIDEOPHONE_H__89BD4619_5EFF_4236_8A49_91A420B2F3CB__INCLUDED_)
#define AFX_3GVIDEOPHONE_H__89BD4619_5EFF_4236_8A49_91A420B2F3CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3GVideoPhone.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3GVideoPhone dialog
#include "../control/MJPGStatic.h"
#include "../control/CEStatic.h"
#include "../Data/LanguageResource.h"
#include "../Data/SoundSegment.h"
#include "../Data/ContactInfo.h"
#include "../Util/OggCodec.h"

class C3GVideoPhone : public CDialog
{
// Construction
public:
	C3GVideoPhone(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(C3GVideoPhone)
	enum { IDD = IDD_DLG_MOVIE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
public:
	void ChangeVolume(int w);
	void Mute();
	std::string GetSoundPath(void);
	bool RecStart(void);
	bool RecStop();
	bool InitData(void);
	CMJPGStatic m_MJPGList;

private:
	int m_SoundSize;
	BOOL m_IsSound;

	boost::shared_ptr<Data::ContactInfo> m_spContactInfo;
	boost::shared_ptr<Data::SoundSegment> m_spSoundSegment;
	std::vector<boost::shared_ptr<Data::SoundSegment> >m_vSoundSegment;
	OggCodec* m_pOggCodec;
	
	WAVEFORMATEX m_waveform;
	HWAVEIN m_hWaveIn;
	PWAVEHDR m_pWaveHdr[WAVE_BUFFER_COUNT];
	PBYTE m_pBuffer[WAVE_BUFFER_COUNT];
	
	volatile BOOL m_bEnding;
	BOOL m_bPlaying;
	BOOL m_bRecording;
	BOOL m_bHasCallID;
	BOOL m_bFirwall;
	BOOL m_bRing;
	BOOL m_bRingPlayed;
	BOOL m_bPlayingLeaveWord;
	UINT m_uiRemainRecordSecond;
	UINT m_uiRecordTimer;
	UINT m_uiRecordSecond;
	UINT m_uiTelephoneTimer;
	UINT m_uiTelephoneSecond;
	UINT m_uTelSecondOff;
	UINT m_uiRingCount;
	UINT m_uiTipTimer;
	UINT m_uiInNoCount;
	UINT m_uiRecordCound;
	DWORD m_dwDataLength;
	std::string path;
	std::string m_sRecordFileName;
	CString m_strRecord;
	CString m_strStopRecord;
	CString m_strHangOff;
	CString m_strHangOn;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3GVideoPhone)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(C3GVideoPhone)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMJPG(WPARAM w, LPARAM l);
	afx_msg void OnButtonTelephoneRecord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3GVIDEOPHONE_H__89BD4619_5EFF_4236_8A49_91A420B2F3CB__INCLUDED_)
